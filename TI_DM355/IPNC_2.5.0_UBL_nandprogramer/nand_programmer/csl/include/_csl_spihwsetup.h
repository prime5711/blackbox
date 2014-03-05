
/** @file _csl_spiHwSetup.h
 *
 *    @brief File for defining private functions used in CSL API @a CSL_spiHwSetup()
 *
 *  Description
 *    - The definitions of functions associated with @a CSL_spiHwSetup() function
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


#ifndef _CSL_SPIHWSETUP_H_
#define _CSL_SPIHWSETUP_H_

#include <csl_spi.h>
//#include "testaccess.h"


CSL_IDEF_INLINE
void _CSL_spiHwSetupPriFmt(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                   hSpi,
    /** instance number of the format register to setup */
    Uint8                              fmtNum,
    /** setup structure for the format register */
    CSL_SpiHwSetupPriFmt            *confFmt
)
{

    hSpi->regs->SPIFMT[fmtNum] = CSL_FMK(SPI_SPIFMT_WDELAY,
					    confFmt->wDelay) |
                                    CSL_FMK(SPI_SPIFMT_PRESCALE,
					    confFmt->preScale) |
                                    CSL_FMK(SPI_SPIFMT_CHARLEN,
					    confFmt->charLen) |
                                    CSL_FMK(SPI_SPIFMT_WAITEN,
					    confFmt->waitEna) |
                                    CSL_FMKR(confFmt->parity, 23, 22) |
                                    CSL_FMK(SPI_SPIFMT_POLARITY,
					    confFmt->polarity) |
                                    CSL_FMK(SPI_SPIFMT_PHASE,
					    confFmt->phase) |
                                    CSL_FMK(SPI_SPIFMT_SHIFTDIR,
					    confFmt->shiftDir);

}


CSL_IDEF_INLINE
void _CSL_spiHwSetupCptInt(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                   hSpi,
    /** bit-vector for interrupts */
    Uint32                             intVal
)
{

    hSpi->regs->SPILVL = intVal;

}


CSL_IDEF_INLINE
void _CSL_spiHwSetupGen(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                   hSpi,
    CSL_SpiHwSetupGen               *genPtr
)
{

    CSL_SpiRegsOvly regsPtr = hSpi->regs;
    Uint32 temp;

    CSL_FINS(regsPtr->MIBSPIE, SPI_MIBSPIE_MIBSPIENA, genPtr->protocol);

    temp = regsPtr->SPIGCR1;
    CSL_FINS(temp, SPI_SPIGCR1_CLKMOD, CSL_FEXTR(genPtr->opMode, 1, 1));
    CSL_FINS(temp, SPI_SPIGCR1_MASTER, CSL_FEXTR(genPtr->opMode, 0, 0));
    regsPtr->SPIGCR1 = temp;

    CSL_FINS(regsPtr->SPIINT0, SPI_SPIINT0_ENAHIGHZ, genPtr->enaHiZ);

    temp = CSL_FMK(SPI_SPIDELAY_C2T, genPtr->c2tDelay) |
           CSL_FMK(SPI_SPIDELAY_T2C, genPtr->t2cDelay) |
           CSL_FMK(SPI_SPIDELAY_T2E, genPtr->t2eTmout) |
           CSL_FMK(SPI_SPIDELAY_C2E, genPtr->c2eTmout);
    regsPtr->SPIDELAY = temp;
        
}


CSL_IDEF_INLINE
void _CSL_spiHwSetupGpio(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                   hSpi,
    _CSL_SpiGpioType                gpioType,
    Uint32                             value
)
{
    switch (gpioType) {
        case CSL_SPI_GPIOTYPE_FUNC:
            hSpi->regs->SPIPC0 = value;
            break;

        case CSL_SPI_GPIOTYPE_DIR:
            hSpi->regs->SPIPC1 = value;
            break;

        case CSL_SPI_GPIOTYPE_PULLCTRL:
            hSpi->regs->SPIPC7 = value;
            break;

        case CSL_SPI_GPIOTYPE_OPNDRAIN:
            hSpi->regs->SPIPC6 = value;
            break;

    };

}


CSL_IDEF_INLINE
void _CSL_spiHwSetupCptFmtCtrl(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                   hSpi,
    CSL_SpiHwSetupFmtCtrl           *fmtCtrlPtr
)
{

    Uint32 temp;
    temp = CSL_FMK(SPI_SPIDAT1_CSHOLD, fmtCtrlPtr->csHold) |
           CSL_FMK(SPI_SPIDAT1_WDEL, fmtCtrlPtr->wDel) |
           CSL_FMK(SPI_SPIDAT1_DFSEL, fmtCtrlPtr->fmtSel) |
           CSL_FMK(SPI_SPIDAT1_CSNR, fmtCtrlPtr->cSel);
    CSL_FINSR(hSpi->regs->SPIDAT1, 32, 16, CSL_FEXTR(temp, 32, 16));

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
