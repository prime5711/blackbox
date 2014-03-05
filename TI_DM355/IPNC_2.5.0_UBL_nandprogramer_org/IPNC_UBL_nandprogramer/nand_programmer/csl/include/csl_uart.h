/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage UART CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the UART module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given UART module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular UART Module. The CSL developer should use his discretion designing
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

/** \file  csl_uart.h
    \brief UART functional layer API header file
    \internal
    \date 30th April, 2004
    \author Pratheesh Gangadhar (pratheesh@ti.com)
*/
#ifndef _CSL_UART_H_
#define _CSL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_uart.h>

/**
\defgroup CSL_UART_API UART
   \internal Based on <b>Catalog DSP UART Peripheral Module Design 
     Requirement/Specification Rev 0.7 </b> 
*/
/**
\defgroup CSL_UART_DATASTRUCT Data Structures
\ingroup CSL_UART_API
*/
/**
\defgroup CSL_UART_DEFINE  Defines
\ingroup CSL_UART_API
*/
/**
\defgroup CSL_UART_ENUM  Enumerated Data Types
\ingroup CSL_UART_API
*/

/**
\defgroup CSL_UART_FUNCTION  Functions
\ingroup CSL_UART_API
*/


/**************************************************************************\
* UART global macro declarations
\**************************************************************************/ 
  
/** \defgroup CSL_UART_PARITY_DEFINE Parity Controls
*   \ingroup CSL_UART_DEFINE
*   
*
* @{ */
/** Disable parity generation and detection */
#define CSL_UART_PARITY_DISABLE      (CSL_FMK(UART_LCR_PEN, 0) >> 3)
/** Enable parity generation and detection */
#define CSL_UART_PARITY_ENABLE       (CSL_FMK(UART_LCR_PEN, 1) >> 3)  
/** Enable odd parity generation and detection, if PARITY is enabled */
#define CSL_UART_PARITY_ODD          (CSL_FMK(UART_LCR_EPS, 0) >> 3)      
/** Enable even parity generation and detection, if PARITY is enabled */
#define CSL_UART_PARITY_EVEN         (CSL_FMK(UART_LCR_EPS, 1) >> 3)
/** Disable stick parity */
#define CSL_UART_STICKPARITY_DISABLE (CSL_FMK(UART_LCR_SP, 0) >> 3)
/** When stick parity is enabled and even parity is selected, the parity bit
    is transmitted and checked as cleared 
\n When stick parity is enabled and odd parity is selected, the parity bit is
    transmitted and checked as set */
#define CSL_UART_STICKPARITY_ENABLE  (CSL_FMK(UART_LCR_SP, 1) >> 3)
/** 
@} */

/** \defgroup CSL_UART_INTRSTAT_DEFINE Interrupt Enable Masks
*   \ingroup CSL_UART_DEFINE
*   
*
* @{ */
/** Enable modem status interrupt */
#define CSL_UART_INTR_MODEMSTAT       CSL_FMK(UART_IER_EDSSI, 1) 
/** Enable receiver line status interrupt */
#define CSL_UART_INTR_RXLINESTAT      CSL_FMK(UART_IER_ELSI, 1)  
/** Enable THR empty interrupt */
#define CSL_UART_INTR_THR_EMPTY       CSL_FMK(UART_IER_ETBEI, 1) 
/** Enable receive data available and character timeout indication interrupt*/
#define CSL_UART_INTR_RDA_CTI         CSL_FMK(UART_IER_ERBI, 1)        
/** 
@} */



/** \defgroup CSL_UART_LINESTAT_DEFINE Line Status Masks
*   \ingroup CSL_UART_DEFINE
*   
*
* @{ */
/** This bit is set whenever a complete incoming character has been received
    and transferred into the RBR or the FIFO */
#define CSL_UART_LINESTAT_DATAREADY      CSL_FMK(UART_LSR_DR, 1)
/** This bit is set whenever a character is written to the RBR before 
    the previous character was read by the CPU, in so doing the
    previous character is lost */
#define CSL_UART_LINESTAT_OVERRUN_ERR    CSL_FMK(UART_LSR_OE, 1)
/** This bit is set whenever the parity of the receive data character
    does not match the parity selected in the LCR[4]*/
#define CSL_UART_LINESTAT_PARITY_ERR     CSL_FMK(UART_LSR_PE, 1)
/** This bit is set whenever the receive data character does not have 
    a valid stop bit*/
#define CSL_UART_LINESTAT_FRAME_ERR      CSL_FMK(UART_LSR_FE, 1)
/** This bit is set whenever the receive data input was held low for 
    longer than a full-word transmission time*/
#define CSL_UART_LINESTAT_BREAKINDI      CSL_FMK(UART_LSR_BI, 1)
/** This bit is set whenever the THR is empty, indicating that the UART
    is ready to accept a new character*/
#define CSL_UART_LINESTAT_THR_EMPTY      CSL_FMK(UART_LSR_THRE, 1)
/**  This bit is set when both the THR and the TSR are empty */
#define CSL_UART_LINESTAT_TXEMPTY        CSL_FMK(UART_LSR_TEMT, 1)
/** In the non-FIFO mode, this bit is set when there is a parity error, 
    framing error, or break indicator in the receiver buffer register (RBR)*/ 
#define CSL_UART_LINESTAT_RXFIFO_ERR     CSL_FMK(UART_LSR_RXFIFOE, 1)
/** 
@} */



/** \defgroup CSL_UART_MODEMSTAT_DEFINE Modem Status Masks
*   \ingroup CSL_UART_DEFINE
*   
*
* @{ */
/** Indicates that the CTS input has changed state 
    since the last time it was read by the CPU */
#define CSL_UART_MODEMSTAT_CTS    CSL_FMK(UART_MSR_DCTS, 1)
/** Indicates that the DSR input has changed state 
    since the last time it was read by the CPU */
#define CSL_UART_MODEMSTAT_DSR    CSL_FMK(UART_MSR_DDSR, 1)
/** Indicates that the RI input has changed from a low to a high */
#define CSL_UART_MODEMSTAT_RI     CSL_FMK(UART_MSR_TERI, 1)
/** Indicates that the DCD input has changed state 
    since the last time it was read by the CPU */
#define CSL_UART_MODEMSTAT_DCD    CSL_FMK(UART_MSR_DDCD, 1)
/*TODO Need to change abbreviation in register layer*/
/** The complement of the CTS input*/
#define CSL_UART_MODEMSTAT_NCTS   CSL_FMK(UART_MSR_CTS, 1)
/** The complement of the DSR input*/
#define CSL_UART_MODEMSTAT_NDSR   CSL_FMK(UART_MSR_DSR, 1)
/** The complement of the RI input*/
#define CSL_UART_LINESTAT_NRI     CSL_FMK(UART_MSR_RI, 1)
/** The complement of the DCD input*/
#define CSL_UART_LINESTAT_NDCD    CSL_FMK(UART_MSR_DCD, 1)
/** 
@} */

/**\defgroup CSL_UART_BAUDRATE_DEFINE Baud Rate
*  \ingroup CSL_UART_DEFINE
*
*  
@{*/
/** 2.4Kb/s*/
#define CSL_UART_BAUD_2_4K        2400                	                       
/** 4.8 Kb/s */
#define CSL_UART_BAUD_4_8K        4800  	                    
/** 9.6 Kb/s   */
#define CSL_UART_BAUD_9_6K        9600  	                   
/**14.4 Kb/s */
#define CSL_UART_BAUD_14_4K      14400  	                  
/**19.2 Kb/s */
#define CSL_UART_BAUD_19_2K      19200  	  	               
/**28.8 Kb/s */
#define CSL_UART_BAUD_28_8K      28800         	  		     
/** 38.4 Kb/s*/
#define CSL_UART_BAUD_38_4K      38400  	  	               
/**57.6 Kb/s */
#define CSL_UART_BAUD_57_6K      57600  	  	               
/**115.2 Kb/s */
#define CSL_UART_BAUD_115_2K    115200    		 	      
/**
@} */

/**************************************************************************\
* UART global typedef declarations
\**************************************************************************/



/**\defgroup CSL_UART_NUMSTOPBIT_ENUM Number of Stop Bits
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for the Number of Stop bits
*
*   This is used by control commands to set the number of stop bits
*/
typedef enum {
  CSL_UART_NUMSTOP_1      = 0,  /**< <b>:  Number of Stop bits = 1 </b>*/
  CSL_UART_NUMSTOP_1_5or2 = 1 /**< <b>: Number of Stop bits = 1.5
 		               (when character length equal to 5) or 2 
                         (when character length equal to 6,7 or 8) </b>*/
} CSL_UartNumStop;
/**
@} */

/**\defgroup CSL_UART_DMACTRL_ENUM DMA Control
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for DMA configuration
*
*   This is used by control commands to configure DMA mode
*/
typedef enum {
  CSL_UART_DMA_DISABLE = CSL_UART_FCR_DMAMODE1_DISABLE,
                      /**< <b>:  Disable DMA </b>*/
  CSL_UART_DMA_ENABLE = CSL_UART_FCR_DMAMODE1_ENABLE
                      /**< <b>: Enable DMA MODE1</b>*/
} CSL_UartDmaCtrl;
/**
@} */


/**\defgroup CSL_UART_EMUMODE_ENUM Emulation Control
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for Emulation modes
*
*   This is used by control commands to configure emulation mode
*/
typedef enum {
  CSL_UART_EMU_STOP = CSL_UART_PWREMU_MGMT_FREE_STOP,
                      /**< <b>: UART stops immediately</b>*/
  CSL_UART_EMU_RUN  = CSL_UART_PWREMU_MGMT_FREE_RUN
                      /**< <b>: UART runs free </b>*/
} CSL_UartEmuCtrl;
/**
@} */


/**\defgroup CSL_UART_RSTCTRL_ENUM Reset Control
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for Reset control
*
*   This is used by control commands to reset\enable UART
*/
typedef enum {
  CSL_UART_RESET = CSL_UART_PWREMU_MGMT_URST_RESET,
                      /**< <b>: Resets UART</b>*/
  CSL_UART_ENABLE = CSL_UART_PWREMU_MGMT_URST_ENABLE
                      /**< <b>: Re-enables transmitter and receiver</b>*/
} CSL_UartResetCtrl;
/**
@} */


/**\defgroup CSL_UART_LOOPCTRL_ENUM Loop Control
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for Loopback mode control
*
*   This is used by control commands to enable\disable loopback mode
*/
typedef enum {
  CSL_UART_LOOP_DISABLE = CSL_UART_MCR_LOOP_DISABLE,
                      /**< <b>: Normal mode of operation</b>*/
  CSL_UART_LOOP_ENABLE  = CSL_UART_MCR_LOOP_ENABLE
                      /**< <b>: Enables loopback mode</b>*/
} CSL_UartLoopCtrl;
/**
@} */

/**\defgroup CSL_UART_FIFOCTRL_ENUM Fifo Control
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for Fifo configuration
*
*   This is used by control commands to enable\disable FIFO
*/
typedef enum {
  CSL_UART_FIFO_DISABLE = CSL_UART_FCR_FIFOEN_DISABLE,
                      /**< <b>:Disables FIFO mode</b>*/
  CSL_UART_FIFO_ENABLE  = CSL_UART_FCR_FIFOEN_ENABLE
                      /**< <b>: Enables FIFO mode</b>*/
} CSL_UartFifoCtrl;
/**
@} */




/**\defgroup CSL_UART_CHARLEN_ENUM Character Length
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for the UART character length
*
* This is used by control commands to set the UART character length
*/
typedef enum {
  CSL_UART_CHARLEN_5    =  CSL_UART_LCR_WLS_BITS5,/**<<b>: 5 bits </b>*/
  CSL_UART_CHARLEN_6    =  CSL_UART_LCR_WLS_BITS6,/**<<b>: 6 bits </b>*/
  CSL_UART_CHARLEN_7    =  CSL_UART_LCR_WLS_BITS7,/**<<b>: 7 bits </b>*/
  CSL_UART_CHARLEN_8    =  CSL_UART_LCR_WLS_BITS8/**<<b>: 8 bits </b> */       
} CSL_UartCharLen;
/**
@} */


/**\defgroup CSL_UART_RXFIFTL_ENUM Receive FIFO Trigger Level
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for the UART receive FIFO trigger level 
*
* This is used by control commands to set rx FIFO trigger level 
*/
typedef enum {
  CSL_UART_RXTRIGLEVEL_1    =  CSL_UART_FCR_RXFIFTL_CHAR1,
                               /**< <b>: Character length of 1 </b> */	
  CSL_UART_RXTRIGLEVEL_4    =  CSL_UART_FCR_RXFIFTL_CHAR4,  
                               /**<<b>: Character length of 4 </b> */  
  CSL_UART_RXTRIGLEVEL_8    =  CSL_UART_FCR_RXFIFTL_CHAR8,
                               /**<<b>: Character length of 8 </b>*/
  CSL_UART_RXTRIGLEVEL_14    =  CSL_UART_FCR_RXFIFTL_CHAR14  
	                       /**<<b>: Character length of 14 </b> */       
} CSL_UartRxFifoTrigLevel;
/**
@} */


/**\defgroup CSL_UART_AUTOFLOW_ENUM Autoflow Controls
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for the UART autoflow configuration
*
* This is used for configuring autoflow settings
*/
typedef enum {
  CSL_UART_AUTOFLOW_DISABLE,/**<<b>: Auto-rts and auto-cts disabled  </b>*/
  CSL_UART_AUTO_CTS,/**<<b>: Auto-cts only enabled  </b>*/             
  CSL_UART_AUTO_CTS_RTS/**<<b>: Auto-rts and auto-cts enabled  </b>*/             
} CSL_UartAutoflowCtrl;
/**
@} */

/**\defgroup CSL_UART_INTRSOURCE_ENUM Interrupt Source
*  \ingroup CSL_UART_ENUM
*
*  
@{*/
/** \brief  Enums for the Interrupt Source
*
* This is used for query Interrupt Source.
*/
typedef enum {
  CSL_UART_INTRSOURCE_NONE        =  CSL_UART_IIR_IPEND_NONE, 
                                     /**<<b>: No Interrupt Occured</b>*/  
  CSL_UART_INTRSOURCE_MODEMSTAT   =  CSL_UART_IIR_INTID_MODSTAT << 1,
                                     /**<<b>: Modem status interrupt</b>*/   
  CSL_UART_INTRSOURCE_THRE        =  CSL_UART_IIR_INTID_THRE << 1,
                                     /**<<b>: THR empty interrupt</b>*/  
  CSL_UART_INTRSOURCE_RDA         =  CSL_UART_IIR_INTID_RDA << 1,
                                    /**<<b>: Rx  data available interrupt</b>*/   
  CSL_UART_INTRSOURCE_RXLINESTAT  =  CSL_UART_IIR_INTID_RLS << 1 ,
                                     /**<<b>:  Rx line status interrupt</b>*/    
  CSL_UART_INTRSOURCE_CHARTIMEOUT =  CSL_UART_IIR_INTID_CTI << 1    
                                     /**<<b>: Character timeout indication
				             interrupt</b>*/  
} CSL_UartIntrSource;
/**
@} */



/**\defgroup CSL_UART_CONTROLCMD_ENUM  Control Commands 
*  \ingroup CSL_UART_CONTROL_API
@{*/
/** \brief Enumeration for control commands passed to \a CSL_uartHwControl()
*
* This is the set of commands that are passed to the \a CSL_uartHwControl()
* with an optional argument type-casted to \a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration
*/
typedef enum {
  CSL_UART_CMD_WRITEBYTE,/**< \brief Write a byte to THR 
                              \param (Uint8*) 
                              \return CSL_SOK */
  CSL_UART_CMD_READBYTE,/**< \brief Read a byte from RBR if data is ready 
                              \param (Uint8*) 
                              \return CSL_SOK */
  CSL_UART_CMD_WRITE,/**< \brief  Write specified number of characters into
			          TxFIFO
                         \n The "transfernum" field acts as output parameter
			 reporting the actual number of bytes written
                          \param (CSL_UartBufferDescriptor*)    
                          \return CSL_SOK
                          \sa CSL_UartBufferDescriptor */
  CSL_UART_CMD_READ,/**< \brief  Read specified number of characters from
			         RxFIFO
                         \n The "transfernum"  and "error" fields act 
                         as output parameters reporting the actual number of
                         bytes read and error respectively
                         \param (CSL_UartBufferDescriptor*)    
                         \return CSL_SOK
                         \sa CSL_UartBufferDescriptor */ 
  CSL_UART_CMD_INTR_DISABLE, /**< \brief Disables interrupts specified by input
                                         bitmask
                                \param (CSL_BitMask32*) 
                                \return CSL_SOK
				\sa CSL_UART_INTRSTAT_DEFINE  */   
  CSL_UART_CMD_INTR_ENABLE,/**< \brief Enables interrupts specified by input
                                       bitmask
                                \param (CSL_BitMask32*) 
                                \return CSL_SOK
				\sa CSL_UART_INTRSTAT_DEFINE  */ 
  CSL_UART_CMD_RTS_HIGH,/**< \brief Forces RTS output high
                             \param NONE 
                             \return CSL_SOK */
  CSL_UART_CMD_RTS_LOW,/**< \brief Forces RTS output low
                            \param NONE 
                            \return CSL_SOK */
  CSL_UART_CMD_DTR_HIGH,/**< \brief Forces DTR output high
                             \param NONE 
                             \return CSL_SOK */
  CSL_UART_CMD_DTR_LOW,/**< \brief Forces DTR output low
                            \param NONE 
                            \return CSL_SOK */
  CSL_UART_CMD_OUT1_HIGH,/**< \brief Forces OUT1 output high
                             \param NONE 
                             \return CSL_SOK */
  CSL_UART_CMD_OUT1_LOW,/**< \brief Forces OUT1 output low
                            \param NONE 
                            \return CSL_SOK */
  CSL_UART_CMD_OUT2_HIGH,/**< \brief Forces OUT2 output high
                             \param NONE 
                             \return CSL_SOK */
  CSL_UART_CMD_OUT2_LOW,/**< \brief Forces OUT2 output low
                            \param NONE 
                            \return CSL_SOK */
  CSL_UART_CMD_FIFO_DISABLE,/**< \brief Disables FIFO mode
                                 \param NONE 
                                 \return CSL_SOK */ 
  CSL_UART_CMD_FIFO_ENABLE,/**< \brief Enables FIFO mode
                                \param NONE 
                                \return CSL_SOK */ 
  CSL_UART_CMD_CONFIG_FIFO,/**< \brief Configures FIFO, set triggerlevel and
			               DMA mode 
                                \param (CSL_UartFifoConfig*) 
                                \return CSL_SOK 
                                 \sa CSL_UartFifoConfig */ 
  CSL_UART_CMD_RESET_TXFIFO,/**< \brief  Reset TxFIFO counters
                                 \param  NONE
                                 \return CSL_SOK */ 
  CSL_UART_CMD_RESET_RXFIFO,/**< \brief  Reset RxFIFO counters
                                 \param  NONE
                                 \return CSL_SOK */ 
  CSL_UART_CMD_BREAK_DISABLE,/**< \brief Disables break condition
                                 \param  NONE
                                 \return CSL_SOK */
  CSL_UART_CMD_BREAK_ENABLE,/**< \brief  Enables break condition
                                 \param  NONE
                                 \return CSL_SOK */
  CSL_UART_CMD_CONFIG_BAUDRATE,/**< \brief  Configures baudrate
                                    \param  (CSL_UartBaudConfig*)
                                    \return CSL_SOK
				    \sa CSL_UartBaudConfig */
  CSL_UART_CMD_CONFIG_LINE,/**< \brief  Controls the format of the
			         asynchronous data communication exchange 
                                 \param  (CSL_UartLineConfig*)
                                 \return CSL_SOK
				 \sa CSL_UartLineConfig */
  CSL_UART_CMD_CONFIG_AUTOFLOW,/**< \brief  Configures autoflow parameters 
                                 \param  (CSL_UartAutoflowCtrl*)
                                 \return CSL_SOK 
                                 \sa CSL_UartAutoflowCtrl */
  CSL_UART_CMD_LOOPBACK_DISABLE,/**< \brief  Disables loopback mode
                                     \param  NONE
                                     \return CSL_SOK */
  CSL_UART_CMD_LOOPBACK_ENABLE,/**< \brief  Enables loopback mode
                                   \param  NONE
                                   \return CSL_SOK */
  CSL_UART_CMD_STOP, /**< \brief the UART stop immediately if a transmission
                  isn't in progress. If a transmission is in progress, the UART
                  stops after completion of the one word transmission
                          \param  NONE
                          \return CSL_SOK */
  CSL_UART_CMD_RUN_FREE, /**< \brief  UART runs free
                             \param  NONE
                             \return CSL_SOK */
  CSL_UART_CMD_ENABLE,/**< \brief  Re-enables transmitter and receiver
                           \param  NONE
                           \return CSL_SOK */
  CSL_UART_CMD_RESET/**< \brief  Resets UART module
                         \param  NONE
                         \return CSL_SOK */
} CSL_UartHwControlCmd;
/**
@} */

/**\defgroup CSL_UART_QUERYCMD_ENUM Query Commands 
*  \ingroup CSL_UART_QUERY_API
* @{ */

/** \brief Enumeration for queries passed to \a CSL_UartGetHwStatus()
*
* This is used to get the status of different operations.The arguments
* to be passed with each enumeration if any are specified next to 
* the enumeration */
typedef enum {
  CSL_UART_QUERY_LINESTATUS,/**< \brief Queries Receiver Line Status 
                                  \param (CSL_BitMask32*) 
                                  \return CSL_SOK
				  \sa CSL_UART_LINESTAT_DEFINE  */ 
  CSL_UART_QUERY_MODEMSTATUS,/**< \brief Queries Modem Status 
                                  \param (CSL_BitMask32*) 
                                  \return CSL_SOK
				  \sa CSL_UART_MODEMSTAT_DEFINE  */	  
  CSL_UART_QUERY_INTRSTATUS,/**< \brief Interrupt Enable Status 
                                  \param (CSL_BitMask32*) 
                                  \return CSL_SOK
				  \sa CSL_UART_INTRSTAT_DEFINE  */
  CSL_UART_QUERY_VERSION,/**< \brief Queries Version of UART 
                              \param (Uint8*) 
                              \return CSL_SOK */
  CSL_UART_QUERY_INTRSOURCE/**< \brief Queries Interrupt Source 
                                \param (CSL_UartIntrSource*) 
                                \return CSL_SOK
                                \sa CSL_UartIntrSource */
} CSL_UartHwStatusQuery;
/**
@} */



/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/
/** \brief UART buffer descriptor
* 
*    Used when sending/receiving data
*   \sa CSL_UART_LINESTAT_DEFINE
*/

typedef struct CSL_UartBufferDescriptor {
  Uint8* buff;/**< Pointer to data transfer/receive buffer */               
  Uint32 num;/**< Number of bytes(5/6/7/8 chars) to be transferred */				  
  Uint32 transfernum;/**< Number of bytes(5/6/7/8 chars) actually 
                         transferred */            
  CSL_BitMask32 error ;/**< ORed combination of errors i.e parity 
                             and/or break and/or frame Error */			  
} CSL_UartBufferDescriptor;
/**
@} */

/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/
/** \brief UART baudrate confiuration
* 
* Used to configure UART baudarate clock divisor
* \sa CSL_UART_BAUDRATE_DEFINE 
*/
typedef struct CSL_UartBaudConfig {
  Uint32  moduleClock;/**< Module Clock Frequency(Hz) */
  Uint32  baudrate;/**< Standard baudrate setting */
} CSL_UartBaudConfig;
/**
@} */

/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/
/** \brief UART line confiuration
* 
* Used to configure character length, stopbits and parity
* \sa CSL_UART_PARITY_DEFINE
*/
typedef struct CSL_UartLineConfig {
  CSL_UartCharLen  charLen;/**< Serial character word length*/
  CSL_UartNumStop  numStopBits;/**< Stop bits generated */
  CSL_BitMask32    parityCtrl;/**< Parity control bitfield*/ 
} CSL_UartLineConfig;
/**
@} */

/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/
/** \brief UART FIFO confiuration
* 
* Used to configure RX FIFO threshold level and DMA
* 
*/

typedef struct CSL_UartFifoConfig {
  CSL_UartRxFifoTrigLevel  trigLevel;/**< Rx FIFO trigger level*/
  CSL_UartDmaCtrl          dmaMode;/**< When set selects mode1 of DMA */
} CSL_UartFifoConfig;
/**
@} */


/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up UART   
*  \sa CSL_UART_INTRSTAT_DEFINE
* 
*/
typedef struct CSL_UartHwSetup {
  CSL_UartAutoflowCtrl flowControl;
  CSL_UartFifoCtrl   fifoEnable;
  CSL_UartLoopCtrl   loopEnable;
  CSL_UartEmuCtrl    emuConfig;
  CSL_BitMask32      interruptEnable;
  CSL_UartBaudConfig setupBaud;
  CSL_UartLineConfig setupLine;
  CSL_UartFifoConfig setupFifo;
  void*       extendSetup;
} CSL_UartHwSetup;
/**
@} */



/** 
\addtogroup CSL_UART_DATASTRUCT 
@{
*/
/** \brief This object contains the reference to the instance of UART
* opened using the @a CSL_uartOpen()
*
* The pointer to this is passed to all UART CSL APIs
* This structure has the fields required to configure UART for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/
typedef struct CSL_UartObj {
  CSL_OpenMode openMode;/**< This is the mode in which the CSL instance
                             is opened */
  CSL_Uid      uid;/**< This is a unique identifier to the instance of UART 
                        being referred to by this object */
  CSL_Xio      xio;/**< This is the variable that holds the current state
                        of a resource being shared by current instance of
                        UART with other peripherals */
  CSL_UartRegsOvly regs;/**< This is a pointer to the registers of the  
                             instance of UART referred to by this object */
  CSL_UartNum  uartNum;/**< This is the instance of UART being referred to
                           by this object */
} CSL_UartObj;


/** \brief this is a pointer to @a CSL_UartObj and is passed as the first
* parameter to all UART CSL APIs */
typedef CSL_UartObj* CSL_UartHandle; 

/**
@} */


/*****************************************************************************\
  CSL3.x mandatory function declarations
\*****************************************************************************/

/**
\defgroup CSL_UART_INIT_API UART Init API
\ingroup CSL_UART_FUNCTION
    \brief Initializes the UART
@{*/
CSL_Status CSL_uartInit ( void );
/**
@} */

/**
\defgroup CSL_UART_OPEN_API UART Open API
\ingroup CSL_UART_FUNCTION
    \brief Opens if possible the instance of UART requested
    \n  Reserves the specified uart for use. The device can be 
    re-opened anytime after it has been normally closed if so
    required. The handle returned by this call is input as an
    essential argument for rest of the APIs described for this
    module.

    <b> Usage Constraints: </b>
    The CSL system as well as UART must be succesfully initialized
    via @a CSL_sysInit() and @a CSL_uartInit() before calling this
    function. Memory for the @a CSL_UartObj must be allocated outside
    this call. This object must be retained while usage of this peripheral.
  
    \b Example:
    \verbatim


    CSL_UartObj     uartObj;
    CSL_UartHwSetup uartSetup;
    CSL_Status       status;
    ...
    hUart = CSL_uartOpen(&uartobj,
                         CSL_UART_0,
                         CSL_EXCLUSIVE,
                         &uartSetup,
                         &status);
    \endverbatim
  
@{*/
CSL_UartHandle CSL_uartOpen ( CSL_UartObj *hUartObj,
                              CSL_UartNum uartNum,
                              CSL_OpenMode openMode,
                              CSL_UartHwSetup *hwSetup,
                              CSL_Status *status);
   
/**
@} */


/**
\defgroup CSL_UART_CLOSE_API UART Close API
\ingroup CSL_UART_FUNCTION
    \brief Closes the instance of UART requested

    
@{*/
CSL_Status  CSL_uartClose ( CSL_UartHandle hUart );
/**
@} */



/**
\defgroup CSL_UART_SETUP_API UART Setup API
\ingroup CSL_UART_FUNCTION
    \brief Programs the UART with the setup configuration as specified in
           the arguments
   		  
	   
@{*/
CSL_Status CSL_uartHwSetup ( CSL_UartHandle hUart,
                             CSL_UartHwSetup *setup );
/**
@} */

/**
\defgroup CSL_UART_GETSETUP_API UART GetSetup API
\ingroup CSL_UART_FUNCTION
    \brief Reads the UART setup configuration
   		  
	   
@{*/
CSL_Status CSL_uartGetHwSetup ( CSL_UartHandle hUart,
                             CSL_UartHwSetup *setup );
/**
@} */

/**
\defgroup CSL_UART_CONTROL_API UART Control API
\ingroup CSL_UART_FUNCTION
    \brief Controls the different operations that can be performed by UART 
 		  
     		  
    
@{*/
CSL_Status CSL_uartHwControl ( CSL_UartHandle hUart,
                               CSL_UartHwControlCmd cmd,
                               void *arg );
/**
@} */

/**
\defgroup CSL_UART_QUERY_API UART Query API
\ingroup CSL_UART_FUNCTION
    \brief Returns the status of the requested operation on UART
@{*/

CSL_Status CSL_uartGetHwStatus ( CSL_UartHandle hUart,
                                 CSL_UartHwStatusQuery query,
                                 void *response );

/**
@}*/

#ifdef __cplusplus
}
#endif

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:10 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:08    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
