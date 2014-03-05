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
/* davinci_vpfe.c */

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

#include <media/davinci_vpfe.h>

MODULE_LICENSE("GPL");

/* Global variable for insmode command which will decide that either MT9T001
 * (device =0)  or TVP5146 (device = 1) is the device
 */
static int device_type = TVP5146;

module_param(device_type, int, 0);

static struct v4l2_rect ntsc_bounds = VPFE_WIN_NTSC;
static struct v4l2_rect pal_bounds = VPFE_WIN_PAL;
static struct v4l2_fract ntsc_aspect = VPFE_PIXELASPECT_NTSC;
static struct v4l2_fract pal_aspect = VPFE_PIXELASPECT_PAL;
static struct v4l2_rect ntscsp_bounds = VPFE_WIN_NTSC_SP;
static struct v4l2_rect palsp_bounds = VPFE_WIN_PAL_SP;
static struct v4l2_fract sp_aspect = VPFE_PIXELASPECT_NTSC_SP;

static struct v4l2_rect VGA_bounds = VPFE_WIN_VGA;
static struct v4l2_rect SVGA_bounds = VPFE_WIN_SVGA;
static struct v4l2_rect XGA_bounds = VPFE_WIN_XGA;
static struct v4l2_rect P480_bounds = VPFE_WIN_480p;
static struct v4l2_rect P576_bounds = VPFE_WIN_576p;
static struct v4l2_rect P720_bounds = VPFE_WIN_720p;
static struct v4l2_rect P1080_bounds = VPFE_WIN_1080p;
static struct v4l2_fract default_aspect = VPFE_PIXELASPECT_DEFAULT;

static vpfe_obj vpfe_device_ycbcr = {	/* the default format is NTSC */
	.usrs = 0,
	.io_usrs = 0,
	.std = VPFE_STD_AUTO,
	.vwin = VPFE_WIN_PAL,
	.bounds = VPFE_WIN_PAL,
	.pixelaspect = VPFE_PIXELASPECT_NTSC,
	.pixelfmt = V4L2_PIX_FMT_UYVY,
	.field = V4L2_FIELD_INTERLACED,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = TVP5146,
	.ccdc_params_ycbcr = {
			      .pix_fmt = CCDC_PIXFMT_YCBCR_8BIT,
			      .frm_fmt = CCDC_FRMFMT_INTERLACED,
			      .win = VPFE_WIN_PAL,
			      .fid_pol = CCDC_PINPOL_POSITIVE,
			      .vd_pol = CCDC_PINPOL_POSITIVE,
			      .hd_pol = CCDC_PINPOL_POSITIVE,
			      .bt656_enable = TRUE,
			      .pix_order = CCDC_PIXORDER_CBYCRY,
			      .buf_type = CCDC_BUFTYPE_FLD_INTERLEAVED},

	.ccdc_params_raw = {0},
	.tvp5146_params = {
			   .mode = TVP5146_MODE_AUTO,
			   .amuxmode = TVP5146_AMUX_COMPOSITE,
			   .enablebt656sync = TRUE,
			   .data_width = TVP5146_WIDTH_8BIT},
	.irqlock = SPIN_LOCK_UNLOCKED
};

static vpfe_obj vpfe_device_raw = {	/*Default is vga mode */
	.usrs = 0,
	.io_usrs = 0,
	.std = V4L2_STD_MT9T001_VGA_30FPS,
	.vwin = VPFE_WIN_VGA,
	.bounds = VPFE_WIN_VGA,
	.pixelaspect = VPFE_PIXELASPECT_DEFAULT,
	.pixelfmt = V4L2_PIX_FMT_SBGGR8,
	.field = V4L2_FIELD_NONE,
	.numbuffers = VPFE_DEFNUM_FBUFS,
	.capture_device = MT9T001,
	.ccdc_params_raw = {
			    .pix_fmt = CCDC_PIXFMT_RAW,
			    .frm_fmt = CCDC_FRMFMT_PROGRESSIVE,
			    .win = VPFE_WIN_PAL,
			    .fid_pol = CCDC_PINPOL_POSITIVE,
			    .vd_pol = CCDC_PINPOL_POSITIVE,
			    .hd_pol = CCDC_PINPOL_POSITIVE,
			    .image_invert_enable = FALSE,
			    .data_sz = _10BITS,
			    .alaw = {
				     .b_alaw_enable = FALSE},
			    .blk_clamp = {
					  .b_clamp_enable = FALSE,
					  .dc_sub = 0},
			    .blk_comp = {0, 0, 0, 0},
			    .fault_pxl = {
					  .fpc_enable = FALSE},
			    },
	.ccdc_params_ycbcr = {0},
	.irqlock = SPIN_LOCK_UNLOCKED
};

static ccdc_frmfmt frm_format;
static ccdc_imgwin image_window;
static vpfe_obj vpfe_device = { 0 };
struct device *vpfe_dev;

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
/*This function will get current input standard for TVP5146*/
static int sense_std(v4l2_std_id * std_id)
{
	v4l2_std_id id = 0;
	tvp5146_mode mode;
	int ret;
	dev_dbg(vpfe_dev, "\nStarting Davinci_vpfe sense_std...");
	ret = tvp5146_ctrl(TVP5146_GET_STD, &mode);
	if (ret < 0)
		return ret;
	switch (mode & 0x7) {
	case TVP5146_MODE_NTSC:
		id = V4L2_STD_NTSC;
		break;
	case TVP5146_MODE_PAL:
		id = V4L2_STD_PAL;
		break;
	case TVP5146_MODE_PAL_M:
		id = V4L2_STD_PAL_M;
		break;
	case TVP5146_MODE_PAL_CN:
		id = V4L2_STD_PAL_N;
		break;
	case TVP5146_MODE_SECAM:
		id = V4L2_STD_SECAM;
		break;
	case TVP5146_MODE_PAL_60:
		id = V4L2_STD_PAL_60;
		break;
	}
	if (mode & 0x8) {	/* square pixel mode */
		id <<= 32;
	}
	if (mode == TVP5146_MODE_AUTO) {
		id = VPFE_STD_AUTO;	/* auto-detection for all other modes */
	} else if (mode == TVP5146_MODE_AUTO_SQP) {
		id = VPFE_STD_AUTO_SQP;
	}
	if (id == 0)
		return -EINVAL;
	*std_id = id;
	dev_dbg(vpfe_dev, "\nEnd of Davinci_vpfe sense_std...");
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
	unsigned long jiffies_time = get_jiffies_64();
	struct timeval timevalue;
	int val = 0;

	val = ccdc_sbl_reset();

	/*Convert time representations between jiffies and struct timeval */
	jiffies_to_timeval(jiffies_time, &timevalue);

	dev_dbg(vpfe_dev, "\nStarting Davinci_vpfe\vpfe_isr...");
	if (frm_format == CCDC_FRMFMT_INTERLACED) {
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
	} else if (frm_format == CCDC_FRMFMT_PROGRESSIVE) {

		dev_dbg(vpfe_dev, "\nframe format is progressive...");
		if (vpfe->curFrm != vpfe->nextFrm) {
			/* Copy frame capture time value in curFrm->ts */
			vpfe->curFrm->ts = timevalue;
			vpfe->curFrm->state = STATE_DONE;
			wake_up_interruptible(&vpfe->curFrm->done);
			vpfe->curFrm = vpfe->nextFrm;
		}

	}
	dev_dbg(vpfe_dev, "interrupt returned.\n");
	return IRQ_RETVAL(1);
}

static irqreturn_t vdint1_isr(int irq, void *dev_id, struct pt_regs *regs)
{

	vpfe_obj *vpfe = &vpfe_device;

	dev_dbg(vpfe_dev, "\nInside vdint1_isr...");

	if (frm_format == CCDC_FRMFMT_PROGRESSIVE) {
		if (!list_empty(&vpfe->dma_queue)
		    && vpfe->curFrm == vpfe->nextFrm) {
			vpfe->nextFrm =
			    list_entry(vpfe->dma_queue.next,
				       struct videobuf_buffer, queue);
			list_del(&vpfe->nextFrm->queue);
			vpfe->nextFrm->state = STATE_ACTIVE;
			ccdc_setfbaddr((unsigned long)vpfe->nextFrm->boff);
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
	if (device_type == TVP5146) {
		buf_size = VPFE_TVP5146_MAX_FBUF_SIZE;
	} else {
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
	if (device_type == TVP5146) {
		*size = buf_size = VPFE_TVP5146_MAX_FBUF_SIZE;
	} else {
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
	if (device_type == TVP5146) {
		buf_size = VPFE_TVP5146_MAX_FBUF_SIZE;
	} else {
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

	vb->state = STATE_NEEDS_INIT;
	dev_dbg(vpfe_dev, "\nEnd of buffer_release");
}

static struct videobuf_queue_ops video_qops = {
	.buf_setup = buffer_setup,
	.buf_prepare = buffer_prepare,
	.buf_queue = buffer_queue,
	.buf_release = buffer_release,
	.buf_config = buffer_config,
};

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
	}

	switch (cmd) {
	case VIDIOC_QUERYCAP:
		{
			struct v4l2_capability *cap =
			    (struct v4l2_capability *)arg;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUEYCAP ioctl");
			memset(cap, 0, sizeof(*cap));
			*cap = vpfe_drvcap;
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_QUEYCAP ioctl");
			break;
		}
	case VIDIOC_ENUM_FMT:
		{
			struct v4l2_fmtdesc *fmt = (struct v4l2_fmtdesc *)arg;
			u32 index = fmt->index;
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_ENUM_FMT ioctl");
			memset(fmt, 0, sizeof(*fmt));

			fmt->index = index;
			if (device_type == TVP5146) {
				if (index == 0) {
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
			} else if (device_type == MT9T001) {
				if (index == 0) {
					/* only Bayer Raw Mode format is 
					 * supported at this point 
					 */
					fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
					strcpy(fmt->description,
					       "Raw Mode -Bayer Pattern GrRBGb");
					fmt->pixelformat = V4L2_PIX_FMT_SBGGR8;
				} else {
					ret = -EINVAL;
				}
			}
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_ENUM_FMT ioctl");

			break;
		}
	case VIDIOC_G_FMT:
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
				if (device_type == TVP5146) {
					pixfmt->bytesperline =
					    pixfmt->width * 2;
				} else {
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

			if (device_type == TVP5146) {
				img_win = &(vpfe->ccdc_params_ycbcr.win);
				buf_type = &(vpfe->ccdc_params_ycbcr.buf_type);
				frame_format =
				    &(vpfe->ccdc_params_ycbcr.frm_fmt);
			} else {
				img_win = &(vpfe->ccdc_params_raw.win);
				buf_type = &(vpfe->ccdc_params_raw.buf_type);
				frame_format = &(vpfe->ccdc_params_raw.frm_fmt);

			}

			dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_FMT ioctl");
			if (vpfe->started) {
				/* make sure streaming is not started */
				ret = -EBUSY;
				break;
			}

			down_interruptible(&vpfe->lock);
			dev_dbg(vpfe_dev, "\nAfter down_interruptible");
			if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				ret = -EINVAL;
				up(&vpfe->lock);
				break;
			}

			if ((pixfmt->width + vpfe->vwin.left <=
			     vpfe->bounds.width)
			    && (pixfmt->height + vpfe->vwin.top <=
				vpfe->bounds.height)) {
				/* this is the case when no scaling is 
				 * supported 
				 */
				/* crop window is directed modified */
				vpfe->vwin.height = pixfmt->height;
				vpfe->vwin.width = pixfmt->width;
				img_win->width = pixfmt->width;
				img_win->height = pixfmt->height;
			} else {
				ret = -EINVAL;
				up(&vpfe->lock);
				break;
			}

			/* setup the CCDC parameters accordingly */
			if (device_type == TVP5146) {

				if (pixfmt->pixelformat == V4L2_PIX_FMT_YUYV) {
					vpfe->ccdc_params_ycbcr.pix_order =
					    CCDC_PIXORDER_YCBYCR;
					vpfe->pixelfmt = pixfmt->pixelformat;
				} else if (pixfmt->pixelformat ==
					   V4L2_PIX_FMT_UYVY) {
					vpfe->ccdc_params_ycbcr.pix_order =
					    CCDC_PIXORDER_CBYCRY;
					vpfe->pixelfmt = pixfmt->pixelformat;
				} else {
					/* not supported format */
					ret = -EINVAL;
					up(&vpfe->lock);
					break;
				}
			} else if (device_type == MT9T001) {

				if (pixfmt->pixelformat == V4L2_PIX_FMT_SBGGR8) {
					vpfe->pixelfmt = pixfmt->pixelformat;
				} else {
					/* not supported format */
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
				ret = -EINVAL;
			}

			up(&vpfe->lock);
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_FMT ioctl");
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

				if (device_type == TVP5146) {
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
				} else if (device_type == MT9T001) {
					if (pixfmt->width >
					    vpfe->bounds.width
					    || pixfmt->height >
					    vpfe->bounds.height
					    || (pixfmt->pixelformat !=
						V4L2_PIX_FMT_SBGGR8)) {
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
			dev_dbg(vpfe_dev, "\nStarting VIDIOC_G_FMT ioctl");
			*id = vpfe->std;
			dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_FMT ioctl");
			break;
		}
	case VIDIOC_S_STD:
		{
			v4l2_std_id id = *(v4l2_std_id *) arg;
			tvp5146_mode mode = TVP5146_MODE_INV;
			int sqp = 0;
			dev_dbg(vpfe_dev, "\nStarting of VIDIOC_S_STD ioctl");
			/* make sure streaming is not started */
			if (vpfe->started) {
				ret = -EBUSY;
				break;
			}
			down_interruptible(&vpfe->lock);

			if (device_type == TVP5146) {
				if (id & V4L2_STD_625_50) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = pal_bounds;
					vpfe->pixelaspect = pal_aspect;
					vpfe->ccdc_params_ycbcr.win =
					    pal_bounds;

				} else if (id & V4L2_STD_525_60) {
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
					mode = TVP5146_MODE_AUTO;
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
					mode = TVP5146_MODE_AUTO_SQP;
					vpfe->pixelaspect = sp_aspect;
				} else {
					ret = -EINVAL;
				}
				if (id == V4L2_STD_PAL_60) {
					mode = TVP5146_MODE_PAL_60;
				} else if (id == V4L2_STD_PAL_M) {
					mode = TVP5146_MODE_PAL_M;
				} else if (id == V4L2_STD_PAL_Nc
					   || id == V4L2_STD_PAL_N) {
					mode = TVP5146_MODE_PAL_CN;
				} else if (id & V4L2_STD_PAL) {
					mode = TVP5146_MODE_PAL;
				} else if (id & V4L2_STD_NTSC) {
					mode = TVP5146_MODE_NTSC;
				} else if (id & V4L2_STD_SECAM) {
					mode = TVP5146_MODE_SECAM;
				}
				vpfe->tvp5146_params.mode = mode | (sqp << 3);
				tvp5146_ctrl(TVP5146_CONFIG,
					     &vpfe->tvp5146_params);
			} else if (device_type == MT9T001) {
				/* Store image window paramters and pixel 
				 * aspect values as per standard 
				 * passed by application 
				 */
				if ((id == V4L2_STD_MT9T001_VGA_30FPS)
				    || (id == V4L2_STD_MT9T001_VGA_60FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = VGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = VGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_SVGA_30FPS)
					   || (id ==
					       V4L2_STD_MT9T001_SVGA_60FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = SVGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = SVGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_XGA_30FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = XGA_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = XGA_bounds;
				} else if ((id == V4L2_STD_MT9T001_480p_30FPS)
					   || (id ==
					       V4L2_STD_MT9T001_480p_60FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P480_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P480_bounds;
				} else if ((id == V4L2_STD_MT9T001_576p_25FPS)
					   || (id ==
					       V4L2_STD_MT9T001_576p_50FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P576_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P576_bounds;
				} else if ((id == V4L2_STD_MT9T001_720p_24FPS)
					   || (id ==
					       V4L2_STD_MT9T001_720p_30FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin = P720_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win = P720_bounds;
				} else if ((id == V4L2_STD_MT9T001_1080p_18FPS)) {
					vpfe->std = id;
					vpfe->bounds = vpfe->vwin =
					    P1080_bounds;
					vpfe->pixelaspect = default_aspect;
					vpfe->ccdc_params_raw.win =
					    P1080_bounds;
				} else {
					ret = -EINVAL;
				}
				if (ret != -EINVAL) {
					/* Call device control function to 
					 * configure video standard 
					 */
					ret =
					    vpfe->
					    config_dev_fxn(MT9T001_SET_STD,
							   &vpfe->std,
							   vpfe->device_params);
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

			if (device_type == TVP5146) {
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
			} else if (device_type == MT9T001) {
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
				} else {
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
			if (device_type != TVP5146) {
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
			if (device_type != TVP5146) {
				return -1;
			}
			*index = vpfe->tvp5146_params.amuxmode;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_G_INPUT ioctl");
			break;
		}
	case VIDIOC_S_INPUT:
		{
			int *index = (int *)arg;
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_S_INPUT ioctl");
			if (device_type != TVP5146) {
				return -1;
			}
			if (*index > 1 || *index < 0) {
				ret = -EINVAL;
			}
			vpfe->tvp5146_params.amuxmode = *index;
			tvp5146_ctrl(TVP5146_SET_AMUXMODE, index);
			dev_dbg(vpfe_dev, "\nStart of VIDIOC_S_INPUT ioctl");
			break;
		}
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
			if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				/* only capture is supported */
				ret = -EINVAL;
			} else {
				struct v4l2_captureparm *capparm =
				    &parm->parm.capture;
				memset(capparm, 0,
				       sizeof(struct v4l2_captureparm));
				down_interruptible(&vpfe->lock);

				if (device_type == TVP5146) {
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
				} else if (device_type == MT9T001) {
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

		if (device_type == TVP5146) {
			tvp5146_ctrl(VIDIOC_G_CTRL, arg);
		} else if (device_type == MT9T001) {
			/* Call device control function to get control value */
			ret = vpfe->config_dev_fxn(VIDIOC_G_CTRL, arg,
						   vpfe->device_params);
		}
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_G_CTRL ioctl");
		break;
	case VIDIOC_S_CTRL:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_S_CTRL ioctl");
		down_interruptible(&vpfe->lock);

		if (device_type == TVP5146) {
			ret = tvp5146_ctrl(VIDIOC_S_CTRL, arg);
		} else if (device_type == MT9T001) {
			/* Call device control function to configure control 
			 * value 
			 */
			ret = vpfe->config_dev_fxn(VIDIOC_S_CTRL, arg,
						   vpfe->device_params);
		}
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_S_CTRL ioctl");
		break;
	case VIDIOC_QUERYCTRL:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUERYCTRL ioctl");
		down_interruptible(&vpfe->lock);

		if (device_type == TVP5146) {
			tvp5146_ctrl(VIDIOC_QUERYCTRL, arg);
		} else if (device_type == MT9T001) {
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
			image_win = (device_type == TVP5146)
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

			if (device_type == TVP5146) {
				down_interruptible(&vpfe->lock);
				ret = sense_std(id);
				up(&vpfe->lock);
			} else {
				/* return -EINVAL indicating this ioctl 
				 * is not supported 
				 */
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
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_REQBUFS ioctl");
		if (vpfe->io_usrs != 0) {
			ret = -EBUSY;
			break;
		}
		down_interruptible(&vpfe->lock);
		videobuf_queue_init(&vpfe->bufqueue, &video_qops, NULL,
				    &vpfe->irqlock,
				    V4L2_BUF_TYPE_VIDEO_CAPTURE,
				    vpfe->field,
				    sizeof(struct videobuf_buffer), fh);

		videobuf_set_buftype(&vpfe->bufqueue, VIDEOBUF_BUF_LINEAR);

		fh->io_allowed = TRUE;
		vpfe->io_usrs = 1;
		INIT_LIST_HEAD(&vpfe->dma_queue);
		ret = videobuf_reqbufs(&vpfe->bufqueue, arg);
		up(&vpfe->lock);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_REQBUFS ioctl");
		break;
	case VIDIOC_QUERYBUF:
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_QUERYBUF ioctl");
		ret = videobuf_querybuf(&vpfe->bufqueue, arg);
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
		dev_dbg(vpfe_dev, "\nStarting VIDIOC_DQBUF ioctl");
		if (!fh->io_allowed)
			ret = -EACCES;
		else
			ret = videobuf_dqbuf(&vpfe->bufqueue, arg, 0);
		dev_dbg(vpfe_dev, "\nEnd of VIDIOC_DQBUF ioctl");
		break;

	case VIDIOC_STREAMON:
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

		if (device_type == TVP5146) {
			/* sense the current video input standard */
			tvp5146_ctrl(TVP5146_CONFIG, &vpfe->tvp5146_params);
			frm_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
			image_window = vpfe_device.ccdc_params_ycbcr.win;
			/* configure the ccdc and resizer as needed   */
			/* start capture by enabling CCDC and resizer */
			ccdc_config_ycbcr(&vpfe->ccdc_params_ycbcr);
		} else {
			frm_format = vpfe_device.ccdc_params_raw.frm_fmt;
			image_window = vpfe_device.ccdc_params_raw.win;
			/* configure the ccdc and resizer as needed   */
			/* start capture by enabling CCDC and resizer */
			ccdc_config_raw(&vpfe->ccdc_params_raw);
		}

		/* setup the memory address for the frame buffer */
		ccdc_setfbaddr(((unsigned long)(vpfe->curFrm->boff)));
		/* enable CCDC */
		vpfe->field_id = 0;
		vpfe->started = TRUE;
		vpfe->mode_changed = FALSE;
		vpfe->field_offset = (vpfe->vwin.height - 2) * vpfe->vwin.width;
		ccdc_enable(TRUE);
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
		/* disable CCDC */
		down_interruptible(&vpfe->lock);
		ccdc_enable(FALSE);
		vpfe->started = FALSE;
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
			ccdc_params_raw *ccd_params = &vpfe->ccdc_params_raw;
			ccdc_config_params_raw *raw_params =
			    (ccdc_config_params_raw *) arg;
			unsigned int *fpc_virtaddr = NULL;
			unsigned int *fpc_physaddr = NULL;

			dev_dbg(vpfe_dev,
				"\nStarting VPFE_CMD_CONFIG_CCDC_RAW ioctl");

			/* copy parameter values for Raw Mode to vpfe_device, 
			 * from arg passed by application 
			 */
			if ((ccd_params->frm_fmt != CCDC_FRMFMT_INTERLACED)
			    && (raw_params->image_invert_enable == TRUE)) {
				dev_err(vpfe_dev,
					"\nImage invert not supported");
				return -EINVAL;
			}
			ccd_params->image_invert_enable =
			    raw_params->image_invert_enable;

			dev_dbg(vpfe_dev, "\nimage_invert_enable = %d",
				ccd_params->image_invert_enable);

			ccd_params->data_sz = raw_params->data_sz;
			dev_dbg(vpfe_dev, "\ndata_sz = %d",
				ccd_params->data_sz);
			ccd_params->alaw.b_alaw_enable =
			    raw_params->alaw.b_alaw_enable;
			dev_dbg(vpfe_dev, "\nALaw Enable = %d",
				ccd_params->alaw.b_alaw_enable);
			/* copy A-Law configurations to vpfe_device, from arg 
			 * passed by application 
			 */
			if (ccd_params->alaw.b_alaw_enable) {
				if ((raw_params->alaw.gama_wd > BITS_09_0)
				    || (raw_params->alaw.gama_wd < BITS_15_6)
				    || (raw_params->alaw.gama_wd <
					raw_params->data_sz)) {
					dev_err(vpfe_dev,
						"\nInvalid data line select");
					return -EINVAL;
				}
				ccd_params->alaw.gama_wd =
				    raw_params->alaw.gama_wd;
				dev_dbg(vpfe_dev, "\nALaw Gama width = %d",
					ccd_params->alaw.gama_wd);
			}

			/* copy Optical Balck Clamping configurations to 
			 * vpfe_device,from arg passed by application 
			 */
			ccd_params->blk_clamp.b_clamp_enable
			    = raw_params->blk_clamp.b_clamp_enable;
			dev_dbg(vpfe_dev, "\nb_clamp_enable = %d",
				ccd_params->blk_clamp.b_clamp_enable);
			if (ccd_params->blk_clamp.b_clamp_enable) {
				/*gain */
				ccd_params->blk_clamp.sgain =
				    raw_params->blk_clamp.sgain;
				dev_dbg(vpfe_dev, "\nblk_clamp.sgain = %d",
					ccd_params->blk_clamp.sgain);
				/*Start pixel */
				ccd_params->blk_clamp.start_pixel
				    = raw_params->blk_clamp.start_pixel;
				dev_dbg(vpfe_dev,
					"\nblk_clamp.start_pixel = %d",
					ccd_params->blk_clamp.start_pixel);
				/*No of line to be avg */
				ccd_params->blk_clamp.sample_ln
				    = raw_params->blk_clamp.sample_ln;
				dev_dbg(vpfe_dev,
					"\nblk_clamp.sample_ln = %d",
					ccd_params->blk_clamp.sample_ln);
				/*No of pixel/line to be avg */
				ccd_params->blk_clamp.sample_pixel
				    = raw_params->blk_clamp.sample_pixel;
				dev_dbg(vpfe_dev,
					"\nblk_clamp.sample_pixel  = %d",
					ccd_params->blk_clamp.sample_pixel);
			} else {	/* configure DCSub */

				ccd_params->blk_clamp.dc_sub =
				    raw_params->blk_clamp.dc_sub;
				dev_dbg(vpfe_dev,
					"\nblk_clamp.dc_sub  = %d",
					ccd_params->blk_clamp.dc_sub);
			}

			/* copy BalckLevel Compansation configurations to 
			 * vpfe_device,from arg passed by application 
			 */
			ccd_params->blk_comp.r_comp =
			    raw_params->blk_comp.r_comp;
			ccd_params->blk_comp.gr_comp =
			    raw_params->blk_comp.gr_comp;
			ccd_params->blk_comp.b_comp =
			    raw_params->blk_comp.b_comp;
			ccd_params->blk_comp.gb_comp =
			    raw_params->blk_comp.gb_comp;
			dev_dbg(vpfe_dev, "\nblk_comp.r_comp   = %d",
				ccd_params->blk_comp.r_comp);
			dev_dbg(vpfe_dev, "\nblk_comp.gr_comp  = %d",
				ccd_params->blk_comp.gr_comp);
			dev_dbg(vpfe_dev, "\nblk_comp.b_comp   = %d",
				ccd_params->blk_comp.b_comp);
			dev_dbg(vpfe_dev, "\nblk_comp.gb_comp  = %d",
				ccd_params->blk_comp.gb_comp);

			/* copy FPC configurations to vpfe_device,from  
			 * arg passed by application
			 */
			ccd_params->fault_pxl.fpc_enable =
			    raw_params->fault_pxl.fpc_enable;
			dev_dbg(vpfe_dev, "\nfault_pxl.fpc_enable  = %d",
				ccd_params->fault_pxl.fpc_enable);

			if (ccd_params->fault_pxl.fpc_enable) {
				fpc_physaddr =
				    (unsigned int *)ccd_params->fault_pxl.
				    fpc_table_addr;

				fpc_virtaddr = (unsigned int *)
				    phys_to_virt((unsigned long)
						 fpc_physaddr);

				/* Allocate memory for FPC table if current 
				 * FPC table buffer is not big enough to 
				 * accomodate FPC Number requested 
				 */
				if (raw_params->fault_pxl.fp_num !=
				    ccd_params->fault_pxl.fp_num) {
					if (fpc_physaddr != NULL) {
						free_pages((unsigned long)
							   fpc_physaddr,
							   get_order
							   (ccd_params->
							    fault_pxl.
							    fp_num *
							    FP_NUM_BYTES));

					}

					/* Allocate memory for FPC table */
					fpc_virtaddr = (unsigned int *)
					    __get_free_pages(GFP_KERNEL |
							     GFP_DMA,
							     get_order
							     (raw_params->
							      fault_pxl.
							      fp_num *
							      FP_NUM_BYTES));

					if (fpc_virtaddr == NULL) {
						dev_err(vpfe_dev,
							"\n Unable to allocate \
							memory for FPC");
						return -1;
					}
					fpc_physaddr = (unsigned int *)
					    virt_to_phys((void *)
							 fpc_virtaddr);
				}

				/* Copy number of fault pixels and FPC table */
				ccd_params->fault_pxl.fp_num =
				    raw_params->fault_pxl.fp_num;
				copy_from_user((void *)fpc_virtaddr,
					       (void *)raw_params->
					       fault_pxl.fpc_table_addr,
					       (unsigned long)ccd_params->
					       fault_pxl.fp_num * FP_NUM_BYTES);

				ccd_params->fault_pxl.fpc_table_addr =
				    (unsigned int)fpc_physaddr;

			}
			dev_dbg(vpfe_dev,
				"\nEnd of VPFE_CMD_CONFIG_CCDC_RAW ioctl");
			break;
		}

	case VPFE_CMD_CONFIG_TVP5146:
		/* this can be used directly and bypass the V4L2 APIs */
		{
			/* the settings here must be consistant with that of
			   the CCDC's,driver does not check the consistancy */
			tvp5146_params *params = (tvp5146_params *) arg;
			v4l2_std_id std = 0;
			dev_dbg(vpfe_dev,
				"\nStarting VPFE_CMD_CONFIG_TVP5146 ioctl");
			if (vpfe->started) {
				/* only allowed if streaming is not started */
				ret = -EBUSY;
				break;
			}
			down_interruptible(&vpfe->lock);
			/* make sure the other v4l2 related fields have 
			 * consistant settings 
			 */
			switch (params->mode & 0x7) {
			case TVP5146_MODE_NTSC:
				std = V4L2_STD_NTSC;
				break;
			case TVP5146_MODE_PAL:
				std = V4L2_STD_PAL;
				break;
			case TVP5146_MODE_PAL_M:
				std = V4L2_STD_PAL_M;
				break;
			case TVP5146_MODE_PAL_CN:
				std = V4L2_STD_PAL_N;
				break;
			case TVP5146_MODE_SECAM:
				std = V4L2_STD_SECAM;
				break;
			case TVP5146_MODE_PAL_60:
				std = V4L2_STD_PAL_60;
				break;
			}
			dev_dbg(vpfe_dev,
				"\nVPFE_CMD_CONFIG_TVP5146:std = %d", (int)std);
			if (params->mode & 0x8) {	/* square pixel mode */
				std <<= 32;
			}
			/* auto-detection modes */
			if (params->mode == TVP5146_MODE_AUTO) {
				std = VPFE_STD_AUTO;
			} else if (params->mode == TVP5146_MODE_AUTO_SQP) {
				std = VPFE_STD_AUTO_SQP;
			}

			if (std & V4L2_STD_625_50) {
				vpfe->bounds = pal_bounds;
				vpfe->pixelaspect = pal_aspect;
			} else if (std & V4L2_STD_525_60) {
				vpfe->bounds = ntsc_bounds;
				vpfe->pixelaspect = ntsc_aspect;
			} else if (std & VPFE_STD_625_50_SQP) {
				vpfe->bounds = palsp_bounds;
				vpfe->pixelaspect = sp_aspect;
			} else if (std & VPFE_STD_525_60_SQP) {
				vpfe->bounds = ntscsp_bounds;
				vpfe->pixelaspect = sp_aspect;
			}
			vpfe->std = std;
			ret = tvp5146_ctrl(TVP5146_CONFIG, params);
			dev_dbg(vpfe_dev,
				"\nVPFE_CMD_CONFIG_TVP5146:ret = %d", (int)ret);
			vpfe->tvp5146_params = *params;
			up(&vpfe->lock);
			dev_dbg(vpfe_dev,
				"\nEnd VPFE_CMD_CONFIG_TVP5146 ioctl");
			break;
		}
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
		return -ENOMEM;
	}
	*vfd = vpfe_video_template;
	vfd->dev = device;
	vfd->release = video_device_release;
	snprintf(vfd->name, sizeof(vfd->name),
		 "DM644X_VPFE_DRIVER_V%d.%d.%d",
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
	if (video_register_device(vpfe->video_dev, VFL_TYPE_GRABBER, -1) < 0) {
		video_device_release(vpfe->video_dev);
		vpfe->video_dev = NULL;
		return -1;
	}

	dev_dbg(vpfe_dev, "DM644X vpfe: driver version V%d.%d.%d loaded\n",
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

	if (device_type == TVP5146) {
		fbuf_size = VPFE_TVP5146_MAX_FBUF_SIZE;
		vpfe_device = vpfe_device_ycbcr;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
	}

	else if (device_type == MT9T001) {
		fbuf_size = VPFE_MT9T001_MAX_FBUF_SIZE;
		vpfe_device = vpfe_device_raw;
		frame_format = vpfe_device.ccdc_params_raw.frm_fmt;
	} else {
		return -1;
	}
	/* allocate memory at initialization time to guarentee availability */
	for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) {
		mem =
		    (void *)__get_free_pages(GFP_KERNEL | GFP_DMA,
					     get_order(fbuf_size));
		if (mem) {
			unsigned long adr = (unsigned long)mem;
			u32 size = PAGE_SIZE << (get_order(fbuf_size));
			while (size > 0) {
				/* make sure the frame buffers 
				   are never swapped out of memory */
				SetPageReserved(virt_to_page(adr));
				adr += PAGE_SIZE;
				size -= PAGE_SIZE;
			}
			vpfe_device.fbuffers[i] = (u8 *) mem;
		} else {
			while (--i >= 0) {
				free_reserved_pages((unsigned long)
						    vpfe_device.
						    fbuffers[i], fbuf_size);
			}
			printk(KERN_INFO
			       "frame buffer memory allocation failed.\n");
			return -ENOMEM;
		}
	}
	if (driver_register(&vpfe_driver) != 0) {
		printk(KERN_INFO "driver registration failed\n");
		return -1;
	}
	if (platform_device_register(&_vpfe_device) != 0) {
		driver_unregister(&vpfe_driver);
		printk(KERN_INFO "device registration failed\n");
		return -1;
	}

	ccdc_reset();

	if (device_type == TVP5146) {
		ret = tvp5146_ctrl(TVP5146_INIT, NULL);
		if (ret >= 0) {
			ret = tvp5146_ctrl(TVP5146_RESET, NULL);
			/* configure the tvp5146 to default parameters */
			ret |=
			    tvp5146_ctrl(TVP5146_CONFIG,
					 &vpfe_device.tvp5146_params);
		}
		if (ret < 0) {
			tvp5146_ctrl(TVP5146_CLEANUP, NULL);
		}
	} else if (device_type == MT9T001) {
		/* enable video port in case of raw capture */
		ccdc_enable_vport();
		vpfe_device.config_dev_fxn = mt9t001_ctrl;
		ret =
		    vpfe_device.config_dev_fxn(MT9T001_INIT,
					       &vpfe_device.std,
					       &vpfe_device.device_params);
	}

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
	if (frame_format == CCDC_FRMFMT_PROGRESSIVE) {
		ret = request_irq(IRQ_VDINT1, vdint1_isr, SA_INTERRUPT,
				  "dm644xv4l2", (void *)&vpfe_device);
		if (ret < 0) {
			platform_device_unregister(&_vpfe_device);
			driver_unregister(&vpfe_driver);
			/* Free memory for all image buffers */
			for (i = 0; i < VPFE_DEFNUM_FBUFS; i++) {
				free_reserved_pages((unsigned long)
						    vpfe_device.
						    fbuffers[i], fbuf_size);
			}
			return -1;
		}
	}
	ret = request_irq(IRQ_VDINT0, vpfe_isr, SA_INTERRUPT,
			  "dm644xv4l2", (void *)&vpfe_device);
	if (ret < 0) {
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

	printk(KERN_INFO "DaVinci v4l2 capture driver V1.0 loaded\n");
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
	unsigned int *fpc_physaddr = NULL, *fpc_virtaddr = NULL;

	if (device_type == TVP5146) {
		tvp5146_ctrl(TVP5146_CLEANUP, NULL);
		buf_size = VPFE_TVP5146_MAX_FBUF_SIZE;
		frame_format = vpfe_device.ccdc_params_ycbcr.frm_fmt;
	} else {
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
	if (frame_format == CCDC_FRMFMT_PROGRESSIVE) {
		free_irq(IRQ_VDINT1, &vpfe_device);
	}
	fpc_physaddr = (unsigned int *)
	    vpfe_device.ccdc_params_raw.fault_pxl.fpc_table_addr;

	if (fpc_physaddr != NULL) {
		fpc_virtaddr = (unsigned int *)
		    phys_to_virt((unsigned long)fpc_physaddr);
		free_pages((unsigned long)fpc_virtaddr,
			   get_order(vpfe_device.ccdc_params_raw.fault_pxl.
				     fp_num * FP_NUM_BYTES));
	}

	/* Free memory for all image buffers */
	while (--i >= 0) {
		free_reserved_pages((unsigned long)vpfe_device.
				    fbuffers[i], buf_size);
	}
}

module_init(vpfe_init);
module_exit(vpfe_cleanup);
