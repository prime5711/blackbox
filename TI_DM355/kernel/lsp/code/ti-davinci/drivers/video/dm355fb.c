/* 
 * Copyright (C) 2006 Texas Instruments Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option)any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * File: dm355fb.c	
 */

/*
 * Linux includes	
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>	/* for module_param() */
#include <asm/system.h>
#include <video/dm355fb.h>
#include <video/dm355_vpbe.h>

/*
 * Local includes	
 */
#include "dm355fb_osd.c"
#include "dm355fb_dlcd.c"
#include "dm355fb_venc.c"

/*
 *     Module parameter definations
 */
static char *options = "";

module_param(options, charp, S_IRUGO);

/*
 * Globals
 */
 /*     Modelist        */
/* 
 * First mode(0th mode) in case of composite, svideo and component are kept
 * 	dummy. This is for backward compatibility.	
 */
struct vpbe_fb_videomode svideo[] = {
	{"DUMMY",
	 FB_VMODE_INTERLACED,
	 720,
	 480,
	 30,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"NTSC-SV",
	 FB_VMODE_INTERLACED,
	 720,
	 480,
	 30,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"PAL-SV",
	 FB_VMODE_INTERLACED,
	 720,
	 576,
	 25,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

struct vpbe_fb_videomode component[] = {
	{"LCD",
	 FB_VMODE_NONINTERLACED,
	 720,
	 480,
	 60,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"NTSC-CO",
	 FB_VMODE_INTERLACED,
	 720,
	 480,
	 30,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"PAL-CO",
	 FB_VMODE_INTERLACED,
	 720,
	 576,
	 25,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"P525",
	 FB_VMODE_NONINTERLACED,
	 720,
	 480,
	 60,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x22,
	 1},
	{"P625",
	 FB_VMODE_NONINTERLACED,
	 720,
	 576,
	 60,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x22,
	 1},
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

struct vpbe_fb_videomode composite[] = {
	{"DUMMY_COM",
	 FB_VMODE_INTERLACED,
	 720,
	 480,
	 30,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"NTSC_COM",
	 FB_VMODE_INTERLACED,
	 720,
	 480,
	 30,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"PAL_COM",
	 FB_VMODE_INTERLACED,
	 720,
	 576,
	 25,
	 0,
	 0,
	 0,
	 0,
	 0x80,
	 0x12,
	 0,
	 0x80,
	 0x12,
	 1},
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode rgb[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode ycc16[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode ycc8[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

struct vpbe_fb_videomode prgb[] = {
        {"PRGB_480",
         FB_VMODE_NONINTERLACED,
         640,
         480,
         60,
         95,
         70,
         34,
         11,
         9,
         9,
         0,
         0x59,
         0x22,
         1},
        {"PRGB_400",
         FB_VMODE_NONINTERLACED,
         640,
         400,
         60,
         96,
         70,
         40,
         15,
         9,
         9,
         1,
         0x58,
         0x22,
         1},
	{"PRGB_350",
         FB_VMODE_NONINTERLACED,
         640,
         350,
         60,
         96,
         70,
         0x59,
         0x52,
         9,
         9,
         2,
         0x58,
         0x72,
         1},
        {"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

struct vpbe_fb_videomode srgb[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode epson[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode casio1g[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode udisp[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode stn[] = {
	{"\0", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
struct vpbe_fb_videomode *modelist[] =
    { prgb, composite, svideo, component, rgb, ycc16, ycc8, srgb, epson,
	casio1g, udisp, stn
};

/*
 * display controller register I/O routines
 */
inline u32 dispc_reg_in(u32 offset)
{
	return (inl(offset));
}

inline u32 dispc_reg_out(u32 offset, u32 val)
{
	outl(val, offset);
	return (val);
}

inline u32 dispc_reg_merge(u32 offset, u32 val, u32 mask)
{
	u32 addr = offset;
	u32 new_val = (inl(addr) & ~mask) | (val & mask);
	outl(new_val, addr);
	return (new_val);
}

vpbe_dm_info_t dm_static;
vpbe_dm_info_t *dm = &dm_static;
static struct fb_ops dm355fb_ops;
/* flags to indicate that window size is specified from command line */
int flag_osd0 = 0, flag_osd1 = 0, flag_vid0 = 0, flag_vid1 = 0;

/* Default resolutions		*/
#define DISP_XRES	720
#define DISP_YRES	480
/*These should be set to the max resolution supported	*/
#define	DISP_MEMX	720
#define DISP_MEMY	576

/* Random value chosen for now. Should be within the panel's supported range */
#define LCD_PANEL_CLOCK	180000

/* All window widths have to be rounded up to a multiple of 32 bytes */
#define round_32(width)	((((width) + 31) / 32) * 32 )

#define OSD0_MAXBPP	16

/* 16 bpp, Double buffered */
static struct fb_var_screeninfo osd0_default_var = {
	.xres = DISP_XRES,
	.yres = DISP_YRES,
	.xres_virtual = DISP_XRES,
	.yres_virtual = DISP_MEMY * DOUBLE_BUF,
	.xoffset = 0,
	.yoffset = 0,
	.bits_per_pixel = YUV422_RGB565,
	.grayscale = 0,
	.red = {11, 5, 0}, /* for RGB565 */
	.green = {5, 6, 0},
	.blue = {0, 5, 0},
	.transp = {0, 0, 0},
	.nonstd = 0,
	.activate = FB_ACTIVATE_NOW,
	.height = -1,
	.width = -1,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK,	/* picoseconds */
	.left_margin = 40,	/* pixclocks */
	.right_margin = 4,	/* pixclocks */
	.upper_margin = 8,	/* line clocks */
	.lower_margin = 2,	/* line clocks */
	.hsync_len = 4,		/* pixclocks */
	.vsync_len = 2,		/* line clocks */
	.sync = 0,
	.vmode = FB_VMODE_INTERLACED,
};

/* Using the full screen for OSD1 by default */
#define OSD1_MAXBPP     8

static struct fb_var_screeninfo osd1_default_var = {
	.xres = DISP_XRES,
	.yres = DISP_YRES,
	.xres_virtual = DISP_XRES,
	.yres_virtual = DISP_MEMY* DOUBLE_BUF,
	.xoffset = 0,
	.yoffset = 0,
	.bits_per_pixel = BITMAP_4BIT,
	.activate = FB_ACTIVATE_NOW,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK,	/* picoseconds */
	.vmode = FB_VMODE_INTERLACED,
};

/* Using the full screen for OSD0 by default */
#define VID0_MAXBPP     16  

static struct fb_var_screeninfo vid0_default_var = {
	.xres = DISP_XRES,
	.yres = DISP_YRES,
	.xres_virtual = DISP_XRES,
	.yres_virtual = DISP_MEMY * TRIPLE_BUF,
	.xoffset = 0,
	.yoffset = 0,
	.bits_per_pixel = YUV422_RGB565,
	.activate = FB_ACTIVATE_NOW,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK,	/* picoseconds */
	.vmode = FB_VMODE_INTERLACED,
};

/* Using the bottom right quadrant of the screen screen for VID1 by default,
 * but keeping the framebuffer allocated for the full screen, so the user can
 * change the 'xres' and 'yres' later using the FBIOPUT_VSCREENINFO ioctl.
 */
#define VID1_MAXBPP     16

static struct fb_var_screeninfo vid1_default_var = {
	.xres = DISP_XRES,
	.yres = DISP_YRES,
	.xres_virtual = DISP_XRES,
	.yres_virtual = DISP_MEMY * TRIPLE_BUF,
	.xoffset = 0,
	.yoffset = 0,
	.bits_per_pixel = YUV422_RGB565,
	.activate = FB_ACTIVATE_NOW,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK,	/* picoseconds */
	.vmode = FB_VMODE_INTERLACED,
};

#define x_pos(w)    ((w)->win_pos.xpos)
#define y_pos(w)    ((w)->win_pos.ypos)
#define	zoom(w)		((w)->zoom)
/*
 * ======== vpbe_set_display_default ========
 */
/* This function configures all the display registers in default state.*/
void vpbe_set_display_default(void)
{
	dispc_reg_out(VENC_VMOD, 0);
	dispc_reg_out(VENC_CVBS, 0);
	dispc_reg_out(VENC_LCDOUT, 0);
	dispc_reg_out(VENC_VIDCTL, 0x141);
	dispc_reg_out(VENC_DCLKCTL, 0);
	dispc_reg_out(VENC_DCLKPTN0, 0);
	dispc_reg_out(VENC_SYNCCTL, 0);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);
	dispc_reg_out(VPSS_CLK_CTRL, 0);
	dispc_reg_out(VENC_HSPLS, 0);
	dispc_reg_out(VENC_HSTART, 0);
	dispc_reg_out(VENC_HVALID, 0);
	dispc_reg_out(VENC_HINT, 0);
	dispc_reg_out(VENC_VSPLS, 0);
	dispc_reg_out(VENC_VSTART, 0);
	dispc_reg_out(VENC_VVALID, 0);
	dispc_reg_out(VENC_VINT, 0);
	dispc_reg_out(VENC_YCCCTL, 0);
	dispc_reg_out(VENC_DACTST, 0xF000);
	dispc_reg_out(VENC_VDPRO, 0);
}

/* 
 * function to check whether the OSD is YUV422 or RGB565 
 */
int is_rgb565(struct fb_var_screeninfo var)
{
	if( (var.red.length == 5)
	&&  (var.green.length == 6)
	&&  (var.blue.length == 5) )
		return 1;
	else
		return 0;
}

/* Must do checks against the limits of the output device */
static int
dm355fb_venc_check_mode(const vpbe_dm_win_info_t * w,
			  const struct fb_var_screeninfo *var)
{
	return 0;
}
static void set_sdram_params(char *id, u32 addr, u32 line_length);
static irqreturn_t dm355fb_isr(int irq, void *arg, struct pt_regs *regs)
{
	struct vpbe_dm_info *dm = (struct vpbe_dm_info *)arg;
	unsigned long addr = 0;

	if ((dispc_reg_in(VENC_VSTAT) & 0x00000010) == 0x10) {
		xchg(&addr, dm->osd0->sdram_address);
		if (addr) {
			set_sdram_params(dm->osd0->info.fix.id,
					 dm->osd0->sdram_address,
					 dm->osd0->info.fix.line_length);
			dm->osd0->sdram_address = 0;
		}
		addr = 0;
		xchg(&addr, dm->osd1->sdram_address);
		if (addr) {
			set_sdram_params(dm->osd1->info.fix.id,
					 dm->osd1->sdram_address,
					 dm->osd1->info.fix.line_length);
			dm->osd1->sdram_address = 0;
		}
		addr = 0;
		xchg(&addr, dm->vid0->sdram_address);
		if (addr) {
			set_sdram_params(dm->vid0->info.fix.id,
					 dm->vid0->sdram_address,
					 dm->vid0->info.fix.line_length);
			dm->vid0->sdram_address = 0;
		}
		addr = 0;
		xchg(&addr, dm->vid1->sdram_address);
		if (addr) {
			set_sdram_params(dm->vid1->info.fix.id,
					 dm->vid1->sdram_address,
					 dm->vid1->info.fix.line_length);
			dm->vid1->sdram_address = 0;
		}
		return IRQ_HANDLED;
	} else {
		++dm->vsync_cnt;
		wake_up_interruptible(&dm->vsync_wait);
		return IRQ_HANDLED;
	}
	return IRQ_HANDLED;
}

/* Wait for a vsync interrupt.  This routine sleeps so it can only be called
 * from process context.
 */
static int dm355fb_wait_for_vsync(vpbe_dm_win_info_t * w)
{
	vpbe_dm_info_t *dm = w->dm;
	wait_queue_t wq;
	unsigned long cnt;
	int ret;

	init_waitqueue_entry(&wq, current);

	cnt = dm->vsync_cnt;
	ret = wait_event_interruptible_timeout(dm->vsync_wait,
					       cnt != dm->vsync_cnt,
					       dm->timeout);
	if (ret < 0) {
		dev_dbg(dm->dev, "Exited function %s ret=%d\n", __FUNCTION__, ret);
		return ret;
	}
	if (ret == 0) {
		dev_dbg(dm->dev, "Exited function %s ret=%d\n", __FUNCTION__, ret);
		return -ETIMEDOUT;
	}

	return 0;
}

/* Sets a uniform attribute value over a rectangular area on the attribute
 * window. The attribute value (0 to 7) is passed through the fb_fillrect's
 * color parameter.
 */
static int dm355fb_set_attr_blend(struct fb_fillrect *r)
{
	struct fb_info *info = &dm->osd1->info;
	struct fb_var_screeninfo *var = &dm->osd1->info.var;
	unsigned long start = 0;
	u8 blend;
	u32 width_bytes;

	if (r->dx + r->width > var->xres_virtual)
		return -EINVAL;
	if (r->dy + r->height > var->yres_virtual)
		return -EINVAL;
	if (r->color < 0 || r->color > 7)
		return -EINVAL;

	/* since bits_per_pixel = 4, this will truncate the width if it is 
	 * not even. Similarly r->dx will be rounded down to an even pixel.
	 * ... Do we want to return an error otherwise?
	 */
	width_bytes = r->width * var->bits_per_pixel / 8;
	start = dm->osd1->fb_base + r->dy * info->fix.line_length
	    + r->dx * var->bits_per_pixel / 8;

	blend = (((u8) r->color & 0xf) << 4) | ((u8) r->color);
	while (r->height--) {
		start += info->fix.line_length;
		memset((void *)start, blend, width_bytes);
	}

	return 0;
}

/* These position parameters are given through fb_var_screeninfo.
 * xp = var.reserved[0], yp = var.reserved[1],
 * xl = var.xres, yl = var.yres
 */
void set_win_position(char *id, u32 xp, u32 yp, u32 xl, u32 yl)
{
	int i = 0;

	if (is_win(id, VID0)) {
		i = 0;
	} else if (is_win(id, VID1)) {
		i = 1;
	} else if (is_win(id, OSD0)) {
		i = 2;
	} else if (is_win(id, OSD1)) {
		i = 3;
	}

	dispc_reg_out(OSD_WINXP(i), xp);
	dispc_reg_out(OSD_WINYP(i), yp);
	dispc_reg_out(OSD_WINXL(i), xl);
	dispc_reg_out(OSD_WINYL(i), yl);
}

static inline void
get_win_position(vpbe_dm_win_info_t * w, u32 * xp, u32 * yp, u32 * xl, u32 * yl)
{
	struct fb_var_screeninfo *v = &w->info.var;

	*xp = x_pos(w);
	*yp = y_pos(w);
	*xl = v->xres;
	*yl = v->yres;
}

/*
 * ======== dm355fb_check_var ========
 */
/**
 *      dm355fb_check_var - Validates a var passed in.
 *      @var: frame buffer variable screen structure
 *      @info: frame buffer structure that represents a single frame buffer
 *
 *	Checks to see if the hardware supports the state requested by
 *	var passed in. This function does not alter the hardware state!!!
 *	This means the data stored in struct fb_info and struct xxx_par do
 *      not change. This includes the var inside of struct fb_info.
 *	Do NOT change these. This function can be called on its own if we
 *	intent to only test a mode and not actually set it.
 *	If the var passed in is slightly off by what the hardware can support
 *	then we alter the var PASSED in to what we can do.
 *
 *	Returns negative errno on error, or zero on success.
 */
static int
dm355fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	vpbe_dm_win_info_t *w = (vpbe_dm_win_info_t *) info->par;
	struct fb_var_screeninfo v;

	memcpy(&v, var, sizeof(v));

	/* do board-specific checks on the var */
	if (dm355fb_venc_check_mode(w, &v)){
		dev_err(dm->dev, "Exited function %s %d \n",__FUNCTION__, __LINE__);
		return -EINVAL;
	}
	if (v.xres_virtual < v.xres || DISP_MEMY < v.yres) {
                dev_err(dm->dev, "Exited function %s \n", __FUNCTION__);
                return -EINVAL;
        }
	memcpy(var, &v, sizeof(v));
	return 0;
}

/* Interlaced = Frame mode, Non-interlaced = Field mode */
void set_interlaced(char *id, unsigned int on)
{
	if (is_win(id, VID0))
		dispc_reg_merge(OSD_VIDWINMD, on ? OSD_VIDWINMD_VFF0 : 0,
			OSD_VIDWINMD_VFF0);
	else if (is_win(id, VID1))
		dispc_reg_merge(OSD_VIDWINMD, on ? OSD_VIDWINMD_VFF1 : 0,
			OSD_VIDWINMD_VFF1);
	else if (is_win(id, OSD0))
		dispc_reg_merge(OSD_OSDWIN0MD, on ? OSD_OSDWIN0MD_OFF0 : 0,
			OSD_OSDWIN0MD_OFF0);
	else if (is_win(id, OSD1))
		dispc_reg_merge(OSD_OSDWIN1MD, on ? OSD_OSDWIN1MD_OFF1 : 0,
			OSD_OSDWIN1MD_OFF1);
}

/* For zooming, we just have to set the start of framebuffer, the zoom factors 
 * and the display size. The hardware will then read only 
 * (display size / zoom factor) area of the framebuffer and  zoom and 
 * display it. In the following function, we assume that the start of 
 * framebuffer and the display size parameters are set already.
 */
static void set_zoom(int WinID, int h_factor, int v_factor)
{
	switch (WinID) {
	case VID0:
		dispc_reg_merge(OSD_VIDWINMD,
				h_factor << OSD_VIDWINMD_VHZ0_SHIFT,
				OSD_VIDWINMD_VHZ0);
		dispc_reg_merge(OSD_VIDWINMD,
				v_factor << OSD_VIDWINMD_VVZ0_SHIFT,
				OSD_VIDWINMD_VVZ0);
		break;
	case VID1:
		dispc_reg_merge(OSD_VIDWINMD,
				h_factor << OSD_VIDWINMD_VHZ1_SHIFT,
				OSD_VIDWINMD_VHZ1);
		dispc_reg_merge(OSD_VIDWINMD,
				v_factor << OSD_VIDWINMD_VVZ1_SHIFT,
				OSD_VIDWINMD_VVZ1);
		break;
	case OSD0:
		dispc_reg_merge(OSD_OSDWIN0MD,
				h_factor << OSD_OSDWIN0MD_OHZ0_SHIFT,
				OSD_OSDWIN0MD_OHZ0);
		dispc_reg_merge(OSD_OSDWIN0MD,
				v_factor << OSD_OSDWIN0MD_OVZ0_SHIFT,
				OSD_OSDWIN0MD_OVZ0);
		break;
	case OSD1:
		dispc_reg_merge(OSD_OSDWIN1MD,
				h_factor << OSD_OSDWIN1MD_OHZ1_SHIFT,
				OSD_OSDWIN1MD_OHZ1);
		dispc_reg_merge(OSD_OSDWIN1MD,
				v_factor << OSD_OSDWIN1MD_OVZ1_SHIFT,
				OSD_OSDWIN1MD_OVZ1);
		break;
	}
}

static void set_sdram_params(char *id, u32 addr, u32 line_length)
{
	/* The parameters to be written to the registers should be in 
	 * multiple of 32 bytes
	 */
	unsigned int osd_vidwinadh = 0;
	addr = addr;		/* div by 32 */
	addr = addr - 0x80000000; /* Subtract SDRAM base */
	addr = addr >> 5;
	line_length = line_length / 32;
	if (is_win(id, VID0)) {
		osd_vidwinadh =	dispc_reg_in(OSD_VIDWINADH);
		osd_vidwinadh &= 0xFFFFFF80;
		osd_vidwinadh |= (addr & 0x007f0000)>>16;
		dispc_reg_out(OSD_VIDWINADH, osd_vidwinadh);
		dispc_reg_out(OSD_VIDWIN0ADL, (addr & 0x0000FFFF) );
		dispc_reg_out(OSD_VIDWIN0OFST, line_length);
	} else if (is_win(id, VID1)) {
		osd_vidwinadh =	dispc_reg_in(OSD_VIDWINADH);
		osd_vidwinadh &= 0xFFFF80FF;
		osd_vidwinadh |= (addr & 0x007f0000)>>8;
		dispc_reg_out(OSD_VIDWINADH, osd_vidwinadh);
		dispc_reg_out(OSD_VIDWIN1ADL, (addr & 0x0000FFFF) );
		dispc_reg_out(OSD_VIDWIN1OFST, line_length);
	} else if (is_win(id, OSD0)) {
		osd_vidwinadh =	dispc_reg_in(OSD_OSDWINADH);
		osd_vidwinadh &= 0xFFFFFF80;
		osd_vidwinadh |= (addr & 0x007f0000)>>16;
		dispc_reg_out(OSD_OSDWINADH, osd_vidwinadh);
		dispc_reg_out(OSD_OSDWIN0ADL, (addr & 0x0000FFFF) );
		dispc_reg_out(OSD_OSDWIN0OFST, line_length);
	} else if (is_win(id, OSD1)) {
		osd_vidwinadh =	dispc_reg_in(OSD_OSDWINADH);
		osd_vidwinadh &= 0xFFFF80FF;
		osd_vidwinadh |= (addr & 0x007f0000)>>8;
		dispc_reg_out(OSD_OSDWINADH, osd_vidwinadh);
		dispc_reg_out(OSD_OSDWIN1ADL, (addr & 0x0000FFFF) );
		dispc_reg_out(OSD_OSDWIN1OFST, line_length);
	}
}

void set_win_enable(char *id, unsigned int on)
{
	if (is_win(id, VID0))
		dispc_reg_merge(OSD_VIDWINMD, on ? OSD_VIDWINMD_ACT0 : 0,
			OSD_VIDWINMD_ACT0);
	else if (is_win(id, VID1))
		dispc_reg_merge(OSD_VIDWINMD, on ? OSD_VIDWINMD_ACT1 : 0,
			OSD_VIDWINMD_ACT1);
	else if (is_win(id, OSD0))
		dispc_reg_merge(OSD_OSDWIN0MD, on ? OSD_OSDWIN0MD_OACT0 : 0,
			OSD_OSDWIN0MD_OACT0);
	else if (is_win(id, OSD1)) {
		/* The OACT1 bit is applicable only if OSD1 is not used as
		 * the attribute window
		 */
		if (!(dispc_reg_in(OSD_OSDWIN1MD) & OSD_OSDWIN1MD_OASW))
			dispc_reg_merge(OSD_OSDWIN1MD,
				on ? OSD_OSDWIN1MD_OACT1 : 0,
				OSD_OSDWIN1MD_OACT1);
	}
}

static void set_win_mode(char *id)
{
	if ((is_win(id, OSD0))) {
		switch(dm->osd0->info.var.bits_per_pixel)
		{
			case BITMAP_1BIT:
			dispc_reg_merge(OSD_OSDWIN0MD,
					0 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			dispc_reg_merge(OSD_OSDWIN0MD,
					0 << OSD_OSDWIN0MD_BMW0_SHIFT,
					OSD_OSDWIN0MD_BMW0);
			break;		

			case BITMAP_2BIT:
			dispc_reg_merge(OSD_OSDWIN0MD,
					0 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			dispc_reg_merge(OSD_OSDWIN0MD,
					1 << OSD_OSDWIN0MD_BMW0_SHIFT,
					OSD_OSDWIN0MD_BMW0);
			break;

			case BITMAP_4BIT:
			dispc_reg_merge(OSD_OSDWIN0MD,
					0 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			dispc_reg_merge(OSD_OSDWIN0MD,
					2 << OSD_OSDWIN0MD_BMW0_SHIFT,
					OSD_OSDWIN0MD_BMW0);
			break;		

			case BITMAP_8BIT:
			dispc_reg_merge(OSD_OSDWIN0MD,
					0 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			dispc_reg_merge(OSD_OSDWIN0MD,
					3 << OSD_OSDWIN0MD_BMW0_SHIFT,
					OSD_OSDWIN0MD_BMW0);
			break;		

			case YUV422_RGB565:
				if(is_rgb565(dm->osd0->info.var))
					dispc_reg_merge(OSD_OSDWIN0MD,
					1 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
				else /* YUV422 */
					dispc_reg_merge(OSD_OSDWIN0MD,
					3 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			break;		

			case RGB888:
			dispc_reg_merge(OSD_OSDWIN0MD,
					2 << OSD_OSDWIN0MD_BMP0MD_SHIFT,
					OSD_OSDWIN0MD_BMP0MD);
			break;	

			default:
				dev_err(dm->dev,"It should not come here!!\n");
				while(1);
			break;	
		}
		dispc_reg_merge(OSD_OSDWIN0MD,
		(dm->osd0->conf_params.bitmap_params.blend_info.bf) << OSD_OSDWIN0MD_BLND0_SHIFT,
		OSD_OSDWIN0MD_BLND0);
	}
	if ((is_win(id, OSD1))) {
		if (dm->osd1->conf_params.bitmap_params.enable_attribute) {
			dispc_reg_merge(OSD_OSDWIN1MD, OSD_OSDWIN1MD_OASW,
					OSD_OSDWIN1MD_OASW);
			dispc_reg_merge(OSD_OSDWIN1MD,
					2 << OSD_OSDWIN1MD_BMW1_SHIFT,
					OSD_OSDWIN1MD_BMW1);
			dispc_reg_merge(OSD_OSDWIN1MD, 0x00,
					OSD_OSDWIN1MD_OACT1);
		} else {
			dispc_reg_merge(OSD_OSDWIN1MD, 0x00,
					OSD_OSDWIN1MD_OASW);
		switch(dm->osd1->info.var.bits_per_pixel)
		{
			case BITMAP_1BIT:
			dispc_reg_merge(OSD_OSDWIN1MD,
					0 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			dispc_reg_merge(OSD_OSDWIN1MD,
					0 << OSD_OSDWIN1MD_BMW1_SHIFT,
					OSD_OSDWIN1MD_BMW1);
			break;		

			case BITMAP_2BIT:
			dispc_reg_merge(OSD_OSDWIN1MD,
					0 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			dispc_reg_merge(OSD_OSDWIN1MD,
					1 << OSD_OSDWIN1MD_BMW1_SHIFT,
					OSD_OSDWIN1MD_BMW1);
			break;

			case BITMAP_4BIT:
			dispc_reg_merge(OSD_OSDWIN1MD,
					0 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			dispc_reg_merge(OSD_OSDWIN1MD,
					2 << OSD_OSDWIN1MD_BMW1_SHIFT,
					OSD_OSDWIN1MD_BMW1);
			break;		

			case BITMAP_8BIT:
			dispc_reg_merge(OSD_OSDWIN1MD,
					0 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			dispc_reg_merge(OSD_OSDWIN1MD,
					3 << OSD_OSDWIN1MD_BMW1_SHIFT,
					OSD_OSDWIN1MD_BMW1);
			break;		

			case YUV422_RGB565:
				if(is_rgb565(dm->osd1->info.var))
					dispc_reg_merge(OSD_OSDWIN1MD,
					1 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
				else /* YUV422 */
					dispc_reg_merge(OSD_OSDWIN1MD,
					3 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			break;

			case RGB888:
			dispc_reg_merge(OSD_OSDWIN1MD,
					2 << OSD_OSDWIN1MD_BMP1MD_SHIFT,
					OSD_OSDWIN1MD_BMP1MD);
			break;	

			default:
				dev_err(dm->dev,"It should not come here!!\n");
				while(1);
			break;	
		}
			dispc_reg_merge(OSD_OSDWIN1MD,
					(dm->osd1->conf_params.
					 bitmap_params.blend_info.
					 bf) << OSD_OSDWIN1MD_BLND1_SHIFT,
					OSD_OSDWIN1MD_BLND1);
			dispc_reg_merge(OSD_OSDWIN1MD, OSD_OSDWIN1MD_OACT1,
					OSD_OSDWIN1MD_OACT1);
		}
	}
}

static int dm355fb_set_par(struct fb_info *info)
{
	struct vpbe_dm_win_info *w = (struct vpbe_dm_win_info *)info->par;
	struct fb_var_screeninfo *v = &info->var;
	u32 start = 0, offset = 0;
	memset(info->pseudo_palette, 0x0, sizeof(int) * 17);

	/* update the fix screeninfo */
	info->fix.line_length = (v->xres_virtual*v->bits_per_pixel)/8;

	/* First set everything in 'dm' */
	if (is_win(info->fix.id, VID0)) {
		dm->vid0->info = *info;
	}
	if (is_win(info->fix.id, VID1)) {
		dm->vid1->info = *info;
	}
	if (is_win(info->fix.id, OSD0)) {
		dm->osd0->info = *info;
	}
	if (is_win(info->fix.id, OSD1)) {
		dm->osd1->info = *info;
	}

	offset = v->yoffset * info->fix.line_length +
	    v->xoffset * v->bits_per_pixel / 8;
	start = (u32) w->fb_base_phys + offset;
	set_sdram_params(info->fix.id, start, info->fix.line_length);

	set_interlaced(info->fix.id, dm->videomode.vmode);

	if (dm->videomode.vmode == FB_VMODE_INTERLACED) {	/* Interlaced */
		set_win_position(info->fix.id, x_pos(w), y_pos(w) / 2,
				 v->xres, v->yres / 2);
	} else {		/* Progressive */
		set_win_position(info->fix.id, x_pos(w), y_pos(w),
				v->xres, v->yres);
	}
	set_win_mode(info->fix.id);
	return 0;
}

/*
 * dm355fb_ioctl - handler for private ioctls.
 */
static int
dm355fb_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
		unsigned long arg, struct fb_info *info)
{
	struct vpbe_dm_win_info *w = (struct vpbe_dm_win_info *)info->par;
	void __user *argp = (void __user *)arg;
	struct fb_fillrect rect;
	zoom_params_t zoom;
	int retval = 0;
	unsigned int tv_mode = 0;
	char *win_id = w->info.fix.id;
	vpbe_bitmap_blend_params_t blend_para;
	vpbe_blink_option_t blink_option;
	vpbe_video_config_params_t vid_conf_params;
	vpbe_bitmap_config_params_t bitmap_conf_params;
	vpbe_dclk_t dclk;
	vpbe_backg_color_t backg_color;
	struct vpbe_mode_info mode_info;
	struct vpbe_fb_videomode vmode;
	struct vpbe_window_position win_pos;
	struct fb_cursor cursor;
	struct reg_dump regDump;
	u_int32_t std, position;

	if (!w->fb_base) {
		dev_err
		    (dm->dev, "ERROR: window is not enabled from command line.\n");
		return -EINVAL;
	}

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
		/* This ioctl   accepts an integer argument     to specify a
		 * display.      We     only support one display,       so we   will
		 * simply       ignore the argument.
		 */
		return dm355fb_wait_for_vsync(w);
		break;
	case FBIO_SETATTRIBUTE:
		if (copy_from_user(&rect, argp, sizeof(rect)))
			return -EFAULT;
		return dm355fb_set_attr_blend(&rect);
		break;
	case FBIO_SETPOS:
		if (copy_from_user(&win_pos, argp, sizeof(win_pos)))
			return -EFAULT;
		if (win_pos.xpos >= 0 && win_pos.xpos <= DISP_XRES) {
			w->win_pos.xpos = win_pos.xpos;
			if (dm355fb_set_par(&w->info) != 0)
				return -EFAULT;
		} else
			return -EINVAL;
		if (win_pos.ypos >= 0 && win_pos.ypos <= DISP_YRES) {
			w->win_pos.ypos = win_pos.ypos;
			if (dm355fb_set_par(&w->info) != 0)
				return -EFAULT;
		} else
			return -EINVAL;
		break;
	case FBIO_SETZOOM:
		if (copy_from_user(&zoom, argp, sizeof(zoom)))
			return -EFAULT;
		if (((zoom.zoom_h == 2) ||
		     (zoom.zoom_h == 0) ||
		     (zoom.zoom_h == 1)) &&
		    ((zoom.zoom_v == 2) || (zoom.zoom_v == 0)
		     || (zoom.zoom_v == 1))) {
			set_zoom(zoom.window_id, zoom.zoom_h, zoom.zoom_v);
			return 0;
		} else {
			return -EINVAL;
		}
		break;
	case FBIO_ENABLE_DISABLE_WIN:
		switch (arg) {
		case 1:
			retval = vpbe_enable_window(w);
			break;
		case 0:
			retval = vpbe_disable_window(w);
			break;
		default:
			retval = -VPBE_INVALID_PARA_VALUE;
			break;
		}
		break;
	case FBIO_SET_TV_SYSTEM:

		if ((retval =
		     copy_from_user(&tv_mode, argp, sizeof(tv_mode))) < 0)
			return retval;


		dm355fb_pal_ntsc_composite_switch(tv_mode);

		retval = 0;

		break;

	case FBIO_SET_BITMAP_BLEND_FACTOR:
		if ((retval =
		     copy_from_user(&blend_para, argp, sizeof(blend_para))) < 0)
			return retval;
		if ((retval =
		     vpbe_bitmap_set_blend_factor(win_id, &blend_para)) < 0)
			return retval;
		break;

	case FBIO_SET_BITMAP_WIN_RAM_CLUT:
		if ((retval =
		     copy_from_user(dm->ram_clut, argp, RAM_CLUT_SIZE)) < 0)
			return retval;
		vpbe_bitmap_set_ram_clut();
		break;

	case FBIO_ENABLE_DISABLE_ATTRIBUTE_WIN:
		if ((retval = vpbe_enable_disable_attribute_window(arg)) < 0)
			return retval;
		break;

	case FBIO_GET_BLINK_INTERVAL:
		if ((retval = vpbe_get_blinking(win_id, &blink_option)) < 0)
			return retval;
		if ((retval =
		     copy_to_user(argp, &blink_option,
				  sizeof(blink_option))) < 0)
			return retval;
		break;

	case FBIO_SET_BLINK_INTERVAL:
		if ((retval =
		     copy_from_user(&blink_option, argp,
				    sizeof(blink_option))) < 0)
			return retval;
		if ((retval = vpbe_set_blinking(win_id, &blink_option)) < 0)
			return retval;
		break;

	case FBIO_GET_VIDEO_CONFIG_PARAMS:
		if ((retval =
		     vpbe_get_vid_params(win_id, &vid_conf_params)) < 0)
			return retval;
		if ((retval =
		     copy_to_user(argp, &vid_conf_params,
				  sizeof(vid_conf_params))) < 0)
			return retval;
		break;

	case FBIO_SET_VIDEO_CONFIG_PARAMS:
		if ((retval =
		     copy_from_user(&vid_conf_params, argp,
				    sizeof(vid_conf_params))) < 0)
			return retval;
		if ((retval =
		     vpbe_set_vid_params(win_id, &vid_conf_params)) < 0)
			return retval;
		break;

	case FBIO_GET_BITMAP_CONFIG_PARAMS:
		if ((retval =
		     vpbe_bitmap_get_params(win_id, &bitmap_conf_params)) < 0)
			return retval;
		if ((retval =
		     copy_to_user(argp, &bitmap_conf_params,
				  sizeof(bitmap_conf_params))) < 0)
			return retval;
		break;

	case FBIO_SET_BITMAP_CONFIG_PARAMS:
		if ((retval =
		     copy_from_user(&bitmap_conf_params, argp,
				    sizeof(bitmap_conf_params))) < 0)
			return retval;
		if ((retval =
		     vpbe_bitmap_set_params(win_id, &bitmap_conf_params)) < 0)
			return retval;
		break;

	case FBIO_SET_DCLK:
		if (!is_win(win_id, VID0))
			return -EINVAL;
		if ((retval = copy_from_user(&dclk, argp, sizeof(dclk))) < 0)
			return retval;
		if ((retval = vpbe_set_dclk(&dclk)) < 0)
			return retval;
		break;

	case FBIO_SET_INTERFACE:
		flag_osd0 = flag_vid0 = flag_osd1 = flag_vid1 = 0;
		if ((retval = vpbe_set_interface(arg)) < 0)
			return retval;
		break;

	case FBIO_GET_INTERFACE:
		if ((retval =
		     copy_to_user(argp, &dm->display.interface,
				  sizeof(dm->display.interface))) < 0)
			return retval;
		break;

	case FBIO_QUERY_TIMING:
		if ((retval =
		     copy_from_user(&mode_info, argp, sizeof(mode_info))) < 0)
			return retval;
		if ((retval = vpbe_query_mode(&mode_info)) < 0)
			return retval;
		if ((retval =
		     copy_to_user(argp, &mode_info, sizeof(mode_info))) < 0)
			return retval;
		break;

	case FBIO_SET_TIMING:
		if ((retval = copy_from_user(&vmode, argp, sizeof(vmode))) < 0)
			return retval;
		flag_osd0 = flag_vid0 = flag_osd1 = flag_vid1 = 0;
		if ((retval = vpbe_set_mode(&vmode)) < 0)
			return retval;
		break;

	case FBIO_GET_TIMING:
		if ((retval =
		     copy_to_user(argp, &dm->videomode,
				  sizeof(dm->videomode))) < 0)
			return retval;
		break;

	case FBIO_SET_VENC_CLK_SOURCE:
		if (!is_win(win_id, VID0))
			return -EINVAL;
		if ((retval = vpbe_set_venc_clk_source(arg)) < 0)
			return retval;
		break;

	case FBIO_SET_BACKG_COLOR:
		if ((retval =
		     copy_from_user(&backg_color, argp,
				    sizeof(backg_color))) < 0)
			return retval;
		if ((retval = vpbe_set_backg_color(&backg_color)) < 0)
			return retval;
		break;

	case FBIO_ENABLE_DISPLAY:
		if (arg) 
			dm->display_enable = 1;
		else
			dm->display_enable = 0;
       		/* While enabling, enable VENC before DACs
                   while disabling, disable DACS before VENC
                   to minimize corruption */
                if (arg)
                        vpbe_enable_venc(arg);

                if ((dm->display.interface == YCC8)
			|| (dm->display.interface == SRGB))
                        vpbe_enable_lcd(arg);
                else if (dm->display.interface == COMPOSITE)
                        vpbe_enable_dacs(arg);

                if (!arg)
                        vpbe_enable_venc(arg);

		break;

	case FBIO_SET_CURSOR:
		if (copy_from_user(&cursor, argp, sizeof(cursor)))
			return -EFAULT;
		if (vpbe_set_cursor_params(&cursor) < 0)
			return -EFAULT;
		break;
		/*backported IOCTLS */
	case FBIO_GETSTD:
		std = ((dm->display.mode << 16) | (dm->display.interface));
		/*(NTSC <<16) | (COPOSITE); */
		copy_to_user(argp, &std, sizeof(u_int32_t));
		return 0;
		break;

	case FBIO_SETPOSX:
                if (copy_from_user(&position, argp, sizeof(u_int32_t)))
                        return -EFAULT;
                if (position >= 0 && position <= DISP_XRES) {
                        w->win_pos.xpos = position;
                        if (dm355fb_set_par(&w->info) != 0)
                                return -EFAULT;
                        return 0;
                } else
                        return -EINVAL;
                break;

	case FBIO_SETPOSY:
                if (copy_from_user(&position, argp, sizeof(u_int32_t)))
                        return -EFAULT;
                if (position >= 0 && position <= DISP_YRES) {

                        w->win_pos.ypos = position;
                        if (dm355fb_set_par(&w->info) != 0)
                                return -EFAULT;
                        return 0;
                } else
                        return -EINVAL;
                break;

	case FBIO_REG_DUMP:
		if (copy_from_user(&regDump, argp, sizeof(regDump)))
			return -EFAULT;
		if(regDump.val == 0x1234)
			regDump.val = __REG(regDump.addr);
		else
			__REG(regDump.addr) = regDump.val;
		if ((retval =
		     copy_to_user(argp, &regDump, sizeof(regDump))) < 0)
			return retval;
		break;

	default:
		retval = -EINVAL;
		break;
	}
	return retval;
}

/**
 *  	dm355fb_setcolreg - Optional function. Sets a color register.
 *      @regno: Which register in the CLUT we are programming 
 *      @red: The red value which can be up to 16 bits wide 
 *	@green: The green value which can be up to 16 bits wide 
 *	@blue:  The blue value which can be up to 16 bits wide.
 *	@transp: If supported the alpha value which can be up to 16 bits wide.
 *      @info: frame buffer info structure
 * 
 *  	Set a single color register. The values supplied have a 16 bit
 *  	magnitude which needs to be scaled in this function for the hardware.
 *	Things to take into consideration are how many color registers, if
 *	any, are supported with the current color visual. With truecolor mode
 *	no color palettes are supported. Here a psuedo palette is created 
 *	which we store the value in pseudo_palette in struct fb_info. For
 *	pseudocolor mode we have a limited color palette. To deal with this
 *	we can program what color is displayed for a particular pixel value.
 *	DirectColor is similar in that we can program each color field. If
 *	we have a static colormap we don't need to implement this function. 
 * 
 *	Returns negative errno on error, or zero on success.
 */
static int
dm355fb_setcolreg(unsigned regno, unsigned red, unsigned green,
		    unsigned blue, unsigned transp, struct fb_info *info)
{
	/* only pseudo-palette (16 bpp) allowed */
	if (regno >= 16)	/* maximum number of palette entries */
		return 1;

	if (info->var.grayscale) {
		/* grayscale = 0.30*R + 0.59*G + 0.11*B */
		red = green = blue = (red * 77 + green * 151 + blue * 28) >> 8;
	}

	/* Truecolor has hardware-independent 16-entry pseudo-palette */
	if (info->fix.visual == FB_VISUAL_TRUECOLOR) {
		u32 v;

		if (regno >= 16)
			return 1;

		red >>= (16 - info->var.red.length);
		green >>= (16 - info->var.green.length);
		blue >>= (16 - info->var.blue.length);

		v = (red << info->var.red.offset) |
		    (green << info->var.green.offset) | (blue << info->var.
							 blue.offset);

		switch (info->var.bits_per_pixel) {
		case YUV422_RGB565:
			if(!is_rgb565(info->var))
				((u16 *) (info->pseudo_palette))[regno] = v;
			break;
		default:
			return 1;
		}
		return 0;
	}
	return 0;
}

/**
 *      dm355fb_pan_display - NOT a required function. Pans the display.
 *      @var: frame buffer variable screen structure
 *      @info: frame buffer structure that represents a single frame buffer
 *
 *	Pan (or wrap, depending on the `vmode' field) the display using the
 *  	`xoffset' and `yoffset' fields of the `var' structure.
 *  	If the values don't fit, return -EINVAL.
 *
 *      Returns negative errno on error, or zero on success.
 */
static int
dm355fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct vpbe_dm_win_info *w = (struct vpbe_dm_win_info *)info->par;
	u32 start = 0, offset = 0;

	if (var->xoffset > var->xres_virtual - var->xres) {
		dev_err(dm->dev, "Return: %s %d \n", __FUNCTION__, __LINE__);
		return -EINVAL;
	}
	if (var->yoffset > var->yres_virtual - var->yres) {
		dev_err(dm->dev, "Return: %s %d \n", __FUNCTION__, __LINE__);
		return -EINVAL;
	}
	offset = var->yoffset * info->fix.line_length +
	    var->xoffset * var->bits_per_pixel / 8;
	start = (u32) w->fb_base_phys + offset;
	if (dm->videomode.vmode == FB_VMODE_NONINTERLACED)
		set_sdram_params(info->fix.id, start, info->fix.line_length);
	else {
		if ((dispc_reg_in(VENC_VSTAT) & 0x00000010) == 0x10)
			set_sdram_params(info->fix.id, start,
					 info->fix.line_length);
		else
			w->sdram_address = start;
	}
	return 0;
}

/**
 *      dm355fb_blank - NOT a required function. Blanks the display.
 *      @blank_mode: the blank mode we want. 
 *      @info: frame buffer structure that represents a single frame buffer
 *
 *      Blank the screen if blank_mode != 0, else unblank. Return 0 if
 *      blanking succeeded, != 0 if un-/blanking failed due to e.g. a 
 *      video mode which doesn't support it. Implements VESA suspend
 *      and powerdown modes on hardware that supports disabling hsync/vsync:
 *      blank_mode == 2: suspend vsync
 *      blank_mode == 3: suspend hsync
 *      blank_mode == 4: powerdown
 *
 *      Returns negative errno on error, or zero on success.
 *
 */
int dm355fb_blank(int blank_mode, struct fb_info *info)
{
	/* ... */
	return 0;
}

int parse_win_params(char *wp, int *xres, int *yres, int *xpos, int *ypos)
{
	char *s;

	if ((s = strsep(&wp, "x")) == NULL)
		return -EINVAL;
	*xres = simple_strtoul(s, NULL, 0);

	if ((s = strsep(&wp, "@")) == NULL)
		return -EINVAL;
	*yres = simple_strtoul(s, NULL, 0);

	if ((s = strsep(&wp, ",")) == NULL)
		return -EINVAL;
	*xpos = simple_strtoul(s, NULL, 0);

	if ((s = strsep(&wp, ":")) == NULL)
		return -EINVAL;
	*ypos = simple_strtoul(s, NULL, 0);

	return 0;
}

struct fb_info *init_fb_info(struct vpbe_dm_win_info *w,
			     struct fb_var_screeninfo *var, char *id)
{
	struct fb_info *info = &(w->info);
	vpbe_dm_info_t *dm = w->dm;

	/* initialize the fb_info structure */
	info->flags = FBINFO_DEFAULT;
	info->fbops = &dm355fb_ops;
	info->screen_base = (char *)(w->fb_base);
	info->pseudo_palette = w->pseudo_palette;
	info->par = w;

	/* Initialize variable screeninfo.
	 * The variable screeninfo can be directly specified by the user
	 * via an ioctl.
	 */
	memcpy(&info->var, var, sizeof(info->var));
	info->var.activate = FB_ACTIVATE_NOW;

	/* Initialize fixed screeninfo.
	 * The fixed screeninfo cannot be directly specified by the user, but
	 * it may change to reflect changes to the var info.
	 */
	strlcpy(info->fix.id, id, sizeof(info->fix.id));
	info->fix.smem_start = w->fb_base_phys;
	info->fix.smem_len = w->fb_size;
	info->fix.type = FB_TYPE_PACKED_PIXELS;
	info->fix.visual = (info->var.bits_per_pixel <= 8) ?
	    FB_VISUAL_PSEUDOCOLOR : FB_VISUAL_TRUECOLOR;
	info->fix.xpanstep = 0;
	info->fix.ypanstep = 1;
	info->fix.ywrapstep = 0;
	info->fix.type_aux = 0;
	info->fix.mmio_start = dm->mmio_base_phys;
	info->fix.mmio_len = dm->mmio_size;
	info->fix.accel = FB_ACCEL_NONE;
	info->fix.line_length = (info->var.xres_virtual*info->var.bits_per_pixel)/8;
	w->sdram_address = 0;

	return info;
}

/*
 * Pass boot-time options by adding the following string to the boot params:
 *      video=dm355fb:options
 * Valid options:
 *      output=[ntsc|pal]
 *      format=[composite|ycc8|srgb]
 *      vid0=[off|MxN@X,Y]
 *      vid1=[off|MxN@X,Y]
 *      osd0=[off|MxN@X,Y]
 *      osd1=[off|MxN@X,Y]
 *              MxN specify the window resolution (displayed size)
 *              X,Y specify the window position
 *              M, N, X, Y are integers
 *              M, X should be multiples of 16
 *
 *      For example
 *      video=dm355fb:format=composite:output=ntsc:vid0=720x480@0,0
 *      :osd0=off:osd1=off
 */

int dm355fb_setup(char *options)
{
	char *this_opt;
	u32 xres, yres, xpos, ypos;
	struct fb_info *info;
	int format_yres = 480;
	int flag = 0;

	/* Default settings for var_screeninfo and fix_screeninfo */
	info = init_fb_info(dm->vid0, &vid0_default_var, VID0_FBNAME);
	info = init_fb_info(dm->vid1, &vid1_default_var, VID1_FBNAME);
	info = init_fb_info(dm->osd0, &osd0_default_var, OSD0_FBNAME);
	info = init_fb_info(dm->osd1, &osd1_default_var, OSD1_FBNAME);
	/* Configure default settings for video/bitmap/cursor config params */
	set_vid0_default_conf();
	set_vid1_default_conf();
	set_osd0_default_conf();
	set_osd1_default_conf();
	set_cursor_default_conf();

	/* Default setting for backg/dclk/display_interface/display_mode */
	set_dm_default_conf();
	switch (dm->display.interface) {
	case SVIDEO:
	case COMPONENT:
	case YCC8:
	case SRGB:
	case COMPOSITE:{
			if ((dm->display.mode == NTSC) ||
			    (dm->display.mode == P525) ||
			    (dm->display.mode == LCD))
				format_yres = 480;
			else
				/*P625, PAL */
				format_yres = 576;
		}
		break;
	case PRGB:{
			if (dm->display.mode == P350)
				format_yres = 350;
			else if (dm->display.mode == P400)
				format_yres = 400;
			else
				format_yres = 480;
		}
		break;
	default:
		break;
	}

	dm->osd0->info.var.yres = osd0_default_var.yres = format_yres;
	dm->osd1->info.var.yres = osd1_default_var.yres = format_yres;
	dm->vid0->info.var.yres = vid0_default_var.yres = format_yres;
	dm->vid1->info.var.yres = vid1_default_var.yres = format_yres;

	dev_dbg(dm->dev, "dm355fb: Options \"%s\"\n", options);

	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ":")) != NULL) {

		if (!*this_opt)
			continue;

		if (!strncmp(this_opt, "output=", 7)) {
			flag = 1;
			if (!strncmp(this_opt + 7, "ntsc", 4))
				dm->display.mode = NTSC;
			else if (!strncmp(this_opt + 7, "pal", 3))
				dm->display.mode = PAL;
			else if (!strncmp(this_opt + 7, "525p", 4))
				dm->display.mode = P525;
			else if (!strncmp(this_opt + 7, "625p", 4))
				dm->display.mode = P625;
			else if (!strncmp(this_opt + 7, "480p", 4))
				dm->display.mode = P480;
			else if (!strncmp(this_opt + 7, "400p", 4))
				dm->display.mode = P400;
			else if (!strncmp(this_opt + 7, "350p", 4))
				dm->display.mode = P350;
			else
				dm->display.mode = NTSC;
		} else if (!strncmp(this_opt, "format=", 7)) {
			flag = 1;
			if (!strncmp(this_opt + 7, "s-video", 7))
				dm->display.interface = SVIDEO;
			else if (!strncmp(this_opt + 7, "component", 9))
				dm->display.interface = COMPONENT;
			else if (!strncmp(this_opt + 7, "composite", 9))
				dm->display.interface = COMPOSITE;
			else if (!strncmp(this_opt + 7, "rgb", 3))
				dm->display.interface = RGB;
			else if (!strncmp(this_opt + 7, "ycc16", 5))
				dm->display.interface = YCC16;
			else if (!strncmp(this_opt + 7, "ycc8", 4))
				dm->display.interface = YCC8;
			else if (!strncmp(this_opt + 7, "prgb", 4))
				dm->display.interface = PRGB;
			else if (!strncmp(this_opt + 7, "srgb", 4))
				dm->display.interface = SRGB;
			else if (!strncmp(this_opt + 7, "epson", 5))
				dm->display.interface = EPSON;
			else if (!strncmp(this_opt + 7, "casio1g", 7))
				dm->display.interface = CASIO1G;
			else if (!strncmp(this_opt + 7, "udisp", 5))
				dm->display.interface = UDISP;
			else if (!strncmp(this_opt + 7, "stn", 3))
				dm->display.interface = STN;
			else
				dm->display.interface = COMPOSITE;
		} else if (!strncmp(this_opt, "vid0=", 5)) {
			flag_vid0 = 1;
			if (!strncmp(this_opt + 5, "off", 3))
				dm->vid0->window_enable = VPBE_DISABLE;
			else if (!parse_win_params(this_opt + 5,
						   &xres, &yres, &xpos,
						   &ypos)) {
				dm->vid0->info.var.xres = xres;
				dm->vid0->info.var.yres = yres;
				dm->vid0->win_pos.xpos = xpos;
				dm->vid0->win_pos.ypos = ypos;
				dm->vid0->window_enable = VPBE_DISABLE;
				if ((dm355fb_check_var
				     (&dm->vid0->info.var, &dm->vid0->info))
				    < 0) {

					dm->vid0->info.var.xres =
					    vid0_default_var.xres;
					dm->vid0->info.var.yres =
					    vid0_default_var.yres;
					dm->vid0->win_pos.xpos = 0;
					dm->vid0->win_pos.ypos = 0;
				}
				dm->vid0->window_enable = VPBE_ENABLE;
			}
		} else if (!strncmp(this_opt, "vid1=", 5)) {
			flag_vid1 = 1;
			if (!strncmp(this_opt + 5, "off", 3))
				dm->vid1->window_enable = VPBE_DISABLE;
			else if (!parse_win_params(this_opt + 5,
						   &xres, &yres, &xpos,
						   &ypos)) {
				dm->vid1->info.var.xres = xres;
				dm->vid1->info.var.yres = yres;
				dm->vid1->win_pos.xpos = xpos;
				dm->vid1->win_pos.ypos = ypos;
				dm->vid1->window_enable = VPBE_DISABLE;
				if ((dm355fb_check_var
				     (&dm->vid1->info.var, &dm->vid1->info))
				    < 0) {
					dm->vid1->info.var.xres =
					    vid1_default_var.xres;
					dm->vid1->info.var.yres =
					    vid1_default_var.yres;
					dm->vid1->win_pos.xpos = 0;
					dm->vid1->win_pos.ypos = 0;
				}
				dm->vid1->window_enable = VPBE_ENABLE;
			}
		} else if (!strncmp(this_opt, "osd0=", 5)) {
			flag_osd0 = 1;
			if (!strncmp(this_opt + 5, "off", 3))
				dm->osd0->window_enable = VPBE_DISABLE;
			else if (!parse_win_params(this_opt + 5,
						   &xres, &yres, &xpos,
						   &ypos)) {
				dm->osd0->info.var.xres = xres;
				dm->osd0->info.var.yres = yres;
				dm->osd0->win_pos.xpos = xpos;
				dm->osd0->win_pos.ypos = ypos;
				dm->osd0->window_enable = VPBE_DISABLE;
				if ((dm355fb_check_var
				     (&dm->osd0->info.var, &dm->osd0->info))
				    < 0) {
					dm->osd0->info.var.xres =
					    osd0_default_var.xres;
					dm->osd0->info.var.yres =
					    osd0_default_var.yres;
					dm->osd0->win_pos.xpos = 0;
					dm->osd0->win_pos.ypos = 0;
				}
				dm->osd0->window_enable = VPBE_ENABLE;
			}
		} else if (!strncmp(this_opt, "osd1=", 5)) {
			flag_osd1 = 1;
			if (!strncmp(this_opt + 5, "off", 3))
				dm->osd1->window_enable = VPBE_DISABLE;
			else if (!parse_win_params(this_opt + 5,
						   &xres, &yres, &xpos,
						   &ypos)) {
				dm->osd1->info.var.xres = xres;
				dm->osd1->info.var.yres = yres;
				dm->osd1->win_pos.xpos = xpos;
				dm->osd1->win_pos.ypos = ypos;
				dm->osd1->window_enable = VPBE_DISABLE;
				if ((dm355fb_check_var
				     (&dm->osd1->info.var, &dm->osd1->info))
				    < 0) {
					dm->osd1->info.var.xres =
					    osd1_default_var.xres;
					dm->osd1->info.var.yres =
					    osd1_default_var.yres;
					dm->osd1->win_pos.xpos = 0;
					dm->osd1->win_pos.ypos = 0;
				}
				dm->osd1->window_enable = VPBE_ENABLE;
			}
		}
	}
	if (flag) {
		switch (dm->display.interface) {
		case SVIDEO:
		case COMPONENT:
		case YCC8:
		case SRGB:
		case COMPOSITE:{
				if ((dm->display.mode == NTSC) ||
				    (dm->display.mode == P525) ||
				    (dm->display.mode == LCD))
					format_yres = 480;
				else
					/*P625, PAL */
					format_yres = 576;
			}
			break;
		case PRGB:{
				if (dm->display.mode == P350)
					format_yres = 350;
				else if (dm->display.mode == P400)
					format_yres = 400;
				else
					format_yres = 480;
			}
			break;
		default:
			break;
		}
		if (!flag_osd0)
			dm->osd0->info.var.yres
			    = osd0_default_var.yres = format_yres;
		if (!flag_osd1)
			dm->osd1->info.var.yres
			    = osd1_default_var.yres = format_yres;
		if (!flag_vid0)
			dm->vid0->info.var.yres
			    = vid0_default_var.yres = format_yres;
		if (!flag_vid1)
			dm->vid1->info.var.yres
			    = vid1_default_var.yres = format_yres;
	}

	dev_dbg(dm->dev, "DaVinci: "
		"Output on %s%s, Enabled windows: %s %s %s %s\n",
		((dm->display.mode == P480)
		 && (dm->display.interface ==
		     PRGB)) ? "480P" : ((dm->display.mode == P400)
					&& (dm->display.interface ==
					    PRGB)) ? "400P" : ((dm->display.
								mode == P350)
							       && (dm->display.
								   interface ==
								   PRGB)) ?
		"350P" : (dm->display.mode ==
			  LCD) ? "LCD" : (dm->display.mode ==
					  NTSC) ? "NTSC" : (dm->display.mode ==
							    PAL) ? "PAL" : (dm->
									    display.
									    mode
									    ==
									    P525)
		? "525P" : (dm->display.mode ==
			    P625) ? "625P" : "unknown device!",
		(dm->display.interface ==
		 SVIDEO) ? " in SVIDEO format" : (dm->display.interface ==
						  COMPONENT) ?
		" in COMPONENT format" : (dm->display.interface ==
					  COMPOSITE) ? " in COMPOSITE format"
		: (dm->display.interface ==
		   RGB) ? " in RGB format" : (dm->display.interface == YCC16)
		? " in YCC16 format" : (dm->display.interface ==
					YCC8) ? " in YCC8 format" : (dm->
								     display.
								     interface
								     ==
								     PRGB) ?
		" in PRGB format" : (dm->display.interface ==
				     SRGB) ? " in SRGB format" : (dm->display.
								  interface ==
								  EPSON) ?
		" in EPSON format" : (dm->display.interface ==
				      CASIO1G) ? " in CASIO1G format" : (dm->
									 display.
									 interface
									 ==
									 UDISP)
		? " in UDISP format" : (dm->display.interface ==
					STN) ? " in STN format" : "",
		(dm->vid0->window_enable) ? "Video0" : "",
		(dm->vid1->window_enable) ? "Video1" : "",
		(dm->osd0->window_enable) ? "OSD0" : "",
		(dm->osd1->window_enable) ? "OSD1" : "");

	if (dm->vid0->window_enable)
		dev_dbg(dm->dev, "Setting Video0 size %dx%d, "
			"position (%d,%d)\n",
			dm->vid0->info.var.xres, dm->vid0->info.var.yres,
			dm->vid0->win_pos.xpos, dm->vid0->win_pos.ypos);
	if (dm->vid1->window_enable)
		dev_dbg(dm->dev, "Setting Video1 size %dx%d, "
			"position (%d,%d)\n",
			dm->vid1->info.var.xres, dm->vid1->info.var.yres,
			dm->vid1->win_pos.xpos, dm->vid1->win_pos.ypos);
	if (dm->osd0->window_enable)
		dev_dbg(dm->dev, "Setting OSD0 size %dx%d, "
			"position (%d,%d)\n",
			dm->osd0->info.var.xres, dm->osd0->info.var.yres,
			dm->osd0->win_pos.xpos, dm->osd0->win_pos.ypos);
	if (dm->osd1->window_enable)
		dev_dbg(dm->dev, "Setting OSD1 size %dx%d, "
			"position (%d,%d)\n",
			dm->osd1->info.var.xres, dm->osd1->info.var.yres,
			dm->osd1->win_pos.xpos, dm->osd1->win_pos.ypos);
	return 0;
}

/*
 * mem_release
 */
int mem_release(struct vpbe_dm_win_info *w)
{
	if (!w->alloc_fb_mem) {
		iounmap((void *)w->fb_base);
		release_mem_region(w->fb_base_phys, w->fb_size);
	} else {
		vpbe_mem_release_window_buf(w);
	}
	if (w)
		kfree(w);
	return 0;
}

void init_display_function(struct vpbe_display_format *d)
{
	if ((d->mode == NTSC) && (d->interface == COMPOSITE))
		dm->output_device_config = dm355fb_ntsc_composite_config;
	else if ((d->mode == PAL) && (d->interface == COMPOSITE))
		dm->output_device_config = dm355fb_pal_composite_config;
	else if ((d->mode == NTSC) && (d->interface == YCC8))
		dm->output_device_config = vpbe_dm355fb_ntsc_ycc8_config;
	else if ((d->mode == PAL) && (d->interface == YCC8))
		dm->output_device_config = vpbe_dm355fb_pal_ycc8_config;
	else if ((d->mode == NTSC) && (d->interface == SRGB))
		dm->output_device_config = vpbe_dm355fb_ntsc_srgb_config;
	else if ((d->mode == PAL) && (d->interface == SRGB))
		dm->output_device_config = vpbe_dm355fb_pal_srgb_config;
	else if ((d->mode == P480) && (d->interface == PRGB))
		dm->output_device_config = vpbe_dm355fb_480p_prgb_config;
	else if ((d->mode == DEFAULT_MODE) && (d->interface == PRGB))
		dm->output_device_config = vpbe_dm355fb_default_prgb_config;
	else if ((d->mode == DEFAULT_MODE) && (d->interface == YCC16))
		dm->output_device_config = vpbe_dm355fb_default_ycc16_config;
	else {
		dev_err(dm->dev, "Unsupported output device!\n");
		dm->output_device_config = NULL;
	}
}

/*
 * vpbe_mem_alloc_window_buf 
 */
/* The function allocates the memory required for the frame buffers of the window. */
int vpbe_mem_alloc_window_buf(struct vpbe_dm_win_info *w)
{
	/* Calculate size */
	w->fb_size = w->info.var.xres_virtual * w->info.var.yres_virtual
	    * (w->info.var.bits_per_pixel) / 8;
	dev_dbg(dm->dev, "Allocating buffer of size: %x\n", w->fb_size);
	w->fb_base = (unsigned long) dma_alloc_coherent
	    (dm->dev, w->fb_size, &w->fb_base_phys, GFP_KERNEL | GFP_DMA);
	
	if (!w->fb_base) {
		dev_err(dm->dev, "%s : dma_alloc_coherent fail.\n", __FUNCTION__);
		return -ENOMEM;
	}
	w->info.fix.smem_start = w->fb_base_phys;
	w->info.fix.smem_len = w->fb_size;

	w->info.screen_base = (char *) (w->fb_base);
	return 0;
}

/*
 * vpbe_mem_alloc_max
 */
/* The function	allocates max memory required for the frame buffers of the window. */
int vpbe_mem_alloc_max(struct vpbe_dm_win_info *w)
{
	dev_dbg(dm->dev, "Allocating buffer of size: %x\n", w->fb_size);
	w->fb_base = (unsigned long) dma_alloc_coherent
	    (dm->dev, w->fb_size, &w->fb_base_phys, GFP_KERNEL | GFP_DMA);
	if (!w->fb_base) {
		dev_err(dm->dev, "%s : dma_alloc_coherent fail.\n", __FUNCTION__);
		return -ENOMEM;
	}
	w->info.fix.smem_start = w->fb_base_phys;
	w->info.fix.smem_len = w->fb_size;

	w->info.screen_base = (char *) (w->fb_base);

	return 0;
}

/*
 * ========	vpbe_mem_release_window_buf	========
 */
/* The function	allocates the memory required for the 
				frame buffers of the window. */
int vpbe_mem_release_window_buf(struct vpbe_dm_win_info *w)
{
	dma_free_coherent(NULL, w->fb_size, (void *)w->fb_base,
			  w->fb_base_phys);
	w->info.fix.smem_start = 0;
	w->info.fix.smem_len = 0;
	return 0;
}

/*
 * ========	vpbe_mem_alloc_struct	========
 */
/* The function	allocate the memory required for the 
				frame buffers of the window. */
int vpbe_mem_alloc_struct(struct vpbe_dm_win_info **w)
{
	*w = kmalloc(sizeof(struct vpbe_dm_win_info), GFP_KERNEL);
	if (!*w) {
		return -ENOMEM;
	}
	memset(*w, 0, sizeof(struct vpbe_dm_win_info));

	return 0;
}

/*
 * ========	set_vid0_default_conf	========
 */
/* The function	set defaults of VID0 window. */
void set_vid0_default_conf()
{
	vpbe_video_params_t *vid_par = &dm->vid0->conf_params.video_params;
	vpbe_video_config_params_t vid_conf_params;

	vid_par->cb_cr_order = SET_0;	/* default order CB-CR */
	vid_par->exp_info.horizontal = VPBE_DISABLE;	/* Expansion disable */

	dm->vid0->alloc_fb_mem = 1;
	dm->vid0->window_enable = VPBE_DISABLE;
	dm->vid0->zoom.window_id = VID0;
	dm->vid0->zoom.zoom_h = 0;	/* No Zoom */
	dm->vid0->field_frame_select = FB_VMODE_NONINTERLACED;
	dm->vid0->numbufs = TRIPLE_BUF;

	/* Set the defaults vid params in registers */
	vid_conf_params.cb_cr_order = vid_par->cb_cr_order;
	vid_conf_params.exp_info = vid_par->exp_info;
	vpbe_set_vid_params(dm->vid0->info.fix.id, &vid_conf_params);

	/* Set default zoom in register */
	set_zoom(dm->vid0->zoom.window_id, dm->vid0->zoom.zoom_h,
		 dm->vid0->zoom.zoom_v);

	/* Set default buffer size */
	dm->vid0->fb_size = TRIPLE_BUF*DISP_MEMY
				*round_32(DISP_MEMX*(VID0_MAXBPP/8));
}

/*
 * ========	set_vid1_default_conf	========
 */
/* The function	set defaults of VID1 window. */
void set_vid1_default_conf()
{
	vpbe_video_params_t *vid_par = &dm->vid1->conf_params.video_params;
	vpbe_video_config_params_t vid_conf_params;

	vid_par->cb_cr_order = SET_0;	/* default order CB-CR */
	vid_par->exp_info.horizontal = VPBE_DISABLE;	/* Expansion disable */
	vid_par->exp_info.vertical = VPBE_DISABLE;

	dm->vid1->alloc_fb_mem = 1;
	dm->vid1->window_enable = VPBE_ENABLE;
	dm->vid1->zoom.window_id = VID1;
	dm->vid1->zoom.zoom_h = 0;	/* No Zoom */
	dm->vid1->zoom.zoom_v = 0;
	dm->vid1->field_frame_select = FB_VMODE_NONINTERLACED;
	dm->vid1->numbufs = TRIPLE_BUF;

	/* Set the defaults vid params in registers */
	vid_conf_params.cb_cr_order = vid_par->cb_cr_order;
	vid_conf_params.exp_info = vid_par->exp_info;
	vpbe_set_vid_params(dm->vid1->info.fix.id, &vid_conf_params);

	/* Set default zoom in register */
	set_zoom(dm->vid1->zoom.window_id, dm->vid1->zoom.zoom_h,
		 dm->vid1->zoom.zoom_v);

	/* Set default buffer size */
	dm->vid1->fb_size = TRIPLE_BUF*DISP_MEMY
				*round_32(DISP_MEMX*(VID1_MAXBPP/8));
}

/*
 * ========	set_osd0_default_conf	========
 */
/* The function	set defaults of OSD0 window. */
void set_osd0_default_conf()
{
	vpbe_bitmap_params_t *bit_par = &dm->osd0->conf_params.bitmap_params;
	vpbe_bitmap_config_params_t bitmap_config_params;

	bit_par->attenuation_enable = SET_0;	/* Attenuation Disabled */
	bit_par->clut_select = SET_0;	/* ROM_CLUT */
	bit_par->enable_attribute = SET_0;
	bit_par->blend_info.bf = 0x07;
	bit_par->blink_info.blinking = VPBE_DISABLE;
	bit_par->blink_info.interval = 0;

	dm->osd0->alloc_fb_mem = 1;
	dm->osd0->window_enable = VPBE_ENABLE;
	dm->osd0->zoom.window_id = OSD0;
	dm->osd0->zoom.zoom_h = 0;	/* No Zoom */
	dm->osd0->zoom.zoom_v = 0;
	dm->osd0->field_frame_select = FB_VMODE_NONINTERLACED;
	dm->osd0->numbufs = DOUBLE_BUF;

	bitmap_config_params.attenuation_enable = bit_par->attenuation_enable;
	bitmap_config_params.clut_select = bit_par->clut_select;

	/* Set default bitmap params in registers */
	vpbe_bitmap_set_params(dm->osd0->info.fix.id, &bitmap_config_params);
	/* Set default zoom in register */
	set_zoom(dm->osd0->zoom.window_id, dm->osd0->zoom.zoom_h,
		 dm->osd0->zoom.zoom_v);
	/* Set default buffer size */
	dm->osd0->fb_size = DOUBLE_BUF*DISP_MEMY
				*round_32(DISP_MEMX*(OSD0_MAXBPP/8));
}

/*
 * ========	set_osd1_default_conf	========
 */
/* The function	set defaults of OSD1 window. */
void set_osd1_default_conf()
{
	vpbe_bitmap_params_t *bit_par = &dm->osd1->conf_params.bitmap_params;
	vpbe_bitmap_config_params_t bitmap_config_params;

	bit_par->attenuation_enable = SET_0;	/* Attenuation Disabled */
	bit_par->clut_select = 2;	/* RAM CLUT */
	bit_par->enable_attribute = SET_1;
	bit_par->blend_info.bf = 0x07;
	bit_par->blink_info.blinking = VPBE_DISABLE;
	bit_par->blink_info.interval = 0;

	dm->osd1->alloc_fb_mem = 1;
	dm->osd1->window_enable = VPBE_ENABLE;
	dm->osd1->zoom.window_id = OSD1;
	dm->osd1->zoom.zoom_h = 0;
	dm->osd1->zoom.zoom_v = 0;
	dm->osd1->field_frame_select = FB_VMODE_NONINTERLACED;
	dm->osd1->numbufs = DOUBLE_BUF;
	dm->osd1->conf_params.bitmap_params.stored_bits_per_pixel = SET_0;

	bitmap_config_params.attenuation_enable = bit_par->attenuation_enable;
	bitmap_config_params.clut_select = bit_par->clut_select;

	/* Set default bitmap params in registers */
	vpbe_bitmap_set_params(dm->osd1->info.fix.id, &bitmap_config_params);
	/* Set default zoom in register */
	set_zoom(dm->osd1->zoom.window_id, dm->osd1->zoom.zoom_h,
		 dm->osd1->zoom.zoom_v);
	/* Set default buffer size */
	dm->osd1->fb_size = DOUBLE_BUF*DISP_MEMY
				*round_32(DISP_MEMX*(OSD1_MAXBPP/8));
}

/*
 * ========	set_cursor_default_conf	========
 */
/* The function	set defaults of cursor window. */
void set_cursor_default_conf()
{
	dm->cursor.enable = VPBE_DISABLE;
	dm->cursor.image.dx = 10;
	dm->cursor.image.dy = 10;
	dm->cursor.image.width = 10;
	dm->cursor.image.height = 10;
	dm->cursor.image.fg_color = 10;
	dm->cursor.image.depth = 10;

	/* Set default cursor params in registers. */
	vpbe_set_cursor_params(&dm->cursor);
}

/*
 * ========	set_dm_default_conf	========
 */
/* The function	set defaults of dm structure. */
void set_dm_default_conf()
{
	dm->backg.clut_select = SET_0;
	dm->backg.color_offset = 0x80;

	dm->dclk.dclk_pattern_width = 0;
	dm->dclk.dclk_pattern0 = 0;
	dm->dclk.dclk_pattern1 = 0;
	dm->dclk.dclk_pattern2 = 0;
	dm->dclk.dclk_pattern3 = 0;

	dm->display.mode = NTSC;
	dm->display.interface = COMPOSITE;

	/* Configure defaults in registers */

	/* Initialize the VPSS Clock Control register */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

	/* Reset OSD registers to default. */
	dispc_reg_out(OSD_MODE, 0);
	dispc_reg_out(OSD_OSDWIN0MD, 0);

	/* Set blue background color */
	vpbe_set_backg_color(&dm->backg);
}

/*
 *  Cleanup
 */
int dm355fb_remove(struct device *dev)
{
	free_irq(IRQ_VENCINT, &dm);

	/* Cleanup all framebuffers */
	if (dm->osd0) {
		unregister_framebuffer(&dm->osd0->info);
		mem_release(dm->osd0);
	}
	if (dm->osd1) {
		unregister_framebuffer(&dm->osd1->info);
		mem_release(dm->osd1);
	}
	if (dm->vid0) {
		unregister_framebuffer(&dm->vid0->info);
		mem_release(dm->vid0);
	}
	if (dm->vid1) {
		unregister_framebuffer(&dm->vid1->info);
		mem_release(dm->vid1);
	}

	/* Turn OFF the output device */
	dm->output_device_config();
	vpbe_enable_venc(0);
	if ((dm->display.interface == YCC8)
		|| (dm->display.interface == SRGB))
		vpbe_enable_lcd(0);
	else if (dm->display.interface == COMPOSITE)
		vpbe_enable_dacs(0);

	if (dm->mmio_base)
		iounmap((void *)dm->mmio_base);
	release_mem_region(dm->mmio_base_phys, dm->mmio_size);

	return 0;
}

/*
 * ========	vpbe_dm355fb_probe ========
 */
int vpbe_dm355fb_probe(struct device *dev)
{
	struct platform_device *pdev;

	pdev = to_platform_device(dev);
	if (pdev->num_resources != 0) {
		dev_err(dev, "probed for an	unknown	device\n");
		return -ENODEV;
	}

	dm->dev = dev;
	dm->mmio_base_phys = OSD_REG_BASE;
	dm->mmio_size = OSD_REG_SIZE;

	/* request the mem regions */
	if (!request_mem_region
	    (dm->mmio_base_phys, dm->mmio_size, DM355FB_DRIVER)) {
		dev_err(dev, ":	cannot reserve MMIO	region\n");
		return -ENODEV;
	}

	/* map the regions */
	dm->mmio_base =
	    (unsigned long)ioremap(dm->mmio_base_phys, dm->mmio_size);
	if (!dm->mmio_base) {
		dev_err(dev, ":	cannot map MMIO\n");
		goto release_mmio;
	}

	/* Do default settings for all the windows      */
	dm355fb_setup(options);

	/* Set fb_videomode structure */
	dm->videomode = modelist[dm->display.interface][dm->display.mode];

	/* choose config function */
	init_display_function(&dm->display);

	/* initialize   the     vsync   wait queue */
	init_waitqueue_head(&dm->vsync_wait);
	dm->timeout = HZ / 5;

/*
 *	WINDOW SETUP:	
 *		Perform following for all the windows
 *	a)	If window is to be enabled, allocate buffers.
 *	b)	Register fb device.
 *	c)	Call dm355fb_set_par to configure all OSD registers
 *	d)	Set appropriate register to enable the window, 
 *		if command line option is given to enable the window.    
 *	e)	Initialize video output by executing configuration function.
 */

	/*
	 * OSD0 Setup
	 */
	/* Allocate buffers */
	if (dm->osd0->window_enable) {
		if (vpbe_mem_alloc_max(dm->osd0) < 0)
			goto vpbe_probe_exit;
		dm->osd0->info.screen_base = (char *)(dm->osd0->fb_base);
		/* memset buffer with some value for background color */
		memset((void *)dm->osd0->fb_base, 0x00, dm->osd0->fb_size);
	}
	/* Register FB device */
	if (register_framebuffer(&dm->osd0->info) < 0) {
		dev_err(dev,
			OSD0_FBNAME
			"Unable	to register	OSD0 framebuffer\n");
		mem_release(dm->osd0);
		goto vpbe_probe_exit;
	}
	/* Configure registers */
	dm355fb_set_par(&dm->osd0->info);
	/* Enable the window */
	set_win_enable(dm->osd0->info.fix.id, dm->osd0->window_enable);

	/*
	 * VID0 Setup
	 */
	/* Allocate buffers */
	if (dm->vid0->window_enable) {
		if (vpbe_mem_alloc_max(dm->vid0) < 0)
			goto vpbe_probe_exit;

		dm->vid0->info.screen_base = (char *)(dm->vid0->fb_base);

		memset((void *)dm->vid0->fb_base, 0x80, dm->vid0->fb_size);
	}
	if (register_framebuffer(&dm->vid0->info) < 0) {
		dev_err(dev,
			VID0_FBNAME
			"Unable	to register	VID0 framebuffer\n");
		mem_release(dm->vid0);
		goto vpbe_probe_exit;
	}
	dm355fb_set_par(&dm->vid0->info);
	set_win_enable(dm->vid0->info.fix.id, dm->vid0->window_enable);

	/*
	 * OSD1 Setup
	 */
	if (dm->osd1->window_enable) {
		if (vpbe_mem_alloc_max(dm->osd1) < 0)
			goto vpbe_probe_exit;
		dm->osd1->info.screen_base = (char *)(dm->osd1->fb_base);
		memset((void *)dm->osd1->fb_base, 0x77, dm->osd1->fb_size);
	}
	if (register_framebuffer(&dm->osd1->info) < 0) {
		dev_err(dev,
			OSD1_FBNAME
			"Unable	to register	OSD1 framebuffer\n");
		mem_release(dm->osd1);
		goto vpbe_probe_exit;
	}
	dm355fb_set_par(&dm->osd1->info);
	set_win_enable(dm->osd1->info.fix.id, dm->osd1->window_enable);

	/*
	 * VID1 Setup
	 */
	if (dm->vid1->window_enable) {
		if (vpbe_mem_alloc_max(dm->vid1) < 0)
			goto vpbe_probe_exit;
		dm->vid1->info.screen_base = (char *)(dm->vid1->fb_base);
		memset((void *)dm->vid1->fb_base, 0x80, dm->vid1->fb_size);
	}
	if (register_framebuffer(&dm->vid1->info) < 0) {
		dev_err(dev,
			VID1_FBNAME
			"Unable	to register	VID1 framebuffer\n");
		mem_release(dm->vid1);
		goto vpbe_probe_exit;
	}
	dm355fb_set_par(&dm->vid1->info);
	set_win_enable(dm->vid1->info.fix.id, dm->vid1->window_enable);
	/* set interrupt registers */
	//dispc_reg_out(VPSSBL_INTSEL, 0xA983CB10);
	//dispc_reg_out(VPSSBL_EVNTSEL, 0x7B3C0004);

	/* Call the config function */
	dm->output_device_config();
	vpbe_enable_venc(1);
	if ((dm->display.interface == PRGB)
		|| (dm->display.interface == YCC8)
		|| (dm->display.interface == SRGB))
		vpbe_enable_lcd(1);
	else if (dm->display.interface == COMPOSITE)
		vpbe_enable_dacs(1);

	if (request_irq
	    (IRQ_VENCINT, dm355fb_isr, SA_SHIRQ, DM355FB_DRIVER, dm)) {
		dev_err(dev,
			DM355FB_DRIVER
			": could not install interrupt service routine\n");
		goto vpbe_probe_exit;
	}

	/* If window is disabled from command line, set var structure 
		and line_length to zero */
	if (!dm->osd0->window_enable) {
		memset(&dm->osd0->info.var, 0x00, sizeof(dm->osd0->info.var));
		dm->osd0->info.fix.line_length = 0;
	}
	if (!dm->vid0->window_enable) {
		memset(&dm->vid0->info.var, 0x00, sizeof(dm->vid0->info.var));
		dm->vid0->info.fix.line_length = 0;
	}
	if (!dm->osd1->window_enable) {
		memset(&dm->osd1->info.var, 0x00, sizeof(dm->osd1->info.var));
		dm->osd1->info.fix.line_length = 0;
	}
	if (!dm->vid1->window_enable) {
		memset(&dm->vid1->info.var, 0x00, sizeof(dm->vid1->info.var));
		dm->vid1->info.fix.line_length = 0;
	}

	return 0;

      vpbe_probe_exit:
	dm355fb_remove(dev);
	return -ENODEV;

      release_mmio:
	release_mem_region(dm->mmio_base_phys, dm->mmio_size);
	return -ENODEV;
}
int no_soft_cursor(struct fb_info *info, struct fb_cursor *cursor)
{
	return 0;
}

/*
 *  Frame buffer operations
 */
static struct fb_ops dm355fb_ops = {
	.owner = THIS_MODULE,
	.fb_check_var = dm355fb_check_var,
	.fb_set_par = dm355fb_set_par,
	.fb_setcolreg = dm355fb_setcolreg,
	.fb_blank = dm355fb_blank,
	.fb_pan_display = dm355fb_pan_display,
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea = cfb_copyarea,
	.fb_imageblit = cfb_imageblit,
	/* .fb_cursor = soft_cursor, */
	.fb_cursor = no_soft_cursor,
	.fb_rotate = NULL,
	.fb_sync = NULL,
	.fb_ioctl = dm355fb_ioctl,
};

static void dm355fb_release_dev(struct device *dev)
{
}

static u64 dm355fb_dmamask = ~(u32) 0;

static struct platform_device dm355fb_device = {
	.name = DM355FB_DEVICE,
	.id = 0,
	.dev = {
		.release = dm355fb_release_dev,
		.dma_mask = &dm355fb_dmamask,
		.coherent_dma_mask = 0xffffffff,
		},
	.num_resources = 0,
};

static struct device_driver dm355fb_driver = {
	.name = DM355FB_DRIVER,
	.bus = &platform_bus_type,
	.probe = vpbe_dm355fb_probe,
	.remove = dm355fb_remove,
	.suspend = NULL,
	.resume = NULL,
};

/*	Register both	the	driver and the device	*/
static int __init dm355fb_init(void)
{
	int r = 0;
	struct device *dev = &dm355fb_device.dev;

	/* Allocate memory for struc 'vpbe_dm_win_info' for all the windows */
	if (vpbe_mem_alloc_struct(&dm->vid0) < 0) {
		r = -1;
		goto vpbe_init_exit;
	}
	if (vpbe_mem_alloc_struct(&dm->vid1) < 0) {
		r = -1;
		goto vpbe_init_exit;
	}
	if (vpbe_mem_alloc_struct(&dm->osd0) < 0) {
		r = -1;
		goto vpbe_init_exit;
	}
	if (vpbe_mem_alloc_struct(&dm->osd1) < 0) {
		r = -1;
		goto vpbe_init_exit;
	}
	dm->vid0->dm = dm;
	dm->vid1->dm = dm;
	dm->osd0->dm = dm;
	dm->osd1->dm = dm;

#ifndef MODULE
	{
		/* handle options for "dm64xxfb" for backwards compatability */
		char *names[] = { "davincifb", "dm64xxfb", "dm355fb" };
		int i, num_names = 3, done = 0;

		for (i = 0; i < num_names && !done; i++) {
			if (fb_get_options(names[i], &options)) {
				dev_err(dev, " Disabled on command-line.\n");
				r = -ENODEV;
				goto vpbe_init_exit;
			}
			if (options)
				break;
		}
	}
#endif

	/* Register the device with LDM */
	if (platform_device_register(&dm355fb_device)) {
		dev_err(dev, "failed to	register dm355fb device\n");
		r = -ENODEV;
		goto vpbe_init_exit;
	}
	/* Register the driver with LDM */
	if (driver_register(&dm355fb_driver)) {
		dev_err(dev, "failed to	register dm355fb driver\n");
		platform_device_unregister(&dm355fb_device);
		r = -ENODEV;
		goto vpbe_init_exit;
	}
	return 0;
      vpbe_init_exit:
	if (dm->vid0)
		kfree(dm->vid0);
	if (dm->vid1)
		kfree(dm->vid1);
	if (dm->osd0)
		kfree(dm->osd0);
	if (dm->osd1)
		kfree(dm->osd1);
	return r;
}

static void __exit dm355fb_cleanup(void)
{
	driver_unregister(&dm355fb_driver);
	platform_device_unregister(&dm355fb_device);
}

module_init(dm355fb_init);
module_exit(dm355fb_cleanup);

MODULE_DESCRIPTION("Framebuffer driver for TI DaVinci");
MODULE_AUTHOR("Texas Instruments");
MODULE_LICENSE("GPL");
