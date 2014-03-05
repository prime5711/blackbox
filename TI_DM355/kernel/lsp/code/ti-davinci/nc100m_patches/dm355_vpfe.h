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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* dm355_vpfe.h */

#ifndef DM355_VPFE_H
#define DM355_VPFE_H

#include <media/ccdc_dm355.h>
#include <media/tvp5146.h>
#include <media/tvp5150.h>
#include <media/mt9t001.h>
#include <media/mt9p031.h>
#include <media/mt9m131.h>
#include <media/tvp7002.h>

#define TRUE 1
#define FALSE 0

#define MAX_STREAM_EXT_NUM	(2)

typedef struct	_Res_Set{
int	width;
int	height;
int	index;
int	diff;
}Res_Set;



typedef struct _stream_ext_params{
	int	ext_res_num;
	Res_Set	res_setting[MAX_STREAM_EXT_NUM];
} stream_ext_params;

#define VPFE_EXT_HEADER_LEN	( ((sizeof(Res_Set)+31)/32)*32 )

/* vpfe specific video standards */
#define VPFE_STD_625_50_SQP ((V4L2_STD_625_50)<<32)
#define VPFE_STD_525_60_SQP ((V4L2_STD_525_60)<<32)
#define VPFE_STD_AUTO ((v4l2_std_id)(0x1000000000000000ULL))
#define VPFE_STD_AUTO_SQP ((v4l2_std_id)(0x2000000000000000ULL))

#define VPFE_CMD_CONFIG_CCDC_YCBCR \
			_IOW('V',BASE_VIDIOC_PRIVATE + 1,ccdc_params_ycbcr)
#define VPFE_CMD_LATEST_FRM_ONLY \
			_IOW('V',BASE_VIDIOC_PRIVATE + 2,int)
#define VPFE_CMD_CONFIG_TVP5146 \
			_IOW('V',BASE_VIDIOC_PRIVATE + 3,tvp5146_params)
#define VPFE_CMD_CONFIG_TVP5150 \
			_IOW('V',BASE_VIDIOC_PRIVATE + 3,tvp5150_params)

#define VPFE_CMD_CONFIG_MT9M131 \
			_IOW('V',BASE_VIDIOC_PRIVATE + 3,mt9m131_params)
#define VPFE_CMD_CONFIG_TVP7002 \
			_IOW('V',BASE_VIDIOC_PRIVATE + 3,tvp7002_params)
			
#define VPFE_CMD_CONFIG_CCDC_RAW \
			_IOW('V',BASE_VIDIOC_PRIVATE + 4,ccdc_config_params_raw)
#define VPFE_CMD_S_MT9T001_PARAMS \
			_IOW('V',BASE_VIDIOC_PRIVATE + 5,struct mt9t001_params)
#define VPFE_CMD_G_MT9T001_PARAMS \
			_IOR('V',BASE_VIDIOC_PRIVATE + 6,struct mt9t001_params)

#define VPFE_CMD_S_EXT_RES \
			_IOW('V',BASE_VIDIOC_PRIVATE + 7,stream_ext_params)


/* Define for extra pixel/line and extra lines/frame */
#define NUM_EXTRAPIXELS		0//16
#define NUM_EXTRALINES		0//8

/* settings for commonly used video formats */
#define VPFE_WIN_NTSC    {0,0,720,480}
#define VPFE_WIN_PAL     {0,0,720,576}
#define VPFE_WIN_NTSC_SP {0,0,640,480}	/* ntsc square pixel */
#define VPFE_WIN_PAL_SP  {0,0,768,576}	/* pal square pixel */
#define VPFE_WIN_CIF     {0,0,352,288}
#define VPFE_WIN_QCIF    {0,0,176,144}
#define VPFE_WIN_QVGA    {0,0,320,240}
#define VPFE_WIN_SIF     {0,0,352,240}

#define VPFE_WIN_SXVGA   			{0,0,1280,1024}
#define VPFE_WIN_SXVGA_960   {0,0,1280,960}

#define VPFE_WIN_VGA	{0,0,(640 + NUM_EXTRAPIXELS),(480 + NUM_EXTRALINES)}
#define VPFE_WIN_VGA_512	{0,0,(640 + NUM_EXTRAPIXELS),(512 + NUM_EXTRALINES)}

#define VPFE_WIN_SVGA 	{0,0,(800 + NUM_EXTRAPIXELS),(600 + NUM_EXTRALINES)}
#define VPFE_WIN_XGA	{0,0,(1024+ NUM_EXTRAPIXELS),(768 + NUM_EXTRALINES)}
#define VPFE_WIN_480p	{0,0,(720 + NUM_EXTRAPIXELS),(480 + NUM_EXTRALINES)}
#define VPFE_WIN_576p	{0,0,(720 + NUM_EXTRAPIXELS),(576 + NUM_EXTRALINES)}
#define VPFE_WIN_720p 	{0,0,(1280+ NUM_EXTRAPIXELS),(720 + NUM_EXTRALINES)}
#define VPFE_WIN_1080p 	{0,0,(1920),(1080)}

/* V4L2 defines added for new standards, 
					  these will be moved to videodev2.h */
#define V4L2_STD_MT9T001_VGA_30FPS	(10)
#define V4L2_STD_MT9T001_VGA_60FPS	(11)
#define V4L2_STD_MT9T001_VGA_BIN_30FPS	(12)
#define V4L2_STD_MT9T001_SVGA_30FPS	(13)
#define V4L2_STD_MT9T001_SVGA_60FPS	(14)
#define V4L2_STD_MT9T001_XGA_30FPS	(15)
#define V4L2_STD_MT9T001_480p_30FPS	(16)
#define V4L2_STD_MT9T001_480p_60FPS	(17)
#define V4L2_STD_MT9T001_576p_25FPS	(18)
#define V4L2_STD_MT9T001_576p_50FPS	(19)
#define V4L2_STD_MT9T001_720p_24FPS	(20)
#define V4L2_STD_MT9T001_720p_30FPS	(21)
#define V4L2_STD_MT9T001_1080p_18FPS	(22)
#define V4L2_STD_MT9T001_SXVGA_15FPS	(23)

#define V4L2_STD_MT9M131_VGA_30FPS	(10)
#define V4L2_STD_MT9M131_VGA_60FPS	(11)
#define V4L2_STD_MT9M131_VGA_BIN_30FPS	(12)
#define V4L2_STD_MT9M131_SVGA_30FPS	(13)
#define V4L2_STD_MT9M131_SVGA_60FPS	(14)
#define V4L2_STD_MT9M131_XGA_30FPS	(15)
#define V4L2_STD_MT9M131_480p_30FPS	(16)
#define V4L2_STD_MT9M131_480p_60FPS	(17)
#define V4L2_STD_MT9M131_576p_25FPS	(18)
#define V4L2_STD_MT9M131_576p_50FPS	(19)
#define V4L2_STD_MT9M131_720p_24FPS	(20)
#define V4L2_STD_MT9M131_720p_30FPS	(21)
#define V4L2_STD_MT9M131_1080p_18FPS	(22)
#define V4L2_STD_MT9M131_SXVGA_15FPS	(23) // 1280 x 1024 15 frame

//added by xact 2008.11.14
#define V4L2_STD_MT9P031_MODE_480p_30FPS	(16)
#define V4L2_STD_MT9M131_MODE_480p_30FPS	(16)
/* Define for device type to be passed in init */
#define 	MT9T001	0
#define		TVP5146	1
#define 	MT9T031	2
#define 	MT9P031	3
#define		TVP5150 4 
#define		MT9M131 5 
#define		TVP7002	6

#ifdef __KERNEL__

#include <media/video-buf.h>

#define VPFE_MAJOR_RELEASE 0
#define VPFE_MINOR_RELEASE 0
#define VPFE_BUILD         1

#define VPFE_VERSION_CODE \
     (VPFE_MAJOR_RELEASE<<16)  | (VPFE_MINOR_RELEASE<<8) | VPFE_BUILD

/* By default, the driver is setup for auto-swich mode */
#define VPFE_DEFAULT_STD VPFE_STD_AUTO

#define VPFE_PIXELASPECT_NTSC 		{11, 10}
#define VPFE_PIXELASPECT_PAL  		{54, 59}
#define VPFE_PIXELASPECT_NTSC_SP    	{1, 1}
#define VPFE_PIXELASPECT_PAL_SP     	{1, 1}
#define VPFE_PIXELASPECT_DEFAULT    	{1, 1}

/* Buffer size defines for TVP5146 and MT9T001 */
#define VPFE_TVP5146_MAX_FRAME_WIDTH      768	/* for PAL Sqpixel mode */
#define VPFE_TVP5146_MAX_FRAME_HEIGHT     576	/* for PAL              */
/* Buffer size defines for TVP5150 added by xact 2008.09.04*/
#define VPFE_TVP5150_MAX_FRAME_WIDTH      768	/* for PAL Sqpixel mode */
#define VPFE_TVP5150_MAX_FRAME_HEIGHT     576	/* for PAL              */
#define VPFE_TVP7002_MAX_FRAME_WIDTH      (1920)	/* for PAL Sqpixel mode *///1920
#define VPFE_TVP7002_MAX_FRAME_HEIGHT     (1080)	/* for PAL              *///720

/* 4:2:2 data */
#define VPFE_TVP5146_MAX_FBUF_SIZE      \
		 (VPFE_TVP5146_MAX_FRAME_WIDTH*VPFE_TVP5146_MAX_FRAME_HEIGHT*2)
//added by xact 2008.09.04
#define VPFE_TVP5150_MAX_FBUF_SIZE      \
		 (VPFE_TVP5150_MAX_FRAME_WIDTH*VPFE_TVP5150_MAX_FRAME_HEIGHT*2)
#define VPFE_TVP7002_MAX_FBUF_SIZE      \
		 (VPFE_TVP7002_MAX_FRAME_WIDTH*VPFE_TVP7002_MAX_FRAME_HEIGHT*2)

#define VPFE_MT9T001_MAX_FRAME_WIDTH     (1920)
#define VPFE_MT9T001_MAX_FRAME_HEIGHT    (1080)

#define VPFE_MT9M131_MAX_FRAME_WIDTH     (1280)
#define VPFE_MT9M131_MAX_FRAME_HEIGHT    (1024)

/* 2 BYTE FOR EACH PIXEL */
#define VPFE_MT9T001_MAX_FBUF_SIZE       \
		(VPFE_MT9T001_MAX_FRAME_WIDTH*VPFE_MT9T001_MAX_FRAME_HEIGHT*2)

#define VPFE_MT9M131_MAX_FBUF_SIZE       \
		(VPFE_MT9M131_MAX_FRAME_WIDTH*VPFE_MT9M131_MAX_FRAME_HEIGHT*2)

/* frame buffers allocate at driver initialization time */
#define VPFE_DEFNUM_FBUFS             3

/* Defines for number of lines for all formats */
#define FMT_VGA_NUMLINES				(480 + NUM_EXTRALINES)
#define FMT_SVGA_NUMLINES				(600 + NUM_EXTRALINES)
#define FMT_XGA_NUMLINES				(768 + NUM_EXTRALINES)
#define FMT_480p_NUMLINES				(480 + NUM_EXTRALINES)
#define FMT_576p_NUMLINES				(576 + NUM_EXTRALINES)
#define FMT_720p_NUMLINES				(720 + NUM_EXTRALINES)
#define FMT_1080i_NUMLINES				(1080)

/* Defines for FPS values */
#define FPS_18_NUMERATOR				(1)
#define FPS_18_DENOMIRATOR				(18)
#define FPS_24_NUMERATOR				(1)
#define FPS_24_DENOMIRATOR  				(24)
#define FPS_25_NUMERATOR				(1)
#define FPS_25_DENOMIRATOR  				(25)
#define FPS_30_NUMERATOR				(1)
#define FPS_30_DENOMIRATOR  				(30)
#define FPS_50_NUMERATOR    				(1)
#define FPS_50_DENOMIRATOR  				(50)
#define FPS_60_NUMERATOR    				(1)
#define FPS_60_DENOMIRATOR  				(60)

#define FP_NUM_BYTES					(4)
/* Device configuration function pointer */
typedef int (*device_cfg_fxn) (unsigned int cmd, void *arg, void *params);

typedef struct vpfe_obj {
	struct video_device *video_dev;
	struct videobuf_queue bufqueue;	/* queue with frame buffers      */
	struct list_head dma_queue;
	u32 latest_only;	/* indicate whether to return the most */
	/* recent captured buffers only        */
	u32 usrs;
	u32 io_usrs;
	struct v4l2_prio_state prio;
	v4l2_std_id std;
	struct v4l2_rect vwin;
	struct v4l2_rect bounds;
	struct v4l2_fract pixelaspect;
	spinlock_t irqlock;
	struct semaphore lock;
	enum v4l2_field field;
	u32 pixelfmt;
	u32 numbuffers;
	u8 *fbuffers[VIDEO_MAX_FRAME];
	struct videobuf_buffer *curFrm;
	struct videobuf_buffer *nextFrm;
	int field_id;
	int mode_changed;
	int started;
	int field_offset;
	unsigned char capture_device;	/*set in vpfe_init()  (1=TVP5146, 0=MT9T001) */
	tvp5150_params tvp5150_params;
	tvp7002_params tvp7002_params;
	//mt9m131_params mt9m131_params;
	void *device_params;	/*Pointer for video device. (MT9T001) */
	ccdc_params_raw ccdc_params_raw;	/*Object for CCDC raw mode */
	ccdc_params_ycbcr ccdc_params_ycbcr;	/*Object for CCDC ycbcr mode */
	device_cfg_fxn config_dev_fxn;	/*where,typedef int(*deviceCfg_Fxn)
					   (Uns cmd, void *arg, void *params); */

} vpfe_obj;

/* file handle */
typedef struct vpfe_fh {
	struct vpfe_obj *dev;
	int io_allowed;
	enum v4l2_priority prio;
} vpfe_fh;
#endif

#endif				/* DM355_VPFE_H */
