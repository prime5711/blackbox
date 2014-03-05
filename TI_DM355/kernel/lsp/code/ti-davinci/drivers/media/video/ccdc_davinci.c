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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* ccdc_davinci.c */
#define DEBUG
#include <linux/device.h>
#include <media/ccdc_davinci.h>

extern struct device *vpfe_dev;

void ccdc_readregs(void)
{
	unsigned int val = 0;

	val = regr(ALAW);
	dev_dbg(vpfe_dev, "\nReading 0x%x to ALAW...\n", val);
	val = regr(CLAMP);
	dev_dbg(vpfe_dev, "\nReading 0x%x to CLAMP...\n", val);
	val = regr(DCSUB);
	dev_dbg(vpfe_dev, "\nReading 0x%x to DCSUB...\n", val);
	val = regr(BLKCMP);
	dev_dbg(vpfe_dev, "\nReading 0x%x to BLKCMP...\n", val);
	val = regr(FPC_ADDR);
	dev_dbg(vpfe_dev, "\nReading 0x%x to FPC_ADDR...\n", val);
	val = regr(FPC);
	dev_dbg(vpfe_dev, "\nReading 0x%x to FPC...\n", val);
	val = regr(FMTCFG);
	dev_dbg(vpfe_dev, "\nReading 0x%x to FMTCFG...\n", val);
	val = regr(COLPTN);
	dev_dbg(vpfe_dev, "\nReading 0x%x to COLPTN...\n", val);
	val = regr(FMT_HORZ);
	dev_dbg(vpfe_dev, "\nReading 0x%x to FMT_HORZ...\n", val);
	val = regr(FMT_VERT);
	dev_dbg(vpfe_dev, "\nReading 0x%x to FMT_VERT...\n", val);
	val = regr(HSIZE_OFF);
	dev_dbg(vpfe_dev, "\nReading 0x%x to HSIZE_OFF...\n", val);
	val = regr(SDOFST);
	dev_dbg(vpfe_dev, "\nReading 0x%x to SDOFST...\n", val);
	val = regr(VP_OUT);
	dev_dbg(vpfe_dev, "\nReading 0x%x to VP_OUT...\n", val);
	val = regr(SYN_MODE);
	dev_dbg(vpfe_dev, "\nReading 0x%x to SYN_MODE...\n", val);
	val = regr(HORZ_INFO);
	dev_dbg(vpfe_dev, "\nReading 0x%x to HORZ_INFO...\n", val);
	val = regr(VERT_START);
	dev_dbg(vpfe_dev, "\nReading 0x%x to VERT_START...\n", val);
	val = regr(VERT_LINES);
	dev_dbg(vpfe_dev, "\nReading 0x%x to VERT_LINES...\n", val);
}

/*                                                       
 * ======== ccdc_reset  ========                        
 */
/*This function will reset all CCDc reg */

void ccdc_reset()
{
	int i;
	/* disable CCDC */
	ccdc_enable(0);
	/* set all registers to default value */
	for (i = 0; i <= 0x94; i += 4) {
		regw(0, i);
	}
	regw(0, PCR);
	regw(0, SYN_MODE);
	regw(0, HD_VD_WID);
	regw(0, PIX_LINES);
	regw(0, HORZ_INFO);
	regw(0, VERT_START);
	regw(0, VERT_LINES);
	regw(0xffff00ff, CULLING);
	regw(0, HSIZE_OFF);
	regw(0, SDOFST);
	regw(0, SDR_ADDR);
	regw(0, VDINT);
	regw(0, REC656IF);
	regw(0, CCDCFG);
	regw(0, FMTCFG);
	regw(0, VP_OUT);
}

/*                                                       
 * ======== ccdc_setwin  ========                        
 */
/*This function will configure the window size to be capture in CCDC reg */
void ccdc_setwin(ccdc_imgwin * image_win, ccdc_frmfmt frm_fmt, int ppc)
{
	int horz_start, horz_nr_pixels;
	int vert_start, vert_nr_lines;
	int val = 0, mid_img = 0;
	dev_dbg(vpfe_dev, "\nStarting ccdc_setwin...");
	/* configure horizonal and vertical starts and sizes */
	horz_start = image_win->left << (ppc - 1);
	horz_nr_pixels = (image_win->width << (ppc - 1)) - 1;
	regw((horz_start << CCDC_HORZ_INFO_SPH_SHIFT) | horz_nr_pixels,
	     HORZ_INFO);

	vert_start = image_win->top;

	if (frm_fmt == CCDC_FRMFMT_INTERLACED) {
		vert_nr_lines = (image_win->height >> 1) - 1;
		vert_start >>= 1;
		/* configure VDINT0 and VDINT1 */
		val = (vert_start << CCDC_VDINT_VDINT0_SHIFT);
		regw(val, VDINT);

	} else {
		vert_nr_lines = image_win->height - 1;
		/* configure VDINT0 and VDINT1 */
		mid_img = vert_start + (image_win->height / 2);
		val = (vert_start << CCDC_VDINT_VDINT0_SHIFT) |
		    (mid_img & CCDC_VDINT_VDINT1_MASK);
		regw(val, VDINT);

	}
	regw((vert_start << CCDC_VERT_START_SLV0_SHIFT) | vert_start,
	     VERT_START);
	regw(vert_nr_lines, VERT_LINES);
	dev_dbg(vpfe_dev, "\nEnd of ccdc_setwin...");
}

/*                                
 * ======== ccdc_config_ycbcr  ======== 
 */
/*This function will configure CCDC for YCbCr parameters*/
void ccdc_config_ycbcr(ccdc_params_ycbcr * params)
{
	u32 syn_mode;
	unsigned int val;

	/* first reset the CCDC                                          */
	/* all registers have default values after reset                 */
	/* This is important since we assume default values to be set in */
	/* a lot of registers that we didn't touch                       */
	dev_dbg(vpfe_dev, "\nStarting ccdc_config_ycbcr...");
	ccdc_reset();

	/* configure pixel format */
	syn_mode = (params->pix_fmt & 0x3) << 12;

	/* configure video frame format */
	syn_mode |= (params->frm_fmt & 0x1) << 7;

	/* setup BT.656 sync mode */
	if (params->bt656_enable) {
		regw(3, REC656IF);

		/* configure the FID, VD, HD pin polarity */
		/* fld,hd pol positive, vd negative, 8-bit pack mode */
		syn_mode |= 0x00000F04;
	} else {		/* y/c external sync mode */
		syn_mode |= ((params->fid_pol & 0x1) << 4);
		syn_mode |= ((params->hd_pol & 0x1) << 3);
		syn_mode |= ((params->vd_pol & 0x1) << 2);
	}

	/* configure video window */
	ccdc_setwin(&params->win, params->frm_fmt, 2);

	/* configure the order of y cb cr in SD-RAM */
	regw((params->pix_order << 11) | 0x8000, CCDCFG);

	/* configure the horizontal line offset */
	/* this is done by rounding up width to a multiple of 16 pixels */
	/* and multiply by two to account for y:cb:cr 4:2:2 data */
	regw(((params->win.width * 2) + 31) & 0xffffffe0, HSIZE_OFF);

	/* configure the memory line offset */
	if (params->buf_type == CCDC_BUFTYPE_FLD_INTERLEAVED) {
		/* two fields are interleaved in memory */
		regw(0x00000249, SDOFST);
	}
	/* enable output to SDRAM */
	syn_mode |= (0x1 << 17);
	/* enable internal timing generator */
	syn_mode |= (0x1 << 16);

	syn_mode |= CCDC_DATA_PACK_ENABLE;
	regw(syn_mode, SYN_MODE);

	val = (unsigned int)ccdc_sbl_reset();
	dev_dbg(vpfe_dev, "\nReading 0x%x from SBL...\n", val);

	dev_dbg(vpfe_dev, "\nEnd of ccdc_config_ycbcr...\n");
	ccdc_readregs();
}

/*                                                        
 * ======== ccdc_config_raw  ========                   
 */
/*This function will configure CCDC for Raw mode parameters*/
void ccdc_config_raw(ccdc_params_raw * params)
{

	unsigned int syn_mode = 0;
	unsigned int val;
	dev_dbg(vpfe_dev, "\nStarting ccdc_config_raw...");
	/*      Reset CCDC */
	ccdc_reset();
	/* Disable latching function registers on VSYNC  */
	regw(CCDC_LATCH_ON_VSYNC_DISABLE, CCDCFG);

	/*      Configure the vertical sync polarity(SYN_MODE.VDPOL) */
	syn_mode = (params->vd_pol & CCDC_VD_POL_MASK) << CCDC_VD_POL_SHIFT;

	/*      Configure the horizontal sync polarity (SYN_MODE.HDPOL) */
	syn_mode |= (params->hd_pol & CCDC_HD_POL_MASK) << CCDC_HD_POL_SHIFT;

	/*      Configure frame id polarity (SYN_MODE.FLDPOL) */
	syn_mode |= (params->fid_pol & CCDC_FID_POL_MASK) << CCDC_FID_POL_SHIFT;

	/* Configure frame format(progressive or interlace) */
	syn_mode |= (params->frm_fmt & CCDC_FRM_FMT_MASK) << CCDC_FRM_FMT_SHIFT;

	/* Configure the data size(SYNMODE.DATSIZ) */
	syn_mode |= (params->data_sz & CCDC_DATA_SZ_MASK) << CCDC_DATA_SZ_SHIFT;

	/* Configure pixel format (Input mode) */
	syn_mode |= (params->pix_fmt & CCDC_PIX_FMT_MASK) << CCDC_PIX_FMT_SHIFT;

	/* Configure VP2SDR bit of syn_mode = 0 */
	syn_mode &= CCDC_VP2SDR_DISABLE;
	syn_mode |= ~CCDC_VP2SDR_DISABLE; /* enable VP2SDR */

	/* Enable write enable bit */
	syn_mode |= CCDC_WEN_ENABLE;

	/* Disable output to resizer */
	syn_mode &= CCDC_SDR2RSZ_DISABLE;

	/* enable internal timing generator */
	syn_mode |= CCDC_VDHDEN_ENABLE;

	/* Enable and configure aLaw register if needed */
	if (params->alaw.b_alaw_enable) {
		val = (params->alaw.gama_wd & CCDC_ALAW_GAMA_WD_MASK);
		val |= CCDC_ALAW_ENABLE;	/*set enable bit of alaw */
		regw(val, ALAW);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to ALAW...\n", val);
	}

	/* configure video window */
	ccdc_setwin(&params->win, params->frm_fmt, PPC_RAW);

	if (params->blk_clamp.b_clamp_enable) {
		val = (params->blk_clamp.sgain) & CCDC_BLK_SGAIN_MASK;	/*gain */
		val |= (params->blk_clamp.start_pixel & CCDC_BLK_ST_PXL_MASK)
		    << CCDC_BLK_ST_PXL_SHIFT;	/*Start pixel */
		val |= (params->blk_clamp.sample_ln & CCDC_BLK_SAMPLE_LINE_MASK)
		    << CCDC_BLK_SAMPLE_LINE_SHIFT;	/*No of line to be avg */
		val |=
		    (params->blk_clamp.sample_pixel & CCDC_BLK_SAMPLE_LN_MASK)
		    << CCDC_BLK_SAMPLE_LN_SHIFT;	/*No of pixel/line to be avg */
		val |= CCDC_BLK_CLAMP_ENABLE;	/*Enable the Black clamping */
		regw(val, CLAMP);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to CLAMP...\n", val);
		regw(DCSUB_DEFAULT_VAL, DCSUB);	/*If Black clamping is enable 
						   then make dcsub 0 */
		dev_dbg(vpfe_dev, "\nWriting 0x00000000 to DCSUB...\n");

	} else {
		/* configure DCSub */
		val = (params->blk_clamp.dc_sub) & CCDC_BLK_DC_SUB_MASK;
		regw(val, DCSUB);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to DCSUB...\n", val);
		regw(CLAMP_DEFAULT_VAL, CLAMP);

		dev_dbg(vpfe_dev, "\nWriting 0x0000 to CLAMP...\n");
	}

	/*      Configure Black level compensation */
	val = (params->blk_comp.b_comp & CCDC_BLK_COMP_MASK);
	val |= (params->blk_comp.gb_comp & CCDC_BLK_COMP_MASK)
	    << CCDC_BLK_COMP_GB_COMP_SHIFT;
	val |= (params->blk_comp.gr_comp & CCDC_BLK_COMP_MASK)
	    << CCDC_BLK_COMP_GR_COMP_SHIFT;
	val |= (params->blk_comp.r_comp & CCDC_BLK_COMP_MASK)
	    << CCDC_BLK_COMP_R_COMP_SHIFT;

	regw(val, BLKCMP);

	dev_dbg(vpfe_dev, "\nWriting 0x%x to BLKCMP...\n", val);
	dev_dbg(vpfe_dev, "\nbelow 	regw(val, BLKCMP)...");
	/* Configure Fault pixel if needed */
	if (params->fault_pxl.fpc_enable) {
		regw(params->fault_pxl.fpc_table_addr, FPC_ADDR);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to FPC_ADDR...\n",
			(params->fault_pxl.fpc_table_addr));
		/* Write the FPC params with FPC disable */
		val = params->fault_pxl.fp_num & CCDC_FPC_FPC_NUM_MASK;
		regw(val, FPC);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to FPC...\n", val);
		/* read the FPC register */
		val = regr(FPC);
		val |= CCDC_FPC_ENABLE;
		regw(val, FPC);

		dev_dbg(vpfe_dev, "\nWriting 0x%x to FPC...\n", val);
	}
	/* If data size is 8 bit then pack the data */
	if ((params->data_sz == _8BITS) || params->alaw.b_alaw_enable) {
		syn_mode |= CCDC_DATA_PACK_ENABLE;
	}
#if VIDEO_PORT_ENABLE
	val = ENABLE_VIDEO_PORT;	/* enable video port */
#else
	val = DISABLE_VIDEO_PORT;	/* disable video port */
#endif

	if (params->data_sz == _8BITS)
		val |= (_10BITS & CCDC_FMTCFG_VPIN_MASK)
		    << CCDC_FMTCFG_VPIN_SHIFT;
	else
		val |= (params->data_sz & CCDC_FMTCFG_VPIN_MASK)
		    << CCDC_FMTCFG_VPIN_SHIFT;

	/* Write value in FMTCFG */
	regw(val, FMTCFG);

	dev_dbg(vpfe_dev, "\nWriting 0x%x to FMTCFG...\n", val);

	/* Configure the color pattern according to mt9t001 sensor */
	regw(CCDC_COLPTN_VAL, COLPTN);

	dev_dbg(vpfe_dev, "\nWriting 0xBB11BB11 to COLPTN...\n");
	/* Configure Data formatter(Video port) pixel selection 
	 * (FMT_HORZ, FMT_VERT) 
	 */
	val = 0;
	val |= ((params->win.left) & CCDC_FMT_HORZ_FMTSPH_MASK)
	    << CCDC_FMT_HORZ_FMTSPH_SHIFT;
	val |= (((params->win.width)) & CCDC_FMT_HORZ_FMTLNH_MASK);
	regw(val, FMT_HORZ);

	dev_dbg(vpfe_dev, "\nWriting 0x%x to FMT_HORZ...\n", val);
	val = 0;
	val |= (params->win.top & CCDC_FMT_VERT_FMTSLV_MASK)
	    << CCDC_FMT_VERT_FMTSLV_SHIFT;
	if (params->frm_fmt == CCDC_FRMFMT_PROGRESSIVE)
		val |= (params->win.height) & CCDC_FMT_VERT_FMTLNV_MASK;
	else
		val |= (params->win.height >> 1) & CCDC_FMT_VERT_FMTLNV_MASK;

	dev_dbg(vpfe_dev, "\nparams->win.height  0x%x ...\n",
		params->win.height);
	regw(val, FMT_VERT);

	dev_dbg(vpfe_dev, "\nWriting 0x%x to FMT_VERT...\n", val);

	dev_dbg(vpfe_dev, "\nbelow regw(val, FMT_VERT)...");

	/* Configure Horizontal offset register */
	/* If pack 8 is enabled then 1 pixel will take 1 byte */
	if ((params->data_sz == _8BITS) || params->alaw.b_alaw_enable) {
		regw(((params->win.width) + CCDC_32BYTE_ALIGN_VAL)
		     & CCDC_HSIZE_OFF_MASK, HSIZE_OFF);

	} else {		/* else one pixel will take 2 byte */

		regw(((params->win.width * TWO_BYTES_PER_PIXEL)
		      + CCDC_32BYTE_ALIGN_VAL)
		     & CCDC_HSIZE_OFF_MASK, HSIZE_OFF);

	}

	/* Set value for SDOFST */

	if (params->frm_fmt == CCDC_FRMFMT_INTERLACED) {
		if (params->image_invert_enable) {
			/* For intelace inverse mode */
			regw(INTERLACED_IMAGE_INVERT, SDOFST);
			dev_dbg(vpfe_dev, "\nWriting 0x4B6D to SDOFST...\n");
		}

		else {
			/* For intelace non inverse mode */
			regw(INTERLACED_NO_IMAGE_INVERT, SDOFST);
			dev_dbg(vpfe_dev, "\nWriting 0x0249 to SDOFST...\n");
		}
	} else if (params->frm_fmt == CCDC_FRMFMT_PROGRESSIVE) {
		regw(PROGRESSIVE_NO_IMAGE_INVERT, SDOFST);
		dev_dbg(vpfe_dev, "\nWriting 0x0000 to SDOFST...\n");
	}

	/* Configure video port pixel selection (VPOUT) */
	/* Here -1 is to make the height value less than FMT_VERT.FMTLNV */
	if (params->frm_fmt == CCDC_FRMFMT_PROGRESSIVE) {
		val = (((params->win.height - 1) & CCDC_VP_OUT_VERT_NUM_MASK))
		    << CCDC_VP_OUT_VERT_NUM_SHIFT;
	} else {
		val =
		    ((((params->win.
			height >> CCDC_INTERLACED_HEIGHT_SHIFT) -
		       1) & CCDC_VP_OUT_VERT_NUM_MASK))
		    << CCDC_VP_OUT_VERT_NUM_SHIFT;
	}

	val |= ((((params->win.width))) & CCDC_VP_OUT_HORZ_NUM_MASK)
	    << CCDC_VP_OUT_HORZ_NUM_SHIFT;
	val |= (params->win.left) & CCDC_VP_OUT_HORZ_ST_MASK;
	regw(val, VP_OUT);

	dev_dbg(vpfe_dev, "\nWriting 0x%x to VP_OUT...\n", val);
	regw(syn_mode, SYN_MODE);
	dev_dbg(vpfe_dev, "\nWriting 0x%x to SYN_MODE...\n", syn_mode);

	val = (unsigned int)ccdc_sbl_reset();
	dev_dbg(vpfe_dev, "\nReading 0x%x from SBL...\n", val);

	dev_dbg(vpfe_dev, "\nend of ccdc_config_raw...");
	ccdc_readregs();
}
