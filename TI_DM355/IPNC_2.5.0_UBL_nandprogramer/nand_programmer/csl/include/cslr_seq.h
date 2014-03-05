#ifndef _CSLR_SEQ_H_
#define _CSLR_SEQ_H_

#include <cslr.h>
#include "cslr_seq_001.h"

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_SEQ_PER_CNT          1

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_SeqRegs *  CSL_SeqRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_SEQ_0_REGS                 ((CSL_SeqRegsOvly) 0x01cdff00)


#endif
