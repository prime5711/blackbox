#ifndef _CSLR_COPC_1_H_
#define _CSLR_COPC_1_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_copc_1.h
 * 
 * \brief This file contains the Register Desciptions for COPC
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 ENA;
    volatile Uint16 CNT_1;
    volatile Uint16 CNT_2;
    volatile Uint16 STATE_0;
    volatile Uint16 STATE_1;
    volatile Uint16 MEM_CNT;
    volatile Uint16 SW_IMG_MD0;
    volatile Uint16 SW_IMG_MD1;
    volatile Uint16 SW_IMG_MD2;
    volatile Uint16 SW_SEQ_MD0;
    volatile Uint16 SW_SEQ_MD1;
    volatile Uint16 SW_OTHERS;
    volatile Uint16 CNT_3;
    volatile Uint16 CNT_4;
    volatile Uint16 RSVD0;
    volatile Uint16 RSVD1;
    volatile Uint16 INT_STAT;
    volatile Uint16 PROG_INT;
    volatile Uint16 INFO_STAT;
} CSL_CopcRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* ENA */

#define CSL_COPC_ENA_RESET_MASK          (0x0010u)
#define CSL_COPC_ENA_RESET_SHIFT         (0x0004u)
#define CSL_COPC_ENA_RESET_RESETVAL      (0x0000u)

/*----RESET Tokens----*/
#define CSL_COPC_ENA_RESET_NORMAL        (0x0000u)
#define CSL_COPC_ENA_RESET_RESET         (0x0001u)

#define CSL_COPC_ENA_RESERVED_MASK       (0x000Cu)
#define CSL_COPC_ENA_RESERVED_SHIFT      (0x0002u)
#define CSL_COPC_ENA_RESERVED_RESETVAL   (0x0000u)

#define CSL_COPC_ENA_ONESHOT_MASK        (0x0002u)
#define CSL_COPC_ENA_ONESHOT_SHIFT       (0x0001u)
#define CSL_COPC_ENA_ONESHOT_RESETVAL    (0x0000u)

#define CSL_COPC_ENA_START_MASK          (0x0001u)
#define CSL_COPC_ENA_START_SHIFT         (0x0000u)
#define CSL_COPC_ENA_START_RESETVAL      (0x0000u)

/*----START Tokens----*/
#define CSL_COPC_ENA_START_IDLE          (0x0000u)
#define CSL_COPC_ENA_START_START         (0x0001u)

#define CSL_COPC_ENA_RESETVAL            (0x0000u)

/* CNT_1 */

#define CSL_COPC_CNT_1_VERT_MASK         (0xFF00u)
#define CSL_COPC_CNT_1_VERT_SHIFT        (0x0008u)
#define CSL_COPC_CNT_1_VERT_RESETVAL     (0x0000u)

#define CSL_COPC_CNT_1_HORZ_MASK         (0x00FFu)
#define CSL_COPC_CNT_1_HORZ_SHIFT        (0x0000u)
#define CSL_COPC_CNT_1_HORZ_RESETVAL     (0x0000u)

#define CSL_COPC_CNT_1_RESETVAL          (0x0000u)

/* CNT_2 */

#define CSL_COPC_CNT_2_ENCDEC_MASK       (0x8000u)
#define CSL_COPC_CNT_2_ENCDEC_SHIFT      (0x000Fu)
#define CSL_COPC_CNT_2_ENCDEC_RESETVAL   (0x0000u)

/*----ENCDEC Tokens----*/
#define CSL_COPC_CNT_2_ENCDEC_ENCODE     (0x0000u)
#define CSL_COPC_CNT_2_ENCDEC_DECODE     (0x0001u)

#define CSL_COPC_CNT_2_STILLMOV_MASK     (0x4000u)
#define CSL_COPC_CNT_2_STILLMOV_SHIFT    (0x000Eu)
#define CSL_COPC_CNT_2_STILLMOV_RESETVAL (0x0000u)

/*----STILLMOV Tokens----*/
#define CSL_COPC_CNT_2_STILLMOV_STILL    (0x0000u)
#define CSL_COPC_CNT_2_STILLMOV_MOVIE    (0x0001u)

#define CSL_COPC_CNT_2_CLK_MASK          (0x2000u)
#define CSL_COPC_CNT_2_CLK_SHIFT         (0x000Du)
#define CSL_COPC_CNT_2_CLK_RESETVAL      (0x0000u)

/*----CLK Tokens----*/
#define CSL_COPC_CNT_2_CLK_CLOCKDISABLE  (0x0000u)
#define CSL_COPC_CNT_2_CLK_CLOCKENABLE   (0x0001u)

#define CSL_COPC_CNT_2_PREPOST_MASK      (0x1000u)
#define CSL_COPC_CNT_2_PREPOST_SHIFT     (0x000Cu)
#define CSL_COPC_CNT_2_PREPOST_RESETVAL  (0x0000u)

/*----PREPOST Tokens----*/
#define CSL_COPC_CNT_2_PREPOST_DISABLE   (0x0000u)
#define CSL_COPC_CNT_2_PREPOST_ENABLE    (0x0001u)

#define CSL_COPC_CNT_2_PREDDIS_MASK      (0x0100u)
#define CSL_COPC_CNT_2_PREDDIS_SHIFT     (0x0008u)
#define CSL_COPC_CNT_2_PREDDIS_RESETVAL  (0x0000u)

/*----PREDDIS Tokens----*/
#define CSL_COPC_CNT_2_PREDDIS_LEFTPRED_DIS (0x0000u)
#define CSL_COPC_CNT_2_PREDDIS_UPLEFTPRED_EN (0x0001u)

#define CSL_COPC_CNT_2_SLICE_MASK        (0x00FFu)
#define CSL_COPC_CNT_2_SLICE_SHIFT       (0x0000u)
#define CSL_COPC_CNT_2_SLICE_RESETVAL    (0x00FFu)

#define CSL_COPC_CNT_2_RESETVAL          (0x00FFu)

/* STATE_0 */

#define CSL_COPC_STATE_0_VERT_MASK       (0xFF00u)
#define CSL_COPC_STATE_0_VERT_SHIFT      (0x0008u)
#define CSL_COPC_STATE_0_VERT_RESETVAL   (0x0000u)

#define CSL_COPC_STATE_0_HORZ_MASK       (0x00FFu)
#define CSL_COPC_STATE_0_HORZ_SHIFT      (0x0000u)
#define CSL_COPC_STATE_0_HORZ_RESETVAL   (0x0000u)

#define CSL_COPC_STATE_0_RESETVAL        (0x0000u)

/* STATE_1 */

#define CSL_COPC_STATE_1_VERT_MASK       (0x1000u)
#define CSL_COPC_STATE_1_VERT_SHIFT      (0x000Cu)
#define CSL_COPC_STATE_1_VERT_RESETVAL   (0x0000u)

#define CSL_COPC_STATE_1_HORZ_MASK       (0x0100u)
#define CSL_COPC_STATE_1_HORZ_SHIFT      (0x0008u)
#define CSL_COPC_STATE_1_HORZ_RESETVAL   (0x0000u)

#define CSL_COPC_STATE_1_IMBUFMOD_MASK   (0x0030u)
#define CSL_COPC_STATE_1_IMBUFMOD_SHIFT  (0x0004u)
#define CSL_COPC_STATE_1_IMBUFMOD_RESETVAL (0x0000u)

#define CSL_COPC_STATE_1_RESERVED_MASK   (0x0002u)
#define CSL_COPC_STATE_1_RESERVED_SHIFT  (0x0001u)
#define CSL_COPC_STATE_1_RESERVED_RESETVAL (0x0000u)

#define CSL_COPC_STATE_1_SEQMOD_MASK     (0x0001u)
#define CSL_COPC_STATE_1_SEQMOD_SHIFT    (0x0000u)
#define CSL_COPC_STATE_1_SEQMOD_RESETVAL (0x0000u)

#define CSL_COPC_STATE_1_RESETVAL        (0x0000u)

/* MEM_CNT */

#define CSL_COPC_MEM_CNT_IMBUFMODSIZE_MASK (0x3000u)
#define CSL_COPC_MEM_CNT_IMBUFMODSIZE_SHIFT (0x000Cu)
#define CSL_COPC_MEM_CNT_IMBUFMODSIZE_RESETVAL (0x0000u)

#define CSL_COPC_MEM_CNT_SEQMODSIZE_MASK (0x0100u)
#define CSL_COPC_MEM_CNT_SEQMODSIZE_SHIFT (0x0008u)
#define CSL_COPC_MEM_CNT_SEQMODSIZE_RESETVAL (0x0000u)

#define CSL_COPC_MEM_CNT_IMGBUFA_MASK    (0x0080u)
#define CSL_COPC_MEM_CNT_IMGBUFA_SHIFT   (0x0007u)
#define CSL_COPC_MEM_CNT_IMGBUFA_RESETVAL (0x0000u)

/*----IMGBUFA Tokens----*/
#define CSL_COPC_MEM_CNT_IMGBUFA_AUTOMODE (0x0000u)
#define CSL_COPC_MEM_CNT_IMGBUFA_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_IMGBUFB_MASK    (0x0040u)
#define CSL_COPC_MEM_CNT_IMGBUFB_SHIFT   (0x0006u)
#define CSL_COPC_MEM_CNT_IMGBUFB_RESETVAL (0x0000u)

/*----IMGBUFB Tokens----*/
#define CSL_COPC_MEM_CNT_IMGBUFB_AUTOMODE (0x0000u)
#define CSL_COPC_MEM_CNT_IMGBUFB_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_IMGBUFC_MASK    (0x0020u)
#define CSL_COPC_MEM_CNT_IMGBUFC_SHIFT   (0x0005u)
#define CSL_COPC_MEM_CNT_IMGBUFC_RESETVAL (0x0000u)

/*----IMGBUFC Tokens----*/
#define CSL_COPC_MEM_CNT_IMGBUFC_AUTOMODE (0x0000u)
#define CSL_COPC_MEM_CNT_IMGBUFC_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_DCAC_MASK       (0x0010u)
#define CSL_COPC_MEM_CNT_DCAC_SHIFT      (0x0004u)
#define CSL_COPC_MEM_CNT_DCAC_RESETVAL   (0x0000u)

/*----DCAC Tokens----*/
#define CSL_COPC_MEM_CNT_DCAC_AUTOMODE   (0x0000u)
#define CSL_COPC_MEM_CNT_DCAC_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_SEQ1_MASK       (0x0008u)
#define CSL_COPC_MEM_CNT_SEQ1_SHIFT      (0x0003u)
#define CSL_COPC_MEM_CNT_SEQ1_RESETVAL   (0x0000u)

/*----SEQ1 Tokens----*/
#define CSL_COPC_MEM_CNT_SEQ1_AUTOMODE   (0x0000u)
#define CSL_COPC_MEM_CNT_SEQ1_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_SEQ2_MASK       (0x0004u)
#define CSL_COPC_MEM_CNT_SEQ2_SHIFT      (0x0002u)
#define CSL_COPC_MEM_CNT_SEQ2_RESETVAL   (0x0000u)

/*----SEQ2 Tokens----*/
#define CSL_COPC_MEM_CNT_SEQ2_AUTOMODE   (0x0000u)
#define CSL_COPC_MEM_CNT_SEQ2_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_SEQ3_MASK       (0x0002u)
#define CSL_COPC_MEM_CNT_SEQ3_SHIFT      (0x0001u)
#define CSL_COPC_MEM_CNT_SEQ3_RESETVAL   (0x0000u)

/*----SEQ3 Tokens----*/
#define CSL_COPC_MEM_CNT_SEQ3_AUTOMODE   (0x0000u)
#define CSL_COPC_MEM_CNT_SEQ3_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_SEQ4_MASK       (0x0001u)
#define CSL_COPC_MEM_CNT_SEQ4_SHIFT      (0x0000u)
#define CSL_COPC_MEM_CNT_SEQ4_RESETVAL   (0x0000u)

/*----SEQ4 Tokens----*/
#define CSL_COPC_MEM_CNT_SEQ4_AUTOMODE   (0x0000u)
#define CSL_COPC_MEM_CNT_SEQ4_MANUALMODE (0x0001u)

#define CSL_COPC_MEM_CNT_RESETVAL        (0x0000u)

/* SW_IMG_MD0 */

#define CSL_COPC_SW_IMG_MD0_IMGBUFA_MASK (0x0300u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFA_SHIFT (0x0008u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFA_RESETVAL (0x0000u)

/*----IMGBUFA Tokens----*/
#define CSL_COPC_SW_IMG_MD0_IMGBUFA_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFA_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFA_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD0_IMGBUFB_MASK (0x0030u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFB_SHIFT (0x0004u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFB_RESETVAL (0x0000u)

/*----IMGBUFB Tokens----*/
#define CSL_COPC_SW_IMG_MD0_IMGBUFB_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFB_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFB_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD0_IMGBUFC_MASK (0x0003u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFC_SHIFT (0x0000u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFC_RESETVAL (0x0000u)

/*----IMGBUFC Tokens----*/
#define CSL_COPC_SW_IMG_MD0_IMGBUFC_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFC_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD0_IMGBUFC_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD0_RESETVAL     (0x0000u)

/* SW_IMG_MD1 */

#define CSL_COPC_SW_IMG_MD1_IMGBUFA_MASK (0x0300u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFA_SHIFT (0x0008u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFA_RESETVAL (0x0000u)

/*----IMGBUFA Tokens----*/
#define CSL_COPC_SW_IMG_MD1_IMGBUFA_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFA_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFA_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD1_IMGBUFB_MASK (0x0030u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFB_SHIFT (0x0004u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFB_RESETVAL (0x0000u)

/*----IMGBUFB Tokens----*/
#define CSL_COPC_SW_IMG_MD1_IMGBUFB_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFB_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFB_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD1_IMGBUFC_MASK (0x0003u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFC_SHIFT (0x0000u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFC_RESETVAL (0x0000u)

/*----IMGBUFC Tokens----*/
#define CSL_COPC_SW_IMG_MD1_IMGBUFC_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFC_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD1_IMGBUFC_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD1_RESETVAL     (0x0000u)

/* SW_IMG_MD2 */

#define CSL_COPC_SW_IMG_MD2_IMGBUFA_MASK (0x0300u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFA_SHIFT (0x0008u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFA_RESETVAL (0x0000u)

/*----IMGBUFA Tokens----*/
#define CSL_COPC_SW_IMG_MD2_IMGBUFA_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFA_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFA_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD2_IMGBUFB_MASK (0x0030u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFB_SHIFT (0x0004u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFB_RESETVAL (0x0000u)

/*----IMGBUFB Tokens----*/
#define CSL_COPC_SW_IMG_MD2_IMGBUFB_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFB_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFB_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD2_IMGBUFC_MASK (0x0003u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFC_SHIFT (0x0000u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFC_RESETVAL (0x0000u)

/*----IMGBUFC Tokens----*/
#define CSL_COPC_SW_IMG_MD2_IMGBUFC_IMX  (0x0000u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFC_BIMX (0x0001u)
#define CSL_COPC_SW_IMG_MD2_IMGBUFC_DMA  (0x0002u)

#define CSL_COPC_SW_IMG_MD2_RESETVAL     (0x0000u)

/* SW_SEQ_MD0 */

#define CSL_COPC_SW_SEQ_MD0_DCAC_MASK    (0x0100u)
#define CSL_COPC_SW_SEQ_MD0_DCAC_SHIFT   (0x0008u)
#define CSL_COPC_SW_SEQ_MD0_DCAC_RESETVAL (0x0000u)

/*----DCAC Tokens----*/
#define CSL_COPC_SW_SEQ_MD0_DCAC_QIQ_DMA (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_DCAC_DMA_QIQ (0x0001u)

#define CSL_COPC_SW_SEQ_MD0_SEQ1_MASK    (0x00C0u)
#define CSL_COPC_SW_SEQ_MD0_SEQ1_SHIFT   (0x0006u)
#define CSL_COPC_SW_SEQ_MD0_SEQ1_RESETVAL (0x0000u)

/*----SEQ1 Tokens----*/
#define CSL_COPC_SW_SEQ_MD0_SEQ1_BIMX_DCT (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_SEQ1_DCT_BIMX (0x0001u)
#define CSL_COPC_SW_SEQ_MD0_SEQ1_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD0_SEQ2_MASK    (0x0030u)
#define CSL_COPC_SW_SEQ_MD0_SEQ2_SHIFT   (0x0004u)
#define CSL_COPC_SW_SEQ_MD0_SEQ2_RESETVAL (0x0000u)

/*----SEQ2 Tokens----*/
#define CSL_COPC_SW_SEQ_MD0_SEQ2_DCT_QIQ (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_SEQ2_QIQ_DCT (0x0001u)
#define CSL_COPC_SW_SEQ_MD0_SEQ2_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD0_SEQ3_MASK    (0x000Cu)
#define CSL_COPC_SW_SEQ_MD0_SEQ3_SHIFT   (0x0002u)
#define CSL_COPC_SW_SEQ_MD0_SEQ3_RESETVAL (0x0000u)

/*----SEQ3 Tokens----*/
#define CSL_COPC_SW_SEQ_MD0_SEQ3_QIQ_BPS (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_SEQ3_BPS_QIQ (0x0001u)
#define CSL_COPC_SW_SEQ_MD0_SEQ3_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD0_SEQ4_MASK    (0x0003u)
#define CSL_COPC_SW_SEQ_MD0_SEQ4_SHIFT   (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_SEQ4_RESETVAL (0x0000u)

/*----SEQ4 Tokens----*/
#define CSL_COPC_SW_SEQ_MD0_SEQ4_BPS_BPS (0x0000u)
#define CSL_COPC_SW_SEQ_MD0_SEQ4_BPS_DMA (0x0001u)
#define CSL_COPC_SW_SEQ_MD0_SEQ4_DMA_BPS (0x0002u)
#define CSL_COPC_SW_SEQ_MD0_SEQ4_DMA_DMA (0x0003u)

#define CSL_COPC_SW_SEQ_MD0_RESETVAL     (0x0000u)

/* SW_SEQ_MD1 */

#define CSL_COPC_SW_SEQ_MD1_DCAC_MASK    (0x0100u)
#define CSL_COPC_SW_SEQ_MD1_DCAC_SHIFT   (0x0008u)
#define CSL_COPC_SW_SEQ_MD1_DCAC_RESETVAL (0x0000u)

/*----DCAC Tokens----*/
#define CSL_COPC_SW_SEQ_MD1_DCAC_QIQ_DMA (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_DCAC_DMA_QIQ (0x0001u)

#define CSL_COPC_SW_SEQ_MD1_SEQ1_MASK    (0x00C0u)
#define CSL_COPC_SW_SEQ_MD1_SEQ1_SHIFT   (0x0006u)
#define CSL_COPC_SW_SEQ_MD1_SEQ1_RESETVAL (0x0000u)

/*----SEQ1 Tokens----*/
#define CSL_COPC_SW_SEQ_MD1_SEQ1_BIMX_DCT (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_SEQ1_DCT_BIMX (0x0001u)
#define CSL_COPC_SW_SEQ_MD1_SEQ1_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD1_SEQ2_MASK    (0x0030u)
#define CSL_COPC_SW_SEQ_MD1_SEQ2_SHIFT   (0x0004u)
#define CSL_COPC_SW_SEQ_MD1_SEQ2_RESETVAL (0x0000u)

/*----SEQ2 Tokens----*/
#define CSL_COPC_SW_SEQ_MD1_SEQ2_DCT_QIQ (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_SEQ2_QIQ_DCT (0x0001u)
#define CSL_COPC_SW_SEQ_MD1_SEQ2_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD1_SEQ3_MASK    (0x000Cu)
#define CSL_COPC_SW_SEQ_MD1_SEQ3_SHIFT   (0x0002u)
#define CSL_COPC_SW_SEQ_MD1_SEQ3_RESETVAL (0x0000u)

/*----SEQ3 Tokens----*/
#define CSL_COPC_SW_SEQ_MD1_SEQ3_QIQ_BPS (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_SEQ3_BPS_QIQ (0x0001u)
#define CSL_COPC_SW_SEQ_MD1_SEQ3_DMA_DMA (0x0002u)

#define CSL_COPC_SW_SEQ_MD1_SEQ4_MASK    (0x0003u)
#define CSL_COPC_SW_SEQ_MD1_SEQ4_SHIFT   (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_SEQ4_RESETVAL (0x0000u)

/*----SEQ4 Tokens----*/
#define CSL_COPC_SW_SEQ_MD1_SEQ4_BPS_BPS (0x0000u)
#define CSL_COPC_SW_SEQ_MD1_SEQ4_BPS_DMA (0x0001u)
#define CSL_COPC_SW_SEQ_MD1_SEQ4_DMA_BPS (0x0002u)
#define CSL_COPC_SW_SEQ_MD1_SEQ4_DMA_DMA (0x0003u)

#define CSL_COPC_SW_SEQ_MD1_RESETVAL     (0x0000u)

/* SW_OTHERS */

#define CSL_COPC_SW_OTHERS_IMXCOEF_MASK  (0x0400u)
#define CSL_COPC_SW_OTHERS_IMXCOEF_SHIFT (0x000Au)
#define CSL_COPC_SW_OTHERS_IMXCOEF_RESETVAL (0x0000u)

/*----IMXCOEF Tokens----*/
#define CSL_COPC_SW_OTHERS_IMXCOEF_IMX   (0x0000u)
#define CSL_COPC_SW_OTHERS_IMXCOEF_DMA   (0x0001u)

#define CSL_COPC_SW_OTHERS_IMXCMD_MASK   (0x0300u)
#define CSL_COPC_SW_OTHERS_IMXCMD_SHIFT  (0x0008u)
#define CSL_COPC_SW_OTHERS_IMXCMD_RESETVAL (0x0000u)

/*----IMXCMD Tokens----*/
#define CSL_COPC_SW_OTHERS_IMXCMD_IMX    (0x0000u)
#define CSL_COPC_SW_OTHERS_IMXCMD_BIMX   (0x0001u)
#define CSL_COPC_SW_OTHERS_IMXCMD_DMA    (0x0002u)

#define CSL_COPC_SW_OTHERS_BIMXCMD_MASK  (0x00C0u)
#define CSL_COPC_SW_OTHERS_BIMXCMD_SHIFT (0x0006u)
#define CSL_COPC_SW_OTHERS_BIMXCMD_RESETVAL (0x0000u)

/*----BIMXCMD Tokens----*/
#define CSL_COPC_SW_OTHERS_BIMXCMD_IMX   (0x0000u)
#define CSL_COPC_SW_OTHERS_BIMXCMD_BIMX  (0x0001u)
#define CSL_COPC_SW_OTHERS_BIMXCMD_DMA   (0x0002u)

#define CSL_COPC_SW_OTHERS_RESERVED_MASK (0x0020u)
#define CSL_COPC_SW_OTHERS_RESERVED_SHIFT (0x0005u)
#define CSL_COPC_SW_OTHERS_RESERVED_RESETVAL (0x0000u)

#define CSL_COPC_SW_OTHERS_BIMXREF_MASK  (0x0010u)
#define CSL_COPC_SW_OTHERS_BIMXREF_SHIFT (0x0004u)
#define CSL_COPC_SW_OTHERS_BIMXREF_RESETVAL (0x0000u)

/*----BIMXREF Tokens----*/
#define CSL_COPC_SW_OTHERS_BIMXREF_BIMX  (0x0000u)
#define CSL_COPC_SW_OTHERS_BIMXREF_DMA   (0x0001u)

#define CSL_COPC_SW_OTHERS_BIMXORG_MASK  (0x0008u)
#define CSL_COPC_SW_OTHERS_BIMXORG_SHIFT (0x0003u)
#define CSL_COPC_SW_OTHERS_BIMXORG_RESETVAL (0x0000u)

/*----BIMXORG Tokens----*/
#define CSL_COPC_SW_OTHERS_BIMXORG_BIMX  (0x0000u)
#define CSL_COPC_SW_OTHERS_BIMXORG_DMA   (0x0001u)

#define CSL_COPC_SW_OTHERS_QIQ_MASK      (0x0004u)
#define CSL_COPC_SW_OTHERS_QIQ_SHIFT     (0x0002u)
#define CSL_COPC_SW_OTHERS_QIQ_RESETVAL  (0x0000u)

/*----QIQ Tokens----*/
#define CSL_COPC_SW_OTHERS_QIQ_QIQ       (0x0000u)
#define CSL_COPC_SW_OTHERS_QIQ_DMA       (0x0001u)

#define CSL_COPC_SW_OTHERS_MV_MASK       (0x0002u)
#define CSL_COPC_SW_OTHERS_MV_SHIFT      (0x0001u)
#define CSL_COPC_SW_OTHERS_MV_RESETVAL   (0x0000u)

/*----MV Tokens----*/
#define CSL_COPC_SW_OTHERS_MV_BP         (0x0000u)
#define CSL_COPC_SW_OTHERS_MV_DMA        (0x0001u)

#define CSL_COPC_SW_OTHERS_HUFF_MASK     (0x0001u)
#define CSL_COPC_SW_OTHERS_HUFF_SHIFT    (0x0000u)
#define CSL_COPC_SW_OTHERS_HUFF_RESETVAL (0x0000u)

/*----HUFF Tokens----*/
#define CSL_COPC_SW_OTHERS_HUFF_VLCD     (0x0000u)
#define CSL_COPC_SW_OTHERS_HUFF_DMA      (0x0001u)

#define CSL_COPC_SW_OTHERS_RESETVAL      (0x0000u)

/* CNT_3 */

#define CSL_COPC_CNT_3_SLICE_MASK        (0xFFFFu)
#define CSL_COPC_CNT_3_SLICE_SHIFT       (0x0000u)
#define CSL_COPC_CNT_3_SLICE_RESETVAL    (0xFFFFu)

#define CSL_COPC_CNT_3_RESETVAL          (0xFFFFu)

/* CNT_4 */

#define CSL_COPC_CNT_4_EXTVERT_MASK      (0x1000u)
#define CSL_COPC_CNT_4_EXTVERT_SHIFT     (0x000Cu)
#define CSL_COPC_CNT_4_EXTVERT_RESETVAL  (0x0000u)

/*----EXTVERT Tokens----*/
#define CSL_COPC_CNT_4_EXTVERT_NOEXT     (0x0000u)
#define CSL_COPC_CNT_4_EXTVERT_EXT       (0x0001u)

#define CSL_COPC_CNT_4_EXTHORZ_MASK      (0x0100u)
#define CSL_COPC_CNT_4_EXTHORZ_SHIFT     (0x0008u)
#define CSL_COPC_CNT_4_EXTHORZ_RESETVAL  (0x0000u)

/*----EXTHORZ Tokens----*/
#define CSL_COPC_CNT_4_EXTHORZ_NOEXT     (0x0000u)
#define CSL_COPC_CNT_4_EXTHORZ_EXT       (0x0001u)

#define CSL_COPC_CNT_4_RESERVED_MASK     (0x00FCu)
#define CSL_COPC_CNT_4_RESERVED_SHIFT    (0x0002u)
#define CSL_COPC_CNT_4_RESERVED_RESETVAL (0x0000u)

#define CSL_COPC_CNT_4_EXTSLICE_MASK     (0x0003u)
#define CSL_COPC_CNT_4_EXTSLICE_SHIFT    (0x0000u)
#define CSL_COPC_CNT_4_EXTSLICE_RESETVAL (0x0000u)

/*----EXTSLICE Tokens----*/
#define CSL_COPC_CNT_4_EXTSLICE_NOEXT    (0x0000u)
#define CSL_COPC_CNT_4_EXTSLICE_EXT      (0x0001u)

#define CSL_COPC_CNT_4_RESETVAL          (0x0000u)

/* RSVD0 */

#define CSL_COPC_RSVD0_RESETVAL          (0x0000u)

/* RSVD1 */

#define CSL_COPC_RSVD1_RESETVAL          (0x0000u)

/* INT_STAT */

#define CSL_COPC_INT_STAT_STAT_MASK      (0x00FFu)
#define CSL_COPC_INT_STAT_STAT_SHIFT     (0x0000u)
#define CSL_COPC_INT_STAT_STAT_RESETVAL  (0x0000u)

/*----STAT Tokens----*/
#define CSL_COPC_INT_STAT_STAT_EDMA      (0x0080u)
#define CSL_COPC_INT_STAT_STAT_RSVD      (0x0040u)
#define CSL_COPC_INT_STAT_STAT_RATE      (0x0020u)
#define CSL_COPC_INT_STAT_STAT_BP        (0x0010u)
#define CSL_COPC_INT_STAT_STAT_QIQ       (0x0008u)
#define CSL_COPC_INT_STAT_STAT_DCT       (0x0004u)
#define CSL_COPC_INT_STAT_STAT_BIMX      (0x0002u)
#define CSL_COPC_INT_STAT_STAT_IMX       (0x0000u)

#define CSL_COPC_INT_STAT_RESETVAL       (0x0000u)

/* PROG_INT */

#define CSL_COPC_PROG_INT_MODE_MASK      (0x8000u)
#define CSL_COPC_PROG_INT_MODE_SHIFT     (0x000Fu)
#define CSL_COPC_PROG_INT_MODE_RESETVAL  (0x0000u)

/*----MODE Tokens----*/
#define CSL_COPC_PROG_INT_MODE_NORMAL    (0x0000u)
#define CSL_COPC_PROG_INT_MODE_PROGRAMMABLE (0x0001u)

#define CSL_COPC_PROG_INT_START_MASK     (0x00FFu)
#define CSL_COPC_PROG_INT_START_SHIFT    (0x0000u)
#define CSL_COPC_PROG_INT_START_RESETVAL (0x0000u)

/*----START Tokens----*/
#define CSL_COPC_PROG_INT_START_EDMA     (0x0080u)
#define CSL_COPC_PROG_INT_START_RSVD     (0x0040u)
#define CSL_COPC_PROG_INT_START_RATE     (0x0020u)
#define CSL_COPC_PROG_INT_START_BP       (0x0010u)
#define CSL_COPC_PROG_INT_START_QIQ      (0x0008u)
#define CSL_COPC_PROG_INT_START_DCT      (0x0004u)
#define CSL_COPC_PROG_INT_START_BIMX     (0x0002u)
#define CSL_COPC_PROG_INT_START_IMX      (0x0000u)

#define CSL_COPC_PROG_INT_RESETVAL       (0x0000u)

/* INFO_STAT */

#define CSL_COPC_INFO_STAT_STAT_MASK     (0x003Fu)
#define CSL_COPC_INFO_STAT_STAT_SHIFT    (0x0000u)
#define CSL_COPC_INFO_STAT_STAT_RESETVAL (0x0000u)

/*----STAT Tokens----*/
#define CSL_COPC_INFO_STAT_STAT_UPPREDDIS (0x0020u)
#define CSL_COPC_INFO_STAT_STAT_LEFTPREDDIS (0x0010u)
#define CSL_COPC_INFO_STAT_STAT_FIRSTROWMB (0x0008u)
#define CSL_COPC_INFO_STAT_STAT_LASTROWMB (0x0004u)
#define CSL_COPC_INFO_STAT_STAT_FIRSTCOLMB (0x0002u)
#define CSL_COPC_INFO_STAT_STAT_LASTCOLMB (0x0000u)

#define CSL_COPC_INFO_STAT_RESETVAL      (0x0000u)

#endif
