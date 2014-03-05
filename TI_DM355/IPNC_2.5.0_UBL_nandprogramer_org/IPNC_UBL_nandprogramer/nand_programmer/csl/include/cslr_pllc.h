#ifndef _CSLR_PLLC_H_
#define _CSLR_PLLC_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_pllc_001.h"
extern void * _CSL_pllclookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_PLLC_PER_CNT          2

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_PllcRegs *  CSL_PllcRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_PLLC_0_REGS                 ((CSL_PllcRegsOvly) 0x01c40800)
#define CSL_PLLC_1_REGS                 ((CSL_PllcRegsOvly) 0x01c40c00)

/*****************************************************************************\
              Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_PllcRegsOvly _CSL_pllcGetBaseAddr(Uint16 pllcNum) {
  return (CSL_PllcRegsOvly)_CSL_pllclookup[pllcNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:53    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
