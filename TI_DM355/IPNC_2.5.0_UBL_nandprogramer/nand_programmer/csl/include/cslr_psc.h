#ifndef _CSLR_PSC_H_
#define _CSLR_PSC_H_

#include <cslr.h>
#include "cslr_psc_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_PSC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_PscRegs *  CSL_PscRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_PSC_0_REGS                 ((CSL_PscRegsOvly) 0x01c41000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:53    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
