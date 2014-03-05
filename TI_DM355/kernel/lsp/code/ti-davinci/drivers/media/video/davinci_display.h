/*
 * Copyright (C) 2005 Texas Instruments Inc
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* davincihd_display.h */

#ifndef DAVINCI_DISPLAY_H
#define DAVINCI_DISPLAY_H

#ifdef __KERNEL__

/* Header files */
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <media/video-buf.h>
#include "vid_encoder_if.h"
#include "display_vpbe.h"
#endif

#ifdef __KERNEL__

#define VPBE_DISPLAY_NUM_ENCODERS        4

/* Macros */
#define VPBE_MAJOR_RELEASE              0
#define VPBE_MINOR_RELEASE              0
#define VPBE_BUILD                      1
#define VPBE_DISPLAY_VERSION_CODE       ((VPBE_MAJOR_RELEASE<<16) | \
	(VPBE_MINOR_RELEASE<<8)  | \
	VPBE_BUILD)


#define VPBE_VALID_FIELD(field)   ((V4L2_FIELD_ANY == field) || (V4L2_FIELD_INTERLACED == field))


#define VPBE_DISPLAY_MAX_PLANES        2 /* One for each of VID0, VID1 */
#define VPBE_MAX_ENCODER_STD		50

/* Macros */
#define ISNULL(p)       ((NULL) == (p))
#define ISALIGNED(a)    (0 == (a%8))

struct zoom_params
{
    u_int32_t window_id;
    u_int32_t zoom_h;
    u_int32_t zoom_v;
};  

/* structures */
/* Table to keep track of the standards supported in all the decoders */
struct vpbe_encoder_std_tbl {
	u8 enc_idx;
	u8 std_idx;
	v4l2_std_id std;
};

/* display object structure */
struct display_obj {
	/* Buffer specific parameters */
	u32 fbuffers[VIDEO_MAX_FRAME];	/* List of buffer pointers for
					   storing frames */
	u32 numbuffers;		/* number of buffers in fbuffers */
	struct videobuf_buffer *curFrm;	/* Pointer pointing to current
					   v4l2_buffer */
	struct videobuf_buffer *nextFrm;	/* Pointer pointing to current
						   v4l2_buffer */
	/* videobuf specific parameters */
	struct videobuf_queue buffer_queue;	/* Buffer queue used in
						   video-buf */
	struct list_head dma_queue;	/* Queue of filled frames */
	spinlock_t irqlock;	/* Used in video-buf */


	/* V4l2 specific parameters */
	struct video_device *video_dev;	/* Identifies video device for
					   this channel */
	enum v4l2_memory memory;	/* This field keeps track of type
					   of buffer exchange mechanism user
					   has selected */
	struct v4l2_prio_state prio;	/* Used to keep track of state of
					   the priority */
	struct v4l2_pix_format fmt;	/* Used to store pixel format */
	enum v4l2_field buf_field;
	struct vpbe_windowinfo window_par;

	/* vpbe specific parameters */
	unsigned char window_enable;	/*enabke window for display */
	vpbe_conf_params_t conf_params;
	struct zoom_params zoom;	/*Zooming parameters */
	unsigned int 	field_frame_select; /* interlaced or none */

	u32 usrs;		/* number of open instances of
				   the channel */
	u32 io_usrs;		/* number of users performing
				   IO */
	u32 field_id;		/* Indicates id of the field
				   which is being displayed */

	u8 started;		/* Indicates whether streaming
				   started */
	enum vpbe_plane_id plane_id;	/* Identifies device object*/

	/* Function pointer to set the addresses */
	void (*set_sdram_addr) (u32, u32);
	/* For this plane, set defaults in vpbe registers */
	void (*set_win_size)(u32,u32, u32, u32, u32);
	/* setting interlace or non interlace */
	void (*set_interlaced)(unsigned int);
	/* Enable/disable window */
	void (*set_win_enable)(unsigned int);
};

/* File handle structure */
struct davinci_fh {
	struct display_obj *channel; /* pointer to channel object for
					opened device */
	u8 io_allowed;		 /* Indicates whether this file handle
				    is doing IO */
	enum v4l2_priority prio; /* Used to keep track priority of
				    this instance */
	u8 initialized;		 /* Used to keep track of whether this 
				    file handle has initialized encoder or not*/
};

/*  Structure for background color  */
typedef struct vpbe_backg_color {
	unsigned char clut_select;	/* 2: RAM CLUT 1:ROM1 CLUT 0:ROM0 CLUT */
	unsigned char color_offset;	/* index of color */
} vpbe_backg_color_t;

/* Structure to hold window position */
typedef struct vpbe_window_position {
	unsigned int xpos;	/* X position of the window */
	unsigned int ypos;	/* Y position of the window */
} vpbe_window_position_t;


/* vpbe device structure */
struct davinci_display {
	/* channel specifc parameters */
	struct semaphore lock;	/* lock used to access this
				   structure */
	struct encoder_device *encoder[VPBE_DISPLAY_NUM_ENCODERS];
	struct vpbe_display_stdinfo std_info;	/* Keeps track of the
						   information about the
						   standard */
	int count_std; 				   /* Variable used to keep 
						      track of number of
						      standards in the 
						      std_tbl */
	v4l2_std_id std;	/* Currently selected or default
				   standard */
	struct vpbe_encoder_std_tbl std_tbl[VPBE_MAX_ENCODER_STD]; /* Table of 
								      standards
								      supported
								      by all
						      		      decoders 
								      */
	u8 numencoders;		/* indicates number of encoders
				   registered to VPBE-V4L2 */
	u8 current_encoder;	/* Index of the currently
				   selected encoder */
	u8 initialized;		/* flag to indicate whether
				   encoder is initialized */
	vpbe_backg_color_t backg;	/* background color */
	//struct video_config_param video_config;	/*Cuurent videomode configuration*/
   	/* to map the registers */
   	dma_addr_t mmio_base_phys;
   	unsigned long mmio_base;
   	unsigned long mmio_size;
	struct display_obj *dev[VPBE_DISPLAY_MAX_PLANES];
};

struct buf_config_params {
	u8 min_numbuffers;
	u8 numbuffers[VPBE_DISPLAY_MAX_PLANES];
	u32 min_bufsize[VPBE_DISPLAY_MAX_PLANES];
	u32 channel_bufsize[VPBE_DISPLAY_MAX_PLANES];
};

#endif	/* End of __KERNEL__ */

/* IOCTLs */

#define VPBE_S_VPBE_PARAMS   _IOW('V', BASE_VIDIOC_PRIVATE+1, struct vpbe_interface)
#define VPBE_G_VPBE_PARAMS   _IOR('V', BASE_VIDIOC_PRIVATE+2, struct vpbe_interface)

#define VPBE_CMD_S_ENCODER_PARAMS _IOW('V',BASE_VIDIOC_PRIVATE+9, \
	void *)
#define VPBE_CMD_G_ENCODER_PARAMS _IOR('V',BASE_VIDIOC_PRIVATE+8, \
	void *)

#endif	/* DAVINCI_DISPLAY_H */
