#ifndef _CSLR_EDMATC1_H_
#define _CSLR_EDMATC1_H_

#include <cslr.h>
#include "cslr_edmatc1_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_EDMATC1_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_Edmatc1Regs *  CSL_Edmatc1RegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_EDMATC_1_REGS                 ((CSL_Edmatc1RegsOvly) 0x01c10400)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:43    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
