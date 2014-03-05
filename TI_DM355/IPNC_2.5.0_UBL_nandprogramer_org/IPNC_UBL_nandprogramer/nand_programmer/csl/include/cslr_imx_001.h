#ifndef _CSLR_IMX_1_H_
#define _CSLR_IMX_1_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_imx_1.h
 * 
 * \brief This file contains the Register Desciptions for IMX
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 START;
    volatile Uint16 CMDADR;
    volatile Uint16 CMDPTR;
    volatile Uint16 CLKCNTRL;
    volatile Uint16 MINIMUM_ID;
    volatile Uint16 MINIMUM_VALUE;
    volatile Uint16 BRKPTR;
} CSL_ImxRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* START */

#define CSL_IMX_START_START_MASK         (0x0001u)
#define CSL_IMX_START_START_SHIFT        (0x0000u)
#define CSL_IMX_START_START_RESETVAL     (0x0000u)

/*----START Tokens----*/
#define CSL_IMX_START_START_IDLE         (0x0000u)
#define CSL_IMX_START_START_BUSY         (0x0001u)

#define CSL_IMX_START_RESETVAL           (0x0000u)

/* CMDADR */

#define CSL_IMX_CMDADR_CMDADR_MASK       (0x07FFu)
#define CSL_IMX_CMDADR_CMDADR_SHIFT      (0x0000u)
#define CSL_IMX_CMDADR_CMDADR_RESETVAL   (0x0000u)

#define CSL_IMX_CMDADR_RESETVAL          (0x0000u)

/* CMDPTR */

#define CSL_IMX_CMDPTR_CMDPTR_MASK       (0x07FFu)
#define CSL_IMX_CMDPTR_CMDPTR_SHIFT      (0x0000u)
#define CSL_IMX_CMDPTR_CMDPTR_RESETVAL   (0x0000u)

#define CSL_IMX_CMDPTR_RESETVAL          (0x0000u)

/* CLKCNTRL */

#define CSL_IMX_CLKCNTRL_CLKCNTRL_MASK   (0x0001u)
#define CSL_IMX_CLKCNTRL_CLKCNTRL_SHIFT  (0x0000u)
#define CSL_IMX_CLKCNTRL_CLKCNTRL_RESETVAL (0x0000u)

/*----CLKCNTRL Tokens----*/
#define CSL_IMX_CLKCNTRL_CLKCNTRL_ON     (0x0000u)
#define CSL_IMX_CLKCNTRL_CLKCNTRL_OFF    (0x0001u)

#define CSL_IMX_CLKCNTRL_RESETVAL        (0x0000u)

/* MINIMUM_ID */

#define CSL_IMX_MINIMUM_ID_ID_MASK       (0xFFFFu)
#define CSL_IMX_MINIMUM_ID_ID_SHIFT      (0x0000u)
#define CSL_IMX_MINIMUM_ID_ID_RESETVAL   (0x0000u)

#define CSL_IMX_MINIMUM_ID_RESETVAL      (0x0000u)

/* MINIMUM_VALUE */

#define CSL_IMX_MINIMUM_VALUE_VALUE_MASK (0xFFFFu)
#define CSL_IMX_MINIMUM_VALUE_VALUE_SHIFT (0x0000u)
#define CSL_IMX_MINIMUM_VALUE_VALUE_RESETVAL (0x0000u)

#define CSL_IMX_MINIMUM_VALUE_RESETVAL   (0x0000u)

/* BRKPTR */

#define CSL_IMX_BRKPTR_STATUS_MASK       (0x2000u)
#define CSL_IMX_BRKPTR_STATUS_SHIFT      (0x000Du)
#define CSL_IMX_BRKPTR_STATUS_RESETVAL   (0x0000u)

#define CSL_IMX_BRKPTR_ENABLE_MASK       (0x1000u)
#define CSL_IMX_BRKPTR_ENABLE_SHIFT      (0x000Cu)
#define CSL_IMX_BRKPTR_ENABLE_RESETVAL   (0x0000u)

/*----ENABLE Tokens----*/
#define CSL_IMX_BRKPTR_ENABLE_DISABLE    (0x0000u)
#define CSL_IMX_BRKPTR_ENABLE_ENABLE     (0x0001u)

#define CSL_IMX_BRKPTR_RESERVED_MASK     (0x0800u)
#define CSL_IMX_BRKPTR_RESERVED_SHIFT    (0x000Bu)
#define CSL_IMX_BRKPTR_RESERVED_RESETVAL (0x0000u)

#define CSL_IMX_BRKPTR_PTR_MASK          (0x07FFu)
#define CSL_IMX_BRKPTR_PTR_SHIFT         (0x0000u)
#define CSL_IMX_BRKPTR_PTR_RESETVAL      (0x0000u)

#define CSL_IMX_BRKPTR_RESETVAL          (0x0000u)

#endif
