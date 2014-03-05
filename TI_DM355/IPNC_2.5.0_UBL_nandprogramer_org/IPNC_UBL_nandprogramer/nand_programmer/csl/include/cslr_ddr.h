#ifndef _CSLR_DDR_H_
#define _CSLR_DDR_H_

#include <cslr.h>
#include <dm350.h>
//#include "cslr_ddr_001.h"
#include "cslr_ddremif_00a_DM350.h"

extern void * _CSL_ddrlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_DDR_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_DdrRegs *  CSL_DdrRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_DDR_0_REGS                 ((CSL_DdrRegsOvly) 0x20000000)


/*****************************************************************************\
              Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_DdrRegsOvly _CSL_ddrGetBaseAddr (Uint16 ddrNum) {
  return (CSL_DdrRegsOvly)_CSL_ddrlookup[ddrNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:41    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
