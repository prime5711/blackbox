#ifndef _CSLR_MS_H_
#define _CSLR_MS_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_ms_001.h"
extern void * _CSL_mslookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_MS_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_MsRegs *  CSL_MsRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_MS_0_REGS                 ((CSL_MsRegsOvly) 0x01e20000)

/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

/** @brief Returns the pointer to the MS Register Overlay Structure
 *         for a given instance of the MS
 *
 */
CSL_IDEF_INLINE CSL_MsRegsOvly _CSL_msGetBaseAddr (Uint16 msNum) {
  return (CSL_MsRegsOvly)_CSL_mslookup[msNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:51    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
