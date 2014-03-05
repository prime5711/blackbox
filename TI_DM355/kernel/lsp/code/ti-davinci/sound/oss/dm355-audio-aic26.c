/*
 * linux/sound/oss/dm355-audio-aic26.c
 *
 * Glue driver for AIC33 for Davinci processors
 *
 * Copyright (C) 2006 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * History:
 *  -------
 *  2005-10-18 Rishi Bhattacharya - Support for AIC33 codec and Davinci DM644x Processor
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/errno.h>
#include <linux/sound.h>
#include <linux/soundcard.h>
#include <sound/davincisound.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/mcbsp.h>
#include <asm/hardware/clock.h>
#include <linux/kernel.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <asm/errno.h>
#include <linux/device.h>
#include <asm/arch/gio.h>
#include <linux/spi/spi.h>
#include <linux/spi/dm355_spi_master.h>



#include "dm355-aic26.h"
#include "dm355-audio.h"
#include "dm355-audio-dma-intfc.h"

#if 0
#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#define PROC_START_FILE "driver/aic33-audio-start"
#define PROC_STOP_FILE  "driver/aic33-audio-stop"
#endif
#endif


#define KERN_EMERG "<0>" /* system is unusable   */
#define KERN_ALERT "<1>" /* action must be taken immediately */
#define KERN_CRIT "<2>" /* critical conditions   */
#define KERN_ERR "<3>" /* error conditions   */
#define KERN_WARNING "<4>" /* warning conditions   */
#define KERN_NOTICE "<5>" /* normal but significant condition */
#define KERN_INFO "<6>" /* informational   */
#define KERN_DEBUG "<7>" /* debug-level messages   */




//#define DEBUG

#ifdef DEBUG
#define DPRINTK(ARGS...)        do { \
                                        printk("<%s>: ",__FUNCTION__);printk(ARGS); \
                                } while (0)
#else
#define DPRINTK( x... )
#endif

typedef unsigned char   Uint8;

struct spi_aic26_t {
        struct spi_device *spi;
        spinlock_t lock;
};

#define CODEC_NAME               "AIC26"
#define PLATFORM_NAME            "DAVINCI"

struct spi_aic26_t aic26_spi_obj;

#define SPI_PHASE_IN TRUE
#define SPI_PHASE_OUT FALSE

#define PINMUX4		 __REG(0x01c40010)
#define SELECT_GIO103 PINMUX4 |= 0x1        //enable SPI function

#define GPIO28	28
#define GPIO103	103

#define	AIC26_RESET		GPIO28
#define	AIC26_EN		GPIO103


#define CONFIG 1
#define TRANSFER 2
//#define Audio_AIC26_ADC_DAC_TEST   3
#define Audio_AIC26_PLAYBACK_START 4
#define Audio_AIC26_PLAYBACK_STOP  5
#define Audio_AIC26_RECORDE_START  6
#define Audio_AIC26_RECORDE_STOP   7
#define Audio_SetSamplingRate      8
#define Audio_AGC_ENABLE           9
#define Audio_AIC26_RATE_8K        10
#define Audio_AIC26_RATE_12K       11
#define Audio_AIC26_RATE_24K       12
#define Audio_AIC26_RATE_48K       13
#define Audio_AIC26_VOLUME_NORMAL  14
#define Audio_AIC26_VOLUME_MED     15
#define Audio_AIC26_VOLUME_LOW     16
#define Audio_AIC26_VOLUME_LOWEST  17
#define Audio_AIC26_VOLUME_MUTE    18


#define AIC26_DRIVER	"AIC26"

/* Define to set the AIC26 as the master w.r.t McBSP */
#define AIC26_MASTER

/* codec clock frequency */
#ifdef CONFIG_ARCH_DAVINCI355
#define MCLK  27
#else
#define	MCLK  22
#endif

/*
 * AUDIO related MACROS
 */
#define DEFAULT_BITPERSAMPLE          16


/* Select the McBSP For Audio */

#ifdef CONFIG_ARCH_DAVINCI355
#define AUDIO_MCBSP         DAVINCI_MCBSP2
#else
#define	AUDIO_MCBSP         DAVINCI_MCBSP1
#endif

#define REC_MASK                      (SOUND_MASK_LINE | SOUND_MASK_MIC)
#define DEV_MASK                      (REC_MASK | SOUND_MASK_VOLUME)


static int major_num = SPI_MAJOR;

static audio_stream_t output_stream = {
	.id = "AIC26 out",
#ifdef CONFIG_ARCH_DAVINCI355
	.dma_dev = DM355_DMA_MCBSP1_TX,
#else
	.dma_dev = DAVINCI_DMA_MCBSP_TX,
#endif
	.input_or_output = FMODE_WRITE
};

static audio_stream_t input_stream = {
	.id = "AIC26 in",
#ifdef CONFIG_ARCH_DAVINCI355
	.dma_dev = DM355_DMA_MCBSP1_RX,
#else
	.dma_dev = DAVINCI_DMA_MCBSP_RX,
#endif
	.input_or_output = FMODE_READ
};

static int audio_dev_id, mixer_dev_id;

static struct aic26_local_info {
	u8 volume;
	u16 volume_reg;
	u8 line;
	u8 mic;
	int recsrc;
	u8 nochan;
	u16 igain;
	u16 ogain;
	u8 micbias;
	u8 bass;
	u8 treble;
	u16 input_volume_reg;
	int mod_cnt;
} aic26_local;


struct sample_rate_reg_info {
	u32 sample_rate;
	u32 Fsref;
	float divider;
	u8 data;
};

extern struct clk *davinci_mcbsp_get_clock(void);


static struct davinci_mcbsp_reg_cfg initial_config = {
	.spcr2 = FREE | XINTM(3),
	.spcr1 = RINTM(3),
    .rcr2  = 0x0001,
    .rcr1  = 0x0040,
    .xcr2  = 0x0001,
    .xcr1  = 0x0040,
    .srgr1 = 0x0025,
    .srgr2 = 0x101f,
    .pcr0  = 0x0,
};

static void davinci_aic26_initialize(void *dummy);
static void davinci_aic26_shutdown(void *dummy);
static int davinci_aic26_ioctl(struct inode *inode, struct file *file,
			       uint cmd, ulong arg);
static int davinci_aic26_probe(void);


static int mixer_open(struct inode *inode, struct file *file);
static int mixer_release(struct inode *inode, struct file *file);
static int mixer_ioctl(struct inode *inode, struct file *file, uint cmd,
		       ulong arg);
#if 0
#ifdef CONFIG_PROC_FS
static int codec_start(char *buf, char **start, off_t offset, int count,
		       int *eof, void *data);
static int codec_stop(char *buf, char **start, off_t offset, int count,
		      int *eof, void *data);
#endif
#endif

#ifdef MODULE
static void davinci_aic26_remove(void);
#endif


static int audio_aic26_write(unsigned short addr, unsigned short val);		       
static int aic26_probe(struct spi_device *this_dev);
static int aic26_remove(struct spi_device *this_dev);                        	       

/* File Op structure for mixer */
static struct file_operations davinci_mixer_fops = {
	.open = mixer_open,
	.release = mixer_release,
	.ioctl = mixer_ioctl,
	.owner = THIS_MODULE
};


/* To store characteristic info regarding the codec for the audio driver */
static audio_state_t aic26_state = {
	.owner = THIS_MODULE,
	.output_stream = &output_stream,
	.input_stream = &input_stream,
	.hw_init = davinci_aic26_initialize,
	.hw_shutdown = davinci_aic26_shutdown,
	.client_ioctl = davinci_aic26_ioctl,
	.hw_probe = davinci_aic26_probe,
};


static struct spi_driver aic26_driver = {

        .driver = {
                   .name = AIC26_DRIVER,
                   },
        .probe = aic26_probe,
        .remove = aic26_remove,

};


/* This will be defined in the audio.h */
static struct file_operations *davinci_audio_fops;




#if 0
static int aic26_update(int flag, int val)
{
	u16 volume;
	s16 left_gain, left_val, right_gain, right_val;

	switch (flag) {
	case SET_VOLUME:
		/* Ignore separate left/right channel for now,
	   	   even the codec does support it. */
		val &= 0xff;

		if (val < 0 || val > 100) {
			DPRINTK("Trying a bad volume value(%d)!\n", val);
			return -EPERM;
		}
		// Convert 0 -> 100 volume to 0x77 (LHV_MIN) -> 0x00 (LHV_MAX)
		volume =
		    ((val * OUTPUT_VOLUME_RANGE) / 100) + OUTPUT_VOLUME_MIN;

		aic33_local.volume_reg = OUTPUT_VOLUME_MAX - volume;

		if (aic33_local.nochan == STEREO) {
			audio_aic33_write(47, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(64, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(82, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(92, LOPM_ON | aic33_local.volume_reg);
		} else if (aic33_local.nochan == MONO) {
#ifdef CONFIG_MONOSTEREO_DIFFJACK
			/* DACL1 to MONO_LOP/M routing and volume control */
			audio_aic33_write(75, LOPM_ON | aic33_local.volume_reg);

			/* DACR1 to MONO_LOP/M routing and volume control */
			audio_aic33_write(78, LOPM_ON | aic33_local.volume_reg);
#else
			audio_aic33_write(47, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(64, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(82, LOPM_ON | aic33_local.volume_reg);
			audio_aic33_write(92, LOPM_ON | aic33_local.volume_reg);
#endif
		}

		break;

	case SET_LINE:
	case SET_MIC:
		/* Ignore separate left/right channel for now,
	   	   even the codec does support it. */
		val &= 0xff;

		if (val < 0 || val > 100) {
			DPRINTK("Trying a bad volume value(%d)!\n", val);
			return -EPERM;
		}

		volume = ((val * INPUT_VOLUME_RANGE) / 100) + INPUT_VOLUME_MIN;

		aic33_local.input_volume_reg = volume;

		audio_aic33_write(15, aic33_local.input_volume_reg);
		audio_aic33_write(16, aic33_local.input_volume_reg);

		break;

	case SET_RECSRC:
		/* Ignore separate left/right channel for now,
	   	   even the codec does support it. */
		val &= 0xff;

		if (hweight32(val) > 1)
			val &= ~aic33_local.recsrc;

		if (val == SOUND_MASK_MIC) {
			/* enable the mic input*/
			DPRINTK("Enabling mic\n");
			audio_aic33_write(17, 0x0);
			audio_aic33_write(18, 0x0);

			/* enable ADC's and disable the line input*/
			audio_aic33_write(19, 0x7C);
			audio_aic33_write(22, 0x7C);

		}
		else if (val == SOUND_MASK_LINE) {
			/* enable ADC's, enable line iput */
			DPRINTK(" Enabling line in\n");
			audio_aic33_write(19, 0x4);
			audio_aic33_write(22, 0x4);

			/* disable the mic input */
			audio_aic33_write(17, 0xff);
			audio_aic33_write(18, 0xff);
		}
		else {
			/* do nothing */
		}
		aic33_local.recsrc = val;
		break;

	case SET_IGAIN:
		left_val = val & 0xFF;
		right_val = val >> 8;

		if (left_val < 0 || left_val > 100) {
			DPRINTK("Trying a bad igain value(%d)!\n", left_val);
			return -EPERM;
		}
		if (right_val < 0 || right_val > 100) {
			DPRINTK("Trying a bad igain value(%d)!\n", right_val);
			return -EPERM;
		}

		left_gain = ((left_val * INPUT_GAIN_RANGE) / 100) + INPUT_GAIN_MIN;
		right_gain = ((right_val * INPUT_GAIN_RANGE) / 100) + INPUT_GAIN_MIN;

		DPRINTK("left gain reg val = 0x%x", left_gain << 1);
		DPRINTK("right gain reg val = 0x%x", left_gain << 1);

		/* Left AGC control */
		audio_aic33_write(26, 0x80);
		audio_aic33_write(27, left_gain << 1);
		audio_aic33_write(28, 0x0);

		/* Right AGC control */
		audio_aic33_write(29, 0x80);
		audio_aic33_write(30, right_gain << 1);
		audio_aic33_write(31, 0x0);

		break;

	case SET_OGAIN:
		left_val = val & 0xFF;
		right_val = val >> 8;

		if (left_val < 0 || left_val > 100) {
			DPRINTK("Trying a bad igain value(%d)!\n", left_val);
			return -EPERM;
		}
		if (right_val < 0 || right_val > 100) {
			DPRINTK("Trying a bad igain value(%d)!\n", right_val);
			return -EPERM;
		}

		left_gain = ((left_val * OUTPUT_GAIN_RANGE) / 100) + OUTPUT_GAIN_MIN;
		left_gain = OUTPUT_GAIN_MAX - left_gain;
		right_gain = ((right_val * OUTPUT_GAIN_RANGE) / 100) + OUTPUT_GAIN_MIN;
		right_gain = OUTPUT_GAIN_MAX - right_gain;

		/* Left/Right DAC digital volume gain */
		audio_aic33_write(43, left_gain);
		audio_aic33_write(44, right_gain);
		break;

	case SET_MICBIAS:
		/* Ignore separate left/right channel for now,
	   	   even the codec does support it. */
		val &= 0xff;

		if (val < 0 || val > 3) {
			DPRINTK
			    ("Request for non supported mic bias level(%d)!\n",
			     val);
			return -EPERM;
		}

		if (val == 0)
			audio_aic33_write(25, 0x00);

		else if (val == 1)
			audio_aic33_write(25, MICBIAS_OUTPUT_2_0V);

		else if (val == 2)
			audio_aic33_write(25, MICBIAS_OUTPUT_2_5V);

		else if (val == 3)
			audio_aic33_write(25, MICBIAS_OUTPUT_AVDD);

		break;

	case SET_BASS:
		break;

	case SET_TREBLE:
		break;
	}
	return 0;
}
#endif



















static int mixer_open(struct inode *inode, struct file *file)
{
	/* Any mixer specific initialization */
	return 0;
}

static int mixer_release(struct inode *inode, struct file *file)
{
	/* Any mixer specific Un-initialization */
	return 0;
}

static int
mixer_ioctl(struct inode *inode, struct file *file, uint cmd, ulong arg)
{
	int val;
	int ret = 0;
	int nr = _IOC_NR(cmd);

	/*
	 * We only accept mixer (type 'M') ioctls.
	 */
	if (_IOC_TYPE(cmd) != 'M')
		return -EINVAL;

	DPRINTK(" 0x%08x\n", cmd);

	if (cmd == SOUND_MIXER_INFO) {
		struct mixer_info mi;

		strncpy(mi.id, "AIC26", sizeof(mi.id));
		strncpy(mi.name, "TI AIC26", sizeof(mi.name));
		mi.modify_counter = aic26_local.mod_cnt;

		return copy_to_user((void *)arg, &mi, sizeof(mi));
	}

	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		ret = get_user(val, (int *)arg);
		if (ret)
			goto out;

		switch (nr) {
		case SOUND_MIXER_VOLUME:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_VOLUME, val);
			if (!ret)
				aic26_local.volume = val;
			break;

		case SOUND_MIXER_LINE:
			aic26_local.mod_cnt++;
			//ret = aic26_update(SET_LINE, val);
			if (!ret)
				aic26_local.line = val;
			break;

		case SOUND_MIXER_MIC:
			aic26_local.mod_cnt++;
			///ret = aic33_update(SET_MIC, val);
			if (!ret)
				aic26_local.mic = val;
			break;

		case SOUND_MIXER_RECSRC:
			if ((val & SOUND_MASK_LINE) ||
			    (val & SOUND_MASK_MIC)) {
				if (aic26_local.recsrc != val) {
					aic26_local.mod_cnt++;
					///aic26_update(SET_RECSRC, val);
				}
			}
			else {
				ret = -EINVAL;
			}
			break;

		case SOUND_MIXER_BASS:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_BASS, val);
			if (!ret)
				aic26_local.bass = val;
			break;

		case SOUND_MIXER_TREBLE:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_TREBLE, val);
			if (!ret)
				aic26_local.treble = val;
			break;

		case SOUND_MIXER_IGAIN:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_IGAIN, val);
			if (!ret)
				aic26_local.igain = val;
			break;

		case SOUND_MIXER_OGAIN:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_OGAIN, val);
			if (!ret)
				aic26_local.ogain = val;
			break;

		case SOUND_MIXER_MICBIAS:
			aic26_local.mod_cnt++;
			///ret = aic26_update(SET_MICBIAS, val);
			if (!ret)
				aic26_local.micbias = val;
			break;

		default:
			ret = -EINVAL;
		}

	}

	if (ret == 0 && _IOC_DIR(cmd) & _IOC_READ) {
		ret = 0;

		switch (nr) {
		case SOUND_MIXER_VOLUME:
			val = aic26_local.volume;
			break;
		case SOUND_MIXER_LINE:
			val = aic26_local.line;
			break;
		case SOUND_MIXER_MIC:
			val = aic26_local.mic;
			break;
		case SOUND_MIXER_RECSRC:
			val = aic26_local.recsrc;
			break;
		case SOUND_MIXER_RECMASK:
			val = REC_MASK;
			break;
		case SOUND_MIXER_IGAIN:
			val = aic26_local.igain;
			break;
		case SOUND_MIXER_OGAIN:
			val = aic26_local.ogain;
			break;
		case SOUND_MIXER_DEVMASK:
			val = DEV_MASK;
			break;
		case SOUND_MIXER_BASS:
			val = aic26_local.bass;
			break;
		case SOUND_MIXER_TREBLE:
			val = aic26_local.treble;
			break;
		case SOUND_MIXER_CAPS:
			val = 0;
			break;
		case SOUND_MIXER_STEREODEVS:
			val = SOUND_MASK_VOLUME;
			break;
		case SOUND_MIXER_MICBIAS:
			val = aic26_local.micbias;
			break;
		default:
			val = 0;
			ret = -EINVAL;
			break;
		}

		if (ret == 0)
			ret = put_user(val, (int *)arg);
	}
      out:
	return ret;

}


static void davinci_aic26_shutdown(void *dummy)
{
	/*
	   Turn off codec after it is done.
	   Can't do it immediately, since it may still have
	   buffered data.

	   Wait 20ms (arbitrary value) and then turn it off.
	 */
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(2);

	davinci_mcbsp_stop_tx(AUDIO_MCBSP);
	davinci_mcbsp_stop_rx(AUDIO_MCBSP);
	davinci_mcbsp_free(AUDIO_MCBSP);
}



static void davinci_aic26_initialize(void *dummy)
{
	DPRINTK("entry\n");

	if (davinci_mcbsp_request(AUDIO_MCBSP) < 0) {
		DPRINTK("MCBSP request failed\n");
		return;
	}

	/* if configured, then stop mcbsp */
	davinci_mcbsp_stop_tx(AUDIO_MCBSP);
	davinci_mcbsp_stop_rx(AUDIO_MCBSP);


	davinci_mcbsp_config(AUDIO_MCBSP, &initial_config);

	DPRINTK("exit\n");
}




static int
davinci_aic26_ioctl(struct inode *inode, struct file *file, uint cmd, ulong arg)
{
	long val;   
	int ret = 0;

	DPRINTK(" 0x%08x\n", cmd);

	/*
	 * These are platform dependent ioctls which are not handled by the
	 * generic davinci-audio module.
	 */
	switch (cmd) {
#if 0	
	case SNDCTL_DSP_STEREO:
		ret = get_user(val, (int *)arg);
		if (ret)
			return ret;
		/* the Davinci supports AIC33 as stereo, mono on stereo jack */
		ret = (val == 0) ? -EINVAL : 1;
		return put_user(ret, (int *)arg);

	case SNDCTL_DSP_CHANNELS:

		ret = get_user(val, (long *)arg);
		if (ret) {
			DPRINTK("get_user failed\n");
			break;
		}
		if (val == STEREO) {
			DPRINTK("Driver support for AIC26 as stereo\n");
			///aic26_local.nochan = STEREO;
			///davinci_set_mono_stereo(aic33_local.nochan);
		} else if (val == MONO) {
			DPRINTK("Driver support for AIC26 as mono\n");
			///aic26_local.nochan = MONO;
			///davinci_set_mono_stereo(aic26_local.nochan);
		} else {
			DPRINTK
			    ("Driver support for AIC26 as stereo/mono mode\n");
			return -EPERM;
		}

	case SOUND_PCM_READ_CHANNELS:
		/* the Davinci supports AIC26 as stereo, mono on stereo jack */
		if (aic26_local.nochan == MONO)
			return put_user(MONO, (long *)arg);
		else
			return put_user(STEREO, (long *)arg);

	case SNDCTL_DSP_SPEED:
		ret = get_user(val, (long *)arg);
		if (ret) {
			DPRINTK("get_user failed\n");
			break;
		}
		ret = davinci_set_samplerate(val);
		if (ret) {
			DPRINTK("davinci_set_samplerate failed\n");
			break;
		}
		/* fall through */

	case SOUND_PCM_READ_RATE:
		return put_user(audio_samplerate, (long *)arg);

	case SNDCTL_DSP_SETFMT:	/* set Format */
		ret = get_user(val, (long *)arg);
		if (ret) {
			DPRINTK("get_user failed\n");
			break;
		}
		if (val != AFMT_S16_LE) {
			DPRINTK
			    ("Driver supports only AFMT_S16_LE audio format\n");
			return -EPERM;
		}

	case SOUND_PCM_READ_BITS:
	case SNDCTL_DSP_GETFMTS:
		/* we can do 16-bit only */
		return put_user(AFMT_S16_LE, (long *)arg);
#endif
#if 0
	case Audio_AIC26_ADC_DAC_TEST:
        audio_aic26_write(AIC26_ADC_GAIN, 0x3C01);		// ADC not muted, PGA = 30dB, AGC Enable
        audio_aic26_write(AIC26_DAC_GAIN, 0x0000);		// DAC left/right channel control
        break;
#endif

#if 0
    case Audio_AIC26_PLAYBACK_START:
        audio_aic26_write(AIC26_PWR_CNT, 0x19C3);		// turn on sidetone power
        ///audio_aic26_write(AIC26_DAC_GAIN, 0x0000);
        audio_aic26_write(AIC26_DAC_GAIN, 0x0000);		// DAC left/right channel control
        ///audio_aic26_write(AIC26_SIDETONE, 0x7F00);		
        break;
           
    case Audio_AIC26_PLAYBACK_STOP:
        audio_aic26_write(AIC26_DAC_GAIN, 0x8080);		// DAC left/right mute
        break;
            
    case Audio_AIC26_RECORDE_START:
        audio_aic26_write(AIC26_PWR_CNT, 0x19C3);		// turn on sidetone power
        audio_aic26_write(AIC26_ADC_GAIN, 0x3800);      //AD gain is 28db
        ///audio_aic26_write(AIC26_SIDETONE, 0x7F00);
        ///audio_aic26_write(AIC26_ADC_GAIN, 0x0000);		// ADC not muted, AGC Disable
        break;
            
    case Audio_AIC26_RECORDE_STOP:
        audio_aic26_write(AIC26_ADC_GAIN, 0x8000);		// ADC muted
        break;
#endif


    case Audio_AIC26_PLAYBACK_START:
        ///audio_aic26_write(AIC26_DAC_GAIN, 0x0202);//0x7F7F);//0x0000);		// DAC left/right channel control
        break;
           
    case Audio_AIC26_PLAYBACK_STOP:
        audio_aic26_write(AIC26_DAC_GAIN, 0x8080);		// DAC left/right mute
        break;
            
    case Audio_AIC26_RECORDE_START:
        break;
            
    case Audio_AIC26_RECORDE_STOP:
        //audio_aic26_write(AIC26_ADC_GAIN, 0x8000);		// ADC muted
        break;
       
	case Audio_SetSamplingRate:
		audio_aic26_write(AIC26_PLL_PRG1, 0x9220);	//MCLK=24M PLL Enable, Q=2, P=2, J=8, Fsref=48kHz	Sam 2007 06 12
        audio_aic26_write(AIC26_PLL_PRG2, 0x1E00);	// D=1920		
        audio_aic26_write(AIC26_AUD_CNT1, 0x0100);	// mic I/P, 16bit, DSP mode, ADC/DAC FS=Fsref/1=48kHz
        break;

	case Audio_AGC_ENABLE:
        break;
        	
    case Audio_AIC26_RATE_8K:
        audio_aic26_write(AIC26_AUD_CNT1, 0x013F);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=Fsref/6=8kHz
        break;
        
    case Audio_AIC26_RATE_12K:
        audio_aic26_write(AIC26_AUD_CNT1, 0x0124);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=12kHz
        break;
        	
    case Audio_AIC26_RATE_24K:
        audio_aic26_write(AIC26_AUD_CNT1, 0x0112);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=24kHz
        break;
        
    case Audio_AIC26_RATE_48K:
        audio_aic26_write(AIC26_AUD_CNT1, 0x0100);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=48kHz
        break;

    case Audio_AIC26_VOLUME_NORMAL:
        audio_aic26_write(AIC26_DAC_GAIN, 0x0000);         
        break;
        
    case Audio_AIC26_VOLUME_MED:
        audio_aic26_write(AIC26_DAC_GAIN, 0x0E0E);         //-7.0dB
        break;

    case Audio_AIC26_VOLUME_LOW:
        audio_aic26_write(AIC26_DAC_GAIN, 0x1B1B);         //-14dB
        break;

    case Audio_AIC26_VOLUME_LOWEST:
        audio_aic26_write(AIC26_DAC_GAIN, 0x2F2F);         //-24dB
        break;

    case Audio_AIC26_VOLUME_MUTE:
        audio_aic26_write(AIC26_DAC_GAIN, 0x8080);         
        break;

	default:
		return ret;
		/* Maybe this is meant for the mixer (As per OSS Docs) */
		///return mixer_ioctl(inode, file, cmd, arg);
	}

	return ret;
}

static int davinci_aic26_probe(void)
{

	/* Get the fops from audio oss driver */
	if (!(davinci_audio_fops = audio_get_fops())) {
		DPRINTK
		    ("Unable to get the file operations for AIC26 OSS driver\n");
		audio_unregister_codec(&aic26_state);
		return -EPERM;
	}

#if 0
	aic26_local.volume = DEFAULT_OUTPUT_VOLUME;
	aic26_local.line = DEFAULT_INPUT_VOLUME;
	aic26_local.recsrc = SOUND_MASK_LINE;	/* either of SOUND_MASK_LINE/SOUND_MASK_MIC */
	aic26_local.igain = DEFAULT_INPUT_IGAIN | (DEFAULT_INPUT_IGAIN << 8);
	aic26_local.ogain = DEFAULT_INPUT_OGAIN | (DEFAULT_INPUT_OGAIN << 8);
	aic26_local.nochan = STEREO;
	aic26_local.micbias = 1;
#endif
	aic26_local.mod_cnt = 0;	


	/* register devices */
	audio_dev_id = register_sound_dsp(davinci_audio_fops, -1);
    mixer_dev_id = register_sound_mixer(&davinci_mixer_fops, -1);


#if 0
#ifdef CONFIG_PROC_FS
	create_proc_read_entry(PROC_START_FILE, 0 /* default mode */ ,
			       NULL /* parent dir */ ,
			       codec_start, NULL /* client data */ );

	create_proc_read_entry(PROC_STOP_FILE, 0 /* default mode */ ,
			       NULL /* parent dir */ ,
			       codec_stop, NULL /* client data */ );
#endif
#endif

	/* Announcement Time */
	DPRINTK(PLATFORM_NAME " " CODEC_NAME " audio support initialized\n");


	return 0;
}

#ifdef MODULE
static void __exit davinci_aic26_remove(void)
{
	/* Un-Register the codec with the audio driver */
	unregister_sound_dsp(audio_dev_id);

}
#endif				/* MODULE */


/**
 * spiee_probe - probe function for client driver
 * @this_dev: structure spi_device of SPI Master Controller
 *
 * This function will be called from kernel when match will be successful
 * for SPI controller device and client driver.
 */

DEVICE_ATTR(aic26, S_IRUGO, NULL, NULL);

static int aic26_probe(struct spi_device *this_dev)
{

        /*Assign the pointer of SPI device so that whenevr in future during
           open call of this driver it will be assigned to file->privare_data */
  
        DPRINTK("************ aic26_probe *************\n");
        if (this_dev)
	{
		DPRINTK("\n*** probe GOT IT ***\n");
                aic26_spi_obj.spi = this_dev;

		SELECT_GIO103;
		gpio_set(AIC26_EN);
		gpio_set_direction(AIC26_EN, GIO_DIR_OUTPUT);
	}
        else
	{
                return ERROR;
	}

        /* Initialize bits oer word */
        this_dev->bits_per_word = 8;

        /*create the attribute file for this client driver */
        device_create_file(&this_dev->dev, &dev_attr_aic26);
        DPRINTK("************ aic26_probe ************* return\n");
        return 0;

}

/**
 * spiee_remove - remove function for client driver
 * @this_dev: structure spi_device of SPI Master Controller
 *
 * This function will be called from kernel when client driver will be
 * released from SPI Maser controller.
 */
static int aic26_remove(struct spi_device *this_dev){
        /*remove the attribute file for this client driver */
//        DEBUG_DLCD("\nIn function spiee_remove\n");
        
        device_remove_file(&this_dev->dev, &dev_attr_aic26);
        return 0;
}


int aic26_config()
  // ABICO control LCD Panel
{
	int status = 0;
	struct spi_device *spi;
	dm355_spi_config_t *spi_cfg;

	/*struct spi_message message;
        struct spi_transfer xfer[2];*/

	if(aic26_spi_obj.spi == NULL)
		printk("*** aic26_spi_obj.spi = NULL ***\n");
	spi = aic26_spi_obj.spi;

	if (spi == NULL) {
			printk("\n***	SPI is NULL	***\n");
			return -1;
	}


	status = spi_setup(spi);

	if (status != 0)
			return status;

	if (spi->controller_data == NULL) {
			printk("spi->controller_data = NULL\n");
	}
	spi_cfg = (dm355_spi_config_t *) spi->controller_data;

	if (spi_cfg == NULL) {
			printk("spi_cfg = NULL\n");
	}

	spi_cfg->clkInternal = TRUE;//clock is internal
	spi_cfg->csHold = FALSE;//ok
	spi_cfg->intrLevel = FALSE;

	spi_cfg->pinOpModes = SPI_OPMODE_3PIN;//enable SPI_DO,SPI_CLK,SPI_DI function
	/*SPI_OPMODE_3PIN;*//* SPI_OPMODE_SPISCS_4PIN; *//*SPI_OPMODE_SPIENA_4PIN; *//*SPI_OPMODE_5PIN; */

	spi_cfg->clkHigh = FALSE;//IDLE_CLK_HIGH
	spi_cfg->lsbFirst = FALSE;//LSB first
	spi_cfg->parityEnable = TRUE;//?
	spi_cfg->oddParity = FALSE;//?
	spi_cfg->phaseIn = SPI_PHASE_OUT;//ok
	spi_cfg->op_mode = TRUE;   /*Polling mode -> FALSE, Interrupt mode -> TRUE */
	spi_cfg->loopBack = FALSE;

	spi->bits_per_word = 8;//8 bit char len
			/*data = arg; */
	status = spi_setup(spi);
	if (status != 0)
		return status;

	return 0;
}

/*
 */
int AIC26_CloseSerIf()
{
	return 0;
}


/*
 */
int audio_aic26_write(unsigned short addr, unsigned short val)
{
	struct spi_message message;
	struct spi_transfer xfer[4];
	struct spi_device *spi;
	unsigned char data[4];
	unsigned short SendData;
	int status = 0,i;

	status = aic26_config();

	if(status != 0)
		return status;
	spi = aic26_spi_obj.spi;
    
    addr &= 0x7FFF;

    ///printk("********************* Set AIC26 Register *********************\n");
	SendData = (Uint8) ((addr & 0xFF00) >> 8); 
	data[0] = SendData;
	SendData = (Uint8) (addr & 0x00FF);	
	data[1] = SendData;
    SendData = (Uint8) ((val & 0xFF00) >> 8);
	data[2] = SendData;
    SendData = (Uint8) (val & 0x00FF);	
	data[3] = SendData;

    memset(xfer, 0, sizeof(struct spi_transfer) * 4);
	gpio_clear(AIC26_EN);

    spi_message_init(&message);

	for(i=0;i<4;i+=2){
        xfer[i].tx_buf = &data[i];
        xfer[i].len = 1;
        spi_message_add_tail(&xfer[i], &message);
	}

    status = spi_sync(spi, &message);

	gpio_set(AIC26_EN);

    if (status < 0) {
             printk("Error from spi_sync = 0x%x\n", status);
             return status;
        }
    AIC26_CloseSerIf();

	return 0;
}


static int __init audio_aic26_init_start(void)
{
	int err = 0;
	
	/* register the codec with the audio driver */
	if ((err = audio_register_codec(&aic26_state))) {
		DPRINTK
		    ("Failed to register AIC26 driver with Audio OSS Driver\n");
	} else {
		DPRINTK("codec driver register success\n");
	}


	err = spi_register_driver(&aic26_driver);
	if(err == 0){
		DPRINTK("aic26_init success\n");
	} else
	    unregister_chrdev(major_num, AIC26_DRIVER);
	    
	    
	    
	gpio_set_direction(AIC26_RESET, GIO_DIR_OUTPUT);
	gpio_set(AIC26_RESET);
	mdelay(100);
	gpio_clear(AIC26_RESET);
	mdelay(100);
	gpio_set(AIC26_RESET);
	
	audio_aic26_write(AIC26_RST, 0xBB00);
	
    audio_aic26_write(AIC26_PWR_CNT, 0x2FC0);

        /* 8KHz */
		// 0x02800: 44.1Khz  Fsref 
		// 0x00800: 48Khz  Fsref 

//   	audio_aic26_write(AIC26_AUD_CNT3, 0x2800);         // master mode
 	audio_aic26_write(AIC26_AUD_CNT3, 0x0800);         // master mode
	audio_aic26_write(AIC26_AUD_CNT1, 0x0100/*0x013F*/);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=Fsref/6=8kHz
	audio_aic26_write(AIC26_PLL_PRG1, 0x9220);         // MCLK=24M PLL Enable, Q=2, P=2, J=8, Fsref=48kHz
	audio_aic26_write(AIC26_PLL_PRG2, 0x1E00);         // D=1920
    
	// Keep Driver Pop Reduction settings at the default and power up headphone driver
	audio_aic26_write(AIC26_AUD_CNT5, 0xFE02);
    audio_aic26_write(AIC26_PWR_CNT, 0x3FC0);
  
    // Power up Sidetone; but keep it muted
	audio_aic26_write(AIC26_SIDETONE, 0xC580);	// Analog/Digital sidetone muted
    audio_aic26_write(AIC26_PWR_CNT, 0x1FC0);	// Analog Sidetone power up
  
    // Enable DAC pop reduction features, and set it to slow and long options
	audio_aic26_write(AIC26_AUD_CNT4, 0x0030);

    // Power up DAC and then un-mute both channles
    audio_aic26_write(AIC26_DAC_GAIN, 0xFFFF);  // DAC both channles muted
	audio_aic26_write(AIC26_PWR_CNT, 0x1BC0);   // Power up DAC markde by sam
	
	//audio_aic26_write(AIC26_DAC_GAIN, 0x0000);       // DAC both channles un-muted Sam Added

    mdelay(10);
    audio_aic26_write(AIC26_PWR_CNT, 0x19C0);  // Power up ADC
  
    //AGC Setting
    // modify yikim  
    // audio_aic26_write(AIC26_ADC_GAIN, 0x8059);//AGC on, Target level 24dB, 8ms/500ms //0x8019); AGC on, Target level -10.0dB, 8ms/500ms
    // audio_aic26_write(AIC26_ADC_GAIN, 0x8019);//AGC on, Target level 24dB, 8ms/500ms //0x8019); AGC on, Target level -5.5dB, 8ms/500ms

// kongsema test : 8KHz
//      audio_aic26_write(AIC26_ADC_GAIN, 0x8099);//AGC on, Target level -14dB
// kongsema test : AGC Off
//      audio_aic26_write(AIC26_ADC_GAIN, (( 6*2)<<8) );//AGC off,  6dB
//      audio_aic26_write(AIC26_ADC_GAIN, ((12*2)<<8) );//AGC off, 12dB
//      audio_aic26_write(AIC26_ADC_GAIN, ((24*2)<<8) );//AGC off, 24dB
    audio_aic26_write(AIC26_ADC_GAIN, ((32*2)<<8) );//AGC off, 32dB
//      audio_aic26_write(AIC26_ADC_GAIN, ((48*2)<<8) );//AGC off, 48dB
//      audio_aic26_write(AIC26_ADC_GAIN, ((52*2)<<8) );//AGC off, 52dB

        /* 8KHz */
		// 0x02800: 44.1Khz  Fsref 
		// 0x00800: 48Khz  Fsref 
    audio_aic26_write(AIC26_AUD_CNT3, 0x0830);	// master mode, AGC Noise threshold -90dB
    audio_aic26_write(AIC26_AUD_CNT5, 0xB302);  // MAX input Gain Applicable for AGC 40dB, 0ms/0ms
    audio_aic26_write(AIC26_AUD_CNT4, 0x0230);  // AGC Hysteresis 2-dB

#if 1
	mdelay(100);
	audio_aic26_write(AIC26_AUD_CNT1, 0x013F);         // mic I/P, 16bit, DSP mode, ADC/DAC FS=Fsref/6=8kHz
#endif

	return err;



}

static void __exit audio_aic26_exit(void)
{
	davinci_aic26_shutdown(NULL);
	(void)audio_unregister_codec(&aic26_state);
	return;
}


module_init(audio_aic26_init_start);
module_exit(audio_aic26_exit);


MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("Glue audio driver for the TI AIC26 codec.");
MODULE_LICENSE("GPL");
