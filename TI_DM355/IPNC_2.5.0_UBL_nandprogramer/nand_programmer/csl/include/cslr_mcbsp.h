#ifndef _CSLR_MCBSP_H_
#define _CSLR_MCBSP_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_mcbsp_001.h"
extern void * _CSL_mcbsplookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_MCBSP_PER_CNT          2

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_McbspRegs *  CSL_McbspRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_MCBSP_0_REGS                 ((CSL_McbspRegsOvly) 0x01e02000)
#define CSL_MCBSP_1_REGS                 ((CSL_McbspRegsOvly) 0x01e04000)



CSL_IDEF_INLINE CSL_McbspRegsOvly _CSL_mcbspGetBaseAddr (Uint16 mcbspNum) {
  return (CSL_McbspRegsOvly)_CSL_mcbsplookup[mcbspNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:50    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
