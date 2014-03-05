#ifndef _CSLR_GPIO_H_
#define _CSLR_GPIO_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_gpio_001.h"
extern void * _CSL_gpiolookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_GPIO_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_GpioRegs *  CSL_GpioRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_GPIO_REGS                 ((CSL_GpioRegsOvly) 0x01c67000)


CSL_IDEF_INLINE CSL_GpioRegsOvly _CSL_gpioGetBaseAddr (Uint16 gpioNum) {
  return (CSL_GpioRegsOvly)_CSL_gpiolookup[gpioNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:44    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
