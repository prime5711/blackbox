#ifndef _CSLR_EMIF_H_
#define _CSLR_EMIF_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_aemif_003_DM350.h"
//#include "cslr_emif_001.h"

extern void * _CSL_emiflookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_EMIF_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_EmifRegs *  CSL_EmifRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_EMIF_0_REGS                 ((CSL_EmifRegsOvly) 0x01e10000)


/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

/** @brief Returns the pointer to the EMIF Register Overlay Structure
 *         for a given instance of the EMIF
 *
 */
CSL_IDEF_INLINE CSL_EmifRegsOvly _CSL_emifGetBaseAddr (Uint16 emifNum) {
  return (CSL_EmifRegsOvly)_CSL_emiflookup[emifNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:44    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
