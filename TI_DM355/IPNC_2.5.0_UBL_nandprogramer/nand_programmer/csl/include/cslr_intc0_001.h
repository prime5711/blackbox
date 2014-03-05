/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/

#ifndef _CSLR_INTC0_001_H_
#define _CSLR_INTC0_001_H_

#include <csl_types.h>

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_INTC0_CPSR */
#define CSL_INTC0_CPSR_N_MASK         (0x80000000u)
#define CSL_INTC0_CPSR_N_SHIFT        (0x0000001Fu)
#define CSL_INTC0_CPSR_N_RESETVAL     (0x00000000u)

#define CSL_INTC0_CPSR_Z_MASK         (0x40000000u)
#define CSL_INTC0_CPSR_Z_SHIFT        (0x0000001Eu)
#define CSL_INTC0_CPSR_Z_RESETVAL     (0x00000000u)

#define CSL_INTC0_CPSR_C_MASK         (0x20000000u)
#define CSL_INTC0_CPSR_C_SHIFT        (0x0000001Du)
#define CSL_INTC0_CPSR_C_RESETVAL     (0x00000000u)

#define CSL_INTC0_CPSR_V_MASK         (0x10000000u)
#define CSL_INTC0_CPSR_V_SHIFT        (0x0000001Cu)
#define CSL_INTC0_CPSR_V_RESETVAL     (0x00000000u)

#define CSL_INTC0_CPSR_Q_MASK         (0x08000000u)
#define CSL_INTC0_CPSR_Q_SHIFT        (0x0000001Bu)
#define CSL_INTC0_CPSR_Q_RESETVAL     (0x00000000u)

#define CSL_INTC0_CPSR_I_MASK         (0x00000080u)
#define CSL_INTC0_CPSR_I_SHIFT        (0x00000007u)
#define CSL_INTC0_CPSR_I_RESETVAL     (0x00000000u)
#define CSL_INTC0_CPSR_I_ENABLE       (0x00000000u)
#define CSL_INTC0_CPSR_I_DISABLE      (0x00000001u)

#define CSL_INTC0_CPSR_F_MASK         (0x00000040u)
#define CSL_INTC0_CPSR_F_SHIFT        (0x00000006u)
#define CSL_INTC0_CPSR_F_RESETVAL     (0x00000000u)
#define CSL_INTC0_CPSR_F_ENABLE       (0x00000000u)
#define CSL_INTC0_CPSR_F_DISABLE      (0x00000001u)

#define CSL_INTC0_CPSR_T_MASK         (0x00000020u)
#define CSL_INTC0_CPSR_T_SHIFT        (0x00000005u)
#define CSL_INTC0_CPSR_T_RESETVAL     (0x00000000u)
#define CSL_INTC0_CPSR_T_ARM          (0x00000000u)
#define CSL_INTC0_CPSR_T_THUMB        (0x00000001u)

#define CSL_INTC0_CPSR_M_MASK         (0x0000001Fu)
#define CSL_INTC0_CPSR_M_SHIFT        (0x00000000u)
#define CSL_INTC0_CPSR_M_RESETVAL     (0x0000000Du)
#define CSL_INTC0_CPSR_M_USR          (0x00000010u)
#define CSL_INTC0_CPSR_M_FIQ          (0x0000000Bu)
#define CSL_INTC0_CPSR_M_IRQ          (0x0000000Cu)
#define CSL_INTC0_CPSR_M_SVC          (0x0000000Du)
#define CSL_INTC0_CPSR_M_ABT          (0x00000011u)
#define CSL_INTC0_CPSR_M_UND          (0x0000001Bu)
#define CSL_INTC0_CPSR_M_SYS          (0x0000001Fu)

#define CSL_INTC0_CPSR_RESETVAL       (0x0000000Du)


  /* CSL_INTC0_SPSR */
#define CSL_INTC0_SPSR_N_MASK         (0x80000000u)
#define CSL_INTC0_SPSR_N_SHIFT        (0x0000001Fu)
#define CSL_INTC0_SPSR_N_RESETVAL     (0x00000000u)

#define CSL_INTC0_SPSR_Z_MASK         (0x40000000u)
#define CSL_INTC0_SPSR_Z_SHIFT        (0x0000001Eu)
#define CSL_INTC0_SPSR_Z_RESETVAL     (0x00000000u)

#define CSL_INTC0_SPSR_C_MASK         (0x20000000u)
#define CSL_INTC0_SPSR_C_SHIFT        (0x0000001Du)
#define CSL_INTC0_SPSR_C_RESETVAL     (0x00000000u)

#define CSL_INTC0_SPSR_V_MASK         (0x10000000u)
#define CSL_INTC0_SPSR_V_SHIFT        (0x0000001Cu)
#define CSL_INTC0_SPSR_V_RESETVAL     (0x00000000u)

#define CSL_INTC0_SPSR_Q_MASK         (0x08000000u)
#define CSL_INTC0_SPSR_Q_SHIFT        (0x0000001Bu)
#define CSL_INTC0_SPSR_Q_RESETVAL     (0x00000000u)

#define CSL_INTC0_SPSR_I_MASK         (0x00000080u)
#define CSL_INTC0_SPSR_I_SHIFT        (0x00000007u)
#define CSL_INTC0_SPSR_I_RESETVAL     (0x00000000u)
#define CSL_INTC0_SPSR_I_ENABLE       (0x00000000u)
#define CSL_INTC0_SPSR_I_DISABLE      (0x00000001u)

#define CSL_INTC0_SPSR_F_MASK         (0x00000040u)
#define CSL_INTC0_SPSR_F_SHIFT        (0x00000006u)
#define CSL_INTC0_SPSR_F_RESETVAL     (0x00000000u)
#define CSL_INTC0_SPSR_F_ENABLE       (0x00000000u)
#define CSL_INTC0_SPSR_F_DISABLE      (0x00000001u)

#define CSL_INTC0_SPSR_T_MASK         (0x00000020u)
#define CSL_INTC0_SPSR_T_SHIFT        (0x00000005u)
#define CSL_INTC0_SPSR_T_RESETVAL     (0x00000000u)
#define CSL_INTC0_SPSR_T_ARM          (0x00000000u)
#define CSL_INTC0_SPSR_T_THUMB        (0x00000001u)

#define CSL_INTC0_SPSR_M_MASK         (0x0000001Fu)
#define CSL_INTC0_SPSR_M_SHIFT        (0x00000000u)
#define CSL_INTC0_SPSR_M_RESETVAL     (0x0000000Du)
#define CSL_INTC0_SPSR_M_USR          (0x00000010u)
#define CSL_INTC0_SPSR_M_FIQ          (0x0000000Bu)
#define CSL_INTC0_SPSR_M_IRQ          (0x0000000Cu)
#define CSL_INTC0_SPSR_M_SVC          (0x0000000Du)
#define CSL_INTC0_SPSR_M_ABT          (0x00000011u)
#define CSL_INTC0_SPSR_M_UND          (0x0000001Bu)
#define CSL_INTC0_SPSR_M_SYS          (0x0000001Fu)

#define CSL_INTC0_SPSR_RESETVAL       (0x0000000Du)




#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4          Aug 10:09:25 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:48    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
