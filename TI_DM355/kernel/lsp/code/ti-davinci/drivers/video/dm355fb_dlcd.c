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
 * File: dm355fb_dlcd.c	
 */
#include <video/dm355_vpbe.h>
#include <video/dm355fb.h>

extern vpbe_dm_info_t *dm;

void vpbe_enable_lcd(int on)
{
        if (on) {
                /* Enable LCD out */
                dispc_reg_out(VENC_LCDOUT, 0x1);
        } else {
                /* Disable LCD out */
                dispc_reg_out(VENC_LCDOUT, 0x0);
        }
}

/*
 * vpbe_set_display_timing 
 */
/* This function sets the display timing from the fb_info structure*/
void vpbe_set_display_timing(struct vpbe_fb_videomode *videomode)
{
	dispc_reg_out(VENC_HSPLS, videomode->hsync_len);
	dispc_reg_out(VENC_HSTART, videomode->left_margin);
	dispc_reg_out(VENC_HVALID, (videomode->xres));
	dispc_reg_out(VENC_HINT,
		      videomode->xres + videomode->left_margin +
		      videomode->right_margin - 1);

	dispc_reg_out(VENC_VSPLS, videomode->vsync_len);
	dispc_reg_out(VENC_VSTART, videomode->upper_margin);
	dispc_reg_out(VENC_VVALID, (videomode->yres));
	dispc_reg_out(VENC_VINT,
		      videomode->yres + videomode->upper_margin +
		      videomode->lower_margin);
};
/* NTSC interlaced YCC8 standard LCD display */
void vpbe_dm355fb_ntsc_ycc8_config(void)
{
        volatile unsigned int venc, lcdout;
        venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
        lcdout = dispc_reg_in(VENC_LCDOUT);

	/* Clock enable for DLCD controller and VENC controller */ 
	dispc_reg_out(VENC_CLKCTL, 0x011);

	vpbe_set_display_default();
	vpbe_set_display_timing(&dm->videomode);

	/* Set pinmux1 for digital display out */
	dispc_reg_merge(PINMUX1, 1<<GIO71_SHIFT, PINMUX1_GIO71);
	dispc_reg_merge(PINMUX1, 0<<HVSYNC_SHIFT, PINMUX1_HVSYNC);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, 1 << VENC_VDPRO_ATYCC_SHIFT, 
					VENC_VDPRO_ATYCC);
	
	/* Enable VCLKE */ 
	dispc_reg_merge(VENC_VIDCTL, 1 << VENC_VIDCTL_VCLKE_SHIFT,
			VENC_VIDCTL_VCLKE);
	/* VCLK pin output enable */ 
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_VCLKZ_SHIFT,
			VENC_VIDCTL_VCLKZ);
	/* Hsync Vsync pin output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_SYDIR_SHIFT,
			VENC_VIDCTL_SYDIR);
	/* YOUT/COUT output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_YCDIR_SHIFT,
			VENC_VIDCTL_YCDIR);
	/* Digital Data Output Mode is 'normal' */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_DOMD_SHIFT,
			VENC_VIDCTL_DOMD);
	/* Hsync Vsync polarity negative */
	dispc_reg_out(VENC_SYNCCTL,
		(VENC_SYNCCTL_SYEV | VENC_SYNCCTL_SYEH | VENC_SYNCCTL_HPL
		| VENC_SYNCCTL_VPL));
	
	/* Clock settings to generate 27 MHz for Philips TPG105 LCD */
	/* Note: DCLK is 27 MHx */
	/* DCLK pattern mode enable */
	dispc_reg_merge(VENC_DCLKCTL,
			1 << VENC_DCLKCTL_DCKEC_SHIFT,
			VENC_DCLKCTL_DCKEC);
	/* 2 valid bits in DCLKPTN0 register */
	dispc_reg_merge(VENC_DCLKCTL,
			0 << VENC_DCLKCTL_DCKPW_SHIFT,
			VENC_DCLKCTL_DCKPW);
	dispc_reg_out(VENC_DCLKPTN0, 0x1);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Change window parameters for interlaced display */
	change_win_param(FB_VMODE_INTERLACED);

	/* set VPSS clock */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

        /* Configure VMOD. No change in VENC bit */
        dispc_reg_out(VENC_VMOD, 0x1402 | venc);
        dispc_reg_out(VENC_LCDOUT, lcdout);
}
/* 
 * Following functions are NOT TESTED.
 * It is required to configure LCD before using mode 
 * other than ycc8/ntsc.
 */
/* PAL interlaced YCC8 standard LCD display */
void vpbe_dm355fb_pal_ycc8_config(void)
{
        volatile unsigned int venc, lcdout;
        venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
        lcdout = dispc_reg_in(VENC_LCDOUT);

	/* Clock enable for DLCD controller and VENC controller */ 
	dispc_reg_out(VENC_CLKCTL, 0x011);

	vpbe_set_display_default();
	vpbe_set_display_timing(&dm->videomode);

	/* Set pinmux1 for digital display out */
	dispc_reg_merge(PINMUX1, 1<<GIO71_SHIFT, PINMUX1_GIO71);
	dispc_reg_merge(PINMUX1, 0<<HVSYNC_SHIFT, PINMUX1_HVSYNC);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, 1 << VENC_VDPRO_ATYCC_SHIFT, 
					VENC_VDPRO_ATYCC);
	
	/* Enable VCLKE */ 
	dispc_reg_merge(VENC_VIDCTL, 1 << VENC_VIDCTL_VCLKE_SHIFT,
			VENC_VIDCTL_VCLKE);
	/* VCLK pin output enable */ 
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_VCLKZ_SHIFT,
			VENC_VIDCTL_VCLKZ);
	/* Hsync Vsync pin output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_SYDIR_SHIFT,
			VENC_VIDCTL_SYDIR);
	/* YOUT/COUT output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_YCDIR_SHIFT,
			VENC_VIDCTL_YCDIR);
	/* Digital Data Output Mode is 'normal' */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_DOMD_SHIFT,
			VENC_VIDCTL_DOMD);
	/* Hsync Vsync polarity negative */
	dispc_reg_out(VENC_SYNCCTL,
		(VENC_SYNCCTL_SYEV | VENC_SYNCCTL_SYEH | VENC_SYNCCTL_HPL
		| VENC_SYNCCTL_VPL));
	
	/* Clock settings to generate 27 MHz for Philips TPG105 LCD */
	/* Note: DCLK is 27 MHx */
	/* DCLK pattern mode enable */
	dispc_reg_merge(VENC_DCLKCTL,
			1 << VENC_DCLKCTL_DCKEC_SHIFT,
			VENC_DCLKCTL_DCKEC);
	/* 2 valid bits in DCLKPTN0 register */
	dispc_reg_merge(VENC_DCLKCTL,
			0 << VENC_DCLKCTL_DCKPW_SHIFT,
			VENC_DCLKCTL_DCKPW);
	dispc_reg_out(VENC_DCLKPTN0, 0x1);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Change window parameters for interlaced display */
	change_win_param(FB_VMODE_INTERLACED);

	/* set VPSS clock */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

        /* Configure VMOD. No change in VENC bit */
        dispc_reg_out(VENC_VMOD, 0x1442 | venc);
        dispc_reg_out(VENC_LCDOUT, lcdout);
}
void vpbe_dm355fb_ntsc_srgb_config(void)
{
        volatile unsigned int venc, lcdout;
        venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
        lcdout = dispc_reg_in(VENC_LCDOUT);

	/* Clock enable for DLCD controller and VENC controller */ 
	dispc_reg_out(VENC_CLKCTL, 0x011);

	vpbe_set_display_default();
	vpbe_set_display_timing(&dm->videomode);

	/* Set pinmux1 for digital display out */
	dispc_reg_merge(PINMUX1, 1<<GIO71_SHIFT, PINMUX1_GIO71);
	dispc_reg_merge(PINMUX1, 0<<HVSYNC_SHIFT, PINMUX1_HVSYNC);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, 1 << VENC_VDPRO_ATYCC_SHIFT, 
					VENC_VDPRO_ATYCC);
	
	/* Enable VCLKE */ 
	dispc_reg_merge(VENC_VIDCTL, 1 << VENC_VIDCTL_VCLKE_SHIFT,
			VENC_VIDCTL_VCLKE);
	/* VCLK pin output enable */ 
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_VCLKZ_SHIFT,
			VENC_VIDCTL_VCLKZ);
	/* Hsync Vsync pin output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_SYDIR_SHIFT,
			VENC_VIDCTL_SYDIR);
	/* YOUT/COUT output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_YCDIR_SHIFT,
			VENC_VIDCTL_YCDIR);
	/* Digital Data Output Mode is 'normal' */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_DOMD_SHIFT,
			VENC_VIDCTL_DOMD);
	/* Hsync Vsync polarity negative */
	dispc_reg_out(VENC_SYNCCTL,
		(VENC_SYNCCTL_SYEV | VENC_SYNCCTL_SYEH | VENC_SYNCCTL_HPL
		| VENC_SYNCCTL_VPL));
	
	/* Clock settings to generate 27 MHz for Philips TPG105 LCD */
	/* Note: DCLK is 27 MHx */
	/* DCLK pattern mode enable */
	dispc_reg_merge(VENC_DCLKCTL,
			1 << VENC_DCLKCTL_DCKEC_SHIFT,
			VENC_DCLKCTL_DCKEC);
	/* 2 valid bits in DCLKPTN0 register */
	dispc_reg_merge(VENC_DCLKCTL,
			0 << VENC_DCLKCTL_DCKPW_SHIFT,
			VENC_DCLKCTL_DCKPW);
	dispc_reg_out(VENC_DCLKPTN0, 0x1);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Change window parameters for interlaced display */
	change_win_param(FB_VMODE_INTERLACED);

	/* set VPSS clock */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

        /* Configure VMOD. No change in VENC bit */
        dispc_reg_out(VENC_VMOD, 0x3402 | venc);
        dispc_reg_out(VENC_LCDOUT, lcdout);
}
/* PAL interlaced RGB8 standard LCD display */
void vpbe_dm355fb_pal_srgb_config(void)
{
        volatile unsigned int venc, lcdout;
        venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
        lcdout = dispc_reg_in(VENC_LCDOUT);

	/* Clock enable for DLCD controller and VENC controller */ 
	dispc_reg_out(VENC_CLKCTL, 0x011);

	vpbe_set_display_default();
	vpbe_set_display_timing(&dm->videomode);

	/* Set pinmux1 for digital display out */
	dispc_reg_merge(PINMUX1, 1<<GIO71_SHIFT, PINMUX1_GIO71);
	dispc_reg_merge(PINMUX1, 0<<HVSYNC_SHIFT, PINMUX1_HVSYNC);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* VDPRO settings */
	dispc_reg_merge(VENC_VDPRO, 1 << VENC_VDPRO_ATYCC_SHIFT, 
					VENC_VDPRO_ATYCC);
	
	/* Enable VCLKE */ 
	dispc_reg_merge(VENC_VIDCTL, 1 << VENC_VIDCTL_VCLKE_SHIFT,
			VENC_VIDCTL_VCLKE);
	/* VCLK pin output enable */ 
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_VCLKZ_SHIFT,
			VENC_VIDCTL_VCLKZ);
	/* Hsync Vsync pin output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_SYDIR_SHIFT,
			VENC_VIDCTL_SYDIR);
	/* YOUT/COUT output */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_YCDIR_SHIFT,
			VENC_VIDCTL_YCDIR);
	/* Digital Data Output Mode is 'normal' */
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_DOMD_SHIFT,
			VENC_VIDCTL_DOMD);
	/* Hsync Vsync polarity negative */
	dispc_reg_out(VENC_SYNCCTL,
		(VENC_SYNCCTL_SYEV | VENC_SYNCCTL_SYEH | VENC_SYNCCTL_HPL
		| VENC_SYNCCTL_VPL));
	
	/* Clock settings to generate 27 MHz for Philips TPG105 LCD */
	/* Note: DCLK is 27 MHx */
	/* DCLK pattern mode enable */
	dispc_reg_merge(VENC_DCLKCTL,
			1 << VENC_DCLKCTL_DCKEC_SHIFT,
			VENC_DCLKCTL_DCKEC);
	/* 2 valid bits in DCLKPTN0 register */
	dispc_reg_merge(VENC_DCLKCTL,
			0 << VENC_DCLKCTL_DCKPW_SHIFT,
			VENC_DCLKCTL_DCKPW);
	dispc_reg_out(VENC_DCLKPTN0, 0x1);
	dispc_reg_out(VENC_OSDCLK0, 1);
	dispc_reg_out(VENC_OSDCLK1, 2);

	/* Change window parameters for interlaced display */
	change_win_param(FB_VMODE_INTERLACED);

	/* set VPSS clock */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

        /* Configure VMOD. No change in VENC bit */
        dispc_reg_out(VENC_VMOD, 0x3442 | venc);
        dispc_reg_out(VENC_LCDOUT, lcdout);
}

/*
 * vpbe_dm355fb_480p_prgb_config 
 */
void vpbe_dm355fb_480p_prgb_config(void)
{
	unsigned int venc, lcdout;

	venc = dispc_reg_in(VENC_VMOD) & VENC_VMOD_VENC;
	lcdout = dispc_reg_in(VENC_LCDOUT);
	vpbe_set_display_default();

	/* set VPSS clock */
	dispc_reg_out(VPSS_CLK_CTRL, 0x18);

	/* Enable the venc and dlcd clocks. */
	dispc_reg_out(VENC_CLKCTL, 0x11);

	/* Set the OSD Divisor to 1. */
	dispc_reg_out(VENC_OSDCLK0, 0);
	dispc_reg_out(VENC_OSDCLK1, 1);

	/* Clear composite mode register */
	dispc_reg_out(VENC_CVBS, 0);

	/* Set PINMUX1 to enable all outputs needed to support RGB666 */
	dispc_reg_out(PINMUX1, 0x00005555);

	/* Set Base Pixel X and Base Pixel Y */
	dispc_reg_out(OSD_BASEPX, dm->videomode.basepx);
	dispc_reg_out(OSD_BASEPY, dm->videomode.basepy);

	/* Set VIDCTL to select VCLKE = 1, 
	VCLKZ =0, SYDIR = 0 (set o/p), DOMD = 0 */
	dispc_reg_merge(VENC_VIDCTL, 1 << VENC_VIDCTL_VCLKE_SHIFT,
			VENC_VIDCTL_VCLKE);
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_VCLKZ_SHIFT,
			VENC_VIDCTL_VCLKZ);
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_SYDIR_SHIFT,
			VENC_VIDCTL_SYDIR);
	dispc_reg_merge(VENC_VIDCTL, 0 << VENC_VIDCTL_YCDIR_SHIFT,
			VENC_VIDCTL_YCDIR);

	dispc_reg_merge(VENC_DCLKCTL,
			1 << VENC_DCLKCTL_DCKEC_SHIFT,
			VENC_DCLKCTL_DCKEC);

	dispc_reg_out(VENC_DCLKPTN0, 0x1);

	vpbe_set_display_timing(&dm->videomode);
	dispc_reg_out(VENC_SYNCCTL,
		(VENC_SYNCCTL_SYEV |
		VENC_SYNCCTL_SYEH | VENC_SYNCCTL_HPL
		| VENC_SYNCCTL_VPL));

	/* Configure VMOD. No change in VENC bit */
	dispc_reg_out(VENC_VMOD, 0x2010 | venc);
	dispc_reg_out(VENC_LCDOUT, lcdout);

	/* Change window parameters for progressive display */
	change_win_param(FB_VMODE_NONINTERLACED);
}

/* Following functions are not implemented */
void vpbe_dm355fb_default_ycc16_config(void)
{
}
void vpbe_dm355fb_default_prgb_config(void)
{
}
