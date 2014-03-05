/*This is Temprory data, created for compile purpose, indivisual module owners should overwrite can overwrite this file */

#ifndef _CSLR_IMX_H_
#define _CSLR_IMX_H_

#include <cslr.h>
#include "cslr_imx_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_IMX_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_ImxRegs *  CSL_ImxRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_IMX_0_REGS                 ((CSL_ImxRegsOvly) 0x01cd0000)


#endif
