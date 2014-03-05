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
#include <media/mt9m131.h>

// pixel plus 1.5M sensor
//#define	PO1150K // in <include/media/mt9m131.h>

#define MT9M131_I2C_ENABLE 1
#define IS_DM350	(0)
#define PO1150K
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
#define MT9M131_RESET_PIN	 26 // MT9 112...
#else
#define MT9M131_RESET_PIN	 0x2
#endif

#define ENABLE_PLL_SETTING	(1)

#define PINMUX4          __REG(0x01c40010)
#define PINMUX0   	 __REG(0x01c40000)
#define PINMUX1          __REG(0x01c40004)
#define PINMUX3          __REG(0x01c4000c)



//######################################################################################
#define	HIGHPOWER	0 //Context B(Full Power Mode)
#define	LOWPOWER	1 //Context A(Low Power Mode)

#define MT9M131_CONTEXT_CONTROL		0x0c8

#define MT9M131_RESET_SYNC_CHANGES	(1 << 15)
#define MT9M131_RESET_RESTART_BAD_FRAME	(1 << 9)
#define MT9M131_RESET_SHOW_BAD_FRAMES	(1 << 8)
#define MT9M131_RESET_RESET_SOC		(1 << 5)
#define MT9M131_RESET_OUTPUT_DISABLE	(1 << 4)
#define MT9M131_RESET_CHIP_ENABLE	(1 << 3)
#define MT9M131_RESET_ANALOG_STANDBY	(1 << 2)
#define MT9M131_RESET_RESTART_FRAME	(1 << 1)
#define MT9M131_RESET_RESET_MODE	(1 << 0)

#define MT9M131_RMB_MIRROR_COLS		(1 << 1)
#define MT9M131_RMB_MIRROR_ROWS		(1 << 0)
#define MT9M131_CTXT_CTRL_RESTART	(1 << 15)
#define MT9M131_CTXT_CTRL_DEFECTCOR_B	(1 << 12)
#define MT9M131_CTXT_CTRL_RESIZE_B	(1 << 10)
#define MT9M131_CTXT_CTRL_CTRL2_B	(1 << 9)
#define MT9M131_CTXT_CTRL_GAMMA_B	(1 << 8)
#define MT9M131_CTXT_CTRL_XENON_EN	(1 << 7)
#define MT9M131_CTXT_CTRL_READ_MODE_B	(1 << 3)
#define MT9M131_CTXT_CTRL_LED_FLASH_EN	(1 << 2)
#define MT9M131_CTXT_CTRL_VBLANK_SEL_B	(1 << 1)
#define MT9M131_CTXT_CTRL_HBLANK_SEL_B	(1 << 0)
/*
 * mt9m131: Colorpipe register addresses (0x100..0x1ff)
 */
#define MT9M131_OPER_MODE_CTRL		0x106
#define MT9M131_OUTPUT_FORMAT_CTRL	0x108
#define MT9M131_REDUCER_XZOOM_B		0x1a0
#define MT9M131_REDUCER_XSIZE_B		0x1a1
#define MT9M131_REDUCER_YZOOM_B		0x1a3
#define MT9M131_REDUCER_YSIZE_B		0x1a4
#define MT9M131_REDUCER_XZOOM_A		0x1a6
#define MT9M131_REDUCER_XSIZE_A		0x1a7
#define MT9M131_REDUCER_YZOOM_A		0x1a9
#define MT9M131_REDUCER_YSIZE_A		0x1aa

#define MT9M131_OUTPUT_FORMAT_CTRL2_A	0x13a
#define MT9M131_OUTPUT_FORMAT_CTRL2_B	0x19b

#define MT9M131_OPMODE_AUTOEXPO_EN	(1 << 14)
#define MT9M131_OPMODE_AUTOWHITEBAL_EN	(1 << 1)

#define MT9M131_OUTFMT_PROCESSED_BAYER	(1 << 14)
#define MT9M131_OUTFMT_BYPASS_IFP	(1 << 10)
#define MT9M131_OUTFMT_INV_PIX_CLOCK	(1 << 9)
#define MT9M131_OUTFMT_RGB		(1 << 8)
#define MT9M131_OUTFMT_RGB565		(0x0 << 6)
#define MT9M131_OUTFMT_RGB555		(0x1 << 6)
#define MT9M131_OUTFMT_RGB444x		(0x2 << 6)
#define MT9M131_OUTFMT_RGBx444		(0x3 << 6)
#define MT9M131_OUTFMT_TST_RAMP_OFF	(0x0 << 4)
#define MT9M131_OUTFMT_TST_RAMP_COL	(0x1 << 4)
#define MT9M131_OUTFMT_TST_RAMP_ROW	(0x2 << 4)
#define MT9M131_OUTFMT_TST_RAMP_FRAME	(0x3 << 4)
#define MT9M131_OUTFMT_SHIFT_3_UP	(1 << 3)
#define MT9M131_OUTFMT_AVG_CHROMA	(1 << 2)
#define MT9M131_OUTFMT_SWAP_YCbCr_C_Y	(1 << 1)
#define MT9M131_OUTFMT_SWAP_RGB_EVEN	(1 << 1)
#define MT9M131_OUTFMT_SWAP_YCbCr_Cb_Cr	(1 << 0)


#define	MT9M131_SET_CONTEXT				0
#define	MT9M131_SET_AWB						1
static int context_value;


//##########################################################################################



/* i2c global variable for mt9m131*/
static struct i2c_client mt9m131_i2c_client;
static struct i2c_driver mt9m131_i2c_driver;
static int mt9m131_i2c_registration = 0;
static unsigned short mt9m131_output_ctrl;



struct device *mt9m131_i2c_dev;

static v4l2_std_id mt9m131_Cur_mode = MT9M131_MODE_VGA_30FPS;

/*	Function prototype*/
int mt9m131_ctrl(unsigned int cmd, void *arg, void *params);
static int mt9m131_init(void *arg, void **params);
static int mt9m131_cleanup(void *params);
static int mt9m131_configpca9543(void);
static int mt9m131_setstd(void *arg, void *params);
static int mt9m131_setformat(struct mt9m131_format_params *mt9tformats);
static int mt9m131_getformat(struct mt9m131_format_params *mt9tformats);
static int mt9m131_queryctrl(void *arg);
static int mt9m131_setgain(int arg);
static int mt9m131_getgain(int *arg);
static int mt9m131_getparams(void *arg);
static int mt9m131_setExposure(int arg);
static int mt9m131_setBin(int arg);
static int mt9m131_setBinning_Skip(int arg);
static int mt9m131_set50_60Hz(int arg);

/*i2c function proto types*/
#ifdef PO1150K
#define i2c_read_reg			i2c_smbus_read_byte_data
#define i2c_write_reg			i2c_smbus_write_byte_data

//  #define i2c_read_reg			i2c_smbus_read_word_data
//  #define i2c_write_reg			i2c_smbus_write_word_data
#else
static int i2c_read_reg(struct i2c_client *, unsigned short,
			unsigned short *, bool);
static int i2c_write_reg(struct i2c_client *, unsigned short,
			 unsigned short, bool);
#endif
static int _i2c_attach_client(struct i2c_client *, struct i2c_driver *,
			      struct i2c_adapter *, int);
static int _i2c_detach_client(struct i2c_client *);
static int mt9m131_i2c_probe_adapter(struct i2c_adapter *);
static int mt9m131_i2c_probe_adapter_1(struct i2c_adapter *adap); 
static int mt9m131_i2c_init(void);
void mt9m131_i2c_cleanup(void);


void mt9m131_readregs(void)
{

		
}
void mt9m131_writeregs(void)
{


}

void mt9m131_write_apro(int page, int addr, int val)
{

}

static int mt9m131_set_autowhitebalance(int on)
{

	return 0;
}

void mt9m131_ioctl_command(int page, int val)
{
	int err = 0;
	
	printk("mt9m131_ioctl_command page : %d\n",page);
	printk("mt9m131_ioctl_command val : %d\n",val);
			
	switch(page)
	{
		case MT9M131_SET_CONTEXT:
		{
			//val = 1; //ContextB:0(Full Power), ContextA:1(Low Power)
			err = mt9m131_set_context(val);
			if (err < 0) 
			{
				printk("mt9m131_set_context fails...");
				return err;
			} 
			
			break;
		}	
		case MT9M131_SET_AWB:
		{
			//val = 1; //1 : on, 0: off
			err = mt9m131_set_autowhitebalance(val);
			if (err < 0) 
			{
				printk("mt9m131_set_autowb fails...");
				return err;
			} 
			
			break;
		}	
	}
	
	return err;
}
int mt9m131_read_apro(int page, int addr)
{

	return 0; //(int)tmp;	
}

/*
 * ======== mt9m131_ctrl  ========
 */

/*This function will provide different control commands for MT9M131
		configuration.*/
int mt9m131_ctrl(unsigned int cmd, void *arg, void *params)
{
	int err = 0;

	//printk("In MT9M131_CTRL function...\n");
	switch (cmd) {
	
	case MT9M131_SET_STD:
		{
			printk("MT9M131 Set STD...\n");

			err = mt9m131_setstd(arg, params);
			if (err < 0) 
			{
				printk("MT9M131 set standard fails...");
				return err;
			} 
			
			break;
		}
	
	case MT9M131_GET_PARAMS:
		{
			printk("MT9M131_GET_PARAMS\n");
			struct mt9m131_params *vpfe_mt9m131params =
			    (struct mt9m131_params *)params;
			struct mt9m131_params *user_mt9m131params =
			    (struct mt9m131_params *)arg;

			err = mt9m131_getparams(arg);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev,
					"\nMT9M131 get parameters  fails...");
				return err;
			}
			/* Update the global parameter of vpfe_obj */
			memcpy(vpfe_mt9m131params, user_mt9m131params,
			       sizeof(struct mt9m131_params));

			break;
		}
	case MT9M131_INIT:
		{
			printk("MT9M131_INIT\n");
			err = mt9m131_init(arg, params);
			if (err < 0) {
				printk("Unable to initialize MT9M131...");
				return err;
			}
			break;
		}
	case MT9M131_READ_ALL:
		{
#if 0
			mt9m131_readregs();
			printk("test #########\n");
			int err = 0;
			unsigned short val;
			
			err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0,0x00, MT9M131_I2C_CONFIG);
			if (err >= 0) 
			{
				printk("	i2c_write_reg OK\n");
			}
			else
			{
				printk("	i2c_write_reg Err\n");
			}			
			
			err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_CHIP_VERSION,&val, MT9M131_I2C_CONFIG);
			if (err < 0) 
			{
					printk("error MT9M131_CHIP_VERSION\n");
					dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
					return err;
			}
			else
					printk("*** MT9M131_CHIP_VERSION : %04x\n",val);	

#endif
			printk("*** MT9M131_READ_ALL\n");	
					break;	
		}
	
	case MT9M131_WRITE_ALL:
		{
			printk("*** MT9M131_WRITE_ALL\n");	
				mt9m131_writeregs();
				break;
		}
		
	case MT9M131_CLEANUP:
		{
			printk("*** MT9M131 CLEANUP\n");	
			mt9m131_cleanup(params);

			break;
		}
	case VIDIOC_S_CTRL:
		{
			printk("*** MT9M131 S_CTRL\n");	
			struct v4l2_control *ctrl = arg;

			if (ctrl->id == V4L2_CID_GAIN) {
				err = mt9m131_setgain((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 set gain fails...");
					return err;
				}
			}
			else if (ctrl->id == V4L2_CID_EXPOSURE)
			{
				err = mt9m131_setExposure((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 set exposure fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_BIN)
			{
				err = mt9m131_setBin((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 set bin fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_50_60Hz)
			{
				err = mt9m131_set50_60Hz((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 set 50/60 fails...");
					return err;
				}
			}
			else if( ctrl->id == V4L2_CID_BIN_SKIP_MODE)
			{
				err = mt9m131_setBinning_Skip((int)ctrl->value);
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 set bin_skip fails...");
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
			printk("*** MT9M131 G_CTRL\n");	
			struct v4l2_control *ctrl = arg;

			if (ctrl->id == V4L2_CID_GAIN) {
				err = mt9m131_getgain((int *)
						      &(ctrl->value));
				if (err < 0) {
					dev_err(mt9m131_i2c_dev,
						"\n MT9M131 get gain fails...");
					return err;
				}
			} else {
				err = -EINVAL;
			}
			break;
		}
	case VIDIOC_QUERYCTRL:
		{
			printk("*** MT9M131 QUERYCTRL\n");	
			err = mt9m131_queryctrl(arg);
			break;
		}
	default:
		{
			dev_err(mt9m131_i2c_dev, "\n Undefined command");
			return -1;
		}

	}

	return err;
}

/*
 * ======== mt9m131_init  ========
 */
/*	This function will set the video format standart*/
static int mt9m131_init(void *arg, void **params)
{
	struct i2c_driver *driver = &mt9m131_i2c_driver;
	struct i2c_client *client = &mt9m131_i2c_client;
	struct mt9m131_params *temp_params = NULL;
	int utemp = 0;
	int err = 0;
	int devId_h = 0; 
	int devId_l = 0; 
	int cnt, i2cRet; 
	unsigned short val,tmp;

#if MT9M131_I2C_ENABLE
	/* Register MT9M131 I2C client */
	err = i2c_add_driver(driver);
	if (err) 
	{
		printk(KERN_ERR "Failed to register MT9M131 I2C client.\n");
		return -1;
	}
	else
	{
			printk("OK to register MT9M131 I2C client.\n");
	}	
	mt9m131_i2c_registration = MT9M131_I2C_REGISTERED;
#endif

	temp_params = kmalloc(sizeof(struct mt9m131_params), GFP_KERNEL);
	if (temp_params == NULL) {
		printk(KERN_ERR "\n Unable to allocate memory...");
		return -1;
	}
	(*params) = temp_params;

#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	utemp = PINMUX3;
	PINMUX3 = (utemp | 0x10000); // by xact 2008.11.06 PINMUX3[16] 1:CLKOUT 24MHz Clock out
	err = 0;
	gpio_set(MT9M131_RESET_PIN);
	//gpio_set(83);
	gpio_set_direction(MT9M131_RESET_PIN, GIO_DIR_OUTPUT);
	//gpio_set_direction(83, GIO_DIR_OUTPUT);
	gpio_clear(MT9M131_RESET_PIN);
	mdelay(10);//10ms
	gpio_set(MT9M131_RESET_PIN);
	mdelay(10);
#else
	if(IS_DM350){
		gpio_set(MT9M131_RESET_PIN);
		gpio_set_direction(MT9M131_RESET_PIN, GIO_DIR_OUTPUT);
		gpio_clear(MT9M131_RESET_PIN);
		mdelay(10);//10ms
		gpio_set(MT9M131_RESET_PIN);
		mdelay(10);
	}
	err = mt9m131_configpca9543();
#endif
	if(err >= 0)
	{
		i2cRet = 0; 
		i2cRet = i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);
		if(i2cRet == 0) 
			i2cRet = i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x00);

		if(i2cRet == 0) {
			devId_h = i2c_read_reg(&mt9m131_i2c_client, 0x00);
			devId_l = i2c_read_reg(&mt9m131_i2c_client, 0x01);	
		}

		printk("First Addr = 0x%04X Checking Camera ID is = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);
		printk("First Addr = 0x%04X Checking Camera ID is = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);
		printk("First Addr = 0x%04X Checking Camera ID is = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);

		if (((devId_h & 0x0FF) != 0x11) || ((devId_l & 0x0FF) != 0x50)) {

//  			for(cnt = 0; cnt < 10000; cnt++) {
//  				if (i2c_check_addr(client->adapter, client->addr) == 0) {
//  					break; 
//  				}
//  				mdelay(10);
//  			}

			//shcho
//  			client->addr = (0xE8 >> 1); 
			client->addr = MT9M131_I2C_ADDR ; 
			
			gpio_clear(MT9M131_RESET_PIN);
			mdelay(10);//10ms
			gpio_set(MT9M131_RESET_PIN);
			mdelay(10);

//  			printk("Counter %d\n", cnt); 

//  			i2c_write_reg(client, 0x03, 0x00);
//  			i2c_write_reg(client, 0x4D, 0x00);
//      
//  			devId_h = i2c_read_reg(client, 0x00);
//  			devId_l = i2c_read_reg(client, 0x01);	
//      
//  			printk("Second Addr = 0x%04X Checking Dev ID = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);
//  			printk("Second Addr = 0x%04X Checking Dev ID = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);
//  			printk("Second Addr = 0x%04X Checking Dev ID = 0x%04X 0x%04X\n", client->addr, devId_h, devId_l);
		}

	}
	else
	{
		mt9m131_cleanup((*params));
	}
	

	return err;
}

static int mt9m131_getformat(struct mt9m131_format_params *mt9tformats)
{

	return 0;
}

static int mt9m131_setformat(struct mt9m131_format_params *mt9tformats)
{

	return 0;
}
/*
 * ======== configPCA9543  ========
 */
/*	This function will configure PCA9543 control register*/
static int mt9m131_configpca9543()
{

	return 0;
}
/*
 * ======== mt9m131_cleanup  ========
 */

/*This function will free the memory allocated for mt9m131_params*/
static int mt9m131_cleanup(void *params)
{
	struct i2c_driver *driver = &mt9m131_i2c_driver;
	struct mt9m131_params *temp_params = (struct mt9m131_params *)params;
	if (temp_params != NULL)
		kfree(temp_params);
	params = NULL;
#if MT9M131_I2C_ENABLE
	if (mt9m131_i2c_registration) {
		i2c_detach_client(&mt9m131_i2c_client);
		i2c_del_driver(driver);
		mt9m131_i2c_client.adapter = NULL;
		mt9m131_i2c_registration = MT9M131_I2C_UNREGISTERED;
	}
#endif
	return 0;
}

/*
 * ======== mt9m131_setstd  ========
 */

/*	Function to set the video frame format*/
static int mt9m131_setstd(void *arg, void *params)
{

	int err = 0;
	v4l2_std_id mode = *(v4l2_std_id *) arg;
	//struct vpfe_obj *vp_apro = *(vpfe_obj *)params;
	struct mt9m131_format_params mt9tformats;

	int utemp = 0;


	gpio_set(MT9M131_RESET_PIN);
	gpio_set_direction(MT9M131_RESET_PIN, GIO_DIR_OUTPUT);
	gpio_clear(MT9M131_RESET_PIN);
	mdelay(10);//10ms
	gpio_set(MT9M131_RESET_PIN);
	mdelay(10);

#ifdef PO1150K
	// Start up... 
	// dummy command

	printk("Camera I2C Addr = 0x%04X\n", mt9m131_i2c_client.addr);
	printk("Camera I2C Addr = 0x%04X\n", mt9m131_i2c_client.addr);
	printk("Camera I2C Addr = 0x%04X\n", mt9m131_i2c_client.addr);

	err = i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);
	// output Hi-z release 
	i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x00);

	// Clock Setting...
	// PLL on  & make MCLK 48Mhz
//#if 1	//54 Mhz input   * (8 / 9) = 48Mhz
#if 0	//54 Mhz input   * (8 / 9) = 48Mhz
	i2c_write_reg(&mt9m131_i2c_client, 0x59, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x5A, 0x09);
	i2c_write_reg(&mt9m131_i2c_client, 0x5B, 0x08);

#else	//24Mhz * (2 / 1) = 48Mhz
	i2c_write_reg(&mt9m131_i2c_client, 0x59, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x5A, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x5B, 0x02);
#endif

	// clkdiv	- pclk to 24Mhz
	i2c_write_reg(&mt9m131_i2c_client, 0x5E, 0x00);

	// default setting from pixelplus
	//#################### BANK A ####################
	i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

	i2c_write_reg(&mt9m131_i2c_client, 0x27, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0x53, 0x7C);
	i2c_write_reg(&mt9m131_i2c_client, 0x57, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x58, 0x1F);
	i2c_write_reg(&mt9m131_i2c_client, 0x73, 0x00);
	//default set WH Mirror
#ifndef PO1150K_CONNECT_TYPE
	i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0xC7);
#else
	// HV mirror disable 
	#ifdef NEW_CMOS_CONNECT_HAVETO_NO_MIRROR
	i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x07);
	#else
	i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x07);
	#endif
#endif 
	i2c_write_reg(&mt9m131_i2c_client, 0x83, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0xA4, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xA6, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xAD, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xAE, 0xD2);
	i2c_write_reg(&mt9m131_i2c_client, 0xAF, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xB0, 0xC2);
	i2c_write_reg(&mt9m131_i2c_client, 0xB1, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xB2, 0xE5);
	i2c_write_reg(&mt9m131_i2c_client, 0xB5, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xB6, 0x53);
	i2c_write_reg(&mt9m131_i2c_client, 0xBE, 0x78);
	i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x8A);
	i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xAA);
	i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xEE, 0x9A);
	i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x53);
	i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0xD4);

	i2c_write_reg(&mt9m131_i2c_client, 0x0E, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x0F, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xA8);
	i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x15, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x16, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x17, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0x18, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x19, 0x11);
	i2c_write_reg(&mt9m131_i2c_client, 0x1A, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x1B, 0xD2);
	i2c_write_reg(&mt9m131_i2c_client, 0x1C, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x1D, 0xF0);
	i2c_write_reg(&mt9m131_i2c_client, 0x1E, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x1F, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x1A);
	i2c_write_reg(&mt9m131_i2c_client, 0x22, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x23, 0x39);
	i2c_write_reg(&mt9m131_i2c_client, 0x24, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0x25, 0xFC);
	i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x28);
	i2c_write_reg(&mt9m131_i2c_client, 0x62, 0x68);
	i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x66, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x85, 0x40);

	//#################### BANK B ####################
	i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x3C, 0x42);
	i2c_write_reg(&mt9m131_i2c_client, 0x3D, 0x9A);
	i2c_write_reg(&mt9m131_i2c_client, 0x3E, 0x88);
	i2c_write_reg(&mt9m131_i2c_client, 0x3F, 0x8C);
	i2c_write_reg(&mt9m131_i2c_client, 0x40, 0x3C);
	i2c_write_reg(&mt9m131_i2c_client, 0x41, 0x8F);
	i2c_write_reg(&mt9m131_i2c_client, 0x42, 0x81);
	i2c_write_reg(&mt9m131_i2c_client, 0x43, 0x99);
	i2c_write_reg(&mt9m131_i2c_client, 0x44, 0x3B);
	i2c_write_reg(&mt9m131_i2c_client, 0x45, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x07);
	i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x19);
	i2c_write_reg(&mt9m131_i2c_client, 0x4A, 0x2C);
	i2c_write_reg(&mt9m131_i2c_client, 0x4B, 0x3C);
	i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x55);
	i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x6A);
	i2c_write_reg(&mt9m131_i2c_client, 0x4E, 0x8B);
	i2c_write_reg(&mt9m131_i2c_client, 0x4F, 0xA7);
	i2c_write_reg(&mt9m131_i2c_client, 0x50, 0xC0);
	i2c_write_reg(&mt9m131_i2c_client, 0x51, 0xD6);
	i2c_write_reg(&mt9m131_i2c_client, 0x52, 0xEC);
	i2c_write_reg(&mt9m131_i2c_client, 0x53, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0x80, 0x50);
	i2c_write_reg(&mt9m131_i2c_client, 0x9D, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xA7, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xA8, 0x81);
	i2c_write_reg(&mt9m131_i2c_client, 0xA9, 0x88);
	i2c_write_reg(&mt9m131_i2c_client, 0xAA, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xAB, 0x9B);
	i2c_write_reg(&mt9m131_i2c_client, 0xAC, 0x70);

	i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x20);
	i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
	i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x0A);
	i2c_write_reg(&mt9m131_i2c_client, 0x6A, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0x6B, 0x20);
	i2c_write_reg(&mt9m131_i2c_client, 0x6C, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x6D, 0xDC);
	i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
	i2c_write_reg(&mt9m131_i2c_client, 0xBB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xBC, 0x1C);
	i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xA8);
	i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0xE4);
	i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xC6);
	i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x5A);
	i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0xB0);
	i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x05);
	i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xA8);
	i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0xE4);
	i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xC6);
	i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x5A);
	i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0xB0);
	i2c_write_reg(&mt9m131_i2c_client, 0xDF, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xE0, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0xE1, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0xE2, 0xD2);
	i2c_write_reg(&mt9m131_i2c_client, 0xE3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x11);
	i2c_write_reg(&mt9m131_i2c_client, 0xE5, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xE6, 0xF0);
	i2c_write_reg(&mt9m131_i2c_client, 0xE7, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xE8, 0xF4);
	i2c_write_reg(&mt9m131_i2c_client, 0xE9, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xEA, 0xE2);
	i2c_write_reg(&mt9m131_i2c_client, 0xEB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xEC, 0xB2);
	i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xEE, 0x50);
	i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0xF1, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0xF2, 0xD2);
	i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0x11);
	i2c_write_reg(&mt9m131_i2c_client, 0xF5, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xF6, 0xF0);
	i2c_write_reg(&mt9m131_i2c_client, 0xF7, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xF8, 0xF4);
	i2c_write_reg(&mt9m131_i2c_client, 0xF9, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xFA, 0xE2);
	i2c_write_reg(&mt9m131_i2c_client, 0xFB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xFC, 0xB2);
	i2c_write_reg(&mt9m131_i2c_client, 0xFD, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0xFE, 0x50);
	//#################### BANK C ####################
	i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x02);

	i2c_write_reg(&mt9m131_i2c_client, 0x05, 0x44);
	i2c_write_reg(&mt9m131_i2c_client, 0x06, 0x90);
	i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x13, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x15, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x16, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x17, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x18, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x19, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x1A, 0x42);
	i2c_write_reg(&mt9m131_i2c_client, 0x1B, 0x46);
	i2c_write_reg(&mt9m131_i2c_client, 0x1C, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x1D, 0x42);
	i2c_write_reg(&mt9m131_i2c_client, 0x1E, 0x45);
	i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x12);
	i2c_write_reg(&mt9m131_i2c_client, 0x22, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x23, 0x12);
	i2c_write_reg(&mt9m131_i2c_client, 0x24, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x25, 0x24);
	i2c_write_reg(&mt9m131_i2c_client, 0x26, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x27, 0x01);
	i2c_write_reg(&mt9m131_i2c_client, 0x28, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x29, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x2A, 0x42);
	i2c_write_reg(&mt9m131_i2c_client, 0x2B, 0x46);
	i2c_write_reg(&mt9m131_i2c_client, 0x2C, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x2D, 0x42);
	i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0x45);
	i2c_write_reg(&mt9m131_i2c_client, 0x3A, 0x44);
	i2c_write_reg(&mt9m131_i2c_client, 0x3C, 0x0C);
	i2c_write_reg(&mt9m131_i2c_client, 0x3E, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0x3F, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x40, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x41, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0x42, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0x43, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x84);
	i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x70);
	i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x70);
	i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x70);
	i2c_write_reg(&mt9m131_i2c_client, 0x4A, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x4B, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x06);
	i2c_write_reg(&mt9m131_i2c_client, 0x4E, 0x19);
	i2c_write_reg(&mt9m131_i2c_client, 0x4F, 0x32);
	i2c_write_reg(&mt9m131_i2c_client, 0x50, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x53, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x54, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x55, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x5E, 0x26);
	i2c_write_reg(&mt9m131_i2c_client, 0x5F, 0x46);
	i2c_write_reg(&mt9m131_i2c_client, 0x60, 0x36);
	i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x31);
	i2c_write_reg(&mt9m131_i2c_client, 0x62, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x63, 0x13);
	i2c_write_reg(&mt9m131_i2c_client, 0x64, 0x54);
	i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x29);
	i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x2E);
	i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x58);
	i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x1F);
	i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x09);
	i2c_write_reg(&mt9m131_i2c_client, 0x6A, 0x0E);
	i2c_write_reg(&mt9m131_i2c_client, 0x6B, 0x14);
	i2c_write_reg(&mt9m131_i2c_client, 0x6C, 0xFA);
	i2c_write_reg(&mt9m131_i2c_client, 0x6D, 0x0A);
	i2c_write_reg(&mt9m131_i2c_client, 0x6E, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0x6F, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0x70, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0x73, 0x7E);
	i2c_write_reg(&mt9m131_i2c_client, 0x74, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0x75, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0x76, 0x7E);
	i2c_write_reg(&mt9m131_i2c_client, 0x77, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0x78, 0x80);
	i2c_write_reg(&mt9m131_i2c_client, 0x79, 0x2F);
	// Bank C
	i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x3F);
	i2c_write_reg(&mt9m131_i2c_client, 0x7B, 0x51);
	i2c_write_reg(&mt9m131_i2c_client, 0x7E, 0x0C);
	i2c_write_reg(&mt9m131_i2c_client, 0x7F, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0x80, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0x81, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0x82, 0x69);
	i2c_write_reg(&mt9m131_i2c_client, 0x83, 0x69);
	i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x3F);
	i2c_write_reg(&mt9m131_i2c_client, 0x85, 0x3F);
	i2c_write_reg(&mt9m131_i2c_client, 0x86, 0x78);
	i2c_write_reg(&mt9m131_i2c_client, 0x87, 0x78);
	i2c_write_reg(&mt9m131_i2c_client, 0x88, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x89, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0x8A, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x8B, 0x06);
	i2c_write_reg(&mt9m131_i2c_client, 0x8C, 0x39);
	i2c_write_reg(&mt9m131_i2c_client, 0x8D, 0x8B);
	i2c_write_reg(&mt9m131_i2c_client, 0x8E, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x8F, 0x2E);
	i2c_write_reg(&mt9m131_i2c_client, 0x90, 0x34);
	i2c_write_reg(&mt9m131_i2c_client, 0x91, 0x84);
	i2c_write_reg(&mt9m131_i2c_client, 0x92, 0x84);
	i2c_write_reg(&mt9m131_i2c_client, 0x93, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0x94, 0x25);
	i2c_write_reg(&mt9m131_i2c_client, 0x95, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x96, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x97, 0x25);
	i2c_write_reg(&mt9m131_i2c_client, 0x98, 0x03);
	i2c_write_reg(&mt9m131_i2c_client, 0x99, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x9A, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0x9B, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x9C, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0x9D, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0x9E, 0x38);
	i2c_write_reg(&mt9m131_i2c_client, 0x9F, 0x52);
	i2c_write_reg(&mt9m131_i2c_client, 0xA0, 0x58);
	i2c_write_reg(&mt9m131_i2c_client, 0xA3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xA4, 0x20);
	i2c_write_reg(&mt9m131_i2c_client, 0xA5, 0x30);
	i2c_write_reg(&mt9m131_i2c_client, 0xA6, 0xFF);
	i2c_write_reg(&mt9m131_i2c_client, 0xA7, 0x7F);
	i2c_write_reg(&mt9m131_i2c_client, 0xAB, 0x18);
	i2c_write_reg(&mt9m131_i2c_client, 0xAC, 0x3C);
	i2c_write_reg(&mt9m131_i2c_client, 0xAD, 0x20);
	i2c_write_reg(&mt9m131_i2c_client, 0xAE, 0x3C);
	i2c_write_reg(&mt9m131_i2c_client, 0xAF, 0x70);
	i2c_write_reg(&mt9m131_i2c_client, 0xB0, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0xBB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xBC, 0x0C);
	i2c_write_reg(&mt9m131_i2c_client, 0xBD, 0x1C);
	i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x0C);
	i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x1C);
	i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x7F);
	i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x5F);
	i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x3F);
	i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x18);
	i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x10);
	i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x7F);
	i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x73);
	i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x5B);
	i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x7F);
	i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x73);
	i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x5B);
	i2c_write_reg(&mt9m131_i2c_client, 0xDF, 0x02);
	i2c_write_reg(&mt9m131_i2c_client, 0xE0, 0x04);
	i2c_write_reg(&mt9m131_i2c_client, 0xE1, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xE3, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xE5, 0x18);
	i2c_write_reg(&mt9m131_i2c_client, 0xE7, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xE8, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xE9, 0x18);
	i2c_write_reg(&mt9m131_i2c_client, 0xEB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xEC, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x08);
	i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x50);
	i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x48);
	i2c_write_reg(&mt9m131_i2c_client, 0xF1, 0x48);
	i2c_write_reg(&mt9m131_i2c_client, 0xF7, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0xF8, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0xF9, 0x40);
	i2c_write_reg(&mt9m131_i2c_client, 0xFB, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xFC, 0x00);
	i2c_write_reg(&mt9m131_i2c_client, 0xFD, 0x00);

	//#################### BANK D ####################
	i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x03);



#else
	//Set Default
	err = i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0009, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0029, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0008, MT9M131_I2C_CONFIG);
	mdelay(500);
	#if 1 //bt diable
	err = i2c_write_reg(&mt9m131_i2c_client, 0x013a, 0x0200, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x019b, 0x0200, MT9M131_I2C_CONFIG);
	#else //bt enable
	err = i2c_write_reg(&mt9m131_i2c_client, 0x013a, 0x0a00, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x019b, 0x0a00, MT9M131_I2C_CONFIG);
	#endif
#endif

#ifdef PO1150K
	/* Select configuration parameters as per video mode  */
	if (mode == MT9M131_MODE_VGA_30FPS) 
	{
		printk("MT9M131_MODE_VGA_30FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x2B);
		// windowy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x11);
		// windowx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xAA);
		// windowy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0xF0);
		// vsyncstartrow - 003C
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x3C);
		// vsyncstoprow - 03FC
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0xFC);
		// itcomp_factor - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x40);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x40);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 02D4
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0xD4);
		// auto_vstop - 001C
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x1C);
		// ae_fwx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x2B);
		// ae_fwx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xAA);
		// ae_fwy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x11);
		// ae_fwy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xF0);
		// ae_cwx1 - 0101
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0x01);
		// ae_cwx2 - 01D5
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xD5);
		// ae_cwy1 - 00B2
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0xB2);
		// ae_cwy2 - 0150
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0x50);
		// awb_fwx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x2B);
		// awb_fwx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xAA);
		// awb_fwy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x11);
		// awb_fwy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0xF0);
		// awb_cwx1 - 0101
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x01);
		// awb_cwx2 - 01D5
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xD5);
		// awb_cwy1 - 00B2
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0xB2);
		// awb_cwy2 - 0150
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x50);


		
		
	} 
	else if (mode == MT9M131_MODE_VGA_15FPS) 
	{
		printk("MT9M131_MODE_VGA_15FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x2B);
		// windowy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x13);
		// windowx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xAA);
		// windowy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0xF2);
		// vsyncstartrow - 0036
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x36);
		// vsyncstoprow - 03F6
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0xF6);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x40);
		// scaley - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x40);
		// scale_th - 02D4
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0xD4);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x2B);
		// ae_fwx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xAA);
		// ae_fwy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x13);
		// ae_fwy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xF2);
		// ae_cwx1 - 0101
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0x01);
		// ae_cwx2 - 01D5
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xD5);
		// ae_cwy1 - 00B4
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0xB4);
		// ae_cwy2 - 0152
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0x52);
		// awb_fwx1 - 002B
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x2B);
		// awb_fwx2 - 02AA
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xAA);
		// awb_fwy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x13);
		// awb_fwy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0xF2);
		// awb_cwx1 - 0101
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x01);
		// awb_cwx2 - 01D5
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xD5);
		// awb_cwy1 - 00B4
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0xB4);
		// awb_cwy2 - 0152
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x52);

	}
	else if (mode == MT9M131_MODE_SXVGA_960) //1280x960
	{
		printk("MT9M131_MODE_SXVGA_960 in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x55);
		// windowy1 - 0025
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x25);
		// windowx2 - 0554
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0x54);
		// windowy2 - 03E4
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0xE4);
		// vsyncstartrow - 0036
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x36);
		// vsyncstoprow - 03F6
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0xF6);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x20);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 000A
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x0A);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x55);
		// ae_fwx2 - 0515 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0x15);
		// ae_fwy1 - 0025
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x25);
		// ae_fwy2 - 03E4
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xE4);
		// ae_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0x00);
		// ae_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xAA);
		// ae_cwy1 - 0166 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x66);
		// ae_cwy2 - 02A4 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0xA4);
		// awb_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x55);
		// awb_fwx2 - 0515 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0x15);
		// awb_fwy1 - 0025
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x25);
		// awb_fwy2 - 03E4
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0xE4);
		// awb_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x00);
		// awb_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xAA);
		// awb_cwy1 - 0166 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x66);
		// awb_cwy2 - 02A4 ***
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0xA4);

	}	
	else if (mode == MT9M131_MODE_SXVGA_15FPS) //1280x1024
	{
		printk("MT9M131_MODE_SXVGA_15FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x55);
		// windowy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x05);
		// windowx2 - 0554
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0x54);
		// windowy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0x04);
		// vsyncstartrow - 0016
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x16);
		// vsyncstoprow - 0416
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x16);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x20);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 000A
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x0A);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x55);
		// ae_fwx2 - 0504
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0x04);
		// ae_fwy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x05);
		// ae_fwy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0x04);
		// ae_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0x00);
		// ae_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xAA);
		// ae_cwy1 - 015B
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x5B);
		// ae_cwy2 - 02AF
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0xAF);
		// awb_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x55);
		// awb_fwx2 - 0504
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0x04);
		// awb_fwy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x05);
		// awb_fwy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0x04);
		// awb_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x00);
		// awb_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xAA);
		// awb_cwy1 - 015B
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x5B);
		// awb_cwy2 - 02AF
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0xAF);

	}
	else if (mode == MT9M131_MODE_P1280_720) 
	{
		printk("MT9M131_MODE_P1280_720 in mt9m131.c\n");
#if 0
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x55);
		// windowy1 - 009D
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x9D);
		// windowx2 - 0554
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0x54);
		// windowy2 - 036C
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0x6C);
		// vsyncstartrow - 00AE
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0xAE);
		// vsyncstoprow - 037E
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x7E);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x20);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 000A
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x0A);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x55);
		// ae_fwx2 - 0554
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0x54);
		// ae_fwy1 - 009D
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x9D);
		// ae_fwy2 - 036C
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0x6C);
		// ae_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0x00);
		// ae_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xAA);
		// ae_cwy1 - 018E
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x8E);
		// ae_cwy2 - 027C
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0x7C);
		// awb_fwx1 - 0055
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x55);
		// awb_fwx2 - 0544
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0x54);
		// awb_fwy1 - 009D
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x9D);
		// awb_fwy2 - 036C
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0x6C);
		// awb_cwx1 - 0200
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x00);
		// awb_cwx2 - 03AA
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xAA);
		// awb_cwy1 - 018E
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x8E);
		// awb_cwy2 - 027C
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x7C);
#endif
		//BANK A
		//W0300
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);
		// W0406 W0507 frame width
		i2c_write_reg(&mt9m131_i2c_client, 0x04, 0x06);
		i2c_write_reg(&mt9m131_i2c_client, 0x05, 0x07);

		//mode1
		// W0A04 W0B23 W0C04 W0D23 FD_FH
//  		i2c_write_reg(&mt9m131_i2c_client, 0x0A, 0x04);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x0B, 0x23);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x0C, 0x04);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x23);

		// W1600 W1755 W1800 W199D W1A05 W1B54 W1C03 W1D6C  Windows
		i2c_write_reg(&mt9m131_i2c_client, 0x16, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x17, 0x55);
		i2c_write_reg(&mt9m131_i2c_client, 0x18, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x19, 0x9D);
		i2c_write_reg(&mt9m131_i2c_client, 0x1A, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0x1B, 0x54);
		i2c_write_reg(&mt9m131_i2c_client, 0x1C, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x1D, 0x6C);

		// W2200 W23AE W2403 W257E Vsync
		i2c_write_reg(&mt9m131_i2c_client, 0x22, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x23, 0xAE);
		i2c_write_reg(&mt9m131_i2c_client, 0x24, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x25, 0x7E);

		// W6228	 Skip
		i2c_write_reg(&mt9m131_i2c_client, 0x62, 0x28);
		//W8520 Itcomp
		i2c_write_reg(&mt9m131_i2c_client, 0x85, 0x20);
		// W5F01 mode select
		i2c_write_reg(&mt9m131_i2c_client, 0x5F, 0x01);

		// W7AC7 VH mirror
 	#ifndef PO1150K_CONNECT_TYPE
		i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0xC7);
	#else 
		// HV mirror disable 
		#ifdef NEW_CMOS_CONNECT_HAVETO_NO_MIRROR
		i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x07);
		#else
		i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x07);
		#endif
	#endif 
		//W2740 W3910 W4D00 W537C W5701 W581F W5E00 W6000
		i2c_write_reg(&mt9m131_i2c_client, 0x27, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0x39, 0x10);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x53, 0x7C);
		i2c_write_reg(&mt9m131_i2c_client, 0x57, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x58, 0x1F);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x5E, 0x00);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x60, 0x00);

		//W6500 W66FF W7300 W7A07 W83FF WA401 WA608
//  		i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x00);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0xFF);
		i2c_write_reg(&mt9m131_i2c_client, 0x73, 0x00);
		//i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x07);
		i2c_write_reg(&mt9m131_i2c_client, 0x83, 0xFF);
		i2c_write_reg(&mt9m131_i2c_client, 0xA4, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xA6, 0x08);

		//WAD05 WAED2 WAF05 WB0C2 WB105 WB2E5 WB505 WB653 WBE30 WC48A
		i2c_write_reg(&mt9m131_i2c_client, 0xAD, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xAE, 0xD2);
		i2c_write_reg(&mt9m131_i2c_client, 0xAF, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xB0, 0xC2);
		i2c_write_reg(&mt9m131_i2c_client, 0xB1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xB2, 0xE5);
		i2c_write_reg(&mt9m131_i2c_client, 0xB5, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xB6, 0x53);
		i2c_write_reg(&mt9m131_i2c_client, 0xBE, 0x30);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x8A);

		//WC6AA WE480 WED00 WEE9A WEF05 WF053 WF305 WF4D4
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xAA);
		i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x80);
		i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xEE, 0x9A);
		i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x53);
		i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0xD4);


		//BANK B
		//W0301
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		//W3626 W3C42 W3D9A W3E88 W3F8C W403C W418F W4281 W4399 W443B
		i2c_write_reg(&mt9m131_i2c_client, 0x36, 0x26);
		i2c_write_reg(&mt9m131_i2c_client, 0x3C, 0x42);
		i2c_write_reg(&mt9m131_i2c_client, 0x3D, 0x9A);
		i2c_write_reg(&mt9m131_i2c_client, 0x3E, 0x88);
		i2c_write_reg(&mt9m131_i2c_client, 0x3F, 0x8C);
		i2c_write_reg(&mt9m131_i2c_client, 0x40, 0x3C);
		i2c_write_reg(&mt9m131_i2c_client, 0x41, 0x8F);
		i2c_write_reg(&mt9m131_i2c_client, 0x42, 0x81);
		i2c_write_reg(&mt9m131_i2c_client, 0x43, 0x99);
		i2c_write_reg(&mt9m131_i2c_client, 0x44, 0x3B);

		//WBB00 WBC0E
//  		i2c_write_reg(&mt9m131_i2c_client, 0xBB, 0x00);
//  		i2c_write_reg(&mt9m131_i2c_client, 0xBC, 0x0E);
		i2c_write_reg(&mt9m131_i2c_client, 0x45, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x07);
		i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x19);
		i2c_write_reg(&mt9m131_i2c_client, 0x4A, 0x2C);
		i2c_write_reg(&mt9m131_i2c_client, 0x4B, 0x3C);
		i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x55);
		i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x6A);
		i2c_write_reg(&mt9m131_i2c_client, 0x4E, 0x8B);
		i2c_write_reg(&mt9m131_i2c_client, 0x4F, 0xA7);
		i2c_write_reg(&mt9m131_i2c_client, 0x50, 0xC0);
		i2c_write_reg(&mt9m131_i2c_client, 0x51, 0xD6);
		i2c_write_reg(&mt9m131_i2c_client, 0x52, 0xEC);
		i2c_write_reg(&mt9m131_i2c_client, 0x53, 0xFF);


		//W8050 W9D08 WA700 WA881 WA988 WAA00 WAB9B
		i2c_write_reg(&mt9m131_i2c_client, 0x80, 0x50);
		i2c_write_reg(&mt9m131_i2c_client, 0x9D, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xA7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xA8, 0x81);
		i2c_write_reg(&mt9m131_i2c_client, 0xA9, 0x88);
		i2c_write_reg(&mt9m131_i2c_client, 0xAA, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xAB, 0x9B);

		//WAC70
		i2c_write_reg(&mt9m131_i2c_client, 0xAC, 0x70);
		//W4500 W4603 W470C W4819 W4926 W4A3F W4B52 W4C6E   gamma
		i2c_write_reg(&mt9m131_i2c_client, 0x45, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x0C);
		i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x19);
		i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x26);
		i2c_write_reg(&mt9m131_i2c_client, 0x4A, 0x3F);
		i2c_write_reg(&mt9m131_i2c_client, 0x4B, 0x52);
		i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x6E);

		//W4D82 W4EA1 W4FB9 W50CE W51E0 W52F0 W53FF
		i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x82);
		i2c_write_reg(&mt9m131_i2c_client, 0x4E, 0xA1);
		i2c_write_reg(&mt9m131_i2c_client, 0x4F, 0xB9);
		i2c_write_reg(&mt9m131_i2c_client, 0x50, 0xCE);
		i2c_write_reg(&mt9m131_i2c_client, 0x51, 0xE0);
		i2c_write_reg(&mt9m131_i2c_client, 0x52, 0xF0);
		i2c_write_reg(&mt9m131_i2c_client, 0x53, 0xFF);

		//W6A20 W6B20 W6C00 W6D0A  mode 1
		i2c_write_reg(&mt9m131_i2c_client, 0x6A, 0x20);
		i2c_write_reg(&mt9m131_i2c_client, 0x6B, 0x20);
		i2c_write_reg(&mt9m131_i2c_client, 0x6C, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x6D, 0x0A);

		//WBB00 WBC1C
		i2c_write_reg(&mt9m131_i2c_client, 0xBB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBC, 0x1C);


		//WDF00 WE055 WE105 WE254 WE300 WE49D WE503 WE66C    # ae full area
		i2c_write_reg(&mt9m131_i2c_client, 0xDF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xE0, 0x55);
		i2c_write_reg(&mt9m131_i2c_client, 0xE1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xE2, 0x54);
		i2c_write_reg(&mt9m131_i2c_client, 0xE3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x9D);
		i2c_write_reg(&mt9m131_i2c_client, 0xE5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xE6, 0x6C);

		// WE702 WE800 WE903 WEAAA WEB01 WEC8E WED02 WEE7C    # ae center area
		i2c_write_reg(&mt9m131_i2c_client, 0xE7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xE8, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xE9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xEA, 0xAA);
		i2c_write_reg(&mt9m131_i2c_client, 0xEB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xEC, 0x8E);
		i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xEE, 0x7C);

		// WEF00 WF055 WF105 WF254 WF300 WF49D WF503 WF66C    # awb full area
		i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x55);
		i2c_write_reg(&mt9m131_i2c_client, 0xF1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xF2, 0x54);
		i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0x9D);
		i2c_write_reg(&mt9m131_i2c_client, 0xF5, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xF6, 0x6C);


		//WF702 WF800 WF903 WFAAA WFB01 WFC8E WFD02 WFE7C    # awb center area
		i2c_write_reg(&mt9m131_i2c_client, 0xF7, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xF8, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xF9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xFA, 0xAA);
		i2c_write_reg(&mt9m131_i2c_client, 0xFB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xFC, 0x8E);
		i2c_write_reg(&mt9m131_i2c_client, 0xFD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xFE, 0x7C);

		// BANK C
		//W0302
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x02);
		//W0564 W0690
		i2c_write_reg(&mt9m131_i2c_client, 0x05, 0x64);
		i2c_write_reg(&mt9m131_i2c_client, 0x06, 0x90);

		// W2002 W2112 Mode 1
		//i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x02);
		//i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x12);
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x04);//YUN 110526 better low sensitivity 
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x14);//YUN 110526 better low sensitivity
		//W2204 W2324
		i2c_write_reg(&mt9m131_i2c_client, 0x22, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x23, 0x24);

		//15 fps W2404 W2524
		i2c_write_reg(&mt9m131_i2c_client, 0x24, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x25, 0x24);

		i2c_write_reg(&mt9m131_i2c_client, 0x26, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x27, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x28, 0x00);
		//W2900 W2A10 W2B90
		i2c_write_reg(&mt9m131_i2c_client, 0x29, 0x00);
		//i2c_write_reg(&mt9m131_i2c_client, 0x2A, 0x10);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x2A, 0x0C);//YUN 110526 better low sensitivity
		i2c_write_reg(&mt9m131_i2c_client, 0x2A, 0x14);
		//i2c_write_reg(&mt9m131_i2c_client, 0x2B, 0x90);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x2B, 0x6C);//YUN 110526 better low sensitivity
		i2c_write_reg(&mt9m131_i2c_client, 0x2B, 0xB4);

		// tatol gain x8 W2C00 W2D10 W2E90
		i2c_write_reg(&mt9m131_i2c_client, 0x2C, 0x00);
		//i2c_write_reg(&mt9m131_i2c_client, 0x2D, 0x10);
		//i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0x90);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x2D, 0x0C);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0x2D, 0x14);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0x6B);//YUN Total gain x3
		i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0xB4);
		//W3A44 W3C0C
		i2c_write_reg(&mt9m131_i2c_client, 0x3A, 0x44);
		i2c_write_reg(&mt9m131_i2c_client, 0x3C, 0x0C);

		i2c_write_reg(&mt9m131_i2c_client, 0x3E, 0x20);
		i2c_write_reg(&mt9m131_i2c_client, 0x3F, 0x20);
	
		//W4010 W4140 W4280 W43FF
		i2c_write_reg(&mt9m131_i2c_client, 0x40, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0x41, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0x42, 0x80);
		i2c_write_reg(&mt9m131_i2c_client, 0x43, 0xFF);

		//ae weight W3E20 W3F20
//  		i2c_write_reg(&mt9m131_i2c_client, 0x3E, 0x20);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x3F, 0x20);

		//target W4676 W4750 W4876 W4950
//  		i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x76);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x50);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x76);
//  		i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x50);
		//W4684 W4770 W4870 W4970 W4A10 W4B40 W4C03 W4D06 W4E19 W4F32
		i2c_write_reg(&mt9m131_i2c_client, 0x46, 0x84);
		i2c_write_reg(&mt9m131_i2c_client, 0x47, 0x70);
		i2c_write_reg(&mt9m131_i2c_client, 0x48, 0x70);
		i2c_write_reg(&mt9m131_i2c_client, 0x49, 0x70);
		i2c_write_reg(&mt9m131_i2c_client, 0x4A, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0x4B, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x06);
		i2c_write_reg(&mt9m131_i2c_client, 0x4E, 0x19);
		i2c_write_reg(&mt9m131_i2c_client, 0x4F, 0x32);
		//W5008 W5308 W5408 W5508 W5E26 W5F46 W6036 W6131 W6208 W6313
		i2c_write_reg(&mt9m131_i2c_client, 0x50, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0x53, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0x54, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0x55, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0x5E, 0x26);
		i2c_write_reg(&mt9m131_i2c_client, 0x5F, 0x46);
		i2c_write_reg(&mt9m131_i2c_client, 0x60, 0x36);
		i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x31);
		i2c_write_reg(&mt9m131_i2c_client, 0x62, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0x63, 0x13);
		//W6454 W6529 W662E W6758 W681F W6909 W6A0E W6B14 W6CFA W6D0A
		i2c_write_reg(&mt9m131_i2c_client, 0x64, 0x54);
		i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x29);
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x2E);
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x58);
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x1F);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x09);
		i2c_write_reg(&mt9m131_i2c_client, 0x6A, 0x0E);
		i2c_write_reg(&mt9m131_i2c_client, 0x6B, 0x14);
		i2c_write_reg(&mt9m131_i2c_client, 0x6C, 0xFA);
		i2c_write_reg(&mt9m131_i2c_client, 0x6D, 0x0A);
		//W6EFF W6F10 W7030
		i2c_write_reg(&mt9m131_i2c_client, 0x6E, 0xFF);
		i2c_write_reg(&mt9m131_i2c_client, 0x6F, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0x70, 0x30);

		//AWB ratio W737E W7480
		i2c_write_reg(&mt9m131_i2c_client, 0x73, 0x7E);
		i2c_write_reg(&mt9m131_i2c_client, 0x74, 0x80);
		//W7580 W767E
		i2c_write_reg(&mt9m131_i2c_client, 0x75, 0x80);
		i2c_write_reg(&mt9m131_i2c_client, 0x76, 0x7E);
		//W7780 W7880
		i2c_write_reg(&mt9m131_i2c_client, 0x77, 0x80);
		i2c_write_reg(&mt9m131_i2c_client, 0x78, 0x80);
		//W792F W7A3F W7B51
		i2c_write_reg(&mt9m131_i2c_client, 0x79, 0x2F);
		//  this area is  Bank C
		i2c_write_reg(&mt9m131_i2c_client, 0x7A, 0x3F);

		i2c_write_reg(&mt9m131_i2c_client, 0x7B, 0x51);
		//W7E0C W7F08
		i2c_write_reg(&mt9m131_i2c_client, 0x7E, 0x0C);
		i2c_write_reg(&mt9m131_i2c_client, 0x7F, 0x08);

		//AWB Min/Max W8030 W8130
		i2c_write_reg(&mt9m131_i2c_client, 0x80, 0x30);
		i2c_write_reg(&mt9m131_i2c_client, 0x81, 0x30);
		//W8269 W8369
		i2c_write_reg(&mt9m131_i2c_client, 0x82, 0x69);
		i2c_write_reg(&mt9m131_i2c_client, 0x83, 0x69);
		//W843F W853F
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x3F);
		i2c_write_reg(&mt9m131_i2c_client, 0x85, 0x3F);
		//W8678 W8778
		i2c_write_reg(&mt9m131_i2c_client, 0x86, 0x78);
		i2c_write_reg(&mt9m131_i2c_client, 0x87, 0x78);
		//W8802 W8903 W8A04 W8B06
		i2c_write_reg(&mt9m131_i2c_client, 0x88, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x89, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x8A, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x8B, 0x06);

		// CS W8C39 W8D9B W8E00 W8F2E
		i2c_write_reg(&mt9m131_i2c_client, 0x8C, 0x39);
		i2c_write_reg(&mt9m131_i2c_client, 0x8D, 0x9B);
		i2c_write_reg(&mt9m131_i2c_client, 0x8E, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x8F, 0x2E);
		//W9034 W9194 W9284 W9330
		i2c_write_reg(&mt9m131_i2c_client, 0x90, 0x34);
		i2c_write_reg(&mt9m131_i2c_client, 0x91, 0x94);
		i2c_write_reg(&mt9m131_i2c_client, 0x92, 0x84);
		//i2c_write_reg(&mt9m131_i2c_client, 0x93, 0x30);
		i2c_write_reg(&mt9m131_i2c_client, 0x93, 0x34);//YUN
		//W9425 W9590 W9600 W9725
		i2c_write_reg(&mt9m131_i2c_client, 0x94, 0x25);
		i2c_write_reg(&mt9m131_i2c_client, 0x95, 0x90);
		i2c_write_reg(&mt9m131_i2c_client, 0x96, 0x00);
		//i2c_write_reg(&mt9m131_i2c_client, 0x97, 0x25);
		i2c_write_reg(&mt9m131_i2c_client, 0x97, 0x2A);//YUN

		//lens W9803 W9900
		i2c_write_reg(&mt9m131_i2c_client, 0x98, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x99, 0x00);
		//W9A04 W9B00
		i2c_write_reg(&mt9m131_i2c_client, 0x9A, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x9B, 0x00);
		//W9C02 W9D00
		i2c_write_reg(&mt9m131_i2c_client, 0x9C, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x9D, 0x00);
		//user_cs WA120
		//i2c_write_reg(&mt9m131_i2c_client, 0xA1, 0x20);
//  		i2c_write_reg(&mt9m131_i2c_client, 0xA1, 0x28);//YUN
		//W9E38 W9F52 WA058
		i2c_write_reg(&mt9m131_i2c_client, 0x9E, 0x38);
		i2c_write_reg(&mt9m131_i2c_client, 0x9F, 0x52);
		i2c_write_reg(&mt9m131_i2c_client, 0xA0, 0x58);
	
		i2c_write_reg(&mt9m131_i2c_client, 0xA1, 0x28);
		//WA300 WA420 WA530 WA6FF WA77F
		i2c_write_reg(&mt9m131_i2c_client, 0xA3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xA4, 0x20);
		i2c_write_reg(&mt9m131_i2c_client, 0xA5, 0x30);
		i2c_write_reg(&mt9m131_i2c_client, 0xA6, 0xFF);
		i2c_write_reg(&mt9m131_i2c_client, 0xA7, 0x7F);
		//WAB08 WAC10 WAD10 WAE1C dark_Xref
		//i2c_write_reg(&mt9m131_i2c_client, 0xAB, 0x08);
		//i2c_write_reg(&mt9m131_i2c_client, 0xAC, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0xAB, 0x03);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xAC, 0x06);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xAD, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0xAE, 0x1C);
		//WAF76 WB010 dark_Ymean
		//i2c_write_reg(&mt9m131_i2c_client, 0xAF, 0x76);
		i2c_write_reg(&mt9m131_i2c_client, 0xAF, 0x50);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xB0, 0x10);
		//WBB00 WBC0C
		i2c_write_reg(&mt9m131_i2c_client, 0xBB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBC, 0x0C);
		//WBD1C WBF00 WC00C WC11C
		i2c_write_reg(&mt9m131_i2c_client, 0xBD, 0x1C);
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x0C);
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x1C);
		//dark_blf WC37F WC45F WC53F
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x7F);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x5F);
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x3F);
		//WCB00 WCC08 WCD18
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x18);
		//WD304 WD408 WD510
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x10);
		//WD77F WD873 WD95B
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x7F);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0x73);
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x5B);
		//WDB7F WDC73 WDD5B
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x7F);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x73);
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x5B);

		//dark_edge_th WDF04 WE008 WE108
		i2c_write_reg(&mt9m131_i2c_client, 0xDF, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xE0, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xE1, 0x08);

		// dark_Y_gm WE300 WE408 WE510
		i2c_write_reg(&mt9m131_i2c_client, 0xE3, 0x00);
		//i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x08);
		//i2c_write_reg(&mt9m131_i2c_client, 0xE5, 0x10);
		i2c_write_reg(&mt9m131_i2c_client, 0xE4, 0x00);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xE5, 0x00);//YUN

		//dark_rgb_gm WE700 WE808 WE918
		i2c_write_reg(&mt9m131_i2c_client, 0xE7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xE8, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xE9, 0x18);

		//dark_ccr WEB00 WEC08 WED10
		i2c_write_reg(&mt9m131_i2c_client, 0xEB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xEC, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xED, 0x10);

		//dark_y contrast WEF40 WF040 WF140
		i2c_write_reg(&mt9m131_i2c_client, 0xEF, 0x40);
		//i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x40);
		//i2c_write_reg(&mt9m131_i2c_client, 0xF1, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x4C);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xF1, 0x58);//YUN

		//dark_y brighness WF306 WF406 WF50A
		//i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x06);
		//i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0x06);
		//i2c_write_reg(&mt9m131_i2c_client, 0xF5, 0x0A);
		i2c_write_reg(&mt9m131_i2c_client, 0xF3, 0x04);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xF4, 0x08);//YUN
		i2c_write_reg(&mt9m131_i2c_client, 0xF5, 0x10);//YUN
		//WF740 WF840 WF940
		i2c_write_reg(&mt9m131_i2c_client, 0xF7, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0xF8, 0x40);
		i2c_write_reg(&mt9m131_i2c_client, 0xF9, 0x40);
		//WFB00 WFC08 WFD10
		i2c_write_reg(&mt9m131_i2c_client, 0xFB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xFC, 0x08);
		i2c_write_reg(&mt9m131_i2c_client, 0xFD, 0x10);

	}	
	else if (mode == MT9M131_MODE_D1_NTSC_30FPS) 
	{
		printk("MT9M131_MODE_D1_NTSC_30FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x03);
		// windowy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x11);
		// windowx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xD2);
		// windowy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0xF0);
		// vsyncstartrow - 003C
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x3C);
		// vsyncstoprow - 03FC
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0xFC);
		// itcomp_factor - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x40);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x40);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 02DD
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0xDD);
		// auto_vstop - 001C
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x1C);
		// ae_fwx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x03);
		// ae_fwx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xD2);
		// ae_fwy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x11);
		// ae_fwy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xF0);
		// ae_cwx1 - 00F4
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0xF4);
		// ae_cwx2 - 01E2
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xE2);
		// ae_cwy1 - 00B2
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0xB2);
		// ae_cwy2 - 0150
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0x50);
		// awb_fwx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x03);
		// awb_fwx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xD2);
		// awb_fwy1 - 0011
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x11);
		// awb_fwy2 - 01F0
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0xF0);
		// awb_cwx1 - 00F4
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0xF4);
		// awb_cwx2 - 01E2
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xE2);
		// awb_cwy1 - 00B2
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0xB2);
		// awb_cwy2 - 0150
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x50);

	}	
	else if (mode == MT9M131_MODE_D1_NTSC_15FPS) 
	{
		printk("MT9M131_MODE_D1_NTSC_15FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x03);
		// windowy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x13);
		// windowx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xD2);
		// windowy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0xF2);
		// vsyncstartrow - 0036
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x36);
		// vsyncstoprow - 03F6
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0xF6);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x40);
		// scaley - 40
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x40);
		// scale_th - 02DD
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0xDD);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x03);
		// ae_fwx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xD2);
		// ae_fwy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x13);
		// ae_fwy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0xF2);
		// ae_cwx1 - 00F4
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0xF4);
		// ae_cwx2 - 01E2
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xE2);
		// ae_cwy1 - 00B4
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0xB4);
		// ae_cwy2 - 0152
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0x52);
		// awb_fwx1 - 0003
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x03);
		// awb_fwx2 - 02D2
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xD2);
		// awb_fwy1 - 0013
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x13);
		// awb_fwy2 - 01F2
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0xF2);
		// awb_cwx1 - 00F4
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0xF4);
		// awb_cwx2 - 01E2
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xE2);
		// awb_cwy1 - 00B4
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0xB4);
		// awb_cwy2 - 0152
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x52);

	}	
	else if (mode == MT9M131_MODE_1440_1024_15FPS) 
	{
		printk("MT9M131_MODE_1440_1024_15FPS in mt9m131.c\n");
		// Group A
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x00);

		// Y-Skip
		//i2c_write_reg(&mt9m131_i2c_client, 0x61, 0x68);
		// windowx1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0x0e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x0f, 0x05);
		// windowy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x05);
		// windowx2 - 05A4
		i2c_write_reg(&mt9m131_i2c_client, 0x12, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0x13, 0xA4);
		// windowy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0x14, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x15, 0x04);
		// vsyncstartrow - 0016
		i2c_write_reg(&mt9m131_i2c_client, 0x1e, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x1f, 0x16);
		// vsyncstoprow - 0416
		i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x16);
		// itcomp_factor - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x84, 0x20);
		
		// Group B
		i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x01);

		// scalex - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x20);
		// scaley - 20
		i2c_write_reg(&mt9m131_i2c_client, 0x67, 0x20);
		// scale_th - 000A
		i2c_write_reg(&mt9m131_i2c_client, 0x68, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0x69, 0x0A);
		// auto_vstop - 000E
		i2c_write_reg(&mt9m131_i2c_client, 0xB9, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xBA, 0x0E);
		// ae_fwx1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xBF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC0, 0x05);
		// ae_fwx2 - 05A4
		i2c_write_reg(&mt9m131_i2c_client, 0xC1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xC2, 0xA4);
		// ae_fwy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xC3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xC4, 0x05);
		// ae_fwy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0xC5, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xC6, 0x04);
		// ae_cwx1 - 01E5
		i2c_write_reg(&mt9m131_i2c_client, 0xC7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xC8, 0xE5);
		// ae_cwx2 - 03C4
		i2c_write_reg(&mt9m131_i2c_client, 0xC9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xCA, 0xC4);
		// ae_cwy1 - 015A
		i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xCC, 0x5A);
		// ae_cwy2 - 02AF
		i2c_write_reg(&mt9m131_i2c_client, 0xCD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xCE, 0xAF);
		// awb_fwx1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xCF, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD0, 0x05);
		// awb_fwx2 - 05A4
		i2c_write_reg(&mt9m131_i2c_client, 0xD1, 0x05);
		i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0xA4);
		// awb_fwy1 - 0005
		i2c_write_reg(&mt9m131_i2c_client, 0xD3, 0x00);
		i2c_write_reg(&mt9m131_i2c_client, 0xD4, 0x05);
		// awb_fwy2 - 0404
		i2c_write_reg(&mt9m131_i2c_client, 0xD5, 0x04);
		i2c_write_reg(&mt9m131_i2c_client, 0xD6, 0x04);
		// awb_cwx1 - 01E5
		i2c_write_reg(&mt9m131_i2c_client, 0xD7, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xD8, 0xE5);
		// awb_cwx2 - 03C4
		i2c_write_reg(&mt9m131_i2c_client, 0xD9, 0x03);
		i2c_write_reg(&mt9m131_i2c_client, 0xDA, 0xC4);
		// awb_cwy1 - 015A
		i2c_write_reg(&mt9m131_i2c_client, 0xDB, 0x01);
		i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x5A);
		// awb_cwy2 - 02AF
		i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x02);
		i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0xAF);

	}	
	else 
	{
		printk(KERN_ERR "\n Invalid frame format");
		return -1;
	}
#else
	/* Select configuration parameters as per video mode  */
	if (mode == MT9M131_MODE_VGA_30FPS) 
	{
		printk("MT9M131_MODE_VGA_30FPS in mt9m131.c\n");
		//[Default Registers-NAVICAM Normal Condition 640X480 for 54MHz]
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x0048, MT9M131_I2C_CONFIG);     // 20100721
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0002, 0x005E, MT9M131_I2C_CONFIG);     // 20100721
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0020, 0x010a, MT9M131_I2C_CONFIG);     // Read Mode Context B
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0022, 0x0D0F, MT9M131_I2C_CONFIG);     // Dark col / rows
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0024, 0x8000, MT9M131_I2C_CONFIG);     // Extra Reset
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0059, 0x00FF, MT9M131_I2C_CONFIG);     // Black Rows

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x361a , MT9M131_I2C_CONFIG);	// Clock - 27M
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x3618 , MT9M131_I2C_CONFIG);	// Clock - 27M
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0 , MT9M131_I2C_CONFIG);		// 
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x0280 , MT9M131_I2C_CONFIG);     // Vertical Output Size A - 480
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x01E0 , MT9M131_I2C_CONFIG);     // Vertical Output Size A - 480
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A9, 0x03C0 , MT9M131_I2C_CONFIG);     // Vertical Output Size A - 480
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0200, MT9M131_I2C_CONFIG);     // 20100721
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0239, 0x068C, MT9M131_I2C_CONFIG);     // AE Line size Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x023B, 0x03DA, MT9M131_I2C_CONFIG);     // AE shutter delay limit Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0257, 0x0218, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (60Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0258, 0x0284, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (50Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);     // 60Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);     // 50Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0264, 0x5E1C, MT9M131_I2C_CONFIG);     // Flicker parameter
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x0000, MT9M131_I2C_CONFIG);     //DEFAULT_CONFIG set to Context B
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);     //PROGRAM_SELECT DEFAULT PROGRAM
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);     //PROGRAM_ADVANCE, ADVANCE_1

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);     //Reserved
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     //Mode Control
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02DD, 0x18E0, MT9M131_I2C_CONFIG);     //BR Threshold
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0224, 0x5F20, MT9M131_I2C_CONFIG);     //MATRIX_ADJ_LIMITS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0228, 0xEF02, MT9M131_I2C_CONFIG);     //AWB_ADVANCED_CONTROL_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025F, 0x4143, MT9M131_I2C_CONFIG);     //RATIO_DELTA_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0202, 0x00EE, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0203, 0x291A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K1_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0204, 0x02A4, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K6_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0209, 0x0068, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020A, 0x002A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020B, 0x0004, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020C, 0x0093, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020D, 0x0082, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020E, 0x0040, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020F, 0x005F, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0210, 0x004E, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0211, 0x005B, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0215, 0x00C9, MT9M131_I2C_CONFIG);     //DELTA_COEFS_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0216, 0x005E, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0217, 0x009D, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0218, 0x0006, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0219, 0x0089, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021A, 0x0012, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021B, 0x00A1, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021C, 0x00E4, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021D, 0x007A, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021E, 0x0064, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02F6, 0x005F, MT9M131_I2C_CONFIG);     // FLASH_WB_POS

		// 20100721 - added for context B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);

		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x608E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);	// context B
	} 
	else if (mode == MT9M131_MODE_VGA_15FPS) 
	{
		printk("MT9M131_MODE_VGA_15FPS in mt9m131.c\n");
		/* [NAVICAM 640*480 ver0.9 15fps] from yypark */
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x03C0, MT9M131_I2C_CONFIG);     // Row Width
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0008, 0x0030, MT9M131_I2C_CONFIG);     // Context A (preview) Vertical Blank

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x3618 , MT9M131_I2C_CONFIG);	// Clock - 27M
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0 , MT9M131_I2C_CONFIG);		// 

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0021, 0x8400, MT9M131_I2C_CONFIG);     // Read Mode Context A
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0022, 0x0D0F, MT9M131_I2C_CONFIG);     // Dark col / rows
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0024, 0x8000, MT9M131_I2C_CONFIG);     // Extra Reset
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0059, 0x00FF, MT9M131_I2C_CONFIG);     // Black Rows
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01AA, 0x1E0 , MT9M131_I2C_CONFIG);     // Vertical Output Size A - 480
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0100, MT9M131_I2C_CONFIG);     // Shutter Width Lim AE
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0239, 0x068C, MT9M131_I2C_CONFIG);     // AE Line size Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x023B, 0x03DA, MT9M131_I2C_CONFIG);     // AE shutter delay limit Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0257, 0x0218, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (60Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0258, 0x0284, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (50Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);     // 60Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);     // 50Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0264, 0x5E1C, MT9M131_I2C_CONFIG);     // Flicker parameter
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x0000, MT9M131_I2C_CONFIG);     //DEFAULT_CONFIG set to Context B
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);     //PROGRAM_SELECT DEFAULT PROGRAM
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);     //PROGRAM_ADVANCE, ADVANCE_1

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);     //Reserved
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     //Mode Control
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02DD, 0x18E0, MT9M131_I2C_CONFIG);     //BR Threshold
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0224, 0x5F20, MT9M131_I2C_CONFIG);     //MATRIX_ADJ_LIMITS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0228, 0xEF02, MT9M131_I2C_CONFIG);     //AWB_ADVANCED_CONTROL_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025F, 0x4143, MT9M131_I2C_CONFIG);     //RATIO_DELTA_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0202, 0x00EE, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0203, 0x291A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K1_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0204, 0x02A4, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K6_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0209, 0x0068, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020A, 0x002A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020B, 0x0004, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020C, 0x0093, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020D, 0x0082, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020E, 0x0040, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020F, 0x005F, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0210, 0x004E, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0211, 0x005B, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0215, 0x00C9, MT9M131_I2C_CONFIG);     //DELTA_COEFS_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0216, 0x005E, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0217, 0x009D, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0218, 0x0006, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0219, 0x0089, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021A, 0x0012, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021B, 0x00A1, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021C, 0x00E4, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021D, 0x007A, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021E, 0x0064, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02F6, 0x005F, MT9M131_I2C_CONFIG);     // FLASH_WB_POS

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     // Mode Control

	}
	else if (mode == MT9M131_MODE_SXVGA_960) 
	{
		printk("MT9M131_MODE_SXVGA_960 in mt9m131.c\n");
		/* [NAVICAM 1280*960 ver0.9] from yypark */
		//[Default Registers-NAVICAM 1280X960 for54MHz, Fixed 15FPS ]                 

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x3C0 , MT9M131_I2C_CONFIG);	// V size	- 960

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02F, 0x0042, MT9M131_I2C_CONFIG);     // GLOBAL_GAIN_REG

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x009, 0x01FC, MT9M131_I2C_CONFIG);     // INTEG_TIME_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02B, 0x0042, MT9M131_I2C_CONFIG);     // GREEN1_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02C, 0x0060, MT9M131_I2C_CONFIG);     // BLUE_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D, 0x0046, MT9M131_I2C_CONFIG);     // RED_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02E, 0x0042, MT9M131_I2C_CONFIG);     // GREEN2_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x061, 0x0026, MT9M131_I2C_CONFIG);     // CAL_G1
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x062, 0x002E, MT9M131_I2C_CONFIG);     // CAL_B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x063, 0x0028, MT9M131_I2C_CONFIG);     // CAL_R
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x064, 0x0025, MT9M131_I2C_CONFIG);     // CAL_G2
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0C8, 0x000B, MT9M131_I2C_CONFIG);     // SENSOR_CONTEXT_CONTROL
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2C8, 0x1F0B, MT9M131_I2C_CONFIG);     // CONTEXT_CONTROL
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2CC, 0x0004, MT9M131_I2C_CONFIG);     // PROGRAM_SELECT
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2D2, 0x007F, MT9M131_I2C_CONFIG);     // DEFAULT_CONFIG

		// frame rate will be down if set above 0x100...default value is 0x300
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0100, MT9M131_I2C_CONFIG); // gain zone limit

		#if 0	// video clip mode
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D1, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0003, MT9M131_I2C_CONFIG);
		//#else	//preview mode
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);
		#endif
	}	
	else if (mode == MT9M131_MODE_SXVGA_15FPS) 
	{
		printk("MT9M131_MODE_SXVGA_15FPS in mt9m131.c\n");
		/* [NAVICAM 1280*960 ver0.9] from yypark */
		//[Default Registers-NAVICAM 1280X1024 for54MHz, Fixed 15FPS ]                 
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02F, 0x0042, MT9M131_I2C_CONFIG);     // GLOBAL_GAIN_REG

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x009, 0x01FC, MT9M131_I2C_CONFIG);     // INTEG_TIME_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02B, 0x0042, MT9M131_I2C_CONFIG);     // GREEN1_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02C, 0x0060, MT9M131_I2C_CONFIG);     // BLUE_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D, 0x0046, MT9M131_I2C_CONFIG);     // RED_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02E, 0x0042, MT9M131_I2C_CONFIG);     // GREEN2_GAIN_REG
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x061, 0x0026, MT9M131_I2C_CONFIG);     // CAL_G1
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x062, 0x002E, MT9M131_I2C_CONFIG);     // CAL_B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x063, 0x0028, MT9M131_I2C_CONFIG);     // CAL_R
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x064, 0x0025, MT9M131_I2C_CONFIG);     // CAL_G2
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0C8, 0x000B, MT9M131_I2C_CONFIG);     // SENSOR_CONTEXT_CONTROL - context B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2C8, 0x1F0B, MT9M131_I2C_CONFIG);     // CONTEXT_CONTROL
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2CC, 0x0004, MT9M131_I2C_CONFIG);     // PROGRAM_SELECT
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x2D2, 0x007F, MT9M131_I2C_CONFIG);     // DEFAULT_CONFIG

	}
	else if (mode == MT9M131_MODE_P1280_720) 
	{
		printk("MT9M131_MODE_P1280_720 in mt9m131.c\n");
#if 1
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0134, 0x0010, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0220, 0xff00, MT9M131_I2C_CONFIG);

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x023A, 0x068C, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x023C, 0x0530, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0259, 0x0218, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025A, 0x0284, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02DD, 0x18E0, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0224, 0x5F20, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0228, 0xEF02, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025F, 0x4143, MT9M131_I2C_CONFIG);
		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0202, 0x00EE, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0203, 0x291A, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0204, 0x02A4, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0209, 0x0068, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020A, 0x002A, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020B, 0x0004, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020C, 0x0093, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020D, 0x0082, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020E, 0x0040, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x020F, 0x005F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0210, 0x004E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0211, 0x005B, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0215, 0x00C9, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0216, 0x005E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0217, 0x009D, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0218, 0x0006, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0219, 0x0089, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x021A, 0x0012, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x021B, 0x00A1, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x021C, 0x00E4, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x021D, 0x007A, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x021E, 0x0064, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02F6, 0x005F, MT9M131_I2C_CONFIG);
#endif
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x2D0 , MT9M131_I2C_CONFIG);	// V size	- 720 - 0x348==> 20fps
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x04, 0x500 , MT9M131_I2C_CONFIG);	// H size - 1280 (default)
		
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x361a , MT9M131_I2C_CONFIG);	// Clock - 27M
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x360b , MT9M131_I2C_CONFIG);	// Clock - 27M
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0 , MT9M131_I2C_CONFIG);		// 
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x500 , MT9M131_I2C_CONFIG);	// H size - 1280 (default)
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x2D0 , MT9M131_I2C_CONFIG);	// V size	- 720

		#if 0	// video clip mode
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D1, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0003, MT9M131_I2C_CONFIG);
		#else	//preview mode
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);
		#endif
		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x608E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);	// context B

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x00e0, MT9M131_I2C_CONFIG);

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0080, MT9M131_I2C_CONFIG); // gain zone limit
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0125, 0x005D, MT9M131_I2C_CONFIG); // gain zone limit

	}	
	else if (mode == MT9M131_MODE_D1_NTSC_15FPS || MT9M131_MODE_D1_NTSC_30FPS) 
	{
		printk("MT9M131_MODE_D1_NTSC_15FPS in mt9m131.c\n");
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x006C, MT9M131_I2C_CONFIG);     // 20100721
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0002, 0x0072, MT9M131_I2C_CONFIG);     // 20100721
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x0348, MT9M131_I2C_CONFIG);     // 20100721
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0022, 0x0D0F, MT9M131_I2C_CONFIG);     // Dark col / rows
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0024, 0x8000, MT9M131_I2C_CONFIG);     // Extra Reset
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0059, 0x00FF, MT9M131_I2C_CONFIG);     // Black Rows
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x2D0 , MT9M131_I2C_CONFIG);     
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A9, 0x348 , MT9M131_I2C_CONFIG);     
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x1E0 , MT9M131_I2C_CONFIG);    

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0080, MT9M131_I2C_CONFIG);     // Shutter Width Lim AE
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0239, 0x068C, MT9M131_I2C_CONFIG);     // AE Line size Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x023B, 0x03DA, MT9M131_I2C_CONFIG);     // AE shutter delay limit Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0257, 0x0218, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (60Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0258, 0x0284, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (50Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);     // 60Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);     // 50Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0264, 0x5E1C, MT9M131_I2C_CONFIG);     // Flicker parameter
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);     //Reserved
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     //Mode Control
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02DD, 0x18E0, MT9M131_I2C_CONFIG);     //BR Threshold
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0224, 0x5F20, MT9M131_I2C_CONFIG);     //MATRIX_ADJ_LIMITS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0228, 0xEF02, MT9M131_I2C_CONFIG);     //AWB_ADVANCED_CONTROL_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025F, 0x4143, MT9M131_I2C_CONFIG);     //RATIO_DELTA_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0202, 0x00EE, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0203, 0x291A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K1_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0204, 0x02A4, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K6_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0209, 0x0068, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020A, 0x002A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020B, 0x0004, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020C, 0x0093, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020D, 0x0082, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020E, 0x0040, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020F, 0x005F, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0210, 0x004E, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0211, 0x005B, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0215, 0x00C9, MT9M131_I2C_CONFIG);     //DELTA_COEFS_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0216, 0x005E, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0217, 0x009D, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0218, 0x0006, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0219, 0x0089, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021A, 0x0012, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021B, 0x00A1, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021C, 0x00E4, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021D, 0x007A, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021E, 0x0064, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02F6, 0x005F, MT9M131_I2C_CONFIG);     // FLASH_WB_POS

		// 20100721 - added for context B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);

		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x608E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);	// context B

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x00e0, MT9M131_I2C_CONFIG);


		// 2010-09-30 added...
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0125, 0x005D, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0150, 0x0008, MT9M131_I2C_CONFIG);

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01DC, 0x0C04, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01DD, 0x4723, MT9M131_I2C_CONFIG);

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01DE, 0x9A76, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01DF, 0xCDB6, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01E0, 0xF0E0, MT9M131_I2C_CONFIG);
	}	
	/* Select configuration parameters as per video mode  */
	else if (mode == MT9M131_MODE_QVGA_30FPS) 
	{
		printk("MT9M131_MODE_QVGA_15FPS in mt9m131.c\n");
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x0048, MT9M131_I2C_CONFIG);     // 20100721
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0002, 0x005E, MT9M131_I2C_CONFIG);     // 20100721

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0020, 0x0108, MT9M131_I2C_CONFIG);     // Read Mode Context B

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0022, 0x0D0F, MT9M131_I2C_CONFIG);     // Dark col / rows
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0024, 0x8000, MT9M131_I2C_CONFIG);     // Extra Reset
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0059, 0x00FF, MT9M131_I2C_CONFIG);     // Black Rows

		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x0140, MT9M131_I2C_CONFIG);     // Horizontal Output Size B - 320
		err = i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x00F0, MT9M131_I2C_CONFIG);     // Vertical Output Size B - 240

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0200, MT9M131_I2C_CONFIG);     // 20100721
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0239, 0x068C, MT9M131_I2C_CONFIG);     // AE Line size Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x023B, 0x03DA, MT9M131_I2C_CONFIG);     // AE shutter delay limit Context A
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0257, 0x0218, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (60Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0258, 0x0284, MT9M131_I2C_CONFIG);     // Context A Flicker full frame time (50Hz)
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);     // 60Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);     // 50Hz Flicker Search Range
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0264, 0x5E1C, MT9M131_I2C_CONFIG);     // Flicker parameter
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x0000, MT9M131_I2C_CONFIG);     //DEFAULT_CONFIG set to Context B
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);     //PROGRAM_SELECT DEFAULT PROGRAM
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);     //PROGRAM_ADVANCE, ADVANCE_1

		err = i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);     //Reserved
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     //Mode Control
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02DD, 0x18E0, MT9M131_I2C_CONFIG);     //BR Threshold
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0224, 0x5F20, MT9M131_I2C_CONFIG);     //MATRIX_ADJ_LIMITS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0228, 0xEF02, MT9M131_I2C_CONFIG);     //AWB_ADVANCED_CONTROL_REG
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x025F, 0x4143, MT9M131_I2C_CONFIG);     //RATIO_DELTA_REG
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0202, 0x00EE, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0203, 0x291A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K1_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0204, 0x02A4, MT9M131_I2C_CONFIG);     //BASE_MATRIX_SCALE_K6_K9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0209, 0x0068, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020A, 0x002A, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020B, 0x0004, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020C, 0x0093, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020D, 0x0082, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020E, 0x0040, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x020F, 0x005F, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0210, 0x004E, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0211, 0x005B, MT9M131_I2C_CONFIG);     //BASE_MATRIX_COEF_K9
		
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0215, 0x00C9, MT9M131_I2C_CONFIG);     //DELTA_COEFS_SIGNS
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0216, 0x005E, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D1
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0217, 0x009D, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D2
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0218, 0x0006, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D3
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0219, 0x0089, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D4
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021A, 0x0012, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D5
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021B, 0x00A1, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D6
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021C, 0x00E4, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D7
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021D, 0x007A, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D8
		err = i2c_write_reg(&mt9m131_i2c_client, 0x021E, 0x0064, MT9M131_I2C_CONFIG);     //DELTA_MATRIX_COEF_D9
		err = i2c_write_reg(&mt9m131_i2c_client, 0x02F6, 0x005F, MT9M131_I2C_CONFIG);     // FLASH_WB_POS

		// 20100721 - added for context B
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);

		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x608E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);	// context B

	} 
	else 
	{
		printk(KERN_ERR "\n Invalid frame format");
		return -1;
	}
#endif
	mt9m131_Cur_mode = mode;

	
	return err;
}
//#############################################################################
static int mt9m131_set_context(int ctxt_val)
{
	int err = 0;
#if 0
	int valB;
	int valA;
	unsigned short tmp;
	
	printk("ctxt_val : %d\n",ctxt_val);
	
	err = i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x00, MT9M131_I2C_CONFIG);	
	err |= i2c_read_reg(&mt9m131_i2c_client, (0x0FF & MT9M131_CONTEXT_CONTROL), &tmp, MT9M131_I2C_CONFIG);
	
	valB = MT9M131_CTXT_CTRL_RESTART | MT9M131_CTXT_CTRL_DEFECTCOR_B
		| MT9M131_CTXT_CTRL_RESIZE_B | MT9M131_CTXT_CTRL_CTRL2_B
		| MT9M131_CTXT_CTRL_GAMMA_B | MT9M131_CTXT_CTRL_READ_MODE_B
		| MT9M131_CTXT_CTRL_VBLANK_SEL_B
		| MT9M131_CTXT_CTRL_HBLANK_SEL_B;
	valA = MT9M131_CTXT_CTRL_RESTART;

	if (ctxt_val == HIGHPOWER)
	{
		printk("HIGHPOWER[0] Mode\n");
		err |= i2c_write_reg(&mt9m131_i2c_client, MT9M131_CONTEXT_CONTROL, valB, MT9M131_I2C_CONFIG);
	}	
	else
	{
		printk("LOW POWER[1] Mode\n");
		err |=i2c_write_reg(&mt9m131_i2c_client, MT9M131_CONTEXT_CONTROL, valA, MT9M131_I2C_CONFIG);
	}	
#endif	
	return err;
}

//#############################################################################

/*
 * ======== queryctrl ========
 */
 /* This function will return parameter values for control command passed */
static int mt9m131_queryctrl(void *arg)
{
	struct v4l2_queryctrl *queryctrl = arg;
	int ret = 0;
	int id = queryctrl->id;
	dev_dbg(mt9m131_i2c_dev, "\nStarting mt9m131_queryctrl...");
	if (queryctrl == NULL) {
		dev_err(mt9m131_i2c_dev,
			"\n Invalid argument in query control");
		return -1;
	}

	memset(queryctrl, 0, sizeof(*queryctrl));
	queryctrl->id = id;
	switch (id) {
	case V4L2_CID_GAIN:
		strcpy(queryctrl->name, "GAIN");
		queryctrl->type = V4L2_CTRL_TYPE_INTEGER;
		queryctrl->minimum = MT9M131_GAIN_MINVAL;
		queryctrl->maximum = MT9M131_GAIN_MAXVAL;
		queryctrl->step = MT9M131_GAIN_STEP;
		queryctrl->default_value = MT9M131_GAIN_DEFAULTVAL;
		break;
	default:
		if (id < V4L2_CID_LASTP1)
			queryctrl->flags = V4L2_CTRL_FLAG_DISABLED;
		else
			ret = -EINVAL;
		break;
	}			/* end switch (id) */
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_queryctrl...");
	return ret;
}

/*
 * ======== mt9m131_setgain  ========
 */

/*	This function will set the global gain of MT9M131*/
static int mt9m131_setgain(int arg)
{

	int err = 0;
#if 0
	unsigned short gain = (unsigned short)arg;
	dev_dbg(mt9m131_i2c_dev,
		"\nStarting mt9m131_setgain with gain = %d...", (int)gain);
	err =
	    i2c_write_reg(&mt9m131_i2c_client, MT9M131_GLOBAL_GAIN, gain,
			  MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_setgain...");
#endif
	return err;
}

static int mt9m131_setExposure(int arg)
{

	int err = 0;
#if 0
	unsigned short shutter_width = (unsigned short)arg;
	dev_dbg(mt9m131_i2c_dev,
		"\nStarting mt9m131_setExposure with shutter = %d...", (int)shutter_width);
	err =
	    i2c_write_reg(&mt9m131_i2c_client, MT9M131_SHUTTER_WIDTH, shutter_width,
			  MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_setExposure...");
#endif
	return err;
}

static int mt9m131_setBin(int arg)
{

	int err = 0;
#if 0
	unsigned short bin_mode = (unsigned short)arg;
	dev_dbg(mt9m131_i2c_dev,
		"\nStarting mt9m131_setBin with bin = %d...", (int)bin_mode);
	err =
	    i2c_write_reg(&mt9m131_i2c_client, MT9M131_READ_MODE2, bin_mode,
			  MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_setBin...");
#endif
	return err;
}

static int mt9m131_setBinning_Skip(int arg)
{

	int err = 0;
#if 0
	unsigned short binning_mode = (unsigned short)arg;
	dev_dbg(mt9m131_i2c_dev,
		"\nStarting mt9m131_setBin with bin = %d...", (int)bin_mode);
	
	if( binning_mode == 1 )
	{
		if( mt9m131_Cur_mode == MT9M131_MODE_VGA_BIN_30FPS )
		{
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE, 0x33, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE, 0x33, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}
		else
		{
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE, 0x11, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE, 0x11, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}

	}else{

		if( mt9m131_Cur_mode == MT9M131_MODE_VGA_BIN_30FPS )
		{
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE, 0x03, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE, 0x03, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}

		}else{
		
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE, 0x01, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
			err =i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE, 0x01, MT9M131_I2C_CONFIG);
			if (err < 0) {
				return err;
			}
		}
	}
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
		return err;
	}
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_setBinning_skip...");
#endif
	return err;
}



static int mt9m131_set50_60Hz(int arg)
{

	int err = 0;
#if 0
	unsigned short Is_50_60Hz_mode = (unsigned short)arg;
	dev_dbg(mt9m131_i2c_dev,
		"\nStarting mt9m131_setBin with bin = %d...", (int)bin_mode);
	if( mt9m131_Cur_mode == MT9M131_MODE_VGA_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2575,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 776,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2775+10,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 1010-10,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
	
		}
	}
	else if( mt9m131_Cur_mode ==  MT9M131_MODE_720p_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2583,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 786,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2767+31,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 1026-31,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			
	
		}


	}
	else if( mt9m131_Cur_mode == MT9M131_MODE_VGA_BIN_30FPS )
	{
		if( Is_50_60Hz_mode == 0 )
		{
			/* 60 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2575,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 1470,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
		}else{
			/* 50 Hz */
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH, 2775,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			err =
			i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK, 1830,
					MT9M131_I2C_CONFIG);
			if (err < 0) {
				dev_err(mt9m131_i2c_dev, "\n I2C write fails...");
				return err;
			}
			
	
		}

	}

	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_setBin...");
#endif
	return err;
}


/*
 * ======== mt9m131_getgain  ========
 */

/*	This function will get the global gain of MT9M131*/
static int mt9m131_getgain(int *arg)
{
	int err = 0;
#if 0
	unsigned short gain;
	dev_dbg(mt9m131_i2c_dev, "\nStarting mt9m131_getgain...");
	if (arg == NULL) {
		dev_err(mt9m131_i2c_dev,
			"\nInvalid argument pointer in get gain function");
		return -1;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_GLOBAL_GAIN,
			   &gain, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C reads fails...");
		return err;
	}
	*arg = gain;
	dev_dbg(mt9m131_i2c_dev, "\nEnd of mt9m131_getgain...");
#endif
	return err;
}

/*
 * ======== mt9m131_getparams  ========
 */
/*#define MT9M131_CHIP_VERSION			(0x00)
#define MT9M131_ROW_START			(0x01)
#define MT9M131_COL_START			(0x02)
#define MT9M131_HEIGHT				(0x03)
#define MT9M131_WIDTH				(0x04)
*/
/*This function will get MT9M131 configuration values.*/

static int mt9m131_getparams(void *arg)
{

	int err = 0;
#if 0
	struct mt9m131_params *params = (struct mt9m131_params *)arg;
	unsigned short val;
	printk("mt9m131_getparams by xact\n");	
	dev_dbg(mt9m131_i2c_dev, "\nStarting mt9m131_getparams");

	/* get video format related parameters */
	err = mt9m131_getformat(&params->format);
	printk("test 1\n");
	if (err < 0) {
		return err;
	}
	/*      Read green1 gain */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_GREEN1_GAIN,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}

#if 0	
	printk("test 2\n");
		err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_CHIP_VERSION,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) 
	{
		printk("error MT9M131_CHIP_VERSION\n");
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	else
		printk("MT9M131_CHIP_VERSION : %d\n",val);	
	
#endif



	params->rgb_gain.green1_analog_gain = val & MT9M131_ANALOG_GAIN_MASK;
	params->rgb_gain.green1_digital_gain =
	    (val & MT9M131_DIGITAL_GAIN_MASK) >> MT9M131_DIGITAL_GAIN_SHIFT;

	/*      Read blue gain */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_BLUE_GAIN,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.blue_analog_gain = val & MT9M131_ANALOG_GAIN_MASK;
	params->rgb_gain.blue_digital_gain =
	    (val & MT9M131_DIGITAL_GAIN_MASK) >> MT9M131_DIGITAL_GAIN_SHIFT;

	/*      Read red gain */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_RED_GAIN,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.red_analog_gain = val & MT9M131_ANALOG_GAIN_MASK;
	params->rgb_gain.red_digital_gain =
	    (val & MT9M131_DIGITAL_GAIN_MASK) >> MT9M131_DIGITAL_GAIN_SHIFT;

	/*      Read green2 gain */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_GREEN2_GAIN,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->rgb_gain.green2_analog_gain = val & MT9M131_ANALOG_GAIN_MASK;
	params->rgb_gain.green2_digital_gain =
	    (val & MT9M131_DIGITAL_GAIN_MASK) >> MT9M131_DIGITAL_GAIN_SHIFT;

	/*      Read green1 offset */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_GREEN1_OFFSET,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.green1_offset = val & MT9M131_GREEN1_OFFSET_MASK;

	/*      Read green2 offset */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_GREEN2_OFFSET,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.green2_offset = val & MT9M131_GREEN2_OFFSET_MASK;

	/*      Read red offset */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_RED_OFFSET,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.red_offset = val & MT9M131_RED_OFFSET_MASK;

	/*      Read blue offset */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_BLUE_OFFSET,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.blue_offset = val & MT9M131_BLUE_OFFSET_MASK;

	/*      Read Black level caliberation information */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_BLK_LVL_CALIB,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.manual_override =
	    val & MT9M131_MANUAL_OVERRIDE_MASK;
	params->black_calib.disable_calibration =
	    (val & MT9M131_DISABLE_CALLIBERATION_MASK)
	    >> MT9M131_DISABLE_CALLIBERATION_SHIFT;
	params->black_calib.recalculate_black_level =
	    (val & MT9M131_RECAL_BLACK_LEVEL_MASK)
	    >> MT9M131_RECAL_BLACK_LEVEL_SHIFT;
	params->black_calib.lock_red_blue_calibration =
	    (val & MT9M131_LOCK_RB_CALLIBERATION_MASK)
	    >> MT9M131_LOCK_RB_CALIBRATION_SHIFT;
	params->black_calib.lock_green_calibration =
	    (val & MT9M131_LOCK_GREEN_CALLIBERATION_MASK)
	    >> MT9M131_LOCK_GREEN_CALIBRATION_SHIFT;

	/*      Read Black Level Caliberation Coarse Threshold Value */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_BLC_DELTA_THRESHOLDS,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.low_coarse_thrld = val
	    & MT9M131_LOW_COARSE_THELD_MASK;
	params->black_calib.high_coarse_thrld =
	    (val & MT9M131_HIGH_COARSE_THELD_MASK)
	    >> MT9M131_HIGH_COARSE_THELD_SHIFT;

	/*      Read Black Level Caliberation Target Threshold Value */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_BLC_TARGET_THRESHOLDS,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...");
		return err;
	}
	params->black_calib.low_target_thrld = val
	    & MT9M131_LOW_TARGET_THELD_MASK;
	params->black_calib.high_target_thrld =
	    (val & MT9M131_HIGH_COARSE_THELD_MASK)
	    >> MT9M131_HIGH_COARSE_THELD_SHIFT;

	dev_dbg(mt9m131_i2c_dev, "\nEnd of getparamsMT9M131...");
#endif
	return 0;
}

/*
 * ======== i2c_read_reg  ========
 */

/*This function is used to read value from register for i2c client. */
#ifdef PO1150K

// not use this...
#define PO1150K_BANK_SEL		0x03

#else
#define MT9M112_PAGE_MAP		0x0f0
static int reg_page_map_set(struct i2c_client *client, const u16 reg)
{
	int ret;
	u16 page;
	static int lastpage = -1;	/* PageMap cache value */

	page = (reg >> 8);
	if (page == lastpage)
		return 0;
	if (page > 2)
		return -EINVAL;

	ret = i2c_smbus_write_word_data(client, MT9M112_PAGE_MAP, swab16(page));
	if (!ret)
		lastpage = page;
	return ret;
}

static int i2c_read_reg(struct i2c_client *client, unsigned short reg,
			unsigned short *val, bool configdev)
{
	int ret;

	ret = reg_page_map_set(client, reg);
	if (!ret)
		ret = swab16(i2c_smbus_read_word_data(client, reg & 0xff));

	dev_dbg(&client->dev, "read  reg.%03x -> %04x\n", reg, ret);
	*val = (unsigned short)ret;

	return ret;
}


/*
 * ======== i2c_write_reg  ========
 */
/*This function is used to write value into register for i2c client. */

static int i2c_write_reg(struct i2c_client *client, unsigned short reg,
			 unsigned short val, bool configdev)
{
	int ret;

	ret = reg_page_map_set(client, reg);
	if (!ret)
		ret = i2c_smbus_write_word_data(client, reg & 0xff,
						swab16(val));
	dev_dbg(&client->dev, "write reg.%03x = %04x -> %d\n", reg, data, ret);
	return ret;
}

#endif

/*
 * ======== _i2c_attach_client  ========
 */
/* This function is used to attach i2c client */
static int _i2c_attach_client(struct i2c_client *client,
			      struct i2c_driver *driver,
			      struct i2c_adapter *adap, int addr)
{
	int err = 0;
#ifdef MT9M131_I2C_ENABLE
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
#ifdef MT9M131_I2C_ENABLE
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
 * ======== mt9m131_i2c_probe_adapter  ========
 */

/*This function is used to probe adapter for i2c_client. It returns
    0 if i2c_client is attached to adapter and error code on failure.*/
static int mt9m131_i2c_probe_adapter(struct i2c_adapter *adap)
{
	mt9m131_i2c_dev = &(adap->dev);
	return _i2c_attach_client(&mt9m131_i2c_client, &mt9m131_i2c_driver,
				  adap, MT9M131_I2C_ADDR);
}


/*This function is used to probe adapter for i2c_client. It returns
    0 if i2c_client is attached to adapter and error code on failure.*/
static int mt9m131_i2c_probe_adapter_1(struct i2c_adapter *adap)
{
	mt9m131_i2c_dev = &(adap->dev);
//  	return _i2c_attach_client(&mt9m131_i2c_client, &mt9m131_i2c_driver,
//  				  adap, (0xE8 >> 1));
	//shcho
	return _i2c_attach_client(&mt9m131_i2c_client, &mt9m131_i2c_driver,
				  adap, MT9M131_I2C_ADDR );
}

/*
 * ======== mt9m131_i2c_init  ========
 */

/*	This function is used to initialize the i2c*/
static int mt9m131_i2c_init(void)
{
	int err = 0;
#ifdef MT9M131_I2C_ENABLE
	struct i2c_driver *driver = &mt9m131_i2c_driver;

	driver->owner = THIS_MODULE;
	strlcpy(driver->name, "MT9M131 CMOS sensor I2C driver",
		sizeof(driver->name));
	driver->id = I2C_DRIVERID_EXP0;
	driver->flags = I2C_DF_NOTIFY;
	driver->attach_adapter = mt9m131_i2c_probe_adapter;
	driver->detach_client = _i2c_detach_client;
#endif
	return err;
}

/*
 * ======== mt9m131_i2c_cleanup  ========
 */

void mt9m131_i2c_cleanup(void)
{
#ifdef MT9M131_I2C_ENABLE
	struct i2c_driver *driver = &mt9m131_i2c_driver;

	if (mt9m131_i2c_registration) {
		i2c_detach_client(&mt9m131_i2c_client);
		i2c_del_driver(driver);
		mt9m131_i2c_client.adapter = NULL;
		mt9m131_i2c_registration = MT9M131_I2C_UNREGISTERED;
	}
#endif
}

module_init(mt9m131_i2c_init);
module_exit(mt9m131_i2c_cleanup);

EXPORT_SYMBOL(mt9m131_ctrl);
MODULE_LICENSE("GPL");

/**********************************************************************/
/* End of file                                                        */
/**********************************************************************/
