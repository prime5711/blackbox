#ifndef _CSLR_EDMATC0_H_
#define _CSLR_EDMATC0_H_

#include <cslr.h>
#include "cslr_edmatc0_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_EDMATC0_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_Edmatc0Regs *  CSL_Edmatc0RegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_EDMATC_0_REGS                 ((CSL_Edmatc0RegsOvly) 0x01c10000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:42    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
