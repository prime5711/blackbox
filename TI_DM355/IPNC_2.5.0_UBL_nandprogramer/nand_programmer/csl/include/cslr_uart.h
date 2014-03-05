#ifndef _CSLR_UART_H_
#define _CSLR_UART_H_

#include <cslr.h>
#include "cslr_uart_001.h"
#include <dm350.h>
extern void * _CSL_uartlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_UART_PER_CNT          3

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_UartRegs *  CSL_UartRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_UART_0_REGS                 ((CSL_UartRegsOvly) 0x01c20000)
#define CSL_UART_1_REGS                 ((CSL_UartRegsOvly) 0x01c20400)
#define CSL_UART_2_REGS                 ((CSL_UartRegsOvly) 0x01e06000)


/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_UartRegsOvly _CSL_uartGetBaseAddr (Uint16 uartNum) {
  return (CSL_UartRegsOvly)_CSL_uartlookup[uartNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:58    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
