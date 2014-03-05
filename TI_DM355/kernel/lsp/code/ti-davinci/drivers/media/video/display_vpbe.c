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
/* display_vpbe.c */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/device.h>
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
#include <video/dm355_vpbe.h>
#else
#include <video/davinci_vpbe.h>
#endif
#include "display_vpbe.h"

extern struct device *davinci_display_dev;
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

static struct vpbe_windowinfo vid0_default_var = {
   .x_pos = 0,
   .y_pos = 0,
   .width = 720,
   .height = 480,
   .bits_per_pixel = 16,
};

/* Using the bottom right quadrant of the screen screen for VID1 by default,
 * but keeping the framebuffer allocated for the full screen, so the user can
 * change the 'xres' and 'yres' later using the FBIOPUT_VSCREENINFO ioctl.
 */
static struct vpbe_windowinfo vid1_default_var = {
   .x_pos = 0,
   .y_pos = 0,
   .width = 720,
   .height = 480,
   .bits_per_pixel = 16,
};


#define VPBE_MAX_VIDEO_MODES 4
static struct video_config_param vpbe_config_params[VPBE_MAX_VIDEO_MODES] = {
		{
			"NTSC", 720, 480, 30, 1 , 0x88, 0x12, 0,0,0,0,0,0,0},
		{
			"PAL", 720, 576, 25, 1, 0x86, 0x14,0,0,0,0,0,0,0 },
			/* Next 2 values for timing (hint ... vstata) Must match with the values used in ths8200 */
		{	
			"720P", 1280, 720, 30,0, 0x50, 0x5, 1649, 300,1280,749, 26,720,0 }, /* Must match with the values in ths8200 */
		{
			"1080I-30",1920, 1080, 30, 0,0x58, 0x5, (2200-1),200,1920,(1125-1),13,(1088/2) }
};

void vpbe_get_hardware_address_range(dma_addr_t *mmio_base, unsigned long *mmio_size)
{
   	*mmio_base = OSD_REG_BASE;
   	*mmio_size = OSD_REG_SIZE;
}

static int vpbe_get_plane_video_config(char *video_mode_name, struct video_config_param *video_params)
{
     int i, found=0;
     for (i=0; i < VPBE_MAX_VIDEO_MODES ; i++)
     {
		if (!strcmp(vpbe_config_params[i].name, video_mode_name))
		{	
			found=1;
			memcpy((char *)video_params,&vpbe_config_params[i], sizeof (struct video_config_param));
			break;
		}
     }
     return (found);
}

void vpbe_get_vid0_window_defaults(struct vpbe_windowinfo *info)
{
	if (ISNULL(info))
	{
		dev_err(davinci_display_dev,"ERROR: info is NULL\n");
		return;
	}
	memcpy((void *)info,(void *)&vid0_default_var,sizeof(struct vpbe_windowinfo));
}
/*
 * vpbe_set_vid_params 
 */
/* This	function sets the cb-cr	order in the pixel data	for video windows. */
void vpbe_osd_set_vid_params(CB_CR_ORDER cb_cr_order)
{

	dev_dbg(davinci_display_dev,"vpbe_osd_set_vid_params: cb_cr_order = %d\n",cb_cr_order);
	dispc_reg_merge(OSD_MODE,
			cb_cr_order << OSD_MODE_CS_SHIFT,
			OSD_MODE_CS);
}

/*
 * vpbe_set_vid_expansion. Only for VIDO
 */
/* Function to set pixel clock parameters */
/* Note: we are setting expantion for video window only */
void vpbe_osd_set_vid_expansion(vpbe_win_expansion_t * required)
{
	/*Set fields OSD_MODE.VHRSZ and OSD_MODE.VVRSZ accordingly */
	dispc_reg_merge(OSD_MODE,
			required->horizontal << OSD_MODE_VHRSZ_SHIFT,
			OSD_MODE_VHRSZ);
	dispc_reg_merge(OSD_MODE,
			required->vertical << OSD_MODE_VVRSZ_SHIFT,
			OSD_MODE_VVRSZ);
}

void vpbe_get_vid1_window_defaults(struct vpbe_windowinfo *info)
{
	if (ISNULL(info)) {
		dev_err(davinci_display_dev, "ERROR: info is NULL\n");
		return;
	}
	memcpy((void *)info,(void *)&vid1_default_var,sizeof(struct vpbe_windowinfo));
}

void vpbe_set_vid0_win_size(u32 mode, u32 xp, u32 yp, u32 xl, u32 yl)
{
	if (mode) {	
		/* interlaced */
		dispc_reg_out(OSD_WINXP(0), xp);
		dispc_reg_out(OSD_WINYP(0), yp/2);
		dispc_reg_out(OSD_WINXL(0), xl);
		dispc_reg_out(OSD_WINYL(0), yl/2);
	}
	else {
		/* progressive mode */
		dispc_reg_out(OSD_WINXP(0), xp);
		dispc_reg_out(OSD_WINYP(0), yp);
		dispc_reg_out(OSD_WINXL(0), xl);
		dispc_reg_out(OSD_WINYL(0), yl);
	}
}
void vpbe_set_vid1_win_size(u32 mode, u32 xp, u32 yp, u32 xl, u32 yl)
{
	if (mode) {	
		/* interlaced */
		dispc_reg_out(OSD_WINXP(1), xp);
		dispc_reg_out(OSD_WINYP(1), yp/2);
		dispc_reg_out(OSD_WINXL(1), xl);
		dispc_reg_out(OSD_WINYL(1), yl/2);
	}
	else {
		/* progressive mode */
		dispc_reg_out(OSD_WINXP(1), xp);
		dispc_reg_out(OSD_WINYP(1), yp);
		dispc_reg_out(OSD_WINXL(1), xl);
		dispc_reg_out(OSD_WINYL(1), yl);
	}
}

int vpbe_get_display_field_id(void)
{
	return ((dispc_reg_in(VENC_VSTAT) & 0x00000010) >> 4);
}

/* Interlaced = Frame mode, Non-interlaced = Field mode */
void vpbe_set_vid0_interlaced(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_VIDWINMD, on, OSD_VIDWINMD_VFF0);
}

void vpbe_set_vid1_interlaced(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_VIDWINMD, on, OSD_VIDWINMD_VFF1);
}

/* For zooming, we just have to set the start of framebuffer, the zoom factors 
 * and the display size. The hardware will then read only 
 * (display size / zoom factor) area of the framebuffer and  zoom and 
 * display it. In the following function, we assume that the start of 
 * framebuffer and the display size parameters are set already.
 */

void vpbe_osd_set_zoom_vid0(int h_factor, int v_factor)
{
	dispc_reg_merge(OSD_VIDWINMD,
			h_factor << OSD_VIDWINMD_VHZ0_SHIFT,
			OSD_VIDWINMD_VHZ0);
	dispc_reg_merge(OSD_VIDWINMD,
			v_factor << OSD_VIDWINMD_VVZ0_SHIFT,
			OSD_VIDWINMD_VVZ0);
}
void vpbe_osd_set_zoom_vid1(int h_factor, int v_factor)
{
	dispc_reg_merge(OSD_VIDWINMD,
			h_factor << OSD_VIDWINMD_VHZ1_SHIFT,
			OSD_VIDWINMD_VHZ1);
	dispc_reg_merge(OSD_VIDWINMD,
			v_factor << OSD_VIDWINMD_VVZ1_SHIFT,
			OSD_VIDWINMD_VVZ1);
}
void vpbe_osd_set_zoom_osd0(int h_factor, int v_factor)
{
	dispc_reg_merge(OSD_OSDWIN0MD,
			h_factor << OSD_OSDWIN0MD_OHZ0_SHIFT,
			OSD_OSDWIN0MD_OHZ0);
	dispc_reg_merge(OSD_OSDWIN0MD,
			v_factor << OSD_OSDWIN0MD_OVZ0_SHIFT,
			OSD_OSDWIN0MD_OVZ0);
}
void vpbe_osd_set_zoom_osd1(int h_factor, int v_factor)
{
	dispc_reg_merge(OSD_OSDWIN1MD,
			h_factor << OSD_OSDWIN1MD_OHZ1_SHIFT,
			OSD_OSDWIN1MD_OHZ1);
	dispc_reg_merge(OSD_OSDWIN1MD,
			v_factor << OSD_OSDWIN1MD_OVZ1_SHIFT,
			OSD_OSDWIN1MD_OVZ1);
}

/* Chooses the ROM CLUT for now. Can be extended later. */
void vpbe_set_bg_color(u8 clut, u8 color_offset)
{
	clut = 0;		/* 0 = ROM, 1 = RAM */

	dispc_reg_merge(OSD_MODE, OSD_MODE_BCLUT & clut, OSD_MODE_BCLUT);
	dispc_reg_merge(OSD_MODE, color_offset << OSD_MODE_CABG_SHIFT,
			OSD_MODE_CABG);
}

void vpbe_set_vid0_sdram_address(u32 addr, u32 line_length)
{
	/* The parameters to be written to the registers should be in 
	 * multiple of 32 bytes
	 */
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	unsigned int osd_vidwinadh = 0;

	line_length = line_length >> 5;
	addr = addr - 0x80000000;
	addr = addr >> 5;
	osd_vidwinadh = dispc_reg_in(OSD_VIDWINADH);
	osd_vidwinadh &= 0xFFFFFF80;
	osd_vidwinadh |= (addr & 0x007f0000)>>16;
	dispc_reg_out(OSD_VIDWINADH, osd_vidwinadh);
	dispc_reg_out(OSD_VIDWIN0ADL, (addr & 0x0000FFFF));
	dispc_reg_out(OSD_VIDWIN0OFST, line_length);
#else
	line_length /= 32;
	dispc_reg_out(OSD_VIDWIN0ADR, addr);
	dispc_reg_out(OSD_VIDWIN0OFST, line_length);
#endif
}
void vpbe_set_vid1_sdram_address(u32 addr, u32 line_length)
{
	/* The parameters to be written to the registers should be in 
	 * multiple of 32 bytes
	 */
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	unsigned int osd_vidwinadh = 0;

	line_length = line_length >> 5;
	addr = addr - 0x80000000;
	addr = addr >> 5;
	osd_vidwinadh =	dispc_reg_in(OSD_VIDWINADH);
	osd_vidwinadh &= 0xFFFF80FF;
	osd_vidwinadh |= (addr & 0x007f0000)>>8;
	dispc_reg_out(OSD_VIDWINADH, osd_vidwinadh);
	dispc_reg_out(OSD_VIDWIN1ADL, (addr & 0x0000FFFF) );
	dispc_reg_out(OSD_VIDWIN1OFST, line_length);
#else
	line_length /= 32;
	dispc_reg_out(OSD_VIDWIN1ADR, addr);
	dispc_reg_out(OSD_VIDWIN1OFST, line_length);
#endif
}

void vpbe_set_vid0_win_enable(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_VIDWINMD, on, OSD_VIDWINMD_ACT0);
}

int vpbe_check_for_dma_error(void)
{
	int error = 0;

#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	error = dispc_reg_in(OSD_MISCCT);
	return (error & 0x40);
#else
	return error;
#endif
	
}


void vpbe_reset_dma_error(void)
{
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	int val;

	val = dispc_reg_in(OSD_MISCCT);
	val |= 0x40;
	dispc_reg_out(OSD_MISCCT, val);
#endif
}


void vpbe_set_vid1_win_enable(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_VIDWINMD, on, OSD_VIDWINMD_ACT1);
}

void vpbe_set_osd0_win_enable(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_OSDWIN0MD, on, OSD_OSDWIN0MD_OACT0);
}

void vpbe_set_osd1_win_enable(unsigned int on)
{
	on = (on == 0) ? 0 : ~0;
	dispc_reg_merge(OSD_OSDWIN1MD, on, OSD_OSDWIN1MD_OACT1);
}


void vpbe_set_vid0_win_mode(short bits_per_pixel)
{
	if (bits_per_pixel == 16) {
		/* disable RGB888 format */
		dispc_reg_merge(OSD_MISCCT,
				VPBE_DISABLE <<
				OSD_MISCCT_RGBEN_SHIFT,
				OSD_MISCCT_RGBEN);
	}
	if (bits_per_pixel == 24) {
		/* set RGB888 format */
		dispc_reg_merge(OSD_MISCCT,
				SET_0 << OSD_MISCCT_RGBWIN_SHIFT,
				OSD_MISCCT_RGBWIN);
		dispc_reg_merge(OSD_MISCCT,
				VPBE_ENABLE <<
				OSD_MISCCT_RGBEN_SHIFT,
				OSD_MISCCT_RGBEN);
	}
}
void vpbe_set_vid1_win_mode(short bits_per_pixel)
{
	if (bits_per_pixel == 16) {
		/* disable RGB888 format */
		dispc_reg_merge(OSD_MISCCT,
				VPBE_DISABLE <<
				OSD_MISCCT_RGBEN_SHIFT,
				OSD_MISCCT_RGBEN);
	}
	if (bits_per_pixel == 24) {
		/* set RGB888 format */
		dispc_reg_merge(OSD_MISCCT,
				SET_1 << OSD_MISCCT_RGBWIN_SHIFT,
				OSD_MISCCT_RGBWIN);
		dispc_reg_merge(OSD_MISCCT,
				VPBE_ENABLE <<
				OSD_MISCCT_RGBEN_SHIFT,
				OSD_MISCCT_RGBEN);
	}
}

static void vpbe_enable_venc(int on)
{
	dispc_reg_merge(VENC_VMOD, on, VENC_VMOD_VENC);
}

static void vpbe_enable_dacs(int on)
{
	if (on) {
		dispc_reg_out(VENC_DACTST, 0);
	} else {
		dispc_reg_out(VENC_DACTST, 0xF000);
	}
}

void vpbe_enable_vpbe_output(int on)
{
	vpbe_enable_venc(on);
	vpbe_enable_dacs(on);
}

void vpbe_set_interrupt_register(void)
{
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)|| defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	/* Setup EVENT SEL registers for VENC interrupt */
	dispc_reg_out(VPSSBL_EVNTSEL, 0x7B3C0004);
#endif
}

static void enableDigitalOutput(int bEnable)
{
   if (bEnable)
   {
      // Set PINMUX0 reg to enable LCD (all other settings are kept per u-boot)

#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM) && !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
      dispc_reg_merge(PINMUX0, PINMUX0_LOEEN, PINMUX0_LOEEN);
#endif

      // Set PCR register for FULL clock
      dispc_reg_out(VPBE_PCR, 0);

      // Enable video clock output and inverse clock polarity
      dispc_reg_out(VENC_VIDCTL, (VENC_VIDCTL_VCLKE | VENC_VIDCTL_VCLKP));

      // Setting DRGB Matrix registers back to default values
      dispc_reg_out(VENC_DRGBX0, 0x00000400);
      dispc_reg_out(VENC_DRGBX1, 0x00000576);
      dispc_reg_out(VENC_DRGBX2, 0x00000159);
      dispc_reg_out(VENC_DRGBX3, 0x000002cb);
      dispc_reg_out(VENC_DRGBX4, 0x000006ee);


      // Enable DCLOCK
      dispc_reg_out(VENC_DCLKCTL, VENC_DCLKCTL_DCKEC);
      // Set DCLOCK pattern
      dispc_reg_out(VENC_DCLKPTN0, 1);
      dispc_reg_out(VENC_DCLKPTN1, 0);
      dispc_reg_out(VENC_DCLKPTN2, 0);
      dispc_reg_out(VENC_DCLKPTN3, 0);
      dispc_reg_out(VENC_DCLKPTN0A, 2);
      dispc_reg_out(VENC_DCLKPTN1A, 0);
      dispc_reg_out(VENC_DCLKPTN2A, 0);
      dispc_reg_out(VENC_DCLKPTN3A, 0);
      dispc_reg_out(VENC_DCLKHS, 0);
      dispc_reg_out(VENC_DCLKHSA, 1);
      dispc_reg_out(VENC_DCLKHR, 0);
      dispc_reg_out(VENC_DCLKVS, 0);
      dispc_reg_out(VENC_DCLKVR, 0);


      // Enable LCD output control (accepting default polarity)
      dispc_reg_out(VENC_LCDOUT, 0x1);

      // Set brightness start position and pulse width to zero
      dispc_reg_out(VENC_BRTS, 0);
      dispc_reg_out(VENC_BRTW, 0);

      // Set LCD AC toggle interval and horizontal position to zero
      dispc_reg_out(VENC_ACCTL, 0);

      // Set PWM period and width to zero
      dispc_reg_out(VENC_PWMP, 0);
      dispc_reg_out(VENC_PWMW, 0);

      // Clear component and composite mode registers (applicable to Analog DACS)
      dispc_reg_out(VENC_CVBS, 0);
 
#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
      dispc_reg_out(VENC_CMPNT, 0);
#endif

      // turning on horizontal and vertical syncs
      dispc_reg_out(VENC_SYNCCTL, (VENC_SYNCCTL_SYEV|VENC_SYNCCTL_SYEH) );

      // Set OSD clock and OSD Sync Adavance registers
      dispc_reg_out(VENC_OSDCLK0, 0);
      dispc_reg_out(VENC_OSDCLK1, 1);
      dispc_reg_out(VENC_OSDHAD, 0);

      // set VPSS clock
#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
      dispc_reg_out(VPSS_CLKCTL, 0x0a);
#else
      dispc_reg_out(VPSS_CLK_CTRL, 0x0a);
#endif

   }
   else
   {
      /* Initialize the VPSS Clock Control register */
      //dispc_reg_out(VPSS_CLKCTL, 0x18);

      // Set PINMUX0 reg to enable LCD (all other settings are kept per u-boot)

#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
      dispc_reg_merge(PINMUX0, 0, PINMUX0_LOEEN);
      dispc_reg_merge(PINMUX0, 0, PINMUX0_LFLDEN);
#endif

      /* disable VCLK output pin enable */
      dispc_reg_out(VENC_VIDCTL, 0x1101);

      // Disable output sync pins
      dispc_reg_out(VENC_SYNCCTL, 0 );

      // Disable DCLOCK
      dispc_reg_out(VENC_DCLKCTL, 0);
      dispc_reg_out(VENC_DRGBX1, 0x0000057C);

      // Disable LCD output control (accepting default polarity)
      dispc_reg_out(VENC_LCDOUT, 0);
#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
      dispc_reg_out(VENC_CMPNT, 0x100);
#endif

      dispc_reg_out(VENC_HSPLS, 0);
      dispc_reg_out(VENC_VSPLS, 0);
      dispc_reg_out(VENC_HINT, 0);
      dispc_reg_out(VENC_HSTART, 0);
      dispc_reg_out(VENC_HVALID, 0);
      dispc_reg_out(VENC_VINT, 0);
      dispc_reg_out(VENC_VSTART, 0);
      dispc_reg_out(VENC_VVALID, 0);
      dispc_reg_out(VENC_HSDLY, 0);
      dispc_reg_out(VENC_VSDLY, 0);
      dispc_reg_out(VENC_YCCCTL, 0);
      dispc_reg_out(VENC_VSTARTA, 0);

      // Set OSD clock and OSD Sync Adavance registers
      dispc_reg_out(VENC_OSDCLK0, 1);
      dispc_reg_out(VENC_OSDCLK1, 2);
   }
}

void vpbe_set_display_default(void)
{
	dispc_reg_out(VENC_VMOD, 0);
	dispc_reg_out(VENC_CVBS, 0);

#if !defined(CONFIG_MACH_DAVINCI_DM355_EVM)&& !defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VENC_CMPNT, 0x100);
	dispc_reg_merge(PINMUX0, 0 << PINMUX0_RGB666_SHIFT, PINMUX0_RGB666);
	dispc_reg_merge(PINMUX0, 0 << PINMUX0_LOEEN_SHIFT, PINMUX0_LOEEN);
#endif
	dispc_reg_out(VENC_LCDOUT, 0);
	dispc_reg_out(VENC_VIDCTL, 0x141);
	dispc_reg_out(VENC_DCLKCTL, 0);
	dispc_reg_out(VENC_DCLKPTN0, 0);
	dispc_reg_out(VENC_SYNCCTL, 0);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);
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
	dispc_reg_out(VENC_DACSEL, 0);
}

static void vpbe_set_ntsc_composite_config(struct video_config_param *param)
{
	volatile unsigned int venc;
	dev_dbg(davinci_display_dev,"Setting the output for NTSC\n");

    venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
	//vpbe_set_display_default();	
	enableDigitalOutput(0);
	
	//dispc_reg_out(USB_PHY_CTRL, 0xC3);
	//dispc_reg_out(VPSS_CLK_CTRL, 0x38);
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);
	dispc_reg_out(VENC_CLKCTL, 0x01);
	dispc_reg_out(VPBE_PCR, 0);
	/* DM 350 Configure VDAC_CONFIG , why ?? */
	dispc_reg_out(VDAC_CONFIG, 0x3131bece);
#else
	dispc_reg_out(VPSS_CLKCTL, 0x18);
	/* to set VENC CLK DIV to 1 - final clock is 54 MHz */
	dispc_reg_merge(VPBE_PCR, 0, VPBE_PCR_VENC_DIV);
	/* Set REC656 Mode */
	dispc_reg_out(VENC_YCCCTL, 0x1);
	dispc_reg_merge(VENC_VDPRO, 0, VENC_VDPRO_DAFRQ);
	dispc_reg_merge(VENC_VDPRO, 0, VENC_VDPRO_DAUPS);

#endif


	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, param->basepx);
	dispc_reg_out(OSD_BASEPY, param->basepy);


	/* Configure VMOD. No change in VENC bit */
	dispc_reg_out(VENC_VMOD, 0x0003 | venc);
	dispc_reg_out(VENC_DACTST, 0x0);
}

static void vpbe_set_pal_composite_config(struct video_config_param *param)
{
	volatile unsigned int venc;
	dev_dbg(davinci_display_dev,"Setting the output for PAL\n");

    venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
	enableDigitalOutput(0);
	//vpbe_set_display_default();	
	//dispc_reg_out(USB_PHY_CTRL, 0xC3);
	//dispc_reg_out(VPSS_CLK_CTRL, 0x38);

#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);
	dispc_reg_out(VENC_CLKCTL, 0x1);
	dispc_reg_out(VPBE_PCR, 0);
	/* DM350 Configure VDAC_CONFIG  */
	dispc_reg_out(VDAC_CONFIG, 0x3131bfbe);
#else
	dispc_reg_out(VPSS_CLKCTL, 0x18);
	/* to set VENC CLK DIV to 1 - final clock is 54 MHz */
	dispc_reg_merge(VPBE_PCR, 0, VPBE_PCR_VENC_DIV);
	/* Set REC656 Mode */
	dispc_reg_out(VENC_YCCCTL, 0x1);
#endif

	dispc_reg_out(OSD_BASEPX, param->basepx);
	dispc_reg_out(OSD_BASEPY, param->basepy);
	/* Configure VMOD. No change in VENC bit */
	dispc_reg_out(VENC_VMOD, 0x1043 | venc);
	dispc_reg_out(VENC_DACTST, 0x0);
}

static void vpbe_set_non_standard_venc_timing(struct video_config_param *param)
{
	// Set Timing parameters for 720p or 1080I frame (must match what THS8200 expects)
	dispc_reg_out(VENC_HSPLS, param->basepx);
	dispc_reg_out(VENC_VSPLS, param->basepy);
	dispc_reg_out(VENC_HINT,param->hint);
	dispc_reg_out(VENC_HSTART,param->hstart );
	dispc_reg_out(VENC_HVALID,param->hvalid);
	dispc_reg_out(VENC_VINT,param->vint);
	dispc_reg_out(VENC_VSTART, param->vstart);
	dispc_reg_out(VENC_VVALID, param->vvalid);
	dispc_reg_out(VENC_HSDLY, 0);
	dispc_reg_out(VENC_VSDLY, 0);
	dispc_reg_out(VENC_YCCCTL, 0);
	dispc_reg_out(VENC_VSTARTA, param->vstata);
	return;
}


static int vpbe_set_720p_config(struct video_config_param *param)
{   
	// Reset video encoder module 
   	dispc_reg_out(VENC_VMOD, 0);

	// Set new baseX and baseY
	dispc_reg_out(OSD_BASEPX, param->basepx);
	dispc_reg_out(OSD_BASEPY, param->basepy);

	enableDigitalOutput(1);

#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	//dispc_reg_out(PINMUX1,PINMUX1_DLCD_ENABLE_PROGRESSIVE);
#else
	/* This may need to go in system module later */
	dispc_reg_merge(PINMUX0, 0, PINMUX0_LFLDEN);
#endif

	/* Set VENC for non-standard timing */
	vpbe_set_non_standard_venc_timing(param);

	// Enable all VENC, non-standard timing mode, master timing, HD, progressive
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VENC_VMOD, (VENC_VMOD_VENC|VENC_VMOD_VMD) );
#else
	dispc_reg_out(VENC_VMOD, (VENC_VMOD_VENC|VENC_VMOD_VMD | VENC_VMOD_HDMD) );
#endif
	return 0;
}

static int vpbe_set_1080i_config(struct video_config_param *param)
{
	// Reset video encoder module 
	dispc_reg_out(VENC_VMOD, 0);

	// Set new baseX and baseY
	dispc_reg_out(OSD_BASEPX, param->basepx);
	dispc_reg_out(OSD_BASEPY, param->basepy);

	enableDigitalOutput(1);
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	//dispc_reg_out(PINMUX1,PINMUX1_DLCD_ENABLE_INTERLACED);
#else
	dispc_reg_merge(PINMUX0, PINMUX0_LFLDEN, PINMUX0_LFLDEN);
#endif

	/* Set VENC for non-standard timing */
	vpbe_set_non_standard_venc_timing(param);

	// Enable all VENC, non-standard timing mode, master timing, HD, interlaced
#if defined(CONFIG_MACH_DAVINCI_DM355_EVM)||defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VENC_VMOD, (VENC_VMOD_VENC|VENC_VMOD_VMD | VENC_VMOD_NSIT) );
#else
	dispc_reg_out(VENC_VMOD, (VENC_VMOD_VENC|VENC_VMOD_VMD | VENC_VMOD_HDMD | VENC_VMOD_NSIT) );
#endif
	return 0;
}

int vpbe_config_display_output(struct vpbe_display_stdinfo *stdinfo)
{
	struct video_config_param video_params;

	dev_dbg(davinci_display_dev,"<vpbe_config_display_output>\n");

    vpbe_get_plane_video_config(stdinfo->name, &video_params);

	if (!strcmp(stdinfo->name,"NTSC"))
		vpbe_set_ntsc_composite_config(&video_params);
	else if (!strcmp(stdinfo->name,"PAL"))
		vpbe_set_pal_composite_config(&video_params);
	else if (!strcmp(stdinfo->name,"720P"))
		vpbe_set_720p_config(&video_params);
	else if (!strcmp(stdinfo->name,"1080I-30"))
		vpbe_set_1080i_config(&video_params);
	else 
		return -1;
	return 0;
	dev_dbg(davinci_display_dev,"</vpbe_config_display_output>\n");
}
	
int vpbe_display_get_std_info(struct vpbe_display_stdinfo *std_info)
{
	int index, found = -1;
	
	if(!std_info)
		return found;

	/* loop on the number of mode supported per channel */
	for(index = 0 ; index < VPBE_MAX_VIDEO_MODES ; index++) {

		/* If the mode is found, set the parameter in VPIF register */
		if(0 == strcmp(vpbe_config_params[index].name,
		   std_info->name)) {
			std_info->activelines
			    = vpbe_config_params[index].activelines;
			std_info->activepixels
			    = vpbe_config_params[index].activepixels;
			std_info->fps
			    = vpbe_config_params[index].fps;
			std_info->frame_format
			    = vpbe_config_params[index].mode;
			found = 1;
			break;
		}
	}
	return found;
}

