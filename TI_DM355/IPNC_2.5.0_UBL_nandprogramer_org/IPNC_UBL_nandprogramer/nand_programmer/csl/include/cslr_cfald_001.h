#ifndef _CSLR_CFALD_CFALD_SPEC_V04_H_
#define _CSLR_CFALD_CFALD_SPEC_V04_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_cfald_cfald_spec_v04.h
 * 
 * \brief This file contains the Register Desciptions for CFALD
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
    volatile Uint32 RD_BASE;
    volatile Uint32 RD_OFST;
    volatile Uint32 FRAME_SIZE;
    volatile Uint32 INITXY;
    volatile Uint32 WR_BASE;
    volatile Uint32 WR_OFST;
    volatile Uint32 MASK_BASE;
    volatile Uint32 MASK_OFST;
    volatile Uint32 CONFIG;
    volatile Uint32 CENTER;
    volatile Uint32 KHV;
    volatile Uint32 BLOCK;
    volatile Uint32 LUT_ADDR;
    volatile Uint32 LUT_WDATA;
    volatile Uint32 LUT_RDATA;
} CSL_CfaldRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* PID */

#define CSL_CFALD_PID_TID_MASK           (0x00FF0000u)
#define CSL_CFALD_PID_TID_SHIFT          (0x00000010u)
#define CSL_CFALD_PID_TID_RESETVAL       (0x00000024u)

#define CSL_CFALD_PID_CID_MASK           (0x0000FF00u)
#define CSL_CFALD_PID_CID_SHIFT          (0x00000008u)
#define CSL_CFALD_PID_CID_RESETVAL       (0x000000FEu)

#define CSL_CFALD_PID_PREV_MASK          (0x000000FFu)
#define CSL_CFALD_PID_PREV_SHIFT         (0x00000000u)
#define CSL_CFALD_PID_PREV_RESETVAL      (0x00000004u)

#define CSL_CFALD_PID_RESETVAL           (0x0024FE04u)

/* PCR */

#define CSL_CFALD_PCR_BUSY_MASK          (0x00000004u)
#define CSL_CFALD_PCR_BUSY_SHIFT         (0x00000002u)
#define CSL_CFALD_PCR_BUSY_RESETVAL      (0x00000000u)

/*----BUSY Tokens----*/
#define CSL_CFALD_PCR_BUSY_IDLE          (0x00000000u)
#define CSL_CFALD_PCR_BUSY_BUSY          (0x00000001u)

#define CSL_CFALD_PCR_MODE_MASK          (0x00000002u)
#define CSL_CFALD_PCR_MODE_SHIFT         (0x00000001u)
#define CSL_CFALD_PCR_MODE_RESETVAL      (0x00000000u)

/*----MODE Tokens----*/
#define CSL_CFALD_PCR_MODE_CFA           (0x00000000u)
#define CSL_CFALD_PCR_MODE_LD            (0x00000001u)

#define CSL_CFALD_PCR_EN_MASK            (0x00000001u)
#define CSL_CFALD_PCR_EN_SHIFT           (0x00000000u)
#define CSL_CFALD_PCR_EN_RESETVAL        (0x00000000u)

/*----EN Tokens----*/
#define CSL_CFALD_PCR_EN_ENABLE          (0x00000001u)

#define CSL_CFALD_PCR_RESETVAL           (0x00000000u)

/* RD_BASE */

#define CSL_CFALD_RD_BASE_RBASE_MASK     (0xFFFFFFFFu)
#define CSL_CFALD_RD_BASE_RBASE_SHIFT    (0x00000000u)
#define CSL_CFALD_RD_BASE_RBASE_RESETVAL (0x00000000u)

#define CSL_CFALD_RD_BASE_RESETVAL       (0x00000000u)

/* RD_OFST */

#define CSL_CFALD_RD_OFST_ROFST_MASK     (0xFFFFFFFFu)
#define CSL_CFALD_RD_OFST_ROFST_SHIFT    (0x00000000u)
#define CSL_CFALD_RD_OFST_ROFST_RESETVAL (0x00000000u)

#define CSL_CFALD_RD_OFST_RESETVAL       (0x00000000u)

/* FRAME_SIZE */

#define CSL_CFALD_FRAME_SIZE_H_MASK      (0x0FFF0000u)
#define CSL_CFALD_FRAME_SIZE_H_SHIFT     (0x00000010u)
#define CSL_CFALD_FRAME_SIZE_H_RESETVAL  (0x00000000u)

#define CSL_CFALD_FRAME_SIZE_W_MASK      (0x00000FFFu)
#define CSL_CFALD_FRAME_SIZE_W_SHIFT     (0x00000000u)
#define CSL_CFALD_FRAME_SIZE_W_RESETVAL  (0x00000000u)

#define CSL_CFALD_FRAME_SIZE_RESETVAL    (0x00000000u)

/* INITXY */

#define CSL_CFALD_INITXY_INITY_MASK      (0x0FFF0000u)
#define CSL_CFALD_INITXY_INITY_SHIFT     (0x00000010u)
#define CSL_CFALD_INITXY_INITY_RESETVAL  (0x00000000u)

#define CSL_CFALD_INITXY_INITX_MASK      (0x00000FFFu)
#define CSL_CFALD_INITXY_INITX_SHIFT     (0x00000000u)
#define CSL_CFALD_INITXY_INITX_RESETVAL  (0x00000000u)

#define CSL_CFALD_INITXY_RESETVAL        (0x00000000u)

/* WR_BASE */

#define CSL_CFALD_WR_BASE_WBASE_MASK     (0xFFFFFFFFu)
#define CSL_CFALD_WR_BASE_WBASE_SHIFT    (0x00000000u)
#define CSL_CFALD_WR_BASE_WBASE_RESETVAL (0x00000000u)

#define CSL_CFALD_WR_BASE_RESETVAL       (0x00000000u)

/* WR_OFST */

#define CSL_CFALD_WR_OFST_WOFST_MASK     (0xFFFFFFFFu)
#define CSL_CFALD_WR_OFST_WOFST_SHIFT    (0x00000000u)
#define CSL_CFALD_WR_OFST_WOFST_RESETVAL (0x00000000u)

#define CSL_CFALD_WR_OFST_RESETVAL       (0x00000000u)

/* MASK_BASE */

#define CSL_CFALD_MASK_BASE_MBASE_MASK   (0xFFFFFFFFu)
#define CSL_CFALD_MASK_BASE_MBASE_SHIFT  (0x00000000u)
#define CSL_CFALD_MASK_BASE_MBASE_RESETVAL (0x00000000u)

#define CSL_CFALD_MASK_BASE_RESETVAL     (0x00000000u)

/* MASK_OFST */

#define CSL_CFALD_MASK_OFST_MOFST_MASK   (0xFFFFFFFFu)
#define CSL_CFALD_MASK_OFST_MOFST_SHIFT  (0x00000000u)
#define CSL_CFALD_MASK_OFST_MOFST_RESETVAL (0x00000000u)

#define CSL_CFALD_MASK_OFST_RESETVAL     (0x00000000u)

/* CONFIG */

#define CSL_CFALD_CONFIG_RTH_MASK        (0x3FFF0000u)
#define CSL_CFALD_CONFIG_RTH_SHIFT       (0x00000010u)
#define CSL_CFALD_CONFIG_RTH_RESETVAL    (0x00000000u)

#define CSL_CFALD_CONFIG_GMODE_MASK      (0x00000040u)
#define CSL_CFALD_CONFIG_GMODE_SHIFT     (0x00000006u)
#define CSL_CFALD_CONFIG_GMODE_RESETVAL  (0x00000000u)

#define CSL_CFALD_CONFIG_INITC_MASK      (0x00000030u)
#define CSL_CFALD_CONFIG_INITC_SHIFT     (0x00000004u)
#define CSL_CFALD_CONFIG_INITC_RESETVAL  (0x00000000u)

#define CSL_CFALD_CONFIG_T_MASK          (0x0000000Fu)
#define CSL_CFALD_CONFIG_T_SHIFT         (0x00000000u)
#define CSL_CFALD_CONFIG_T_RESETVAL      (0x00000000u)

#define CSL_CFALD_CONFIG_RESETVAL        (0x00000000u)

/* CENTER */

#define CSL_CFALD_CENTER_V0_MASK         (0x0FFF0000u)
#define CSL_CFALD_CENTER_V0_SHIFT        (0x00000010u)
#define CSL_CFALD_CENTER_V0_RESETVAL     (0x00000000u)

#define CSL_CFALD_CENTER_H0_MASK         (0x00000FFFu)
#define CSL_CFALD_CENTER_H0_SHIFT        (0x00000000u)
#define CSL_CFALD_CENTER_H0_RESETVAL     (0x00000000u)

#define CSL_CFALD_CENTER_RESETVAL        (0x00000000u)

/* KHV */

#define CSL_CFALD_KHV_KVL_MASK           (0xFF000000u)
#define CSL_CFALD_KHV_KVL_SHIFT          (0x00000018u)
#define CSL_CFALD_KHV_KVL_RESETVAL       (0x00000000u)

#define CSL_CFALD_KHV_KVU_MASK           (0x00FF0000u)
#define CSL_CFALD_KHV_KVU_SHIFT          (0x00000010u)
#define CSL_CFALD_KHV_KVU_RESETVAL       (0x00000000u)

#define CSL_CFALD_KHV_KHR_MASK           (0x0000FF00u)
#define CSL_CFALD_KHV_KHR_SHIFT          (0x00000008u)
#define CSL_CFALD_KHV_KHR_RESETVAL       (0x00000000u)

#define CSL_CFALD_KHV_KHL_MASK           (0x000000FFu)
#define CSL_CFALD_KHV_KHL_SHIFT          (0x00000000u)
#define CSL_CFALD_KHV_KHL_RESETVAL       (0x00000000u)

#define CSL_CFALD_KHV_RESETVAL           (0x00000000u)

/* BLOCK */

#define CSL_CFALD_BLOCK_PIXPAD_MASK      (0x000F0000u)
#define CSL_CFALD_BLOCK_PIXPAD_SHIFT     (0x00000010u)
#define CSL_CFALD_BLOCK_PIXPAD_RESETVAL  (0x00000000u)

#define CSL_CFALD_BLOCK_OBH_MASK         (0x0000FF00u)
#define CSL_CFALD_BLOCK_OBH_SHIFT        (0x00000008u)
#define CSL_CFALD_BLOCK_OBH_RESETVAL     (0x00000000u)

#define CSL_CFALD_BLOCK_OBW_MASK         (0x000000FFu)
#define CSL_CFALD_BLOCK_OBW_SHIFT        (0x00000000u)
#define CSL_CFALD_BLOCK_OBW_RESETVAL     (0x00000000u)

#define CSL_CFALD_BLOCK_RESETVAL         (0x00000000u)

/* LUT_ADDR */

#define CSL_CFALD_LUT_ADDR_ADDR_MASK     (0x000003FFu)
#define CSL_CFALD_LUT_ADDR_ADDR_SHIFT    (0x00000000u)
#define CSL_CFALD_LUT_ADDR_ADDR_RESETVAL (0x00000000u)

#define CSL_CFALD_LUT_ADDR_RESETVAL      (0x00000000u)

/* LUT_WDATA */

#define CSL_CFALD_LUT_WDATA_WDATA_MASK   (0x00003FFFu)
#define CSL_CFALD_LUT_WDATA_WDATA_SHIFT  (0x00000000u)
#define CSL_CFALD_LUT_WDATA_WDATA_RESETVAL (0x00000000u)

#define CSL_CFALD_LUT_WDATA_RESETVAL     (0x00000000u)

/* LUT_RDATA */

#define CSL_CFALD_LUT_RDATA_RDATA_MASK   (0x00003FFFu)
#define CSL_CFALD_LUT_RDATA_RDATA_SHIFT  (0x00000000u)
#define CSL_CFALD_LUT_RDATA_RDATA_RESETVAL (0x00000000u)

#define CSL_CFALD_LUT_RDATA_RESETVAL     (0x00000000u)

#endif
