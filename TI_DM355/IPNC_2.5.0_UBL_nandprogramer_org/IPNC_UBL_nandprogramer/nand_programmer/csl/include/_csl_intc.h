/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/

#ifndef __CSL_INTC_H_
#define __CSL_INTC_H_

#define _CSL_INTC_EVTHANDLER_UNPLUGGED                                    \
    ((CSL_IntcEventHandler) 0)

/*
 * ======================================================
 *   @func   _csl_intcDispatchFIQ.c
 *   @desc   the FIQ dispatcher
 *
 *
 *   @ret  void
 *
 *              This is the FIQ dispatcher that gets plugged to the FIQ
 *              exception-vector on the ARM core, when L<dispatcherInit>
 *              is called. Thus, on reception of an FIQ, this FIQ dispatcher
 *              gets executed, which then probes the level-1 and level-2
 *              interrupt-controllers to find the source of the interrupt.
 *              It then calls the Interrupt Service Routine ("Event Handler")
 *              corresponding to the interrupt-number, if one is plugged, to
 *              service the interrupt. Before passing on control to the
 *              interrupted code, it also "acknowledges" the receipt of
 *              the interrupt to the controllers to enable the generation
 *              of a new FIQ.
 *
 *
 *   @eg
 *
 *
 * ======================================================
*/

interrupt
void
    _CSL_intcDispatchFIQ (
        void
);

/*
 * ======================================================
 *   @func   _csl_intcDispatchIRQ.c
 *   @desc   the IRQ dispatcher
 *
 *
 *   @ret  void
 *
 *              This is the IRQ dispatcher that gets plugged to the IRQ
 *              exception-vector on the ARM core, when L<dispatcherInit>
 *              is called. Thus, on reception of an IRQ, this IRQ dispatcher
 *              gets executed, which then probes the level-1 and level-2
 *              interrupt-controllers to find the source of the interrupt.
 *              It then calls the Interrupt Service Routine ("Event Handler")
 *              corresponding to the interrupt-number, if one is plugged, to
 *              service the interrupt. Before passing on control to the
 *              interrupted code, it also "acknowledges" the receipt of
 *              the interrupt to the controllers to enable the generation
 *              of a new IRQ.
 *
 *
 *   @eg
 *
 *
 * ======================================================
*/

interrupt
void
    _CSL_intcDispatchIRQ (
        void
);


extern
CSL_BitMask32                _CSL_intcAllocMask[ ];

extern
CSL_BitMask32                _CSL_intcPriAllocMask[ ];

extern
CSL_IntcEventHandlerRecord   _CSL_intcEventHandlerRecord[ ];

extern
Uint32   _CSL_intcInterruptEntryTable[ ];

#endif /* __CSL_INTC_H_ */
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:08:49 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:09    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
