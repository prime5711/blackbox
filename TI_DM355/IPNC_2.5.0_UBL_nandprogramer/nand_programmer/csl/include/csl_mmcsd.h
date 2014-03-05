/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage MMCSD CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the MMCSD module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given MMCSD module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular MMCSD Module. The CSL developer should use his discretion designing
 * new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
 *
 */
/** \file csl_mmcsd.h
    \brief MMC/SD functional layer API header file
    \internal   
    \date   12 May, 2004
    \author Pratheesh Gangadhar (pratheesh@ti.com)
 */
#ifndef _CSL_MMCSD_H_
#define _CSL_MMCSD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_mmcsd.h>

/**
\defgroup CSL_MMCSD_API MMC/SD
   \internal Based on <b>Imaging and Audio Group MMCSD Detailed Design 
             Specification Rev 1.00.w.05 </b> 
*/
/**
\defgroup CSL_MMCSD_DATASTRUCT Data Structures
\ingroup CSL_MMCSD_API
*/
/**
\defgroup CSL_MMCSD_DEFINE  Defines
\ingroup CSL_MMCSD_API
*/
/**
\defgroup CSL_MMCSD_ENUM  Enumerated Data Types
\ingroup CSL_MMCSD_API
*/

/**
\defgroup CSL_MMCSD_FUNCTION  Functions
\ingroup CSL_MMCSD_API
*/

/*****************************************************************************\
        MMCSD global macro declarations
\*****************************************************************************/
/** \defgroup CSL_MMCSD_INTEN_DEFINE Interrupt Enable Masks
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */
#define CSL_MMCSD_INTEN_TRNDNE      CSL_FMKT (MMCSD_MMCIM_ETRNDNE, PERMIT)
                                /**< Enable Data transfer done interrupt */ 
#define CSL_MMCSD_INTEN_DATED       CSL_FMKT (MMCSD_MMCIM_EDATED, PERMIT)
                                /**< Enable DAT3 edge detect interrupt */ 
#define CSL_MMCSD_INTEN_DRRDY       CSL_FMKT (MMCSD_MMCIM_EDRRDY, PERMIT)
                                /**< Enable data receive ready interrupt */ 
#define CSL_MMCSD_INTEN_DXRDY       CSL_FMKT (MMCSD_MMCIM_EDXRDY, PERMIT)
                                /**< Enable data transmit ready interrupt */   
#define CSL_MMCSD_INTEN_CRCRS       CSL_FMKT (MMCSD_MMCIM_ECRCRS, PERMIT)
                                /**< Enable response CRC error interrupt */ 
#define CSL_MMCSD_INTEN_CRCRD       CSL_FMKT (MMCSD_MMCIM_ECRCRD, PERMIT)
                                /**< Enable read-data CRC error interrupt */ 
#define CSL_MMCSD_INTEN_CRCWR       CSL_FMKT (MMCSD_MMCIM_ECRCWR, PERMIT)
                                /**< Enable write-data CRC error interrupt */ 
#define CSL_MMCSD_INTEN_TOUTRS      CSL_FMKT (MMCSD_MMCIM_ETOUTRS, PERMIT)
                                /**< Enable response time-out interrupt */ 
#define CSL_MMCSD_INTEN_TOUTRD      CSL_FMKT (MMCSD_MMCIM_ETOUTRD, PERMIT)
                                /**< Enable read-data time-out interrupt */ 
#define CSL_MMCSD_INTEN_RSPDNE      CSL_FMKT (MMCSD_MMCIM_ERSPDNE, PERMIT)
                                /**< Enable response/command done interrupt */ 
#define CSL_MMCSD_INTEN_BSYDNE      CSL_FMKT (MMCSD_MMCIM_EBSYDNE, PERMIT)
                                /**< Enable busy done interrupt */
#define CSL_MMCSD_INTEN_DATDNE      CSL_FMKT (MMCSD_MMCIM_EDATDNE, PERMIT)
                                /**< Enable data transfer done interrupt */                                 
/** 
@} */


/** \defgroup CSL_MMCSD_SDIOINT_DEFINE SDIO Interrupt Enable/Status Masks
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */

#define CSL_MMCSD_SDIOINT_RDWT      CSL_FMKT (MMCSD_SDIOIEN_RWSEN, ENABLE) 
                                   /**< Enable read Wait interrupt */ 
#define CSL_MMCSD_SDIOINT_IOINT     CSL_FMKT (MMCSD_SDIOIEN_IOINTEN, ENABLE)
                                   /**< Enable SDIO card interrupt */

/** 
@} */


/** \defgroup CSL_MMCSD_SDIOSTAT_DEFINE SDIO Status Masks
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */

#define CSL_MMCSD_SDIOSTAT_RDWT     CSL_FMKT (MMCSD_SDIOIEN_RDWTST, ENABLE) 
                                   /**< Read wait operation in progress */ 
#define CSL_MMCSD_SDIOSTAT_INTPRD   CSL_FMKT (MMCSD_SDIOIEN_INTPRD, ENABLE)
                                   /**< Interrupt asserted */
#define CSL_MMCSD_SDIOSTAT_DAT1     CSL_FMKT (MMCSD_SDIOIEN_DAT1, ENABLE)
                                   /**< DAT1 status */                                   

/** 
@} */




/** \defgroup CSL_MMCSD_STAT_DEFINE MMC/SD Status Masks
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */

#define CSL_MMCSD_STAT0_TRNDNE      CSL_FMK (MMCSD_MMCST0_TRNDNE, 1)
                                /**< Data transfer of specified length done */
#define CSL_MMCSD_STAT0_DATED       CSL_FMK (MMCSD_MMCST0_DATED, 1)
                                /**< DAT3 edge detect */ 
#define CSL_MMCSD_STAT0_DRRDY       CSL_FMK (MMCSD_MMCST0_DRRDY, 1)
                                /**< Data receive ready detect */ 
#define CSL_MMCSD_STAT0_DXRDY       CSL_FMK (MMCSD_MMCST0_DXRDY, 1)
                                /**< Data transmit ready detect */ 
#define CSL_MMCSD_STAT0_CRCRS       CSL_FMK (MMCSD_MMCST0_CRCRS, 1)
                                /**< Response CRC error detect */ 
#define CSL_MMCSD_STAT0_CRCRD       CSL_FMK (MMCSD_MMCST0_CRCRD, 1)
                                /**< Read-data CRC error detect */ 
#define CSL_MMCSD_STAT0_CRCWR       CSL_FMK (MMCSD_MMCST0_CRCWR, 1)
                                /**< Write-data CRC error detect */ 
#define CSL_MMCSD_STAT0_TOUTRS      CSL_FMK (MMCSD_MMCST0_TOUTRS, 1)
                                /**< Response time-out detect */ 
#define CSL_MMCSD_STAT0_TOUTRD      CSL_FMK (MMCSD_MMCST0_TOUTRD, 1)
                                /**< Read-data time-out detect */ 
#define CSL_MMCSD_STAT0_RSPDNE      CSL_FMK (MMCSD_MMCST0_RSPDNE, 1)
                                /**< Write-data CRC error detect */ 
#define CSL_MMCSD_STAT0_BSYDNE      CSL_FMK (MMCSD_MMCST0_BSYDNE, 1)
                                /**< Response time-out detect */ 
#define CSL_MMCSD_STAT0_DATDNE      CSL_FMK (MMCSD_MMCST0_DATDNE, 1)
                                /**< Read-data time-out detect */ 
                                    
#define CSL_MMCSD_STAT1_BUSY        CSL_FMK (MMCSD_MMCST1_BUSY, 1)
                                /**< Busy signal detect */ 
#define CSL_MMCSD_STAT1_CLKLOW      CSL_FMK (MMCSD_MMCST1_CLKSTP, 1)
                                /**< CLK is held low. Possible reasons:
                                 Manual stop (CLKEN = 0), data receive full
                                 condition, or data transmit empty condition */
#define CSL_MMCSD_STAT1_DXEMP       CSL_FMK (MMCSD_MMCST1_DXEMP, 1)
                                /**< Data transmit empty */  
#define CSL_MMCSD_STAT1_DRFUL       CSL_FMK (MMCSD_MMCST1_DRFUL, 1)
                                /**< Data receive full */ 
#define CSL_MMCSD_STAT1_DAT3ST      CSL_FMK (MMCSD_MMCST1_DAT3ST, 1)
                                /**< DAT3 status */ 
#define CSL_MMCSD_STAT1_FIFOEMP     CSL_FMK (MMCSD_MMCST1_FIFOEMP, 1)
                                /**< FIFO empty */ 
#define CSL_MMCSD_STAT1_FIFOFUL     CSL_FMK (MMCSD_MMCST1_FIFOFUL, 1)
                                /**< FIFO full */  
/** 
@} */


/** \defgroup CSL_MMCSD_RESET_DEFINE Reset Logic Select
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */
#define CSL_MMCSD_RESET_CMD      CSL_FMKT(MMCSD_MMCCTL_CMDRST, DISABLE)
                               /**< The CMD line portion is disabled and in 
                                    reset state */
#define CSL_MMCSD_RESET_DAT      CSL_FMKT(MMCSD_MMCCTL_DATRST, DISABLE)
                               /**< The DAT line portion is disabled and in 
                                    reset state */
                     
/** 
@} */



/** \defgroup CSL_MMCSD_CMD_FLAG_DEFINE Command Enhancements
*   \ingroup CSL_MMCSD_DEFINE
*   
*
* @{ */

#define CSL_MMCSD_DCLR           CSL_FMKT(MMCSD_MMCCMD_DCLR, CLEAR)
                    /**< OR with \CSL_MmcsdCmdType, when command has to be sent
                     with data receive/transmit clear */
#define CSL_MMCSD_GENDMAREQ      CSL_FMKT(MMCSD_MMCCMD_DMATRIG, YES)
                    /**< OR with \CSL_MmcsdCmdType, when DMA transfer event has
                     to be generated */
                     
/** 
@} */


/*****************************************************************************\
        MMCSD global typedef declarations
\*****************************************************************************/

/**\defgroup CSL_MMCSD_MCLK_ENUM Memory Clock Control
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/
/** \brief MMC/SD memory clock control
*
*/


typedef enum {
  CSL_MMCSD_MEMCLK_DISABLE = CSL_MMCSD_MMCCLK_CLKEN_DISABLE,/**< <b>: Memory 
                         clock appears on SD_CLK pin only when required </b> */
  CSL_MMCSD_MEMCLK_ENABLE = CSL_MMCSD_MMCCLK_CLKEN_ENABLE
                    /**< <b>: Memory clock always appears on SD_CLK pin </b> */
} CSL_MmcsdMclkCtl;
/**
@} */

/**\defgroup CSL_MMCSD_ENDIAN_ENUM MMC/SD Endianness
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD endianness */
typedef enum {
  CSL_MMCSD_LITTLE_ENDIAN,/**< <b>: Little endian </b> */
  CSL_MMCSD_BIG_ENDIAN/**< <b>: Big endian </b>*/
} CSL_MmcsdEndian;
/**
@} */


/**\defgroup CSL_MMCSD_DAT3_ENUM MMC/SD DAT3 Edge Detection
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD DAT3 edge detection */
typedef enum {
  CSL_MMCSD_DAT3_DISABLE      = CSL_MMCSD_MMCCTL_DATEG_DISABLE,
                       /**< <b>: DAT3 edge detection is disabled </b> */
  CSL_MMCSD_DAT3_RISING_EDGE  = CSL_MMCSD_MMCCTL_DATEG_R_EDGE,
                       /**< <b>: DAT3 rising edge detection is enabled </b>*/  
  CSL_MMCSD_DAT3_FALLING_EDGE = CSL_MMCSD_MMCCTL_DATEG_F_EDGE,
                       /**< <b>: DAT3 falling edge detection is enabled </b>*/
  CSL_MMCSD_DAT3_DUAL_EDGE    = CSL_MMCSD_MMCCTL_DATEG_RF_EDGE
                       /**< <b>: DAT3 both edge detection is enabled </b>*/
} CSL_MmcsdDat3EdgeDetect;
/**
@} */



/**\defgroup CSL_MMCSD_BUSWIDTH_ENUM MMC/SD Bus Width
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD bus widths */
typedef enum {
  CSL_MMCSD_SD_BUSWIDTH_1BIT  =  CSL_MMCSD_MMCCTL_WIDTH_BIT1, 
                         /**< <b>: Bus width of MMCSD is 1 </b>*/
  CSL_MMCSD_SD_BUSWIDTH_4BIT  =  CSL_MMCSD_MMCCTL_WIDTH_BIT4
                         /**< <b>: Bus width of MMCSD is 4 </b>*/
} CSL_MmcsdBusWidth;

/**
@} */


/**\defgroup CSL_MMCSD_FIFOLEV_ENUM FIFO Threshold
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD FIFO threshold level */
typedef enum {
  CSL_MMCSD_FIFO_THRESHOLD_128BIT  =  CSL_MMCSD_MMCFIFOCTL_FIFOLEV_128BIT,
                          /**< <b>: DMA Request triggered every 128 bit 
                               sent/received </b> */
  CSL_MMCSD_FIFO_THRESHOLD_256BIT  =  CSL_MMCSD_MMCFIFOCTL_FIFOLEV_256BIT
                          /** <b>: DMA Request triggered every 256 bit 
                               sent/received </b> */
} CSL_MmcsdFifoThreshold;

/**
@} */


/**\defgroup CSL_MMCSD_FIFODIR_ENUM FIFO Direction
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD FIFO direction */
typedef enum {
  CSL_MMCSD_FIFODIR_READ  =  CSL_MMCSD_MMCFIFOCTL_FIFODIR_READ,
                          /**< <b>: DMA Request triggered every 128 bit 
                               sent/received </b> */
  CSL_MMCSD_FIFODIR_WRITE =  CSL_MMCSD_MMCFIFOCTL_FIFODIR_WRITE
                          /** <b>: DMA Request triggered every 256 bit 
                               sent/received </b> */
} CSL_MmcsdFifoDir;

/**
@} */

/**\defgroup CSL_MMCSD_FIFOACCWD_ENUM FIFO Access Width
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD FIFO access width
 *   \n Used by FIFO control to determine full/empty flag  
 */
typedef enum {
  CSL_MMCSD_FIFOACCWD_1BYTE  =  CSL_MMCSD_MMCFIFOCTL_ACCWD_1BYTE,
                          /**< <b>: CPU/DMA access width of 1 byte </b> */
  CSL_MMCSD_FIFOACCWD_2BYTES =  CSL_MMCSD_MMCFIFOCTL_ACCWD_2BYTES,
                          /**< <b>: CPU/DMA access width of 2 bytes </b> */
  CSL_MMCSD_FIFOACCWD_3BYTES  =  CSL_MMCSD_MMCFIFOCTL_ACCWD_3BYTES,
                         /**< <b>: CPU/DMA access width of 3 bytes </b> */
  CSL_MMCSD_FIFOACCWD_4BYTES =  CSL_MMCSD_MMCFIFOCTL_ACCWD_4BYTES
                          /**< <b>: CPU/DMA access width of 4 bytes </b> */                               
} CSL_MmcsdFifoAccessWidth;

/**
@} */


/**\defgroup CSL_MMCSD_CMDTYP_ENUM MMC/SD Command Type
*  \ingroup CSL_MMCSD_ENUM
*
*  
@{*/

/** \brief Enumeration for MMC/SD command to write to MMCCMD register */

typedef enum {
  CSL_MMCSD_CMDTYP_GO_IDLE_STATE =  
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)|
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) | 
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, NORSP) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 0),
  /**< <b>: Resets all cards to idle state  </b> */  
  CSL_MMCSD_CMDTYP_MMC_SEND_OP_COND =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) | 
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP,NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) | 
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R3) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, OD) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 1),
  /**< <b>: Asks all cards in idle state to send their operation conditions
            register contents in the response on the CMD line </b> */
  CSL_MMCSD_CMDTYP_ALL_SEND_CID =  
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R2) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, OD) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 2),
  /**< <b>: Asks all cards to send their CID numberson the CMD line */
  CSL_MMCSD_CMDTYP_MMC_SET_RELATIVE_ADDR = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, OD) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 3),
  /**< <b>: Assigns relative address to the card */ 
  CSL_MMCSD_CMDTYP_SD_SET_RELATIVE_ADDR = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R6) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, OD) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 3),
  /**< <b>: Assigns relative address to the SD card </b> */
  CSL_MMCSD_CMDTYP_SET_DSR = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, NORSP) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 4),
  /**< <b>: Programs the DSR of all cards </b> */
  CSL_MMCSD_CMDTYP_SD_ACMD6_SET_BUSWIDTH = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 6),
  /**< <b>: Sets buswidth of SD card </b> */
  CSL_MMCSD_CMDTYP_SELECT_CARD =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 7),
  /**< <b>: Command toggles a card between the stand-by and transfer states or
    between the programming and disconnect states. In both cases the card is
    selected by its own relative address and gets deselected by any other
    address; address 0 deselects all </b> */
                                
  CSL_MMCSD_CMDTYP_DESELECT_CARD = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, NORSP) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 7),
  /**< <b>: Command toggles a card between the stand-by and transfer states or
    between the programming and disconnect states. In both cases the card is
    selected by its own relative address and gets deselected by any other
    address; address 0 deselects all </b> */

  CSL_MMCSD_CMDTYP_SEND_CSD =   
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R2) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 9),
  /**< <b>: Addressed card sends its card-specific data (CSD) on the CMD line
       </b> */

  CSL_MMCSD_CMDTYP_SEND_CID =    
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R2) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK (MMCSD_MMCCMD_CMD, 10),
  /**< <b>: Addressed card sends its card-specific identification (CID) on CMD
            the line </b> */

  CSL_MMCSD_CMDTYP_MMC_READ_DAT_UNTIL_STOP = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, STREAM)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) | 
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 11),
  /**< <b>: Reads data stream from the card, starting at the given address,
            until a STOP_TRANSMISSION follows  </b> */ 

  CSL_MMCSD_CMDTYP_STOP_READ_TRANSMISSION =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 12),
  /**< <b>: Forces the card to stop read transmission </b> */ 

  CSL_MMCSD_CMDTYP_STOP_WRITE_TRANSMISSION = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 12),
  /**< <b>: Forces the card to stop write transmission </b> */ 

  CSL_MMCSD_CMDTYP_SEND_STATUS = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 13),
  /**< <b>: Addressed card sends its status register </b> */ 

  CSL_MMCSD_CMDTYP_SD_ACMD13_SDSTATUS =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 13),
  /**< <b>: Addressed SD card sends its status register </b> */ 
  CSL_MMCSD_CMDTYP_GO_INACTIVE_STATE =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, NORSP) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 15),
  /**< <b>: Sets the card to inactive state in order to protect the card stack
       against communication breakdowns </b> */
                               
  CSL_MMCSD_CMDTYP_SET_BLOCKLEN =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 16),
  /**< <b>: Sets the block length (in bytes) for all following block commands
            (read and write). Default block length is specified in the CSD
       </b> */
  CSL_MMCSD_CMDTYP_READ_SINGLE_BLOCK = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 17),
  /**< <b>: Reads a block of the size selected by the SET_BLOCKLEN command
       </b> */

  CSL_MMCSD_CMDTYP_READ_MULTIPLE_BLOCK = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 18),
  /**< <b>: Continuously transfers data blocks from card to host until
    interrupted by a stop command or the requested number of data block
    transmitted </b> */
  
  CSL_MMCSD_CMDTYP_MMC_WRITE_DAT_UNTIL_STOP =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, STREAM)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 20),
  /**< <b>: Writes data stream from the host, starting at the given address,
            until a STOP_TRANSMISSION follows </b> */
                                    
  CSL_MMCSD_CMDTYP_SD_ACMD22_SEND_NUM_WR_BLOCKS =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 22),
  /**< <b>: Sends number of SD write blocks </b> */
  CSL_MMCSD_CMDTYP_SD_ACMD23_SET_NUM_WR_BLK_ERASE_COUNT = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 23),
  /**< <b>: Sets number of SD write block erase count </b> */
                                       
  CSL_MMCSD_CMDTYP_SET_BLOCK_COUNT = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK(MMCSD_MMCCMD_CMD, 23),
  /**< <b>: Defines the number of blocks which are going to be transferred in
            the immediately succeeding multiple block read or write command
       </b> */

                                        
  CSL_MMCSD_CMDTYP_WRITE_BLOCK = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 24),
  /**< <b>: Writes a block of the size selected by the SET_BLOCKLEN command </b> */

  CSL_MMCSD_CMDTYP_WRITE_MULTIPLE_BLOCK =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 25),
  
  /**< <b>: Continuously writes blocks of data until a STOP_TRANSMISSION 
       follows or the requested number of block received </b> */
  CSL_MMCSD_CMDTYP_PROGRAM_CID = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 26),
  /**< <b>: Programming of the card identification register. This command shall
    be issued only once per card. The card contains hardware to prevent this
    operation after the first programming. Normally this command is reserved
    for the manufacturer </b> */

  CSL_MMCSD_CMDTYP_PROGRAM_CSD = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 27),
  /**< <b>: Programming of the programmable bits of the CSD </b> */
                               
  CSL_MMCSD_CMDTYP_SET_WRITE_PROT = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                     CSL_FMK  (MMCSD_MMCCMD_CMD, 28),
  /**< <b>: If the card has write protection features, this command sets the
            write protection bit of the addressed group </b> */

  CSL_MMCSD_CMDTYP_CLR_WRITE_PROT = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 29),

  /**< <b>: If the card has write protection features, this command clears the
            write protection bit of the addressed group </b> */
                                    
  CSL_MMCSD_CMDTYP_SEND_WRITE_PROT =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 30),

  /**< <b>: If the card has write protection features, this command asks the
            card to send the status of the write protection bits </b> */
  
  CSL_MMCSD_CMDTYP_SD_ERASE_WR_BLK_START = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 32),
  
  /**< <b>: Erase write block start in SD card </b> */                                 
  CSL_MMCSD_CMDTYP_SD_ERASE_WR_BLK_END = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 33),
  
  /**< <b>: Erase write block end in SD card </b> */
  CSL_MMCSD_CMDTYP_MMC_ERASE_GROUP_START = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 35),
  
  /**< <b>: Sets the address of the first erase group within a range to be
            selected for erase </b> */
                                     
  CSL_MMCSD_CMDTYP_MMC_ERASE_GROUP_END  =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 36),
  
  /**< <b>: Sets the address of the last erase group within a continuous range
            to be selected for erase </b> */

   CSL_MMCSD_CMDTYP_ERASE  = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 38),

  /**< <b>: Erases all previously selected write blocks </b> */
   
  CSL_MMCSD_CMDTYP_MMC_FAST_IO = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R4) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 39),
  
  /**< <b>: Used to write and read 8 bit (register) data fields. The command
      addresses a card and a register and provides the data for writing if the
      write flag is set. The R4 response contains data read from the addressed
      register. This command accesses application dependent registers which are
      not defined in the MultiMediaCard standard </b> */
                         
  CSL_MMCSD_CMDTYP_MMC_GO_IRQ_STATE = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R5) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, OD) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 40),

  /**< <b>: Sets the system into interrupt mode </b> */
  
  CSL_MMCSD_CMDTYP_SD_ACMD41_SD_APP_OP_COND =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R3) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 41),                                
                                    
  /**< <b>: Application command for SD card OP_COND </b> */  //Revisit documentation                              
  CSL_MMCSD_CMDTYP_MMC_LOCK_UNLOCK =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 42),
  /**< <b>: Used to set/reset the password or lock/unlock the card. The size
            of the data block is set by the SET_BLOCK_LEN command </b> */

  CSL_MMCSD_CMDTYP_SD_LOCK_UNLOCK =  
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 42),
  /**< <b>: Used to set/reset the password or lock/unlock the SD card. 
           The size of the data block is set by the SET_BLOCK_LEN command
       </b> */
  CSL_MMCSD_CMDTYP_SD_ACMD42_SET_CLR_CARD_DETECT = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 42),

  /**< <b>: Used to set/clear card detect </b> */
  CSL_MMCSD_CMDTYP_SD_ACMD51_SEND_SCR =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 51),
  /**< <b>: Used to send SCR </b> */
                               
  CSL_MMCSD_CMDTYP_APP_CMD =  
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, NO)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 55),
  /**< <b>: Indicates to the card that the next command is an application
            specific command rather than a standard command </b> */

  CSL_MMCSD_CMDTYP_MMC_GEN_READ_CMD = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK (MMCSD_MMCCMD_CMD, 56),
  /**< <b>: Used to get a data block from the card for general purpose /
            application specific commands. The size of the data block shall be
	    set by the SET_BLOCK_LEN command  </b> */
  
  CSL_MMCSD_CMDTYP_SD_GEN_READ_CMD =
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, READ) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 56),
  /**< <b>: Used to get a data block from the SD card for general purpose /
            application specific commands. The size of the data block shall be
	    set by the SET_BLOCK_LEN command  </b> */
  
  CSL_MMCSD_CMDTYP_MMC_GEN_WRITE_CMD = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, BUSY) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK  (MMCSD_MMCCMD_CMD, 56),
  /**< <b>: Used to transfer a data block to the card for general
           purpose / application specific commands. The size of the data block
	   shall be set by the SET_BLOCK_LEN command </b> */
  CSL_MMCSD_CMDTYP_SD_GEN_WRITE_CMD = 
                                    CSL_FMKT (MMCSD_MMCCMD_DMATRIG, NO) |  
                                    CSL_FMKT (MMCSD_MMCCMD_DCLR, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_STRMTP, BLOCK)| 
                                    CSL_FMKT (MMCSD_MMCCMD_WDATX, DATA) |
                                    CSL_FMKT (MMCSD_MMCCMD_DTRW, WRITE) |
                                    CSL_FMKT (MMCSD_MMCCMD_BSYEXP, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_RSPFMT, R1) |
                                    CSL_FMKT (MMCSD_MMCCMD_INITCK, NO) |
                                    CSL_FMKT (MMCSD_MMCCMD_PPLEN, PP) |
                                    CSL_FMK(MMCSD_MMCCMD_CMD, 56)
                                 
  /**< <b>: Used to transfer a data block to the SD card for general
           purpose / application specific commands. The size of the data block
	   shall be set by the SET_BLOCK_LEN command </b> */                                   

} CSL_MmcsdCmdType;

/**
@} */


/**\defgroup CSL_MMCSD_CONTROLCMD_ENUM  Control Commands 
*  \ingroup CSL_MMCSD_CONTROL_API
@{*/
/** \brief Enumeration for control commands passed to \a CSL_mmcsdHwControl()
*
* This is the set of commands that are passed to the \a CSL_mmcsdHwControl()
* with an optional argument type-casted to \a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration
*/

typedef enum {
    CSL_MMCSD_CMD_RESET,/**< \brief Resets MMC/SD controller specified by input
			            bitmask
                             \param (CSL_BitMask16*)
                             \return CSL_SOK
                             \sa CSL_MMCSD_RESET_DEFINE */
    CSL_MMCSD_CMD_ENABLE,/**< \brief Re-enables MMC/SD controller from RESET 
			             specified by input bitmask
                              \param (CSL_BitMask16*)
                              \return CSL_SOK
                              \sa CSL_MMCSD_RESET_DEFINE */
    CSL_MMCSD_CMD_INITSEQ_SEND,/**< \brief Send the initialization sequence
                                     necessary at the start
                                    \param NONE
                                    \return CSL_SOK */
    
    CSL_MMCSD_CMD_READ_WORD,/**< \brief Read a word from FIFO
                                 \n Address of the word to read kept in arg
                                   typecasted to (Uint32*)   
                                 \param NONE
                                 \return CSL_SOK */
    CSL_MMCSD_CMD_WRITE_WORD,/**< \brief Write a word to FIFO
                                  \param (Uint32*)
                                  \return CSL_SOK */
    CSL_MMCSD_CMD_READ,/**< \brief Read specified number of words from FIFO
                            \n The "transfernum" and error field acts as output
                             parameters reporting the actual number of words 
                             read and error if any occured during read  
                            \param (CSL_MmcsdBufferDescriptor*)     
                            \return CSL_SOK 
                            \sa  CSL_MmcsdBufferDescriptor */
    CSL_MMCSD_CMD_WRITE,/**< \brief Write specified number of words into FIFO
                            \n The "transfernum" and error field acts as output
                             parameters reporting the actual number of words 
                             written and error if any occured during write  
                            \param (CSL_MmcsdBufferDescriptor*)     
                            \return CSL_SOK 
                            \sa  CSL_MmcsdBufferDescriptor */
    CSL_MMCSD_CMD_SEND_CMD,/**< \brief Send the specified command to MMC/SD 
                                 controller 
                                \param (CSL_MmcsdCmd*)
                                \return CSL_SOK
                                \sa CSL_MmcsdCmd */
    CSL_MMCSD_CMD_SET_SD_BUSWIDTH,/**< \brief Set the SD bus width
                                       \param (CSL_MmcsdBusWidth*)
                                       \return CSL_SOK
                                       \sa CSL_MmcsdBusWidth */
    CSL_MMCSD_CMD_SET_BLKSIZE,/**< \brief Set the size of blocks to transfer
                                   \param (Uint16*)
                                   \return CSL_SOK */
    CSL_MMCSD_CMD_SET_NUMBLKS,/**< \brief Set the number of blocks to transfer
                                   \param (Uint16*)
                                   \return CSL_SOK */
    CSL_MMCSD_CMD_CLEAR_STATUS,/**< \brief Clear the interrupt status
                                    \param NONE
                                    \return CSL_SOK */
    CSL_MMCSD_CMD_CLEAR_RESPONSE,/**< \brief Clear the response
                                      \param NONE
                                      \return CSL_SOK */
    CSL_MMCSD_CMD_INTR_ENABLE,/**< \brief Enable the interrupts specified by 
                                    mask
                                   \param (CSL_BitMask16*)
                                   \return CSL_SOK 
                                   \sa CSL_MMCSD_INTEN_DEFINE */
    CSL_MMCSD_CMD_INTR_DISABLE,/**< \brief Disable the interrupts specified by 
                                    mask
                                   \param (CSL_BitMask16*)
                                   \return CSL_SOK 
                                   \sa CSL_MMCSD_INTEN_DEFINE */
    CSL_MMCSD_CMD_SET_ACCESS_WIDTH,/**< \brief Set acces width, used by FIFO
                                         control to determine full/empty flag
                                        \param (CSL_MmcsdFifoAccessWidth*)
                                        \return CSL_SOK 
                                        \sa CSL_MmcsdFifoAccessWidth */                                    
    CSL_MMCSD_CMD_SET_FIFO_THRESHOLD,/**< \brief Set FIFO threshold level
                                          \param (CSL_MmcsdFifoThreshold*)
                                          \return CSL_SOK 
                                          \sa CSL_MmcsdFifoThreshold */
    CSL_MMCSD_CMD_SET_FIFO_DIR,/**< \brief Set FIFO direction
                                    \param (CSL_MmcsdFifoDir*)
                                    \return CSL_SOK 
                                    \sa CSL_MmcsdFifoDir */
    CSL_MMCSD_CMD_RESET_FIFO,/**< \brief Reset FIFO
                                  \param NONE
                                  \return CSL_SOK */
    CSL_MMCSD_CMD_SDIO_ENABLE_RDWT_CRC,/**< \brief Enable automatic start of
                                            read wait on CRC error 
                                            \param NONE
                                            \return CSL_SOK */

    CSL_MMCSD_CMD_SDIO_START_RDWT,/**< \brief Starts read wait operation 2
                                       clocks after the end of the read data
                                       block
                                       \param NONE
                                       \return CSL_SOK */
    CSL_MMCSD_CMD_SDIO_END_RDWT,/**< \brief End read wait operation and 
                                     release DAT2  
                                     \param NONE
                                     \return CSL_SOK */
    CSL_MMCSD_CMD_SDIO_INTR_ENABLE,/**< \brief Enable the SDIO interrupts
				               specified by input bitmask 
                                        \param (CSL_BitMask16*)
                                        \return CSL_SOK 
                                        \sa CSL_MMCSD_SDIOINT_DEFINE */ 
    CSL_MMCSD_CMD_SDIO_INTR_DISABLE,/**< \brief Disable the SDIO interrupts 
				                specified by input bitmask
                                         \param (CSL_BitMask16*)
                                         \return CSL_SOK 
                                         \sa CSL_MMCSD_SDIOINT_DEFINE */ 
    CSL_MMCSD_CMD_SDIO_CLEAR_STATUS/**< \brief Clear SDIO interrupts based on
                                         mask
                                         \param (CSL_BitMask16*)
                                         \return CSL_SOK 
                                         \sa CSL_MMCSD_SDIOINT_DEFINE */ 
    
} CSL_MmcsdHwControlCmd;

/**
@} */


/**\defgroup CSL_MMCSD_QUERYCMD_ENUM Query Commands 
*  \ingroup CSL_MMCSD_QUERY_API
* @{ */

/** \brief Enumeration for queries passed to \a CSL_mmcsdGetHwStatus()
*
* This is used to get the status of different operations.The arguments
* to be passed with each enumeration if any are specified next to 
* the enumeration */

typedef enum {
    CSL_MMCSD_QUERY_CMD_INDEX,/**< \brief Queries command index register
                                   \param (Uint16*)
                                   \return CSL_SOK  */
     
    CSL_MMCSD_QUERY_RESPONSE,/**< \brief Get the response to the command last
                                         sent
                                  \param (CSL_MmcsdResponse*)
                                  \return CSL_SOK 
                                  \sa  CSL_MmcsdResponse */
    CSL_MMCSD_QUERY_SD_BUSWIDTH,/**< \brief Get the bus width of the data bus
                                     \param (CSL_MmcsdBusWidth*)
                                     \return CSL_SOK 
                                     \sa  CSL_MmcsdBusWidth */
    CSL_MMCSD_QUERY_NUMBLKS,/**< \brief Get the number of blocks left to
                                        transmit
                                 \param (Uint16*)
                                 \return CSL_SOK */
    CSL_MMCSD_QUERY_STATUS0,/**< \brief Queries status0 register
                                 \param (CSL_BitMask16*)
                                 \return CSL_SOK 
                                 \sa CSL_MMCSD_STAT_DEFINE */
    CSL_MMCSD_QUERY_STATUS1,/**< \brief Queries status1 register
                                 \param (CSL_BitMask16*)
                                 \return CSL_SOK 
                                 \sa CSL_MMCSD_STAT_DEFINE */
    CSL_MMCSD_QUERY_FIFO_STATUS,/**< \brief Queries FIFO control register
                                     \param (CSL_MmcsdFifoStat*)
                                     \return CSL_SOK 
                                     \sa CSL_MmcsdFifoStat */
    CSL_MMCSD_QUERY_INTEN_STATUS,/**< \brief Get the interrupts enabled
                                      \param (CSL_BitMask16*)
                                      \return CSL_SOK 
                                      \sa CSL_MMCSD_INTEN_DEFINE */
    CSL_MMCSD_QUERY_SDIO_STATUS,/**< \brief Get SDIO status
                                      \param (CSL_BitMask16*)
                                      \return CSL_SOK 
                                      \sa CSL_MMCSD_SDIOSTAT_DEFINE */
    CSL_MMCSD_QUERY_SDIO_INTRSTAT,/**< \brief Get SDIO interrupt status
                                       \param (CSL_BitMask16*)
                                       \return CSL_SOK 
                                       \sa CSL_MMCSD_SDIOINT_DEFINE */
    CSL_MMCSD_QUERY_SDIO_INTEN_STAT/**< \brief Get SDIO interrupts enabled
                                               status
                                        \param (CSL_BitMask16*)
                                        \return CSL_SOK 
                                        \sa CSL_MMCSD_SDIOINT_DEFINE */
} CSL_MmcsdHwStatusQuery;

/**
@} */
    


/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/
/** \brief MMC/SD/SDIO card command and argument
* 
*   Used to send command and arguments to MMC/SD controller  
*   
*   
*/

    
typedef struct CSL_MmcsdCmd {
  CSL_MmcsdCmdType  cmdType; 
  Uint32            arg;/**< This is the optional 32-bit argument that
                             accompanies the command */
  CSL_BitMask32     flag;/**< flag for generating DMA event and clearing
                              DXRDY/DRRDY optionally
			      /sa CSL_MMCSD_CMD_FLAG_DEFINE */                              
} CSL_MmcsdCmd;

/**
@} */



/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/
/** \brief Response from the MMC/SD card
* 
*  Contains response to the previously sent command
*/

typedef struct CSL_MmcsdResponse {
    Uint32  response[4];/**< The status part of the response of a MMC/SD/SDIO 
                             command */
} CSL_MmcsdResponse;

/**
@} */



/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/
/** \brief MMC/SD FIFO status
*
*   Consists FIFO threshold level and FIFO direction 
*/

typedef struct CSL_MmcsdFifoStat {
    CSL_MmcsdFifoThreshold threshold;
    CSL_MmcsdFifoDir       direction;
} CSL_MmcsdFifoStat;

/**
@} */



/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/
/** \brief MMC/SD buffer descriptor
* 
*    Used when sending/receiving data
*   
*/

typedef struct CSL_MmcsdBufferDescriptor {
  Uint32* buff;/**< Pointer to data transfer/receive buffer */            
  Uint32 num;/**< Number of words to be transferred */
  Uint32 transfernum;/**< Number of words actually transferred */
  CSL_BitMask32 error ;/**< Error bit mask
                            \sa CSL_MMCSD_LINESTAT_DEFINE */
} CSL_MmcsdBufferDescriptor;

/**
@} */



/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up MMC/SD
*
*  Used to put MMC/SD known useful state       
*   
*/
typedef struct CSL_MmcsdHwSetup {
  CSL_MmcsdEndian         readEndian;
  CSL_MmcsdEndian         writeEndian;
  CSL_MmcsdDat3EdgeDetect dat3Detect;
  CSL_MmcsdBusWidth       busWidth;
  Uint32                  dataTimeout; /**< Time out count for data read 
					\n 0: No timeout
					\n 1-2097151 CLK clocks */
  Uint32                  responseTimeout; /**< Time out count for response  
					    \n 0: No timeout
                                            \n 1-255 CLK clocks */
  Uint32                  intEnable; /**< Interrupt enable 
					\n \sa CSL_MMCSD_INTEN_DEFINE */
  CSL_MmcsdMclkCtl        memClkEnable; 
  Uint32                  clockPinRate;  /**< Clock pin rate (0-255) */
  void*  extendSetup; 
} CSL_MmcsdHwSetup;

/**
@} */




/** 
\addtogroup CSL_MMCSD_DATASTRUCT 
@{
*/
/** \brief This object contains the reference to the instance of MMC/SD
* opened using the @a CSL_mmcsdOpen()
*
* The pointer to this is passed to all MMC/SD CSL APIs
* This structure has the fields required to configure MMC/SD for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/
typedef struct CSL_MmcsdObj {
  CSL_OpenMode openMode;/**< This is the mode in which the CSL instance
                             is opened */
  CSL_Uid      uid;/**< This is a unique identifier to the instance of MMC/SD 
                        being referred to by this object */
  CSL_Xio      xio;/**< This is the variable that holds the current state
                        of a resource being shared by current instance of
                        MMC/SD with other peripherals */
  CSL_MmcsdRegsOvly regs;/**< This is a pointer to the registers of the  
                             instance of MMC/SD referred to by this object */
  CSL_MmcsdNum  mmcsdNum;/**< This is the instance of MMC/SD being referred to
                           by this object */
} CSL_MmcsdObj;


/** \brief this is a pointer to @a CSL_MmcsdObj and is passed as the first
* parameter to all MMC/SD CSL APIs */
typedef CSL_MmcsdObj* CSL_MmcsdHandle; 

/**
@} */

/*****************************************************************************\
          CSL3.x mandatory function prototype definitions
\*****************************************************************************/

/**
\defgroup CSL_MMCSD_INIT_API MMC/SD Init API
\ingroup CSL_MMCSD_FUNCTION
    \brief Initializes the MMC/SD
@{*/
CSL_Status  CSL_mmcsdInit ( void );
/**
@} */


/**
\defgroup CSL_MMCSD_OPEN_API MMCSD Open API
\ingroup CSL_MMCSD_FUNCTION
    \brief Opens if possible the instance of MMCSD requested
    \n  Reserves the specified mmcsd for use. The device can be 
     re-opened anytime after it has been normally closed if so
     required. The handle returned by this call is input as an
     essential argument for rest of the APIs described for this
     module.

    <b> Usage Constraints: </b>
    The CSL system as well as MMCSD must be succesfully initialized
    via @a CSL_sysInit() and @a CSL_mmcsdInit() before calling this
    function. Memory for the @a CSL_MmcsdObj must be allocated outside
    this call. This object must be retained while usage of this peripheral.
  
    \b Example:
    \verbatim


    CSL_MmcsdObj     mmcsdObj;
    CSL_MmcsdHwSetup mmcsdSetup;
    CSL_Status       status;
    ...
    hMmcsd = CSL_mmcsdOpen(&mmcsdobj,
                           CSL_MMCSD_0,
                           CSL_EXCLUSIVE,
                           &mmcsdSetup,
                           &status);
    \endverbatim
  
@{*/
CSL_MmcsdHandle CSL_mmcsdOpen( CSL_MmcsdObj *hMmcsdObj,
                               CSL_MmcsdNum mmcsdNum,
                               CSL_OpenMode openMode,
                               CSL_MmcsdHwSetup *hwSetup,
                               CSL_Status *status );


/**
@} */

/**
\defgroup CSL_MMCSD_CLOSE_API MMC/SD Close API
\ingroup CSL_MMCSD_FUNCTION
    \brief Closes the instance of MMC/SD requested

    
@{*/
CSL_Status  CSL_mmcsdClose( CSL_MmcsdHandle hMmcsd );


/**
@} */

/**
\defgroup CSL_MMCSD_SETUP_API MMC/SD Setup API
\ingroup CSL_MMCSD_FUNCTION
    \brief Programs the MMC/SD with the setup configuration as specified in
           the arguments
   		  
	   
@{*/
CSL_Status  CSL_mmcsdHwSetup ( CSL_MmcsdHandle hMmcsd,
                               CSL_MmcsdHwSetup *setup );
/**
@} */

/**
\defgroup CSL_MMCSD_GETSETUP_API MMC/SD GetSetup API
\ingroup CSL_MMCSD_FUNCTION
    \brief Reads the MMC/SD setup configuration 
   		  
	   
@{*/
CSL_Status  CSL_mmcsdGetHwSetup ( CSL_MmcsdHandle hMmcsd,
                                  CSL_MmcsdHwSetup *setup );
/**
@} */                                  

/**
\defgroup CSL_MMCSD_CONTROL_API MMC/SD Control API
\ingroup CSL_MMCSD_FUNCTION
    \brief Controls the different operations that can be performed by MMC/SD 
 		  
     		  
    
@{*/
CSL_Status  CSL_mmcsdHwControl( CSL_MmcsdHandle hMmcsd,
                                CSL_MmcsdHwControlCmd cmd,
                                void *arg );

/**
@} */

/**
\defgroup CSL_MMCSD_QUERY_API MMC/SD Query API
\ingroup CSL_MMCSD_FUNCTION
    \brief Returns the status of the requested operation on MMC/SD
@{*/
CSL_Status  CSL_mmcsdGetHwStatus ( CSL_MmcsdHandle hMmcsd,
                                   CSL_MmcsdHwStatusQuery query,
                                   void *response );
/**
@}*/

#ifdef __cplusplus
}
#endif
	
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:04 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:05    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
