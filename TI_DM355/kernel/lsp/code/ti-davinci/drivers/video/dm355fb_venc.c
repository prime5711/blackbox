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
 * File: dm355fb_venc.c	
 */
#include <linux/config.h>
#include <video/dm355_vpbe.h>
#include <video/dm355fb.h>

extern vpbe_dm_info_t *dm;
/* flags to indicate that window size is specified from command line */
extern int flag_osd0, flag_vid0, flag_osd1, flag_vid1;

void vpbe_enable_venc(int on)
{
	dispc_reg_merge(VENC_VMOD, on ? VENC_VMOD_VENC : 0, VENC_VMOD_VENC);
}

void vpbe_enable_dacs(int on)
{
	if (on) {
		dispc_reg_out(VENC_DACTST, 0);
	} else {
		dispc_reg_out(VENC_DACTST, 0xF000);
	}
}

/*
 * Function to change window parameters as per display.
 */
void change_win_param(int flag)
{
	if(!flag_vid0){
		dm->vid0->win_pos.xpos = 0;
		dm->vid0->win_pos.ypos = 0;
		dm->vid0->info.var.xres = dm->videomode.xres;
		dm->vid0->info.var.yres = dm->videomode.yres;
	}
	dm->vid0->info.var.vmode = dm->videomode.vmode;

	if(!flag_vid1){
		dm->vid1->win_pos.xpos = 0;
		dm->vid1->win_pos.ypos = 0;
		dm->vid1->info.var.xres = dm->videomode.xres;
		dm->vid1->info.var.yres = dm->videomode.yres;
	}
	dm->vid1->info.var.vmode = dm->videomode.vmode;

	if(!flag_osd0){
		dm->osd0->win_pos.xpos = 0;
		dm->osd0->win_pos.ypos = 0;
		dm->osd0->info.var.xres = dm->videomode.xres;
		dm->osd0->info.var.yres = dm->videomode.yres;
	}
	dm->osd0->info.var.vmode = dm->videomode.vmode;

	if(!flag_osd1){
		dm->osd1->win_pos.xpos = 0;
		dm->osd1->win_pos.ypos = 0;
		dm->osd1->info.var.xres = dm->videomode.xres;
		dm->osd1->info.var.yres = dm->videomode.yres;
	}
	dm->osd1->info.var.vmode = dm->videomode.vmode;

	set_interlaced(dm->vid0->info.fix.id,
		(dm->videomode.vmode == FB_VMODE_INTERLACED));
	set_interlaced(dm->vid1->info.fix.id,
		(dm->videomode.vmode == FB_VMODE_INTERLACED));
	set_interlaced(dm->osd0->info.fix.id,
		(dm->videomode.vmode == FB_VMODE_INTERLACED));
	set_interlaced(dm->osd1->info.fix.id,
		(dm->videomode.vmode == FB_VMODE_INTERLACED));

	if (flag == FB_VMODE_INTERLACED) {
		set_win_position(dm->vid0->info.fix.id,
				 dm->vid0->win_pos.xpos,
				 dm->vid0->win_pos.ypos / 2,
				 dm->vid0->info.var.xres,
				 dm->vid0->info.var.yres / 2);
		set_win_position(dm->vid1->info.fix.id,
				 dm->vid1->win_pos.xpos,
				 dm->vid1->win_pos.ypos / 2,
				 dm->vid1->info.var.xres,
				 dm->vid1->info.var.yres / 2);
		set_win_position(dm->osd0->info.fix.id,
				 dm->osd0->win_pos.xpos,
				 dm->osd0->win_pos.ypos / 2,
				 dm->osd0->info.var.xres,
				 dm->osd0->info.var.yres / 2);
		set_win_position(dm->osd1->info.fix.id,
				 dm->osd1->win_pos.xpos,
				 dm->osd1->win_pos.ypos / 2,
				 dm->osd1->info.var.xres,
				 dm->osd1->info.var.yres / 2);
	} else {
		set_win_position(dm->vid0->info.fix.id,
				 dm->vid0->win_pos.xpos,
				 dm->vid0->win_pos.ypos,
				 dm->vid0->info.var.xres,
				 dm->vid0->info.var.yres);
		set_win_position(dm->vid1->info.fix.id,
				 dm->vid1->win_pos.xpos,
				 dm->vid1->win_pos.ypos,
				 dm->vid1->info.var.xres,
				 dm->vid1->info.var.yres);
		set_win_position(dm->osd0->info.fix.id,
				 dm->osd0->win_pos.xpos,
				 dm->osd0->win_pos.ypos,
				 dm->osd0->info.var.xres,
				 dm->osd0->info.var.yres);
		set_win_position(dm->osd1->info.fix.id,
				 dm->osd1->win_pos.xpos,
				 dm->osd1->win_pos.ypos,
				 dm->osd1->info.var.xres,
				 dm->osd1->info.var.yres);
	}
}

void dm355fb_ntsc_composite_config(void)
{
	unsigned int venc;

	venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;

	vpbe_set_display_default();

	/* Enable the MXI2 oscillator. */
	dispc_reg_out(USB_PHY_CTRL,
		dispc_reg_in(USB_PHY_CTRL) & ~(1 << 2));

	/*
	 * Enable the venc and dac clocks.
	 * Select the MXI2 clock source.
	 */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);
#else
	dispc_reg_out(VPSS_CLK_CTRL, 0x38);
#endif
	/*
	 * Select divide-by-one for the venc clock.
	 * Enable the vpbe clock.
	 */
	dispc_reg_merge(VPSSCLK_CLKCTRL, 1, (1 << 2) | (1 << 0));

	/* Enable the venc clock. */
	dispc_reg_merge(VENC_CLKCTL, 1, 1);

	/*
	 * Set the OSD Divisor to 1.
	 * Note: For progressive scan, the values must be changed from
	 * (1,2) to (0,1).
	 */
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, VENC_VDPRO_ATCOM, VENC_VDPRO_ATCOM);

	/* Configure VDAC_CONFIG  */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VDAC_CONFIG, 0x3131BECE);
#else
	dispc_reg_out(VDAC_CONFIG, 0x0E21A6B6);
#endif

	/* Change window parameters for interlaced/progressive display */
	change_win_param(dm->videomode.vmode);

        /* Configure VMOD. No change in VENC bit */
	if (dm->videomode.vmode == FB_VMODE_INTERLACED)
        	dispc_reg_out(VENC_VMOD, 0x002 | venc);
	else
		dispc_reg_out(VENC_VMOD, 0x402 | venc);
}

void dm355fb_pal_composite_config(void)
{
	volatile unsigned int venc, dactst;
	dactst = dispc_reg_in(VENC_DACTST);
	venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
	vpbe_set_display_default();

	dispc_reg_out(VPSS_CLK_CTRL, 0x18);
	dispc_reg_out(VENC_CLKCTL, 0x1);

	/* set OSD Divisor to 1 */
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, 1 << VENC_VDPRO_ATCOM_SHIFT, 
					VENC_VDPRO_ATCOM);

	/* Configure VDAC_CONFIG  */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
	dispc_reg_out(VDAC_CONFIG, 0x3131BECE);
#else
	dispc_reg_out(VDAC_CONFIG, 0x0E21A6B6);
#endif

	/* Change window parameters for interlaced display */
	change_win_param(FB_VMODE_INTERLACED);

	/* Configure VMOD. No change in VENC bit */
	dispc_reg_out(VENC_VMOD, 0x1042 | venc);
	dispc_reg_out(VENC_DACTST, dactst);
}

void dm355fb_pal_ntsc_composite_switch(int IsNtsc)
{
	volatile unsigned int venc;
	venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
	if( IsNtsc == 1 )
	{
		/* Configure VMOD. No change in VENC bit */
		if (dm->videomode.vmode == FB_VMODE_INTERLACED)
			dispc_reg_out(VENC_VMOD, 0x002 | venc);
		else
			dispc_reg_out(VENC_VMOD, 0x402 | venc);

		set_win_position(dm->vid0->info.fix.id,
			0,
			0,
			720,
			480/2);

		set_win_position(dm->vid1->info.fix.id,
			0,
			0,
			720,
			480/2);
		set_win_position(dm->osd0->info.fix.id,
			0,
			0,
			720,
			480/2);
		set_win_position(dm->osd1->info.fix.id,
			0,
			0,
			720,
			480/2);
	}else{
		dispc_reg_out(VENC_VMOD, 0x1042 | venc);

		set_win_position(dm->vid0->info.fix.id,
			0,
			0,
			720,
			576/2);
		set_win_position(dm->vid1->info.fix.id,
			0,
			0,
			720,
			576/2);
		set_win_position(dm->osd0->info.fix.id,
			0,
			0,
			720,
			576/2);
		set_win_position(dm->osd1->info.fix.id,
			0,
			0,
			720,
			576/2);
	}
}
