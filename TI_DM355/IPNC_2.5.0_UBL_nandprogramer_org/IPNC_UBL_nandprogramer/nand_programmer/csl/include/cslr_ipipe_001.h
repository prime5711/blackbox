#ifndef _CSLR_IPIPE_IPIPE_SPEC_V0113_H_
#define _CSLR_IPIPE_IPIPE_SPEC_V0113_H_
/*********************************************************************
 * Copyright (C) 2003-2006 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_ipipe_ipipe_spec_v0113.h
 * 
 * \brief This file contains the Register Desciptions for IPIPE
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure for D2F_THR 
\**************************************************************************/
typedef struct  {
    volatile Uint16 THR;
    volatile Uint8 RSVD0[2];
} CSL_IpipeD2f_thrRegs;

/**************************************************************************\
* Register Overlay Structure for D2F_STR 
\**************************************************************************/
typedef struct  {
    volatile Uint16 STR;
    volatile Uint8 RSVD0[2];
} CSL_IpipeD2f_strRegs;

/**************************************************************************\
* Register Overlay Structure for RSZ 
\**************************************************************************/
typedef struct  {
    volatile Uint16 RSZ_EN;
    volatile Uint8 RSVD0[2];
    volatile Uint16 RSZ_MODE;
    volatile Uint8 RSVD1[2];
    volatile Uint16 RSZ_I_VST;
    volatile Uint8 RSVD2[2];
    volatile Uint16 RSZ_I_VSZ;
    volatile Uint8 RSVD3[2];
    volatile Uint16 RSZ_I_HST;
    volatile Uint8 RSVD4[2];
    volatile Uint16 RSZ_O_VSZ;
    volatile Uint8 RSVD5[2];
    volatile Uint16 RSZ_O_HST;
    volatile Uint8 RSVD6[2];
    volatile Uint16 RSZ_O_HSZ;
    volatile Uint8 RSVD7[2];
    volatile Uint16 RSZ_V_PHS;
    volatile Uint8 RSVD8[2];
    volatile Uint16 RSZ_V_PHS_O;
    volatile Uint8 RSVD9[2];
    volatile Uint16 RSZ_V_DIF;
    volatile Uint8 RSVD10[2];
    volatile Uint16 RSZ_V_SIZ_O;
    volatile Uint8 RSVD11[2];
    volatile Uint16 RSZ_H_PHS;
    volatile Uint8 RSVD12[2];
    volatile Uint16 RSZ_H_DIF;
    volatile Uint8 RSVD13[2];
    volatile Uint16 RSZ_H_TYP;
    volatile Uint8 RSVD14[2];
    volatile Uint16 RSZ_H_LSE;
    volatile Uint8 RSVD15[2];
    volatile Uint16 RSZ_H_LPF;
    volatile Uint8 RSVD16[2];
    volatile Uint16 RSZ_RGB_EN;
    volatile Uint8 RSVD17[2];
    volatile Uint16 RSZ_RGB_TYP;
    volatile Uint8 RSVD18[2];
    volatile Uint16 RSZ_RGB_BLD;
    volatile Uint8 RSVD19[2];
    volatile Uint16 RSZ_SDR_BAD_H;
    volatile Uint8 RSVD20[2];
    volatile Uint16 RSZ_SDR_BAD_L;
    volatile Uint8 RSVD21[2];
    volatile Uint16 RSZ_SDR_SAD_H;
    volatile Uint8 RSVD22[2];
    volatile Uint16 RSZ_SDR_SAD_L;
    volatile Uint8 RSVD23[2];
    volatile Uint16 RSZ_SDR_OFT;
    volatile Uint8 RSVD24[2];
    volatile Uint16 RSZ_SDR_PTR_S;
    volatile Uint8 RSVD25[2];
    volatile Uint16 RSZ_SDR_PTR_E;
    volatile Uint8 RSVD26[2];
    volatile Uint16 RSZ_SDR_PTR_O;
    volatile Uint8 RSVD27[2];
} CSL_IpipeRszRegs;

/**************************************************************************\
* Register Overlay Structure for HST_RGN 
\**************************************************************************/
typedef struct  {
    volatile Uint16 HST_VST;
    volatile Uint8 RSVD0[2];
    volatile Uint16 HST_VSZ;
    volatile Uint8 RSVD1[2];
    volatile Uint16 HST_HST;
    volatile Uint8 RSVD2[2];
    volatile Uint16 HST_HSZ;
    volatile Uint8 RSVD3[2];
} CSL_IpipeHst_rgnRegs;

/**************************************************************************\
* Register Overlay Structure for BSC_ROW_SUM 
\**************************************************************************/
typedef struct  {
    volatile Uint16 BSC_ROW_SUM;
} CSL_IpipeBsc_row_sumRegs;

/**************************************************************************\
* Register Overlay Structure for BSC_COL_SUM 
\**************************************************************************/
typedef struct  {
    volatile Uint16 BSC_COL_SUM;
} CSL_IpipeBsc_col_sumRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 IPIPE_EN;
    volatile Uint8 RSVD0[2];
    volatile Uint16 IPIPE_MODE;
    volatile Uint8 RSVD1[2];
    volatile Uint16 IPIPE_DPATHS;
    volatile Uint8 RSVD2[2];
    volatile Uint16 IPIPE_COLPAT;
    volatile Uint8 RSVD3[2];
    volatile Uint16 IPIPE_VST;
    volatile Uint8 RSVD4[2];
    volatile Uint16 IPIPE_VSZ;
    volatile Uint8 RSVD5[2];
    volatile Uint16 IPIPE_HST;
    volatile Uint8 RSVD6[2];
    volatile Uint16 IPIPE_HSZ;
    volatile Uint8 RSVD7[2];
    volatile Uint16 IPIPE_STA;
    volatile Uint8 RSVD8[2];
    volatile Uint16 GCL_ARM;
    volatile Uint8 RSVD9[2];
    volatile Uint16 GCL_CCD;
    volatile Uint8 RSVD10[2];
    volatile Uint16 GCL_SDR;
    volatile Uint8 RSVD11[2];
    volatile Uint16 RAM_MODE;
    volatile Uint8 RSVD12[2];
    volatile Uint16 RAM_ADR;
    volatile Uint8 RSVD13[2];
    volatile Uint16 RAM_WDT;
    volatile Uint8 RSVD14[2];
    volatile Uint16 RAM_RDT;
    volatile Uint8 RSVD15[2];
    volatile Uint16 IRQ_EN;
    volatile Uint8 RSVD16[2];
    volatile Uint16 IRQ_RZA;
    volatile Uint8 RSVD17[2];
    volatile Uint16 IRQ_RZB;
    volatile Uint8 RSVD18[2];
    volatile Uint16 DFC_EN;
    volatile Uint8 RSVD19[2];
    volatile Uint16 DFC_SEL;
    volatile Uint8 RSVD20[2];
    volatile Uint16 DFC_ADR;
    volatile Uint8 RSVD21[2];
    volatile Uint16 DFC_SIZ;
    volatile Uint8 RSVD22[2];
    volatile Uint16 D2F_EN;
    volatile Uint8 RSVD23[2];
    volatile Uint16 D2F_CFG;
    volatile Uint8 RSVD24[2];
    CSL_IpipeD2f_thrRegs D2F_THR[32];
    CSL_IpipeD2f_strRegs D2F_STR[32];
    volatile Uint16 PRE_EN;
    volatile Uint8 RSVD25[2];
    volatile Uint16 PRE_TYP;
    volatile Uint8 RSVD26[2];
    volatile Uint16 PRE_SHF;
    volatile Uint8 RSVD27[2];
    volatile Uint16 PRE_GAIN;
    volatile Uint8 RSVD28[2];
    volatile Uint16 PRE_THR_G;
    volatile Uint8 RSVD29[2];
    volatile Uint16 PRE_THR_B;
    volatile Uint8 RSVD30[2];
    volatile Uint16 PRE_THR_1;
    volatile Uint8 RSVD31[2];
    volatile Uint16 WB2_DGN;
    volatile Uint8 RSVD32[2];
    volatile Uint16 WB2_WG_R;
    volatile Uint8 RSVD33[2];
    volatile Uint16 WB2_WG_GR;
    volatile Uint8 RSVD34[2];
    volatile Uint16 WB2_WG_GB;
    volatile Uint8 RSVD35[2];
    volatile Uint16 WB2_WG_B;
    volatile Uint8 RSVD36[98];
    volatile Uint16 RGB_MUL_RR;
    volatile Uint8 RSVD37[2];
    volatile Uint16 RGB_MUL_GR;
    volatile Uint8 RSVD38[2];
    volatile Uint16 RGB_MUL_BR;
    volatile Uint8 RSVD39[2];
    volatile Uint16 RGB_MUL_RG;
    volatile Uint8 RSVD40[2];
    volatile Uint16 RGB_MUL_GG;
    volatile Uint8 RSVD41[2];
    volatile Uint16 RGB_MUL_BG;
    volatile Uint8 RSVD42[2];
    volatile Uint16 RGB_MUL_RB;
    volatile Uint8 RSVD43[2];
    volatile Uint16 RGB_MUL_GB;
    volatile Uint8 RSVD44[2];
    volatile Uint16 RGB_MUL_BB;
    volatile Uint8 RSVD45[2];
    volatile Uint16 RGB_OFT_OR;
    volatile Uint8 RSVD46[2];
    volatile Uint16 RGB_OFT_OG;
    volatile Uint8 RSVD47[2];
    volatile Uint16 RGB_OFT_OB;
    volatile Uint8 RSVD48[2];
    volatile Uint16 GMM_CFG;
    volatile Uint8 RSVD49[2];
    volatile Uint16 YUV_ADJ;
    volatile Uint8 RSVD50[2];
    volatile Uint16 YUV_MUL_RY;
    volatile Uint8 RSVD51[2];
    volatile Uint16 YUV_MUL_GY;
    volatile Uint8 RSVD52[2];
    volatile Uint16 YUV_MUL_BY;
    volatile Uint8 RSVD53[2];
    volatile Uint16 YUV_MUL_RCB;
    volatile Uint8 RSVD54[2];
    volatile Uint16 YUV_MUL_GCB;
    volatile Uint8 RSVD55[2];
    volatile Uint16 YUV_MUL_BCB;
    volatile Uint8 RSVD56[2];
    volatile Uint16 YUV_MUL_RCR;
    volatile Uint8 RSVD57[2];
    volatile Uint16 YUV_MUL_GCR;
    volatile Uint8 RSVD58[2];
    volatile Uint16 YUV_MUL_BCR;
    volatile Uint8 RSVD59[2];
    volatile Uint16 YUV_OFT_Y;
    volatile Uint8 RSVD60[2];
    volatile Uint16 YUV_OFT_CB;
    volatile Uint8 RSVD61[2];
    volatile Uint16 YUV_OFT_CR;
    volatile Uint8 RSVD62[2];
    volatile Uint16 YUV_Y_MIN;
    volatile Uint8 RSVD63[2];
    volatile Uint16 YUV_Y_MAX;
    volatile Uint8 RSVD64[2];
    volatile Uint16 YUV_C_MIN;
    volatile Uint8 RSVD65[2];
    volatile Uint16 YUV_C_MAX;
    volatile Uint8 RSVD66[2];
    volatile Uint16 YUV_PHS;
    volatile Uint8 RSVD67[2];
    volatile Uint16 YEE_EN;
    volatile Uint8 RSVD68[2];
    volatile Uint16 YEE_EMF;
    volatile Uint8 RSVD69[2];
    volatile Uint16 YEE_SHF;
    volatile Uint8 RSVD70[2];
    volatile Uint16 YEE_MUL_00;
    volatile Uint8 RSVD71[2];
    volatile Uint16 YEE_MUL_01;
    volatile Uint8 RSVD72[2];
    volatile Uint16 YEE_MUL_02;
    volatile Uint8 RSVD73[2];
    volatile Uint16 YEE_MUL_10;
    volatile Uint8 RSVD74[2];
    volatile Uint16 YEE_MUL_11;
    volatile Uint8 RSVD75[2];
    volatile Uint16 YEE_MUL_12;
    volatile Uint8 RSVD76[2];
    volatile Uint16 YEE_MUL_20;
    volatile Uint8 RSVD77[2];
    volatile Uint16 YEE_MUL_21;
    volatile Uint8 RSVD78[2];
    volatile Uint16 YEE_MUL_22;
    volatile Uint8 RSVD79[2];
    volatile Uint16 FCS_EN;
    volatile Uint8 RSVD80[2];
    volatile Uint16 FCS_TYP;
    volatile Uint8 RSVD81[2];
    volatile Uint16 FCS_SHF_Y;
    volatile Uint8 RSVD82[2];
    volatile Uint16 FCS_SHF_C;
    volatile Uint8 RSVD83[2];
    volatile Uint16 FCS_THR;
    volatile Uint8 RSVD84[2];
    volatile Uint16 FCS_SGN;
    volatile Uint8 RSVD85[2];
    volatile Uint16 FCS_LTH;
    volatile Uint8 RSVD86[2];
    volatile Uint16 RSZ_SEQ;
    volatile Uint8 RSVD87[2];
    volatile Uint16 RSZ_AAL;
    volatile Uint8 RSVD88[2];
    CSL_IpipeRszRegs RSZ[2];
    volatile Uint16 BOX_EN;
    volatile Uint8 RSVD89[2];
    volatile Uint16 BOX_MODE;
    volatile Uint8 RSVD90[2];
    volatile Uint16 BOX_TYP;
    volatile Uint8 RSVD91[2];
    volatile Uint16 BOX_SHF;
    volatile Uint8 RSVD92[2];
    volatile Uint16 HST_EN;
    volatile Uint8 RSVD93[2];
    volatile Uint16 HST_MODE;
    volatile Uint8 RSVD94[2];
    volatile Uint16 HST_SEL;
    volatile Uint8 RSVD95[2];
    volatile Uint16 HST_PARA;
    volatile Uint8 RSVD96[2];
    CSL_IpipeHst_rgnRegs HST_RGN[4];
    volatile Uint16 BSC_EN;
    volatile Uint8 RSVD97[2];
    volatile Uint16 BSC_MODE;
    volatile Uint8 RSVD98[2];
    volatile Uint16 BSC_TYP;
    volatile Uint8 RSVD99[2];
    volatile Uint16 BSC_ROW_VCT;
    volatile Uint8 RSVD100[2];
    volatile Uint16 BSC_ROW_SHF;
    volatile Uint8 RSVD101[2];
    volatile Uint16 BSC_ROW_VPOS;
    volatile Uint8 RSVD102[2];
    volatile Uint16 BSC_ROW_VNUM;
    volatile Uint8 RSVD103[2];
    volatile Uint16 BSC_ROW_VSKIP;
    volatile Uint8 RSVD104[2];
    volatile Uint16 BSC_ROW_HPOS;
    volatile Uint8 RSVD105[2];
    volatile Uint16 BSC_ROW_HNUM;
    volatile Uint8 RSVD106[2];
    volatile Uint16 BSC_ROW_HSKIP;
    volatile Uint8 RSVD107[2];
    volatile Uint16 BSC_COL_VCT;
    volatile Uint8 RSVD108[2];
    volatile Uint16 BSC_COL_SHF;
    volatile Uint8 RSVD109[2];
    volatile Uint16 BSC_COL_VPOS;
    volatile Uint8 RSVD110[2];
    volatile Uint16 BSC_COL_VNUM;
    volatile Uint8 RSVD111[2];
    volatile Uint16 BSC_COL_VSKIP;
    volatile Uint8 RSVD112[2];
    volatile Uint16 BSC_COL_HPOS;
    volatile Uint8 RSVD113[2];
    volatile Uint16 BSC_COL_HNUM;
    volatile Uint8 RSVD114[2];
    volatile Uint16 BSC_COL_HSKIP;
    volatile Uint8 RSVD115[2994];
    CSL_IpipeBsc_row_sumRegs BSC_ROW_SUM[1920];
    volatile Uint8 RSVD116[256];
    CSL_IpipeBsc_col_sumRegs BSC_COL_SUM[1920];
} CSL_IpipeRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* IPIPE_EN */

#define CSL_IPIPE_IPIPE_EN_EN_MASK       (0x0001u)
#define CSL_IPIPE_IPIPE_EN_EN_SHIFT      (0x0000u)
#define CSL_IPIPE_IPIPE_EN_EN_RESETVAL   (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_IPIPE_EN_EN_DISABLE    (0x0000u)
#define CSL_IPIPE_IPIPE_EN_EN_ENABLE     (0x0001u)

#define CSL_IPIPE_IPIPE_EN_RESETVAL      (0x0000u)

/* IPIPE_MODE */

#define CSL_IPIPE_IPIPE_MODE_WRT_MASK    (0x0002u)
#define CSL_IPIPE_IPIPE_MODE_WRT_SHIFT   (0x0001u)
#define CSL_IPIPE_IPIPE_MODE_WRT_RESETVAL (0x0000u)

/*----WRT Tokens----*/
#define CSL_IPIPE_IPIPE_MODE_WRT_DISABLE (0x0000u)
#define CSL_IPIPE_IPIPE_MODE_WRT_ENABLE  (0x0001u)

#define CSL_IPIPE_IPIPE_MODE_ONESHOT_MASK (0x0001u)
#define CSL_IPIPE_IPIPE_MODE_ONESHOT_SHIFT (0x0000u)
#define CSL_IPIPE_IPIPE_MODE_ONESHOT_RESETVAL (0x0000u)

/*----ONESHOT Tokens----*/
#define CSL_IPIPE_IPIPE_MODE_ONESHOT_DISABLE (0x0000u)
#define CSL_IPIPE_IPIPE_MODE_ONESHOT_ENABLE (0x0001u)

#define CSL_IPIPE_IPIPE_MODE_RESETVAL    (0x0000u)

/* IPIPE_DPATHS */

#define CSL_IPIPE_IPIPE_DPATHS_BYPASS_MASK (0x0004u)
#define CSL_IPIPE_IPIPE_DPATHS_BYPASS_SHIFT (0x0002u)
#define CSL_IPIPE_IPIPE_DPATHS_BYPASS_RESETVAL (0x0000u)

/*----BYPASS Tokens----*/
#define CSL_IPIPE_IPIPE_DPATHS_BYPASS_OFF (0x0000u)
#define CSL_IPIPE_IPIPE_DPATHS_BYPASS_ON (0x0001u)

#define CSL_IPIPE_IPIPE_DPATHS_FMT_MASK  (0x0003u)
#define CSL_IPIPE_IPIPE_DPATHS_FMT_SHIFT (0x0000u)
#define CSL_IPIPE_IPIPE_DPATHS_FMT_RESETVAL (0x0000u)

/*----FMT Tokens----*/
#define CSL_IPIPE_IPIPE_DPATHS_FMT_RAW2YUV (0x0000u)
#define CSL_IPIPE_IPIPE_DPATHS_FMT_RAW2RAW (0x0001u)
#define CSL_IPIPE_IPIPE_DPATHS_FMT_RAW2BOX (0x0002u)
#define CSL_IPIPE_IPIPE_DPATHS_FMT_YUV2YUV (0x0003u)

#define CSL_IPIPE_IPIPE_DPATHS_RESETVAL  (0x0000u)

/* IPIPE_COLPAT */

#define CSL_IPIPE_IPIPE_COLPAT_OLOP_MASK (0x00C0u)
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_SHIFT (0x0006u)
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_RESETVAL (0x0003u)

/*----OLOP Tokens----*/
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_R    (0x0000u)
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_GR   (0x0001u)
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_GB   (0x0002u)
#define CSL_IPIPE_IPIPE_COLPAT_OLOP_B    (0x0003u)

#define CSL_IPIPE_IPIPE_COLPAT_OLEP_MASK (0x0030u)
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_SHIFT (0x0004u)
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_RESETVAL (0x0002u)

/*----OLEP Tokens----*/
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_R    (0x0000u)
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_GR   (0x0001u)
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_GB   (0x0002u)
#define CSL_IPIPE_IPIPE_COLPAT_OLEP_B    (0x0003u)

#define CSL_IPIPE_IPIPE_COLPAT_ELOP_MASK (0x000Cu)
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_SHIFT (0x0002u)
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_RESETVAL (0x0001u)

/*----ELOP Tokens----*/
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_R    (0x0000u)
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_GR   (0x0001u)
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_GB   (0x0002u)
#define CSL_IPIPE_IPIPE_COLPAT_ELOP_B    (0x0003u)

#define CSL_IPIPE_IPIPE_COLPAT_ELEP_MASK (0x0003u)
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_SHIFT (0x0000u)
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_RESETVAL (0x0000u)

/*----ELEP Tokens----*/
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_R    (0x0000u)
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_GR   (0x0001u)
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_GB   (0x0002u)
#define CSL_IPIPE_IPIPE_COLPAT_ELEP_B    (0x0003u)

#define CSL_IPIPE_IPIPE_COLPAT_RESETVAL  (0x00E4u)

/* IPIPE_VST */

#define CSL_IPIPE_IPIPE_VST_VAL_MASK     (0x1FFFu)
#define CSL_IPIPE_IPIPE_VST_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_IPIPE_VST_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_IPIPE_VST_RESETVAL     (0x0000u)

/* IPIPE_VSZ */

#define CSL_IPIPE_IPIPE_VSZ_VAL_MASK     (0x1FFFu)
#define CSL_IPIPE_IPIPE_VSZ_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_IPIPE_VSZ_VAL_RESETVAL (0x0001u)

#define CSL_IPIPE_IPIPE_VSZ_RESETVAL     (0x0001u)

/* IPIPE_HST */

#define CSL_IPIPE_IPIPE_HST_VAL_MASK     (0x1FFFu)
#define CSL_IPIPE_IPIPE_HST_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_IPIPE_HST_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_IPIPE_HST_RESETVAL     (0x0000u)

/* IPIPE_HSZ */

#define CSL_IPIPE_IPIPE_HSZ_VAL_MASK     (0x1FFFu)
#define CSL_IPIPE_IPIPE_HSZ_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_IPIPE_HSZ_VAL_RESETVAL (0x0001u)

#define CSL_IPIPE_IPIPE_HSZ_RESETVAL     (0x0001u)

/* IPIPE_STA */

#define CSL_IPIPE_IPIPE_STA_BOX_STATUS_MASK (0x0002u)
#define CSL_IPIPE_IPIPE_STA_BOX_STATUS_SHIFT (0x0001u)
#define CSL_IPIPE_IPIPE_STA_BOX_STATUS_RESETVAL (0x0000u)

#define CSL_IPIPE_IPIPE_STA_RSZ_STATUS_MASK (0x0001u)
#define CSL_IPIPE_IPIPE_STA_RSZ_STATUS_SHIFT (0x0000u)
#define CSL_IPIPE_IPIPE_STA_RSZ_STATUS_RESETVAL (0x0000u)

#define CSL_IPIPE_IPIPE_STA_RESETVAL     (0x0000u)

/* GCL_ARM */

#define CSL_IPIPE_GCL_ARM_REG_MASK       (0x0001u)
#define CSL_IPIPE_GCL_ARM_REG_SHIFT      (0x0000u)
#define CSL_IPIPE_GCL_ARM_REG_RESETVAL   (0x0000u)

/*----REG Tokens----*/
#define CSL_IPIPE_GCL_ARM_REG_OFF        (0x0000u)
#define CSL_IPIPE_GCL_ARM_REG_ON         (0x0001u)

#define CSL_IPIPE_GCL_ARM_RESETVAL       (0x0000u)

/* GCL_CCD */

#define CSL_IPIPE_GCL_CCD_G2_MASK        (0x0004u)
#define CSL_IPIPE_GCL_CCD_G2_SHIFT       (0x0002u)
#define CSL_IPIPE_GCL_CCD_G2_RESETVAL    (0x0000u)

/*----G2 Tokens----*/
#define CSL_IPIPE_GCL_CCD_G2_OFF         (0x0000u)
#define CSL_IPIPE_GCL_CCD_G2_ON          (0x0001u)

#define CSL_IPIPE_GCL_CCD_G1_MASK        (0x0002u)
#define CSL_IPIPE_GCL_CCD_G1_SHIFT       (0x0001u)
#define CSL_IPIPE_GCL_CCD_G1_RESETVAL    (0x0000u)

/*----G1 Tokens----*/
#define CSL_IPIPE_GCL_CCD_G1_OFF         (0x0000u)
#define CSL_IPIPE_GCL_CCD_G1_ON          (0x0001u)

#define CSL_IPIPE_GCL_CCD_G0_MASK        (0x0001u)
#define CSL_IPIPE_GCL_CCD_G0_SHIFT       (0x0000u)
#define CSL_IPIPE_GCL_CCD_G0_RESETVAL    (0x0000u)

/*----G0 Tokens----*/
#define CSL_IPIPE_GCL_CCD_G0_OFF         (0x0000u)
#define CSL_IPIPE_GCL_CCD_G0_ON          (0x0001u)

#define CSL_IPIPE_GCL_CCD_RESETVAL       (0x0000u)

/* GCL_SDR */

#define CSL_IPIPE_GCL_SDR_RSZ_MASK       (0x0001u)
#define CSL_IPIPE_GCL_SDR_RSZ_SHIFT      (0x0000u)
#define CSL_IPIPE_GCL_SDR_RSZ_RESETVAL   (0x0000u)

/*----RSZ Tokens----*/
#define CSL_IPIPE_GCL_SDR_RSZ_OFF        (0x0000u)
#define CSL_IPIPE_GCL_SDR_RSZ_ON         (0x0001u)

#define CSL_IPIPE_GCL_SDR_RESETVAL       (0x0000u)

/* RAM_MODE */

#define CSL_IPIPE_RAM_MODE_WIT_MASK      (0x8000u)
#define CSL_IPIPE_RAM_MODE_WIT_SHIFT     (0x000Fu)
#define CSL_IPIPE_RAM_MODE_WIT_RESETVAL  (0x0001u)

/*----WIT Tokens----*/
#define CSL_IPIPE_RAM_MODE_WIT_BUSY      (0x0000u)
#define CSL_IPIPE_RAM_MODE_WIT_NOTBUSY   (0x0001u)

#define CSL_IPIPE_RAM_MODE_EXT_MASK      (0x0040u)
#define CSL_IPIPE_RAM_MODE_EXT_SHIFT     (0x0006u)
#define CSL_IPIPE_RAM_MODE_EXT_RESETVAL  (0x0000u)

/*----EXT Tokens----*/
#define CSL_IPIPE_RAM_MODE_EXT_OUTPUT    (0x0000u)
#define CSL_IPIPE_RAM_MODE_EXT_NOOUTPUT  (0x0001u)

#define CSL_IPIPE_RAM_MODE_WDT_MASK      (0x0020u)
#define CSL_IPIPE_RAM_MODE_WDT_SHIFT     (0x0005u)
#define CSL_IPIPE_RAM_MODE_WDT_RESETVAL  (0x0000u)

/*----WDT Tokens----*/
#define CSL_IPIPE_RAM_MODE_WDT_READONLY  (0x0000u)
#define CSL_IPIPE_RAM_MODE_WDT_WRITE     (0x0001u)

#define CSL_IPIPE_RAM_MODE_ADR_MASK      (0x0010u)
#define CSL_IPIPE_RAM_MODE_ADR_SHIFT     (0x0004u)
#define CSL_IPIPE_RAM_MODE_ADR_RESETVAL  (0x0001u)

/*----ADR Tokens----*/
#define CSL_IPIPE_RAM_MODE_ADR_MANUAL    (0x0000u)
#define CSL_IPIPE_RAM_MODE_ADR_AUTOINCR  (0x0001u)

#define CSL_IPIPE_RAM_MODE_SEL_MASK      (0x000Fu)
#define CSL_IPIPE_RAM_MODE_SEL_SHIFT     (0x0000u)
#define CSL_IPIPE_RAM_MODE_SEL_RESETVAL  (0x0000u)

/*----SEL Tokens----*/
#define CSL_IPIPE_RAM_MODE_SEL_HIST0     (0x0000u)
#define CSL_IPIPE_RAM_MODE_SEL_HIST1     (0x0001u)
#define CSL_IPIPE_RAM_MODE_SEL_BSC0      (0x0002u)
#define CSL_IPIPE_RAM_MODE_SEL_BSC1      (0x0003u)
#define CSL_IPIPE_RAM_MODE_SEL_DFC       (0x0004u)
#define CSL_IPIPE_RAM_MODE_SEL_GAMMAR    (0x0005u)
#define CSL_IPIPE_RAM_MODE_SEL_GAMMAG    (0x0006u)
#define CSL_IPIPE_RAM_MODE_SEL_GAMMAB    (0x0007u)
#define CSL_IPIPE_RAM_MODE_SEL_GAMMARGB  (0x0008u)
#define CSL_IPIPE_RAM_MODE_SEL_YEE       (0x0009u)

#define CSL_IPIPE_RAM_MODE_RESETVAL      (0x8010u)

/* RAM_ADR */

#define CSL_IPIPE_RAM_ADR_VAL_MASK       (0x1FFFu)
#define CSL_IPIPE_RAM_ADR_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_RAM_ADR_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_RAM_ADR_RESETVAL       (0x0000u)

/* RAM_WDT */

#define CSL_IPIPE_RAM_WDT_VAL_MASK       (0xFFFFu)
#define CSL_IPIPE_RAM_WDT_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_RAM_WDT_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_RAM_WDT_RESETVAL       (0x0000u)

/* RAM_RDT */

#define CSL_IPIPE_RAM_RDT_VAL_MASK       (0xFFFFu)
#define CSL_IPIPE_RAM_RDT_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_RAM_RDT_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_RAM_RDT_RESETVAL       (0x0000u)

/* IRQ_EN */

#define CSL_IPIPE_IRQ_EN_INT5_MASK       (0x0020u)
#define CSL_IPIPE_IRQ_EN_INT5_SHIFT      (0x0005u)
#define CSL_IPIPE_IRQ_EN_INT5_RESETVAL   (0x0000u)

/*----INT5 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT5_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT5_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_INT4_MASK       (0x0010u)
#define CSL_IPIPE_IRQ_EN_INT4_SHIFT      (0x0004u)
#define CSL_IPIPE_IRQ_EN_INT4_RESETVAL   (0x0000u)

/*----INT4 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT4_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT4_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_INT3_MASK       (0x0008u)
#define CSL_IPIPE_IRQ_EN_INT3_SHIFT      (0x0003u)
#define CSL_IPIPE_IRQ_EN_INT3_RESETVAL   (0x0000u)

/*----INT3 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT3_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT3_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_INT2_MASK       (0x0004u)
#define CSL_IPIPE_IRQ_EN_INT2_SHIFT      (0x0002u)
#define CSL_IPIPE_IRQ_EN_INT2_RESETVAL   (0x0000u)

/*----INT2 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT2_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT2_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_INT1_MASK       (0x0002u)
#define CSL_IPIPE_IRQ_EN_INT1_SHIFT      (0x0001u)
#define CSL_IPIPE_IRQ_EN_INT1_RESETVAL   (0x0001u)

/*----INT1 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT1_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT1_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_INT0_MASK       (0x0001u)
#define CSL_IPIPE_IRQ_EN_INT0_SHIFT      (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT0_RESETVAL   (0x0000u)

/*----INT0 Tokens----*/
#define CSL_IPIPE_IRQ_EN_INT0_DISABLE    (0x0000u)
#define CSL_IPIPE_IRQ_EN_INT0_ENABLE     (0x0001u)

#define CSL_IPIPE_IRQ_EN_RESETVAL        (0x0002u)

/* IRQ_RZA */

#define CSL_IPIPE_IRQ_RZA_VAL_MASK       (0x1FFFu)
#define CSL_IPIPE_IRQ_RZA_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_IRQ_RZA_VAL_RESETVAL   (0x1FFFu)

#define CSL_IPIPE_IRQ_RZA_RESETVAL       (0x1FFFu)

/* IRQ_RZB */

#define CSL_IPIPE_IRQ_RZB_VAL_MASK       (0x1FFFu)
#define CSL_IPIPE_IRQ_RZB_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_IRQ_RZB_VAL_RESETVAL   (0x1FFFu)

#define CSL_IPIPE_IRQ_RZB_RESETVAL       (0x1FFFu)

/* DFC_EN */

#define CSL_IPIPE_DFC_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_DFC_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_DFC_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_DFC_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_DFC_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_DFC_EN_RESETVAL        (0x0000u)

/* DFC_SEL */

#define CSL_IPIPE_DFC_SEL_SEL_MASK       (0x0001u)
#define CSL_IPIPE_DFC_SEL_SEL_SHIFT      (0x0000u)
#define CSL_IPIPE_DFC_SEL_SEL_RESETVAL   (0x0000u)

/*----SEL Tokens----*/
#define CSL_IPIPE_DFC_SEL_SEL_FROMTOP    (0x0000u)
#define CSL_IPIPE_DFC_SEL_SEL_FROMBOTTOM (0x0001u)

#define CSL_IPIPE_DFC_SEL_RESETVAL       (0x0000u)

/* DFC_ADR */

#define CSL_IPIPE_DFC_ADR_ADR_MASK       (0x03FFu)
#define CSL_IPIPE_DFC_ADR_ADR_SHIFT      (0x0000u)
#define CSL_IPIPE_DFC_ADR_ADR_RESETVAL   (0x0000u)

#define CSL_IPIPE_DFC_ADR_RESETVAL       (0x0000u)

/* DFC_SIZ */

#define CSL_IPIPE_DFC_SIZ_SIZ_MASK       (0x03FFu)
#define CSL_IPIPE_DFC_SIZ_SIZ_SHIFT      (0x0000u)
#define CSL_IPIPE_DFC_SIZ_SIZ_RESETVAL   (0x0000u)

#define CSL_IPIPE_DFC_SIZ_RESETVAL       (0x0000u)

/* D2F_EN */

#define CSL_IPIPE_D2F_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_D2F_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_D2F_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_D2F_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_D2F_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_D2F_EN_RESETVAL        (0x0000u)

/* D2F_CFG */

#define CSL_IPIPE_D2F_CFG_TYP_MASK       (0x0010u)
#define CSL_IPIPE_D2F_CFG_TYP_SHIFT      (0x0004u)
#define CSL_IPIPE_D2F_CFG_TYP_RESETVAL   (0x0000u)

/*----TYP Tokens----*/
#define CSL_IPIPE_D2F_CFG_TYP_BOX        (0x0000u)
#define CSL_IPIPE_D2F_CFG_TYP_DIAMOND    (0x0001u)

#define CSL_IPIPE_D2F_CFG_SHF_MASK       (0x000Cu)
#define CSL_IPIPE_D2F_CFG_SHF_SHIFT      (0x0002u)
#define CSL_IPIPE_D2F_CFG_SHF_RESETVAL   (0x0000u)

#define CSL_IPIPE_D2F_CFG_SPR_MASK       (0x0003u)
#define CSL_IPIPE_D2F_CFG_SPR_SHIFT      (0x0000u)
#define CSL_IPIPE_D2F_CFG_SPR_RESETVAL   (0x0000u)

#define CSL_IPIPE_D2F_CFG_RESETVAL       (0x0000u)

/* THR */

#define CSL_IPIPE_THR_VAL_MASK           (0x0FFFu)
#define CSL_IPIPE_THR_VAL_SHIFT          (0x0000u)
#define CSL_IPIPE_THR_VAL_RESETVAL       (0x0000u)

#define CSL_IPIPE_THR_RESETVAL           (0x0000u)

/* STR */

#define CSL_IPIPE_STR_VAL_MASK           (0x001Fu)
#define CSL_IPIPE_STR_VAL_SHIFT          (0x0000u)
#define CSL_IPIPE_STR_VAL_RESETVAL       (0x0000u)

#define CSL_IPIPE_STR_RESETVAL           (0x0000u)

/* PRE_EN */

#define CSL_IPIPE_PRE_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_PRE_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_PRE_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_PRE_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_PRE_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_PRE_EN_RESETVAL        (0x0000u)

/* PRE_TYP */

#define CSL_IPIPE_PRE_TYP_EN1_MASK       (0x0008u)
#define CSL_IPIPE_PRE_TYP_EN1_SHIFT      (0x0003u)
#define CSL_IPIPE_PRE_TYP_EN1_RESETVAL   (0x0000u)

/*----EN1 Tokens----*/
#define CSL_IPIPE_PRE_TYP_EN1_DISABLE    (0x0000u)
#define CSL_IPIPE_PRE_TYP_EN1_ENABLE     (0x0001u)

#define CSL_IPIPE_PRE_TYP_EN0_MASK       (0x0004u)
#define CSL_IPIPE_PRE_TYP_EN0_SHIFT      (0x0002u)
#define CSL_IPIPE_PRE_TYP_EN0_RESETVAL   (0x0000u)

/*----EN0 Tokens----*/
#define CSL_IPIPE_PRE_TYP_EN0_DISABLE    (0x0000u)
#define CSL_IPIPE_PRE_TYP_EN0_ENABLE     (0x0001u)

#define CSL_IPIPE_PRE_TYP_SEL1_MASK      (0x0002u)
#define CSL_IPIPE_PRE_TYP_SEL1_SHIFT     (0x0001u)
#define CSL_IPIPE_PRE_TYP_SEL1_RESETVAL  (0x0000u)

/*----SEL1 Tokens----*/
#define CSL_IPIPE_PRE_TYP_SEL1_AVG4PIX   (0x0000u)
#define CSL_IPIPE_PRE_TYP_SEL1_AVG2MEDPIX (0x0001u)

#define CSL_IPIPE_PRE_TYP_SEL0_MASK      (0x0001u)
#define CSL_IPIPE_PRE_TYP_SEL0_SHIFT     (0x0000u)
#define CSL_IPIPE_PRE_TYP_SEL0_RESETVAL  (0x0000u)

/*----SEL0 Tokens----*/
#define CSL_IPIPE_PRE_TYP_SEL0_AVG4PIX   (0x0000u)
#define CSL_IPIPE_PRE_TYP_SEL0_AVG2MEDPIX (0x0001u)

#define CSL_IPIPE_PRE_TYP_RESETVAL       (0x0000u)

/* PRE_SHF */

#define CSL_IPIPE_PRE_SHF_VAL_MASK       (0x000Fu)
#define CSL_IPIPE_PRE_SHF_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_PRE_SHF_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_PRE_SHF_RESETVAL       (0x0000u)

/* PRE_GAIN */

#define CSL_IPIPE_PRE_GAIN_VAL_MASK      (0x00FFu)
#define CSL_IPIPE_PRE_GAIN_VAL_SHIFT     (0x0000u)
#define CSL_IPIPE_PRE_GAIN_VAL_RESETVAL  (0x0000u)

#define CSL_IPIPE_PRE_GAIN_RESETVAL      (0x0000u)

/* PRE_THR_G */

#define CSL_IPIPE_PRE_THR_G_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_PRE_THR_G_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_PRE_THR_G_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_PRE_THR_G_RESETVAL     (0x0000u)

/* PRE_THR_B */

#define CSL_IPIPE_PRE_THR_B_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_PRE_THR_B_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_PRE_THR_B_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_PRE_THR_B_RESETVAL     (0x0000u)

/* PRE_THR_1 */

#define CSL_IPIPE_PRE_THR_1_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_PRE_THR_1_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_PRE_THR_1_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_PRE_THR_1_RESETVAL     (0x0000u)

/* WB2_DGN */

#define CSL_IPIPE_WB2_DGN_DGAIN_MASK     (0x03FFu)
#define CSL_IPIPE_WB2_DGN_DGAIN_SHIFT    (0x0000u)
#define CSL_IPIPE_WB2_DGN_DGAIN_RESETVAL (0x0100u)

#define CSL_IPIPE_WB2_DGN_RESETVAL       (0x0100u)

/* WB2_WG_R */

#define CSL_IPIPE_WB2_WG_R_WG_MASK       (0x03FFu)
#define CSL_IPIPE_WB2_WG_R_WG_SHIFT      (0x0000u)
#define CSL_IPIPE_WB2_WG_R_WG_RESETVAL   (0x0080u)

#define CSL_IPIPE_WB2_WG_R_RESETVAL      (0x0080u)

/* WB2_WG_GR */

#define CSL_IPIPE_WB2_WG_GR_WG_MASK      (0x03FFu)
#define CSL_IPIPE_WB2_WG_GR_WG_SHIFT     (0x0000u)
#define CSL_IPIPE_WB2_WG_GR_WG_RESETVAL  (0x0080u)

#define CSL_IPIPE_WB2_WG_GR_RESETVAL     (0x0080u)

/* WB2_WG_GB */

#define CSL_IPIPE_WB2_WG_GB_WG_MASK      (0x03FFu)
#define CSL_IPIPE_WB2_WG_GB_WG_SHIFT     (0x0000u)
#define CSL_IPIPE_WB2_WG_GB_WG_RESETVAL  (0x0080u)

#define CSL_IPIPE_WB2_WG_GB_RESETVAL     (0x0080u)

/* WB2_WG_B */

#define CSL_IPIPE_WB2_WG_B_WG_MASK       (0x03FFu)
#define CSL_IPIPE_WB2_WG_B_WG_SHIFT      (0x0000u)
#define CSL_IPIPE_WB2_WG_B_WG_RESETVAL   (0x0080u)

#define CSL_IPIPE_WB2_WG_B_RESETVAL      (0x0080u)

/* RGB_MUL_RR */

#define CSL_IPIPE_RGB_MUL_RR_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_RR_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_RR_VAL_RESETVAL (0x0100u)

#define CSL_IPIPE_RGB_MUL_RR_RESETVAL    (0x0100u)

/* RGB_MUL_GR */

#define CSL_IPIPE_RGB_MUL_GR_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_GR_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_GR_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_GR_RESETVAL    (0x0000u)

/* RGB_MUL_BR */

#define CSL_IPIPE_RGB_MUL_BR_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_BR_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_BR_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_BR_RESETVAL    (0x0000u)

/* RGB_MUL_RG */

#define CSL_IPIPE_RGB_MUL_RG_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_RG_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_RG_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_RG_RESETVAL    (0x0000u)

/* RGB_MUL_GG */

#define CSL_IPIPE_RGB_MUL_GG_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_GG_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_GG_VAL_RESETVAL (0x0100u)

#define CSL_IPIPE_RGB_MUL_GG_RESETVAL    (0x0100u)

/* RGB_MUL_BG */

#define CSL_IPIPE_RGB_MUL_BG_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_BG_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_BG_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_BG_RESETVAL    (0x0000u)

/* RGB_MUL_RB */

#define CSL_IPIPE_RGB_MUL_RB_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_RB_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_RB_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_RB_RESETVAL    (0x0000u)

/* RGB_MUL_GB */

#define CSL_IPIPE_RGB_MUL_GB_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_GB_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_GB_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_MUL_GB_RESETVAL    (0x0000u)

/* RGB_MUL_BB */

#define CSL_IPIPE_RGB_MUL_BB_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_MUL_BB_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_MUL_BB_VAL_RESETVAL (0x0100u)

#define CSL_IPIPE_RGB_MUL_BB_RESETVAL    (0x0100u)

/* RGB_OFT_OR */

#define CSL_IPIPE_RGB_OFT_OR_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_OFT_OR_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_OFT_OR_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_OFT_OR_RESETVAL    (0x0000u)

/* RGB_OFT_OG */

#define CSL_IPIPE_RGB_OFT_OG_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_OFT_OG_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_OFT_OG_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_OFT_OG_RESETVAL    (0x0000u)

/* RGB_OFT_OB */

#define CSL_IPIPE_RGB_OFT_OB_VAL_MASK    (0x0FFFu)
#define CSL_IPIPE_RGB_OFT_OB_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_RGB_OFT_OB_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RGB_OFT_OB_RESETVAL    (0x0000u)

/* GMM_CFG */

#define CSL_IPIPE_GMM_CFG_SIZ_MASK       (0x0060u)
#define CSL_IPIPE_GMM_CFG_SIZ_SHIFT      (0x0005u)
#define CSL_IPIPE_GMM_CFG_SIZ_RESETVAL   (0x0003u)

/*----SIZ Tokens----*/
#define CSL_IPIPE_GMM_CFG_SIZ__128       (0x0000u)
#define CSL_IPIPE_GMM_CFG_SIZ__256       (0x0001u)
#define CSL_IPIPE_GMM_CFG_SIZ_RESERVED   (0x0002u)
#define CSL_IPIPE_GMM_CFG_SIZ__512       (0x0003u)

#define CSL_IPIPE_GMM_CFG_TBL_MASK       (0x0010u)
#define CSL_IPIPE_GMM_CFG_TBL_SHIFT      (0x0004u)
#define CSL_IPIPE_GMM_CFG_TBL_RESETVAL   (0x0000u)

/*----TBL Tokens----*/
#define CSL_IPIPE_GMM_CFG_TBL_RAM        (0x0000u)
#define CSL_IPIPE_GMM_CFG_TBL_ROM        (0x0001u)

#define CSL_IPIPE_GMM_CFG_BYPB_MASK      (0x0004u)
#define CSL_IPIPE_GMM_CFG_BYPB_SHIFT     (0x0002u)
#define CSL_IPIPE_GMM_CFG_BYPB_RESETVAL  (0x0001u)

/*----BYPB Tokens----*/
#define CSL_IPIPE_GMM_CFG_BYPB_ENABLE    (0x0000u)
#define CSL_IPIPE_GMM_CFG_BYPB_BYPASS    (0x0001u)

#define CSL_IPIPE_GMM_CFG_BYPG_MASK      (0x0002u)
#define CSL_IPIPE_GMM_CFG_BYPG_SHIFT     (0x0001u)
#define CSL_IPIPE_GMM_CFG_BYPG_RESETVAL  (0x0001u)

/*----BYPG Tokens----*/
#define CSL_IPIPE_GMM_CFG_BYPG_ENABLE    (0x0000u)
#define CSL_IPIPE_GMM_CFG_BYPG_BYPASS    (0x0001u)

#define CSL_IPIPE_GMM_CFG_BYPR_MASK      (0x0001u)
#define CSL_IPIPE_GMM_CFG_BYPR_SHIFT     (0x0000u)
#define CSL_IPIPE_GMM_CFG_BYPR_RESETVAL  (0x0001u)

/*----BYPR Tokens----*/
#define CSL_IPIPE_GMM_CFG_BYPR_ENABLE    (0x0000u)
#define CSL_IPIPE_GMM_CFG_BYPR_BYPASS    (0x0001u)

#define CSL_IPIPE_GMM_CFG_RESETVAL       (0x0067u)

/* YUV_ADJ */

#define CSL_IPIPE_YUV_ADJ_BRT_MASK       (0xFF00u)
#define CSL_IPIPE_YUV_ADJ_BRT_SHIFT      (0x0008u)
#define CSL_IPIPE_YUV_ADJ_BRT_RESETVAL   (0x0000u)

#define CSL_IPIPE_YUV_ADJ_CTR_MASK       (0x00FFu)
#define CSL_IPIPE_YUV_ADJ_CTR_SHIFT      (0x0000u)
#define CSL_IPIPE_YUV_ADJ_CTR_RESETVAL   (0x0010u)

#define CSL_IPIPE_YUV_ADJ_RESETVAL       (0x0010u)

/* YUV_MUL_RY */

#define CSL_IPIPE_YUV_MUL_RY_VAL_MASK    (0x03FFu)
#define CSL_IPIPE_YUV_MUL_RY_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YUV_MUL_RY_VAL_RESETVAL (0x004Du)

#define CSL_IPIPE_YUV_MUL_RY_RESETVAL    (0x004Du)

/* YUV_MUL_GY */

#define CSL_IPIPE_YUV_MUL_GY_VAL_MASK    (0x03FFu)
#define CSL_IPIPE_YUV_MUL_GY_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YUV_MUL_GY_VAL_RESETVAL (0x0096u)

#define CSL_IPIPE_YUV_MUL_GY_RESETVAL    (0x0096u)

/* YUV_MUL_BY */

#define CSL_IPIPE_YUV_MUL_BY_VAL_MASK    (0x03FFu)
#define CSL_IPIPE_YUV_MUL_BY_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YUV_MUL_BY_VAL_RESETVAL (0x001Du)

#define CSL_IPIPE_YUV_MUL_BY_RESETVAL    (0x001Du)

/* YUV_MUL_RCB */

#define CSL_IPIPE_YUV_MUL_RCB_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_RCB_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_RCB_VAL_RESETVAL (0x03D5u)

#define CSL_IPIPE_YUV_MUL_RCB_RESETVAL   (0x03D5u)

/* YUV_MUL_GCB */

#define CSL_IPIPE_YUV_MUL_GCB_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_GCB_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_GCB_VAL_RESETVAL (0x03ABu)

#define CSL_IPIPE_YUV_MUL_GCB_RESETVAL   (0x03ABu)

/* YUV_MUL_BCB */

#define CSL_IPIPE_YUV_MUL_BCB_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_BCB_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_BCB_VAL_RESETVAL (0x0080u)

#define CSL_IPIPE_YUV_MUL_BCB_RESETVAL   (0x0080u)

/* YUV_MUL_RCR */

#define CSL_IPIPE_YUV_MUL_RCR_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_RCR_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_RCR_VAL_RESETVAL (0x0080u)

#define CSL_IPIPE_YUV_MUL_RCR_RESETVAL   (0x0080u)

/* YUV_MUL_GCR */

#define CSL_IPIPE_YUV_MUL_GCR_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_GCR_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_GCR_VAL_RESETVAL (0x0395u)

#define CSL_IPIPE_YUV_MUL_GCR_RESETVAL   (0x0395u)

/* YUV_MUL_BCR */

#define CSL_IPIPE_YUV_MUL_BCR_VAL_MASK   (0x03FFu)
#define CSL_IPIPE_YUV_MUL_BCR_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_YUV_MUL_BCR_VAL_RESETVAL (0x03EBu)

#define CSL_IPIPE_YUV_MUL_BCR_RESETVAL   (0x03EBu)

/* YUV_OFT_Y */

#define CSL_IPIPE_YUV_OFT_Y_VAL_MASK     (0x01FFu)
#define CSL_IPIPE_YUV_OFT_Y_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_YUV_OFT_Y_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YUV_OFT_Y_RESETVAL     (0x0000u)

/* YUV_OFT_CB */

#define CSL_IPIPE_YUV_OFT_CB_VAL_MASK    (0x01FFu)
#define CSL_IPIPE_YUV_OFT_CB_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YUV_OFT_CB_VAL_RESETVAL (0x0080u)

#define CSL_IPIPE_YUV_OFT_CB_RESETVAL    (0x0080u)

/* YUV_OFT_CR */

#define CSL_IPIPE_YUV_OFT_CR_VAL_MASK    (0x01FFu)
#define CSL_IPIPE_YUV_OFT_CR_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YUV_OFT_CR_VAL_RESETVAL (0x0080u)

#define CSL_IPIPE_YUV_OFT_CR_RESETVAL    (0x0080u)

/* YUV_Y_MIN */

#define CSL_IPIPE_YUV_Y_MIN_VAL_MASK     (0x00FFu)
#define CSL_IPIPE_YUV_Y_MIN_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_YUV_Y_MIN_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YUV_Y_MIN_RESETVAL     (0x0000u)

/* YUV_Y_MAX */

#define CSL_IPIPE_YUV_Y_MAX_VAL_MASK     (0x00FFu)
#define CSL_IPIPE_YUV_Y_MAX_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_YUV_Y_MAX_VAL_RESETVAL (0x00FFu)

#define CSL_IPIPE_YUV_Y_MAX_RESETVAL     (0x00FFu)

/* YUV_C_MIN */

#define CSL_IPIPE_YUV_C_MIN_VAL_MASK     (0x00FFu)
#define CSL_IPIPE_YUV_C_MIN_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_YUV_C_MIN_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YUV_C_MIN_RESETVAL     (0x0000u)

/* YUV_C_MAX */

#define CSL_IPIPE_YUV_C_MAX_VAL_MASK     (0x00FFu)
#define CSL_IPIPE_YUV_C_MAX_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_YUV_C_MAX_VAL_RESETVAL (0x00FFu)

#define CSL_IPIPE_YUV_C_MAX_RESETVAL     (0x00FFu)

/* YUV_PHS */

#define CSL_IPIPE_YUV_PHS_LPF_MASK       (0x0002u)
#define CSL_IPIPE_YUV_PHS_LPF_SHIFT      (0x0001u)
#define CSL_IPIPE_YUV_PHS_LPF_RESETVAL   (0x0000u)

/*----LPF Tokens----*/
#define CSL_IPIPE_YUV_PHS_LPF_OFF        (0x0000u)
#define CSL_IPIPE_YUV_PHS_LPF_ON         (0x0001u)

#define CSL_IPIPE_YUV_PHS_POS_MASK       (0x0001u)
#define CSL_IPIPE_YUV_PHS_POS_SHIFT      (0x0000u)
#define CSL_IPIPE_YUV_PHS_POS_RESETVAL   (0x0000u)

/*----POS Tokens----*/
#define CSL_IPIPE_YUV_PHS_POS_SAME_AS_Y  (0x0000u)
#define CSL_IPIPE_YUV_PHS_POS_MIDDLE_OF_Y (0x0001u)

#define CSL_IPIPE_YUV_PHS_RESETVAL       (0x0000u)

/* YEE_EN */

#define CSL_IPIPE_YEE_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_YEE_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_YEE_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_YEE_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_YEE_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_YEE_EN_RESETVAL        (0x0000u)

/* YEE_EMF */

#define CSL_IPIPE_YEE_EMF_EN_MASK        (0x0001u)
#define CSL_IPIPE_YEE_EMF_EN_SHIFT       (0x0000u)
#define CSL_IPIPE_YEE_EMF_EN_RESETVAL    (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_YEE_EMF_EN_DISABLE     (0x0000u)
#define CSL_IPIPE_YEE_EMF_EN_ENABLE      (0x0001u)

#define CSL_IPIPE_YEE_EMF_RESETVAL       (0x0000u)

/* YEE_SHF */

#define CSL_IPIPE_YEE_SHF_SHF_MASK       (0x000Fu)
#define CSL_IPIPE_YEE_SHF_SHF_SHIFT      (0x0000u)
#define CSL_IPIPE_YEE_SHF_SHF_RESETVAL   (0x0000u)

/*----SHF Tokens----*/
#define CSL_IPIPE_YEE_SHF_SHF_DISABLE    (0x0000u)
#define CSL_IPIPE_YEE_SHF_SHF_ENABLE     (0x0001u)

#define CSL_IPIPE_YEE_SHF_RESETVAL       (0x0000u)

/* YEE_MUL_00 */

#define CSL_IPIPE_YEE_MUL_00_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_00_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_00_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_00_RESETVAL    (0x0000u)

/* YEE_MUL_01 */

#define CSL_IPIPE_YEE_MUL_01_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_01_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_01_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_01_RESETVAL    (0x0000u)

/* YEE_MUL_02 */

#define CSL_IPIPE_YEE_MUL_02_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_02_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_02_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_02_RESETVAL    (0x0000u)

/* YEE_MUL_10 */

#define CSL_IPIPE_YEE_MUL_10_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_10_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_10_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_10_RESETVAL    (0x0000u)

/* YEE_MUL_11 */

#define CSL_IPIPE_YEE_MUL_11_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_11_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_11_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_11_RESETVAL    (0x0000u)

/* YEE_MUL_12 */

#define CSL_IPIPE_YEE_MUL_12_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_12_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_12_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_12_RESETVAL    (0x0000u)

/* YEE_MUL_20 */

#define CSL_IPIPE_YEE_MUL_20_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_20_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_20_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_20_RESETVAL    (0x0000u)

/* YEE_MUL_21 */

#define CSL_IPIPE_YEE_MUL_21_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_21_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_21_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_21_RESETVAL    (0x0000u)

/* YEE_MUL_22 */

#define CSL_IPIPE_YEE_MUL_22_VAL_MASK    (0x00FFu)
#define CSL_IPIPE_YEE_MUL_22_VAL_SHIFT   (0x0000u)
#define CSL_IPIPE_YEE_MUL_22_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_YEE_MUL_22_RESETVAL    (0x0000u)

/* FCS_EN */

#define CSL_IPIPE_FCS_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_FCS_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_FCS_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_FCS_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_FCS_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_FCS_EN_RESETVAL        (0x0000u)

/* FCS_TYP */

#define CSL_IPIPE_FCS_TYP_TYP_MASK       (0x0007u)
#define CSL_IPIPE_FCS_TYP_TYP_SHIFT      (0x0000u)
#define CSL_IPIPE_FCS_TYP_TYP_RESETVAL   (0x0000u)

/*----TYP Tokens----*/
#define CSL_IPIPE_FCS_TYP_TYP_Y          (0x0000u)
#define CSL_IPIPE_FCS_TYP_TYP_HPF_HORZ   (0x0001u)
#define CSL_IPIPE_FCS_TYP_TYP_HPF_VERT   (0x0002u)
#define CSL_IPIPE_FCS_TYP_TYP_HPF_2D     (0x0003u)
#define CSL_IPIPE_FCS_TYP_TYP_HPF_2D_YEE (0x0004u)

#define CSL_IPIPE_FCS_TYP_RESETVAL       (0x0000u)

/* FCS_SHF_Y */

#define CSL_IPIPE_FCS_SHF_Y_SHF_MASK     (0x0003u)
#define CSL_IPIPE_FCS_SHF_Y_SHF_SHIFT    (0x0000u)
#define CSL_IPIPE_FCS_SHF_Y_SHF_RESETVAL (0x0000u)

#define CSL_IPIPE_FCS_SHF_Y_RESETVAL     (0x0000u)

/* FCS_SHF_C */

#define CSL_IPIPE_FCS_SHF_C_SHF_MASK     (0x0007u)
#define CSL_IPIPE_FCS_SHF_C_SHF_SHIFT    (0x0000u)
#define CSL_IPIPE_FCS_SHF_C_SHF_RESETVAL (0x0000u)

#define CSL_IPIPE_FCS_SHF_C_RESETVAL     (0x0000u)

/* FCS_THR */

#define CSL_IPIPE_FCS_THR_THR_MASK       (0x00FFu)
#define CSL_IPIPE_FCS_THR_THR_SHIFT      (0x0000u)
#define CSL_IPIPE_FCS_THR_THR_RESETVAL   (0x0000u)

#define CSL_IPIPE_FCS_THR_RESETVAL       (0x0000u)

/* FCS_SGN */

#define CSL_IPIPE_FCS_SGN_SGN_MASK       (0x00FFu)
#define CSL_IPIPE_FCS_SGN_SGN_SHIFT      (0x0000u)
#define CSL_IPIPE_FCS_SGN_SGN_RESETVAL   (0x0000u)

#define CSL_IPIPE_FCS_SGN_RESETVAL       (0x0000u)

/* FCS_LTH */

#define CSL_IPIPE_FCS_LTH_LIM_MASK       (0x01FFu)
#define CSL_IPIPE_FCS_LTH_LIM_SHIFT      (0x0000u)
#define CSL_IPIPE_FCS_LTH_LIM_RESETVAL   (0x0000u)

#define CSL_IPIPE_FCS_LTH_RESETVAL       (0x0000u)

/* RSZ_SEQ */

#define CSL_IPIPE_RSZ_SEQ_CRV_MASK       (0x0010u)
#define CSL_IPIPE_RSZ_SEQ_CRV_SHIFT      (0x0004u)
#define CSL_IPIPE_RSZ_SEQ_CRV_RESETVAL   (0x0000u)

#define CSL_IPIPE_RSZ_SEQ_VRV_MASK       (0x0008u)
#define CSL_IPIPE_RSZ_SEQ_VRV_SHIFT      (0x0003u)
#define CSL_IPIPE_RSZ_SEQ_VRV_RESETVAL   (0x0000u)

#define CSL_IPIPE_RSZ_SEQ_HRV_MASK       (0x0004u)
#define CSL_IPIPE_RSZ_SEQ_HRV_SHIFT      (0x0002u)
#define CSL_IPIPE_RSZ_SEQ_HRV_RESETVAL   (0x0000u)

#define CSL_IPIPE_RSZ_SEQ_TMM_MASK       (0x0002u)
#define CSL_IPIPE_RSZ_SEQ_TMM_SHIFT      (0x0001u)
#define CSL_IPIPE_RSZ_SEQ_TMM_RESETVAL   (0x0000u)

#define CSL_IPIPE_RSZ_SEQ_SEQ_MASK       (0x0001u)
#define CSL_IPIPE_RSZ_SEQ_SEQ_SHIFT      (0x0000u)
#define CSL_IPIPE_RSZ_SEQ_SEQ_RESETVAL   (0x0000u)

#define CSL_IPIPE_RSZ_SEQ_RESETVAL       (0x0000u)

/* RSZ_AAL */

#define CSL_IPIPE_RSZ_AAL_AAL_MASK       (0x0001u)
#define CSL_IPIPE_RSZ_AAL_AAL_SHIFT      (0x0000u)
#define CSL_IPIPE_RSZ_AAL_AAL_RESETVAL   (0x0000u)

/*----AAL Tokens----*/
#define CSL_IPIPE_RSZ_AAL_AAL_DISABLE    (0x0000u)
#define CSL_IPIPE_RSZ_AAL_AAL_ENABLE     (0x0001u)

#define CSL_IPIPE_RSZ_AAL_RESETVAL       (0x0000u)

/* RSZ_EN */

#define CSL_IPIPE_RSZ_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_RSZ_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_RSZ_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_RSZ_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_RSZ_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_RSZ_EN_RESETVAL        (0x0000u)

/* RSZ_MODE */

#define CSL_IPIPE_RSZ_MODE_OST_MASK      (0x0001u)
#define CSL_IPIPE_RSZ_MODE_OST_SHIFT     (0x0000u)
#define CSL_IPIPE_RSZ_MODE_OST_RESETVAL  (0x0000u)

/*----OST Tokens----*/
#define CSL_IPIPE_RSZ_MODE_OST_CONTINUOUS (0x0000u)
#define CSL_IPIPE_RSZ_MODE_OST_ONESHOT   (0x0001u)

#define CSL_IPIPE_RSZ_MODE_RESETVAL      (0x0000u)

/* RSZ_I_VST */

#define CSL_IPIPE_RSZ_I_VST_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_I_VST_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_I_VST_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_I_VST_RESETVAL     (0x0000u)

/* RSZ_I_VSZ */

#define CSL_IPIPE_RSZ_I_VSZ_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_I_VSZ_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_I_VSZ_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_I_VSZ_RESETVAL     (0x0000u)

/* RSZ_I_HST */

#define CSL_IPIPE_RSZ_I_HST_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_I_HST_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_I_HST_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_I_HST_RESETVAL     (0x0000u)

/* RSZ_O_VSZ */

#define CSL_IPIPE_RSZ_O_VSZ_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_O_VSZ_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_O_VSZ_VAL_RESETVAL (0x0001u)

#define CSL_IPIPE_RSZ_O_VSZ_RESETVAL     (0x0001u)

/* RSZ_O_HST */

#define CSL_IPIPE_RSZ_O_HST_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_O_HST_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_O_HST_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_O_HST_RESETVAL     (0x0000u)

/* RSZ_O_HSZ */

#define CSL_IPIPE_RSZ_O_HSZ_VAL_MASK     (0x0FFFu)
#define CSL_IPIPE_RSZ_O_HSZ_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_O_HSZ_VAL_RESETVAL (0x0001u)

#define CSL_IPIPE_RSZ_O_HSZ_RESETVAL     (0x0001u)

/* RSZ_V_PHS */

#define CSL_IPIPE_RSZ_V_PHS_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_RSZ_V_PHS_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_V_PHS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_V_PHS_RESETVAL     (0x0000u)

/* RSZ_V_PHS_O */

#define CSL_IPIPE_RSZ_V_PHS_O_VAL_MASK   (0x3FFFu)
#define CSL_IPIPE_RSZ_V_PHS_O_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_RSZ_V_PHS_O_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_V_PHS_O_RESETVAL   (0x0000u)

/* RSZ_V_DIF */

#define CSL_IPIPE_RSZ_V_DIF_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_RSZ_V_DIF_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_V_DIF_VAL_RESETVAL (0x0100u)

#define CSL_IPIPE_RSZ_V_DIF_RESETVAL     (0x0100u)

/* RSZ_V_SIZ_O */

#define CSL_IPIPE_RSZ_V_SIZ_O_VAL_MASK   (0x1FFFu)
#define CSL_IPIPE_RSZ_V_SIZ_O_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_RSZ_V_SIZ_O_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_V_SIZ_O_RESETVAL   (0x0000u)

/* RSZ_H_PHS */

#define CSL_IPIPE_RSZ_H_PHS_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_RSZ_H_PHS_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_H_PHS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_H_PHS_RESETVAL     (0x0000u)

/* RSZ_H_DIF */

#define CSL_IPIPE_RSZ_H_DIF_VAL_MASK     (0x3FFFu)
#define CSL_IPIPE_RSZ_H_DIF_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_H_DIF_VAL_RESETVAL (0x0100u)

#define CSL_IPIPE_RSZ_H_DIF_RESETVAL     (0x0100u)

/* RSZ_H_TYP */

#define CSL_IPIPE_RSZ_H_TYP_TYP_MASK     (0x0001u)
#define CSL_IPIPE_RSZ_H_TYP_TYP_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_H_TYP_TYP_RESETVAL (0x0000u)

/*----TYP Tokens----*/
#define CSL_IPIPE_RSZ_H_TYP_TYP_CUBIC    (0x0000u)
#define CSL_IPIPE_RSZ_H_TYP_TYP_LINEAR   (0x0001u)

#define CSL_IPIPE_RSZ_H_TYP_RESETVAL     (0x0000u)

/* RSZ_H_LSE */

#define CSL_IPIPE_RSZ_H_LSE_SEL_MASK     (0x0001u)
#define CSL_IPIPE_RSZ_H_LSE_SEL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_H_LSE_SEL_RESETVAL (0x0000u)

/*----SEL Tokens----*/
#define CSL_IPIPE_RSZ_H_LSE_SEL_INTERNALVALUE (0x0000u)
#define CSL_IPIPE_RSZ_H_LSE_SEL_PROGRAMMEDVALUE (0x0001u)

#define CSL_IPIPE_RSZ_H_LSE_RESETVAL     (0x0000u)

/* RSZ_H_LPF */

#define CSL_IPIPE_RSZ_H_LPF_VAL_MASK     (0x00FFu)
#define CSL_IPIPE_RSZ_H_LPF_VAL_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_H_LPF_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_H_LPF_RESETVAL     (0x0000u)

/* RSZ_RGB_EN */

#define CSL_IPIPE_RSZ_RGB_EN_EN_MASK     (0x0001u)
#define CSL_IPIPE_RSZ_RGB_EN_EN_SHIFT    (0x0000u)
#define CSL_IPIPE_RSZ_RGB_EN_EN_RESETVAL (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_RSZ_RGB_EN_EN_DISABLE  (0x0000u)
#define CSL_IPIPE_RSZ_RGB_EN_EN_ENABLE   (0x0001u)

#define CSL_IPIPE_RSZ_RGB_EN_RESETVAL    (0x0000u)

/* RSZ_RGB_TYP */

#define CSL_IPIPE_RSZ_RGB_TYP_MSK1_MASK  (0x0004u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK1_SHIFT (0x0002u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK1_RESETVAL (0x0000u)

/*----MSK1 Tokens----*/
#define CSL_IPIPE_RSZ_RGB_TYP_MSK1_NOMASK (0x0000u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK1_MASKLAST2 (0x0001u)

#define CSL_IPIPE_RSZ_RGB_TYP_MSK0_MASK  (0x0002u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK0_SHIFT (0x0001u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK0_RESETVAL (0x0000u)

/*----MSK0 Tokens----*/
#define CSL_IPIPE_RSZ_RGB_TYP_MSK0_NOMASK (0x0000u)
#define CSL_IPIPE_RSZ_RGB_TYP_MSK0_MASKFIRST2 (0x0001u)

#define CSL_IPIPE_RSZ_RGB_TYP_TYP_MASK   (0x0001u)
#define CSL_IPIPE_RSZ_RGB_TYP_TYP_SHIFT  (0x0000u)
#define CSL_IPIPE_RSZ_RGB_TYP_TYP_RESETVAL (0x0000u)

/*----TYP Tokens----*/
#define CSL_IPIPE_RSZ_RGB_TYP_TYP_OUTPUT32BITS (0x0000u)
#define CSL_IPIPE_RSZ_RGB_TYP_TYP_OUTPUT16BITS (0x0001u)

#define CSL_IPIPE_RSZ_RGB_TYP_RESETVAL   (0x0000u)

/* RSZ_RGB_BLD */

#define CSL_IPIPE_RSZ_RGB_BLD_VAL_MASK   (0x00FFu)
#define CSL_IPIPE_RSZ_RGB_BLD_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_RSZ_RGB_BLD_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_RGB_BLD_RESETVAL   (0x0000u)

/* RSZ_SDR_BAD_H */

#define CSL_IPIPE_RSZ_SDR_BAD_H_VAL_MASK (0x0FFFu)
#define CSL_IPIPE_RSZ_SDR_BAD_H_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_BAD_H_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_BAD_H_RESETVAL (0x0000u)

/* RSZ_SDR_BAD_L */

#define CSL_IPIPE_RSZ_SDR_BAD_L_VAL_MASK (0xFFFFu)
#define CSL_IPIPE_RSZ_SDR_BAD_L_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_BAD_L_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_BAD_L_RESETVAL (0x0000u)

/* RSZ_SDR_SAD_H */

#define CSL_IPIPE_RSZ_SDR_SAD_H_VAL_MASK (0x0FFFu)
#define CSL_IPIPE_RSZ_SDR_SAD_H_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_SAD_H_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_SAD_H_RESETVAL (0x0000u)

/* RSZ_SDR_SAD_L */

#define CSL_IPIPE_RSZ_SDR_SAD_L_VAL_MASK (0xFFFFu)
#define CSL_IPIPE_RSZ_SDR_SAD_L_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_SAD_L_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_SAD_L_RESETVAL (0x0000u)

/* RSZ_SDR_OFT */

#define CSL_IPIPE_RSZ_SDR_OFT_OFT_MASK   (0xFFFFu)
#define CSL_IPIPE_RSZ_SDR_OFT_OFT_SHIFT  (0x0000u)
#define CSL_IPIPE_RSZ_SDR_OFT_OFT_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_OFT_RESETVAL   (0x0000u)

/* RSZ_SDR_PTR_S */

#define CSL_IPIPE_RSZ_SDR_PTR_S_VAL_MASK (0x1FFFu)
#define CSL_IPIPE_RSZ_SDR_PTR_S_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_PTR_S_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_PTR_S_RESETVAL (0x0000u)

/* RSZ_SDR_PTR_E */

#define CSL_IPIPE_RSZ_SDR_PTR_E_VAL_MASK (0x1FFFu)
#define CSL_IPIPE_RSZ_SDR_PTR_E_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_PTR_E_VAL_RESETVAL (0x1FFFu)

#define CSL_IPIPE_RSZ_SDR_PTR_E_RESETVAL (0x1FFFu)

/* RSZ_SDR_PTR_O */

#define CSL_IPIPE_RSZ_SDR_PTR_O_VAL_MASK (0x1FFFu)
#define CSL_IPIPE_RSZ_SDR_PTR_O_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_RSZ_SDR_PTR_O_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_RSZ_SDR_PTR_O_RESETVAL (0x0000u)

/* BOX_EN */

#define CSL_IPIPE_BOX_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_BOX_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_BOX_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_BOX_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_BOX_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_BOX_EN_RESETVAL        (0x0000u)

/* BOX_MODE */

#define CSL_IPIPE_BOX_MODE_OST_MASK      (0x0001u)
#define CSL_IPIPE_BOX_MODE_OST_SHIFT     (0x0000u)
#define CSL_IPIPE_BOX_MODE_OST_RESETVAL  (0x0000u)

/*----OST Tokens----*/
#define CSL_IPIPE_BOX_MODE_OST_CONTINUOUS (0x0000u)
#define CSL_IPIPE_BOX_MODE_OST_ONESHOT   (0x0001u)

#define CSL_IPIPE_BOX_MODE_RESETVAL      (0x0000u)

/* BOX_TYP */

#define CSL_IPIPE_BOX_TYP_SEL_MASK       (0x0001u)
#define CSL_IPIPE_BOX_TYP_SEL_SHIFT      (0x0000u)
#define CSL_IPIPE_BOX_TYP_SEL_RESETVAL   (0x0000u)

/*----SEL Tokens----*/
#define CSL_IPIPE_BOX_TYP_SEL__8X8       (0x0000u)
#define CSL_IPIPE_BOX_TYP_SEL__16X16     (0x0001u)

#define CSL_IPIPE_BOX_TYP_RESETVAL       (0x0000u)

/* BOX_SHF */

#define CSL_IPIPE_BOX_SHF_VAL_MASK       (0x0007u)
#define CSL_IPIPE_BOX_SHF_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_BOX_SHF_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_BOX_SHF_RESETVAL       (0x0000u)

/* HST_EN */

#define CSL_IPIPE_HST_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_HST_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_HST_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_HST_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_HST_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_HST_EN_RESETVAL        (0x0000u)

/* HST_MODE */

#define CSL_IPIPE_HST_MODE_OST_MASK      (0x0001u)
#define CSL_IPIPE_HST_MODE_OST_SHIFT     (0x0000u)
#define CSL_IPIPE_HST_MODE_OST_RESETVAL  (0x0000u)

/*----OST Tokens----*/
#define CSL_IPIPE_HST_MODE_OST_CONTINUOUS (0x0000u)
#define CSL_IPIPE_HST_MODE_OST_ONESHOT   (0x0001u)

#define CSL_IPIPE_HST_MODE_RESETVAL      (0x0000u)

/* HST_SEL */

#define CSL_IPIPE_HST_SEL_GREENSEL_MASK  (0x0002u)
#define CSL_IPIPE_HST_SEL_GREENSEL_SHIFT (0x0001u)
#define CSL_IPIPE_HST_SEL_GREENSEL_RESETVAL (0x0000u)

/*----GREENSEL Tokens----*/
#define CSL_IPIPE_HST_SEL_GREENSEL_GR    (0x0000u)
#define CSL_IPIPE_HST_SEL_GREENSEL_GB    (0x0001u)

#define CSL_IPIPE_HST_SEL_SOURCE_MASK    (0x0001u)
#define CSL_IPIPE_HST_SEL_SOURCE_SHIFT   (0x0000u)
#define CSL_IPIPE_HST_SEL_SOURCE_RESETVAL (0x0000u)

/*----SOURCE Tokens----*/
#define CSL_IPIPE_HST_SEL_SOURCE_FROMNOISEFILTER (0x0000u)
#define CSL_IPIPE_HST_SEL_SOURCE_FROMRGB2YUV (0x0001u)

#define CSL_IPIPE_HST_SEL_RESETVAL       (0x0000u)

/* HST_PARA */

#define CSL_IPIPE_HST_PARA_BIN_MASK      (0x3000u)
#define CSL_IPIPE_HST_PARA_BIN_SHIFT     (0x000Cu)
#define CSL_IPIPE_HST_PARA_BIN_RESETVAL  (0x0000u)

/*----BIN Tokens----*/
#define CSL_IPIPE_HST_PARA_BIN__32       (0x0000u)
#define CSL_IPIPE_HST_PARA_BIN__64       (0x0001u)
#define CSL_IPIPE_HST_PARA_BIN__128      (0x0002u)
#define CSL_IPIPE_HST_PARA_BIN__256      (0x0003u)

#define CSL_IPIPE_HST_PARA_SHF_MASK      (0x0F00u)
#define CSL_IPIPE_HST_PARA_SHF_SHIFT     (0x0008u)
#define CSL_IPIPE_HST_PARA_SHF_RESETVAL  (0x0000u)

#define CSL_IPIPE_HST_PARA_COL3_MASK     (0x0080u)
#define CSL_IPIPE_HST_PARA_COL3_SHIFT    (0x0007u)
#define CSL_IPIPE_HST_PARA_COL3_RESETVAL (0x0000u)

/*----COL3 Tokens----*/
#define CSL_IPIPE_HST_PARA_COL3_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_COL3_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_COL2_MASK     (0x0040u)
#define CSL_IPIPE_HST_PARA_COL2_SHIFT    (0x0006u)
#define CSL_IPIPE_HST_PARA_COL2_RESETVAL (0x0000u)

/*----COL2 Tokens----*/
#define CSL_IPIPE_HST_PARA_COL2_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_COL2_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_COL1_MASK     (0x0020u)
#define CSL_IPIPE_HST_PARA_COL1_SHIFT    (0x0005u)
#define CSL_IPIPE_HST_PARA_COL1_RESETVAL (0x0000u)

/*----COL1 Tokens----*/
#define CSL_IPIPE_HST_PARA_COL1_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_COL1_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_COL0_MASK     (0x0010u)
#define CSL_IPIPE_HST_PARA_COL0_SHIFT    (0x0004u)
#define CSL_IPIPE_HST_PARA_COL0_RESETVAL (0x0000u)

/*----COL0 Tokens----*/
#define CSL_IPIPE_HST_PARA_COL0_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_COL0_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_RGN3_MASK     (0x0008u)
#define CSL_IPIPE_HST_PARA_RGN3_SHIFT    (0x0003u)
#define CSL_IPIPE_HST_PARA_RGN3_RESETVAL (0x0000u)

/*----RGN3 Tokens----*/
#define CSL_IPIPE_HST_PARA_RGN3_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_RGN3_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_RGN2_MASK     (0x0004u)
#define CSL_IPIPE_HST_PARA_RGN2_SHIFT    (0x0002u)
#define CSL_IPIPE_HST_PARA_RGN2_RESETVAL (0x0000u)

/*----RGN2 Tokens----*/
#define CSL_IPIPE_HST_PARA_RGN2_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_RGN2_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_RGN1_MASK     (0x0002u)
#define CSL_IPIPE_HST_PARA_RGN1_SHIFT    (0x0001u)
#define CSL_IPIPE_HST_PARA_RGN1_RESETVAL (0x0000u)

/*----RGN1 Tokens----*/
#define CSL_IPIPE_HST_PARA_RGN1_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_RGN1_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_RGN0_MASK     (0x0001u)
#define CSL_IPIPE_HST_PARA_RGN0_SHIFT    (0x0000u)
#define CSL_IPIPE_HST_PARA_RGN0_RESETVAL (0x0000u)

/*----RGN0 Tokens----*/
#define CSL_IPIPE_HST_PARA_RGN0_DISABLE  (0x0000u)
#define CSL_IPIPE_HST_PARA_RGN0_ENABLE   (0x0001u)

#define CSL_IPIPE_HST_PARA_RESETVAL      (0x0000u)

/* HST_VST */

#define CSL_IPIPE_HST_VST_VAL_MASK       (0x0FFFu)
#define CSL_IPIPE_HST_VST_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_HST_VST_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_HST_VST_RESETVAL       (0x0000u)

/* HST_VSZ */

#define CSL_IPIPE_HST_VSZ_VAL_MASK       (0x0FFFu)
#define CSL_IPIPE_HST_VSZ_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_HST_VSZ_VAL_RESETVAL   (0x0001u)

#define CSL_IPIPE_HST_VSZ_RESETVAL       (0x0001u)

/* HST_HST */

#define CSL_IPIPE_HST_HST_VAL_MASK       (0x0FFFu)
#define CSL_IPIPE_HST_HST_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_HST_HST_VAL_RESETVAL   (0x0000u)

#define CSL_IPIPE_HST_HST_RESETVAL       (0x0000u)

/* HST_HSZ */

#define CSL_IPIPE_HST_HSZ_VAL_MASK       (0x0FFFu)
#define CSL_IPIPE_HST_HSZ_VAL_SHIFT      (0x0000u)
#define CSL_IPIPE_HST_HSZ_VAL_RESETVAL   (0x0001u)

#define CSL_IPIPE_HST_HSZ_RESETVAL       (0x0001u)

/* BSC_EN */

#define CSL_IPIPE_BSC_EN_EN_MASK         (0x0001u)
#define CSL_IPIPE_BSC_EN_EN_SHIFT        (0x0000u)
#define CSL_IPIPE_BSC_EN_EN_RESETVAL     (0x0000u)

/*----EN Tokens----*/
#define CSL_IPIPE_BSC_EN_EN_DISABLE      (0x0000u)
#define CSL_IPIPE_BSC_EN_EN_ENABLE       (0x0001u)

#define CSL_IPIPE_BSC_EN_RESETVAL        (0x0000u)

/* BSC_MODE */

#define CSL_IPIPE_BSC_MODE_OST_MASK      (0x0001u)
#define CSL_IPIPE_BSC_MODE_OST_SHIFT     (0x0000u)
#define CSL_IPIPE_BSC_MODE_OST_RESETVAL  (0x0000u)

/*----OST Tokens----*/
#define CSL_IPIPE_BSC_MODE_OST_CONTINUOUS (0x0000u)
#define CSL_IPIPE_BSC_MODE_OST_ONESHOT   (0x0001u)

#define CSL_IPIPE_BSC_MODE_RESETVAL      (0x0000u)

/* BSC_TYP */

#define CSL_IPIPE_BSC_TYP_CEN_MASK       (0x0008u)
#define CSL_IPIPE_BSC_TYP_CEN_SHIFT      (0x0003u)
#define CSL_IPIPE_BSC_TYP_CEN_RESETVAL   (0x0000u)

/*----CEN Tokens----*/
#define CSL_IPIPE_BSC_TYP_CEN_DISABLE    (0x0000u)
#define CSL_IPIPE_BSC_TYP_CEN_ENABLE     (0x0001u)

#define CSL_IPIPE_BSC_TYP_REN_MASK       (0x0004u)
#define CSL_IPIPE_BSC_TYP_REN_SHIFT      (0x0002u)
#define CSL_IPIPE_BSC_TYP_REN_RESETVAL   (0x0000u)

/*----REN Tokens----*/
#define CSL_IPIPE_BSC_TYP_REN_DISABLE    (0x0000u)
#define CSL_IPIPE_BSC_TYP_REN_ENABLE     (0x0001u)

#define CSL_IPIPE_BSC_TYP_COL_MASK       (0x0003u)
#define CSL_IPIPE_BSC_TYP_COL_SHIFT      (0x0000u)
#define CSL_IPIPE_BSC_TYP_COL_RESETVAL   (0x0000u)

/*----COL Tokens----*/
#define CSL_IPIPE_BSC_TYP_COL_Y          (0x0000u)
#define CSL_IPIPE_BSC_TYP_COL_CB         (0x0001u)
#define CSL_IPIPE_BSC_TYP_COL_CR         (0x0002u)

#define CSL_IPIPE_BSC_TYP_RESETVAL       (0x0000u)

/* BSC_ROW_VCT */

#define CSL_IPIPE_BSC_ROW_VCT_ROWNUM_MASK (0x0003u)
#define CSL_IPIPE_BSC_ROW_VCT_ROWNUM_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_VCT_ROWNUM_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_VCT_RESETVAL   (0x0000u)

/* BSC_ROW_SHF */

#define CSL_IPIPE_BSC_ROW_SHF_VAL_MASK   (0x0007u)
#define CSL_IPIPE_BSC_ROW_SHF_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_BSC_ROW_SHF_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_SHF_RESETVAL   (0x0000u)

/* BSC_ROW_VPOS */

#define CSL_IPIPE_BSC_ROW_VPOS_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_ROW_VPOS_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_VPOS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_VPOS_RESETVAL  (0x0000u)

/* BSC_ROW_VNUM */

#define CSL_IPIPE_BSC_ROW_VNUM_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_ROW_VNUM_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_VNUM_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_VNUM_RESETVAL  (0x0000u)

/* BSC_ROW_VSKIP */

#define CSL_IPIPE_BSC_ROW_VSKIP_ROWSKIPV_MASK (0x001Fu)
#define CSL_IPIPE_BSC_ROW_VSKIP_ROWSKIPV_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_VSKIP_ROWSKIPV_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_VSKIP_RESETVAL (0x0000u)

/* BSC_ROW_HPOS */

#define CSL_IPIPE_BSC_ROW_HPOS_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_ROW_HPOS_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_HPOS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_HPOS_RESETVAL  (0x0000u)

/* BSC_ROW_HNUM */

#define CSL_IPIPE_BSC_ROW_HNUM_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_ROW_HNUM_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_HNUM_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_HNUM_RESETVAL  (0x0000u)

/* BSC_ROW_HSKIP */

#define CSL_IPIPE_BSC_ROW_HSKIP_ROWSKIPH_MASK (0x001Fu)
#define CSL_IPIPE_BSC_ROW_HSKIP_ROWSKIPH_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_HSKIP_ROWSKIPH_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_HSKIP_RESETVAL (0x0000u)

/* BSC_COL_VCT */

#define CSL_IPIPE_BSC_COL_VCT_COLNUM_MASK (0x0003u)
#define CSL_IPIPE_BSC_COL_VCT_COLNUM_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_VCT_COLNUM_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_VCT_RESETVAL   (0x0000u)

/* BSC_COL_SHF */

#define CSL_IPIPE_BSC_COL_SHF_VAL_MASK   (0x0007u)
#define CSL_IPIPE_BSC_COL_SHF_VAL_SHIFT  (0x0000u)
#define CSL_IPIPE_BSC_COL_SHF_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_SHF_RESETVAL   (0x0000u)

/* BSC_COL_VPOS */

#define CSL_IPIPE_BSC_COL_VPOS_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_COL_VPOS_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_VPOS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_VPOS_RESETVAL  (0x0000u)

/* BSC_COL_VNUM */

#define CSL_IPIPE_BSC_COL_VNUM_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_COL_VNUM_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_VNUM_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_VNUM_RESETVAL  (0x0000u)

/* BSC_COL_VSKIP */

#define CSL_IPIPE_BSC_COL_VSKIP_COLSKIPV_MASK (0x001Fu)
#define CSL_IPIPE_BSC_COL_VSKIP_COLSKIPV_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_VSKIP_COLSKIPV_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_VSKIP_RESETVAL (0x0000u)

/* BSC_COL_HPOS */

#define CSL_IPIPE_BSC_COL_HPOS_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_COL_HPOS_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_HPOS_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_HPOS_RESETVAL  (0x0000u)

/* BSC_COL_HNUM */

#define CSL_IPIPE_BSC_COL_HNUM_VAL_MASK  (0x1FFFu)
#define CSL_IPIPE_BSC_COL_HNUM_VAL_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_HNUM_VAL_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_HNUM_RESETVAL  (0x0000u)

/* BSC_COL_HSKIP */

#define CSL_IPIPE_BSC_COL_HSKIP_COLSKIPH_MASK (0x001Fu)
#define CSL_IPIPE_BSC_COL_HSKIP_COLSKIPH_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_HSKIP_COLSKIPH_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_HSKIP_RESETVAL (0x0000u)

/* BSC_ROW_SUM */

#define CSL_IPIPE_BSC_ROW_SUM_DATA_MASK  (0xFFFFu)
#define CSL_IPIPE_BSC_ROW_SUM_DATA_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_ROW_SUM_DATA_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_ROW_SUM_RESETVAL   (0x0000u)

/* BSC_COL_SUM */

#define CSL_IPIPE_BSC_COL_SUM_DATA_MASK  (0xFFFFu)
#define CSL_IPIPE_BSC_COL_SUM_DATA_SHIFT (0x0000u)
#define CSL_IPIPE_BSC_COL_SUM_DATA_RESETVAL (0x0000u)

#define CSL_IPIPE_BSC_COL_SUM_RESETVAL   (0x0000u)

#endif
