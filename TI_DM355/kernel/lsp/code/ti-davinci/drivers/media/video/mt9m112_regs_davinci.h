/*
 * drivers/media/video/mt9v113_regs.h
 *
 * Copyright (C) 2008 Texas Instruments Inc
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
 *
 * Contributors:
 *     Sivaraj R <sivaraj@ti.com>
 *     Brijesh R Jadav <brijesh.j@ti.com>
 *     Hardik Shah <hardik.shah@ti.com>
 *     Manjunath Hadli <mrh@ti.com>
 *     Karicheri Muralidharan <m-karicheri2@ti.com>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _MT9M112_REGS_H
#define _MT9M112_REGS_H

/* mt9m112: Sensor register addresses */
#define MT9M112_CHIP_VERSION		0x000
#define MT9M112_ROW_START		0x001
#define MT9M112_COLUMN_START		0x002
#define MT9M112_WINDOW_HEIGHT		0x003
#define MT9M112_WINDOW_WIDTH		0x004
#define MT9M112_HORIZONTAL_BLANKING_B	0x005
#define MT9M112_VERTICAL_BLANKING_B	0x006
#define MT9M112_HORIZONTAL_BLANKING_A	0x007
#define MT9M112_VERTICAL_BLANKING_A	0x008
#define MT9M112_SHUTTER_WIDTH		0x009
#define MT9M112_ROW_SPEED		0x00a
#define MT9M112_EXTRA_DELAY		0x00b
#define MT9M112_SHUTTER_DELAY		0x00c
#define MT9M112_RESET			0x00d
#define MT9M112_READ_MODE_B		0x020
#define MT9M112_READ_MODE_A		0x021
#define MT9M112_FLASH_CONTROL		0x023
#define MT9M112_GREEN1_GAIN		0x02b
#define MT9M112_BLUE_GAIN		0x02c
#define MT9M112_RED_GAIN		0x02d
#define MT9M112_GREEN2_GAIN		0x02e
#define MT9M112_GLOBAL_GAIN		0x02f
#define MT9M112_CONTEXT_CONTROL		0x0c8
#define MT9M112_PAGE_MAP		0x0f0
#define MT9M112_BYTE_WISE_ADDR		0x0f1

#define MT9M112_RESET_SYNC_CHANGES	(1 << 15)
#define MT9M112_RESET_RESTART_BAD_FRAME	(1 << 9)
#define MT9M112_RESET_SHOW_BAD_FRAMES	(1 << 8)
#define MT9M112_RESET_RESET_SOC		(1 << 5)
#define MT9M112_RESET_OUTPUT_DISABLE	(1 << 4)
#define MT9M112_RESET_CHIP_ENABLE	(1 << 3)
#define MT9M112_RESET_ANALOG_STANDBY	(1 << 2)
#define MT9M112_RESET_RESTART_FRAME	(1 << 1)
#define MT9M112_RESET_RESET_MODE	(1 << 0)

#define MT9M112_RMB_MIRROR_COLS		(1 << 1)
#define MT9M112_RMB_MIRROR_ROWS		(1 << 0)
#define MT9M112_CTXT_CTRL_RESTART       (1 << 15)
#define MT9M112_CTXT_CTRL_XENON_EN      (1 << 7)
#define MT9M112_CTXT_CTRL_READ_MODE_B   (1 << 3)
#define MT9M112_CTXT_CTRL_LED_FLASH_EN  (1 << 2)
#define MT9M112_CTXT_CTRL_VBLANK_SEL_B  (1 << 1)
#define MT9M112_CTXT_CTRL_HBLANK_SEL_B  (1 << 0)

/*
 * mt9m111: Colorpipe register addresses (0x100..0x1ff)
 */
#define MT9M112_OPER_MODE_CTRL		0x106
#define MT9M112_OUTPUT_FORMAT_CTRL	0x108
#define MT9M112_REDUCER_XZOOM_B		0x1a0
#define MT9M112_REDUCER_XSIZE_B		0x1a1
#define MT9M112_REDUCER_YZOOM_B		0x1a3
#define MT9M112_REDUCER_YSIZE_B		0x1a4
#define MT9M112_REDUCER_XZOOM_A		0x1a6
#define MT9M112_REDUCER_XSIZE_A		0x1a7
#define MT9M112_REDUCER_YZOOM_A		0x1a9
#define MT9M112_REDUCER_YSIZE_A		0x1aa

#define MT9M112_OUTPUT_FORMAT_CTRL2_A	0x13a
#define MT9M112_OUTPUT_FORMAT_CTRL2_B	0x19b

#define MT9M112_OPMODE_AUTOEXPO_EN	(1 << 14)
#define MT9M112_OPMODE_AUTOWHITEBAL_EN	(1 << 1)

#define MT9M112_OUTFMT_PROCESSED_BAYER	(1 << 14)
#define MT9M112_OUTFMT_BYPASS_IFP	(1 << 10)
#define MT9M112_OUTFMT_INV_PIX_CLOCK	(1 << 9)
#define MT9M112_OUTFMT_RGB		(1 << 8)
#define MT9M112_OUTFMT_RGB565		(0x0 << 6)
#define MT9M112_OUTFMT_RGB555		(0x1 << 6)
#define MT9M112_OUTFMT_RGB444x		(0x2 << 6)
#define MT9M112_OUTFMT_RGBx444		(0x3 << 6)
#define MT9M112_OUTFMT_TST_RAMP_OFF	(0x0 << 4)
#define MT9M112_OUTFMT_TST_RAMP_COL	(0x1 << 4)
#define MT9M112_OUTFMT_TST_RAMP_ROW	(0x2 << 4)
#define MT9M112_OUTFMT_TST_RAMP_FRAME	(0x3 << 4)
#define MT9M112_OUTFMT_SHIFT_3_UP	(1 << 3)
#define MT9M112_OUTFMT_AVG_CHROMA	(1 << 2)
#define MT9M112_OUTFMT_SWAP_YCbCr_C_Y	(1 << 1)
#define MT9M112_OUTFMT_SWAP_RGB_EVEN	(1 << 1)
#define MT9M112_OUTFMT_SWAP_YCbCr_Cb_Cr	(1 << 0)
/*
 * mt9m112: Camera control register addresses (0x200..0x2ff not implemented)
 */

#define reg_read(reg) mt9m112_reg_read(client, MT9M112_##reg)
#define reg_write(reg, val) mt9m112_reg_write(client, MT9M112_##reg, (val))
#define reg_set(reg, val) mt9m112_reg_set(client, MT9M112_##reg, (val))
#define reg_clear(reg, val) mt9m112_reg_clear(client, MT9M112_##reg, (val))

#define MT9M112_MIN_DARK_ROWS	8
#define MT9M112_MIN_DARK_COLS	24
#define MT9M112_MAX_HEIGHT	1024
#define MT9M112_MAX_WIDTH	1280

#define MT9M112_IMAGE_STD_VGA			(0x01)
#define MT9M112_IMAGE_STD_QVGA			(0x02)
#define MT9M112_IMAGE_STD_INVALID		(0xFF)

enum mt9m112_context {
	HIGHPOWER = 0,
	LOWPOWER,
};

/* Tokens for register write */
#define TOK_WRITE                       (0)	/* token for write operation */
#define TOK_TERM                        (1)	/* terminating token */
#define TOK_DELAY                       (2)	/* delay token for reg list */
#define TOK_SKIP                        (3)	/* token to skip a register */
/**
 * struct mt9m112_reg - Structure for MT9M112 register initialization values
 * @token - Token: TOK_WRITE, TOK_TERM etc..
 * @reg - Register offset
 * @val - Register Value for TOK_WRITE or delay in ms for TOK_DELAY
 */
struct mt9m112_reg {
	unsigned short token;
	unsigned short reg;
	unsigned short val;
};

/**
 * struct mt9m112_init_seq - Structure for MT9M112 power up
 *		Sequence.
 * @ no_regs - Number of registers to write for power up sequence.
 * @ init_reg_seq - Array of registers and respective value to write.
 */
struct mt9m112_init_seq {
	unsigned int no_regs;
	const struct mt9m112_reg *init_reg_seq;
};

#define MT9M112_CHIP_ID			(0x148c) 
// #define MT9M112_CHIP_ID			(0x143a)

#endif /* ifndef _MT9M112_REGS_H */
