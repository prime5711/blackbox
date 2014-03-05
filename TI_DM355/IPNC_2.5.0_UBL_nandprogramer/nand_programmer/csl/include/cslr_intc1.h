#ifndef _CSLR_ARMINTC_H_
#define _CSLR_ARMINTC_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_intc_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_ARMINTC_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_IntcRegs *  CSL_Intc1RegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_INTC1_REGS                 ((CSL_Intc1RegsOvly) 0x01c48000)

CSL_IDEF_INLINE CSL_Intc1RegsOvly _CSL_intc1GetBaseAddr (Uint16 intcNum)
{
    return (CSL_Intc1RegsOvly)CSL_INTC1_REGS;
}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:48    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
