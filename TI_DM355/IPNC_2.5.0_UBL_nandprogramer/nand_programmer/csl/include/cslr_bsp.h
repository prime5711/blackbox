#ifndef _CSLR_BSP_H_
#define _CSLR_BSP_H_

#include <cslr.h>
#include "cslr_bsp_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_BSP_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_BspRegs *  CSL_BspRegsOvly;

/**************************************************************************\
* Peripheral Base Address commented because Redefinition in DM350.h
\**************************************************************************/
//#define CSL_BSP_0_REGS                 ((CSL_BspRegsOvly) 0x01cd0200)


#endif
