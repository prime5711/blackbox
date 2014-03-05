#ifndef _CSLR_COPC_H_
#define _CSLR_COPC_H_

#include <cslr.h>
#include "cslr_copc_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_COPC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_CopcRegs *  CSL_CopcRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_COPC_0_REGS                 ((CSL_CopcRegsOvly) 0x01cd0380)


#endif
