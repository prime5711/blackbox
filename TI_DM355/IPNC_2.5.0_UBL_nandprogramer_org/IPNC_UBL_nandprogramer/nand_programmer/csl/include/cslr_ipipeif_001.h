#ifndef _CSLR_IPIPEIF_ISPIF_SPEC_V02_H_
#define _CSLR_IPIPEIF_ISPIF_SPEC_V02_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_ipipeif_ispif_spec_v02.h
 * 
 * \brief This file contains the Register Desciptions for IPIPEIF
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 ENABLE;
    volatile Uint8 RSVD0[2];
    volatile Uint16 CFG;
    volatile Uint8 RSVD1[2];
    volatile Uint16 PPLN;
    volatile Uint8 RSVD2[2];
    volatile Uint16 LPFR;
    volatile Uint8 RSVD3[2];
    volatile Uint16 HNUM;
    volatile Uint8 RSVD4[2];
    volatile Uint16 VNUM;
    volatile Uint8 RSVD5[2];
    volatile Uint16 ADDRU;
    volatile Uint8 RSVD6[2];
    volatile Uint16 ADDRL;
    volatile Uint8 RSVD7[2];
    volatile Uint16 ADOFS;
    volatile Uint8 RSVD8[2];
    volatile Uint16 RSZ;
    volatile Uint8 RSVD9[2];
    volatile Uint16 GAIN;
} CSL_IpipeifRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* ENABLE */

#define CSL_IPIPEIF_ENABLE_ENABLE_MASK   (0x0001u)
#define CSL_IPIPEIF_ENABLE_ENABLE_SHIFT  (0x0000u)
#define CSL_IPIPEIF_ENABLE_ENABLE_RESETVAL (0x0000u)

/*----ENABLE Tokens----*/
#define CSL_IPIPEIF_ENABLE_ENABLE_DISABLE (0x0000u)
#define CSL_IPIPEIF_ENABLE_ENABLE_ENABLE (0x0001u)

#define CSL_IPIPEIF_ENABLE_RESETVAL      (0x0000u)

/* CFG */

#define CSL_IPIPEIF_CFG_DATASFT_MASK     (0x3800u)
#define CSL_IPIPEIF_CFG_DATASFT_SHIFT    (0x000Bu)
#define CSL_IPIPEIF_CFG_DATASFT_RESETVAL (0x0002u)

#define CSL_IPIPEIF_CFG_CLKSEL_MASK      (0x0400u)
#define CSL_IPIPEIF_CFG_CLKSEL_SHIFT     (0x000Au)
#define CSL_IPIPEIF_CFG_CLKSEL_RESETVAL  (0x0000u)

/*----CLKSEL Tokens----*/
#define CSL_IPIPEIF_CFG_CLKSEL_PCLK      (0x0000u)
#define CSL_IPIPEIF_CFG_CLKSEL_SDRAM_DIV_CLK (0x0001u)

#define CSL_IPIPEIF_CFG_IALAW_MASK       (0x0200u)
#define CSL_IPIPEIF_CFG_IALAW_SHIFT      (0x0009u)
#define CSL_IPIPEIF_CFG_IALAW_RESETVAL   (0x0000u)

/*----IALAW Tokens----*/
#define CSL_IPIPEIF_CFG_IALAW_OFF        (0x0000u)
#define CSL_IPIPEIF_CFG_IALAW_ON         (0x0001u)

#define CSL_IPIPEIF_CFG_PACK8IN_MASK     (0x0100u)
#define CSL_IPIPEIF_CFG_PACK8IN_SHIFT    (0x0008u)
#define CSL_IPIPEIF_CFG_PACK8IN_RESETVAL (0x0000u)

/*----PACK8IN Tokens----*/
#define CSL_IPIPEIF_CFG_PACK8IN_NORMAL_16_BITS_PIXEL (0x0000u)
#define CSL_IPIPEIF_CFG_PACK8IN_PACK_8_BITS_PIXEL (0x0001u)

#define CSL_IPIPEIF_CFG_AVGFILT_MASK     (0x0080u)
#define CSL_IPIPEIF_CFG_AVGFILT_SHIFT    (0x0007u)
#define CSL_IPIPEIF_CFG_AVGFILT_RESETVAL (0x0000u)

/*----AVGFILT Tokens----*/
#define CSL_IPIPEIF_CFG_AVGFILT_OFF      (0x0000u)
#define CSL_IPIPEIF_CFG_AVGFILT_ON       (0x0001u)

#define CSL_IPIPEIF_CFG_CLKDIV_MASK      (0x0070u)
#define CSL_IPIPEIF_CFG_CLKDIV_SHIFT     (0x0004u)
#define CSL_IPIPEIF_CFG_CLKDIV_RESETVAL  (0x0000u)

/*----CLKDIV Tokens----*/
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_HALF  (0x0000u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_THIRD (0x0001u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_FOURTH (0x0002u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_FIFTH (0x0003u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_SIXTH (0x0004u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_EIGHTH (0x0005u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_SIXTEENTH (0x0006u)
#define CSL_IPIPEIF_CFG_CLKDIV_ONE_THIRTYSECOND (0x0007u)

#define CSL_IPIPEIF_CFG_INPSRC_MASK      (0x000Cu)
#define CSL_IPIPEIF_CFG_INPSRC_SHIFT     (0x0002u)
#define CSL_IPIPEIF_CFG_INPSRC_RESETVAL  (0x0000u)

/*----INPSRC Tokens----*/
#define CSL_IPIPEIF_CFG_INPSRC_CCDC      (0x0000u)
#define CSL_IPIPEIF_CFG_INPSRC_SDRAM_RAW (0x0001u)
#define CSL_IPIPEIF_CFG_INPSRC_CCDC_DARKFM (0x0002u)
#define CSL_IPIPEIF_CFG_INPSRC_SDRAM_YUV (0x0003u)

#define CSL_IPIPEIF_CFG_DECM_MASK        (0x0002u)
#define CSL_IPIPEIF_CFG_DECM_SHIFT       (0x0001u)
#define CSL_IPIPEIF_CFG_DECM_RESETVAL    (0x0000u)

/*----DECM Tokens----*/
#define CSL_IPIPEIF_CFG_DECM_NODECIMATION (0x0000u)
#define CSL_IPIPEIF_CFG_DECM_DECIMATE    (0x0001u)

#define CSL_IPIPEIF_CFG_ONESHOT_MASK     (0x0001u)
#define CSL_IPIPEIF_CFG_ONESHOT_SHIFT    (0x0000u)
#define CSL_IPIPEIF_CFG_ONESHOT_RESETVAL (0x0000u)

/*----ONESHOT Tokens----*/
#define CSL_IPIPEIF_CFG_ONESHOT_CONTINUOUS (0x0000u)
#define CSL_IPIPEIF_CFG_ONESHOT_ONESHOT  (0x0001u)

#define CSL_IPIPEIF_CFG_RESETVAL         (0x1000u)

/* PPLN */

#define CSL_IPIPEIF_PPLN_PPLN_MASK       (0x1FFFu)
#define CSL_IPIPEIF_PPLN_PPLN_SHIFT      (0x0000u)
#define CSL_IPIPEIF_PPLN_PPLN_RESETVAL   (0x0000u)

#define CSL_IPIPEIF_PPLN_RESETVAL        (0x0000u)

/* LPFR */

#define CSL_IPIPEIF_LPFR_LPFR_MASK       (0x1FFFu)
#define CSL_IPIPEIF_LPFR_LPFR_SHIFT      (0x0000u)
#define CSL_IPIPEIF_LPFR_LPFR_RESETVAL   (0x0000u)

#define CSL_IPIPEIF_LPFR_RESETVAL        (0x0000u)

/* HNUM */

#define CSL_IPIPEIF_HNUM_HNUM_MASK       (0x1FFFu)
#define CSL_IPIPEIF_HNUM_HNUM_SHIFT      (0x0000u)
#define CSL_IPIPEIF_HNUM_HNUM_RESETVAL   (0x0000u)

#define CSL_IPIPEIF_HNUM_RESETVAL        (0x0000u)

/* VNUM */

#define CSL_IPIPEIF_VNUM_VNUM_MASK       (0x1FFFu)
#define CSL_IPIPEIF_VNUM_VNUM_SHIFT      (0x0000u)
#define CSL_IPIPEIF_VNUM_VNUM_RESETVAL   (0x0000u)

#define CSL_IPIPEIF_VNUM_RESETVAL        (0x0000u)

/* ADDRU */

#define CSL_IPIPEIF_ADDRU_ADDRU_MASK     (0x007Fu)
#define CSL_IPIPEIF_ADDRU_ADDRU_SHIFT    (0x0000u)
#define CSL_IPIPEIF_ADDRU_ADDRU_RESETVAL (0x0000u)

#define CSL_IPIPEIF_ADDRU_RESETVAL       (0x0000u)

/* ADDRL */

#define CSL_IPIPEIF_ADDRL_ADDRL_MASK     (0xFFFFu)
#define CSL_IPIPEIF_ADDRL_ADDRL_SHIFT    (0x0000u)
#define CSL_IPIPEIF_ADDRL_ADDRL_RESETVAL (0x0000u)

#define CSL_IPIPEIF_ADDRL_RESETVAL       (0x0000u)

/* ADOFS */

#define CSL_IPIPEIF_ADOFS_ADOFS_MASK     (0x01FFu)
#define CSL_IPIPEIF_ADOFS_ADOFS_SHIFT    (0x0000u)
#define CSL_IPIPEIF_ADOFS_ADOFS_RESETVAL (0x0000u)

#define CSL_IPIPEIF_ADOFS_RESETVAL       (0x0000u)

/* RSZ */

#define CSL_IPIPEIF_RSZ_RSZ_MASK         (0x007Fu)
#define CSL_IPIPEIF_RSZ_RSZ_SHIFT        (0x0000u)
#define CSL_IPIPEIF_RSZ_RSZ_RESETVAL     (0x0010u)

#define CSL_IPIPEIF_RSZ_RESETVAL         (0x0010u)

/* GAIN */

#define CSL_IPIPEIF_GAIN_GAIN_MASK       (0x03FFu)
#define CSL_IPIPEIF_GAIN_GAIN_SHIFT      (0x0000u)
#define CSL_IPIPEIF_GAIN_GAIN_RESETVAL   (0x0200u)

#define CSL_IPIPEIF_GAIN_RESETVAL        (0x0200u)

#endif
