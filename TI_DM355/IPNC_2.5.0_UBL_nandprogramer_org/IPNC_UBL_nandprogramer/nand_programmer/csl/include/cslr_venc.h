#ifndef _CSLR_VENC_H_
#define _CSLR_VENC_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_venc_001.h"
extern void * _CSL_venclookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_VENC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_VencRegs *  CSL_VencRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_VENC_0_REGS                 ((CSL_VencRegsOvly) 0x01c70400)


/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_VencRegsOvly _CSL_vencGetBaseAddr (Uint16 vencNum) {
  return (CSL_VencRegsOvly)_CSL_venclookup[vencNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:59    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
