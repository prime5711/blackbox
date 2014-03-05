/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2004
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/

/** @mainpage GPIO CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the GPIO module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given GPIO module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular GPIO Module. The CSL developer should use his discretion designing
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

/** \file  csl_gpio.h
    \brief GPIO functional layer API header file
    \internal
    \date 11th June, 2004
    \author Pratheesh Gangadhar (pratheesh@ti.com)
*/
#ifndef _CSL_GPIO_H_
#define _CSL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_gpio.h>

/* Added from cslr_gpio.h to this file */

#define CSL_GPIO_NUM_PINS          (69)

#define CSL_GPIO_NUM_BANKS       (CSL_GPIO_NUM_PINS + 15)/16

           
typedef enum {
  CSL_GPIO_PIN0,
  CSL_GPIO_PIN1,
  CSL_GPIO_PIN2,
  CSL_GPIO_PIN3,
  CSL_GPIO_PIN4,
  CSL_GPIO_PIN5,
  CSL_GPIO_PIN6,
  CSL_GPIO_PIN7,
  CSL_GPIO_PIN8,
  CSL_GPIO_PIN9,
  CSL_GPIO_PIN10,
  CSL_GPIO_PIN11,
  CSL_GPIO_PIN12,
  CSL_GPIO_PIN13,
  CSL_GPIO_PIN14,
  CSL_GPIO_PIN15,
  CSL_GPIO_PIN16,
  CSL_GPIO_PIN17,
  CSL_GPIO_PIN18,
  CSL_GPIO_PIN19,
  CSL_GPIO_PIN20,
  CSL_GPIO_PIN21,
  CSL_GPIO_PIN22,
  CSL_GPIO_PIN23,
  CSL_GPIO_PIN24,
  CSL_GPIO_PIN25,
  CSL_GPIO_PIN26,
  CSL_GPIO_PIN27,
  CSL_GPIO_PIN28,
  CSL_GPIO_PIN29,
  CSL_GPIO_PIN30,
  CSL_GPIO_PIN31,
  CSL_GPIO_PIN32,
  CSL_GPIO_PIN33,
  CSL_GPIO_PIN34,
  CSL_GPIO_PIN35,
  CSL_GPIO_PIN36,
  CSL_GPIO_PIN37,
  CSL_GPIO_PIN38,
  CSL_GPIO_PIN39,
  CSL_GPIO_PIN40,
  CSL_GPIO_PIN41,
  CSL_GPIO_PIN42,
  CSL_GPIO_PIN43,
  CSL_GPIO_PIN44,
  CSL_GPIO_PIN45,
  CSL_GPIO_PIN46,
  CSL_GPIO_PIN47,
  CSL_GPIO_PIN48,
  CSL_GPIO_PIN49,
  CSL_GPIO_PIN50,
  CSL_GPIO_PIN51,
  CSL_GPIO_PIN52,
  CSL_GPIO_PIN53,
  CSL_GPIO_PIN54,
  CSL_GPIO_PIN55,
  CSL_GPIO_PIN56,
  CSL_GPIO_PIN57,
  CSL_GPIO_PIN58,
  CSL_GPIO_PIN59,
  CSL_GPIO_PIN60,
  CSL_GPIO_PIN61,
  CSL_GPIO_PIN62,
  CSL_GPIO_PIN63,
  CSL_GPIO_PIN64,
  CSL_GPIO_PIN65,
  CSL_GPIO_PIN66,
  CSL_GPIO_PIN67,
  CSL_GPIO_PIN68
} CSL_GpioPinNum;

typedef enum {
  CSL_GPIO_BANK0,
  CSL_GPIO_BANK1,
  CSL_GPIO_BANK2,
  CSL_GPIO_BANK3,
  CSL_GPIO_BANK4
} CSL_GpioBankNum;

/**
\defgroup CSL_GPIO_API GPIO
   \internal Based on <b>Imaging and Audio Group GPIO Detailed Design 
                Specification Rev 1.00.w.02 </b> 
*/
/**
\defgroup CSL_GPIO_DATASTRUCT Data Structures
\ingroup CSL_GPIO_API
*/
/**
\defgroup CSL_GPIO_DEFINE  Defines
\ingroup CSL_GPIO_API
*/
/**
\defgroup CSL_GPIO_ENUM  Enumerated Data Types
\ingroup CSL_GPIO_API
*/

/**
\defgroup CSL_GPIO_FUNCTION  Functions
\ingroup CSL_GPIO_API
*/

/** \defgroup CSL_GPIO_ERROR_DEFINE Error Code
*   \ingroup CSL_GPIO_DEFINE
*   
*
* @{ */
#define CSL_EGPIO_INVPARAM CSL_EGPIO_FIRST
	                   /**< Invalid argument */
/**
@} */

/*****************************************************************************\
          GPIO global typedef declarations
\*****************************************************************************/
/**\defgroup CSL_GPIO_DIR_ENUM GPIO Pin Direction 
*  \ingroup CSL_GPIO_ENUM
*
*  
@{*/

/** \brief  Enums for configuring GPIO pin direction
* 
*/
typedef enum {
  CSL_GPIO_DIR_OUTPUT,/**<<b>: Output pin</b>*/
  CSL_GPIO_DIR_INPUT  /**<<b>: Input pin</b>*/
} CSL_GpioDirection;

/**
@} */

/**\defgroup CSL_GPIO_EDGDET_ENUM GPIO Edge Detection
*  \ingroup CSL_GPIO_ENUM
*
*  
@{*/

/** \brief  Enums for configuring GPIO pin edge detection
* 
*/
typedef enum {
  CSL_GPIO_TRIG_CLEAR_EDGE,  /**<<b>: No edge detection </b>*/
  CSL_GPIO_TRIG_RISING_EDGE, /**<<b>: Rising edge detection </b>*/
  CSL_GPIO_TRIG_FALLING_EDGE,/**<<b>: Falling edge detection </b>*/
  CSL_GPIO_TRIG_DUAL_EDGE    /**<<b>: Dual edge detection </b>*/
} CSL_GpioTriggerType;
/**
@} */


/**\defgroup CSL_GPIO_CONTROLCMD_ENUM  Control Commands 
*  \ingroup CSL_GPIO_CONTROL_API
@{*/
/** \brief Enumeration for control commands passed to \a CSL_gpioHwControl()
*
* This is the set of commands that are passed to the \a CSL_gpioHwControl()
* with an optional argument type-casted to \a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration
*/
typedef enum {
  CSL_GPIO_CMD_BANK_INT_ENABLE, /**< \brief Enables interrupt on specified bank 
                                     \param CSL_GpioBankNum
                                     \return CSL_SOK or CSL_EGPIO_INVPARAM
                                     \see CSL_GpioBankNum */
  CSL_GPIO_CMD_BANK_INT_DISABLE,/**< \brief Disables interrupt on specified 
                                            bank 
                                     \param CSL_GpioBankNum
                                     \return CSL_SOK or CSL_EGPIO_INVPARAM
                                     \see CSL_GpioBankNum */
  CSL_GPIO_CMD_CONFIG_BIT,/**< \brief Configures GPIO pin direction and edge
                                      detection properties  
                               \param CSL_GpioConfig
                               \return CSL_SOK or CSL_EGPIO_INVPARAM
                               \see CSL_GpioConfig */
  CSL_GPIO_CMD_SET_BIT,/**< \brief Changes output state of GPIO pin to logic-1 
                            \param CSL_GpioPinNum
                            \return CSL_SOK or CSL_EGPIO_INVPARAM
                            \see CSL_GpioPinNum */
  CSL_GPIO_CMD_CLEAR_BIT,/**< \brief Changes output state of GPIO pin to 
                                     logic-0 
                              \param CSL_GpioPinNum
                              \return CSL_SOK or CSL_EGPIO_INVPARAM
                              \see CSL_GpioPinNum */
  CSL_GPIO_CMD_GET_INTSTATUS,/**< \brief Gets the interrupt status of 
                                         individual pins on banks 2*i and 2*i+1
                                  \param CSL_GpioBankData
                                  \n The "data" field act as output parameter
                                      reporting the interrupt status of the 
                                      GPIO pins on the banks
                                  \return CSL_SOK or CSL_EGPIO_INVPARAM
                                  \see CSL_GpioBankData */
  CSL_GPIO_CMD_GET_INPUTBIT,/**< \brief Gets the state of input pins on banks
                                         2*i and 2*i+1
                                  \param CSL_GpioBankData
                                  \n The "data" field act as output parameter
                                      reporting the input state of the 
                                      GPIO pins on the banks.
                                  \return CSL_SOK or CSL_EGPIO_INVPARAM
                                  \see CSL_GpioBankData */
  CSL_GPIO_CMD_GET_OUTDRVSTATE/**< \brief Gets the state of output pins on
                                    banks 2*i and 2*i+1
                                  \param CSL_GpioBankData
                                  \n The "data" field act as output parameter
                                      reporting the output drive state of the 
                                      GPIO pins on the banks 
                                  \return CSL_SOK or CSL_EGPIO_INVPARAM
                                  \see CSL_GpioBankData */
} CSL_GpioHwControlCmd;
/**
@} */


/**\defgroup CSL_GPIO_QUERYCMD_ENUM Query Commands 
*  \ingroup CSL_GPIO_QUERY_API
* @{ */

/** \brief Enumeration for queries passed to \a CSL_GpioGetHwStatus()
*
* This is used to get the status of different operations.The arguments
* to be passed with each enumeration if any are specified next to 
* the enumeration */
typedef enum {
  CSL_GPIO_QUERY_PID,/**< \brief Queries GPIO peripheral id
                          \param (Uint32*) 
                          \return CSL_SOK */
  CSL_GPIO_QUERY_BINTEN_STAT /**< \brief Queries GPIO bank interrupt enable
                                         status  
                                  \param (CSL_BitMask32*) 
                                  \return CSL_SOK */
} CSL_GpioHwStatusQuery;
/**
@} */


/** 
\addtogroup CSL_GPIO_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for configuring a GPIO pin
*
*  This is used to configure the direction and edge detection
* 
*/
typedef struct CSL_GpioConfig {
  CSL_GpioPinNum pinNum;
  CSL_GpioDirection direction;
  CSL_GpioTriggerType trigger;
} CSL_GpioConfig;

/**
@} */

/** 
\addtogroup CSL_GPIO_DATASTRUCT 
@{
*/

/**
   \brief Generic argument to get the pin information on banks 2*index and
          2*index+1
*
*  Used in CSL_gpioHwControl, designed this way to reduce the number of
*  register reads by a higher layer function 
*/
typedef struct CSL_GpioBankData {
  Uint32         index; /**< bank select*/   
  CSL_BitMask32  data;/**< desired information on the pins*/
} CSL_GpioBankData;
/**
@} */

/** 
\addtogroup CSL_GPIO_DATASTRUCT 
@{
*/

/**
   \brief Input parameters for setting up GPIO during startup
*
*  This is just a placeholder as GPIO is a simple module, which doesn't 
*  require any setup
*/

typedef struct CSL_GpioHwSetup {
  void *extendSetup;
} CSL_GpioHwSetup;
/**
@} */

/** 
\addtogroup CSL_GPIO_DATASTRUCT 
@{
*/

/** \brief This object contains the reference to the instance of GPIO
*          opened using the @a CSL_gpioOpen()
*
* The pointer to this is passed to all GPIO CSL APIs
* This structure has the fields required to configure GPIO for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/
typedef struct CSL_GpioObj {
  CSL_OpenMode openMode;/**< This is the mode in which the CSL instance
                             is opened */
  CSL_Uid uid;/**< This is a unique identifier to the instance of GPIO 
                   being referred to by this object */
  CSL_Xio xio;/**< This is the variable that holds the current state
                   of a resource being shared by current instance of
                   GPIO with other peripherals */
  CSL_GpioRegsOvly regs;/**< This is a pointer to the registers of the  
                                instance of GPIO referred to by this object */
  CSL_GpioNum gpioNum; /**< This is the instance of GPIO being referred to
                            by this object */
  Uint32 numPins; /**< This is the maximum number of pins supported by this 
                       instance of GPIO */
  Uint32 numBanks;/**< This is the maximum number of banks supported by this 
                       instance of GPIO */
} CSL_GpioObj;

/** \brief this is a pointer to @a CSL_GpioObj and is passed as the first
* parameter to all GPIO CSL APIs */
typedef CSL_GpioObj *CSL_GpioHandle;
/**
@} */


/*****************************************************************************\
          CSL3.x mandatory function prototype definitions
\*****************************************************************************/

/**
\defgroup CSL_GPIO_INIT_API GPIO Init API
\ingroup CSL_GPIO_FUNCTION
    \brief Initializes the GPIO
@{*/
CSL_Status CSL_gpioInit ( void );
/**
@} */  


/**
\defgroup CSL_GPIO_OPEN_API GPIO Open API
\ingroup CSL_GPIO_FUNCTION
    \brief Opens if possible the instance of GPIO requested
    \n  Reserves the specified gpio for use. The device can be 
    re-opened anytime after it has been normally closed if so
    required. The handle returned by this call is input as an
    essential argument for rest of the APIs described for this
    module.

    <b> Usage Constraints: </b>
    The CSL system as well as GPIO must be succesfully initialized
    via @a CSL_sysInit() and @a CSL_gpioInit() before calling this
    function. Memory for the @a CSL_GpioObj must be allocated outside
    this call. This object must be retained while usage of this peripheral.
  
    \b Example:
    \verbatim


    CSL_GpioObj     gpioObj;
    CSL_GpioHwSetup gpioSetup;
    CSL_Status       status;
    ...
    hGpio = CSL_gpioOpen(&gpioobj,
                         CSL_GPIO_0,
                         CSL_EXCLUSIVE,
                         &gpioSetup,
                         &status);
    \endverbatim
  
@{*/
CSL_GpioHandle CSL_gpioOpen ( CSL_GpioObj * hGpioObj,
                              CSL_GpioNum gpioNum,
                              CSL_OpenMode openMode,
                              CSL_GpioHwSetup * hwSetup, 
                              CSL_Status * status );
/**
@} */  

/**
\defgroup CSL_GPIO_CLOSE_API GPIO Close API
\ingroup CSL_GPIO_FUNCTION
    \brief Closes the instance of GPIO requested
    
@{*/                            
CSL_Status CSL_gpioClose ( CSL_GpioHandle hGpio );
/**
@} */

/**
\defgroup CSL_GPIO_SETUP_API GPIO Setup API
\ingroup CSL_GPIO_FUNCTION
    \brief Programs the GPIO with the setup configuration as specified in
           the arguments, now just a place holder
   		  
	   
@{*/
CSL_Status CSL_gpioHwSetup ( CSL_GpioHandle hGpio, 
                             CSL_GpioHwSetup *setup );
/**
@} */

/**
\defgroup CSL_GPIO_GETSETUP_API GPIO GetSetup API
\ingroup CSL_GPIO_FUNCTION
    \brief Reads the GPIO setup configuration, now just a place holder   		  
	   
@{*/
CSL_Status CSL_gpioGetHwSetup ( CSL_GpioHandle hGpio,
                                CSL_GpioHwSetup *setup );
/**
@} */


/**
\defgroup CSL_GPIO_CONTROL_API GPIO Control API
\ingroup CSL_GPIO_FUNCTION
    \brief Controls the different operations that can be performed by GPIO 
 		  
     		  
    
@{*/
CSL_Status CSL_gpioHwControl ( CSL_GpioHandle hGpio,
                               CSL_GpioHwControlCmd cmd, 
                               void *arg );

/**
@} */                             


/**
\defgroup CSL_GPIO_QUERY_API GPIO Query API
\ingroup CSL_GPIO_FUNCTION
    \brief Returns the status of the requested operation on GPIO
@{*/
CSL_Status CSL_gpioGetHwStatus( CSL_GpioHandle hGpio,
                                CSL_GpioHwStatusQuery query, 
                                void *response);
/**
@} */

#ifdef __cplusplus
}
#endif

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:01 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:04    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
