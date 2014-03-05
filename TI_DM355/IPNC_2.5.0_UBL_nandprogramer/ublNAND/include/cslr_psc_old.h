/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_psc.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/
#ifndef _CSLR_PSC_H_
#define _CSLR_PSC_H_

#include <cslr_psc_001.h>
typedef volatile CSL_PscRegs *CSL_PscRegsOvly;
#define CSL_PSC_0_REGS       ((CSL_PscRegsOvly) 0X01C41000)

#endif/*_CSLR_PSC_H_*/

