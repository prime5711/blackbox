#ifndef _CSLR_PWM_H_
#define _CSLR_PWM_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_pwm_001.h"
extern void * _CSL_pwmlookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_PWM_PER_CNT          4

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_PwmRegs *  CSL_PwmRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_PWM_0_REGS                 ((CSL_PwmRegsOvly) 0x01c22000)
#define CSL_PWM_1_REGS                 ((CSL_PwmRegsOvly) 0x01c22400)
#define CSL_PWM_2_REGS                 ((CSL_PwmRegsOvly) 0x01c22800)
#define CSL_PWM_3_REGS                 ((CSL_PwmRegsOvly) 0x01c22c00)

/*****************************************************************************\
              Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_PwmRegsOvly _CSL_pwmGetBaseAddr (Uint16 pwmNum) {
  return (CSL_PwmRegsOvly)_CSL_pwmlookup[pwmNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:54    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
