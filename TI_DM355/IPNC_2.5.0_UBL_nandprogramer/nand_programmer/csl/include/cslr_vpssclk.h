#ifndef _CSLR_VPSSCLK_H_
#define _CSLR_VPSSCLK_H_

#include <cslr.h>
#include "cslr_vpssclk_001.h"
extern void * _CSL_vpssclklookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_VPSSCLK_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_VpssclkRegs *  CSL_VpssclkRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_VPSSCLK_0_REGS                 ((CSL_VpssclkRegsOvly) 0x01c70000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:01    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
