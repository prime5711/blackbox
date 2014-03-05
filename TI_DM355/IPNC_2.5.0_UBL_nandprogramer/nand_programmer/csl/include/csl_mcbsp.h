/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage MCBSP CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the MCBSP module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given MCBSP module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular MCBSP Module. The CSL developer should use his discretion designing
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


/** @file csl_mcbsp.h
 *
 *  @brief Header file for functional layer of McBSP CSL
 *
 *  Description
 *  - The different symbolic constants, enumerations, structure definitions
 *    and function prototype declarations
 *
 *  @date    29 June, 2004
 *  @author  Pratheesh Gangadhar
 */

/**
 * @defgroup CSL_MCBSP_API MCBSP
 */

/**
 * @defgroup CSL_MCBSP_DATASTRUCT MCBSP Data Structures
 * @ingroup CSL_MCBSP_API
 */
/**
 * @defgroup CSL_MCBSP_SYMBOL  Symbols
 * @ingroup CSL_MCBSP_API
 */
/**
 * @defgroup CSL_MCBSP_ENUM  Enumerated Data Types
 * @ingroup CSL_MCBSP_API
 */
/**
 * @defgroup CSL_MCBSP_FUNCTION  Functions
 * @ingroup CSL_MCBSP_API
 */

#ifndef _CSL_MCBSP_H_
#define _CSL_MCBSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_mcbsp.h>

/**************************************************************************\
* MCBSP global macro declarations
\**************************************************************************/

/** @defgroup CSL_MCBSP_IO_SYMBOL General purpose IO pin selection
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * Use this symbol as pin mask for @a CSL_mcbspIoRead() and
 * @a CSL_mcbspIoWrite() functions
 *
 * @{ */
/** I/O Pin Input/Output configuration for CLKX Pin       */
#define CSL_MCBSP_IO_CLKX                             (1)

/** I/O Pin Input/Output configuration for FSX  Pin       */
#define CSL_MCBSP_IO_FSX                              (2)

/** Not Configurable. Always Output.                      */
#define CSL_MCBSP_IO_DX                               (4)

/** I/O Pin Input/Output configuration for CLKR Pin       */
#define CSL_MCBSP_IO_CLKR                             (8)

/** I/O Pin Input/Output configuration for FSR Pin        */
#define CSL_MCBSP_IO_FSR                              (16)

/** Not Configurable. Always Input.                        */
#define CSL_MCBSP_IO_DR                               (32)

/** Not Configurable. Always Input.                        */
#define CSL_MCBSP_IO_CLKS                             (64)
/**
@} */


/** @defgroup CSL_MCBSP_IDLECTRL_SYMBOL Idle Control
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * Use this symbol as enable/disable idle control bitmask for
 * @a CSL_mcbspHwControl() function
 *
 * @{ */
/** To disable idle controller */
#define CSL_MCBSP_CTRL_IDLE_DISABLE                     (0)

/** To enable idle controller  */
#define CSL_MCBSP_CTRL_IDLE_ENABLE                      (1)
/**
@} */



/** @defgroup CSL_MCBSP_CTRL_SYMBOL Enable/Disable Controls
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * Use this symbol as enable/disable control bitmask for
 * @a CSL_mcbspHwControl() function
 *
 * @{ */
/** To enable Receiver in resetControl Function             */
#define CSL_MCBSP_CTRL_RX_ENABLE                      (1)

/** To enable Transmitter in resetControl Function          */
#define CSL_MCBSP_CTRL_TX_ENABLE                      (2)

/** To disable Receiver in resetControl Function            */
#define CSL_MCBSP_CTRL_RX_DISABLE                     (4)

/** To disable Transmitter in resetControl Function         */
#define CSL_MCBSP_CTRL_TX_DISABLE                     (8)

/** To enable Frame Sync Generation in resetControl Function */
#define CSL_MCBSP_CTRL_FSYNC_ENABLE                   (16)

/** To enable Sample Rate Generator in resetControl Function */
#define CSL_MCBSP_CTRL_SRG_ENABLE                     (32)

/** To disable Frame Sync Generation in resetControl Function */
#define CSL_MCBSP_CTRL_FSYNC_DISABLE                  (64)

/** To disable Sample Rate Generator in resetControl Function */
#define CSL_MCBSP_CTRL_SRG_DISABLE                    (128)
/**
@} */

/** @defgroup CSL_DEVSTATUS_SYMBOL RCV/XMT status symbols
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * Use this symbol to compare the return value of @a CSL_mcbspGetHwStatus()
 * function for @a CSL_MCBSP_QUERY_DEV_STATUS query
 * \n For eg:- On RFULL event, (response & CSL_MCBSP_RFULL) == 0x0004
 *
 * @{ */
/** RCV ready status */
#define CSL_MCBSP_RRDY        0x0001
/** XMT ready status */
#define CSL_MCBSP_XRDY        0x0002
/** RCV full status */
#define CSL_MCBSP_RFULL       0x0004
/** XMT empty status */
#define CSL_MCBSP_XEMPTY      0x0008
/** RCV frame sync error status */
#define CSL_MCBSP_RSYNCERR    0x0010
/** XMT frame sync error status */
#define CSL_MCBSP_XSYNCERR    0x0020
/**
@} */

/** @defgroup CSL_EMCBSP_SYMBOL Error codes
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * @{ */
/** Invalid Control Command */
#define CSL_EMCBSP_INVCNTLCMD                         (CSL_EMCBSP_FIRST - 0)

/** Invalid Query */
#define CSL_EMCBSP_INVQUERY                           (CSL_EMCBSP_FIRST - 1)

/** Invalid Parameter */
#define CSL_EMCBSP_INVPARAMS                          (CSL_EMCBSP_FIRST - 2)

/** Invalid Size */
#define CSL_EMCBSP_INVSIZE                            (CSL_EMCBSP_FIRST - 3)

/** 'Does not exist' */
#define CSL_EMCBSP_NOTEXIST                           (CSL_EMCBSP_FIRST - 4)

/** Invalid mode to conduct operation */ 
#define CSL_EMCBSP_INVMODE                            (CSL_EMCBSP_FIRST - 5)
/**
@} */

/**
 * @defgroup CSL_MCBSP_DEFAULT_SYMBOL MCBSP CSL Defaults
 * @ingroup CSL_MCBSP_SYMBOL
 *
 * @{ */
 
/** Data Setup defaults */
#define CSL_MCBSP_DATASETUP_DEFAULTS   {            \
   (CSL_McbspPhase)CSL_MCBSP_PHASE_SINGLE,          \
   (CSL_McbspWordLen)CSL_MCBSP_WORDLEN_16,          \
   1,                                               \
   (CSL_McbspWordLen)0,                             \
   0,                                               \
   (CSL_McbspFrmSync)CSL_MCBSP_FRMSYNC_DETECT,      \
   (CSL_McbspCompand)CSL_MCBSP_COMPAND_OFF_MSB_FIRST, \
   (CSL_McbspDataDelay)CSL_MCBSP_DATADELAY_0_BIT,   \
   (CSL_McbspRjustDxena)0,                          \
   (CSL_McbspIntMode)CSL_MCBSP_INTMODE_ON_READY,     \
   (CSL_McbspBitReversal)CSL_MCBSP_32BIT_REVERS_DISABLE }\


/** Clock Setup defaults */
#define CSL_MCBSP_CLOCKSETUP_DEFAULTS  {                \
 (CSL_McbspFsClkMode)CSL_MCBSP_FSCLKMODE_EXTERNAL,      \
 (CSL_McbspFsClkMode)CSL_MCBSP_FSCLKMODE_EXTERNAL,      \
 (CSL_McbspTxRxClkMode)CSL_MCBSP_TXRXCLKMODE_INTERNAL,  \
 (CSL_McbspTxRxClkMode)CSL_MCBSP_TXRXCLKMODE_EXTERNAL,  \
 (CSL_McbspFsPol)0,                                     \
 (CSL_McbspFsPol)0,                                     \
 (CSL_McbspClkPol)0,                                    \
 (CSL_McbspClkPol)0,                                    \
 1,                                                     \
 0x40,                                                  \
 0xFF,                                                  \
 (CSL_McbspSrgClk)0,                                    \
 (CSL_McbspClkPol)0,                                    \
 (CSL_McbspTxFsMode)CSL_MCBSP_SRG_FRMSYNC_SRG,          \
 (CSL_McbspClkgSyncMode)CSL_MCBSP_CLKGSYNCMODE_OFF     }\


/** Multichannel Setup defaults */
#define CSL_MCBSP_MULTICHAN_DEFAULTS  {                 \
 (CSL_McbspPartMode)CSL_MCBSP_PARTMODE_2PARTITION,      \
 (CSL_McbspPartMode)CSL_MCBSP_PARTMODE_2PARTITION,      \
 (Uint16)0,                                             \
 (Uint16)0,                                             \
 (CSL_McbspPABlk)CSL_MCBSP_PABLK_0,                     \
 (CSL_McbspPBBlk)CSL_MCBSP_PBBLK_1,                     \
 (CSL_McbspPABlk)CSL_MCBSP_PABLK_0,                     \
 (CSL_McbspPBBlk)CSL_MCBSP_PBBLK_1,                     \
 (Uint16)0,                                             \
 (Uint16)0                                             }\


/** Global parameters Setup defaults */
#define CSL_MCBSP_GLOBALSETUP_DEFAULTS  {               \
 (CSL_McbspIOMode)CSL_MCBSP_IOMODE_TXDIS_RXDIS,         \
 (CSL_McbspDlbMode)CSL_MCBSP_DLBMODE_OFF,               \
 (CSL_McbspClkStp)CSL_MCBSP_CLKSTP_DISABLE }             \


/** Default Emulation mode - Stop */
#define CSL_MCBSP_EMUMODE_DEFAULT  CSL_MCBSP_EMU_STOP

/** Default DX state */
#define CSL_MCBSP_DXSTATE_DEFAULT  CSL_MCBSP_DX_HIGHZ

/** Default Receive superframe configuration */
#define CSL_MCBSP_RSYNCCNT_DEFAULT  (Uint16)0

/** Default Transmit superframe configuration */
#define CSL_MCBSP_XSYNCCNT_DEFAULT  (Uint16)0

/** Extend Setup default - NULL */
#define CSL_MCBSP_EXTENDSETUP_DEFAULT  NULL
    

/**
@} */
        
/**************************************************************************\
* MCBSP global typedef declarations
\**************************************************************************/

/**
 * @defgroup CSL_MCBSP_WORDLEN_ENUM Word length
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Word lengths supported on MCBSP
 *
 * Use this symbol for setting Word Length in each Phase for every Frame
 * @{ */
typedef enum {
    CSL_MCBSP_WORDLEN_8        =                  0,
    CSL_MCBSP_WORDLEN_12       =                  1,
    CSL_MCBSP_WORDLEN_16       =                  2,
    CSL_MCBSP_WORDLEN_20       =                  3,
    CSL_MCBSP_WORDLEN_24       =                  4,
    CSL_MCBSP_WORDLEN_32       =                  5
} CSL_McbspWordLen;
/**
@} */

/**
 * @defgroup CSL_MCBSP_COMPAND_ENUM Companding options
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief MCBSP companding options
 *
 * Use this symbol to set Companding related options
 * @{ */
typedef enum {
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST =                 0,
    CSL_MCBSP_COMPAND_OFF_LSB_FIRST =                 1,
    CSL_MCBSP_COMPAND_MULAW         =                 2,
    CSL_MCBSP_COMPAND_ALAW          =                 3
} CSL_McbspCompand;
/**
@} */

/**
 * @defgroup CSL_MCBSP_DATADELAY_ENUM Data delay
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Data delay in bits
 *
 * Use this symbol to set XMT/RCV Data Delay (in bits)
 * @{ */
typedef enum {
    CSL_MCBSP_DATADELAY_0_BIT      =                 0,
    CSL_MCBSP_DATADELAY_1_BIT      =                 1,
    CSL_MCBSP_DATADELAY_2_BITS     =                 2
} CSL_McbspDataDelay;
/**
@} */

/**
 * @defgroup CSL_MCBSP_EVENT_ENUM Interrupt mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief MCBSP Interrupt mode
 *
 * Use this symbol to set Interrupt mode (i.e. source of interrupt generation).
 * This symbol is used on both RCV and XMT for RINT and XINT generation mode.
 * @{ */
typedef enum {
    /** Interrupt generated on RRDY of RCV or XRDY of XMT */
    CSL_MCBSP_INTMODE_ON_READY         =                  0,
    /** Interrupt generated on end of 16-channel block transfer
     *  in multichannel mode */
    CSL_MCBSP_INTMODE_ON_EOB           =                  1,
    /** Interrupt generated on frame sync */
    CSL_MCBSP_INTMODE_ON_FSYNC         =                  2,
    /** Interrupt generated on synchronisation error */
    CSL_MCBSP_INTMODE_ON_SYNCERR       =                  3
} CSL_McbspIntMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_FSCLKMODE_ENUM Frame sync clock source
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Frame sync clock source
 *
 * Use this symbol to set the frame sync clock source as internal or external
 * @{ */
typedef enum {
    CSL_MCBSP_FSCLKMODE_EXTERNAL    =                  0,
    CSL_MCBSP_FSCLKMODE_INTERNAL    =                  1
} CSL_McbspFsClkMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CLKMODE_ENUM Clock Mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Clock source
 *
 * Use this symbol to set the clock source as internal or external
 * @{ */
typedef enum {
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL    =                  0,
    CSL_MCBSP_TXRXCLKMODE_INTERNAL    =                  1
} CSL_McbspTxRxClkMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_FSPOLAR_ENUM Frame sync polarity
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Frame sync polarity
 *
 * Use this symbol to set frame sync polarity as active-high or active-low
 * @{ */
typedef enum {
    CSL_MCBSP_FSPOL_ACTIVE_HIGH      =                  0,
    CSL_MCBSP_FSPOL_ACTIVE_LOW       =                  1
} CSL_McbspFsPol;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CLKPOLAR_ENUM Clock polarity
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Clock polarity
 *
 * Use this symbol to set XMT or RCV clock polarity as rising or falling edge
 * @{ */
typedef enum {
    CSL_MCBSP_CLKPOL_TX_RISING_EDGE      =                 0,
    CSL_MCBSP_CLKPOL_RX_FALLING_EDGE     =                 0,
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE     =                 0,
    CSL_MCBSP_CLKPOL_TX_FALLING_EDGE     =                 1,
    CSL_MCBSP_CLKPOL_RX_RISING_EDGE      =                 1,
    CSL_MCBSP_CLKPOL_SRG_FALLING_EDGE    =                 1
} CSL_McbspClkPol;
/**
@} */

/**
 * @defgroup CSL_MCBSP_SRGCLK_ENUM SRG clock source
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief SRG clock source
 *
 * Use this symbol to select input clock source for Sample Rate Generator
 * @{ */
typedef enum {
    CSL_MCBSP_SRGCLK_CLKS                 =                  0,
    CSL_MCBSP_SRGCLK_CLKCPU               =                  1,
    CSL_MCBSP_SRGCLK_CLKR                 =                  2,
    CSL_MCBSP_SRGCLK_CLKX                 =                  3
} CSL_McbspSrgClk;
/**
@} */

/**
 * @defgroup CSL_MCBSP_TXFSMODE_ENUM XMT Frame Sync generation mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief XMT Frame Sync generation mode
 *
 * Use this symbol to set XMT Frame Sync generation mode
 * @{ */
typedef enum {
    CSL_MCBSP_TXFSMODE_DXRCOPY  =                  0,
    CSL_MCBSP_TXFSMODE_SRG      =                  1
} CSL_McbspTxFsMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_IOMODE_ENUM XMT and RCV IO Mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief XMT and RCV IO Mode
 *
 * Use this symbol to Enable/Disable IO Mode for XMT and RCV
 * @{ */
typedef enum {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS       =                  0,
    CSL_MCBSP_IOMODE_TXDIS_RXEN        =                  1,
    CSL_MCBSP_IOMODE_TXEN_RXDIS        =                  2,
    CSL_MCBSP_IOMODE_TXEN_RXEN         =                  3
} CSL_McbspIOMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CLKSTP_ENUM Clock Stop Mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Clock Stop Mode
 *
 * Use this symbol to Enable/Disable Clock Stop Mode
 * @{ */
typedef enum {
    CSL_MCBSP_CLKSTP_DISABLE       =                  1,
    CSL_MCBSP_CLKSTP_WITHOUT_DELAY =                  2,
    CSL_MCBSP_CLKSTP_WITH_DELAY    =                  3
} CSL_McbspClkStp;
/**
@} */

/**
 * @defgroup CSL_MCBSP_PARTMODE_ENUM Multichannel mode Partition type
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Multichannel mode Partition type
 *
 * Use this symbol to select the partition type in multichannel mode
 * @{ */
typedef enum {
    CSL_MCBSP_PARTMODE_2PARTITION           =                  0,
    CSL_MCBSP_PARTMODE_8PARTITION           =                  1
} CSL_McbspPartMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_PABLK_ENUM Multichannel mode PartitionA block
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Multichannel mode PartitionA block
 *
 * Use this symbol to assign Blocks to Partition-A in multichannel mode
 * @{ */
typedef enum {
    CSL_MCBSP_PABLK_0              =                  0,
    CSL_MCBSP_PABLK_2              =                  1,
    CSL_MCBSP_PABLK_4              =                  2,
    CSL_MCBSP_PABLK_6              =                  3
} CSL_McbspPABlk;
/**
@} */

/**
 * @defgroup CSL_MCBSP_PBBLK_ENUM Multichannel mode PartitionB block
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Multichannel mode PartitionB block
 *
 * Use this symbol to assign Blocks to Partition-B in multichannel mode
 * @{ */
typedef enum {
    CSL_MCBSP_PBBLK_1              =                  0,
    CSL_MCBSP_PBBLK_3              =                  1,
    CSL_MCBSP_PBBLK_5              =                  2,
    CSL_MCBSP_PBBLK_7              =                  3
} CSL_McbspPBBlk;
/**
@} */

/**
 * @defgroup CSL_MCBSP_EMU_ENUM Emulation mode setting
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Emulation mode setting
 *
 * Use this symbol to set the Emulation Mode
 * @{ */
typedef enum {
    CSL_MCBSP_EMU_STOP             =                  0,
    CSL_MCBSP_EMU_TX_STOP          =                  1,
    CSL_MCBSP_EMU_FREERUN          =                  2
} CSL_McbspEmu;
/**
@} */

/**
 * @defgroup CSL_MCBSP_PARTITION_ENUM Multichannel mode Partition select
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Multichannel mode Partition select
 *
 * Use this symbol in multichannel mode to select the Partition
 * for assigning a block to
 * @{ */
typedef enum {
    CSL_MCBSP_PARTITION_ATX        =                  0,
    CSL_MCBSP_PARTITION_ARX        =                  1,
    CSL_MCBSP_PARTITION_BTX        =                  2,
    CSL_MCBSP_PARTITION_BRX        =                  3
} CSL_McbspPartition;
/**
@} */

/**
 * @defgroup CSL_MCBSP_BLOCK_ENUM Multichannel mode Block select
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Multichannel mode Block select
 *
 * Use this symbol in multichannel mode to select block on which the
 * operation is to be performed
 * @{ */
typedef enum {
    CSL_MCBSP_BLOCK_0               =                  0,
    CSL_MCBSP_BLOCK_1               =                  1,
    CSL_MCBSP_BLOCK_2               =                  2,
    CSL_MCBSP_BLOCK_3               =                  3,
    CSL_MCBSP_BLOCK_4               =                  4,
    CSL_MCBSP_BLOCK_5               =                  5,
    CSL_MCBSP_BLOCK_6               =                  6,
    CSL_MCBSP_BLOCK_7               =                  7
} CSL_McbspBlock;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CHAN_ENUM Channel control in multichannel mode
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Channel control in multichannel mode
 *
 * Use this symbol to enable/disable a channel in multichannel mode.
 * This is a member of @a CSL_McbspChanControl structure, which is input to
 * @a CSL_mcbspHwControl() function for @a CSL_MCBSP_CMD_CHANNEL_CONTROL command
 * @{ */
typedef enum {
    CSL_MCBSP_CHCTRL_TX_ENABLE       =                  0,
    CSL_MCBSP_CHCTRL_TX_DISABLE      =                  1,
    CSL_MCBSP_CHCTRL_RX_ENABLE       =                  2,
    CSL_MCBSP_CHCTRL_RX_DISABLE      =                  3
} CSL_McbspChCtrl;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CHANTYPE_ENUM Channel type: TX, RX or both
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Channel type: TX, RX ot both
 *
 * Use this symbol to select the channel type for @a CSL_mcbspHwControl()
 * @a CSL_MCBSP_CMD_CLEAR_FRAME_SYNC command
 * @{ */
typedef enum {
    CSL_MCBSP_CHTYPE_RX                   =                  1,
    CSL_MCBSP_CHTYPE_TX                   =                  2,
    CSL_MCBSP_CHTYPE_TXRX                 =                  4
} CSL_McbspChType;
/**
@} */

/**
 * @defgroup CSL_MCBSP_DLBMODE_ENUM Digital Loopback mode selection
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Digital Loopback mode selection
 *
 * Use this symbol to enable/disable digital loopback mode
 * @{ */
typedef enum {
    CSL_MCBSP_DLBMODE_OFF                  =                  0,
    CSL_MCBSP_DLBMODE_ON                   =                  1
} CSL_McbspDlbMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_PHASE_ENUM Phase count selection
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Phase count selection
 *
 * Use this symbol to select number of phases per frame
 * @{ */
typedef enum {
    CSL_MCBSP_PHASE_SINGLE                =                  0,
    CSL_MCBSP_PHASE_DUAL                  =                  1
} CSL_McbspPhase;
/**
@} */

/**
 * @defgroup CSL_MCBSP_FRMSYNC_ENUM Frame sync ignore status
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Frame sync ignore status
 *
 * Use this symbol to detect or ignore frame synchronisation
 * @{ */
typedef enum {
    CSL_MCBSP_FRMSYNC_DETECT                  =                  0,
    CSL_MCBSP_FRMSYNC_IGNORE                  =                  1
} CSL_McbspFrmSync;
/**
@} */

/**
 * @defgroup CSL_MCBSP_RJUSTDXENA_ENUM RJUST or DXENA settings
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief RJUST or DXENA settings
 *
 * Use this symbol for setting up RCV sign-extension and justification mode
 * or enabling/disabling XMT DX pin delay
 * @{ */
typedef enum {
    /** RCV setting - right justify, fill MSBs with zeros */
    CSL_MCBSP_RJUSTDXENA_RJUST_RZF                 =                  0,
    /** XMT setting - Delay at DX pin disabled */
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF                 =                  0,
    /** RCV setting - right justify, sign-extend the data into MSBs */
    CSL_MCBSP_RJUSTDXENA_RJUST_RSE                 =                  1,
    /** XMT setting - Delay at DX pin enabled */
    CSL_MCBSP_RJUSTDXENA_DXENA_ON                  =                  1,
    /** RCV setting - left justify, fill LSBs with zeros */
    CSL_MCBSP_RJUSTDXENA_RJUST_LZF                 =                  2
} CSL_McbspRjustDxena;
/**
@} */

/**
 * @defgroup CSL_MCBSP_CLKGSYNCMODE_ENUM CLKG sync mode selection
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief CLKG sync mode selection
 *
 * Use this symbol to enable/disable CLKG synchronisation when
 * input CLK source for SRGR is external
 * @{ */
typedef enum {
    CSL_MCBSP_CLKGSYNCMODE_OFF                 =                  0,
    CSL_MCBSP_CLKGSYNCMODE_ON                  =                  1
} CSL_McbspClkgSyncMode;
/**
@} */

/**
 * @defgroup CSL_MCBSP_RSTSTAT_ENUM Tx/Rx reset status
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Tx/Rx reset status
 *
 * Use this symbol to compare the output of @a CSL_mcbspGetHwStatus() for
 * @a CSL_MCBSP_QUERY_TX_RST_STAT and @a CSL_MCBSP_QUERY_RX_RST_STAT queries
 *
 * @{ */
typedef enum {
    CSL_MCBSP_RSTSTAT_TX_IN_RESET             =                  0,
    CSL_MCBSP_RSTSTAT_RX_IN_RESET             =                  0,
    CSL_MCBSP_RSTSTAT_TX_OUTOF_RESET          =                  1,
    CSL_MCBSP_RSTSTAT_RX_OUTOF_RESET          =                  1
} CSL_McbspRstStat;
/**
@} */


/**
 * @defgroup CSL_MCBSP_BITREVERS_ENUM 32-bit Reversal 
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief McBSP512 32-bit reversal feature
 *
 *
 * @{ */
typedef enum {
    CSL_MCBSP_32BIT_REVERS_DISABLE  = 0,/**< 32-bit reversal disabled */
    CSL_MCBSP_32BIT_REVERS_ENABLE    = 1 /**< 32-bit reversal enabled. 32-bit 
		  data is received LSB first. Word length should be set for
		  32-bit operation; else operation undefined  */
} CSL_McbspBitReversal;
/**
@} */


/**
 * @defgroup CSL_MCBSP_DXSTATE_ENUM DX Pin State
 * @ingroup CSL_MCBSP_ENUM
 *
 * @brief Transmit pin mode configuration
 *
 *
 * @{ */
typedef enum {
    CSL_MCBSP_DX_HIGHZ   = 0,/**< High Z */ 
    CSL_MCBSP_DX_LOGIC_1 = 1,/**< Driven as 1 */
    CSL_MCBSP_DX_LOGIC_0 = 2/**< Driven as 0 */
} CSL_McbspDxState;
/**
@} */


/**
 * @defgroup CSL_MCBSP_CONTROLCMD_ENUM Control commands
 * @ingroup CSL_MCBSP_CONTROL_API
 * @{ */
/**
 * This is the set of control commands that are passed to
 * @a CSL_mcbspHwControl(), with an optional argument type-casted to @a void*
 *
 * The arguments, if any, to be passed with each command are specified
 * next to that command.
 */
typedef enum {
/**
 * Assigns a block to a particular partition in multichannel mode
 *
 * @param (CSL_McbspBlkAssign *)
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_ASSIGN_BLOCK     =                  0,
/**
 * Enables or disables a channel in multichannel mode
 *
 * @param (CSL_McbspChanControl *)
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_CHANNEL_CONTROL  =                  1,
/**
 * Clears frame sync error for XMT or RCV
 *
 * @param (CSL_McbspChType *)
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_CLEAR_FRAME_SYNC =                  2,
/**
 * Configures selected MCBSP pins for Input/Output
 *
 * @param (CSL_McbspIoControl *)
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_IO_MODE_CONTROL  =                  3,
/**
 * Resets all the registers to their power-on default values
 *
 * @param None
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_REG_RESET        =                  4,
/**
 *  Enable/Disable - Frame Sync, Sample Rate Generator and XMT/RCV Operation
 *
 * @param (CSL_BitMask16 *)
 * @return CSL_SOK
 */
    CSL_MCBSP_CMD_RESET_CONTROL    =                  5,
/**
 *  Enable/Disable - Idle 
 *
 * @param  (CSL_BitMask16 *)
 * @return CSL_SOK
 * @sa     CSL_MCBSP_IDLECTRL_SYMBOL
 */
    CSL_MCBSP_CMD_IDLE_CONTROL      =                 6    
} CSL_McbspControlCmd;
/**
@} */

/**
 * @defgroup CSL_MCBSP_QUERYCMD_ENUM Query commands
 * @ingroup CSL_MCBSP_QUERY_API
 * @{ */
/**
 * This is the set of query commands to get the status of various
 * operations in MCBSP
 *
 * The arguments, if any, to be passed with each command are specified
 * next to that command. */
typedef enum {
/**
 * Queries the current XMT block
 * @param  (CSL_McbspBlock *)
 * @return CSL_SOK
 */
    CSL_MCBSP_QUERY_CUR_TX_BLK =                  6,
/**
 * Queries the current RCV block
 * @param  (CSL_McbspBlock *)
 * @return CSL_SOK
 */
    CSL_MCBSP_QUERY_CUR_RX_BLK =                  7,
/**
 * Queries the status of RRDY, XRDY, RFULL, XEMPTY, RSYNCERR and XSYNCERR
 * events and returns them in supplied CSL_BitMask16 argument
 * 
 *
 * @param  (CSL_BitMask16 *)
 * @return CSL_SOK
 * @see CSL_DEVSTATUS_SYMBOL
 */
    CSL_MCBSP_QUERY_DEV_STATUS =                  8,
/**
 * Queries XMT reset status
 *
 * @param  (CSL_McbspRstStat *)
 * @return CSL_SOK
 */
    CSL_MCBSP_QUERY_TX_RST_STAT =                  9,
/**
 * Queries RCV reset status
 *
 * @param  (CSL_McbspRstStat *)
 * @return CSL_SOK
 */
    CSL_MCBSP_QUERY_RX_RST_STAT =                 10,
/**
 * Queries Peripheral ID 
 *
 * @param  (CSL_McbspPeriId *)
 * @return CSL_SOK
 */
    CSL_MCBSP_QUERY_PID =                         11    
} CSL_McbspHwStatusQuery;
/**
@} */

typedef Uint32   CSL_McbspCount;


/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * Pointer to this structure is used as the third argument in
 * @a CSL_mcbspHwControl() for block assignment in multichannel mode */
typedef struct CSL_McbspBlkAssign {
    /** Partition to choose */
    CSL_McbspPartition      partition;
    /** Block to choose */
    CSL_McbspBlock          block;
} CSL_McbspBlkAssign;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * Pointer to this structure is used as the third argument in
 * @a CSL_mcbspHwControl() for channel control operations (Enable/Disable TX/RX)
 * in multichannel mode */
typedef struct CSL_McbspChanControl {
    /** Channel number to control */
    Uint16              channelNo;
    /** Control operation */
    CSL_McbspChCtrl  operation;
} CSL_McbspChanControl;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * Pointer to this structure is used as the third argument in
 * @a CSL_mcbspGetHwStatus () for getting revison, type and class info
 * of McBSP512  */
typedef struct CSL_McbspPeriId {
   Uint16  type;
   Uint16  devclass;
   Uint16  revision;
} CSL_McbspPeriId;
/**
@} */


/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * Pointer to this structure is used as the third argument in
 * @a CSL_mcbspHwControl() for IO mode control operations (selecting the pins
 * to use as input and output */
typedef struct CSL_McbspIoControl {
    /** Pinmask to select one or more output pins */
    CSL_BitMask16           outputsel;
    /** Pinmask to select one or more input pins */
    CSL_BitMask16           inputsel;
} CSL_McbspIoControl;
/**
@} */


/* Structure(Internal) used during resource allocation */
typedef struct CSL_McbspResAttrs {
    CSL_Xio                 xio;
    CSL_Uid                 uid;
    CSL_McbspRegsOvly       *regs;
    Int16               perNum;
} CSL_McbspResAttrs;

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is a sub-structure in @ CSL_McbspHwSetup. This structure is used for
 * configuring input/output data related parameters */
typedef struct CSL_McbspDataSetup {
    /** Number of phases in a frame */
    CSL_McbspPhase              phaseNum;
    /** Number of bits per word in phase 1 */
    CSL_McbspWordLen     wordLength1;
    /** Number of words per frame in phase 1 */
    Uint16              frmLength1;
    /** Number of bits per word in phase 2 */
    CSL_McbspWordLen     wordLength2;
    /** Number of words per frame in phase 2 */
    Uint16              frmLength2;
    /** Frame Sync ignore */
    CSL_McbspFrmSync              frmSyncIgn;
    /** Companding options */
    CSL_McbspCompand        compand;
    /** Data delay in number of bits */
    CSL_McbspDataDelay      dataDelay;
    /** Controls DX delay for XMT or sign-extension and justification for RCV */
    CSL_McbspRjustDxena              rjust_dxenable;
    /** Interrupt event mask */
    CSL_McbspIntMode       IntEvent;
    /** 32-bit reversal feature */
    CSL_McbspBitReversal             wordReverse;
} CSL_McbspDataSetup;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is a sub-structure in @a CSL_McbspHwSetup. This structure is used for
 * configuring Clock and Frame Sync generation parameters */
typedef struct CSL_McbspClkSetup {
    /** XMT frame sync mode */
    CSL_McbspFsClkMode        frmSyncTxMode;
    /** RCV frame sync mode */
    CSL_McbspFsClkMode        frmSyncRxMode;
    /** XMT clock mode */
    CSL_McbspTxRxClkMode        clkTxMode;
    /** RCV clock mode */
    CSL_McbspTxRxClkMode        clkRxMode;
    /** XMT frame sync polarity */
    CSL_McbspFsPol    frmSyncTxPolarity;
    /** RCV frame sync polarty */
    CSL_McbspFsPol    frmSyncRxPolarity;
    /** XMT clock polarity */
    CSL_McbspClkPol    clkTxPolarity;
    /** RCV clock polarity */
    CSL_McbspClkPol    clkRxPolarity;
    /** SRG frame sync pulse width */
    Uint16              srgFrmPulseWidth;
    /** SRG frame sync period */
    Uint16              srgFrmPeriod;
    /** SRG divide-down ratio */
    Uint16              srgClkDivide;
    /** SRG input clock mode */
    CSL_McbspSrgClk    srgInputClkMode;
    /** SRG clock polarity */
    CSL_McbspClkPol    srgClkPolarity;
    /** SRG XMT frame-synchronisatoin mode */
    CSL_McbspTxFsMode  srgTxFrmSyncMode;
    /** SRG clock synchronisation mode */
    CSL_McbspClkgSyncMode              srgClkSync;
} CSL_McbspClkSetup;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is a sub-structure in @a CSL_McbspHwSetup. This structure is used for
 * configuring the parameters global to MCBSP */
typedef struct CSL_McbspGlobalSetup {
    /** XMT and RCV IO enable bit */
    CSL_McbspIOMode       ioEnableMode;
    /** Digital Loopback mode */
    CSL_McbspDlbMode      dlbMode;
    /** Clock stop mode */
    CSL_McbspClkStp       clkStopMode;
} CSL_McbspGlobalSetup;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is a sub-structure in @a CSL_McbspHwSetup. This structure is used for
 * configuring Multichannel mode parameters */
typedef struct CSL_McbspMulChSetup {
    /** RCV partition */
    CSL_McbspPartMode  rxPartition;
    /** XMT partition */
    CSL_McbspPartMode  txPartition;
    /** RCV multichannel selection mode */
    Uint16             rxMulChSel;
    /** XMT multichannel selection mode */
    Uint16             txMulChSel;
    /** RCV partition A block */
    CSL_McbspPABlk     rxPartABlk;
    /** RCV partition B block */
    CSL_McbspPBBlk     rxPartBBlk;
    /** XMT partition A block */
    CSL_McbspPABlk     txPartABlk;
    /** XMT partition B block */
    CSL_McbspPBBlk     txPartBBlk;
    /** RCV 512 channel mode with PDMA */
    Uint16             rx512channel;
    /** XMT 512 channel mode with PDMA */
    Uint16             tx512channel;
} CSL_McbspMulChSetup;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is the Setup structure for configuring MCBSP using @a CSL_mcbspHwSetup()
 * function */
typedef struct CSL_McbspHwSetup {
    /** Global configuration parameters */
    CSL_McbspGlobalSetup    *global;
    /** RCV data setup related parameters */
    CSL_McbspDataSetup      *rxdataset;
    /** XMT data setup related parameters */
    CSL_McbspDataSetup      *txdataset;
    /** Clock configuration parameters */
    CSL_McbspClkSetup       *clkset;
    /** Multichannel mode configuration parameters */
    CSL_McbspMulChSetup     *mulCh;
    /** Emulation mode parameters */
    CSL_McbspEmu            emumode ;
   /**  Transmit pin mode configuration, this is used to configure the DX pin 
        state while the time slot is disabled */
    CSL_McbspDxState        dxState;
    /** Super frame synchronization configuration in RCV path */
    Uint32                  rxsynccount;
    /** Super frame synchronization configuration in XMT path */
    Uint32                  txsynccount;
    /** Any extra parameters, for future use */
    void                    *extendSetup;
} CSL_McbspHwSetup;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * @brief This structure/object holds the context of the instance of MCBSP
 * opened using @a CSL_mcbspOpen() function.
 *
 * Pointer to this object is passed as MCBSP Handle to all MCBSP CSL APIs.
 * @a CSL_mcbspOpen() function intializes this structure based on the parameters
 * passed */
typedef struct CSL_McbspObj {
    CSL_OpenMode            openMode;
    CSL_Uid                 uid;
    CSL_Xio                 xio;
    CSL_McbspRegsOvly    regs;
    CSL_McbspNum            perNum;
} CSL_McbspObj;
/**
@} */

/** @addtogroup CSL_MCBSP_DATASTRUCT
*
* @{ */
/**
 * This is a pointer to @a CSL_McbspObj and is passed as the first
 * parameter to all MCBSP CSL APIs */
typedef struct CSL_McbspObj *CSL_McbspHandle;
/**
@} */

/* ************************************************************************\
 * MCBSP global function declarations
\* ************************************************************************/

/**
  * @defgroup CSL_MCBSP_INIT_API MCBSP init API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Initializes MCBSP CSL
  @{ */
CSL_Status  CSL_mcbspInit(
    void
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_OPEN_API MCBSP open API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Opens an instance of MCBSP
  * @{ */
CSL_McbspHandle  CSL_mcbspOpen(
    CSL_McbspObj                            *hMcbspObj,
    CSL_McbspNum                            mcbspNum,
    CSL_OpenMode                            openMode,
    CSL_McbspHwSetup                        *hwSetup,
    CSL_Status                              *status
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_CLOSE_API MCBSP close API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Closes the MCBSP instance
  * @{ */
CSL_Status  CSL_mcbspClose(
    CSL_McbspHandle                         hMcbsp
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_SETUP_API MCBSP h/w setup API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Sets up various configuration parameters
  * @{ */
CSL_Status  CSL_mcbspHwSetup(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspHwSetup                        *setup
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_READ_API MCBSP read API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Read data from MCBSP
  * @{ */
CSL_Status  CSL_mcbspRead(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspWordLen                        wordLen,
    void                                    *data
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_WRITE_API MCBSP write API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Write data from MCBSP
  * @{ */
CSL_Status  CSL_mcbspWrite(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspWordLen                        wordLen,
    void                                    *data
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_IOWRITE_API MCBSP IO pin write API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Send bit data to pins (I/O mode pins)
  * @{ */
void  CSL_mcbspIoWrite(
    CSL_McbspHandle                         hMcbsp,
    CSL_BitMask16                           outputSel,
    Uint16                              outputData
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_IOREAD_API MCBSP IO pin read API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Receive bit data from pins (I/O mode pins)
  * @{ */
Uint16  CSL_mcbspIoRead(
    CSL_McbspHandle                         hMcbsp,
    CSL_BitMask16                           inputSel
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_CONTROL_API MCBSP h/w control API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Controls MCBSP operation based on the control command
  * @{ */
CSL_Status  CSL_mcbspHwControl(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspControlCmd                     cmd,
    void                                    *arg
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_QUERY_API MCBSP h/w status query  API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Get the status of different operations
  * @{ */
CSL_Status  CSL_mcbspGetHwStatus(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspHwStatusQuery                  myQuery,
    void                                    *response
);
/**
@} */

/**
  * @defgroup CSL_MCBSP_GETHWSETUP_API MCBSP h/w setup query API
  * @ingroup CSL_MCBSP_FUNCTION
  * @brief Retrieve or read back the required setup parameters
  * @{ */
CSL_Status  CSL_mcbspGetHwSetup(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspHwSetup                       *myHwSetup
);
/**
@} */

#ifdef __cplusplus
}
#endif

#endif

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:03 9         2272             xkeshavm      */
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
