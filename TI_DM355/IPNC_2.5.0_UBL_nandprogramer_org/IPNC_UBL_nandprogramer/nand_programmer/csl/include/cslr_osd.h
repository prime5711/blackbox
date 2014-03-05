#ifndef _CSLR_OSD_H_
#define _CSLR_OSD_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_osd_001.h"
extern void * _CSL_osdlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_OSD_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_OsdRegs *  CSL_OsdRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_OSD_0_REGS                 ((CSL_OsdRegsOvly) 0x01c70200)

/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_OsdRegsOvly _CSL_osdGetBaseAddr (Uint16 osdNum) {
  return (CSL_OsdRegsOvly)_CSL_osdlookup[osdNum];
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:52    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
