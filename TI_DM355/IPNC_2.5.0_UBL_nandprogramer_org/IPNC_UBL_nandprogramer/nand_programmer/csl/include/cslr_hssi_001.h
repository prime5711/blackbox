#ifndef _CSLR_HSSI_HSSI_SPEC_V05_H_
#define _CSLR_HSSI_HSSI_SPEC_V05_H_
/*********************************************************************
 * Copyright (C) 2003-2006 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_hssi_hssi_spec_v05.h
 * 
 * \brief This file contains the Register Desciptions for HSSI
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PID;
    volatile Uint32 PCR;
    volatile Uint32 CODE;
    volatile Uint32 INTMASK;
    volatile Uint32 INTSTATUS;
    volatile Uint32 SLN;
    volatile Uint32 ELN;
    volatile Uint32 LNCNT;
    volatile Uint8 RSVD0[92];
    volatile Uint32 PHYCNTL;
} CSL_HssiRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* PID */

#define CSL_HSSI_PID_MAJOR_MASK          (0x000000F0u)
#define CSL_HSSI_PID_MAJOR_SHIFT         (0x00000004u)
#define CSL_HSSI_PID_MAJOR_RESETVAL      (0x00000002u)

#define CSL_HSSI_PID_MINOR_MASK          (0x0000000Fu)
#define CSL_HSSI_PID_MINOR_SHIFT         (0x00000000u)
#define CSL_HSSI_PID_MINOR_RESETVAL      (0x00000000u)

#define CSL_HSSI_PID_RESETVAL            (0x00000020u)

/* PCR */

#define CSL_HSSI_PCR_FORMAT_MASK         (0x00007000u)
#define CSL_HSSI_PCR_FORMAT_SHIFT        (0x0000000Cu)
#define CSL_HSSI_PCR_FORMAT_RESETVAL     (0x00000000u)

/*----FORMAT Tokens----*/
#define CSL_HSSI_PCR_FORMAT_YUV422       (0x00000000u)
#define CSL_HSSI_PCR_FORMAT_RAW8         (0x00000001u)
#define CSL_HSSI_PCR_FORMAT_RAW8_DPCM    (0x00000002u)
#define CSL_HSSI_PCR_FORMAT_RAW10        (0x00000003u)
#define CSL_HSSI_PCR_FORMAT_RAW12        (0x00000004u)
#define CSL_HSSI_PCR_FORMAT_RAW14        (0x00000005u)
#define CSL_HSSI_PCR_FORMAT_RAW16        (0x00000006u)
#define CSL_HSSI_PCR_FORMAT_RAW14R       (0x00000007u)

#define CSL_HSSI_PCR_SCRM_MASK           (0x00000020u)
#define CSL_HSSI_PCR_SCRM_SHIFT          (0x00000005u)
#define CSL_HSSI_PCR_SCRM_RESETVAL       (0x00000000u)

/*----SCRM Tokens----*/
#define CSL_HSSI_PCR_SCRM_DISABLE        (0x00000000u)
#define CSL_HSSI_PCR_SCRM_ENABLE         (0x00000001u)

#define CSL_HSSI_PCR_CLK_STRB_MASK       (0x00000010u)
#define CSL_HSSI_PCR_CLK_STRB_SHIFT      (0x00000004u)
#define CSL_HSSI_PCR_CLK_STRB_RESETVAL   (0x00000000u)

/*----CLK_STRB Tokens----*/
#define CSL_HSSI_PCR_CLK_STRB_STROBE     (0x00000000u)
#define CSL_HSSI_PCR_CLK_STRB_CLOCK      (0x00000001u)

#define CSL_HSSI_PCR_RESET_MASK          (0x00000001u)
#define CSL_HSSI_PCR_RESET_SHIFT         (0x00000000u)
#define CSL_HSSI_PCR_RESET_RESETVAL      (0x00000000u)

/*----RESET Tokens----*/
#define CSL_HSSI_PCR_RESET_ASSERT_RESET  (0x00000000u)
#define CSL_HSSI_PCR_RESET_RELEASE_RESET (0x00000001u)

#define CSL_HSSI_PCR_RESETVAL            (0x00000000u)

/* CODE */

#define CSL_HSSI_CODE_FEC_MASK           (0x0000F000u)
#define CSL_HSSI_CODE_FEC_SHIFT          (0x0000000Cu)
#define CSL_HSSI_CODE_FEC_RESETVAL       (0x00000003u)

#define CSL_HSSI_CODE_FSC_MASK           (0x00000F00u)
#define CSL_HSSI_CODE_FSC_SHIFT          (0x00000008u)
#define CSL_HSSI_CODE_FSC_RESETVAL       (0x00000002u)

#define CSL_HSSI_CODE_LEC_MASK           (0x000000F0u)
#define CSL_HSSI_CODE_LEC_SHIFT          (0x00000004u)
#define CSL_HSSI_CODE_LEC_RESETVAL       (0x00000001u)

#define CSL_HSSI_CODE_LSC_MASK           (0x0000000Fu)
#define CSL_HSSI_CODE_LSC_SHIFT          (0x00000000u)
#define CSL_HSSI_CODE_LSC_RESETVAL       (0x00000000u)

#define CSL_HSSI_CODE_RESETVAL           (0x00003210u)

/* INTMASK */

#define CSL_HSSI_INTMASK_FIFO_MASK       (0x00000040u)
#define CSL_HSSI_INTMASK_FIFO_SHIFT      (0x00000006u)
#define CSL_HSSI_INTMASK_FIFO_RESETVAL   (0x00000001u)

/*----FIFO Tokens----*/
#define CSL_HSSI_INTMASK_FIFO_ENABLE     (0x00000000u)
#define CSL_HSSI_INTMASK_FIFO_DISABLE    (0x00000001u)

#define CSL_HSSI_INTMASK_FS_MASK         (0x00000020u)
#define CSL_HSSI_INTMASK_FS_SHIFT        (0x00000005u)
#define CSL_HSSI_INTMASK_FS_RESETVAL     (0x00000001u)

/*----FS Tokens----*/
#define CSL_HSSI_INTMASK_FS_ENABLE       (0x00000000u)
#define CSL_HSSI_INTMASK_FS_DISABLE      (0x00000001u)

#define CSL_HSSI_INTMASK_LE_MASK         (0x00000010u)
#define CSL_HSSI_INTMASK_LE_SHIFT        (0x00000004u)
#define CSL_HSSI_INTMASK_LE_RESETVAL     (0x00000001u)

/*----LE Tokens----*/
#define CSL_HSSI_INTMASK_LE_ENABLE       (0x00000000u)
#define CSL_HSSI_INTMASK_LE_DISABLE      (0x00000001u)

#define CSL_HSSI_INTMASK_LS_MASK         (0x00000008u)
#define CSL_HSSI_INTMASK_LS_SHIFT        (0x00000003u)
#define CSL_HSSI_INTMASK_LS_RESETVAL     (0x00000001u)

/*----LS Tokens----*/
#define CSL_HSSI_INTMASK_LS_ENABLE       (0x00000000u)
#define CSL_HSSI_INTMASK_LS_DISABLE      (0x00000001u)

#define CSL_HSSI_INTMASK_FE_MASK         (0x00000004u)
#define CSL_HSSI_INTMASK_FE_SHIFT        (0x00000002u)
#define CSL_HSSI_INTMASK_FE_RESETVAL     (0x00000001u)

/*----FE Tokens----*/
#define CSL_HSSI_INTMASK_FE_ENABLE       (0x00000000u)
#define CSL_HSSI_INTMASK_FE_DISABLE      (0x00000001u)

#define CSL_HSSI_INTMASK_CRC_MASK        (0x00000002u)
#define CSL_HSSI_INTMASK_CRC_SHIFT       (0x00000001u)
#define CSL_HSSI_INTMASK_CRC_RESETVAL    (0x00000001u)

/*----CRC Tokens----*/
#define CSL_HSSI_INTMASK_CRC_ENABLE      (0x00000000u)
#define CSL_HSSI_INTMASK_CRC_DISABLE     (0x00000001u)

#define CSL_HSSI_INTMASK_FSS_MASK        (0x00000001u)
#define CSL_HSSI_INTMASK_FSS_SHIFT       (0x00000000u)
#define CSL_HSSI_INTMASK_FSS_RESETVAL    (0x00000001u)

/*----FSS Tokens----*/
#define CSL_HSSI_INTMASK_FSS_ENABLE      (0x00000000u)
#define CSL_HSSI_INTMASK_FSS_DISABLE     (0x00000001u)

#define CSL_HSSI_INTMASK_RESETVAL        (0x0000007Fu)

/* INTSTATUS */

#define CSL_HSSI_INTSTATUS_FIFO_MASK     (0x00000040u)
#define CSL_HSSI_INTSTATUS_FIFO_SHIFT    (0x00000006u)
#define CSL_HSSI_INTSTATUS_FIFO_RESETVAL (0x00000000u)

/*----FIFO Tokens----*/
#define CSL_HSSI_INTSTATUS_FIFO_CLEAR    (0x00000001u)

#define CSL_HSSI_INTSTATUS_FS_MASK       (0x00000020u)
#define CSL_HSSI_INTSTATUS_FS_SHIFT      (0x00000005u)
#define CSL_HSSI_INTSTATUS_FS_RESETVAL   (0x00000000u)

/*----FS Tokens----*/
#define CSL_HSSI_INTSTATUS_FS_CLEAR      (0x00000001u)

#define CSL_HSSI_INTSTATUS_LE_MASK       (0x00000010u)
#define CSL_HSSI_INTSTATUS_LE_SHIFT      (0x00000004u)
#define CSL_HSSI_INTSTATUS_LE_RESETVAL   (0x00000000u)

/*----LE Tokens----*/
#define CSL_HSSI_INTSTATUS_LE_CLEAR      (0x00000001u)

#define CSL_HSSI_INTSTATUS_LS_MASK       (0x00000008u)
#define CSL_HSSI_INTSTATUS_LS_SHIFT      (0x00000003u)
#define CSL_HSSI_INTSTATUS_LS_RESETVAL   (0x00000000u)

/*----LS Tokens----*/
#define CSL_HSSI_INTSTATUS_LS_CLEAR      (0x00000001u)

#define CSL_HSSI_INTSTATUS_FE_MASK       (0x00000004u)
#define CSL_HSSI_INTSTATUS_FE_SHIFT      (0x00000002u)
#define CSL_HSSI_INTSTATUS_FE_RESETVAL   (0x00000000u)

/*----FE Tokens----*/
#define CSL_HSSI_INTSTATUS_FE_CLEAR      (0x00000001u)

#define CSL_HSSI_INTSTATUS_CRC_MASK      (0x00000002u)
#define CSL_HSSI_INTSTATUS_CRC_SHIFT     (0x00000001u)
#define CSL_HSSI_INTSTATUS_CRC_RESETVAL  (0x00000000u)

/*----CRC Tokens----*/
#define CSL_HSSI_INTSTATUS_CRC_CLEAR     (0x00000001u)

#define CSL_HSSI_INTSTATUS_FSS_MASK      (0x00000001u)
#define CSL_HSSI_INTSTATUS_FSS_SHIFT     (0x00000000u)
#define CSL_HSSI_INTSTATUS_FSS_RESETVAL  (0x00000000u)

/*----FSS Tokens----*/
#define CSL_HSSI_INTSTATUS_FSS_CLEAR     (0x00000001u)

#define CSL_HSSI_INTSTATUS_RESETVAL      (0x00000000u)

/* SLN */

#define CSL_HSSI_SLN_START_MASK          (0x0000FFFFu)
#define CSL_HSSI_SLN_START_SHIFT         (0x00000000u)
#define CSL_HSSI_SLN_START_RESETVAL      (0x00000000u)

#define CSL_HSSI_SLN_RESETVAL            (0x00000000u)

/* ELN */

#define CSL_HSSI_ELN_END_MASK            (0x0000FFFFu)
#define CSL_HSSI_ELN_END_SHIFT           (0x00000000u)
#define CSL_HSSI_ELN_END_RESETVAL        (0x0000FFFFu)

#define CSL_HSSI_ELN_RESETVAL            (0x0000FFFFu)

/* LNCNT */

#define CSL_HSSI_LNCNT_CNT_MASK          (0x0000FFFFu)
#define CSL_HSSI_LNCNT_CNT_SHIFT         (0x00000000u)
#define CSL_HSSI_LNCNT_CNT_RESETVAL      (0x00000000u)

#define CSL_HSSI_LNCNT_RESETVAL          (0x00000000u)

/* PHYCNTL */

#define CSL_HSSI_PHYCNTL_SELBGEN_MASK    (0xC0000000u)
#define CSL_HSSI_PHYCNTL_SELBGEN_SHIFT   (0x0000001Eu)
#define CSL_HSSI_PHYCNTL_SELBGEN_RESETVAL (0x00000001u)

#define CSL_HSSI_PHYCNTL_SELREG_MASK     (0x30000000u)
#define CSL_HSSI_PHYCNTL_SELREG_SHIFT    (0x0000001Cu)
#define CSL_HSSI_PHYCNTL_SELREG_RESETVAL (0x00000001u)

#define CSL_HSSI_PHYCNTL_SELDADEL_MASK   (0x0C000000u)
#define CSL_HSSI_PHYCNTL_SELDADEL_SHIFT  (0x0000001Au)
#define CSL_HSSI_PHYCNTL_SELDADEL_RESETVAL (0x00000000u)

#define CSL_HSSI_PHYCNTL_SELSTDEL_MASK   (0x03000000u)
#define CSL_HSSI_PHYCNTL_SELSTDEL_SHIFT  (0x00000018u)
#define CSL_HSSI_PHYCNTL_SELSTDEL_RESETVAL (0x00000000u)

#define CSL_HSSI_PHYCNTL_GCTRL_MASK      (0x00FF0000u)
#define CSL_HSSI_PHYCNTL_GCTRL_SHIFT     (0x00000010u)
#define CSL_HSSI_PHYCNTL_GCTRL_RESETVAL  (0x00000000u)

#define CSL_HSSI_PHYCNTL_PWDLVDS_MASK    (0x00000040u)
#define CSL_HSSI_PHYCNTL_PWDLVDS_SHIFT   (0x00000006u)
#define CSL_HSSI_PHYCNTL_PWDLVDS_RESETVAL (0x00000001u)

/*----PWDLVDS Tokens----*/
#define CSL_HSSI_PHYCNTL_PWDLVDS_POWERON (0x00000000u)
#define CSL_HSSI_PHYCNTL_PWDLVDS_POWERDOWN (0x00000001u)

#define CSL_HSSI_PHYCNTL_PWDBIAS_MASK    (0x00000020u)
#define CSL_HSSI_PHYCNTL_PWDBIAS_SHIFT   (0x00000005u)
#define CSL_HSSI_PHYCNTL_PWDBIAS_RESETVAL (0x00000001u)

/*----PWDBIAS Tokens----*/
#define CSL_HSSI_PHYCNTL_PWDBIAS_POWERON (0x00000000u)
#define CSL_HSSI_PHYCNTL_PWDBIAS_POWERDOWN (0x00000001u)

#define CSL_HSSI_PHYCNTL_PWDHV_MASK      (0x00000010u)
#define CSL_HSSI_PHYCNTL_PWDHV_SHIFT     (0x00000004u)
#define CSL_HSSI_PHYCNTL_PWDHV_RESETVAL  (0x00000001u)

/*----PWDHV Tokens----*/
#define CSL_HSSI_PHYCNTL_PWDHV_POWERON   (0x00000000u)
#define CSL_HSSI_PHYCNTL_PWDHV_POWERDOWN (0x00000001u)

#define CSL_HSSI_PHYCNTL_SELVDD_MASK     (0x00000008u)
#define CSL_HSSI_PHYCNTL_SELVDD_SHIFT    (0x00000003u)
#define CSL_HSSI_PHYCNTL_SELVDD_RESETVAL (0x00000001u)

/*----SELVDD Tokens----*/
#define CSL_HSSI_PHYCNTL_SELVDD_VOLTS1_2 (0x00000000u)
#define CSL_HSSI_PHYCNTL_SELVDD_VOLTS1_3 (0x00000001u)

#define CSL_HSSI_PHYCNTL_LOOPBACK_MASK   (0x00000002u)
#define CSL_HSSI_PHYCNTL_LOOPBACK_SHIFT  (0x00000001u)
#define CSL_HSSI_PHYCNTL_LOOPBACK_RESETVAL (0x00000000u)

/*----LOOPBACK Tokens----*/
#define CSL_HSSI_PHYCNTL_LOOPBACK_LOOPBACKOFF (0x00000000u)
#define CSL_HSSI_PHYCNTL_LOOPBACK_LOOPBACKON (0x00000001u)

#define CSL_HSSI_PHYCNTL_RESET_MASK      (0x00000001u)
#define CSL_HSSI_PHYCNTL_RESET_SHIFT     (0x00000000u)
#define CSL_HSSI_PHYCNTL_RESET_RESETVAL  (0x00000000u)

/*----RESET Tokens----*/
#define CSL_HSSI_PHYCNTL_RESET_ASSERT_RESET (0x00000000u)
#define CSL_HSSI_PHYCNTL_RESET_RELEASE_RESET (0x00000001u)

#define CSL_HSSI_PHYCNTL_RESETVAL        (0x50000078u)

#endif
