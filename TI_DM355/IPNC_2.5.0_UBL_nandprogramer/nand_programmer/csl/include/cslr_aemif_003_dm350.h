#ifndef _CSLR_EMIF_1_H_
#define _CSLR_EMIF_1_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_emif_1.h
 * 
 * \brief This file contains the Register Desciptions for EMIF
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 ERCSR;
    volatile Uint32 AWCCR;
    volatile Uint8 RSVD0[8];
    volatile Uint32 AB1CR;
    volatile Uint32 AB2CR;
    volatile Uint8 RSVD1[40];
    volatile Uint32 EIRR;
    volatile Uint32 EIMR;
    volatile Uint32 EIMSR;
    volatile Uint32 EIMCR;
    volatile Uint8 RSVD2[12];
    volatile Uint32 ONENANDCTL;
    volatile Uint32 NANDFCR;
    volatile Uint32 NANDFSR;
    //volatile Uint8 RSVD3[8];VIG Added F1 and F4ECC
	volatile Uint32 NANDF1ECC;
	volatile Uint32 NANDF2ECC;
    volatile Uint32 NANDF3ECC;
    volatile Uint32 NANDF4ECC;
    volatile Uint8 RSVD4[56];
    volatile Uint32 MODRNR;
    volatile Uint8 RSVD5[8];
    volatile Uint32 NAND4BITECCLOAD;
    volatile Uint32 NAND4BITECC1;
    volatile Uint32 NAND4BITECC2;
    volatile Uint32 NAND4BITECC3;
    volatile Uint32 NAND4BITECC4;
    volatile Uint32 NANDERRADD1;
    volatile Uint32 NANDERRADD2;
    volatile Uint32 NANDERRVAL1;
    volatile Uint32 NANDERRVAL2;
} CSL_EmifRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* ERCSR */

#define CSL_EMIF_ERCSR_BE_MASK           (0x80000000u)
#define CSL_EMIF_ERCSR_BE_SHIFT          (0x0000001Fu)
#define CSL_EMIF_ERCSR_BE_RESETVAL       (0x00000001u)

#define CSL_EMIF_ERCSR_FR_MASK           (0x40000000u)
#define CSL_EMIF_ERCSR_FR_SHIFT          (0x0000001Eu)
#define CSL_EMIF_ERCSR_FR_RESETVAL       (0x00000001u)

#define CSL_EMIF_ERCSR_MID_MASK          (0x3FFF0000u)
#define CSL_EMIF_ERCSR_MID_SHIFT         (0x00000010u)
#define CSL_EMIF_ERCSR_MID_RESETVAL      (0x00000000u)

#define CSL_EMIF_ERCSR_MAJREV_MASK       (0x0000FF00u)
#define CSL_EMIF_ERCSR_MAJREV_SHIFT      (0x00000008u)
#define CSL_EMIF_ERCSR_MAJREV_RESETVAL   (0x00000002u)

#define CSL_EMIF_ERCSR_MINREV_MASK       (0x000000FFu)
#define CSL_EMIF_ERCSR_MINREV_SHIFT      (0x00000000u)
#define CSL_EMIF_ERCSR_MINREV_RESETVAL   (0x00000001u)

#define CSL_EMIF_ERCSR_RESETVAL          (0xC0000201u)

/* AWCCR */

#define CSL_EMIF_AWCCR_WP3_MASK          (0x80000000u)
#define CSL_EMIF_AWCCR_WP3_SHIFT         (0x0000001Fu)
#define CSL_EMIF_AWCCR_WP3_RESETVAL      (0x00000001u)

/*----WP3 Tokens----*/
#define CSL_EMIF_AWCCR_WP3_WAITLOW       (0x00000000u)
#define CSL_EMIF_AWCCR_WP3_WAITHIGH      (0x00000001u)

#define CSL_EMIF_AWCCR_WP2_MASK          (0x40000000u)
#define CSL_EMIF_AWCCR_WP2_SHIFT         (0x0000001Eu)
#define CSL_EMIF_AWCCR_WP2_RESETVAL      (0x00000001u)

/*----WP2 Tokens----*/
#define CSL_EMIF_AWCCR_WP2_WAITLOW       (0x00000000u)
#define CSL_EMIF_AWCCR_WP2_WAITHIGH      (0x00000001u)

#define CSL_EMIF_AWCCR_WP1_MASK          (0x20000000u)
#define CSL_EMIF_AWCCR_WP1_SHIFT         (0x0000001Du)
#define CSL_EMIF_AWCCR_WP1_RESETVAL      (0x00000001u)

/*----WP1 Tokens----*/
#define CSL_EMIF_AWCCR_WP1_WAITLOW       (0x00000000u)
#define CSL_EMIF_AWCCR_WP1_WAITHIGH      (0x00000001u)

#define CSL_EMIF_AWCCR_WP0_MASK          (0x10000000u)
#define CSL_EMIF_AWCCR_WP0_SHIFT         (0x0000001Cu)
#define CSL_EMIF_AWCCR_WP0_RESETVAL      (0x00000001u)

/*----WP0 Tokens----*/
#define CSL_EMIF_AWCCR_WP0_WAITLOW       (0x00000000u)
#define CSL_EMIF_AWCCR_WP0_WAITHIGH      (0x00000001u)

#define CSL_EMIF_AWCCR_CS5WAIT_MASK      (0x00C00000u)
#define CSL_EMIF_AWCCR_CS5WAIT_SHIFT     (0x00000016u)
#define CSL_EMIF_AWCCR_CS5WAIT_RESETVAL  (0x00000000u)

#define CSL_EMIF_AWCCR_CS4WAIT_MASK      (0x00300000u)
#define CSL_EMIF_AWCCR_CS4WAIT_SHIFT     (0x00000014u)
#define CSL_EMIF_AWCCR_CS4WAIT_RESETVAL  (0x00000000u)

#define CSL_EMIF_AWCCR_CS3WAIT_MASK      (0x000C0000u)
#define CSL_EMIF_AWCCR_CS3WAIT_SHIFT     (0x00000012u)
#define CSL_EMIF_AWCCR_CS3WAIT_RESETVAL  (0x00000000u)

#define CSL_EMIF_AWCCR_CS2WAIT_MASK      (0x00030000u)
#define CSL_EMIF_AWCCR_CS2WAIT_SHIFT     (0x00000010u)
#define CSL_EMIF_AWCCR_CS2WAIT_RESETVAL  (0x00000000u)

#define CSL_EMIF_AWCCR_MEWC_MASK         (0x000000FFu)
#define CSL_EMIF_AWCCR_MEWC_SHIFT        (0x00000000u)
#define CSL_EMIF_AWCCR_MEWC_RESETVAL     (0x00000080u)

#define CSL_EMIF_AWCCR_RESETVAL          (0xF0000080u)

/* AB1CR */

#define CSL_EMIF_AB1CR_SS_MASK           (0x80000000u)
#define CSL_EMIF_AB1CR_SS_SHIFT          (0x0000001Fu)
#define CSL_EMIF_AB1CR_SS_RESETVAL       (0x00000000u)

#define CSL_EMIF_AB1CR_EW_MASK           (0x40000000u)
#define CSL_EMIF_AB1CR_EW_SHIFT          (0x0000001Eu)
#define CSL_EMIF_AB1CR_EW_RESETVAL       (0x00000000u)

#define CSL_EMIF_AB1CR_W_SETUP_MASK      (0x3C000000u)
#define CSL_EMIF_AB1CR_W_SETUP_SHIFT     (0x0000001Au)
#define CSL_EMIF_AB1CR_W_SETUP_RESETVAL  (0x0000000Fu)

#define CSL_EMIF_AB1CR_W_STROBE_MASK     (0x03F00000u)
#define CSL_EMIF_AB1CR_W_STROBE_SHIFT    (0x00000014u)
#define CSL_EMIF_AB1CR_W_STROBE_RESETVAL (0x0000003Fu)

#define CSL_EMIF_AB1CR_W_HOLD_MASK       (0x000E0000u)
#define CSL_EMIF_AB1CR_W_HOLD_SHIFT      (0x00000011u)
#define CSL_EMIF_AB1CR_W_HOLD_RESETVAL   (0x00000007u)

#define CSL_EMIF_AB1CR_R_SETUP_MASK      (0x0001E000u)
#define CSL_EMIF_AB1CR_R_SETUP_SHIFT     (0x0000000Du)
#define CSL_EMIF_AB1CR_R_SETUP_RESETVAL  (0x0000000Fu)

#define CSL_EMIF_AB1CR_R_STROBE_MASK     (0x00001F80u)
#define CSL_EMIF_AB1CR_R_STROBE_SHIFT    (0x00000007u)
#define CSL_EMIF_AB1CR_R_STROBE_RESETVAL (0x0000003Fu)

#define CSL_EMIF_AB1CR_R_HOLD_MASK       (0x00000070u)
#define CSL_EMIF_AB1CR_R_HOLD_SHIFT      (0x00000004u)
#define CSL_EMIF_AB1CR_R_HOLD_RESETVAL   (0x00000007u)

#define CSL_EMIF_AB1CR_TA_MASK           (0x0000000Cu)
#define CSL_EMIF_AB1CR_TA_SHIFT          (0x00000002u)
#define CSL_EMIF_AB1CR_TA_RESETVAL       (0x00000003u)

#define CSL_EMIF_AB1CR_ASIZE_MASK        (0x00000003u)
#define CSL_EMIF_AB1CR_ASIZE_SHIFT       (0x00000000u)
#define CSL_EMIF_AB1CR_ASIZE_RESETVAL    (0x00000001u)

/*----ASIZE Tokens----*/
#define CSL_EMIF_AB1CR_ASIZE_ASIZE_8BITS (0x00000000u)
#define CSL_EMIF_AB1CR_ASIZE_ASIZE_16BITS (0x00000001u)

#define CSL_EMIF_AB1CR_RESETVAL          (0x3FFFFFFDu)

/* AB2CR */

#define CSL_EMIF_AB2CR_SS_MASK           (0x80000000u)
#define CSL_EMIF_AB2CR_SS_SHIFT          (0x0000001Fu)
#define CSL_EMIF_AB2CR_SS_RESETVAL       (0x00000000u)

#define CSL_EMIF_AB2CR_EW_MASK           (0x40000000u)
#define CSL_EMIF_AB2CR_EW_SHIFT          (0x0000001Eu)
#define CSL_EMIF_AB2CR_EW_RESETVAL       (0x00000000u)

#define CSL_EMIF_AB2CR_W_SETUP_MASK      (0x3C000000u)
#define CSL_EMIF_AB2CR_W_SETUP_SHIFT     (0x0000001Au)
#define CSL_EMIF_AB2CR_W_SETUP_RESETVAL  (0x0000000Fu)

#define CSL_EMIF_AB2CR_W_STROBE_MASK     (0x03F00000u)
#define CSL_EMIF_AB2CR_W_STROBE_SHIFT    (0x00000014u)
#define CSL_EMIF_AB2CR_W_STROBE_RESETVAL (0x0000003Fu)

#define CSL_EMIF_AB2CR_W_HOLD_MASK       (0x000E0000u)
#define CSL_EMIF_AB2CR_W_HOLD_SHIFT      (0x00000011u)
#define CSL_EMIF_AB2CR_W_HOLD_RESETVAL   (0x00000007u)

#define CSL_EMIF_AB2CR_R_SETUP_MASK      (0x0001E000u)
#define CSL_EMIF_AB2CR_R_SETUP_SHIFT     (0x0000000Du)
#define CSL_EMIF_AB2CR_R_SETUP_RESETVAL  (0x0000000Fu)

#define CSL_EMIF_AB2CR_R_STROBE_MASK     (0x00001F80u)
#define CSL_EMIF_AB2CR_R_STROBE_SHIFT    (0x00000007u)
#define CSL_EMIF_AB2CR_R_STROBE_RESETVAL (0x0000003Fu)

#define CSL_EMIF_AB2CR_R_HOLD_MASK       (0x00000070u)
#define CSL_EMIF_AB2CR_R_HOLD_SHIFT      (0x00000004u)
#define CSL_EMIF_AB2CR_R_HOLD_RESETVAL   (0x00000007u)

#define CSL_EMIF_AB2CR_TA_MASK           (0x0000000Cu)
#define CSL_EMIF_AB2CR_TA_SHIFT          (0x00000002u)
#define CSL_EMIF_AB2CR_TA_RESETVAL       (0x00000003u)

#define CSL_EMIF_AB2CR_ASIZE_MASK        (0x00000003u)
#define CSL_EMIF_AB2CR_ASIZE_SHIFT       (0x00000000u)
#define CSL_EMIF_AB2CR_ASIZE_RESETVAL    (0x00000001u)

/*----ASIZE Tokens----*/
#define CSL_EMIF_AB2CR_ASIZE_ASIZE_8BITS (0x00000000u)
#define CSL_EMIF_AB2CR_ASIZE_ASIZE_16BITS (0x00000001u)

#define CSL_EMIF_AB2CR_RESETVAL          (0x3FFFFFFDu)

/* EIRR */

#define CSL_EMIF_EIRR_WR_MASK            (0x0000003Cu)
#define CSL_EMIF_EIRR_WR_SHIFT           (0x00000002u)
#define CSL_EMIF_EIRR_WR_RESETVAL        (0x00000000u)

#define CSL_EMIF_EIRR_LT_MASK            (0x00000002u)
#define CSL_EMIF_EIRR_LT_SHIFT           (0x00000001u)
#define CSL_EMIF_EIRR_LT_RESETVAL        (0x00000000u)

#define CSL_EMIF_EIRR_AT_MASK            (0x00000001u)
#define CSL_EMIF_EIRR_AT_SHIFT           (0x00000000u)
#define CSL_EMIF_EIRR_AT_RESETVAL        (0x00000000u)

#define CSL_EMIF_EIRR_RESETVAL           (0x00000000u)

/* EIMR */

#define CSL_EMIF_EIMR_WRM_MASK           (0x0000003Cu)
#define CSL_EMIF_EIMR_WRM_SHIFT          (0x00000002u)
#define CSL_EMIF_EIMR_WRM_RESETVAL       (0x00000000u)

#define CSL_EMIF_EIMR_LTM_MASK           (0x00000002u)
#define CSL_EMIF_EIMR_LTM_SHIFT          (0x00000001u)
#define CSL_EMIF_EIMR_LTM_RESETVAL       (0x00000000u)

#define CSL_EMIF_EIMR_ATM_MASK           (0x00000001u)
#define CSL_EMIF_EIMR_ATM_SHIFT          (0x00000000u)
#define CSL_EMIF_EIMR_ATM_RESETVAL       (0x00000000u)

#define CSL_EMIF_EIMR_RESETVAL           (0x00000000u)

/* EIMSR */

#define CSL_EMIF_EIMSR_WRMSET_MASK       (0x0000003Cu)
#define CSL_EMIF_EIMSR_WRMSET_SHIFT      (0x00000002u)
#define CSL_EMIF_EIMSR_WRMSET_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMSR_LTMSET_MASK       (0x00000002u)
#define CSL_EMIF_EIMSR_LTMSET_SHIFT      (0x00000001u)
#define CSL_EMIF_EIMSR_LTMSET_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMSR_ATMSET_MASK       (0x00000001u)
#define CSL_EMIF_EIMSR_ATMSET_SHIFT      (0x00000000u)
#define CSL_EMIF_EIMSR_ATMSET_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMSR_RESETVAL          (0x00000000u)

/* EIMCR */

#define CSL_EMIF_EIMCR_WRMCLR_MASK       (0x0000003Cu)
#define CSL_EMIF_EIMCR_WRMCLR_SHIFT      (0x00000002u)
#define CSL_EMIF_EIMCR_WRMCLR_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMCR_LTMCLR_MASK       (0x00000002u)
#define CSL_EMIF_EIMCR_LTMCLR_SHIFT      (0x00000001u)
#define CSL_EMIF_EIMCR_LTMCLR_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMCR_ATMCLR_MASK       (0x00000001u)
#define CSL_EMIF_EIMCR_ATMCLR_SHIFT      (0x00000000u)
#define CSL_EMIF_EIMCR_ATMCLR_RESETVAL   (0x00000000u)

#define CSL_EMIF_EIMCR_RESETVAL          (0x00000000u)

/* ONENANDCTL */

#define CSL_EMIF_ONENANDCTL_RD_LATENCY_MASK (0x00000700u)
#define CSL_EMIF_ONENANDCTL_RD_LATENCY_SHIFT (0x00000008u)
#define CSL_EMIF_ONENANDCTL_RD_LATENCY_RESETVAL (0x00000004u)

#define CSL_EMIF_ONENANDCTL_CS3ONENANDRDMOD_MASK (0x00000020u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDRDMOD_SHIFT (0x00000005u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDRDMOD_RESETVAL (0x00000000u)

/*----CS3ONENANDRDMOD Tokens----*/
#define CSL_EMIF_ONENANDCTL_CS3ONENANDRDMOD_ONENAND_ASYNC (0x00000000u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDRDMOD_ONENAND_SYNC (0x00000001u)

#define CSL_EMIF_ONENANDCTL_CS2ONENANDRDMOD_MASK (0x00000010u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDRDMOD_SHIFT (0x00000004u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDRDMOD_RESETVAL (0x00000000u)

/*----CS2ONENANDRDMOD Tokens----*/
#define CSL_EMIF_ONENANDCTL_CS2ONENANDRDMOD_ONENAND_ASYNC (0x00000000u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDRDMOD_ONENAND_SYNC (0x00000001u)

#define CSL_EMIF_ONENANDCTL_CS3ONENANDSEL_MASK (0x00000002u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDSEL_SHIFT (0x00000001u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDSEL_RESETVAL (0x00000000u)

/*----CS3ONENANDSEL Tokens----*/
#define CSL_EMIF_ONENANDCTL_CS3ONENANDSEL_DISABLE (0x00000000u)
#define CSL_EMIF_ONENANDCTL_CS3ONENANDSEL_ONENANDMOD (0x00000001u)

#define CSL_EMIF_ONENANDCTL_CS2ONENANDSEL_MASK (0x00000001u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDSEL_SHIFT (0x00000000u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDSEL_RESETVAL (0x00000000u)

/*----CS2ONENANDSEL Tokens----*/
#define CSL_EMIF_ONENANDCTL_CS2ONENANDSEL_DISABLE (0x00000000u)
#define CSL_EMIF_ONENANDCTL_CS2ONENANDSEL_ONENANDMOD (0x00000001u)

#define CSL_EMIF_ONENANDCTL_RESETVAL     (0x00000400u)

/* NANDFCR */

#define CSL_EMIF_NANDFCR_4BITECC_ADD_CALC_START_MASK (0x00002000u)
#define CSL_EMIF_NANDFCR_4BITECC_ADD_CALC_START_SHIFT (0x0000000Du)
#define CSL_EMIF_NANDFCR_4BITECC_ADD_CALC_START_RESETVAL (0x00000000u)

/*----4BITECC_ADD_CALC_START Tokens----*/
#define CSL_EMIF_NANDFCR_4BITECC_ADD_CALC_START_NOACTION (0x00000000u)
#define CSL_EMIF_NANDFCR_4BITECC_ADD_CALC_START_STARTECCADDCALC (0x00000001u)

#define CSL_EMIF_NANDFCR_4BITECC_START_MASK (0x00001000u)
#define CSL_EMIF_NANDFCR_4BITECC_START_SHIFT (0x0000000Cu)
#define CSL_EMIF_NANDFCR_4BITECC_START_RESETVAL (0x00000000u)

/*----4BITECC_START Tokens----*/
#define CSL_EMIF_NANDFCR_4BITECC_START_NOACTION (0x00000000u)
#define CSL_EMIF_NANDFCR_4BITECC_START_STARTECCADDCALC (0x00000001u)

#define CSL_EMIF_NANDFCR_CS3ECC_MASK     (0x00000200u)
#define CSL_EMIF_NANDFCR_CS3ECC_SHIFT    (0x00000009u)
#define CSL_EMIF_NANDFCR_CS3ECC_RESETVAL (0x00000000u)

/*----CS3ECC Tokens----*/
#define CSL_EMIF_NANDFCR_CS3ECC_NOACTION (0x00000000u)
#define CSL_EMIF_NANDFCR_CS3ECC_STARTCALC (0x00000001u)

#define CSL_EMIF_NANDFCR_CS2ECC_MASK     (0x00000100u)
#define CSL_EMIF_NANDFCR_CS2ECC_SHIFT    (0x00000008u)
#define CSL_EMIF_NANDFCR_CS2ECC_RESETVAL (0x00000000u)

/*----CS2ECC Tokens----*/
#define CSL_EMIF_NANDFCR_CS2ECC_NOACTION (0x00000000u)
#define CSL_EMIF_NANDFCR_CS2ECC_STARTCALC (0x00000001u)

#define CSL_EMIF_NANDFCR_4BITECCSEL_MASK (0x00000030u)
#define CSL_EMIF_NANDFCR_4BITECCSEL_SHIFT (0x00000004u)
#define CSL_EMIF_NANDFCR_4BITECCSEL_RESETVAL (0x00000000u)

/*----4BITECCSEL Tokens----*/
#define CSL_EMIF_NANDFCR_4BITECCSEL_ECC4BITCS2 (0x00000000u)
#define CSL_EMIF_NANDFCR_4BITECCSEL_ECC4BITCS3 (0x00000001u)

#define CSL_EMIF_NANDFCR_CS3NAND_MASK    (0x00000002u)
#define CSL_EMIF_NANDFCR_CS3NAND_SHIFT   (0x00000001u)
#define CSL_EMIF_NANDFCR_CS3NAND_RESETVAL (0x00000000u)

/*----CS3NAND Tokens----*/
#define CSL_EMIF_NANDFCR_CS3NAND_DISABLE (0x00000000u)
#define CSL_EMIF_NANDFCR_CS3NAND_ENABLE  (0x00000001u)

#define CSL_EMIF_NANDFCR_CS2NAND_MASK    (0x00000001u)
#define CSL_EMIF_NANDFCR_CS2NAND_SHIFT   (0x00000000u)
#define CSL_EMIF_NANDFCR_CS2NAND_RESETVAL (0x00000000u)

/*----CS2NAND Tokens----*/
#define CSL_EMIF_NANDFCR_CS2NAND_DISABLE (0x00000000u)
#define CSL_EMIF_NANDFCR_CS2NAND_ENABLE  (0x00000001u)

#define CSL_EMIF_NANDFCR_RESETVAL        (0x00000000u)

/* NANDFSR */

#define CSL_EMIF_NANDFSR_ECC_ERRNUM_MASK (0x00030000u)
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_SHIFT (0x00000010u)
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_RESETVAL (0x00000000u)

/*----ECC_ERRNUM Tokens----*/
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_NUMERRORS1 (0x00000000u)
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_NUMERRORS2 (0x00000001u)
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_NUMERRORS3 (0x00000002u)
#define CSL_EMIF_NANDFSR_ECC_ERRNUM_NUMERRORS4 (0x00000003u)

#define CSL_EMIF_NANDFSR_ECC_STATE_MASK  (0x00000F00u)
#define CSL_EMIF_NANDFSR_ECC_STATE_SHIFT (0x00000008u)
#define CSL_EMIF_NANDFSR_ECC_STATE_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDFSR_WAITST_MASK     (0x0000000Fu)
#define CSL_EMIF_NANDFSR_WAITST_SHIFT    (0x00000000u)
#define CSL_EMIF_NANDFSR_WAITST_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDFSR_RESETVAL        (0x00000000u)

/* NANDF2ECC */

#define CSL_EMIF_NANDF2ECC_P2048O_MASK   (0x08000000u)
#define CSL_EMIF_NANDF2ECC_P2048O_SHIFT  (0x0000001Bu)
#define CSL_EMIF_NANDF2ECC_P2048O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P1024O_MASK   (0x04000000u)
#define CSL_EMIF_NANDF2ECC_P1024O_SHIFT  (0x0000001Au)
#define CSL_EMIF_NANDF2ECC_P1024O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P512O_MASK    (0x02000000u)
#define CSL_EMIF_NANDF2ECC_P512O_SHIFT   (0x00000019u)
#define CSL_EMIF_NANDF2ECC_P512O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P256O_MASK    (0x01000000u)
#define CSL_EMIF_NANDF2ECC_P256O_SHIFT   (0x00000018u)
#define CSL_EMIF_NANDF2ECC_P256O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P128O_MASK    (0x00800000u)
#define CSL_EMIF_NANDF2ECC_P128O_SHIFT   (0x00000017u)
#define CSL_EMIF_NANDF2ECC_P128O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P64O_MASK     (0x00400000u)
#define CSL_EMIF_NANDF2ECC_P64O_SHIFT    (0x00000016u)
#define CSL_EMIF_NANDF2ECC_P64O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P32O_MASK     (0x00200000u)
#define CSL_EMIF_NANDF2ECC_P32O_SHIFT    (0x00000015u)
#define CSL_EMIF_NANDF2ECC_P32O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P16O_MASK     (0x00100000u)
#define CSL_EMIF_NANDF2ECC_P16O_SHIFT    (0x00000014u)
#define CSL_EMIF_NANDF2ECC_P16O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P8O_MASK      (0x00080000u)
#define CSL_EMIF_NANDF2ECC_P8O_SHIFT     (0x00000013u)
#define CSL_EMIF_NANDF2ECC_P8O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P4O_MASK      (0x00040000u)
#define CSL_EMIF_NANDF2ECC_P4O_SHIFT     (0x00000012u)
#define CSL_EMIF_NANDF2ECC_P4O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P2O_MASK      (0x00020000u)
#define CSL_EMIF_NANDF2ECC_P2O_SHIFT     (0x00000011u)
#define CSL_EMIF_NANDF2ECC_P2O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P1O_MASK      (0x00010000u)
#define CSL_EMIF_NANDF2ECC_P1O_SHIFT     (0x00000010u)
#define CSL_EMIF_NANDF2ECC_P1O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P2048E_MASK   (0x00000800u)
#define CSL_EMIF_NANDF2ECC_P2048E_SHIFT  (0x0000000Bu)
#define CSL_EMIF_NANDF2ECC_P2048E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P1024E_MASK   (0x00000400u)
#define CSL_EMIF_NANDF2ECC_P1024E_SHIFT  (0x0000000Au)
#define CSL_EMIF_NANDF2ECC_P1024E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P512E_MASK    (0x00000200u)
#define CSL_EMIF_NANDF2ECC_P512E_SHIFT   (0x00000009u)
#define CSL_EMIF_NANDF2ECC_P512E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P256E_MASK    (0x00000100u)
#define CSL_EMIF_NANDF2ECC_P256E_SHIFT   (0x00000008u)
#define CSL_EMIF_NANDF2ECC_P256E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P128E_MASK    (0x00000080u)
#define CSL_EMIF_NANDF2ECC_P128E_SHIFT   (0x00000007u)
#define CSL_EMIF_NANDF2ECC_P128E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P64E_MASK     (0x00000040u)
#define CSL_EMIF_NANDF2ECC_P64E_SHIFT    (0x00000006u)
#define CSL_EMIF_NANDF2ECC_P64E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P32E_MASK     (0x00000020u)
#define CSL_EMIF_NANDF2ECC_P32E_SHIFT    (0x00000005u)
#define CSL_EMIF_NANDF2ECC_P32E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P16E_MASK     (0x00000010u)
#define CSL_EMIF_NANDF2ECC_P16E_SHIFT    (0x00000004u)
#define CSL_EMIF_NANDF2ECC_P16E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P8E_MASK      (0x00000008u)
#define CSL_EMIF_NANDF2ECC_P8E_SHIFT     (0x00000003u)
#define CSL_EMIF_NANDF2ECC_P8E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P4E_MASK      (0x00000004u)
#define CSL_EMIF_NANDF2ECC_P4E_SHIFT     (0x00000002u)
#define CSL_EMIF_NANDF2ECC_P4E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P2E_MASK      (0x00000002u)
#define CSL_EMIF_NANDF2ECC_P2E_SHIFT     (0x00000001u)
#define CSL_EMIF_NANDF2ECC_P2E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_P1E_MASK      (0x00000001u)
#define CSL_EMIF_NANDF2ECC_P1E_SHIFT     (0x00000000u)
#define CSL_EMIF_NANDF2ECC_P1E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF2ECC_RESETVAL      (0x00000000u)

/* NANDF3ECC */

#define CSL_EMIF_NANDF3ECC_P2048O_MASK   (0x08000000u)
#define CSL_EMIF_NANDF3ECC_P2048O_SHIFT  (0x0000001Bu)
#define CSL_EMIF_NANDF3ECC_P2048O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P1024O_MASK   (0x04000000u)
#define CSL_EMIF_NANDF3ECC_P1024O_SHIFT  (0x0000001Au)
#define CSL_EMIF_NANDF3ECC_P1024O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P512O_MASK    (0x02000000u)
#define CSL_EMIF_NANDF3ECC_P512O_SHIFT   (0x00000019u)
#define CSL_EMIF_NANDF3ECC_P512O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P256O_MASK    (0x01000000u)
#define CSL_EMIF_NANDF3ECC_P256O_SHIFT   (0x00000018u)
#define CSL_EMIF_NANDF3ECC_P256O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P128O_MASK    (0x00800000u)
#define CSL_EMIF_NANDF3ECC_P128O_SHIFT   (0x00000017u)
#define CSL_EMIF_NANDF3ECC_P128O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P64O_MASK     (0x00400000u)
#define CSL_EMIF_NANDF3ECC_P64O_SHIFT    (0x00000016u)
#define CSL_EMIF_NANDF3ECC_P64O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P32O_MASK     (0x00200000u)
#define CSL_EMIF_NANDF3ECC_P32O_SHIFT    (0x00000015u)
#define CSL_EMIF_NANDF3ECC_P32O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P16O_MASK     (0x00100000u)
#define CSL_EMIF_NANDF3ECC_P16O_SHIFT    (0x00000014u)
#define CSL_EMIF_NANDF3ECC_P16O_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P8O_MASK      (0x00080000u)
#define CSL_EMIF_NANDF3ECC_P8O_SHIFT     (0x00000013u)
#define CSL_EMIF_NANDF3ECC_P8O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P4O_MASK      (0x00040000u)
#define CSL_EMIF_NANDF3ECC_P4O_SHIFT     (0x00000012u)
#define CSL_EMIF_NANDF3ECC_P4O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P2O_MASK      (0x00020000u)
#define CSL_EMIF_NANDF3ECC_P2O_SHIFT     (0x00000011u)
#define CSL_EMIF_NANDF3ECC_P2O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P1O_MASK      (0x00010000u)
#define CSL_EMIF_NANDF3ECC_P1O_SHIFT     (0x00000010u)
#define CSL_EMIF_NANDF3ECC_P1O_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P2048E_MASK   (0x00000800u)
#define CSL_EMIF_NANDF3ECC_P2048E_SHIFT  (0x0000000Bu)
#define CSL_EMIF_NANDF3ECC_P2048E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P1024E_MASK   (0x00000400u)
#define CSL_EMIF_NANDF3ECC_P1024E_SHIFT  (0x0000000Au)
#define CSL_EMIF_NANDF3ECC_P1024E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P512E_MASK    (0x00000200u)
#define CSL_EMIF_NANDF3ECC_P512E_SHIFT   (0x00000009u)
#define CSL_EMIF_NANDF3ECC_P512E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P256E_MASK    (0x00000100u)
#define CSL_EMIF_NANDF3ECC_P256E_SHIFT   (0x00000008u)
#define CSL_EMIF_NANDF3ECC_P256E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P128E_MASK    (0x00000080u)
#define CSL_EMIF_NANDF3ECC_P128E_SHIFT   (0x00000007u)
#define CSL_EMIF_NANDF3ECC_P128E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P64E_MASK     (0x00000040u)
#define CSL_EMIF_NANDF3ECC_P64E_SHIFT    (0x00000006u)
#define CSL_EMIF_NANDF3ECC_P64E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P32E_MASK     (0x00000020u)
#define CSL_EMIF_NANDF3ECC_P32E_SHIFT    (0x00000005u)
#define CSL_EMIF_NANDF3ECC_P32E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P16E_MASK     (0x00000010u)
#define CSL_EMIF_NANDF3ECC_P16E_SHIFT    (0x00000004u)
#define CSL_EMIF_NANDF3ECC_P16E_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P8E_MASK      (0x00000008u)
#define CSL_EMIF_NANDF3ECC_P8E_SHIFT     (0x00000003u)
#define CSL_EMIF_NANDF3ECC_P8E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P4E_MASK      (0x00000004u)
#define CSL_EMIF_NANDF3ECC_P4E_SHIFT     (0x00000002u)
#define CSL_EMIF_NANDF3ECC_P4E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P2E_MASK      (0x00000002u)
#define CSL_EMIF_NANDF3ECC_P2E_SHIFT     (0x00000001u)
#define CSL_EMIF_NANDF3ECC_P2E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_P1E_MASK      (0x00000001u)
#define CSL_EMIF_NANDF3ECC_P1E_SHIFT     (0x00000000u)
#define CSL_EMIF_NANDF3ECC_P1E_RESETVAL  (0x00000000u)

#define CSL_EMIF_NANDF3ECC_RESETVAL      (0x00000000u)

/* MODRNR */

#define CSL_EMIF_MODRNR_RLNUM_MASK       (0x000000FFu)
#define CSL_EMIF_MODRNR_RLNUM_SHIFT      (0x00000000u)
#define CSL_EMIF_MODRNR_RLNUM_RESETVAL   (0x00000003u)

#define CSL_EMIF_MODRNR_RESETVAL         (0x00000003u)

/* NAND4BITECCLOAD */

#define CSL_EMIF_NAND4BITECCLOAD_4BITECCLOAD_MASK (0x000003FFu)
#define CSL_EMIF_NAND4BITECCLOAD_4BITECCLOAD_SHIFT (0x00000000u)
#define CSL_EMIF_NAND4BITECCLOAD_4BITECCLOAD_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECCLOAD_RESETVAL (0x00000000u)

/* NAND4BITECC1 */

#define CSL_EMIF_NAND4BITECC1_4BITECCVAL2_MASK (0x03FF0000u)
#define CSL_EMIF_NAND4BITECC1_4BITECCVAL2_SHIFT (0x00000010u)
#define CSL_EMIF_NAND4BITECC1_4BITECCVAL2_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC1_4BITECCVAL1_MASK (0x000003FFu)
#define CSL_EMIF_NAND4BITECC1_4BITECCVAL1_SHIFT (0x00000000u)
#define CSL_EMIF_NAND4BITECC1_4BITECCVAL1_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC1_RESETVAL   (0x00000000u)

/* NAND4BITECC2 */

#define CSL_EMIF_NAND4BITECC2_4BITECCVAL4_MASK (0x03FF0000u)
#define CSL_EMIF_NAND4BITECC2_4BITECCVAL4_SHIFT (0x00000010u)
#define CSL_EMIF_NAND4BITECC2_4BITECCVAL4_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC2_4BITECCVAL3_MASK (0x000003FFu)
#define CSL_EMIF_NAND4BITECC2_4BITECCVAL3_SHIFT (0x00000000u)
#define CSL_EMIF_NAND4BITECC2_4BITECCVAL3_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC2_RESETVAL   (0x00000000u)

/* NAND4BITECC3 */

#define CSL_EMIF_NAND4BITECC3_4BITECCVAL6_MASK (0x03FF0000u)
#define CSL_EMIF_NAND4BITECC3_4BITECCVAL6_SHIFT (0x00000010u)
#define CSL_EMIF_NAND4BITECC3_4BITECCVAL6_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC3_4BITECCVAL5_MASK (0x000003FFu)
#define CSL_EMIF_NAND4BITECC3_4BITECCVAL5_SHIFT (0x00000000u)
#define CSL_EMIF_NAND4BITECC3_4BITECCVAL5_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC3_RESETVAL   (0x00000000u)

/* NAND4BITECC4 */

#define CSL_EMIF_NAND4BITECC4_4BITECCVAL8_MASK (0x03FF0000u)
#define CSL_EMIF_NAND4BITECC4_4BITECCVAL8_SHIFT (0x00000010u)
#define CSL_EMIF_NAND4BITECC4_4BITECCVAL8_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC4_4BITECCVAL7_MASK (0x000003FFu)
#define CSL_EMIF_NAND4BITECC4_4BITECCVAL7_SHIFT (0x00000000u)
#define CSL_EMIF_NAND4BITECC4_4BITECCVAL7_RESETVAL (0x00000000u)

#define CSL_EMIF_NAND4BITECC4_RESETVAL   (0x00000000u)

/* NANDERRADD1 */

#define CSL_EMIF_NANDERRADD1_4BITECCERRADD2_MASK (0x03FF0000u)
#define CSL_EMIF_NANDERRADD1_4BITECCERRADD2_SHIFT (0x00000010u)
#define CSL_EMIF_NANDERRADD1_4BITECCERRADD2_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRADD1_4BITECCERRADD1_MASK (0x000003FFu)
#define CSL_EMIF_NANDERRADD1_4BITECCERRADD1_SHIFT (0x00000000u)
#define CSL_EMIF_NANDERRADD1_4BITECCERRADD1_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRADD1_RESETVAL    (0x00000000u)

/* NANDERRADD2 */

#define CSL_EMIF_NANDERRADD2_4BITECCERRADD4_MASK (0x03FF0000u)
#define CSL_EMIF_NANDERRADD2_4BITECCERRADD4_SHIFT (0x00000010u)
#define CSL_EMIF_NANDERRADD2_4BITECCERRADD4_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRADD2_4BITECCERRADD3_MASK (0x000003FFu)
#define CSL_EMIF_NANDERRADD2_4BITECCERRADD3_SHIFT (0x00000000u)
#define CSL_EMIF_NANDERRADD2_4BITECCERRADD3_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRADD2_RESETVAL    (0x00000000u)

/* NANDERRVAL1 */

#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL2_MASK (0x03FF0000u)
#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL2_SHIFT (0x00000010u)
#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL2_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL1_MASK (0x000003FFu)
#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL1_SHIFT (0x00000000u)
#define CSL_EMIF_NANDERRVAL1_4BITECCERRVAL1_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRVAL1_RESETVAL    (0x00000000u)

/* NANDERRVAL2 */

#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL4_MASK (0x03FF0000u)
#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL4_SHIFT (0x00000010u)
#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL4_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL3_MASK (0x000003FFu)
#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL3_SHIFT (0x00000000u)
#define CSL_EMIF_NANDERRVAL2_4BITECCERRVAL3_RESETVAL (0x00000000u)

#define CSL_EMIF_NANDERRVAL2_RESETVAL    (0x00000000u)

#endif