#ifndef _CSLR_HSSI_H_
#define _CSLR_HSSI_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_hssi_001.h"
extern void * _CSL_hssilookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_HSSI_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_HssiRegs *  CSL_HssiRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_HSSI_0_REGS                 ((CSL_HssiRegsOvly) 0x01c70300)


#endif
