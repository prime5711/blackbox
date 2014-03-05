/*
 * Copyright (C) 2007 Texas Instruments Inc
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
/* davinci_display.c */

#define DEBUG
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/device.h>
#include <asm/irq.h>
#include <asm/page.h>

#include "davinci_display.h"
#include "display_vpbe.h"


#define DAVINCI_DISPLAY_DRIVER "DavinciDisplay"

static char *output = "";
module_param(output, charp, S_IRUGO);
static char *mode = "";
module_param(mode, charp, S_IRUGO);

static u32 video0_numbuffers = 3;
static u32 video1_numbuffers = 3;

#define DAVINCI_HD_BUF_SIZE (1920*1088*2)
#define DAVINCI_SD_BUF_SIZE (720*576*2)

/* TBD this should move to KConfig */
//#define CONFIG_DAVINCI_ENABLE_HD 1

#ifdef CONFIG_DAVINCI_ENABLE_HD
static u32 video0_bufsize = DAVINCI_HD_BUF_SIZE;
static u32 video1_bufsize = DAVINCI_HD_BUF_SIZE;
#else
static u32 video0_bufsize = DAVINCI_SD_BUF_SIZE;
static u32 video1_bufsize = DAVINCI_SD_BUF_SIZE;
#endif

module_param(video0_numbuffers, uint, S_IRUGO);
module_param(video1_numbuffers, uint, S_IRUGO);

module_param(video0_bufsize, uint, S_IRUGO);
module_param(video1_bufsize, uint, S_IRUGO);

#define DEFAULT_NUM_BUFS 3
static struct buf_config_params display_buf_config_params = {
	.min_numbuffers      = DEFAULT_NUM_BUFS,
	.numbuffers[0]       = DEFAULT_NUM_BUFS,
	.numbuffers[1]       = DEFAULT_NUM_BUFS,
	.min_bufsize[0]      = DAVINCI_SD_BUF_SIZE,
	.min_bufsize[1]      = DAVINCI_SD_BUF_SIZE,
#ifdef CONFIG_DAVINCI_ENABLE_HD
	.channel_bufsize[0]  = DAVINCI_HD_BUF_SIZE,
	.channel_bufsize[1]  = DAVINCI_HD_BUF_SIZE,
#else
	.channel_bufsize[0]  = DAVINCI_SD_BUF_SIZE,
	.channel_bufsize[1]  = DAVINCI_SD_BUF_SIZE,
#endif
};

static int davinci_display_nr[] = { 2, 3};

/* global variables */
static struct davinci_display davinci_dm;

struct device *davinci_display_dev= NULL;

static struct v4l2_capability davinci_display_videocap = {
	.driver = DAVINCI_DISPLAY_DRIVER,
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)
	.card = "DM355 EVM",
#elif defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	.card = "DM355 IPNC",
#else
	.card = "DM6446 EVM",
#endif
	.bus_info = "Platform",
	.version = VPBE_DISPLAY_VERSION_CODE,
	.capabilities = V4L2_CAP_VIDEO_OUTPUT | V4L2_CAP_STREAMING
};


/*
 * Pass boot-time options by adding the following string to the boot params:
 * 	video=davinci_display:[option[:option]]
 * Valid options:
 * 	output=[ntsc|pal]
 *  vid0_numBuf=[number of buffers per window]; 0 - use user allocated video buffer,
 *  vid1_numBuf=[number of buffers per window]; 0 - use user allocated video buffer,
 *  nonzero for Kernel allocated buffer
 */

#ifndef MODULE
char *davinci_modes[] = { "NTSC", "PAL","720P","1080I-30" };

int __init davinci_display_setup(char *options)
{
	char *this_opt;

	printk("davinci_display_setup: Options \"%s\"\n", options);
	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ":")) != NULL) {
	
		if (!*this_opt)
			continue;

		if (!strncmp(this_opt, "output=", 7)) {
	    	if (!strncmp(this_opt + 7, "ntsc", 4))
			{
				mode = davinci_modes[0];
				printk("<davinci_display_setup : mode = %s\n",mode);
			}
			else if (!strncmp(this_opt + 7, "pal", 3))
			{
				mode = davinci_modes[1];
				printk("<davinci_display_setup : mode = %s\n",mode);
			}
			else if (!strncmp(this_opt + 7, "720p", 4))
			{
				mode = davinci_modes[2];
				printk("<davinci_display_setup : mode = %s\n",mode);
			}
			else if (!strncmp(this_opt + 7, "1080i", 5))
			{
				mode = davinci_modes[2];
				printk("<davinci_display_setup : mode = %s\n",mode);
			}
			else
				printk("<davinci_display_setup : No Video mode selected in boot args\n");
		} else if (!strncmp(this_opt, "vid0_numBuf=",12)) {
			video0_numbuffers = simple_strtoul(this_opt+12, NULL,0);
			printk("<davinci_display_setup : vid0_numBuf = %d\n",video0_numbuffers);
		} else if (!strncmp(this_opt, "vid1_numBuf=",12)) {
			video1_numbuffers = simple_strtoul(this_opt+12, NULL,0);
			printk("<davinci_display_setup : vid1_numBuf = %d\n",video1_numbuffers);
		}
	}
	return 0;
}
__setup("v4l2_video_display=", davinci_display_setup);

#endif


/*
 *=====davinci_alloc_buffer=====*/
/* Allocate memory for buffers */
static inline unsigned long davinci_alloc_buffer(unsigned int buf_size)
{
	
	void *mem = 0;
	u32 size = PAGE_SIZE << (get_order(buf_size));

	mem = (void *) __get_free_pages(GFP_KERNEL | GFP_DMA,
					get_order(buf_size));
	if (mem) {
		unsigned long adr = (unsigned long) mem;
		while (size > 0) {
			SetPageReserved(virt_to_page(adr));
			adr += PAGE_SIZE;
			size -= PAGE_SIZE;
		}
	}
	return (unsigned long) mem;
}

/*
 *=====davinci_free_buffer=====*/
/* Free memory for buffers */
static inline void davinci_free_buffer(unsigned long addr,
				    unsigned int buf_size)
{
	unsigned int size, adr;

	if (!addr)
		return;
	adr = addr;
	size = PAGE_SIZE << (get_order(buf_size));
	while (size > 0) {
		ClearPageReserved(virt_to_page(adr));
		adr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	free_pages(addr, get_order(buf_size));
}

/*
 * ===== davinci_uservirt_to_phys =====
 *
 * This inline function is used to convert user space virtual address
 * to physical address.
 */
static inline u32 davinci_uservirt_to_phys(u32 virt)
{
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *pte;

	struct mm_struct *mm = current->mm;
	pgd = pgd_offset(mm, virt);
	if (!(pgd_none(*pgd) || pgd_bad(*pgd))) {
		pmd = pmd_offset(pgd, virt);

		if (!(pmd_none(*pmd) || pmd_bad(*pmd))) {
			pte = pte_offset_kernel(pmd, virt);

			if (pte_present(*pte)) {
				return __pa(page_address(pte_page(*pte))
					    + (virt & ~PAGE_MASK));
			}
		}
	}

	return 0;
}

/*
 * =====buffer_prepare=====*/
/* This is the callback function called from videobuf_qbuf() function
 * the buffer is prepared and user space virtual address is converted into
 * physical address */
static int davinci_buffer_prepare(struct videobuf_queue *q,
			       struct videobuf_buffer *vb,
			       enum v4l2_field field)
{
	/* Get the file handle object and channel object */
	struct davinci_fh *fh = q->priv_data;
	struct display_obj *channel = fh->channel;
	dev_dbg(davinci_display_dev,"<davinci_buffer_prepare>\n");

	/* If buffer is not initialized, initialize it */
	if (STATE_NEEDS_INIT == vb->state) {
		vb->width = davinci_dm.std_info.activepixels;
		vb->height = davinci_dm.std_info.activelines;
		vb->size = vb->width * vb->height;
		vb->field = field;
	}
	vb->state = STATE_PREPARED;
	/* if user pointer memory mechanism is used, get the physical
	 * address of the buffer
	 */
	if (V4L2_MEMORY_USERPTR == channel->memory) {
		vb->boff = davinci_uservirt_to_phys(vb->baddr);
		if(!ISALIGNED(vb->boff)){
			dev_err(davinci_display_dev,"buffer_prepare:offset is \
				not aligned to 8 bytes\n");
			return -EINVAL;
		}
	}
	dev_dbg(davinci_display_dev,"</davinci_buffer_prepare>\n");
	return 0;
}

/*
 * =====davinci_buffer_config=====*/
/* This function is responsible to responsible for buffer's
 * physical address */
static void davinci_buffer_config(struct videobuf_queue *q,
			       unsigned int count)
{
	/* Get the file handle object and channel object */
	struct davinci_fh *fh = q->priv_data;
	struct display_obj *channel = fh->channel;
	int i;
	dev_dbg(davinci_display_dev,"<davinci_buffer_config>\n");

	/* If memory type is not mmap, return */
	if (V4L2_MEMORY_MMAP != channel->memory) {
		dev_err(davinci_display_dev,"Not MMAP\n");
		return;
	}
	/* Convert kernel space virtual address to physical address */
	for (i = 0; i < count; i++) {
		q->bufs[i]->boff =
		    virt_to_phys((void*) channel->fbuffers[i]);
		dev_dbg(davinci_display_dev,"buffer address: %x\n", q->bufs[i]->boff);
	}
	dev_dbg(davinci_display_dev,"</davinci_buffer_config>\n");
}

/*
 * =====davinci_buffer_setup=====*/
/* This function allocates memory for the buffers */
static int davinci_buffer_setup(struct videobuf_queue *q, unsigned int *count,
			     unsigned int *size)
{
	/* Get the file handle object and channel object */
	struct davinci_fh *fh = q->priv_data;
	struct display_obj *channel = fh->channel;
	int i;
	dev_dbg(davinci_display_dev,"<davinci_buffer_setup>\n");

	/* If memory type is not mmap, return */
	if (V4L2_MEMORY_MMAP != channel->memory) {
		dev_err(davinci_display_dev,"Not MMAP\n");
		return 0;
	}

	/* Calculate the size of the buffer */
	*size = display_buf_config_params.channel_bufsize[channel->plane_id];

	for (i = display_buf_config_params.numbuffers[channel->plane_id] ; 
		i < *count ; i++) {
		/* Allocate memory for the buffers */
		channel->fbuffers[i] = davinci_alloc_buffer(*size);
		if (!channel->fbuffers[i])
			break;
	}
	/* Store number of buffers allocated in numbuffer member */
	*count = channel->numbuffers = i;
	dev_dbg(davinci_display_dev,"</davinci_buffer_setup>\n");
	return 0;
}

/*
 * =====davinci_buffer_queue=====*/
/* This function adds the buffer to DMA queue  */
static void davinci_buffer_queue(struct videobuf_queue *q,
			      struct videobuf_buffer *vb)
{
	/* Get the file handle object and channel object */
	struct davinci_fh *fh = q->priv_data;
	struct display_obj *channel = fh->channel;
	dev_dbg(davinci_display_dev,"<davinci_buffer_queue>\n");

	/* add the buffer to the DMA queue */
	list_add_tail(&vb->queue, &channel->dma_queue);
	/* Change state of the buffer */
	vb->state = STATE_QUEUED;
	dev_dbg(davinci_display_dev,"</davinci_buffer_queue>\n");
}

/*
 * =====davinci_buffer_release=====*/
/* This function is called from the videobuf layer to free memory allocated to
 * the buffers */
static void davinci_buffer_release(struct videobuf_queue *q,
				struct videobuf_buffer *vb)
{
	/* Get the file handle object and channel object */
	struct davinci_fh *fh = q->priv_data;
	struct display_obj *channel = fh->channel;
	unsigned int buf_size = 0;
	dev_dbg(davinci_display_dev,"<davinci_buffer_release>\n");

	/* If memory type is not mmap, return */
	if (V4L2_MEMORY_MMAP != channel->memory) {
		dev_err(davinci_display_dev,"Not MMAP\n");
		return;
	}
	/* Calculate the size of the buffer */
	buf_size = display_buf_config_params.channel_bufsize[channel->plane_id];

	if (((vb->i < channel->numbuffers)
		       	&& (vb->i >= display_buf_config_params.numbuffers[
				channel->plane_id]))
		&& channel->fbuffers[vb->i]) {
		davinci_free_buffer(channel->fbuffers[vb->i], buf_size);
		channel->fbuffers[vb->i] = 0;
	}
	vb->state = STATE_NEEDS_INIT;
	dev_dbg(davinci_display_dev,"</davinci_buffer_release>\n");
}

static struct videobuf_queue_ops video_qops = {
	.buf_setup = davinci_buffer_setup,
	.buf_prepare = davinci_buffer_prepare,
	.buf_queue = davinci_buffer_queue,
	.buf_release = davinci_buffer_release,
	.buf_config = davinci_buffer_config,
};

static u8 channel_first_int = 1;

/* TBD */
/* =====davinci_channel_isr=====*/
/* It changes status of the displayed buffer, takes next buffer from the queue
 * and sets its address in VPBE registers */
static irqreturn_t davinci_display_isr(int irq, void *dispObj,
				    struct pt_regs *regs)
{
	unsigned long jiffies_time = get_jiffies_64();
	struct timeval timevalue;
	int i,fid;
	unsigned long addr = 0;
	struct display_obj *channel = NULL;
	struct davinci_display *dispDevice = (struct davinci_display *)dispObj;

	
	/* Convert time represention from jiffies to timeval */
	jiffies_to_timeval(jiffies_time, &timevalue);

	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		channel = dispDevice->dev[i];  	
		/* If streaming is started in this channel */
		if (!channel->started)
			continue;
			/* Check the field format */
		if ((V4L2_FIELD_NONE == channel->fmt.field) &&
		    (!list_empty(&channel->dma_queue))) {
			/* Progressive mode */
			if (!channel_first_int) {
				/* Mark status of the curFrm to
				 * done and unlock semaphore on it */
				channel->curFrm->ts = timevalue;
				channel->curFrm->state = STATE_DONE;
				wake_up_interruptible(&channel->
						      curFrm->done);
				/* Make curFrm pointing to nextFrm */
				channel->curFrm = channel->nextFrm;
			}
			/* Get the next buffer from buffer queue */
			channel->nextFrm =
			    list_entry(channel->dma_queue.next,
				       struct videobuf_buffer, queue);
			/* Remove that buffer from the buffer queue */
			list_del(&channel->nextFrm->queue);
			/* Mark status of the buffer as active */
			channel->nextFrm->state = STATE_ACTIVE;
			/* Set top and bottom field addresses in
			   VPIF registers */
			addr = channel->curFrm->boff;
			channel->set_sdram_addr(addr, channel->fmt.bytesperline);
		} else {
			/* Interlaced mode */
			/* If it is first interrupt, ignore it */
			if (channel_first_int) {
				channel_first_int = 0;
				dev_dbg(davinci_display_dev,"irq_first time\n");
				return IRQ_HANDLED;
			}
		
			channel->field_id ^= 1;
			/* Get field id from VPIF registers */
			fid = vpbe_get_display_field_id();
			/* If field id does not match with stored
			   field id */
			if (fid != channel->field_id) {
				/* Make them in sync */
				if (0 == fid) {
					channel->field_id = fid;
					dev_dbg(davinci_display_dev,"field synced\n");
				}
				return IRQ_HANDLED;
			}
			/* device field id and local field id are
			   in sync */
			/* If this is even field */
			if (0 == fid) {
			
				if (channel->curFrm == channel->nextFrm)
					continue;
				/* one frame is displayed If next frame is
				 *  available, release curFrm and move on*/

				/* Copy frame display time */
				channel->curFrm->ts = timevalue;
				/* Change status of the curFrm */
				dev_dbg(davinci_display_dev,"Done with this video buffer\n");
				channel->curFrm->state = STATE_DONE;

				if (vpbe_check_for_dma_error())
				{
					dev_dbg(davinci_display_dev,"DMA status error, resetting\n");
					vpbe_reset_dma_error();
				}
				
				/* unlock semaphore on curFrm */
				wake_up_interruptible
				    (&channel->curFrm->done);
				/* Make curFrm pointing to
				   nextFrm */
				channel->curFrm = channel->nextFrm;
			} else if (1 == fid) {	/* odd field */
				if (list_empty(&channel->dma_queue)
				    || (channel->curFrm !=
					channel->nextFrm))
					continue;

				/* one field is displayed configure
				   the next frame if it is available
				   otherwise hold on current frame
				 */
				/* Get next from the buffer
				   queue */
				channel->nextFrm = list_entry(channel->
							      dma_queue.
							      next, struct
							      videobuf_buffer,
							      queue);

				/* Remove that from the
				   buffer queue */
				list_del(&channel->nextFrm->queue);

				/* Mark state of the frame
				   to active */
				channel->nextFrm->state = STATE_ACTIVE;
				addr = channel->nextFrm->boff;
				channel->set_sdram_addr(addr, channel->fmt.bytesperline);
			}
		}
	}
	return IRQ_HANDLED;
}

static inline void
get_win_position(struct display_obj *w, u32 *xp, u32 *yp, u32 *xl, u32 *yl)
{
	struct vpbe_windowinfo *v = &w->window_par;

	*xp = v->x_pos;
	*yp = v->y_pos;
	*xl = v->width;
	*yl = v->height;
}

/* Returns 1 if the window parameters are within VID0, 0 otherwise */
int within_vid0_limits(struct display_obj *w,  vpbe_window_position_t *win_pos)
{
	u32 vid0_xp = 0, vid0_yp = 0, vid0_xl = 0, vid0_yl = 0;
	u32 curr_win_height, curr_win_width;
	curr_win_height = w->window_par.height;
	curr_win_width = w->window_par.width;
	get_win_position(davinci_dm.dev[VPBE_PLANE_VIDEO0], &vid0_xp, &vid0_yp, &vid0_xl, &vid0_yl);

	if ((win_pos->xpos >= vid0_xp) && 
	    (win_pos->ypos >= vid0_yp) && 
	    ((win_pos->xpos + curr_win_width) <= (vid0_xp + vid0_xl)) && 
	    ((win_pos->xpos + curr_win_height) <= (vid0_yp + vid0_yl)))
		return 1;
	return 0;
}

static int davinci_get_std_info(void)
{
	struct encoder_device *enc = davinci_dm.encoder[davinci_dm.current_encoder];
	struct v4l2_standard standard;
	int index=0, ret;

	/* Get standard name from the encoder by enumerating standards */
	do {
		standard.index = index;
		ret = enc->std_ops->enumstd(&standard, enc);
		if (standard.id & davinci_dm.std) {
			strncpy(davinci_dm.std_info.name, standard.name,
					sizeof(davinci_dm.std_info.name));
			break;
		}
		index ++;
	}while(ret >= 0);

	/* Get standard information from VPBE layer */
	ret |= vpbe_display_get_std_info(&davinci_dm.std_info);

	return ret;
}

static void davinci_config_default_format(enum vpbe_plane_id plane_id)
{

	struct display_obj *plane = davinci_dm.dev[plane_id];
	plane->fmt.bytesperline = ((davinci_dm.std_info.activepixels) * (plane->window_par.bits_per_pixel/8));
	plane->fmt.pixelformat = V4L2_PIX_FMT_YUYV;
	plane->fmt.field = V4L2_FIELD_ANY;
	plane->fmt.sizeimage = (davinci_dm.std_info.activelines) * (plane->fmt.bytesperline);
	plane->fmt.width = plane->fmt.bytesperline / (plane->window_par.bits_per_pixel/8);
	plane->fmt.height= plane->fmt.sizeimage / plane->fmt.bytesperline;
	plane->window_par.width = davinci_dm.std_info.activepixels; 
	plane->window_par.height = davinci_dm.std_info.activelines; 
	plane->window_par.x_pos = 0;
	plane->window_par.y_pos = 0;

	if(display_buf_config_params.numbuffers[plane_id] == 0)
		plane->memory = V4L2_MEMORY_USERPTR;
	else
		plane->memory = V4L2_MEMORY_MMAP;
}

static void davinci_config_std(enum vpbe_plane_id plane_id)
{
	int ret;
	struct encoder_device *enc =
	    davinci_dm.encoder[davinci_dm.current_encoder];
	dev_dbg(davinci_display_dev,"<davinci_config_std>\n");
	/* Detect the standard from the device */
	ret = enc->std_ops->getstd(&davinci_dm.std, enc);
	if (ret)
		return;
	davinci_get_std_info();

	davinci_config_default_format(plane_id);
	dev_dbg(davinci_display_dev,"</davinci_config_std>\n");
}

static int davinci_check_format(struct display_obj *channel,
			     struct v4l2_pix_format *pixfmt)
{
	enum v4l2_field field = pixfmt->field;
	dev_dbg(davinci_display_dev,"<davinci_check_format, >\n");
	dev_dbg(davinci_display_dev,"<pixelformat = %x, height = %d, width = %d, field = %d >\n",
			(unsigned int)pixfmt->pixelformat,
			(unsigned int)pixfmt->height,
			(unsigned int)pixfmt->width,
			(unsigned int)pixfmt->field);

	if (!(VPBE_VALID_FIELD(field))) {
		dev_err(davinci_display_dev,"invalid frame format , field = %d\n",(int)field );
		return -EINVAL;
	}

	if ( pixfmt->pixelformat != V4L2_PIX_FMT_UYVY) 
	{
		dev_err(davinci_display_dev, "invalid frame format\n");
		return -EINVAL;
	}

	if (pixfmt->width && ((pixfmt->width%16) != 0))
	{
		/* must be a mutliple of 32 */
		dev_err(davinci_display_dev,"width should be a multiple of 16\n");
		return -EINVAL;
	}
	
	if (( pixfmt->width > davinci_dm.std_info.activepixels) ||
	    ( pixfmt->height > davinci_dm.std_info.activelines)) {
		dev_err(davinci_display_dev,"Invalid width or height\n");
		return -EINVAL;
	}

	dev_dbg(davinci_display_dev,"</davinci_check_format>\n");
	return 0;
}

static int davinci_convert_index(struct display_obj *channel, int *index,
			       int *enc_index)
{
	int i, sumoutput=0, err=0;
	struct encoder_device *enc;
	dev_dbg(davinci_display_dev,"<davinci_convert_index>\n");
	*enc_index = 0;
	for (i = 0; i < davinci_dm.numencoders; i++) {
		enc = davinci_dm.encoder[i];
		if (ISNULL(enc))
			continue;
		sumoutput += enc->output_ops->count;
		if (*index < sumoutput) {
			*enc_index = i;
			sumoutput -= enc->output_ops->count;
			break;
		}
	}
	*index -= sumoutput;
	if (i == davinci_dm.numencoders)
	{
		dev_err(davinci_display_dev,"Invalid index\n");
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"</davinci_convert_index>\n");

	return err;
}

static void davinci_set_video_params
		(
		enum vpbe_plane_id plane_id, vpbe_video_config_params_t *required
		)
{
	if ((plane_id !=VPBE_PLANE_VIDEO0) && (plane_id !=VPBE_PLANE_VIDEO1))
	{
		dev_err(davinci_display_dev,"ERROR: Invlid plane ID, plane_id = %d\n", plane_id);
	    return;
	}
	vpbe_osd_set_vid_params(required->cb_cr_order);
	davinci_dm.dev[plane_id]->conf_params.video_params.cb_cr_order =
	    required->cb_cr_order;
	if (plane_id == VPBE_PLANE_VIDEO0)
	{
		vpbe_osd_set_vid_expansion(&required->exp_info);
		davinci_dm.dev[plane_id]->conf_params.video_params.exp_info =
		    required->exp_info;
	}
	return;
}

static void davinci_set_vid0_default_conf(void)
{
	vpbe_video_params_t *vid_par = &davinci_dm.dev[VPBE_PLANE_VIDEO0]->conf_params.video_params;
	vpbe_video_config_params_t vid_conf_params;
	vid_par->cb_cr_order = SET_0;	/* default order CB-CR */
	vid_par->exp_info.horizontal = VPBE_DISABLE;	/* Expansion disable */

	davinci_dm.dev[VPBE_PLANE_VIDEO0]->window_enable= VPBE_DISABLE;
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_h = 0;	/* No Zoom */
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_v = 0;	/* No Zoom */
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->field_frame_select = VPBE_INTERLACED;
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->set_sdram_addr=vpbe_set_vid0_sdram_address;
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->plane_id = VPBE_PLANE_VIDEO0;

	/* Set the defaults vid params in registers */
	vid_conf_params.cb_cr_order = vid_par->cb_cr_order;
	vid_conf_params.exp_info = vid_par->exp_info;
	davinci_set_video_params(VPBE_PLANE_VIDEO0,&vid_conf_params);

	vpbe_set_bg_color(0, 75);

	/* Set default zoom in register */
	vpbe_osd_set_zoom_vid0(davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_h, davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_v);
	/* Set default buffer size */
}

/*
 * ========	set_vid1_default_conf	========
 */
/* The function	set defaults of VID1 window. */
static void davinci_set_vid1_default_conf(void)
{
	vpbe_video_params_t *vid_par = &davinci_dm.dev[VPBE_PLANE_VIDEO1]->conf_params.video_params;
	vpbe_video_config_params_t vid_conf_params;

	vid_par->cb_cr_order = SET_0;	/* default order CB-CR */
	vid_par->exp_info.horizontal = VPBE_DISABLE;	/* Expansion disable */
	vid_par->exp_info.vertical = VPBE_DISABLE;

	davinci_dm.dev[VPBE_PLANE_VIDEO1]->window_enable = VPBE_DISABLE;
	davinci_dm.dev[VPBE_PLANE_VIDEO1]->zoom.zoom_h = 0;	/* No Zoom */
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_v = 0;	/* No Zoom */
	davinci_dm.dev[VPBE_PLANE_VIDEO1]->field_frame_select = VPBE_INTERLACED;
	davinci_dm.dev[VPBE_PLANE_VIDEO1]->set_sdram_addr=vpbe_set_vid1_sdram_address;
	davinci_dm.dev[VPBE_PLANE_VIDEO0]->plane_id = VPBE_PLANE_VIDEO0;

	/* Set the defaults vid params in registers */
	vid_conf_params.cb_cr_order = vid_par->cb_cr_order;
	vid_conf_params.exp_info = vid_par->exp_info;
	davinci_set_video_params(VPBE_PLANE_VIDEO1,&vid_conf_params);

	/* Set default zoom in register */
	vpbe_osd_set_zoom_vid1(davinci_dm.dev[VPBE_PLANE_VIDEO1]->zoom.zoom_h, davinci_dm.dev[VPBE_PLANE_VIDEO0]->zoom.zoom_v);
	/* Set default buffer size */
}

static int davinci_streaming_started(void)
{
	return (davinci_dm.dev[VPBE_PLANE_VIDEO0]->started || davinci_dm.dev[VPBE_PLANE_VIDEO0]->started);
}

static int davinci_set_video_display_params(struct display_obj *channel)
{
	unsigned long addr;
	dev_dbg(davinci_display_dev,"davinci_dm.std_info.frame_format = %d\n",davinci_dm.std_info.frame_format);
	dev_dbg(davinci_display_dev,"plane.pixfmt.field= %d\n",channel->fmt.field);

	/* Configure the window size registers */
	dev_dbg(davinci_display_dev,"plane->window_par.x_pos = %d,plane->window_par.y_pos=%d,plane->window_par.width=%d,plane->window_par.height=%d\n",
			channel->window_par.x_pos,
			channel->window_par.y_pos,
			channel->window_par.width,
			channel->window_par.height
		  );	

	addr = channel->curFrm->boff;

	if (channel->fmt.pixelformat == V4L2_PIX_FMT_UYVY)
		vpbe_osd_set_vid_params(SET_0);
	else
	{
		dev_err(davinci_display_dev,"Incorrect format\n");
		return -EINVAL;
 	}

	/* Set address in the display registers */
	channel->set_sdram_addr(addr, channel->fmt.bytesperline);

	/* Configure the video mode in OSD */
	if (channel->plane_id == VPBE_PLANE_VIDEO0) 
		vpbe_set_vid0_win_mode(channel->window_par.bits_per_pixel);
	else if (channel->plane_id == VPBE_PLANE_VIDEO1)
		vpbe_set_vid1_win_mode(channel->window_par.bits_per_pixel);
	else {
		dev_err(davinci_display_dev,"Error, invalid plane id\n");
		/* error */
		return -EINVAL;
	}


	/* Configure Output device */
	if(!davinci_streaming_started())
	{
		dev_dbg(davinci_display_dev,"streamings not started in either VID plane\n");
		if (vpbe_config_display_output(&davinci_dm.std_info) < 0)
		{
			dev_err(davinci_display_dev,"Error, output selected\n");
			return -EINVAL;
		}
	}
	/* based on mode, set the interlace status */
	channel->set_interlaced(davinci_dm.std_info.frame_format);
	channel->set_win_size(
			davinci_dm.std_info.frame_format,
			channel->window_par.x_pos,
			channel->window_par.y_pos,
			channel->window_par.width,
			channel->window_par.height
			);

	channel->set_win_enable(1);

	/* Configure Output device */
#if 0
	if(!davinci_streaming_started())
	{
		/* Enable vpbe output on DAC or DLCD */
		vpbe_enable_vpbe_output(1);
	}
#endif

	/* Enable the window */
	channel->window_enable=1;
	return 0;
}
/*
 * ======== vpbe_doioctl ========*
 * This function will provide different V4L2 commands.This function can be
 * used to configure driver or get status of driver as per command passed
 * by application */
static int davinci_doioctl(struct inode *inode, struct file *file,
			unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct davinci_fh *fh = file->private_data;
	struct display_obj *channel = fh->channel;
	unsigned int index = 0, i = 0;
	unsigned long addr, flags;
	struct encoder_device *enc = davinci_dm.encoder[davinci_dm.current_encoder];
	dev_dbg(davinci_display_dev,"<vpbe_doioctl>\n");

	/* Check for the priority */
#if 0
	if (  (VPBE_PLANE_VIDEO0 == channel->plane_id)
	      (VPBE_PLANE_VIDEO1 == channel->plane_id)
	   ) {
#endif
		switch (cmd) {
		case VIDIOC_S_STD:
		case VIDIOC_S_OUTPUT:
		case VIDIOC_S_FMT:
			ret = v4l2_prio_check(&channel->prio, &fh->prio);
			if (0 != ret)
				return ret;
#if 0
			fh->initialized = 1;
#endif
			break;
		}
#if 0
	}
#endif
	/* This file handle has not initialized the channel, 
	   It is not allowed to do settings */
#if 0
	if 	(  (VPBE_PLANE_VIDEO0 == channel->plane_id)
	      (VPBE_PLANE_VIDEO1 == channel->plane_id)
		)
	{
#endif
		switch (cmd) {
			case VIDIOC_S_STD:
			case VIDIOC_S_OUTPUT:
#if 0
			case VIDIOC_S_FMT:
			case VIDIOC_REQBUFS:
#endif
				if(!fh->initialized){
					dev_err(davinci_display_dev,"Channel Busy\n");
					return -EBUSY;
				}
		}
#if 0
	}
#endif
	/* Check for null value of parameter */
	if (ISNULL((void *) arg)) {
		dev_err(davinci_display_dev,"Null pointer\n");
		return -EINVAL;
	}
	/* Switch on the command value */
	switch (cmd) {
		/* If the case is for querying capabilities */
	case VIDIOC_QUERYCAP:
		{
			struct v4l2_capability *cap =
			    (struct v4l2_capability *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_QUERYCAP, plane id = %d\n",channel->plane_id);
			memset(cap, 0, sizeof(*cap));
			*cap = davinci_display_videocap;
			break;
		}

		/* If the case is for enumerating outputs */
	case VIDIOC_ENUMOUTPUT:
		{
			int index;
			struct v4l2_output *output =
			    (struct v4l2_output *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_ENUMOUTPUT, plane id = %d\n", channel->plane_id);

			index = output->index;
			/* Map the index to the index of the encoder */
			ret = davinci_convert_index(channel, &(output->index), &i);
			if(ret < 0) {
				break;
			}
			ret = davinci_dm.encoder[i]->output_ops->
			    enumoutput(output, davinci_dm.encoder[i]);
			output->index = index;
			break;
		}

		/* If the case is for getting output */
	case VIDIOC_G_OUTPUT:
		{
			int index = 0;
			dev_dbg(davinci_display_dev,"VIDIOC_G_OUTPUT, plane id = %d\n", channel->plane_id);
			/* Call getoutput function of the encoder
			 * device */
			enc->output_ops->getoutput((int *) arg, enc);
			/* Configure standard on the selected encoder */
			davinci_config_std(channel->plane_id);
			for(index = 0 ; index < davinci_dm.current_encoder;
					index++)
				*(int *) arg += 
				    davinci_dm.encoder[i]->output_ops->count;
			break;
		}

		/* If the case is for setting output */
	case VIDIOC_S_OUTPUT:
		{
			int index = *(int *) arg;
			int index1 = index;
			dev_dbg(davinci_display_dev,"VIDIOC_S_OUTPUT, plane id = %d\n", channel->plane_id);
			/* If streaming is started return device busy
			 * error */
			if (channel->started) {
				dev_err(davinci_display_dev,"Streaming is on\n");
				ret = -EBUSY;
				break;
			}
			if (ISNULL(enc->output_ops)
			    || ISNULL(enc->output_ops->setoutput) ||
			    ISNULL(enc->deinitialize)
			    || ISNULL(enc->initialize)) {
				dev_err(davinci_display_dev,"vpbe_doioctl:No setoutput\n");
				return -EINVAL;
			}
			/* Map the index to the index of the encoder */
			ret = davinci_convert_index(channel, &index, &i);
			if (ret < 0 ) {
				break;
			}

			enc = davinci_dm.encoder[davinci_dm.current_encoder];
			davinci_dm.current_encoder = i;
			/* Deinitialize the previous encoder */
			enc->deinitialize(enc);
			/* Initialize the new encoder */
			enc = davinci_dm.encoder[i];
			enc->initialize(enc, 0);
			/* Set the standard in the encoder */
			ret = enc->output_ops->setoutput(&index, enc);
			*(int *) arg = index1;
			davinci_config_std(channel->plane_id);
			break;
		}

		/* If the case is for enumerating standards */
	case VIDIOC_ENUMSTD:
		{
			struct v4l2_standard *std = 
					(struct v4l2_standard *)arg;
			int index = std->index, enc_idx;
			dev_dbg(davinci_display_dev,"VIDIOC_ENUMSTD, plane id = %d\n", channel->plane_id);
			/* Call enumstd function of encoder device */
			if (ISNULL(enc->std_ops)
			    || ISNULL(enc->std_ops->enumstd)) {
				dev_err(davinci_display_dev,"vpbe_doioctl:No enumstd\n");
				return -EINVAL;
			}

			if(index >= davinci_dm.count_std) {
				dev_err(davinci_display_dev,"Invalid index\n");
				return -EINVAL;
			}
			enc_idx = davinci_dm.std_tbl[index].enc_idx;
			enc = davinci_dm.encoder[enc_idx];
			std->index = davinci_dm.std_tbl[index].std_idx;
			ret = enc->std_ops->enumstd(std, enc);
			std->index = index;
			break;
		}

		/* If the case is for getting standard */
	case VIDIOC_G_STD:
		{
			v4l2_std_id *std = (v4l2_std_id *)arg;
			dev_dbg(davinci_display_dev,"VIDIOC_G_STD, plane id = %d\n", channel->plane_id);
			if (ISNULL(enc->std_ops)
			    || ISNULL(enc->std_ops->getstd)) {
				dev_err(davinci_display_dev,"vpbe_doioctl:No getstd\n");
				return -EINVAL;
			}
			/* Call getstd function of encoder device */
			ret = enc->std_ops->getstd(std, enc);
			break;
		}

		/* If the case is for setting standard */
	case VIDIOC_S_STD:
		{
			v4l2_std_id std = *(v4l2_std_id *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_S_STD, plane id = %d\n", channel->plane_id);

			/* If streaming is started, return device
			   busy error */
			if (channel->started) {
				dev_err(davinci_display_dev,"streaming is started\n");
				return -EBUSY;
			}
			if (ISNULL(enc->std_ops)
			    || ISNULL(enc->std_ops->setstd)) {
				dev_err(davinci_display_dev,"vpbe_doioctl:No setstd\n");
				return -EINVAL;
			}
			/* Call encoder driver function to set the
			   standard */
			ret = enc->std_ops->setstd(&std, enc);
			/* If it returns error, return error */
			if (ret)
				return ret;
			/* Get the information about the standard from
			 * the encoder */
			davinci_dm.std = std;
			davinci_get_std_info();

			if (davinci_dm.std_info.activelines *
				davinci_dm.std_info.activepixels * 2 >
				display_buf_config_params.channel_bufsize[channel->plane_id]) {
				dev_err(davinci_display_dev,"invalid std for this size\n");
				return -EINVAL;
			}

			davinci_config_default_format(channel->plane_id);
			break;
		}

#if 0
	case VIDIOC_CROPCAP:
		{
			/* TBD for setting the image crop limits */
			break;
		}
#endif

	case VIDIOC_G_CROP:
		{
			/* TBD to get the x,y and height/width params */
			struct v4l2_crop  *crop = 
					(struct v4l2_crop *)arg;
			dev_dbg(davinci_display_dev,"VIDIOC_G_CROP, plane id = %d\n", channel->plane_id);

			if (crop->type == V4L2_BUF_TYPE_VIDEO_OUTPUT)
			{
				struct v4l2_rect *rect = &crop->c;
				down_interruptible(&davinci_dm.lock);
				rect->top    = channel->window_par.y_pos;
				rect->left   = channel->window_par.x_pos;
				rect->width  = channel->window_par.width;
				rect->height = channel->window_par.height;
				up(&davinci_dm.lock);
			} else {
				dev_err(davinci_display_dev,"Invalid buf type \n");
				return -EINVAL;
			}
			break;
		}

	case VIDIOC_S_CROP:
		{
			/* TBD to get the x,y and height/width params */
			struct v4l2_crop  *crop = 
					(struct v4l2_crop *)arg;
			dev_dbg(davinci_display_dev,"VIDIOC_S_CROP, plane id = %d\n", channel->plane_id);

			if (crop->type == V4L2_BUF_TYPE_VIDEO_OUTPUT)
			{
				struct v4l2_rect *rect = &crop->c;
				down_interruptible(&davinci_dm.lock);
				channel->window_par.y_pos = rect->top;
				channel->window_par.x_pos = rect->left;
				channel->window_par.width = rect->width;
				channel->window_par.height = rect->height; 
				up(&davinci_dm.lock);
			} else {
				dev_err(davinci_display_dev,"Invalid buf type \n");
				return -EINVAL;
			}
			break;
		}

#if 0
	case VIDIOC_S_VPBE_PARAMS:
		{
			/* TBD to set ZOOM factor or scale factor and other parameters in VPBE */
			break;
		}
	case VIDIOC_G_VPBE_PARAMS:
		{
			/* TBD to get ZOOM factor or scale factor and other parameters in VPBE */
			break;
		}
#endif
		/* If the case is for enumerating formats */
	case VIDIOC_ENUM_FMT:
		{
			struct v4l2_fmtdesc *fmt = 
					(struct v4l2_fmtdesc *)arg;
			dev_dbg(davinci_display_dev,"VIDIOC_ENUM_FMT, plane id = %d\n", channel->plane_id);
			if (fmt->index > 0) {
				dev_err(davinci_display_dev,"Invalid format index\n");
				return -EINVAL;
			}
			/* Fill in the information about format */

			index = fmt->index;
			memset(fmt, 0, sizeof(*fmt));
			fmt->index = index;
			fmt->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
			if (index == 0)
			{
				strcpy(fmt->description,
			       "YUV 4:2:2 - UYVY");
				fmt->pixelformat = V4L2_PIX_FMT_UYVY;
			}
			break;
		}

		/* If the case is for getting formats */
	case VIDIOC_G_FMT:
		{
			struct v4l2_format *fmt =
			    (struct v4l2_format *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_G_FMT, plane id = %d\n", channel->plane_id);

			/* If buffer type is video output */
			if (V4L2_BUF_TYPE_VIDEO_OUTPUT == fmt->type) {
				struct v4l2_pix_format *pixfmt =
				    &fmt->fmt.pix;
				/* Fill in the information about
				 * format */
				down_interruptible(&davinci_dm.lock);
				*pixfmt = channel->fmt;
				if (channel->fmt.field == V4L2_FIELD_ANY)
					pixfmt->field = V4L2_FIELD_INTERLACED;
					
				up(&davinci_dm.lock);
			} else {
				dev_err(davinci_display_dev,"invalid type\n");
				ret = -EINVAL;
			}
			break;
		}

		/* If the case is for setting formats */
	case VIDIOC_S_FMT:
		{
			struct v4l2_format *fmt =
			    (struct v4l2_format *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_S_FMT, plane id = %d\n", channel->plane_id);

			/* If streaming is started, return error */
			if (channel->started) {
				dev_err(davinci_display_dev,"Streaming is started\n");
				return -EBUSY;
			}
			if (V4L2_BUF_TYPE_VIDEO_OUTPUT == fmt->type) {
				struct v4l2_pix_format *pixfmt =
				    &fmt->fmt.pix;
				/* Check for valid field format */
				ret = davinci_check_format(channel, pixfmt);

				if (ret)
					return ret;

				/* store the pixel format in the channel
				 * object */

				channel->fmt = *pixfmt;
				
				channel->fmt.bytesperline = pixfmt->width * (channel->window_par.bits_per_pixel/8);
				channel->fmt.sizeimage = pixfmt->height * channel->fmt.bytesperline; 
				channel->window_par.width = pixfmt->width;
				channel->window_par.height = pixfmt->height; 
				channel->window_par.x_pos = 0;
				channel->window_par.y_pos = 0;
				
				/* Now set the format in the hardware */
				if (channel->plane_id == VPBE_PLANE_VIDEO0) 
					vpbe_set_vid0_win_mode(channel->window_par.bits_per_pixel);
				else if (channel->plane_id == VPBE_PLANE_VIDEO1)
					vpbe_set_vid1_win_mode(channel->window_par.bits_per_pixel);
				else {
					dev_err(davinci_display_dev,"invalid plane id\n");
					ret = -EINVAL;
				}
			} else {
				dev_err(davinci_display_dev,"invalid type\n");
				ret = -EINVAL;
			}
			break;
		}

		/* If the case is for trying formats */
	case VIDIOC_TRY_FMT:
		{
			struct v4l2_format *fmt;
			dev_dbg(davinci_display_dev,"VIDIOC_TRY_FMT\n");
			fmt = (struct v4l2_format *) arg;

			if (V4L2_BUF_TYPE_VIDEO_OUTPUT == fmt->type) {
				struct v4l2_pix_format *pixfmt =
				    &fmt->fmt.pix;
				/* Check for valid field format */
				ret = davinci_check_format(channel, pixfmt);
				if (ret) {
					*pixfmt = channel->fmt;
				}
			} else {
				dev_err(davinci_display_dev,"invalid type\n");
				ret = -EINVAL;
			}
			break;
		}

		/* If the case is for requesting buffer allocation */
	case VIDIOC_REQBUFS:
		{
			struct v4l2_requestbuffers *reqbuf;
			enum v4l2_field field;
			reqbuf = (struct v4l2_requestbuffers *) arg;
			dev_dbg(davinci_display_dev,"VIDIOC_REQBUFS, count= %d, type = %d, memory = %d\n",
				reqbuf->count,
				reqbuf->type,
				reqbuf->memory);

			/* If io users of the channel is not zero,
			   return error */
			if (0 != channel->io_usrs) {
				dev_err(davinci_display_dev,"not IO user\n");
				ret = -EBUSY;
				break;
			}
			down_interruptible(&davinci_dm.lock);
			if (channel->fmt.field == V4L2_FIELD_ANY)
				field = V4L2_FIELD_INTERLACED;
			else
				field = channel->fmt.field;
			/* Initialize videobuf queue as per the
			   buffer type */
			videobuf_queue_init(&channel->buffer_queue,
					    &video_qops, NULL,
					    &channel->irqlock,
					    V4L2_BUF_TYPE_VIDEO_OUTPUT,
					    field,
					    sizeof(struct videobuf_buffer),
					    fh);
			/* Set buffer to Linear buffer */
			videobuf_set_buftype(&channel->buffer_queue,
					     VIDEOBUF_BUF_LINEAR);
			/* Set io allowed member of file handle to
			 * TRUE */
			fh->io_allowed = 1;
			/* Increment io usrs member of channel object
			   to 1 */
			channel->io_usrs = 1;
			/* Store type of memory requested in channel
			   object */
			channel->memory = reqbuf->memory;
			/* Initialize buffer queue */
			INIT_LIST_HEAD(&channel->dma_queue);
			/* Allocate buffers */
			ret =
			    videobuf_reqbufs(&channel->buffer_queue,
					     reqbuf);
			up(&davinci_dm.lock);
			break;
		}

		/* If the case is for en-queing buffer in the buffer
		 * queue */
	case VIDIOC_QBUF:
		{
			struct v4l2_buffer tbuf;
			struct videobuf_buffer *buf1;
			dev_dbg(davinci_display_dev,"VIDIOC_QBUF, plane id = %d\n", channel->plane_id);

			/* If this file handle is not allowed to do IO,
			   return error */
			if (!fh->io_allowed) {
				dev_err(davinci_display_dev,"No io_allowed\n");
				ret = -EACCES;
				break;
			}
			if(!(list_empty(&channel->dma_queue)) ||
				(channel->curFrm != channel->nextFrm) ||
				!(channel->started)){

				ret = videobuf_qbuf(&channel->buffer_queue,
					(struct v4l2_buffer *)arg);
				break;
			}
			/* bufferqueue is empty store buffer address
			 *  in VPBE registers */
			
			down(&channel->buffer_queue.lock);
			tbuf = *(struct v4l2_buffer *) arg;
			buf1 = channel->buffer_queue.bufs[tbuf.index];
			if (buf1->memory != tbuf.memory) {
				dev_err(davinci_display_dev,"invalid buffer type\n");
				up(&channel->buffer_queue.lock);
				return -EINVAL;
			}
			if ((buf1->state == STATE_QUEUED) ||
			    (buf1->state == STATE_ACTIVE)) {
				up(&channel->buffer_queue.lock);
				dev_err(davinci_display_dev,"invalid state\n");
				return -EINVAL;
			}

			switch (buf1->memory) {
			case V4L2_MEMORY_MMAP:
				if (buf1->baddr == 0) {
					up(&channel->buffer_queue.lock);
					dev_err(davinci_display_dev,"No Buffer address\n");
					return -EINVAL;
				}
				break;
			case V4L2_MEMORY_USERPTR:
				if (tbuf.length < buf1->bsize) {
					up(&channel->buffer_queue.lock);
					dev_err(davinci_display_dev,"No Buffer address\n");
					return -EINVAL;
				}
				if ((STATE_NEEDS_INIT != buf1->state)
				    && (buf1->baddr != tbuf.m.userptr))
					davinci_buffer_release(&channel->
							    buffer_queue,
							    buf1);
				buf1->baddr = tbuf.m.userptr;
				break;
			default:
				up(&channel->buffer_queue.lock);
				dev_err(davinci_display_dev,"Unknow Buffer type \n");
				return -EINVAL;
			}
			local_irq_save(flags);
			ret =
			    davinci_buffer_prepare(&channel->buffer_queue,
						buf1,
						channel->buffer_queue.
						field);
			
			buf1->state = STATE_ACTIVE;
			addr = buf1->boff;
			channel->nextFrm = buf1;
			channel->set_sdram_addr(addr, channel->fmt.bytesperline);
			local_irq_restore(flags);
			list_add_tail(&buf1->stream, 
				&(channel->buffer_queue.stream));
			up(&channel->buffer_queue.lock);
			break;
		}

		/* If the case is for de-queing buffer from the
		 * buffer queue */
	case VIDIOC_DQBUF:
		{
			dev_dbg(davinci_display_dev,"VIDIOC_DQBUF, plane id = %d\n", channel->plane_id);

			/* If this file handle is not allowed to do IO,
			   return error */
			if (!fh->io_allowed) {
				dev_err(davinci_display_dev,"No io_allowed\n");
				ret = -EACCES;
				break;
			}
			if (file->f_flags & O_NONBLOCK)
				/* Call videobuf_dqbuf for non
				   blocking mode */
				ret =
				    videobuf_dqbuf(&channel->buffer_queue,
						   (struct v4l2_buffer *)
						   arg, 1);
			else
				/* Call videobuf_dqbuf for
				   blocking mode */
				ret =
				    videobuf_dqbuf(&channel->buffer_queue,
						   (struct v4l2_buffer *)
						   arg, 0);
			break;
		}

		/* If the case is for querying information about
		 *  buffer for memory mapping io */
	case VIDIOC_QUERYBUF:
		{
			dev_dbg(davinci_display_dev,"VIDIOC_QUERYBUF, plane id = %d\n", channel->plane_id);
			/* Call videobuf_querybuf to get information */
			ret = videobuf_querybuf(&channel->buffer_queue,
						(struct v4l2_buffer *)
						arg);
			break;
		}

		/* If the case is starting streaming */
	case VIDIOC_STREAMON:
		{
			dev_dbg(davinci_display_dev,"VIDIOC_STREAMON, plane id = %d\n", channel->plane_id);
			/* If file handle is not allowed IO,
			 * return error */
			if (!fh->io_allowed) {
				dev_err(davinci_display_dev,"No io_allowed\n");
				ret = -EACCES;
				break;
			}
			/* If Streaming is already started,
			 * return error */
			if (channel->started) {
				dev_err(davinci_display_dev,"channel is already streaming\n");
				ret = -EBUSY;
				break;
			}

			/* Call videobuf_streamon to start streaming
			   in videobuf */
			ret = videobuf_streamon(&channel->buffer_queue);
			if (ret) {
				dev_err(davinci_display_dev,"error in videobuf_streamon\n");
				break;
			}
			down_interruptible(&davinci_dm.lock);
			/* If buffer queue is empty, return error */
			if (list_empty(&channel->dma_queue)) {
				dev_err(davinci_display_dev,"buffer queue is empty\n");
				ret = -EIO;
				up(&davinci_dm.lock);
				break;
			}
			/* Get the next frame from the buffer queue */
			channel->nextFrm = channel->curFrm =
			    list_entry(channel->dma_queue.next,
				       struct videobuf_buffer, queue);
			/* Remove buffer from the buffer queue */
			list_del(&channel->curFrm->queue);
			/* Mark state of the current frame to active */
			channel->curFrm->state = STATE_ACTIVE;
			/* Initialize field_id and started member */

			channel->field_id = 0;


			/* Set parameters in OSD and VENC */
			ret = davinci_set_video_display_params(channel);
			if (ret < 0) {
				up(&davinci_dm.lock);
				return ret;
			}
			channel->started = 1;
			dev_dbg(davinci_display_dev,"Started streaming on plane id = %d, ret = %d\n",channel->plane_id, ret );
			channel_first_int = 1;
			up(&davinci_dm.lock);
			break;
		}

		/* If the case is for stopping streaming */
	case VIDIOC_STREAMOFF:
		{
			dev_dbg(davinci_display_dev,"VIDIOC_STREAMOFF,plane id = %d\n", channel->plane_id);
			/* If io is allowed for this file handle,
			   return error */
			if (!fh->io_allowed) {
				dev_err(davinci_display_dev,"No io_allowed\n");
				ret = -EACCES;
				break;
			}
			/* If streaming is not started, return error */
			if (!channel->started) {
				dev_err(davinci_display_dev,"streaming not started in plane id = %d\n",channel->plane_id);
				ret = -EINVAL;
				break;
			}
			down_interruptible(&davinci_dm.lock);
			channel->set_win_enable(0);
			channel->started = 0;
			up(&davinci_dm.lock);
			ret = videobuf_streamoff(&channel->buffer_queue);
			break;
		}

	default:
		return -EINVAL;
	}

	dev_dbg(davinci_display_dev,"<vpbe_doioctl>\n");
	return ret;
}

/*
 * ======== davinci_ioctl ========*/
/* Calls davinci_doioctl function */
static int davinci_ioctl(struct inode *inode, struct file *file,
		      unsigned int cmd, unsigned long arg)
{
	int ret;
	dev_dbg(davinci_display_dev,"Start of davinci ioctl\n");
	ret = video_usercopy(inode, file, cmd, arg, (void *) davinci_doioctl);
	if ((ret >= 0) && (VIDIOC_S_FMT == cmd || VIDIOC_TRY_FMT == cmd)) {
		ret = video_usercopy(inode, file, VIDIOC_G_FMT,
				     arg, (void *) davinci_doioctl);
	}
	dev_dbg(davinci_display_dev,"</davinci_ioctl>\n");
	return ret;
}

/*
 * ======== vpbe_mmap ========*/
/* It is used to map kernel space buffers into user spaces */
static int davinci_mmap(struct file *filep, struct vm_area_struct *vma)
{
	/* Get the channel object and file handle object */
	struct davinci_fh *fh = filep->private_data;
	struct display_obj *channel = fh->channel;
	int err = 0;
	dev_dbg(davinci_display_dev,"<davinci_mmap>\n");

	err = videobuf_mmap_mapper(&channel->buffer_queue, vma);
	dev_dbg(davinci_display_dev,"</davinci_mmap>\n");
	return err;
}

/*
 *=====davinci_open===== */
/* It creates object of file handle structure and stores it in private_data
 * member of filepointer */
static int davinci_open(struct inode *inode, struct file *filep)
{
	int minor = iminor(inode);
	int found = -1;
	int i = 0, err = 0, j=0, count;
	struct display_obj *channel;
	struct encoder_device *enc;
	struct davinci_fh *fh = NULL;
	struct v4l2_output out;
	struct v4l2_standard std;


	/* Check for valid minor number */
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		/* Get the pointer to the channel object */
		channel = davinci_dm.dev[i];
		if (minor == channel->video_dev->minor) {
			found = i;
			break;
		}
	}

	/* If not found, return error no device */
	if (0 > found) {
		dev_err(davinci_display_dev,"device not found\n");
		return -ENODEV;
	}
	/* TBD, how current_encoder is set ? */
	//davinci_dm.current_encoder = 0;
	/* Check if encoder registered for default device */
	enc = davinci_dm.encoder[davinci_dm.current_encoder];
	if (ISNULL(enc)) {
		dev_err(davinci_display_dev,"No encoder registered\n");
		return -ENXIO;
	}
	if (ISNULL(enc->std_ops) || ISNULL(enc->std_ops->getstd)) {
		dev_err(davinci_display_dev,"No standard functions in encoder\n");
		return -EINVAL;
	}
	/* Allocate memory for the file handle object */
	fh = kmalloc(sizeof(struct davinci_fh), GFP_KERNEL);
	if (ISNULL(fh)) {
		dev_err(davinci_display_dev,"unable to allocate memory for file handle object\n");
		return -ENOMEM;
	}
	/* store pointer to fh in private_data member of filep */
	filep->private_data = fh;
	fh->channel = channel;
	fh->initialized = 0;
	dev_dbg(davinci_display_dev,"<davinci open> plane = %d\n", channel->plane_id);
	/* If encoder is not initialized. initialize it */
	if (!davinci_dm.initialized) {
		fh->initialized = 1;
		/* Initialize encoder by calling initialize function */
		enc->initialize(enc, ENCODER_FULL_INIT_FLAG);
		davinci_dm.initialized = 1;

		/* Set the output in the encoder */
		if (output) {
			found = 0;
			/* Map the index to the index of the encoder */
			for (i = 0; i < VPBE_DISPLAY_NUM_ENCODERS; i++) {
				enc = davinci_dm.encoder[i];
				if (!enc)
					continue;
				count = enc->output_ops->count;
				for (j = 0; j < count; j++) {
					out.index = j;
					enc->output_ops->
						enumoutput(&out, enc);
					if (0 !=
						strcmp(out.name, output))
						continue;
					found = 1;
					enc->output_ops->setoutput(&j, enc);
					davinci_dm.current_encoder = i;
					break;
				}
				if (found)
					break;
			}
		}
		else {
			for (i = 0; i < VPBE_DISPLAY_NUM_ENCODERS; i++) {
				enc = davinci_dm.encoder[i];
				if (!enc)
					continue;
				j = 0;
				enc->output_ops->setoutput(&j, enc);
				davinci_dm.current_encoder = i;
				break;
			}
		}

		if (mode) {
			enc = davinci_dm.encoder[davinci_dm.current_encoder];
			if (!enc)
			{
				dev_err(davinci_display_dev,"No encoder set\n");
				return -ENODEV;
			}
			std.index = 0;
			do {
				err = enc->std_ops->enumstd(&std, enc);
				if (err) {
					break;
				}
				if (0 != strcmp(std.name, mode)) {
					std.index++;
					continue;
				}
				err = enc->std_ops->setstd(&std.id, enc);
				break;
			} while (!err);
		}
#if 0
		vpbe_enable_dacs(0);
		channel->set_win_enable(0);
		vpbe_enable_venc(0);
#endif

		/* Get the default standard and info about standard */
		enc->std_ops->getstd(&davinci_dm.std, enc);
		davinci_get_std_info();
	}

	if (!channel->usrs)
	{
		/* Configure the default format information */
		davinci_config_default_format(channel->plane_id);
	}

	/* Increment channel usrs counter */
	channel->usrs++;
	/* Set io_allowed member to false */
	fh->io_allowed = 0;
	/* Initialize priority of this instance to default priority */
	fh->prio = V4L2_PRIORITY_UNSET;
	v4l2_prio_open(&channel->prio, &fh->prio);
	dev_dbg(davinci_display_dev,"</davinci_open>\n");
	return 0;
}

/*
 *=====davinci_release=====*/
/* This function deletes buffer queue, frees the buffers and the vpbe file
 * handle */
static int davinci_release(struct inode *inode, struct file *filep)
{
	/* Get the channel object and file handle object */
	struct davinci_fh *fh = filep->private_data;
	struct display_obj *channel = fh->channel;
	struct encoder_device *enc = davinci_dm.encoder[davinci_dm.current_encoder];
	enum vpbe_plane_id other_plane;
	
	other_plane = (channel->plane_id == VPBE_PLANE_VIDEO0) ? VPBE_PLANE_VIDEO1:VPBE_PLANE_VIDEO0;

	dev_dbg(davinci_display_dev,"<davinci_release>\n");
	/* If this is doing IO and other channels are not closed */
	if ((channel->usrs != 1) && fh->io_allowed) {
		dev_dbg(davinci_display_dev,"Close other instances\n");
		return -EAGAIN;
	}
	/* Get the lock on channel object */
	down_interruptible(&davinci_dm.lock);
	/* if this instance is doing IO */
	if (fh->io_allowed) {
		/* Reset io_usrs member of channel object */
		channel->io_usrs = 0;
		channel->set_win_enable(0);
		channel->started = 0;
		/* Free buffers allocated */
		videobuf_queue_cancel(&channel->buffer_queue);
	}

	/* Decrement channel usrs counter */
	channel->usrs--;
	/* If this file handle has initialize encoder device, reset it */
	if(fh->initialized){
		enc->deinitialize(enc);
		davinci_dm.initialized = 0;

	}

#if 0
	if (!channel->usrs && !davinci_dm.dev[other_plane]->usrs)
	{
		vpbe_enable_vpbe_output(0);
	}
#endif

	if (!channel->usrs)
	{
	     channel->set_win_enable(0);	
	}

	/* Close the priority */
	v4l2_prio_close(&channel->prio, &fh->prio);
	filep->private_data = NULL;
	fh->initialized = 0;
	/* Free memory allocated to file handle object */
	if (!ISNULL(fh))
		kfree(fh);
	/* unlock semaphore on channel object */
	up(&davinci_dm.lock);
	dev_dbg(davinci_display_dev,"</davinci_release>\n");
	return 0;
}

static void davinci_platform_release(struct device
				  *device)
{
	/* This is called when the reference count goes to zero */
}

static struct file_operations davinci_fops = {
	.owner = THIS_MODULE,
	.open = davinci_open,
	.release = davinci_release,
	.ioctl = davinci_ioctl,
	.mmap = davinci_mmap
};
static struct video_device davinci_video_template = {
	.name = "davinci",
	.type = VID_TYPE_CAPTURE,
	.hardware = 0,
	.fops = &davinci_fops,
	.minor = -1
};

/*
 *=====davinci_probe=====*/
/* This function creates device entries by register itself to the V4L2 driver
 * and initializes fields of each channel objects */
static __init int davinci_probe(struct device *device)
{
	int i, j = 0, k, err = 0, index = 0;
	struct video_device *vbd = NULL;
	struct display_obj *channel = NULL;
	struct platform_device *pdev;

	davinci_display_dev = device;

	dev_dbg(davinci_display_dev,"<davinci_probe>\n");

	/* First request memory region for io */
   	pdev = to_platform_device(device);
   	if (pdev->num_resources != 0) {
      	dev_err(davinci_display_dev,"probed for an unknown device\n");
      	return -ENODEV;
   	}

	vpbe_get_hardware_address_range(&davinci_dm.mmio_base_phys,&davinci_dm.mmio_size);

   	if (!request_mem_region(davinci_dm.mmio_base_phys, davinci_dm.mmio_size, DAVINCI_DISPLAY_DRIVER)) {
      dev_err(davinci_display_dev,": cannot reserve MMIO region\n");
      return -ENODEV;
   	}

   	/* map the regions */
   	davinci_dm.mmio_base = (unsigned long)ioremap(davinci_dm.mmio_base_phys, davinci_dm.mmio_size);
   	if (!davinci_dm.mmio_base) {
      	dev_err(davinci_display_dev,": cannot map MMIO\n");
      	goto release_mmio;
   	}

	
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		/* Get the pointer to the channel object */
		channel = davinci_dm.dev[i];
		/* Allocate memory for video device */
		vbd = video_device_alloc();
		if (ISNULL(vbd)) {
			for (j = 0; j < i; j++) {
				video_device_release
				    (davinci_dm.dev[j]->video_dev);
			}
      		dev_err(davinci_display_dev,"ran out of memory\n");
			return -ENOMEM;
		}

		/* Initialize field of video device */
		*vbd = davinci_video_template;
		vbd->dev = device;
		vbd->release = video_device_release;
		snprintf(vbd->name, sizeof(vbd->name),
			 "DaVinci_VPBEDisplay_DRIVER_V%d.%d.%d",
			 (VPBE_DISPLAY_VERSION_CODE >> 16)
			 & 0xff,
			 (VPBE_DISPLAY_VERSION_CODE >> 8) &
			 0xff, (VPBE_DISPLAY_VERSION_CODE) & 0xff);
		/* Set video_dev to the video device */
		channel->video_dev = vbd;
	}

	for (j = 0; j < VPBE_DISPLAY_MAX_PLANES; j++) {
		channel = davinci_dm.dev[j];
		/* Initialize field of the channel objects */
		channel->usrs = channel->io_usrs = davinci_dm.std = 0;
		channel->started = davinci_dm.initialized = 0;
		channel->plane_id = j;
#if 0
		if (j < 2)
			channel->numbuffers
			       	= display_buf_config_params.numbuffers[channel->channel_id];
		else
			channel->numbuffers = 0;
#endif

		davinci_dm.current_encoder= 0;
		davinci_dm.numencoders = 0;

		for (index = 0; index < VPBE_DISPLAY_NUM_ENCODERS; index++) {
			davinci_dm.encoder[index] = NULL;
		}

		/* Initialize prio member of channel object */
		v4l2_prio_init(&channel->prio);
		/* Initialize mutex */
		init_MUTEX(&davinci_dm.lock);

		/* register video device */
		printk(KERN_NOTICE "Trying to register davinci display video device.\n");
		printk(KERN_NOTICE "channel=%x,channel->video_dev=%x\n",
			(int) channel, (int) &channel->video_dev);

		err = video_register_device(channel->
						  video_dev,
						  VFL_TYPE_GRABBER,
						  davinci_display_nr[j]);
		if (err)
			goto probe_out;
	}

	vpbe_set_display_default();
	vpbe_set_interrupt_register();
	return 0;

      probe_out:
	for (k = 0; k < j; k++) {
		/* Get the pointer to the channel object */
		channel = davinci_dm.dev[k];
		/* Unregister video device */
		video_unregister_device(channel->video_dev);
		/* Release video device */
		video_device_release(channel->video_dev);
		channel->video_dev = NULL;
	}
	iounmap((void *)davinci_dm.mmio_base);
	
	release_mmio:
	release_mem_region(davinci_dm.mmio_base_phys, davinci_dm.mmio_size);
	dev_dbg(davinci_display_dev,"</davinci_probe>\n");
	return err;
}

/*
 * ===== davinci_remove =====*/
/* It un-register hardware planes from V4L2 driver */
static int davinci_remove(struct device *device)
{
	int i;
	struct display_obj *plane;
	dev_dbg(davinci_display_dev,"<davinci_remove>\n");
	/* un-register device */
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		/* Get the pointer to the channel object */
		plane = davinci_dm.dev[i];
		/* Unregister video device */
		video_unregister_device(plane->video_dev);

		plane->video_dev = NULL;
	}
	if (davinci_dm.mmio_base)
		iounmap((void *)davinci_dm.mmio_base);
	release_mem_region(davinci_dm.mmio_base_phys, davinci_dm.mmio_size);

	dev_dbg(davinci_display_dev,"</davinci_remove>\n");
	return 0;
}

static struct device_driver davinci_driver = {
	.name = DAVINCI_DISPLAY_DRIVER,
	.bus = &platform_bus_type,
	.probe = davinci_probe,
	.remove = davinci_remove,
};
static struct platform_device _davinci_display_device = {
	.name = DAVINCI_DISPLAY_DRIVER,
	.id = 1,
	.dev = {
		.release = davinci_platform_release,
	}
};


/*
 *=====davinci_init=====*/
/* This function registers device and driver to the kernel, requests irq
 * handler and allocates memory for channel objects */
static __init int davinci_init(void)
{
	int err = 0, i, j;
	int free_channel_objects_index;
	int free_irq_no_index;
	int free_buffer_channel_index;
	int free_buffer_index;
	u32 addr;
	int size;

	printk(KERN_INFO "<davinci_init>\n");

	/* Default number of buffers should be 3 */
	if((video0_numbuffers > 0) && 
		(video0_numbuffers < display_buf_config_params.min_numbuffers))
		video0_numbuffers = display_buf_config_params.min_numbuffers;
	if((video1_numbuffers > 0) && 
		(video1_numbuffers < display_buf_config_params.min_numbuffers))
		video1_numbuffers = display_buf_config_params.min_numbuffers;

	/* Set buffer size to min buffers size if invalid buffer size is
	 * given */

	if (video0_bufsize <
		display_buf_config_params.min_bufsize[VPBE_PLANE_VIDEO0])
		video0_bufsize =
			display_buf_config_params.min_bufsize[VPBE_PLANE_VIDEO0];

	if (video1_bufsize <
		display_buf_config_params.min_bufsize[VPBE_PLANE_VIDEO1])
		video1_bufsize =
			display_buf_config_params.min_bufsize[VPBE_PLANE_VIDEO1];

	if(video0_numbuffers) {
		display_buf_config_params.numbuffers[VPBE_PLANE_VIDEO0] = video0_numbuffers;
	}
	if(video1_numbuffers) {
		display_buf_config_params.numbuffers[VPBE_PLANE_VIDEO1] = video1_numbuffers;
	}

	/* Allocate memory for four plane display objects */
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		davinci_dm.dev[i] =
		    kmalloc(sizeof(struct display_obj), GFP_KERNEL);
		/* If memory allocation fails, return error */
		if (!davinci_dm.dev[i]) {
			free_channel_objects_index = i;
			printk(KERN_ERR "ran out of memory\n");
			err = -ENOMEM;
			goto vpbe_init_free_channel_objects;
		}
		davinci_dm.dev[i]->irqlock = SPIN_LOCK_UNLOCKED;
		switch((enum vpbe_plane_id)i) {
			case VPBE_PLANE_VIDEO0:
				vpbe_get_vid0_window_defaults(&davinci_dm.dev[i]->window_par);
				davinci_set_vid0_default_conf();
				davinci_dm.dev[i]->set_win_size=vpbe_set_vid0_win_size ;
				davinci_dm.dev[i]->set_sdram_addr=vpbe_set_vid0_sdram_address;
				davinci_dm.dev[i]->set_interlaced=vpbe_set_vid0_interlaced;
				davinci_dm.dev[i]->set_win_enable=vpbe_set_vid0_win_enable;
				break;
			case VPBE_PLANE_VIDEO1:
				vpbe_get_vid1_window_defaults(&davinci_dm.dev[i]->window_par);
				davinci_set_vid1_default_conf();
				davinci_dm.dev[i]->set_win_size=vpbe_set_vid1_win_size ;
				davinci_dm.dev[i]->set_sdram_addr=vpbe_set_vid1_sdram_address;
				davinci_dm.dev[i]->set_interlaced=vpbe_set_vid1_interlaced;
				davinci_dm.dev[i]->set_win_enable=vpbe_set_vid1_win_enable;
				break;
		}
	}
	free_channel_objects_index = VPBE_DISPLAY_MAX_PLANES;

	/* Allocate memory for buffers */
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		size = display_buf_config_params.channel_bufsize[i];
		for (j = 0; j < display_buf_config_params.numbuffers[i]; j++) {
			addr = davinci_alloc_buffer(size);
			if (!addr) {
				free_buffer_channel_index = i;
				free_buffer_index = j;
				printk(KERN_ERR "ran out of memory\n");
				err = -ENOMEM;
				goto vpbe_init_free_buffers;
			}
			davinci_dm.dev[i]->fbuffers[j] = addr;
		}
	}
	free_buffer_channel_index = VPBE_DISPLAY_MAX_PLANES;
	free_buffer_index = display_buf_config_params.numbuffers[i - 1];
	/* Register driver to the kernel */
	err = driver_register(&davinci_driver);
	if (0 != err) {
		goto vpbe_init_free_buffers;
	}
	/* register device as a platform device to the kernel */
	err = platform_device_register(&_davinci_display_device);
	if (0 != err) {
		goto vpbe_init_unregister_vpbe_driver;
	}
	err =
	    request_irq(IRQ_VENCINT,
			davinci_display_isr,
			SA_INTERRUPT, DAVINCI_DISPLAY_DRIVER,
			(void *)(&(davinci_dm)));
	if (0 != err) {
		free_irq_no_index = j;
		goto vpbe_init_free_irq;

	}
	free_irq_no_index = VPBE_DISPLAY_MAX_PLANES;
	printk(KERN_NOTICE
	       "davinci_init:DaVinci V4L2 Display Driver V1.0 loaded\n");
	printk(KERN_INFO "</davinci_init>\n");
	return 0;
      vpbe_init_free_irq:
	for (j = 0; j < free_irq_no_index; j++) {
		free_irq(IRQ_VENCINT,
		       (void *)(&(davinci_dm)));
	}
	platform_device_unregister(&_davinci_display_device);

      vpbe_init_unregister_vpbe_driver:
	driver_unregister(&davinci_driver);

vpbe_init_free_buffers:
	for (i = 0; i < free_buffer_channel_index; i++) {
		for (j = 0; j < display_buf_config_params.numbuffers[i]; j++) {
			addr = davinci_dm.dev[i]->fbuffers[j];
			if (addr) {
				davinci_free_buffer(addr,
					display_buf_config_params.
					channel_bufsize[i]
					);
				davinci_dm.dev[i]->fbuffers[j] = 0;
			}
		}
	}
	for(j = 0 ; j < display_buf_config_params.numbuffers[free_buffer_index];
		j++) {
		addr = davinci_dm.dev[free_buffer_channel_index]->fbuffers[j];
		if(addr) {
			davinci_free_buffer(addr,
				display_buf_config_params.channel_bufsize[i]);
			davinci_dm.dev[free_buffer_channel_index]->fbuffers[j]
				= 0;
		}

	}

vpbe_init_free_channel_objects:
		for (j = 0; j < free_channel_objects_index; j++) {
		if (davinci_dm.dev[i]) {
			kfree(davinci_dm.dev[j]);
			davinci_dm.dev[i] = NULL;
		}
	}
	return err;
}

/* =====davinci_cleanup=====
 * This function un-registers device and driver to the kernel, frees requested
 * irq handler and de-allocates memory allocated for channel objects.
 * */
static void davinci_cleanup(void)
{
	int i = 0, j = 0;
	u32 addr;
	printk(KERN_INFO "<davinci_cleanup>\n");

	free_irq(IRQ_VENCINT, (void *)(&(davinci_dm)));
	platform_device_unregister(&_davinci_display_device);
	driver_unregister(&davinci_driver);
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		for (j = 0; j < display_buf_config_params.numbuffers[i]; j++) {
			addr = davinci_dm.dev[i]->fbuffers[j];
			if (addr) {
				davinci_free_buffer(addr,
					 display_buf_config_params.
					 channel_bufsize[i]);
				davinci_dm.dev[i]->fbuffers[j] = 0;
			}
		}
	}
	for (i = 0; i < VPBE_DISPLAY_MAX_PLANES; i++) {
		if (davinci_dm.dev[i]) {
			kfree(davinci_dm.dev[i]);
			davinci_dm.dev[i] = NULL;
		}
	}
	printk(KERN_INFO "</davinci_cleanup>\n");
}

/*
 *=====vpbe_enum_std=====*/
/* Function to enumerate all the standards in all the registered encoders */
static int davinci_enum_std(void)
{
	int i, j, index;
	struct v4l2_standard standard;
	struct encoder_device *enc;
	v4l2_std_id all_std = 0;
	int ret = 0;
	davinci_dm.count_std = 0;

	/* For all the registered encoders */
	for(i = 0 ; i < davinci_dm.numencoders ; i ++) {
		enc = davinci_dm.encoder[i];
		/* Do the enumstd */
		for(j = 0; j < enc->std_ops->count; j ++) {
			standard.index = j;
			ret = enc->std_ops->enumstd(&standard, enc);
			
			if(ret)
				return ret;
			
			/* If the standard is already added, 
			 * do not add it to the table */
			if(all_std & standard.id)
				continue;
			/* Store the standard information in the table */
			index = davinci_dm.count_std;
			davinci_dm.std_tbl[index].enc_idx = i;
			davinci_dm.std_tbl[index].std_idx = j;
			davinci_dm.std_tbl[index].std = standard.id;
			davinci_dm.count_std  ++;
			all_std |= standard.id;
		}
	}
	return 0;
}

/*
 *=====davinci_register_encoder=====*/
/* This function will be called by the encoder driver to register its
 * functionalities to vpbe driver */
int davinci_register_encoder(struct encoder_device
			  *encoder)
{
	int err = -EINVAL;

	printk(KERN_NOTICE "<davinci_register_encoder>\n");
	if(ISNULL(encoder)){
		return err;
	}
	if (davinci_dm.numencoders < VPBE_DISPLAY_NUM_ENCODERS) {
		davinci_dm.encoder[davinci_dm.numencoders++] = encoder;
		err = davinci_enum_std();
		return err;
	}

	printk(KERN_NOTICE "</davinci_register_encoder>\n");
	return err;
}

/*
 *=====vpbe_unregister_encoder=====*/
/* This function will be called by the encoder driver to un-register its
 * functionalities to vpbe driver */
int davinci_unregister_encoder(struct encoder_device
			    *encoder)
{
	int i, j = 0, err = 0;
	printk(KERN_NOTICE "<davinci_unregister_encoder>\n");

	for (i = 0; i < davinci_dm.numencoders; i++) {
		if (encoder == davinci_dm.encoder[i]) {
			if (davinci_dm.encoder[davinci_dm.current_encoder] == encoder && davinci_dm.initialized)
				return -EBUSY;
			davinci_dm.encoder[i] = NULL;
			for (j = i; j < davinci_dm.numencoders - 1; j++)
				davinci_dm.encoder[j] = davinci_dm.encoder[j + 1];
			davinci_dm.numencoders--;
			err = davinci_enum_std();
			break;
		}
	}
	printk(KERN_NOTICE "</davinci_unregister_encoder>\n");
	return err;
}

EXPORT_SYMBOL(davinci_register_encoder);
EXPORT_SYMBOL(davinci_unregister_encoder);
EXPORT_SYMBOL(davinci_display_dev);
MODULE_LICENSE("GPL");
/* Function for module initialization and cleanup */
module_init(davinci_init);
module_exit(davinci_cleanup);
