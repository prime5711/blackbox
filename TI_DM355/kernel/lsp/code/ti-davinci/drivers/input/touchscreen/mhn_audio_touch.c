/*
 *  drivers/input/touchscreen/mhn_audio_touch.c.
 *
 *  Author:	bridge.wu@marvell.com
 *  Created:	Nov 17, 2006
 *  Copyright:	Marvell Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/interrupt.h>

#include <sound/driver.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>

#include <asm/semaphore.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <linux/suspend.h>
#include <linux/spinlock.h>

#include <asm/arch/codec/acodec.h>
#include <asm/arch/mhn_audio_plat.h>

static p_zy_acocec_context_t p_zy_codec_ctxt = NULL;

#include <linux/input.h>
#include <asm/arch/codec/wm9713.h>
#define PEN_DOWN 1
#define PEN_UP 0
typedef struct {
	unsigned int is_registered;	/* Is the driver AC97 registered */
	int adc_errs;			/* sample read back errors */
	spinlock_t lock;
	struct input_dev *idev;
	struct completion thread_init;
	struct completion thread_exit;
	struct task_struct *rtask;
	struct semaphore  sem;
	int (*cont_mode)(void);
	int  use_count;
	int restart;
} codec_zy_ts_t;

static codec_zy_ts_t codec_zy_ts;

static struct input_dev codec_zy_ts_input;

#ifdef CONFIG_PM
static int touch_suspend = 0 ;
#endif


/*
 * add a touch event
 */
static int codec_zy_ts_evt_add(codec_zy_ts_t* ts, u16 pressure, u16 x, u16 y)
{
        /* add event and remove adc src bits */
        static u16 pre_press = 0;
        input_report_abs(ts->idev, ABS_X, x & 0xfff);
        input_report_abs(ts->idev, ABS_Y, y & 0xfff);
        if (pressure == pre_press)
                pressure--;
        pre_press = pressure;
        input_report_abs(ts->idev, ABS_PRESSURE, pressure & 0xfff);
        return 0;
}

/*
 * add a pen up event
 */
static void codec_zy_ts_evt_release(codec_zy_ts_t* ts)
{
	input_report_abs(ts->idev, ABS_PRESSURE, 0);
}

/*
 * Kill the touchscreen thread and stop
 * the touch digitiser.
 */
static void codec_zy_ts_input_close(struct input_dev *idev)
{
	codec_zy_ts_t *ts = (codec_zy_ts_t *) &codec_zy_ts;
	dev_dbg(idev->dev, "close ts input!\n");
	spin_lock(&ts->lock);
	if (--ts->use_count == 0) {
		spin_unlock(&ts->lock);
		/* kill thread */
		init_completion(&ts->thread_exit);
		if (ts->rtask) {
			send_sig(SIGKILL, ts->rtask, 1);
			wait_for_completion(&ts->thread_exit);
		}

		/* stop digitiser */
		zy_acodec_disable_touch(p_zy_codec_ctxt);
	} else
		spin_unlock(&ts->lock);
}

/*
 * Sample the touchscreen
 */
int ac97_poll_touch(codec_zy_ts_t *ts)
{
	unsigned short x = 0, y = 0;
	int if_down = 0;

	/* get x value */
	pr_debug("x:0x%x\n",x);
	if (ZY_ACODEC_SUCCESS != zy_acodec_get_adc_sample(p_zy_codec_ctxt, &x,
		ZY_TOUCH_SAMPLE_X, &if_down))
			return -EIO;

	/* the pen is up */
	if (1 != if_down)
		return PEN_UP;

	/* get y value */
	pr_debug("y:0x%x\n",y);
	if( ZY_ACODEC_SUCCESS != zy_acodec_get_adc_sample(p_zy_codec_ctxt, &y,
		ZY_TOUCH_SAMPLE_Y, &if_down))
			return -EIO;

	/* the pen is up */
	if (1 != if_down)
		return PEN_UP;
	/*
 	 * the pen is down, can not get the pressure value,
	 * so if pen is down, give the max pressure value
	 */
	codec_zy_ts_evt_add(ts,0xfff, x, y);
	return PEN_DOWN;
}

/*
 * The touchscreen sample reader thread
 */
static int codec_zy_ts_thread(void *_ts)
{
	codec_zy_ts_t *ts = (codec_zy_ts_t *)_ts;
	struct task_struct *tsk = current;
	int valid = 0;
	int pen_status = PEN_UP;
	ts->rtask = tsk;

	/* set up thread context */
	daemonize("ktsd");

	/* we will die when we receive SIGKILL */
	spin_lock_irq(&tsk->proc_lock);
	siginitsetinv(&tsk->blocked, sigmask(SIGKILL));
	recalc_sigpending();
	spin_unlock_irq(&tsk->proc_lock);

	/* init is complete */
	complete(&ts->thread_init);

	/* touch reader loop */
	for (;;) {
		ts->restart = 0;
		if(signal_pending(tsk))
			break;

		/* get x_y value */
		pen_status = ac97_poll_touch(ts);
		if (PEN_DOWN == pen_status )
			valid = 1;
		else if (PEN_UP == pen_status && valid) {
			valid = 0;
			codec_zy_ts_evt_release(ts);
		}

		set_task_state(tsk, TASK_INTERRUPTIBLE);
		if (HZ >= 100)
			schedule_timeout(HZ/100);
		else
			schedule_timeout(1);
#ifdef CONFIG_PM
		if(tsk->flags & PF_FREEZE){
			refrigerator();
		}
#endif
	}

	ts->rtask = NULL;
	complete_and_exit(&ts->thread_exit, 0);

	return 0;
}

/*
 * Start the touchscreen thread and
 * the touch digitiser.
 */
static int codec_zy_ts_input_open(struct input_dev *idev)
{
	codec_zy_ts_t *ts = (codec_zy_ts_t *) &codec_zy_ts;
	int ret;
#ifdef CONFIG_PM
	if(touch_suspend){
		printk(KERN_INFO "touch is suspended!\n");
		return -1;
	}
#endif

	spin_lock(&ts->lock);
	if ( ts->use_count++ == 0 ) {
		spin_unlock( &ts->lock );
		/* start touchscreen thread */
		ts->idev = idev;
		init_completion(&ts->thread_init);
		ret = kernel_thread(codec_zy_ts_thread, ts, 0);
		if (ret >= 0)
			wait_for_completion(&ts->thread_init);

		/* start digitiser */
		zy_acodec_enable_touch(p_zy_codec_ctxt);

	} else
		spin_unlock( &ts->lock );
	return 0;
}

/*
 * initilze the pxa touch screen
 */
 static int alsa_ts_init( void )
{
	codec_zy_ts_t* ts = &codec_zy_ts;

	memset(ts, 0, sizeof(codec_zy_ts_t));

	/* tell input system what we events we accept and register */
	codec_zy_ts_input.name = "codec zy touchscreen";
	codec_zy_ts_input.open = codec_zy_ts_input_open;
	codec_zy_ts_input.close = codec_zy_ts_input_close;
	__set_bit(EV_ABS, codec_zy_ts_input.evbit);
	__set_bit(ABS_X, codec_zy_ts_input.absbit);
	__set_bit(ABS_Y, codec_zy_ts_input.absbit);
	__set_bit(ABS_PRESSURE, codec_zy_ts_input.absbit);
	input_register_device(&codec_zy_ts_input);
	spin_lock_init(&ts->lock);
	init_MUTEX(&ts->sem);

	return 0;
}

#ifdef CONFIG_PM
static int touch_codec_zy_suspend(struct device *_dev, pm_message_t state, u32 level)
{
	int ret=0;
	if(level == SUSPEND_DISABLE) {
		zy_acodec_suspend(p_zy_codec_ctxt);
		pxa_set_cken(CKEN_AC97, 0);
		pxa_set_cken(CKEN_SSP3, 0);
		printk(KERN_INFO "alsa touch suspend!\n");
	}
	touch_suspend = 1;
	return ret;
}

static int touch_codec_zy_resume(struct device *_dev, u32 level)
{
	int ret = 0;
	if (level == RESUME_ENABLE){
		pxa_set_cken(CKEN_AC97, 1);
		zy_acodec_resume(p_zy_codec_ctxt);
		printk(KERN_INFO "alsa touch resume!\n");
	}
	touch_suspend = 0 ;
	return ret;
}
#else
#define touch_codec_zy_suspend	NULL
#define touch_codec_zy_resume	NULL
#endif

static int __devinit touch_codec_zy_probe(struct device *dev)
{
	snd_card_t *card = NULL;
	int ret;
	zy_acodec_error_t status;

	ret = alsa_prepare_for_zy(&p_zy_codec_ctxt);
	if (ret)
		goto err;

	status = zy_acodec_init(p_zy_codec_ctxt, 1);
	if (ZY_ACODEC_SUCCESS != status) {
		ret = -EIO;
		goto err;
	}
	/* power down the units of the acodec, sleep the acodec, zy_acodec_init()
	 * will open all the units' power of the codec while ALSA need all the codec
	 * units power down and the codec should sleep if it can.
	 * So on the zylonite platform we call below function to power down and sleep
	 * wm9713 codec.
	 */
	p_zy_codec_ctxt->g_pfn_codec_specific_dinit(p_zy_codec_ctxt);

	alsa_ts_init();

	return 0;
err:
	if (p_zy_codec_ctxt) {
		zy_acodec_deinit(p_zy_codec_ctxt);
		pxa_set_cken(CKEN_AC97, 0);
		kfree(p_zy_codec_ctxt);
		p_zy_codec_ctxt = NULL;
	}

	if (card)
		snd_card_free(card);

	return ret;
}

static int __devexit touch_codec_zy_remove(struct device *dev)
{
	snd_card_t *card = dev_get_drvdata(dev);

	input_unregister_device(&codec_zy_ts_input);

	if (p_zy_codec_ctxt) {
		alsa_zy_codec_put(p_zy_codec_ctxt);
		kfree(p_zy_codec_ctxt);
		p_zy_codec_ctxt = NULL;
	}

	if (card) {
		snd_card_free(card);
		dev_set_drvdata(dev, NULL);
	}

	return 0;
}

static struct device_driver touch_codec_zy_driver = {
	.name 	= 	"pxa2xx-touch",
	.bus 	=	&platform_bus_type,
	.probe 	= 	touch_codec_zy_probe,
	.remove = 	__devexit_p(touch_codec_zy_remove),
	.suspend= 	touch_codec_zy_suspend,
	.resume = 	touch_codec_zy_resume,
};

static int __init touch_codec_zy_init(void)
{
	return driver_register(&touch_codec_zy_driver);
}

static void __exit touch_code_zy_exit(void)
{
	driver_unregister(&touch_codec_zy_driver);
}
module_init(touch_codec_zy_init);
module_exit(touch_code_zy_exit);

EXPORT_SYMBOL(p_zy_codec_ctxt);

MODULE_AUTHOR("bridge.wu@marvell.com");
MODULE_DESCRIPTION("zylonite audio touch codec driver on ALSA");
MODULE_LICENSE("GPL");

