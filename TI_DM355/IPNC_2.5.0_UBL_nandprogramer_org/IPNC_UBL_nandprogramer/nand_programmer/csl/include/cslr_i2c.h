#ifndef _CSLR_I2C_H_
#define _CSLR_I2C_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_i2c_001.h"
extern void * _CSL_i2clookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_I2C_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_I2cRegs *  CSL_I2cRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_I2C_0_REGS                 ((CSL_I2cRegsOvly) 0x01c21000)


/*****************************************************************************\
               Overlay structure typedef definition
\*****************************************************************************/

/** @brief Returns the pointer to the I2C Register Overlay Structure
 *         for a given instance of the I2C
 *
 */
CSL_IDEF_INLINE CSL_I2cRegsOvly _CSL_i2cGetBaseAddr (Uint16 i2cNum) {
  return (CSL_I2cRegsOvly)_CSL_i2clookup[i2cNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:45    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
