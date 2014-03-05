#ifndef _CSLR_IMCOP_H_
#define _CSLR_IMCOP_H_

#include <cslr.h>
#include "cslr_imcop_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_IMCOP_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_ImcopRegs *  CSL_ImcopRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_IMCOP_0_REGS                 ((CSL_ImcopRegsOvly) 0x01cc0000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:46    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
