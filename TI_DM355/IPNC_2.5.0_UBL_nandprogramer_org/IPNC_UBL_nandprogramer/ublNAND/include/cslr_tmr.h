/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_tmr.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _CSLR_TMR_H_
#define _CSLR_TMR_H_

#include <cslr_tmr_001.h>

typedef volatile CSL_TmrRegs *CSL_TmrRegsOvly;
#define CSL_TMR_0_REGS ((CSL_TmrRegsOvly) 0X01C21400)
#define CSL_TMR_1_REGS ((CSL_TmrRegsOvly) 0X01C21800)
#define CSL_TMR_2_REGS ((CSL_TmrRegsOvly) 0X01C21C00)

#endif/*_CSLR_TMR_H_*/
