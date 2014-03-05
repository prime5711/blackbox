#ifndef _CSLR_DEVICECONFIG_H_
#define _CSLR_DEVICECONFIG_H_

#include "cslr.h"
#include "cslr_deviceconfig_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
//  typedef CSL_PinmuxRegs    UART_Regs;
/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
  #define DEVICECONFIG_BASEADDR                (0x01C40000u)
  #define DEVICECONFIG_REGS                    ((CSL_DeviceconfigRegs    *) 0x01C40000u)
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         25 Jun 2004 10:31:04    1881             xkeshavm      */
/*                                                                  */
/* Command header files added to develop test cases                 */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 2         27 Jul 2004 12:52:07    2195             xkeshavm      */
/*                                                                  */
/* To change base addr of pinmux and added more function |Ds       */
/********************************************************************/ 
