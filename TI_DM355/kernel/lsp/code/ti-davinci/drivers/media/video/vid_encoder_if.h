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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* vid_encoder_if.h */

/* This file contains data structure which will be used as the interface
 * between vpif and encoder */

#ifndef VID_ENCODER_IF_H
#define VID_ENCODER_IF_H

#ifdef __KERNEL__

#include <linux/videodev.h>
#include <linux/videodev2.h>

#define ENCODER_I2C_BIND_FLAG		(0)
#define ENCODER_FULL_INIT_FLAG		(1)

#define FRAME_FORMAT_INTERLACED         (0)
#define FRAME_FORMAT_PROGRESSIVE        (1)

#define ENCODER_MAX_NAME		30

typedef enum {
	INTERFACE_TYPE_BT656 = 1,
	INTERFACE_TYPE_BT1120 = 2
} vid_display_interface_type;

/* structures */

/* Parameters ops structure */
struct param_ops {
	int (*setparams) (void *params, void *enc);   /* Pointer to function
							 to set parameters */
	int (*getparams) (void *params, void *enc);   /* Pointer to function
							 to get parameters */
};

/* standard ops structure */
struct standard_ops {
	int count;		/* Indicates number of standards
				   supported */
	int (*enumstd) (struct v4l2_standard * argp, 
						void *enc); /* Pointer to 
							       function to
							       enumerate 
							       standard */
	int (*setstd) (v4l2_std_id * argp, void *enc);     /* Pointer to 
							      function to set 
							      standard */
	int (*getstd) (v4l2_std_id * argp, void *enc);     /* Pointer to 
							      function to get 
							      standard */
};

/* format ops structure */
struct format_ops {
	int count;		/* Indicats number of formats
				   supported */
	int (*enumformat) (struct v4l2_fmtdesc * argp, void *enc);/*Pointer 
								    to 
								    function to
								    enumerate
								    formats */
	int (*tryformat) (struct v4l2_format * argp, void *enc); /* Pointer to 
								    function 
								    to try 
								    format */
	int (*setformat) (struct v4l2_format * argp, void *enc); /* Pointer to 
								    function 
								    to set 
								    formats */
	int (*getformat) (struct v4l2_format * argp, void *enc); /* Pointer to 
								    function 
								    to get 
								    formats */
};

/* control ops structure */
struct control_ops {
	int count;		/* Indicats number of controls
				   supported */
	int (*queryctrl) (struct v4l2_queryctrl * argp, 
						void *enc); /* Pointer to 
							       function to 
							       enumerate 
							       controls */
	
	int (*setcontrol) (struct v4l2_control * argp, 
						void *enc); /* Pointer to 
							       function to 
							       set controls */
	int (*getcontrol) (struct v4l2_control * argp, 
						void *enc); /* Pointer to 
							       function to 
							       get controls */
	
};

/* output ops structure */
struct output_ops {
	int count;		/* Indicats number of
				   outputs supported */
	int (*enumoutput) (struct v4l2_output * argp, 
						void *enc); /* Pointer to 
							       function to 
							       query outputs */
	
	int (*setoutput) (int *argp, void *enc);	    /* Pointer to 
							       function to 
							       set outputs */
	int (*getoutput) (int *argp, void *enc);	    /* Pointer to 
							       function to 
							       get outputs */
};

/* encoder device structure */
struct encoder_device {
	u8 name[ENCODER_MAX_NAME];	    /*Name of the
					      encoder device */
	vid_display_interface_type if_type; /*Encoder interface
					      type i.e. BT656 */
	int channel_id;			/* Id of the channel to which
					   encoder is
					   connected */
	u32 capabilities;		/* encoder capabilities, 
					   This is only for VBI
					   capabilities */
	int (*initialize) (void *enc, int flag);	/* Pointer to initialize
							   function to 
							   initialize encoder */
	struct standard_ops *std_ops;	/* Set of functions
					   pointers for
					   standard related
					   functions */
	struct control_ops *ctrl_ops;	/* Set of functions
					   pointers for
					   control related
					   functions */
	struct output_ops *output_ops;	/* Set of functions
					   pointers for 
					   output related
					   functions */
	struct format_ops *fmt_ops;	/* Set of functions
					   pointers for
					   format related
					   functions */
	struct param_ops *params_ops;	/* Set of functions
					   pointers for
					   device specific
					   configs */
	int (*deinitialize) (void *enc); /*Pointer to
					   deinitialize
					   function */
};

int davinci_register_encoder(struct encoder_device
			  *encoder);	/* Function to register encoder to the
					   VPIF-V4L2 layer */
int davinci_unregister_encoder(struct encoder_device
			    *encoder);	/* Function to un-register encoder to
					   the VPIF-V4L2 layer */

#endif				/* #ifdef __KERNEL__ */

#endif				/* #ifdef VID_ENCODER_IF_H */
