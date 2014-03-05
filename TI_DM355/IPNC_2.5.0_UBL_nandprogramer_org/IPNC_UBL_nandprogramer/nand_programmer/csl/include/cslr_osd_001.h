#ifndef _CSLR_OSD_1_H_
#define _CSLR_OSD_1_H_
/*********************************************************************
 * Copyright (C) 2003-2006 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_osd_1.h
 * 
 * \brief This file contains the Register Desciptions for OSD
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 MODE;
    volatile Uint8 RSVD0[2];
    volatile Uint16 VIDWINMD;
    volatile Uint8 RSVD1[2];
    volatile Uint16 OSDWIN0MD;
    volatile Uint8 RSVD2[2];
    volatile Uint16 OSDWIN1MD;
    volatile Uint8 RSVD3[2];
    volatile Uint16 RECTCUR;
    volatile Uint8 RSVD4[2];
    volatile Uint16 RSV0;
    volatile Uint8 RSVD5[2];
    volatile Uint16 VIDWIN0OFST;
    volatile Uint8 RSVD6[2];
    volatile Uint16 VIDWIN1OFST;
    volatile Uint8 RSVD7[2];
    volatile Uint16 OSDWIN0OFST;
    volatile Uint8 RSVD8[2];
    volatile Uint16 OSDWIN1OFST;
    volatile Uint8 RSVD9[2];
    volatile Uint16 VIDWINADH;
    volatile Uint8 RSVD10[2];
    volatile Uint16 VIDWIN0ADL;
    volatile Uint8 RSVD11[2];
    volatile Uint16 VIDWIN1ADL;
    volatile Uint8 RSVD12[2];
    volatile Uint16 OSDWINADH;
    volatile Uint8 RSVD13[2];
    volatile Uint16 OSDWIN0ADL;
    volatile Uint8 RSVD14[2];
    volatile Uint16 OSDWIN1ADL;
    volatile Uint8 RSVD15[2];
    volatile Uint16 BASEPX;
    volatile Uint8 RSVD16[2];
    volatile Uint16 BASEPY;
    volatile Uint8 RSVD17[2];
    volatile Uint16 VIDWIN0XP;
    volatile Uint8 RSVD18[2];
    volatile Uint16 VIDWIN0YP;
    volatile Uint8 RSVD19[2];
    volatile Uint16 VIDWIN0XL;
    volatile Uint8 RSVD20[2];
    volatile Uint16 VIDWIN0YL;
    volatile Uint8 RSVD21[2];
    volatile Uint16 VIDWIN1XP;
    volatile Uint8 RSVD22[2];
    volatile Uint16 VIDWIN1YP;
    volatile Uint8 RSVD23[2];
    volatile Uint16 VIDWIN1XL;
    volatile Uint8 RSVD24[2];
    volatile Uint16 VIDWIN1YL;
    volatile Uint8 RSVD25[2];
    volatile Uint16 OSDWIN0XP;
    volatile Uint8 RSVD26[2];
    volatile Uint16 OSDWIN0YP;
    volatile Uint8 RSVD27[2];
    volatile Uint16 OSDWIN0XL;
    volatile Uint8 RSVD28[2];
    volatile Uint16 OSDWIN0YL;
    volatile Uint8 RSVD29[2];
    volatile Uint16 OSDWIN1XP;
    volatile Uint8 RSVD30[2];
    volatile Uint16 OSDWIN1YP;
    volatile Uint8 RSVD31[2];
    volatile Uint16 OSDWIN1XL;
    volatile Uint8 RSVD32[2];
    volatile Uint16 OSDWIN1YL;
    volatile Uint8 RSVD33[2];
    volatile Uint16 CURXP;
    volatile Uint8 RSVD34[2];
    volatile Uint16 CURYP;
    volatile Uint8 RSVD35[2];
    volatile Uint16 CURXL;
    volatile Uint8 RSVD36[2];
    volatile Uint16 CURYL;
    volatile Uint8 RSVD37[2];
    volatile Uint16 RSV1;
    volatile Uint8 RSVD38[2];
    volatile Uint16 RSV2;
    volatile Uint8 RSVD39[2];
    volatile Uint16 W0BMP01;
    volatile Uint8 RSVD40[2];
    volatile Uint16 W0BMP23;
    volatile Uint8 RSVD41[2];
    volatile Uint16 W0BMP45;
    volatile Uint8 RSVD42[2];
    volatile Uint16 W0BMP67;
    volatile Uint8 RSVD43[2];
    volatile Uint16 W0BMP89;
    volatile Uint8 RSVD44[2];
    volatile Uint16 W0BMPAB;
    volatile Uint8 RSVD45[2];
    volatile Uint16 W0BMPCD;
    volatile Uint8 RSVD46[2];
    volatile Uint16 W0BMPEF;
    volatile Uint8 RSVD47[2];
    volatile Uint16 W1BMP01;
    volatile Uint8 RSVD48[2];
    volatile Uint16 W1BMP23;
    volatile Uint8 RSVD49[2];
    volatile Uint16 W1BMP45;
    volatile Uint8 RSVD50[2];
    volatile Uint16 W1BMP67;
    volatile Uint8 RSVD51[2];
    volatile Uint16 W1BMP89;
    volatile Uint8 RSVD52[2];
    volatile Uint16 W1BMPAB;
    volatile Uint8 RSVD53[2];
    volatile Uint16 W1BMPCD;
    volatile Uint8 RSVD54[2];
    volatile Uint16 W1BMPEF;
    volatile Uint8 RSVD55[2];
    volatile Uint16 VBNDRY;
    volatile Uint8 RSVD56[2];
    volatile Uint16 EXTMODE;
    volatile Uint8 RSVD57[2];
    volatile Uint16 MISCCTL;
    volatile Uint8 RSVD58[2];
    volatile Uint16 CLUTRAMYCB;
    volatile Uint8 RSVD59[2];
    volatile Uint16 CLUTRAMCR;
    volatile Uint8 RSVD60[2];
    volatile Uint16 TRANSPVALL;
    volatile Uint8 RSVD61[2];
    volatile Uint16 TRANSPVALU;
    volatile Uint8 RSVD62[2];
    volatile Uint16 TRANSPBMPIDX;
} CSL_OsdRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* MODE */

#define CSL_OSD_MODE_CS_MASK             (0x8000u)
#define CSL_OSD_MODE_CS_SHIFT            (0x000Fu)
#define CSL_OSD_MODE_CS_RESETVAL         (0x0000u)

/*----CS Tokens----*/
#define CSL_OSD_MODE_CS_CB_CR            (0x0000u)
#define CSL_OSD_MODE_CS_CR_CB            (0x0001u)

#define CSL_OSD_MODE_OVRSZ_MASK          (0x4000u)
#define CSL_OSD_MODE_OVRSZ_SHIFT         (0x000Eu)
#define CSL_OSD_MODE_OVRSZ_RESETVAL      (0x0000u)

/*----OVRSZ Tokens----*/
#define CSL_OSD_MODE_OVRSZ_X1            (0x0000u)
#define CSL_OSD_MODE_OVRSZ_X6_5          (0x0001u)

#define CSL_OSD_MODE_OHRSZ_MASK          (0x2000u)
#define CSL_OSD_MODE_OHRSZ_SHIFT         (0x000Du)
#define CSL_OSD_MODE_OHRSZ_RESETVAL      (0x0000u)

/*----OHRSZ Tokens----*/
#define CSL_OSD_MODE_OHRSZ_X1            (0x0000u)
#define CSL_OSD_MODE_OHRSZ_X9_8          (0x0001u)

#define CSL_OSD_MODE_EF_MASK             (0x1000u)
#define CSL_OSD_MODE_EF_SHIFT            (0x000Cu)
#define CSL_OSD_MODE_EF_RESETVAL         (0x0000u)

/*----EF Tokens----*/
#define CSL_OSD_MODE_EF_OFF              (0x0000u)
#define CSL_OSD_MODE_EF_ON               (0x0001u)

#define CSL_OSD_MODE_VVRSZ_MASK          (0x0800u)
#define CSL_OSD_MODE_VVRSZ_SHIFT         (0x000Bu)
#define CSL_OSD_MODE_VVRSZ_RESETVAL      (0x0000u)

/*----VVRSZ Tokens----*/
#define CSL_OSD_MODE_VVRSZ_X1            (0x0000u)
#define CSL_OSD_MODE_VVRSZ_X6_5          (0x0001u)

#define CSL_OSD_MODE_VHRSZ_MASK          (0x0400u)
#define CSL_OSD_MODE_VHRSZ_SHIFT         (0x000Au)
#define CSL_OSD_MODE_VHRSZ_RESETVAL      (0x0000u)

/*----VHRSZ Tokens----*/
#define CSL_OSD_MODE_VHRSZ_X1            (0x0000u)
#define CSL_OSD_MODE_VHRSZ_X9_8          (0x0001u)

#define CSL_OSD_MODE_FSINV_MASK          (0x0200u)
#define CSL_OSD_MODE_FSINV_SHIFT         (0x0009u)
#define CSL_OSD_MODE_FSINV_RESETVAL      (0x0000u)

/*----FSINV Tokens----*/
#define CSL_OSD_MODE_FSINV_UNINVERTED    (0x0000u)
#define CSL_OSD_MODE_FSINV_INVERTED      (0x0001u)

#define CSL_OSD_MODE_BCLUT_MASK          (0x0100u)
#define CSL_OSD_MODE_BCLUT_SHIFT         (0x0008u)
#define CSL_OSD_MODE_BCLUT_RESETVAL      (0x0000u)

/*----BCLUT Tokens----*/
#define CSL_OSD_MODE_BCLUT_ROM           (0x0000u)
#define CSL_OSD_MODE_BCLUT_RAM           (0x0001u)

#define CSL_OSD_MODE_CABG_MASK           (0x00FFu)
#define CSL_OSD_MODE_CABG_SHIFT          (0x0000u)
#define CSL_OSD_MODE_CABG_RESETVAL       (0x0000u)

#define CSL_OSD_MODE_RESETVAL            (0x0000u)

/* VIDWINMD */

#define CSL_OSD_VIDWINMD_VFINV_MASK      (0x8000u)
#define CSL_OSD_VIDWINMD_VFINV_SHIFT     (0x000Fu)
#define CSL_OSD_VIDWINMD_VFINV_RESETVAL  (0x0000u)

/*----VFINV Tokens----*/
#define CSL_OSD_VIDWINMD_VFINV_NORMAL    (0x0000u)
#define CSL_OSD_VIDWINMD_VFINV_INVERSED  (0x0001u)

#define CSL_OSD_VIDWINMD_V1EFC_MASK      (0x4000u)
#define CSL_OSD_VIDWINMD_V1EFC_SHIFT     (0x000Eu)
#define CSL_OSD_VIDWINMD_V1EFC_RESETVAL  (0x0000u)

/*----V1EFC Tokens----*/
#define CSL_OSD_VIDWINMD_V1EFC_SAMECOEFICIENTSFORFIELD_0ANDFIELD_1 (0x0000u)
#define CSL_OSD_VIDWINMD_V1EFC_DIFFERENTCOEFICIENTSFORFIELD_0ANDFIELD_1 (0x0001u)

#define CSL_OSD_VIDWINMD_VHZ1_MASK       (0x3000u)
#define CSL_OSD_VIDWINMD_VHZ1_SHIFT      (0x000Cu)
#define CSL_OSD_VIDWINMD_VHZ1_RESETVAL   (0x0000u)

/*----VHZ1 Tokens----*/
#define CSL_OSD_VIDWINMD_VHZ1_X1         (0x0000u)
#define CSL_OSD_VIDWINMD_VHZ1_X2         (0x0001u)
#define CSL_OSD_VIDWINMD_VHZ1_X4         (0x0002u)
#define CSL_OSD_VIDWINMD_VHZ1_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_VIDWINMD_VVZ1_MASK       (0x0C00u)
#define CSL_OSD_VIDWINMD_VVZ1_SHIFT      (0x000Au)
#define CSL_OSD_VIDWINMD_VVZ1_RESETVAL   (0x0000u)

/*----VVZ1 Tokens----*/
#define CSL_OSD_VIDWINMD_VVZ1_X1         (0x0000u)
#define CSL_OSD_VIDWINMD_VVZ1_X2         (0x0001u)
#define CSL_OSD_VIDWINMD_VVZ1_X4         (0x0002u)
#define CSL_OSD_VIDWINMD_VVZ1_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_VIDWINMD_VFF1_MASK       (0x0200u)
#define CSL_OSD_VIDWINMD_VFF1_SHIFT      (0x0009u)
#define CSL_OSD_VIDWINMD_VFF1_RESETVAL   (0x0000u)

/*----VFF1 Tokens----*/
#define CSL_OSD_VIDWINMD_VFF1_FIELDMODE  (0x0000u)
#define CSL_OSD_VIDWINMD_VFF1_FRAMEMODE  (0x0001u)

#define CSL_OSD_VIDWINMD_ACT1_MASK       (0x0100u)
#define CSL_OSD_VIDWINMD_ACT1_SHIFT      (0x0008u)
#define CSL_OSD_VIDWINMD_ACT1_RESETVAL   (0x0000u)

/*----ACT1 Tokens----*/
#define CSL_OSD_VIDWINMD_ACT1_OFF        (0x0000u)
#define CSL_OSD_VIDWINMD_ACT1_ON         (0x0001u)

#define CSL_OSD_VIDWINMD_V0EFC_MASK      (0x0040u)
#define CSL_OSD_VIDWINMD_V0EFC_SHIFT     (0x0006u)
#define CSL_OSD_VIDWINMD_V0EFC_RESETVAL  (0x0000u)

/*----V0EFC Tokens----*/
#define CSL_OSD_VIDWINMD_V0EFC_SAMECO_EFICIENTSFORFIELD_0ANDFIELD_1 (0x0000u)
#define CSL_OSD_VIDWINMD_V0EFC_DIFFERENTCO_EFICIENTSFORFIELD_0ANDFIELD_1 (0x0001u)

#define CSL_OSD_VIDWINMD_VHZ0_MASK       (0x0030u)
#define CSL_OSD_VIDWINMD_VHZ0_SHIFT      (0x0004u)
#define CSL_OSD_VIDWINMD_VHZ0_RESETVAL   (0x0000u)

/*----VHZ0 Tokens----*/
#define CSL_OSD_VIDWINMD_VHZ0_X1         (0x0000u)
#define CSL_OSD_VIDWINMD_VHZ0_X2         (0x0001u)
#define CSL_OSD_VIDWINMD_VHZ0_X4         (0x0002u)
#define CSL_OSD_VIDWINMD_VHZ0_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_VIDWINMD_VVZ0_MASK       (0x000Cu)
#define CSL_OSD_VIDWINMD_VVZ0_SHIFT      (0x0002u)
#define CSL_OSD_VIDWINMD_VVZ0_RESETVAL   (0x0000u)

/*----VVZ0 Tokens----*/
#define CSL_OSD_VIDWINMD_VVZ0_X1         (0x0000u)
#define CSL_OSD_VIDWINMD_VVZ0_X2         (0x0001u)
#define CSL_OSD_VIDWINMD_VVZ0_X4         (0x0002u)
#define CSL_OSD_VIDWINMD_VVZ0_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_VIDWINMD_VFF0_MASK       (0x0002u)
#define CSL_OSD_VIDWINMD_VFF0_SHIFT      (0x0001u)
#define CSL_OSD_VIDWINMD_VFF0_RESETVAL   (0x0000u)

/*----VFF0 Tokens----*/
#define CSL_OSD_VIDWINMD_VFF0_FIELDMODE  (0x0000u)
#define CSL_OSD_VIDWINMD_VFF0_FRAMEMODE  (0x0001u)

#define CSL_OSD_VIDWINMD_ACT0_MASK       (0x0001u)
#define CSL_OSD_VIDWINMD_ACT0_SHIFT      (0x0000u)
#define CSL_OSD_VIDWINMD_ACT0_RESETVAL   (0x0000u)

/*----ACT0 Tokens----*/
#define CSL_OSD_VIDWINMD_ACT0_OFF        (0x0000u)
#define CSL_OSD_VIDWINMD_ACT0_ON         (0x0001u)

#define CSL_OSD_VIDWINMD_RESETVAL        (0x0000u)

/* OSDWIN0MD */

#define CSL_OSD_OSDWIN0MD_BMP0MD_MASK    (0x6000u)
#define CSL_OSD_OSDWIN0MD_BMP0MD_SHIFT   (0x000Du)
#define CSL_OSD_OSDWIN0MD_BMP0MD_RESETVAL (0x0000u)

/*----BMP0MD Tokens----*/
#define CSL_OSD_OSDWIN0MD_BMP0MD_BITMAP  (0x0000u)
#define CSL_OSD_OSDWIN0MD_BMP0MD_RGB16   (0x0001u)
#define CSL_OSD_OSDWIN0MD_BMP0MD_RGB24   (0x000Au)
#define CSL_OSD_OSDWIN0MD_BMP0MD_YC      (0x000Bu)

#define CSL_OSD_OSDWIN0MD_CLUTS0_MASK    (0x1000u)
#define CSL_OSD_OSDWIN0MD_CLUTS0_SHIFT   (0x000Cu)
#define CSL_OSD_OSDWIN0MD_CLUTS0_RESETVAL (0x0000u)

/*----CLUTS0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_CLUTS0_ROM_LOOK_UPTABLE (0x0000u)
#define CSL_OSD_OSDWIN0MD_CLUTS0_RAM_LOOK_UPTABLE (0x0001u)

#define CSL_OSD_OSDWIN0MD_OHZ0_MASK      (0x0C00u)
#define CSL_OSD_OSDWIN0MD_OHZ0_SHIFT     (0x000Au)
#define CSL_OSD_OSDWIN0MD_OHZ0_RESETVAL  (0x0000u)

/*----OHZ0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_OHZ0_X1        (0x0000u)
#define CSL_OSD_OSDWIN0MD_OHZ0_X2        (0x0001u)
#define CSL_OSD_OSDWIN0MD_OHZ0_X4        (0x0002u)
#define CSL_OSD_OSDWIN0MD_OHZ0_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_OSDWIN0MD_OVZ0_MASK      (0x0300u)
#define CSL_OSD_OSDWIN0MD_OVZ0_SHIFT     (0x0008u)
#define CSL_OSD_OSDWIN0MD_OVZ0_RESETVAL  (0x0000u)

/*----OVZ0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_OVZ0_X1        (0x0000u)
#define CSL_OSD_OSDWIN0MD_OVZ0_X2        (0x0001u)
#define CSL_OSD_OSDWIN0MD_OVZ0_X4        (0x0002u)
#define CSL_OSD_OSDWIN0MD_OVZ0_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_OSDWIN0MD_BMW0_MASK      (0x00C0u)
#define CSL_OSD_OSDWIN0MD_BMW0_SHIFT     (0x0006u)
#define CSL_OSD_OSDWIN0MD_BMW0_RESETVAL  (0x0000u)

/*----BMW0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_BMW0_1         (0x0000u)
#define CSL_OSD_OSDWIN0MD_BMW0_2         (0x0001u)
#define CSL_OSD_OSDWIN0MD_BMW0_4         (0x0002u)
#define CSL_OSD_OSDWIN0MD_BMW0_8         (0x0003u)

#define CSL_OSD_OSDWIN0MD_BLND0_MASK     (0x0038u)
#define CSL_OSD_OSDWIN0MD_BLND0_SHIFT    (0x0003u)
#define CSL_OSD_OSDWIN0MD_BLND0_RESETVAL (0x0000u)

/*----BLND0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_0_VIDEO_1 (0x0000u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_1_8_VIDEO_7_8 (0x0001u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_2_8_VIDEO_6_8 (0x0002u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_3_8_VIDEO_5_8 (0x0003u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_4_8_VIDEO_4_8 (0x0004u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_5_8_VIDEO_3_8 (0x0005u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_6_8_VIDEO_2_8 (0x0006u)
#define CSL_OSD_OSDWIN0MD_BLND0_OSD_1_VIDEO_0 (0x0007u)

#define CSL_OSD_OSDWIN0MD_TE0_MASK       (0x0004u)
#define CSL_OSD_OSDWIN0MD_TE0_SHIFT      (0x0002u)
#define CSL_OSD_OSDWIN0MD_TE0_RESETVAL   (0x0000u)

/*----TE0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_TE0_DISABLE    (0x0000u)
#define CSL_OSD_OSDWIN0MD_TE0_ENABLE     (0x0001u)

#define CSL_OSD_OSDWIN0MD_OFF0_MASK      (0x0002u)
#define CSL_OSD_OSDWIN0MD_OFF0_SHIFT     (0x0001u)
#define CSL_OSD_OSDWIN0MD_OFF0_RESETVAL  (0x0000u)

/*----OFF0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_OFF0_FIELDMODE (0x0000u)
#define CSL_OSD_OSDWIN0MD_OFF0_FRAMEMODE (0x0001u)

#define CSL_OSD_OSDWIN0MD_OACT0_MASK     (0x0001u)
#define CSL_OSD_OSDWIN0MD_OACT0_SHIFT    (0x0000u)
#define CSL_OSD_OSDWIN0MD_OACT0_RESETVAL (0x0000u)

/*----OACT0 Tokens----*/
#define CSL_OSD_OSDWIN0MD_OACT0_OFF      (0x0000u)
#define CSL_OSD_OSDWIN0MD_OACT0_ON       (0x0001u)

#define CSL_OSD_OSDWIN0MD_RESETVAL       (0x0000u)

/* OSDWIN1MD */

#define CSL_OSD_OSDWIN1MD_OASW_MASK      (0x8000u)
#define CSL_OSD_OSDWIN1MD_OASW_SHIFT     (0x000Fu)
#define CSL_OSD_OSDWIN1MD_OASW_RESETVAL  (0x0000u)

/*----OASW Tokens----*/
#define CSL_OSD_OSDWIN1MD_OASW_OSDWINDOW1 (0x0000u)
#define CSL_OSD_OSDWIN1MD_OASW_ATTRIBUTEWINDOW (0x0001u)

#define CSL_OSD_OSDWIN1MD_BMP1MD_MASK    (0x6000u)
#define CSL_OSD_OSDWIN1MD_BMP1MD_SHIFT   (0x000Du)
#define CSL_OSD_OSDWIN1MD_BMP1MD_RESETVAL (0x0000u)

/*----BMP1MD Tokens----*/
#define CSL_OSD_OSDWIN1MD_BMP1MD_BITMAP  (0x0000u)
#define CSL_OSD_OSDWIN1MD_BMP1MD_RGB16   (0x0001u)
#define CSL_OSD_OSDWIN1MD_BMP1MD_RGB24   (0x000Au)
#define CSL_OSD_OSDWIN1MD_BMP1MD_YC      (0x000Bu)
#define CSL_OSD_OSDWIN1MD_BMP1MD_BITMAP  (0x0000u)
#define CSL_OSD_OSDWIN1MD_BMP1MD_ATTENUATED_Y_16_235_CR_16_240_CB_16_240_ (0x0001u)

#define CSL_OSD_OSDWIN1MD_CLUTS1_MASK    (0x1000u)
#define CSL_OSD_OSDWIN1MD_CLUTS1_SHIFT   (0x000Cu)
#define CSL_OSD_OSDWIN1MD_CLUTS1_RESETVAL (0x0000u)

/*----CLUTS1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_CLUTS1_ROM_LOOK_UPTABLE (0x0000u)
#define CSL_OSD_OSDWIN1MD_CLUTS1_RAM_LOOK_UPTABLE (0x0001u)

#define CSL_OSD_OSDWIN1MD_OHZ1_MASK      (0x0C00u)
#define CSL_OSD_OSDWIN1MD_OHZ1_SHIFT     (0x000Au)
#define CSL_OSD_OSDWIN1MD_OHZ1_RESETVAL  (0x0000u)

/*----OHZ1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_OHZ1_X1        (0x0000u)
#define CSL_OSD_OSDWIN1MD_OHZ1_X2        (0x0001u)
#define CSL_OSD_OSDWIN1MD_OHZ1_X4        (0x0002u)
#define CSL_OSD_OSDWIN1MD_OHZ1_RESERVED  (0x0003u)

#define CSL_OSD_OSDWIN1MD_OVZ1_MASK      (0x0300u)
#define CSL_OSD_OSDWIN1MD_OVZ1_SHIFT     (0x0008u)
#define CSL_OSD_OSDWIN1MD_OVZ1_RESETVAL  (0x0000u)

/*----OVZ1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_OVZ1_X1        (0x0000u)
#define CSL_OSD_OSDWIN1MD_OVZ1_X2        (0x0001u)
#define CSL_OSD_OSDWIN1MD_OVZ1_X4        (0x0002u)
#define CSL_OSD_OSDWIN1MD_OVZ1_RESERVED  (0x0003u)

#define CSL_OSD_OSDWIN1MD_BMW1_MASK      (0x00C0u)
#define CSL_OSD_OSDWIN1MD_BMW1_SHIFT     (0x0006u)
#define CSL_OSD_OSDWIN1MD_BMW1_RESETVAL  (0x0000u)

/*----BMW1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_BMW1_1_BIT     (0x0000u)
#define CSL_OSD_OSDWIN1MD_BMW1_2_BITS    (0x0001u)
#define CSL_OSD_OSDWIN1MD_BMW1_4_BITS    (0x0002u)
#define CSL_OSD_OSDWIN1MD_BMW1_8_BITS    (0x0003u)

#define CSL_OSD_OSDWIN1MD_BLND1_MASK     (0x0038u)
#define CSL_OSD_OSDWIN1MD_BLND1_SHIFT    (0x0003u)
#define CSL_OSD_OSDWIN1MD_BLND1_RESETVAL (0x0000u)

/*----BLND1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_0_VIDEO_1 (0x0000u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_1_8_VIDEO_7_8 (0x0001u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_2_8_VIDEO_6_8 (0x0002u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_3_8_VIDEO_5_8 (0x0003u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_4_8_VIDEO_4_8 (0x0004u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_5_8_VIDEO_3_8 (0x0005u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_6_8_VIDEO_2_8 (0x0006u)
#define CSL_OSD_OSDWIN1MD_BLND1_OSD_1_VIDEO_0 (0x0007u)

#define CSL_OSD_OSDWIN1MD_TE1_MASK       (0x0004u)
#define CSL_OSD_OSDWIN1MD_TE1_SHIFT      (0x0002u)
#define CSL_OSD_OSDWIN1MD_TE1_RESETVAL   (0x0000u)

/*----TE1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_TE1_DISABLE    (0x0000u)
#define CSL_OSD_OSDWIN1MD_TE1_ENABLE     (0x0001u)

#define CSL_OSD_OSDWIN1MD_OFF1_MASK      (0x0002u)
#define CSL_OSD_OSDWIN1MD_OFF1_SHIFT     (0x0001u)
#define CSL_OSD_OSDWIN1MD_OFF1_RESETVAL  (0x0000u)

/*----OFF1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_OFF1_FIELDMODE (0x0000u)
#define CSL_OSD_OSDWIN1MD_OFF1_FRAMEMODE (0x0001u)

#define CSL_OSD_OSDWIN1MD_OACT1_MASK     (0x0001u)
#define CSL_OSD_OSDWIN1MD_OACT1_SHIFT    (0x0000u)
#define CSL_OSD_OSDWIN1MD_OACT1_RESETVAL (0x0000u)

/*----OACT1 Tokens----*/
#define CSL_OSD_OSDWIN1MD_OACT1_OFF      (0x0000u)
#define CSL_OSD_OSDWIN1MD_OACT1_ON       (0x0001u)

#define CSL_OSD_OSDWIN1MD_RESETVAL       (0x0000u)

/* OSDATRMD */

#define CSL_OSD_OSDATRMD_OASW_MASK       (0x8000u)
#define CSL_OSD_OSDATRMD_OASW_SHIFT      (0x000Fu)
#define CSL_OSD_OSDATRMD_OASW_RESETVAL   (0x0000u)

/*----OASW Tokens----*/
#define CSL_OSD_OSDATRMD_OASW_OSDWINDOW1 (0x0000u)
#define CSL_OSD_OSDATRMD_OASW_ATTRIBUTEWINDOW (0x0001u)

#define CSL_OSD_OSDATRMD_OHZA_MASK       (0x0C00u)
#define CSL_OSD_OSDATRMD_OHZA_SHIFT      (0x000Au)
#define CSL_OSD_OSDATRMD_OHZA_RESETVAL   (0x0000u)

/*----OHZA Tokens----*/
#define CSL_OSD_OSDATRMD_OHZA_X1         (0x0000u)
#define CSL_OSD_OSDATRMD_OHZA_X2         (0x0001u)
#define CSL_OSD_OSDATRMD_OHZA_X4         (0x0002u)
#define CSL_OSD_OSDATRMD_OHZA_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_OSDATRMD_OVZA_MASK       (0x0300u)
#define CSL_OSD_OSDATRMD_OVZA_SHIFT      (0x0008u)
#define CSL_OSD_OSDATRMD_OVZA_RESETVAL   (0x0000u)

/*----OVZA Tokens----*/
#define CSL_OSD_OSDATRMD_OVZA_X1         (0x0000u)
#define CSL_OSD_OSDATRMD_OVZA_X2         (0x0001u)
#define CSL_OSD_OSDATRMD_OVZA_X4         (0x0002u)
#define CSL_OSD_OSDATRMD_OVZA_RESERVED_SAMEAS_00__ (0x0003u)

#define CSL_OSD_OSDATRMD_BLNKINT_MASK    (0x00C0u)
#define CSL_OSD_OSDATRMD_BLNKINT_SHIFT   (0x0006u)
#define CSL_OSD_OSDATRMD_BLNKINT_RESETVAL (0x0000u)

/*----BLNKINT Tokens----*/
#define CSL_OSD_OSDATRMD_BLNKINT_1_UNIT  (0x0000u)
#define CSL_OSD_OSDATRMD_BLNKINT_2_UNITS (0x0001u)
#define CSL_OSD_OSDATRMD_BLNKINT_3_UNITS (0x0002u)
#define CSL_OSD_OSDATRMD_BLNKINT_4_UNITS (0x0003u)

#define CSL_OSD_OSDATRMD_OFFA_MASK       (0x0002u)
#define CSL_OSD_OSDATRMD_OFFA_SHIFT      (0x0001u)
#define CSL_OSD_OSDATRMD_OFFA_RESETVAL   (0x0000u)

/*----OFFA Tokens----*/
#define CSL_OSD_OSDATRMD_OFFA_FIELDMODE  (0x0000u)
#define CSL_OSD_OSDATRMD_OFFA_FRAMEMODE  (0x0001u)

#define CSL_OSD_OSDATRMD_BLNK_MASK       (0x0001u)
#define CSL_OSD_OSDATRMD_BLNK_SHIFT      (0x0000u)
#define CSL_OSD_OSDATRMD_BLNK_RESETVAL   (0x0000u)

/*----BLNK Tokens----*/
#define CSL_OSD_OSDATRMD_BLNK_DISABLE    (0x0000u)
#define CSL_OSD_OSDATRMD_BLNK_ENABLE     (0x0001u)

#define CSL_OSD_OSDATRMD_RESETVAL        (0x0000u)

/* RECTCUR */

#define CSL_OSD_RECTCUR_RCAD_MASK        (0xFF00u)
#define CSL_OSD_RECTCUR_RCAD_SHIFT       (0x0008u)
#define CSL_OSD_RECTCUR_RCAD_RESETVAL    (0x0000u)

#define CSL_OSD_RECTCUR_CLUTSR_MASK      (0x0080u)
#define CSL_OSD_RECTCUR_CLUTSR_SHIFT     (0x0007u)
#define CSL_OSD_RECTCUR_CLUTSR_RESETVAL  (0x0000u)

/*----CLUTSR Tokens----*/
#define CSL_OSD_RECTCUR_CLUTSR_ROM_LOOK_UPTABLE (0x0000u)
#define CSL_OSD_RECTCUR_CLUTSR_RAM_LOOK_UPTABLE (0x0001u)

#define CSL_OSD_RECTCUR_RCHW_MASK        (0x0070u)
#define CSL_OSD_RECTCUR_RCHW_SHIFT       (0x0004u)
#define CSL_OSD_RECTCUR_RCHW_RESETVAL    (0x0000u)

#define CSL_OSD_RECTCUR_RCVW_MASK        (0x000Eu)
#define CSL_OSD_RECTCUR_RCVW_SHIFT       (0x0001u)
#define CSL_OSD_RECTCUR_RCVW_RESETVAL    (0x0000u)

#define CSL_OSD_RECTCUR_RCACT_MASK       (0x0001u)
#define CSL_OSD_RECTCUR_RCACT_SHIFT      (0x0000u)
#define CSL_OSD_RECTCUR_RCACT_RESETVAL   (0x0000u)

/*----RCACT Tokens----*/
#define CSL_OSD_RECTCUR_RCACT_OFF        (0x0000u)
#define CSL_OSD_RECTCUR_RCACT_ON         (0x0001u)

#define CSL_OSD_RECTCUR_RESETVAL         (0x0000u)

/* RSV0 */

#define CSL_OSD_RSV0_RSV0_MASK           (0xFFFFu)
#define CSL_OSD_RSV0_RSV0_SHIFT          (0x0000u)
#define CSL_OSD_RSV0_RSV0_RESETVAL       (0x0000u)

#define CSL_OSD_RSV0_RESETVAL            (0x0000u)

/* VIDWIN0OFST */

#define CSL_OSD_VIDWIN0OFST_V0LO_MASK    (0x01FFu)
#define CSL_OSD_VIDWIN0OFST_V0LO_SHIFT   (0x0000u)
#define CSL_OSD_VIDWIN0OFST_V0LO_RESETVAL (0x0000u)

#define CSL_OSD_VIDWIN0OFST_RESETVAL     (0x0000u)

/* VIDWIN1OFST */

#define CSL_OSD_VIDWIN1OFST_V1LO_MASK    (0x01FFu)
#define CSL_OSD_VIDWIN1OFST_V1LO_SHIFT   (0x0000u)
#define CSL_OSD_VIDWIN1OFST_V1LO_RESETVAL (0x0000u)

#define CSL_OSD_VIDWIN1OFST_RESETVAL     (0x0000u)

/* OSDWIN0OFST */

#define CSL_OSD_OSDWIN0OFST_O0LO_MASK    (0x01FFu)
#define CSL_OSD_OSDWIN0OFST_O0LO_SHIFT   (0x0000u)
#define CSL_OSD_OSDWIN0OFST_O0LO_RESETVAL (0x0000u)

#define CSL_OSD_OSDWIN0OFST_RESETVAL     (0x0000u)

/* OSDWIN1OFST */

#define CSL_OSD_OSDWIN1OFST_O1LO_MASK    (0x01FFu)
#define CSL_OSD_OSDWIN1OFST_O1LO_SHIFT   (0x0000u)
#define CSL_OSD_OSDWIN1OFST_O1LO_RESETVAL (0x0000u)

#define CSL_OSD_OSDWIN1OFST_RESETVAL     (0x0000u)

/* VIDWINADH */

#define CSL_OSD_VIDWINADH_V1AH_MASK      (0x7F00u)
#define CSL_OSD_VIDWINADH_V1AH_SHIFT     (0x0008u)
#define CSL_OSD_VIDWINADH_V1AH_RESETVAL  (0x0000u)

#define CSL_OSD_VIDWINADH_V0AH_MASK      (0x007Fu)
#define CSL_OSD_VIDWINADH_V0AH_SHIFT     (0x0000u)
#define CSL_OSD_VIDWINADH_V0AH_RESETVAL  (0x0000u)

#define CSL_OSD_VIDWINADH_RESETVAL       (0x0000u)

/* VIDWIN0ADL */

#define CSL_OSD_VIDWIN0ADL_VIDWIN0ADL_MASK (0xFFFFu)
#define CSL_OSD_VIDWIN0ADL_VIDWIN0ADL_SHIFT (0x0000u)
#define CSL_OSD_VIDWIN0ADL_VIDWIN0ADL_RESETVAL (0x0000u)

#define CSL_OSD_VIDWIN0ADL_RESETVAL      (0x0000u)

/* VIDWIN1ADL */

#define CSL_OSD_VIDWIN1ADL_VIDWIN1ADL_MASK (0xFFFFu)
#define CSL_OSD_VIDWIN1ADL_VIDWIN1ADL_SHIFT (0x0000u)
#define CSL_OSD_VIDWIN1ADL_VIDWIN1ADL_RESETVAL (0x0000u)

#define CSL_OSD_VIDWIN1ADL_RESETVAL      (0x0000u)

/* OSDWINADH */

#define CSL_OSD_OSDWINADH_O1AH_MASK      (0x7F00u)
#define CSL_OSD_OSDWINADH_O1AH_SHIFT     (0x0008u)
#define CSL_OSD_OSDWINADH_O1AH_RESETVAL  (0x0000u)

#define CSL_OSD_OSDWINADH_O0AH_MASK      (0x007Fu)
#define CSL_OSD_OSDWINADH_O0AH_SHIFT     (0x0000u)
#define CSL_OSD_OSDWINADH_O0AH_RESETVAL  (0x0000u)

#define CSL_OSD_OSDWINADH_RESETVAL       (0x0000u)

/* OSDWIN0ADL */

#define CSL_OSD_OSDWIN0ADL_BMPWIN0ADL_MASK (0xFFFFu)
#define CSL_OSD_OSDWIN0ADL_BMPWIN0ADL_SHIFT (0x0000u)
#define CSL_OSD_OSDWIN0ADL_BMPWIN0ADL_RESETVAL (0x0000u)

#define CSL_OSD_OSDWIN0ADL_RESETVAL      (0x0000u)

/* OSDWIN1ADL */

#define CSL_OSD_OSDWIN1ADL_BMPWIN1ADL_MASK (0xFFFFu)
#define CSL_OSD_OSDWIN1ADL_BMPWIN1ADL_SHIFT (0x0000u)
#define CSL_OSD_OSDWIN1ADL_BMPWIN1ADL_RESETVAL (0x0000u)

#define CSL_OSD_OSDWIN1ADL_RESETVAL      (0x0000u)

/* BASEPX */

#define CSL_OSD_BASEPX_BPX_MASK          (0x03FFu)
#define CSL_OSD_BASEPX_BPX_SHIFT         (0x0000u)
#define CSL_OSD_BASEPX_BPX_RESETVAL      (0x0000u)

#define CSL_OSD_BASEPX_RESETVAL          (0x0000u)

/* BASEPY */

#define CSL_OSD_BASEPY_BPY_MASK          (0x01FFu)
#define CSL_OSD_BASEPY_BPY_SHIFT         (0x0000u)
#define CSL_OSD_BASEPY_BPY_RESETVAL      (0x0000u)

#define CSL_OSD_BASEPY_RESETVAL          (0x0000u)

/* VIDWIN0XP */

#define CSL_OSD_VIDWIN0XP_V0X_MASK       (0x07FFu)
#define CSL_OSD_VIDWIN0XP_V0X_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN0XP_V0X_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN0XP_RESETVAL       (0x0000u)

/* VIDWIN0YP */

#define CSL_OSD_VIDWIN0YP_V0Y_MASK       (0x01FFu)
#define CSL_OSD_VIDWIN0YP_V0Y_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN0YP_V0Y_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN0YP_RESETVAL       (0x0000u)

/* VIDWIN0XL */

#define CSL_OSD_VIDWIN0XL_V0W_MASK       (0x07FFu)
#define CSL_OSD_VIDWIN0XL_V0W_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN0XL_V0W_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN0XL_RESETVAL       (0x0000u)

/* VIDWIN0YL */

#define CSL_OSD_VIDWIN0YL_V0H_MASK       (0x03FFu)
#define CSL_OSD_VIDWIN0YL_V0H_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN0YL_V0H_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN0YL_RESETVAL       (0x0000u)

/* VIDWIN1XP */

#define CSL_OSD_VIDWIN1XP_V1X_MASK       (0x07FFu)
#define CSL_OSD_VIDWIN1XP_V1X_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN1XP_V1X_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN1XP_RESETVAL       (0x0000u)

/* VIDWIN1YP */

#define CSL_OSD_VIDWIN1YP_V1Y_MASK       (0x03FFu)
#define CSL_OSD_VIDWIN1YP_V1Y_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN1YP_V1Y_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN1YP_RESETVAL       (0x0000u)

/* VIDWIN1XL */

#define CSL_OSD_VIDWIN1XL_V1W_MASK       (0x07FFu)
#define CSL_OSD_VIDWIN1XL_V1W_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN1XL_V1W_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN1XL_RESETVAL       (0x0000u)

/* VIDWIN1YL */

#define CSL_OSD_VIDWIN1YL_V1H_MASK       (0x03FFu)
#define CSL_OSD_VIDWIN1YL_V1H_SHIFT      (0x0000u)
#define CSL_OSD_VIDWIN1YL_V1H_RESETVAL   (0x0000u)

#define CSL_OSD_VIDWIN1YL_RESETVAL       (0x0000u)

/* OSDWIN0XP */

#define CSL_OSD_OSDWIN0XP_W0X_MASK       (0x07FFu)
#define CSL_OSD_OSDWIN0XP_W0X_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN0XP_W0X_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN0XP_RESETVAL       (0x0000u)

/* OSDWIN0YP */

#define CSL_OSD_OSDWIN0YP_W0Y_MASK       (0x03FFu)
#define CSL_OSD_OSDWIN0YP_W0Y_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN0YP_W0Y_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN0YP_RESETVAL       (0x0000u)

/* OSDWIN0XL */

#define CSL_OSD_OSDWIN0XL_W0W_MASK       (0x07FFu)
#define CSL_OSD_OSDWIN0XL_W0W_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN0XL_W0W_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN0XL_RESETVAL       (0x0000u)

/* OSDWIN0YL */

#define CSL_OSD_OSDWIN0YL_W0H_MASK       (0x03FFu)
#define CSL_OSD_OSDWIN0YL_W0H_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN0YL_W0H_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN0YL_RESETVAL       (0x0000u)

/* OSDWIN1XP */

#define CSL_OSD_OSDWIN1XP_W1X_MASK       (0x07FFu)
#define CSL_OSD_OSDWIN1XP_W1X_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN1XP_W1X_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN1XP_RESETVAL       (0x0000u)

/* OSDWIN1YP */

#define CSL_OSD_OSDWIN1YP_W1Y_MASK       (0x03FFu)
#define CSL_OSD_OSDWIN1YP_W1Y_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN1YP_W1Y_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN1YP_RESETVAL       (0x0000u)

/* OSDWIN1XL */

#define CSL_OSD_OSDWIN1XL_W1W_MASK       (0x07FFu)
#define CSL_OSD_OSDWIN1XL_W1W_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN1XL_W1W_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN1XL_RESETVAL       (0x0000u)

/* OSDWIN1YL */

#define CSL_OSD_OSDWIN1YL_W1H_MASK       (0x03FFu)
#define CSL_OSD_OSDWIN1YL_W1H_SHIFT      (0x0000u)
#define CSL_OSD_OSDWIN1YL_W1H_RESETVAL   (0x0000u)

#define CSL_OSD_OSDWIN1YL_RESETVAL       (0x0000u)

/* CURXP */

#define CSL_OSD_CURXP_RCSX_MASK          (0x07FFu)
#define CSL_OSD_CURXP_RCSX_SHIFT         (0x0000u)
#define CSL_OSD_CURXP_RCSX_RESETVAL      (0x0000u)

#define CSL_OSD_CURXP_RESETVAL           (0x0000u)

/* CURYP */

#define CSL_OSD_CURYP_RCSY_MASK          (0x03FFu)
#define CSL_OSD_CURYP_RCSY_SHIFT         (0x0000u)
#define CSL_OSD_CURYP_RCSY_RESETVAL      (0x0000u)

#define CSL_OSD_CURYP_RESETVAL           (0x0000u)

/* CURXL */

#define CSL_OSD_CURXL_RCSW_MASK          (0x07FFu)
#define CSL_OSD_CURXL_RCSW_SHIFT         (0x0000u)
#define CSL_OSD_CURXL_RCSW_RESETVAL      (0x0000u)

#define CSL_OSD_CURXL_RESETVAL           (0x0000u)

/* CURYL */

#define CSL_OSD_CURYL_RCSH_MASK          (0x03FFu)
#define CSL_OSD_CURYL_RCSH_SHIFT         (0x0000u)
#define CSL_OSD_CURYL_RCSH_RESETVAL      (0x0000u)

#define CSL_OSD_CURYL_RESETVAL           (0x0000u)

/* RSV1 */

#define CSL_OSD_RSV1_RSV1_MASK           (0xFFFFu)
#define CSL_OSD_RSV1_RSV1_SHIFT          (0x0000u)
#define CSL_OSD_RSV1_RSV1_RESETVAL       (0x0000u)

#define CSL_OSD_RSV1_RESETVAL            (0x0000u)

/* RSV2 */

#define CSL_OSD_RSV2_RSV2_MASK           (0xFFFFu)
#define CSL_OSD_RSV2_RSV2_SHIFT          (0x0000u)
#define CSL_OSD_RSV2_RSV2_RESETVAL       (0x0000u)

#define CSL_OSD_RSV2_RESETVAL            (0x0000u)

/* W0BMP01 */

#define CSL_OSD_W0BMP01_PAL01_MASK       (0xFF00u)
#define CSL_OSD_W0BMP01_PAL01_SHIFT      (0x0008u)
#define CSL_OSD_W0BMP01_PAL01_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP01_PAL00_MASK       (0x00FFu)
#define CSL_OSD_W0BMP01_PAL00_SHIFT      (0x0000u)
#define CSL_OSD_W0BMP01_PAL00_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP01_RESETVAL         (0x0000u)

/* W0BMP23 */

#define CSL_OSD_W0BMP23_PAL03_MASK       (0xFF00u)
#define CSL_OSD_W0BMP23_PAL03_SHIFT      (0x0008u)
#define CSL_OSD_W0BMP23_PAL03_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP23_PAL02_MASK       (0x00FFu)
#define CSL_OSD_W0BMP23_PAL02_SHIFT      (0x0000u)
#define CSL_OSD_W0BMP23_PAL02_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP23_RESETVAL         (0x0000u)

/* W0BMP45 */

#define CSL_OSD_W0BMP45_PAL05_MASK       (0xFF00u)
#define CSL_OSD_W0BMP45_PAL05_SHIFT      (0x0008u)
#define CSL_OSD_W0BMP45_PAL05_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP45_PAL04_MASK       (0x00FFu)
#define CSL_OSD_W0BMP45_PAL04_SHIFT      (0x0000u)
#define CSL_OSD_W0BMP45_PAL04_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP45_RESETVAL         (0x0000u)

/* W0BMP67 */

#define CSL_OSD_W0BMP67_PAL07_MASK       (0xFF00u)
#define CSL_OSD_W0BMP67_PAL07_SHIFT      (0x0008u)
#define CSL_OSD_W0BMP67_PAL07_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP67_PAL06_MASK       (0x00FFu)
#define CSL_OSD_W0BMP67_PAL06_SHIFT      (0x0000u)
#define CSL_OSD_W0BMP67_PAL06_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP67_RESETVAL         (0x0000u)

/* W0BMP89 */

#define CSL_OSD_W0BMP89_PAL09_MASK       (0xFF00u)
#define CSL_OSD_W0BMP89_PAL09_SHIFT      (0x0008u)
#define CSL_OSD_W0BMP89_PAL09_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP89_PAL08_MASK       (0x00FFu)
#define CSL_OSD_W0BMP89_PAL08_SHIFT      (0x0000u)
#define CSL_OSD_W0BMP89_PAL08_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMP89_RESETVAL         (0x0000u)

/* W0BMPAB */

#define CSL_OSD_W0BMPAB_PAL11_MASK       (0xFF00u)
#define CSL_OSD_W0BMPAB_PAL11_SHIFT      (0x0008u)
#define CSL_OSD_W0BMPAB_PAL11_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPAB_PAL10_MASK       (0x00FFu)
#define CSL_OSD_W0BMPAB_PAL10_SHIFT      (0x0000u)
#define CSL_OSD_W0BMPAB_PAL10_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPAB_RESETVAL         (0x0000u)

/* W0BMPCD */

#define CSL_OSD_W0BMPCD_PAL13_MASK       (0xFF00u)
#define CSL_OSD_W0BMPCD_PAL13_SHIFT      (0x0008u)
#define CSL_OSD_W0BMPCD_PAL13_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPCD_PAL12_MASK       (0x00FFu)
#define CSL_OSD_W0BMPCD_PAL12_SHIFT      (0x0000u)
#define CSL_OSD_W0BMPCD_PAL12_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPCD_RESETVAL         (0x0000u)

/* W0BMPEF */

#define CSL_OSD_W0BMPEF_PAL15_MASK       (0xFF00u)
#define CSL_OSD_W0BMPEF_PAL15_SHIFT      (0x0008u)
#define CSL_OSD_W0BMPEF_PAL15_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPEF_PAL14_MASK       (0x00FFu)
#define CSL_OSD_W0BMPEF_PAL14_SHIFT      (0x0000u)
#define CSL_OSD_W0BMPEF_PAL14_RESETVAL   (0x0000u)

#define CSL_OSD_W0BMPEF_RESETVAL         (0x0000u)

/* W1BMP01 */

#define CSL_OSD_W1BMP01_PAL01_MASK       (0xFF00u)
#define CSL_OSD_W1BMP01_PAL01_SHIFT      (0x0008u)
#define CSL_OSD_W1BMP01_PAL01_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP01_PAL00_MASK       (0x00FFu)
#define CSL_OSD_W1BMP01_PAL00_SHIFT      (0x0000u)
#define CSL_OSD_W1BMP01_PAL00_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP01_RESETVAL         (0x0000u)

/* W1BMP23 */

#define CSL_OSD_W1BMP23_PAL03_MASK       (0xFF00u)
#define CSL_OSD_W1BMP23_PAL03_SHIFT      (0x0008u)
#define CSL_OSD_W1BMP23_PAL03_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP23_PAL02_MASK       (0x00FFu)
#define CSL_OSD_W1BMP23_PAL02_SHIFT      (0x0000u)
#define CSL_OSD_W1BMP23_PAL02_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP23_RESETVAL         (0x0000u)

/* W1BMP45 */

#define CSL_OSD_W1BMP45_PAL05_MASK       (0xFF00u)
#define CSL_OSD_W1BMP45_PAL05_SHIFT      (0x0008u)
#define CSL_OSD_W1BMP45_PAL05_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP45_PAL04_MASK       (0x00FFu)
#define CSL_OSD_W1BMP45_PAL04_SHIFT      (0x0000u)
#define CSL_OSD_W1BMP45_PAL04_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP45_RESETVAL         (0x0000u)

/* W1BMP67 */

#define CSL_OSD_W1BMP67_PAL07_MASK       (0xFF00u)
#define CSL_OSD_W1BMP67_PAL07_SHIFT      (0x0008u)
#define CSL_OSD_W1BMP67_PAL07_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP67_PAL06_MASK       (0x00FFu)
#define CSL_OSD_W1BMP67_PAL06_SHIFT      (0x0000u)
#define CSL_OSD_W1BMP67_PAL06_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP67_RESETVAL         (0x0000u)

/* W1BMP89 */

#define CSL_OSD_W1BMP89_PAL09_MASK       (0xFF00u)
#define CSL_OSD_W1BMP89_PAL09_SHIFT      (0x0008u)
#define CSL_OSD_W1BMP89_PAL09_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP89_PAL08_MASK       (0x00FFu)
#define CSL_OSD_W1BMP89_PAL08_SHIFT      (0x0000u)
#define CSL_OSD_W1BMP89_PAL08_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMP89_RESETVAL         (0x0000u)

/* W1BMPAB */

#define CSL_OSD_W1BMPAB_PAL11_MASK       (0xFF00u)
#define CSL_OSD_W1BMPAB_PAL11_SHIFT      (0x0008u)
#define CSL_OSD_W1BMPAB_PAL11_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPAB_PAL10_MASK       (0x00FFu)
#define CSL_OSD_W1BMPAB_PAL10_SHIFT      (0x0000u)
#define CSL_OSD_W1BMPAB_PAL10_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPAB_RESETVAL         (0x0000u)

/* W1BMPCD */

#define CSL_OSD_W1BMPCD_PAL13_MASK       (0xFF00u)
#define CSL_OSD_W1BMPCD_PAL13_SHIFT      (0x0008u)
#define CSL_OSD_W1BMPCD_PAL13_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPCD_PAL12_MASK       (0x00FFu)
#define CSL_OSD_W1BMPCD_PAL12_SHIFT      (0x0000u)
#define CSL_OSD_W1BMPCD_PAL12_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPCD_RESETVAL         (0x0000u)

/* W1BMPEF */

#define CSL_OSD_W1BMPEF_PAL15_MASK       (0xFF00u)
#define CSL_OSD_W1BMPEF_PAL15_SHIFT      (0x0008u)
#define CSL_OSD_W1BMPEF_PAL15_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPEF_PAL14_MASK       (0x00FFu)
#define CSL_OSD_W1BMPEF_PAL14_SHIFT      (0x0000u)
#define CSL_OSD_W1BMPEF_PAL14_RESETVAL   (0x0000u)

#define CSL_OSD_W1BMPEF_RESETVAL         (0x0000u)

/* VBNDRY */

#define CSL_OSD_VBNDRY_TSTPATNCHROMA_MASK (0xFF00u)
#define CSL_OSD_VBNDRY_TSTPATNCHROMA_SHIFT (0x0008u)
#define CSL_OSD_VBNDRY_TSTPATNCHROMA_RESETVAL (0x0000u)

#define CSL_OSD_VBNDRY_VFILINCMD_MASK    (0x0008u)
#define CSL_OSD_VBNDRY_VFILINCMD_SHIFT   (0x0003u)
#define CSL_OSD_VBNDRY_VFILINCMD_RESETVAL (0x0000u)

/*----VFILINCMD Tokens----*/
#define CSL_OSD_VBNDRY_VFILINCMD_OFF     (0x0000u)
#define CSL_OSD_VBNDRY_VFILINCMD_ON      (0x0001u)

#define CSL_OSD_VBNDRY_VBNDRYPRCSEN_MASK (0x0004u)
#define CSL_OSD_VBNDRY_VBNDRYPRCSEN_SHIFT (0x0002u)
#define CSL_OSD_VBNDRY_VBNDRYPRCSEN_RESETVAL (0x0000u)

/*----VBNDRYPRCSEN Tokens----*/
#define CSL_OSD_VBNDRY_VBNDRYPRCSEN_OFF  (0x0000u)
#define CSL_OSD_VBNDRY_VBNDRYPRCSEN_ON   (0x0001u)

#define CSL_OSD_VBNDRY_TEST1_MASK        (0x0002u)
#define CSL_OSD_VBNDRY_TEST1_SHIFT       (0x0001u)
#define CSL_OSD_VBNDRY_TEST1_RESETVAL    (0x0000u)

/*----TEST1 Tokens----*/
#define CSL_OSD_VBNDRY_TEST1_NORMAL      (0x0000u)
#define CSL_OSD_VBNDRY_TEST1_RESET_ALL_REGS (0x0001u)

#define CSL_OSD_VBNDRY_TEST0_MASK        (0x0001u)
#define CSL_OSD_VBNDRY_TEST0_SHIFT       (0x0000u)
#define CSL_OSD_VBNDRY_TEST0_RESETVAL    (0x0000u)

/*----TEST0 Tokens----*/
#define CSL_OSD_VBNDRY_TEST0_NORMAL      (0x0000u)
#define CSL_OSD_VBNDRY_TEST0_TEST_MODE   (0x0001u)

#define CSL_OSD_VBNDRY_RESETVAL          (0x0000u)

/* EXTMODE */

#define CSL_OSD_EXTMODE_EXPMDSEL_MASK    (0x8000u)
#define CSL_OSD_EXTMODE_EXPMDSEL_SHIFT   (0x000Fu)
#define CSL_OSD_EXTMODE_EXPMDSEL_RESETVAL (0x0000u)

/*----EXPMDSEL Tokens----*/
#define CSL_OSD_EXTMODE_EXPMDSEL_FILTER_BEFORE (0x0000u)
#define CSL_OSD_EXTMODE_EXPMDSEL_FILTER_AFTER (0x0001u)

#define CSL_OSD_EXTMODE_SCRNHEXP_MASK    (0x6000u)
#define CSL_OSD_EXTMODE_SCRNHEXP_SHIFT   (0x000Du)
#define CSL_OSD_EXTMODE_SCRNHEXP_RESETVAL (0x0000u)

/*----SCRNHEXP Tokens----*/
#define CSL_OSD_EXTMODE_SCRNHEXP_X1      (0x0000u)
#define CSL_OSD_EXTMODE_SCRNHEXP_X9_8    (0x0001u)
#define CSL_OSD_EXTMODE_SCRNHEXP_X3_2    (0x000Au)
#define CSL_OSD_EXTMODE_SCRNHEXP_RESERVED (0x000Bu)

#define CSL_OSD_EXTMODE_SCRNVEXP_MASK    (0x1000u)
#define CSL_OSD_EXTMODE_SCRNVEXP_SHIFT   (0x000Cu)
#define CSL_OSD_EXTMODE_SCRNVEXP_RESETVAL (0x0000u)

/*----SCRNVEXP Tokens----*/
#define CSL_OSD_EXTMODE_SCRNVEXP_X1      (0x0000u)
#define CSL_OSD_EXTMODE_SCRNVEXP_X6_5    (0x0001u)

#define CSL_OSD_EXTMODE_OSD1BLDCHR_MASK  (0x0800u)
#define CSL_OSD_EXTMODE_OSD1BLDCHR_SHIFT (0x000Bu)
#define CSL_OSD_EXTMODE_OSD1BLDCHR_RESETVAL (0x0000u)

/*----OSD1BLDCHR Tokens----*/
#define CSL_OSD_EXTMODE_OSD1BLDCHR_GLOBAL_BLEND (0x0000u)
#define CSL_OSD_EXTMODE_OSD1BLDCHR_PIXEL_BLEND (0x0001u)

#define CSL_OSD_EXTMODE_OSD0BLDCHR_MASK  (0x0400u)
#define CSL_OSD_EXTMODE_OSD0BLDCHR_SHIFT (0x000Au)
#define CSL_OSD_EXTMODE_OSD0BLDCHR_RESETVAL (0x0000u)

/*----OSD0BLDCHR Tokens----*/
#define CSL_OSD_EXTMODE_OSD0BLDCHR_GLOBAL_BLEND (0x0000u)
#define CSL_OSD_EXTMODE_OSD0BLDCHR_PIXEL_BLEND (0x0001u)

#define CSL_OSD_EXTMODE_ATNOSD1EN_MASK   (0x0200u)
#define CSL_OSD_EXTMODE_ATNOSD1EN_SHIFT  (0x0009u)
#define CSL_OSD_EXTMODE_ATNOSD1EN_RESETVAL (0x0000u)

/*----ATNOSD1EN Tokens----*/
#define CSL_OSD_EXTMODE_ATNOSD1EN_NORMAL (0x0000u)
#define CSL_OSD_EXTMODE_ATNOSD1EN_ATTENUATED (0x0001u)

#define CSL_OSD_EXTMODE_ATNOSD0EN_MASK   (0x0100u)
#define CSL_OSD_EXTMODE_ATNOSD0EN_SHIFT  (0x0008u)
#define CSL_OSD_EXTMODE_ATNOSD0EN_RESETVAL (0x0000u)

/*----ATNOSD0EN Tokens----*/
#define CSL_OSD_EXTMODE_ATNOSD0EN_NORMAL (0x0000u)
#define CSL_OSD_EXTMODE_ATNOSD0EN_ATTENUATED (0x0001u)

#define CSL_OSD_EXTMODE_OSDHRSZ15_MASK   (0x0080u)
#define CSL_OSD_EXTMODE_OSDHRSZ15_SHIFT  (0x0007u)
#define CSL_OSD_EXTMODE_OSDHRSZ15_RESETVAL (0x0000u)

/*----OSDHRSZ15 Tokens----*/
#define CSL_OSD_EXTMODE_OSDHRSZ15_HRSZ_NORMAL (0x0000u)
#define CSL_OSD_EXTMODE_OSDHRSZ15_HRSZ_1P5 (0x0001u)

#define CSL_OSD_EXTMODE_VIDHRSZ15_MASK   (0x0040u)
#define CSL_OSD_EXTMODE_VIDHRSZ15_SHIFT  (0x0006u)
#define CSL_OSD_EXTMODE_VIDHRSZ15_RESETVAL (0x0000u)

/*----VIDHRSZ15 Tokens----*/
#define CSL_OSD_EXTMODE_VIDHRSZ15_HRSZ_NORMAL (0x0000u)
#define CSL_OSD_EXTMODE_VIDHRSZ15_HRSZ_1P5 (0x0001u)

#define CSL_OSD_EXTMODE_ZMFILV1HEN_MASK  (0x0020u)
#define CSL_OSD_EXTMODE_ZMFILV1HEN_SHIFT (0x0005u)
#define CSL_OSD_EXTMODE_ZMFILV1HEN_RESETVAL (0x0000u)

/*----ZMFILV1HEN Tokens----*/
#define CSL_OSD_EXTMODE_ZMFILV1HEN_OFF   (0x0000u)
#define CSL_OSD_EXTMODE_ZMFILV1HEN_ON    (0x0001u)

#define CSL_OSD_EXTMODE_ZMFILV1VEN_MASK  (0x0010u)
#define CSL_OSD_EXTMODE_ZMFILV1VEN_SHIFT (0x0004u)
#define CSL_OSD_EXTMODE_ZMFILV1VEN_RESETVAL (0x0000u)

/*----ZMFILV1VEN Tokens----*/
#define CSL_OSD_EXTMODE_ZMFILV1VEN_OFF   (0x0000u)
#define CSL_OSD_EXTMODE_ZMFILV1VEN_ON    (0x0001u)

#define CSL_OSD_EXTMODE_ZMFILV0HEN_MASK  (0x0008u)
#define CSL_OSD_EXTMODE_ZMFILV0HEN_SHIFT (0x0003u)
#define CSL_OSD_EXTMODE_ZMFILV0HEN_RESETVAL (0x0000u)

/*----ZMFILV0HEN Tokens----*/
#define CSL_OSD_EXTMODE_ZMFILV0HEN_OFF   (0x0000u)
#define CSL_OSD_EXTMODE_ZMFILV0HEN_ON    (0x0001u)

#define CSL_OSD_EXTMODE_ZMFILV0VEN_MASK  (0x0004u)
#define CSL_OSD_EXTMODE_ZMFILV0VEN_SHIFT (0x0002u)
#define CSL_OSD_EXTMODE_ZMFILV0VEN_RESETVAL (0x0000u)

/*----ZMFILV0VEN Tokens----*/
#define CSL_OSD_EXTMODE_ZMFILV0VEN_OFF   (0x0000u)
#define CSL_OSD_EXTMODE_ZMFILV0VEN_ON    (0x0001u)

#define CSL_OSD_EXTMODE_EXPFILHEN_MASK   (0x0002u)
#define CSL_OSD_EXTMODE_EXPFILHEN_SHIFT  (0x0001u)
#define CSL_OSD_EXTMODE_EXPFILHEN_RESETVAL (0x0000u)

/*----EXPFILHEN Tokens----*/
#define CSL_OSD_EXTMODE_EXPFILHEN_OFF    (0x0000u)
#define CSL_OSD_EXTMODE_EXPFILHEN_ON     (0x0001u)

#define CSL_OSD_EXTMODE_EXPFILVEN_MASK   (0x0001u)
#define CSL_OSD_EXTMODE_EXPFILVEN_SHIFT  (0x0000u)
#define CSL_OSD_EXTMODE_EXPFILVEN_RESETVAL (0x0000u)

/*----EXPFILVEN Tokens----*/
#define CSL_OSD_EXTMODE_EXPFILVEN_OFF    (0x0000u)
#define CSL_OSD_EXTMODE_EXPFILVEN_ON     (0x0001u)

#define CSL_OSD_EXTMODE_RESETVAL         (0x0000u)

/* MISCCTL */

#define CSL_OSD_MISCCTL_FIELD_ID_MASK    (0x0080u)
#define CSL_OSD_MISCCTL_FIELD_ID_SHIFT   (0x0007u)
#define CSL_OSD_MISCCTL_FIELD_ID_RESETVAL (0x0000u)

/*----FIELD_ID Tokens----*/
#define CSL_OSD_MISCCTL_FIELD_ID_TOP     (0x0000u)
#define CSL_OSD_MISCCTL_FIELD_ID_BOTTOM  (0x0001u)

#define CSL_OSD_MISCCTL_DMANG_MASK       (0x0040u)
#define CSL_OSD_MISCCTL_DMANG_SHIFT      (0x0006u)
#define CSL_OSD_MISCCTL_DMANG_RESETVAL   (0x0000u)

/*----DMANG Tokens----*/
#define CSL_OSD_MISCCTL_DMANG_NO_ERROR   (0x0000u)
#define CSL_OSD_MISCCTL_DMANG_ERROR      (0x0001u)

#define CSL_OSD_MISCCTL_RSEL_MASK        (0x0010u)
#define CSL_OSD_MISCCTL_RSEL_SHIFT       (0x0004u)
#define CSL_OSD_MISCCTL_RSEL_RESETVAL    (0x0000u)

/*----RSEL Tokens----*/
#define CSL_OSD_MISCCTL_RSEL_CLUT0       (0x0000u)
#define CSL_OSD_MISCCTL_RSEL_CLUT1       (0x0001u)

#define CSL_OSD_MISCCTL_CPBSY_MASK       (0x0008u)
#define CSL_OSD_MISCCTL_CPBSY_SHIFT      (0x0003u)
#define CSL_OSD_MISCCTL_CPBSY_RESETVAL   (0x0000u)

/*----CPBSY Tokens----*/
#define CSL_OSD_MISCCTL_CPBSY_NOTBUSY    (0x0000u)
#define CSL_OSD_MISCCTL_CPBSY_BUSY       (0x0001u)

#define CSL_OSD_MISCCTL_RESETVAL         (0x0000u)

/* CLUTRAMYCB */

#define CSL_OSD_CLUTRAMYCB_Y_MASK        (0xFF00u)
#define CSL_OSD_CLUTRAMYCB_Y_SHIFT       (0x0008u)
#define CSL_OSD_CLUTRAMYCB_Y_RESETVAL    (0x0000u)

#define CSL_OSD_CLUTRAMYCB_CB_MASK       (0x00FFu)
#define CSL_OSD_CLUTRAMYCB_CB_SHIFT      (0x0000u)
#define CSL_OSD_CLUTRAMYCB_CB_RESETVAL   (0x0000u)

#define CSL_OSD_CLUTRAMYCB_RESETVAL      (0x0000u)

/* CLUTRAMCR */

#define CSL_OSD_CLUTRAMCR_CR_MASK        (0xFF00u)
#define CSL_OSD_CLUTRAMCR_CR_SHIFT       (0x0008u)
#define CSL_OSD_CLUTRAMCR_CR_RESETVAL    (0x0000u)

#define CSL_OSD_CLUTRAMCR_CADDR_MASK     (0x00FFu)
#define CSL_OSD_CLUTRAMCR_CADDR_SHIFT    (0x0000u)
#define CSL_OSD_CLUTRAMCR_CADDR_RESETVAL (0x0000u)

#define CSL_OSD_CLUTRAMCR_RESETVAL       (0x0000u)

/* TRANSPVALL */

#define CSL_OSD_TRANSPVALL_RGBL_MASK     (0xFFFFu)
#define CSL_OSD_TRANSPVALL_RGBL_SHIFT    (0x0000u)
#define CSL_OSD_TRANSPVALL_RGBL_RESETVAL (0x0000u)

#define CSL_OSD_TRANSPVALL_RESETVAL      (0x0000u)

/* TRANSPVALU */

#define CSL_OSD_TRANSPVALU_Y_MASK        (0xFF00u)
#define CSL_OSD_TRANSPVALU_Y_SHIFT       (0x0008u)
#define CSL_OSD_TRANSPVALU_Y_RESETVAL    (0x0000u)

#define CSL_OSD_TRANSPVALU_RGBU_MASK     (0x00FFu)
#define CSL_OSD_TRANSPVALU_RGBU_SHIFT    (0x0000u)
#define CSL_OSD_TRANSPVALU_RGBU_RESETVAL (0x0000u)

#define CSL_OSD_TRANSPVALU_RESETVAL      (0x0000u)

/* TRANSPBMPIDX */

#define CSL_OSD_TRANSPBMPIDX_BMP1_MASK   (0xFF00u)
#define CSL_OSD_TRANSPBMPIDX_BMP1_SHIFT  (0x0008u)
#define CSL_OSD_TRANSPBMPIDX_BMP1_RESETVAL (0x0000u)

#define CSL_OSD_TRANSPBMPIDX_BMP0_MASK   (0x00FFu)
#define CSL_OSD_TRANSPBMPIDX_BMP0_SHIFT  (0x0000u)
#define CSL_OSD_TRANSPBMPIDX_BMP0_RESETVAL (0x0000u)

#define CSL_OSD_TRANSPBMPIDX_RESETVAL    (0x0000u)

#endif
