#ifndef _CSLR_SPI_H_
#define _CSLR_SPI_H_

#include <cslr.h>
#include <dm350.h>
#include "cslr_spi_001.h"
extern void * _CSL_spilookup[];
/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_SPI_PER_CNT          3

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_SpiRegs *  CSL_SpiRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_SPI_0_REGS                 ((CSL_SpiRegsOvly) 0x01c66000)
#define CSL_SPI_1_REGS                 ((CSL_SpiRegsOvly) 0x01c66800)
#define CSL_SPI_2_REGS                 ((CSL_SpiRegsOvly) 0x01c67800)


/*****************************************************************************\
              Overlay structure typedef definition
\*****************************************************************************/

CSL_IDEF_INLINE CSL_SpiRegsOvly _CSL_spiGetBaseAddr (Uint16 spiNum) {
  return (CSL_SpiRegsOvly)_CSL_spilookup[spiNum];
}
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:55    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
