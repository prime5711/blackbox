
/** @file _csl_spiGetHwSetup.h
 *
 *    @brief File for defining private functions used in CSL API @a CSL_spiGetHwSetup()
 *
 *  Description
 *    - The definitions of functions associated with @a CSL_spiGetHwSetup() function
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
 *    - reason: removed MIB from code & documentation
 *
 *  @date 26th Dec, 2003
 *    @author Sumant S. NaikKhanvte
 */


#ifndef _CSL_SPIGETHWSETUP_H_
#define _CSL_SPIGETHWSETUP_H_

#include <csl_spi.h>

CSL_IDEF_INLINE
void _CSL_spiGetHwSetupPriFmt(
    CSL_SpiHandle                   hSpi,
    Uint8                              fmtNum,
    CSL_SpiHwSetupPriFmt            *confFmt
)
{

    Uint32 temp = hSpi->regs->SPIFMT[fmtNum];

    confFmt->wDelay   = CSL_FEXT(temp, SPI_SPIFMT_WDELAY);
    confFmt->preScale = CSL_FEXT(temp, SPI_SPIFMT_PRESCALE);
    confFmt->charLen  = CSL_FEXT(temp, SPI_SPIFMT_CHARLEN);
    confFmt->waitEna  = (CSL_SpiWaitEn)CSL_FEXT(temp, SPI_SPIFMT_WAITEN);
    confFmt->parity   = (CSL_SpiParity)CSL_FEXTR(temp, 23, 22);
    confFmt->phase    = (CSL_SpiPhase)CSL_FEXT(temp, SPI_SPIFMT_PHASE);
    confFmt->polarity = (CSL_SpiPolarity)
	                CSL_FEXT(temp, SPI_SPIFMT_POLARITY);
    confFmt->shiftDir = (CSL_SpiShDir)
	                CSL_FEXT(temp, SPI_SPIFMT_SHIFTDIR);

}


CSL_IDEF_INLINE
void _CSL_spiGetHwSetupCptInt(
    CSL_SpiHandle                   hSpi,
    Uint32                             *intVal
)
{
    *intVal = hSpi->regs->SPILVL;
    
}


CSL_IDEF_INLINE
void _CSL_spiGetHwSetupGen(
    CSL_SpiHandle                   hSpi,
    CSL_SpiHwSetupGen               *genPtr
)
{
    CSL_SpiRegsOvly regsPtr = hSpi->regs;
    Uint32 temp;

    genPtr->protocol = (CSL_SpiProtocol)CSL_FEXT(regsPtr->MIBSPIE, SPI_MIBSPIE_MIBSPIENA);
    genPtr->opMode   = (CSL_SpiOpMod)CSL_FEXTR(regsPtr->SPIGCR1, 1, 0);
    genPtr->enaHiZ   = (CSL_SpiEnaHiZ)CSL_FEXT(regsPtr->SPIINT0, SPI_SPIINT0_ENAHIGHZ);

    temp = regsPtr->SPIDELAY;
    genPtr->c2tDelay = CSL_FEXT(temp, SPI_SPIDELAY_C2T);
    genPtr->t2cDelay = CSL_FEXT(temp, SPI_SPIDELAY_T2C);
    genPtr->t2eTmout = CSL_FEXT(temp, SPI_SPIDELAY_T2E);
    genPtr->c2eTmout = CSL_FEXT(temp, SPI_SPIDELAY_C2E);

}


CSL_IDEF_INLINE
void _CSL_spiGetHwSetupGpio(
    CSL_SpiHandle                   hSpi,
    _CSL_SpiGpioType                gpioType,
    Uint32                             *value
)
{
    switch (gpioType) {
        case CSL_SPI_GPIOTYPE_FUNC:
            *value = hSpi->regs->SPIPC0;
            break;

        case CSL_SPI_GPIOTYPE_DIR:
            *value = hSpi->regs->SPIPC1;
            break;

        case CSL_SPI_GPIOTYPE_PULLCTRL:
            *value = hSpi->regs->SPIPC7;
            break;

        case CSL_SPI_GPIOTYPE_OPNDRAIN:
            *value = hSpi->regs->SPIPC6;
            break;

    };
}


CSL_IDEF_INLINE
void _CSL_spiGetHwSetupCptFmtCtrl(
    CSL_SpiHandle                   hSpi,
    CSL_SpiHwSetupFmtCtrl           *fmtCtrlPtr
)
{

    Uint32 temp = hSpi->regs->SPIDAT1;
    
    fmtCtrlPtr->csHold = (CSL_SpiCsHold)CSL_FEXT(temp, SPI_SPIDAT1_CSHOLD);
    fmtCtrlPtr->wDel   = (CSL_SpiWDelayEn)CSL_FEXT(temp, SPI_SPIDAT1_WDEL);
    fmtCtrlPtr->fmtSel = (CSL_SpiFmtSel)CSL_FEXT(temp, SPI_SPIDAT1_DFSEL);
    fmtCtrlPtr->cSel   = CSL_FEXT(temp, SPI_SPIDAT1_CSNR);

}

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         14 Jan 2005 13:32:23    5888             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL0.57 JAN 2005 Release and built the library for ARM and DSP*/
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:11    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
