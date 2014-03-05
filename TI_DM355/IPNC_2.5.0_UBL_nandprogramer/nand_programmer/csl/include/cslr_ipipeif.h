#ifndef _CSLR_IPIPEIF_H_
#define _CSLR_IPIPEIF_H_

#include <cslr.h>
#include "cslr_ipipeif_001.h"
extern void * _CSL_ipipeiflookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_IPIPEIF_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_IpipeifRegs *  CSL_IpipeifRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_IPIPEIF_0_REGS                 ((CSL_IpipeifRegsOvly) 0x01c70100)


#endif
