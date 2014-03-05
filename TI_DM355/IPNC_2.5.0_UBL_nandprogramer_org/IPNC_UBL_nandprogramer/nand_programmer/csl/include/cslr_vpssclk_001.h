#ifndef _CSLR_VPSSCLK_VPSS_SPEC_V04_H_
#define _CSLR_VPSSCLK_VPSS_SPEC_V04_H_
/*********************************************************************
 * Copyright (C) 2003-2006 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_vpssclk_vpss_spec_v04.h
 * 
 * \brief This file contains the Register Desciptions for VPSSCLK
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PID;
    volatile Uint32 CLKCTRL;
} CSL_VpssclkRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* PID */

#define CSL_VPSSCLK_PID_TID_MASK         (0x00FF0000u)
#define CSL_VPSSCLK_PID_TID_SHIFT        (0x00000010u)
#define CSL_VPSSCLK_PID_TID_RESETVAL     (0x00000001u)

#define CSL_VPSSCLK_PID_CID_MASK         (0x0000FF00u)
#define CSL_VPSSCLK_PID_CID_SHIFT        (0x00000008u)
#define CSL_VPSSCLK_PID_CID_RESETVAL     (0x000000FFu)

#define CSL_VPSSCLK_PID_PREV_MASK        (0x000000FFu)
#define CSL_VPSSCLK_PID_PREV_SHIFT       (0x00000000u)
#define CSL_VPSSCLK_PID_PREV_RESETVAL    (0x00000001u)

#define CSL_VPSSCLK_PID_RESETVAL         (0x0001FF01u)

/* CLKCTRL */

#define CSL_VPSSCLK_CLKCTRL_CCDC_CLK_MASK (0x00000040u)
#define CSL_VPSSCLK_CLKCTRL_CCDC_CLK_SHIFT (0x00000006u)
#define CSL_VPSSCLK_CLKCTRL_CCDC_CLK_RESETVAL (0x00000001u)

/*----CCDC_CLK Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_CCDC_CLK_OFF (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_CCDC_CLK_ON  (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_IPIPE_CLK_MASK (0x00000020u)
#define CSL_VPSSCLK_CLKCTRL_IPIPE_CLK_SHIFT (0x00000005u)
#define CSL_VPSSCLK_CLKCTRL_IPIPE_CLK_RESETVAL (0x00000001u)

/*----IPIPE_CLK Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_IPIPE_CLK_OFF (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_IPIPE_CLK_ON (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_H3A_CLK_MASK (0x00000010u)
#define CSL_VPSSCLK_CLKCTRL_H3A_CLK_SHIFT (0x00000004u)
#define CSL_VPSSCLK_CLKCTRL_H3A_CLK_RESETVAL (0x00000001u)

/*----H3A_CLK Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_H3A_CLK_OFF  (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_H3A_CLK_ON   (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_CFALD_CLK_MASK (0x00000008u)
#define CSL_VPSSCLK_CLKCTRL_CFALD_CLK_SHIFT (0x00000003u)
#define CSL_VPSSCLK_CLKCTRL_CFALD_CLK_RESETVAL (0x00000001u)

/*----CFALD_CLK Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_CFALD_CLK_OFF (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_CFALD_CLK_ON (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_VENC_CLKSEL_MASK (0x00000004u)
#define CSL_VPSSCLK_CLKCTRL_VENC_CLKSEL_SHIFT (0x00000002u)
#define CSL_VPSSCLK_CLKCTRL_VENC_CLKSEL_RESETVAL (0x00000000u)

/*----VENC_CLKSEL Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_VENC_CLKSEL_ENCCLK (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_VENC_CLKSEL_HALFENCCLK (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_VPBE_CLK_MASK (0x00000001u)
#define CSL_VPSSCLK_CLKCTRL_VPBE_CLK_SHIFT (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_VPBE_CLK_RESETVAL (0x00000001u)

/*----VPBE_CLK Tokens----*/
#define CSL_VPSSCLK_CLKCTRL_VPBE_CLK_OFF (0x00000000u)
#define CSL_VPSSCLK_CLKCTRL_VPBE_CLK_ON  (0x00000001u)

#define CSL_VPSSCLK_CLKCTRL_RESETVAL     (0x00000079u)

#endif
