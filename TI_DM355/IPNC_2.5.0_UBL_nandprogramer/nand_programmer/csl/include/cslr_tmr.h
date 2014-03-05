#ifndef _CSLR_TMR_H_
#define _CSLR_TMR_H_

#include <cslr.h>
#include "cslr_tmr_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_TMR_PER_CNT          4

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_TmrRegs *  CSL_TmrRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_TMR_0_REGS                 ((CSL_TmrRegsOvly) 0x01c21400)
#define CSL_TMR_1_REGS                 ((CSL_TmrRegsOvly) 0x01c21800)
#define CSL_TMR_2_REGS                 ((CSL_TmrRegsOvly) 0x01c21c00)
#define CSL_TMR_3_REGS                 ((CSL_TmrRegsOvly) 0x01c20800)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:57    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
