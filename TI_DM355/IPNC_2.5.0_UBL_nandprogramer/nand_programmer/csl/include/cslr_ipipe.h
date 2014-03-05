#ifndef _CSLR_IPIPE_H_
#define _CSLR_IPIPE_H_

#include <cslr.h>
#include "cslr_ipipe_001.h"
#include <dm350.h>
extern void * _CSL_ipipelookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_IPIPE_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_IpipeRegs *  CSL_IpipeRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_IPIPE_0_REGS                 ((CSL_IpipeRegsOvly) 0x01c71000)


#endif
