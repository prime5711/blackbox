/*
 * drivers/media/video/mt9m112_davinci.c
 *
 * Based on TI TVP5146/47 decoder driver
 *
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

#include <linux/i2c.h>
#include <linux/delay.h>
//#include <linux/gpio.h>
//#include <linux/videodev2.h>
//#include <media/v4l2-int-device.h>
#include <media/mt9m112_davinci.h>
//#include <mach/hardware.h>
//#include <linux/io.h>

#include <linux/i2c.h>
#include <linux/videodev.h>
#include <linux/device.h>

#include "mt9m112_regs_davinci.h"

/* Module Name */
#define MT9M112_MODULE_NAME		"mt9m112"

/* Private macros for TVP */
#define I2C_RETRY_COUNT                 (5)
#define LOCK_RETRY_COUNT                (5)
#define LOCK_RETRY_DELAY                (200)

/* Debug functions */
static int debug = 1;
module_param(debug, bool, 0644);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

#define dump_reg(client, reg, val)				\
	do {							\
		val = mt9m112_read_reg(client, reg);		\
		v4l_info(client, "Reg(0x%.2X): 0x%.2X\n", reg, val); \
	} while (0)

/**
 * enum mt9m112_std - enum for supported standards
 */
enum mt9m112_std {
	MT9M112_STD_VGA = 0,
	MT9M112_STD_QVGA,
	MT9M112_STD_SXVGA,
	MT9M112_STD_INVALID
};

/**
 * enum mt9m112_state - enum for different decoder states
 */
enum mt9m112_state {
	STATE_NOT_DETECTED,
	STATE_DETECTED
};

/**
 * struct mt9m112_std_info - Structure to store standard informations
 * @width: Line width in pixels
 * @height:Number of active lines
 * @video_std: Value to write in REG_VIDEO_STD register
 * @standard: v4l2 standard structure information
 */
struct mt9m112_std_info {
	unsigned long width;
	unsigned long height;
	u8 video_std;
	struct v4l2_standard standard;
};

/**
 * struct mt9m112_decoder - decoder object
 * @v4l2_int_device: Slave handle
 * @pdata: Board specific
 * @client: I2C client data
 * @id: Entry from I2C table
 * @ver: Chip version
 * @state: decoder state - detected or not-detected
 * @pix: Current pixel format
 * @num_fmts: Number of formats
 * @fmt_list: Format list
 * @current_std: Current standard
 * @num_stds: Number of standards
 * @std_list: Standards list
 * @route: input and output routing at chip level
 */
struct mt9m112_decoder {
	struct v4l2_int_device *v4l2_int_device;
	const struct mt9m112_platform_data *pdata;
	struct i2c_client *client;

	struct i2c_device_id *id;

	int ver;
	enum mt9m112_state state;

	struct v4l2_pix_format pix;
	int num_fmts;
	const struct v4l2_fmtdesc *fmt_list;

	enum mt9m112_std current_std;
	int num_stds;
	struct mt9m112_std_info *std_list;

	struct v4l2_routing route;

        enum mt9m112_context context;
        struct v4l2_rect rect;
        u32 pixfmt;

        unsigned int gain;
        unsigned char autoexposure;
        unsigned char datawidth;
        unsigned int powered:1;
        unsigned int hflip:1;
        unsigned int vflip:1;
        unsigned int swap_rgb_even_odd:1;
        unsigned int swap_rgb_red_blue:1;
        unsigned int swap_yuv_y_chromas:1;
        unsigned int swap_yuv_cb_cr:1;
        unsigned int autowhitebalance:1;
};

#if 1
/* MT9M131 register set for VGA mode */
static struct mt9m112_reg mt9m112_vga_reg[] = {
	{TOK_WRITE, 0x01A7, 0x0280},	/* Reducer horizontal output size - Context A */
	{TOK_WRITE, 0x01AA, 0x0200},	/* Reducer vertical output size - Context A */
	{TOK_WRITE, 0x02D2, 0x0000},	/* Camera control default config */
	{TOK_WRITE, 0x02CB, 0x0001},	/* Camera control program advance */
	{TOK_DELAY, 0, 100},
	{TOK_TERM, 0, 0},
};

static struct mt9m112_reg mt9m112_sxvga_reg[] = {
	{TOK_WRITE, 0x01A7, 0x0500},	/* Reducer horizontal output size - Context A */
	{TOK_WRITE, 0x01AA, 0x0400},	/* Reducer vertical output size - Context A */
	{TOK_WRITE, 0x02D2, 0x007F},	/* Camera control default config */
	{TOK_WRITE, 0x02CB, 0x0001},	/* Camera control program advance */
	{TOK_DELAY, 0, 100},
	{TOK_TERM, 0, 0},
};
#endif

#if 0
static struct mt9m112_reg mt9m112_reg_list[] = {
/* [NAVICAM 640*480 ver0.9 15fps] from yypark */
	{TOK_WRITE, 0x000D, 0x0009},	/* RESET */
	{TOK_WRITE, 0x000D, 0x0029},
	{TOK_WRITE, 0x000D, 0x0008},
	{TOK_DELAY, 0, 500},
        {TOK_WRITE, 0x002F, 0x003E},    //GLOBAL_GAIN_REG
        {TOK_WRITE, 0x0001, 0x0080},    //ROW_WINDOW_START_REG
        {TOK_WRITE, 0x0002, 0x0068},    //COL_WINDOW_START_REG
        {TOK_WRITE, 0x0003, 0x035C},    //ROW_WINDOW_SIZE_REG
        {TOK_WRITE, 0x0004, 0x0500},    //COL_WINDOW_SIZE_REG
        {TOK_WRITE, 0x0005, 0x0184},    //HORZ_BLANK_B
        {TOK_WRITE, 0x0006, 0x000D},    //VERT_BLANK_B
        {TOK_WRITE, 0x0007, 0x00BE},    //HORZ_BLANK_A
        {TOK_WRITE, 0x0008, 0x0042},    //VERT_BLANK_A
        {TOK_WRITE, 0x0009, 0x0218},    //INTEG_TIME_REG
        {TOK_WRITE, 0x000A, 0x8011},    //PIXCLK_SPEED_CTRL_REG
        {TOK_WRITE, 0x000B, 0x0000},    //EXTRA_DELAY_REG
        {TOK_WRITE, 0x000C, 0x0000},    //SHUTTER_DELAY_REG
        {TOK_WRITE, 0x000D, 0x0008},    //RESET_REG
        {TOK_WRITE, 0x001F, 0x0000},    //FRAME_VALID_CONTROL
        {TOK_WRITE, 0x0020, 0x0100},    //READ_MODE_B
        {TOK_WRITE, 0x0021, 0x8400},    //READ_MODE_A
        {TOK_WRITE, 0x0022, 0x0D0F},    //DARK_ROWS_COLS_REG
        {TOK_WRITE, 0x0023, 0x0E08},    //FLASH_CONTROL_REG
        {TOK_WRITE, 0x0024, 0x8000},    //EXTRA_RESET_REG
        {TOK_WRITE, 0x0025, 0x0000},    //LINE_VALID_CONTROL
        {TOK_WRITE, 0x002B, 0x003e},    //GREEN1_GAIN_REG
        {TOK_WRITE, 0x002C, 0x005a},    //BLUE_GAIN_REG
        {TOK_WRITE, 0x002D, 0x0040},    //RED_GAIN_REG
        {TOK_WRITE, 0x002E, 0x003e},    //GREEN2_GAIN_REG
        {TOK_WRITE, 0x0030, 0x042A},    //ROW_NOISE_CONTROL_REG
        {TOK_WRITE, 0x0059, 0x00FF},    //BLACK_ROWS_REG
        {TOK_WRITE, 0x005B, 0x0022},    //DARK_G1_AVG_REG
        {TOK_WRITE, 0x005C, 0x0021},    //DARK_B_AVG_REG
        {TOK_WRITE, 0x005D, 0x0022},    //DARK_R_AVG_REG
        {TOK_WRITE, 0x005E, 0x0020},    //DARK_G2_AVG_REG
        {TOK_WRITE, 0x005F, 0x231D},    //CAL_THRESHOLD
        {TOK_WRITE, 0x0060, 0x0080},    //CAL_CTRL
        {TOK_WRITE, 0x0061, 0x003B},    //CAL_G1
        {TOK_WRITE, 0x0062, 0x0014},    //CAL_B
        {TOK_WRITE, 0x0063, 0x0013},    //CAL_R
        {TOK_WRITE, 0x0064, 0x0039},    //CAL_G2
        {TOK_WRITE, 0x0065, 0xE000},    //CLOCK_ENABLING
        {TOK_WRITE, 0x0066, 0x1E01},    //PLL_REG
        {TOK_WRITE, 0x0067, 0x0501},    //PLL2_REG
        {TOK_WRITE, 0x0068, 0x005D},    //IO_SLEW_RATE_CONTROL
        {TOK_WRITE, 0x00C0, 0x0000},    //GRST_CONTROL
        {TOK_WRITE, 0x00C1, 0x004B},    //START_INTEGRATION
        {TOK_WRITE, 0x00C2, 0x004B},    //START_READOUT
        {TOK_WRITE, 0x00C3, 0x0071},    //ASSERT_STROBE
        {TOK_WRITE, 0x00C4, 0x0096},    //DE_ASSERT_STROBE
        {TOK_WRITE, 0x00C5, 0x004B},    //ASSERT_FLASH
        {TOK_WRITE, 0x00C6, 0x005A},    //DE_ASSERT_FLASH
        {TOK_WRITE, 0x00C8, 0x0000},    //SENSOR_CONTEXT_CONTROL
        {TOK_WRITE, 0x00F0, 0x0002},    //ADDR_SPACE_SEL
        {TOK_WRITE, 0x0105, 0x0007},    //APERTURE_GAIN
        {TOK_WRITE, 0x0106, 0x708E},    //MODE_CONTROL
        {TOK_WRITE, 0x0108, 0x0080},    //FORMAT_CONTROL
        {TOK_WRITE, 0x0125, 0x0055},    //AWB_SPEED_SATURATION
        {TOK_WRITE, 0x0134, 0x0000},    //LUMA_OFFSET
        {TOK_WRITE, 0x0135, 0xFF00},    //CLIPPING_LIM_OUT_LUMA
        {TOK_WRITE, 0x013A, 0x0200},    //FORMAT_OUTPUT_CONTROL2A
        {TOK_WRITE, 0x013B, 0x042A},    //IFP_BLACK_LEVEL_SUBTRACTION
        {TOK_WRITE, 0x013C, 0x0400},    //IFP_BLACK_LEVEL_ADDITION
        {TOK_WRITE, 0x0147, 0x1030},    //THRESH_EDGE_DETECT
        {TOK_WRITE, 0x0148, 0x0000},    //TEST_PATTERN_GEN
        {TOK_WRITE, 0x0153, 0x0804},    //GAMMA_A_Y1_Y2
        {TOK_WRITE, 0x0154, 0x2010},    //GAMMA_A_Y3_Y4
        {TOK_WRITE, 0x0155, 0x6040},    //GAMMA_A_Y5_Y6
        {TOK_WRITE, 0x0156, 0x9f80},    //GAMMA_A_Y7_Y8
        {TOK_WRITE, 0x0157, 0xdfbf},    //GAMMA_A_Y9_Y10
        {TOK_WRITE, 0x0158, 0xFF00},    //GAMMA_A_Y0_Y11
        {TOK_WRITE, 0x0180, 0x0000},    //LENS_CORRECT_CONTROL
        {TOK_WRITE, 0x0181, 0x0000},    //LENS_ADJ_VERT_RED_0
        {TOK_WRITE, 0x0182, 0x0000},    //LENS_ADJ_VERT_RED_1_2
        {TOK_WRITE, 0x0183, 0x0000},    //LENS_ADJ_VERT_RED_3_4
        {TOK_WRITE, 0x0184, 0x0000},    //LENS_ADJ_VERT_GREEN_0
        {TOK_WRITE, 0x0185, 0x0000},    //LENS_ADJ_VERT_GREEN_1_2
        {TOK_WRITE, 0x0186, 0x0000},    //LENS_ADJ_VERT_GREEN_3_4
        {TOK_WRITE, 0x0187, 0x0000},    //LENS_ADJ_VERT_BLUE_0
        {TOK_WRITE, 0x0188, 0x0000},    //LENS_ADJ_VERT_BLUE_1_2
        {TOK_WRITE, 0x0189, 0x0000},    //LENS_ADJ_VERT_BLUE_3_4
        {TOK_WRITE, 0x018A, 0x0000},    //LENS_ADJ_HORIZ_RED_0
        {TOK_WRITE, 0x018B, 0x0000},    //LENS_ADJ_HORIZ_RED_1_2
        {TOK_WRITE, 0x018C, 0x0000},    //LENS_ADJ_HORIZ_RED_3_4
        {TOK_WRITE, 0x018D, 0x0000},    //LENS_ADJ_HORIZ_RED_5
        {TOK_WRITE, 0x018E, 0x0000},    //LENS_ADJ_HORIZ_GREEN_0
        {TOK_WRITE, 0x018F, 0x0000},    //LENS_ADJ_HORIZ_GREEN_1_2
        {TOK_WRITE, 0x0190, 0x0000},    //LENS_ADJ_HORIZ_GREEN_3_4
        {TOK_WRITE, 0x0191, 0x0000},    //LENS_ADJ_HORIZ_GREEN_5
        {TOK_WRITE, 0x0192, 0x0000},    //LENS_ADJ_HORIZ_BLUE_0
        {TOK_WRITE, 0x0193, 0x0000},    //LENS_ADJ_HORIZ_BLUE_1_2
        {TOK_WRITE, 0x0194, 0x0000},    //LENS_ADJ_HORIZ_BLUE_3_4
        {TOK_WRITE, 0x0195, 0x0000},    //LENS_ADJ_HORIZ_BLUE_5
        {TOK_WRITE, 0x0199, 0x0006},    //LINE_COUNTER
        {TOK_WRITE, 0x019A, 0x02f3},    //FRAME_COUNTER
        {TOK_WRITE, 0x019B, 0x0200},    //FORMAT_OUTPUT_CONTROL2B
        {TOK_WRITE, 0x019D, 0x3CAE},    //DEFECT_CORRECTION
        {TOK_WRITE, 0x01A1, 0x0280},    //HORIZ_SIZE_RESIZE_B
        {TOK_WRITE, 0x01A4, 0x01e0},    //VERT_SIZE_RESIZE_B
        {TOK_WRITE, 0x01A5, 0x4000},    //HORIZ_PAN_RESIZE_A
        {TOK_WRITE, 0x01A6, 0x0500},    //HORIZ_ZOOM_RESIZE_A
        {TOK_WRITE, 0x01A7, 0x0280},    //HORIZ_SIZE_RESIZE_A
        {TOK_WRITE, 0x01A8, 0x4000},    //VERT_PAN_RESIZE_A
        {TOK_WRITE, 0x01A9, 0x035C},    //VERT_ZOOM_RESIZE_A
        {TOK_WRITE, 0x01AA, 0x0200},    //VERT_SIZE_RESIZE_A
        {TOK_WRITE, 0x01AC, 0x035C},    //RESERVED_IP_AC
        {TOK_WRITE, 0x01AE, 0x0C09},    //REDUCER_ZOOM_STEP_SIZE
        {TOK_WRITE, 0x01AF, 0x0000},    //REDUCER_ZOOM_CONTROL
        {TOK_WRITE, 0x01B6, 0x0000},    //LENS_ADJ_VERT_RED_5_6
        {TOK_WRITE, 0x01B7, 0x0000},    //LENS_ADJ_VERT_RED_7_8
        {TOK_WRITE, 0x01B8, 0x0000},    //LENS_ADJ_VERT_GREEN_5_6
        {TOK_WRITE, 0x01B9, 0x0000},    //LENS_ADJ_VERT_GREEN_7_8
        {TOK_WRITE, 0x01BA, 0x0000},    //LENS_ADJ_VERT_BLUE_5_6
        {TOK_WRITE, 0x01BB, 0x0000},    //LENS_ADJ_VERT_BLUE_7_8
        {TOK_WRITE, 0x01BC, 0x0000},    //LENS_ADJ_HORIZ_RED_6_7
        {TOK_WRITE, 0x01BD, 0x0000},    //LENS_ADJ_HORIZ_RED_8_9
        {TOK_WRITE, 0x01BE, 0x0000},    //LENS_ADJ_HORIZ_RED_10
        {TOK_WRITE, 0x01BF, 0x0000},    //LENS_ADJ_HORIZ_GREEN_6_7
        {TOK_WRITE, 0x01C0, 0x0000},    //LENS_ADJ_HORIZ_GREEN_8_9
        {TOK_WRITE, 0x01C1, 0x0000},    //LENS_ADJ_HORIZ_GREEN_10
        {TOK_WRITE, 0x01C2, 0x0000},    //LENS_ADJ_HORIZ_BLUE_6_7
        {TOK_WRITE, 0x01C3, 0x0000},    //LENS_ADJ_HORIZ_BLUE_8_9
        {TOK_WRITE, 0x01C4, 0x0000},    //LENS_ADJ_HORIZ_BLUE_10
        {TOK_WRITE, 0x01C8, 0x1F0B},    //CONTEXT_CONTROL_COLPIPE
        {TOK_WRITE, 0x01DC, 0x0E04},    //GAMMA_B_Y1_Y2
        {TOK_WRITE, 0x01DD, 0x4C28},    //GAMMA_B_Y3_Y4
        {TOK_WRITE, 0x01DE, 0x9777},    //GAMMA_B_Y5_Y6
        {TOK_WRITE, 0x01DF, 0xC7B1},    //GAMMA_B_Y7_Y8
        {TOK_WRITE, 0x01E0, 0xEEDB},    //GAMMA_B_Y9_Y10
        {TOK_WRITE, 0x01E1, 0xFF00},    //GAMMA_B_Y0_Y11
        {TOK_WRITE, 0x01E2, 0x7000},    //EFFECTS_MODE
        {TOK_WRITE, 0x01E3, 0xB023},    //EFFECTS_SEPIA
        {TOK_WRITE, 0x0202, 0x00EE},    //BASE_MATRIX_SIGNS
        {TOK_WRITE, 0x0203, 0x291A},    //BASE_MATRIX_SCALE_K1_K5
        {TOK_WRITE, 0x0204, 0x02A4},    //BASE_MATRIX_SCALE_K6_K9
        {TOK_WRITE, 0x0206, 0x708E},    //MODE_CONTROL_CAMCTRL
        {TOK_WRITE, 0x0209, 0x0068},    //BASE_MATRIX_COEF_K1
        {TOK_WRITE, 0x020A, 0x002A},    //BASE_MATRIX_COEF_K2
        {TOK_WRITE, 0x020B, 0x0004},    //BASE_MATRIX_COEF_K3
        {TOK_WRITE, 0x020C, 0x0093},    //BASE_MATRIX_COEF_K4
        {TOK_WRITE, 0x020D, 0x0082},    //BASE_MATRIX_COEF_K5
        {TOK_WRITE, 0x020E, 0x0040},    //BASE_MATRIX_COEF_K6
        {TOK_WRITE, 0x020F, 0x005F},    //BASE_MATRIX_COEF_K7
        {TOK_WRITE, 0x0210, 0x004E},    //BASE_MATRIX_COEF_K8
        {TOK_WRITE, 0x0211, 0x005B},    //BASE_MATRIX_COEF_K9
        {TOK_WRITE, 0x0212, 0x0036},    //AWB_POSITION
        {TOK_WRITE, 0x0213, 0x0090},    //AWB_RED_GAIN
        {TOK_WRITE, 0x0214, 0x007F},    //AWB_BLUE_GAIN
        {TOK_WRITE, 0x0215, 0x00C9},    //DELTA_COEFS_SIGNS
        {TOK_WRITE, 0x0216, 0x005E},    //DELTA_MATRIX_COEF_D1
        {TOK_WRITE, 0x0217, 0x009D},    //DELTA_MATRIX_COEF_D2
        {TOK_WRITE, 0x0218, 0x0006},    //DELTA_MATRIX_COEF_D3
        {TOK_WRITE, 0x0219, 0x0089},    //DELTA_MATRIX_COEF_D4
        {TOK_WRITE, 0x021A, 0x0012},    //DELTA_MATRIX_COEF_D5
        {TOK_WRITE, 0x021B, 0x00A1},    //DELTA_MATRIX_COEF_D6
        {TOK_WRITE, 0x021C, 0x00E4},    //DELTA_MATRIX_COEF_D7
        {TOK_WRITE, 0x021D, 0x007A},    //DELTA_MATRIX_COEF_D8
        {TOK_WRITE, 0x021E, 0x0064},    //DELTA_MATRIX_COEF_D9
        {TOK_WRITE, 0x021F, 0x0180},    //AWB_CR_CB_LIMITS
        {TOK_WRITE, 0x0220, 0xC814},    //LUM_LIMITS_WB_STATS
        {TOK_WRITE, 0x0221, 0x8080},    //R_B_GAIN_MANUAL_WB
        {TOK_WRITE, 0x0222, 0x9080},    //AWB_RED_LIMIT
        {TOK_WRITE, 0x0223, 0x8878},    //AWB_BLUE_LIMIT
        {TOK_WRITE, 0x0224, 0x5F20},    //MATRIX_ADJ_LIMITS
        {TOK_WRITE, 0x0226, 0x8000},    //H_BOUNDS_AE_WIN
        {TOK_WRITE, 0x0227, 0x8008},    //V_BOUNDS_AE_WIN
        {TOK_WRITE, 0x0228, 0xEF02},    //AWB_ADVANCED_CONTROL_REG
        {TOK_WRITE, 0x0229, 0x867A},    //AWB_WIDE_GATES
        {TOK_WRITE, 0x022A, 0x00D0},    //STD_LIM_MONO_ZONE
        {TOK_WRITE, 0x022B, 0x6020},    //H_BOUNDS_AE_WIN_C
        {TOK_WRITE, 0x022C, 0x6020},    //V_BOUNDS_AE_WIN_C
        {TOK_WRITE, 0x022D, 0xF0A0},    //BOUND_AWB_WIN
        {TOK_WRITE, 0x022E, 0x0C45},    //AE_PRECISION_TARGET
        {TOK_WRITE, 0x022F, 0x9120},    //AE_SPEED_CTRL_A
        {TOK_WRITE, 0x0230, 0x00A1},    //RED_AWB_MEASUREMENT
        {TOK_WRITE, 0x0231, 0x00B5},    //LUMA_AWB_MEASUREMENT
        {TOK_WRITE, 0x0232, 0x00B6},    //BLUE_AWB_MEASUREMENT
        {TOK_WRITE, 0x0233, 0x146E},    //SHARPNESS_SAT_CTRL
        {TOK_WRITE, 0x0235, 0xB010},    //DYNAMIC_TARGET_LUMA
        {TOK_WRITE, 0x0236, 0x7810},    //GAIN_LIM_AE
        {TOK_WRITE, 0x0237, 0x0100},    //SHUTTER_WIDTH_LIM_AE
        {TOK_WRITE, 0x0238, 0x0440},    //AE_EXP_TABLE_GAIN_RANGE
        {TOK_WRITE, 0x0239, 0x068C},    //AE_LINE_SIZE_REG_PREVIEW
        {TOK_WRITE, 0x023A, 0x068C},    //AE_LINE_SIZE_REG_FULLRES
        {TOK_WRITE, 0x023B, 0x03DA},    //AE_SHUT_DEL_LIM_A
        {TOK_WRITE, 0x023C, 0x0530},    //AE_SHUT_DEL_LIM_B
        {TOK_WRITE, 0x023D, 0x18DD},    //ADC_LIMITS_AE_ADJ
        {TOK_WRITE, 0x023E, 0x1CFF},    //GAIN_THRESHOLD_REG
        {TOK_WRITE, 0x023F, 0x0004},    //AE_ZONE_INDEX
        {TOK_WRITE, 0x0246, 0x0000},    //AE_LUM_THRESHOLD
        {TOK_WRITE, 0x024B, 0x0004},    //RESERVED_CAMCTRL_4B
        {TOK_WRITE, 0x024C, 0x6000},    //LUM_SUM_MEASUREMENT
        {TOK_WRITE, 0x024D, 0x0060},    //TIME_ADV_SUM_LUMA
        {TOK_WRITE, 0x024F, 0x004A},    //RESERVED_CAMCTRL_4F
        {TOK_WRITE, 0x0257, 0x0218},    //AE_WIDTH_60HZ_PREVIEW
        {TOK_WRITE, 0x0258, 0x0284},    //AE_WIDTH_50HZ_PREVIEW
        {TOK_WRITE, 0x0259, 0x0218},    //AE_WIDTH_60HZ_FULLRES
        {TOK_WRITE, 0x025A, 0x0284},    //AE_WIDTH_50HZ_FULLRES
        {TOK_WRITE, 0x025B, 0x8002},    //FLICKER_CONTROL
        {TOK_WRITE, 0x025C, 0x130E},    //SEARCH_FLICKER_60
        {TOK_WRITE, 0x025D, 0x1712},    //SEARCH_FLICKER_50
        {TOK_WRITE, 0x025E, 0x5848},    //RATIO_BASE_REG
        {TOK_WRITE, 0x025F, 0x4143},    //RATIO_DELTA_REG
        {TOK_WRITE, 0x0260, 0x0002},    //SIGNS_DELTA_REG
        {TOK_WRITE, 0x0261, 0x5D42},    //GAIN_RATIOS
        {TOK_WRITE, 0x0262, 0x1010},    //AE_DIG_GAIN_REG
        {TOK_WRITE, 0x0263, 0x102A},    //RESERVED_CAMCTRL_63
        {TOK_WRITE, 0x0264, 0x5E1C},    //RESERVED_CAMCTRL_64
        {TOK_WRITE, 0x0265, 0x0000},    //AE_LUMA_OFFSET
        {TOK_WRITE, 0x0267, 0x2010},    //DIGITAL_GAIN_LIMITS_AE
        {TOK_WRITE, 0x0281, 0x8308},    //AE_GAIN_ZONE_MAX_LIMIT
        {TOK_WRITE, 0x0282, 0x03FC},    //AE_GAIN_Z1_DELTA
        {TOK_WRITE, 0x0283, 0x0301},    //AE_GAIN_Z2_DELTA
        {TOK_WRITE, 0x0284, 0x00C1},    //AE_GAIN_Z3_DELTA
        {TOK_WRITE, 0x0285, 0x03A1},    //AE_GAIN_Z4_DELTA
        {TOK_WRITE, 0x0286, 0x03D4},    //AE_GAIN_Z5_DELTA
        {TOK_WRITE, 0x0287, 0x03D7},    //AE_GAIN_Z6_DELTA
        {TOK_WRITE, 0x0288, 0x0399},    //AE_GAIN_Z7_DELTA
        {TOK_WRITE, 0x0289, 0x03F8},    //AE_GAIN_Z8_DELTA
        {TOK_WRITE, 0x028A, 0x001C},    //AE_GAIN_Z9_DELTA
        {TOK_WRITE, 0x028B, 0x03BD},    //AE_GAIN_Z10_DELTA
        {TOK_WRITE, 0x028C, 0x03DB},    //AE_GAIN_Z11_DELTA
        {TOK_WRITE, 0x028D, 0x03BD},    //AE_GAIN_Z12_DELTA
        {TOK_WRITE, 0x028E, 0x03FC},    //AE_GAIN_Z13_DELTA
        {TOK_WRITE, 0x028F, 0x03DE},    //AE_GAIN_Z14_DELTA
        {TOK_WRITE, 0x0290, 0x03DE},    //AE_GAIN_Z15_DELTA
        {TOK_WRITE, 0x0291, 0x03DE},    //AE_GAIN_Z16_17_DELTA
        {TOK_WRITE, 0x0292, 0x03DE},    //AE_GAIN_Z18_19_DELTA
        {TOK_WRITE, 0x0293, 0x001F},    //AE_GAIN_Z20_21_DELTA
        {TOK_WRITE, 0x0294, 0x0041},    //AE_GAIN_Z22_23_DELTA
        {TOK_WRITE, 0x0295, 0x0363},    //AE_GAIN_Z24_DELTA
        {TOK_WRITE, 0x0296, 0x0000},    //RESERVED_CAMCTRL_96
        {TOK_WRITE, 0x0297, 0x2100},    //WEIGHTED_AVG_SAT_PIXELS
        {TOK_WRITE, 0x029A, 0x9065},    //CURRENT_TARGET_LUMA
        {TOK_WRITE, 0x029B, 0x00D9},    //LUMA_SAT_THRES
        {TOK_WRITE, 0x029C, 0xD100},    //AE_SPEED_CTRL_B
        {TOK_WRITE, 0x029D, 0xC50A},    //BLACK_ALARM_PXL_THRES
        {TOK_WRITE, 0x02A0, 0x00FE},    //MAX_GAIN_DELTA_1
        {TOK_WRITE, 0x02A1, 0x0064},    //MAX_GAIN_DELTA_2
        {TOK_WRITE, 0x02A2, 0x0042},    //MAX_GAIN_DELTA_3
        {TOK_WRITE, 0x02A3, 0x0021},    //MAX_GAIN_DELTA_4
        {TOK_WRITE, 0x02A4, 0x0021},    //MAX_GAIN_DELTA_5
        {TOK_WRITE, 0x02A5, 0x0000},    //MAX_GAIN_DELTA_6
        {TOK_WRITE, 0x02C8, 0x1F0B},    //CONTEXT_CONTROL
        {TOK_WRITE, 0x02C9, 0x0000},    //CONTEXT_CONTROL_CAMERA
        {TOK_WRITE, 0x02CB, 0x0000},    //PROGRAM_ADVANCE
        {TOK_WRITE, 0x02CC, 0x0004},    //PROGRAM_SELECT
        {TOK_WRITE, 0x02CD, 0x21A0},    //SNAPSHOT_PROGRAM_CONFIG
        {TOK_WRITE, 0x02CE, 0x1E9B},    //LED_FLASH_CONFIG
        {TOK_WRITE, 0x02CF, 0x4A4A},    //LED_LUMA_THRESHOLDS
        {TOK_WRITE, 0x02D0, 0x168D},    //XENON_FLASH_CONFIG
        {TOK_WRITE, 0x02D1, 0x004D},    //VIDEO_CLIP_CONFIG
        {TOK_WRITE, 0x02D2, 0x007F},    //DEFAULT_CONFIG
        {TOK_WRITE, 0x02D3, 0x0000},    //USER_CONTEXT_CONTROL
        {TOK_WRITE, 0x02D4, 0x0209},    //XENON_CONTROL_REG
        {TOK_WRITE, 0x02D5, 0x0000},    //CAMERA_CONTEXT_CONTROL
        {TOK_WRITE, 0x02D6, 0x0000},    //SNAPSHOT_CAPTURE_FRAME
        {TOK_WRITE, 0x02DC, 0x0FF8},    //MG_THRESHOLD
        {TOK_WRITE, 0x02DD, 0x18E0},    //BR_THRESHOLD
        {TOK_WRITE, 0x02DE, 0x0001},    //NEW_TEST_FLAG_ON
        {TOK_WRITE, 0x02EF, 0x0008},    //AWB_FLASH_ADV_CONTROL
        {TOK_WRITE, 0x02F0, 0x0002},    //ADDR_SPACE_SEL_CAMCTRL
        {TOK_WRITE, 0x02F2, 0x0000},    //AWB_R_B_GAIN_OFFSETS
        {TOK_WRITE, 0x02F3, 0x02A3},    //RESERVED_CAMCTRL_F3 
        {TOK_WRITE, 0x02F5, 0x0040},    //MAN_WB_POS
        {TOK_WRITE, 0x02F6, 0x005F},    //FLASH_WB_POS
        {TOK_WRITE, 0x02FF, 0xA880},    //FLASH_GAINS_REG
        {TOK_WRITE, 0x013a, 0x0a00},    /* */
        {TOK_WRITE, 0x019b, 0x0a00},    /* */
        {TOK_WRITE, 0x0106, 0x708E},    // Mode Control
#if 0
        {TOK_WRITE, 0x01A7, 0x0280},    /* */
        {TOK_WRITE, 0x01AA, 0x0200},    /* */
        {TOK_WRITE, 0x02D2, 0x0000},    /* */
        {TOK_WRITE, 0x02CB, 0x0001},    /* */
        {TOK_WRITE, 0x013a, 0x0a00},    /* */
        {TOK_WRITE, 0x019b, 0x0a00},    /* */
        {TOK_WRITE, 0x0237, 0x0080},    /* */

        {TOK_WRITE, 0x0106, 0x708E},    // Mode Control
/* from apro code */
#endif
	{TOK_TERM, 0, 0},
};
#endif
#if 0
static struct mt9m112_reg mt9m112_reg_list[] = {
/* [NAVICAM 1280*960 ver0.9] from yypark */
        {TOK_WRITE, 0x002F, 0x003E},    //GLOBAL_GAIN_REG
        {TOK_WRITE, 0x0001, 0x0080},    //ROW_WINDOW_START_REG
        {TOK_WRITE, 0x0002, 0x0068},    //COL_WINDOW_START_REG
        {TOK_WRITE, 0x0003, 0x035C},    //ROW_WINDOW_SIZE_REG
        {TOK_WRITE, 0x0004, 0x0500},    //COL_WINDOW_SIZE_REG
        {TOK_WRITE, 0x0005, 0x0184},    //HORZ_BLANK_B
        {TOK_WRITE, 0x0006, 0x000D},    //VERT_BLANK_B
        {TOK_WRITE, 0x0007, 0x00BE},    //HORZ_BLANK_A
        {TOK_WRITE, 0x0008, 0x0042},    //VERT_BLANK_A
        {TOK_WRITE, 0x0009, 0x0218},    //INTEG_TIME_REG
        {TOK_WRITE, 0x000A, 0x8011},    //PIXCLK_SPEED_CTRL_REG
        {TOK_WRITE, 0x000B, 0x0000},    //EXTRA_DELAY_REG
        {TOK_WRITE, 0x000C, 0x0000},    //SHUTTER_DELAY_REG
        {TOK_WRITE, 0x000D, 0x0008},    //RESET_REG
        {TOK_WRITE, 0x001F, 0x0000},    //FRAME_VALID_CONTROL
        {TOK_WRITE, 0x0020, 0x0100},    //READ_MODE_B
        {TOK_WRITE, 0x0021, 0x8400},    //READ_MODE_A
        {TOK_WRITE, 0x0022, 0x0D0F},    //DARK_ROWS_COLS_REG
        {TOK_WRITE, 0x0023, 0x0E08},    //FLASH_CONTROL_REG
        {TOK_WRITE, 0x0024, 0x8000},    //EXTRA_RESET_REG
        {TOK_WRITE, 0x0025, 0x0000},    //LINE_VALID_CONTROL
        {TOK_WRITE, 0x002B, 0x002A},    //GREEN1_GAIN_REG
        {TOK_WRITE, 0x002C, 0x003C},    //BLUE_GAIN_REG
        {TOK_WRITE, 0x002D, 0x002C},    //RED_GAIN_REG
        {TOK_WRITE, 0x002E, 0x002A},    //GREEN2_GAIN_REG
        {TOK_WRITE, 0x0030, 0x042A},    //ROW_NOISE_CONTROL_REG
        {TOK_WRITE, 0x0059, 0x00FF},    //BLACK_ROWS_REG
        {TOK_WRITE, 0x005B, 0x0020},    //DARK_G1_AVG_REG
        {TOK_WRITE, 0x005C, 0x001F},    //DARK_B_AVG_REG
        {TOK_WRITE, 0x005D, 0x0022},    //DARK_R_AVG_REG
        {TOK_WRITE, 0x005E, 0x0021},    //DARK_G2_AVG_REG
        {TOK_WRITE, 0x005F, 0x231D},    //CAL_THRESHOLD
        {TOK_WRITE, 0x0060, 0x0080},    //CAL_CTRL
        {TOK_WRITE, 0x0061, 0x0035},    //CAL_G1
        {TOK_WRITE, 0x0062, 0x000D},    //CAL_B
        {TOK_WRITE, 0x0063, 0x000F},    //CAL_R
        {TOK_WRITE, 0x0064, 0x0036},    //CAL_G2
        {TOK_WRITE, 0x0065, 0xE000},    //CLOCK_ENABLING
        {TOK_WRITE, 0x0066, 0x1E01},    //PLL_REG
        {TOK_WRITE, 0x0067, 0x0501},    //PLL2_REG
        {TOK_WRITE, 0x0068, 0x005D},    //IO_SLEW_RATE_CONTROL
        {TOK_WRITE, 0x00C0, 0x0000},    //GRST_CONTROL
        {TOK_WRITE, 0x00C1, 0x004B},    //START_INTEGRATION
        {TOK_WRITE, 0x00C2, 0x004B},    //START_READOUT
        {TOK_WRITE, 0x00C3, 0x0071},    //ASSERT_STROBE
        {TOK_WRITE, 0x00C4, 0x0096},    //DE_ASSERT_STROBE
        {TOK_WRITE, 0x00C5, 0x004B},    //ASSERT_FLASH
        {TOK_WRITE, 0x00C6, 0x005A},    //DE_ASSERT_FLASH
        {TOK_WRITE, 0x00C8, 0x0000},    //SENSOR_CONTEXT_CONTROL
        {TOK_WRITE, 0x00F0, 0x0002},    //ADDR_SPACE_SEL
        {TOK_WRITE, 0x0105, 0x0004},    //APERTURE_GAIN
        {TOK_WRITE, 0x0106, 0x708E},    //MODE_CONTROL
        {TOK_WRITE, 0x0108, 0x0080},    //FORMAT_CONTROL
        {TOK_WRITE, 0x0125, 0x0055},    //AWB_SPEED_SATURATION
        {TOK_WRITE, 0x0134, 0x0000},    //LUMA_OFFSET
        {TOK_WRITE, 0x0135, 0xFF00},    //CLIPPING_LIM_OUT_LUMA
        {TOK_WRITE, 0x013A, 0x0200},    //FORMAT_OUTPUT_CONTROL2A
        {TOK_WRITE, 0x013B, 0x042A},    //IFP_BLACK_LEVEL_SUBTRACTION
        {TOK_WRITE, 0x013C, 0x0400},    //IFP_BLACK_LEVEL_ADDITION
        {TOK_WRITE, 0x0147, 0x1030},    //THRESH_EDGE_DETECT
        {TOK_WRITE, 0x0148, 0x0000},    //TEST_PATTERN_GEN
        {TOK_WRITE, 0x0153, 0x0E04},    //GAMMA_A_Y1_Y2
        {TOK_WRITE, 0x0154, 0x4C28},    //GAMMA_A_Y3_Y4
        {TOK_WRITE, 0x0155, 0x9777},    //GAMMA_A_Y5_Y6
        {TOK_WRITE, 0x0156, 0xC7B1},    //GAMMA_A_Y7_Y8
        {TOK_WRITE, 0x0157, 0xEEDB},    //GAMMA_A_Y9_Y10
        {TOK_WRITE, 0x0158, 0xFF00},    //GAMMA_A_Y0_Y11
        {TOK_WRITE, 0x0180, 0x0000},    //LENS_CORRECT_CONTROL
        {TOK_WRITE, 0x0181, 0x0000},    //LENS_ADJ_VERT_RED_0
        {TOK_WRITE, 0x0182, 0x0000},    //LENS_ADJ_VERT_RED_1_2
        {TOK_WRITE, 0x0183, 0x0000},    //LENS_ADJ_VERT_RED_3_4
        {TOK_WRITE, 0x0184, 0x0000},    //LENS_ADJ_VERT_GREEN_0
        {TOK_WRITE, 0x0185, 0x0000},    //LENS_ADJ_VERT_GREEN_1_2
        {TOK_WRITE, 0x0186, 0x0000},    //LENS_ADJ_VERT_GREEN_3_4
        {TOK_WRITE, 0x0187, 0x0000},    //LENS_ADJ_VERT_BLUE_0
        {TOK_WRITE, 0x0188, 0x0000},    //LENS_ADJ_VERT_BLUE_1_2
        {TOK_WRITE, 0x0189, 0x0000},    //LENS_ADJ_VERT_BLUE_3_4
        {TOK_WRITE, 0x018A, 0x0000},    //LENS_ADJ_HORIZ_RED_0
        {TOK_WRITE, 0x018B, 0x0000},    //LENS_ADJ_HORIZ_RED_1_2
        {TOK_WRITE, 0x018C, 0x0000},    //LENS_ADJ_HORIZ_RED_3_4
        {TOK_WRITE, 0x018D, 0x0000},    //LENS_ADJ_HORIZ_RED_5
        {TOK_WRITE, 0x018E, 0x0000},    //LENS_ADJ_HORIZ_GREEN_0
        {TOK_WRITE, 0x018F, 0x0000},    //LENS_ADJ_HORIZ_GREEN_1_2
        {TOK_WRITE, 0x0190, 0x0000},    //LENS_ADJ_HORIZ_GREEN_3_4
        {TOK_WRITE, 0x0191, 0x0000},    //LENS_ADJ_HORIZ_GREEN_5
        {TOK_WRITE, 0x0192, 0x0000},    //LENS_ADJ_HORIZ_BLUE_0
        {TOK_WRITE, 0x0193, 0x0000},    //LENS_ADJ_HORIZ_BLUE_1_2
        {TOK_WRITE, 0x0194, 0x0000},    //LENS_ADJ_HORIZ_BLUE_3_4
        {TOK_WRITE, 0x0195, 0x0000},    //LENS_ADJ_HORIZ_BLUE_5
        {TOK_WRITE, 0x0199, 0x010A},    //LINE_COUNTER
        {TOK_WRITE, 0x019A, 0x64C0},    //FRAME_COUNTER
        {TOK_WRITE, 0x019B, 0x0200},    //FORMAT_OUTPUT_CONTROL2B
        {TOK_WRITE, 0x019D, 0x3CAE},    //DEFECT_CORRECTION
        {TOK_WRITE, 0x01A1, 0x0500},    //HORIZ_SIZE_RESIZE_B
        {TOK_WRITE, 0x01A4, 0x03C0},    //VERT_SIZE_RESIZE_B
        {TOK_WRITE, 0x01A5, 0x4000},    //HORIZ_PAN_RESIZE_A
        {TOK_WRITE, 0x01A6, 0x0500},    //HORIZ_ZOOM_RESIZE_A
        {TOK_WRITE, 0x01A7, 0x0280},    //HORIZ_SIZE_RESIZE_A
        {TOK_WRITE, 0x01A8, 0x4000},    //VERT_PAN_RESIZE_A
        {TOK_WRITE, 0x01A9, 0x03C0},    //VERT_ZOOM_RESIZE_A
        {TOK_WRITE, 0x01AA, 0x01E0},    //VERT_SIZE_RESIZE_A
        {TOK_WRITE, 0x01AE, 0x0C09},    //REDUCER_ZOOM_STEP_SIZE
        {TOK_WRITE, 0x01AF, 0x0000},    //REDUCER_ZOOM_CONTROL
        {TOK_WRITE, 0x01B6, 0x0000},    //LENS_ADJ_VERT_RED_5_6
        {TOK_WRITE, 0x01B7, 0x0000},    //LENS_ADJ_VERT_RED_7_8
        {TOK_WRITE, 0x01B8, 0x0000},    //LENS_ADJ_VERT_GREEN_5_6
        {TOK_WRITE, 0x01B9, 0x0000},    //LENS_ADJ_VERT_GREEN_7_8
        {TOK_WRITE, 0x01BA, 0x0000},    //LENS_ADJ_VERT_BLUE_5_6
        {TOK_WRITE, 0x01BB, 0x0000},    //LENS_ADJ_VERT_BLUE_7_8
        {TOK_WRITE, 0x01BC, 0x0000},    //LENS_ADJ_HORIZ_RED_6_7
        {TOK_WRITE, 0x01BD, 0x0000},    //LENS_ADJ_HORIZ_RED_8_9
        {TOK_WRITE, 0x01BE, 0x0000},    //LENS_ADJ_HORIZ_RED_10
        {TOK_WRITE, 0x01BF, 0x0000},    //LENS_ADJ_HORIZ_GREEN_6_7
        {TOK_WRITE, 0x01C0, 0x0000},    //LENS_ADJ_HORIZ_GREEN_8_9
        {TOK_WRITE, 0x01C1, 0x0000},    //LENS_ADJ_HORIZ_GREEN_10
        {TOK_WRITE, 0x01C2, 0x0000},    //LENS_ADJ_HORIZ_BLUE_6_7
        {TOK_WRITE, 0x01C3, 0x0000},    //LENS_ADJ_HORIZ_BLUE_8_9
        {TOK_WRITE, 0x01C4, 0x0000},    //LENS_ADJ_HORIZ_BLUE_10
        {TOK_WRITE, 0x01C8, 0x0000},    //CONTEXT_CONTROL_COLPIPE
        {TOK_WRITE, 0x01DC, 0x0E04},    //GAMMA_B_Y1_Y2
        {TOK_WRITE, 0x01DD, 0x4C28},    //GAMMA_B_Y3_Y4
        {TOK_WRITE, 0x01DE, 0x9777},    //GAMMA_B_Y5_Y6
        {TOK_WRITE, 0x01DF, 0xC7B1},    //GAMMA_B_Y7_Y8
        {TOK_WRITE, 0x01E0, 0xEEDB},    //GAMMA_B_Y9_Y10
        {TOK_WRITE, 0x01E1, 0xFF00},    //GAMMA_B_Y0_Y11
        {TOK_WRITE, 0x01E2, 0x7000},    //EFFECTS_MODE
        {TOK_WRITE, 0x01E3, 0xB023},    //EFFECTS_SEPIA
        {TOK_WRITE, 0x0202, 0x00EE},    //BASE_MATRIX_SIGNS
        {TOK_WRITE, 0x0203, 0x291A},    //BASE_MATRIX_SCALE_K1_K5
        {TOK_WRITE, 0x0204, 0x02A4},    //BASE_MATRIX_SCALE_K6_K9
        {TOK_WRITE, 0x0206, 0x708E},    //MODE_CONTROL_CAMCTRL
        {TOK_WRITE, 0x0209, 0x0068},    //BASE_MATRIX_COEF_K1
        {TOK_WRITE, 0x020A, 0x002A},    //BASE_MATRIX_COEF_K2
        {TOK_WRITE, 0x020B, 0x0004},    //BASE_MATRIX_COEF_K3
        {TOK_WRITE, 0x020C, 0x0093},    //BASE_MATRIX_COEF_K4
        {TOK_WRITE, 0x020D, 0x0082},    //BASE_MATRIX_COEF_K5
        {TOK_WRITE, 0x020E, 0x0040},    //BASE_MATRIX_COEF_K6
        {TOK_WRITE, 0x020F, 0x005F},    //BASE_MATRIX_COEF_K7
        {TOK_WRITE, 0x0210, 0x004E},    //BASE_MATRIX_COEF_K8
        {TOK_WRITE, 0x0211, 0x005B},    //BASE_MATRIX_COEF_K9
        {TOK_WRITE, 0x0212, 0x0037},    //AWB_POSITION
        {TOK_WRITE, 0x0213, 0x0090},    //AWB_RED_GAIN
        {TOK_WRITE, 0x0214, 0x0083},    //AWB_BLUE_GAIN
        {TOK_WRITE, 0x0215, 0x00C9},    //DELTA_COEFS_SIGNS
        {TOK_WRITE, 0x0216, 0x005E},    //DELTA_MATRIX_COEF_D1
        {TOK_WRITE, 0x0217, 0x009D},    //DELTA_MATRIX_COEF_D2
        {TOK_WRITE, 0x0218, 0x0006},    //DELTA_MATRIX_COEF_D3
        {TOK_WRITE, 0x0219, 0x0089},    //DELTA_MATRIX_COEF_D4
        {TOK_WRITE, 0x021A, 0x0012},    //DELTA_MATRIX_COEF_D5
        {TOK_WRITE, 0x021B, 0x00A1},    //DELTA_MATRIX_COEF_D6
        {TOK_WRITE, 0x021C, 0x00E4},    //DELTA_MATRIX_COEF_D7
        {TOK_WRITE, 0x021D, 0x007A},    //DELTA_MATRIX_COEF_D8
        {TOK_WRITE, 0x021E, 0x0064},    //DELTA_MATRIX_COEF_D9
        {TOK_WRITE, 0x021F, 0x0180},    //AWB_CR_CB_LIMITS
        {TOK_WRITE, 0x0220, 0xC814},    //LUM_LIMITS_WB_STATS
        {TOK_WRITE, 0x0221, 0x8080},    //R_B_GAIN_MANUAL_WB
        {TOK_WRITE, 0x0222, 0x9080},    //AWB_RED_LIMIT
        {TOK_WRITE, 0x0223, 0x8878},    //AWB_BLUE_LIMIT
        {TOK_WRITE, 0x0224, 0x5F20},    //MATRIX_ADJ_LIMITS
        {TOK_WRITE, 0x0226, 0x8000},    //H_BOUNDS_AE_WIN
        {TOK_WRITE, 0x0227, 0x8008},    //V_BOUNDS_AE_WIN
        {TOK_WRITE, 0x0228, 0xEF02},    //AWB_ADVANCED_CONTROL_REG
        {TOK_WRITE, 0x0229, 0x867A},    //AWB_WIDE_GATES
        {TOK_WRITE, 0x022A, 0x00D0},    //STD_LIM_MONO_ZONE
        {TOK_WRITE, 0x022B, 0x6020},    //H_BOUNDS_AE_WIN_C
        {TOK_WRITE, 0x022C, 0x6020},    //V_BOUNDS_AE_WIN_C
        {TOK_WRITE, 0x022D, 0xF0A0},    //BOUND_AWB_WIN
        {TOK_WRITE, 0x022E, 0x0C44},    //AE_PRECISION_TARGET
        {TOK_WRITE, 0x022F, 0x9120},    //AE_SPEED_CTRL_A
        {TOK_WRITE, 0x0230, 0x00A7},    //RED_AWB_MEASUREMENT
        {TOK_WRITE, 0x0231, 0x00BD},    //LUMA_AWB_MEASUREMENT
        {TOK_WRITE, 0x0232, 0x00B6},    //BLUE_AWB_MEASUREMENT
        {TOK_WRITE, 0x0233, 0x146E},    //SHARPNESS_SAT_CTRL
        {TOK_WRITE, 0x0235, 0xB010},    //DYNAMIC_TARGET_LUMA
        {TOK_WRITE, 0x0236, 0x7810},    //GAIN_LIM_AE
        {TOK_WRITE, 0x0237, 0x0100},    //SHUTTER_WIDTH_LIM_AE
        {TOK_WRITE, 0x0238, 0x0440},    //AE_EXP_TABLE_GAIN_RANGE
        {TOK_WRITE, 0x0239, 0x068C},    //AE_LINE_SIZE_REG_PREVIEW
        {TOK_WRITE, 0x023A, 0x068C},    //AE_LINE_SIZE_REG_FULLRES
        {TOK_WRITE, 0x023B, 0x03DA},    //AE_SHUT_DEL_LIM_A
        {TOK_WRITE, 0x023C, 0x0530},    //AE_SHUT_DEL_LIM_B
        {TOK_WRITE, 0x023D, 0x18DD},    //ADC_LIMITS_AE_ADJ
        {TOK_WRITE, 0x023E, 0x1CFF},    //GAIN_THRESHOLD_REG
        {TOK_WRITE, 0x023F, 0x0004},    //AE_ZONE_INDEX
        {TOK_WRITE, 0x0246, 0x0000},    //AE_LUM_THRESHOLD
        {TOK_WRITE, 0x024B, 0x0004},    //RESERVED_CAMCTRL_4B
        {TOK_WRITE, 0x024C, 0x5600},    //LUM_SUM_MEASUREMENT
        {TOK_WRITE, 0x024D, 0x0056},    //TIME_ADV_SUM_LUMA
        {TOK_WRITE, 0x024F, 0x004C},    //RESERVED_CAMCTRL_4F
        {TOK_WRITE, 0x0257, 0x0218},    //AE_WIDTH_60HZ_PREVIEW
        {TOK_WRITE, 0x0258, 0x0284},    //AE_WIDTH_50HZ_PREVIEW
        {TOK_WRITE, 0x0259, 0x0218},    //AE_WIDTH_60HZ_FULLRES
        {TOK_WRITE, 0x025A, 0x0284},    //AE_WIDTH_50HZ_FULLRES
        {TOK_WRITE, 0x025B, 0x8002},    //FLICKER_CONTROL
        {TOK_WRITE, 0x025C, 0x130E},    //SEARCH_FLICKER_60
        {TOK_WRITE, 0x025D, 0x1712},    //SEARCH_FLICKER_50
        {TOK_WRITE, 0x025E, 0x5848},    //RATIO_BASE_REG
        {TOK_WRITE, 0x025F, 0x4143},    //RATIO_DELTA_REG
        {TOK_WRITE, 0x0260, 0x0002},    //SIGNS_DELTA_REG
        {TOK_WRITE, 0x0261, 0x5C43},    //GAIN_RATIOS
        {TOK_WRITE, 0x0262, 0x1010},    //AE_DIG_GAIN_REG
        {TOK_WRITE, 0x0263, 0x6484},    //RESERVED_CAMCTRL_63
        {TOK_WRITE, 0x0264, 0x5E1C},    //RESERVED_CAMCTRL_64
        {TOK_WRITE, 0x0265, 0x0000},    //AE_LUMA_OFFSET
        {TOK_WRITE, 0x0267, 0x2010},    //DIGITAL_GAIN_LIMITS_AE
        {TOK_WRITE, 0x0281, 0x8308},    //AE_GAIN_ZONE_MAX_LIMIT
        {TOK_WRITE, 0x0282, 0x03FC},    //AE_GAIN_Z1_DELTA
        {TOK_WRITE, 0x0283, 0x0301},    //AE_GAIN_Z2_DELTA
        {TOK_WRITE, 0x0284, 0x00C1},    //AE_GAIN_Z3_DELTA
        {TOK_WRITE, 0x0285, 0x03A1},    //AE_GAIN_Z4_DELTA
        {TOK_WRITE, 0x0286, 0x03D4},    //AE_GAIN_Z5_DELTA
        {TOK_WRITE, 0x0287, 0x03D7},    //AE_GAIN_Z6_DELTA
        {TOK_WRITE, 0x0288, 0x0399},    //AE_GAIN_Z7_DELTA
        {TOK_WRITE, 0x0289, 0x03F8},    //AE_GAIN_Z8_DELTA
        {TOK_WRITE, 0x028A, 0x001C},    //AE_GAIN_Z9_DELTA
        {TOK_WRITE, 0x028B, 0x03BD},    //AE_GAIN_Z10_DELTA
        {TOK_WRITE, 0x028C, 0x03DB},    //AE_GAIN_Z11_DELTA
        {TOK_WRITE, 0x028D, 0x03BD},    //AE_GAIN_Z12_DELTA
        {TOK_WRITE, 0x028E, 0x03FC},    //AE_GAIN_Z13_DELTA
        {TOK_WRITE, 0x028F, 0x03DE},    //AE_GAIN_Z14_DELTA
        {TOK_WRITE, 0x0290, 0x03DE},    //AE_GAIN_Z15_DELTA
        {TOK_WRITE, 0x0291, 0x03DE},    //AE_GAIN_Z16_17_DELTA
        {TOK_WRITE, 0x0292, 0x03DE},    //AE_GAIN_Z18_19_DELTA
        {TOK_WRITE, 0x0293, 0x001F},    //AE_GAIN_Z20_21_DELTA
        {TOK_WRITE, 0x0294, 0x0041},    //AE_GAIN_Z22_23_DELTA
        {TOK_WRITE, 0x0295, 0x0363},    //AE_GAIN_Z24_DELTA
        {TOK_WRITE, 0x0296, 0x0000},    //RESERVED_CAMCTRL_96
        {TOK_WRITE, 0x0297, 0x0C00},    //WEIGHTED_AVG_SAT_PIXELS
        {TOK_WRITE, 0x029A, 0x1058},    //CURRENT_TARGET_LUMA
        {TOK_WRITE, 0x029B, 0x00D9},    //LUMA_SAT_THRES
        {TOK_WRITE, 0x029C, 0xD100},    //AE_SPEED_CTRL_B
        {TOK_WRITE, 0x029D, 0xC50A},    //BLACK_ALARM_PXL_THRES
        {TOK_WRITE, 0x02A0, 0x00FE},    //MAX_GAIN_DELTA_1
        {TOK_WRITE, 0x02A1, 0x0064},    //MAX_GAIN_DELTA_2
        {TOK_WRITE, 0x02A2, 0x0042},    //MAX_GAIN_DELTA_3
        {TOK_WRITE, 0x02A3, 0x0021},    //MAX_GAIN_DELTA_4
        {TOK_WRITE, 0x02A4, 0x0021},    //MAX_GAIN_DELTA_5
        {TOK_WRITE, 0x02A5, 0x0000},    //MAX_GAIN_DELTA_6
        {TOK_WRITE, 0x02C8, 0x0000},    //CONTEXT_CONTROL
        {TOK_WRITE, 0x02C9, 0x0000},    //CONTEXT_CONTROL_CAMERA
        {TOK_WRITE, 0x02CB, 0x0000},    //PROGRAM_ADVANCE
        {TOK_WRITE, 0x02CC, 0x0004},    //PROGRAM_SELECT
        {TOK_WRITE, 0x02CD, 0x21A0},    //SNAPSHOT_PROGRAM_CONFIG
        {TOK_WRITE, 0x02CE, 0x1E9B},    //LED_FLASH_CONFIG
        {TOK_WRITE, 0x02CF, 0x4A4A},    //LED_LUMA_THRESHOLDS
        {TOK_WRITE, 0x02D0, 0x168D},    //XENON_FLASH_CONFIG
        {TOK_WRITE, 0x02D1, 0x004D},    //VIDEO_CLIP_CONFIG
        {TOK_WRITE, 0x02D2, 0x0000},    //DEFAULT_CONFIG
        {TOK_WRITE, 0x02D3, 0x0000},    //USER_CONTEXT_CONTROL
        {TOK_WRITE, 0x02D4, 0x0209},    //XENON_CONTROL_REG
        {TOK_WRITE, 0x02D5, 0x0000},    //CAMERA_CONTEXT_CONTROL
        {TOK_WRITE, 0x02D6, 0x0000},    //SNAPSHOT_CAPTURE_FRAME
        {TOK_WRITE, 0x02DC, 0x0FF8},    //MG_THRESHOLD
        {TOK_WRITE, 0x02DD, 0x18E0},    //BR_THRESHOLD
        {TOK_WRITE, 0x02DE, 0x0001},    //NEW_TEST_FLAG_ON
        {TOK_WRITE, 0x02EF, 0x0008},    //AWB_FLASH_ADV_CONTROL
        {TOK_WRITE, 0x02F0, 0x0002},    //ADDR_SPACE_SEL_CAMCTRL
        {TOK_WRITE, 0x02F2, 0x0000},    //AWB_R_B_GAIN_OFFSETS
        {TOK_WRITE, 0x02F5, 0x0040},    //MAN_WB_POS
        {TOK_WRITE, 0x02F6, 0x005F},    //FLASH_WB_POS
        {TOK_WRITE, 0x02FF, 0xA880},   //FLASH_GAINS_REG
	{TOK_TERM, 0, 0},
};
#endif

#if 0
/* MT9M131 default register values */
static struct mt9m112_reg mt9m112_reg_list[] = {
	{TOK_WRITE, 0x000D, 0x0009},	/* RESET */
	{TOK_WRITE, 0x000D, 0x0029},
	{TOK_WRITE, 0x000D, 0x0008},
	{TOK_DELAY, 0, 500},
	{TOK_WRITE, 0x0007, 0x00D5},	/* HORZ_BLANK_A */
	{TOK_WRITE, 0x0008, 0x0051},	/* VERT_BLANK_A */
	{TOK_WRITE, 0x0021, 0x0004},	/* READ_MODE_A */
	{TOK_DELAY, 0, 100}, 
	{TOK_WRITE, 0x022E, 0x144C},	/* AE PRECISION TARGET */
	{TOK_WRITE, 0x0224, 0x7F7F},	/* MATRIX ADJ LIMITS */
	{TOK_WRITE, 0x0237, 0x0100},	/* */
	{TOK_WRITE, 0x0108, 0x0080},	/* */
	{TOK_WRITE, 0x0106, 0xF08E},	/* */
	{TOK_WRITE, 0x0158, 0x0000},	/* */
#if 1
	{TOK_WRITE, 0x0153, 0x0903},	/* */
	{TOK_WRITE, 0x0154, 0x351a},	/* */
	{TOK_WRITE, 0x0155, 0x9368},	/* */
	{TOK_WRITE, 0x0156, 0xc4b0},	/* */
#endif
	{TOK_WRITE, 0x0157, 0xE0D4},	/* */
	{TOK_WRITE, 0x01E1, 0x0000},	/* */
	{TOK_WRITE, 0x01DC, 0x0903},	/* */
	{TOK_WRITE, 0x01DD, 0x351A},	/* */
	{TOK_WRITE, 0x01DE, 0x9368},	/* */
	{TOK_WRITE, 0x01DF, 0xC4B0},	/* */
	{TOK_WRITE, 0x01E0, 0xE0D4},	/* */
	{TOK_WRITE, 0x0021, 0x0004},	/* */
	{TOK_WRITE, 0x022E, 0x144C},	/* */
	{TOK_WRITE, 0x0237, 0x0100},	/* */
	{TOK_WRITE, 0x014C, 0x0003},	/* */
	{TOK_WRITE, 0x014D, 0x0003},	/* */
/* 0x0106 */
        {TOK_WRITE, 0x0106, 0xF08E},    /* MODE CONTROL */
	{TOK_WRITE, 0x0202, 0x00AE},	/* */
	{TOK_WRITE, 0x0203, 0x2723},	/* */
	{TOK_WRITE, 0x0204, 0x02A4},	/* */
	{TOK_WRITE, 0x0209, 0x00B4},	/* */
	{TOK_WRITE, 0x0216, 0x0022},	/* */
	{TOK_WRITE, 0x020A, 0x00EA},	/* */
	{TOK_WRITE, 0x0217, 0x0013},	/* */
	{TOK_WRITE, 0x020B, 0x00AF},	/* */
	{TOK_WRITE, 0x0218, 0x0055},	/* */
	{TOK_WRITE, 0x020C, 0x0074},	/* */
	{TOK_WRITE, 0x0219, 0x001D},	/* */
	{TOK_WRITE, 0x020D, 0x0096},	/* */
	{TOK_WRITE, 0x021A, 0x0009},	/* */
	{TOK_WRITE, 0x020E, 0x0027},	/* */
	{TOK_WRITE, 0x021B, 0x005A},	/* */
	{TOK_WRITE, 0x020F, 0x008A},	/* */
	{TOK_WRITE, 0x021C, 0x0065},	/* */
	{TOK_WRITE, 0x0210, 0x005F},	/* */
	{TOK_WRITE, 0x021D, 0x0069},	/* */
	{TOK_WRITE, 0x0211, 0x0069},	/* */
	{TOK_WRITE, 0x021E, 0x003F},	/* */
	{TOK_WRITE, 0x025E, 0x6A4B},	/* */
	{TOK_WRITE, 0x025F, 0x3B29},	/* */
	{TOK_WRITE, 0x0260, 0x0002},	/* */
/* 0x0106 */
        {TOK_WRITE, 0x0106, 0xF08E},    /* MODE CONTROL */
/* 0x0148 */
        {TOK_WRITE, 0x0148, 0x0080},    /* */
/* 0x0106 */
        {TOK_WRITE, 0x0106, 0x708E},    /* MODE CONTROL */
/* 0x0148 */
        {TOK_WRITE, 0x0148, 0x0000},    /* */
	{TOK_WRITE, 0x01A1, 0x0280},	/* */
	{TOK_WRITE, 0x01A4, 0x01E0},	/* */
	{TOK_WRITE, 0x02D2, 0x007f},	/* */
	{TOK_WRITE, 0x02CB, 0x0001},	/* */
#if 1
/* from apro code */
//	{TOK_WRITE, 0x01A7, 0x0280},	/* */
//	{TOK_WRITE, 0x01AA, 0x0200},	/* */
	{TOK_WRITE, 0x02D2, 0x0000},	/* */
	{TOK_WRITE, 0x02CB, 0x0001},	/* */
        {TOK_WRITE, 0x013a, 0x0a00},    /* */
        {TOK_WRITE, 0x019b, 0x0a00},    /* */
        {TOK_WRITE, 0x0237, 0x0080},    /* */

#endif

/* from code */
#if 0
	{TOK_WRITE, 0x01e2, 0x7000},	/* effect mode */
	{TOK_WRITE, 0x01A7, 0x0280},	/* Reducer horizontal size - context A */
	{TOK_WRITE, 0x01AA, 0x0200},	/* Reducer vertical size - context A */
	{TOK_WRITE, 0x02D2, 0x0000},	/* Camera control default configuration */
	{TOK_WRITE, 0x02CB, 0x0001},	/* camera control program advance */
	{TOK_WRITE, 0x0232, 0x0a00},	/* AWB blue measurement */
	{TOK_WRITE, 0x029b, 0x0a00},	/* */
	{TOK_WRITE, 0x0237, 0x0080},	/* Auto exposure gain limits */
#endif
	{TOK_TERM, 0, 0},
};
#endif

#if 0
/* MT9M131 default register values */
static struct mt9m112_reg mt9m112_reg_list[] = {
        {TOK_WRITE, 0x000D, 0x0009},    /* RESET */
        {TOK_WRITE, 0x000D, 0x0029},
        {TOK_WRITE, 0x000D, 0x0008},
        {TOK_DELAY, 0, 500},
        {TOK_WRITE, 0x0007, 0x00D5},    /* HORZ_BLANK_A */
        {TOK_WRITE, 0x0008, 0x0051},    /* VERT_BLANK_A */
        {TOK_WRITE, 0x0021, 0x0004},    /* READ_MODE_A */
        {TOK_DELAY, 0, 100}, 
        {TOK_WRITE, 0x0236, 0x7810},    /* GAIN LIM AE */
        {TOK_WRITE, 0x022E, 0x144C},    /* AE PRECISION TARGET */
        {TOK_DELAY, 0, 500},
        {TOK_WRITE, 0x022E, 0x144C},    /* AE PRECISION TARGET */
        {TOK_WRITE, 0x0224, 0x7F7F},    /* MATRIX ADJ LIMITS */
        {TOK_WRITE, 0x0202, 0x00EE},    /* BASE MATRIX SIGNS */
        {TOK_WRITE, 0x0215, 0x00EE},    /* DELTA COEFS SIGNS */
        {TOK_WRITE, 0x0209, 0x00B4},    /* BASE MATRIX COEF K1 */
        {TOK_WRITE, 0x0216, 0x0022},    /* DELTA MATRIX COEF D1 */
        {TOK_WRITE, 0x020A, 0x00EA},    /* BASE MATRIX COEF K2 */
        {TOK_WRITE, 0x0217, 0x0013},    /* DELTA MATRIX COEF D2 */
        {TOK_WRITE, 0x020B, 0x00AF},    /* BASE MATRIX COEF K3 */
        {TOK_WRITE, 0x0218, 0x0055},    /* DELTA MATRIX COEF D3 */
        {TOK_WRITE, 0x020C, 0x0074},    /* BASE MATRIX COEF K4 */
        {TOK_WRITE, 0x0219, 0x001D},    /* DELTA MATRIX COEF D4 */
        {TOK_WRITE, 0x020D, 0x0096},    /* BASE MATRIX COEF K5 */
        {TOK_WRITE, 0x021A, 0x0009},    /* DELTA MATRIX COEF D5 */
        {TOK_WRITE, 0x020E, 0x0027},    /* BASE MATRIX COEF K6 */
        {TOK_WRITE, 0x021B, 0x005A},    /* DELTA MATRIX COEF D6 */
        {TOK_WRITE, 0x020F, 0x008A},    /* BASE MATRIX COEF K7 */
        {TOK_WRITE, 0x021C, 0x0065},    /* DELTA MATRIX COEF D7 */
        {TOK_WRITE, 0x0210, 0x005F},    /* BASE MATRIX COEF K8 */
        {TOK_WRITE, 0x021D, 0x0069},    /* DELTA MATRIX COEF D8 */
        {TOK_WRITE, 0x0211, 0x0069},    /* BASE MATRIX COEF K9 */
        {TOK_WRITE, 0x021E, 0x003F},    /* DELTA MATRIX COEF D9 */
        {TOK_WRITE, 0x0203, 0x2723},    /* BASE MATRIX SCALE K1~K5 */
        {TOK_WRITE, 0x0204, 0x02A4},    /* BASE MATRIX SCALE K6~K9 */
        {TOK_WRITE, 0x025E, 0x6A4B},    /* RATIO_BASE_REG */
        {TOK_WRITE, 0x025F, 0x3B29},    /* RATIO_DELTA_REG */
        {TOK_WRITE, 0x0260, 0x0002},    /* SIGNS_DELTA_REG */
        {TOK_WRITE, 0x0106, 0xF08E},    /* MODE CONTROL */
        {TOK_DELAY, 0, 500},
        {TOK_WRITE, 0x0106, 0x708E},    /* MODE CONTROL */
        {TOK_WRITE, 0x0158, 0x0000},    /* GAMMA A Y0","Gamma 0.55, Contrast 1.60  */
        {TOK_WRITE, 0x0153, 0x0903},    /* GAMMA A Y1 Y2 */
        {TOK_WRITE, 0x0153, 0x0903},    /* GAMMA A Y1 Y2 */
        {TOK_WRITE, 0x0154, 0x351A},    /* GAMMA A Y3 Y4 */
        {TOK_WRITE, 0x0155, 0x9368},    /* GAMMA A Y5 Y6 */
        {TOK_WRITE, 0x0155, 0x9368},    /* GAMMA A Y5 Y6 */
        {TOK_WRITE, 0x0156, 0xC4B0},    /* GAMMA A Y7 Y8 */
        {TOK_WRITE, 0x0156, 0xC4B0},    /* GAMMA A Y7 Y8 */
        {TOK_WRITE, 0x0157, 0xE0D4},    /* GAMMA A Y9 Y10 */
        {TOK_WRITE, 0x0157, 0xE0D4},    /* GAMMA A Y9 Y10 */
        {TOK_WRITE, 0x01E1, 0x0000},    /* GAMMA B Y0 */
        {TOK_WRITE, 0x01DC, 0x0903},    /* GAMMA B Y1 Y2 */
        {TOK_WRITE, 0x01DC, 0x0903},    /* GAMMA B Y1 Y2 */
        {TOK_WRITE, 0x01DD, 0x351A},    /* GAMMA B Y3 Y4 */
        {TOK_WRITE, 0x01DD, 0x351A},    /* GAMMA B Y3 Y4 */
        {TOK_WRITE, 0x01DE, 0x9368},    /* GAMMA B Y5 Y6 */
        {TOK_WRITE, 0x01DE, 0x9368},    /* GAMMA B Y5 Y6 */
        {TOK_WRITE, 0x01DF, 0xC4B0},    /* GAMMA B Y7 Y8 */
        {TOK_WRITE, 0x01DF, 0xC4B0},    /* GAMMA B Y7 Y8 */
        {TOK_WRITE, 0x01E0, 0xE0D4},    /* GAMMA B Y9 Y10 */
        {TOK_WRITE, 0x01E0, 0xE0D4},    /* GAMMA B Y9 Y10 */
        {TOK_WRITE, 0x0021, 0x0004},    /* READ MODE A */
        {TOK_WRITE, 0x022E, 0x144C},    /* AE PRECISION TARGET */
        {TOK_WRITE, 0x0237, 0x0100},    /* SHUTTER WIDTH LIM AE */
        {TOK_WRITE, 0x014C, 0x0003},    /* DEFECT CONTEXT A */
        {TOK_WRITE, 0x014D, 0x0003},    /* DEFECT CONTEXT B */
        {TOK_TERM, 0, 0},
};
#endif

#if 0
/* MT9M112 default register values */
static struct mt9m112_reg mt9m112_reg_list[] = {
	{TOK_WRITE, 0x000D, 0x0009},	/* RESET */
	{TOK_WRITE, 0x000D, 0x0029},
	{TOK_WRITE, 0x000D, 0x0008},
	{TOK_DELAY, 0, 500},
	{TOK_WRITE, 0x0106, 0x709E},	/* MODE_CONTROL */
	{TOK_WRITE, 0x0125, 0x002D},	/* */
	{TOK_WRITE, 0x013B, 0x0436},
	{TOK_WRITE, 0x0153, 0x2214},	//GAMMA_A_Y1_Y2_KHM
	{TOK_WRITE, 0x0154, 0x5E38},	//GAMMA_A_Y3_Y4_KHM
	{TOK_WRITE, 0x0155, 0xC39F},	//GAMMA_A_Y5_Y6_KHM
	{TOK_WRITE, 0x0156, 0xE6D8},	//GAMMA_A_Y7_Y8_KHM
	{TOK_WRITE, 0x0157, 0xF8F0},	//GAMMA_A_Y9_Y10_KHM
	{TOK_WRITE, 0x01A1, 0x0280},	// Horizontal Output Size B 
	{TOK_WRITE, 0x01A4, 0x0200},	// Vertical Output Size B 
	{TOK_WRITE, 0x0180, 0x0010},	//
	{TOK_WRITE, 0x01A6, 0x0280},
	{TOK_WRITE, 0x01A9, 0x0200},
	{TOK_WRITE, 0x01DC, 0x2214},	//GAMMA_B_Y1_Y2_KHM
	{TOK_WRITE, 0x01DD, 0x5E38},	//GAMMA_B_Y3_Y4_KHM
	{TOK_WRITE, 0x01DE, 0xC39F},	//GAMMA_B_Y5_Y6_KHM
	{TOK_WRITE, 0x01DF, 0xE6D8},	//GAMMA_B_Y7_Y8_KHM
	{TOK_WRITE, 0x01E0, 0xF8F0},	//GAMMA_B_Y9_Y10_KHM
	{TOK_WRITE, 0x0203, 0x291A},	//BASE_MATRIX_SCALE_K1_K5
	{TOK_WRITE, 0x0204, 0x02A4},	//BASE_MATRIX_SCALE_K6_K9
	{TOK_WRITE, 0x0209, 0x0068},	//BASE_MATRIX_COEF_K1
	{TOK_WRITE, 0x020A, 0x002A},	//BASE_MATRIX_COEF_K2
	{TOK_WRITE, 0x020C, 0x0093},	//BASE_MATRIX_COEF_K4
	{TOK_WRITE, 0x020E, 0x0040},	//BASE_MATRIX_COEF_K6
	{TOK_WRITE, 0x020F, 0x005F},	//BASE_MATRIX_COEF_K7
	{TOK_WRITE, 0x0210, 0x004E},	//BASE_MATRIX_COEF_K8
	{TOK_WRITE, 0x0211, 0x005B},	//BASE_MATRIX_COEF_K9
	{TOK_WRITE, 0x0216, 0x005E},	//DELTA_MATRIX_COEF_D1
	{TOK_WRITE, 0x0217, 0x009D},	//DELTA_MATRIX_COEF_D2
	{TOK_WRITE, 0x0218, 0x0006},	//DELTA_MATRIX_COEF_D3
	{TOK_WRITE, 0x0219, 0x0089},	//DELTA_MATRIX_COEF_D4
	{TOK_WRITE, 0x021A, 0x0012},	//DELTA_MATRIX_COEF_D5
	{TOK_WRITE, 0x021B, 0x00A1},	//DELTA_MATRIX_COEF_D6
	{TOK_WRITE, 0x021C, 0x00E4},	//DELTA_MATRIX_COEF_D7
	{TOK_WRITE, 0x021D, 0x007A},	//DELTA_MATRIX_COEF_D8
	{TOK_WRITE, 0x021E, 0x0064},	//DELTA_MATRIX_COEF_D9
	{TOK_WRITE, 0x0224, 0x5F20},	//MATRIX_ADJ_LIMITS
	{TOK_WRITE, 0x0228, 0xEF02},	//AWB_ADVANCED_CONTROL_REG
	{TOK_WRITE, 0x022E, 0x094A},	//AE_PRECISION_TARGET
	{TOK_WRITE, 0x0237, 0x0300},	//SHUTTER_WIDTH_LIM_AE
	{TOK_WRITE, 0x025B, 0x8002},	//FLICKER_CONTROL
	{TOK_WRITE, 0x025F, 0x4143},	//
	{TOK_WRITE, 0x02C8, 0x270B},	//CONTEXT_CONTROL
	{TOK_WRITE, 0x02CC, 0x0003},	//PROGRAM_SELECT
	{TOK_WRITE, 0x02D1, 0x00CF},	//DEFAULT_CONFIG
	{TOK_WRITE, 0x02D2, 0x00FF},	//DEFAULT_CONFIG
	{TOK_WRITE, 0x02DD, 0x18E0},	//BR_THRESHOLD
	{TOK_WRITE, 0x02F6, 0x005F},	//FLASH_WB_POS
	{TOK_TERM, 0, 0},
};
#endif

#if 1
/* Default Registers-DTEG Dark Improve 640X480 for 54MHz, 15FPS Fixed */
/* MT9M112 default register values */
/* normally captured */
static struct mt9m112_reg mt9m112_reg_list[] = {
#if 1
	{TOK_WRITE, 0x000D, 0x0009},	/* RESET */
	{TOK_WRITE, 0x000D, 0x0029},
	{TOK_WRITE, 0x000D, 0x0008},
	{TOK_DELAY, 0, 500},
#endif
#if 0
	{TOK_WRITE, 0x0003, 0x03C0},	// Row Width
	{TOK_WRITE, 0x0005, 0x0184},	// Context B (full-res) Horizontal Blank
	{TOK_WRITE, 0x0006, 0x000D},	// Context B (full-res) Vertical Blank
	{TOK_WRITE, 0x0007, 0x00BE},	// Context A (preview) Horizontal Blank
	{TOK_WRITE, 0x0008, 0x0042},	// Context A (preview) Vertical Blank
	{TOK_WRITE, 0x0020, 0x0100},	// Read Mode Context B
	{TOK_WRITE, 0x0021, 0x8400},	// Read Mode Context A
	{TOK_WRITE, 0x0022, 0x0D0F},	// Dark col / rows
	{TOK_WRITE, 0x0024, 0x8000},	// Extra Reset
	{TOK_WRITE, 0x0059, 0x00FF},	// Black Rows
	{TOK_WRITE, 0x00C8, 0x000B},	// Sensor Context Control
	{TOK_WRITE, 0x01A4, 0x03C0},	// Vertical Output Size B
//        {TOK_WRITE, 0x019b, 0x0a00},    /* */
	{TOK_WRITE, 0x01A1, 0x0280},	//HORIZ_SIZE_RESIZE_B
	{TOK_WRITE, 0x01A4, 0x01E0},	//VERT_SIZE_RESIZE_B
	{TOK_WRITE, 0x0237, 0x0100},	// Shutter Width Lim AE
	{TOK_WRITE, 0x0239, 0x068C},	// AE Line size Context A
	{TOK_WRITE, 0x023A, 0x068C},	// AE Line size Context B
	{TOK_WRITE, 0x023B, 0x03DA},	// AE shutter delay limit Context A
	{TOK_WRITE, 0x023C, 0x0530},	// AE shutter delay limit Context B
	{TOK_WRITE, 0x0257, 0x0218},	// Context A Flicker full frame time (60Hz)
	{TOK_WRITE, 0x0258, 0x0284},	// Context A Flicker full frame time (50Hz)
	{TOK_WRITE, 0x0259, 0x0218},	// Context B Flicker full frame time (60Hz)
	{TOK_WRITE, 0x025A, 0x0284},	// Context B Flicker full frame time (50Hz)
	{TOK_WRITE, 0x025C, 0x130E},	// 60Hz Flicker Search Range
	{TOK_WRITE, 0x025D, 0x1712},	// 50Hz Flicker Search Range
	{TOK_WRITE, 0x0264, 0x5E1C},	// Flicker parameter
	{TOK_WRITE, 0x02D2, 0x007F},	//DEFAULT_CONFIG set to Context B
	{TOK_WRITE, 0x02CC, 0x0004},	//PROGRAM_SELECT DEFAULT PROGRAM
	{TOK_WRITE, 0x02CB, 0x0001},	//PROGRAM_ADVANCE, ADVANCE_1
	{TOK_WRITE, 0x0035, 0x0022},	//Reserved
#endif
#if 1
	{TOK_WRITE, 0x0106, 0xF08E},	//Mode Control 0x708E -> 0xF08E
	{TOK_WRITE, 0x02DD, 0x18E0},	//BR Threshold
	{TOK_WRITE, 0x0224, 0x5F20},	//MATRIX_ADJ_LIMITS
	{TOK_WRITE, 0x0228, 0xEF02},	//AWB_ADVANCED_CONTROL_REG
	{TOK_WRITE, 0x025F, 0x4143},	//RATIO_DELTA_REG
	{TOK_WRITE, 0x0202, 0x00EE},	//BASE_MATRIX_SIGNS
	{TOK_WRITE, 0x0203, 0x291A},	//BASE_MATRIX_SCALE_K1_K5
	{TOK_WRITE, 0x0204, 0x02A4},	//BASE_MATRIX_SCALE_K6_K9
	{TOK_WRITE, 0x0209, 0x0068},	//BASE_MATRIX_COEF_K1
	{TOK_WRITE, 0x020A, 0x002A},	//BASE_MATRIX_COEF_K2
	{TOK_WRITE, 0x020B, 0x0004},	//BASE_MATRIX_COEF_K3
	{TOK_WRITE, 0x020C, 0x0093},	//BASE_MATRIX_COEF_K4
	{TOK_WRITE, 0x020D, 0x0082},	//BASE_MATRIX_COEF_K5
	{TOK_WRITE, 0x020E, 0x0040},	//BASE_MATRIX_COEF_K6
	{TOK_WRITE, 0x020F, 0x005F},	//BASE_MATRIX_COEF_K7
	{TOK_WRITE, 0x0210, 0x004E},	//BASE_MATRIX_COEF_K8
	{TOK_WRITE, 0x0211, 0x005B},	//BASE_MATRIX_COEF_K9
	{TOK_WRITE, 0x0215, 0x00C9},	//DELTA_COEFS_SIGNS
	{TOK_WRITE, 0x0216, 0x005E},	//DELTA_MATRIX_COEF_D1
	{TOK_WRITE, 0x0217, 0x009D},	//DELTA_MATRIX_COEF_D2
	{TOK_WRITE, 0x0218, 0x0006},	//DELTA_MATRIX_COEF_D3
	{TOK_WRITE, 0x0219, 0x0089},	//DELTA_MATRIX_COEF_D4
	{TOK_WRITE, 0x021A, 0x0012},	//DELTA_MATRIX_COEF_D5
	{TOK_WRITE, 0x021B, 0x00A1},	//DELTA_MATRIX_COEF_D6
	{TOK_WRITE, 0x021C, 0x00E4},	//DELTA_MATRIX_COEF_D7
	{TOK_WRITE, 0x021D, 0x007A},	//DELTA_MATRIX_COEF_D8
	{TOK_WRITE, 0x021E, 0x0064},	//DELTA_MATRIX_COEF_D9
	{TOK_WRITE, 0x02F6, 0x005F},	// FLASH_WB_POS
	{TOK_WRITE, 0x0106, 0XF08E},	//Mode Control
	{TOK_DELAY, 0, 500},
	{TOK_WRITE, 0x0106, 0x708E},	// Mode Control
#endif
#if 0
/* from apro code */
        {TOK_WRITE, 0x01A7, 0x0280},    /* */
        {TOK_WRITE, 0x01AA, 0x0200},    /* */
        {TOK_WRITE, 0x02D2, 0x0000},    /* */
        {TOK_WRITE, 0x02CB, 0x0001},    /* */
        {TOK_WRITE, 0x013a, 0x0a00},    /* */
        {TOK_WRITE, 0x019b, 0x0a00},    /* */
        {TOK_WRITE, 0x0237, 0x0080},    /* */

#endif

	{TOK_TERM, 0, 0},
};
#endif

/* List of image formats supported by mt9m112 
 * Currently we are using 8 bit mode only, but can be
 * extended to 10/20 bit mode.
 */
static const struct v4l2_fmtdesc mt9m112_fmt_list[] = {
	{
	 .index = 0,
	 .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
	 .flags = 0,
	 .description = "8-bit UYVY 4:2:2 Format",
	 .pixelformat = V4L2_PIX_FMT_UYVY,
	 },
};

/*
 * Supported standards -
 *
 * Currently supports two standards only, need to add support for rest of the
 * modes, like SECAM, etc...
 */
static struct mt9m112_std_info mt9m112_std_list[] = {
	/* Standard: */
	[MT9M112_STD_VGA] = {
			     .width = VGA_NUM_ACTIVE_PIXELS,
			     .height = VGA_NUM_ACTIVE_LINES,
			     .video_std = MT9M112_IMAGE_STD_VGA,
			     .standard = {
					  .index = 0,
					  .id = V4L2_STD_VGA,
					  .name = "VGA",
					  .frameperiod = {1001, 30000},
					  .framelines = 480},
			     /* Standard: STD_PAL_BDGHIN */
			     },
	[MT9M112_STD_QVGA] = {
			      .width = QVGA_NUM_ACTIVE_PIXELS,
			      .height = QVGA_NUM_ACTIVE_LINES,
			      .video_std = MT9M112_IMAGE_STD_QVGA,
			      .standard = {
					   .index = 1,
					   .id = MT9M112_IMAGE_STD_QVGA,
					   .name = "QVGA",
					   .frameperiod = {1001, 30000},
					   .framelines = 320},
			      },
	/* Standard: need to add for additional standard */
};

/*
 * Control structure for Auto Gain
 *     This is temporary data, will get replaced once
 *     v4l2_ctrl_query_fill supports it.
 */
static const struct v4l2_queryctrl mt9m112_autogain_ctrl = {
	.id = V4L2_CID_AUTOGAIN,
	.name = "Gain, Automatic",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.minimum = 0,
	.maximum = 1,
	.step = 1,
	.default_value = 1,
};

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
static int mt9m112_read_reg(struct i2c_client *client, unsigned short reg)
{
	int err = 0;
	int ret;

	if (!client->adapter) {
		err = -ENODEV;
		return err;
	} else {
		ret = reg_page_map_set(client, reg);
		if (!ret)
			ret =
			    swab16(i2c_smbus_read_word_data
				   (client, (reg & 0xff)));

		printk("[MT9M131]read  reg.%03x -> %04x\n", reg, ret);
	}
	return ret;
}

static int mt9m112_write_reg(struct i2c_client *client, unsigned short reg,
			     unsigned short val)
{
	int ret;

	ret = reg_page_map_set(client, reg);
	if (!ret)
		ret = i2c_smbus_write_word_data(client, (reg & 0xff),
						swab16(val));
	printk("[MT9M131] write reg.%03x = %04x -> %d\n", reg, val, ret);
	return ret;
}
#endif
static int mt9m112_reg_read(struct i2c_client *client, const u16 reg)
{
	int ret;

	ret = reg_page_map_set(client, reg);
	if (!ret)
		ret = swab16(i2c_smbus_read_word_data(client, reg & 0xff));

	dev_dbg(&client->dev, "read  reg.%03x -> %04x\n", reg, ret);
	return ret;
}

static int mt9m112_reg_write(struct i2c_client *client, const u16 reg,
			     const u16 data)
{
	int ret;

	ret = reg_page_map_set(client, reg);
	if (!ret)
		ret = i2c_smbus_write_word_data(client, reg & 0xff,
						swab16(data));
	dev_dbg(&client->dev, "write reg.%03x = %04x -> %d\n", reg, data, ret);
	return ret;
}

static int mt9m112_reg_set(struct i2c_client *client, const u16 reg,
			   const u16 data)
{
	int ret;

	ret = mt9m112_reg_read(client, reg);
	if (ret >= 0)
		ret = mt9m112_reg_write(client, reg, ret | data);
	return ret;
}

static int mt9m112_reg_clear(struct i2c_client *client, const u16 reg,
			     const u16 data)
{
	int ret;

	ret = mt9m112_reg_read(client, reg);
	return mt9m112_reg_write(client, reg, ret & ~data);
}

static int mt9m112_set_autowhitebalance(struct i2c_client *client, int on)
{
        int ret;

        if (on)
                ret = reg_set(OPER_MODE_CTRL, MT9M112_OPMODE_AUTOWHITEBAL_EN);
        else
                ret = reg_clear(OPER_MODE_CTRL, MT9M112_OPMODE_AUTOWHITEBAL_EN);

        return ret;
}

static int mt9m112_set_context(struct i2c_client *client,
			       enum mt9m112_context ctxt)
{
	int valB = MT9M112_CTXT_CTRL_RESTART | MT9M112_CTXT_CTRL_READ_MODE_B
		| MT9M112_CTXT_CTRL_VBLANK_SEL_B
		| MT9M112_CTXT_CTRL_HBLANK_SEL_B;
	int valA = MT9M112_CTXT_CTRL_RESTART;

	if (ctxt == HIGHPOWER)
		return reg_write(CONTEXT_CONTROL, valB);
	else
		return reg_write(CONTEXT_CONTROL, valA);
}

static int mt9m112_setup_rect(struct i2c_client *client,
			      struct v4l2_rect *rect)
{
	return 0;
}

static int mt9m112_setup_pixfmt(struct i2c_client *client, u16 outfmt)
{
	int ret;

	ret = reg_write(OUTPUT_FORMAT_CTRL2_A, outfmt);
	if (!ret)
		ret = reg_write(OUTPUT_FORMAT_CTRL2_B, outfmt);
	return ret;
}

static int mt9m112_setfmt_bayer8(struct i2c_client *client)
{
	return mt9m112_setup_pixfmt(client, MT9M112_OUTFMT_PROCESSED_BAYER);
}

static int mt9m112_setfmt_bayer10(struct i2c_client *client)
{
	return mt9m112_setup_pixfmt(client, MT9M112_OUTFMT_BYPASS_IFP);
}

static int mt9m112_setfmt_rgb565(struct i2c_client *client)
{
	return 0;
}

static int mt9m112_setfmt_rgb555(struct i2c_client *client)
{
	return 0;
}

static int mt9m112_setfmt_yuv(struct i2c_client *client)
{
	return 0;
}

static int mt9m112_enable(struct i2c_client *client)
{
	int ret;

	ret = reg_set(RESET, MT9M112_RESET_CHIP_ENABLE);
	return ret;
}

static int mt9m112_reset(struct i2c_client *client)
{
	int ret;

	ret = reg_set(RESET, MT9M112_RESET_RESET_MODE);
	if (!ret)
		ret = reg_set(RESET, MT9M112_RESET_RESET_SOC);
	if (!ret)
		ret = reg_clear(RESET, MT9M112_RESET_RESET_MODE
				| MT9M112_RESET_RESET_SOC);

	return ret;
}

/* configure mux, for DM355 EVM only */
#ifndef CONFIG_MACH_DM355_LEOPARD
static int mt9m112_en_mux(struct i2c_client *client)
{
	int err = 0;
	int trycnt = 0;
	/* unsigned short readval = 0; */

	struct i2c_msg msg[1];
	unsigned char data[4];
	err = -1;
	printk(KERN_INFO "\n entering mt9m112_en_mux \n");

	while ((err < 0) && (trycnt < 5)) {
		trycnt++;
		if (!client->adapter) {
			err = -ENODEV;
		} else {
			msg->addr = 0x25;
			msg->flags = 0;
			msg->len = I2C_TWO_BYTE_TRANSFER;
			msg->buf = data;
			data[0] = (unsigned char)(0x08 & I2C_TXRX_DATA_MASK);
			data[1] = (unsigned char)(0x80 & I2C_TXRX_DATA_MASK);

			err = i2c_transfer(client->adapter, msg, 1);
			if (err < 0) {
				printk(KERN_INFO
				       "\n ERROR in ECP  register write\n");
			}
		}
	}
	if (err < 0) {
		printk(KERN_INFO "\n I2C write failed");
	}
	return err;
}
#endif

/*
 * mt9m112_write_regs : Initializes a list of registers
 *		if token is TOK_TERM, then entire write operation terminates
 *		if token is TOK_DELAY, then a delay of 'val' msec is introduced
 *		if token is TOK_SKIP, then the register write is skipped
 *		if token is TOK_WRITE, then the register write is performed
 *
 * reglist - list of registers to be written
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9m112_write_regs(struct i2c_client *client,
			      const struct mt9m112_reg reglist[])
{
	int err;
	const struct mt9m112_reg *next = reglist;

	for (; next->token != TOK_TERM; next++) {
		if (next->token == TOK_DELAY) {
			msleep(next->val);
			continue;
		}

		if (next->token == TOK_SKIP)
			continue;

		err = mt9m112_reg_write(client, next->reg, next->val);
		if (err < 0) {
			v4l_err(client, "Write failed. Err[%d]\n", err);
			return err;
		}
	}
	return 0;
}

/*
 * mt9m112_get_current_std:
 * Returns the current standard 
 */
static enum mt9m112_std mt9m112_get_current_std(struct mt9m112_decoder
						*decoder)
{
	return MT9M112_STD_VGA;
}

/*
 * Configure the mt9m112 with the current register settings
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9m112_configure(struct mt9m112_decoder *decoder)
{
	int err;

	/* common register initialization */
	err = mt9m112_write_regs(decoder->client, mt9m112_reg_list);
	if (err)
		return err;
#if 0
        err = mt9m112_set_autowhitebalance(decoder->client, 1);
#endif
#if 0
/* pll setting */
        mt9m112_reg_write(decoder->client, 0x066, ((0x1c<<8) | 0x1));
        mt9m112_reg_write(decoder->client, 0x067, 6);

        u16 val = mt9m112_reg_read(decoder->client, 0x064);
        val &= ~(1<<14);
        mt9m112_reg_write(decoder->client, 0x064, val);
        mdelay(10);
        val = mt9m112_reg_read(decoder->client, 0x065);
        val &= ~(1<<15);
        mt9m112_reg_write(decoder->client, 0x065, val);
#endif
//      if (debug)
//              mt9m112_reg_dump(decoder);

	return 0;
}

/*
 * Configure the MT9V113 to VGA mode 
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9m112_vga_mode(struct mt9m112_decoder *decoder)
{
	int err;

	err = mt9m112_write_regs(decoder->client, mt9m112_vga_reg);
	if (err)
		return err;

	return 0;
}

/*
 * Detect if an mt9m112 is present, and if so which revision.
 * A device is considered to be detected if the chip ID (LSB and MSB)
 * registers match the expected values.
 * Any value of the rom version register is accepted.
 * Returns ENODEV error number if no device is detected, or zero
 * if a device is detected.
 */
static int mt9m112_detect(struct mt9m112_decoder *decoder)
{
	unsigned short val = 0;

#ifndef CONFIG_MACH_DM355_LEOPARD
	mt9m112_en_mux(decoder->client);
#endif

	val = mt9m112_reg_read(decoder->client, MT9M112_CHIP_VERSION);

	v4l_dbg(1, debug, decoder->client, "chip id detected 0x%x\n", val);

	if (MT9M112_CHIP_ID != val) {
		/* We didn't read the values we expected, so this must not be
		 * MT9M112.
		 */
		v4l_err(decoder->client,
			"chip id mismatch read 0x%x, expecting 0x%x\n", val,
			MT9M112_CHIP_ID);
		return -ENODEV;
	}

	decoder->ver = val;
	decoder->state = STATE_DETECTED;

	v4l_info(decoder->client,
		 "%s found at 0x%x (%s)\n", decoder->client->name,
		 decoder->client->addr << 1, decoder->client->adapter->name);

	return 0;
}

/*
 * Following are decoder interface functions implemented by
 * mt9m112 decoder driver.
 */

/**
 * ioctl_querystd - V4L2 decoder interface handler for VIDIOC_QUERYSTD ioctl
 * @s: pointer to standard V4L2 device structure
 * @std_id: standard V4L2 std_id ioctl enum
 *
 * Returns the current standard detected by mt9m112. If no active input is
 * detected, returns -EINVAL
 */
static int ioctl_querystd(struct v4l2_int_device *s, v4l2_std_id * std_id)
{
	struct mt9m112_decoder *decoder = s->priv;
	enum mt9m112_std current_std;

	if (std_id == NULL)
		return -EINVAL;

	/* get the current standard */
	current_std = mt9m112_get_current_std(decoder);
	if (current_std == MT9M112_IMAGE_STD_INVALID)
		return -EINVAL;

	decoder->current_std = current_std;
	*std_id = decoder->std_list[current_std].standard.id;

	v4l_dbg(1, debug, decoder->client, "Current STD: %s",
		decoder->std_list[current_std].standard.name);
	return 0;
}

/**
 * ioctl_s_std - V4L2 decoder interface handler for VIDIOC_S_STD ioctl
 * @s: pointer to standard V4L2 device structure
 * @std_id: standard V4L2 v4l2_std_id ioctl enum
 *
 * If std_id is supported, sets the requested standard. Otherwise, returns
 * -EINVAL
 */
static int ioctl_s_std(struct v4l2_int_device *s, v4l2_std_id * std_id)
{
	struct mt9m112_decoder *decoder = s->priv;
	int err, i;

	if (std_id == NULL)
		return -EINVAL;

	for (i = 0; i < decoder->num_stds; i++)
		if (*std_id & decoder->std_list[i].standard.id)
			break;

	if ((i == decoder->num_stds) || (i == MT9M112_STD_INVALID))
		return -EINVAL;

	decoder->current_std = i;

	v4l_dbg(1, debug, decoder->client, "Standard set to: %s",
		decoder->std_list[i].standard.name);
	return 0;
}

/**
 * ioctl_s_routing - V4L2 decoder interface handler for VIDIOC_S_INPUT ioctl
 * @s: pointer to standard V4L2 device structure
 * @index: number of the input
 *
 * If index is valid, selects the requested input. Otherwise, returns -EINVAL if
 * the input is not supported or there is no active signal present in the
 * selected input.
 */
static int ioctl_s_routing(struct v4l2_int_device *s,
			   struct v4l2_routing *route)
{
	return 0;
}

/**
 * ioctl_queryctrl - V4L2 decoder interface handler for VIDIOC_QUERYCTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @qctrl: standard V4L2 v4l2_queryctrl structure
 *
 * If the requested control is supported, returns the control information.
 * Otherwise, returns -EINVAL if the control is not supported.
 */
static int
ioctl_queryctrl(struct v4l2_int_device *s, struct v4l2_queryctrl *qctrl)
{
	struct mt9m112_decoder *decoder = s->priv;
	int err = -EINVAL;

	if (qctrl == NULL)
		return err;

	switch (qctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		/* Brightness supported is same as standard one (0-255),
		 * so make use of standard API provided.
		 */
		err = v4l2_ctrl_query_fill_std(qctrl);
		break;
	case V4L2_CID_CONTRAST:
	case V4L2_CID_SATURATION:
		/* Saturation and Contrast supported is -
		 *      Contrast: 0 - 255 (Default - 128)
		 *      Saturation: 0 - 255 (Default - 128)
		 */
		err = v4l2_ctrl_query_fill(qctrl, 0, 255, 1, 128);
		break;
	case V4L2_CID_HUE:
		/* Hue Supported is -
		 *      Hue - -180 - +180 (Default - 0, Step - +180)
		 */
		err = v4l2_ctrl_query_fill(qctrl, -180, 180, 180, 0);
		break;
	case V4L2_CID_AUTOGAIN:
		/* Autogain is either 0 or 1 */
		memcpy(qctrl, &mt9m112_autogain_ctrl,
		       sizeof(struct v4l2_queryctrl));
		err = 0;
		break;
	default:
		v4l_err(decoder->client, "invalid control id %d\n", qctrl->id);
		return err;
	}

	v4l_dbg(1, debug, decoder->client,
		"Query Control: %s : Min - %d, Max - %d, Def - %d",
		qctrl->name,
		qctrl->minimum, qctrl->maximum, qctrl->default_value);

	return err;
}

/**
 * ioctl_g_ctrl - V4L2 decoder interface handler for VIDIOC_G_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @ctrl: pointer to v4l2_control structure
 *
 * If the requested control is supported, returns the control's current
 * value from the decoder. Otherwise, returns -EINVAL if the control is not
 * supported.
 */
static int ioctl_g_ctrl(struct v4l2_int_device *s, struct v4l2_control *ctrl)
{
	struct mt9m112_decoder *decoder = s->priv;

	if (ctrl == NULL)
		return -EINVAL;

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		break;
	case V4L2_CID_CONTRAST:
		break;
	case V4L2_CID_SATURATION:
		break;
	case V4L2_CID_HUE:
		break;
	case V4L2_CID_AUTOGAIN:
		break;
	default:
		v4l_err(decoder->client, "invalid control id %d\n", ctrl->id);
		return -EINVAL;
	}

	v4l_dbg(1, debug, decoder->client,
		"Get Control: ID - %d - %d", ctrl->id, ctrl->value);
	return 0;
}

/**
 * ioctl_s_ctrl - V4L2 decoder interface handler for VIDIOC_S_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @ctrl: pointer to v4l2_control structure
 *
 * If the requested control is supported, sets the control's current
 * value in HW. Otherwise, returns -EINVAL if the control is not supported.
 */
static int ioctl_s_ctrl(struct v4l2_int_device *s, struct v4l2_control *ctrl)
{
	struct mt9m112_decoder *decoder = s->priv;
	int err = -EINVAL, value;

	if (ctrl == NULL)
		return err;

	value = (__s32) ctrl->value;

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		break;
	case V4L2_CID_CONTRAST:
		break;
	case V4L2_CID_SATURATION:
		break;
	case V4L2_CID_HUE:
		break;
	case V4L2_CID_AUTOGAIN:
		break;
	default:
		v4l_err(decoder->client, "invalid control id %d\n", ctrl->id);
		return err;
	}

	v4l_dbg(1, debug, decoder->client,
		"Set Control: ID - %d - %d", ctrl->id, ctrl->value);

	return err;
}

/**
 * ioctl_enum_fmt_cap - Implement the CAPTURE buffer VIDIOC_ENUM_FMT ioctl
 * @s: pointer to standard V4L2 device structure
 * @fmt: standard V4L2 VIDIOC_ENUM_FMT ioctl structure
 *
 * Implement the VIDIOC_ENUM_FMT ioctl to enumerate supported formats
 */
static int
ioctl_enum_fmt_cap(struct v4l2_int_device *s, struct v4l2_fmtdesc *fmt)
{
	struct mt9m112_decoder *decoder = s->priv;
	int index;

	if (fmt == NULL)
		return -EINVAL;

	index = fmt->index;
	if ((index >= decoder->num_fmts) || (index < 0))
		return -EINVAL;	/* Index out of bound */

	if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;	/* only capture is supported */

	memcpy(fmt, &decoder->fmt_list[index], sizeof(struct v4l2_fmtdesc));

	v4l_dbg(1, debug, decoder->client,
		"Current FMT: index - %d (%s)",
		decoder->fmt_list[index].index,
		decoder->fmt_list[index].description);
	return 0;
}

/**
 * ioctl_try_fmt_cap - Implement the CAPTURE buffer VIDIOC_TRY_FMT ioctl
 * @s: pointer to standard V4L2 device structure
 * @f: pointer to standard V4L2 VIDIOC_TRY_FMT ioctl structure
 *
 * Implement the VIDIOC_TRY_FMT ioctl for the CAPTURE buffer type. This
 * ioctl is used to negotiate the image capture size and pixel format
 * without actually making it take effect.
 */
static int ioctl_try_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct mt9m112_decoder *decoder = s->priv;
	int ifmt;
	struct v4l2_pix_format *pix;
	enum mt9m112_std current_std;

	if (f == NULL)
		return -EINVAL;

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		f->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	pix = &f->fmt.pix;

	/* Calculate height and width based on current standard */
	current_std = mt9m112_get_current_std(decoder);
	if (current_std == MT9M112_STD_INVALID)
		return -EINVAL;

	decoder->current_std = current_std;
	pix->width = decoder->std_list[current_std].width;
	pix->height = decoder->std_list[current_std].height;

	for (ifmt = 0; ifmt < decoder->num_fmts; ifmt++) {
		if (pix->pixelformat == decoder->fmt_list[ifmt].pixelformat)
			break;
	}
	if (ifmt == decoder->num_fmts)
		ifmt = 0;	/* None of the format matched, select default */
	pix->pixelformat = decoder->fmt_list[ifmt].pixelformat;

	pix->field = V4L2_FIELD_NONE;
	pix->bytesperline = pix->width * 2;
	pix->sizeimage = pix->bytesperline * pix->height;
	pix->colorspace = V4L2_COLORSPACE_JPEG; /* V4L2_COLORSPACE_SMPTE170M; */
	pix->priv = 0;

	v4l_dbg(1, debug, decoder->client,
		"Try FMT: pixelformat - %s, bytesperline - %d"
		"Width - %d, Height - %d",
		decoder->fmt_list[ifmt].description, pix->bytesperline,
		pix->width, pix->height);
	return 0;
}

/**
 * ioctl_s_fmt_cap - V4L2 decoder interface handler for VIDIOC_S_FMT ioctl
 * @s: pointer to standard V4L2 device structure
 * @f: pointer to standard V4L2 VIDIOC_S_FMT ioctl structure
 *
 * If the requested format is supported, configures the HW to use that
 * format, returns error code if format not supported or HW can't be
 * correctly configured.
 */
static int ioctl_s_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct mt9m112_decoder *decoder = s->priv;
	struct v4l2_pix_format *pix;
	int rval;

	if (f == NULL)
		return -EINVAL;

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;	/* only capture is supported */

	pix = &f->fmt.pix;
	rval = ioctl_try_fmt_cap(s, f);
	if (rval)
		return rval;

	decoder->pix = *pix;

	return rval;
}

/**
 * ioctl_g_fmt_cap - V4L2 decoder interface handler for ioctl_g_fmt_cap
 * @s: pointer to standard V4L2 device structure
 * @f: pointer to standard V4L2 v4l2_format structure
 *
 * Returns the decoder's current pixel format in the v4l2_format
 * parameter.
 */
static int ioctl_g_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct mt9m112_decoder *decoder = s->priv;

	if (f == NULL)
		return -EINVAL;

	if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;	/* only capture is supported */

	f->fmt.pix = decoder->pix;

	v4l_dbg(1, debug, decoder->client,
		"Current FMT: bytesperline - %d"
		"Width - %d, Height - %d",
		decoder->pix.bytesperline,
		decoder->pix.width, decoder->pix.height);
	return 0;
}

/**
 * ioctl_g_parm - V4L2 decoder interface handler for VIDIOC_G_PARM ioctl
 * @s: pointer to standard V4L2 device structure
 * @a: pointer to standard V4L2 VIDIOC_G_PARM ioctl structure
 *
 * Returns the decoder's video CAPTURE parameters.
 */
static int ioctl_g_parm(struct v4l2_int_device *s, struct v4l2_streamparm *a)
{
	struct mt9m112_decoder *decoder = s->priv;
	struct v4l2_captureparm *cparm;
	enum mt9m112_std current_std;

	if (a == NULL)
		return -EINVAL;

	if (a->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;	/* only capture is supported */

	memset(a, 0, sizeof(*a));
	a->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	/* get the current standard */
	current_std = mt9m112_get_current_std(decoder);
	if (current_std == MT9M112_STD_INVALID)
		return -EINVAL;

	decoder->current_std = current_std;

	cparm = &a->parm.capture;
	cparm->capability = V4L2_CAP_TIMEPERFRAME;
	cparm->timeperframe =
	    decoder->std_list[current_std].standard.frameperiod;

	return 0;
}

/**
 * ioctl_s_parm - V4L2 decoder interface handler for VIDIOC_S_PARM ioctl
 * @s: pointer to standard V4L2 device structure
 * @a: pointer to standard V4L2 VIDIOC_S_PARM ioctl structure
 *
 * Configures the decoder to use the input parameters, if possible. If
 * not possible, returns the appropriate error code.
 */
static int ioctl_s_parm(struct v4l2_int_device *s, struct v4l2_streamparm *a)
{
	struct mt9m112_decoder *decoder = s->priv;
	struct v4l2_fract *timeperframe;
	enum mt9m112_std current_std;

	if (a == NULL)
		return -EINVAL;

	if (a->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;	/* only capture is supported */

	timeperframe = &a->parm.capture.timeperframe;

	/* get the current standard */
	current_std = mt9m112_get_current_std(decoder);
	if (current_std == MT9M112_STD_INVALID)
		return -EINVAL;

	decoder->current_std = current_std;

	*timeperframe = decoder->std_list[current_std].standard.frameperiod;

	return 0;
}

/**
 * ioctl_g_ifparm - V4L2 decoder interface handler for vidioc_int_g_ifparm_num
 * @s: pointer to standard V4L2 device structure
 * @p: pointer to standard V4L2 vidioc_int_g_ifparm_num ioctl structure
 *
 * Gets slave interface parameters.
 * Calculates the required xclk value to support the requested
 * clock parameters in p. This value is returned in the p
 * parameter.
 */
static int ioctl_g_ifparm(struct v4l2_int_device *s, struct v4l2_ifparm *p)
{
	struct mt9m112_decoder *decoder = s->priv;
	int rval;

	if (p == NULL)
		return -EINVAL;

	if (NULL == decoder->pdata->ifparm)
		return -EINVAL;

	rval = decoder->pdata->ifparm(p);
	if (rval) {
		v4l_err(decoder->client, "g_ifparm.Err[%d]\n", rval);
		return rval;
	}

	p->u.bt656.clock_curr = 27000000;	// TODO: read clock rate from sensor

	return 0;
}

/**
 * ioctl_g_priv - V4L2 decoder interface handler for vidioc_int_g_priv_num
 * @s: pointer to standard V4L2 device structure
 * @p: void pointer to hold decoder's private data address
 *
 * Returns device's (decoder's) private data area address in p parameter
 */
static int ioctl_g_priv(struct v4l2_int_device *s, void *p)
{
	struct mt9m112_decoder *decoder = s->priv;

	if (NULL == decoder->pdata->priv_data_set)
		return -EINVAL;

	return decoder->pdata->priv_data_set(p);
}

/**
 * ioctl_s_power - V4L2 decoder interface handler for vidioc_int_s_power_num
 * @s: pointer to standard V4L2 device structure
 * @on: power state to which device is to be set
 *
 * Sets devices power state to requrested state, if possible.
 */
static int ioctl_s_power(struct v4l2_int_device *s, enum v4l2_power on)
{
	struct mt9m112_decoder *decoder = s->priv;
	int err = 0;

	switch (on) {
	case V4L2_POWER_OFF:
		/* Power Down Sequence */
		/*	err = mt9m112_write_reg(decoder->client, 0x000D, 0x0000);*/
		/* Disable mux for mt9m112 data path */
		if (decoder->pdata->power_set)
			err |= decoder->pdata->power_set(on);
		decoder->state = STATE_NOT_DETECTED;
		break;

	case V4L2_POWER_STANDBY:
		if (decoder->pdata->power_set)
			err = decoder->pdata->power_set(on);
		break;

	case V4L2_POWER_ON:
		/* Enable mux for mt9m112 data path */
		if ((decoder->pdata->power_set) &&
		    (decoder->state == STATE_NOT_DETECTED)) {

			err = decoder->pdata->power_set(on);

			/* Detect the sensor is not already detected */
			err |= mt9m112_detect(decoder);
			if (err) {
				v4l_err(decoder->client,
					"Unable to detect decoder\n");
				return err;
			}
		}
		// Only VGA mode for now
		// err |= mt9m112_vga_mode(decoder);
		break;

	default:
		err = -ENODEV;
		break;
	}

	return err;
}

/**
 * ioctl_init - V4L2 decoder interface handler for VIDIOC_INT_INIT
 * @s: pointer to standard V4L2 device structure
 *
 * Initialize the decoder device (calls mt9m112_configure())
 */
static int ioctl_init(struct v4l2_int_device *s)
{
//      struct mt9m112_decoder *decoder = s->priv;
	int err = 0;

	return err;
}

/**
 * ioctl_dev_exit - V4L2 decoder interface handler for vidioc_int_dev_exit_num
 * @s: pointer to standard V4L2 device structure
 *
 * Delinitialise the dev. at slave detach. The complement of ioctl_dev_init.
 */
static int ioctl_dev_exit(struct v4l2_int_device *s)
{
	return 0;
}

/**
 * ioctl_dev_init - V4L2 decoder interface handler for vidioc_int_dev_init_num
 * @s: pointer to standard V4L2 device structure
 *
 * Initialise the device when slave attaches to the master. Returns 0 if
 * mt9m112 device could be found, otherwise returns appropriate error.
 */
static int ioctl_dev_init(struct v4l2_int_device *s)
{
	struct mt9m112_decoder *decoder = s->priv;
	int err;

	err = mt9m112_detect(decoder);
	if (err < 0) {
		v4l_err(decoder->client, "Unable to detect decoder\n");
		return err;
	}

	v4l_info(decoder->client,
		 "chip version 0x%.2x detected\n", decoder->ver);

	err |= mt9m112_configure(decoder);
	// err |= mt9m112_vga_mode(decoder);

	return 0;
}

static struct v4l2_int_ioctl_desc mt9m112_ioctl_desc[] = {
	{vidioc_int_dev_init_num, (v4l2_int_ioctl_func *) ioctl_dev_init},
	{vidioc_int_dev_exit_num, (v4l2_int_ioctl_func *) ioctl_dev_exit},
	{vidioc_int_s_power_num, (v4l2_int_ioctl_func *) ioctl_s_power},
	{vidioc_int_g_priv_num, (v4l2_int_ioctl_func *) ioctl_g_priv},
	{vidioc_int_g_ifparm_num, (v4l2_int_ioctl_func *) ioctl_g_ifparm},
	{vidioc_int_init_num, (v4l2_int_ioctl_func *) ioctl_init},
	{vidioc_int_enum_fmt_cap_num,
	 (v4l2_int_ioctl_func *) ioctl_enum_fmt_cap},
	{vidioc_int_try_fmt_cap_num,
	 (v4l2_int_ioctl_func *) ioctl_try_fmt_cap},
	{vidioc_int_g_fmt_cap_num,
	 (v4l2_int_ioctl_func *) ioctl_g_fmt_cap},
	{vidioc_int_s_fmt_cap_num,
	 (v4l2_int_ioctl_func *) ioctl_s_fmt_cap},
	{vidioc_int_g_parm_num, (v4l2_int_ioctl_func *) ioctl_g_parm},
	{vidioc_int_s_parm_num, (v4l2_int_ioctl_func *) ioctl_s_parm},
	{vidioc_int_queryctrl_num,
	 (v4l2_int_ioctl_func *) ioctl_queryctrl},
	{vidioc_int_g_ctrl_num, (v4l2_int_ioctl_func *) ioctl_g_ctrl},
	{vidioc_int_s_ctrl_num, (v4l2_int_ioctl_func *) ioctl_s_ctrl},
	{vidioc_int_querystd_num, (v4l2_int_ioctl_func *) ioctl_querystd},
	{vidioc_int_s_std_num, (v4l2_int_ioctl_func *) ioctl_s_std},
	{vidioc_int_s_video_routing_num,
	 (v4l2_int_ioctl_func *) ioctl_s_routing},
};

static struct v4l2_int_slave mt9m112_slave = {
	.ioctls = mt9m112_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(mt9m112_ioctl_desc),
};

static struct mt9m112_decoder mt9m112_dev = {
	.state = STATE_NOT_DETECTED,

	.fmt_list = mt9m112_fmt_list,
	.num_fmts = ARRAY_SIZE(mt9m112_fmt_list),

	.pix = {		/* Default to 8-bit YUV 422 */
		.width = VGA_NUM_ACTIVE_PIXELS,
		.height = VGA_NUM_ACTIVE_LINES,
		.pixelformat = V4L2_PIX_FMT_UYVY,
		.field = V4L2_FIELD_NONE,
		.bytesperline = VGA_NUM_ACTIVE_PIXELS * 2,
		.sizeimage = VGA_NUM_ACTIVE_PIXELS * 2 * VGA_NUM_ACTIVE_LINES,
		.colorspace = V4L2_COLORSPACE_SMPTE170M,
		},

	.current_std = MT9M112_STD_VGA,
	.std_list = mt9m112_std_list,
	.num_stds = ARRAY_SIZE(mt9m112_std_list),

};

static struct v4l2_int_device mt9m112_int_device = {
	.module = THIS_MODULE,
	.name = MT9M112_MODULE_NAME,
	.priv = &mt9m112_dev,
	.type = v4l2_int_type_slave,
	.u = {
	      .slave = &mt9m112_slave,
	      },
};

/**
 * mt9m112_probe - decoder driver i2c probe handler
 * @client: i2c driver client device structure
 *
 * Register decoder as an i2c client device and V4L2
 * device.
 */
static int
mt9m112_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct mt9m112_decoder *decoder = &mt9m112_dev;
	int err;

	/* cmos reset */
	gpio_request(26, "cmos-reset");
	gpio_direction_output(26, 0);
	mdelay(10);
	gpio_direction_output(26, 1);

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

	decoder->pdata = client->dev.platform_data;
	if (!decoder->pdata) {
		v4l_err(client, "No platform data!!\n");
		return -ENODEV;
	}
	/*
	 * Fetch platform specific data, and configure the
	 * mt9m112_reg_list[] accordingly. Since this is one
	 * time configuration, no need to preserve.
	 */

	/*mt9m112_reg_list[REG_OUTPUT_FORMATTER2].val |=
	   (decoder->pdata->clk_polarity << 1);
	   mt9m112_reg_list[REG_SYNC_CONTROL].val |=
	   ((decoder->pdata->hs_polarity << 2) |
	   (decoder->pdata->vs_polarity << 3));
	 */
	/*
	 * Save the id data, required for power up sequence
	 */
	decoder->id = (struct i2c_device_id *)id;
	/* Attach to Master */
	strcpy(mt9m112_int_device.u.slave->attach_to, decoder->pdata->master);
	decoder->v4l2_int_device = &mt9m112_int_device;
	decoder->client = client;
	i2c_set_clientdata(client, decoder);

	/* Register with V4L2 layer as slave device */
	err = v4l2_int_device_register(decoder->v4l2_int_device);
	if (err) {
		i2c_set_clientdata(client, NULL);
		v4l_err(client, "Unable to register to v4l2. Err[%d]\n", err);

	} else
		v4l_info(client, "Registered to v4l2 master %s!!\n",
			 decoder->pdata->master);
	return 0;
}

/**
 * mt9m112_remove - decoder driver i2c remove handler
 * @client: i2c driver client device structure
 *
 * Unregister decoder as an i2c client device and V4L2
 * device. Complement of mt9m112_probe().
 */
static int __exit mt9m112_remove(struct i2c_client *client)
{
	struct mt9m112_decoder *decoder = i2c_get_clientdata(client);

	if (!client->adapter)
		return -ENODEV;	/* our client isn't attached */

	v4l2_int_device_unregister(decoder->v4l2_int_device);
	i2c_set_clientdata(client, NULL);

	return 0;
}

/*
 * mt9m112 Init/Power on Sequence
 */
static const struct mt9m112_reg mt9m112_init_reg_seq[] = {
	{TOK_WRITE, 0x000D, 0x0009},	/* RESET */
	{TOK_WRITE, 0x000D, 0x0029},
	{TOK_WRITE, 0x000D, 0x0008},
};

static const struct mt9m112_init_seq mt9m112_dev_init = {
	.no_regs = ARRAY_SIZE(mt9m112_init_reg_seq),
	.init_reg_seq = mt9m112_init_reg_seq,
};

/*
 * I2C Device Table -
 *
 * name - Name of the actual device/chip.
 * driver_data - Driver data
 */
static const struct i2c_device_id mt9m112_id[] = {
	{"mt9m112", (unsigned long)&mt9m112_dev_init},
	{},
};

MODULE_DEVICE_TABLE(i2c, mt9m112_id);

static struct i2c_driver mt9m112_i2c_driver = {
	.driver = {
		   .name = MT9M112_MODULE_NAME,
		   .owner = THIS_MODULE,
		   },
	.probe = mt9m112_probe,
	.remove = __exit_p(mt9m112_remove),
	.id_table = mt9m112_id,
};

/**
 * mt9m112_init
 *
 * Module init function
 */
static int __init mt9m112_init(void)
{
	return i2c_add_driver(&mt9m112_i2c_driver);
}

/**
 * mt9m112_cleanup
 *
 * Module exit function
 */
static void __exit mt9m112_cleanup(void)
{
	i2c_del_driver(&mt9m112_i2c_driver);
}

module_init(mt9m112_init);
module_exit(mt9m112_cleanup);

MODULE_AUTHOR("Seokheon Bae shbae@navicam.co.kr");
MODULE_DESCRIPTION("MT9M112 sensor linux driver");
MODULE_LICENSE("GPL");
