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
/* vpbe_dac.c */

/* Kernel Specific header files */


#include <linux/kernel.h>
#include <linux/config.h>
#include <linux/init.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/device.h>
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
#include <video/dm355_vpbe.h>
#else
#include <video/davinci_vpbe.h>
#endif
#include "vpbe_dac.h"
#include "vid_encoder_if.h"
#include <asm/arch/hardware.h>
#include <asm/io.h>

extern struct device *davinci_display_dev;


/* Function prototypes */
static int vpbe_dac_initialize(void *enc, int flag);
static int vpbe_dac_deinitialize(void *enc);

static int vpbe_dac_setstd(v4l2_std_id *, void *enc);
static int vpbe_dac_getstd(v4l2_std_id *, void *enc);
static int vpbe_dac_enumstd(struct v4l2_standard *, void *enc);

static int vpbe_dac_setoutput(int *, void *enc);
static int vpbe_dac_getoutput(int *, void *enc);
static int vpbe_dac_enumoutput(struct v4l2_output *, void *enc);

/*
 * display controller register I/O routines
 */
static __inline__ u32 dispc_reg_in(u32 offset)
{
   return (inl(offset));
}
static __inline__ u32 dispc_reg_out(u32 offset, u32 val)
{
   outl(val, offset);
   return (val);
}
static __inline__ u32 dispc_reg_merge(u32 offset, u32 val, u32 mask)
{
   u32 addr = offset;
   u32 new_val = (inl(addr) & ~mask) | (val & mask);
   outl(new_val, addr);
   return (new_val);
}

static int vpbe_dac_set_dac(unsigned int type)
{

	int error=0;
#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	switch (type)
	{
		case VPBE_DAC_COMPOSITE_ID:
			dev_dbg(davinci_display_dev,"Setting output to Composite\n");
			dispc_reg_out(VENC_DACSEL, 0);
		break;
		case VPBE_DAC_SVIDEO_ID:
			dev_dbg(davinci_display_dev,"Setting output to S-Video\n");
			dispc_reg_out(VENC_DACSEL, 0x210);	
		break;
		case VPBE_DAC_COMPONENT_ID:
			dev_dbg(davinci_display_dev,"Setting output to Component Video\n");
			dispc_reg_out(VENC_DACSEL, 0x543);	
		break;
		default:
			error=-1;
	}

#endif
	return error;
}
			
static struct v4l2_standard
vpbe_dac_composite_standards[VPBE_DAC_COMPOSITE_NUM_STD] = {
	{
		.index = 0,
		.id = V4L2_STD_525_60,
		.name = "NTSC",
		.frameperiod = {1001, 30000},
		.framelines = 525},
	{
		.index = 1,
		.id = V4L2_STD_625_50,
		.name = "PAL",
		.frameperiod = {1, 25},
		.framelines = 625}
};

static struct vpbe_dac_config vpbe_dac_configuration = {
		.no_of_outputs = VPBE_DAC_MAX_NO_OUTPUTS,
		.output[0] = {
			.output_type = VPBE_DAC_COMPOSITE_ID,
			.output_info = {
				.index = 0,
				.name = VPBE_DAC_COMPOSITE_OUTPUT_NAME,
				.type = V4L2_OUTPUT_TYPE_ANALOG,
				.std = VPBE_DAC_COMPOSITE_STD 
				},
		    .no_of_standard = VPBE_DAC_COMPOSITE_NUM_STD,
		    .standard = (struct v4l2_standard *)
			    &vpbe_dac_composite_standards
#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
			},
		.output[1] = {
			.output_type = VPBE_DAC_SVIDEO_ID,
			.output_info = {
				.index = 1,
				.name = VPBE_DAC_SVIDEO_OUTPUT_NAME,
				.type = V4L2_OUTPUT_TYPE_ANALOG,
				.std = VPBE_DAC_SVIDEO_STD
				},
		    .no_of_standard = VPBE_DAC_SVIDEO_NUM_STD,
		    .standard = (struct v4l2_standard *)
			    &vpbe_dac_composite_standards
			},
		.output[2] = {
			.output_type = VPBE_DAC_COMPONENT_ID,
			.output_info = {
				.index = 2,
				.name = VPBE_DAC_COMPONENT_OUTPUT_NAME,
				.type = V4L2_OUTPUT_TYPE_ANALOG,
				.std =  VPBE_DAC_COMPONENT_STD
				},
			.no_of_standard = VPBE_DAC_COMPONENT_NUM_STD,
			.standard = (struct v4l2_standard *)
			    &vpbe_dac_composite_standards
#endif
		}
};

static struct vpbe_dac_channel vpbe_dac_channel_info = {
		.params.outputidx = VPBE_DAC_COMPOSITE_ID,
		.params.std = V4L2_STD_525_60,
		.enc_device = NULL
};


static struct output_ops outputs_ops = {
	.count = VPBE_DAC_MAX_NO_OUTPUTS,
	.enumoutput = vpbe_dac_enumoutput,
	.setoutput = vpbe_dac_setoutput,
	.getoutput = vpbe_dac_getoutput
};
static struct standard_ops standards_ops = {
	.count = VPBE_DAC_COMPOSITE_NUM_STD + VPBE_DAC_COMPONENT_NUM_STD,
	.setstd = vpbe_dac_setstd,
	.getstd = vpbe_dac_getstd,
	.enumstd = vpbe_dac_enumstd
};
static struct encoder_device vpbe_dac_dev = {
		.name = "VPBE_DAC",
		.if_type = INTERFACE_TYPE_BT656,
		.capabilities = 0,
		.initialize = vpbe_dac_initialize,
		.std_ops = &standards_ops,
		.ctrl_ops = NULL,
		.output_ops = &outputs_ops,
		.fmt_ops = NULL,
		.params_ops = NULL,
		.deinitialize = vpbe_dac_deinitialize,
};


/* This function is called by the vpbe driver to initialize ADV7343 driver.
 * It initializes all registers of vpbe_dac with the default values
 */
static int vpbe_dac_initialize(void *enc, int flag)
{
	v4l2_std_id std;
	int err = 0;
	int index;
	if (NULL == enc) {
		printk(KERN_ERR "NULL Pointer\n");
		return -EINVAL;
	}
	vpbe_dac_channel_info.enc_device = (struct encoder_device *) enc;

	printk( "VPBE DAC driver registered\n");

	/* call set standard */
	std = vpbe_dac_channel_info.params.std;
	index = vpbe_dac_channel_info.params.outputidx;
	err |= vpbe_dac_setoutput(&index, enc);
	err |= vpbe_dac_setstd(&std, enc);
	if (err < 0) {
		err = -EINVAL;
		vpbe_dac_deinitialize(enc);
		return err;
	}
	printk( "</vpbe_dac_initialize>\n");
	return err;
}

static int vpbe_dac_deinitialize(void *enc)
{
	if (NULL == enc) {
		printk(KERN_ERR "NULL Pointer\n");
		return -EINVAL;
	}
	vpbe_dac_channel_info.enc_device = NULL;
	return 0;
}



/* Following function is used to set the standard */
static int vpbe_dac_setstd(v4l2_std_id * id, void *enc)
{
	int err = 0;
	int i = 0;
	struct v4l2_standard *standard;
	int output_idx;

	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer\n");
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"Start of vpbe_dac_setstd..\n");
	output_idx = vpbe_dac_channel_info.params.outputidx;

	for (i = 0; i
		<
		vpbe_dac_configuration.output[output_idx].no_of_standard; i++) {
		standard = &vpbe_dac_configuration.output[output_idx].standard[i];
		if (standard->id & *id) {
			break;
		}
	}
	if (i == vpbe_dac_configuration.output[output_idx].no_of_standard) {
		dev_err(davinci_display_dev,"Invalid id...\n");
		return -EINVAL;
	}

	/* Store the standard in global object of vpbe_dac */
	vpbe_dac_channel_info.params.std = *id;

	dev_dbg(davinci_display_dev,"</vpbe_dac_setstd>\n");
	return err;
}

/* Following function is used to get currently selected standard.*/
static int vpbe_dac_getstd(v4l2_std_id * id, void *enc)
{
	int err = 0;
	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer\n");
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"<vpbe_dac_getstd>\n");

	if (id == NULL) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}
	/* Read the video standard */
	*id = vpbe_dac_channel_info.params.std;
	dev_dbg(davinci_display_dev,"</vpbe_dac_getstd>\n");
	return err;
}


/* Following function is used to enumerate the standard. It information about
   the standard. */
static int vpbe_dac_enumstd(struct v4l2_standard *std, void *enc)
{
	int index,index1;
	int err = 0;
	int output_idx,sumstd = 0;
	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer\n");
		return -EINVAL;
	}
	if (std == NULL) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}
	index = std->index;
	index1=index;
	/* Check for valid value of index */
	for(output_idx =0; output_idx < vpbe_dac_configuration.no_of_outputs; output_idx++)
	{
		sumstd += vpbe_dac_configuration.output[output_idx].no_of_standard;
		if(index < sumstd )
		{
			sumstd -= vpbe_dac_configuration.output[output_idx].no_of_standard;
			break;
		}		
	}

	if(output_idx == vpbe_dac_configuration.no_of_outputs)
	{
		return -EINVAL;
	}

	index -= sumstd;

	memset(std, 0, sizeof(*std));
	memcpy(std, &vpbe_dac_configuration.output[output_idx].standard[index], sizeof(struct v4l2_standard));
	std->index = index1;
	return err;
}

/* Following function is used to set output format in VPBE DAC device. The index
   of the output format is  passed as the argument to this function. */
static int vpbe_dac_setoutput(int *index, void *enc)
{
	int err = 0;
	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer\n");
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"<vpbe_dac_setoutput>\n");

	/* check for null pointer */
	if (index == NULL) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}
	if ((*index >= vpbe_dac_configuration.no_of_outputs)
		|| (*index < 0)) {
		return -EINVAL;
	}

	if (vpbe_dac_set_dac(*index) < 0) {
		dev_err(davinci_display_dev,"<vpbe_dac_setoutput, error in setting DAC config>\n");
		return -EINVAL;
	}

	vpbe_dac_channel_info.params.outputidx = *index;
	/* set default standard */
	vpbe_dac_channel_info.params.std 
			= vpbe_dac_configuration.output[*index].standard[0].id;
	err |= vpbe_dac_setstd(&vpbe_dac_channel_info.params.std, enc);
	if (err < 0) {
		return err;
	}

	dev_dbg(davinci_display_dev,"</vpbe_dac_setoutput>\n");
	return err;
}

/* Following function is used to get index of the output currently selected.*/
static int vpbe_dac_getoutput(int *index, void *enc)
{
	int err = 0;
	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer\n");
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"<vpbe_dac_getoutput>\n");
	/* check for null pointer */
	if (index == NULL) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}


	*index = vpbe_dac_channel_info.params.outputidx;
	dev_dbg(davinci_display_dev,"</vpbe_dac_getoutput>\n");
	return err;
}

/* Following function is used to enumerate outputs supported by the driver.
   It fills in information about the output in the outp. */
static int vpbe_dac_enumoutput(struct v4l2_output *output, void *enc)
{
	int err = 0;
	int index = 0;
	if (NULL == enc) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}
	/* check for null pointer */
	if (output == NULL) {
		dev_err(davinci_display_dev,"NULL Pointer.\n");
		return -EINVAL;
	}
	/* Only one output is available */
	if (output->index >= vpbe_dac_configuration.no_of_outputs) {
		return -EINVAL;
	}
	dev_dbg(davinci_display_dev,"<vpbe_dac_enumoutput>\n");
	index = output->index;
	memset(output, 0, sizeof(*output));
	output->index = index;
	memcpy(output,
		&vpbe_dac_configuration.output[index].output_info,
		sizeof(struct v4l2_output));
	dev_dbg(davinci_display_dev,"</vpbe_dac_enumoutput>\n");
	return err;
}

/* This function used to initialize the i2c driver */
static int vpbe_dac_init(void)
{
	int err = 0;

	err = davinci_register_encoder(&vpbe_dac_dev);
	return err;
}

/* Function used to cleanup i2c driver */
static void vpbe_dac_cleanup(void)
{
	davinci_unregister_encoder(&vpbe_dac_dev);
}

module_init(vpbe_dac_init);
module_exit(vpbe_dac_cleanup);

MODULE_LICENSE("GPL");
