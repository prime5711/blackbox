/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/

#ifndef _CSL_INTC0_H_
#define _CSL_INTC0_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_intc0.h>

/**************************************************************************\
* INTC0 global macro declarations
\**************************************************************************/

/**************************************************************************\
* INTC0 global typedef declarations
\**************************************************************************/
/* ARM processor modes */
typedef enum {
    _CSL_INTC0_MODE_USR             =               0x10,
    _CSL_INTC0_MODE_FIQ             =               0x11,
    _CSL_INTC0_MODE_IRQ             =               0x12,
    _CSL_INTC0_MODE_SVC             =               0x13,
    _CSL_INTC0_MODE_ABT             =               0x17,
    _CSL_INTC0_MODE_UND             =               0x1B,
    _CSL_INTC0_MODE_SYS             =               0x1F
} _CSL_Intc0Mode;

/* ARM CPU exceptions */
typedef enum {
    _CSL_INTC0_EXCP_RESET           =                  0,
    _CSL_INTC0_EXCP_UNDEF           =                  1,
    _CSL_INTC0_EXCP_SWI             =                  2,
    _CSL_INTC0_EXCP_PREABT          =                  3,
    _CSL_INTC0_EXCP_DATABT          =                  4,
    _CSL_INTC0_EXCP_IRQ             =                  6,
    _CSL_INTC0_EXCP_FIQ             =                  7
} _CSL_Intc0Exception;

/* IRQ exception enable/disable state */
typedef enum {
    _CSL_INTC0_IRQSTATE_ENABLE      =                  0,
    _CSL_INTC0_IRQSTATE_DISABLE     =                  1
} _CSL_Intc0IrqEnableState;

/* FIQ exception enable/disable state */
typedef enum {
    _CSL_INTC0_FIQSTATE_ENABLE      =                  0,
    _CSL_INTC0_FIQSTATE_DISABLE     =                  1
} _CSL_Intc0FiqEnableState;

/**************************************************************************\
* INTC0 global function declarations
\**************************************************************************/
/*
 * ======================================================
 *   @func   csl_intc0EnableIRQ.c
 *   @desc   Enable IRQ exception
 *
 *
 *   @ret  IrqEnableState
 *
 *              Enable IRQ exception
 *
 *
 *   @eg
 *         Enable IRQ exception
 *
 *
 * ======================================================
*/

_CSL_Intc0IrqEnableState  _CSL_intc0EnableIRQ(
    void
);

/*
 * ======================================================
 *   @func   csl_intc0DisableIRQ.c
 *   @desc   Disable IRQ exception
 *
 *
 *   @ret  IrqEnableState
 *
 *              Disable IRQ exception
 *
 *
 *   @eg
 *         Disable IRQ exception
 *
 *
 * ======================================================
*/

_CSL_Intc0IrqEnableState  _CSL_intc0DisableIRQ(
    void
);

/*
 * ======================================================
 *   @func   csl_intc0RestoreIRQ.c
 *   @desc   Disable IRQ exception
 *
 *
 *   @arg  prevState
 *              the enable-state to restore to
 *   @ret  IrqEnableState
 *
 *              Restore the IRQ exception enable-state
 *
 *
 *   @eg
 *         Restore the IRQ exception enable-state
 *
 *
 * ======================================================
*/

_CSL_Intc0IrqEnableState  _CSL_intc0RestoreIRQ(
    _CSL_Intc0IrqEnableState                 prevState
);

/*
 * ======================================================
 *   @func   csl_intc0EnableFIQ.c
 *   @desc   Enable FIQ exception
 *
 *
 *   @ret  FiqEnableState
 *
 *              Enable FIQ exception
 *
 *
 *   @eg
 *         Enable FIQ exception
 *
 *
 * ======================================================
*/

_CSL_Intc0FiqEnableState  _CSL_intc0EnableFIQ(
    void
);

/*
 * ======================================================
 *   @func   csl_intc0DisableFIQ.c
 *   @desc   Disable FIQ exception
 *
 *
 *   @ret  FiqEnableState
 *
 *              Disable FIQ exception
 *
 *
 *   @eg
 *         Disable FIQ exception
 *
 *
 * ======================================================
*/

_CSL_Intc0FiqEnableState  _CSL_intc0DisableFIQ(
    void
);

/*
 * ======================================================
 *   @func   csl_intc0RestoreFIQ.c
 *   @desc   Disable FIQ exception
 *
 *
 *   @arg  prevState
 *              the enable-state to restore to
 *   @ret  FiqEnableState
 *
 *              Restore the FIQ exception enable-state
 *
 *
 *   @eg
 *         Restore the FIQ exception enable-state
 *
 *
 * ======================================================
*/

_CSL_Intc0FiqEnableState  _CSL_intc0RestoreFIQ(
    _CSL_Intc0FiqEnableState                 prevState
);

/**************************************************************************\
* INTC0 inline function declarations
\**************************************************************************/
/*CSL_WEBS*/
CSL_IDEF_INLINE void  _CSL_intc0Init(
    void
);


CSL_IDEF_INLINE void  _CSL_intc0HookVector(
    _CSL_Intc0Exception                      exception,
    void                                    *handler
);

/**************************************************************************\
* INTC0 inline function definitions
\**************************************************************************/

CSL_IDEF_INLINE void  _CSL_intc0Init(
    void
)
{
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc0HookVector(
    _CSL_Intc0Exception exception,
    void *              handler
)
{
    /* generate a B(ranch) instruction to the 'handler' from the exception vector;
       the Exception enum has been so designed that multiplying it by 4 would yield
       the corresponding exception-vector address */

    *((volatile int *)((int)exception * 4)) =
        0xEA000000 | (((int)handler - ((int)exception * 4) - 8) >> 2);

    /* TODO: the assumption, above, is that the vectors are at the default
       low-vector area -- starting at 0x00000000; does not check-for/support
       high-vector mapping -- through CP15? */
}

/*------------------------------------------------------------------------*/


#endif

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         14 Jan 2005 13:32:22    5888             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL0.57 JAN 2005 Release and built the library for ARM and DSP*/
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:10    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
