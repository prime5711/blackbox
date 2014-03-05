#ifndef _CSLR_MMCSD_H_
#define _CSLR_MMCSD_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_mmcsd_001.h"
extern void * _CSL_mmcsdlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_MMCSD_PER_CNT          2

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_MmcsdRegs *  CSL_MmcsdRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_MMCSD_0_REGS                 ((CSL_MmcsdRegsOvly) 0x01e11000)
#define CSL_MMCSD_1_REGS                 ((CSL_MmcsdRegsOvly) 0x01e00000)

/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_MmcsdRegsOvly _CSL_mmcsdGetBaseAddr (Uint16 mmcsdNum) {
  return (CSL_MmcsdRegsOvly)_CSL_mmcsdlookup[mmcsdNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:51    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
