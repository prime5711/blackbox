#ifndef _CSLR_RCNT_H_
#define _CSLR_RCNT_H_

#include <cslr.h>
#include "cslr_rcnt_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_RCNT_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_RcntRegs *  CSL_RcntRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_RCNT_0_REGS                 ((CSL_RcntRegsOvly) 0x01cd0300)


#endif
