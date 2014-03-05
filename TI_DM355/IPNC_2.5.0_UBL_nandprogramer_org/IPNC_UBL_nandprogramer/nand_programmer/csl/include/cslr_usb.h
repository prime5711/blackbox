#ifndef _CSLR_USB_H_
#define _CSLR_USB_H_

#include <cslr.h>
#include "cslr_usb_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_USB_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_UsbRegs *  CSL_UsbRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_USB_0_REGS                 ((CSL_UsbRegsOvly) 0x01c64000)


#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:59    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
