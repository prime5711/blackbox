
/** @mainpage SPI
*
* @section Introduction
*
* @subsection xxx Purpose and Scope
* The purpose of this document is to identify a set of common CSL APIs for
* the SPI Module across various devices. The CSL developer is expected
* to refer to this document while designing APIs for these modules. Some
* of the listed APIs may not be applicable to a given SPI Module. While
* in other cases this list of APIs may not be sufficient to cover all the
* features of a particular SPI Module. The CSL developer should use his
* discretion in designing new APIs or extending the existing ones to cover
* these.
*
*
* @subsection aaa Terms and Abbreviations
*   -# CSL:  Chip Support Library
*   -# API:  Application Programmer Interface
*   -# SPI:  Serial Port Interface
*   -# MibSPI1: Multi Buffer SPI
*
* @subsection References
*    -# CSL 3.x Technical Requirements Specifications Version 0.5, dated
*       May 14th, 2003
*    -# MibSPI1 functional specification: http://dspdesign.india.ti.com/twiki/pub/Trinity/TrinityMiBSPI1/SPI_SPI_Platform.pdf
*
* @subsection Assumptions
*     The abbreviations SPI, spi1 and Spi have been used throughout
*     this document to refer to the Multi Buffer Serial Port Interface Module
*/


/** @file csl_spi1.h
 *
 *  @brief Header file for functional layer of CSL
 *
 *  Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 *  Modification 1
 *    - modified on: 26/12/2003
 *    - reason: created the sources
 *
 *  Modification 2
 *    - modified on: 31/12/2003
 *    - reason: created better documentation
 *
 *  Modification 3
 *    - modified on: 10/05/2004
 *    - reason: removed MIB from code & documnentaion
 *
 *  @date 26th Dec, 2003
 *  @author Sumant S. NaikKhanvte
 */

#ifndef _CSL_SPI_H_
#define _CSL_SPI_H_

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>

#include <cslr_spi.h>

/**************************************************************************\
* SPI global macro declarations
\**************************************************************************/


/**************************************************************************\
* SPI global typedef declarations
\**************************************************************************/

/** @brief Enumeration for control commands passed to @a CSL_spiHwControl()
 *
 * This is the set of commands that are passed to the @a CSL_spiHwControl()
 * with an optional argument type-casted to @a void* . The arguments to be
 * passed with each enumeration if any are specified next to the enumeration */
typedef enum {
    /** reset the SPI : no argument */
    CSL_SPI_CMD_PRI_RESET  =  1,
    /** power up the SPI : no argument */
    CSL_SPI_CMD_POWER_UP,
    /** power down the SPI : no argument */
    CSL_SPI_CMD_POWER_DOWN,
    /** enable the DMA transaction capability for DMA in compatibility mode :
     * no argument */
    CSL_SPI_CMD_CPT_DMA_ENABLE,
    /** disable the DMA transaction capability for DMA in compatibility mode :
     * no argument */
    CSL_SPI_CMD_CPT_DMA_DISABLE,
    /** write the data in argument to SPIDAT0 register for transmitting out :
     * argument : @a Uint16* */
    CSL_SPI_CMD_CPT_WRITE0,
    /** write the data in argument to SPIDAT1 register for transmitting out :
     * argument : @a Uint16* */
    CSL_SPI_CMD_CPT_WRITE1,
    /** read the data in SPIBUF register to the argument : argument :
     * @a CSL_SpiCptData* */
    CSL_SPI_CMD_CPT_READ,
    /** enable the interrupts in the bit-vector argument : argument :
     * @a Uint16* */
    CSL_SPI_CMD_INT_ENABLE,
    /** disable the interrupts in the bit-vector argument : argument :
     * @a Uint16* */
    CSL_SPI_CMD_INT_DISABLE,
    /** set the value passed in the argument over the pins configured as GPIO
     * : argument : @a Uint16* */
    CSL_SPI_CMD_PINS_VALUE,
    /** set the pins passed in the bit-vector argument if configured as GPIO
     * : argument : @a Uint16* */
    CSL_SPI_CMD_PINS_SET,
    /** clear the pins passed in the bit-vector argument if configured as GPIO
     * : argument : @a Uint16* */
    CSL_SPI_CMD_PINS_CLEAR,
    /** enable the data-transfer section of the SPI : no argument */
    CSL_SPI_CMD_XFER_ENABLE,
    /** disable the data-transfer section of the SPI : no argument */
    CSL_SPI_CMD_XFER_DISABLE
} CSL_SpiHwControlCmd;


/** @brief Enumeration for queries passed to @a CSL_spiGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of SPI. The arguments to be passed with each enumeration
 * if any are specified next to the enumeration */
typedef enum {
    /** get the power status of the SPI : argument :
     * @a CSL_SpiPwrStat* */
    CSL_SPI_QUERY_POWER_STATUS  =  1,
    /** get the value present on the pins as a bit-vector : argument :
     * @a Uint16* */
    CSL_SPI_QUERY_PINS_VALUE,
    /** get the value of the highest priority interrupt that has occured on
     * INT0 line : argument : @a CSL_SpiMbfInt* */
    CSL_SPI_QUERY_INT_VECTOR0,
    /** get the value of the highest priority interrupt that has occured on
     * INT1 line : argument : @a CSL_SpiMbfInt* */
    CSL_SPI_QUERY_INT_VECTOR1,
    /** get the bit-vector of interrupts that have been enabled : argument :
     * @a Uint16* */
    CSL_SPI_QUERY_INT_ENABLED,
    /** get the status of whether DMA is enabled in compatibility mode :
     * argument : @a CSL_SpiCptDma* */
    CSL_SPI_QUERY_CPT_DMA_ENABLED
} CSL_SpiHwStatusQuery;

/** @brief enumeration for Spi general interrupts */
typedef enum {
    /** interrupt on successful receive */
    CSL_SPI_INT_RX       =  CSL_FMKT(SPI_SPIINT0_RXINT, ENABLE),
    /** interrupt on receiver overrun */
    CSL_SPI_INT_OVRN     =  CSL_FMKT(SPI_SPIINT0_OVRNINT, ENABLE),
    /** interrupt on bit error */
    CSL_SPI_INT_BITERR   =  CSL_FMKT(SPI_SPIINT0_BITERR, ENABLE),
    /** interrupt on loss of synchronization between master & slave */
    CSL_SPI_INT_DESYNC   =  CSL_FMKT(SPI_SPIINT0_DESYNC, ENABLE),
    /** interrupt on parity error */
    CSL_SPI_INT_PARERR   =  CSL_FMKT(SPI_SPIINT0_PARERR, ENABLE),
    /** interrupt on timeout error */
    CSL_SPI_INT_TIMEOUT  =  CSL_FMKT(SPI_SPIINT0_TIMEOUT, ENABLE)
} CSL_SpiInt;


/** @brief enumeration for Spi status bits */
typedef enum {
    /** indicates that a word has been received in SPIBUF */
    CSL_SPI_BUFSTATUS_RXINT   =  (1 << 7),
    /** indicates that receive BUF is empty */
    CSL_SPI_BUFSTATUS_RXEPTY  =  (1 << 6),
    /** indicates that overrun occured/not */
    CSL_SPI_BUFSTATUS_RXOVRN  =  (1 << 5),
    /** indicates that transmit BUF is full */
    CSL_SPI_BUFSTATUS_TXFULL  =  (1 << 4),
    /** indicates that a bit error occurred during transaction */
    CSL_SPI_BUFSTATUS_BITERR  =  (1 << 3),
    /** indicates that desynchronization with slave detected */
    CSL_SPI_BUFSTATUS_DESYNC  =  (1 << 2),
    /** indicates that parity error is detected */
    CSL_SPI_BUFSTATUS_PARITY  =  (1 << 1),
    /** indicates timeout */
    CSL_SPI_BUFSTATUS_TMOUT   =  (1 << 0)
} CSL_SpiBufStatus;


/** @brief enumeration to hold the chip select active between 2 transfers */
typedef enum {
    /** hold chip select active between consecutive transfers */
    CSL_SPI_CSHOLD_YES  =  CSL_SPI_SPIDAT1_CSHOLD_YES,
    /** chip select to be inactivated after the each transfer */
    CSL_SPI_CSHOLD_NO   =  CSL_SPI_SPIDAT1_CSHOLD_NO
} CSL_SpiCsHold;


/** @brief enumeration to control applying the format delay to consecutive
 * transfers */
typedef enum {
    /** enable format delay between 2 consecutive transfers */
    CSL_SPI_WDELAYEN_YES  =  CSL_SPI_SPIDAT1_WDEL_DELAY,
    /** disable format delay between 2 consecutive transfers */
    CSL_SPI_WDELAYEN_NO   =  CSL_SPI_SPIDAT1_WDEL_NODELAY
} CSL_SpiWDelayEn;


/** @brief enumeration to select the required data transfer format */
typedef enum {
    /** select format 0 */
    CSL_SPI_FMTSEL_0  =  CSL_SPI_SPIDAT1_DFSEL_FMT0,
    /** select format 1 */
    CSL_SPI_FMTSEL_1  =  CSL_SPI_SPIDAT1_DFSEL_FMT1,
    /** select format 2 */
    CSL_SPI_FMTSEL_2  =  CSL_SPI_SPIDAT1_DFSEL_FMT2,
    /** select format 3 */
    CSL_SPI_FMTSEL_3  =  CSL_SPI_SPIDAT1_DFSEL_FMT3
} CSL_SpiFmtSel;


/** @brief enumeration to control the dependence of transfer in ENA signal in
 * master mode */
typedef enum {
    /** if in master mode; wait for ENA signal from slave */
    CSL_SPI_WAITEN_YES  =  CSL_SPI_SPIFMT_WAITEN_YES,
    /** do not wait for ENA signal from slave */
    CSL_SPI_WAITEN_NO   =  CSL_SPI_SPIFMT_WAITEN_NO
} CSL_SpiWaitEn;


/** @brief enumeration to control the parity setting in the data format */
typedef enum {
    /** enable even parity */
    CSL_SPI_PARITY_EVEN     =  (CSL_SPI_SPIFMT_PARITY_ENABLE |
                               (CSL_SPI_SPIFMT_PARPOL_EVEN << 1)),
    /** enable odd parity */
    CSL_SPI_PARITY_ODD      =  (CSL_SPI_SPIFMT_PARITY_ENABLE |
                               (CSL_SPI_SPIFMT_PARPOL_ODD << 1)),
    /** disable parity */
    CSL_SPI_PARITY_DISABLE  =  CSL_SPI_SPIFMT_PARITY_DISABLE
} CSL_SpiParity;


/** @brief enumeration to control the polarity of serial clock */
typedef enum {
    /** clock is low when inactive */
    CSL_SPI_POLARITY_INACTIVELO = CSL_SPI_SPIFMT_POLARITY_INACTIVELO,
    /** clock is high when inactive */
    CSL_SPI_POLARITY_INACTIVEHI = CSL_SPI_SPIFMT_POLARITY_INACTIVEHI
} CSL_SpiPolarity;


/** @brief enumeration to control phase relationship between data & clock */
typedef enum {
    /** data & clock in phase */
    CSL_SPI_PHASE_IN   =  CSL_SPI_SPIFMT_PHASE_IN,
    /** data 1/2 cycle before clock */
    CSL_SPI_PHASE_OUT  =  CSL_SPI_SPIFMT_PHASE_OUT
} CSL_SpiPhase;


/** @brief enumeration to control direction of the word during transfer */
typedef enum {
    /** transfer MSB first */
    CSL_SPI_SHDIR_MSBFIRST  =  CSL_SPI_SPIFMT_SHIFTDIR_MSBFIRST,
    /** transfer LSB first */
    CSL_SPI_SHDIR_LSBFIRST  =  CSL_SPI_SPIFMT_SHIFTDIR_LSBFIRST
} CSL_SpiShDir;


/** @brief enumeration to control the protocol of Spi */
typedef enum {
    /** use the multi-buffer mode */
    CSL_SPI_PROTOCOL_MULTIBUFFER  =  CSL_SPI_MIBSPIE_MIBSPIENA_YES,
    /** use the compatibility mode */
    CSL_SPI_PROTOCOL_COMPATIBLE   =  CSL_SPI_MIBSPIE_MIBSPIENA_NO
} CSL_SpiProtocol;


/** @brief enumeration tocontrol the operating mode of Spi */
typedef enum {
    /** operate as master */
    CSL_SPI_OPMOD_MASTER  =  ((CSL_SPI_SPIGCR1_CLKMOD_INTERNAL << 1) |
                                 CSL_SPI_SPIGCR1_MASTER_YES),
    /** operate as slave */
    CSL_SPI_OPMOD_SLAVE   =  ((CSL_SPI_SPIGCR1_CLKMOD_EXTERNAL << 1) |
                                 CSL_SPI_SPIGCR1_MASTER_NO)
} CSL_SpiOpMod;


/** @brief enumeration to control the SPIENA status whe inactive */
typedef enum {
    /** force SPIENA signal high-z when inactive */
    CSL_SPI_ENAHIZ_YES  =  CSL_SPI_SPIINT0_ENAHIGHZ_ENABLE,
    /** keep SPIENA signal a value when inactive */
    CSL_SPI_ENAHIZ_NO   =  CSL_SPI_SPIINT0_ENAHIGHZ_DISABLE
} CSL_SpiEnaHiZ;


typedef enum {
    /* operate on functionality register */
    CSL_SPI_GPIOTYPE_FUNC      =  0,
    /* operate on direction register */
    CSL_SPI_GPIOTYPE_DIR       =  1,
    /* operate on pull control register */
    CSL_SPI_GPIOTYPE_PULLCTRL  =  2,
    /* operate on open drain register */
    CSL_SPI_GPIOTYPE_OPNDRAIN  =  3
} _CSL_SpiGpioType;


/** @brief enumeration for Spi power modes */
typedef enum {
    /** Spi is in active state */
    CSL_SPI_PWRSTAT_ACTIVE  =  CSL_SPI_SPIGCR1_PWRDN_ACTIVE,
    /** Spi is powered down */
    CSL_SPI_PWRSTAT_DOWN    =  CSL_SPI_SPIGCR1_PWRDN_PWRDN
} CSL_SpiPwrStat;


/** @brief enumeartion for Spi serial communication pins */
typedef enum {
    /** SOMI pin */
    CSL_SPI_PINTYPE_SOMI  =  CSL_FMKT(SPI_SPIPC0_SOMIFUN, SPI),
    /** SIMO pin */
    CSL_SPI_PINTYPE_SIMO  =  CSL_FMKT(SPI_SPIPC0_SIMOFUN, SPI),
    /** CLK pin */
    CSL_SPI_PINTYPE_CLK   =  CSL_FMKT(SPI_SPIPC0_CLKFUN, SPI),
    /** ENA pin */
    CSL_SPI_PINTYPE_ENA   =  CSL_FMKT(SPI_SPIPC0_ENAFUN, SPI),
    /** SCS7 pin */
    CSL_SPI_PINTYPE_SCS7  =  CSL_FMKT(SPI_SPIPC0_SCSFUN7, SPI),
    /** SCS6 pin */
    CSL_SPI_PINTYPE_SCS6  =  CSL_FMKT(SPI_SPIPC0_SCSFUN6, SPI),
    /** SCS5 pin */
    CSL_SPI_PINTYPE_SCS5  =  CSL_FMKT(SPI_SPIPC0_SCSFUN5, SPI),
    /** SCS4 pin */
    CSL_SPI_PINTYPE_SCS4  =  CSL_FMKT(SPI_SPIPC0_SCSFUN4, SPI),
    /** SCS3 pin */
    CSL_SPI_PINTYPE_SCS3  =  CSL_FMKT(SPI_SPIPC0_SCSFUN3, SPI),
    /** SCS2 pin */
    CSL_SPI_PINTYPE_SCS2  =  CSL_FMKT(SPI_SPIPC0_SCSFUN2, SPI),
    /** SCS1 pin */
    CSL_SPI_PINTYPE_SCS1  =  CSL_FMKT(SPI_SPIPC0_SCSFUN1, SPI),
    /** SCS0 pin */
    CSL_SPI_PINTYPE_SCS0  =  CSL_FMKT(SPI_SPIPC0_SCSFUN0, SPI)
} CSL_SpiPinType;


/** @brief enumeration to control DMA enabling in compatibility mode */
typedef enum {
    /** enable dma servicing in compatibility mode */
    CSL_SPI_CPTDMA_ENABLE   =  CSL_SPI_SPIINT0_DMAREQ_ENABLE,
    /** disable dma servicing in compatibility mode */
    CSL_SPI_CPTDMA_DISABLE  =  CSL_SPI_SPIINT0_DMAREQ_DISABLE
} CSL_SpiCptDma;


/** @brief enumeration to control reset of transfer mechnism of Spi */
typedef enum {
    /** enable spi to begin transfers */
    CSL_SPI_XFEREN_DISABLE  =  CSL_SPI_SPIGCR1_SPIEN_INRESET,
    /** hold spi transfer mechanism in reset */
    CSL_SPI_XFEREN_ENABLE   =  CSL_SPI_SPIGCR1_SPIEN_ACTIVE
} CSL_SpiXferEn;


/** @brief this object contains the reference to the instance of SPI
 * opened using the @a CSL_spiOpen()
 *
 * An object related to this structure is passed to all SPI CSL APIs
 * as the first argument */
typedef struct CSL_SpiObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode        openMode;
    /** This is a unique identifier to the instance of SPI being referred
     * to by this object */
    CSL_Uid             uid;
    /** This is the variable that contains the current state of a resource
     * being shared by current instance of SPI with other peripherals */
    CSL_Xio             xio;
    /** This is a pointer to the registers of the instance of SPI referred
     * to by this object */
    CSL_SpiRegsOvly  regs;
    /** This is the instance of SPI being referred to by this object */
    CSL_SpiNum       perNum;
} CSL_SpiObj;


/** @brief a pointer to @a CSL_SpiObj; this is passed to all SPI CSL
 * APIs */
typedef struct CSL_SpiObj *CSL_SpiHandle;


/** @brief sets up the properties if the pins of Spi
 *
 * This object is used to setup or get the setup of the pins in Spi */
typedef struct CSL_SpiHwSetupPins {
    /** decides if the pins will be Spi or GPIO */
    Uint32  *func;
    /** if GPIO, decides the directions of the pins */
    Uint32  *dir;
    /** if configured as GPIO output, decides if the pull control is to
     * be turned on */
    Uint32  *pullCtrl;
    /** if configured as GPIO output, decides if the pin should output
     * logic 1 or tristate */
    Uint32  *opnDrain;
} CSL_SpiHwSetupPins;


/** @brief sets up the format selection for a set of transfers
 *
 * This object is used to setup or get the setup of the format selection
 * for a set of transfers */
typedef struct CSL_SpiHwSetupFmtCtrl {
    /** decides if chip select is to be held active between transfers */
    CSL_SpiCsHold    csHold;
    /** decides if delay specified in the selected format must be allowed
     * between 2 consecutive transfers */
    CSL_SpiWDelayEn  wDel;
    /** decides which format to select */
    CSL_SpiFmtSel    fmtSel;
    /** defines the chip select that will be activated for the transfer */
    Uint8               cSel;
} CSL_SpiHwSetupFmtCtrl;


/** @brief sets up the Spi for compatibility mode
 *
 * This structure is used to setup or get the setup of Spi in comaptibility
 * mode */
typedef struct CSL_SpiHwSetupCpt {
    /** selects if interrupts should go to lines INT0 or INT1 */
    Uint32                    *lvl;
    /** selects the format & associated controls */
    CSL_SpiHwSetupFmtCtrl  *fmtCtrlPtr;
} CSL_SpiHwSetupCpt;


/** @brief sets up the a formatting for an outgoing word
 *
 * This object is used to set up or get the setup of the format registers in
 * Spi */
typedef struct CSL_SpiHwSetupPriFmt {
    /** delay between 2 consecutive words */
    Uint8             wDelay;
    /** the factor with which to multiply functional clock in order to get the
     * serial clock */
    Uint8             preScale;
    /** the length of the word to be transmitted and/or received */
    Uint8             charLen;
    /** if in master mode; whether Spi should wait for ENA from slave */
    CSL_SpiWaitEn  waitEna;
    /** whether parity should be enabled; if enabled then even or odd */
    CSL_SpiParity  parity;
    /** whether clock should be high or low when inactive */
    CSL_SpiPolarity  polarity;
    /** whether data should be in phase of 1/2 cycle ahead of the clock */
    CSL_SpiPhase   phase;
    /** whether LSB or MSB should be shifted first */
    CSL_SpiShDir   shiftDir;
} CSL_SpiHwSetupPriFmt;


/** @brief sets up the parameters to be setup in priority mode
 *
 * This object is used to setup or get the setup of the parameters to be setup
 * in priority mode */
typedef struct CSL_SpiHwSetupPri {
    /** array of pointers to structures of formats of an outgoing word */
    CSL_SpiHwSetupPriFmt  *fmt[4];
} CSL_SpiHwSetupPri;


/** @brief sets up the parameters that are needed by multi-buffer as well as
 * compatibility modes
 *
 * This objetc is used to set up or get the setup of parameters that are needed
 * by multi-buffer as well as compatibility modes */
typedef struct CSL_SpiHwSetupGen {
    /** whether to use Spi in compatibility mode or multi-buffer mode */
    CSL_SpiProtocol   protocol;
    /** master or slave mode */
    CSL_SpiOpMod      opMode;
    /** whether ENA signal should be tristated when inactive or if it should
     * bear a value */
    CSL_SpiEnaHiZ     enaHiZ;
    /** the default value on Chip select when inactive */
    Uint8                csDefault;
    /** Chip-select-active-to-transmit-start-delay */
    Uint8                c2tDelay;
    /** Transmit-end-to-chip-select-inactive-delay */
    Uint8                t2cDelay;
    /** Transmit-data-finished-to-ENA-pin-inactive-time-out */
    Uint8                t2eTmout;
    /** Chip-select-active-to-ENA-signal-active-time-out */
    Uint8                c2eTmout;
} CSL_SpiHwSetupGen;


/** @brief main structure that is used to setup the SPI 
 *
 * This structure is used to setup or obtain the existing setup of SPI
 * using @a CSL_spiHwSetup() & @a CSL_spiGetHwStatus() functions
 * respectively. If a particular member pointer is null, then these functions
 * do not setup or get the setup of the corresponding part of SPI
 * respectively */
typedef struct CSL_SpiHwSetup {
    /** This pointer to @a CSL_SpiHwSetupCommon is used to hold
     * information on the common part of the SPI setup */
    CSL_SpiHwSetupGen   *genPtr;
    /** This pointer to @a CSL_SpiHwSetupFifo is used to hold information
     * on the data FIFO part of the SPI setup */
    CSL_SpiHwSetupPri   *priPtr;
    /** This pointer to @a CSL_SpiHwSetupSdio is used to hold information
     * on the SDIO part of the SPI setup */
    CSL_SpiHwSetupCpt   *cptPtr;
    /** This pointer to @a CSL_SpiHwSetupSdio is used to hold information
     * on the SPI part of the SPI setup */
    CSL_SpiHwSetupPins  *pinsPtr;
} CSL_SpiHwSetup;


/** @brief gets the information for interrupt vectors */
typedef struct CSL_SpiIntVec {
    /** interrupt vector number */
    Uint8  intVal;
    /** status of suspend */
    Bool   suspend;
} CSL_SpiIntVec;


/** @brief gets the status of buffer after a transfer */
typedef struct CSL_SpiBufStat {
    /** status as a bit-vector of the different events */
    Uint8  status;
    /** status of the chip select during last transfer */
    Uint8  cSel;
} CSL_SpiBufStat;


typedef struct CSL_SpiCptData {
    /** Place to hold the data */
    Uint16            data;
    /** Place to hold the status */
    CSL_SpiBufStat *bufStat;
} CSL_SpiCptData;


/** default setting for @a CSL_SpiHwSetup */
#define CSL_SPI_HWSETUP_DEFAULTS { \
    NULL,                             \
    NULL,                             \
    NULL,                             \
    NULL                              \
    }


/** default setting for @a CSL_SpiHwSetupGen */
#define CSL_SPI_HWSETUP_GEN_DEFAULTS { \
    CSL_SPI_PROTOCOL_COMPATIBLE,       \
    CSL_SPI_OPMOD_SLAVE,               \
    CSL_SPI_ENAHIZ_NO,                 \
    0,                                    \
    0,                                    \
    0,                                    \
    0,                                    \
    0                                     \
    }


/** default setting for @a CSL_SpiHwSetupPri */
#define CSL_SPI_HWSETUP_PRI_DEFAULTS { \
    {                                     \
        NULL,                             \
        NULL,                             \
        NULL,                             \
        NULL                              \
    }                                     \
    }


/** default setting for @a CSL_SpiHwSetupPriFmt */
#define CSL_SPI_HWSETUP_PRI_FMT_DEFAULTS { \
    0,                                        \
    0,                                        \
    0,                                        \
    CSL_SPI_WAITEN_NO,                     \
    CSL_SPI_PARITY_DISABLE,                \
    CSL_SPI_PHASE_IN,                      \
    CSL_SPI_SHDIR_LSBFIRST                 \
    }


/** default setting for @a CSL_SpiHwSetupCpt */
#define CSL_SPI_HWSETUP_CPT_DEFAULTS { \
    NULL,                                 \
    NULL                                  \
    }


/** default setting for @a CSL_SpiHwSetupFmtCtrl */
#define CSL_SPI_HWSETUP_FMTCTRL_DEFAULTS { \
    CSL_SPI_CSHOLD_NO,                     \
    CSL_SPI_WDELAYEN_NO,                   \
    CSL_SPI_FMTSEL_0,                      \
    0,                                        \
    }


/** default setting for @a CSL_SpiHwSetupPins */
#define CSL_SPI_HWSETUP_PINS_DEFAULTS { \
    NULL,                                  \
    NULL,                                  \
    NULL,                                  \
    NULL                                   \
    }


/** @brief opens if possible the instance of SPI requested */
CSL_SpiHandle CSL_spiOpen(
    /** pointer to the object that holds reference to the instance of SPI
     * requested after the call */
    CSL_SpiObj                            *hSpiObj,
    /** instance of SPI to which a handle is requested */
    CSL_SpiNum                            spiNum,
    /** specifies if SPI should be opened with excusive or share access to
     * the associate pins */
    CSL_OpenMode                             openMode,
    /** if a valid structure is passed (not @a NULL), then the 
     * @a CSL_spiHwSetup() is called with this parameter */
    CSL_SpiHwSetup                        *hwSetup,
    /** This returns the status (success/errors) of the call */
    CSL_Status                               *status
);



/** @brief closes the instance of SPI requested */
CSL_Status  CSL_spiClose(
    /** pointer to the object that holds reference to the instance of SPI
     * requested after the call */
    CSL_SpiHandle                         hSpi
);



/** @brief programs the SPI to a useful state as specified */
CSL_Status  CSL_spiHwSetup(
    /** pointer to the object that holds reference to the instance of SPI
     * requested after the call */
    CSL_SpiHandle                         hSpi,
    /** pointer to setup structure which contains the information to program
     * SPI to a useful state */
    CSL_SpiHwSetup                        *setup
);


/** @brief controls the different operations that can be performed by
 * SPI */
CSL_Status  CSL_spiHwControl(
    /** pointer to the object that holds reference to the instance of SPI
     * requested after the call */
    CSL_SpiHandle                         hSpi,
    /** the command to this API which indicates the action to be taken */
    CSL_SpiHwControlCmd                   cmd,
    /** an optional argument @a void* casted */
    void                                     *arg
);


/** @brief initializes the SPI */
CSL_Status  CSL_spiInit(
    void
);


/** @brief returns the status of different operations or the current setup of
 * SPI */
CSL_Status  CSL_spiGetHwStatus(
    /** pointer to the object that holds reference to the instance of SPI
     * requested after the call */
    CSL_SpiHandle                         hSpi,
    /** the query to this API which indicates the status/setup to be
     * returned */
    CSL_SpiHwStatusQuery                  query,
    /** placeholder to return the status; @a void* casted */
    void                                     *response
);

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:08 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:07    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
