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
#define MT9M131_I2C_ENABLE 1
#define IS_DM350	(0)

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
static int i2c_read_reg(struct i2c_client *, unsigned short,
			unsigned short *, bool);
static int i2c_write_reg(struct i2c_client *, unsigned short,
			 unsigned short, bool);
static int _i2c_attach_client(struct i2c_client *, struct i2c_driver *,
			      struct i2c_adapter *, int);
static int _i2c_detach_client(struct i2c_client *);
static int mt9m131_i2c_probe_adapter(struct i2c_adapter *);
static int mt9m131_i2c_init(void);
void mt9m131_i2c_cleanup(void);


void mt9m131_readregs(void)
{
#if 0
	unsigned short temp = 0;
	int err = 0;
	unsigned char regcnt;

	err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0,0x01, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		//printk("	i2c_write_reg OK\n");
	}
	else
	{
		printk("	i2c_write_reg Err\n");
	}	
		
	err = i2c_read_reg(&mt9m131_i2c_client, 0xA7, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
		{
			printk("read back 0x%x = 0x%x...\n", 0xA7, temp);
		}
	err = i2c_read_reg(&mt9m131_i2c_client, 0xAA, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
		{
			printk("read back 0x%x = 0x%x...\n", 0xAA, temp);
		}
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0,0x02, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		//printk("	i2c_write_reg OK\n");
	}
	else
	{
		printk("	i2c_write_reg Err\n");
	}		
	
	err = i2c_read_reg(&mt9m131_i2c_client, 0xD2, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
		{
			printk("read back 0x%x = 0x%x...\n", 0xD2, temp);
		}
	
	
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0,0x02, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		//printk("	i2c_write_reg OK\n");
	}
	else
	{
		printk("	i2c_write_reg Err\n");
	}		
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xCB,0x01, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		//printk("	i2c_write_reg OK\n");
	}
	else
	{
		printk("	i2c_write_reg Err\n");
	}		
	
	err = i2c_read_reg(&mt9m131_i2c_client, 0xCB, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
		{
			printk("read back 0x%x = 0x%x...\n", 0xCB, temp);
		}
	
	//#################################################################
	// MT9M131_CHIP_VERSION
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0,0x00, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		//printk("	i2c_write_reg OK\n");
	}
	else
	{
		printk("	i2c_write_reg Err\n");
	}	
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_CHIP_VERSION, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
		{
			printk("read MT9M131_CHIP_VERSION 0x%x = 0x%x...\n", 0x00, temp);
		}
	
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0,0x01, MT9M131_I2C_CONFIG);
	if(err < 0)
		printk("	i2c_write_reg aaaaaa Err\n");
	err = i2c_read_reg(&mt9m131_i2c_client, 0x3A, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		printk("read 0x3A(Output Format Control Contex A[Page1]0x%x = 0x%x...\n", 0x3A, temp);
	}
	else
		printk("	i2c_write_reg aaaaaa Err\n");
		
	err = i2c_read_reg(&mt9m131_i2c_client, 0x9B, &temp, MT9M131_I2C_CONFIG);
	if (err >= 0) 
	{
		printk("read 0x3A(Output Format Control Contex B[Page1]0x%x = 0x%x...\n", 0x9B, temp);
	}
	else
		printk("	i2c_write_reg aaaaaa Err\n");	
		
#endif
		
}
void mt9m131_writeregs(void)
{
	unsigned short val,tmp;
	int err = 0;
	int utemp = 0;

#if 0
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


	err = i2c_read_reg(&mt9m131_i2c_client, 0x00, &tmp, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x0009, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x0029, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x0008, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x07, 0x00D5, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x08, 0x0051, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x0004, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0002, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0x144C, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x24, 0x7F7F, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x37, 0x0100, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0001, MT9M131_I2C_CONFIG);

//err |= i2c_write_reg(&mt9m131_i2c_client, 0x9B, 0x0a00, MT9M131_I2C_CONFIG); //R155:1-0x19B output format control 2-context B 
//Enables embedding Rec ITU-R BT.656				
  
  
//err |= i2c_write_reg(&mt9m131_i2c_client, 0x3A, 0x0a00, MT9M131_I2C_CONFIG); //R155:1-0x19B output format control 2-context A 
//Enables embedding Rec ITU-R BT.656		
  
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x08, 0x0080, MT9M131_I2C_CONFIG); // Output format control 
  
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x06, 0xF08E, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x58, 0x0000, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x53, 0x0903, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x54, 0x351A, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x55, 0x9368, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x56, 0xC4B0, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x57, 0xE0D4, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xE1, 0x0000, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xDC, 0x0903, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xDD, 0x351A, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xDE, 0x9368, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xDF, 0xC4B0, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xE0, 0xE0D4, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0000, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x21, 0x0004, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x2E, 0x144C, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x37, 0x0100, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0001, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x4C, 0x0003, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x4D, 0x0003, MT9M131_I2C_CONFIG);
  
  err = i2c_read_reg(&mt9m131_i2c_client, 0x06, &tmp, MT9M131_I2C_CONFIG);
  printk("tmp = 0x%x\n",tmp);
  tmp = tmp & 0xFBFF;
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x06, &tmp, MT9M131_I2C_CONFIG);
  
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x02, 0x00AE, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x2723, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x04, 0x02A4, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x09, 0x00B4, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x16, 0x0022, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0A, 0x00EA, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x17, 0x0013, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0B, 0x00AF, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x18, 0x0055, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0C, 0x0074, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x19, 0x001D, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x0096, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x1A, 0x0009, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0E, 0x0027, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x1B, 0x005A, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0F, 0x008A, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x1C, 0x0065, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x10, 0x005F, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x1D, 0x0069, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x11, 0x0069, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x1E, 0x003F, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x5E, 0x6A4B, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x5F, 0x3B29, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x60, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x0001, MT9M131_I2C_CONFIG);
  err = i2c_read_reg(&mt9m131_i2c_client, 0x06, &tmp, MT9M131_I2C_CONFIG);
  tmp = tmp | 0x8000;
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x06, tmp, MT9M131_I2C_CONFIG);
  
  err = i2c_read_reg(&mt9m131_i2c_client, 0x48, &tmp, MT9M131_I2C_CONFIG);
  tmp = tmp | 0x0080;
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x48, tmp, MT9M131_I2C_CONFIG);
  
  err = i2c_read_reg(&mt9m131_i2c_client, 0x06, &tmp, MT9M131_I2C_CONFIG);
  tmp = tmp & 0x7FFF;
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x06, tmp, MT9M131_I2C_CONFIG);
  
  err = i2c_read_reg(&mt9m131_i2c_client, 0x48, &tmp, MT9M131_I2C_CONFIG);
  tmp = tmp & 0xFF7F;
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x48, tmp, MT9M131_I2C_CONFIG);
 
  //err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x0001, MT9M131_I2C_CONFIG);
 
  
  
  /*err |= i2c_write_reg(&mt9m131_i2c_client, 0xA7, 640, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xAA, 480, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0x0000, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x0001, MT9M131_I2C_CONFIG);
  */
  
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x0001, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xA1, 640, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xA4, 480, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x0002, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xD2, 0x007f, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0xCB, 0x0001, MT9M131_I2C_CONFIG);

#endif

}

void mt9m131_write_apro(int page, int addr, int val)
{
#if 0	
	int err = 0;
	

	err = i2c_write_reg(&mt9m131_i2c_client, 0xF0, page, MT9M131_I2C_CONFIG);	
	err |= i2c_write_reg(&mt9m131_i2c_client, addr, val, MT9M131_I2C_CONFIG);
	if(err < 0)
		printk("ERROR mt9m131_write_apro\n");
#endif 
}
/*
static int mt9m131_set_autoexposure(int on)
{
	struct i2c_client *client = to_i2c_client(icd->control);
	struct mt9m131 *mt9m131 = container_of(icd, struct mt9m131, icd);
	int ret;

	if (on)
		ret = reg_set(MT9M131_OPER_MODE_CTRL, MT9M131_OPMODE_AUTOEXPO_EN);
	else
		ret = reg_clear(MT9M131_OPER_MODE_CTRL, MT9M131_OPMODE_AUTOEXPO_EN);

	if (!ret)
		mt9m131->autoexposure = on;

	return ret;
}
*/
static int mt9m131_set_autowhitebalance(int on)
{
	int err=0;
#if 0
	unsigned short tmp;
	
	err = i2c_write_reg(&mt9m131_i2c_client, 0xF0, 0x01, MT9M131_I2C_CONFIG);	
	err |= i2c_read_reg(&mt9m131_i2c_client, (0x0FF & MT9M131_OPER_MODE_CTRL), &tmp, MT9M131_I2C_CONFIG);

	printk("auto wb on : %d\n",on);

	if(on == 1)
	{
		printk("Auto White Balance ON\n");
		tmp = tmp | MT9M131_OPMODE_AUTOWHITEBAL_EN;
		err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_OPER_MODE_CTRL, tmp, MT9M131_I2C_CONFIG);	
	}	
	else
	{
		printk("Auto White Balance OFF\n");
		tmp = tmp & !MT9M131_OPMODE_AUTOWHITEBAL_EN;
		err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_OPER_MODE_CTRL, tmp, MT9M131_I2C_CONFIG);	
	}	
	
	if (err < 0)
	{
		printk("ERROR mt9m131_set_autowhitebalance\n");	
	}
#endif
	return err;
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
	
	int err = 0;
#if 0
	unsigned short tmp;
	err = i2c_write_reg(&mt9m131_i2c_client, 0xF0, page, MT9M131_I2C_CONFIG);	
	err |= i2c_read_reg(&mt9m131_i2c_client, addr, &tmp, MT9M131_I2C_CONFIG);
		
	if(err < 0)
		printk("ERROR mt9m131_read_apro\n");
#endif	
	return err; //(int)tmp;	
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
	struct mt9m131_params *temp_params = NULL;
	int utemp = 0;
	int err = 0;
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

	}
	else
	{
		mt9m131_cleanup((*params));
	}
	
// raniyan
#if 0
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x00, MT9M131_I2C_CONFIG);
  err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x09, MT9M131_I2C_CONFIG);
	mdelay(30);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x0D, 0x08, MT9M131_I2C_CONFIG);
	mdelay(30);
#else
#if 0
static struct tvp514x_reg tvp514x_reg_list_default[] = {
        {TOK_WRITE, 0x000D, 0x0009},    /* Chip Enable + Reset Register*/
        {TOK_WRITE, 0x000D, 0x0029},	/* Chip Enable + Reset Register + Reset SOC Reg */
        {TOK_WRITE, 0x000D, 0x0008},	/* Chip Enable */
        {TOK_DELAY, 0, 500},
        {TOK_WRITE, 0x013a, 0x0a00},
        {TOK_WRITE, 0x019b, 0x0a00},
        {TOK_WRITE, 0x0106, 0x708E},    /* MODE CONTROL */
        {TOK_TERM, 0, 0},
};
#endif

	err = i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0009, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0029, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0008, MT9M131_I2C_CONFIG);
	mdelay(500);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x013a, 0x0a00, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x019b, 0x0a00, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);

#if 0
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x00c8, 0x000B, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x02c8, 0x1f0b, MT9M131_I2C_CONFIG);
#endif

#endif	
	return err;
}

static int mt9m131_getformat(struct mt9m131_format_params *mt9tformats)
{
	int err = 0;
#if 0
	unsigned short val = 0;

	/*Read the height width and blanking information required
	   for particular format */
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_HEIGHT,
			   &mt9tformats->row_size, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...height");
		return err;
	}

	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_WIDTH,
			   &mt9tformats->col_size, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...width");
		return err;
	}

	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_HBLANK,
			   &mt9tformats->h_blank, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...hblk");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_VBLANK,
			   &mt9tformats->v_blank, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...vblk");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_SHUTTER_WIDTH,
			   &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...shutterwidth");
		return err;
	}
	mt9tformats->shutter_width = val & MT9M131_SHUTTER_WIDTH_LOWER_MASK;

	err =
	    i2c_read_reg(&mt9m131_i2c_client, MT9M131_SHUTTER_WIDTH_UPPER,
			 &val, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...upper");
		return err;
	}
	mt9tformats->shutter_width |= ((val & MT9M131_SHUTTER_WIDTH_UPPER_MASK)
				       << MT9M131_SHUTTER_WIDTH_UPPER_SHIFT);

	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE,
			   &mt9tformats->row_addr_mode, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...addrmoderow");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE,
			   &mt9tformats->col_addr_mode, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...addrmodecol");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_ROW_BLK_TARGET,
			   &mt9tformats->black_level, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...black_level");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_ROW_START,
			   &mt9tformats->row_start, MT9M131_I2C_CONFIG);
	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...rowstart");
		return err;
	}
	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_COL_START,
			   &mt9tformats->col_start, MT9M131_I2C_CONFIG);

	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...colstart");
		return err;
	}

	err = i2c_read_reg(&mt9m131_i2c_client, MT9M131_PIXEL_CLK_CTRL,
			   &mt9tformats->pixel_clk_control, MT9M131_I2C_CONFIG);

	if (err < 0) {
		dev_err(mt9m131_i2c_dev, "\n I2C read fails...clkctrl");
		return err;
	}
#endif
	return err;
}

static int mt9m131_setformat(struct mt9m131_format_params *mt9tformats)
{
	int err = 0;
#if 0
	unsigned short val;

	/* stop changes  */
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_OUTPUT_CTRL,
			    MT9M131_HALT_MODE, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...outputctrl");
		return err;
	}
	/*Write the height width and blanking information required
	   for particular format */
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_HEIGHT,
			    mt9tformats->row_size, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...height");
		return err;
	}

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_WIDTH,
			    mt9tformats->col_size, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...width");
		return err;
	}
	/* Configure for default video standard */

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_HBLANK,
			    mt9tformats->h_blank, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...hblk");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_VBLANK,
			    mt9tformats->v_blank, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...vblk");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_SHUTTER_WIDTH,
			    (unsigned short)(mt9tformats->
					     shutter_width &
					     MT9M131_SHUTTER_WIDTH_LOWER_MASK),
			    MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...shutterwidth");
		return err;
	}

	err =
	    i2c_write_reg(&mt9m131_i2c_client, MT9M131_SHUTTER_WIDTH_UPPER,
			  (unsigned short)(mt9tformats->
					   shutter_width >>
					   MT9M131_SHUTTER_WIDTH_UPPER_SHIFT),
			  MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...upper");
		return err;
	}

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_ADDR_MODE,
			    mt9tformats->row_addr_mode, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...addrmoderow");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_ADDR_MODE,
			    mt9tformats->col_addr_mode, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...addrmodecol");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_BLK_DEF_OFFSET,
			    mt9tformats->black_level, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...black_level");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_ROW_START,
			    mt9tformats->row_start, MT9M131_I2C_CONFIG);
	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...rowstart");
		return err;
	}
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_COL_START,
			    mt9tformats->col_start, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...colstart");
		return err;
	}

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_PIXEL_CLK_CTRL,
			    mt9tformats->pixel_clk_control, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...clkctrl");
		return err;
	}
#if ENABLE_PLL_SETTING
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_PLL_CONTROL,
			    MT9M131_PLL_USE_EXTERNAL, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll external");
		return err;
	}

	val = (mt9tformats->pll_m << MT9M131_PLL_M_SHIFT) |
		((mt9tformats->pll_n - 1) & MT9M131_PLL_N_MASK);
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_PLL_CONFIG1,
			    val, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_PLL_CONFIG2,
			    (mt9tformats -> pll_p1 - 1) & MT9M131_PLL_P1_MASK ,
			    MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll change");
		return err;
	}

	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_PLL_CONTROL,
			    MT9M131_PLL_ENABLE, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...pll enable");
		return err;
	}
#endif
	mt9m131_output_ctrl = (mt9tformats ->output_config) &
		MT9M131_OUTPUT_CTRL_MASK;
	/* applying changes  */
	err = i2c_write_reg(&mt9m131_i2c_client, MT9M131_OUTPUT_CTRL,
			    mt9m131_output_ctrl, MT9M131_I2C_CONFIG);

	if (err < 0) {
		printk(KERN_ERR "\n I2C write fails...outputctrl");
		return err;
	}
#endif
	return err;
}
/*
 * ======== configPCA9543  ========
 */
/*	This function will configure PCA9543 control register*/
static int mt9m131_configpca9543()
{
	int err = 0;
#if 0
	/* enable path from mt9t001 to*/ 
	err |= i2c_write_reg(&mt9m131_i2c_client, ECP_REGADDR, 
		ECP_REGVAL, ECP_I2C_CONFIG);
	/* Configure PCA9543A, here discard register address */
	err = i2c_write_reg(&mt9m131_i2c_client, 0,
			    PCA9543_REGVAL, PCA9543_I2C_CONFIG);
#endif
	return err;
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

#if 0
	int utemp = 0;


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
#endif

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
	//err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x381a, MT9M131_I2C_CONFIG);
	//err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0, MT9M131_I2C_CONFIG);
		
#if 1
	/* Select configuration parameters as per video mode  */
	if (mode == MT9M131_MODE_VGA_30FPS) 
	{
		printk("MT9M131_MODE_VGA_30FPS in mt9m131.c\n");
		//[Default Registers-NAVICAM Normal Condition 640X480 for 54MHz]
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x03C0, MT9M131_I2C_CONFIG);     // Row Width
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0005, 0x0184, MT9M131_I2C_CONFIG);     // Context B (full-res) Horizontal Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0006, 0x0011, MT9M131_I2C_CONFIG);     // Context B (full-res) Vertical Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0007, 0x00C0, MT9M131_I2C_CONFIG);     // Context A (preview) Horizontal Blank
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0008, 0x0030, MT9M131_I2C_CONFIG);     // Context A (preview) Vertical Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0020, 0x0100, MT9M131_I2C_CONFIG);     // Read Mode Context B

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x361a , MT9M131_I2C_CONFIG);	// Clock - 27M
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0 , MT9M131_I2C_CONFIG);		// 

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
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0XF08E, MT9M131_I2C_CONFIG);     //Mode Control
		//mdelay(500);
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     // Mode Control
	} 
	else if (mode == MT9M131_MODE_VGA_15FPS) 
	{
		printk("MT9M131_MODE_VGA_15FPS in mt9m131.c\n");
		/* [NAVICAM 640*480 ver0.9 15fps] from yypark */
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x03C0, MT9M131_I2C_CONFIG);     // Row Width
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0005, 0x0184, MT9M131_I2C_CONFIG);     // Context B (full-res) Horizontal Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0006, 0x0011, MT9M131_I2C_CONFIG);     // Context B (full-res) Vertical Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0007, 0x00BE, MT9M131_I2C_CONFIG);     // Context A (preview) Horizontal Blank
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0008, 0x0030, MT9M131_I2C_CONFIG);     // Context A (preview) Vertical Blank
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0020, 0x0100, MT9M131_I2C_CONFIG);     // Read Mode Context B

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x361a , MT9M131_I2C_CONFIG);	// Clock - 27M
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
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0XF08E, MT9M131_I2C_CONFIG);     //Mode Control
		//mdelay(500);
		err = i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);     // Mode Control

	}
	else if (mode == MT9M131_MODE_SXVGA_960) 
	{
		printk("MT9M131_MODE_SXVGA_960 in mt9m131.c\n");
		/* [NAVICAM 1280*960 ver0.9] from yypark */
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0009, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0029, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0008, MT9M131_I2C_CONFIG);
		
		//mdelay(500);
		//[Default Registers-NAVICAM 1280X960 for54MHz, Fixed 15FPS ]                 
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x03C0, MT9M131_I2C_CONFIG);
#if 0
#else

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x0026, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0002, 0x005E, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0005, 0x0184, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0006, 0x000D, MT9M131_I2C_CONFIG);

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x3, 0x3C0 , MT9M131_I2C_CONFIG);	// V size	- 960
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x500 , MT9M131_I2C_CONFIG);	// H size - 1280 (default)
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0147, 0x5050, MT9M131_I2C_CONFIG); // noise

		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x3C0 , MT9M131_I2C_CONFIG);	// V size	- 960
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A6, 0x500 , MT9M131_I2C_CONFIG);	// H zoom - 1280(default)
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A9, 0x400 , MT9M131_I2C_CONFIG);	// V zoom - 1024(default)

		// frame rate will be down if set above 0x100...default value is 0x300
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0100, MT9M131_I2C_CONFIG); // gain zone limit

		
		// if context B, it's automatically controlled
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02C8, 0x070b, MT9M131_I2C_CONFIG); // noise

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x500, MT9M131_I2C_CONFIG); // noise
		//mdelay(500);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x8500, MT9M131_I2C_CONFIG); // noise

		#if 1	// video clip mode
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D1, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0003, MT9M131_I2C_CONFIG);
		#else	//preview mode
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);
		#endif


		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.


		

		
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);	// reserved
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0xF08E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);	// context B
#endif

	}	
	else if (mode == MT9M131_MODE_SXVGA_15FPS) 
	{
		printk("MT9M131_MODE_SXVGA_15FPS in mt9m131.c\n");
		/* [NAVICAM 1280*960 ver0.9] from yypark */
		//err = i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0009, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0029, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x000D, 0x0008, MT9M131_I2C_CONFIG);
		
		//mdelay(500);

		//[Default Registers-NAVICAM 1280X1024 for54MHz, Fixed 15FPS ]                 
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x03C0, MT9M131_I2C_CONFIG);
#if 0
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x0026, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0002, 0x005E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0003, 0x0400, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0005, 0x0184, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0006, 0x000D, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0007, 0x00BE, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0008, 0x0042, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0020, 0x0100, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0021, 0x8400, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0022, 0x0D0F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0024, 0x8000, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0059, 0x00FF, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x00C8, 0x000B, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x400 , MT9M131_I2C_CONFIG);	//1024
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A9, 0x400 , MT9M131_I2C_CONFIG);	//1024
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01AA, 0x1E0 , MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0100, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0239, 0x068C, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x023A, 0x068C, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x023B, 0x03DA, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x023C, 0x0530, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0257, 0x0218, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0258, 0x0284, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0259, 0x0218, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025A, 0x0284, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025C, 0x130E, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x025D, 0x1712, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0264, 0x5E1C, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);
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
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0XF08E, MT9M131_I2C_CONFIG);
		//mdelay(500);                                                                  
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x708E, MT9M131_I2C_CONFIG);
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0106, 0x600E, MT9M131_I2C_CONFIG);
#else
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
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x2F0, 0x0002, MT9M131_I2C_CONFIG);     // ADDR_SPACE_SEL_CAMCTRL

#endif
	}
	else if (mode == MT9M131_MODE_P1280_720) 
	{
		printk("MT9M131_MODE_P1280_720 in mt9m131.c\n");
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0001, 0x00e0, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0005, 0x00c0, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0006, 0x000d, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0134, 0x0010, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0220, 0xff00, MT9M131_I2C_CONFIG);

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





		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x03, 0x2D0 , MT9M131_I2C_CONFIG);	// V size	- 720
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x04, 0x500 , MT9M131_I2C_CONFIG);	// H size - 1280 (default)
		
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x66, 0x361a , MT9M131_I2C_CONFIG);	// Clock - 27M
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x65, 0x0 , MT9M131_I2C_CONFIG);		// 
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x20, 0x500 , MT9M131_I2C_CONFIG);		// red...
		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A1, 0x500 , MT9M131_I2C_CONFIG);	// H size - 1280 (default)
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A4, 0x2D0 , MT9M131_I2C_CONFIG);	// V size	- 720
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A6, 0x500 , MT9M131_I2C_CONFIG);	// H zoom - 1280(default)
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x01A9, 0x400 , MT9M131_I2C_CONFIG);	// V zoom - 1024(default)

		// frame rate will be down if set above 0x100...default value is 0x300
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x0237, 0x0100, MT9M131_I2C_CONFIG); // gain zone limit

		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0147, 0x4040, MT9M131_I2C_CONFIG); // noise

		#if 0	// video clip mode
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D1, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0003, MT9M131_I2C_CONFIG);
		#else	//preview mode
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x004F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02D2, 0x007F, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CC, 0x0004, MT9M131_I2C_CONFIG);
		#endif


		
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x02CB, 0x0001, MT9M131_I2C_CONFIG);	// Program Adv.

		

		
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x0035, 0x0022, MT9M131_I2C_CONFIG);	// reserved
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


#if 0
	if(MT9M131_MODE_VGA_30FPS == mt9m131_Cur_mode)
		printk("\nCurrent Mode Changed to MODE_VGA_30FPS on MT9M131\n");
	else
		printk("\nCurrent Mode Changed to %d on MT9M131\n",(int) mt9m131_Cur_mode);
#endif


#if 0
	//err = mt9m131_setformat(&mt9tformats);
	//bt656 enable
	//if(vp_apro->bt656_enable == 1)
	if(1)
	{
		printk("############################################  bt656_enable TRUE...\n");
		err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x01, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x3a, 0xa00, MT9M131_I2C_CONFIG); // Falling Edge
		//err |= i2c_write_reg(&mt9m131_i2c_client, 0x3a, 0x800, MT9M131_I2C_CONFIG); // Rising Edge
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x9b, 0xa00, MT9M131_I2C_CONFIG);
	}
	else
	{
		printk("	!!!!!!!!!!! bt656_enable FALSE...\n");	
		err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x01, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x3a, 0x200, MT9M131_I2C_CONFIG);
		err |= i2c_write_reg(&mt9m131_i2c_client, 0x9b, 0x200, MT9M131_I2C_CONFIG);
	}	
	
	err |= i2c_write_reg(&mt9m131_i2c_client, 0xf0, 0x02, MT9M131_I2C_CONFIG);
	err |= i2c_write_reg(&mt9m131_i2c_client, 0x37, 0x80, MT9M131_I2C_CONFIG); //Slow Shutter를 강제로 해지한다
#endif																																					// 프레임수가 떨어져서 해제시킴
	
	//2009.09.15
	
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

#if 0
static int i2c_read_reg(struct i2c_client *client, unsigned short reg,
			unsigned short *val, bool configdev)
{
	int err = 0;
//#ifndef MT9M131_I2C_ENABLE
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
			dev_err(mt9m131_i2c_dev,
				"\n ERROR in PCA9543A control register read\n");
		}

	} else if (configdev == MT9M131_I2C_CONFIG) {
		msg->addr = client->addr;
		msg->flags = 0;
		msg->len = I2C_ONE_BYTE_TRANSFER;
		msg->buf = data;
#if 0
		data[0] = reg;
#else
		data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >>
		    I2C_TXRX_DATA_SHIFT;
		data[1] = (reg & I2C_TXRX_DATA_MASK);
#endif
		err = i2c_transfer(client->adapter, msg, 1);
		if (err >= 0) {
			msg->flags = I2C_M_RD;
			msg->len = I2C_TWO_BYTE_TRANSFER;	/* 2 byte read */
			err = i2c_transfer(client->adapter, msg, 1);
			if (err >= 0) {
				*val = ((data[0] & I2C_TXRX_DATA_MASK)
					<< I2C_TXRX_DATA_SHIFT)
				    | (data[1] & I2C_TXRX_DATA_MASK);
				    printk("reg:%x, *val : %x\n",reg,*val);
			}
		}
	}
//#endif
	return err;
}
#else
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
#endif

/*
 * ======== i2c_write_reg  ========
 */
/*This function is used to write value into register for i2c client. */
#if 0
static int i2c_write_reg(struct i2c_client *client, unsigned short reg,
			 unsigned short val, bool configdev)
{
	int err = 0;
	int trycnt = 0;
	unsigned short readval = 0;

#ifdef MT9M131_I2C_ENABLE

	struct i2c_msg msg[1];
	unsigned char data[4];
	err = -1;
			printk("i2c_write_reg in MT9M131 \n");
	while ((err < 0) && (trycnt < 5)) 
	{
		trycnt++;
		if (!client->adapter) 
		{
			err = -ENODEV;
		} 
		else if (configdev == ECP_I2C_CONFIG) 
		{
			/* if device is ECP then discard reg value
			 * and set ECP I2C address
			 */
			msg->addr = ECP_I2C_ADDR;
			printk("ECP_I2C_CONFIG [%x]\n",msg->addr);
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
		} 
		else if (configdev == MT9M131_I2C_CONFIG) 
		{
			msg->addr = client->addr;
			printk("MT9M131_I2C_CONFIG [%x]\n",msg->addr);
			msg->flags = 0;
			msg->len = I2C_THREE_BYTE_TRANSFER;
			msg->buf = data;
#if 0
			data[0] = reg;
#else
			data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >>
			    I2C_TXRX_DATA_SHIFT;
			data[1] = (reg & I2C_TXRX_DATA_MASK);
#endif
			data[2] = (val & I2C_TXRX_DATA_MASK_UPPER) >>
			    I2C_TXRX_DATA_SHIFT;
			data[3] = (val & I2C_TXRX_DATA_MASK);

			err = i2c_transfer(client->adapter, msg, 1);

			if (err >= 0) 
			{
				readval = val;
				err = i2c_read_reg(&mt9m131_i2c_client, reg, &readval, MT9M131_I2C_CONFIG);
				//if ((err >= 0) && (val != readval)) 
				if(err < 0) 
				{
					printk("\n ERROR: i2c readback failed\n");
				}
				readval = 0;
			}
		} else if (configdev == PCA9543_I2C_CONFIG) {
			/* if device is PCA9543 then discard reg value
			 * and set PCA9543 I2C address
			 */
			msg->addr = PCA9543_I2C_ADDR;
			printk("PCA9543_I2C_CONFIG [%x]\n", msg->addr);
			msg->flags = 0;
			msg->len = I2C_ONE_BYTE_TRANSFER;
			msg->buf = data;
			data[0] = (unsigned char)(val & I2C_TXRX_DATA_MASK);

			err = i2c_transfer(client->adapter, msg, 1);
			if (err < 0) 
			{
				printk("\n ERROR in PCA9543  register write\n");
			}
		}
	}
#endif
	if (err < 0) {
		printk(KERN_INFO "\n I2C write failed");
	}
	return err;
}
#else
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
