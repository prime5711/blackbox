#ifndef _CSLR_RTO_H_
#define _CSLR_RTO_H_

#include <cslr.h>
#include "cslr_rto_002.h"
#include <dm350.h>
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_RTO_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_RtoRegs *  CSL_RtoRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_RTO_0_REGS                 ((CSL_RtoRegsOvly) 0x01c20c00)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:54    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
