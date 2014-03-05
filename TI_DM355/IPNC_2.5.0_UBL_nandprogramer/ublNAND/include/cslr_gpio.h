/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_edma.h 												      */
/*	PURPOSE	: GPIO CSL file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _CSLR_GPIO_H_
#define _CSLR_GPIO_H_

#include "cslr_gpio_001.h"

#define CSL_GPIO_PER_CNT          1

typedef volatile CSL_GpioRegs *  CSL_GpioRegsOvly;

#define CSL_GPIO_REGS                 ((CSL_GpioRegsOvly) 0x01c67000)

#endif /*_CSLR_EDMA_H_*/

