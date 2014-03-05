#ifndef _CSLR_VENC_1_H_
#define _CSLR_VENC_1_H_
/*********************************************************************
 * Copyright (C) 2003-2006 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_venc_1.h
 * 
 * \brief This file contains the Register Desciptions for VENC
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 VMOD;
    volatile Uint8 RSVD0[2];
    volatile Uint16 VIDCTL;
    volatile Uint8 RSVD1[2];
    volatile Uint16 VDPRO;
    volatile Uint8 RSVD2[2];
    volatile Uint16 SYNCCTL;
    volatile Uint8 RSVD3[2];
    volatile Uint16 HSPLS;
    volatile Uint8 RSVD4[2];
    volatile Uint16 VSPLS;
    volatile Uint8 RSVD5[2];
    volatile Uint16 HINT;
    volatile Uint8 RSVD6[2];
    volatile Uint16 HSTART;
    volatile Uint8 RSVD7[2];
    volatile Uint16 HVALID;
    volatile Uint8 RSVD8[2];
    volatile Uint16 VINT;
    volatile Uint8 RSVD9[2];
    volatile Uint16 VSTART;
    volatile Uint8 RSVD10[2];
    volatile Uint16 VVALID;
    volatile Uint8 RSVD11[2];
    volatile Uint16 HSDLY;
    volatile Uint8 RSVD12[2];
    volatile Uint16 VSDLY;
    volatile Uint8 RSVD13[2];
    volatile Uint16 YCCCTL;
    volatile Uint8 RSVD14[2];
    volatile Uint16 RGBCTL;
    volatile Uint8 RSVD15[2];
    volatile Uint16 RGBCLP;
    volatile Uint8 RSVD16[2];
    volatile Uint16 LINECTL;
    volatile Uint8 RSVD17[2];
    volatile Uint16 CULLLINE;
    volatile Uint8 RSVD18[2];
    volatile Uint16 LCDOUT;
    volatile Uint8 RSVD19[2];
    volatile Uint16 BRTS;
    volatile Uint8 RSVD20[2];
    volatile Uint16 BRTW;
    volatile Uint8 RSVD21[2];
    volatile Uint16 ACCTL;
    volatile Uint8 RSVD22[2];
    volatile Uint16 PWMP;
    volatile Uint8 RSVD23[2];
    volatile Uint16 PWMW;
    volatile Uint8 RSVD24[2];
    volatile Uint16 DCLKCTL;
    volatile Uint8 RSVD25[2];
    volatile Uint16 DCLKPTN0;
    volatile Uint8 RSVD26[2];
    volatile Uint16 DCLKPTN1;
    volatile Uint8 RSVD27[2];
    volatile Uint16 DCLKPTN2;
    volatile Uint8 RSVD28[2];
    volatile Uint16 DCLKPTN3;
    volatile Uint8 RSVD29[2];
    volatile Uint16 DCLKPTN0A;
    volatile Uint8 RSVD30[2];
    volatile Uint16 DCLKPTN1A;
    volatile Uint8 RSVD31[2];
    volatile Uint16 DCLKPTN2A;
    volatile Uint8 RSVD32[2];
    volatile Uint16 DCLKPTN3A;
    volatile Uint8 RSVD33[2];
    volatile Uint16 DCLKHS;
    volatile Uint8 RSVD34[2];
    volatile Uint16 DCLKHSA;
    volatile Uint8 RSVD35[2];
    volatile Uint16 DCLKHR;
    volatile Uint8 RSVD36[2];
    volatile Uint16 DCLKVS;
    volatile Uint8 RSVD37[2];
    volatile Uint16 DCLKVR;
    volatile Uint8 RSVD38[2];
    volatile Uint16 CAPCTL;
    volatile Uint8 RSVD39[2];
    volatile Uint16 CAPDO;
    volatile Uint8 RSVD40[2];
    volatile Uint16 CAPDE;
    volatile Uint8 RSVD41[2];
    volatile Uint16 ATR0;
    volatile Uint8 RSVD42[2];
    volatile Uint16 ATR1;
    volatile Uint8 RSVD43[2];
    volatile Uint16 ATR2;
    volatile Uint8 RSVD44[2];
    volatile Uint16 RSV0;
    volatile Uint8 RSVD45[2];
    volatile Uint16 VSTAT;
    volatile Uint8 RSVD46[2];
    volatile Uint16 RAMADR;
    volatile Uint8 RSVD47[2];
    volatile Uint16 RAMPORT;
    volatile Uint8 RSVD48[2];
    volatile Uint16 DACTST;
    volatile Uint8 RSVD49[2];
    volatile Uint16 YCOLVL;
    volatile Uint8 RSVD50[2];
    volatile Uint16 SCPROG;
    volatile Uint8 RSVD51[2];
    volatile Uint16 RSV1;
    volatile Uint8 RSVD52[2];
    volatile Uint16 RSV2;
    volatile Uint8 RSVD53[2];
    volatile Uint16 RSV3;
    volatile Uint8 RSVD54[2];
    volatile Uint16 CVBS;
    volatile Uint8 RSVD55[2];
    volatile Uint16 RSV4;
    volatile Uint8 RSVD56[2];
    volatile Uint16 ETMG0;
    volatile Uint8 RSVD57[2];
    volatile Uint16 ETMG1;
    volatile Uint8 RSVD58[2];
    volatile Uint16 RSV5;
    volatile Uint8 RSVD59[2];
    volatile Uint16 RSV6;
    volatile Uint8 RSVD60[2];
    volatile Uint16 RSV7;
    volatile Uint8 RSVD61[2];
    volatile Uint16 RSV8;
    volatile Uint8 RSVD62[2];
    volatile Uint16 RSV9;
    volatile Uint8 RSVD63[2];
    volatile Uint16 RSV10;
    volatile Uint8 RSVD64[2];
    volatile Uint16 RSV11;
    volatile Uint8 RSVD65[2];
    volatile Uint16 RSV12;
    volatile Uint8 RSVD66[2];
    volatile Uint16 RSV13;
    volatile Uint8 RSVD67[2];
    volatile Uint16 RSV14;
    volatile Uint8 RSVD68[2];
    volatile Uint16 DRGBX0;
    volatile Uint8 RSVD69[2];
    volatile Uint16 DRGBX1;
    volatile Uint8 RSVD70[2];
    volatile Uint16 DRGBX2;
    volatile Uint8 RSVD71[2];
    volatile Uint16 DRGBX3;
    volatile Uint8 RSVD72[2];
    volatile Uint16 DRGBX4;
    volatile Uint8 RSVD73[2];
    volatile Uint16 VSTARTA;
    volatile Uint8 RSVD74[2];
    volatile Uint16 OSDCLK0;
    volatile Uint8 RSVD75[2];
    volatile Uint16 OSDCLK1;
    volatile Uint8 RSVD76[2];
    volatile Uint16 HVLDCL0;
    volatile Uint8 RSVD77[2];
    volatile Uint16 HVLDCL1;
    volatile Uint8 RSVD78[2];
    volatile Uint16 OSDHADV;
    volatile Uint8 RSVD79[2];
    volatile Uint16 CLKCTL;
    volatile Uint8 RSVD80[2];
    volatile Uint16 GAMCTL;
} CSL_VencRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* VMOD */

#define CSL_VENC_VMOD_VDMD_MASK          (0x3000u)
#define CSL_VENC_VMOD_VDMD_SHIFT         (0x000Cu)
#define CSL_VENC_VMOD_VDMD_RESETVAL      (0x0000u)

/*----VDMD Tokens----*/
#define CSL_VENC_VMOD_VDMD_YCBCR_16BIT   (0x0000u)
#define CSL_VENC_VMOD_VDMD_YCBCR_8BIT    (0x0001u)
#define CSL_VENC_VMOD_VDMD_RGB666_18BIT  (0x0002u)
#define CSL_VENC_VMOD_VDMD_RGB8_8BIT     (0x0003u)

#define CSL_VENC_VMOD_ITLCL_MASK         (0x0800u)
#define CSL_VENC_VMOD_ITLCL_SHIFT        (0x000Bu)
#define CSL_VENC_VMOD_ITLCL_RESETVAL     (0x0000u)

/*----ITLCL Tokens----*/
#define CSL_VENC_VMOD_ITLCL_262NTSC_312PAL (0x0000u)
#define CSL_VENC_VMOD_ITLCL_263NTSC_313PAL (0x0001u)

#define CSL_VENC_VMOD_ITLC_MASK          (0x0400u)
#define CSL_VENC_VMOD_ITLC_SHIFT         (0x000Au)
#define CSL_VENC_VMOD_ITLC_RESETVAL      (0x0000u)

/*----ITLC Tokens----*/
#define CSL_VENC_VMOD_ITLC_INTERLACE     (0x0000u)
#define CSL_VENC_VMOD_ITLC_NON_INTERLACE (0x0001u)

#define CSL_VENC_VMOD_NSIT_MASK          (0x0200u)
#define CSL_VENC_VMOD_NSIT_SHIFT         (0x0009u)
#define CSL_VENC_VMOD_NSIT_RESETVAL      (0x0000u)

/*----NSIT Tokens----*/
#define CSL_VENC_VMOD_NSIT_PROGRESSIVE   (0x0000u)
#define CSL_VENC_VMOD_NSIT_INTERLACE     (0x0001u)

#define CSL_VENC_VMOD_HDMD_MASK          (0x0100u)
#define CSL_VENC_VMOD_HDMD_SHIFT         (0x0008u)
#define CSL_VENC_VMOD_HDMD_RESETVAL      (0x0000u)

/*----HDMD Tokens----*/
#define CSL_VENC_VMOD_HDMD_SDTV          (0x0000u)
#define CSL_VENC_VMOD_HDMD_HDTV          (0x0001u)

#define CSL_VENC_VMOD_TVTYP_MASK         (0x00C0u)
#define CSL_VENC_VMOD_TVTYP_SHIFT        (0x0006u)
#define CSL_VENC_VMOD_TVTYP_RESETVAL     (0x0000u)

/*----TVTYP Tokens----*/
#define CSL_VENC_VMOD_TVTYP_NTSC_525P    (0x0000u)
#define CSL_VENC_VMOD_TVTYP_PAL_625P     (0x0001u)
#define CSL_VENC_VMOD_TVTYP_RESERVED1    (0x0002u)
#define CSL_VENC_VMOD_TVTYP_RESERVED2    (0x0003u)

#define CSL_VENC_VMOD_SLAVE_MASK         (0x0020u)
#define CSL_VENC_VMOD_SLAVE_SHIFT        (0x0005u)
#define CSL_VENC_VMOD_SLAVE_RESETVAL     (0x0000u)

/*----SLAVE Tokens----*/
#define CSL_VENC_VMOD_SLAVE_MASTERMODE   (0x0000u)
#define CSL_VENC_VMOD_SLAVE_SLAVEMODE    (0x0001u)

#define CSL_VENC_VMOD_VMD_MASK           (0x0010u)
#define CSL_VENC_VMOD_VMD_SHIFT          (0x0004u)
#define CSL_VENC_VMOD_VMD_RESETVAL       (0x0000u)

/*----VMD Tokens----*/
#define CSL_VENC_VMOD_VMD_NTSC_PALTIMING (0x0000u)
#define CSL_VENC_VMOD_VMD_NOTNTSC_PALTIMING (0x0001u)

#define CSL_VENC_VMOD_BLNK_MASK          (0x0008u)
#define CSL_VENC_VMOD_BLNK_SHIFT         (0x0003u)
#define CSL_VENC_VMOD_BLNK_RESETVAL      (0x0000u)

/*----BLNK Tokens----*/
#define CSL_VENC_VMOD_BLNK_NORMAL        (0x0000u)
#define CSL_VENC_VMOD_BLNK_FORCEBLANKING (0x0001u)

#define CSL_VENC_VMOD_VIE_MASK           (0x0002u)
#define CSL_VENC_VMOD_VIE_SHIFT          (0x0001u)
#define CSL_VENC_VMOD_VIE_RESETVAL       (0x0000u)

/*----VIE Tokens----*/
#define CSL_VENC_VMOD_VIE_FIXED_L_LEVEL_OUT (0x0000u)
#define CSL_VENC_VMOD_VIE_NORMAL_COMPOSITE_OUT (0x0001u)

#define CSL_VENC_VMOD_VENC_MASK          (0x0001u)
#define CSL_VENC_VMOD_VENC_SHIFT         (0x0000u)
#define CSL_VENC_VMOD_VENC_RESETVAL      (0x0000u)

/*----VENC Tokens----*/
#define CSL_VENC_VMOD_VENC_DISABLE       (0x0000u)
#define CSL_VENC_VMOD_VENC_ENABLE        (0x0001u)

#define CSL_VENC_VMOD_RESETVAL           (0x0000u)

/* VIDCTL */

#define CSL_VENC_VIDCTL_VCLKP_MASK       (0x4000u)
#define CSL_VENC_VIDCTL_VCLKP_SHIFT      (0x000Eu)
#define CSL_VENC_VIDCTL_VCLKP_RESETVAL   (0x0000u)

/*----VCLKP Tokens----*/
#define CSL_VENC_VIDCTL_VCLKP_NON_INVERSE (0x0000u)
#define CSL_VENC_VIDCTL_VCLKP_INVERSE    (0x0001u)

#define CSL_VENC_VIDCTL_VCLKE_MASK       (0x2000u)
#define CSL_VENC_VIDCTL_VCLKE_SHIFT      (0x000Du)
#define CSL_VENC_VIDCTL_VCLKE_RESETVAL   (0x0000u)

/*----VCLKE Tokens----*/
#define CSL_VENC_VIDCTL_VCLKE_OFF        (0x0000u)
#define CSL_VENC_VIDCTL_VCLKE_ON         (0x0001u)

#define CSL_VENC_VIDCTL_VCLKZ_MASK       (0x1000u)
#define CSL_VENC_VIDCTL_VCLKZ_SHIFT      (0x000Cu)
#define CSL_VENC_VIDCTL_VCLKZ_RESETVAL   (0x0001u)

/*----VCLKZ Tokens----*/
#define CSL_VENC_VIDCTL_VCLKZ_OUTPUT     (0x0000u)
#define CSL_VENC_VIDCTL_VCLKZ_HIGHIMPEDENCE (0x0001u)

#define CSL_VENC_VIDCTL_SYDIR_MASK       (0x0100u)
#define CSL_VENC_VIDCTL_SYDIR_SHIFT      (0x0008u)
#define CSL_VENC_VIDCTL_SYDIR_RESETVAL   (0x0001u)

/*----SYDIR Tokens----*/
#define CSL_VENC_VIDCTL_SYDIR_OUTPUT     (0x0000u)
#define CSL_VENC_VIDCTL_SYDIR_INPUT      (0x0001u)

#define CSL_VENC_VIDCTL_DOMD_MASK        (0x0030u)
#define CSL_VENC_VIDCTL_DOMD_SHIFT       (0x0004u)
#define CSL_VENC_VIDCTL_DOMD_RESETVAL    (0x0000u)

/*----DOMD Tokens----*/
#define CSL_VENC_VIDCTL_DOMD_NORMAL_OUT  (0x0000u)
#define CSL_VENC_VIDCTL_DOMD_INVERSE_OUT (0x0001u)
#define CSL_VENC_VIDCTL_DOMD_L_LEVEL_OUT (0x0002u)
#define CSL_VENC_VIDCTL_DOMD_H_LEVEL_OUT (0x0003u)

#define CSL_VENC_VIDCTL_YCSWAP_MASK      (0x0008u)
#define CSL_VENC_VIDCTL_YCSWAP_SHIFT     (0x0003u)
#define CSL_VENC_VIDCTL_YCSWAP_RESETVAL  (0x0000u)

/*----YCSWAP Tokens----*/
#define CSL_VENC_VIDCTL_YCSWAP_NORMAL    (0x0000u)
#define CSL_VENC_VIDCTL_YCSWAP_SWAP_YC   (0x0001u)

#define CSL_VENC_VIDCTL_YCOL_MASK        (0x0004u)
#define CSL_VENC_VIDCTL_YCOL_SHIFT       (0x0002u)
#define CSL_VENC_VIDCTL_YCOL_RESETVAL    (0x0000u)

/*----YCOL Tokens----*/
#define CSL_VENC_VIDCTL_YCOL_NORMAL_OUT  (0x0000u)
#define CSL_VENC_VIDCTL_YCOL_DCLEVEL_OUT (0x0001u)

#define CSL_VENC_VIDCTL_YCOMD_MASK       (0x0002u)
#define CSL_VENC_VIDCTL_YCOMD_SHIFT      (0x0001u)
#define CSL_VENC_VIDCTL_YCOMD_RESETVAL   (0x0000u)

/*----YCOMD Tokens----*/
#define CSL_VENC_VIDCTL_YCOMD_DIGITAL_VIDEO_OUT (0x0000u)
#define CSL_VENC_VIDCTL_YCOMD_YC_INPUT_PASSTHRU (0x0001u)

#define CSL_VENC_VIDCTL_YCDIR_MASK       (0x0001u)
#define CSL_VENC_VIDCTL_YCDIR_SHIFT      (0x0000u)
#define CSL_VENC_VIDCTL_YCDIR_RESETVAL   (0x0001u)

/*----YCDIR Tokens----*/
#define CSL_VENC_VIDCTL_YCDIR_OUTPUT     (0x0000u)
#define CSL_VENC_VIDCTL_YCDIR_INPUT      (0x0001u)

#define CSL_VENC_VIDCTL_RESETVAL         (0x1101u)

/* VDPRO */

#define CSL_VENC_VDPRO_PFLTC_MASK        (0xC000u)
#define CSL_VENC_VDPRO_PFLTC_SHIFT       (0x000Eu)
#define CSL_VENC_VDPRO_PFLTC_RESETVAL    (0x0000u)

/*----PFLTC Tokens----*/
#define CSL_VENC_VDPRO_PFLTC_NOFILTER    (0x0000u)
#define CSL_VENC_VDPRO_PFLTC_1_1         (0x0001u)
#define CSL_VENC_VDPRO_PFLTC_1_2_1       (0x0002u)
#define CSL_VENC_VDPRO_PFLTC_RESERVED    (0x0003u)

#define CSL_VENC_VDPRO_PFLTY_MASK        (0x3000u)
#define CSL_VENC_VDPRO_PFLTY_SHIFT       (0x000Cu)
#define CSL_VENC_VDPRO_PFLTY_RESETVAL    (0x0000u)

/*----PFLTY Tokens----*/
#define CSL_VENC_VDPRO_PFLTY_NOFILTER    (0x0000u)
#define CSL_VENC_VDPRO_PFLTY_1_1         (0x0001u)
#define CSL_VENC_VDPRO_PFLTY_1_2_1       (0x0002u)
#define CSL_VENC_VDPRO_PFLTY_RESERVED    (0x0003u)

#define CSL_VENC_VDPRO_PFLTR_MASK        (0x0800u)
#define CSL_VENC_VDPRO_PFLTR_SHIFT       (0x000Bu)
#define CSL_VENC_VDPRO_PFLTR_RESETVAL    (0x0000u)

/*----PFLTR Tokens----*/
#define CSL_VENC_VDPRO_PFLTR_ENCCLOCK_2  (0x0000u)
#define CSL_VENC_VDPRO_PFLTR_ENCCLOCK    (0x0001u)

#define CSL_VENC_VDPRO_CBTYP_MASK        (0x0200u)
#define CSL_VENC_VDPRO_CBTYP_SHIFT       (0x0009u)
#define CSL_VENC_VDPRO_CBTYP_RESETVAL    (0x0000u)

/*----CBTYP Tokens----*/
#define CSL_VENC_VDPRO_CBTYP_75PCT       (0x0000u)
#define CSL_VENC_VDPRO_CBTYP_100PCT      (0x0001u)

#define CSL_VENC_VDPRO_CBMD_MASK         (0x0100u)
#define CSL_VENC_VDPRO_CBMD_SHIFT        (0x0008u)
#define CSL_VENC_VDPRO_CBMD_RESETVAL     (0x0000u)

/*----CBMD Tokens----*/
#define CSL_VENC_VDPRO_CBMD_NORMAL       (0x0000u)
#define CSL_VENC_VDPRO_CBMD_COLORBAR     (0x0001u)

#define CSL_VENC_VDPRO_ATRGB_MASK        (0x0040u)
#define CSL_VENC_VDPRO_ATRGB_SHIFT       (0x0006u)
#define CSL_VENC_VDPRO_ATRGB_RESETVAL    (0x0000u)

/*----ATRGB Tokens----*/
#define CSL_VENC_VDPRO_ATRGB_NOATTENUATION (0x0000u)
#define CSL_VENC_VDPRO_ATRGB_REC601_LVL  (0x0001u)

#define CSL_VENC_VDPRO_ATYCC_MASK        (0x0020u)
#define CSL_VENC_VDPRO_ATYCC_SHIFT       (0x0005u)
#define CSL_VENC_VDPRO_ATYCC_RESETVAL    (0x0000u)

/*----ATYCC Tokens----*/
#define CSL_VENC_VDPRO_ATYCC_NOATTENUATION (0x0000u)
#define CSL_VENC_VDPRO_ATYCC_REC601_LVL  (0x0001u)

#define CSL_VENC_VDPRO_ATCOM_MASK        (0x0010u)
#define CSL_VENC_VDPRO_ATCOM_SHIFT       (0x0004u)
#define CSL_VENC_VDPRO_ATCOM_RESETVAL    (0x0000u)

/*----ATCOM Tokens----*/
#define CSL_VENC_VDPRO_ATCOM_NOATTENUATION (0x0000u)
#define CSL_VENC_VDPRO_ATCOM_REC601_LVL  (0x0001u)

#define CSL_VENC_VDPRO_CUPS_MASK         (0x0002u)
#define CSL_VENC_VDPRO_CUPS_SHIFT        (0x0001u)
#define CSL_VENC_VDPRO_CUPS_RESETVAL     (0x0000u)

/*----CUPS Tokens----*/
#define CSL_VENC_VDPRO_CUPS_OFF          (0x0000u)
#define CSL_VENC_VDPRO_CUPS_ON           (0x0001u)

#define CSL_VENC_VDPRO_YUPS_MASK         (0x0001u)
#define CSL_VENC_VDPRO_YUPS_SHIFT        (0x0000u)
#define CSL_VENC_VDPRO_YUPS_RESETVAL     (0x0000u)

/*----YUPS Tokens----*/
#define CSL_VENC_VDPRO_YUPS_OFF          (0x0000u)
#define CSL_VENC_VDPRO_YUPS_ON           (0x0001u)

#define CSL_VENC_VDPRO_RESETVAL          (0x0000u)

/* SYNCCTL */

#define CSL_VENC_SYNCCTL_OVD_MASK        (0x4000u)
#define CSL_VENC_SYNCCTL_OVD_SHIFT       (0x000Eu)
#define CSL_VENC_SYNCCTL_OVD_RESETVAL    (0x0000u)

/*----OVD Tokens----*/
#define CSL_VENC_SYNCCTL_OVD_NODELAY     (0x0000u)
#define CSL_VENC_SYNCCTL_OVD_DEALY       (0x0001u)

#define CSL_VENC_SYNCCTL_EXFMD_MASK      (0x3000u)
#define CSL_VENC_SYNCCTL_EXFMD_SHIFT     (0x000Cu)
#define CSL_VENC_SYNCCTL_EXFMD_RESETVAL  (0x0000u)

/*----EXFMD Tokens----*/
#define CSL_VENC_SYNCCTL_EXFMD_LATCH_EXT_FLD_AT_VDHIGH (0x0000u)
#define CSL_VENC_SYNCCTL_EXFMD_USE_RAW_EXT_FLD (0x0001u)
#define CSL_VENC_SYNCCTL_EXFMD_USE_EXT_VSYNC_AS_FLDID (0x0002u)
#define CSL_VENC_SYNCCTL_EXFMD_DET_EXT_VSYNC_PH (0x0003u)

#define CSL_VENC_SYNCCTL_EXFIV_MASK      (0x0800u)
#define CSL_VENC_SYNCCTL_EXFIV_SHIFT     (0x000Bu)
#define CSL_VENC_SYNCCTL_EXFIV_RESETVAL  (0x0000u)

/*----EXFIV Tokens----*/
#define CSL_VENC_SYNCCTL_EXFIV_NON_INVERSE (0x0000u)
#define CSL_VENC_SYNCCTL_EXFIV_INVERSE   (0x0001u)

#define CSL_VENC_SYNCCTL_EXSYNC_MASK     (0x0400u)
#define CSL_VENC_SYNCCTL_EXSYNC_SHIFT    (0x000Au)
#define CSL_VENC_SYNCCTL_EXSYNC_RESETVAL (0x0000u)

/*----EXSYNC Tokens----*/
#define CSL_VENC_SYNCCTL_EXSYNC_HSYNC_VSYNCPIN (0x0000u)
#define CSL_VENC_SYNCCTL_EXSYNC_CCDSYNCSIGNAL (0x0001u)

#define CSL_VENC_SYNCCTL_EXVIV_MASK      (0x0200u)
#define CSL_VENC_SYNCCTL_EXVIV_SHIFT     (0x0009u)
#define CSL_VENC_SYNCCTL_EXVIV_RESETVAL  (0x0000u)

/*----EXVIV Tokens----*/
#define CSL_VENC_SYNCCTL_EXVIV_ACTIVEH   (0x0000u)
#define CSL_VENC_SYNCCTL_EXVIV_ACTIVEL   (0x0001u)

#define CSL_VENC_SYNCCTL_EXHIV_MASK      (0x0100u)
#define CSL_VENC_SYNCCTL_EXHIV_SHIFT     (0x0008u)
#define CSL_VENC_SYNCCTL_EXHIV_RESETVAL  (0x0000u)

/*----EXHIV Tokens----*/
#define CSL_VENC_SYNCCTL_EXHIV_ACTIVEH   (0x0000u)
#define CSL_VENC_SYNCCTL_EXHIV_ACTIVEL   (0x0001u)

#define CSL_VENC_SYNCCTL_CSP_MASK        (0x0080u)
#define CSL_VENC_SYNCCTL_CSP_SHIFT       (0x0007u)
#define CSL_VENC_SYNCCTL_CSP_RESETVAL    (0x0000u)

/*----CSP Tokens----*/
#define CSL_VENC_SYNCCTL_CSP_ACTIVEH     (0x0000u)
#define CSL_VENC_SYNCCTL_CSP_ACTIVEL     (0x0001u)

#define CSL_VENC_SYNCCTL_CSE_MASK        (0x0040u)
#define CSL_VENC_SYNCCTL_CSE_SHIFT       (0x0006u)
#define CSL_VENC_SYNCCTL_CSE_RESETVAL    (0x0000u)

/*----CSE Tokens----*/
#define CSL_VENC_SYNCCTL_CSE_OFF         (0x0000u)
#define CSL_VENC_SYNCCTL_CSE_ON          (0x0001u)

#define CSL_VENC_SYNCCTL_SYSW_MASK       (0x0020u)
#define CSL_VENC_SYNCCTL_SYSW_SHIFT      (0x0005u)
#define CSL_VENC_SYNCCTL_SYSW_RESETVAL   (0x0000u)

/*----SYSW Tokens----*/
#define CSL_VENC_SYNCCTL_SYSW_NORMAL     (0x0000u)
#define CSL_VENC_SYNCCTL_SYSW_SYNC_PW_PROC (0x0001u)

#define CSL_VENC_SYNCCTL_VSYNCS_MASK     (0x0010u)
#define CSL_VENC_SYNCCTL_VSYNCS_SHIFT    (0x0004u)
#define CSL_VENC_SYNCCTL_VSYNCS_RESETVAL (0x0000u)

/*----VSYNCS Tokens----*/
#define CSL_VENC_SYNCCTL_VSYNCS_VSYNC    (0x0000u)
#define CSL_VENC_SYNCCTL_VSYNCS_COMPOSITE_SYNC (0x0001u)

#define CSL_VENC_SYNCCTL_VPL_MASK        (0x0008u)
#define CSL_VENC_SYNCCTL_VPL_SHIFT       (0x0003u)
#define CSL_VENC_SYNCCTL_VPL_RESETVAL    (0x0000u)

/*----VPL Tokens----*/
#define CSL_VENC_SYNCCTL_VPL_ACTIVEH     (0x0000u)
#define CSL_VENC_SYNCCTL_VPL_ACTIVEL     (0x0001u)

#define CSL_VENC_SYNCCTL_HPL_MASK        (0x0004u)
#define CSL_VENC_SYNCCTL_HPL_SHIFT       (0x0002u)
#define CSL_VENC_SYNCCTL_HPL_RESETVAL    (0x0000u)

/*----HPL Tokens----*/
#define CSL_VENC_SYNCCTL_HPL_ACTIVEH     (0x0000u)
#define CSL_VENC_SYNCCTL_HPL_ACTIVEL     (0x0001u)

#define CSL_VENC_SYNCCTL_SYEV_MASK       (0x0002u)
#define CSL_VENC_SYNCCTL_SYEV_SHIFT      (0x0001u)
#define CSL_VENC_SYNCCTL_SYEV_RESETVAL   (0x0000u)

/*----SYEV Tokens----*/
#define CSL_VENC_SYNCCTL_SYEV_OFF        (0x0000u)
#define CSL_VENC_SYNCCTL_SYEV_ON         (0x0001u)

#define CSL_VENC_SYNCCTL_SYEH_MASK       (0x0001u)
#define CSL_VENC_SYNCCTL_SYEH_SHIFT      (0x0000u)
#define CSL_VENC_SYNCCTL_SYEH_RESETVAL   (0x0000u)

/*----SYEH Tokens----*/
#define CSL_VENC_SYNCCTL_SYEH_OFF        (0x0000u)
#define CSL_VENC_SYNCCTL_SYEH_ON         (0x0001u)

#define CSL_VENC_SYNCCTL_RESETVAL        (0x0000u)

/* HSPLS */

#define CSL_VENC_HSPLS_HSPLS_MASK        (0x1FFFu)
#define CSL_VENC_HSPLS_HSPLS_SHIFT       (0x0000u)
#define CSL_VENC_HSPLS_HSPLS_RESETVAL    (0x0000u)

#define CSL_VENC_HSPLS_RESETVAL          (0x0000u)

/* VSPLS */

#define CSL_VENC_VSPLS_VSPLS_MASK        (0x1FFFu)
#define CSL_VENC_VSPLS_VSPLS_SHIFT       (0x0000u)
#define CSL_VENC_VSPLS_VSPLS_RESETVAL    (0x0000u)

#define CSL_VENC_VSPLS_RESETVAL          (0x0000u)

/* HINT */

#define CSL_VENC_HINT_HINT_MASK          (0x1FFFu)
#define CSL_VENC_HINT_HINT_SHIFT         (0x0000u)
#define CSL_VENC_HINT_HINT_RESETVAL      (0x0000u)

#define CSL_VENC_HINT_RESETVAL           (0x0000u)

/* HSTART */

#define CSL_VENC_HSTART_HSTART_MASK      (0x1FFFu)
#define CSL_VENC_HSTART_HSTART_SHIFT     (0x0000u)
#define CSL_VENC_HSTART_HSTART_RESETVAL  (0x0000u)

#define CSL_VENC_HSTART_RESETVAL         (0x0000u)

/* HVALID */

#define CSL_VENC_HVALID_HVALID_MASK      (0x1FFFu)
#define CSL_VENC_HVALID_HVALID_SHIFT     (0x0000u)
#define CSL_VENC_HVALID_HVALID_RESETVAL  (0x0000u)

#define CSL_VENC_HVALID_RESETVAL         (0x0000u)

/* VINT */

#define CSL_VENC_VINT_VINT_MASK          (0x1FFFu)
#define CSL_VENC_VINT_VINT_SHIFT         (0x0000u)
#define CSL_VENC_VINT_VINT_RESETVAL      (0x0000u)

#define CSL_VENC_VINT_RESETVAL           (0x0000u)

/* VSTART */

#define CSL_VENC_VSTART_VSTART_MASK      (0x1FFFu)
#define CSL_VENC_VSTART_VSTART_SHIFT     (0x0000u)
#define CSL_VENC_VSTART_VSTART_RESETVAL  (0x0000u)

#define CSL_VENC_VSTART_RESETVAL         (0x0000u)

/* VVALID */

#define CSL_VENC_VVALID_VVALID_MASK      (0x1FFFu)
#define CSL_VENC_VVALID_VVALID_SHIFT     (0x0000u)
#define CSL_VENC_VVALID_VVALID_RESETVAL  (0x0000u)

#define CSL_VENC_VVALID_RESETVAL         (0x0000u)

/* HSDLY */

#define CSL_VENC_HSDLY_HSDLY_MASK        (0x1FFFu)
#define CSL_VENC_HSDLY_HSDLY_SHIFT       (0x0000u)
#define CSL_VENC_HSDLY_HSDLY_RESETVAL    (0x0000u)

#define CSL_VENC_HSDLY_RESETVAL          (0x0000u)

/* VSDLY */

#define CSL_VENC_VSDLY_VSDLY_MASK        (0x1FFFu)
#define CSL_VENC_VSDLY_VSDLY_SHIFT       (0x0000u)
#define CSL_VENC_VSDLY_VSDLY_RESETVAL    (0x0000u)

#define CSL_VENC_VSDLY_RESETVAL          (0x0000u)

/* YCCCTL */

#define CSL_VENC_YCCCTL_CHM_MASK         (0x0010u)
#define CSL_VENC_YCCCTL_CHM_SHIFT        (0x0004u)
#define CSL_VENC_YCCCTL_CHM_RESETVAL     (0x0000u)

/*----CHM Tokens----*/
#define CSL_VENC_YCCCTL_CHM_IMMEDIATE    (0x0000u)
#define CSL_VENC_YCCCTL_CHM_LATCH        (0x0001u)

#define CSL_VENC_YCCCTL_YCP_MASK         (0x000Cu)
#define CSL_VENC_YCCCTL_YCP_SHIFT        (0x0002u)
#define CSL_VENC_YCCCTL_YCP_RESETVAL     (0x0000u)

/*----YCP Tokens----*/
#define CSL_VENC_YCCCTL_YCP_YCC16_CBCR   (0x0000u)
#define CSL_VENC_YCCCTL_YCP_YCC16_CRCB   (0x0001u)
#define CSL_VENC_YCCCTL_YCP_YCC8_CB_Y_CR_Y (0x0000u)
#define CSL_VENC_YCCCTL_YCP_YCC8_Y_CR_Y_CB (0x0001u)
#define CSL_VENC_YCCCTL_YCP_YCC8_CR_Y_CB_Y (0x0002u)
#define CSL_VENC_YCCCTL_YCP_YCC8_Y_CB_Y_CR (0x0003u)

#define CSL_VENC_YCCCTL_R656_MASK        (0x0001u)
#define CSL_VENC_YCCCTL_R656_SHIFT       (0x0000u)
#define CSL_VENC_YCCCTL_R656_RESETVAL    (0x0000u)

/*----R656 Tokens----*/
#define CSL_VENC_YCCCTL_R656_NORMAL      (0x0000u)
#define CSL_VENC_YCCCTL_R656_REC656      (0x0001u)

#define CSL_VENC_YCCCTL_RESETVAL         (0x0000u)

/* RGBCTL */

#define CSL_VENC_RGBCTL_RGBLAT_MASK      (0x8000u)
#define CSL_VENC_RGBCTL_RGBLAT_SHIFT     (0x000Fu)
#define CSL_VENC_RGBCTL_RGBLAT_RESETVAL  (0x0000u)

/*----RGBLAT Tokens----*/
#define CSL_VENC_RGBCTL_RGBLAT_NORMALOUTPUT (0x0000u)
#define CSL_VENC_RGBCTL_RGBLAT_LATCHMODE (0x0001u)

#define CSL_VENC_RGBCTL_IRSWP_MASK       (0x2000u)
#define CSL_VENC_RGBCTL_IRSWP_SHIFT      (0x000Du)
#define CSL_VENC_RGBCTL_IRSWP_RESETVAL   (0x0000u)

/*----IRSWP Tokens----*/
#define CSL_VENC_RGBCTL_IRSWP_NORMAL     (0x0000u)
#define CSL_VENC_RGBCTL_IRSWP_IR_SWAP    (0x0001u)

#define CSL_VENC_RGBCTL_IR9_MASK         (0x1000u)
#define CSL_VENC_RGBCTL_IR9_SHIFT        (0x000Cu)
#define CSL_VENC_RGBCTL_IR9_RESETVAL     (0x0000u)

/*----IR9 Tokens----*/
#define CSL_VENC_RGBCTL_IR9_IR_8BIT      (0x0000u)
#define CSL_VENC_RGBCTL_IR9_IR_9BIT      (0x0001u)

#define CSL_VENC_RGBCTL_IRONM_MASK       (0x0800u)
#define CSL_VENC_RGBCTL_IRONM_SHIFT      (0x000Bu)
#define CSL_VENC_RGBCTL_IRONM_RESETVAL   (0x0000u)

/*----IRONM Tokens----*/
#define CSL_VENC_RGBCTL_IRONM_NORMAL     (0x0000u)
#define CSL_VENC_RGBCTL_IRONM_IRONMAN    (0x0001u)

#define CSL_VENC_RGBCTL_DFLTR_MASK       (0x0400u)
#define CSL_VENC_RGBCTL_DFLTR_SHIFT      (0x000Au)
#define CSL_VENC_RGBCTL_DFLTR_RESETVAL   (0x0000u)

/*----DFLTR Tokens----*/
#define CSL_VENC_RGBCTL_DFLTR_ENCCLOCK_2 (0x0000u)
#define CSL_VENC_RGBCTL_DFLTR_ENCCLOCK   (0x0001u)

#define CSL_VENC_RGBCTL_DFLTS_MASK       (0x0300u)
#define CSL_VENC_RGBCTL_DFLTS_SHIFT      (0x0008u)
#define CSL_VENC_RGBCTL_DFLTS_RESETVAL   (0x0000u)

/*----DFLTS Tokens----*/
#define CSL_VENC_RGBCTL_DFLTS_NOFILTER   (0x0000u)
#define CSL_VENC_RGBCTL_DFLTS_3TAP       (0x0001u)
#define CSL_VENC_RGBCTL_DFLTS_7TAP       (0x0002u)
#define CSL_VENC_RGBCTL_DFLTS_RESERVED   (0x0003u)

#define CSL_VENC_RGBCTL_RGBEF_MASK       (0x0070u)
#define CSL_VENC_RGBCTL_RGBEF_SHIFT      (0x0004u)
#define CSL_VENC_RGBCTL_RGBEF_RESETVAL   (0x0000u)

/*----RGBEF Tokens----*/
#define CSL_VENC_RGBCTL_RGBEF_R0_G1_B2   (0x0000u)
#define CSL_VENC_RGBCTL_RGBEF_R0_B1_G2   (0x0001u)
#define CSL_VENC_RGBCTL_RGBEF_G0_R1_B2   (0x0002u)
#define CSL_VENC_RGBCTL_RGBEF_G0_B1_R2   (0x0003u)
#define CSL_VENC_RGBCTL_RGBEF_B0_R1_G2   (0x0004u)
#define CSL_VENC_RGBCTL_RGBEF_B0_G1_R2   (0x0005u)
#define CSL_VENC_RGBCTL_RGBEF_RESERVED1  (0x0006u)
#define CSL_VENC_RGBCTL_RGBEF_RESERVED2  (0x0007u)

#define CSL_VENC_RGBCTL_RGBOF_MASK       (0x0007u)
#define CSL_VENC_RGBCTL_RGBOF_SHIFT      (0x0000u)
#define CSL_VENC_RGBCTL_RGBOF_RESETVAL   (0x0000u)

/*----RGBOF Tokens----*/
#define CSL_VENC_RGBCTL_RGBOF_R0_G1_B2   (0x0000u)
#define CSL_VENC_RGBCTL_RGBOF_R0_B1_G2   (0x0001u)
#define CSL_VENC_RGBCTL_RGBOF_G0_R1_B2   (0x0002u)
#define CSL_VENC_RGBCTL_RGBOF_G0_B1_R2   (0x0003u)
#define CSL_VENC_RGBCTL_RGBOF_B0_R1_G2   (0x0004u)
#define CSL_VENC_RGBCTL_RGBOF_B0_G1_R2   (0x0005u)
#define CSL_VENC_RGBCTL_RGBOF_RESERVED1  (0x0006u)
#define CSL_VENC_RGBCTL_RGBOF_RESERVED2  (0x0007u)

#define CSL_VENC_RGBCTL_RESETVAL         (0x0000u)

/* RGBCLP */

#define CSL_VENC_RGBCLP_UCLIP_MASK       (0xFF00u)
#define CSL_VENC_RGBCLP_UCLIP_SHIFT      (0x0008u)
#define CSL_VENC_RGBCLP_UCLIP_RESETVAL   (0x00FFu)

#define CSL_VENC_RGBCLP_OFST_MASK        (0x00FFu)
#define CSL_VENC_RGBCLP_OFST_SHIFT       (0x0000u)
#define CSL_VENC_RGBCLP_OFST_RESETVAL    (0x0000u)

#define CSL_VENC_RGBCLP_RESETVAL         (0xFF00u)

/* LINECTL */

#define CSL_VENC_LINECTL_EXIDP_MASK      (0x8000u)
#define CSL_VENC_LINECTL_EXIDP_SHIFT     (0x000Fu)
#define CSL_VENC_LINECTL_EXIDP_RESETVAL  (0x0000u)

/*----EXIDP Tokens----*/
#define CSL_VENC_LINECTL_EXIDP_NON_INVERSE (0x0000u)
#define CSL_VENC_LINECTL_EXIDP_INVERSE   (0x0001u)

#define CSL_VENC_LINECTL_EXIDE_MASK      (0x4000u)
#define CSL_VENC_LINECTL_EXIDE_SHIFT     (0x000Eu)
#define CSL_VENC_LINECTL_EXIDE_RESETVAL  (0x0000u)

/*----EXIDE Tokens----*/
#define CSL_VENC_LINECTL_EXIDE_INTERNALIDLINE (0x0000u)
#define CSL_VENC_LINECTL_EXIDE_EXTERNALIDLINE (0x0001u)

#define CSL_VENC_LINECTL_VSTF_MASK       (0x0800u)
#define CSL_VENC_LINECTL_VSTF_SHIFT      (0x000Bu)
#define CSL_VENC_LINECTL_VSTF_RESETVAL   (0x0000u)

/*----VSTF Tokens----*/
#define CSL_VENC_LINECTL_VSTF_NORMAL     (0x0000u)
#define CSL_VENC_LINECTL_VSTF_FIELD      (0x0001u)

#define CSL_VENC_LINECTL_VCLID_MASK      (0x0700u)
#define CSL_VENC_LINECTL_VCLID_SHIFT     (0x0008u)
#define CSL_VENC_LINECTL_VCLID_RESETVAL  (0x0000u)

#define CSL_VENC_LINECTL_VCLRD_MASK      (0x0080u)
#define CSL_VENC_LINECTL_VCLRD_SHIFT     (0x0007u)
#define CSL_VENC_LINECTL_VCLRD_RESETVAL  (0x0000u)

/*----VCLRD Tokens----*/
#define CSL_VENC_LINECTL_VCLRD_RESET_0   (0x0000u)
#define CSL_VENC_LINECTL_VCLRD_RESET_RAND (0x0001u)

#define CSL_VENC_LINECTL_VCL56_MASK      (0x0040u)
#define CSL_VENC_LINECTL_VCL56_SHIFT     (0x0006u)
#define CSL_VENC_LINECTL_VCL56_RESETVAL  (0x0000u)

/*----VCL56 Tokens----*/
#define CSL_VENC_LINECTL_VCL56_NOCULLING (0x0000u)
#define CSL_VENC_LINECTL_VCL56_5_6CULLING (0x0001u)

#define CSL_VENC_LINECTL_HLDF_MASK       (0x0020u)
#define CSL_VENC_LINECTL_HLDF_SHIFT      (0x0005u)
#define CSL_VENC_LINECTL_HLDF_RESETVAL   (0x0000u)

/*----HLDF Tokens----*/
#define CSL_VENC_LINECTL_HLDF_NORMAL     (0x0000u)
#define CSL_VENC_LINECTL_HLDF_OUTPUTHOLD (0x0001u)

#define CSL_VENC_LINECTL_HLDL_MASK       (0x0010u)
#define CSL_VENC_LINECTL_HLDL_SHIFT      (0x0004u)
#define CSL_VENC_LINECTL_HLDL_RESETVAL   (0x0000u)

/*----HLDL Tokens----*/
#define CSL_VENC_LINECTL_HLDL_NORMAL     (0x0000u)
#define CSL_VENC_LINECTL_HLDL_OUTPUTHOLD (0x0001u)

#define CSL_VENC_LINECTL_LINID_MASK      (0x0008u)
#define CSL_VENC_LINECTL_LINID_SHIFT     (0x0003u)
#define CSL_VENC_LINECTL_LINID_RESETVAL  (0x0000u)

/*----LINID Tokens----*/
#define CSL_VENC_LINECTL_LINID_LINEID_0  (0x0000u)
#define CSL_VENC_LINECTL_LINID_LINEID_1  (0x0001u)

#define CSL_VENC_LINECTL_DCKCLP_MASK     (0x0004u)
#define CSL_VENC_LINECTL_DCKCLP_SHIFT    (0x0002u)
#define CSL_VENC_LINECTL_DCKCLP_RESETVAL (0x0000u)

/*----DCKCLP Tokens----*/
#define CSL_VENC_LINECTL_DCKCLP_OFF      (0x0000u)
#define CSL_VENC_LINECTL_DCKCLP_ON       (0x0001u)

#define CSL_VENC_LINECTL_DCKCLI_MASK     (0x0002u)
#define CSL_VENC_LINECTL_DCKCLI_SHIFT    (0x0001u)
#define CSL_VENC_LINECTL_DCKCLI_RESETVAL (0x0000u)

/*----DCKCLI Tokens----*/
#define CSL_VENC_LINECTL_DCKCLI_OFF      (0x0000u)
#define CSL_VENC_LINECTL_DCKCLI_ON       (0x0001u)

#define CSL_VENC_LINECTL_RGBCL_MASK      (0x0001u)
#define CSL_VENC_LINECTL_RGBCL_SHIFT     (0x0000u)
#define CSL_VENC_LINECTL_RGBCL_RESETVAL  (0x0000u)

/*----RGBCL Tokens----*/
#define CSL_VENC_LINECTL_RGBCL_OFF       (0x0000u)
#define CSL_VENC_LINECTL_RGBCL_ON        (0x0001u)

#define CSL_VENC_LINECTL_RESETVAL        (0x0000u)

/* CULLLINE */

#define CSL_VENC_CULLLINE_CLOF_MASK      (0xF000u)
#define CSL_VENC_CULLLINE_CLOF_SHIFT     (0x000Cu)
#define CSL_VENC_CULLLINE_CLOF_RESETVAL  (0x0000u)

#define CSL_VENC_CULLLINE_CLEF_MASK      (0x0F00u)
#define CSL_VENC_CULLLINE_CLEF_SHIFT     (0x0008u)
#define CSL_VENC_CULLLINE_CLEF_RESETVAL  (0x0000u)

#define CSL_VENC_CULLLINE_CULI_MASK      (0x000Fu)
#define CSL_VENC_CULLLINE_CULI_SHIFT     (0x0000u)
#define CSL_VENC_CULLLINE_CULI_RESETVAL  (0x0000u)

#define CSL_VENC_CULLLINE_RESETVAL       (0x0000u)

/* LCDOUT */

#define CSL_VENC_LCDOUT_OES_MASK         (0x0100u)
#define CSL_VENC_LCDOUT_OES_SHIFT        (0x0008u)
#define CSL_VENC_LCDOUT_OES_RESETVAL     (0x0000u)

/*----OES Tokens----*/
#define CSL_VENC_LCDOUT_OES_LCD_OE       (0x0000u)
#define CSL_VENC_LCDOUT_OES_BRIGHT       (0x0001u)

#define CSL_VENC_LCDOUT_FIDP_MASK        (0x0080u)
#define CSL_VENC_LCDOUT_FIDP_SHIFT       (0x0007u)
#define CSL_VENC_LCDOUT_FIDP_RESETVAL    (0x0000u)

/*----FIDP Tokens----*/
#define CSL_VENC_LCDOUT_FIDP_NON_INVERSE (0x0000u)
#define CSL_VENC_LCDOUT_FIDP_INVERSE     (0x0001u)

#define CSL_VENC_LCDOUT_PWMP_MASK        (0x0040u)
#define CSL_VENC_LCDOUT_PWMP_SHIFT       (0x0006u)
#define CSL_VENC_LCDOUT_PWMP_RESETVAL    (0x0000u)

/*----PWMP Tokens----*/
#define CSL_VENC_LCDOUT_PWMP_ACTIVEH     (0x0000u)
#define CSL_VENC_LCDOUT_PWMP_ACTIVEL     (0x0001u)

#define CSL_VENC_LCDOUT_PWME_MASK        (0x0020u)
#define CSL_VENC_LCDOUT_PWME_SHIFT       (0x0005u)
#define CSL_VENC_LCDOUT_PWME_RESETVAL    (0x0000u)

/*----PWME Tokens----*/
#define CSL_VENC_LCDOUT_PWME_OFF         (0x0000u)
#define CSL_VENC_LCDOUT_PWME_ON          (0x0001u)

#define CSL_VENC_LCDOUT_ACE_MASK         (0x0010u)
#define CSL_VENC_LCDOUT_ACE_SHIFT        (0x0004u)
#define CSL_VENC_LCDOUT_ACE_RESETVAL     (0x0000u)

/*----ACE Tokens----*/
#define CSL_VENC_LCDOUT_ACE_OFF          (0x0000u)
#define CSL_VENC_LCDOUT_ACE_ON           (0x0001u)

#define CSL_VENC_LCDOUT_BRP_MASK         (0x0008u)
#define CSL_VENC_LCDOUT_BRP_SHIFT        (0x0003u)
#define CSL_VENC_LCDOUT_BRP_RESETVAL     (0x0000u)

/*----BRP Tokens----*/
#define CSL_VENC_LCDOUT_BRP_ACTIVEH      (0x0000u)
#define CSL_VENC_LCDOUT_BRP_ACTIVEL      (0x0001u)

#define CSL_VENC_LCDOUT_BRE_MASK         (0x0004u)
#define CSL_VENC_LCDOUT_BRE_SHIFT        (0x0002u)
#define CSL_VENC_LCDOUT_BRE_RESETVAL     (0x0000u)

/*----BRE Tokens----*/
#define CSL_VENC_LCDOUT_BRE_OFF          (0x0000u)
#define CSL_VENC_LCDOUT_BRE_ON           (0x0001u)

#define CSL_VENC_LCDOUT_OEP_MASK         (0x0002u)
#define CSL_VENC_LCDOUT_OEP_SHIFT        (0x0001u)
#define CSL_VENC_LCDOUT_OEP_RESETVAL     (0x0000u)

/*----OEP Tokens----*/
#define CSL_VENC_LCDOUT_OEP_ACTIVEH      (0x0000u)
#define CSL_VENC_LCDOUT_OEP_ACTIVEL      (0x0001u)

#define CSL_VENC_LCDOUT_OEE_MASK         (0x0001u)
#define CSL_VENC_LCDOUT_OEE_SHIFT        (0x0000u)
#define CSL_VENC_LCDOUT_OEE_RESETVAL     (0x0000u)

/*----OEE Tokens----*/
#define CSL_VENC_LCDOUT_OEE_OFF          (0x0000u)
#define CSL_VENC_LCDOUT_OEE_ON           (0x0001u)

#define CSL_VENC_LCDOUT_RESETVAL         (0x0000u)

/* BRTS */

#define CSL_VENC_BRTS_BRTS_MASK          (0x1FFFu)
#define CSL_VENC_BRTS_BRTS_SHIFT         (0x0000u)
#define CSL_VENC_BRTS_BRTS_RESETVAL      (0x0000u)

#define CSL_VENC_BRTS_RESETVAL           (0x0000u)

/* BRTW */

#define CSL_VENC_BRTW_BRTW_MASK          (0x1FFFu)
#define CSL_VENC_BRTW_BRTW_SHIFT         (0x0000u)
#define CSL_VENC_BRTW_BRTW_RESETVAL      (0x0000u)

#define CSL_VENC_BRTW_RESETVAL           (0x0000u)

/* ACCTL */

#define CSL_VENC_ACCTL_ACTF_MASK         (0xE000u)
#define CSL_VENC_ACCTL_ACTF_SHIFT        (0x000Du)
#define CSL_VENC_ACCTL_ACTF_RESETVAL     (0x0000u)

#define CSL_VENC_ACCTL_ACTH_MASK         (0x1FFFu)
#define CSL_VENC_ACCTL_ACTH_SHIFT        (0x0000u)
#define CSL_VENC_ACCTL_ACTH_RESETVAL     (0x0000u)

#define CSL_VENC_ACCTL_RESETVAL          (0x0000u)

/* PWMP */

#define CSL_VENC_PWMP_PWMP_MASK          (0x1FFFu)
#define CSL_VENC_PWMP_PWMP_SHIFT         (0x0000u)
#define CSL_VENC_PWMP_PWMP_RESETVAL      (0x0000u)

#define CSL_VENC_PWMP_RESETVAL           (0x0000u)

/* PWMW */

#define CSL_VENC_PWMW_PWMW_MASK          (0x1FFFu)
#define CSL_VENC_PWMW_PWMW_SHIFT         (0x0000u)
#define CSL_VENC_PWMW_PWMW_RESETVAL      (0x0000u)

#define CSL_VENC_PWMW_RESETVAL           (0x0000u)

/* DCLKCTL */

#define CSL_VENC_DCLKCTL_DCKIM_MASK      (0x8000u)
#define CSL_VENC_DCLKCTL_DCKIM_SHIFT     (0x000Fu)
#define CSL_VENC_DCLKCTL_DCKIM_RESETVAL  (0x0000u)

/*----DCKIM Tokens----*/
#define CSL_VENC_DCLKCTL_DCKIM_OFF       (0x0000u)
#define CSL_VENC_DCLKCTL_DCKIM_ON        (0x0001u)

#define CSL_VENC_DCLKCTL_DOFST_MASK      (0x3000u)
#define CSL_VENC_DCLKCTL_DOFST_SHIFT     (0x000Cu)
#define CSL_VENC_DCLKCTL_DOFST_RESETVAL  (0x0000u)

/*----DOFST Tokens----*/
#define CSL_VENC_DCLKCTL_DOFST_ZERO      (0x0000u)
#define CSL_VENC_DCLKCTL_DOFST_MINUS_HALF (0x0001u)
#define CSL_VENC_DCLKCTL_DOFST_PLUS_HALF (0x0002u)
#define CSL_VENC_DCLKCTL_DOFST_PLUS_ONE  (0x0003u)

#define CSL_VENC_DCLKCTL_DCKEC_MASK      (0x0800u)
#define CSL_VENC_DCLKCTL_DCKEC_SHIFT     (0x000Bu)
#define CSL_VENC_DCLKCTL_DCKEC_RESETVAL  (0x0000u)

/*----DCKEC Tokens----*/
#define CSL_VENC_DCLKCTL_DCKEC_LEVEL     (0x0000u)
#define CSL_VENC_DCLKCTL_DCKEC_ENABLE    (0x0001u)

#define CSL_VENC_DCLKCTL_DCKME_MASK      (0x0400u)
#define CSL_VENC_DCLKCTL_DCKME_SHIFT     (0x000Au)
#define CSL_VENC_DCLKCTL_DCKME_RESETVAL  (0x0000u)

/*----DCKME Tokens----*/
#define CSL_VENC_DCLKCTL_DCKME_OFF       (0x0000u)
#define CSL_VENC_DCLKCTL_DCKME_ON        (0x0001u)

#define CSL_VENC_DCLKCTL_DCKOH_MASK      (0x0200u)
#define CSL_VENC_DCLKCTL_DCKOH_SHIFT     (0x0009u)
#define CSL_VENC_DCLKCTL_DCKOH_RESETVAL  (0x0000u)

/*----DCKOH Tokens----*/
#define CSL_VENC_DCLKCTL_DCKOH_DIV_BY_1  (0x0000u)
#define CSL_VENC_DCLKCTL_DCKOH_DIV_BY_2  (0x0001u)

#define CSL_VENC_DCLKCTL_DCKIH_MASK      (0x0100u)
#define CSL_VENC_DCLKCTL_DCKIH_SHIFT     (0x0008u)
#define CSL_VENC_DCLKCTL_DCKIH_RESETVAL  (0x0000u)

/*----DCKIH Tokens----*/
#define CSL_VENC_DCLKCTL_DCKIH_DIV_BY_1  (0x0000u)
#define CSL_VENC_DCLKCTL_DCKIH_DIV_BY_2  (0x0001u)

#define CSL_VENC_DCLKCTL_DCKPW_MASK      (0x003Fu)
#define CSL_VENC_DCLKCTL_DCKPW_SHIFT     (0x0000u)
#define CSL_VENC_DCLKCTL_DCKPW_RESETVAL  (0x0000u)

#define CSL_VENC_DCLKCTL_RESETVAL        (0x0000u)

/* DCLKPTN0 */

#define CSL_VENC_DCLKPTN0_DCPTN0_MASK    (0xFFFFu)
#define CSL_VENC_DCLKPTN0_DCPTN0_SHIFT   (0x0000u)
#define CSL_VENC_DCLKPTN0_DCPTN0_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN0_RESETVAL       (0x0000u)

/* DCLKPTN1 */

#define CSL_VENC_DCLKPTN1_DCPTN1_MASK    (0xFFFFu)
#define CSL_VENC_DCLKPTN1_DCPTN1_SHIFT   (0x0000u)
#define CSL_VENC_DCLKPTN1_DCPTN1_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN1_RESETVAL       (0x0000u)

/* DCLKPTN2 */

#define CSL_VENC_DCLKPTN2_DCPTN2_MASK    (0xFFFFu)
#define CSL_VENC_DCLKPTN2_DCPTN2_SHIFT   (0x0000u)
#define CSL_VENC_DCLKPTN2_DCPTN2_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN2_RESETVAL       (0x0000u)

/* DCLKPTN3 */

#define CSL_VENC_DCLKPTN3_DCPTN3_MASK    (0xFFFFu)
#define CSL_VENC_DCLKPTN3_DCPTN3_SHIFT   (0x0000u)
#define CSL_VENC_DCLKPTN3_DCPTN3_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN3_RESETVAL       (0x0000u)

/* DCLKPTN0A */

#define CSL_VENC_DCLKPTN0A_DCPTN0A_MASK  (0xFFFFu)
#define CSL_VENC_DCLKPTN0A_DCPTN0A_SHIFT (0x0000u)
#define CSL_VENC_DCLKPTN0A_DCPTN0A_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN0A_RESETVAL      (0x0000u)

/* DCLKPTN1A */

#define CSL_VENC_DCLKPTN1A_DCPTN1A_MASK  (0xFFFFu)
#define CSL_VENC_DCLKPTN1A_DCPTN1A_SHIFT (0x0000u)
#define CSL_VENC_DCLKPTN1A_DCPTN1A_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN1A_RESETVAL      (0x0000u)

/* DCLKPTN2A */

#define CSL_VENC_DCLKPTN2A_DCPTN2A_MASK  (0xFFFFu)
#define CSL_VENC_DCLKPTN2A_DCPTN2A_SHIFT (0x0000u)
#define CSL_VENC_DCLKPTN2A_DCPTN2A_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN2A_RESETVAL      (0x0000u)

/* DCLKPTN3A */

#define CSL_VENC_DCLKPTN3A_DCPTN3A_MASK  (0xFFFFu)
#define CSL_VENC_DCLKPTN3A_DCPTN3A_SHIFT (0x0000u)
#define CSL_VENC_DCLKPTN3A_DCPTN3A_RESETVAL (0x0000u)

#define CSL_VENC_DCLKPTN3A_RESETVAL      (0x0000u)

/* DCLKHS */

#define CSL_VENC_DCLKHS_DCHS_MASK        (0x1FFFu)
#define CSL_VENC_DCLKHS_DCHS_SHIFT       (0x0000u)
#define CSL_VENC_DCLKHS_DCHS_RESETVAL    (0x0000u)

#define CSL_VENC_DCLKHS_RESETVAL         (0x0000u)

/* DCLKHSA */

#define CSL_VENC_DCLKHSA_DCHS_MASK       (0x1FFFu)
#define CSL_VENC_DCLKHSA_DCHS_SHIFT      (0x0000u)
#define CSL_VENC_DCLKHSA_DCHS_RESETVAL   (0x0000u)

#define CSL_VENC_DCLKHSA_RESETVAL        (0x0000u)

/* DCLKHR */

#define CSL_VENC_DCLKHR_DCHR_MASK        (0x1FFFu)
#define CSL_VENC_DCLKHR_DCHR_SHIFT       (0x0000u)
#define CSL_VENC_DCLKHR_DCHR_RESETVAL    (0x0000u)

#define CSL_VENC_DCLKHR_RESETVAL         (0x0000u)

/* DCLKVS */

#define CSL_VENC_DCLKVS_DCVS_MASK        (0x1FFFu)
#define CSL_VENC_DCLKVS_DCVS_SHIFT       (0x0000u)
#define CSL_VENC_DCLKVS_DCVS_RESETVAL    (0x0000u)

#define CSL_VENC_DCLKVS_RESETVAL         (0x0000u)

/* DCLKVR */

#define CSL_VENC_DCLKVR_DCVR_MASK        (0x1FFFu)
#define CSL_VENC_DCLKVR_DCVR_SHIFT       (0x0000u)
#define CSL_VENC_DCLKVR_DCVR_RESETVAL    (0x0000u)

#define CSL_VENC_DCLKVR_RESETVAL         (0x0000u)

/* CAPCTL */

#define CSL_VENC_CAPCTL_CADF_MASK        (0x7F00u)
#define CSL_VENC_CAPCTL_CADF_SHIFT       (0x0008u)
#define CSL_VENC_CAPCTL_CADF_RESETVAL    (0x0000u)

#define CSL_VENC_CAPCTL_CAPF_MASK        (0x0003u)
#define CSL_VENC_CAPCTL_CAPF_SHIFT       (0x0000u)
#define CSL_VENC_CAPCTL_CAPF_RESETVAL    (0x0000u)

/*----CAPF Tokens----*/
#define CSL_VENC_CAPCTL_CAPF_NODATA      (0x0000u)
#define CSL_VENC_CAPCTL_CAPF_ODD         (0x0001u)
#define CSL_VENC_CAPCTL_CAPF_EVEN        (0x0002u)
#define CSL_VENC_CAPCTL_CAPF_DUAL        (0x0003u)

#define CSL_VENC_CAPCTL_RESETVAL         (0x0000u)

/* CAPDO */

#define CSL_VENC_CAPDO_CADO0_MASK        (0x7F00u)
#define CSL_VENC_CAPDO_CADO0_SHIFT       (0x0008u)
#define CSL_VENC_CAPDO_CADO0_RESETVAL    (0x0000u)

#define CSL_VENC_CAPDO_CADO1_MASK        (0x007Fu)
#define CSL_VENC_CAPDO_CADO1_SHIFT       (0x0000u)
#define CSL_VENC_CAPDO_CADO1_RESETVAL    (0x0000u)

#define CSL_VENC_CAPDO_RESETVAL          (0x0000u)

/* CAPDE */

#define CSL_VENC_CAPDE_CADE0_MASK        (0x7F00u)
#define CSL_VENC_CAPDE_CADE0_SHIFT       (0x0008u)
#define CSL_VENC_CAPDE_CADE0_RESETVAL    (0x0000u)

#define CSL_VENC_CAPDE_CADE1_MASK        (0x007Fu)
#define CSL_VENC_CAPDE_CADE1_SHIFT       (0x0000u)
#define CSL_VENC_CAPDE_CADE1_RESETVAL    (0x0000u)

#define CSL_VENC_CAPDE_RESETVAL          (0x0000u)

/* ATR0 */

#define CSL_VENC_ATR0_ATR0_MASK          (0x00FFu)
#define CSL_VENC_ATR0_ATR0_SHIFT         (0x0000u)
#define CSL_VENC_ATR0_ATR0_RESETVAL      (0x0000u)

#define CSL_VENC_ATR0_RESETVAL           (0x0000u)

/* ATR1 */

#define CSL_VENC_ATR1_ATR1_MASK          (0x00FFu)
#define CSL_VENC_ATR1_ATR1_SHIFT         (0x0000u)
#define CSL_VENC_ATR1_ATR1_RESETVAL      (0x0000u)

#define CSL_VENC_ATR1_RESETVAL           (0x0000u)

/* ATR2 */

#define CSL_VENC_ATR2_ATR2_MASK          (0x00FFu)
#define CSL_VENC_ATR2_ATR2_SHIFT         (0x0000u)
#define CSL_VENC_ATR2_ATR2_RESETVAL      (0x0000u)

#define CSL_VENC_ATR2_RESETVAL           (0x0000u)

/* RSV0 */

#define CSL_VENC_RSV0_RESETVAL           (0x0000u)

/* VSTAT */

#define CSL_VENC_VSTAT_CAEST_MASK        (0x0200u)
#define CSL_VENC_VSTAT_CAEST_SHIFT       (0x0009u)
#define CSL_VENC_VSTAT_CAEST_RESETVAL    (0x0000u)

/*----CAEST Tokens----*/
#define CSL_VENC_VSTAT_CAEST_READY       (0x0000u)
#define CSL_VENC_VSTAT_CAEST_DATA_BEING_INPUT (0x0001u)

#define CSL_VENC_VSTAT_CAOST_MASK        (0x0100u)
#define CSL_VENC_VSTAT_CAOST_SHIFT       (0x0008u)
#define CSL_VENC_VSTAT_CAOST_RESETVAL    (0x0000u)

/*----CAOST Tokens----*/
#define CSL_VENC_VSTAT_CAOST_READY       (0x0000u)
#define CSL_VENC_VSTAT_CAOST_DATA_BEING_INPUT (0x0001u)

#define CSL_VENC_VSTAT_FIDST_MASK        (0x0010u)
#define CSL_VENC_VSTAT_FIDST_SHIFT       (0x0004u)
#define CSL_VENC_VSTAT_FIDST_RESETVAL    (0x0000u)

#define CSL_VENC_VSTAT_RESERVED_MASK     (0x0008u)
#define CSL_VENC_VSTAT_RESERVED_SHIFT    (0x0003u)
#define CSL_VENC_VSTAT_RESERVED_RESETVAL (0x0000u)

#define CSL_VENC_VSTAT_RESETVAL          (0x0000u)

/* RAMADR */

#define CSL_VENC_RAMADR_RAMADR_MASK      (0x003Fu)
#define CSL_VENC_RAMADR_RAMADR_SHIFT     (0x0000u)
#define CSL_VENC_RAMADR_RAMADR_RESETVAL  (0x0000u)

#define CSL_VENC_RAMADR_RESETVAL         (0x0000u)

/* RAMPORT */

#define CSL_VENC_RAMPORT_RAMPORT_MASK    (0xFFFFu)
#define CSL_VENC_RAMPORT_RAMPORT_SHIFT   (0x0000u)
#define CSL_VENC_RAMPORT_RAMPORT_RESETVAL (0x0000u)

#define CSL_VENC_RAMPORT_RESETVAL        (0x0000u)

/* DACTST */

#define CSL_VENC_DACTST_DAPD0_MASK       (0x1000u)
#define CSL_VENC_DACTST_DAPD0_SHIFT      (0x000Cu)
#define CSL_VENC_DACTST_DAPD0_RESETVAL   (0x0001u)

/*----DAPD0 Tokens----*/
#define CSL_VENC_DACTST_DAPD0_NORMAL     (0x0000u)
#define CSL_VENC_DACTST_DAPD0_POWERDOWN  (0x0001u)

#define CSL_VENC_DACTST_DACDC_MASK       (0x0400u)
#define CSL_VENC_DACTST_DACDC_SHIFT      (0x000Au)
#define CSL_VENC_DACTST_DACDC_RESETVAL   (0x0000u)

/*----DACDC Tokens----*/
#define CSL_VENC_DACTST_DACDC_NORMAL     (0x0000u)
#define CSL_VENC_DACTST_DACDC_DCOUTPUTMODE (0x0001u)

#define CSL_VENC_DACTST_DALVL_MASK       (0x03FFu)
#define CSL_VENC_DACTST_DALVL_SHIFT      (0x0000u)
#define CSL_VENC_DACTST_DALVL_RESETVAL   (0x0000u)

#define CSL_VENC_DACTST_RESETVAL         (0x1000u)

/* YCOLVL */

#define CSL_VENC_YCOLVL_YLVL_MASK        (0xFF00u)
#define CSL_VENC_YCOLVL_YLVL_SHIFT       (0x0008u)
#define CSL_VENC_YCOLVL_YLVL_RESETVAL    (0x0000u)

#define CSL_VENC_YCOLVL_CLVL_MASK        (0x00FFu)
#define CSL_VENC_YCOLVL_CLVL_SHIFT       (0x0000u)
#define CSL_VENC_YCOLVL_CLVL_RESETVAL    (0x0000u)

#define CSL_VENC_YCOLVL_RESETVAL         (0x0000u)

/* SCPROG */

#define CSL_VENC_SCPROG_SCSD_MASK        (0x03FFu)
#define CSL_VENC_SCPROG_SCSD_SHIFT       (0x0000u)
#define CSL_VENC_SCPROG_SCSD_RESETVAL    (0x017Au)

#define CSL_VENC_SCPROG_RESETVAL         (0x017Au)

/* RSV1 */

#define CSL_VENC_RSV1_RESETVAL           (0x0000u)

/* RSV2 */

#define CSL_VENC_RSV2_RESETVAL           (0x0000u)

/* RSV3 */

#define CSL_VENC_RSV3_RESETVAL           (0x0000u)

/* CVBS */

#define CSL_VENC_CVBS_YCDLY_MASK         (0x7000u)
#define CSL_VENC_CVBS_YCDLY_SHIFT        (0x000Cu)
#define CSL_VENC_CVBS_YCDLY_RESETVAL     (0x0000u)

/*----YCDLY Tokens----*/
#define CSL_VENC_CVBS_YCDLY_0            (0x0000u)
#define CSL_VENC_CVBS_YCDLY_PLUS_1       (0x0001u)
#define CSL_VENC_CVBS_YCDLY_PLUS_2       (0x0002u)
#define CSL_VENC_CVBS_YCDLY_PLUS_3       (0x0003u)
#define CSL_VENC_CVBS_YCDLY_MINUS_4      (0x0004u)
#define CSL_VENC_CVBS_YCDLY_MINUS_3      (0x0005u)
#define CSL_VENC_CVBS_YCDLY_MINUS_2      (0x0006u)
#define CSL_VENC_CVBS_YCDLY_MINUS_1      (0x0007u)

#define CSL_VENC_CVBS_CVLVL_MASK         (0x0020u)
#define CSL_VENC_CVBS_CVLVL_SHIFT        (0x0005u)
#define CSL_VENC_CVBS_CVLVL_RESETVAL     (0x0000u)

/*----CVLVL Tokens----*/
#define CSL_VENC_CVBS_CVLVL_286MV_714MV  (0x0000u)
#define CSL_VENC_CVBS_CVLVL_300MV_700MV  (0x0001u)

#define CSL_VENC_CVBS_CSTUP_MASK         (0x0010u)
#define CSL_VENC_CVBS_CSTUP_SHIFT        (0x0004u)
#define CSL_VENC_CVBS_CSTUP_RESETVAL     (0x0000u)

/*----CSTUP Tokens----*/
#define CSL_VENC_CVBS_CSTUP_0_PCT        (0x0000u)
#define CSL_VENC_CVBS_CSTUP_7P5_PCT      (0x0001u)

#define CSL_VENC_CVBS_CBLS_MASK          (0x0008u)
#define CSL_VENC_CVBS_CBLS_SHIFT         (0x0003u)
#define CSL_VENC_CVBS_CBLS_RESETVAL      (0x0000u)

/*----CBLS Tokens----*/
#define CSL_VENC_CVBS_CBLS_ENABLE        (0x0000u)
#define CSL_VENC_CVBS_CBLS_DISABLE       (0x0001u)

#define CSL_VENC_CVBS_CRCUT_MASK         (0x0004u)
#define CSL_VENC_CVBS_CRCUT_SHIFT        (0x0002u)
#define CSL_VENC_CVBS_CRCUT_RESETVAL     (0x0000u)

/*----CRCUT Tokens----*/
#define CSL_VENC_CVBS_CRCUT_1P5MHZ_CUT0FF (0x0000u)
#define CSL_VENC_CVBS_CRCUT_3MHZ_CUTOFF  (0x0001u)

#define CSL_VENC_CVBS_CBBLD_MASK         (0x0002u)
#define CSL_VENC_CVBS_CBBLD_SHIFT        (0x0001u)
#define CSL_VENC_CVBS_CBBLD_RESETVAL     (0x0000u)

/*----CBBLD Tokens----*/
#define CSL_VENC_CVBS_CBBLD_140US        (0x0000u)
#define CSL_VENC_CVBS_CBBLD_300US        (0x0001u)

#define CSL_VENC_CVBS_CSBLD_MASK         (0x0001u)
#define CSL_VENC_CVBS_CSBLD_SHIFT        (0x0000u)
#define CSL_VENC_CVBS_CSBLD_RESETVAL     (0x0000u)

/*----CSBLD Tokens----*/
#define CSL_VENC_CVBS_CSBLD_140US        (0x0000u)
#define CSL_VENC_CVBS_CSBLD_200US        (0x0001u)

#define CSL_VENC_CVBS_RESETVAL           (0x0000u)

/* RSV4 */

#define CSL_VENC_RSV4_RESETVAL           (0x0000u)

/* ETMG0 */

#define CSL_VENC_ETMG0_CEPW_MASK         (0x0F00u)
#define CSL_VENC_ETMG0_CEPW_SHIFT        (0x0008u)
#define CSL_VENC_ETMG0_CEPW_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG0_CFSW_MASK         (0x00F0u)
#define CSL_VENC_ETMG0_CFSW_SHIFT        (0x0004u)
#define CSL_VENC_ETMG0_CFSW_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG0_CLSW_MASK         (0x000Fu)
#define CSL_VENC_ETMG0_CLSW_SHIFT        (0x0000u)
#define CSL_VENC_ETMG0_CLSW_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG0_RESETVAL          (0x0000u)

/* ETMG1 */

#define CSL_VENC_ETMG1_CBSE_MASK         (0xF000u)
#define CSL_VENC_ETMG1_CBSE_SHIFT        (0x000Cu)
#define CSL_VENC_ETMG1_CBSE_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG1_CBST_MASK         (0x0F00u)
#define CSL_VENC_ETMG1_CBST_SHIFT        (0x0008u)
#define CSL_VENC_ETMG1_CBST_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG1_CFPW_MASK         (0x00F0u)
#define CSL_VENC_ETMG1_CFPW_SHIFT        (0x0004u)
#define CSL_VENC_ETMG1_CFPW_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG1_CLBI_MASK         (0x000Fu)
#define CSL_VENC_ETMG1_CLBI_SHIFT        (0x0000u)
#define CSL_VENC_ETMG1_CLBI_RESETVAL     (0x0000u)

#define CSL_VENC_ETMG1_RESETVAL          (0x0000u)

/* RSV5 */

#define CSL_VENC_RSV5_RESETVAL           (0x0000u)

/* RSV6 */

#define CSL_VENC_RSV6_RESETVAL           (0x0000u)

/* RSV7 */

#define CSL_VENC_RSV7_RESETVAL           (0x0000u)

/* RSV8 */

#define CSL_VENC_RSV8_RESETVAL           (0x0000u)

/* RSV9 */

#define CSL_VENC_RSV9_RESETVAL           (0x0000u)

/* RSV10 */

#define CSL_VENC_RSV10_RESETVAL          (0x0000u)

/* RSV11 */

#define CSL_VENC_RSV11_RESETVAL          (0x0000u)

/* RSV12 */

#define CSL_VENC_RSV12_RESETVAL          (0x0000u)

/* RSV13 */

#define CSL_VENC_RSV13_RESETVAL          (0x0000u)

/* RSV14 */

#define CSL_VENC_RSV14_RESETVAL          (0x0000u)

/* DRGBX0 */

#define CSL_VENC_DRGBX0_DGY_MASK         (0x07FFu)
#define CSL_VENC_DRGBX0_DGY_SHIFT        (0x0000u)
#define CSL_VENC_DRGBX0_DGY_RESETVAL     (0x0400u)

#define CSL_VENC_DRGBX0_RESETVAL         (0x0400u)

/* DRGBX1 */

#define CSL_VENC_DRGBX1_DRV_MASK         (0x07FFu)
#define CSL_VENC_DRGBX1_DRV_SHIFT        (0x0000u)
#define CSL_VENC_DRGBX1_DRV_RESETVAL     (0x057Cu)

#define CSL_VENC_DRGBX1_RESETVAL         (0x057Cu)

/* DRGBX2 */

#define CSL_VENC_DRGBX2_DGU_MASK         (0x07FFu)
#define CSL_VENC_DRGBX2_DGU_SHIFT        (0x0000u)
#define CSL_VENC_DRGBX2_DGU_RESETVAL     (0x0159u)

#define CSL_VENC_DRGBX2_RESETVAL         (0x0159u)

/* DRGBX3 */

#define CSL_VENC_DRGBX3_DGV_MASK         (0x07FFu)
#define CSL_VENC_DRGBX3_DGV_SHIFT        (0x0000u)
#define CSL_VENC_DRGBX3_DGV_RESETVAL     (0x02CBu)

#define CSL_VENC_DRGBX3_RESETVAL         (0x02CBu)

/* DRGBX4 */

#define CSL_VENC_DRGBX4_DBU_MASK         (0x07FFu)
#define CSL_VENC_DRGBX4_DBU_SHIFT        (0x0000u)
#define CSL_VENC_DRGBX4_DBU_RESETVAL     (0x06EEu)

#define CSL_VENC_DRGBX4_RESETVAL         (0x06EEu)

/* VSTARTA */

#define CSL_VENC_VSTARTA_VSTPA_MASK      (0x1FFFu)
#define CSL_VENC_VSTARTA_VSTPA_SHIFT     (0x0000u)
#define CSL_VENC_VSTARTA_VSTPA_RESETVAL  (0x0000u)

#define CSL_VENC_VSTARTA_RESETVAL        (0x0000u)

/* OSDCLK0 */

#define CSL_VENC_OSDCLK0_OCPW_MASK       (0x000Fu)
#define CSL_VENC_OSDCLK0_OCPW_SHIFT      (0x0000u)
#define CSL_VENC_OSDCLK0_OCPW_RESETVAL   (0x0001u)

#define CSL_VENC_OSDCLK0_RESETVAL        (0x0001u)

/* OSDCLK1 */

#define CSL_VENC_OSDCLK1_OCPT_MASK       (0xFFFFu)
#define CSL_VENC_OSDCLK1_OCPT_SHIFT      (0x0000u)
#define CSL_VENC_OSDCLK1_OCPT_RESETVAL   (0x0002u)

#define CSL_VENC_OSDCLK1_RESETVAL        (0x0002u)

/* HVLDCL0 */

#define CSL_VENC_HVLDCL0_HCM_MASK        (0x0010u)
#define CSL_VENC_HVLDCL0_HCM_SHIFT       (0x0004u)
#define CSL_VENC_HVLDCL0_HCM_RESETVAL    (0x0000u)

/*----HCM Tokens----*/
#define CSL_VENC_HVLDCL0_HCM_NORMAL      (0x0000u)
#define CSL_VENC_HVLDCL0_HCM_CULLING     (0x0001u)

#define CSL_VENC_HVLDCL0_HCPW_MASK       (0x000Fu)
#define CSL_VENC_HVLDCL0_HCPW_SHIFT      (0x0000u)
#define CSL_VENC_HVLDCL0_HCPW_RESETVAL   (0x0000u)

#define CSL_VENC_HVLDCL0_RESETVAL        (0x0000u)

/* HVLDCL1 */

#define CSL_VENC_HVLDCL1_HCPT_MASK       (0xFFFFu)
#define CSL_VENC_HVLDCL1_HCPT_SHIFT      (0x0000u)
#define CSL_VENC_HVLDCL1_HCPT_RESETVAL   (0x0000u)

#define CSL_VENC_HVLDCL1_RESETVAL        (0x0000u)

/* OSDHADV */

#define CSL_VENC_OSDHADV_OHAD_MASK       (0x00FFu)
#define CSL_VENC_OSDHADV_OHAD_SHIFT      (0x0000u)
#define CSL_VENC_OSDHADV_OHAD_RESETVAL   (0x0000u)

#define CSL_VENC_OSDHADV_RESETVAL        (0x0000u)

/* CLKCTL */

#define CSL_VENC_CLKCTL_CKGAM_MASK       (0x0100u)
#define CSL_VENC_CLKCTL_CKGAM_SHIFT      (0x0008u)
#define CSL_VENC_CLKCTL_CKGAM_RESETVAL   (0x0000u)

/*----CKGAM Tokens----*/
#define CSL_VENC_CLKCTL_CKGAM_OFF        (0x0000u)
#define CSL_VENC_CLKCTL_CKGAM_ON         (0x0001u)

#define CSL_VENC_CLKCTL_CLKDIG_MASK      (0x0010u)
#define CSL_VENC_CLKCTL_CLKDIG_SHIFT     (0x0004u)
#define CSL_VENC_CLKCTL_CLKDIG_RESETVAL  (0x0000u)

/*----CLKDIG Tokens----*/
#define CSL_VENC_CLKCTL_CLKDIG_OFF       (0x0000u)
#define CSL_VENC_CLKCTL_CLKDIG_ON        (0x0001u)

#define CSL_VENC_CLKCTL_CLKENC_MASK      (0x0001u)
#define CSL_VENC_CLKCTL_CLKENC_SHIFT     (0x0000u)
#define CSL_VENC_CLKCTL_CLKENC_RESETVAL  (0x0000u)

/*----CLKENC Tokens----*/
#define CSL_VENC_CLKCTL_CLKENC_OFF       (0x0000u)
#define CSL_VENC_CLKCTL_CLKENC_ON        (0x0001u)

#define CSL_VENC_CLKCTL_RESETVAL         (0x0000u)

/* GAMCTL */

#define CSL_VENC_GAMCTL_GAMON_MASK       (0x0001u)
#define CSL_VENC_GAMCTL_GAMON_SHIFT      (0x0000u)
#define CSL_VENC_GAMCTL_GAMON_RESETVAL   (0x0000u)

/*----GAMON Tokens----*/
#define CSL_VENC_GAMCTL_GAMON_OFF        (0x0000u)
#define CSL_VENC_GAMCTL_GAMON_ON         (0x0001u)

#define CSL_VENC_GAMCTL_RESETVAL         (0x0000u)

#endif
