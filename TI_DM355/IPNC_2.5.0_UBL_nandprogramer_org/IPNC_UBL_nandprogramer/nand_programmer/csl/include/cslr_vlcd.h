#ifndef _CSLR_VLCD_H_
#define _CSLR_VLCD_H_

#include <cslr.h>
#include "cslr_vlcd_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_VLCD_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_VlcdRegs *  CSL_VlcdRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_VLCD_0_REGS                 ((CSL_VlcdRegsOvly) 0x01cd0280)


#endif
