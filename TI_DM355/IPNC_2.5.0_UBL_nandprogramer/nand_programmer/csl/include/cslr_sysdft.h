#ifndef _CSLR_SYSDFT_H_
#define _CSLR_SYSDFT_H_

#include <cslr.h>
#include "cslr_sysdft_001.h"
#include "dm350.h"
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_SYSDFT_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_SysdftRegs *  CSL_SysdftRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_SYSDFT_0_REGS                 ((CSL_SysdftRegsOvly) 0x01c42000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:56    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
