/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_mmcsd.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _CSLR_MMCSD_H_
#define _CSLR_MMCSD_H_

#include "cslr_mmcsd_001.h"

typedef volatile CSL_MmcsdRegs *  CSL_MmcsdRegsOvly;

#define CSL_MMCSD_PER_CNT          2

//#define CSL_EMIF_0_REGS ((CSL_EmifRegsOvly) 0x01E00000) DaVinci
#define CSL_MMCSD_0_REGS                 ((CSL_MmcsdRegsOvly) 0x01E11000)
#define CSL_MMCSD_1_REGS                 ((CSL_MmcsdRegsOvly) 0x01E00000)// Not used by RBL

#endif/*_CSLR_MMCSD_H_*/


