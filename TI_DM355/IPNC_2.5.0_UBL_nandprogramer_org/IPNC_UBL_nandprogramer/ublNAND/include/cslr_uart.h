/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: cslr_uart.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _CSLR_UART_H_
#define _CSLR_UART_H_

#include <cslr_uart_001.h>
typedef volatile CSL_UartRegs *CSL_UartRegsOvly;
#define CSL_UART_0_REGS                 ((CSL_UartRegsOvly) 0x01C20000)         
#define CSL_UART_1_REGS                 ((CSL_UartRegsOvly) 0x01C20400)         
#define CSL_UART_2_REGS                 ((CSL_UartRegsOvly) 0x01C20800)         


#endif/*_CSLR_EMIF_H_*/

