#ifndef _CSLR_SYS_H_
#define _CSLR_SYS_H_

#include <cslr.h>
#include "cslr_sys_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_SYS_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_SysRegs *  CSL_SysRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_SYS_0_REGS                 ((CSL_SysRegsOvly) 0x01c40000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:56    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
