/** @mainpage VPBE CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the VPBE module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given VPBE module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular VPBE Module. The CSL developer should use his discretion designing
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

/** @file csl_vpbe.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 10/11/2004
 *    - reason: Created the sources
 *
 *
 * @date 11th October, 2004
 * @author Santosh Narayanan.
 *
 */
#ifndef _CSL_VPBE_H_
#define _CSL_VPBE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_vpbe.h>


/**************************************************************************\
* VPBE global typedef declarations
\**************************************************************************/

/** @brief This object contains the reference to the instance of VPBE opened
 *  using the @a CSL_vpbeOpen().
 *
 *  The pointer to this, is passed to all VPBE CSL APIs.
 */
typedef struct CSL_VpbeObj {
        /** This is the mode which the CSL instance is opened     */
        CSL_OpenMode openMode;
        /** This is a unique identifier to the instance of VPBE being
         *  referred to by this object
         */
        CSL_Uid uid;
        /** This is the variable that contains the current state of a
         *  resource being shared by current instance of VPBE with
         *  other peripherals
         */
        CSL_Xio xio;
        /** This is a pointer to the registers of the instance of VPBE
         *  referred to by this object
         */  
      CSL_VpbeRegsOvly regs;  
      /** This is the instance of VPBE being referred to by this object  */
      CSL_VpbeNum perNum;
}CSL_VpbeObj;

typedef struct CSL_VpbeObj *CSL_VpbeHandle;

/** @brief This has all the fields required to configure VPBE at Power Up
 *  (After a Hardware Reset) or a Soft Reset
 *
 *  This structure is used to setup or obtain existing setup of
 *  VPBE using @a CSL_vpbeHwSetup() function.
 */
 
typedef struct CSL_VpbeHwSetup_  CSL_VpbeHwSetup;

/** @brief Enumeration for queries passed to @a CSL_vpbeGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of VPBE.
 */
typedef enum {
    /** Get the Peripheral ID, Class and Rev, number (response type: @a CSL_VpbeClassRevStatus *)        */
    CSL_VPBE_QUERY_CLASS_REV_STATUS = 1
} CSL_VpbeHwStatusQuery;

/** @brief VPBE Periphreal ID, Class and Revision number structure
*/

typedef struct CSL_VpbeClassRevStatus_{
    /** Peripheral identification */
    Uint16 peripheralID;
    /** Class information */
    Uint16 classID;
    /** Revision number */
    Uint16 revNum;
}CSL_VpbeClassRevStatus;
	

/** @brief Enumeration for queries passed to @a CSL_vpbeHwControl()
 *
 * This is used to select the commands to control the operations
 * existing setup of VPBE. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration.
 */
 typedef enum {
      /** Full clock rate select : no argument 	 */
      CSL_VPBE_CMD_CLKSEL_FULL ,
      /** Half clock rate select : no argument 	 */
      CSL_VPBE_CMD_CLKSEL_HALF 
} CSL_VpbeHwControlCmd;



/**************************************************************************\
* VPBE global function declarations
\**************************************************************************/

/**  This function is idempotent in that calling it many times is same as
 *   calling it once. This function initializes the MS CSL data structures.
 *
 * <b> Usage Constraints: </b>
 * CSL system initialization must be successfully completed by invoking
 * @a CSL_sysInit() before calling this function. This function should be
 * called before using any of the CSL APIs
 *
 * @b Example:
 * @verbatim


   ...
   CSL_sysInit();
   if (CSL_SOK != CSL_vpbeInit()) {
       return;
   }
   @endverbatim
 *
 * @return returns the status of the operation
 *
 */
CSL_Status  CSL_vpbeInit(
    void
);


/** The open call sets up the data structures for the particular instance of
 *  VPBE device. The device can be re-opened anytime after it has been normally
 *  closed if so required. VPBE Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as VPBE must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_vpbeInit() before calling this
 *  function. Memory for the @a CSL_vpbeObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

	  CSL_VpbeObj     vpbeObj;
	  CSL_Status       status;
 		...
	  hVpbe = CSL_vpbeOpen(&vpbeObj,
                          CSL_VPBE_0,
                          CSL_EXCLUSIVE,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_VpbeHandle to the requested instance of
 * VPBE if the call is successful, otherwise, a @a NULL is returned.
 *
 */
CSL_VpbeHandle CSL_vpbeOpen (
     /** Pointer to the object that holds reference to the
      *  instance of VPBE requested after the call
      */
     CSL_VpbeObj              *hVpbeObj,
     /** Instance of VPBE to which a handle is requested
      */
     CSL_VpbeNum              vpbeNum,
     /** Specifies if VPBE should be opened with exclusive or
      *  shared access to the associate pins
      */
     CSL_OpenMode            openMode,
     /** This returns the status (success/errors) of the call
      */
     CSL_Status              *status
);

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_vpbeInit() and @a CSL_vpbeOpen() must be called successfully
 * in that order before @a CSL_vpbeClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_VpbeHandle hVpbe;
   ...
   CSL_vpbeClose(hVpbe);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vpbeClose(
    /** Pointer to the object that holds reference to the
     *  instance of VPBE requested after the call
     */
    CSL_VpbeHandle                         hVpbe
);


/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_MsHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vpbeInit() and @a CSL_vpbeOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_vpbeHandle hVpbe;
     CSL_VpbeHwSetup hwSetup = CSL_VPBE_HWSETUP_DEFAULTS;
     CSL_vpbeHwSetup(hVpbe, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vpbeHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VPBE requested after the call
     */
    CSL_VpbeHandle                         hVpbe,
    /** Pointer to setup structure which contains the
     *  information to program VPBE to a useful state
     */
    CSL_VpbeHwSetup                        *setup
);


/** Control operations for the VPBE.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_VpbeHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vpbeInit() and @a CSL_vpbeOpen() must be called successfully
 *  in that order before @a CSL_vpbeHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_VpbeHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_VpbeHandle hVpbe;
       CSL_Status status;
       ...
       status = CSL_vpbeHwControl(hVpbe,
                                    CSL_VPBE_CMD_ENABLE,
                                   &command);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vpbeHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of MS requested after the call
     */
    CSL_VpbeHandle                         hVpbe,
    /** The command to this API indicates the action to be taken
     */
    CSL_VpbeHwControlCmd                   cmd,
    /** An optional argument @a void* casted
     */
    void                                 *arg
);

/** This function is used to read the current device configuration, status flags
 *  and the value present associated registers. Following table details the various
 *  status queries supported and the associated data structure to record the response.
 *  User should allocate memory for the said data type and pass its pointer as an
 *  unadorned void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the response,
 *  refer to @a CSL_VpbeHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vpbeInit() and @a CSL_vpbeOpen() must be called successfully
 *  in that order before @a CSL_vpbeGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_VpbeHwStatusQuery
 *
 * @b Example:
 * @verbatim
	  CSL_VpbeHandle hVpbe;
	  CSL_Status status;
	  Uint16  *response;
	   ...
	  status = CSL_vpbeGetHwStatus(hVpbe,
                           CSL_VPBE_QUERY_BUS_BUSY,
                                  &response);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vpbeGetHwStatus(
   /** Pointer to the object that holds reference to the
    *  instance of VPBE requested after the call
    */
   CSL_VpbeHandle                         hVpbe,
   /** The query to this API which indicates the status
    *  to be returned
    */
   CSL_VpbeHwStatusQuery                  query,
   /** Placeholder to return the status. @a void* casted */
   void                                 *response
);

#ifdef __cplusplus
}
#endif


#endif


