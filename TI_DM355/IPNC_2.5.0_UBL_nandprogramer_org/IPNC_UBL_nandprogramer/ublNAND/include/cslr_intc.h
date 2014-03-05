/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_intc.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/
#ifndef _CSLR_INTC1_H_
#define _CSLR_INTC1_H_

#include <cslr_intc_001.h>
typedef volatile CSL_IntcRegs *CSL_Intc1RegsOvly;
#define CSL_INTC1_0_REGS ((CSL_Intc1RegsOvly) 0x01C48000)

#endif/*_CSLR_INTC1_H_*/
