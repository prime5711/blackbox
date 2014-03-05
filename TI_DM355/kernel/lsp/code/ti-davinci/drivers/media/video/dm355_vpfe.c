/*
 * Copyright (C) 2006 Texas Instruments Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
/* dm355_vpfe.c */
/* !FIXME needs to add code to program the i2c switch on the EVM to select tvp5146 or mt9t001 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kdev_t.h>
#include <linux/string.h>
#include <linux/videodev.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>
#include <linux/time.h>
#include <linux/device.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/io.h>
#include <asm/dma-mapping.h>
#include <linux/config.h>

#include <media/dm355_vpfe.h>

#include <asm-arm/arch-davinci/dm355_ipipe_hw.h>
#include <asm-arm/arch-davinci/dm355_ipipe.h>
#include "ipipe_para.h"

#define PINMUX4          __REG(0x01c40010)
#define PINMUX0   	 		 __REG(0x01c40000)
#define PINMUX1          __REG(0x01c40004)
#define PINMUX3          __REG(0x01c4000c)

#define	DM355_VPFE_DEBUG		0
#define	DM355_VPFE_DEBUG_MT9M131	1
#define	MT9M131_APRO			1


#define	TW2835_MODE_D1_NTSC			(10)
#define	TW2835_MODE_D1_PAL			(11)


MODULE_LICENSE("GPL");

/* Global variable for insmode command which will decide that either MT9T001
 * (device =0)  or TVP5146 (device = 1) is the device
 */
//static int device_type = TVP7002;
//static int device_type = MT9T001;
static int device_type;	// = MT9M131;

struct device *vpfe_dev;

Res_Set	*pResHeader;

static int ccdc_dcsub_update = -1;
module_param(device_type, int, 0);

#ifndef MODULE
/*
 * Pass boot-time options by adding the following string to the boot params:
 * 	v4l2_video_capture:[option[:option]]
 * Valid options:
 * 	device=[MT9T031|MT9T001|TVP5150]
 */

//char *dm355_devices[] = {"MT9T001", "TVP5146", "MT9T031","MT9P031","TVP5150"};
//char *dm355_devices[] = {"MT9M112","TW_2835"};

int __init dm355_v4l2_device_setup(char *options)
{
	char *this_opt;

	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ":")) != NULL) {

		if (!*this_opt)
			continue;

		if (!strncmp(this_opt, "device=", 6)) 
		{
			if (!strncmp(this_opt + 7, "MT9T031", 7))
			{
				printk("MT9T031 selected\n");
				device_type = MT9T031;
			}
			else if (!strncmp(this_opt + 7, "MT9T001", 7))
			{
				printk("MT9T001 selected\n");
				device_type = MT9T001;
			}
			else if (!strncmp(this_opt + 7, "TVP5146", 7))
			{
				printk("TVP5146 selected\n");
				device_type = TVP5146;
			}
			else if (!strncmp(this_opt + 7, "MT9P031", 7))
			{
				printk("MT9P031 selected\n");
				device_type = MT9P031;
			}
			else if (!strncmp(this_opt + 7, "TVP5150", 7))
			{
				printk("TVP5150 selected\n");
				device_type = TVP5150;
			}
			else if (!strncmp(this_opt + 7, "MT9M131", 7))
			{
				printk("MT9M131 selected\n");
				device_type = MT9M131;
			}
			else if (!strncmp(this_opt + 7, "TVP7002", 7))
			{
				device_type = TVP7002;
			}
			else if (!strncmp(this_opt + 7, "TW2835", 6))
			{
				printk("TW2835 selected\n");
				device_type = TW2835;
			}

		}
	}

	//printk("Device type = [%d]\n", device_type);
	return 0;
}
__setup("v4l2_video_capture=", dm355_v4l2_device_setup);
#endif

static struct v4l2_rect ntsc_bounds = VPFE_WIN_NTSC;
static struct v4l2_rect pal_bounds = VPFE_WIN_PAL;
static struct v4l2_fract ntsc_aspect = VPFE_PIXELASPECT_NTSC;
static struct v4l2_fract pal_aspect = VPFE_PIXELASPECT_PAL;
static struct v4l2_rect ntscsp_bounds = VPFE_WIN_NTSC_SP;
static struct v4l2_rect palsp_bounds = VPFE_WIN_PAL_SP;
static struct v4l2_fract sp_aspect = VPFE_PIXELASPECT_NTSC_SP;

static struct v4l2_rect VGA_bounds = VPFE_WIN_VGA;
static struct v4l2_rect VGA_bounds_512 = VPFE_WIN_VGA_512;
static struct v4l2_rect SVGA_bounds = VPFE_WIN_SVGA;
static struct v4l2_rect XGA_bounds = VPFE_WIN_XGA;
static struct v4l2_rect SXGA_bounds = VPFE_WIN_SXVGA;
static struct v4l2_rect SXGA_960_bounds = VPFE_WIN_SXVGA_960;
static struct v4l2_rect P480_bounds = VPFE_WIN_480p;
static struct v4l2_rect P576_bounds = VPFE_WIN_576p;
static struct v4l2_rect P720_bounds = VPFE_WIN_720p;
static struct v4l2_rect P1080_bounds = VPFE_WIN_1080p;
static struct v4l2_fract default_aspect = VPFE_PIXELASPECT_DEFAULT;

static struct v4l2_rect P1024_720_bounds = VPFE_WIN_P1024_720;
static struct v4l2_rect P1440_1024_bounds = VPFE_WIN_1440_1024;

static struct v4l2_rect QVGA_bounds = VPFE_WIN_QVGA;



static vpfe_obj vpfe_device_ycbcr_mt9m131 = {	/* the default format is NTSC */
	.usrs = 0,
	.io_usrs = 0,
	//.std = V4L2_STD_MT9T001_VGA_30FPS,
	#if 1
	.std = MT9M131_MODE_VGA_30FPS, 
	#else
	.std = MT9M131_MODE_SXVGA_960, 
	#endif
	//.vwin = VPFE_WIN_VGA,
	//.bounds = VPFE_WIN_VGA ,
	#if 1
	.vwin = VPFE_WIN_SXVGA,
	.bounds = VPFE_WIN_SXVGA ,
	#else
	.vwin = VPFE_WIN_SXVGA_960,
	.bounds = VPFE_WIN_SXVGA_960,
	#endif
	
	
	.pixelaspect = VPFE_PIXELASPECT_NTSC, //VPFE_PIXELASPECT_NTSC ==> VPFE_PIXELASPECT_DEFAULT
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	
	.field = V4L2_FIELD_NONE, // V4L2_FIELD_INTERLACED ==> V4L2_FIELD_NONE
	//.field = V4L2_FIELD_INTERLACED,
	
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = MT9M131,
	.ccdc_params_ycbcr = {
			      .pix_fmt = CCDC_PIXFMT_YCBCR_8BIT,
			      
			      .frm_fmt = CCDC_FRMFMT_PROGRESSIVE, 		//changed
			      //.frm_fmt = CCDC_FRMFMT_INTERLACED,

				#if 1			      
			      .win = VPFE_WIN_VGA, 									// changed 테스트 VGA
				#else
			      .win = VPFE_WIN_SXVGA_960, 									// changed 테스트 VGA
				#endif
				  
			      .fid_pol = CCDC_PINPOL_POSITIVE,
			      .vd_pol = CCDC_PINPOL_POSITIVE,
			      .hd_pol = CCDC_PINPOL_POSITIVE,
			      
			      
			      //.fid_pol = CCDC_PINPOL_NEGATIVE,
			      //.vd_pol = CCDC_PINPOL_NEGATIVE,
			      //.hd_pol = CCDC_PINPOL_NEGATIVE,
			      
			      //.bt656_enable = FALSE, // TRUE ==> FALSE
			      .bt656_enable = FALSE,
			      .pix_order = CCDC_PIXORDER_CBYCRY,
			      
			      //.buf_type = CCDC_BUFTYPE_FLD_INTERLEAVED}, //==> 영상이 분홍색으로나옴
			      .buf_type = CCDC_BUFTYPE_FLD_SEPARATED}, //changed

	//.ccdc_params_raw = {0},
	.ccdc_params_raw = {
		    .pix_fmt = CCDC_PIXFMT_RAW,	//CCDC_PIXFMT_YCBCR_8BIT,
		    .frm_fmt = CCDC_FRMFMT_PROGRESSIVE,
		    .win = VPFE_WIN_SXVGA,
		    .fid_pol = CCDC_PINPOL_POSITIVE,
		    .vd_pol = CCDC_PINPOL_POSITIVE,
		    .hd_pol = CCDC_PINPOL_POSITIVE,
		    .image_invert_enable = FALSE,
		    .data_sz = _8BITS,
                        .med_filt_thres = 0,
                        .mfilt1 = NO_MEDIAN_FILTER1,
                        .mfilt2 = NO_MEDIAN_FILTER2,
                        .ccdc_offset = 0,
                        .lpf_enable = FALSE,
                        .datasft = 0,
                        .alaw = {
                                 .b_alaw_enable = TRUE,
                                 .gama_wd = 2},
                        .blk_clamp = {
                                      .b_clamp_enable = FALSE,
                                      .sample_pixel = 1,
                                      .start_pixel = 0,
                                      .dc_sub = 0},
                        .blk_comp = {
                                     .b_comp = 0,
                                     .gb_comp = 0,
                                     .gr_comp = 0,
                                     .r_comp = 0},
                        .vertical_dft = {
                                         .ver_dft_en = FALSE},
                        .lens_sh_corr = {
                                         .lsc_enable = FALSE},
                        .data_formatter_r = {
                                             .fmt_enable = FALSE},
                        .color_space_con = {
                                            .csc_enable = FALSE}
                        },
	/*.mt9m131_params = {
			   .mode = MT9M131_MODE_AUTO,
			   .amuxmode = MT9M131_AMUX_COMPOSITE,
			   .enablebt656sync = TRUE,
			   .data_width = MT9M131_WIDTH_8BIT},
	*/		   
	.irqlock = SPIN_LOCK_UNLOCKED
};


static vpfe_obj vpfe_device_ycbcr_tw2835 = {	/* the default format is 720P  */
	.usrs = 0,
	.io_usrs = 0,
	.std = VPFE_STD_AUTO,
	.vwin = VPFE_WIN_NTSC, //VPFE_WIN_720p,
	.bounds = VPFE_WIN_NTSC, //VPFE_WIN_720p,
	.pixelaspect = VPFE_PIXELASPECT_PAL,
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	.field = V4L2_FIELD_INTERLACED,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = TW2835,
	.ccdc_params_ycbcr = {
			      .pix_fmt = CCDC_PIXFMT_YCBCR_8BIT,
			      .frm_fmt = CCDC_FRMFMT_INTERLACED,
			      .win = VPFE_WIN_NTSC,
			      .fid_pol = CCDC_PINPOL_POSITIVE,
			      .vd_pol = CCDC_PINPOL_POSITIVE,
			      .hd_pol = CCDC_PINPOL_POSITIVE,
			      .bt656_enable = TRUE,
			      .pix_order = CCDC_PIXORDER_CBYCRY,
			      .buf_type = CCDC_BUFTYPE_FLD_INTERLEAVED
			      },
	.ccdc_params_raw = {0},
	/////////////////////////////////////////////////////////////////////////////////
	.irqlock = SPIN_LOCK_UNLOCKED
};


static vpfe_obj vpfe_device_ycbcr_tvp7002 = {	/* the default format is 720P 30f */
	.usrs = 0,
	.io_usrs = 0,
	.std = V4L2_STD_720P_30,
	.vwin = VPFE_WIN_720p,
	.bounds = VPFE_WIN_720p,
	.pixelaspect = VPFE_PIXELASPECT_DEFAULT,
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	.field = V4L2_FIELD_NONE,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = TVP7002,
	.ccdc_params_ycbcr = {
			      .pix_fmt = CCDC_PIXFMT_YCBCR_16BIT,
			      .frm_fmt = CCDC_FRMFMT_PROGRESSIVE,
			      .win = VPFE_WIN_720p,
			      .fid_pol = CCDC_PINPOL_POSITIVE,
			      .vd_pol = CCDC_PINPOL_POSITIVE,
			      .hd_pol = CCDC_PINPOL_POSITIVE,
			      .bt656_enable = FALSE,
			      .pix_order = CCDC_PIXORDER_CBYCRY,
			      .buf_type = CCDC_BUFTYPE_FLD_SEPARATED
			      },
	.ccdc_params_raw = {0},
	/////////////////////////////////////////////////////////////////////////////////
	.tvp7002_params = {
			   .mode = TVP7002_MODE_720p_30FPS,      
			   .enablebt656sync = TRUE,
			   .data_width = TVP7002_WIDTH_16BIT
		
	  },
		.irqlock = SPIN_LOCK_UNLOCKED
};


static vpfe_obj vpfe_device_ycbcr = {	/* the default format is NTSC */
	.usrs = 0,
	.io_usrs = 0,
	.std = VPFE_STD_AUTO,
	.vwin = VPFE_WIN_NTSC,
	.bounds = VPFE_WIN_NTSC ,
	//.vwin = VPFE_WIN_PAL,
	//.bounds = VPFE_WIN_PAL,
	.pixelaspect = VPFE_PIXELASPECT_NTSC,
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	.field = V4L2_FIELD_INTERLACED,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = TVP5150,
	.ccdc_params_ycbcr = {
			      .pix_fmt = CCDC_PIXFMT_YCBCR_8BIT,
			      .frm_fmt = CCDC_FRMFMT_INTERLACED,
			      .win = VPFE_WIN_NTSC,
			      //.win = VPFE_WIN_PAL,
			      .fid_pol = CCDC_PINPOL_POSITIVE,
			      .vd_pol = CCDC_PINPOL_POSITIVE,
			      .hd_pol = CCDC_PINPOL_POSITIVE,
			      .bt656_enable = TRUE,
			      .pix_order = CCDC_PIXORDER_CBYCRY,
			      .buf_type = CCDC_BUFTYPE_FLD_INTERLEAVED},

	.ccdc_params_raw = {0},
	.tvp5150_params = {
			   .mode = TVP5150_MODE_AUTO,
			   .amuxmode = TVP5150_AMUX_COMPOSITE,
			   .enablebt656sync = TRUE,
			   .data_width = TVP5150_WIDTH_8BIT},
			   
	.irqlock = SPIN_LOCK_UNLOCKED
};

static vpfe_obj vpfe_device_raw = {	/*Default is vga mode */
	.usrs = 0,
	.io_usrs = 0,
	.std = V4L2_STD_MT9T001_VGA_30FPS,
	.vwin = VPFE_WIN_VGA,
	.bounds = VPFE_WIN_VGA,
	.pixelaspect = VPFE_PIXELASPECT_DEFAULT,
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	.field = V4L2_FIELD_NONE,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = MT9M131,
	.ccdc_params_raw = {
			    .pix_fmt = CCDC_PIXFMT_YCBCR_8BIT,
			    .frm_fmt = CCDC_FRMFMT_PROGRESSIVE,
			    .win = VPFE_WIN_PAL,
			    .fid_pol = CCDC_PINPOL_POSITIVE,
			    .vd_pol = CCDC_PINPOL_POSITIVE,
			    .hd_pol = CCDC_PINPOL_POSITIVE,
			    .image_invert_enable = FALSE,
			    .data_sz = _8BITS,
                            .med_filt_thres = 0,
                            .mfilt1 = NO_MEDIAN_FILTER1,
                            .mfilt2 = NO_MEDIAN_FILTER2,
                            .ccdc_offset = 0,
                            .lpf_enable = FALSE,
                            .datasft = 0,
                            .alaw = {
                                     .b_alaw_enable = TRUE,
                                     .gama_wd = 2},
                            .blk_clamp = {
                                          .b_clamp_enable = FALSE,
                                          .sample_pixel = 1,
                                          .start_pixel = 0,
                                          .dc_sub = 0},
                            .blk_comp = {
                                         .b_comp = 0,
                                         .gb_comp = 0,
                                         .gr_comp = 0,
                                         .r_comp = 0},
                            .vertical_dft = {
                                             .ver_dft_en = FALSE},
                            .lens_sh_corr = {
                                             .lsc_enable = FALSE},
                            .data_formatter_r = {
                                                 .fmt_enable = FALSE},
                            .color_space_con = {
                                                .csc_enable = FALSE}
                            },
	.ccdc_params_ycbcr = {0},
	.irqlock = SPIN_LOCK_UNLOCKED
        //.resizer_no = -1,    /* Extra field for resizer number filled in open */
        //.otf_on = 0          /* On the Fly Flag initialize to 0 i.e. Disabled */
};
static 		int rsz_cnt = 0;
stream_ext_params 	vpfe_stream_ext ={
	.ext_res_num = 0,
	.res_setting[0].width = 352,
	.res_setting[0].height = 192,
	.res_setting[0].index = 0
};

static ccdc_frmfmt frm_format;
static ccdc_imgwin image_window;
static vpfe_obj vpfe_device = { 0 };

enum{
	IRQ_USE_VD1 = 0,
	IRQ_USE_IPIPE = 1,
	IRQ_USE_FREE
};
void irq_setup_by_fmt(int fmt);


int ccdc_addr2;
//int ccdc_addr3;

typedef struct tvp5150_reg_apro
{
	u8 tvp5150_addr;
	u8 val;	
}tvp5150_reg_apro;



struct v4l2_capability vpfe_drvcap = {
	.driver = "vpfe driver",
	.card = "DaVinci EVM",
	.bus_info = "Platform",
	.version = VPFE_VERSION_CODE,
	.capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING
};

/* inline function to free reserved pages  */
void inline free_reserved_pages(unsigned long bufaddr, unsigned long bufsize)
{
	unsigned long size, addr;
	if (!bufaddr)
		return;
	addr = bufaddr;
	size = PAGE_SIZE << (get_order(bufsize));
	while (size > 0) {
		ClearPageReserved(virt_to_page(addr));
		addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	free_pages(bufaddr, get_order(bufsize));
}

/*
 * ======== sense_std ========
 */
/*This function will get current input standard for TVP5150*/
static int sense_std(v4l2_std_id * std_id)
{
	return 0;
}
/*This function will get current input standard for TVP5150*/

/*
 * ======== sense_std ========
 */
/*This function will get current input standard for TVP7002*/
static int sense_std_tvp7002(v4l2_std_id * std_id)
{
	return 0;
}


/*
 * ======== vpfe_isr ========
 */
/*ISR for VINT0*/
static irqreturn_t vpfe_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	vpfe_obj *vpfe = &vpfe_device;
	int fid;
//	unsigned long jiffies_time = get_jiffies_64();
	struct timeval timevalue;
//	int val = 0;

	//val = ccdc_sbl_reset();

	/*Convert time representations between jiffies and struct timeval */
	do_gettimeofday(&timevalue);
	dev_dbg(vpfe_dev, "time is %d:%d\n", timevalue.tv_sec, timevalue.tv_usec);
	dev_dbg(vpfe_dev, "\nStarting Davinci_vpfe\vpfe_isr...");
	if (frm_format == CCDC_FRMFMT_INTERLACED) 
	{
		//printk("######## Inside vpfe_isr CCDC_FRMFMT_INTERLACED\n");
		/* check which field we are in hardware */
		fid = ccdc_getfid();
		/* switch the software maintained field id */
		vpfe->field_id ^= 1;
		dev_dbg(vpfe_dev, "field id = %x:%x.\n", fid, vpfe->field_id);
		if (fid == vpfe->field_id) {	/* we are in-sync here,continue */
			if (fid == 0) {
				/*  One frame is just being captured. If the
				 * next frame is available, release the current
				 * frame and move on
				 */
				if (vpfe->curFrm != vpfe->nextFrm) {
					/* Copy frame capture time value in
					 * curFrm->ts
					 */
					vpfe->curFrm->ts = timevalue;
					vpfe->curFrm->state = STATE_DONE;
					wake_up_interruptible(&vpfe->
							      curFrm->done);
					vpfe->curFrm = vpfe->nextFrm;
				}
				/* based on whether the two fields are stored
				 * interleavely or separately in memory,
				 * reconfigure the CCDC memory address
				 */
				if (vpfe->field == V4L2_FIELD_SEQ_TB) {
					u32 addr =
					    vpfe->curFrm->boff +
					    vpfe->field_offset;
					ccdc_setfbaddr((unsigned long)
						       addr);
				}
			} else if (fid == 1) {
				/* if one field is just being captured */
				/* configure the next frame */
				/* get the next frame from the empty queue */
				/* if no frame is available, */
				/* hold on to the current buffer */
				if (!list_empty(&vpfe->dma_queue)
				    && vpfe->curFrm == vpfe->nextFrm) {
					vpfe->nextFrm =
					    list_entry(vpfe->dma_queue.
						       next, struct
						       videobuf_buffer, queue);
					list_del(&vpfe->nextFrm->queue);
					vpfe->nextFrm->state = STATE_ACTIVE;
					ccdc_setfbaddr((unsigned long)
						       vpfe->nextFrm->boff);
				}
				if (vpfe->mode_changed) {
					ccdc_setwin(&image_window,
						    frm_format, 2);
					/* update the field offset */
					vpfe->field_offset =
					    (vpfe->vwin.height -
					     2) * vpfe->vwin.width;
					vpfe->mode_changed = FALSE;
				}
			}
		} else if (fid == 0) {
			/* recover from any hardware out-of-sync due to */
			/* possible switch of video source              */
			/* for fid == 0, sync up the two fids           */
			/* for fid == 1, no action, one bad frame will  */
			/* go out, but it is not a big deal             */
			vpfe->field_id = fid;
		}
	} 
	else if (frm_format == CCDC_FRMFMT_PROGRESSIVE) 
	{
		//dev_dbg(vpfe_dev, "\nframe format is progressive...");
		//printk("######## Inside vpfe_isr CCDC_FRMFMT_PROGRESSIVE\n");
		if (vpfe->curFrm != vpfe->nextFrm) 
		{
			/* Copy frame capture time value in curFrm->ts */
			vpfe->curFrm->ts = timevalue;
			vpfe->curFrm->state = STATE_DONE;
			wake_up_interruptible(&vpfe->curFrm->done);
			vpfe->curFrm = vpfe->nextFrm;
			//printk("######## vpfe_isr CCDC_FRMFMT_PROGRESSIVE\n");
		}
	}
	dev_dbg(vpfe_dev, "interrupt returned.\n");
	return IRQ_RETVAL(1);
}

static irqreturn_t v4l2Ipipe_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	vpfe_obj 	*vpfe 	= &vpfe_device;
	unsigned int 	offset 	= vpfe->vwin.width*vpfe->vwin.height*2;



	dev_dbg(vpfe_dev, "\nInside v4l2Ipipe_isr...");
	//printk(" kernel fun:%s line:%d\n", __FUNCTION__, __LINE__);


	if (frm_format == CCDC_FRMFMT_PROGRESSIVE) {
//printk(" kernel fun:%s line:%d\n", __FUNCTION__, __LINE__);
		if (!list_empty(&vpfe->dma_queue) && vpfe->curFrm == vpfe->nextFrm) {
			vpfe->nextFrm = list_entry(vpfe->dma_queue.next, struct videobuf_buffer, queue);
			list_del(&vpfe->nextFrm->queue);
			vpfe->nextFrm->state = STATE_ACTIVE;
			//ccdc_setfbaddr((unsigned long)vpfe->nextFrm->boff);
			write_out_addr(0, (unsigned int)vpfe->nextFrm->boff);
			write_out_addr(1, ((unsigned int)vpfe->nextFrm->boff) + offset + VPFE_EXT_HEADER_LEN);

			if( vpfe_stream_ext.ext_res_num > 1 ) {
				rsz_cnt++;
				rsz_cnt = rsz_cnt % MAX_STREAM_EXT_NUM;
			}

			pResHeader = (Res_Set	*)phys_to_virt( ((unsigned long)vpfe->nextFrm->boff)+offset);
			pResHeader->width  = vpfe_stream_ext.res_setting[rsz_cnt].width;
			pResHeader->height = vpfe_stream_ext.res_setting[rsz_cnt].height;
			pResHeader->index = vpfe_stream_ext.res_setting[rsz_cnt].index;
			pResHeader->diff = vpfe_stream_ext.res_setting[rsz_cnt].diff;

			if( vpfe_stream_ext.ext_res_num > 1 ) {
				ipipe_change_rsz1_setting( pResHeader->width, pResHeader->height, pResHeader->diff );
			}

			if( ccdc_dcsub_update >= 0 )
			{
				
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				ccdc_dcsub((int)(ccdc_dcsub_update<<2) );
#else
				ccdc_dcsub((int)ccdc_dcsub_update );
#endif
				ccdc_dcsub_update = -1;
			}
				
				
		}
	}
	return IRQ_RETVAL(1);
}
static irqreturn_t vdint1_isr(int irq, void *dev_id, struct pt_regs *regs)
{

	vpfe_obj *vpfe = &vpfe_device;

	//dev_dbg(vpfe_dev, "\nInside vdint1_isr...");
	//printk("####### Inside vdint1_isr\n");

	if (frm_format == CCDC_FRMFMT_PROGRESSIVE) 
	//if(0)
	{
		if (!list_empty(&vpfe->dma_queue) && vpfe->curFrm == vpfe->nextFrm) 
		{
			vpfe->nextFrm = list_entry(vpfe->dma_queue.next, struct videobuf_buffer, queue);
			list_del(&vpfe->nextFrm->queue);
			vpfe->nextFrm->state = STATE_ACTIVE;
			ccdc_setfbaddr((unsigned long)vpfe->nextFrm->boff);
			//printk("****** vdint1_isr\n");
		}
	}
	return IRQ_RETVAL(1);
}

/*
 * ======== buffer_prepare ========
 */
/* this is the callback function called from videobuf_qbuf() function */
/* the buffer is prepared and queued into the dma queue */
static int buffer_prepare(struct videobuf_queue *q,
			  struct videobuf_buffer *vb, enum v4l2_field field)
{
	vpfe_obj *vpfe = &vpfe_device;
	unsigned int buf_size;
	dev_dbg(vpfe_dev, "\nstarting buffer_prepare");
	if(device_type == TVP5150) 
	{
		buf_size = VPFE_TVP5150_MAX_FBUF_SIZE;
	}
	else if(device_type == MT9M131)
	{
		buf_size = VPFE_MT9M131_MAX_FBUF_SIZE;	
	}	 
	else if (device_type == TVP7002) {
		buf_size = VPFE_TVP7002_MAX_FBUF_SIZE;
	}
	else if(device_type == TW2835)//tw2835
	{
		buf_size = VPFE_TW2835_MAX_FBUF_SIZE;
	}
	else
	{
		buf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
	}
	if (vb->state == STATE_NEEDS_INIT) {
		vb->width = vpfe->vwin.width;
		vb->height = vpfe->vwin.height;
		vb->size = buf_size;
		vb->field = field;
	}
	vb->state = STATE_PREPARED;
	dev_dbg(vpfe_dev, "\nEnd of buffer_prepare");
	return 0;

}

/*
 * ======== buffer_config ========
 */
 /* This function is responsible to queue up vpfe buffer is into video buffer
  * queue.
  */
static void buffer_config(struct videobuf_queue *q, unsigned int count)
{
	vpfe_obj *vpfe = &vpfe_device;
	int i;
	dev_dbg(vpfe_dev, "\nstarting buffer_config");
	for (i = 0; i < count; i++) {
		q->bufs[i]->boff = virt_to_phys(vpfe->fbuffers[i]);
		dev_dbg(vpfe_dev, "buffer address: %x\n", q->bufs[i]->boff);
	}
	dev_dbg(vpfe_dev, "\nEnd of buffer_config");
}

/*
 * ======== buffer_setup ========
 */
 /* This function allocate free pages and makes it sure that they will not
  * swapped
  */
static int
buffer_setup(struct videobuf_queue *q, unsigned int *count, unsigned int *size)
{
	vpfe_obj *vpfe = &vpfe_device;
	int i;
	unsigned int buf_size;
	dev_dbg(vpfe_dev, "\nstarting buffer_setup");
	if(device_type == TVP5150) 
	{
		*size = buf_size = VPFE_TVP5150_MAX_FBUF_SIZE;
	}
	else if(device_type == MT9M131) 
	{
		*size = buf_size = VPFE_MT9M131_MAX_FBUF_SIZE;
	}
	else if (device_type == TVP7002) {
		*size = buf_size = VPFE_TVP7002_MAX_FBUF_SIZE;
	}
	else if(device_type == TW2835) 
	{
		*size = buf_size = VPFE_TW2835_MAX_FBUF_SIZE;
	}
	else
	{
		*size = buf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
	}

	for (i = VPFE_DEFNUM_FBUFS; i < *count; i++) {
		u32 size = PAGE_SIZE << (get_order(buf_size));
		void *mem = (void *)__get_free_pages(GFP_KERNEL | GFP_DMA,
						     get_order(buf_size));
		if (mem) {
			unsigned long adr = (unsigned long)mem;
			while (size > 0) {
				/* make sure the frame buffers are never
				   swapped out of memory */
				SetPageReserved(virt_to_page(adr));
				adr += PAGE_SIZE;
				size -= PAGE_SIZE;
			}
			vpfe->fbuffers[i] = mem;
		} else {
			break;
		}
	}
	*count = vpfe->numbuffers = i;
	dev_dbg(vpfe_dev, "\nEnd of buffer_setup");
	return 0;
}

/*
 * ======== buffer_queue ========
 */
 /* This function adds the buffer to DMA queue */
static void buffer_queue(struct videobuf_queue *q, struct videobuf_buffer *vb)
{
	vpfe_obj *vpfe = &vpfe_device;
	/* add the buffer to the DMA queue */
	dev_dbg(vpfe_dev, "\nstarting buffer_queue");
	list_add_tail(&vb->queue, &vpfe->dma_queue);
	vb->state = STATE_QUEUED;
	dev_dbg(vpfe_dev, "\nEnding buffer_queue");
}

/*
 * ======== buffer_release ========
 */
 /* This function will free the buffer if it is not one
  * of the 3 allocated at initialization time.
  */
static void buffer_release(struct videobuf_queue *q, struct videobuf_buffer *vb)
{
	unsigned int buf_size;
	dev_dbg(vpfe_dev, "\nStarting buffer_release");
	if (device_type == TVP5150) 
	{
		buf_size = VPFE_TVP5150_MAX_FBUF_SIZE;
	}
	else if(device_type == MT9M131)
	{
		buf_size = VPFE_MT9M131_MAX_FBUF_SIZE;	
	}	 
	else if (device_type == TVP7002) {
		buf_size = VPFE_TVP7002_MAX_FBUF_SIZE;
	}
	else if(device_type == TW2835)
	{
		buf_size = VPFE_TW2835_MAX_FBUF_SIZE;	
	}	 
	else
	{
		buf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
	}

	/* free the buffer if it is not one of the 3 allocated at initializaiton
	 * time
	 */
	if (vb->i < vpfe_device.numbuffers
	    && vb->i >= VPFE_DEFNUM_FBUFS && vpfe_device.fbuffers[vb->i]) {
		free_pages((unsigned long)vpfe_device.
			   fbuffers[vb->i], get_order(buf_size));
		vpfe_device.fbuffers[vb->i] = NULL;
	}
	dev_dbg(vpfe_dev, "\nEnd of buffer_release");
}

static struct videobuf_queue_ops video_qops = {
	.buf_setup = buffer_setup,
	.buf_prepare = buffer_prepare,
	.buf_queue = buffer_queue,
	.buf_release = buffer_release,
	.buf_config = buffer_config,
};

static void set_ext_stream(stream_ext_params 	*params, struct ipipe_params *config)
{
	//memcpy( &vpfe_stream_ext, params, sizeof(stream_ext_params) );
	int	cnt = 0;
	if( params->ext_res_num > 0 && params->ext_res_num < MAX_STREAM_EXT_NUM )
	{
		int res_width	= params->res_setting[0].width;
		int res_height	= params->res_setting[0].height;
		int ratio_1	= ((config->rsz_rsc_param[0].rsz_o_hsz+1)*256 )/res_width;
		int ratio_2	= ((config->rsz_rsc_param[0].rsz_o_vsz+1)*256 )/res_height;
		int ratio_use;

		if( ratio_1 < ratio_2 )
		{
			ratio_use = ratio_1;
		}else{
			ratio_use = ratio_2;
		}
		/* reset the  resizer 1*/
		config->rsz_rsc_param[1].rsz_o_hsz 	= res_width - 1;
		config->rsz_rsc_param[1].rsz_o_vsz 	= res_height - 1;
		config->rsz_rsc_param[1].rsz_v_dif 	= ratio_use;
		config->rsz_rsc_param[1].rsz_h_dif 	= ratio_use;
		config->ext_mem_param[1].rsz_sdr_oft	= res_width*2;
		config->ext_mem_param[1].rsz_sdr_ptr_e	= res_height;

	}

	for( cnt = 0; cnt < MAX_STREAM_EXT_NUM ;cnt++ )
	{
		int res_width	= params->res_setting[cnt].width;
		int res_height	= params->res_setting[cnt].height;
		int ratio_1	= ((config->rsz_rsc_param[0].rsz_o_hsz+1)*256 )/res_width;
		int ratio_2	= ((config->rsz_rsc_param[0].rsz_o_vsz+1)*256 )/res_height;
		int ratio_use;

		if( ratio_1 < ratio_2 )
		{
			ratio_use = ratio_1;
		}else{
			ratio_use = ratio_2;
		}

		params->res_setting[cnt].diff = ratio_use;

	}


}

/*
 * ======== vpfe_doioctl ========
 */
 /* This function will provide different V4L2 commands.This function can be
  * used to configure driver or get status of driver as per command passed
  * by application.
  */
static int vpfe_doioctl(struct inode *inode, struct file *file,
			unsigned int cmd, void *arg)
{
	vpfe_obj *vpfe = &vpfe_device;
	vpfe_fh *fh = file->private_data;
	int ret = 0;

	switch (cmd) {
	case VIDIOC_S_CTRL:
	case VIDIOC_S_FMT:
	case VIDIOC_S_STD:
	case VIDIOC_S_CROP:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_CTRL ioctl");
		ret = v4l2_prio_check(&vpfe->prio, &fh->prio);
		if (0 != ret) {
			return ret;
		}
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_CTRL ioctl");
		break;
	case VIDIOC_DQBUF:
		// for debug printk("\nVIDIOC_DQBUF MSG is Passed in IOTCL\n");
		break;
	}

	switch (cmd) {
	case VIDIOC_QUERYCAP:
		{
			struct v4l2_capability *cap =
			    (struct v4l2_capability *)arg;
			//dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUEYCAP ioctl");
			//printk("Starting VIDIOC_QUEYCAP ioctl\n");
			memset(cap, 0, sizeof(*cap));
			*cap = vpfe_drvcap;
			//dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QUEYCAP ioctl");
			//printk("End of VIDIOC_QUEYCAP ioctl\n");
			break;
		}
	case VIDIOC_ENUM_FMT: // not used
		{
			struct v4l2_fmtdesc *fmt = (struct v4l2_fmtdesc *)arg;
			u32 index = fmt->index;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_ENUM_FMT ioctl");
			memset(fmt, 0, sizeof(*fmt));

			fmt->index = index;
			if((device_type == TVP5150) || (device_type == TVP7002))
			{
				if (index == 0) 
				{
					/* only yuv4:2:2 format is supported
					 * at this point
					 */
					fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					strcpy(fmt->description,
					       "YCbCr4:2:2 Interleaved UYUV");
					fmt->pixelformat = V4L2_PIX_FMT_UYVY;
				} else if (index == 1) {
					fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					strcpy(fmt->description,
					       "YCbCr4:2:2 Interleaved YUYV");
					fmt->pixelformat = V4L2_PIX_FMT_YUYV;
				} else {
					ret = -EINVAL;
				}
			} 
			else if (device_type == MT9T001 || device_type == MT9T031 ||device_type == MT9P031) 
			{
				if (index == 0) 
				{
					/* only Bayer Raw Mode format is
					 * supported at this point
					 */
					fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					strcpy(fmt->description,
					       "Raw Mode -Bayer Pattern GrRBGb");
					fmt->pixelformat = V4L2_PIX_FMT_SBGGR8;
				}
				else if (index == 1) 
				{
					fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					strcpy(fmt->description,
					       "Raw Mode -Pattern YUYV");
					fmt->pixelformat = V4L2_PIX_FMT_YUYV;
				}
				else
				{
					ret = -EINVAL;
				}
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_ENUM_FMT ioctl");

			break;
		}
	case VIDIOC_G_FMT:  // not used in app
		{
			struct v4l2_format *fmt = (struct v4l2_format *)arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_G_FMT ioctl");
			if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				ret = -EINVAL;
			} else {
				struct v4l2_pix_format *pixfmt = &fmt->fmt.pix;
				down_interruptible(&vpfe->lock);
				pixfmt->width = vpfe->vwin.width;
				pixfmt->height = vpfe->vwin.height;
				pixfmt->field = vpfe->field;
				pixfmt->pixelformat = vpfe->pixelfmt;
				if((device_type == TVP5150) || (device_type == TVP7002))
				{
					pixfmt->bytesperline = pixfmt->width * 2;
				}
				else
				{
					pixfmt->bytesperline =
					    ((vpfe->ccdc_params_raw.
					      data_sz == _8BITS)
					     || (vpfe->ccdc_params_raw.
						 alaw.b_alaw_enable))
					    ? pixfmt->width : (pixfmt->
							       width * 2);
				}
				pixfmt->sizeimage =
				    pixfmt->bytesperline * pixfmt->height;
				pixfmt->colorspace = V4L2_COLORSPACE_SMPTE170M;
				up(&vpfe->lock);
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_FMT ioctl");
			break;
		}
	case VIDIOC_S_FMT:
		{
			struct v4l2_format *fmt = (struct v4l2_format *)arg;
			struct v4l2_pix_format *pixfmt = &fmt->fmt.pix;
			ccdc_imgwin *img_win;
			ccdc_buftype *buf_type;
			ccdc_frmfmt *frame_format;

			if((device_type == TVP5150)||(device_type == TVP7002))
			{
				//printk("VIDIOC_S_FMT in TVP5150\n");
				img_win = &(vpfe->ccdc_params_ycbcr.win);
				buf_type = &(vpfe->ccdc_params_ycbcr.buf_type);
				frame_format = &(vpfe->ccdc_params_ycbcr.frm_fmt);
			}
			else if(device_type == MT9M131)
			{
				//printk("VIDIOC_S_FMT in MT9M131\n");
				img_win = &(vpfe->ccdc_params_ycbcr.win);
				buf_type = &(vpfe->ccdc_params_ycbcr.buf_type);
				frame_format = &(vpfe->ccdc_params_ycbcr.frm_fmt);
			} 
			
			else //tw2835
			{
				//printk("VIDIOC_S_FMT in MT9P031\n");
				img_win = &(vpfe->ccdc_params_ycbcr.win);
				buf_type = &(vpfe->ccdc_params_ycbcr.buf_type);
				frame_format = &(vpfe->ccdc_params_ycbcr.frm_fmt);

			}

			dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_FMT ioctl");

			if (vpfe->started) 
			{
				/* make sure streaming is not started */
				printk("vpfe->started error\n");
				ret = -EBUSY;
				break;
			}

			down_interruptible(&vpfe->lock);

			dev_dbg(vpfe_dev, "\nAfter down_interruptible");

			if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) 
			{
				printk("V4L2_BUF_TYPE_VIDEO_CAPTURE error\n");
				ret = -EINVAL;
				up(&vpfe->lock);
				break;
			}

			if((pixfmt->width + vpfe->vwin.left <= vpfe->bounds.width)&&(pixfmt->height + vpfe->vwin.top <= vpfe->bounds.height)) 
			{
				/* this is the case when no scaling is
				 * supported
				 */
				/* crop window is directed modified */
				vpfe->vwin.height = pixfmt->height;
				vpfe->vwin.width = pixfmt->width;
				img_win->width = pixfmt->width;
				img_win->height = pixfmt->height;
			} 
			else 
			{
				printk("Window size error!in VPFE_IOCTRL\n");
				ret = -EINVAL;
				dev_dbg(vpfe_dev, "\n window size error!\n");
				up(&vpfe->lock);
				break;
			}



			printk("pixfmt->width : %d\n",pixfmt->width);
			printk("vpfe->vwin.left : %d\n",vpfe->vwin.left);
			printk("vpfe->bounds.width : %d\n",vpfe->bounds.width);
			
			printk("pixfmt->height : %d\n",pixfmt->height);
			printk("vpfe->vwin.top : %d\n",vpfe->vwin.top);
			printk("vpfe->bounds.height : %d\n",vpfe->bounds.height);

			/* setup the CCDC parameters accordingly */
			if((device_type == TVP5150)||(device_type == MT9M131)||(device_type == TVP7002)||(device_type == TW2835))
			{
				//printk("\npixfmt->pixelformat=[%d]\n", pixfmt->pixelformat);
				if (pixfmt->pixelformat == V4L2_PIX_FMT_YUYV) 
				{
					vpfe->ccdc_params_ycbcr.pix_order = CCDC_PIXORDER_YCBYCR;
					vpfe->pixelfmt = pixfmt->pixelformat;
				} 
				else if (pixfmt->pixelformat == V4L2_PIX_FMT_UYVY)
				{
					vpfe->ccdc_params_ycbcr.pix_order = CCDC_PIXORDER_CBYCRY;
					vpfe->pixelfmt = pixfmt->pixelformat;
				} 
				else 
				{
					/* not supported format */
					printk("Not supported format in VPFE_IOCTL\n");
					ret = -EINVAL;
					up(&vpfe->lock);
					break;
				}
			} 
			
			else if (device_type == MT9T001 || device_type == MT9T031 || device_type == MT9P031) 
			{
				if (pixfmt->pixelformat == V4L2_PIX_FMT_SBGGR8) 
				{
					vpfe->pixelfmt = pixfmt->pixelformat;
				}
				else if( pixfmt->pixelformat == V4L2_PIX_FMT_YUYV)
				{
					vpfe->pixelfmt = pixfmt->pixelformat;
				}
				else
				{
					/* not supported format */
				  	printk("No supported format in VPFE_IOCTL\n");
				        dev_dbg(vpfe_dev, "\n format not supported");
					ret = -EINVAL;
					up(&vpfe->lock);
					break;
				}
			}
			
			/* Configure buffer type and frame format as per field
			 *  value passed
			 */
			if (pixfmt->field == V4L2_FIELD_INTERLACED) {
				*buf_type = CCDC_BUFTYPE_FLD_INTERLEAVED;
				*frame_format = CCDC_FRMFMT_INTERLACED;
				vpfe->field = pixfmt->field;
			} else if (pixfmt->field == V4L2_FIELD_SEQ_TB) {
				*buf_type = CCDC_BUFTYPE_FLD_SEPARATED;
				*frame_format = CCDC_FRMFMT_INTERLACED;
				vpfe->field = pixfmt->field;
			} else if (pixfmt->field == V4L2_FIELD_NONE) {
				*frame_format = CCDC_FRMFMT_PROGRESSIVE;
				vpfe->field = pixfmt->field;
			} else {
				printk("field error!in VPFE_IOCTL\n");
				dev_dbg(vpfe_dev, "\n field error!");
				ret = -EINVAL;
			}

			up(&vpfe->lock);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_FMT ioctl");
			//printk("\nEnd of VIDIOC_S_FMT ioctl");
			break;
		}
	case VIDIOC_TRY_FMT:
		{
			struct v4l2_format *fmt = (struct v4l2_format *)arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_TRY_FMT ioctl");
			if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				ret = -EINVAL;
			} else {
				struct v4l2_pix_format *pixfmt = &fmt->fmt.pix;

				if((device_type == TVP5150)||(device_type == TVP7002))
				{
					if (pixfmt->width >
					    vpfe->bounds.width
					    || pixfmt->height >
					    vpfe->bounds.height
					    || (pixfmt->pixelformat !=
						V4L2_PIX_FMT_UYVY
						&& pixfmt->pixelformat !=
						V4L2_PIX_FMT_YUYV)) {
						ret = -EINVAL;
					}
				} else if (device_type == MT9T001 || device_type == MT9T031 ||
						device_type == MT9P031) {
					if (pixfmt->width >
					    vpfe->bounds.width
					    || pixfmt->height >
					    vpfe->bounds.height
					    || (pixfmt->pixelformat !=
						V4L2_PIX_FMT_SBGGR8
						&& pixfmt->pixelformat !=
						V4L2_PIX_FMT_YUYV)) {
						ret = -EINVAL;
					}
				}
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_TRY_FMT ioctl");
			break;
		}
	case VIDIOC_G_STD:
		{
			v4l2_std_id *id = (v4l2_std_id *) arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_G_STD ioctl");
			*id = vpfe->std;
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_STD ioctl");
			break;
		}
	case VIDIOC_S_STD:
		{
			v4l2_std_id id = *(v4l2_std_id *) arg; // v4l2_std_id ==> typedef __u64 v4l2_std_id
			tvp5150_mode mode;
			tvp7002_mode mode2;

			if(device_type == TVP5150)
				mode = TVP5150_MODE_INV;
			else if(device_type == TVP7002)
				mode2 = TVP7002_MODE_INV;
			int sqp = 0;
			
			//printk("VIDIOC_S_STD id : [%d]\n",(int)id);
			
			dev_dbg(vpfe_dev, "Starting of VIDIOC_S_STD ioctl");
			//printk("Starting of VIDIOC_S_STD ioctl...\n");
			/* make sure streaming is not started */
			if (vpfe->started) 
			{
				//printk("vpfe->started...\n");
				ret = -EBUSY;
				break;
			}
			down_interruptible(&vpfe->lock);
#if 0
			if (device_type == TVP7002) 
			{
					
				if (id & V4L2_STD_720P_30) {
						printk("#############V4L2_STD_720P_30 MODE Setting#################################\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P720_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = P720_bounds;
			  }	else if (id & V4L2_STD_625_50) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    pal_bounds;
				} else if (id & V4L2_STD_525_60) {
					printk("##################V4L2_STD_525_60 MODE Setting##################\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = ntsc_bounds;
					vpfe->pixelaspect = ntsc_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    ntsc_bounds;
				} else if (id & VPFE_STD_625_50_SQP) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin =
					    palsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					sqp = 1;
					id >>= 32;
				} else if (id & VPFE_STD_525_60_SQP) {
					vpfe->std = id;
					sqp = 1;
					vpfe->std = id;
					id >>= 32;
					vpfe->bounds = vpfe->vwin =
					    ntscsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    ntscsp_bounds;
				} else if (id & VPFE_STD_AUTO) {
					mode2 = TVP7002_MODE_AUTO;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    pal_bounds;
					vpfe->std = id;
				} else if (id & VPFE_STD_AUTO_SQP) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin =
					    palsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					sqp = 1;
					mode2 = TVP7002_MODE_AUTO_SQP;
					vpfe->pixelaspect = sp_aspect;
				} else {
					ret = -EINVAL;
				}
		    if (id == V4L2_STD_720P_30) {
					mode2 = TVP7002_MODE_720p_30FPS;
			
				} else if (id & V4L2_STD_625_50) {
					mode2 = TVP7002_MODE_576p_25FPS;
				} else if (id & V4L2_STD_525_60) {
					mode2 = TVP7002_MODE_480p_30FPS;
				} else if (id & V4L2_STD_SECAM) {
					mode2 = TVP7002_MODE_SECAM;
				}
				vpfe->tvp7002_params.mode = mode2 | (sqp << 3);
				tvp7002_ctrl(TVP7002_CONFIG,
					     &vpfe->tvp7002_params);

			}
			else if (device_type == TVP5150) 
			{
				if (id & V4L2_STD_625_50) 
				{
					#if DM355_VPFE_DEBUG
					#endif
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    pal_bounds;

				} else if (id & V4L2_STD_525_60) {
					#if DM355_VPFE_DEBUG
					#endif
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = ntsc_bounds;
					vpfe->pixelaspect = ntsc_aspect;
					vpfe->ccdc_params_ycbcr.win = ntsc_bounds;
				} else if (id & VPFE_STD_625_50_SQP) {
					#if DM355_VPFE_DEBUG
					#endif
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = palsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					sqp = 1;
					id >>= 32;
				} else if (id & VPFE_STD_525_60_SQP) {
					#if DM355_VPFE_DEBUG
					#endif
					vpfe->std = id;
					sqp = 1;
					vpfe->std = id;
					id >>= 32;
					vpfe->bounds = vpfe->vwin = ntscsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					vpfe->ccdc_params_ycbcr.win = ntscsp_bounds;
				} else if (id & VPFE_STD_AUTO) {
					#if DM355_VPFE_DEBUG
					#endif
					mode = TVP5150_MODE_AUTO;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    pal_bounds;
					vpfe->std = id;
				} else if (id & VPFE_STD_AUTO_SQP) {
					#if DM355_VPFE_DEBUG
					//printk("id & VPFE_STD_AUTO_SQP\n");
					#endif
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin =
					    palsp_bounds;
					vpfe->pixelaspect = sp_aspect;
					sqp = 1;
					mode = TVP5150_MODE_AUTO_SQP;
					vpfe->pixelaspect = sp_aspect;
				}
				else 
				{
					//printk("else return error\n");
					ret = -EINVAL;
				}
				if (id == V4L2_STD_PAL_60) {
					#if DM355_VPFE_DEBUG
					//printk("id == V4L2_STD_PAL_60\n");
					#endif
					mode = TVP5150_MODE_PAL_60;
				} else if (id == V4L2_STD_PAL_M) {
					#if DM355_VPFE_DEBUG
					//printk("id == V4L2_STD_PAL_M\n");
					#endif
					mode = TVP5150_MODE_PAL_M;
				} else if (id == V4L2_STD_PAL_Nc
					   || id == V4L2_STD_PAL_N) {
					#if DM355_VPFE_DEBUG
					//printk("id == V4L2_STD_PAL_N\n");
					#endif
					mode = TVP5150_MODE_PAL_CN;
				} else if (id & V4L2_STD_PAL) {
					#if DM355_VPFE_DEBUG
					//printk("id & V4L2_STD_PAL\n");
					#endif
					mode = TVP5150_MODE_PAL;
				} else if (id & V4L2_STD_NTSC) {
					#if DM355_VPFE_DEBUG
					//printk("id & V4L2_STD_NTSC\n");
					#endif
					mode = TVP5150_MODE_NTSC;
				} else if (id & V4L2_STD_SECAM) {
					#if DM355_VPFE_DEBUG
					//printk("id & V4L2_STD_SECAM\n");
					#endif
					mode = TVP5150_MODE_SECAM;
				}
				vpfe->tvp5150_params.mode = mode | (sqp << 3);
				//printk("tvp5150_ctrl TVP5150_CONFIG\n");
				tvp5150_ctrl(TVP5150_CONFIG,&vpfe->tvp5150_params);  
			}
#endif
			if (device_type == MT9M131) 
			{
				/* Store image window paramters and pixel
				 * aspect values as per standard
				 * passed by application
				 */
				//printk("in VIDIOC_S_STD ioctl device_type == MT9M131\n");
				//id = V4L2_STD_MT9M131_VGA_30FPS;
				//id = V4L2_STD_MT9T001_VGA_30FPS;
				
				if(id == MT9M131_MODE_VGA_30FPS)
				{
					printk("\nid ==> VGA_30FPS\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = VGA_bounds;//VGA_bounds_512;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = VGA_bounds;//VGA_bounds_512;
					//ret =  vpfe_device.config_dev_fxn(MT9T001_INIT,&vpfe_device.std,&vpfe_device.device_params);
				}
				else if(id == MT9M131_MODE_VGA_15FPS)
				{
					printk("\nid ==> VGA_15FPS\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = VGA_bounds; //VPFE_WIN_720p
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = VGA_bounds;	
				}	
				else if(id == MT9M131_MODE_SXVGA_960)
				{
					printk("\nid ==> 1280x960 MT9M131\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = SXGA_960_bounds; //VPFE_WIN_SXVGA 1280x960
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = SXGA_960_bounds;	
				}
				else if(id == MT9M131_MODE_SXVGA_15FPS)
				{
					printk("\nid ==> SXVGA_15FPS\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = SXGA_bounds; //VPFE_WIN_SXVGA 1280x1024
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = SXGA_bounds;	
				}		
				else if(id == MT9M131_MODE_P1280_720)
				{
					printk("\nid ==> 1280x720p\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P720_bounds; //VPFE_WIN_1024_720
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = P720_bounds;	
				}		
				else if(id == MT9M131_MODE_D1_NTSC_30FPS)
				{
					printk("\nid ==> 720x480\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = ntsc_bounds; //VPFE_WIN_1024_720
					vpfe->pixelaspect = ntsc_aspect;
					vpfe->ccdc_params_ycbcr.win = ntsc_bounds;	
				}		
				else if(id == MT9M131_MODE_D1_NTSC_15FPS)
				{
					printk("\nid ==> 1280x720p\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = ntsc_bounds; //VPFE_WIN_1024_720
					vpfe->pixelaspect = ntsc_aspect;
					vpfe->ccdc_params_ycbcr.win = ntsc_bounds;	
				}		
				else if(id == MT9M131_MODE_1440_1024_15FPS)
				{
					printk("\nid ==> 1440x1024\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P1440_1024_bounds; //VPFE_WIN_1024_720
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = P1440_1024_bounds;	
				}		
				else if(id == MT9M131_MODE_QVGA_30FPS)
				{
					printk("\nid ==> 320x240\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = QVGA_bounds; //VPFE_WIN_1024_720
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = QVGA_bounds;	
				}		
				else {
#if 0
				id = *(v4l2_std_id *) arg = MT9M131_MODE_VGA_30FPS;

				{
					printk("id ==> VGA_30FPS\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = VGA_bounds_512;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_ycbcr.win = VGA_bounds_512;
				}
#else
					printk("id[%d] ==> error\n", id);
					ret = -EINVAL;
#endif
				}

				if (ret != -EINVAL) 
				{
					/* Call device control function to
					 * configure video standard
					 */
			     //printk("id ==> err2\n");
			     dev_dbg(vpfe_dev, "\ncalling mt9t001 setup in SET_STD");
					 ret = vpfe->config_dev_fxn(MT9M131_SET_STD,&vpfe->std,vpfe->device_params);
				}
			}
			else if (device_type == MT9T001 || device_type == MT9T031 ||device_type == MT9P031)
			{
				/* Store image window paramters and pixel
				 * aspect values as per standard
				 * passed by application
				 */
				//printk("in VIDIOC_S_STD ioctl device_type == MT9P031\n");
				if ((id == V4L2_STD_MT9T001_VGA_30FPS)
				    || (id == V4L2_STD_MT9T001_VGA_60FPS)
				    || (id == V4L2_STD_MT9T001_VGA_BIN_30FPS) ) {
					//printk("id ==> 1\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = VGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = VGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_SVGA_30FPS)
					   || (id ==
					       V4L2_STD_MT9T001_SVGA_60FPS)) {
					//printk("id ==> 2\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = SVGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = SVGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_XGA_30FPS)) {
					//printk("id ==> 3\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = XGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = XGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_480p_30FPS)
					   || (id ==
					       V4L2_STD_MT9T001_480p_60FPS)) {
					//printk("id ==> 44444444444444444444444444444444444444444\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P480_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P480_bounds;
				} else if ((id == V4L2_STD_MT9T001_576p_25FPS)
					   || (id ==
					       V4L2_STD_MT9T001_576p_50FPS)) {
					//printk("id ==> 5\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P576_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P576_bounds;
				} else if ((id == V4L2_STD_MT9T001_720p_24FPS)
					   || (id ==
					       V4L2_STD_MT9T001_720p_30FPS)) {
					//printk("id ==> 6\n");       	
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P720_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P720_bounds;
				} else if ((id == V4L2_STD_MT9T001_1080p_18FPS)) {
					//printk("id ==> 7\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin =
					    P1080_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win =
					    P1080_bounds;
				} else {
					//printk("id ==> error\n");
					ret = -EINVAL;
				}
				if (ret != -EINVAL) {
					/* Call device control function to
					 * configure video standard
					 */
			     //printk("id ==> err2\n");
			                dev_dbg(vpfe_dev, "\ncalling mt9t001 setup in SET_STD");
					 ret = vpfe->config_dev_fxn(MT9T001_SET_STD,&vpfe->std,vpfe->device_params);
				}
			}
			else if (device_type == TW2835) 
			{
				/* Store image window paramters and pixel
				 * aspect values as per standard
				 * passed by application
				 */
				//printk("in VIDIOC_S_STD ioctl device_type == TW2835\n");
				
				if(id == TW2835_MODE_D1_NTSC)
				{
					printk("\nid ==> TW2835_MODE_D1_NTSC\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = ntsc_bounds;
					vpfe->pixelaspect = ntsc_aspect;
					vpfe->ccdc_params_ycbcr.win = ntsc_bounds;
				}
				else if(id == TW2835_MODE_D1_PAL)
				{
					printk("\nid ==> TW2835_MODE_D1_PAL\n");
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win = pal_bounds;	
				}	
				else {
					printk("id ==> error\n");
					ret = -EINVAL;
				}

			}
		
			dev_dbg(vpfe_dev, "\nAbove up(&vpfe->lock)");
			up(&vpfe->lock);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_STD ioctl");
			break;
		}
	case VIDIOC_ENUMSTD:
		{
			struct v4l2_standard *std = (struct v4l2_standard *)arg;
			u32 index = std->index;
			dev_dbg(vpfe_dev, "\nStarting of VIDIOC_ENUMSTD ioctl");
			memset(std, 0, sizeof(*std));
			std->index = index;

			if(device_type == TVP5150)
			{
				if (index == 0) {
					std->id = V4L2_STD_525_60;
					strcpy(std->name, "SD-525line-30fps");
					std->framelines = 525;
					std->frameperiod.numerator = 1001;
					std->frameperiod.denominator = 30000;
				} else if (index == 1) {
					std->id = V4L2_STD_625_50;
					strcpy(std->name, "SD-625line-25fps");
					std->framelines = 625;
					std->frameperiod.numerator = 1;
					std->frameperiod.denominator = 25;
				} else if (index == 2) {
					std->id = VPFE_STD_625_50_SQP;
					strcpy(std->name,
					       "SD-625line-25fps square pixel");
					std->framelines = 625;
					std->frameperiod.numerator = 1;
					std->frameperiod.denominator = 25;
				} else if (index == 3) {
					std->id = VPFE_STD_525_60_SQP;
					strcpy(std->name,
					       "SD-525line-25fps square pixel");
					std->framelines = 525;
					std->frameperiod.numerator = 1001;
					std->frameperiod.denominator = 30000;
				} else if (index == 4) {
					std->id = VPFE_STD_AUTO;
					strcpy(std->name, "automatic detect");
					std->framelines = 625;
					std->frameperiod.numerator = 1;
					std->frameperiod.denominator = 1;
				} else if (index == 5) {
					std->id = VPFE_STD_AUTO_SQP;
					strcpy(std->name,
					       "automatic detect square pixel");
					std->framelines = 625;
					std->frameperiod.numerator = 1;
					std->frameperiod.denominator = 1;
				} else {
					ret = -EINVAL;
				}
			} else if (device_type == MT9T001 || device_type == MT9T031 ||
					device_type == MT9P031) {
				/* fill number of lines per frame and FPS value
				 *  as per standard passed by application
				 */
				if (index == 0) {
					std->id = V4L2_STD_MT9T001_VGA_30FPS;
					strcpy(std->name, "VGA-480line- 30fps");
					std->framelines = FMT_VGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 1) {
					std->id = V4L2_STD_MT9T001_VGA_60FPS;
					strcpy(std->name, "VGA-480line- 60fps");
					std->framelines = FMT_VGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_60_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_60_DENOMIRATOR;
				} else if (index == 2) {
					std->id = V4L2_STD_MT9T001_SVGA_30FPS;
					strcpy(std->name,
					       "SVGA-600line- 30fps");
					std->framelines = FMT_SVGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 3) {
					std->id = V4L2_STD_MT9T001_SVGA_60FPS;
					strcpy(std->name,
					       "SVGA-600line- 60fps");
					std->framelines = FMT_SVGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_60_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 4) {
					std->id = V4L2_STD_MT9T001_XGA_30FPS;
					strcpy(std->name, "XGA-768line- 30fps");
					std->framelines = FMT_XGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 5) {
					std->id = V4L2_STD_MT9T001_480p_30FPS;
					strcpy(std->name, "480p-480line-30fps");
					std->framelines = FMT_480p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 6) {
					std->id = V4L2_STD_MT9T001_480p_60FPS;
					strcpy(std->name, "480p-480line-60fps");
					std->framelines = FMT_480p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_60_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_60_DENOMIRATOR;
				} else if (index == 7) {
					std->id = V4L2_STD_MT9T001_576p_25FPS;
					strcpy(std->name, "576p-576line-25fps");
					std->framelines = FMT_576p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_25_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_25_DENOMIRATOR;
				} else if (index == 8) {
					std->id = V4L2_STD_MT9T001_576p_50FPS;
					strcpy(std->name, "576p-576line-50fps");
					std->framelines = FMT_576p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_50_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_50_DENOMIRATOR;
				} else if (index == 9) {
					std->id = V4L2_STD_MT9T001_720p_24FPS;
					strcpy(std->name, "720p-720line-24fps");
					std->framelines = FMT_720p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_24_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_24_DENOMIRATOR;
				} else if (index == 10) {
					std->id = V4L2_STD_MT9T001_720p_30FPS;
					strcpy(std->name, "720p-720line-30fps");
					std->framelines = FMT_720p_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				} else if (index == 11) {
					std->id = V4L2_STD_MT9T001_1080p_18FPS;
					strcpy(std->name,
					       "1080p-1080line-18fps");
					std->framelines = FMT_1080i_NUMLINES;
					std->frameperiod.numerator =
					    FPS_18_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_18_DENOMIRATOR;
				}else if (index == 12) {
					std->id = V4L2_STD_MT9T001_VGA_BIN_30FPS;
					strcpy(std->name, "VGA-480line- 60fps");
					std->framelines = FMT_VGA_NUMLINES;
					std->frameperiod.numerator =
					    FPS_30_NUMERATOR;
					std->frameperiod.denominator =
					    FPS_30_DENOMIRATOR;
				}else {
					ret = -EINVAL;
				}
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_ENUMSTD ioctl");
			break;
		}
	case VIDIOC_ENUMINPUT:
		{
			u32 index = 0;
			struct v4l2_input *input = (struct v4l2_input *)arg;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_ENUMINPUT ioctl");
			if(device_type != TVP5150) 	
			{
				return -1;
			}
			/* only two inputs are available */
			if (input->index > 1)
				ret = -EINVAL;
			index = input->index;
			memset(input, 0, sizeof(*input));
			input->index = index;
			input->type = V4L2_INPUT_TYPE_CAMERA;
			input->std = V4L2_STD_ALL;
			if (input->index == 0) {
				sprintf(input->name, "COMPOSITE");
			} else if (input->index == 1) {
				sprintf(input->name, "S-VIDEO");
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_ENUMINPUT ioctl");
			break;
		}
	case VIDIOC_G_INPUT:
		{
			int *index = (int *)arg;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_G_INPUT ioctl");
			if (device_type != TVP5150) 	
			{
				return -1;
			}
			*index = vpfe->tvp5150_params.amuxmode;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_G_INPUT ioctl");
			break;
		}
	
	case VIDIOC_READ_CCDC:
		{
			//struct ccdc_reg_apro *index = (struct ccdc_reg_apro *)arg;
			int *index = (int *)arg;
			unsigned int reg_value;
			int err=0;
			if (device_type == TVP5150) 	
			{
				return -1;
			}
			//dev_dbg(vpfe_dev, "\nStart VIDIOC_READ_CCDC ioctl");			
			//printk("VIDIOC_READ_CCDC : index->ccdc_addr : %x\n",index->ccdc_addr);

			reg_value = ccdc_read_apro(*index);
			//index->val = reg_value;
			//printk("Kernel CCDC : 0x%x ==> %08x\n",*index, reg_value);
									
			return reg_value;
			
			break;
		}
	case VIDIOC_WRITE_CCDC:
		{
			int *index = (int *)arg;
			unsigned int reg_value;
			if (device_type == TVP5150) 	
			{
				return -1;
			}
			//dev_dbg(vpfe_dev, "\nStart VIDIOC_WRITE_CCDC ioctl");			
			//printk("ccdc_addr : %x, value : %08x\n",index->ccdc_addr,index->val);
			
			//printk("Kern Before Write CCDC addr  : 0x%x\n",*index);
			ccdc_addr2 = *index;
			break;
		}

	case VIDIOC_WRITE_CCDC2:
		{
			//int *index = (int *)arg;
			//struct ccdc_reg_apro *index = (struct ccdc_reg_apro *)arg;
			ccdc_reg_apro *index3 = (ccdc_reg_apro *)arg;
			int reg_value;
			
			if (device_type == TVP5150) 	
			{
				return -1;
			}
			//printk("Kernel CCDC Before Addr : 0x%x, Value : 0x%x\n",index3->ccdc_addr,index3->val);
			
			//ccdc_addr3 = index3->ccdc_addr;
			ccdc_write_apro(index3->ccdc_addr, index3->val);
			reg_value = ccdc_read_apro(index3->ccdc_addr);
			//printk("Kernel CCDC After  Write CCDC  : 0x%x ==> 0x%x\n",index3->ccdc_addr, index3->val);
			break;
		}
	
	case VIDIOC_WRITE_MT9M131:
		{
			mt9m131_reg_apro *index4 = (mt9m131_reg_apro *)arg;
			int r2_value_mt9m131;
			printk("Kernel MT9M131 Before Page : 0x%x, Addr : 0x%x, Value : 0x%x\n",index4->page,index4->mt9m131_addr,index4->val);

			mt9m131_write_apro(index4->page, index4->mt9m131_addr, index4->val);
			r2_value_mt9m131 = mt9m131_read_apro(index4->page, index4->mt9m131_addr);
			printk("Kernel MT9M131 After  : 0x%x ==> 0x%x\n",index4->mt9m131_addr, index4->val);
			break;
		}
	case VIDIOC_MT9M131_IOCTL:
		{
			mt9m131_reg_apro *index_ioctl = (mt9m131_reg_apro *)arg;
			int ioctl_value_mt9m131;
			//printk("Kernel mt9m131_ioctl_command Before Page : 0x%x, Value : 0x%x\n",index_ioctl->page,index_ioctl->val);
			ioctl_value_mt9m131 = mt9m131_ioctl_command(index_ioctl->page, index_ioctl->val);
			//return ioctl_value_mt9m131;
			break;
		}
		
	case VIDIOC_READ_MT9M131:
		{
			mt9m131_reg_apro *index5 = (mt9m131_reg_apro *)arg;
			int r_value_mt9m131;
			printk("Kernel VIDIOC_READ_MT9M131 Before Page : 0x%x, Addr : 0x%x, Value : 0x%x\n",index5->page,index5->mt9m131_addr,index5->val);
			r_value_mt9m131 = mt9m131_read_apro(index5->page, index5->mt9m131_addr);
			printk("Kernel MT9M131 After  ==> 0x%x\n",r_value_mt9m131);
			return r_value_mt9m131;
			break;
		}		
	case TVP5150_SHOW_ALL:
		{
			break;
		}	
	case TVP5150_READ:
	case TVP5150_NTSC_PAL:
			break;
	case TVP5150_RESET_PAL:
	case TVP5150_EPROM:
	case TVP5150_WRITE:
			break;
	case MT9M131_SHOW_ALL:
		{
			int *index = (int *)arg;
			printk("Start of MT9M131_SHOW_ALL Read Register!!\n");
			if (device_type != MT9M131) 
			{
				printk("device_type != MT9M131\n");
				return -1;
			}
			mt9m131_ctrl(MT9M131_READ_ALL, NULL, NULL);
			break;
		}
	
	case CCDC_SHOW_ALL:
		{
			int *index = (int *)arg;
			printk("Start of CCDC_SHOW_ALL Read Register!!\n");
			ccdc_readregs();
			break;
		}
	case MT9M131_WRITE_INIT:
		{
			int *index = (int *)arg;
			printk("Start of MT9M131_WRITE_INIT Register!!\n");
			if (device_type != MT9M131) 
			{
				printk("device_type != MT9M131\n");
				return -1;
			}
			mt9m131_ctrl(MT9M131_WRITE_ALL, NULL, NULL);
			break;
		}	
		
		
	case MT9P031_SHOW_ALL:
		{
			int *index = (int *)arg;
			printk("Start of MT9M131_SHOW_ALL Read Register!!\n");
			if (device_type != MT9P031) 
			{
				printk("device_type != MT9P031\n");
				return -1;
			}
			mt9p031_ctrl(MT9P031_READ_ALL, NULL, NULL);
			break;
		}
	
		
	case PINMUX3_UART2:
		{
			tvp5150_reg_apro *index = (tvp5150_reg_apro *)arg;
			int ret = 0;
			int ret2 =0;
			//printk("tvp5150_addr : %x, value : %x\n",index->tvp5150_addr,index->val);
			
       			ret = __REG(0x01c4000c);
			printk("Before PINMUX3[0x%08x]\n",ret);
       			ret2 = __REG(0x01c40018);
			printk("Before ARM_INTMUX[0x%08x]\n",ret2);
			
		
			ret2 = __REG(0x01C20000);
			printk("UART1 RBR[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20004);
			printk("UART1 IER[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20008);
			printk("UART1 IIR[0x%08x]\n",ret2);
			ret2 = __REG(0x01C2000c);
			printk("UART1 LCR[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20010);
			printk("UART1 MCR[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20014);
			printk("UART1 LSR[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20020);
			printk("UART1 DLL[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20024);
			printk("UART1 DLH[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20028);
			printk("UART1 PID1[0x%08x]\n",ret2);
			ret2 = __REG(0x01C2002c);
			printk("UART1 PID2[0x%08x]\n",ret2);
			ret2 = __REG(0x01C20030);
			printk("UART1 PWREMU_MGMT[0x%08x]\n",ret2);



			ret2 = __REG(0x01E06000);
			printk("UART2 RBR[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06004);
			printk("UART2 IER[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06008);
			printk("UART2 IIR[0x%08x]\n",ret2);
			ret2 = __REG(0x01E0600c);
			//ret2 |= 0x00000003;
			//__REG(0x01E0600c) = ret2;

			printk("UART2 LCR[0x%08x]\n",ret2);
			
			
			ret2 = __REG(0x01E06010);
			//ret2 |= 0x00000002;
			//__REG(0x01E06010) = ret2;
			 
			
			printk("UART2 MCR[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06014);
			printk("UART2 LSR[0x%08x]\n",ret2);
			
			ret2 = __REG(0x01E06020);
			//ret2 |= 0x0000000d;
			//ret2 |= 0x00000027;
			//__REG(0x01E0600c) = ret2;

			printk("UART2 DLL[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06024);
			printk("UART2 DLH[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06028);
			printk("UART2 PID1[0x%08x]\n",ret2);
			ret2 = __REG(0x01E0602c);
			printk("UART2 PID2[0x%08x]\n",ret2);
			ret2 = __REG(0x01E06030);
			printk("UART2 PWREMU_MGMT[0x%08x]\n",ret2);
			
			ret &= 0xffff0fff;
			ret |= 0x0000a000;
			//ret |= 0x0000aa00;
			 
			__REG(0x01c4000c) = ret;
			
			printk("After PINMUX3[0x%08x] UART2 Enabled !!\n",ret);
			
			break;
		}	

	case VIDIOC_S_INPUT:
			break;
	case VIDIOC_CROPCAP:
		{
			struct v4l2_cropcap *cropcap =
			    (struct v4l2_cropcap *)arg;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_CROPCAP ioctl");
			cropcap->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			down_interruptible(&vpfe->lock);
			cropcap->bounds = cropcap->defrect = vpfe->vwin;
			cropcap->pixelaspect = vpfe->pixelaspect;
			up(&vpfe->lock);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_CROPCAP ioctl");
			break;
		}
	case VIDIOC_G_PARM:
		{
			struct v4l2_streamparm *parm =
			    (struct v4l2_streamparm *)arg;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_G_PARM ioctl");
			printk("VIDIOC_G_PARM\n");
			if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				/* only capture is supported */
				ret = -EINVAL;
			} else {
				struct v4l2_captureparm *capparm =
				    &parm->parm.capture;
				memset(capparm, 0,
				       sizeof(struct v4l2_captureparm));
				down_interruptible(&vpfe->lock);

				if(device_type == TVP5150)
				{
					if (vpfe->std & V4L2_STD_625_50) {
						/* PAL 25fps */
						capparm->timeperframe.
						    numerator = 1;
						capparm->timeperframe.
						    denominator = 25;
					} else {
						/*NTSC 29.97fps */
						capparm->timeperframe.
						    numerator = 1001;
						capparm->timeperframe.
						    denominator = 30000;
					}
				} 
				else if (device_type == MT9T001 || device_type == MT9T031 ||
						device_type == MT9P031) {
					/* fill FPS value as per standard */
					if (vpfe->
					    std & V4L2_STD_MT9T001_VGA_30FPS) {
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_VGA_BIN_30FPS) {
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_VGA_60FPS) {
						capparm->timeperframe.
						    numerator =
						    FPS_60_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_60_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_SVGA_30FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_SVGA_60FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_60_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_60_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_XGA_30FPS) {
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_480p_30FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_480p_60FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_60_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_60_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_576p_25FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_25_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_25_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_576p_50FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_50_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_50_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_720p_24FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_24_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_24_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_720p_30FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_30_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_30_DENOMIRATOR;
					} else if (vpfe->
						   std &
						   V4L2_STD_MT9T001_1080p_18FPS)
					{
						capparm->timeperframe.
						    numerator =
						    FPS_18_NUMERATOR;
						capparm->timeperframe.
						    denominator =
						    FPS_18_DENOMIRATOR;
					}
				}

				/* Copy number of buffers allocated to arg
				 * passed
				 */
				capparm->readbuffers = vpfe->numbuffers;

				up(&vpfe->lock);
				dev_dbg(vpfe_dev,
					"\nEnd of VIDIOC_G_PARM ioctl");
			}
			break;
		}
	case VIDIOC_G_CTRL:
		dev_dbg(vpfe_dev, "\nStart of VIDIOC_G_CTRL ioctl");
		down_interruptible(&vpfe->lock);

		if (device_type == MT9T001 || device_type == MT9T031 ||device_type == MT9P031) 
		{
			/* Call device control function to get control value */
			ret = vpfe->config_dev_fxn(VIDIOC_G_CTRL, arg, vpfe->device_params);
		}
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_CTRL ioctl");
		break;
	case VIDIOC_S_CTRL:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_CTRL ioctl");
		down_interruptible(&vpfe->lock);

		if (device_type == MT9T001 || device_type == MT9T031 ||
				device_type == MT9P031) {
			/* Call device control function to configure control
			 * value
			 */
			struct v4l2_control *ctrl = arg;
			if (ctrl->id == V4L2_CID_BLACK_LEVEL)
			{
				ccdc_dcsub_update = ctrl->value;
			}
			else if (ctrl->id == V4L2_CID_MEDIAN)
			{
				ret = ccdc_median_thres((int)ctrl->value );

			}else{

				ret = vpfe->config_dev_fxn(VIDIOC_S_CTRL, arg,
						   vpfe->device_params);
			}
		}
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_CTRL ioctl");
		break;
	case VIDIOC_QUERYCTRL:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUERYCTRL ioctl");
		down_interruptible(&vpfe->lock);

		if (device_type == MT9T001 || device_type == MT9T031 ||
				device_type == MT9P031) {
			/* Call device control function to query about
			 * supported control commands
			 */
			ret = vpfe->config_dev_fxn(VIDIOC_QUERYCTRL, arg,
						   vpfe->device_params);
		}

		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QUERYCTRL ioctl");

		break;
	case VIDIOC_G_CROP:
		{
			struct v4l2_crop *crop = arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_G_CROP ioctl");
			if (crop->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				ret = -EINVAL;
			} else {
				crop->c = vpfe->vwin;
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_CROP ioctl");
			break;
		}
	case VIDIOC_S_CROP:
		{
			struct v4l2_crop *crop = arg;
			ccdc_imgwin *image_win;
			image_win = (device_type == TVP5150)
			    ? (&(vpfe->ccdc_params_ycbcr.win))
			    : (&(vpfe->ccdc_params_raw.win));
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_CROP ioctl");
			if (vpfe->started) {
				/* make sure streaming is not started */
				ret = -EBUSY;
				break;
			}
			/* adjust the width to 16 pixel boundry */
			crop->c.width = ((crop->c.width + 15) / 16) * 16;

			/* make sure parameters are valid */
			if (crop->type == V4L2_BUF_TYPE_VIDEO_CAPTURE
			    && (crop->c.left + crop->c.width
				<= vpfe->bounds.left + vpfe->bounds.width)
			    && (crop->c.top + crop->c.height
				<= vpfe->bounds.top + vpfe->bounds.height)) {

				down_interruptible(&vpfe->lock);
				vpfe->vwin = crop->c;
				*image_win = vpfe->vwin;
				up(&vpfe->lock);
			} else {
				ret = -EINVAL;
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_CROP ioctl");
			break;
		}
	case VIDIOC_QUERYSTD:
		{
			v4l2_std_id *id = (v4l2_std_id *) arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUERYSTD ioctl");

			if(device_type == TVP5150)
			{
				down_interruptible(&vpfe->lock);
				ret = sense_std(id);
				up(&vpfe->lock);
			}
			else if(device_type == TVP7002)
			{
				down_interruptible(&vpfe->lock);
				ret = sense_std_tvp7002(id);
				up(&vpfe->lock);
			}	
			else 
			{
				/* return -EINVAL indicating this ioctl
				 * is not supported
				 */
				printk("VIDIOC_QUERYSTD not used\n");
				ret = -EINVAL;
			}

			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QUERYSTD ioctl");
			break;
		}
	case VIDIOC_G_PRIORITY:
		{
			enum v4l2_priority *p = arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_G_PRIORITY ioctl");
			*p = v4l2_prio_max(&vpfe->prio);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_PRIORITY ioctl");
			break;
		}
	case VIDIOC_S_PRIORITY:
		{
			enum v4l2_priority *p = arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_PRIORITY ioctl");
			ret = v4l2_prio_change(&vpfe->prio, &fh->prio, *p);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_PRIORITY ioctl");
			break;
		}

	case VIDIOC_REQBUFS:
		//dev_dbg(vpfe_dev, "\nEnd of VIDIOC_REQBUFS ioctl");
		//printk("++++++++++++++++VIDIOC_REQBUFS\n");
		if (vpfe->io_usrs != 0) 
		{
			printk("EBUSY[VIDIOC_REQBUFS]\n");
			ret = -EBUSY;
			break;
		}
		down_interruptible(&vpfe->lock);
		//printk("AAAAAAAAAAAAAAAAAAAAAAAAA\n");
		videobuf_queue_init(&vpfe->bufqueue, &video_qops, NULL,
				    &vpfe->irqlock,
				    V4L2_BUF_TYPE_VIDEO_CAPTURE,
				    vpfe->field,
				    sizeof(struct videobuf_buffer), fh);
		//printk("BBBBBBBBBBBBBBBBBBBBBBBB\n");
		videobuf_set_buftype(&vpfe->bufqueue, VIDEOBUF_BUF_LINEAR);
		//printk("CCCCCCCCCCCCCCCCCCCCCCCCCC\n");
		fh->io_allowed = TRUE;
		vpfe->io_usrs = 1;
		INIT_LIST_HEAD(&vpfe->dma_queue);
		//printk("DDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
		ret = videobuf_reqbufs(&vpfe->bufqueue, arg);
		//printk("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEvideobuf_reqbufs : %d\n",ret);
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_REQBUFS ioctl");
		break;
	case VIDIOC_QUERYBUF:
		//dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUERYBUF ioctl");
		//printk("@@@@@@@@@@@@@@@ VIDIOC_QUERYBUF ioctl");
		ret = videobuf_querybuf(&vpfe->bufqueue, arg);
		//printk("@@@@@@@@@@@@@@@ videobuf_querybuf ret : %d\n",ret);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QUERYBUF ioctl");
		break;
	case VIDIOC_QBUF:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_QBUF ioctl");
		if (!fh->io_allowed) {
			dev_dbg(vpfe_dev, "\nfh->io_allowed");
			ret = -EACCES;
		} else {
			dev_dbg(vpfe_dev, "\n else of fh->io_allowed");
			ret = videobuf_qbuf(&vpfe->bufqueue, arg);
		}
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QBUF ioctl");
		break;

	case VIDIOC_DQBUF:
		//dev_dbg(vpfe_dev, "\nStarting VIDIOC_DQBUF ioctl");
		// printk("######### Kernel Starting VIDIOC_DQBUF ioctl");
		if (!fh->io_allowed)
		{
			printk("ERROR [in VIDIOC_DQBUF] -EACCES\n");	
			ret = -EACCES;
		}	
		else
		{
			//printk("###### Before videobuf_dqbuf()\n");	
			ret = videobuf_dqbuf(&vpfe->bufqueue, arg, 0);
			//printk("###### After videobuf_dqbuf()... ret = %d\n", ret);
		}	
		//dev_dbg(vpfe_dev, "\nEnd of VIDIOC_DQBUF ioctl");
		// printk("############ End of VIDIOC_DQBUF ioctl");
		break;

	case VIDIOC_STREAMON:
		//printk("Starting VIDIOC_STREAMON ioctl\n");
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_STREAMON ioctl");
		if (!fh->io_allowed) {
			ret = -EACCES;
			break;
		}
		if (vpfe->started) {
			ret = -EBUSY;
			break;
		}
		ret = videobuf_streamon(&vpfe->bufqueue);
		if (ret)
			break;

		down_interruptible(&vpfe->lock);
		/* get the current and next frame buffers */
		/* we expect at least one buffer is in driver at this point */
		/* if not, error is returned */
		if (list_empty(&vpfe->dma_queue)) {
			ret = -EIO;
			break;
		}
		dev_dbg(vpfe_dev, "cur frame %x.\n",
			(unsigned int)vpfe->dma_queue.next);
		vpfe->nextFrm = vpfe->curFrm =
		    list_entry(vpfe->dma_queue.next,
			       struct videobuf_buffer, queue);
		/* remove the buffer from the queue */
		list_del(&vpfe->curFrm->queue);
		vpfe->curFrm->state = STATE_ACTIVE;


		frm_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
		image_window = vpfe_device.ccdc_params_ycbcr.win;

		if (device_type == MT9M131) 	
		{
			/* sense the current video input standard */
			//tvp5150_ctrl(TVP5150_CONFIG, &vpfe->tvp5150_params);
			/* configure the ccdc and resizer as needed   */
			/* start capture by enabling CCDC and resizer */
			//printk("%%%%%% VIDIOC_STREAMON MT9M131 ccdc_config_ycbcr\n");
			ccdc_config_ycbcr_mt9m131(&vpfe->ccdc_params_ycbcr);
		}
		else if (device_type == TW2835) 	
		{
			/* sense the current video input standard */
			//tvp5150_ctrl(TVP5150_CONFIG, &vpfe->tvp5150_params);
			/* configure the ccdc and resizer as needed   */
			/* start capture by enabling CCDC and resizer */
			//printk("%%%%%% VIDIOC_STREAMON MT9M131 ccdc_config_ycbcr\n");
			ccdc_config_ycbcr_tw2835(&vpfe->ccdc_params_ycbcr);
			//ccdc_config_ycbcr(&vpfe->ccdc_params_ycbcr);
		}
		else 
		{
			/* configure the ccdc and resizer as needed   */
			/* start capture by enabling CCDC and resizer */
			//printk("ccdc_config_raw\n");
			ccdc_config_raw(&vpfe->ccdc_params_raw);
		}


		/* enable CCDC */
		vpfe->field_id = 0;
		vpfe->started = TRUE;
		vpfe->mode_changed = FALSE;
		vpfe->field_offset = (vpfe->vwin.height - 2) * vpfe->vwin.width;


		if((device_type != TVP5150)&&(device_type != TVP7002) && (vpfe->pixelfmt == V4L2_PIX_FMT_YUYV) &&(device_type != MT9M131))	
		//if((device_type != TVP5150) && vpfe->pixelfmt == V4L2_PIX_FMT_YUYV)		
		//if(vpfe->pixelfmt == V4L2_PIX_FMT_YUYV)	
		{
			//printk("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$999999999\n");
			if( device_type == MT9P031 )
			{
				struct ipipe_params *config = &ipipe_mt9p031_VGA_30fps;
				if( vpfe->std  == V4L2_STD_MT9T001_VGA_30FPS )
				{
					config = &ipipe_mt9p031_VGA_30fps;
					config->ipipe_hst = 2;
					config->ipipe_vst = 13;
				}
				else if( vpfe->std  == V4L2_STD_MT9T001_VGA_BIN_30FPS )
				{
					config = &ipipe_mt9p031_VGA_BIN_30fps;
					config->ipipe_hst = 2;
					config->ipipe_vst = 13;
				}
				else if( vpfe->std  == V4L2_STD_MT9T001_VGA_60FPS )
				{
					config = &ipipe_mt9p031_VGA_60fps;
				}
				else if( vpfe->std  == V4L2_STD_MT9T001_720p_30FPS )
				{
					config = &ipipe_mt9p031_720P;
					config->ipipe_hst = 8;//8;
					config->ipipe_vst = 1;//25;
				}
				config->ipipeif_param.clock_select = PIXCEL_CLK;
				config->ipipeif_param.source = CCDC;
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				config->ipipeif_param.gain	 = 0x200;
#else
				config->ipipeif_param.gain	 = 0x3ff;
#endif
				/* setup the second string, please set the command VPFE_CMD_S_EXT_RES correctly */
				set_ext_stream(&vpfe_stream_ext, config);

				/*
				config->ipipe_colpat_elep = RED;	  config->ipipe_colpat_elop = GREEN_RED;
				config->ipipe_colpat_olep = GREEN_BLUE;   config->ipipe_colpat_olop = BLUE;

				config->ipipe_colpat_elep = BLUE;	config->ipipe_colpat_elop = GREEN_BLUE;
				config->ipipe_colpat_olep = GREEN_RED;    config->ipipe_colpat_olop = RED;

				config->ipipe_colpat_elep = GREEN_RED; config->ipipe_colpat_elop = RED;
				config->ipipe_colpat_olep = BLUE;    config->ipipe_colpat_olop = GREEN_BLUE;

				config->ipipe_colpat_elep = GREEN_BLUE; config->ipipe_colpat_elop = BLUE;
				config->ipipe_colpat_olep = RED;    config->ipipe_colpat_olop = GREEN_RED;

				*/
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				ipipe_hw_setup(config);
			}
			else if(device_type == MT9T001)
			{
				struct ipipe_params *config = &ipipe_mt9t001_VGA_30fps;
				if( vpfe->std  == V4L2_STD_MT9T001_VGA_30FPS )
				{
					config = &ipipe_mt9t001_VGA_30fps;
				}
				else if( vpfe->std  == V4L2_STD_MT9T001_VGA_60FPS )
				{
					config = &ipipe_mt9t001_VGA_60fps;
				}
				else if( vpfe->std  == V4L2_STD_MT9T001_720p_30FPS )
				{
					config = &ipipe_mt9t001_720P;
				}
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				ipipe_hw_setup(config);
			}
			else if( device_type == TVP5150 )
			{
				//printk("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$8888888\n");
#if 0
				//struct ipipe_params *config = &ipipe_tvp5150_480P;
				struct ipipe_params *config = &ipipe_tvp5150_480P;
				//printk("vpfe->std : %d, VPFE_STD_AUTO : %d\n",vpfe->std,VPFE_STD_AUTO);
				//if( vpfe->std  == V4L2_STD_MT9T001_VGA_30FPS )
				//{
					config = &ipipe_tvp5150_480P;
					config->ipipe_hst = 2;
					config->ipipe_vst = 13;
				//}
	
				config->ipipeif_param.clock_select = PIXCEL_CLK;
				//config->ipipeif_param.clock_select = SDRAM_CLK;
				config->ipipeif_param.source = YUV2YUV;
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				config->ipipeif_param.gain	 = 0x200;
				//config->ipipeif_param.gain	 = 0x3ff;
#else
				config->ipipeif_param.gain	 = 0x3ff;
#endif
				/* setup the second string, please set the command VPFE_CMD_S_EXT_RES correctly */
				set_ext_stream(&vpfe_stream_ext, config);

				/*
				config->ipipe_colpat_elep = RED;	  config->ipipe_colpat_elop = GREEN_RED;
				config->ipipe_colpat_olep = GREEN_BLUE;   config->ipipe_colpat_olop = BLUE;

				config->ipipe_colpat_elep = BLUE;	config->ipipe_colpat_elop = GREEN_BLUE;
				config->ipipe_colpat_olep = GREEN_RED;    config->ipipe_colpat_olop = RED;

				config->ipipe_colpat_elep = GREEN_RED; config->ipipe_colpat_elop = RED;
				config->ipipe_colpat_olep = BLUE;    config->ipipe_colpat_olop = GREEN_BLUE;

				config->ipipe_colpat_elep = GREEN_BLUE; config->ipipe_colpat_elop = BLUE;
				config->ipipe_colpat_olep = RED;    config->ipipe_colpat_olop = GREEN_RED;

				*/
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				ipipe_hw_setup(config);
#endif
			}

			irq_setup_by_fmt(IRQ_USE_IPIPE);
			write_out_addr(0, (unsigned int)vpfe->nextFrm->boff);
			write_out_addr(1, ((unsigned int)vpfe->nextFrm->boff)
								+vpfe->vwin.width*vpfe->vwin.height*2+VPFE_EXT_HEADER_LEN);

			pResHeader = (Res_Set	*)phys_to_virt( ((unsigned long)vpfe->nextFrm->boff)
								+vpfe->vwin.width*vpfe->vwin.height*2);
			pResHeader->width  = vpfe_stream_ext.res_setting[0].width;
			pResHeader->height = vpfe_stream_ext.res_setting[0].height;
			pResHeader->index = vpfe_stream_ext.res_setting[0].index;
			rsz_cnt = 0;

			sdramop_enable(FALSE); //org
			//sdramop_enable(TRUE); //==> stop kernel error
			ccdc_enable(TRUE);     // org
			//ccdc_enable(FALSE); // screen not display
			
			/*set ipipe clock */
			regw_vpss(0x79, VPSS_CLK);
			regw_ip(0xff, IRQ_EN);
			rsz_enable(0, 1);
			rsz_enable(1, 1);
			regw_ip(1, IPIPE_EN);
			regw_if(1, IPIPEIF_ENABLE);


		}
		else
		{
			//printk("^^^^^^^^^^^^^^^^^^^^^^^^^^else9999\n");
			irq_setup_by_fmt(IRQ_USE_VD1);
			/* setup the memory address for the frame buffer */
			ccdc_setfbaddr(((unsigned long)(vpfe->curFrm->boff)));

			sdramop_enable(TRUE);
			ccdc_enable(TRUE);
		}



		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "started video streaming.\n");
		break;
	case VIDIOC_STREAMOFF:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_STREAMOFF ioctl");
		if (!fh->io_allowed) {
			ret = -EACCES;
			break;
		}
		if (!vpfe->started) {
			ret = -EINVAL;
			break;
		}

		down_interruptible(&vpfe->lock);

		/* disable CCDC */
		ccdc_enable(FALSE);
		sdramop_enable(FALSE);
		vpfe->started = FALSE;
		//if((device_type != TVP5150) &&	vpfe->pixelfmt == V4L2_PIX_FMT_YUYV)
		if(vpfe->pixelfmt == V4L2_PIX_FMT_YUYV)	
		{
			if( device_type == MT9P031 )
			{
				/* disable ipipe*/
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				irq_setup_by_fmt(IRQ_USE_FREE);
			}
			else if(device_type == TVP5150 )
			{
					/* disable ipipe*/
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				irq_setup_by_fmt(IRQ_USE_FREE);
			}	
			else if(device_type == MT9M131 )
			{
					/* disable ipipe*/
				rsz_enable(0, 0);
				rsz_enable(1, 0);
				regw_ip(0, IPIPE_EN);
				regw_if(0, IPIPEIF_ENABLE);

				irq_setup_by_fmt(IRQ_USE_FREE);
			}	
		}

		up(&vpfe->lock);
		ret = videobuf_streamoff(&vpfe->bufqueue);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_STREAMOFF ioctl");
		break;

	case VPFE_CMD_CONFIG_CCDC_YCBCR:
		/* this can be used directly and bypass the V4L2 APIs */
		{
			ccdc_params_ycbcr *params = &(vpfe->ccdc_params_ycbcr);

			if (vpfe->started) {
				/* only allowed if streaming is not started */
				ret = -EBUSY;
				break;
			}
			down_interruptible(&vpfe->lock);
			/* make sure the other v4l2 related fields
			   have consistant settings */
			*params = (*(ccdc_params_ycbcr *) arg);
			vpfe->vwin = params->win;
			if (params->buf_type == CCDC_BUFTYPE_FLD_INTERLEAVED) {
				vpfe->field = V4L2_FIELD_INTERLACED;
			} else if (params->buf_type ==
				   CCDC_BUFTYPE_FLD_SEPARATED) {
				vpfe->field = V4L2_FIELD_SEQ_TB;
			}
			if (params->pix_order == CCDC_PIXORDER_YCBYCR) {
				vpfe->pixelfmt = V4L2_PIX_FMT_YUYV;
			} else if (params->pix_order == CCDC_PIXORDER_CBYCRY) {
				vpfe->pixelfmt = V4L2_PIX_FMT_UYVY;
			}
			up(&vpfe->lock);
			break;
		}
	case VPFE_CMD_CONFIG_CCDC_RAW:
		/* This command is used to configure driver for CCDC Raw
		 * mode parameters
		 */
		{
                        if (validate_ccdc_param((ccdc_config_params_raw *) arg)
                            == -1) {
                                dev_err(vpfe_dev, "\nValidation of ccdc parameters failed \n");
                                return -EINVAL;
                        } else {
                                dev_err(vpfe_dev, "\n Validation pass\n");
                                memcpy(&vpfe->ccdc_params_raw,
                                       (ccdc_config_params_raw *) arg,
                                       sizeof(ccdc_config_params_raw));
                        }
                        break;
                }



	case VPFE_CMD_CONFIG_TVP7002:
			break;
	case VPFE_CMD_S_MT9T001_PARAMS:
		/* This command configures MT9T001 */
		{
			dev_dbg(vpfe_dev,
				"\nStarting VPFE_CMD_S_MT9T001_PARAMS ioctl");
			/* Call device control function to configure MT9T001 */
			ret = vpfe->config_dev_fxn(MT9T001_SET_PARAMS, arg,
						   vpfe->device_params);
			dev_dbg(vpfe_dev,
				"\nEnd VPFE_CMD_S_MT9T001_PARAMS ioctl");
			break;
		}
	case VPFE_CMD_G_MT9T001_PARAMS:
		/* This command returns MT9T001 configuration values to
		 * application.
		 */
		{
			dev_dbg(vpfe_dev,
				"\nStarting VPFE_CMD_G_MT9T001_PARAMS ioctl");
			/* Call device control function to get
			 * MT9T001 configuration values
			 */
			ret = vpfe->config_dev_fxn(MT9T001_GET_PARAMS, arg,
						   vpfe->device_params);
			dev_dbg(vpfe_dev,
				"\nEnd of VPFE_CMD_G_MT9T001_PARAMS ioctl");
			break;
		}
	case VPFE_CMD_S_EXT_RES:
	{
		if( device_type == MT9P031 || 
			device_type == MT9T001 || 
			device_type == TVP5150 || 
			device_type == MT9M131 || 
			device_type == TVP7002 ||
			device_type == TW2835)
		{
			stream_ext_params 	*params = (stream_ext_params *) arg;
			memcpy( &vpfe_stream_ext, params, sizeof(stream_ext_params) );
			//printk("\nparams->ext_res_num = %d \n", params->ext_res_num );
			//printk("params->res_setting[0].width = %d \n", params->res_setting[0].width );
			//printk("params->res_setting[0].height = %d \n", params->res_setting[0].height );
			//printk("params->res_setting[1].width = %d \n", params->res_setting[1].width );
			//printk("params->res_setting[1].height = %d \n", params->res_setting[1].height );
		}

		break;
	}

	default:
		dev_dbg(vpfe_dev, "\nDefault ioctl");
		ret = -ENOIOCTLCMD;
		break;
	}			/* end switch(cmd) */
	return ret;
}

/*
 * ======== vpfe_ioctl ========
 */
static int vpfe_ioctl(struct inode *inode, struct file *file,
		      unsigned int cmd, unsigned long arg)
{
	int ret;
	dev_dbg(vpfe_dev, "\nStarting of vpfe_ioctl...");
	ret = video_usercopy(inode, file, cmd, arg, vpfe_doioctl);
	dev_dbg(vpfe_dev, "\n(vpfe_ioctl) video_usercopy done ...");
	if ((ret >= 0) && (cmd == VIDIOC_S_FMT || cmd == VIDIOC_TRY_FMT)) {
		ret = video_usercopy(inode, file, VIDIOC_G_FMT,
				     arg, vpfe_doioctl);
	}
	dev_dbg(vpfe_dev, "\nEnd of vpfe_ioctl...");
	return ret;
}

/*
 * ======== vpfe_mmap ========
 */
static int vpfe_mmap(struct file *file, struct vm_area_struct *vma)
{
	dev_dbg(vpfe_dev, "\nStarting of vpfe_mmap...");
	return videobuf_mmap_mapper(&vpfe_device.bufqueue, vma);
}

/*
 * ======== vpfe_open ========
 */
 /* Creates a file handle for video object. Initialize itwith default value */
static int vpfe_open(struct inode *inode, struct file *filep)
{
	int minor = iminor(inode);
	vpfe_obj *vpfe = NULL;
	vpfe_fh *fh = NULL;

	dev_dbg(vpfe_dev, "vpfe: open minor=%d\n", minor);

	/* check to make sure the minor numbers match */
	if (vpfe_device.video_dev && vpfe_device.video_dev->minor == minor) {
		vpfe = &vpfe_device;
	} else {		/* device not found here */
		return -ENODEV;
	}

	/* allocate per filehandle data */
	if ((fh = kmalloc(sizeof(*fh), GFP_KERNEL)) == NULL) {
		return -ENOMEM;
	}
	filep->private_data = fh;
	fh->dev = vpfe;
	fh->io_allowed = FALSE;
	fh->prio = V4L2_PRIORITY_UNSET;
	v4l2_prio_open(&vpfe->prio, &fh->prio);
	vpfe->usrs++;
	dev_dbg(vpfe_dev, "\nvpfe_open done ...");
	return 0;
}

/*
 * ======== vpfe_release ========
 */
 /* This function disables the CCDC, Deletes the buffer queue and frees
  * the vpfe file handle.
  */
static int vpfe_release(struct inode *inode, struct file *filep)
{
	vpfe_fh *fh = filep->private_data;
	vpfe_obj *vpfe = fh->dev;
	dev_dbg(vpfe_dev, "\nStarting of vpfe_release...");
	down_interruptible(&vpfe->lock);
	if (fh->io_allowed) {
		vpfe->io_usrs = 0;
		ccdc_enable(FALSE);
		vpfe->started = FALSE;
		videobuf_queue_cancel(&vpfe->bufqueue);
		vpfe->numbuffers = VPFE_DEFNUM_FBUFS;
	}
	vpfe->usrs--;
	v4l2_prio_close(&vpfe->prio, &fh->prio);
	filep->private_data = NULL;
	if (fh != NULL)
		kfree(fh);
	up(&vpfe->lock);
	dev_dbg(vpfe_dev, "\nEnd of vpfe_release...");
	return 0;
}

static struct file_operations vpfe_fops = {
	.owner = THIS_MODULE,
	.open = vpfe_open,
	.release = vpfe_release,
	.ioctl = vpfe_ioctl,
	.mmap = vpfe_mmap
};

static struct video_device vpfe_video_template = {
	.name = "vpfe",
	.type = VID_TYPE_CAPTURE | VID_TYPE_CLIPPING | VID_TYPE_SCALES,
	.hardware = 0,
	.fops = &vpfe_fops,
	.minor = -1,
};

static void vpfe_platform_release(struct device *device)
{
	/* This is called when the reference count goes to zero. */
}

/*
 * ======== vpfe_probe ========
 */
 /* This function will allocate video device initialize and
  * registers the device
  */
static int __init vpfe_probe(struct device *device)
{
	struct video_device *vfd;
	vpfe_obj *vpfe = &vpfe_device;
	vpfe_dev = device;
	dev_dbg(vpfe_dev, "\nStarting of vpfe_probe...");
	/* alloc video device */
	if ((vfd = video_device_alloc()) == NULL) {
		printk("vpfe_probe error\n");
		return -ENOMEM;
	}
	//printk("vpfe_probe OK\n");
	*vfd = vpfe_video_template;
	vfd->dev = device;
	vfd->release = video_device_release;
	snprintf(vfd->name, sizeof(vfd->name),
		 "DM355_VPFE_DRIVER_V%d.%d.%d",
		 (VPFE_VERSION_CODE >> 16) & 0xff,
		 (VPFE_VERSION_CODE >> 8) & 0xff, (VPFE_VERSION_CODE) & 0xff);

	vpfe->video_dev = vfd;
	vpfe->usrs = 0;
	vpfe->io_usrs = 0;
	vpfe->started = FALSE;
	vpfe->latest_only = TRUE;

	v4l2_prio_init(&vpfe->prio);
	init_MUTEX(&vpfe->lock);

	/* register video device */
	dev_dbg(vpfe_dev, "trying to register vpfe device.\n");
	dev_dbg(vpfe_dev, "vpfe=%x,vpfe->video_dev=%x\n", (int)vpfe,
		(int)&vpfe->video_dev);
	//printk("vpfe=%x,vpfe->video_dev=%x\n", (int)vpfe,(int)&vpfe->video_dev);
	if (video_register_device(vpfe->video_dev, VFL_TYPE_GRABBER, -1) < 0) {
		printk("video_register_device error\n");
		video_device_release(vpfe->video_dev);
		vpfe->video_dev = NULL;
		return -1;
	}

	dev_dbg(vpfe_dev, "DM355 vpfe: driver version V%d.%d.%d loaded\n",
		(VPFE_VERSION_CODE >> 16) & 0xff,
		(VPFE_VERSION_CODE >> 8) & 0xff, (VPFE_VERSION_CODE) & 0xff);

	dev_dbg(vpfe_dev, "vpfe: registered device video%d\n",
		vpfe->video_dev->minor & 0x1f);

	/* all done */
	return 0;
}

/*
 * ======== vpfe_remove ========
 */
static int vpfe_remove(struct device *device)
{
	/* un-register device */
	dev_dbg(vpfe_dev, "\nUnregistering device...");
	video_unregister_device(vpfe_device.video_dev);

	return 0;
}

static struct device_driver vpfe_driver = {
	.name = "vpfe",
	.bus = &platform_bus_type,
	.probe = vpfe_probe,
	.remove = vpfe_remove,
};

static struct platform_device _vpfe_device = {
	.name = "vpfe",
	.id = 1,
	.dev = {
		.release = vpfe_platform_release,
		}
};
/*
* fmt = 	0: vd1
*		1: ipipe
*		2: free all
*/

void irq_setup_by_fmt(int fmt)
{
	static int irq_ipipe = 0;
	static int irq_vd1 = 0;
	int ret;

	//printk(" >>>>> fmt:%d, <<<<<\n", fmt);
	if( fmt == 0 )
	{
		if( !irq_vd1 )
		{
			ret = request_irq(IRQ_VDINT1, vdint1_isr, SA_INTERRUPT,
				  "dm355v4l2", (void *)&vpfe_device);
			irq_vd1 = 1;
		}
		if( irq_ipipe )
		{
			free_irq(IRQ_IPIPE_SDR, &vpfe_device);
			irq_ipipe = 0;
		}

	}else if( fmt == 1 ){
		if( !irq_ipipe )
		{
			ret = request_irq(IRQ_IPIPE_SDR, v4l2Ipipe_isr, SA_SHIRQ/*SA_INTERRUPT*/,
				  "dm355v4l2_ipipe", (void *)&vpfe_device);
			irq_ipipe = 1;
		}
		if( irq_vd1 )
		{
			free_irq(IRQ_VDINT1, &vpfe_device);
			irq_vd1 = 0;
		}
	}else if( fmt == 2 ){
		if( irq_vd1 )
		{
			free_irq(IRQ_VDINT1, &vpfe_device);
			irq_vd1 = 0;
		}
		if( irq_ipipe )
		{
			free_irq(IRQ_IPIPE_SDR, &vpfe_device);
			irq_ipipe = 0;
		}
	}

}

/*
 * ======== vpfe_init ========
 */
 /*This function allocates free pages and register the driver. Then reset the
  * CCDC and configure capture device with default parameters
  */
static int vpfe_init(void)
{
	int i = 0;
	int fbuf_size;
	ccdc_frmfmt frame_format;
	void *mem;
	int ret = 0;

        /*PINMUX0- routing the Video signals to VPFE */
        ret = __REG(0x01c40000);
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
        //ret |= 0x7f50; // org code
        //ret |= 0x4750; // test code VD, HD 신호를 GPIO로 설정해버린다.
        ret |= 0x7f55;
        //printk("PINMUX0 : 0x7f50 IPNC\n");
#else
        ret |= 0x7f55;
        //printk("PINMUX0 : 0x7f55\n");
#endif
        __REG(0x01c40000) = ret;

	if (device_type == TVP5150)
	{
		//printk("device_type == TVP5150\n");
		fbuf_size = VPFE_TVP5150_MAX_FBUF_SIZE;	
		vpfe_device = vpfe_device_ycbcr;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
	}
	else if (device_type == TVP7002) 
	{
		fbuf_size = VPFE_TVP7002_MAX_FBUF_SIZE;
		vpfe_device = vpfe_device_ycbcr_tvp7002;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
	}

	else if (device_type == MT9T001 || device_type == MT9T031 ||device_type == MT9P031) 
	{
		//printk("device_type == MT9P031\n");
		fbuf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
		vpfe_device = vpfe_device_raw;
		frame_format = vpfe_device.ccdc_params_raw.frm_fmt;
	}
	else if (device_type == MT9M131) 
	{
		//printk("#### vpfe_device_ycbcr_mt9m131 == MT9M131\n");
		fbuf_size = VPFE_MT9M131_MAX_FBUF_SIZE;	
		vpfe_device = vpfe_device_ycbcr_mt9m131;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
		
	}
	else if (device_type == TW2835) 
	{
		printk("[vpfe_device_ycbcr_tw2835]\n");
		fbuf_size = VPFE_TW2835_MAX_FBUF_SIZE;	
		vpfe_device = vpfe_device_ycbcr_tw2835;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
		
	}
	else 
	{
		return -1;
	}

	
	/* allocate memory at initialization time to guarentee availability */
	for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) 
	{
		mem = (void *)__get_free_pages(GFP_KERNEL | GFP_DMA, get_order(fbuf_size));
		if (mem) 
		{
			unsigned long adr = (unsigned long)mem;
			u32 size = PAGE_SIZE << (get_order(fbuf_size));
			//printk("mem > 0\n");
			while (size > 0) 
			{
				/* make sure the frame buffers
				   are never swapped out of memory */
				SetPageReserved(virt_to_page(adr));
				adr += PAGE_SIZE;
				size -= PAGE_SIZE;
			}
			vpfe_device.fbuffers[i] = (u8 *) mem;
		}
		else
		{
			//printk("mem < 0\n");
			while (--i >= 0) {
				free_reserved_pages((unsigned long)
						    vpfe_device.
						    fbuffers[i], fbuf_size);
			}
			dev_err(vpfe_dev, "frame buffer memory allocation failed.\n");
			return -ENOMEM;
		}
	}
	//printk("Before driver_register \n");
	if (driver_register(&vpfe_driver) != 0) 
	{
		printk("vpfe_driver driver registration failed\n");
		dev_err(vpfe_dev, "driver registration failed\n");
		return -1;
	}
	//printk("After driver_register OK \n");
	if (platform_device_register(&_vpfe_device) != 0) 
	{
		printk("_vpfe_device platform_device_register failed\n");
		driver_unregister(&vpfe_driver);
		dev_err(vpfe_dev, "device registration failed\n");
		return -1;
	}
	//printk("################## After platform_device_register OK \n");
	ccdc_reset();


	if(device_type == MT9T001 || device_type == MT9T031) 
	{
		vpfe_device.config_dev_fxn = mt9t001_ctrl;
		/* enable the i2c switch on the MT9T031 head board */
		if(device_type == MT9T031)
			vpfe_device.config_dev_fxn(MT9T001_ENABLE_I2C_SWITCH,
						NULL, NULL);
		ret =  vpfe_device.config_dev_fxn(MT9T001_INIT,&vpfe_device.std,&vpfe_device.device_params);
	} 
	else if (device_type == MT9M131) 
	{
		vpfe_device.config_dev_fxn = mt9m131_ctrl;
		// enable the i2c switch on the MT9M131 head board 
		
		//printk("========== Before MT9M131_INIT  ==========\n");
		ret =  vpfe_device.config_dev_fxn(MT9M131_INIT,&vpfe_device.std,&vpfe_device.device_params);

		//printk("========== MT9M131 Configured ==========\n");
		
	}
	else if (device_type == MT9P031) 
	{
		vpfe_device.config_dev_fxn = mt9p031_ctrl;
		//ret = vpfe_device.config_dev_fxn(MT9T001_INIT,&vpfe_device.std,&vpfe_device.device_params);
		ret = vpfe_device.config_dev_fxn(MT9P031_INIT,&vpfe_device.std,&vpfe_device.device_params);
	}
	// tw2835 not controlled in local
	
	if (ret < 0) {
		platform_device_unregister(&_vpfe_device);
		driver_unregister(&vpfe_driver);
		/* Free memory for all image buffers */
		for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) {
			free_reserved_pages((unsigned long)
					    vpfe_device.fbuffers[i], fbuf_size);
		}
		return -1;
	}

	/* setup interrupt handling */
	/* request VDINT1 if progressive format */
	if (frame_format == CCDC_FRMFMT_PROGRESSIVE) 
	//if (0) 	
	{
		//printk("########### INTERRUPT CCDC_FRMFMT_PROGRESSIVE\n");
		if((device_type != TVP5150)&&(device_type != TVP7002) && vpfe_device.pixelfmt == V4L2_PIX_FMT_YUYV)	
		{
			//printk("########### irq_setup_by_fmt IRQ_USE_IPIPE\n");
			irq_setup_by_fmt(IRQ_USE_IPIPE);
		}
		
		else
		{
			//printk("###########  irq_setup_by_fmt IRQ_USE_VD1\n");
			irq_setup_by_fmt(IRQ_USE_VD1);
		}
		if (ret < 0) 
		{
			//printk("@@@@@@ CCDC_FRMFMT_PROGRESSIVE ret < 0\n");
			platform_device_unregister(&_vpfe_device);
			driver_unregister(&vpfe_driver);
			/* Free memory for all image buffers */
			for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) 
			{
				free_reserved_pages((unsigned long)
				vpfe_device.fbuffers[i], fbuf_size);
			}
			return -1;
		}
	}
		
	ret = request_irq(IRQ_VDINT0, vpfe_isr, SA_INTERRUPT,"dm355v4l2", (void *)&vpfe_device);
	if (ret < 0) 
	{
		//printk("Request_irq Error in VPFE_INIT\n");
		platform_device_unregister(&_vpfe_device);
		driver_unregister(&vpfe_driver);
		/* Free memory for all image buffers */
		for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) {
			free_reserved_pages((unsigned long)
					    vpfe_device.fbuffers[i], fbuf_size);
		}
		free_irq(IRQ_VDINT1, &vpfe_device);
		return -1;
	}

	dev_err(vpfe_dev, "DaVinci v4l2 capture driver V1.0 loaded\n");
	return 0;
}

/*
 * ======== vpfe_cleanup ========
 */
 /*This function unregisters the driver and free the allocated pages */
static void vpfe_cleanup(void)
{
	int i = vpfe_device.numbuffers;
	int buf_size;
	ccdc_frmfmt frame_format;
//	unsigned int *fpc_physaddr = NULL, *fpc_virtaddr = NULL;

        if(device_type == MT9M131)
	{
		buf_size = VPFE_MT9M131_MAX_FBUF_SIZE;
		/* Free mt9t001 object memory */
		vpfe_device.config_dev_fxn(MT9M131_CLEANUP, NULL,
					   vpfe_device.device_params);
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;	
	}	 
        else if(device_type == TW2835)
	{
		buf_size = VPFE_TW2835_MAX_FBUF_SIZE;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;	
	}	 
	else 
	{
		buf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
		/* Free mt9t001 object memory */
		vpfe_device.config_dev_fxn(MT9T001_CLEANUP, NULL,
					   vpfe_device.device_params);
		frame_format = vpfe_device.ccdc_params_raw.frm_fmt;
	}

	platform_device_unregister(&_vpfe_device);
	driver_unregister(&vpfe_driver);
	/* disable interrupt */
	free_irq(IRQ_VDINT0, &vpfe_device);
	/* Free VDINT1 if progressive format */
	if (frame_format == CCDC_FRMFMT_PROGRESSIVE)
	{
		free_irq(IRQ_VDINT1, &vpfe_device);
		irq_setup_by_fmt(IRQ_USE_FREE);
	}

	/* Free memory for all image buffers */
	while (--i >= 0) {
		free_reserved_pages((unsigned long)vpfe_device.
				    fbuffers[i], buf_size);
	}
}

module_init(vpfe_init);
module_exit(vpfe_cleanup);
