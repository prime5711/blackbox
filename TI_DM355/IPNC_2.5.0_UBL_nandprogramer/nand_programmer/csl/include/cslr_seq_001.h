#ifndef _CSLR_SEQ_1_H_
#define _CSLR_SEQ_1_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_seq_1.h
 * 
 * \brief This file contains the Register Desciptions for SEQ
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 CID;
    volatile Uint16 PID;
    volatile Uint16 CTRL;
    volatile Uint16 RSVD0;
    volatile Uint16 BOOT;
    volatile Uint16 RSVD1;
    volatile Uint16 AREG;
    volatile Uint16 RSVD2;
    volatile Uint16 BREG;
    volatile Uint16 RSVD3;
    volatile Uint16 CREG;
    volatile Uint16 RSVD4;
    volatile Uint16 PREG;
    volatile Uint16 RSVD5;
    volatile Uint16 P2REG;
    volatile Uint16 RSVD6;
    volatile Uint16 PCREG;
    volatile Uint16 RSVD7;
    volatile Uint16 ALUSTAT;
} CSL_SeqRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* CID */

#define CSL_SEQ_CID_CID_MASK             (0xFF00u)
#define CSL_SEQ_CID_CID_SHIFT            (0x0008u)
#define CSL_SEQ_CID_CID_RESETVAL         (0x00C0u)

#define CSL_SEQ_CID_PRN_MASK             (0x00FFu)
#define CSL_SEQ_CID_PRN_SHIFT            (0x0000u)
#define CSL_SEQ_CID_PRN_RESETVAL         (0x0001u)

#define CSL_SEQ_CID_RESETVAL             (0xC001u)

/* PID */

#define CSL_SEQ_PID_PID_MASK             (0x00FFu)
#define CSL_SEQ_PID_PID_SHIFT            (0x0000u)
#define CSL_SEQ_PID_PID_RESETVAL         (0x0001u)

#define CSL_SEQ_PID_RESETVAL             (0x0001u)

/* CTRL */

#define CSL_SEQ_CTRL_RUN_MASK            (0x0001u)
#define CSL_SEQ_CTRL_RUN_SHIFT           (0x0000u)
#define CSL_SEQ_CTRL_RUN_RESETVAL        (0x0000u)

/*----RUN Tokens----*/
#define CSL_SEQ_CTRL_RUN_IDLE            (0x0000u)
#define CSL_SEQ_CTRL_RUN_START           (0x0001u)

#define CSL_SEQ_CTRL_RESETVAL            (0x0000u)

/* RSVD0 */

#define CSL_SEQ_RSVD0_RESETVAL           (0x0000u)

/* BOOT */

#define CSL_SEQ_BOOT_ADDR_MASK           (0x07FFu)
#define CSL_SEQ_BOOT_ADDR_SHIFT          (0x0000u)
#define CSL_SEQ_BOOT_ADDR_RESETVAL       (0x0000u)

#define CSL_SEQ_BOOT_RESETVAL            (0x0000u)

/* RSVD1 */

#define CSL_SEQ_RSVD1_RESETVAL           (0x0000u)

/* AREG */

#define CSL_SEQ_AREG_AREG_MASK           (0xFFFFu)
#define CSL_SEQ_AREG_AREG_SHIFT          (0x0000u)
#define CSL_SEQ_AREG_AREG_RESETVAL       (0x0000u)

#define CSL_SEQ_AREG_RESETVAL            (0x0000u)

/* RSVD2 */

#define CSL_SEQ_RSVD2_RESETVAL           (0x0000u)

/* BREG */

#define CSL_SEQ_BREG_BREG_MASK           (0xFFFFu)
#define CSL_SEQ_BREG_BREG_SHIFT          (0x0000u)
#define CSL_SEQ_BREG_BREG_RESETVAL       (0x0000u)

#define CSL_SEQ_BREG_RESETVAL            (0x0000u)

/* RSVD3 */

#define CSL_SEQ_RSVD3_RESETVAL           (0x0000u)

/* CREG */

#define CSL_SEQ_CREG_CREG_MASK           (0xFFFFu)
#define CSL_SEQ_CREG_CREG_SHIFT          (0x0000u)
#define CSL_SEQ_CREG_CREG_RESETVAL       (0x0000u)

#define CSL_SEQ_CREG_RESETVAL            (0x0000u)

/* RSVD4 */

#define CSL_SEQ_RSVD4_RESETVAL           (0x0000u)

/* PREG */

#define CSL_SEQ_PREG_P1REG_MASK          (0xFFFFu)
#define CSL_SEQ_PREG_P1REG_SHIFT         (0x0000u)
#define CSL_SEQ_PREG_P1REG_RESETVAL      (0x0000u)

#define CSL_SEQ_PREG_RESETVAL            (0x0000u)

/* RSVD5 */

#define CSL_SEQ_RSVD5_RESETVAL           (0x0000u)

/* P2REG */

#define CSL_SEQ_P2REG_P2REG_MASK         (0xFFFFu)
#define CSL_SEQ_P2REG_P2REG_SHIFT        (0x0000u)
#define CSL_SEQ_P2REG_P2REG_RESETVAL     (0x0000u)

#define CSL_SEQ_P2REG_RESETVAL           (0x0000u)

/* RSVD6 */

#define CSL_SEQ_RSVD6_RESETVAL           (0x0000u)

/* PCREG */

#define CSL_SEQ_PCREG_PCREG_MASK         (0xFFFFu)
#define CSL_SEQ_PCREG_PCREG_SHIFT        (0x0000u)
#define CSL_SEQ_PCREG_PCREG_RESETVAL     (0x0000u)

#define CSL_SEQ_PCREG_RESETVAL           (0x0000u)

/* RSVD7 */

#define CSL_SEQ_RSVD7_RESETVAL           (0x0000u)

/* ALUSTAT */

#define CSL_SEQ_ALUSTAT_ALUSTAT_MASK     (0xFFFFu)
#define CSL_SEQ_ALUSTAT_ALUSTAT_SHIFT    (0x0000u)
#define CSL_SEQ_ALUSTAT_ALUSTAT_RESETVAL (0x0000u)

/*----ALUSTAT Tokens----*/
#define CSL_SEQ_ALUSTAT_ALUSTAT_NOOVERFLOW (0x0000u)
#define CSL_SEQ_ALUSTAT_ALUSTAT_CARRY    (0x0001u)

#define CSL_SEQ_ALUSTAT_RESETVAL         (0x0000u)

#endif
