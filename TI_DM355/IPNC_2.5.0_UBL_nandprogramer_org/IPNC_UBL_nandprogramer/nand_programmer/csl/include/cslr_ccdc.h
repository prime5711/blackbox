#ifndef _CSLR_CCDC_H_
#define _CSLR_CCDC_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_ccdc_001.h"
extern void * _CSL_ccdclookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_CCDC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_CcdcRegs *  CSL_CcdcRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_CCDC_0_REGS                 ((CSL_CcdcRegsOvly) 0x01c70600)

/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_CcdcRegsOvly _CSL_ccdcGetBaseAddr (Uint16 ccdcNum) {
  return (CSL_CcdcRegsOvly)_CSL_ccdclookup[ccdcNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:39    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
