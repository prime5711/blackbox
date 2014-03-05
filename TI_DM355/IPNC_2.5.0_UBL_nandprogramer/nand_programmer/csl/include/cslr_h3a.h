#ifndef _CSLR_H3A_H_
#define _CSLR_H3A_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_h3a_001.h"
extern void * _CSL_h3alookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_H3A_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_H3aRegs *  CSL_H3aRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_H3A_0_REGS                 ((CSL_H3aRegsOvly) 0x01c70080)


/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_H3aRegsOvly _CSL_h3aGetBaseAddr (Uint16 h3aNum) {
  return (CSL_H3aRegsOvly)_CSL_h3alookup[h3aNum];
}


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:45    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
