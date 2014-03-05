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

#include <linux/config.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/videodev.h>
#include <linux/device.h>
#include <asm/arch/gio.h>
#include <media/mt9p031.h>
#define MT9P031_I2C_ENABLE 1
#define IS_DM350	(0)

#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
#define MT9P031_RESET_PIN	 26
#else
#define MT9P031_RESET_PIN	 0x2
#endif

#define ENABLE_PLL_SETTING	(1)

#define PINMUX4          __REG(0x01c40010)
#define PINMUX0   	 __REG(0x01c40000)
#define PINMUX1          __REG(0x01c40004)
#define PINMUX3          __REG(0x01c4000c)

/* i2c global variable for mt9p031*/
static struct i2c_client mt9p031_i2c_client;
static struct i2c_driver mt9p031_i2c_driver;
static int mt9p031_i2c_registration = 0;
static unsigned short mt9p031_output_ctrl;
struct device *mt9p031_i2c_dev;

static v4l2_std_id mt9p031_Cur_mode = MT9P031_MODE_720p_30FPS;

/*	Function prototype*/
int mt9p031_ctrl(unsigned int cmd, void *arg, void *params);
static int mt9p031_init(void *arg, void **params);
static int mt9p031_cleanup(void *params);
static int mt9p031_configpca9543(void);
static int mt9p031_setstd(void *arg, void *params);
static int mt9p031_setformat(struct mt9p031_format_params *mt9tformats);
static int mt9p031_getformat(struct mt9p031_format_params *mt9tformats);
static int mt9p031_queryctrl(void *arg);
static int mt9p031_setgain(int arg);
static int mt9p031_getgain(int *arg);
static int mt9p031_setparams(void *arg);
static int mt9p031_getparams(void *arg);
static int mt9p031_setExposure(int arg);
static int mt9p031_setBin(int arg);
static int mt9p031_setBinning_Skip(int arg);
static int mt9p031_set50_60Hz(int arg);
static int mt9p031_set_frame_rate(int arg);

/*i2c function proto types*/
static int i2c_read_reg(struct i2c_client *, unsigned char,
			unsigned short *, bool);
static int i2c_write_reg(struct i2c_client *, unsigned char,
			 unsigned short, bool);
static int _i2c_attach_client(struct i2c_client *, struct i2c_driver *,
			      struct i2c_adapter *, int);
static int _i2c_detach_client(struct i2c_client *);
static int mt9p031_i2c_probe_adapter(struct i2c_adapter *);
static int mt9p031_i2c_init(void);
void mt9p031_i2c_cleanup(void);
static void Micro_Specific_Setting(void);

/* Parameters for  various format supported  */
/* pixel clock = (external clock * M) / ((N) * (P1)) */
/*Format  is
{
	NUMBER OF PIXELS PER LINE, NUMBER OF LINES,
	HRIZONTAL BLANKING WIDTH, VERTICAL BLANKING WIDTH,
	SHUTTER WIDTH, ROW ADDRESS MODE, COL ADDRESS MODE,
	BLACK_LEVEL,PIXEL CLOCK CONTROL,
	ROW START, COL START,
	READ MODE 2 CONFIG,
	OUTPUT CONTROL,
	PLL MULTIPLIER, PLL DIVIDER, PLL P1 DIVIDER
}
*/
/* all these value assume external clock = 24 MHz */
/* 2 <= N <= 11 */
/* 7.5 < M / N < 15 */
/*  */
const struct mt9p031_format_params MT9P031_VGA_30FPS =
    { 2575, 1951, 776, 10, 744, 0x11, 0x11, 64, 0x8000, 32, 24,
    32, 0x1F8E, 0x1C, 4, 2 };
//const struct mt9p031_format_params MT9P031_VGA_BIN_30FPS =
//    { 2575, 1951, 1492, 10, 496, 0x33, 0x33, 64, 0x8000, 32, 24,
//    32, 0x1F82, 0x12, 4, 2 };
const struct mt9p031_format_params MT9P031_VGA_BIN_30FPS =
    { 2575, 1971, 1470, 8, 496, 0x33, 0x33, 64, 0x8000, 32, 24,
    32, 0x1F82, 0x12, 4, 2 };

const struct mt9p031_format_params MT9P031_VGA_60FPS =
    { 2575, 1951, 776, 10, 744, 0x11, 0x11, 64, 0x8000, 0, 0,
    64,  0x1F8E, 0x1C, 4, 2 };
const struct mt9p031_format_params MT9P031_VGA_VGA_30FPS =
    { 2575, 1951, 776, 10, 744, 0x11, 0x11, 64, 0x8000, 32, 24,
    32, 0x1F8E, 0x1C, 4, 2 };
const struct mt9p031_format_params MT9P031_SVGA_30FPS =
    { 1599, 1199, 0, 8, 599, 0x11, 0x11, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_SVGA_60FPS =
    { 799, 599, 0, 8, 599, 0x00, 0x00, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_XGA_30FPS =
    { 2047, 1535, 0, 8, 767, 0x11, 0x11, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_720P_24FPS =
    { 2559, 1439, 0, 8, 719, 0x11, 0x11, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_SXGA_30FPS =
    { 2575, 1951, 776, 10, 744, 0x11, 0x11, 64, 0x8000, 32, 24,
    32, 0x1F8E, 0x1C, 4, 2 };
const struct mt9p031_format_params MT9P031_SXVGA_30FPS =
    { 2575, 1951, 776, 10, 744, 0x11, 0x11, 64, 0x8000, 32, 24,
    32, 0x1F8E, 0x1C, 4, 2 };
const struct mt9p031_format_params MT9P031_720P_30FPS =
    { 2583, 1491, 786, 0, 740, 0x11, 0x11, 64, 0x8000, 272, 24,
    32, 0x1F8E, 16, 3, 2 };
const struct mt9p031_format_params MT9P031_1080P_18FPS =
    { 1919, 1079, 0, 3, 1079, 0x00, 0x00, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_1280x960_24FPS =
    { 2751, 2003, 0, 8, 1024, 0x11, 0x11, 64, 0x8000, 0, 0,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 255, 29, 4 };
const struct mt9p031_format_params MT9P031_5MP_7FPS =
    { 2591, 1943, 450, 9, 1900, 0x00, 0x00, 64, 0x8000, 66, 32,
    MT9P031_READ_MODE2_DEFAULT,
    MT9P031_OUTPUT_CTRL_DEFAULT, 24, 4, 2 }; //18

void mt9p031_readregs(void)
{
	unsigned short temp = 0;
	int err = 0;
	unsigned char regcnt;
	/* Here, 0x64 is number of registers for MT9P031 */
	for (regcnt = 0; regcnt < 0x64; regcnt++) {
		err = i2c_read_reg(&mt9p031_i2c_client, regcnt,
				   &temp, MT9P031_I2C_CONFIG);
		if (err >= 0) {
			dev_dbg(mt9p031_i2c_dev,
				"\nread back 0x%x = 0x%x...", regcnt, temp);
		}
	}
}

/*
 * ======== mt9p031_ctrl  ========
 */

/*This function will provide different control commands for MT9P031
		configuration.*/
int mt9p031_ctrl(unsigned int cmd, void *arg, void *params)
{
	int err = 0;

printk("WARNING~~~~~!!!! Here is in MT9P031 CTRL Func...\n");
	switch (cmd) {
	case MT9P031_SET_PARAMS:
		{

			struct mt9p031_params *vpfe_mt9p031params =
			    (struct mt9p031_params *)params;
			struct mt9p031_params *user_mt9p031params =
			    (struct mt9p031_params *)arg;

			/* Update the global parameter of vpfe_obj */

			if ((arg == NULL) || (params == NULL)) {
				dev_err(mt9p031_i2c_dev, "Invalid argument for \
							MT9P031_SET_PARAMS ");
				return -1;
			}

			memcpy(vpfe_mt9p031params, user_mt9p031params,
			       sizeof(struct mt9p031_params));

			err = mt9p031_setparams(arg);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev,
					"\nMT9P031 set parameters fails...");
				return err;
			}
			break;

		}
	case MT9P031_SET_STD:
		{
			err = mt9p031_setstd(arg, params);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev,
					"\nMT9P031 set standard fails...");
				return err;
			} else {
				//mt9p031_readregs();
				Micro_Specific_Setting();
			}
			break;
		}
	case MT9P031_GET_PARAMS:
		{

			struct mt9p031_params *vpfe_mt9p031params =
			    (struct mt9p031_params *)params;
			struct mt9p031_params *user_mt9p031params =
			    (struct mt9p031_params *)arg;

			err = mt9p031_getparams(arg);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev,
					"\nMT9P031 get parameters  fails...");
				return err;
			}
			/* Update the global parameter of vpfe_obj */
			memcpy(vpfe_mt9p031params, user_mt9p031params,
			       sizeof(struct mt9p031_params));

			break;
		}
	case MT9P031_INIT:
		{
			err = mt9p031_init(arg, params);
			if (err < 0) {
				printk(KERN_ERR
				       "\n Unable to initialize MT9P031...");
				return err;
			}
			break;
		}
	case MT9P031_CLEANUP:
		{
			mt9p031_cleanup(params);

			break;
		}
	case VIDIOC_S_CTRL:
		{
			struct v4l2_control *ctrl = arg;

			if (ctrl->id == V4L2_CID_GAIN) {
				err = mt9p031_setgain((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set gain fails...");
					return err;
				}
			}
			else if (ctrl->id == V4L2_CID_EXPOSURE)
			{
				err = mt9p031_setExposure((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set exposure fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_BIN)
			{
				err = mt9p031_setBin((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set bin fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_50_60Hz)
			{
				err = mt9p031_set50_60Hz((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set 50/60 fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_BIN_SKIP_MODE)
			{
				err = mt9p031_setBinning_Skip((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set bin_skip fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_FRM_CTRL)
			{
				err = mt9p031_set_frame_rate((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 set frame rate fails...");
					return err;
				}
			}
			else
			{
				err = -EINVAL;
			}
			break;
		}
	case VIDIOC_G_CTRL:
		{
			struct v4l2_control *ctrl = arg;

			if (ctrl->id == V4L2_CID_GAIN) {
				err = mt9p031_getgain((int *)
						      &(ctrl->value));
				if (err < 0) {
					dev_err(mt9p031_i2c_dev,
						"\n MT9P031 get gain fails...");
					return err;
				}
			} else {
				err = -EINVAL;
			}
			break;
		}
	case VIDIOC_QUERYCTRL:
		{
			err = mt9p031_queryctrl(arg);
			break;
		}
	default:
		{
			dev_err(mt9p031_i2c_dev, "\n Undefined command");
			return -1;
		}

	}
	return err;
}

/*
 * ======== mt9p031_init  ========
 */
/*	This function will set the video format standart*/
static int mt9p031_init(void *arg, void **params)
{
	struct i2c_driver *driver = &mt9p031_i2c_driver;
	struct mt9p031_params *temp_params = NULL;
	int utemp = 0;
	int err = 0;

#if MT9P031_I2C_ENABLE
	/* Register MT9P031 I2C client */
	err = i2c_add_driver(driver);
	if (err) {
		printk(KERN_ERR "Failed to register MT9P031 I2C client.\n");
		return -1;
	}
	mt9p031_i2c_registration = MT9P031_I2C_REGISTERED;
#endif
	temp_params = kmalloc(sizeof(struct mt9p031_params), GFP_KERNEL);
	if (temp_params == NULL) {
		printk(KERN_ERR "\n Unable to allocate memory...");
		return -1;
	}
	(*params) = temp_params;
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	utemp = PINMUX3;
	PINMUX3 = (utemp | 0x10000);
	err = 0;
	gpio_set(MT9P031_RESET_PIN);
	//gpio_set(83);
	gpio_set_direction(MT9P031_RESET_PIN, GIO_DIR_OUTPUT);
	//gpio_set_direction(83, GIO_DIR_OUTPUT);
	gpio_clear(MT9P031_RESET_PIN);
	mdelay(10);//10ms
	gpio_set(MT9P031_RESET_PIN);
	mdelay(10);
#else
	if(IS_DM350){
		gpio_set(MT9P031_RESET_PIN);
		gpio_set_direction(MT9P031_RESET_PIN, GIO_DIR_OUTPUT);
		gpio_clear(MT9P031_RESET_PIN);
		mdelay(10);//10ms
		gpio_set(MT9P031_RESET_PIN);
		mdelay(10);
	}
	err = mt9p031_configpca9543();
#endif
	if(err >= 0){
		/* Mt9p031 test mode */
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_START,
					MT9P031_ROW_START_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_START,
					MT9P031_COL_START_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_HEIGHT,
					MT9P031_HEIGHT_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH,
					MT9P031_WIDTH_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_READ_MODE2,
					MT9P031_READ_MODE2_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_BLK_DEF_OFFSET,
					MT9P031_ROW_BLK_DEF_OFFSET_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLK_LVL_CALIB,
					MT9P031_BLK_LVL_CALIB_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE,
					MT9P031_ROW_ADDR_MODE_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE,
					MT9P031_COL_ADDR_MODE_DEFAULT, MT9P031_I2C_CONFIG);
#if ENABLE_PLL_SETTING
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONTROL,
					MT9P031_PLL_DISABLE, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG2,
					MT9P031_PLL_CONFIG2_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG1,
					MT9P031_PLL_CONFIG1_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONTROL,
					MT9P031_PLL_ENABLE, MT9P031_I2C_CONFIG);
#endif
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK,
					MT9P031_HBLANK_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_VBLANK,
					MT9P031_VBLANK_DEFAULT, MT9P031_I2C_CONFIG);
		err |= i2c_write_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH,
					MT9P031_SHUTTER_WIDTH_DEFAULT, MT9P031_I2C_CONFIG);
	}

	if (err < 0) {
		mt9p031_cleanup((*params));
	} else {
		/* Configure for default video standard */
		err = mt9p031_setstd(arg, (*params));

		if (err < 0) {
			mt9p031_cleanup(params);
		}
		Micro_Specific_Setting();
	}
	return err;
}

static int mt9p031_getformat(struct mt9p031_format_params *mt9tformats)
{
	int err = 0;
	unsigned short val = 0;

	/*Read the height width and blanking information required
	   for particular format */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_HEIGHT,
			   &mt9tformats->row_size, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...height");
		return err;
	}

	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_WIDTH,
			   &mt9tformats->col_size, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...width");
		return err;
	}

	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_HBLANK,
			   &mt9tformats->h_blank, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...hblk");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_VBLANK,
			   &mt9tformats->v_blank, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...vblk");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...shutterwidth");
		return err;
	}
	mt9tformats->shutter_width = val & MT9P031_SHUTTER_WIDTH_LOWER_MASK;

	err =
	    i2c_read_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH_UPPER,
			 &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...upper");
		return err;
	}
	mt9tformats->shutter_width |= ((val & MT9P031_SHUTTER_WIDTH_UPPER_MASK)
				       << MT9P031_SHUTTER_WIDTH_UPPER_SHIFT);

	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE,
			   &mt9tformats->row_addr_mode, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...addrmoderow");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE,
			   &mt9tformats->col_addr_mode, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...addrmodecol");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_ROW_BLK_TARGET,
			   &mt9tformats->black_level, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...black_level");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_ROW_START,
			   &mt9tformats->row_start, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...rowstart");
		return err;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_COL_START,
			   &mt9tformats->col_start, MT9P031_I2C_CONFIG);

	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...colstart");
		return err;
	}

	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_PIXEL_CLK_CTRL,
			   &mt9tformats->pixel_clk_control, MT9P031_I2C_CONFIG);

	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...clkctrl");
		return err;
	}
	return err;
}

static int mt9p031_setformat(struct mt9p031_format_params *mt9tformats)
{
	int err = 0;
	unsigned short val;

	/* stop changes  */
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_OUTPUT_CTRL,
			    MT9P031_HALT_MODE, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...outputctrl");
		return err;
	}
	/*Write the height width and blanking information required
	   for particular format */
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_HEIGHT,
			    mt9tformats->row_size, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...height");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH,
			    mt9tformats->col_size, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...width");
		return err;
	}
	/* Configure for default video standard */

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK,
			    mt9tformats->h_blank, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...hblk");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_VBLANK,
			    mt9tformats->v_blank, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...vblk");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH,
			    (unsigned short)(mt9tformats->
					     shutter_width &
					     MT9P031_SHUTTER_WIDTH_LOWER_MASK),
			    MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...shutterwidth");
		return err;
	}

	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH_UPPER,
			  (unsigned short)(mt9tformats->
					   shutter_width >>
					   MT9P031_SHUTTER_WIDTH_UPPER_SHIFT),
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...upper");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE,
			    mt9tformats->row_addr_mode, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...addrmoderow");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE,
			    mt9tformats->col_addr_mode, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...addrmodecol");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_BLK_DEF_OFFSET,
			    mt9tformats->black_level, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...black_level");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_START,
			    mt9tformats->row_start, MT9P031_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...rowstart");
		return err;
	}
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_START,
			    mt9tformats->col_start, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...colstart");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PIXEL_CLK_CTRL,
			    mt9tformats->pixel_clk_control, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...clkctrl");
		return err;
	}
#if ENABLE_PLL_SETTING
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONTROL,
			    MT9P031_PLL_USE_EXTERNAL, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll external");
		return err;
	}

	val = (mt9tformats->pll_m << MT9P031_PLL_M_SHIFT) |
		((mt9tformats->pll_n - 1) & MT9P031_PLL_N_MASK);
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG1,
			    val, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG2,
			    (mt9tformats -> pll_p1 - 1) & MT9P031_PLL_P1_MASK ,
			    MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONTROL,
			    MT9P031_PLL_ENABLE, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll enable");
		return err;
	}
#endif
	mt9p031_output_ctrl = (mt9tformats ->output_config) &
		MT9P031_OUTPUT_CTRL_MASK;
	/* applying changes  */
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_OUTPUT_CTRL,
			    mt9p031_output_ctrl, MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...outputctrl");
		return err;
	}

	return err;
}
/*
 * ======== configPCA9543  ========
 */
/*	This function will configure PCA9543 control register*/
static int mt9p031_configpca9543()
{
	int err = 0;
	/* enable path from mt9t001 to*/ 
	err |= i2c_write_reg(&mt9p031_i2c_client, ECP_REGADDR, 
		ECP_REGVAL, ECP_I2C_CONFIG);
	/* Configure PCA9543A, here discard register address */
	err = i2c_write_reg(&mt9p031_i2c_client, 0,
			    PCA9543_REGVAL, PCA9543_I2C_CONFIG);
	return err;
}
/*
 * ======== mt9p031_cleanup  ========
 */

/*This function will free the memory allocated for mt9p031_params*/
static int mt9p031_cleanup(void *params)
{
	struct i2c_driver *driver = &mt9p031_i2c_driver;
	struct mt9p031_params *temp_params = (struct mt9p031_params *)params;
	if (temp_params != NULL)
		kfree(temp_params);
	params = NULL;
#if MT9P031_I2C_ENABLE
	if (mt9p031_i2c_registration) {
		i2c_detach_client(&mt9p031_i2c_client);
		i2c_del_driver(driver);
		mt9p031_i2c_client.adapter = NULL;
		mt9p031_i2c_registration = MT9P031_I2C_UNREGISTERED;
	}
#endif
	return 0;
}
static void Micro_Specific_Setting(void)
{	
	//printk("Micro_Specific_Setting \n");
	i2c_write_reg(&mt9p031_i2c_client,0x29,0x481,MT9P031_I2C_CONFIG);        
	i2c_write_reg(&mt9p031_i2c_client,0x3E,0x87,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x3F,0x07,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x41,0x0003,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x42,0x0003,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x43,0x0003,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x4f,0x0014,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x48,0x18,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x5F,0x1C16,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x57,0x7,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x70,0x005C,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x71,0x5B00,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x72,0x5900,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x73,0x200,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x74,0x200,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x75,0x2800,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x76,0x3E29,MT9P031_I2C_CONFIG);   
	i2c_write_reg(&mt9p031_i2c_client,0x77,0x3E29,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x78,0x583F,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x79,0x5B00,MT9P031_I2C_CONFIG);         
	i2c_write_reg(&mt9p031_i2c_client,0x7A,0x5A00,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x7B,0x5900,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x7C,0x5900,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x7E,0x5900,MT9P031_I2C_CONFIG);
	i2c_write_reg(&mt9p031_i2c_client,0x7F,0x5900,MT9P031_I2C_CONFIG);
}
/*
 * ======== mt9p031_setstd  ========
 */

/*	Function to set the video frame format*/
static int mt9p031_setstd(void *arg, void *params)
{
	v4l2_std_id mode = *(v4l2_std_id *) arg;
	struct mt9p031_format_params mt9tformats;
	int err = 0;

	/* Select configuration parameters as per video mode  */
	if (mode == MT9P031_MODE_VGA_30FPS) {
		mt9tformats = MT9P031_VGA_30FPS;
	} else if (mode == MT9P031_MODE_VGA_BIN_30FPS) {
		mt9tformats = MT9P031_VGA_BIN_30FPS;
	} else if (mode == MT9P031_MODE_VGA_60FPS) {
		mt9tformats = MT9P031_VGA_60FPS;
	} else if (mode == MT9P031_MODE_SVGA_30FPS) {
		mt9tformats = MT9P031_SVGA_30FPS;
	} else if (mode == MT9P031_MODE_SVGA_60FPS) {
		mt9tformats = MT9P031_SVGA_60FPS;
	} else if (mode == MT9P031_MODE_XGA_30FPS) {
		mt9tformats = MT9P031_XGA_30FPS;
#if 0
	} else if (mode == MT9P031_MODE_480p_30FPS) {
		mt9tformats = MT9P031_480P_30FPS;
	} else if (mode == MT9P031_MODE_480p_60FPS) {
		mt9tformats = MT9P031_480P_60FPS;
	} else if (mode == MT9P031_MODE_576p_25FPS) {
		mt9tformats = MT9P031_576P_25FPS;
	} else if (mode == MT9P031_MODE_576p_50FPS) {
		mt9tformats = MT9P031_576P_50FPS;
#endif
	} else if (mode == MT9P031_MODE_720p_24FPS) {
		mt9tformats = MT9P031_720P_24FPS;
	} else if (mode == MT9P031_MODE_720p_30FPS) {
		mt9tformats = MT9P031_720P_30FPS;
	} else if (mode == MT9P031_MODE_1080p_18FPS) {
		mt9tformats = MT9P031_1080P_18FPS;
	//} else if (mode == MT9P031_MODE_1280x960_24FPS) {
	//	mt9tformats = MT9P031_1280x960_24FPS;
	//} else if (mode == MT9P031_MODE_SXGA_30FPS) {
	//	mt9tformats = MT9P031_SXGA_30FPS;
	//} else if (mode == MT9P031_MODE_SXVGA_30FPS) {
	//	mt9tformats = MT9P031_SXVGA_30FPS;
	//} else if (mode == MT9P031_MODE_VGA_VGA_30FPS) {
	//	mt9tformats = MT9P031_VGA_VGA_30FPS;
	//} else if (mode == MT9P031_MODE_5MP_7FPS) {
	//	mt9tformats = MT9P031_5MP_7FPS;
	} else {
		printk(KERN_ERR "\n Invalid frame format");
		return -1;
	}
#if 1
	mt9p031_Cur_mode = mode;

	err = mt9p031_setformat(&mt9tformats);
#else
	printk(KERN_INFO "\n Now change sensor setting not work");
#endif
	return err;

}

/*
 * ======== mt9p031_setparams  ========
 */

/* This function will configure MT9P031 for bayer pattern capture.*/
static int mt9p031_setparams(void *arg)
{
	/*variable to store the return value of i2c read, write function */
	int err = 0;
	struct mt9p031_params *mt9p031params = (struct mt9p031_params *)arg;
	unsigned short val;
	if (arg == NULL) {
		dev_err(mt9p031_i2c_dev, "Invalid argument in config MT9P031");
		return -1;
	}
	/* set video format related parameters */
	err = mt9p031_setformat(&mt9p031params->format);

	/*Write the gain information */
	val = (unsigned short)(mt9p031params->rgb_gain.green1_analog_gain
			       & MT9P031_ANALOG_GAIN_MASK);

	val |= ((mt9p031params->rgb_gain.green1_digital_gain
		 << MT9P031_DIGITAL_GAIN_SHIFT) & MT9P031_DIGITAL_GAIN_MASK);
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_GREEN1_GAIN, val,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = (unsigned short)(mt9p031params->rgb_gain.red_analog_gain)
	    & MT9P031_ANALOG_GAIN_MASK;
	val |= (((mt9p031params->rgb_gain.red_digital_gain)
		 << MT9P031_DIGITAL_GAIN_SHIFT)
		& MT9P031_DIGITAL_GAIN_MASK);
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_RED_GAIN,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = (unsigned short)(mt9p031params->rgb_gain.blue_analog_gain)
	    & MT9P031_ANALOG_GAIN_MASK;
	val |= (((mt9p031params->rgb_gain.blue_digital_gain)
		 << MT9P031_DIGITAL_GAIN_SHIFT)
		& MT9P031_DIGITAL_GAIN_MASK);
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLUE_GAIN,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = (unsigned short)((mt9p031params->rgb_gain.green2_analog_gain)
			       << MT9P031_ANALOG_GAIN_SHIFT) &
	    MT9P031_ANALOG_GAIN_MASK;
	val |= (((mt9p031params->rgb_gain.green2_digital_gain)
		 << MT9P031_DIGITAL_GAIN_SHIFT) & MT9P031_DIGITAL_GAIN_MASK);
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_GREEN2_GAIN, val,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	/*Write the offset value in register */

	val = mt9p031params->black_calib.green1_offset
	    & MT9P031_GREEN1_OFFSET_MASK;
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_GREEN1_OFFSET,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = mt9p031params->black_calib.green2_offset
	    & MT9P031_GREEN2_OFFSET_MASK;
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_GREEN2_OFFSET,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = mt9p031params->black_calib.red_offset & MT9P031_RED_OFFSET_MASK;
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_RED_OFFSET, val,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = mt9p031params->black_calib.blue_offset & MT9P031_BLUE_OFFSET_MASK;
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLUE_OFFSET, val,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	/*Write other black caliberation information */

	val = (unsigned short)(mt9p031params->black_calib.manual_override)
	    & MT9P031_MANUAL_OVERRIDE_MASK;
	val |= ((mt9p031params->black_calib.disable_calibration)
		<< MT9P031_DISABLE_CALLIBERATION_SHIFT)
	    & MT9P031_DISABLE_CALLIBERATION_MASK;
	val |= ((mt9p031params->black_calib.recalculate_black_level)
		<< MT9P031_RECAL_BLACK_LEVEL_SHIFT)
	    & MT9P031_RECAL_BLACK_LEVEL_MASK;
	val |= ((mt9p031params->black_calib.lock_red_blue_calibration)
		<< MT9P031_LOCK_RB_CALIBRATION_SHIFT)
	    & MT9P031_LOCK_RB_CALLIBERATION_MASK;
	val |= ((mt9p031params->black_calib.lock_green_calibration)
		<< MT9P031_LOCK_GREEN_CALIBRATION_SHIFT)
	    & MT9P031_LOCK_GREEN_CALLIBERATION_MASK;
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLK_LVL_CALIB,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	/*Write Thresholds Value */

	val = (unsigned short)mt9p031params->black_calib.low_coarse_thrld
	    & MT9P031_LOW_COARSE_THELD_MASK;
	val |= (mt9p031params->black_calib.high_coarse_thrld
		<< MT9P031_HIGH_COARSE_THELD_SHIFT) &
	    MT9P031_HIGH_COARSE_THELD_MASK;
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLC_DELTA_THRESHOLDS, val,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	val = (unsigned short)mt9p031params->black_calib.low_target_thrld
	    & MT9P031_LOW_TARGET_THELD_MASK;
	val |= (mt9p031params->black_calib.high_target_thrld
		<< MT9P031_HIGH_TARGET_THELD_SHIFT)
	    & MT9P031_HIGH_TARGET_THELD_MASK;
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_BLC_TARGET_THRESHOLDS,
			    val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}

	dev_dbg(mt9p031_i2c_dev, "\nEnd of configMT9P031...");
	return 0;

}

/*
 * ======== queryctrl ========
 */
 /* This function will return parameter values for control command passed */
static int mt9p031_queryctrl(void *arg)
{
	struct v4l2_queryctrl *queryctrl = arg;
	int ret = 0;
	int id = queryctrl->id;
	dev_dbg(mt9p031_i2c_dev, "\nStarting mt9p031_queryctrl...");
	if (queryctrl == NULL) {
		dev_err(mt9p031_i2c_dev,
			"\n Invalid argument in query control");
		return -1;
	}

	memset(queryctrl, 0, sizeof(*queryctrl));
	queryctrl->id = id;
	switch (id) {
	case V4L2_CID_GAIN:
		strcpy(queryctrl->name, "GAIN");
		queryctrl->type = V4L2_CTRL_TYPE_INTEGER;
		queryctrl->minimum = MT9P031_GAIN_MINVAL;
		queryctrl->maximum = MT9P031_GAIN_MAXVAL;
		queryctrl->step = MT9P031_GAIN_STEP;
		queryctrl->default_value = MT9P031_GAIN_DEFAULTVAL;
		break;
	default:
		if (id < V4L2_CID_LASTP1)
			queryctrl->flags = V4L2_CTRL_FLAG_DISABLED;
		else
			ret = -EINVAL;
		break;
	}			/* end switch (id) */
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_queryctrl...");
	return ret;
}

/*
 * ======== mt9p031_setgain  ========
 */

/*	This function will set the global gain of MT9P031*/
static int mt9p031_setgain(int arg)
{

	unsigned short gain = (unsigned short)arg;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev,
		"\nStarting mt9p031_setgain with gain = %d...", (int)gain);
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_GLOBAL_GAIN, gain,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_setgain...");
	return err;
}

static int mt9p031_setExposure(int arg)
{

	unsigned short shutter_width = (unsigned short)arg;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev,
		"\nStarting mt9p031_setExposure with shutter = %d...", (int)shutter_width);
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_SHUTTER_WIDTH, shutter_width,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_setExposure...");
	return err;
}

static int mt9p031_setBin(int arg)
{

	unsigned short bin_mode = (unsigned short)arg;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev,
		"\nStarting mt9p031_setBin with bin = %d...", (int)bin_mode);
	err =
	    i2c_write_reg(&mt9p031_i2c_client, MT9P031_READ_MODE2, bin_mode,
			  MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_setBin...");
	return err;
}

static int mt9p031_setBinning_Skip(int arg)
{

	unsigned short binning_mode = (unsigned short)arg;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev,
		"\nStarting mt9p031_setBin with bin = %d...", (int)bin_mode);
	
	if( binning_mode == 1 )
	{
		if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
		{
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE, 0x33, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE, 0x33, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}
		else
		{
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE, 0x11, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE, 0x11, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}

	}else{

		if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
		{
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE, 0x03, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE, 0x03, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}

		}else{
		
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_ROW_ADDR_MODE, 0x01, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9p031_i2c_client, MT9P031_COL_ADDR_MODE, 0x01, MT9P031_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}
	}
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_setBinning_skip...");
	return err;
}


static int mt9p031_set_frame_rate(int arg)
{
	int framerate = arg;
	unsigned short p1_val = 0x1;
	unsigned short mn_val = 0x1c03;
	unsigned short fifo_mode = 0x1f8e;
	unsigned short vblank = 8;
	int err = 0;

	

	if( framerate == CTRL_FRM_15FS )
	{

		if( mt9p031_Cur_mode == MT9P031_MODE_VGA_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1c03;
			fifo_mode = 0x1f82;
			vblank = MT9P031_VGA_30FPS.v_blank;
		}
		else if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1203;
			fifo_mode = 0x1f82;
			vblank = MT9P031_VGA_BIN_30FPS.v_blank;
		}
		else if( mt9p031_Cur_mode ==  MT9P031_MODE_720p_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1002;
			fifo_mode = 0x1f82;
			vblank = MT9P031_720P_30FPS.v_blank;
		}
		
	}
	else if( framerate == CTRL_FRM_30FS )
	{
			
		if( mt9p031_Cur_mode == MT9P031_MODE_VGA_30FPS )
		{
			p1_val = (MT9P031_VGA_30FPS.pll_p1 - 1) & MT9P031_PLL_P1_MASK;

			mn_val = (MT9P031_VGA_30FPS.pll_m << MT9P031_PLL_M_SHIFT) |
				((MT9P031_VGA_30FPS.pll_n - 1) & MT9P031_PLL_N_MASK);

			fifo_mode = MT9P031_VGA_30FPS.output_config;

			vblank = MT9P031_VGA_30FPS.v_blank;
		}
		else if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
		{
			p1_val = (MT9P031_VGA_BIN_30FPS.pll_p1 - 1) & MT9P031_PLL_P1_MASK;

			mn_val = (MT9P031_VGA_BIN_30FPS.pll_m << MT9P031_PLL_M_SHIFT) |
				((MT9P031_VGA_BIN_30FPS.pll_n - 1) & MT9P031_PLL_N_MASK);

			fifo_mode = MT9P031_VGA_BIN_30FPS.output_config;

			vblank = MT9P031_VGA_BIN_30FPS.v_blank;
		}
		else if( mt9p031_Cur_mode ==  MT9P031_MODE_720p_30FPS )
		{
			p1_val = (MT9P031_720P_30FPS.pll_p1 - 1) & MT9P031_PLL_P1_MASK;

			mn_val = (MT9P031_720P_30FPS.pll_m << MT9P031_PLL_M_SHIFT) |
				((MT9P031_720P_30FPS.pll_n - 1) & MT9P031_PLL_N_MASK);

			fifo_mode = MT9P031_720P_30FPS.output_config;

			vblank = MT9P031_720P_30FPS.v_blank;
		}
		

	}
	else if( framerate == CTRL_FRM_5FS )
	{

		if( mt9p031_Cur_mode == MT9P031_MODE_VGA_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1c03;
			fifo_mode = 0x1f82;
			vblank = 1996;
		}
		else if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1203;
			fifo_mode = 0x1f82;
			vblank = 1008;
		}
		else if( mt9p031_Cur_mode ==  MT9P031_MODE_720p_30FPS )
		{
			p1_val = 0x3;
			mn_val = 0x1002;
			fifo_mode = 0x1f82;
			vblank = 1516;
		}

	}

	

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG2,
			    p1_val,MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_PLL_CONFIG1,
			    mn_val,MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	
	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_VBLANK,
			vblank, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C write fails...vblk");
		return err;
	}

	err = i2c_write_reg(&mt9p031_i2c_client, MT9P031_OUTPUT_CTRL,
			    fifo_mode,MT9P031_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll enable");
		return err;
	}
	

	return err;
}


static int mt9p031_set50_60Hz(int arg)
{

	unsigned short Is_50_60Hz_mode = (unsigned short)arg;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev,
		"\nStarting mt9p031_setBin with bin = %d...", (int)bin_mode);
	if( mt9p031_Cur_mode == MT9P031_MODE_VGA_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2575,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 776,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2775+10,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 1010-10,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
	
		}
	}
	else if( mt9p031_Cur_mode ==  MT9P031_MODE_720p_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2583,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 786,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2767+31,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 1026-31,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			
	
		}


	}
	else if( mt9p031_Cur_mode == MT9P031_MODE_VGA_BIN_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2575,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 1470,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_WIDTH, 2775,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9p031_i2c_client, MT9P031_HBLANK, 1830,
					MT9P031_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9p031_i2c_dev, "\n I2C write fails...");
				return err;
			}
			
	
		}

	}

	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_setBin...");
	return err;
}


/*
 * ======== mt9p031_getgain  ========
 */

/*	This function will get the global gain of MT9P031*/
static int mt9p031_getgain(int *arg)
{
	unsigned short gain;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev, "\nStarting mt9p031_getgain...");
	if (arg == NULL) {
		dev_err(mt9p031_i2c_dev,
			"\nInvalid argument pointer in get gain function");
		return -1;
	}
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_GLOBAL_GAIN,
			   &gain, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C reads fails...");
		return err;
	}
	*arg = gain;
	dev_dbg(mt9p031_i2c_dev, "\nEnd of mt9p031_getgain...");
	return err;
}

/*
 * ======== mt9p031_getparams  ========
 */

/*This function will get MT9P031 configuration values.*/

static int mt9p031_getparams(void *arg)
{

	struct mt9p031_params *params = (struct mt9p031_params *)arg;
	unsigned short val;
	int err = 0;
	dev_dbg(mt9p031_i2c_dev, "\nStarting mt9p031_getparams");

	/* get video format related parameters */
	err = mt9p031_getformat(&params->format);

	if (err < 0) {
		return err;
	}
	/*      Read green1 gain */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_GREEN1_GAIN,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.green1_analog_gain = val & MT9P031_ANALOG_GAIN_MASK;
	params->rgb_gain.green1_digital_gain =
	    (val & MT9P031_DIGITAL_GAIN_MASK) >> MT9P031_DIGITAL_GAIN_SHIFT;

	/*      Read blue gain */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_BLUE_GAIN,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.blue_analog_gain = val & MT9P031_ANALOG_GAIN_MASK;
	params->rgb_gain.blue_digital_gain =
	    (val & MT9P031_DIGITAL_GAIN_MASK) >> MT9P031_DIGITAL_GAIN_SHIFT;

	/*      Read red gain */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_RED_GAIN,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.red_analog_gain = val & MT9P031_ANALOG_GAIN_MASK;
	params->rgb_gain.red_digital_gain =
	    (val & MT9P031_DIGITAL_GAIN_MASK) >> MT9P031_DIGITAL_GAIN_SHIFT;

	/*      Read green2 gain */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_GREEN2_GAIN,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.green2_analog_gain = val & MT9P031_ANALOG_GAIN_MASK;
	params->rgb_gain.green2_digital_gain =
	    (val & MT9P031_DIGITAL_GAIN_MASK) >> MT9P031_DIGITAL_GAIN_SHIFT;

	/*      Read green1 offset */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_GREEN1_OFFSET,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.green1_offset = val & MT9P031_GREEN1_OFFSET_MASK;

	/*      Read green2 offset */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_GREEN2_OFFSET,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.green2_offset = val & MT9P031_GREEN2_OFFSET_MASK;

	/*      Read red offset */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_RED_OFFSET,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.red_offset = val & MT9P031_RED_OFFSET_MASK;

	/*      Read blue offset */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_BLUE_OFFSET,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.blue_offset = val & MT9P031_BLUE_OFFSET_MASK;

	/*      Read Black level caliberation information */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_BLK_LVL_CALIB,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.manual_override =
	    val & MT9P031_MANUAL_OVERRIDE_MASK;
	params->black_calib.disable_calibration =
	    (val & MT9P031_DISABLE_CALLIBERATION_MASK)
	    >> MT9P031_DISABLE_CALLIBERATION_SHIFT;
	params->black_calib.recalculate_black_level =
	    (val & MT9P031_RECAL_BLACK_LEVEL_MASK)
	    >> MT9P031_RECAL_BLACK_LEVEL_SHIFT;
	params->black_calib.lock_red_blue_calibration =
	    (val & MT9P031_LOCK_RB_CALLIBERATION_MASK)
	    >> MT9P031_LOCK_RB_CALIBRATION_SHIFT;
	params->black_calib.lock_green_calibration =
	    (val & MT9P031_LOCK_GREEN_CALLIBERATION_MASK)
	    >> MT9P031_LOCK_GREEN_CALIBRATION_SHIFT;

	/*      Read Black Level Caliberation Coarse Threshold Value */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_BLC_DELTA_THRESHOLDS,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.low_coarse_thrld = val
	    & MT9P031_LOW_COARSE_THELD_MASK;
	params->black_calib.high_coarse_thrld =
	    (val & MT9P031_HIGH_COARSE_THELD_MASK)
	    >> MT9P031_HIGH_COARSE_THELD_SHIFT;

	/*      Read Black Level Caliberation Target Threshold Value */
	err = i2c_read_reg(&mt9p031_i2c_client, MT9P031_BLC_TARGET_THRESHOLDS,
			   &val, MT9P031_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9p031_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.low_target_thrld = val
	    & MT9P031_LOW_TARGET_THELD_MASK;
	params->black_calib.high_target_thrld =
	    (val & MT9P031_HIGH_COARSE_THELD_MASK)
	    >> MT9P031_HIGH_COARSE_THELD_SHIFT;

	dev_dbg(mt9p031_i2c_dev, "\nEnd of getparamsMT9P031...");
	return 0;
}

/*
 * ======== i2c_read_reg  ========
 */

/*This function is used to read value from register for i2c client. */

static int i2c_read_reg(struct i2c_client *client, unsigned char reg,
			unsigned short *val, bool configdev)
{
	int err = 0;
#ifndef MT9P031_I2C_ENABLE
	struct i2c_msg msg[1];
	unsigned char data[2];

	if (!client->adapter) {
		err = -ENODEV;
	} else if (configdev == PCA9543A_I2C_CONFIG) {
		/* if device is PCA9543A then discard reg value, set PCA9543A
		 * address and directly read the data
		 */
		msg->addr = PCA9543A_I2C_ADDR;
		msg->flags = I2C_M_RD;
		msg->len = I2C_ONE_BYTE_TRANSFER;
		msg->buf = data;

		err = i2c_transfer(client->adapter, msg, 1);

		if (err >= 0) {
			*val = (unsigned short)data[0];
		} else {
			dev_err(mt9p031_i2c_dev,
				"\n ERROR in PCA9543A control register read\n");
		}

	} else if (configdev == MT9P031_I2C_CONFIG) {
		msg->addr = client->addr;
		msg->flags = 0;
		msg->len = I2C_ONE_BYTE_TRANSFER;
		msg->buf = data;
		data[0] = reg;
		err = i2c_transfer(client->adapter, msg, 1);
		if (err >= 0) {
			msg->flags = I2C_M_RD;
			msg->len = I2C_TWO_BYTE_TRANSFER;	/* 2 byte read */
			err = i2c_transfer(client->adapter, msg, 1);
			if (err >= 0) {
				*val = ((data[0] & I2C_TXRX_DATA_MASK)
					<< I2C_TXRX_DATA_SHIFT)
				    | (data[1] & I2C_TXRX_DATA_MASK);
			}
		}
	}
#endif
	return err;
}

/*
 * ======== i2c_write_reg  ========
 */
/*This function is used to write value into register for i2c client. */
static int i2c_write_reg(struct i2c_client *client, unsigned char reg,
			 unsigned short val, bool configdev)
{
	int err = 0;
	int trycnt = 0;
	unsigned short readval = 0;

#ifdef MT9P031_I2C_ENABLE

	struct i2c_msg msg[1];
	unsigned char data[3];
	err = -1;
	while ((err < 0) && (trycnt < 5)) {
		trycnt++;
		if (!client->adapter) {
			err = -ENODEV;
		} else if (configdev == ECP_I2C_CONFIG) {
			/* if device is ECP then discard reg value
			 * and set ECP I2C address
			 */
			msg->addr = ECP_I2C_ADDR;
			msg->flags = 0;
			msg->len = I2C_TWO_BYTE_TRANSFER;
			msg->buf = data;
			data[0] = (unsigned char)(reg & I2C_TXRX_DATA_MASK);
			data[1] = (unsigned char)(val & I2C_TXRX_DATA_MASK);

			err = i2c_transfer(client->adapter, msg, 1);
			if (err < 0) {
				printk
				    ("\n ERROR in ECP  register write\n");
			}
		} else if (configdev == MT9P031_I2C_CONFIG) {
			msg->addr = client->addr;
			msg->flags = 0;
			msg->len = I2C_THREE_BYTE_TRANSFER;
			msg->buf = data;
			data[0] = reg;
			data[1] = (val & I2C_TXRX_DATA_MASK_UPPER) >>
			    I2C_TXRX_DATA_SHIFT;
			data[2] = (val & I2C_TXRX_DATA_MASK);
			err = i2c_transfer(client->adapter, msg, 1);
			if (err >= 0) {
				readval = val;
				err =
				    i2c_read_reg(&mt9p031_i2c_client, reg,
						 &readval, MT9P031_I2C_CONFIG);
				if ((err >= 0) && (val != readval)) {
					printk
					    ("\n ERROR: i2c readback failed\n");
				}
				readval = 0;
			}
		} else if (configdev == PCA9543_I2C_CONFIG) {
			/* if device is PCA9543 then discard reg value
			 * and set PCA9543 I2C address
			 */
			msg->addr = PCA9543_I2C_ADDR;
			msg->flags = 0;
			msg->len = I2C_ONE_BYTE_TRANSFER;
			msg->buf = data;
			data[0] = (unsigned char)(val & I2C_TXRX_DATA_MASK);

			err = i2c_transfer(client->adapter, msg, 1);
			if (err < 0) {
				printk
				    ("\n ERROR in PCA9543  register write\n");
			}
		}
	}
#endif
	if (err < 0) {
		printk(KERN_INFO "\n I2C write failed");
	}
	return err;
}

/*
 * ======== _i2c_attach_client  ========
 */
/* This function is used to attach i2c client */
static int _i2c_attach_client(struct i2c_client *client,
			      struct i2c_driver *driver,
			      struct i2c_adapter *adap, int addr)
{
	int err = 0;
#ifdef MT9P031_I2C_ENABLE
	if (client->adapter) {
		err = -EBUSY;	/* our client is already attached */
	} else {
		client->addr = addr;
		client->flags = I2C_CLIENT_ALLOW_USE;
		client->driver = driver;
		client->adapter = adap;

		err = i2c_attach_client(client);
		if (err) {
			client->adapter = NULL;
		}
	}
#endif
	return err;
}

/*
 * ======== _i2c_detach_client  ========
 */
/* This function is used to detach i2c client */
static int _i2c_detach_client(struct i2c_client *client)
{
	int err = 0;
#ifdef MT9P031_I2C_ENABLE
	if (!client->adapter) {
		return -ENODEV;	/* our client isn't attached */
	} else {
		err = i2c_detach_client(client);
		client->adapter = NULL;
	}
#endif
	return err;
}

/*
 * ======== mt9p031_i2c_probe_adapter  ========
 */

/*This function is used to probe adapter for i2c_client. It returns
    0 if i2c_client is attached to adapter and error code on failure.*/
static int mt9p031_i2c_probe_adapter(struct i2c_adapter *adap)
{
	mt9p031_i2c_dev = &(adap->dev);
	return _i2c_attach_client(&mt9p031_i2c_client, &mt9p031_i2c_driver,
				  adap, MT9P031_I2C_ADDR);
}

/*
 * ======== mt9p031_i2c_init  ========
 */

/*	This function is used to initialize the i2c*/
static int mt9p031_i2c_init(void)
{
	int err = 0;
#ifdef MT9P031_I2C_ENABLE
	struct i2c_driver *driver = &mt9p031_i2c_driver;

	driver->owner = THIS_MODULE;
	strlcpy(driver->name, "MT9P031 CMOS sensor I2C driver",
		sizeof(driver->name));
	driver->id = I2C_DRIVERID_EXP0;
	driver->flags = I2C_DF_NOTIFY;
	driver->attach_adapter = mt9p031_i2c_probe_adapter;
	driver->detach_client = _i2c_detach_client;
#endif
	return err;
}

/*
 * ======== mt9p031_i2c_cleanup  ========
 */

void mt9p031_i2c_cleanup(void)
{
#ifdef MT9P031_I2C_ENABLE
	struct i2c_driver *driver = &mt9p031_i2c_driver;

	if (mt9p031_i2c_registration) {
		i2c_detach_client(&mt9p031_i2c_client);
		i2c_del_driver(driver);
		mt9p031_i2c_client.adapter = NULL;
		mt9p031_i2c_registration = MT9P031_I2C_UNREGISTERED;
	}
#endif
}

module_init(mt9p031_i2c_init);
module_exit(mt9p031_i2c_cleanup);

EXPORT_SYMBOL(mt9p031_ctrl);
MODULE_LICENSE("GPL");

/**********************************************************************/
/* End of file                                                        */
/**********************************************************************/
