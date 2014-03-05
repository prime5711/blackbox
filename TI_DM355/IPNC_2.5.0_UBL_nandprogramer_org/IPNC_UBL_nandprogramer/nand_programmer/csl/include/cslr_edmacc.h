#ifndef _CSLR_EDMACC_H_
#define _CSLR_EDMACC_H_

#include <cslr.h>
#include "cslr_edmacc_001.h"


/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_EDMACC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_EdmaccShadowRegs   * CSL_EdmaccShadowRegsOvly;
typedef volatile CSL_EdmaccRegs *  CSL_EdmaccRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
//#define CSL_EDMACC_0_REGS                 ((CSL_EdmaccRegsOvly) 0x01c00000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:41    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
