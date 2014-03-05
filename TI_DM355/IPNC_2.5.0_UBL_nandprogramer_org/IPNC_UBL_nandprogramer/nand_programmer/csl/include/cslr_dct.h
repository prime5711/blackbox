#ifndef _CSLR_DCT_H_
#define _CSLR_DCT_H_

#include <cslr.h>
#include "cslr_dct_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_DCT_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_DctRegs *  CSL_DctRegsOvly;

/**************************************************************************\
* Peripheral Base Address - commented by Vishwa because Redefinition in DM350.h
\**************************************************************************/
//#define CSL_DCT_0_REGS                 ((CSL_DctRegsOvly) 0x01cd0100)


#endif
