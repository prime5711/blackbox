/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_emif.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _CSLR_EMIF_H_
#define _CSLR_EMIF_H_

#include <cslr_aemif_003_DM350.h>
// #include <cslr_emif_001.h> Davinci
typedef volatile CSL_EmifRegs *CSL_EmifRegsOvly;

//#define CSL_EMIF_0_REGS ((CSL_EmifRegsOvly) 0x01E00000) DaVinci
#define CSL_EMIF_0_REGS ((CSL_EmifRegsOvly) 0x01E10000)

#endif/*_CSLR_EMIF_H_*/
