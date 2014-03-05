/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage PWM CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the PWM module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given PWM module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular PWM Module. The CSL developer should use his discretion designing
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
/** \file  csl_pwm.h
    \brief PWM functional layer API header file
    \internal
    \date 27th April, 2004
    \author Pratheesh Gangadhar (pratheesh@ti.com)
*/
#ifndef _CSL_PWM_H_
#define _CSL_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_pwm.h>

/**
\defgroup CSL_PWM_API PWM
   \internal Based on <b>Imaging and Audio Group PWM Detailed Design 
                Specification Rev 1.00.w.03 </b> 
*/
/**
\defgroup CSL_PWM_DATASTRUCT Data Structures
\ingroup CSL_PWM_API
*/
/**
\defgroup CSL_PWM_DEFINE  Defines
\ingroup CSL_PWM_API
*/
/**
\defgroup CSL_PWM_ENUM  Enumerated Data Types
\ingroup CSL_PWM_API
*/

/**
\defgroup CSL_PWM_FUNCTION  Functions
\ingroup CSL_PWM_API
*/

/*****************************************************************************\
          LCDC global macro declarations
\*****************************************************************************/ 

/** \defgroup CSL_PWM_ERROR_DEFINE Error Code
*   \ingroup CSL_PWM_DEFINE
*   
*
* @{ */
#define CSL_EPWM_INVMODE CSL_EPWM_FIRST /**< Invalid mode */

/**
@} */

/** \defgroup CSL_PWM_OUTPUT_DEFINE Output Level
*   \ingroup CSL_PWM_DEFINE
*   
*
* @{ */
#define CSL_PWM_OUTPUT_LOW CSL_PWM_CFG_CURLEV_LOW  /**< Low */
#define CSL_PWM_OUTPUT_HIGH CSL_PWM_CFG_CURLEV_HIGH  /**< High */

/**
@} */

/** \defgroup CSL_PWM_OPERSTAT_DEFINE Operation Staus
*   \ingroup CSL_PWM_DEFINE
*   
*
* @{ */
#define CSL_PWM_OPERSTAT_IDLE CSL_PWM_CFG_OPST_IDLE  /**< Idle */
#define CSL_PWM_OPERSTAT_RUN CSL_PWM_CFG_OPST_RUN  /**< Run */

/**
@} */


/**************************************************************************\
* PWM global typedef declarations
\**************************************************************************/

/**\defgroup CSL_PWM_OPMODE_ENUM PWM Operating Modes 
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/

/** \brief  Enums for PWM operating modes
* 
*/
typedef enum {
  CSL_PWM_DISABLE, /**< <b>: Disable PWM module</b> */
  CSL_PWM_ONESHOT, /**< <b>: One shot mode</b> */
  CSL_PWM_CONTINUOUS/**< <b>: Continuous mode</b> */
} CSL_PwmMode;
/**
@} */

/**\defgroup CSL_PWM_EMUMODE_ENUM Emulation Control
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/
/** \brief  Enums for Emulation modes
*
*   This is used by control commands to configure emulation mode
*/
typedef enum {
  CSL_PWM_EMU_STOP = CSL_PWM_PCR_FREE_STOP,
                      /**< <b>: PWM stops immediately</b>*/
  CSL_PWM_EMU_RUN  = CSL_PWM_PCR_FREE_RUN
                      /**< <b>: PWM runs free </b>*/
} CSL_PwmEmuCtrl;
/**
@} */


/**\defgroup CSL_PWM_INTR_ENUM Interrupt Control 
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/

/** \brief  Enums for PWM interrupt control
* 
*/
typedef enum {
  CSL_PWM_INTR_DISABLE = CSL_PWM_CFG_INTEN_DISABLE, /**< <b>: Disable PWM
                                                       interrupt</b> */
  CSL_PWM_INTR_ENABLE = CSL_PWM_CFG_INTEN_ENABLE /**< <b>: Enable PWM
                                                    interrupt</b> */
} CSL_PwmInterrupt;
/**
@} */


/**\defgroup CSL_PWM_EVENTTRIG_ENUM Event Trigger
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/

/** \brief  Enums for PWM event trigger configuration
* 
*/
typedef enum {
  CSL_PWM_EVTTRIG_DISABLE = CSL_PWM_CFG_EVTRIG_DISABLE, /**< <b>: Disable PWM
                                                           event trigger</b> */
  CSL_PWM_EVTTRIG_POSITIVE_EDGE = CSL_PWM_CFG_EVTRIG_PEDGE, /**< <b>: Enable 
                                                   positive edge trigger</b> */
  CSL_PWM_EVTTRIG_NEGATIVE_EDGE = CSL_PWM_CFG_EVTRIG_NEDGE/**< <b>: Enable 
                                                   negative edge trigger</b> */                                                   
} CSL_PwmEventTrigger;
/**
@} */

/**\defgroup CSL_PWM_INACTLEVEL_ENUM Inactive Output Level 
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/

/** \brief  Enums for PWM inactive phase output level
* 
*/
typedef enum {
  CSL_PWM_INACTOUT_LOW = CSL_PWM_CFG_INACTOUT_LOW,/**< <b>: Inactive level
                                                            low </b> */
  CSL_PWM_INACTOUT_HIGH = CSL_PWM_CFG_INACTOUT_HIGH /**< <b>: Inactive level
                                                             high</b> */
 
} CSL_PwmInactOutLevel;
/**
@} */

/**\defgroup CSL_PWM_P1OUT_ENUM First Phase Output Level 
*  \ingroup CSL_PWM_ENUM
*
*  
@{*/

/** \brief  Enums for PWM first phase output level
* 
*/

typedef enum {
  CSL_PWM_P1OUT_LOW = CSL_PWM_CFG_P1OUT_LOW,/**< <b>: First phase output level
                                                    low </b> */
  CSL_PWM_P1OUT_HIGH = CSL_PWM_CFG_P1OUT_HIGH /**< <b>: First phase output 
                                                      level high </b> */
 
} CSL_PwmP1OutLevel;
/**
@} */

/**\defgroup CSL_PWM_CONTROLCMD_ENUM  Control Commands 
*  \ingroup CSL_PWM_CONTROL_API
@{*/
/** \brief Enumeration for control commands passed to \a CSL_pwmHwControl()
*
* This is the set of commands that are passed to the \a CSL_pwmHwControl()
* with an optional argument type-casted to \a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration
*/
typedef enum {
  CSL_PWM_CMD_START,/**< \brief Start PWM operation
                         \param NONE
                         \return CSL_SOK */
  CSL_PWM_CMD_SET_PERIOD,/**< \brief Set  (PWM output period - 1)
                              \param Uint32
                              \return CSL_SOK */
  CSL_PWM_CMD_SET_PHASE1_DURATION,/**< \brief  Set Phase1 Duration
                                       \param Uint32
                                       \return CSL_SOK */
  CSL_PWM_CMD_SET_MODE,/**< \brief  Set mode of operation for PWM
                            \param  CSL_PwmMode
                            \return CSL_SOK 
			                \sa     CSL_PwmMode */
  CSL_PWM_CMD_CONFIG_ONE_SHOT,/**< \brief  Configure PWM for one-shot mode
                                   \param CSL_PwmConfigOneShot
                                   \return CSL_SOK 
				                   \sa CSL_PwmConfigOneShot  */
  CSL_PWM_CMD_CONFIG_CONTINUOUS,/**< \brief  Configure PWM for continuous mode
                                   \param CSL_PwmConfigContinuous
                                   \return CSL_SOK
				                   \sa CSL_PwmConfigContinuous */
  CSL_PWM_CMD_STOP, /**< \brief PWM stop immediately 
                         \param  NONE
                         \return CSL_SOK */
  CSL_PWM_CMD_RUN_FREE /**< \brief  PWM runs free
                             \param  NONE
                             \return CSL_SOK */
} CSL_PwmHwControlCmd;
/**
@} */


/**\defgroup CSL_PWM_QUERYCMD_ENUM Query Commands 
*  \ingroup CSL_PWM_QUERY_API
* @{ */

/** \brief Enumeration for queries passed to \a CSL_PwmGetHwStatus()
*
* This is used to get the status of different operations.The arguments
* to be passed with each enumeration if any are specified next to 
* the enumeration */
typedef enum {
  CSL_PWM_QUERY_OUTPUT_STATUS,/**< \brief Queries PWM output status
                                  \param (Uint32*) 
                                  \return CSL_SOK 
                                  \sa CSL_PWM_OUTPUT_DEFINE */
  CSL_PWM_QUERY_OPER_STATUS,/**< \brief Queries PWM operation status
                                 \param (Uint32*) 
                                 \return CSL_SOK 
                                 \sa CSL_PWM_OPERSTAT_DEFINE */
  CSL_PWM_QUERY_PID/**< \brief Queries peripheral id register 
                        \param (Uint32*) 
                        \return CSL_SOK */                                 
                                  
} CSL_PwmHwStatusQuery;

/**
@} */



/** 
\addtogroup CSL_PWM_DATASTRUCT 
@{
*/

/**
   \brief Configuration parameters for one-shot mode of operation of PWM 
*
*   Used to configure one-shot mode parameters of PWM   
*/

typedef struct CSL_PwmConfigOneShot {
  CSL_PwmInterrupt   intEn;/**< Interrupt Enable */
  CSL_PwmInactOutLevel   inactOut;/**< Inactive Output Level */
  CSL_PwmP1OutLevel   p1Out;/**< First Phase Output Level */  
  Uint32  rpt;/**< Repeat count
                <br> PWM repeat count = Repeat count+1 clock cycles */
  CSL_PwmEventTrigger   evtTrig;/**< Event Trigger */
} CSL_PwmConfigOneShot;
/**
@} */

/** 
\addtogroup CSL_PWM_DATASTRUCT 
@{
*/

/**
   \brief Configuration parameters for continuous mode of operation of PWM 
*
*   Used to configure continuous mode parameters of PWM   
*/

typedef struct CSL_PwmConfigContinuous {
  CSL_PwmInterrupt   intEn;/**< Interrupt Enable */
  CSL_PwmInactOutLevel   inactOut;/**< Inactive Output Level */
  CSL_PwmP1OutLevel   p1Out;/**< First Phase Output Level */ 
} CSL_PwmConfigContinuous;
/**
@} */


/** 
\addtogroup CSL_PWM_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up PWM
*
*  Used to put PWM known useful state       
*/
typedef struct CSL_PwmHwSetup {
  CSL_PwmMode modeSelect; 	
  Uint32 outPeriod;/**< (PWM output period - 1).*/
  Uint32 phase1Duation;/**<  Phase1 Duration.*/
  CSL_PwmEmuCtrl emuConfig;
  void*  extendSetup; /* Since we use union*/ 
  union {
    CSL_PwmConfigOneShot oneShot;
    CSL_PwmConfigContinuous continuous;
  } mode;
} CSL_PwmHwSetup;
/**
@} */
/** 
\addtogroup CSL_PWM_DATASTRUCT 
@{
*/

/** \brief This object contains the reference to the instance of PWM
* opened using the @a CSL_pwmOpen()
*
* The pointer to this is passed to all PWM CSL APIs
* This structure has the fields required to configure PWM for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/
typedef struct CSL_PwmObj {
  CSL_OpenMode openMode;/**< This is the mode in which the CSL instance
                             is opened */
  CSL_Uid      uid;/**< This is a unique identifier to the instance of PWM 
                        being referred to by this object */
  CSL_Xio      xio;/**< This is the variable that holds the current state
                        of a resource being shared by current instance of
                        PWM with other peripherals */
  CSL_PwmRegsOvly regs;/**< This is a pointer to the registers of the  
                             instance of PWM referred to by this object */
  CSL_PwmNum  pwmNum;/**< This is the instance of PWM being referred to
                           by this object */
} CSL_PwmObj;

/** \brief this is a pointer to @a CSL_PwmObj and is passed as the first
* parameter to all PWM CSL APIs */
typedef CSL_PwmObj* CSL_PwmHandle; 
/**
@} */




/*****************************************************************************\
          CSL3.x mandatory function prototype definitions
\*****************************************************************************/
/**
\defgroup CSL_PWM_INIT_API PWM Init API
\ingroup CSL_PWM_FUNCTION
    \brief Initializes the PWM
@{*/
CSL_Status CSL_pwmInit ( void );
/**
@} */

/**
\defgroup CSL_PWM_OPEN_API PWM Open API
\ingroup CSL_PWM_FUNCTION
    \brief Opens if possible the instance of PWM requested
    \n  Reserves the specified pwm for use. The device can be 
    re-opened anytime after it has been normally closed if so
    required. The handle returned by this call is input as an
    essential argument for rest of the APIs described for this
    module.

    <b> Usage Constraints: </b>
    The CSL system as well as PWM must be succesfully initialized
    via @a CSL_sysInit() and @a CSL_pwmInit() before calling this
    function. Memory for the @a CSL_PwmObj must be allocated outside
    this call. This object must be retained while usage of this peripheral.
  
    \b Example:
    \verbatim


    CSL_PwmObj     pwmObj;
    CSL_PwmHwSetup pwmSetup;
    CSL_Status       status;
    ...
    hPwm = CSL_pwmOpen(&pwmobj,
                         CSL_PWM_0,
                         CSL_EXCLUSIVE,
                         &pwmSetup,
                         &status);
    \endverbatim
  
@{*/
CSL_PwmHandle CSL_pwmOpen ( CSL_PwmObj *hPwmObj,
                            CSL_PwmNum pwmNum,
                            CSL_OpenMode openMode,
                            CSL_PwmHwSetup *hwSetup,
                            CSL_Status *status );
/**
@} */

/**
\defgroup CSL_PWM_CLOSE_API PWM Close API
\ingroup CSL_PWM_FUNCTION
    \brief Closes the instance of PWM requested

    
@{*/
CSL_Status  CSL_pwmClose ( CSL_PwmHandle hPwm );
/**
@} */


/**
\defgroup CSL_PWM_SETUP_API PWM Setup API
\ingroup CSL_PWM_FUNCTION
    \brief Programs the PWM with the setup configuration as specified in
           the arguments
   		  
	   
@{*/
CSL_Status CSL_pwmHwSetup ( CSL_PwmHandle hPwm,
                            CSL_PwmHwSetup *setup );
/**
@} */

/**
\defgroup CSL_PWM_GETSETUP_API PWM GetSetup API
\ingroup CSL_PWM_FUNCTION
    \brief Reads the PWM setup configuration   		  
	   
@{*/
CSL_Status CSL_pwmGetHwSetup ( CSL_PwmHandle hPwm,
                               CSL_PwmHwSetup *setup );
/**
@} */


/**
\defgroup CSL_PWM_CONTROL_API PWM Control API
\ingroup CSL_PWM_FUNCTION
    \brief Controls the different operations that can be performed by PWM 
 		  
     		  
    
@{*/
CSL_Status CSL_pwmHwControl ( CSL_PwmHandle hPwm,
                              CSL_PwmHwControlCmd cmd,
                              void *arg );
/**
@} */

/**
\defgroup CSL_PWM_QUERY_API PWM Query API
\ingroup CSL_PWM_FUNCTION
    \brief Returns the status of the requested operation on PWM
@{*/

CSL_Status CSL_pwmGetHwStatus ( CSL_PwmHandle hPwm,
                                CSL_PwmHwStatusQuery query,
                                void *response );
/**
@} */

#ifdef __cplusplus
}
#endif
	
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:06 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:06    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
