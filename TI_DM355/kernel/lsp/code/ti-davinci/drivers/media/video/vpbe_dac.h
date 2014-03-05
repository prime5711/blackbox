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

#ifndef VPBE_DAC_H
#define VPBE_DAC_H

#ifdef __KERNEL__
/* Kernel Header files */
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#endif


typedef struct {
	v4l2_std_id std;
	int outputidx;
}vpbe_dac_params;

#ifdef __KERNEL__

#define VPBE_DAC_COMPOSITE_OUTPUT_NAME  	"VPBEDAC_Composite"
#define VPBE_DAC_SVIDEO_OUTPUT_NAME  		"VPBEDAC_SVIDEO"
#define VPBE_DAC_COMPONENT_OUTPUT_NAME  	"VPBEDAC_Component"

#define VPBE_DAC_COMPOSITE_ID		0
#define VPBE_DAC_SVIDEO_ID		    1
#define VPBE_DAC_COMPONENT_ID		2

#define VPBE_DAC_COMPOSITE_STD		(V4L2_STD_525_60 | V4L2_STD_625_50)
#define VPBE_DAC_SVIDEO_STD			(V4L2_STD_525_60 | V4L2_STD_625_50)
#define VPBE_DAC_COMPONENT_STD		(V4L2_STD_525_60 | V4L2_STD_625_50)

/* encoder standard related strctures */
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
#define VPBE_DAC_MAX_NO_OUTPUTS			(1)
#define VPBE_DAC_COMPONENT_NUM_STD		(0)
#define VPBE_DAC_SVIDEO_NUM_STD		    (0)
#else
#define VPBE_DAC_MAX_NO_OUTPUTS			(3)
#define VPBE_DAC_COMPONENT_NUM_STD		(2)
#define VPBE_DAC_SVIDEO_NUM_STD		    (2)
#endif

#define VPBE_DAC_COMPOSITE_NUM_STD		(2)


struct vpbe_dac_config {
	int no_of_outputs;
	struct {
		int output_type;
		struct v4l2_output output_info;
		int no_of_standard;
		struct v4l2_standard *standard;
	} output[VPBE_DAC_MAX_NO_OUTPUTS];
};

struct vpbe_dac_channel {
	struct encoder_device *enc_device;
	vpbe_dac_params params;
};

/* PUT all DAC register information here
*/

#endif				/* End of #ifdef __KERNEL__ */

#endif				/* End of #ifndef VPBE_DAC_H */
