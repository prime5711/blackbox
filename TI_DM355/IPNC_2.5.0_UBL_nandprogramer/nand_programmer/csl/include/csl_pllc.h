/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage PLLC CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the PLLC module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given PLLC module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular PLLC Module. The CSL developer should use his discretion designing
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

/** \file  csl_pllc.h
    \brief PLLC functional layer API header file
    \internal
    \date 25 June, 2004
    \author Pratheesh Gangadhar (pratheesh@ti.com)
*/

#ifndef _CSL_PLLC_H_
#define _CSL_PLLC_H_

#ifdef __cplusplus
extern "C" {
#endif
	

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_pllc.h>

/**
\defgroup CSL_PLLC_API PLLC
   \internal Based on <b>PLLC Spec Rev 3.4 </b> 
*/
/**
\defgroup CSL_PLLC_DATASTRUCT Data Structures
\ingroup CSL_PLLC_API
*/
/**
\defgroup CSL_PLLC_DEFINE  Defines
\ingroup CSL_PLLC_API
*/
/**
\defgroup CSL_PLLC_ENUM  Enumerated Data Types
\ingroup CSL_PLLC_API
*/

/**
\defgroup CSL_PLLC_FUNCTION  Functions
\ingroup CSL_PLLC_API
*/

/*****************************************************************************\
          LCDC global macro declarations
\*****************************************************************************/ 

/** \defgroup CSL_PLLC_ERROR_DEFINE Error Code
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_EPLLC_INVPARAM CSL_EPLLC_FIRST /**< Invalid Argument */

/**
@} */


/** \defgroup CSL_PLLC_STATUS_DEFINE PLL Controller Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_STATUS_GO CSL_FMKT (PLLC_PLLSTAT_GOSTAT, INPROG)
                           /**< Set when GO operation (divide-ratio change and
                                clock alignment) is in progress */
#define CSL_PLLC_STATUS_LOCK CSL_FMKT (PLLC_PLLSTAT_LOCK, YES)
                            /**< Set when PLL core is locked */
#define CSL_PLLC_STATUS_STABLE CSL_FMKT (PLLC_PLLSTAT_STABLE, YES)
                            /**< Set when OSCIN/CLKIN is assumed to be 
                                 stable */
/**
@} */

/** \defgroup CSL_PLLC_DCHANGESTAT_DEFINE PLLC Divider Ratio Modified Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_DCHANGESTAT_SYS8 CSL_FMKT (PLLC_DCHANGE_SYS8, YES)
                           /**< SYSCLK8 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS7 CSL_FMKT (PLLC_DCHANGE_SYS7, YES)
                            /**< SYSCLK7 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS6 CSL_FMKT (PLLC_DCHANGE_SYS6, YES)
                            /**< SYSCLK6 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS5 CSL_FMKT (PLLC_DCHANGE_SYS5, YES)
                           /**< SYSCLK5 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS4 CSL_FMKT (PLLC_DCHANGE_SYS4, YES)
                            /**< SYSCLK4 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS3 CSL_FMKT (PLLC_DCHANGE_SYS3, YES)
                            /**< SYSCLK3 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS2 CSL_FMKT (PLLC_DCHANGE_SYS2, YES)
                            /**< SYSCLK2 divide ratio is modified */
#define CSL_PLLC_DCHANGESTAT_SYS1 CSL_FMKT (PLLC_DCHANGE_SYS1, YES)
                            /**< SYSCLK1 divide ratio is modified */                            
/**
@} */


/** \defgroup CSL_PLLC_CLKSTAT_DEFINE PLLC Clock Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_CLKSTAT_AUXON CSL_FMKT (PLLC_CKSTAT_AUXEN, ON)
                           /**< AUXCLK is ON */
#define CSL_PLLC_CLKSTAT_OBSON CSL_FMKT (PLLC_CKSTAT_OBSEN, ON)
                            /**< OBSCLK is ON */
#define CSL_PLLC_CLKSTAT_BPON CSL_FMKT (PLLC_CKSTAT_BPON, ON)
                            /**< SYSCLKBP is ON */
/**
@} */

/** \defgroup CSL_PLLC_CLKEN_DEFINE PLLC Clock Enable Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_CLKEN_AUXEN CSL_FMKT (PLLC_CKEN_AUXEN, ENABLE)
                           /**< AUXCLK enable */
#define CSL_PLLC_CLKEN_OBSEN CSL_FMKT (PLLC_CKEN_OBSEN, ENABLE)
                            /**< OBSCLK enable */
/**
@} */

/** \defgroup CSL_PLLC_SYSCLKSTAT_DEFINE PLLC SYSCLK Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_SYSCLKSTAT_SYS8ON CSL_FMKT (PLLC_CKSTAT_SYS8ON, ON)
                           /**< SYSCLK8 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS7ON CSL_FMKT (PLLC_CKSTAT_SYS7ON, ON)
                           /**< SYSCLK7 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS6ON CSL_FMKT (PLLC_CKSTAT_SYS6ON, ON)
                           /**< SYSCLK6 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS5ON CSL_FMKT (PLLC_CKSTAT_SYS5ON, ON)
                           /**< SYSCLK5 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS4ON CSL_FMKT (PLLC_CKSTAT_SYS4ON, ON)
                           /**< SYSCLK4 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS3ON CSL_FMKT (PLLC_CKSTAT_SYS3ON, ON)
                           /**< SYSCLK3 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS2ON CSL_FMKT (PLLC_CKSTAT_SYS2ON, ON)
                           /**< SYSCLK2 is ON */
#define CSL_PLLC_SYSCLKSTAT_SYS1ON CSL_FMKT (PLLC_CKSTAT_SYS1ON, ON)
                           /**< SYSCLK1 is ON */
                           
/**
@} */


/** \defgroup CSL_PLLC_RESETSTAT_DEFINE PLLC Last Reset Status
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_RESETSTAT_POR CSL_FMKT (PLLC_RSTYPE_POR, YES)
                           /**< Power On Reset */
#define CSL_PLLC_RESETSTAT_XWRST CSL_FMKT (PLLC_RSTYPE_XWRST, YES)
                            /**< External Warm Reset */
#define CSL_PLLC_RESETSTAT_MRST CSL_FMKT (PLLC_RSTYPE_MRST, YES)
                           /**< Maximum Reset */
#define CSL_PLLC_RESETSTAT_SRST CSL_FMKT (PLLC_RSTYPE_SRST, YES)
                            /**< System/Chip Reset */
/**
@} */


/** \defgroup CSL_PLLC_CTRL_DEFINE PLLC Control Mask
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_CTRL_BYPASS CSL_FMKT (PLLC_PLLCTL_PLLEN, BYPASS)
                             /**< PreDiv, PLL, and PostDiv are bypassed. SYSCLK
                                  divided down directly from input reference
                                   clock refclk */
#define CSL_PLLC_CTRL_ENABLE CSL_FMKT (PLLC_PLLCTL_PLLEN, PLL)
                            /**< PLL is used. SYSCLK divided down from PostDiv
                                 output */
#define CSL_PLLC_CTRL_OPERATIONAL CSL_FMKT (PLLC_PLLCTL_PLLPWRDN, NO)
                            /**< Selected PLL Operational */
#define CSL_PLLC_CTRL_POWERDOWN CSL_FMKT (PLLC_PLLCTL_PLLPWRDN, YES)
                            /**< Selected PLL Placed In Power Down State */
#define CSL_PLLC_CTRL_RELEASE_RESET CSL_FMKT (PLLC_PLLCTL_PLLRST, NO)
                            /**< PLL Reset Released */
#define CSL_PLLC_CTRL_RESET CSL_FMKT (PLLC_PLLCTL_PLLRST, YES)
                            /**< PLL Reset Asserted */
#define CSL_PLLC_CTRL_RELEASE_DIS CSL_FMKT (PLLC_PLLCTL_PLLDIS, NO)
                            /**< PLL Disable Released */
#define CSL_PLLC_CTRL_ASSERT_DIS CSL_FMKT (PLLC_PLLCTL_PLLRST, YES)
                            /**< PLL Disable Asserted */
#define CSL_PLLC_CTRL_MUXCTRL_REGBIT CSL_FMKT (PLLC_PLLCTL_PLLENSRC, REGBIT)
                            /**< PLLEN Mux is controlled by PLLCTL.PLLEN.
                                 pllen_pi is donÆt care */
#define CSL_PLLC_CTRL_MUXCTRL_PORT CSL_FMKT (PLLC_PLLCTL_PLLENSRC, PORT)
                            /**< PLLEN Mux is controlled by input pllen_pi.
                                 PLLCTL.PLLEN is donÆt care */
#define CSL_PLLC_CTRL_SELECT_PLLA CSL_FMKT (PLLC_PLLCTL_PLLSELB, PLLA)
                            /**< PLL A is selected. PLL B is put in power
                                 down */
#define CSL_PLLC_CTRL_SELECT_PLLB CSL_FMKT (PLLC_PLLCTL_PLLSELB, PLLB)
                            /**< PLL B is selected. PLL A is put in power
                                 down */
#define CSL_PLLC_CTRL_SELECT_OSCIN CSL_FMKT (PLLC_PLLCTL_CLKMODE, OSCIN)
                            /**< oscin_pi is the reference clock */
#define CSL_PLLC_CTRL_SELECT_CLKIN CSL_FMKT (PLLC_PLLCTL_CLKMODE, CLKIN)
                            /**< clkin_pi is the reference clock */
#define CSL_PLLC_CTRL_ALIGN_PHASE (CSL_FMKT (PLLC_PLLCMD_GOSET, SET)<< 16)
                           /**< A write of 1 to this bit signifies that the new
                                divide ratios in PLLDIV[1:n] are taken into 
                                account at the nearest possible rising edge to
                                phase align the clocks. The actual SYSCLKx to
                                be aligned are selected in register ALNCTL */
#define CSL_PLLC_CTRL_OSCPWRDN (CSL_FMKT (PLLC_PLLCMD_OSCPWRDN, SET)<<16)
                            /**< A write of 1 (doesnÆt need to be a transition
                                from 0 to 1) to this bit initiates oscillator 
                                power down command */

/**
@} */

/** \defgroup CSL_PLLC_ALIGNCTL_DEFINE PLLC Align Control
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_ALIGNCTL_SYSCLK8 CSL_FMKT (PLLC_ALNCTL_ALN8, YES)
                           /**< SYSCLK8 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK7 CSL_FMKT (PLLC_ALNCTL_ALN7, YES)
                           /**< SYSCLK7 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK6 CSL_FMKT (PLLC_ALNCTL_ALN6, YES)
                           /**< SYSCLK6 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK5 CSL_FMKT (PLLC_ALNCTL_ALN5, YES)
                           /**< SYSCLK5 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK4 CSL_FMKT (PLLC_ALNCTL_ALN4, YES)
                           /**< SYSCLK4 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK3 CSL_FMKT (PLLC_ALNCTL_ALN3, YES)
                           /**< SYSCLK3 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK2 CSL_FMKT (PLLC_ALNCTL_ALN2, YES)
                           /**< SYSCLK2 needs to be aligned with other clocks
                               selected in this register */
#define CSL_PLLC_ALIGNCTL_SYSCLK1 CSL_FMKT (PLLC_ALNCTL_ALN1, YES)
                           /**< SYSCLK1 needs to be aligned with other clocks
                               selected in this register */
                           
/**
@} */

/** \defgroup CSL_PLLC_DIVEN_DEFINE PLLC Divider Enable
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_DIVEN_PREDIV    (1 << 0)
                           /**< PREDIV enable */
#define CSL_PLLC_DIVEN_POSTDIV   (1 << 1)
                            /**< POSTDIV enable */
#define CSL_PLLC_DIVEN_OSCDIV1   (1 << 2)
                           /**< Oscillator Divider OD1 Enable */
#define CSL_PLLC_DIVEN_PLLDIV1   (1 << 3) 
                            /**< Enable divider D1 for SYSCLK1 */
#define CSL_PLLC_DIVEN_PLLDIV2   (1 << 4) 
                            /**< Enable divider D2 for SYSCLK2 */
#define CSL_PLLC_DIVEN_PLLDIV3   (1 << 5) 
                            /**< Enable divider D3 for SYSCLK3 */
#define CSL_PLLC_DIVEN_PLLDIV4   (1 << 6) 
                            /**< Enable divider D4 for SYSCLK4 */
#define CSL_PLLC_DIVEN_PLLDIV5   (1 << 7) 
                            /**< Enable divider D5 for SYSCLK5 */
#define CSL_PLLC_DIVEN_PLLDIV6   (1 << 8) 
                            /**< Enable divider D6 for SYSCLK6 */
#define CSL_PLLC_DIVEN_PLLDIV7   (1 << 9) 
                            /**< Enable divider D7 for SYSCLK7 */
#define CSL_PLLC_DIVEN_PLLDIV8   (1 << 10) 
                            /**< Enable divider D8 for SYSCLK8 */
                            
                            
/**
@} */


/** \defgroup CSL_PLLC_DIVSEL_DEFINE Divider Select for SYSCLKs 
*   \ingroup CSL_PLLC_DEFINE
*   
*
* @{ */
#define CSL_PLLC_DIVSEL_PLLDIV1     (1) 
                            /**< Divider D1 for SYSCLK1 */
#define CSL_PLLC_DIVSEL_PLLDIV2     (2) 
                            /**< Divider D2 for SYSCLK2 */
#define CSL_PLLC_DIVSEL_PLLDIV3     (3) 
                            /**< Divider D3 for SYSCLK3 */
#define CSL_PLLC_DIVSEL_PLLDIV4     (4) 
                            /**< Divider D4 for SYSCLK4 */
#define CSL_PLLC_DIVSEL_PLLDIV5     (5) 
                            /**< Divider D5 for SYSCLK5 */
#define CSL_PLLC_DIVSEL_PLLDIV6     (6) 
                            /**< Divider D6 for SYSCLK6 */
#define CSL_PLLC_DIVSEL_PLLDIV7     (7) 
                            /**< Divider D7 for SYSCLK7 */
#define CSL_PLLC_DIVSEL_PLLDIV8     (8) 
                            /**< Divider D8 for SYSCLK8 */
                            
/**
@} */

/**************************************************************************\
* PLLC global typedef declarations
\**************************************************************************/

/**\defgroup CSL_PLLC_RSTDEF_ENUM Reset Definition
*  \ingroup CSL_PLLC_ENUM
*
*  
@{*/

/** \brief  Enums for PLL XWRST pin function
* 
*/
typedef enum {
  CSL_PLLC_RSTDEF_NORMAL = CSL_PLLC_RSTDEF_XWFUNC_NORM, /**< <b>: Normal mode.
                            xwrst_pi_n causes the xwrst_pi_n operation</b> */
  CSL_PLLC_RSTDEF_FAST = CSL_PLLC_RSTDEF_XWFUNC_FAST /**< <b>: Fast mode.
                       xwrst_pi_n causes the system/chip reset operation</b> */
} CSL_PllcResetDef;
/**
@} */


/**\defgroup CSL_PLLC_OSCDIVCTL_ENUM Oscillator Divide Control
*  \ingroup CSL_PLLC_ENUM
*
*  
@{*/

/** \brief  Enums for Oscillator divide enable/ disable
* 
*/
typedef enum {
  CSL_PLLC_OSCDIV_DISABLE = CSL_PLLC_OSCDIV1_OD1EN_DISABLE,
                            /**< <b>: Oscillator Divider OD1 Disable</b> */
  CSL_PLLC_OSCDIV_ENABLE = CSL_PLLC_OSCDIV1_OD1EN_ENABLE 
                           /**< <b>: Oscillator Divider OD1 Enable</b> */
} CSL_PllcOscDivCtrl;
/**
@} */


/**\defgroup CSL_PLLC_PLLDIVCTL_ENUM PLL Divide Control
*  \ingroup CSL_PLLC_ENUM
*
*  
@{*/

/** \brief  Enums for PLL divide enable/ disable
* 
*/
typedef enum {
  CSL_PLLC_PLLDIV_DISABLE = 0,
                            /**< <b>: PLL Divider Disable</b> */
  CSL_PLLC_PLLDIV_ENABLE = 1 
                           /**< <b>: PLL Divider Enable</b> */
} CSL_PllcDivCtrl;
/**
@} */


/**\defgroup CSL_PLLC_CONTROLCMD_ENUM  Control Commands 
*  \ingroup CSL_PLLC_CONTROL_API
@{*/
/** \brief Enumeration for control commands passed to \a CSL_pllcHwControl()
*
* This is the set of commands that are passed to the \a CSL_pllcHwControl()
* with an optional argument type-casted to \a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration
*/
typedef enum {
  CSL_PLLC_CMD_PLLCONTROL,/**< \brief Control PLL based on the bits set in the
                               input argument     
                              \param CSL_BitMask32
                              \return CSL_SOK 
                              \sa CSL_PLLC_CTRL_DEFINE */
  CSL_PLLC_CMD_CLOCK_ENABLE,/**< \brief Enable the clocks as specified by input
			                bitmask
                              \param CSL_BitMask32
                              \return CSL_SOK 
                              \sa CSL_PLLC_CLKEN_DEFINE */
  CSL_PLLC_CMD_CLOCK_DISABLE,/**< \brief Disable the clocks specified by input
			                 bitmask
                              \param CSL_BitMask32
                              \return CSL_SOK 
                              \sa CSL_PLLC_CLKEN_DEFINE */
  CSL_PLLC_CMD_DEFINE_RESET,/**< \brief Define XWRST pin function
                              \param CSL_PllcResetDef
                              \return CSL_SOK  
                              \sa CSL_PllcResetDef */
  CSL_PLLC_CMD_SET_PHASEALIGN,/**< \brief By setting the ALN bits, the selected
                               SYSCLKx (specified by input bitmask) will always
			       be phase aligned to other clocks also selected 
			       in this register
                              \param CSL_BitMask32
                              \return CSL_SOK 
                              \sa CSL_PLLC_ALIGNCTL_DEFINE */
  CSL_PLLC_CMD_SET_PLLM,/**< \brief Set PLL multiplier value
                              \param Uint32
                              \return CSL_SOK */
  CSL_PLLC_CMD_SET_OSCRATIO,/**< \brief Set oscillator divide ratio
                                 \param Uint32
                                 \return CSL_SOK */
  CSL_PLLC_CMD_SET_PLLRATIO,/**< \brief Set PLL divide ratio
                                 \param CSL_PllcDivRatio
                                 \return CSL_SOK
                                 \sa CSL_PllcDivRatio */
  CSL_PLLC_CMD_OSCDIV_CONTROL,/**< \brief Enable/disable  oscillator divider
                                 \param CSL_PllcOscDivCtrl
                                 \return CSL_SOK
                                 \sa CSL_PllcOscDivCtrl */
  CSL_PLLC_CMD_PLLDIV_CONTROL,/**< \brief Enable/disable  PLL divider
                                 \param CSL_PllcDivideControl
                                 \return CSL_SOK
                                 \sa CSL_PllcDivideControl */
  CSL_PLLC_CMD_WAKEUP /**< \brief Enable/disable  Wake Up functionality of wakeup pins
                         \param CSL_BitMask16
                         \return CSL_SOK */
                                 
} CSL_PllcHwControlCmd;
/**
@} */


/**\defgroup CSL_PLLC_QUERYCMD_ENUM Query Commands 
*  \ingroup CSL_PLLC_QUERY_API
* @{ */

/** \brief Enumeration for queries passed to \a CSL_PllcGetHwStatus()
*
* This is used to get the status of different operations.The arguments
* to be passed with each enumeration if any are specified next to 
* the enumeration */
typedef enum {
  CSL_PLLC_QUERY_PID,/**< \brief Queries PLL Control Peripheral ID
                         \param (Uint32*) 
                         \return CSL_SOK */
  CSL_PLLC_QUERY_STATUS,/**< \brief Queries PLL Controller Status
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_STATUS_DEFINE */
  CSL_PLLC_QUERY_DIVRATIO_CHANGE,/**< \brief Queries PLLDIV Modified Status
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_DCHANGESTAT_DEFINE */
  CSL_PLLC_QUERY_CLKSTAT,/**< \brief Queries Clock Status 
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_CLKSTAT_DEFINE */   
  CSL_PLLC_QUERY_SYSCLKSTAT,/**< \brief Queries PLL SYSCLK Status
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_SYSCLKSTAT_DEFINE */
  CSL_PLLC_QUERY_CLKENSTAT, /**< \brief Queries CLK Enable Status
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_CLKEN_DEFINE */
                               
  CSL_PLLC_QUERY_RESETSTAT,/**< \brief Queries Reset Type Status
                              \param (CSL_BitMask32*) 
                              \return CSL_SOK
                              \sa CSL_PLLC_RESETSTAT_DEFINE */
  CSL_PLLC_QUERY_EFUSEERR/**< \brief Queries Fuse Farm Error Status
                              \param (Uint32*) 
                              \return CSL_SOK */                              
                                  
} CSL_PllcHwStatusQuery;

/**
@} */




/** 
\addtogroup CSL_PLLC_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up PLL Divide ratio
*
*  \sa CSL_PLLC_DIVSEL_DEFINE
*  
*/
typedef struct CSL_PllcDivRatio {
  Uint32 divNum;
  Uint32 divRatio;
} CSL_PllcDivRatio;
/**
@} */

/** 
\addtogroup CSL_PLLC_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for enabling\disabling PLL Divide ratio
*
*  \sa CSL_PLLC_DIVSEL_DEFINE
*  
*/
typedef struct CSL_PllcDivideControl {
  Uint32 divNum;
  CSL_PllcDivCtrl divCtrl;
} CSL_PllcDivideControl;
/**
@} */

/** 
\addtogroup CSL_PLLC_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up PLL Controller
*
*  Used to put PLLC known useful state
*  /sa CSL_PLLC_DIVEN_DEFINE       
*/
typedef struct CSL_PllcHwSetup {
  CSL_BitMask32 divEnable;
  Uint32        preDiv;
  Uint32        pllM;
  Uint32        postDiv;
  Uint32        pllDiv1;
  Uint32        pllDiv2;
  Uint32        pllDiv3;
  Uint32        pllDiv4;
  Uint32        pllDiv5;
  Uint32        pllDiv6;
  Uint32        pllDiv7;
  Uint32        pllDiv8;
  Uint32        oscDiv1;
  CSL_BitMask32 phaseAlign;
  void*         extendSetup; 
} CSL_PllcHwSetup;
/**
@} */


/** 
\addtogroup CSL_PLLC_DATASTRUCT 
@{
*/

/** \brief This object contains the reference to the instance of PLLC
* opened using the @a CSL_pllcOpen()
*
* The pointer to this is passed to all PLLC CSL APIs
* This structure has the fields required to configure PLLC for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/
typedef struct CSL_PllcObj {
  CSL_OpenMode openMode;/**< This is the mode in which the CSL instance
                             is opened */
  CSL_Uid      uid;/**< This is a unique identifier to the instance of PLLC 
                        being referred to by this object */
  CSL_Xio      xio;/**< This is the variable that holds the current state
                        of a resource being shared by current instance of
                        PLLC with other peripherals */
  CSL_PllcRegsOvly regs;/**< This is a pointer to the registers of the  
                             instance of PLLC referred to by this object */
  CSL_PllcNum  pllcNum;/**< This is the instance of PLLC being referred to
                           by this object */
} CSL_PllcObj;

/** \brief this is a pointer to @a CSL_PllcObj and is passed as the first
* parameter to all PLLC CSL APIs */
typedef CSL_PllcObj* CSL_PllcHandle; 
/**
@} */




/*****************************************************************************\
          CSL3.x mandatory function prototype definitions
\*****************************************************************************/
/**
\defgroup CSL_PLLC_INIT_API PLLC Init API
\ingroup CSL_PLLC_FUNCTION
    \brief Initializes the PLLC
@{*/
CSL_Status CSL_pllcInit ( void );
/**
@} */

/**
\defgroup CSL_PLLC_OPEN_API PLLC Open API
\ingroup CSL_PLLC_FUNCTION
    \brief Opens if possible the instance of PLLC requested
    \n  Reserves the specified pllc for use. The device can be 
    re-opened anytime after it has been normally closed if so
    required. The handle returned by this call is input as an
    essential argument for rest of the APIs described for this
    module.

    <b> Usage Constraints: </b>
    The CSL system as well as PLLC must be succesfully initialized
    via @a CSL_sysInit() and @a CSL_pllcInit() before calling this
    function. Memory for the @a CSL_PllcObj must be allocated outside
    this call. This object must be retained while usage of this peripheral.
  
    \b Example:
    \verbatim


    CSL_PllcObj     pllcObj;
    CSL_PllcHwSetup pllcSetup;
    CSL_Status       status;
    ...
    hPllc = CSL_pllcOpen(&pllcobj,
                         CSL_PLLC_0,
                         CSL_EXCLUSIVE,
                         &pllcSetup,
                         &status);
    \endverbatim
  
@{*/
CSL_PllcHandle CSL_pllcOpen ( CSL_PllcObj *hPllcObj,
                              CSL_PllcNum pllcNum,
                              CSL_OpenMode openMode,
                              CSL_PllcHwSetup *hwSetup,
                              CSL_Status *status );
/**
@} */

/**
\defgroup CSL_PLLC_CLOSE_API PLLC Close API
\ingroup CSL_PLLC_FUNCTION
    \brief Closes the instance of PLLC requested

    
@{*/
CSL_Status  CSL_pllcClose ( CSL_PllcHandle hPllc );
/**
@} */


/**
\defgroup CSL_PLLC_SETUP_API PLLC Setup API
\ingroup CSL_PLLC_FUNCTION
    \brief Programs the PLLC with the setup configuration as specified in
           the arguments <b>This function requires some tuning, as CLKIN/OSCIN
           values are not known </b>
   		  
	   
@{*/
CSL_Status CSL_pllcHwSetup ( CSL_PllcHandle hPllc,
                             CSL_PllcHwSetup *setup );
/**
@} */

/**
\defgroup CSL_PLLC_GETSETUP_API PLLC GetSetup API
\ingroup CSL_PLLC_FUNCTION
    \brief Reads the PLLC setup configuration   		  
	   
@{*/
CSL_Status CSL_pllcGetHwSetup ( CSL_PllcHandle hPllc,
                                CSL_PllcHwSetup *setup );
/**
@} */


/**
\defgroup CSL_PLLC_CONTROL_API PLLC Control API
\ingroup CSL_PLLC_FUNCTION
    \brief Controls the different operations that can be performed by PLLC 
 		  
     		  
    
@{*/
CSL_Status CSL_pllcHwControl ( CSL_PllcHandle hPllc,
                               CSL_PllcHwControlCmd cmd,
                               void *arg );
/**
@} */

/**
\defgroup CSL_PLLC_QUERY_API PLLC Query API
\ingroup CSL_PLLC_FUNCTION
    \brief Returns the status of the requested operation on PLLC
@{*/

CSL_Status CSL_pllcGetHwStatus ( CSL_PllcHandle hPllc,
                                 CSL_PllcHwStatusQuery query,
                                 void *response );
/**
@} */

#ifdef __cplusplus
}
#endif
	
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         16 Nov 2004 08:01:03    4159             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL 0.55 Release                                    */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:06    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
