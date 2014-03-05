#ifndef _CSLR_VPSSBL_H_
#define _CSLR_VPSSBL_H_

#include <cslr.h>
#include "cslr_vpssbl_001.h"
extern void * _CSL_vpssbllookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_VPSSBL_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_VpssblRegs *  CSL_VpssblRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_VPSSBL_0_REGS                 ((CSL_VpssblRegsOvly) 0x01c70800)


#endif
