#ifndef _CSLR_CFALD_H_
#define _CSLR_CFALD_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_cfald_001.h"
extern void * _CSL_cfaldlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_CFALD_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_CfaldRegs *  CSL_CfaldRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_CFALD_0_REGS                 ((CSL_CfaldRegsOvly) 0x01c70900)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:40    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
