/** @mainpage MS CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the MS module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given MS module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular MS Module. The CSL developer should use his discretion designing
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

/** @file csl_ms.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 3/6/2004
 *    - reason: Created the sources
 *
 *
 * @date 3rd June, 2004
 * @author Santosh Narayanan.
 *
 */
#ifndef _CSL_MS_H_
#define _CSL_MS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_ms.h>

/**************************************************************************\
* MS global macro declarations
\**************************************************************************/

/** Constants for passing parameters to the MS Hardware setup function.
 */

/** For not resetting the MS device           */
#define CSL_MS_NO_RESET                       (0)
/** For  resetting the MS          */
#define CSL_MS_RESET                          (1)
/** For setting the parallel access interface mode for MS      */
#define CSL_MS_ACCESS_PARALLEL                (0)
/** For setting the serial access interface mode for MS */
#define CSL_MS_ACCESS_SERIAL                  (1)
/** For enabling the CRC */
#define CSL_MS_CRC_ENABLE                     (0)
/** For disabling the CRC */
#define CSL_MS_CRC_DISABLE                    (1)
/** For not clearing the Interrupt to Host */
#define CSL_MS_INTCLR_DISABLE                 (0)
/** For clearing the Interrupt to Host */
#define CSL_MS_INTCLR_ENABLE                  (1)
/** For disabling the interrupt to Host */
#define CSL_MS_INT_HOST_DISABLE               (0)
/** For enabling the interrupt to Host */
#define CSL_MS_INT_HOST_ENABLE                (1)
/** For disabling the interrupt to MS device */
#define CSL_MS_INT_DEVICE_DISABLE             (0)
/** For enabling the interrupt to MS device */
#define CSL_MS_INT_DEVICE_ENABLE              (1)
/** For not clearing the FIFO */
#define CSL_MS_FIFO_CLEAR_DISABLE             (0)
/** For clearing the FIFO */
#define CSL_MS_FIFO_CLEAR_ENABLE              (1)
/** For setting the FIFO direction as MS to FIFO to CPU */
#define CSL_MS_FIFO_DIR_READ                  (0)
/** For setting the FIFO direction as FIFO to CPU to MS */
#define CSL_MS_FIFO_DIR_WRITE                 (1)
/** For disabling DMA */
#define CSL_MS_DMA_DISABLE                    (0)
/** For enabling DMA */
#define CSL_MS_DMA_ENABLE                     (1)
/** For disabling DRM */
#define CSL_MS_DRM_DISABLE                    (0)
/** For enabling DRM */
#define CSL_MS_DRM_ENABLE                     (1)
/** For disabling Interrupt when Data request occurs */
#define CSL_MS_DRQSL_DISABLE                  (0)
/** For enabling Interrupt when Data request occurs */
#define CSL_MS_DRQSL_ENABLE                   (1)
/** For disabling the Rising Edge input */
#define CSL_MS_REI_DISABLE                    (0)
/** For enabling the Rising Edge input */
#define CSL_MS_REI_ENABLE                     (1)
/** For disabling the Rising Edge output */
#define CSL_MS_REO_DISABLE                    (0)
/** For enabling the Rising Edge output */
#define CSL_MS_REO_ENABLE                     (1)

/** Constants for passing parameters to the MS Status Query function.
 */


/** For indicating the absence of a Data Request Interrupt */
#define CSL_MS_NO_DRQ                         (0)
/** For indicating the presence of a Data Request Interrupt */
#define CSL_MS_DRQ                            (1)
/** For indicating that no interrupt was received from MS */
#define CSL_MS_NO_MSINT                       (0)
/** For indicating the presence of an  interrupt received from MS */
#define CSL_MS_MSINT                          (1)
/** For indicating command receive disabled */
#define CSL_MS_RDY_DISABLE                    (0)
/** For indicating command receive enabled or protocol ended */
#define CSL_MS_RDY_ENABLE                     (1)
/** For indicating no CRC error */
#define CSL_MS_NO_CRC                         (0)
/** For indicating a CRC error */
#define CSL_MS_CRC                            (1)
/** For indicating no Time-out error */
#define CSL_MS_NO_TOE                         (0)
/** For indicating Time-out error */
#define CSL_MS_TOE                            (1)
/** For indicating that the FIFO is not empty */
#define CSL_MS_FIFO_NOT_EMPTY                 (0)
/** For indicating that the FIFO is empty */
#define CSL_MS_FIFO_EMPTY                     (1)
/** For indicating that the FIFO is not full */
#define CSL_MS_FIFO_NOT_FULL                  (0)
/** For indicating that the FIFO is full */
#define CSL_MS_FIFO_FULL                      (1)
/** For indicating  No MS Command End */
#define CSL_MS_NO_CED                         (0)
/** For indicating an MS command End  */
#define CSL_MS_CED                            (1)
/** For indicating No Memory Stick errors */
#define CSL_MS_NO_ERR                         (0)
/** For indicating Memory Stick error */
#define CSL_MS_ERR                            (1)
/** For indicating No data buffer request */
#define CSL_MS_NO_BRQ                         (0)
/** For indicating a MS data buffer request */
#define CSL_MS_BRQ                            (1)
/** For indicating a command acknowledge from MS */
#define CSL_MS_ACK                            (0)
/** For indicating a command no acknowledge from MS */
#define CSL_MS_NO_ACK                         (1)


/**************************************************************************\
* MS global typedef declarations
\**************************************************************************/

/** @brief This object contains the reference to the instance of MS opened
 *  using the @a CSL_msOpen().
 *
 *  The pointer to this, is passed to all MS CSL APIs.
 */
typedef struct CSL_MsObj {
        /** This is the mode which the CSL instance is opened     */
        CSL_OpenMode openMode;
        /** This is a unique identifier to the instance of MS being
         *  referred to by this object
         */
        CSL_Uid uid;
        /** This is the variable that contains the current state of a
         *  resource being shared by current instance of MS with
         *  other peripherals
         */
        CSL_Xio xio;
        /** This is a pointer to the registers of the instance of MS
         *  referred to by this object
         */
        CSL_MsRegsOvly regs;
        /** This is the instance of MS being referred to by this object  */
        CSL_MsNum perNum;
}CSL_MsObj;

typedef struct CSL_MsObj *CSL_MsHandle;


/** @brief This has all the fields required to configure MS at Power Up
 *  (After a Hardware Reset) or a Soft Reset
 *
 *  This structure is used to setup or obtain existing setup of
 *  MS using @a CSL_msHwSetup() & @a CSL_msGetHwSetup() functions
 *  respectively.
 */
typedef struct CSL_MsHwSetup_ {
        /** Reset mode : 0==> No sync reset is performed, 1==> Reset performed */
        Uint16 reset;   
        /** Access Mode : 0==> Parallel Interface Mode, 1==> Serial Interface Mode  */
        Uint16 mode;
        /** CRC Mode: 0==> CRC output on,  1 ==> CRC output off */
        Uint16 crc;
        /** Interrupt Clear: 0==> No clear, 1==> Clear interrupt   */
        Uint16 intClear;
        /** Interrupt output to HOST : 0 ==> Disable, 1 ==> Enable */
        Uint16 intHost;
        /** Interrupt from MS device : 0 ==> Disable, 1 ==> Enable */
        Uint16 intDevice;
        /** FIFO clear : 0 ==> Disable, 1 ==> Enable	 */
        Uint16 fifoClear;
        /** FIFO direction: 0 ==> Disable, 1 ==> Enable	 */
        Uint16 fifoDir;
        /** DMA Access Mode: 0 ==> Disable, 1 ==> Enable	 */
        Uint16 dmaMode;
        /**  DMA Data Request Mode: 0 ==> Level Mode, 1==> Edge Mode */
        Uint16 drm;
        /** Data Request:  0==>No interrupt with Data request, 1==>Interrupt with Data request */
        Uint16 drqInt;
        /** Rise Edge Input: 0==>Falling Edge Input, 1==>Rising Edge Input */
        Uint16 rei;
        /** Rise Edge Output: 0==>Falling Edge Output, 1==>Rising Edge Output */
        Uint16 reo;
        /** Busy Count */
        Uint16 bsyCount;
}CSL_MsHwSetup;


/** @brief Enumeration for queries passed to @a CSL_msGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of MS.
 */
typedef enum {
      /** Get the Data Request Interrupt Status Flag (response type: @a Uint16 *)        */
      CSL_MS_QUERY_DRQ_STATUS = 1,
      /** Get the Memory Stick interface interrupt status information (response type: @a Uint16 *)        */
      CSL_MS_QUERY_MSINT_STATUS ,
      /** Get the Command receive status information(response type: @a Uint16 *)    */
      CSL_MS_QUERY_RDY ,
      /** Get CRC status information(response type: @a Uint16 *)   */
      CSL_MS_QUERY_CRC_STATUS ,
      /** Get Time-out error status information(response type: @a Uint16 *)   */
      CSL_MS_QUERY_TOE_STATUS,
      /** Check whether the FIFO is empty or has data (response type: @a Uint16 *)   */
      CSL_MS_QUERY_FIFO_EMPTY,
      /** Check whether the FIFO is full or has empty spaces (response type: @a Uint16 *)   */
      CSL_MS_QUERY_FIFO_FULL,
      /** Check whether the MS command has ended  or not (response type: @a Uint16 *)   */
      CSL_MS_QUERY_CED_STATUS,
      /** Check whether any MS errors have been set (response type: @a Uint16 *)   */
      CSL_MS_QUERY_ERR_STATUS,
      /** Check whether any MS data buffer request is indicated (response type: @a Uint16 *)   */
      CSL_MS_QUERY_BRQ_STATUS,
      /** Check whether any command acknowledge from MS is indicated (response type: @a Uint16 *)   */
      CSL_MS_QUERY_ACK_STATUS,
      /** Check whether the MS transfers data from FIFO or external memory (response type: @a Uint16 *)   */
      CSL_MS_QUERY_DSL,
      /** Get the data transfer size (response type: @a Uint16 *)   */
      CSL_MS_QUERY_DSIZE
} CSL_MsHwStatusQuery;


/** @brief Enumeration for queries passed to @a CSL_msHwControl()
 *
 * This is used to select the commands to control the operations
 * existing setup of MS. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration.
 */
 typedef enum {
      /** Set the Transfer Protocol Command : argument @a (Uint16 *)	 */
      CSL_MS_CMD_TPC_SET = 1,
      /** Set the other endpoint of  data transfer from/to MS as FIFO : no argument 	 */
      CSL_MS_CMD_DSL_FIFO ,
      /** Set the other endpoint of  data transfer from/to MS as FIFO : no argument 	 */
      CSL_MS_CMD_DSL_EXTMEM ,
      /** Reset the MS : no argument	 */
      CSL_MS_CMD_RESET ,
      /** Set the data transfer size : argument @a (Uint16 *)	 */
      CSL_MS_CMD_SET_DATA_SIZE ,
      /** Write a word to FIFO :  argument @a (Uint32 *)	 */
      CSL_MS_CMD_WRITE_FIFO ,
      /** Read a word from FIFO :  argument @a (Uint32 *)	 */
      CSL_MS_CMD_READ_FIFO,
      /** Enable MS interrupts : no argument */
      CSL_MS_CMD_INT_ENABLE,
      /** Disable MS interrupts : no argument */
      CSL_MS_CMD_INT_DISABLE,
      /** Enable the DMA mode */
      CSL_MS_CMD_DMA_ENABLE,
      /** Disable the DMA mode */
      CSL_MS_CMD_DMA_DISABLE
	
} CSL_MsHwControlCmd;



/**************************************************************************\
* MS global function declarations
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
   if (CSL_SOK != CSL_msInit()) {
       return;
   }
   @endverbatim
 *
 * @return returns the status of the operation
 *
 */
CSL_Status  CSL_msInit(
    void
);


/** The open call sets up the data structures for the particular instance of
 *  MS device. The device can be re-opened anytime after it has been normally
 *  closed if so required. MS Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as MS must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_msInit() before calling this
 *  function. Memory for the @a CSL_msObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

	  CSL_MsObj     msObj;
	  CSL_MsHwSetup msSetup;
	  CSL_Status       status;
 		...
	  hMs = CSL_msOpen(&msObj,
                          CSL_MS_0,
                          CSL_EXCLUSIVE,
                          &msSetup,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_MsHandle to the requested instance of
 * MS if the call is successful, otherwise, a @a NULL is returned.
 *
 */
CSL_MsHandle CSL_msOpen (
     /** Pointer to the object that holds reference to the
      *  instance of MS requested after the call
      */
     CSL_MsObj              *hMsObj,
     /** Instance of MS to which a handle is requested
      */
     CSL_MsNum              msNum,
     /** Specifies if MS should be opened with exclusive or
      *  shared access to the associate pins
      */
     CSL_OpenMode            openMode,
     /** If a valid structure is passed (not @a NULL), then
      *  the @a CSL_msHwSetup() is called with this parameter
      */
     CSL_MsHwSetup          *hwSetup,
     /** This returns the status (success/errors) of the call
      */
     CSL_Status              *status
);

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_msInit() and @a CSL_msOpen() must be called successfully
 * in that order before @a CSL_msClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_MsHandle hMs;
   ...
   CSL_msClose(hMs);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_msClose(
    /** Pointer to the object that holds reference to the
     *  instance of MS requested after the call
     */
    CSL_MsHandle                         hMs
);


/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_MsHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_msInit() and @a CSL_msOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_MsHandle hMs;
     CSL_MsHwSetup hwSetup = CSL_MS_HWSETUP_DEFAULTS;
     CSL_msHwSetup(hMs, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_msHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of MS requested after the call
     */
    CSL_MsHandle                         hMs,
    /** Pointer to setup structure which contains the
     *  information to program MS to a useful state
     */
    CSL_MsHwSetup                        *setup
);

/** This function gets the current setup of the I2C. The status is
 *  returned through @a CSL_I2cHwSetup. The obtaining of status
 *  is the reverse operation of @a CSL_i2cHwSetup() function.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_msInit() and @a CSL_msOpen() must be called successfully
 *  in that order before @a CSL_msGetHwSetup() can be called.
 *
 * @b Example:
 * @verbatim
	  CSL_MsHandle hMs;
	  CSL_Status status;
	  CSL_MsHwSetup *mysetup;
	   ...
	  status = CSL_msGetHwSetup(hMs, &mysetup);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_msGetHwSetup(
   /** Pointer to the object that holds reference to the
    *  instance of MS requested after the call
    */
   CSL_MsHandle                         hMs,
   /** Pointer to setup structure which contains the
    *  information to program MS to a useful state
    */
   CSL_MsHwSetup                        *setup
);


/** Control operations for the MS.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_MsHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_msInit() and @a CSL_msOpen() must be called successfully
 *  in that order before @a CSL_msHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_MsHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_MsHandle hMs;
       CSL_Status status;
       ...
       status = CSL_msHwControl(hMs,
                                    CSL_MS_CMD_START,
                                   &command);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_msHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of MS requested after the call
     */
    CSL_MsHandle                         hMs,
    /** The command to this API indicates the action to be taken
     */
    CSL_MsHwControlCmd                   cmd,
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
 *  refer to @a CSL_MsHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_msInit() and @a CSL_msOpen() must be called successfully
 *  in that order before @a CSL_msGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_MsHwStatusQuery
 *
 * @b Example:
 * @verbatim
	  CSL_MsHandle hMs;
	  CSL_Status status;
	  Uint16  *response;
	   ...
	  status = CSL_msGetHwStatus(hMs,
                           CSL_MS_QUERY_BUS_BUSY,
                                  &response);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_msGetHwStatus(
   /** Pointer to the object that holds reference to the
    *  instance of MS requested after the call
    */
   CSL_MsHandle                         hMs,
   /** The query to this API which indicates the status
    *  to be returned
    */
   CSL_MsHwStatusQuery                  query,
   /** Placeholder to return the status. @a void* casted */
   void                                 *response
);

#ifdef __cplusplus
}
#endif


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:05 9         2272             xkeshavm      */
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
