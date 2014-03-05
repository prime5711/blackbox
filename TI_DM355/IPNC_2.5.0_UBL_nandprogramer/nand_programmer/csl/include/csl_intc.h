#ifndef _CSL_INTC_H_
#define _CSL_INTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <_csl_intc0.h>
#include <_csl_intc1.h>

/* Interrupt/Exception Counts */
#define _CSL_INTC_EVENTID__INTC0CNT     (8)      /* ARM exception count */
#define _CSL_INTC_EVENTID__INTC1CNT     (64)     /* Level-1 Interrupt count */

/**
 * @brief   Count of the number of interrupt-events
 */
#define CSL_INTC_EVENTID_CNT        \
    (_CSL_INTC_EVENTID__INTC0CNT + _CSL_INTC_EVENTID__INTC1CNT)

/**
 * @brief   Indicates there is no associated event-handler
 */
#define CSL_INTC_EVTHANDLER_NONE        ((CSL_IntcEventHandlerRecord *) 0)

/**
 * @brief   Invalid handle
 */
#define CSL_INTC_BADHANDLE        (0)


/**
 * @brief   Interrupt Vector IDs
 */
typedef enum {
    CSL_INTC_VECTID_DEFAULT   =   0,  /**< default vector */
    CSL_INTC_VECTID_INVALID   =  -1,  /**< invalid vector */

/* for use only with HookIsr -- by design! */
    CSL_INTC_VECTID_RESET     =   1,  /**< the RESET exception vector */
    CSL_INTC_VECTID_UNDEF     =   2,  /**< the UNDEF exception vector */
    CSL_INTC_VECTID_SWI       =   3,  /**< the SWI exception vector */
    CSL_INTC_VECTID_PREABT    =   4,  /**< the PREABT exception vector */
    CSL_INTC_VECTID_DATABT    =   5,  /**< the DATABT exception vector */
    CSL_INTC_VECTID_IRQ       =   7,  /**< the IRQ exception vector */
    CSL_INTC_VECTID_FIQ       =   8   /**< the FIQ exception vector */

} CSL_IntcVectId;



/**
 * @brief   Interrupt Event IDs
 */
typedef enum {

    _CSL_INTC_EVENTID__SPURIOUS         =   0,
    _CSL_INTC_EVENTID__INTC1START       =   1,

    CSL_INTC_EVENTID_VPSSINT0           =   _CSL_INTC_EVENTID__INTC1START + 0,  /**< VPSS - CCDC */
    CSL_INTC_EVENTID_VPSSINT1           =   _CSL_INTC_EVENTID__INTC1START + 1,  /**< VPSS - CCDC */
    CSL_INTC_EVENTID_VPSSINT2           =   _CSL_INTC_EVENTID__INTC1START + 2,  /**< VPSS - CCDC */
    CSL_INTC_EVENTID_VPSSINT3          =   _CSL_INTC_EVENTID__INTC1START + 3,  /**< VPSS - Histogram */
    CSL_INTC_EVENTID_VPSSINT4           =   _CSL_INTC_EVENTID__INTC1START + 4,  /**< VPSS - AE/AWB/AF */
    CSL_INTC_EVENTID_VPSSINT5          =   _CSL_INTC_EVENTID__INTC1START + 5,  /**< VPSS - Previewer */
    CSL_INTC_EVENTID_VPSSINT6           =   _CSL_INTC_EVENTID__INTC1START + 6,  /**< VPSS - Resizer */
    CSL_INTC_EVENTID_VPSSINT7          =   _CSL_INTC_EVENTID__INTC1START + 7,  /**< VPSS - Focus */
    CSL_INTC_EVENTID_VPSSINT8          =   _CSL_INTC_EVENTID__INTC1START + 8,  /**< VPSS - VPBE */
    CSL_INTC_EVENTID_ASQINT           =   _CSL_INTC_EVENTID__INTC1START + 9,  /**< IMCOP - Sqr */
    CSL_INTC_EVENTID_IMXINT           =   _CSL_INTC_EVENTID__INTC1START + 10, /**< IMCOP - iMX */
    CSL_INTC_EVENTID_IMCOPINT          =   _CSL_INTC_EVENTID__INTC1START + 11, /**< IMCOP - VLCD */
    CSL_INTC_EVENTID_USBINT           =   _CSL_INTC_EVENTID__INTC1START + 12, /**< USB OTG Collector */
    CSL_INTC_EVENTID_RTOINT          =   _CSL_INTC_EVENTID__INTC1START + 13, /**< RTO or Timer2-TINT12 */
    CSL_INTC_EVENTID_UARTINT2          =   _CSL_INTC_EVENTID__INTC1START + 14, /**< UART2 or Timer2-TINT34 */
    CSL_INTC_EVENTID_TINT6           =   _CSL_INTC_EVENTID__INTC1START + 15, /**< Timer3 -TINT12 */
    CSL_INTC_EVENTID_CCINT0           =   _CSL_INTC_EVENTID__INTC1START + 16, /**< 3PCC Region 0 */
    CSL_INTC_EVENTID_CCERRINT         =   _CSL_INTC_EVENTID__INTC1START + 17, /**< SPI1 or 3PCC Error */
    CSL_INTC_EVENTID_TCERRINT0        =   _CSL_INTC_EVENTID__INTC1START + 18, /**< SPI1 or 3PTC0 Error */
    CSL_INTC_EVENTID_TCERRINT1        =   _CSL_INTC_EVENTID__INTC1START + 19, /**< SPI2 or 3PTC1 Error */
    CSL_INTC_EVENTID_PSCINT          =   _CSL_INTC_EVENTID__INTC1START + 20, /**< PSC-ALLINT */
    CSL_INTC_EVENTID_SPINT2           =   _CSL_INTC_EVENTID__INTC1START + 21, /**< SPI2 */
    CSL_INTC_EVENTID_TINT7           =   _CSL_INTC_EVENTID__INTC1START + 22, /**< Timer3-TINT34*/
    CSL_INTC_EVENTID_MMCINT1           =   _CSL_INTC_EVENTID__INTC1START + 23, /**< MMC/SD1*/
    CSL_INTC_EVENTID_MBXINT           =   _CSL_INTC_EVENTID__INTC1START + 24, /**< McBSP or Audio Codec */
    CSL_INTC_EVENTID_MBRINT           =   _CSL_INTC_EVENTID__INTC1START + 25, /**< McBSP or Audio Codec*/
    CSL_INTC_EVENTID_MMCINT0           =   _CSL_INTC_EVENTID__INTC1START + 26, /**< MMC/SD or Memory Stick*/
    CSL_INTC_EVENTID_SDIOINT0          =   _CSL_INTC_EVENTID__INTC1START + 27, /**< MMC/SD0 */
    CSL_INTC_EVENTID_PWMINT3            =   _CSL_INTC_EVENTID__INTC1START + 28, /**< PWM3 */
    CSL_INTC_EVENTID_DDRINT           =   _CSL_INTC_EVENTID__INTC1START + 29, /**< DDR EMIF */
    CSL_INTC_EVENTID_AEMIFINT         =   _CSL_INTC_EVENTID__INTC1START + 30, /**< Async EMIF */
    CSL_INTC_EVENTID_SDIOINT1           =   _CSL_INTC_EVENTID__INTC1START + 31, /**<MMC/SD1 */
    CSL_INTC_EVENTID_TINT0            =   _CSL_INTC_EVENTID__INTC1START + 32, /**< Timer 0 - TINT12 */
    CSL_INTC_EVENTID_TINT1            =   _CSL_INTC_EVENTID__INTC1START + 33, /**< Timer 0 - TINT34 */
    CSL_INTC_EVENTID_TINT2            =   _CSL_INTC_EVENTID__INTC1START + 34, /**< Timer 1 - TINT12 */
    CSL_INTC_EVENTID_TINT3            =   _CSL_INTC_EVENTID__INTC1START + 35, /**< Timer 2 - TINT34 */
    CSL_INTC_EVENTID_PWMINT0          =   _CSL_INTC_EVENTID__INTC1START + 36, /**< PWM0 */
    CSL_INTC_EVENTID_PWMINT1          =   _CSL_INTC_EVENTID__INTC1START + 37, /**< PWM1 */
    CSL_INTC_EVENTID_PWMINT2          =   _CSL_INTC_EVENTID__INTC1START + 38, /**< PWM2 */
    CSL_INTC_EVENTID_IICINT           =   _CSL_INTC_EVENTID__INTC1START + 39, /**< I2C */
    CSL_INTC_EVENTID_UARTINT0         =   _CSL_INTC_EVENTID__INTC1START + 40, /**< UART0 */
    CSL_INTC_EVENTID_UARTINT1         =   _CSL_INTC_EVENTID__INTC1START + 41, /**< UART1 */
    CSL_INTC_EVENTID_SPINT0         =   _CSL_INTC_EVENTID__INTC1START + 42, /**< SPI0 */
    CSL_INTC_EVENTID_SPINT1           =   _CSL_INTC_EVENTID__INTC1START + 43, /**< SPI0 */
    CSL_INTC_EVENTID_GPIO0           =   _CSL_INTC_EVENTID__INTC1START + 44, /**< GPIO */
    CSL_INTC_EVENTID_GPIO1           =   _CSL_INTC_EVENTID__INTC1START + 45, /**< GPIO */
    CSL_INTC_EVENTID_GPIO2         =   _CSL_INTC_EVENTID__INTC1START + 46, /**< GPIO */
    CSL_INTC_EVENTID_GPIO3         =   _CSL_INTC_EVENTID__INTC1START + 47, /**< GPIO */
    CSL_INTC_EVENTID_GPIO4            =   _CSL_INTC_EVENTID__INTC1START + 48, /**< GPIO */
    CSL_INTC_EVENTID_GPIO5            =   _CSL_INTC_EVENTID__INTC1START + 49, /**< GPIO */
    CSL_INTC_EVENTID_GPIO6            =   _CSL_INTC_EVENTID__INTC1START + 50, /**< GPIO */
    CSL_INTC_EVENTID_GPIO7            =   _CSL_INTC_EVENTID__INTC1START + 51, /**< GPIO */
    CSL_INTC_EVENTID_GPIO8            =   _CSL_INTC_EVENTID__INTC1START + 52, /**< GPIO */
    CSL_INTC_EVENTID_GPIO9            =   _CSL_INTC_EVENTID__INTC1START + 53, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK0            =   _CSL_INTC_EVENTID__INTC1START + 54, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK1            =   _CSL_INTC_EVENTID__INTC1START + 55, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK2         =   _CSL_INTC_EVENTID__INTC1START + 56, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK3         =   _CSL_INTC_EVENTID__INTC1START + 57, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK4         =   _CSL_INTC_EVENTID__INTC1START + 58, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK5         =   _CSL_INTC_EVENTID__INTC1START + 59, /**< GPIO */
    CSL_INTC_EVENTID_GPIOBNK6         =   _CSL_INTC_EVENTID__INTC1START + 60, /**< GPIO */
    CSL_INTC_EVENTID_COMMTX           =   _CSL_INTC_EVENTID__INTC1START + 61, /**< ARMSS */
    CSL_INTC_EVENTID_COMMRX           =   _CSL_INTC_EVENTID__INTC1START + 62, /**< ARMSS */
    CSL_INTC_EVENTID_EMUINT           =   _CSL_INTC_EVENTID__INTC1START + 63, /**< E2ICE */

    _CSL_INTC_EVENTID__INTC1END       =   _CSL_INTC_EVENTID__INTC1START + _CSL_INTC_EVENTID__INTC1CNT - 1,


    _CSL_INTC_EVENTID__INTC0START       =   _CSL_INTC_EVENTID__INTC1END + 1,

    CSL_INTC_EVENTID_RESET              =   _CSL_INTC_EVENTID__INTC0START + 0,  /**< the RESET exception vector */
    CSL_INTC_EVENTID_UNDEF              =   _CSL_INTC_EVENTID__INTC0START + 1,  /**< the UNDEF exception vector */
    CSL_INTC_EVENTID_SWI                =   _CSL_INTC_EVENTID__INTC0START + 2,  /**< the SWI exception vector */
    CSL_INTC_EVENTID_PREABT             =   _CSL_INTC_EVENTID__INTC0START + 3,  /**< the PREABT exception vector */
    CSL_INTC_EVENTID_DATABT             =   _CSL_INTC_EVENTID__INTC0START + 4,  /**< the DATABT exception vector */
    CSL_INTC_EVENTID_IRQ                =   _CSL_INTC_EVENTID__INTC0START + 6,  /**< the IRQ exception vector */
    CSL_INTC_EVENTID_FIQ                =   _CSL_INTC_EVENTID__INTC0START + 7,  /**< the FIQ exception vector */

    _CSL_INTC_EVENTID__INTC0END         =   _CSL_INTC_EVENTID__INTC0START + _CSL_INTC_EVENTID__INTC0CNT - 1,

    CSL_INTC_EVENTID_INVALID            =   -1                                  /**< Invalid Event-ID */

} CSL_IntcEventId;


/**
 * @brief   Interrupt Priority
 */
typedef enum {
    
    CSL_INTC_PRIORITY_DEFAULT   = 0,  /**< Default priority */
    _CSL_INTC_PRIORITY__INTC1START     = 0,
    CSL_INTC_PRIORITY_0         = 0 ,  /**< Priority 0 (FIQ, Highest) */
    CSL_INTC_PRIORITY_1         = 1 ,  /**< Priority 1 (FIQ, Lowest) */
    CSL_INTC_PRIORITY_2         = 2 ,  /**< Priority 2 (IRQ, Highest) */
    CSL_INTC_PRIORITY_3         = 3 ,  /**< Priority 3 (IRQ) */
    CSL_INTC_PRIORITY_4         = 4 ,  /**< Priority 4 (IRQ) */
    CSL_INTC_PRIORITY_5         = 5 ,  /**< Priority 5 (IRQ) */
    CSL_INTC_PRIORITY_6         = 6 ,  /**< Priority 6 (IRQ) */
    CSL_INTC_PRIORITY_7         = 7 ,  /**< Priority 7 (IRQ, Lowest) */
    _CSL_INTC_PRIORITY__INTC1END       = 7,

    CSL_INTC_PRIORITY_INVALID    =   -1        /**< Invalid Priority */

} CSL_IntcPriority;


/**
 * @brief   Interrupt Type (Routing)
 */
typedef enum {
    CSL_INTC_TYPE_IRQ   = 0,    /**< IRQ -- normal interrupt request */
    CSL_INTC_TYPE_FIQ   = 1     /**< FIQ -- fast interrupt request */
} CSL_IntcType;


/**
 * @brief   Enumeration of the control commands
 *
 * These are the control commands that could be used with
 * CSL_intcHwControl(..). Some of the commands expect an
 * argument as documented along-side the description of
 * the command.
 */
typedef enum {
    CSL_INTC_CMD_EVTENABLE,
        /**<
         * @brief   Enables the event
         * @param   None
         */
    CSL_INTC_CMD_EVTDISABLE,
        /**<
         * @brief   Disables the event
         * @param   None
         */
    CSL_INTC_CMD_SETPRIORITY,
        /**<
         * @brief   Modifies the interrupt priority
         * @param   CSL_IntcPriority
         */
    CSL_INTC_CMD_SETPACE
        /**<
         * @brief   Not Supported
         * @param   Not Applicable
         */
} CSL_IntcHwControlCmd;


/**
 * @brief   Enumeration of the queries
 *
 * These are the queries that could be used with CSL_intcGetHwStatus(..).
 * The queries return a value through the object pointed to by the pointer
 * that it takes as an argument. The argument supported by the query is
 * documented along-side the description of the query.
 */
typedef enum {
    CSL_INTC_QUERY_PRIORITY,
        /**<
         * @brief   Retrieves the interrupt priority
         * @param   (CSL_IntcPriority *)
         */

    CSL_INTC_QUERY_TYPE,
        /**<
         * @brief   Returns the type (route)
         * @param   (CSL_IntcType *)
         */

    CSL_INTC_QUERY_ISEVENTPENDING
        /**<
         * @brief   Checks if event is pending
         * @param   (Bool *)
         */

} CSL_IntcHwStatusQuery;




/**
 * @brief   Event Handler pointer
 *
 * Event handlers ought to conform to this type
 */
typedef void (* CSL_IntcEventHandler)(void *);


/**
 * @brief   Event Handler Record
 *
 * Used to set-up the event-handler using CSL_intcPlugEventHandler(..)
 */
typedef struct CSL_IntcEventHandlerRecord {
    CSL_IntcEventHandler    handler;    /**< pointer to the event handler */
    void *                  arg;        /**< the argument to be passed to the
                                          handler when it is invoked */
    CSL_IntcEventId         evtId;      /**< event Id */
    Uint32                  dummy;      /**< dummy to fill 16 byte size entry */
    /* Note: This structure should be of size 16 bytes */
} CSL_IntcEventHandlerRecord;


/**
 * @brief   The setup-structure
 *
 * Used to configure the interrupt controller for an event using
 * CSL_intcHwSetup(..)
 */
typedef struct CSL_IntcHwSetup {
    CSL_IntcPriority    priority;   /**< The interrupt priority */
} CSL_IntcHwSetup;



/**
 * @brief   Default values for the setup-parameters
 */
#define CSL_INTC_HWSETUP_DEFAULTS {                 \
    (CSL_IntcPriority) CSL_INTC_PRIORITY_DEFAULT   \
}



/**
 * @brief   Event enable state
 */
typedef Uint8 CSL_IntcEventEnableState;


/**
 * @brief   Global Interrupt enable state
 */
typedef Uint8 CSL_IntcGlobalEnableState;


/**
 * @brief   The interrupt handle object
 *
 * This object is used refenced by the handle to identify the event.
 */
typedef struct CSL_IntcObj {
    CSL_IntcEventId eventId;    /**< The event-id */
    CSL_IntcVectId  vectId;     /**< The vector-id */
    CSL_IntcPriority  priority; /**< The priority level */
    void *          reserved;   /**< Reserved for the future */
} CSL_IntcObj;


/**
 * @brief   The interrupt handle
 *
 * This is returned by the CSL_intcOpen(..) API. The handle is used
 * to identify the event of interest in all INTC calls.
 */
typedef struct CSL_IntcObj *    CSL_IntcHandle;




/**
 * @brief   Intialize INTC module
 *
 * This API performs any module-specific initialization. CSL_intcInit(..)
 * must be invoked before calling any other API in the INTC module.
 *
 * @b Example:
 * @verbatim
      CSL_sysInit( );
      if (CSL_intcInit( ) != CSL_SOK) {
        // module initialization failed! //
      }
  
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcInit (
        void
);



/**
 * @brief   Allocates an event for use
 *
 * The API would reserve an interrupt-event for use. It returns
 * a valid handle to the event only if the event is not currently
 * allocated. The user could release the event after use by calling
 * CSL_intcClose(..). The CSL-object ('intcObj') that the user
 * passes would be used to store information pertaining handle.
 *
 * @b Example:
 * @verbatim
        CSL_IntcObj     intcObj;
        CSL_IntcHandle  hIntc;
        CSL_Status      openStatus;
  
        hIntc = CSL_intcOpen(&intcObj, CSL_INTC_EVENTID_TIMER3,
                    CSL_INTC_VECTID_DEFAULT, NULL, &openStatus);
        if (openStatus != CSL_SOK) {
            // open failed //
        }
   @endverbatim
 *
 * @return  Handle identifying the event
 */

CSL_IntcHandle
    CSL_intcOpen (
        CSL_IntcObj *       intcObj,    /**< pointer to the CSL-object allocated by the user */
        CSL_IntcEventId     eventId,    /**< the event-id of the interrupt */
        CSL_IntcVectId      vectId,     /**< the interrupt-vector */
        CSL_IntcHwSetup *   setup,      /**< (optional) pointer to an optional setup-structure */
        CSL_Status *        status      /**< (optional) pointer to a variable that would receive the status */
);




/**
 * @brief   Releases an allocated event
 *
 * CSL_intcClose(..) must be called to release an event that has
 * been previously allocated with a call to CSL_intcOpen(..).
 *
 * @b Example:
 * @verbatim
        if (CSL_intcClose(hIntc) != CSL_SOK) {
            // close failed! //
        }
   @endverbatim
 *
 * @return  CSL_SOK on success; CSL_ESYS_FAIL on failure.
 */

CSL_Status
    CSL_intcClose (
        CSL_IntcHandle      hIntc   /**< Handle to an event; see CSL_intcOpen(..) */
);







/**

 * @brief   Sets up the interrupt controller for a particular event
 *
 * CSL_intcHwSetup(..) API is used to configure the interrupt controller
 * for the event identified by the handle. The user must instantiate
 * and initialize a setup-structure with appropriate configuration
 * parameters before passing it to the function.
 *
 * @b Example:
 * @verbatim
        CSL_IntcHwSetup setup = CSL_INTC_HWSETUP_DEFAULTS;
  
        setup.priority = CSL_INTC_PRIORITY_DEFAULT;
  
        CSL_intcHwSetup(hIntc, &setup);
  
   @endverbatim
 *
 * @return
*/

CSL_Status
    CSL_intcHwSetup (
        CSL_IntcHandle      hIntc,  /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwSetup *   hwSetup /**< Pointer to a Setup-structure */
);




/**
 * @brief   Perform a control-operation
 *
 * This API is used to invoke any of the supported control-operations
 * supported by the module. Note: Refer to the control-command documentation
 * for details on the parameter (if any) that a specific command accepts.
 *
 * @b Example:
 * @verbatim
        CSL_intcHwControl(hIntc, CSL_INTC_CMD_EVTSET, NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success;
 *          CSL_ESYS_FAIL on failure;
 *          CSL_ESYS_BADHANDLE if the handle is invalid;
 *          CSL_ESYS_NOTSUPPORTED if not supported;
 *          CSL_ESYS_INVCMD on an invalid command.
*/

CSL_Status
    CSL_intcHwControl (
        CSL_IntcHandle          hIntc,      /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwControlCmd    command,    /**< Command identifier */
        void *                  commandArg  /**< Optional command parameter */
);




/**
 * @brief   Queries the peripheral for status
 *
 * The CSL_intcGetHwStatus(..) API could be used to retrieve status
 * or configuration information from the peripheral. The user must
 * allocate an object that would hold the retrieved information
 * and pass a pointer to it to the function. The type of the object
 * is specific to the query-command.
 *
 * @b Example:
 * @verbatim
        Bool    evtPending = FALSE;
        while (evtPending == FALSE) {
            CSL_intcGetHwStatus(hIntc, CSL_INTC_QUERY_ISEVENTPENDING, &evtPending);
        }
  
   @endverbatim
 *
 * @return  CSL_SOK on success;
 *          CSL_ESYS_FAIL on failure;
 *          CSL_ESYS_BADHANDLE if the handle is invalid;
 *          CSL_ESYS_NOTSUPPORTED if not supported;
 *          CSL_ESYS_INVQUERY on an invalid query.
*/

CSL_Status
    CSL_intcGetHwStatus (
        CSL_IntcHandle          hIntc,      /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwStatusQuery   query,      /**< Query identifier */
        void *                  response    /**< Pointer to an object that would contain the retrieved information */
);




/**
 * @brief   Enable an event
 *
 * The API enables the specified event. If the user wishes to restore
 * the enable-state of the event at a later point of time, they may
 * store the current state using the parameter, which could be used
 * with CSL_intcEventRestore(..).
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_intcEventEnable(CSL_INTC_EVENTID_UART1, NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcEventEnable (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState *  prevState   /**< (Optional) Pointer to object that would store current state */
);



/**
 * @brief   Disable an event
 *
 * The API disables the specified event. If the user wishes to restore
 * the enable-state of the event at a later point of time, they may
 * store the current state using the parameter, which could be used
 * with CSL_intcEventRestore(..).
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_IntcEventEnableState    oldState;
        CSL_intcEventDisable(CSL_INTC_EVENTID_I2C, &oldState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcEventDisable (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState *  prevState   /**< (Optional) Pointer to object that would store current state */
);



/**
 * @brief   Restore the event to a previous enabled/disabled state
 *
 * The API restores the specified event to a previous enable-state
 * as recorded by the event-enable state passed as an argument.
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_intcEventRestore(CSL_INTC_EVENTID_I2C, oldState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/
CSL_Status
    CSL_intcEventRestore (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState    prevState   /**< Object that contains information about previous state */
);




/**
 * @brief   Globally enable interrupts
 *
 * The API enables the global interrupt by manipulating the processor's
 * global interrupt enable/disable flag. If the user wishes to restore
 * the enable-state at a later point, they may store the current state
 * using the parameter, which could be used with CSL_intcGlobalRestore(..).
 * CSL_intcGlobalEnable(..) must be called from a privileged mode.
 *
 * @b Example:
 * @verbatim
        CSL_intcGlobalEnable(NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success
 */

CSL_Status
    CSL_intcGlobalEnable (
        CSL_IntcGlobalEnableState * prevState   /**< (Optional) Pointer to object that would store current state */
);




/**
 * @brief   Globally disable interrupts
 *
 * The API disables the global interrupt by manipulating the processor's
 * global interrupt enable/disable flag. If the user wishes to restore
 * the enable-state at a later point, they may store the current state
 * using the parameter, which could be used with CSL_intcGlobalRestore(..).
 * CSL_intcGlobalDisable(..) must be called from a privileged mode.
 *
 * @b Example:
 * @verbatim
        CSL_IntcGlobalEnableState   gieState;
  
        CSL_intcGlobalDisable(&gieState);
        // critical-section code //
        CSL_intcGlobalRestore(gieState);
   @endverbatim
 *
 * @return  CSL_SOK on success
 */

CSL_Status
    CSL_intcGlobalDisable (
        CSL_IntcGlobalEnableState * prevState   /**< (Optional) Pointer to object that would store current state */
);




/**
 * @brief   Restore global interrupt enable/disable to a previous state
 *
 * The API restores the global interrupt enable/disable state to a previous
 * state as recorded by the global-event-enable state passed as an argument.
 * CSL_intcGlobalDisable(..) must be called from a privileged mode.
 *
 * @b Example:
 * @verbatim
        CSL_IntcGlobalEnableState   gieState;
  
        CSL_intcGlobalDisable(&gieState);
        // critical-section code //
        CSL_intcGlobalRestore(gieState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcGlobalRestore (
        CSL_IntcGlobalEnableState       prevState   /**< Object containing information about previous state */
);




/**
 * @brief   Hooks up and sets up the CSL interrupt dispatchers
 *
 * The user should call CSL_intcDispatcherInit(..) if they
 * wish to make use of the dispatchers built into the CSL INTC
 * module.
 *
 * Note: This API must be called before using
 * CSL_intcPlugEventHandler(..).
 *
 * @b Example:
 * @verbatim
      CSL_sysInit( );
      if (CSL_intcInit( ) != CSL_SOK) {
        // module initialization failed! //
      }
      if (CSL_intcDispatcherInit( ) != CSL_SOK) {
        // CSL dispatcher setting up failed! //
      }
   @endverbatim
 *
 * @return CSL_SOK on success; CSL_ESYS_FAIL on failure.
*/

CSL_Status
    CSL_intcDispatcherInit (
        void
);



/**
 * @brief   Associate an event-handler with an event
 *
 * CSL_intcPlugEventHandler(..) ties an event-handler to an event; so
 * that the occurence of the event, would result in the event-handler
 * being invoked.
 *
 * @b Example:
 * @verbatim
        CSL_IntcEventHandlerRecord  evtHandlerRecord;
  
        evtHandlerRecord.handler = myIsr;
        evtHandlerRecord.arg     = (void *)hTimer;
        CSL_intcPlugEventHandler(hIntc, &evtHandlerRecord);
   @endverbatim
 *
 * @return  Returns the address of the previous handler
*/

CSL_IntcEventHandler
    CSL_intcPlugEventHandler (
        CSL_IntcHandle                  hIntc,
                /**< Handle identying the interrupt-event; see CSL_intcOpen(..) */
        CSL_IntcEventHandlerRecord *    eventHandlerRecord
                /**< Provides the details of the event-handler */
);



/**
 * @brief   Hook up an exception handler
 *
 * This API hooks up the handler to the specified exception.
 * Note: In this case, it is done by inserting a B(ranch) instruction
 * to the handler. Because of the restriction in the instruction
 * th handler must be within 32MB of the exception vector.
 * Also, the function assumes that the exception vector table is
 * located at its default ("low") address.
 *
 * @b Example:
 * @verbatim
        CSL_intcHookIsr(CSL_INTC_VECTID_UNDEF, (void *)myUndefExceptionHandler);
   @endverbatim
 */
void
    CSL_intcHookIsr (
        CSL_IntcVectId  vectId,     /**< Exception identifier */
        void *          isrAddr     /**< Pointer to the handler */
);

#ifdef __cplusplus
}
#endif


#endif


/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 6         20 Jan 2005 09:57:46    6138             xkeshavm      */
/*                                                                  */
/* psc interrupt number is added                                    */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:04    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
