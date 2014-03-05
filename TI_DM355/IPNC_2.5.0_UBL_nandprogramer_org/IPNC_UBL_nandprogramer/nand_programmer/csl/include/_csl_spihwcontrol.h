
/** @file _csl_spiHwControl.h
 *
 *    @brief File for defining private functions used in CSL API @a CSL_spiHwControl()
 *
 *  Description
 *    - The definitions of functions associated with @a CSL_spiHwControl() function
 *
 *  Modification 1
 *    - modified on: 26/12/2003
 *    - reason: created the sources
 *
 *  Modification 2
 *    - modified on: 05/01/2004
 *    - reason: created better documentation
 *
 *  Modification 3
 *    - modified on: 01/03/2004
 *    - reason: modified according to review comments
 *
 *  Modification 4
 *    - modified on: 10/05/2004
 *    - reason: Removed MIB functionality from code & documentation
 *
 *  @date 26th Dec, 2003
 *    @author Sumant S. NaikKhanvte
 */


#ifndef _CSL_SPIHWCONTROL_H_
#define _CSL_SPIHWCONTROL_H_

#include <csl_spi.h>



CSL_IDEF_INLINE
void _CSL_spiPriResetOut(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi
)
{

    CSL_FINST(hSpi->regs->SPIGCR0, SPI_SPIGCR0_RESET, NO);

}


CSL_IDEF_INLINE
void _CSL_spiPwr(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiPwrStat                        pwrMod
)
{

    CSL_FINS(hSpi->regs->SPIGCR1,
         SPI_SPIGCR1_PWRDN,
         pwrMod);

}


CSL_IDEF_INLINE
void _CSL_spiCptDma(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiCptDma                         dmaEn
)
{

    CSL_FINS(hSpi->regs->SPIINT0, SPI_SPIINT0_DMAREQ, dmaEn);

}


CSL_IDEF_INLINE
void _CSL_spiCptWrite0(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint16                                   val
)
{

    CSL_FINS(hSpi->regs->SPIDAT0, SPI_SPIDAT0_SPIDAT0, val);

}


CSL_IDEF_INLINE
void _CSL_spiCptWrite1(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint16                                   val
)
{

    CSL_FINS(hSpi->regs->SPIDAT1, SPI_SPIDAT1_SPIDAT1, val);

}


CSL_IDEF_INLINE
void _CSL_spiCptRead(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint16                                   *data,
    CSL_SpiBufStat                        *bufStat
)
{

    Uint32 temp = hSpi->regs->SPIBUF;
    *data = CSL_FEXT(temp, SPI_SPIBUF_SPIBUF);
    if (bufStat) {
        bufStat->status = CSL_FEXTR(temp, 31, 25);
        bufStat->cSel   = CSL_FEXT(temp, SPI_SPIBUF_LCSNR);
    }

}


CSL_IDEF_INLINE
void _CSL_spiInt(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiHwControlCmd                   cmd,
    Uint16                                    intVal
)
{

    if (CSL_SPI_CMD_INT_ENABLE == cmd) {
        hSpi->regs->SPIINT0 |= intVal;
    } else if (CSL_SPI_CMD_INT_DISABLE == cmd) {
        hSpi->regs->SPIINT0 &= ~intVal;
    }

}


CSL_IDEF_INLINE
void _CSL_spiPins(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiHwControlCmd                   cmd,
    Uint32                                   pinVal
)
{

    if (CSL_SPI_CMD_PINS_VALUE == cmd) {
        hSpi->regs->SPIPC3 = pinVal;
    } else if (CSL_SPI_CMD_PINS_SET == cmd) {
        hSpi->regs->SPIPC4 = pinVal;
    } else if (CSL_SPI_CMD_PINS_CLEAR == cmd) {
        hSpi->regs->SPIPC5 = pinVal;
    }

}


CSL_IDEF_INLINE
void _CSL_spiXferCtrl(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiXferEn                         xferEn
)
{

    CSL_FINS(hSpi->regs->SPIGCR1, SPI_SPIGCR1_SPIEN, xferEn);

}


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         14 Jan 2005 13:32:24    5888             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL0.57 JAN 2005 Release and built the library for ARM and DSP*/
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:12    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
