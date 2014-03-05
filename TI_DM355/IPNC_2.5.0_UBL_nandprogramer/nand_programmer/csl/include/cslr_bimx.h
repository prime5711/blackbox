#ifndef _CSLR_BIMX_H_
#define _CSLR_BIMX_H_

#include <cslr.h>
#include "cslr_bimx_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_BIMX_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_BimxRegs *  CSL_BimxRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_BIMX_0_REGS                 ((CSL_BimxRegsOvly) 0x01cd0080)


#endif
