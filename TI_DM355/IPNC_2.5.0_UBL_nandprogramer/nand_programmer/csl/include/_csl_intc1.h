/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/

#ifndef _CSL_INTC1_H_
#define _CSL_INTC1_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_intc1.h>

/**************************************************************************\
* INTC1 global macro declarations
\**************************************************************************/
#define _CSL_INTC_PRIORITY__INTC1CNT     (8)     /* Level-1 Interrupt priority count */


/**************************************************************************\
* INTC1 global typedef declarations
\**************************************************************************/
/* Interrupt Priority */
typedef enum {
    _CSL_INTC1_PRIORITY_INVALID     =                 -1,
    _CSL_INTC1_PRIORITY_DEFAULT     =                  0
} _CSL_Intc1Priority;

/* Interrupt Routing */
typedef enum {
    _CSL_INTC1_ROUTE_IRQ            =                  0,
    _CSL_INTC1_ROUTE_FIQ            =                  1
} _CSL_Intc1Route;

/* Interrupt Enable/Disable State */
typedef enum {
    _CSL_INTC1_STATE_ENABLE         =                  0,
    _CSL_INTC1_STATE_DISABLE        =                  1
} _CSL_Intc1EventEnableState;

/* Global Interrupt Enable/Disable State */
typedef enum {
    _CSL_INTC1_GBLSTATE_ENABLE      =                  0,
    _CSL_INTC1_GBLSTATE_DISABLE     =                  1
} _CSL_Intc1GlobalEnableState;

/* The Interrupt Event-ID */
typedef int _CSL_Intc1EventId;

/* Interrupt setup-structure */
typedef struct _CSL_Intc1Setup {
    _CSL_Intc1Priority       priority;
} _CSL_Intc1Setup;

/**************************************************************************\
* INTC1 global function declarations
\**************************************************************************/
/**************************************************************************\
* INTC1 inline function declarations
\**************************************************************************/
/*CSL_WEBS*/
CSL_IDEF_INLINE void  _CSL_intc1Init(
    void
);


CSL_IDEF_INLINE _CSL_Intc1Priority  _CSL_intc1GetPriority(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE void  _CSL_intc1SetPriority(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Priority                       priority
);


CSL_IDEF_INLINE _CSL_Intc1Route  _CSL_intc1GetRoute(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE void  _CSL_intc1Setup(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Setup                          *setup
);


CSL_IDEF_INLINE void  _CSL_intc1GetSetup(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Setup                          *setup
);


CSL_IDEF_INLINE void * _CSL_intc1GetFIQEntry(
    void
);


CSL_IDEF_INLINE void * _CSL_intc1GetIRQEntry(
    void
);


CSL_IDEF_INLINE void  _CSL_intc1AckFIQ(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE void  _CSL_intc1AckIRQ(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Enable(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Disable(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Restore(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1EventEnableState               enState
);


CSL_IDEF_INLINE Bool  _CSL_intc1Test(
    _CSL_Intc1EventId                        evtId
);


CSL_IDEF_INLINE void  _CSL_intc1Reset(
    _CSL_Intc1EventId                        evtId
);


/**************************************************************************\
* INTC1 inline function definitions
\**************************************************************************/

CSL_IDEF_INLINE void  _CSL_intc1Init(
    void
)
{  
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE _CSL_Intc1Priority  _CSL_intc1GetPriority(
    _CSL_Intc1EventId                        evtId
)
{
    Uint32 msb, lsb;
    volatile Uint32 *reg;
    reg = &((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->INTPRI0 + (evtId>>3);
    lsb = ((evtId & 7) * 4);
    msb = lsb + 2;
    return (_CSL_Intc1Priority)CSL_FEXTR(*reg, msb, lsb);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1SetPriority(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Priority                       priority
)
{
    Uint32 msb, lsb;
    volatile Uint32 *reg;
    reg = &((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->INTPRI0 + (evtId>>3);
    lsb = ((evtId & 7) * 4);
    msb = lsb + 2;
    CSL_FINSR(*reg, msb, lsb, priority);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE _CSL_Intc1Route  _CSL_intc1GetRoute(
    _CSL_Intc1EventId                        evtId
)
{
    _CSL_Intc1Priority priority;
    _CSL_Intc1Route route;
    priority = _CSL_intc1GetPriority(evtId);
    if ((priority >= 0) && (priority <= 1)) {
        route =  _CSL_INTC1_ROUTE_FIQ;       
    }
    else {
        route =  _CSL_INTC1_ROUTE_IRQ;       
    }
    return route;
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1Setup(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Setup                          *setup
)
{
    _CSL_intc1SetPriority(evtId, setup->priority);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1GetSetup(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1Setup                          *setup
)
{
    setup->priority = _CSL_intc1GetPriority(evtId);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void * _CSL_intc1GetFIQEntry(
    void
)
{
    return (void *)(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->FIQENTRY);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void *  _CSL_intc1GetIRQEntry(
    void
)
{
    return (void *)(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->IRQENTRY);
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1AckFIQ(
    _CSL_Intc1EventId                        evtId
)
{
    if(evtId < 32) {
        ((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->FIQ0 = (CSL_INTC_FIQ0_FIQ_ACK << evtId);
    } else {
        evtId -= 32;
        ((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->FIQ1 = (CSL_INTC_FIQ1_FIQ_ACK << evtId);
    }
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1AckIRQ(
    _CSL_Intc1EventId                        evtId
)
{
    if(evtId < 32) {
        ((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->IRQ0 = (CSL_INTC_IRQ0_IRQ_ACK << evtId);
    } else {
        evtId -= 32;
        ((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->IRQ1 = (CSL_INTC_IRQ1_IRQ_ACK << evtId);
    }
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Enable(
    _CSL_Intc1EventId                        evtId
)
{
    _CSL_Intc1EventEnableState en;
    if(evtId < 32) {
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId, CSL_INTC_EINT0_EINT_ENABLE);
    } else {
        evtId -= 32;
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId, CSL_INTC_EINT1_EINT_ENABLE);
    }
    return en;
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Disable(
    _CSL_Intc1EventId                        evtId
)
{
    _CSL_Intc1EventEnableState en;
    if(evtId < 32) {
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId, CSL_INTC_EINT0_EINT_DISABLE);
    } else {
        evtId -= 32;
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId, CSL_INTC_EINT1_EINT_DISABLE);
    }
    return en;
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE _CSL_Intc1EventEnableState  _CSL_intc1Restore(
    _CSL_Intc1EventId                        evtId,
    _CSL_Intc1EventEnableState               enState
)
{
    _CSL_Intc1EventEnableState en;
    if (evtId < 32) {
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT0, evtId, evtId, enState /* TODO: use reg-layer token */ );
    } else {
        evtId -= 32;
        en  = (_CSL_Intc1EventEnableState)CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId);
        CSL_FINSR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->EINT1, evtId, evtId, enState /* TODO: use reg-layer token */ );
    }
    return en;
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE Bool _CSL_intc1Test(
    _CSL_Intc1EventId                        evtId
)
{
    Uint32 route;
    Bool intFlag;
    
    route = _CSL_intc1GetRoute(evtId);

    if (route == _CSL_INTC1_ROUTE_FIQ) {
        if (evtId < 32) {
            intFlag = CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->FIQ0, evtId, evtId);
        } else { /* evtId >= 32 */ 
            evtId -= 32;
            intFlag = CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->FIQ1, evtId, evtId);
        }
    } else if (route == _CSL_INTC1_ROUTE_IRQ) {
        if (evtId < 32) {
            intFlag = CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->IRQ0, evtId, evtId);
        } else { /* evtId >= 32 */ 
            evtId -= 32;
            intFlag = CSL_FEXTR(((CSL_Intc1RegsOvly)CSL_INTC1_REGS)->IRQ1, evtId, evtId);
        }
    }
    return intFlag;
}

/*------------------------------------------------------------------------*/

CSL_IDEF_INLINE void  _CSL_intc1Reset(
    _CSL_Intc1EventId                        evtId
)
{
    _CSL_intc1Disable(evtId);
    _CSL_intc1AckFIQ(evtId);
    _CSL_intc1AckIRQ(evtId);
}

/*------------------------------------------------------------------------*/


#endif

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         14 Jan 2005 13:32:23    5888             xkeshavm      */
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
