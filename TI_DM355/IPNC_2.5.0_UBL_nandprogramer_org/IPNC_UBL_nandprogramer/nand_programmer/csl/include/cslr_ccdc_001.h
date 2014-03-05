#ifndef _CSLR_CCDC_CCDC_SPEC_V00_H_
#define _CSLR_CCDC_CCDC_SPEC_V00_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_ccdc_ccdc_spec_v00.h
 * 
 * \brief This file contains the Register Desciptions for CCDC
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure for FMTAPTR 
\**************************************************************************/
typedef struct  {
    volatile Uint16 ADDR_PTR;
    volatile Uint8 RSVD0[2];
} CSL_CcdcFmtaptrRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 SYNCEN;
    volatile Uint8 RSVD0[2];
    volatile Uint16 MODESET;
    volatile Uint8 RSVD1[2];
    volatile Uint16 HDWIDTH;
    volatile Uint8 RSVD2[2];
    volatile Uint16 VDWIDTH;
    volatile Uint8 RSVD3[2];
    volatile Uint16 PPLN;
    volatile Uint8 RSVD4[2];
    volatile Uint16 LPFR;
    volatile Uint8 RSVD5[2];
    volatile Uint16 SPH;
    volatile Uint8 RSVD6[2];
    volatile Uint16 NPH;
    volatile Uint8 RSVD7[2];
    volatile Uint16 SLV0;
    volatile Uint8 RSVD8[2];
    volatile Uint16 SLV1;
    volatile Uint8 RSVD9[2];
    volatile Uint16 NLV;
    volatile Uint8 RSVD10[2];
    volatile Uint16 CULH;
    volatile Uint8 RSVD11[2];
    volatile Uint16 CULV;
    volatile Uint8 RSVD12[2];
    volatile Uint16 HSIZE;
    volatile Uint8 RSVD13[2];
    volatile Uint16 SDOFST;
    volatile Uint8 RSVD14[2];
    volatile Uint16 STADRH;
    volatile Uint8 RSVD15[2];
    volatile Uint16 STADRL;
    volatile Uint8 RSVD16[2];
    volatile Uint16 CLAMP;
    volatile Uint8 RSVD17[2];
    volatile Uint16 DCSUB;
    volatile Uint8 RSVD18[2];
    volatile Uint16 COLPTN;
    volatile Uint8 RSVD19[2];
    volatile Uint16 BLKCMP0;
    volatile Uint8 RSVD20[2];
    volatile Uint16 BLKCMP1;
    volatile Uint8 RSVD21[2];
    volatile Uint16 MEDFILT;
    volatile Uint8 RSVD22[2];
    volatile Uint16 RYEGAIN;
    volatile Uint8 RSVD23[2];
    volatile Uint16 GRCYGAIN;
    volatile Uint8 RSVD24[2];
    volatile Uint16 GBGGAIN;
    volatile Uint8 RSVD25[2];
    volatile Uint16 BMGGAIN;
    volatile Uint8 RSVD26[2];
    volatile Uint16 OFFSET;
    volatile Uint8 RSVD27[2];
    volatile Uint16 OUTCLIP;
    volatile Uint8 RSVD28[2];
    volatile Uint16 VDINT0;
    volatile Uint8 RSVD29[2];
    volatile Uint16 VDINT1;
    volatile Uint8 RSVD30[2];
    volatile Uint16 RSV0;
    volatile Uint8 RSVD31[2];
    volatile Uint16 GAMMAWD;
    volatile Uint8 RSVD32[2];
    volatile Uint16 REC656IF;
    volatile Uint8 RSVD33[2];
    volatile Uint16 CCDCFG;
    volatile Uint8 RSVD34[2];
    volatile Uint16 FMTCFG;
    volatile Uint8 RSVD35[2];
    volatile Uint16 FMTPLEN;
    volatile Uint8 RSVD36[2];
    volatile Uint16 FMTSPH;
    volatile Uint8 RSVD37[2];
    volatile Uint16 FMTLNH;
    volatile Uint8 RSVD38[2];
    volatile Uint16 FMTSLV;
    volatile Uint8 RSVD39[2];
    volatile Uint16 FMTLNV;
    volatile Uint8 RSVD40[2];
    volatile Uint16 FMTRLEN;
    volatile Uint8 RSVD41[2];
    volatile Uint16 FMTHCNT;
    volatile Uint8 RSVD42[2];
    CSL_CcdcFmtaptrRegs FMTAPTR[8];
    volatile Uint16 FMTPGM_VF0;
    volatile Uint8 RSVD43[2];
    volatile Uint16 FMTPGM_VF1;
    volatile Uint8 RSVD44[2];
    volatile Uint16 FMTPGM_AP0;
    volatile Uint8 RSVD45[2];
    volatile Uint16 FMTPGM_AP1;
    volatile Uint8 RSVD46[2];
    volatile Uint16 FMTPGM_AP2;
    volatile Uint8 RSVD47[2];
    volatile Uint16 FMTPGM_AP3;
    volatile Uint8 RSVD48[2];
    volatile Uint16 FMTPGM_AP4;
    volatile Uint8 RSVD49[2];
    volatile Uint16 FMTPGM_AP5;
    volatile Uint8 RSVD50[2];
    volatile Uint16 FMTPGM_AP6;
    volatile Uint8 RSVD51[2];
    volatile Uint16 FMTPGM_AP7;
    volatile Uint8 RSVD52[2];
    volatile Uint16 LSCCFG1;
    volatile Uint8 RSVD53[2];
    volatile Uint16 LSCCFG2;
    volatile Uint8 RSVD54[2];
    volatile Uint16 LSCH0;
    volatile Uint8 RSVD55[2];
    volatile Uint16 LSCV0;
    volatile Uint8 RSVD56[2];
    volatile Uint16 LSCKH;
    volatile Uint8 RSVD57[2];
    volatile Uint16 LSCKV;
    volatile Uint8 RSVD58[2];
    volatile Uint16 LSCMEMCTL;
    volatile Uint8 RSVD59[2];
    volatile Uint16 LSCMEMD;
    volatile Uint8 RSVD60[2];
    volatile Uint16 LSCMEMQ;
    volatile Uint8 RSVD61[2];
    volatile Uint16 DFCCTL;
    volatile Uint8 RSVD62[2];
    volatile Uint16 DFCVSAT;
    volatile Uint8 RSVD63[2];
    volatile Uint16 DFCMEMCTL;
    volatile Uint8 RSVD64[2];
    volatile Uint16 DFCMEM0;
    volatile Uint8 RSVD65[2];
    volatile Uint16 DFCMEM1;
    volatile Uint8 RSVD66[2];
    volatile Uint16 DFCMEM2;
    volatile Uint8 RSVD67[2];
    volatile Uint16 DFCMEM3;
    volatile Uint8 RSVD68[2];
    volatile Uint16 DFCMEM4;
    volatile Uint8 RSVD69[2];
    volatile Uint16 CSCCTL;
    volatile Uint8 RSVD70[2];
    volatile Uint16 CSCM0;
    volatile Uint8 RSVD71[2];
    volatile Uint16 CSCM1;
    volatile Uint8 RSVD72[2];
    volatile Uint16 CSCM2;
    volatile Uint8 RSVD73[2];
    volatile Uint16 CSCM3;
    volatile Uint8 RSVD74[2];
    volatile Uint16 CSCM4;
    volatile Uint8 RSVD75[2];
    volatile Uint16 CSCM5;
    volatile Uint8 RSVD76[2];
    volatile Uint16 CSCM6;
    volatile Uint8 RSVD77[2];
    volatile Uint16 CSCM7;
    volatile Uint8 RSVD78[2];
    volatile Uint16 TEST;
} CSL_CcdcRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* SYNCEN */

#define CSL_CCDC_SYNCEN_WEN_MASK         (0x0002u)
#define CSL_CCDC_SYNCEN_WEN_SHIFT        (0x0001u)
#define CSL_CCDC_SYNCEN_WEN_RESETVAL     (0x0000u)

/*----WEN Tokens----*/
#define CSL_CCDC_SYNCEN_WEN_DISABLE      (0x0000u)
#define CSL_CCDC_SYNCEN_WEN_ENABLE       (0x0001u)

#define CSL_CCDC_SYNCEN_VDHDEN_MASK      (0x0001u)
#define CSL_CCDC_SYNCEN_VDHDEN_SHIFT     (0x0000u)
#define CSL_CCDC_SYNCEN_VDHDEN_RESETVAL  (0x0000u)

/*----VDHDEN Tokens----*/
#define CSL_CCDC_SYNCEN_VDHDEN_DISABLE   (0x0000u)
#define CSL_CCDC_SYNCEN_VDHDEN_ENABLE    (0x0001u)

#define CSL_CCDC_SYNCEN_RESETVAL         (0x0000u)

/* MODESET */

#define CSL_CCDC_MODESET_FLDSTAT_MASK    (0x8000u)
#define CSL_CCDC_MODESET_FLDSTAT_SHIFT   (0x000Fu)
#define CSL_CCDC_MODESET_FLDSTAT_RESETVAL (0x0000u)

/*----FLDSTAT Tokens----*/
#define CSL_CCDC_MODESET_FLDSTAT_ODDFIELD (0x0000u)
#define CSL_CCDC_MODESET_FLDSTAT_EVENFIELD (0x0001u)

#define CSL_CCDC_MODESET_LPF_MASK        (0x4000u)
#define CSL_CCDC_MODESET_LPF_SHIFT       (0x000Eu)
#define CSL_CCDC_MODESET_LPF_RESETVAL    (0x0000u)

/*----LPF Tokens----*/
#define CSL_CCDC_MODESET_LPF_OFF         (0x0000u)
#define CSL_CCDC_MODESET_LPF_ON          (0x0001u)

#define CSL_CCDC_MODESET_IMPMOD_MASK     (0x3000u)
#define CSL_CCDC_MODESET_IMPMOD_SHIFT    (0x000Cu)
#define CSL_CCDC_MODESET_IMPMOD_RESETVAL (0x0002u)

/*----IMPMOD Tokens----*/
#define CSL_CCDC_MODESET_IMPMOD_CCDRAW   (0x0000u)
#define CSL_CCDC_MODESET_IMPMOD_YCBCR16  (0x0001u)
#define CSL_CCDC_MODESET_IMPMOD_YCBCR8   (0x0002u)
#define CSL_CCDC_MODESET_IMPMOD_RESERVED (0x0003u)

#define CSL_CCDC_MODESET_PACK8_MASK      (0x0800u)
#define CSL_CCDC_MODESET_PACK8_SHIFT     (0x000Bu)
#define CSL_CCDC_MODESET_PACK8_RESETVAL  (0x0000u)

/*----PACK8 Tokens----*/
#define CSL_CCDC_MODESET_PACK8_NORMAL_16_BITS_PIXEL (0x0000u)
#define CSL_CCDC_MODESET_PACK8_PACK_8_BITS_PIXEL (0x0001u)

#define CSL_CCDC_MODESET_DATASFT_MASK    (0x0700u)
#define CSL_CCDC_MODESET_DATASFT_SHIFT   (0x0008u)
#define CSL_CCDC_MODESET_DATASFT_RESETVAL (0x0000u)

/*----DATASFT Tokens----*/
#define CSL_CCDC_MODESET_DATASFT_NO_SHIFT (0x0000u)
#define CSL_CCDC_MODESET_DATASFT__1_BIT  (0x0001u)
#define CSL_CCDC_MODESET_DATASFT__2_BITS (0x0002u)
#define CSL_CCDC_MODESET_DATASFT__3_BITS (0x0003u)
#define CSL_CCDC_MODESET_DATASFT__4_BITS (0x0004u)
#define CSL_CCDC_MODESET_DATASFT__5_BITS (0x0005u)
#define CSL_CCDC_MODESET_DATASFT__6_BITS (0x0006u)
#define CSL_CCDC_MODESET_DATASFT_RESERVED (0x0007u)

#define CSL_CCDC_MODESET_FLDMODE_MASK    (0x0080u)
#define CSL_CCDC_MODESET_FLDMODE_SHIFT   (0x0007u)
#define CSL_CCDC_MODESET_FLDMODE_RESETVAL (0x0000u)

/*----FLDMODE Tokens----*/
#define CSL_CCDC_MODESET_FLDMODE_NON_INTERLACED (0x0000u)
#define CSL_CCDC_MODESET_FLDMODE_INTERLACED (0x0001u)

#define CSL_CCDC_MODESET_DATAPOL_MASK    (0x0040u)
#define CSL_CCDC_MODESET_DATAPOL_SHIFT   (0x0006u)
#define CSL_CCDC_MODESET_DATAPOL_RESETVAL (0x0000u)

/*----DATAPOL Tokens----*/
#define CSL_CCDC_MODESET_DATAPOL_NORMAL  (0x0000u)
#define CSL_CCDC_MODESET_DATAPOL_ONES_COMPLEMENT (0x0001u)

#define CSL_CCDC_MODESET_EXWEN_MASK      (0x0020u)
#define CSL_CCDC_MODESET_EXWEN_SHIFT     (0x0005u)
#define CSL_CCDC_MODESET_EXWEN_RESETVAL  (0x0000u)

/*----EXWEN Tokens----*/
#define CSL_CCDC_MODESET_EXWEN_NO_EXTERNAL_WEN (0x0000u)
#define CSL_CCDC_MODESET_EXWEN_EXTERNAL_WEN (0x0001u)

#define CSL_CCDC_MODESET_FLDPOL_MASK     (0x0010u)
#define CSL_CCDC_MODESET_FLDPOL_SHIFT    (0x0004u)
#define CSL_CCDC_MODESET_FLDPOL_RESETVAL (0x0000u)

/*----FLDPOL Tokens----*/
#define CSL_CCDC_MODESET_FLDPOL_POSTIVE  (0x0000u)
#define CSL_CCDC_MODESET_FLDPOL_NEGATIVE (0x0001u)

#define CSL_CCDC_MODESET_HDPOL_MASK      (0x0008u)
#define CSL_CCDC_MODESET_HDPOL_SHIFT     (0x0003u)
#define CSL_CCDC_MODESET_HDPOL_RESETVAL  (0x0000u)

/*----HDPOL Tokens----*/
#define CSL_CCDC_MODESET_HDPOL_POSTIVE   (0x0000u)
#define CSL_CCDC_MODESET_HDPOL_NEGATIVE  (0x0001u)

#define CSL_CCDC_MODESET_VDPOL_MASK      (0x0004u)
#define CSL_CCDC_MODESET_VDPOL_SHIFT     (0x0002u)
#define CSL_CCDC_MODESET_VDPOL_RESETVAL  (0x0000u)

/*----VDPOL Tokens----*/
#define CSL_CCDC_MODESET_VDPOL_POSTIVE   (0x0000u)
#define CSL_CCDC_MODESET_VDPOL_NEGATIVE  (0x0001u)

#define CSL_CCDC_MODESET_FLDOUT_MASK     (0x0002u)
#define CSL_CCDC_MODESET_FLDOUT_SHIFT    (0x0001u)
#define CSL_CCDC_MODESET_FLDOUT_RESETVAL (0x0000u)

/*----FLDOUT Tokens----*/
#define CSL_CCDC_MODESET_FLDOUT_INPUT    (0x0000u)
#define CSL_CCDC_MODESET_FLDOUT_OUTPUT   (0x0001u)

#define CSL_CCDC_MODESET_VDHDOUT_MASK    (0x0001u)
#define CSL_CCDC_MODESET_VDHDOUT_SHIFT   (0x0000u)
#define CSL_CCDC_MODESET_VDHDOUT_RESETVAL (0x0000u)

/*----VDHDOUT Tokens----*/
#define CSL_CCDC_MODESET_VDHDOUT_INPUT   (0x0000u)
#define CSL_CCDC_MODESET_VDHDOUT_OUTPUT  (0x0001u)

#define CSL_CCDC_MODESET_RESETVAL        (0x2000u)

/* HDWIDTH */

#define CSL_CCDC_HDWIDTH_HDW_MASK        (0x0FFFu)
#define CSL_CCDC_HDWIDTH_HDW_SHIFT       (0x0000u)
#define CSL_CCDC_HDWIDTH_HDW_RESETVAL    (0x0000u)

#define CSL_CCDC_HDWIDTH_RESETVAL        (0x0000u)

/* VDWIDTH */

#define CSL_CCDC_VDWIDTH_VDW_MASK        (0x0FFFu)
#define CSL_CCDC_VDWIDTH_VDW_SHIFT       (0x0000u)
#define CSL_CCDC_VDWIDTH_VDW_RESETVAL    (0x0000u)

#define CSL_CCDC_VDWIDTH_RESETVAL        (0x0000u)

/* PPLN */

#define CSL_CCDC_PPLN_PPLN_MASK          (0xFFFFu)
#define CSL_CCDC_PPLN_PPLN_SHIFT         (0x0000u)
#define CSL_CCDC_PPLN_PPLN_RESETVAL      (0x0000u)

#define CSL_CCDC_PPLN_RESETVAL           (0x0000u)

/* LPFR */

#define CSL_CCDC_LPFR_LPFR_MASK          (0xFFFFu)
#define CSL_CCDC_LPFR_LPFR_SHIFT         (0x0000u)
#define CSL_CCDC_LPFR_LPFR_RESETVAL      (0x0000u)

#define CSL_CCDC_LPFR_RESETVAL           (0x0000u)

/* SPH */

#define CSL_CCDC_SPH_SPH_MASK            (0x7FFFu)
#define CSL_CCDC_SPH_SPH_SHIFT           (0x0000u)
#define CSL_CCDC_SPH_SPH_RESETVAL        (0x0000u)

#define CSL_CCDC_SPH_RESETVAL            (0x0000u)

/* NPH */

#define CSL_CCDC_NPH_NPH_MASK            (0x7FFFu)
#define CSL_CCDC_NPH_NPH_SHIFT           (0x0000u)
#define CSL_CCDC_NPH_NPH_RESETVAL        (0x0000u)

#define CSL_CCDC_NPH_RESETVAL            (0x0000u)

/* SLV0 */

#define CSL_CCDC_SLV0_SLV0_MASK          (0x7FFFu)
#define CSL_CCDC_SLV0_SLV0_SHIFT         (0x0000u)
#define CSL_CCDC_SLV0_SLV0_RESETVAL      (0x0000u)

#define CSL_CCDC_SLV0_RESETVAL           (0x0000u)

/* SLV1 */

#define CSL_CCDC_SLV1_SLV1_MASK          (0x7FFFu)
#define CSL_CCDC_SLV1_SLV1_SHIFT         (0x0000u)
#define CSL_CCDC_SLV1_SLV1_RESETVAL      (0x0000u)

#define CSL_CCDC_SLV1_RESETVAL           (0x0000u)

/* NLV */

#define CSL_CCDC_NLV_NLV_MASK            (0x7FFFu)
#define CSL_CCDC_NLV_NLV_SHIFT           (0x0000u)
#define CSL_CCDC_NLV_NLV_RESETVAL        (0x0000u)

#define CSL_CCDC_NLV_RESETVAL            (0x0000u)

/* CULH */

#define CSL_CCDC_CULH_CULHEVN_MASK       (0xFF00u)
#define CSL_CCDC_CULH_CULHEVN_SHIFT      (0x0008u)
#define CSL_CCDC_CULH_CULHEVN_RESETVAL   (0x00FFu)

#define CSL_CCDC_CULH_CULHODD_MASK       (0x00FFu)
#define CSL_CCDC_CULH_CULHODD_SHIFT      (0x0000u)
#define CSL_CCDC_CULH_CULHODD_RESETVAL   (0x00FFu)

#define CSL_CCDC_CULH_RESETVAL           (0xFFFFu)

/* CULV */

#define CSL_CCDC_CULV_CULV_MASK          (0x00FFu)
#define CSL_CCDC_CULV_CULV_SHIFT         (0x0000u)
#define CSL_CCDC_CULV_CULV_RESETVAL      (0x00FFu)

#define CSL_CCDC_CULV_RESETVAL           (0x00FFu)

/* HSIZE */

#define CSL_CCDC_HSIZE_ADR_UPDT_MASK     (0x1000u)
#define CSL_CCDC_HSIZE_ADR_UPDT_SHIFT    (0x000Cu)
#define CSL_CCDC_HSIZE_ADR_UPDT_RESETVAL (0x0000u)

/*----ADR_UPDT Tokens----*/
#define CSL_CCDC_HSIZE_ADR_UPDT_INCREMENT (0x0000u)
#define CSL_CCDC_HSIZE_ADR_UPDT_DECREMENT (0x0001u)

#define CSL_CCDC_HSIZE_LNOFST_MASK       (0x0FFFu)
#define CSL_CCDC_HSIZE_LNOFST_SHIFT      (0x0000u)
#define CSL_CCDC_HSIZE_LNOFST_RESETVAL   (0x0000u)

#define CSL_CCDC_HSIZE_RESETVAL          (0x0000u)

/* SDOFST */

#define CSL_CCDC_SDOFST_FIINV_MASK       (0x4000u)
#define CSL_CCDC_SDOFST_FIINV_SHIFT      (0x000Eu)
#define CSL_CCDC_SDOFST_FIINV_RESETVAL   (0x0000u)

/*----FIINV Tokens----*/
#define CSL_CCDC_SDOFST_FIINV_NONINVERSE (0x0000u)
#define CSL_CCDC_SDOFST_FIINV_INVERSE    (0x0001u)

#define CSL_CCDC_SDOFST_FOFST_MASK       (0x3000u)
#define CSL_CCDC_SDOFST_FOFST_SHIFT      (0x000Cu)
#define CSL_CCDC_SDOFST_FOFST_RESETVAL   (0x0000u)

/*----FOFST Tokens----*/
#define CSL_CCDC_SDOFST_FOFST_PLUS_1LINE (0x0000u)
#define CSL_CCDC_SDOFST_FOFST_PLUS_2LINE (0x0001u)
#define CSL_CCDC_SDOFST_FOFST_PLUS_3LINE (0x0002u)
#define CSL_CCDC_SDOFST_FOFST_PLUS_4LINE (0x0003u)

#define CSL_CCDC_SDOFST_LOFTS0_MASK      (0x0E00u)
#define CSL_CCDC_SDOFST_LOFTS0_SHIFT     (0x0009u)
#define CSL_CCDC_SDOFST_LOFTS0_RESETVAL  (0x0000u)

/*----LOFTS0 Tokens----*/
#define CSL_CCDC_SDOFST_LOFTS0_PLUS_1LINE (0x0000u)
#define CSL_CCDC_SDOFST_LOFTS0_PLUS_2LINES (0x0001u)
#define CSL_CCDC_SDOFST_LOFTS0_PLUS_3LINES (0x0002u)
#define CSL_CCDC_SDOFST_LOFTS0_PLUS_4LINES (0x0003u)
#define CSL_CCDC_SDOFST_LOFTS0_MINUS_1LINE (0x0004u)
#define CSL_CCDC_SDOFST_LOFTS0_MINUS_2LINES (0x0005u)
#define CSL_CCDC_SDOFST_LOFTS0_MINUS_3LINES (0x0006u)
#define CSL_CCDC_SDOFST_LOFTS0_MINUS_4LINES (0x0007u)

#define CSL_CCDC_SDOFST_LOFTS1_MASK      (0x01C0u)
#define CSL_CCDC_SDOFST_LOFTS1_SHIFT     (0x0006u)
#define CSL_CCDC_SDOFST_LOFTS1_RESETVAL  (0x0000u)

/*----LOFTS1 Tokens----*/
#define CSL_CCDC_SDOFST_LOFTS1_PLUS_1LINE (0x0000u)
#define CSL_CCDC_SDOFST_LOFTS1_PLUS_2LINES (0x0001u)
#define CSL_CCDC_SDOFST_LOFTS1_PLUS_3LINES (0x0002u)
#define CSL_CCDC_SDOFST_LOFTS1_PLUS_4LINES (0x0003u)
#define CSL_CCDC_SDOFST_LOFTS1_MINUS_1LINE (0x0004u)
#define CSL_CCDC_SDOFST_LOFTS1_MINUS_2LINES (0x0005u)
#define CSL_CCDC_SDOFST_LOFTS1_MINUS_3LINES (0x0006u)
#define CSL_CCDC_SDOFST_LOFTS1_MINUS_4LINES (0x0007u)

#define CSL_CCDC_SDOFST_LOFTS2_MASK      (0x0038u)
#define CSL_CCDC_SDOFST_LOFTS2_SHIFT     (0x0003u)
#define CSL_CCDC_SDOFST_LOFTS2_RESETVAL  (0x0000u)

/*----LOFTS2 Tokens----*/
#define CSL_CCDC_SDOFST_LOFTS2_PLUS_1LINE (0x0000u)
#define CSL_CCDC_SDOFST_LOFTS2_PLUS_2LINES (0x0001u)
#define CSL_CCDC_SDOFST_LOFTS2_PLUS_3LINES (0x0002u)
#define CSL_CCDC_SDOFST_LOFTS2_PLUS_4LINES (0x0003u)
#define CSL_CCDC_SDOFST_LOFTS2_MINUS_1LINE (0x0004u)
#define CSL_CCDC_SDOFST_LOFTS2_MINUS_2LINES (0x0005u)
#define CSL_CCDC_SDOFST_LOFTS2_MINUS_3LINES (0x0006u)
#define CSL_CCDC_SDOFST_LOFTS2_MINUS_4LINES (0x0007u)

#define CSL_CCDC_SDOFST_LOFTS3_MASK      (0x0007u)
#define CSL_CCDC_SDOFST_LOFTS3_SHIFT     (0x0000u)
#define CSL_CCDC_SDOFST_LOFTS3_RESETVAL  (0x0000u)

/*----LOFTS3 Tokens----*/
#define CSL_CCDC_SDOFST_LOFTS3_PLUS_1LINE (0x0000u)
#define CSL_CCDC_SDOFST_LOFTS3_PLUS_2LINES (0x0001u)
#define CSL_CCDC_SDOFST_LOFTS3_PLUS_3LINES (0x0002u)
#define CSL_CCDC_SDOFST_LOFTS3_PLUS_4LINES (0x0003u)
#define CSL_CCDC_SDOFST_LOFTS3_MINUS_1LINE (0x0004u)
#define CSL_CCDC_SDOFST_LOFTS3_MINUS_2LINES (0x0005u)
#define CSL_CCDC_SDOFST_LOFTS3_MINUS_3LINES (0x0006u)
#define CSL_CCDC_SDOFST_LOFTS3_MINUS_4LINES (0x0007u)

#define CSL_CCDC_SDOFST_RESETVAL         (0x0000u)

/* STADRH */

#define CSL_CCDC_STADRH_ADRH_MASK        (0x007Fu)
#define CSL_CCDC_STADRH_ADRH_SHIFT       (0x0000u)
#define CSL_CCDC_STADRH_ADRH_RESETVAL    (0x0000u)

#define CSL_CCDC_STADRH_RESETVAL         (0x0000u)

/* STADRL */

#define CSL_CCDC_STADRL_STADRL_MASK      (0xFFFFu)
#define CSL_CCDC_STADRL_STADRL_SHIFT     (0x0000u)
#define CSL_CCDC_STADRL_STADRL_RESETVAL  (0x0000u)

#define CSL_CCDC_STADRL_RESETVAL         (0x0000u)

/* CLAMP */

#define CSL_CCDC_CLAMP_CLAMPEN_MASK      (0x8000u)
#define CSL_CCDC_CLAMP_CLAMPEN_SHIFT     (0x000Fu)
#define CSL_CCDC_CLAMP_CLAMPEN_RESETVAL  (0x0000u)

/*----CLAMPEN Tokens----*/
#define CSL_CCDC_CLAMP_CLAMPEN_DISABLE   (0x0000u)
#define CSL_CCDC_CLAMP_CLAMPEN_ENABLE    (0x0001u)

#define CSL_CCDC_CLAMP_OBSLEN_MASK       (0x4000u)
#define CSL_CCDC_CLAMP_OBSLEN_SHIFT      (0x000Eu)
#define CSL_CCDC_CLAMP_OBSLEN_RESETVAL   (0x0000u)

/*----OBSLEN Tokens----*/
#define CSL_CCDC_CLAMP_OBSLEN__2PIXELS   (0x0000u)
#define CSL_CCDC_CLAMP_OBSLEN__4PIXELS   (0x0001u)
#define CSL_CCDC_CLAMP_OBSLEN__8PIXELS   (0x0002u)
#define CSL_CCDC_CLAMP_OBSLEN__16PIXELS  (0x0003u)

#define CSL_CCDC_CLAMP_RESERVED_MASK     (0x2000u)
#define CSL_CCDC_CLAMP_RESERVED_SHIFT    (0x000Du)
#define CSL_CCDC_CLAMP_RESERVED_RESETVAL (0x0000u)

#define CSL_CCDC_CLAMP_OBST_MASK         (0x1FFFu)
#define CSL_CCDC_CLAMP_OBST_SHIFT        (0x0000u)
#define CSL_CCDC_CLAMP_OBST_RESETVAL     (0x0000u)

#define CSL_CCDC_CLAMP_RESETVAL          (0x0000u)

/* DCSUB */

#define CSL_CCDC_DCSUB_OBSLN_MASK        (0xC000u)
#define CSL_CCDC_DCSUB_OBSLN_SHIFT       (0x000Eu)
#define CSL_CCDC_DCSUB_OBSLN_RESETVAL    (0x0000u)

/*----OBSLN Tokens----*/
#define CSL_CCDC_DCSUB_OBSLN__2LINES     (0x0000u)
#define CSL_CCDC_DCSUB_OBSLN__4LINES     (0x0001u)
#define CSL_CCDC_DCSUB_OBSLN__8LINES     (0x0002u)
#define CSL_CCDC_DCSUB_OBSLN__16LINES    (0x0003u)

#define CSL_CCDC_DCSUB_DCSUB_MASK        (0x3FFFu)
#define CSL_CCDC_DCSUB_DCSUB_SHIFT       (0x0000u)
#define CSL_CCDC_DCSUB_DCSUB_RESETVAL    (0x0000u)

#define CSL_CCDC_DCSUB_RESETVAL          (0x0000u)

/* COLPTN */

#define CSL_CCDC_COLPTN_CPELPC0_MASK     (0xC000u)
#define CSL_CCDC_COLPTN_CPELPC0_SHIFT    (0x000Eu)
#define CSL_CCDC_COLPTN_CPELPC0_RESETVAL (0x0000u)

/*----CPELPC0 Tokens----*/
#define CSL_CCDC_COLPTN_CPELPC0_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPELPC0_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPELPC0_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPELPC0_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPELPC1_MASK     (0x3000u)
#define CSL_CCDC_COLPTN_CPELPC1_SHIFT    (0x000Cu)
#define CSL_CCDC_COLPTN_CPELPC1_RESETVAL (0x0000u)

/*----CPELPC1 Tokens----*/
#define CSL_CCDC_COLPTN_CPELPC1_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPELPC1_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPELPC1_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPELPC1_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPELPC2_MASK     (0x0C00u)
#define CSL_CCDC_COLPTN_CPELPC2_SHIFT    (0x000Au)
#define CSL_CCDC_COLPTN_CPELPC2_RESETVAL (0x0000u)

/*----CPELPC2 Tokens----*/
#define CSL_CCDC_COLPTN_CPELPC2_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPELPC2_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPELPC2_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPELPC2_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPELPC3_MASK     (0x0300u)
#define CSL_CCDC_COLPTN_CPELPC3_SHIFT    (0x0008u)
#define CSL_CCDC_COLPTN_CPELPC3_RESETVAL (0x0000u)

/*----CPELPC3 Tokens----*/
#define CSL_CCDC_COLPTN_CPELPC3_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPELPC3_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPELPC3_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPELPC3_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPOLPC0_MASK     (0x00C0u)
#define CSL_CCDC_COLPTN_CPOLPC0_SHIFT    (0x0006u)
#define CSL_CCDC_COLPTN_CPOLPC0_RESETVAL (0x0000u)

/*----CPOLPC0 Tokens----*/
#define CSL_CCDC_COLPTN_CPOLPC0_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPOLPC0_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPOLPC0_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPOLPC0_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPOLPC1_MASK     (0x0030u)
#define CSL_CCDC_COLPTN_CPOLPC1_SHIFT    (0x0004u)
#define CSL_CCDC_COLPTN_CPOLPC1_RESETVAL (0x0000u)

/*----CPOLPC1 Tokens----*/
#define CSL_CCDC_COLPTN_CPOLPC1_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPOLPC1_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPOLPC1_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPOLPC1_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPOLPC2_MASK     (0x000Cu)
#define CSL_CCDC_COLPTN_CPOLPC2_SHIFT    (0x0002u)
#define CSL_CCDC_COLPTN_CPOLPC2_RESETVAL (0x0000u)

/*----CPOLPC2 Tokens----*/
#define CSL_CCDC_COLPTN_CPOLPC2_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPOLPC2_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPOLPC2_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPOLPC2_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_CPOLPC3_MASK     (0x0003u)
#define CSL_CCDC_COLPTN_CPOLPC3_SHIFT    (0x0000u)
#define CSL_CCDC_COLPTN_CPOLPC3_RESETVAL (0x0000u)

/*----CPOLPC3 Tokens----*/
#define CSL_CCDC_COLPTN_CPOLPC3_R_YE     (0x0000u)
#define CSL_CCDC_COLPTN_CPOLPC3_GR_CY    (0x0001u)
#define CSL_CCDC_COLPTN_CPOLPC3_GB_G     (0x0002u)
#define CSL_CCDC_COLPTN_CPOLPC3_B_MG     (0x0003u)

#define CSL_CCDC_COLPTN_RESETVAL         (0x0000u)

/* BLKCMP0 */

#define CSL_CCDC_BLKCMP0_R_YE_MASK       (0xFF00u)
#define CSL_CCDC_BLKCMP0_R_YE_SHIFT      (0x0008u)
#define CSL_CCDC_BLKCMP0_R_YE_RESETVAL   (0x0000u)

#define CSL_CCDC_BLKCMP0_GR_CY_MASK      (0x00FFu)
#define CSL_CCDC_BLKCMP0_GR_CY_SHIFT     (0x0000u)
#define CSL_CCDC_BLKCMP0_GR_CY_RESETVAL  (0x0000u)

#define CSL_CCDC_BLKCMP0_RESETVAL        (0x0000u)

/* BLKCMP1 */

#define CSL_CCDC_BLKCMP1_GB_G_MASK       (0xFF00u)
#define CSL_CCDC_BLKCMP1_GB_G_SHIFT      (0x0008u)
#define CSL_CCDC_BLKCMP1_GB_G_RESETVAL   (0x0000u)

#define CSL_CCDC_BLKCMP1_B_MG_MASK       (0x00FFu)
#define CSL_CCDC_BLKCMP1_B_MG_SHIFT      (0x0000u)
#define CSL_CCDC_BLKCMP1_B_MG_RESETVAL   (0x0000u)

#define CSL_CCDC_BLKCMP1_RESETVAL        (0x0000u)

/* MEDFILT */

#define CSL_CCDC_MEDFILT_MFTHR_MASK      (0x3FFFu)
#define CSL_CCDC_MEDFILT_MFTHR_SHIFT     (0x0000u)
#define CSL_CCDC_MEDFILT_MFTHR_RESETVAL  (0x0000u)

#define CSL_CCDC_MEDFILT_RESETVAL        (0x0000u)

/* RYEGAIN */

#define CSL_CCDC_RYEGAIN_GAIN_MASK       (0x07FFu)
#define CSL_CCDC_RYEGAIN_GAIN_SHIFT      (0x0000u)
#define CSL_CCDC_RYEGAIN_GAIN_RESETVAL   (0x0100u)

#define CSL_CCDC_RYEGAIN_RESETVAL        (0x0100u)

/* GRCYGAIN */

#define CSL_CCDC_GRCYGAIN_GAIN_MASK      (0x07FFu)
#define CSL_CCDC_GRCYGAIN_GAIN_SHIFT     (0x0000u)
#define CSL_CCDC_GRCYGAIN_GAIN_RESETVAL  (0x0100u)

#define CSL_CCDC_GRCYGAIN_RESETVAL       (0x0100u)

/* GBGGAIN */

#define CSL_CCDC_GBGGAIN_GAIN_MASK       (0x07FFu)
#define CSL_CCDC_GBGGAIN_GAIN_SHIFT      (0x0000u)
#define CSL_CCDC_GBGGAIN_GAIN_RESETVAL   (0x0100u)

#define CSL_CCDC_GBGGAIN_RESETVAL        (0x0100u)

/* BMGGAIN */

#define CSL_CCDC_BMGGAIN_GAIN_MASK       (0x07FFu)
#define CSL_CCDC_BMGGAIN_GAIN_SHIFT      (0x0000u)
#define CSL_CCDC_BMGGAIN_GAIN_RESETVAL   (0x0100u)

#define CSL_CCDC_BMGGAIN_RESETVAL        (0x0100u)

/* OFFSET */

#define CSL_CCDC_OFFSET_OFFSET_MASK      (0x03FFu)
#define CSL_CCDC_OFFSET_OFFSET_SHIFT     (0x0000u)
#define CSL_CCDC_OFFSET_OFFSET_RESETVAL  (0x0000u)

#define CSL_CCDC_OFFSET_RESETVAL         (0x0000u)

/* OUTCLIP */

#define CSL_CCDC_OUTCLIP_OCLIP_MASK      (0x3FFFu)
#define CSL_CCDC_OUTCLIP_OCLIP_SHIFT     (0x0000u)
#define CSL_CCDC_OUTCLIP_OCLIP_RESETVAL  (0x3FFFu)

#define CSL_CCDC_OUTCLIP_RESETVAL        (0x3FFFu)

/* VDINT0 */

#define CSL_CCDC_VDINT0_VDINT0_MASK      (0x7FFFu)
#define CSL_CCDC_VDINT0_VDINT0_SHIFT     (0x0000u)
#define CSL_CCDC_VDINT0_VDINT0_RESETVAL  (0x0000u)

#define CSL_CCDC_VDINT0_RESETVAL         (0x0000u)

/* VDINT1 */

#define CSL_CCDC_VDINT1_VDINT1_MASK      (0x7FFFu)
#define CSL_CCDC_VDINT1_VDINT1_SHIFT     (0x0000u)
#define CSL_CCDC_VDINT1_VDINT1_RESETVAL  (0x0000u)

#define CSL_CCDC_VDINT1_RESETVAL         (0x0000u)

/* RSV0 */

#define CSL_CCDC_RSV0_RSV0_MASK          (0xFFFFu)
#define CSL_CCDC_RSV0_RSV0_SHIFT         (0x0000u)
#define CSL_CCDC_RSV0_RSV0_RESETVAL      (0x0000u)

#define CSL_CCDC_RSV0_RESETVAL           (0x0000u)

/* GAMMAWD */

#define CSL_CCDC_GAMMAWD_MFIL1_MASK      (0x0C00u)
#define CSL_CCDC_GAMMAWD_MFIL1_SHIFT     (0x000Au)
#define CSL_CCDC_GAMMAWD_MFIL1_RESETVAL  (0x0000u)

/*----MFIL1 Tokens----*/
#define CSL_CCDC_GAMMAWD_MFIL1_NOMEDIANFILTER (0x0000u)
#define CSL_CCDC_GAMMAWD_MFIL1_AVERAGEFILTER (0x0001u)
#define CSL_CCDC_GAMMAWD_MFIL1_MEDIANFILTER (0x0002u)
#define CSL_CCDC_GAMMAWD_MFIL1_RESERVED  (0x0003u)

#define CSL_CCDC_GAMMAWD_MFIL2_MASK      (0x0300u)
#define CSL_CCDC_GAMMAWD_MFIL2_SHIFT     (0x0008u)
#define CSL_CCDC_GAMMAWD_MFIL2_RESETVAL  (0x0000u)

/*----MFIL2 Tokens----*/
#define CSL_CCDC_GAMMAWD_MFIL2_NOMEDIANFILTER (0x0000u)
#define CSL_CCDC_GAMMAWD_MFIL2_AVERAGEFILTER (0x0001u)
#define CSL_CCDC_GAMMAWD_MFIL2_MEDIANFILTER (0x0002u)
#define CSL_CCDC_GAMMAWD_MFIL2_RESERVED  (0x0003u)

#define CSL_CCDC_GAMMAWD_CFAP_MASK       (0x0020u)
#define CSL_CCDC_GAMMAWD_CFAP_SHIFT      (0x0005u)
#define CSL_CCDC_GAMMAWD_CFAP_RESETVAL   (0x0000u)

/*----CFAP Tokens----*/
#define CSL_CCDC_GAMMAWD_CFAP_MOSAIC     (0x0000u)
#define CSL_CCDC_GAMMAWD_CFAP_STRIPE     (0x0001u)

#define CSL_CCDC_GAMMAWD_GWDI_MASK       (0x001Cu)
#define CSL_CCDC_GAMMAWD_GWDI_SHIFT      (0x0002u)
#define CSL_CCDC_GAMMAWD_GWDI_RESETVAL   (0x0000u)

/*----GWDI Tokens----*/
#define CSL_CCDC_GAMMAWD_GWDI_BITS13_4   (0x0000u)
#define CSL_CCDC_GAMMAWD_GWDI_BITS12_3   (0x0001u)
#define CSL_CCDC_GAMMAWD_GWDI_BITS11_2   (0x0002u)
#define CSL_CCDC_GAMMAWD_GWDI_BITS10_1   (0x0003u)
#define CSL_CCDC_GAMMAWD_GWDI_BITS9_0    (0x0004u)

#define CSL_CCDC_GAMMAWD_CCDTBL_MASK     (0x0001u)
#define CSL_CCDC_GAMMAWD_CCDTBL_SHIFT    (0x0000u)
#define CSL_CCDC_GAMMAWD_CCDTBL_RESETVAL (0x0000u)

/*----CCDTBL Tokens----*/
#define CSL_CCDC_GAMMAWD_CCDTBL_DISABLE  (0x0000u)
#define CSL_CCDC_GAMMAWD_CCDTBL_ENABLE   (0x0001u)

#define CSL_CCDC_GAMMAWD_RESETVAL        (0x0000u)

/* REC656IF */

#define CSL_CCDC_REC656IF_ECCFVH_MASK    (0x0002u)
#define CSL_CCDC_REC656IF_ECCFVH_SHIFT   (0x0001u)
#define CSL_CCDC_REC656IF_ECCFVH_RESETVAL (0x0000u)

/*----ECCFVH Tokens----*/
#define CSL_CCDC_REC656IF_ECCFVH_DISABLE (0x0000u)
#define CSL_CCDC_REC656IF_ECCFVH_ENABLE  (0x0001u)

#define CSL_CCDC_REC656IF_R656ON_MASK    (0x0001u)
#define CSL_CCDC_REC656IF_R656ON_SHIFT   (0x0000u)
#define CSL_CCDC_REC656IF_R656ON_RESETVAL (0x0000u)

/*----R656ON Tokens----*/
#define CSL_CCDC_REC656IF_R656ON_DISABLE (0x0000u)
#define CSL_CCDC_REC656IF_R656ON_ENABLE  (0x0001u)

#define CSL_CCDC_REC656IF_RESETVAL       (0x0000u)

/* CCDCFG */

#define CSL_CCDC_CCDCFG_VDLC_MASK        (0x8000u)
#define CSL_CCDC_CCDCFG_VDLC_SHIFT       (0x000Fu)
#define CSL_CCDC_CCDCFG_VDLC_RESETVAL    (0x0000u)

/*----VDLC Tokens----*/
#define CSL_CCDC_CCDCFG_VDLC_LATCHEDONVSYNC (0x0000u)
#define CSL_CCDC_CCDCFG_VDLC_NOTLATCHEDONVSYNC (0x0001u)

#define CSL_CCDC_CCDCFG_MSBINVO_MASK     (0x4000u)
#define CSL_CCDC_CCDCFG_MSBINVO_SHIFT    (0x000Eu)
#define CSL_CCDC_CCDCFG_MSBINVO_RESETVAL (0x0000u)

/*----MSBINVO Tokens----*/
#define CSL_CCDC_CCDCFG_MSBINVO_NORMAL   (0x0000u)
#define CSL_CCDC_CCDCFG_MSBINVO_MSBINVERTED (0x0001u)

#define CSL_CCDC_CCDCFG_MSBINVI_MASK     (0x2000u)
#define CSL_CCDC_CCDCFG_MSBINVI_SHIFT    (0x000Du)
#define CSL_CCDC_CCDCFG_MSBINVI_RESETVAL (0x0000u)

/*----MSBINVI Tokens----*/
#define CSL_CCDC_CCDCFG_MSBINVI_NORMAL   (0x0000u)
#define CSL_CCDC_CCDCFG_MSBINVI_MSBINVERTED (0x0001u)

#define CSL_CCDC_CCDCFG_BSWD_MASK        (0x1000u)
#define CSL_CCDC_CCDCFG_BSWD_SHIFT       (0x000Cu)
#define CSL_CCDC_CCDCFG_BSWD_RESETVAL    (0x0000u)

/*----BSWD Tokens----*/
#define CSL_CCDC_CCDCFG_BSWD_NORMAL      (0x0000u)
#define CSL_CCDC_CCDCFG_BSWD_SWAPBYTES   (0x0001u)

#define CSL_CCDC_CCDCFG_Y8POS_MASK       (0x0800u)
#define CSL_CCDC_CCDCFG_Y8POS_SHIFT      (0x000Bu)
#define CSL_CCDC_CCDCFG_Y8POS_RESETVAL   (0x0000u)

/*----Y8POS Tokens----*/
#define CSL_CCDC_CCDCFG_Y8POS_EVENPIXEL  (0x0000u)
#define CSL_CCDC_CCDCFG_Y8POS_ODDPIXEL   (0x0001u)

#define CSL_CCDC_CCDCFG_EXTRG_MASK       (0x0400u)
#define CSL_CCDC_CCDCFG_EXTRG_SHIFT      (0x000Au)
#define CSL_CCDC_CCDCFG_EXTRG_RESETVAL   (0x0000u)

/*----EXTRG Tokens----*/
#define CSL_CCDC_CCDCFG_EXTRG_DISABLE    (0x0000u)
#define CSL_CCDC_CCDCFG_EXTRG_ENABLE     (0x0001u)

#define CSL_CCDC_CCDCFG_TRGSEL_MASK      (0x0200u)
#define CSL_CCDC_CCDCFG_TRGSEL_SHIFT     (0x0009u)
#define CSL_CCDC_CCDCFG_TRGSEL_RESETVAL  (0x0000u)

/*----TRGSEL Tokens----*/
#define CSL_CCDC_CCDCFG_TRGSEL_WENBIT    (0x0000u)
#define CSL_CCDC_CCDCFG_TRGSEL_FID_INPUT_PORT (0x0001u)

#define CSL_CCDC_CCDCFG_WENLOG_MASK      (0x0100u)
#define CSL_CCDC_CCDCFG_WENLOG_SHIFT     (0x0008u)
#define CSL_CCDC_CCDCFG_WENLOG_RESETVAL  (0x0000u)

/*----WENLOG Tokens----*/
#define CSL_CCDC_CCDCFG_WENLOG_AND       (0x0000u)
#define CSL_CCDC_CCDCFG_WENLOG_OR        (0x0001u)

#define CSL_CCDC_CCDCFG_FIDMD_MASK       (0x0040u)
#define CSL_CCDC_CCDCFG_FIDMD_SHIFT      (0x0006u)
#define CSL_CCDC_CCDCFG_FIDMD_RESETVAL   (0x0000u)

/*----FIDMD Tokens----*/
#define CSL_CCDC_CCDCFG_FIDMD_LATCH_AT_VSYNC (0x0000u)
#define CSL_CCDC_CCDCFG_FIDMD_NO_LATCH   (0x0001u)

#define CSL_CCDC_CCDCFG_BW656_MASK       (0x0020u)
#define CSL_CCDC_CCDCFG_BW656_SHIFT      (0x0005u)
#define CSL_CCDC_CCDCFG_BW656_RESETVAL   (0x0000u)

/*----BW656 Tokens----*/
#define CSL_CCDC_CCDCFG_BW656__8_BITS    (0x0000u)
#define CSL_CCDC_CCDCFG_BW656__10_BITS   (0x0001u)

#define CSL_CCDC_CCDCFG_YCINSWP_MASK     (0x0010u)
#define CSL_CCDC_CCDCFG_YCINSWP_SHIFT    (0x0004u)
#define CSL_CCDC_CCDCFG_YCINSWP_RESETVAL (0x0000u)

/*----YCINSWP Tokens----*/
#define CSL_CCDC_CCDCFG_YCINSWP_NO_YCIN_SWAP (0x0000u)
#define CSL_CCDC_CCDCFG_YCINSWP_YCIN_SWAP (0x0001u)

#define CSL_CCDC_CCDCFG_YCOUTSWP_MASK    (0x0004u)
#define CSL_CCDC_CCDCFG_YCOUTSWP_SHIFT   (0x0002u)
#define CSL_CCDC_CCDCFG_YCOUTSWP_RESETVAL (0x0000u)

/*----YCOUTSWP Tokens----*/
#define CSL_CCDC_CCDCFG_YCOUTSWP_NO_YCOUT_SWAP (0x0000u)
#define CSL_CCDC_CCDCFG_YCOUTSWP_YCOUT_SWAP (0x0001u)

#define CSL_CCDC_CCDCFG_IDS_MASK         (0x0002u)
#define CSL_CCDC_CCDCFG_IDS_SHIFT        (0x0001u)
#define CSL_CCDC_CCDCFG_IDS_RESETVAL     (0x0000u)

/*----IDS Tokens----*/
#define CSL_CCDC_CCDCFG_IDS_YIN_7        (0x0000u)
#define CSL_CCDC_CCDCFG_IDS_YOUT_7       (0x0001u)

#define CSL_CCDC_CCDCFG_RESETVAL         (0x0000u)

/* FMTCFG */

#define CSL_CCDC_FMTCFG_ADDRINC_MASK     (0x0700u)
#define CSL_CCDC_FMTCFG_ADDRINC_SHIFT    (0x0008u)
#define CSL_CCDC_FMTCFG_ADDRINC_RESETVAL (0x0000u)

#define CSL_CCDC_FMTCFG_LNUM_MASK        (0x0030u)
#define CSL_CCDC_FMTCFG_LNUM_SHIFT       (0x0004u)
#define CSL_CCDC_FMTCFG_LNUM_RESETVAL    (0x0000u)

/*----LNUM Tokens----*/
#define CSL_CCDC_FMTCFG_LNUM_1LINE       (0x0000u)
#define CSL_CCDC_FMTCFG_LNUM_2LINES      (0x0001u)
#define CSL_CCDC_FMTCFG_LNUM_3LINES      (0x0002u)
#define CSL_CCDC_FMTCFG_LNUM_4LINES      (0x0003u)

#define CSL_CCDC_FMTCFG_LNALT_MASK       (0x0004u)
#define CSL_CCDC_FMTCFG_LNALT_SHIFT      (0x0002u)
#define CSL_CCDC_FMTCFG_LNALT_RESETVAL   (0x0000u)

/*----LNALT Tokens----*/
#define CSL_CCDC_FMTCFG_LNALT_NORMALMODE (0x0000u)
#define CSL_CCDC_FMTCFG_LNALT_LINEALTERNATINGMODE (0x0001u)

#define CSL_CCDC_FMTCFG_FMTCBL_MASK      (0x0002u)
#define CSL_CCDC_FMTCFG_FMTCBL_SHIFT     (0x0001u)
#define CSL_CCDC_FMTCFG_FMTCBL_RESETVAL  (0x0000u)

/*----FMTCBL Tokens----*/
#define CSL_CCDC_FMTCFG_FMTCBL_SPLIT     (0x0000u)
#define CSL_CCDC_FMTCFG_FMTCBL_COMBINE   (0x0001u)

#define CSL_CCDC_FMTCFG_FMTEN_MASK       (0x0001u)
#define CSL_CCDC_FMTCFG_FMTEN_SHIFT      (0x0000u)
#define CSL_CCDC_FMTCFG_FMTEN_RESETVAL   (0x0000u)

/*----FMTEN Tokens----*/
#define CSL_CCDC_FMTCFG_FMTEN_OFF        (0x0000u)
#define CSL_CCDC_FMTCFG_FMTEN_ON         (0x0001u)

#define CSL_CCDC_FMTCFG_RESETVAL         (0x0000u)

/* FMTPLEN */

#define CSL_CCDC_FMTPLEN_PLEN3_MASK      (0x7000u)
#define CSL_CCDC_FMTPLEN_PLEN3_SHIFT     (0x000Cu)
#define CSL_CCDC_FMTPLEN_PLEN3_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTPLEN_PLEN2_MASK      (0x0700u)
#define CSL_CCDC_FMTPLEN_PLEN2_SHIFT     (0x0008u)
#define CSL_CCDC_FMTPLEN_PLEN2_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTPLEN_PLEN1_MASK      (0x00F0u)
#define CSL_CCDC_FMTPLEN_PLEN1_SHIFT     (0x0004u)
#define CSL_CCDC_FMTPLEN_PLEN1_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTPLEN_PLEN0_MASK      (0x000Fu)
#define CSL_CCDC_FMTPLEN_PLEN0_SHIFT     (0x0000u)
#define CSL_CCDC_FMTPLEN_PLEN0_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTPLEN_RESETVAL        (0x0000u)

/* FMTSPH */

#define CSL_CCDC_FMTSPH_FMTSPH_MASK      (0x1FFFu)
#define CSL_CCDC_FMTSPH_FMTSPH_SHIFT     (0x0000u)
#define CSL_CCDC_FMTSPH_FMTSPH_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTSPH_RESETVAL         (0x0000u)

/* FMTLNH */

#define CSL_CCDC_FMTLNH_FMTLNH_MASK      (0x1FFFu)
#define CSL_CCDC_FMTLNH_FMTLNH_SHIFT     (0x0000u)
#define CSL_CCDC_FMTLNH_FMTLNH_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTLNH_RESETVAL         (0x0000u)

/* FMTSLV */

#define CSL_CCDC_FMTSLV_FMTSLV_MASK      (0x1FFFu)
#define CSL_CCDC_FMTSLV_FMTSLV_SHIFT     (0x0000u)
#define CSL_CCDC_FMTSLV_FMTSLV_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTSLV_RESETVAL         (0x0000u)

/* FMTLNV */

#define CSL_CCDC_FMTLNV_FMTLNV_MASK      (0x7FFFu)
#define CSL_CCDC_FMTLNV_FMTLNV_SHIFT     (0x0000u)
#define CSL_CCDC_FMTLNV_FMTLNV_RESETVAL  (0x0000u)

#define CSL_CCDC_FMTLNV_RESETVAL         (0x0000u)

/* FMTRLEN */

#define CSL_CCDC_FMTRLEN_FMTRLEN_MASK    (0x1FFFu)
#define CSL_CCDC_FMTRLEN_FMTRLEN_SHIFT   (0x0000u)
#define CSL_CCDC_FMTRLEN_FMTRLEN_RESETVAL (0x0000u)

#define CSL_CCDC_FMTRLEN_RESETVAL        (0x0000u)

/* FMTHCNT */

#define CSL_CCDC_FMTHCNT_FMTHCNT_MASK    (0x1FFFu)
#define CSL_CCDC_FMTHCNT_FMTHCNT_SHIFT   (0x0000u)
#define CSL_CCDC_FMTHCNT_FMTHCNT_RESETVAL (0x0000u)

#define CSL_CCDC_FMTHCNT_RESETVAL        (0x0000u)

/* ADDR_PTR */

#define CSL_CCDC_ADDR_PTR_LINE_MASK      (0x6000u)
#define CSL_CCDC_ADDR_PTR_LINE_SHIFT     (0x000Du)
#define CSL_CCDC_ADDR_PTR_LINE_RESETVAL  (0x0000u)

/*----LINE Tokens----*/
#define CSL_CCDC_ADDR_PTR_LINE__1STLINE  (0x0000u)
#define CSL_CCDC_ADDR_PTR_LINE__2NDLINE  (0x0001u)
#define CSL_CCDC_ADDR_PTR_LINE__3RDLINE  (0x0002u)
#define CSL_CCDC_ADDR_PTR_LINE__4THLINE  (0x0003u)

#define CSL_CCDC_ADDR_PTR_INIT_MASK      (0x1FFFu)
#define CSL_CCDC_ADDR_PTR_INIT_SHIFT     (0x0000u)
#define CSL_CCDC_ADDR_PTR_INIT_RESETVAL  (0x0000u)

#define CSL_CCDC_ADDR_PTR_RESETVAL       (0x0000u)

/* FMTPGM_VF0 */

#define CSL_CCDC_FMTPGM_VF0_PGM15EN_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_VF0_PGM15EN_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_VF0_PGM15EN_RESETVAL (0x0000u)

/*----PGM15EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM15EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM15EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM14EN_MASK (0x4000u)
#define CSL_CCDC_FMTPGM_VF0_PGM14EN_SHIFT (0x000Eu)
#define CSL_CCDC_FMTPGM_VF0_PGM14EN_RESETVAL (0x0000u)

/*----PGM14EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM14EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM14EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM13EN_MASK (0x2000u)
#define CSL_CCDC_FMTPGM_VF0_PGM13EN_SHIFT (0x000Du)
#define CSL_CCDC_FMTPGM_VF0_PGM13EN_RESETVAL (0x0000u)

/*----PGM13EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM13EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM13EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM12EN_MASK (0x1000u)
#define CSL_CCDC_FMTPGM_VF0_PGM12EN_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_VF0_PGM12EN_RESETVAL (0x0000u)

/*----PGM12EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM12EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM12EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM11EN_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_VF0_PGM11EN_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_VF0_PGM11EN_RESETVAL (0x0000u)

/*----PGM11EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM11EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM11EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM10EN_MASK (0x0400u)
#define CSL_CCDC_FMTPGM_VF0_PGM10EN_SHIFT (0x000Au)
#define CSL_CCDC_FMTPGM_VF0_PGM10EN_RESETVAL (0x0000u)

/*----PGM10EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM10EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM10EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM09EN_MASK (0x0200u)
#define CSL_CCDC_FMTPGM_VF0_PGM09EN_SHIFT (0x0009u)
#define CSL_CCDC_FMTPGM_VF0_PGM09EN_RESETVAL (0x0000u)

/*----PGM09EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM09EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM09EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM08EN_MASK (0x0100u)
#define CSL_CCDC_FMTPGM_VF0_PGM08EN_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_VF0_PGM08EN_RESETVAL (0x0000u)

/*----PGM08EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM08EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM08EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM07EN_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_VF0_PGM07EN_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_VF0_PGM07EN_RESETVAL (0x0000u)

/*----PGM07EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM07EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM07EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM06EN_MASK (0x0040u)
#define CSL_CCDC_FMTPGM_VF0_PGM06EN_SHIFT (0x0006u)
#define CSL_CCDC_FMTPGM_VF0_PGM06EN_RESETVAL (0x0000u)

/*----PGM06EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM06EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM06EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM05EN_MASK (0x0020u)
#define CSL_CCDC_FMTPGM_VF0_PGM05EN_SHIFT (0x0005u)
#define CSL_CCDC_FMTPGM_VF0_PGM05EN_RESETVAL (0x0000u)

/*----PGM05EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM05EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM05EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM04EN_MASK (0x0010u)
#define CSL_CCDC_FMTPGM_VF0_PGM04EN_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_VF0_PGM04EN_RESETVAL (0x0000u)

/*----PGM04EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM04EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM04EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM03EN_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_VF0_PGM03EN_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_VF0_PGM03EN_RESETVAL (0x0000u)

/*----PGM03EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM03EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM03EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM02EN_MASK (0x0004u)
#define CSL_CCDC_FMTPGM_VF0_PGM02EN_SHIFT (0x0002u)
#define CSL_CCDC_FMTPGM_VF0_PGM02EN_RESETVAL (0x0000u)

/*----PGM02EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM02EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM02EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM01EN_MASK (0x0002u)
#define CSL_CCDC_FMTPGM_VF0_PGM01EN_SHIFT (0x0001u)
#define CSL_CCDC_FMTPGM_VF0_PGM01EN_RESETVAL (0x0000u)

/*----PGM01EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM01EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM01EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_PGM00EN_MASK (0x0001u)
#define CSL_CCDC_FMTPGM_VF0_PGM00EN_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM00EN_RESETVAL (0x0000u)

/*----PGM00EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF0_PGM00EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF0_PGM00EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF0_RESETVAL     (0x0000u)

/* FMTPGM_VF1 */

#define CSL_CCDC_FMTPGM_VF1_PGM31EN_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_VF1_PGM31EN_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_VF1_PGM31EN_RESETVAL (0x0000u)

/*----PGM31EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM31EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM31EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM30EN_MASK (0x4000u)
#define CSL_CCDC_FMTPGM_VF1_PGM30EN_SHIFT (0x000Eu)
#define CSL_CCDC_FMTPGM_VF1_PGM30EN_RESETVAL (0x0000u)

/*----PGM30EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM30EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM30EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM29EN_MASK (0x2000u)
#define CSL_CCDC_FMTPGM_VF1_PGM29EN_SHIFT (0x000Du)
#define CSL_CCDC_FMTPGM_VF1_PGM29EN_RESETVAL (0x0000u)

/*----PGM29EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM29EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM29EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM28EN_MASK (0x1000u)
#define CSL_CCDC_FMTPGM_VF1_PGM28EN_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_VF1_PGM28EN_RESETVAL (0x0000u)

/*----PGM28EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM28EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM28EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM27EN_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_VF1_PGM27EN_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_VF1_PGM27EN_RESETVAL (0x0000u)

/*----PGM27EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM27EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM27EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM26EN_MASK (0x0400u)
#define CSL_CCDC_FMTPGM_VF1_PGM26EN_SHIFT (0x000Au)
#define CSL_CCDC_FMTPGM_VF1_PGM26EN_RESETVAL (0x0000u)

/*----PGM26EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM26EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM26EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM25EN_MASK (0x0200u)
#define CSL_CCDC_FMTPGM_VF1_PGM25EN_SHIFT (0x0009u)
#define CSL_CCDC_FMTPGM_VF1_PGM25EN_RESETVAL (0x0000u)

/*----PGM25EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM25EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM25EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM24EN_MASK (0x0100u)
#define CSL_CCDC_FMTPGM_VF1_PGM24EN_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_VF1_PGM24EN_RESETVAL (0x0000u)

/*----PGM24EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM24EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM24EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM23EN_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_VF1_PGM23EN_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_VF1_PGM23EN_RESETVAL (0x0000u)

/*----PGM23EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM23EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM23EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM22EN_MASK (0x0040u)
#define CSL_CCDC_FMTPGM_VF1_PGM22EN_SHIFT (0x0006u)
#define CSL_CCDC_FMTPGM_VF1_PGM22EN_RESETVAL (0x0000u)

/*----PGM22EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM22EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM22EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM21EN_MASK (0x0020u)
#define CSL_CCDC_FMTPGM_VF1_PGM21EN_SHIFT (0x0005u)
#define CSL_CCDC_FMTPGM_VF1_PGM21EN_RESETVAL (0x0000u)

/*----PGM21EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM21EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM21EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM20EN_MASK (0x0010u)
#define CSL_CCDC_FMTPGM_VF1_PGM20EN_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_VF1_PGM20EN_RESETVAL (0x0000u)

/*----PGM20EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM20EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM20EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM19EN_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_VF1_PGM19EN_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_VF1_PGM19EN_RESETVAL (0x0000u)

/*----PGM19EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM19EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM19EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM18EN_MASK (0x0004u)
#define CSL_CCDC_FMTPGM_VF1_PGM18EN_SHIFT (0x0002u)
#define CSL_CCDC_FMTPGM_VF1_PGM18EN_RESETVAL (0x0000u)

/*----PGM18EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM18EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM18EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM17EN_MASK (0x0002u)
#define CSL_CCDC_FMTPGM_VF1_PGM17EN_SHIFT (0x0001u)
#define CSL_CCDC_FMTPGM_VF1_PGM17EN_RESETVAL (0x0000u)

/*----PGM17EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM17EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM17EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_PGM16EN_MASK (0x0001u)
#define CSL_CCDC_FMTPGM_VF1_PGM16EN_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM16EN_RESETVAL (0x0000u)

/*----PGM16EN Tokens----*/
#define CSL_CCDC_FMTPGM_VF1_PGM16EN_SKIP_PIXEL (0x0000u)
#define CSL_CCDC_FMTPGM_VF1_PGM16EN_VALID_PIXEL (0x0001u)

#define CSL_CCDC_FMTPGM_VF1_RESETVAL     (0x0000u)

/* FMTPGM_AP0 */

#define CSL_CCDC_FMTPGM_AP0_PGM03UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP0_PGM03UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP0_PGM03UPDT_RESETVAL (0x0000u)

/*----PGM03UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP0_PGM03UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP0_PGM03UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP0_PGM03APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP0_PGM03APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP0_PGM03APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP0_PGM02UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP0_PGM02UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP0_PGM02UPDT_RESETVAL (0x0000u)

/*----PGM02UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP0_PGM02UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP0_PGM02UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP0_PGM02APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP0_PGM02APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP0_PGM02APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP0_PGM01UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP0_PGM01UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP0_PGM01UPDT_RESETVAL (0x0000u)

/*----PGM01UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP0_PGM01UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP0_PGM01UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP0_PGM01APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP0_PGM01APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP0_PGM01APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP0_PGM00UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP0_PGM00UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP0_PGM00UPDT_RESETVAL (0x0000u)

/*----PGM00UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP0_PGM00UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP0_PGM00UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP0_PGM00APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP0_PGM00APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP0_PGM00APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP0_RESETVAL     (0x0000u)

/* FMTPGM_AP1 */

#define CSL_CCDC_FMTPGM_AP1_PGM07UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP1_PGM07UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP1_PGM07UPDT_RESETVAL (0x0000u)

/*----PGM07UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP1_PGM07UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP1_PGM07UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP1_PGM07APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP1_PGM07APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP1_PGM07APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP1_PGM06UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP1_PGM06UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP1_PGM06UPDT_RESETVAL (0x0000u)

/*----PGM06UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP1_PGM06UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP1_PGM06UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP1_PGM06APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP1_PGM06APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP1_PGM06APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP1_PGM05UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP1_PGM05UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP1_PGM05UPDT_RESETVAL (0x0000u)

/*----PGM05UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP1_PGM05UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP1_PGM05UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP1_PGM05APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP1_PGM05APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP1_PGM05APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP1_PGM04UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP1_PGM04UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP1_PGM04UPDT_RESETVAL (0x0000u)

/*----PGM04UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP1_PGM04UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP1_PGM04UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP1_PGM04APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP1_PGM04APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP1_PGM04APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP1_RESETVAL     (0x0000u)

/* FMTPGM_AP2 */

#define CSL_CCDC_FMTPGM_AP2_PGM11UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP2_PGM11UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP2_PGM11UPDT_RESETVAL (0x0000u)

/*----PGM11UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP2_PGM11UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP2_PGM11UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP2_PGM11APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP2_PGM11APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP2_PGM11APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP2_PGM10UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP2_PGM10UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP2_PGM10UPDT_RESETVAL (0x0000u)

/*----PGM10UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP2_PGM10UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP2_PGM10UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP2_PGM10APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP2_PGM10APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP2_PGM10APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP2_PGM09UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP2_PGM09UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP2_PGM09UPDT_RESETVAL (0x0000u)

/*----PGM09UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP2_PGM09UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP2_PGM09UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP2_PGM09APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP2_PGM09APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP2_PGM09APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP2_PGM08UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP2_PGM08UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP2_PGM08UPDT_RESETVAL (0x0000u)

/*----PGM08UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP2_PGM08UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP2_PGM08UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP2_PGM08APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP2_PGM08APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP2_PGM08APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP2_RESETVAL     (0x0000u)

/* FMTPGM_AP3 */

#define CSL_CCDC_FMTPGM_AP3_PGM15UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP3_PGM15UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP3_PGM15UPDT_RESETVAL (0x0000u)

/*----PGM15UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP3_PGM15UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP3_PGM15UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP3_PGM15APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP3_PGM15APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP3_PGM15APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP3_PGM14UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP3_PGM14UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP3_PGM14UPDT_RESETVAL (0x0000u)

/*----PGM14UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP3_PGM14UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP3_PGM14UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP3_PGM14APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP3_PGM14APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP3_PGM14APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP3_PGM13UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP3_PGM13UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP3_PGM13UPDT_RESETVAL (0x0000u)

/*----PGM13UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP3_PGM13UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP3_PGM13UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP3_PGM13APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP3_PGM13APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP3_PGM13APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP3_PGM12UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP3_PGM12UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP3_PGM12UPDT_RESETVAL (0x0000u)

/*----PGM12UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP3_PGM12UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP3_PGM12UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP3_PGM12APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP3_PGM12APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP3_PGM12APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP3_RESETVAL     (0x0000u)

/* FMTPGM_AP4 */

#define CSL_CCDC_FMTPGM_AP4_PGM19UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP4_PGM19UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP4_PGM19UPDT_RESETVAL (0x0000u)

/*----PGM19UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP4_PGM19UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP4_PGM19UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP4_PGM19APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP4_PGM19APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP4_PGM19APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP4_PGM18UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP4_PGM18UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP4_PGM18UPDT_RESETVAL (0x0000u)

/*----PGM18UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP4_PGM18UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP4_PGM18UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP4_PGM18APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP4_PGM18APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP4_PGM18APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP4_PGM17UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP4_PGM17UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP4_PGM17UPDT_RESETVAL (0x0000u)

/*----PGM17UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP4_PGM17UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP4_PGM17UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP4_PGM17APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP4_PGM17APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP4_PGM17APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP4_PGM16UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP4_PGM16UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP4_PGM16UPDT_RESETVAL (0x0000u)

/*----PGM16UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP4_PGM16UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP4_PGM16UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP4_PGM16APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP4_PGM16APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP4_PGM16APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP4_RESETVAL     (0x0000u)

/* FMTPGM_AP5 */

#define CSL_CCDC_FMTPGM_AP5_PGM23UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP5_PGM23UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP5_PGM23UPDT_RESETVAL (0x0000u)

/*----PGM23UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP5_PGM23UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP5_PGM23UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP5_PGM23APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP5_PGM23APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP5_PGM23APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP5_PGM22UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP5_PGM22UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP5_PGM22UPDT_RESETVAL (0x0000u)

/*----PGM22UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP5_PGM22UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP5_PGM22UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP5_PGM22APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP5_PGM22APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP5_PGM22APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP5_PGM21UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP5_PGM21UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP5_PGM21UPDT_RESETVAL (0x0000u)

/*----PGM21UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP5_PGM21UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP5_PGM21UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP5_PGM21APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP5_PGM21APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP5_PGM21APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP5_PGM20UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP5_PGM20UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP5_PGM20UPDT_RESETVAL (0x0000u)

/*----PGM20UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP5_PGM20UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP5_PGM20UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP5_PGM20APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP5_PGM20APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP5_PGM20APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP5_RESETVAL     (0x0000u)

/* FMTPGM_AP6 */

#define CSL_CCDC_FMTPGM_AP6_PGM27UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP6_PGM27UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP6_PGM27UPDT_RESETVAL (0x0000u)

/*----PGM27UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP6_PGM27UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP6_PGM27UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP6_PGM27APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP6_PGM27APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP6_PGM27APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP6_PGM26UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP6_PGM26UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP6_PGM26UPDT_RESETVAL (0x0000u)

/*----PGM26UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP6_PGM26UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP6_PGM26UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP6_PGM26APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP6_PGM26APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP6_PGM26APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP6_PGM25UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP6_PGM25UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP6_PGM25UPDT_RESETVAL (0x0000u)

/*----PGM25UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP6_PGM25UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP6_PGM25UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP6_PGM25APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP6_PGM25APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP6_PGM25APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP6_PGM24UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP6_PGM24UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP6_PGM24UPDT_RESETVAL (0x0000u)

/*----PGM24UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP6_PGM24UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP6_PGM24UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP6_PGM24APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP6_PGM24APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP6_PGM24APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP6_RESETVAL     (0x0000u)

/* FMTPGM_AP7 */

#define CSL_CCDC_FMTPGM_AP7_PGM31UPDT_MASK (0x8000u)
#define CSL_CCDC_FMTPGM_AP7_PGM31UPDT_SHIFT (0x000Fu)
#define CSL_CCDC_FMTPGM_AP7_PGM31UPDT_RESETVAL (0x0000u)

/*----PGM31UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP7_PGM31UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP7_PGM31UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP7_PGM31APTR_MASK (0x7000u)
#define CSL_CCDC_FMTPGM_AP7_PGM31APTR_SHIFT (0x000Cu)
#define CSL_CCDC_FMTPGM_AP7_PGM31APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP7_PGM30UPDT_MASK (0x0800u)
#define CSL_CCDC_FMTPGM_AP7_PGM30UPDT_SHIFT (0x000Bu)
#define CSL_CCDC_FMTPGM_AP7_PGM30UPDT_RESETVAL (0x0000u)

/*----PGM30UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP7_PGM30UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP7_PGM30UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP7_PGM30APTR_MASK (0x0700u)
#define CSL_CCDC_FMTPGM_AP7_PGM30APTR_SHIFT (0x0008u)
#define CSL_CCDC_FMTPGM_AP7_PGM30APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP7_PGM29UPDT_MASK (0x0080u)
#define CSL_CCDC_FMTPGM_AP7_PGM29UPDT_SHIFT (0x0007u)
#define CSL_CCDC_FMTPGM_AP7_PGM29UPDT_RESETVAL (0x0000u)

/*----PGM29UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP7_PGM29UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP7_PGM29UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP7_PGM29APTR_MASK (0x0070u)
#define CSL_CCDC_FMTPGM_AP7_PGM29APTR_SHIFT (0x0004u)
#define CSL_CCDC_FMTPGM_AP7_PGM29APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP7_PGM28UPDT_MASK (0x0008u)
#define CSL_CCDC_FMTPGM_AP7_PGM28UPDT_SHIFT (0x0003u)
#define CSL_CCDC_FMTPGM_AP7_PGM28UPDT_RESETVAL (0x0000u)

/*----PGM28UPDT Tokens----*/
#define CSL_CCDC_FMTPGM_AP7_PGM28UPDT_INCR (0x0000u)
#define CSL_CCDC_FMTPGM_AP7_PGM28UPDT_DECR (0x0001u)

#define CSL_CCDC_FMTPGM_AP7_PGM28APTR_MASK (0x0007u)
#define CSL_CCDC_FMTPGM_AP7_PGM28APTR_SHIFT (0x0000u)
#define CSL_CCDC_FMTPGM_AP7_PGM28APTR_RESETVAL (0x0000u)

#define CSL_CCDC_FMTPGM_AP7_RESETVAL     (0x0000u)

/* LSCCFG1 */

#define CSL_CCDC_LSCCFG1_GFMODE_MASK     (0x0030u)
#define CSL_CCDC_LSCCFG1_GFMODE_SHIFT    (0x0004u)
#define CSL_CCDC_LSCCFG1_GFMODE_RESETVAL (0x0000u)

/*----GFMODE Tokens----*/
#define CSL_CCDC_LSCCFG1_GFMODE_U8Q8_INTERPOLATION (0x0000u)
#define CSL_CCDC_LSCCFG1_GFMODE_U16Q14_INTERPOLATION (0x0001u)
#define CSL_CCDC_LSCCFG1_GFMODE_RESERVED (0x0002u)
#define CSL_CCDC_LSCCFG1_GFMODE_U16Q14   (0x0003u)

#define CSL_CCDC_LSCCFG1_LSCEN_MASK      (0x0001u)
#define CSL_CCDC_LSCCFG1_LSCEN_SHIFT     (0x0000u)
#define CSL_CCDC_LSCCFG1_LSCEN_RESETVAL  (0x0000u)

/*----LSCEN Tokens----*/
#define CSL_CCDC_LSCCFG1_LSCEN_DISABLE   (0x0000u)
#define CSL_CCDC_LSCCFG1_LSCEN_ENABLE    (0x0001u)

#define CSL_CCDC_LSCCFG1_RESETVAL        (0x0000u)

/* LSCCFG2 */

#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_MASK (0xC000u)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_SHIFT (0x000Eu)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_RESETVAL (0x0000u)

/*----GFTSEL_OPOL Tokens----*/
#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_TABLE1 (0x0000u)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_TABLE2 (0x0001u)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPOL_TABLE3 (0x0002u)

#define CSL_CCDC_LSCCFG2_GFTSEL_EPOL_MASK (0x3000u)
#define CSL_CCDC_LSCCFG2_GFTSEL_EPOL_SHIFT (0x000Cu)
#define CSL_CCDC_LSCCFG2_GFTSEL_EPOL_RESETVAL (0x0000u)

#define CSL_CCDC_LSCCFG2_GFTSEL_OPEL_MASK (0x0C00u)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPEL_SHIFT (0x000Au)
#define CSL_CCDC_LSCCFG2_GFTSEL_OPEL_RESETVAL (0x0000u)

#define CSL_CCDC_LSCCFG2_GFTSEL_EPEL_MASK (0x0300u)
#define CSL_CCDC_LSCCFG2_GFTSEL_EPEL_SHIFT (0x0008u)
#define CSL_CCDC_LSCCFG2_GFTSEL_EPEL_RESETVAL (0x0000u)

#define CSL_CCDC_LSCCFG2_GFTINV_MASK     (0x00F0u)
#define CSL_CCDC_LSCCFG2_GFTINV_SHIFT    (0x0004u)
#define CSL_CCDC_LSCCFG2_GFTINV_RESETVAL (0x0000u)

#define CSL_CCDC_LSCCFG2_GFTSF_MASK      (0x000Eu)
#define CSL_CCDC_LSCCFG2_GFTSF_SHIFT     (0x0001u)
#define CSL_CCDC_LSCCFG2_GFTSF_RESETVAL  (0x0007u)

#define CSL_CCDC_LSCCFG2_RESETVAL        (0x000Eu)

/* LSCH0 */

#define CSL_CCDC_LSCH0_LSCCSW_MASK       (0xC000u)
#define CSL_CCDC_LSCH0_LSCCSW_SHIFT      (0x000Eu)
#define CSL_CCDC_LSCH0_LSCCSW_RESETVAL   (0x0000u)

/*----LSCCSW Tokens----*/
#define CSL_CCDC_LSCH0_LSCCSW_TABLE1     (0x0000u)
#define CSL_CCDC_LSCH0_LSCCSW_TABLE2     (0x0001u)
#define CSL_CCDC_LSCH0_LSCCSW_TABLE3     (0x0002u)

#define CSL_CCDC_LSCH0_LSCH0_MASK        (0x3FFFu)
#define CSL_CCDC_LSCH0_LSCH0_SHIFT       (0x0000u)
#define CSL_CCDC_LSCH0_LSCH0_RESETVAL    (0x0000u)

#define CSL_CCDC_LSCH0_RESETVAL          (0x0000u)

/* LSCV0 */

#define CSL_CCDC_LSCV0_LSCV0_MASK        (0x3FFFu)
#define CSL_CCDC_LSCV0_LSCV0_SHIFT       (0x0000u)
#define CSL_CCDC_LSCV0_LSCV0_RESETVAL    (0x0000u)

#define CSL_CCDC_LSCV0_RESETVAL          (0x0000u)

/* LSCKH */

#define CSL_CCDC_LSCKH_KHR_MASK          (0xFF00u)
#define CSL_CCDC_LSCKH_KHR_SHIFT         (0x0008u)
#define CSL_CCDC_LSCKH_KHR_RESETVAL      (0x0000u)

#define CSL_CCDC_LSCKH_KHL_MASK          (0x00FFu)
#define CSL_CCDC_LSCKH_KHL_SHIFT         (0x0000u)
#define CSL_CCDC_LSCKH_KHL_RESETVAL      (0x0000u)

#define CSL_CCDC_LSCKH_RESETVAL          (0x0000u)

/* LSCKV */

#define CSL_CCDC_LSCKV_KVL_MASK          (0xFF00u)
#define CSL_CCDC_LSCKV_KVL_SHIFT         (0x0008u)
#define CSL_CCDC_LSCKV_KVL_RESETVAL      (0x0000u)

#define CSL_CCDC_LSCKV_KVU_MASK          (0x00FFu)
#define CSL_CCDC_LSCKV_KVU_SHIFT         (0x0000u)
#define CSL_CCDC_LSCKV_KVU_RESETVAL      (0x0000u)

#define CSL_CCDC_LSCKV_RESETVAL          (0x0000u)

/* LSCMEMCTL */

#define CSL_CCDC_LSCMEMCTL_LSCMBSY_MASK  (0x0010u)
#define CSL_CCDC_LSCMEMCTL_LSCMBSY_SHIFT (0x0004u)
#define CSL_CCDC_LSCMEMCTL_LSCMBSY_RESETVAL (0x0000u)

/*----LSCMBSY Tokens----*/
#define CSL_CCDC_LSCMEMCTL_LSCMBSY_NOTBUSY (0x0000u)
#define CSL_CCDC_LSCMEMCTL_LSCMBSY_BUSY  (0x0001u)

#define CSL_CCDC_LSCMEMCTL_LSCMRD_MASK   (0x0008u)
#define CSL_CCDC_LSCMEMCTL_LSCMRD_SHIFT  (0x0003u)
#define CSL_CCDC_LSCMEMCTL_LSCMRD_RESETVAL (0x0000u)

/*----LSCMRD Tokens----*/
#define CSL_CCDC_LSCMEMCTL_LSCMRD_WRITE  (0x0000u)
#define CSL_CCDC_LSCMEMCTL_LSCMRD_READ   (0x0001u)

#define CSL_CCDC_LSCMEMCTL_LSCMARST_MASK (0x0004u)
#define CSL_CCDC_LSCMEMCTL_LSCMARST_SHIFT (0x0002u)
#define CSL_CCDC_LSCMEMCTL_LSCMARST_RESETVAL (0x0000u)

/*----LSCMARST Tokens----*/
#define CSL_CCDC_LSCMEMCTL_LSCMARST_INCR_ADDR (0x0000u)
#define CSL_CCDC_LSCMEMCTL_LSCMARST_CLR_ADDR (0x0001u)

#define CSL_CCDC_LSCMEMCTL_LSCMSL_MASK   (0x0003u)
#define CSL_CCDC_LSCMEMCTL_LSCMSL_SHIFT  (0x0000u)
#define CSL_CCDC_LSCMEMCTL_LSCMSL_RESETVAL (0x0000u)

/*----LSCMSL Tokens----*/
#define CSL_CCDC_LSCMEMCTL_LSCMSL_TABLE1 (0x0000u)
#define CSL_CCDC_LSCMEMCTL_LSCMSL_TABLE2 (0x0001u)
#define CSL_CCDC_LSCMEMCTL_LSCMSL_TABLE3 (0x0002u)
#define CSL_CCDC_LSCMEMCTL_LSCMSL_RESERVED (0x0003u)

#define CSL_CCDC_LSCMEMCTL_RESETVAL      (0x0000u)

/* LSCMEMD */

#define CSL_CCDC_LSCMEMD_LSCMD_MASK      (0xFFFFu)
#define CSL_CCDC_LSCMEMD_LSCMD_SHIFT     (0x0000u)
#define CSL_CCDC_LSCMEMD_LSCMD_RESETVAL  (0x0000u)

#define CSL_CCDC_LSCMEMD_RESETVAL        (0x0000u)

/* LSCMEMQ */

#define CSL_CCDC_LSCMEMQ_LSCMQ_MASK      (0xFFFFu)
#define CSL_CCDC_LSCMEMQ_LSCMQ_SHIFT     (0x0000u)
#define CSL_CCDC_LSCMEMQ_LSCMQ_RESETVAL  (0x0000u)

#define CSL_CCDC_LSCMEMQ_RESETVAL        (0x0000u)

/* DFCCTL */

#define CSL_CCDC_DFCCTL_VDFLSFT_MASK     (0x0700u)
#define CSL_CCDC_DFCCTL_VDFLSFT_SHIFT    (0x0008u)
#define CSL_CCDC_DFCCTL_VDFLSFT_RESETVAL (0x0000u)

#define CSL_CCDC_DFCCTL_VDFCUDA_MASK     (0x0080u)
#define CSL_CCDC_DFCCTL_VDFCUDA_SHIFT    (0x0007u)
#define CSL_CCDC_DFCCTL_VDFCUDA_RESETVAL (0x0000u)

/*----VDFCUDA Tokens----*/
#define CSL_CCDC_DFCCTL_VDFCUDA_WHOLELINECORRECT (0x0000u)
#define CSL_CCDC_DFCCTL_VDFCUDA_UPPERDISABLE (0x0001u)

#define CSL_CCDC_DFCCTL_VDFCSL_MASK      (0x0060u)
#define CSL_CCDC_DFCCTL_VDFCSL_SHIFT     (0x0005u)
#define CSL_CCDC_DFCCTL_VDFCSL_RESETVAL  (0x0000u)

/*----VDFCSL Tokens----*/
#define CSL_CCDC_DFCCTL_VDFCSL_NORMAL    (0x0000u)
#define CSL_CCDC_DFCCTL_VDFCSL_HORZINTERPOLIFSAT (0x0001u)
#define CSL_CCDC_DFCCTL_VDFCSL_HORZINTERPOL (0x0002u)
#define CSL_CCDC_DFCCTL_VDFCSL_RESERVED  (0x0003u)

#define CSL_CCDC_DFCCTL_VDFCEN_MASK      (0x0010u)
#define CSL_CCDC_DFCCTL_VDFCEN_SHIFT     (0x0004u)
#define CSL_CCDC_DFCCTL_VDFCEN_RESETVAL  (0x0000u)

/*----VDFCEN Tokens----*/
#define CSL_CCDC_DFCCTL_VDFCEN_OFF       (0x0000u)
#define CSL_CCDC_DFCCTL_VDFCEN_ON        (0x0001u)

#define CSL_CCDC_DFCCTL_GDFCEN_MASK      (0x0001u)
#define CSL_CCDC_DFCCTL_GDFCEN_SHIFT     (0x0000u)
#define CSL_CCDC_DFCCTL_GDFCEN_RESETVAL  (0x0000u)

/*----GDFCEN Tokens----*/
#define CSL_CCDC_DFCCTL_GDFCEN_OFF       (0x0000u)
#define CSL_CCDC_DFCCTL_GDFCEN_ON        (0x0001u)

#define CSL_CCDC_DFCCTL_RESETVAL         (0x0000u)

/* DFCVSAT */

#define CSL_CCDC_DFCVSAT_VDFSLV_MASK     (0x3FFFu)
#define CSL_CCDC_DFCVSAT_VDFSLV_SHIFT    (0x0000u)
#define CSL_CCDC_DFCVSAT_VDFSLV_RESETVAL (0x0000u)

#define CSL_CCDC_DFCVSAT_RESETVAL        (0x0000u)

/* DFCMEMCTL */

#define CSL_CCDC_DFCMEMCTL_DFCMCLR_MASK  (0x0010u)
#define CSL_CCDC_DFCMEMCTL_DFCMCLR_SHIFT (0x0004u)
#define CSL_CCDC_DFCMEMCTL_DFCMCLR_RESETVAL (0x0000u)

/*----DFCMCLR Tokens----*/
#define CSL_CCDC_DFCMEMCTL_DFCMCLR_CLEARCOMPLETE (0x0000u)
#define CSL_CCDC_DFCMEMCTL_DFCMCLR_CLEAR (0x0001u)

#define CSL_CCDC_DFCMEMCTL_DFCMARST_MASK (0x0004u)
#define CSL_CCDC_DFCMEMCTL_DFCMARST_SHIFT (0x0002u)
#define CSL_CCDC_DFCMEMCTL_DFCMARST_RESETVAL (0x0000u)

/*----DFCMARST Tokens----*/
#define CSL_CCDC_DFCMEMCTL_DFCMARST_INCR_ADDR (0x0000u)
#define CSL_CCDC_DFCMEMCTL_DFCMARST_CLR_ADDR (0x0001u)

#define CSL_CCDC_DFCMEMCTL_DFCMRD_MASK   (0x0002u)
#define CSL_CCDC_DFCMEMCTL_DFCMRD_SHIFT  (0x0001u)
#define CSL_CCDC_DFCMEMCTL_DFCMRD_RESETVAL (0x0000u)

/*----DFCMRD Tokens----*/
#define CSL_CCDC_DFCMEMCTL_DFCMRD_READCOMPLETE (0x0000u)
#define CSL_CCDC_DFCMEMCTL_DFCMRD_READ   (0x0001u)

#define CSL_CCDC_DFCMEMCTL_DFCMWR_MASK   (0x0001u)
#define CSL_CCDC_DFCMEMCTL_DFCMWR_SHIFT  (0x0000u)
#define CSL_CCDC_DFCMEMCTL_DFCMWR_RESETVAL (0x0000u)

/*----DFCMWR Tokens----*/
#define CSL_CCDC_DFCMEMCTL_DFCMWR_WRITECOMPLETE (0x0000u)
#define CSL_CCDC_DFCMEMCTL_DFCMWR_WRITE  (0x0001u)

#define CSL_CCDC_DFCMEMCTL_RESETVAL      (0x0000u)

/* DFCMEM0 */

#define CSL_CCDC_DFCMEM0_DFCMEM0_MASK    (0x0FFFu)
#define CSL_CCDC_DFCMEM0_DFCMEM0_SHIFT   (0x0000u)
#define CSL_CCDC_DFCMEM0_DFCMEM0_RESETVAL (0x0000u)

#define CSL_CCDC_DFCMEM0_RESETVAL        (0x0000u)

/* DFCMEM1 */

#define CSL_CCDC_DFCMEM1_DFCMEM1_MASK    (0x1FFFu)
#define CSL_CCDC_DFCMEM1_DFCMEM1_SHIFT   (0x0000u)
#define CSL_CCDC_DFCMEM1_DFCMEM1_RESETVAL (0x0000u)

#define CSL_CCDC_DFCMEM1_RESETVAL        (0x0000u)

/* DFCMEM2 */

#define CSL_CCDC_DFCMEM2_DFCMEM2_MASK    (0x00FFu)
#define CSL_CCDC_DFCMEM2_DFCMEM2_SHIFT   (0x0000u)
#define CSL_CCDC_DFCMEM2_DFCMEM2_RESETVAL (0x0000u)

#define CSL_CCDC_DFCMEM2_RESETVAL        (0x0000u)

/* DFCMEM3 */

#define CSL_CCDC_DFCMEM3_DFCMEM3_MASK    (0x00FFu)
#define CSL_CCDC_DFCMEM3_DFCMEM3_SHIFT   (0x0000u)
#define CSL_CCDC_DFCMEM3_DFCMEM3_RESETVAL (0x0000u)

#define CSL_CCDC_DFCMEM3_RESETVAL        (0x0000u)

/* DFCMEM4 */

#define CSL_CCDC_DFCMEM4_DFCMEM4_MASK    (0x00FFu)
#define CSL_CCDC_DFCMEM4_DFCMEM4_SHIFT   (0x0000u)
#define CSL_CCDC_DFCMEM4_DFCMEM4_RESETVAL (0x0000u)

#define CSL_CCDC_DFCMEM4_RESETVAL        (0x0000u)

/* CSCCTL */

#define CSL_CCDC_CSCCTL_CSCEN_MASK       (0x0001u)
#define CSL_CCDC_CSCCTL_CSCEN_SHIFT      (0x0000u)
#define CSL_CCDC_CSCCTL_CSCEN_RESETVAL   (0x0000u)

/*----CSCEN Tokens----*/
#define CSL_CCDC_CSCCTL_CSCEN_DISABLE    (0x0000u)
#define CSL_CCDC_CSCCTL_CSCEN_ENABLE     (0x0001u)

#define CSL_CCDC_CSCCTL_RESETVAL         (0x0000u)

/* CSCM0 */

#define CSL_CCDC_CSCM0_CSCM01_MASK       (0xFF00u)
#define CSL_CCDC_CSCM0_CSCM01_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM0_CSCM01_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM0_CSCM00_MASK       (0x00FFu)
#define CSL_CCDC_CSCM0_CSCM00_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM0_CSCM00_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM0_RESETVAL          (0x0000u)

/* CSCM1 */

#define CSL_CCDC_CSCM1_CSCM03_MASK       (0xFF00u)
#define CSL_CCDC_CSCM1_CSCM03_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM1_CSCM03_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM1_CSCM02_MASK       (0x00FFu)
#define CSL_CCDC_CSCM1_CSCM02_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM1_CSCM02_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM1_RESETVAL          (0x0000u)

/* CSCM2 */

#define CSL_CCDC_CSCM2_CSCM11_MASK       (0xFF00u)
#define CSL_CCDC_CSCM2_CSCM11_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM2_CSCM11_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM2_CSCM10_MASK       (0x00FFu)
#define CSL_CCDC_CSCM2_CSCM10_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM2_CSCM10_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM2_RESETVAL          (0x0000u)

/* CSCM3 */

#define CSL_CCDC_CSCM3_CSCM13_MASK       (0xFF00u)
#define CSL_CCDC_CSCM3_CSCM13_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM3_CSCM13_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM3_CSCM12_MASK       (0x00FFu)
#define CSL_CCDC_CSCM3_CSCM12_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM3_CSCM12_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM3_RESETVAL          (0x0000u)

/* CSCM4 */

#define CSL_CCDC_CSCM4_CSCM21_MASK       (0xFF00u)
#define CSL_CCDC_CSCM4_CSCM21_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM4_CSCM21_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM4_CSCM20_MASK       (0x00FFu)
#define CSL_CCDC_CSCM4_CSCM20_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM4_CSCM20_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM4_RESETVAL          (0x0000u)

/* CSCM5 */

#define CSL_CCDC_CSCM5_CSCM23_MASK       (0xFF00u)
#define CSL_CCDC_CSCM5_CSCM23_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM5_CSCM23_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM5_CSCM22_MASK       (0x00FFu)
#define CSL_CCDC_CSCM5_CSCM22_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM5_CSCM22_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM5_RESETVAL          (0x0000u)

/* CSCM6 */

#define CSL_CCDC_CSCM6_CSCM31_MASK       (0xFF00u)
#define CSL_CCDC_CSCM6_CSCM31_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM6_CSCM31_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM6_CSCM30_MASK       (0x00FFu)
#define CSL_CCDC_CSCM6_CSCM30_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM6_CSCM30_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM6_RESETVAL          (0x0000u)

/* CSCM7 */

#define CSL_CCDC_CSCM7_CSCM33_MASK       (0xFF00u)
#define CSL_CCDC_CSCM7_CSCM33_SHIFT      (0x0008u)
#define CSL_CCDC_CSCM7_CSCM33_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM7_CSCM32_MASK       (0x00FFu)
#define CSL_CCDC_CSCM7_CSCM32_SHIFT      (0x0000u)
#define CSL_CCDC_CSCM7_CSCM32_RESETVAL   (0x0000u)

#define CSL_CCDC_CSCM7_RESETVAL          (0x0000u)

/* TEST */

#define CSL_CCDC_TEST_TEST_MASK          (0x8000u)
#define CSL_CCDC_TEST_TEST_SHIFT         (0x000Fu)
#define CSL_CCDC_TEST_TEST_RESETVAL      (0x0000u)

/*----TEST Tokens----*/
#define CSL_CCDC_TEST_TEST_NORMALMODE    (0x0000u)
#define CSL_CCDC_TEST_TEST_TESTMODE      (0x0001u)

#define CSL_CCDC_TEST_CLKON1_MASK        (0x0002u)
#define CSL_CCDC_TEST_CLKON1_SHIFT       (0x0001u)
#define CSL_CCDC_TEST_CLKON1_RESETVAL    (0x0000u)

/*----CLKON1 Tokens----*/
#define CSL_CCDC_TEST_CLKON1_NORMALMODE  (0x0000u)
#define CSL_CCDC_TEST_CLKON1_CLK1ACTIVE  (0x0001u)

#define CSL_CCDC_TEST_CLKON2_MASK        (0x0001u)
#define CSL_CCDC_TEST_CLKON2_SHIFT       (0x0000u)
#define CSL_CCDC_TEST_CLKON2_RESETVAL    (0x0000u)

/*----CLKON2 Tokens----*/
#define CSL_CCDC_TEST_CLKON2_NORMALMODE  (0x0000u)
#define CSL_CCDC_TEST_CLKON2_CLK2ACTIVE  (0x0001u)

#define CSL_CCDC_TEST_RESETVAL           (0x0000u)

#endif
