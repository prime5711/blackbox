
/** @file _csl_spiGetHwStatus.h
 *
 *    @brief File for defining private functions used in CSL API @a CSL_spiGetHwStatus()
 *
 *  Description
 *    - The definitions of functions associated with @a CSL_spiGetHwStatus() function
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


#ifndef __CSL_SPIGETHWSTATUS_H_
#define __CSL_SPIGETHWSTATUS_H_

#include <csl_spi.h>

CSL_IDEF_INLINE
void _CSL_spiPwrStat(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiPwrStat                        *pwrMod
)
{

    *pwrMod = (CSL_SpiPwrStat)CSL_FEXT(hSpi->regs->SPIGCR1, SPI_SPIGCR1_PWRDN);

}


CSL_IDEF_INLINE
void _CSL_spiPinVal(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint32                                   *pinVal
)
{

    *pinVal = hSpi->regs->SPIPC2;

}


CSL_IDEF_INLINE
void _CSL_spiIntEnabled(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint32                                   *intVal
)
{

    *intVal = CSL_FEXTR(hSpi->regs->SPIINT0, 8, 0);

}


CSL_IDEF_INLINE
void _CSL_spiEvtStatus(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint8                                    *intVal
)
{

    *intVal = CSL_FEXTR(hSpi->regs->SPIFLG, 8, 1);
    CSL_FINSR(hSpi->regs->SPIFLG, 8, 1, *intVal);

}


CSL_IDEF_INLINE
void _CSL_spiIntVector(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    CSL_SpiHwStatusQuery                  intType,
    CSL_SpiIntVec                         *intVec
)
{

    Uint32 temp;

    if (CSL_SPI_QUERY_INT_VECTOR0 == intType) {
        temp = hSpi->regs->TGINTVEC[0];
    } else if (CSL_SPI_QUERY_INT_VECTOR1 == intType) {
        temp = hSpi->regs->TGINTVEC[1];
    }

    intVec->intVal  = CSL_FEXT(temp, SPI_TGINTVEC_INTVECT);
    intVec->suspend = CSL_FEXT(temp, SPI_TGINTVEC_SUSPEND);

}


CSL_IDEF_INLINE
void _CSL_spiCptDmaEnabled(
    /** pointer to the object that holds reference to the
     * instance of SPI requested after the call */
    CSL_SpiHandle                         hSpi,
    Uint16                                   *dmaVal
)
{

    *dmaVal = CSL_FEXT(hSpi->regs->SPIINT0, SPI_SPIINT0_DMAREQ);

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
