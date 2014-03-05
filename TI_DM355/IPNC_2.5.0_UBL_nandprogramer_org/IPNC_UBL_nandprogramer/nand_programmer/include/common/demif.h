#ifndef _DEMIF_H_
#define _DEMIF_H_

#include <tistdtypes.h>

#define	DDR_BASE 0x20000000
#define EIDRR                   (volatile Uint32*) (DDR_BASE + 0x00)    //EMIF Module ID and Revision Register   
#define SDSTAT                  (volatile Uint32*) (DDR_BASE + 0X04)    //SDRAM Status Register  
#define SDCFG            	(volatile Uint32*) (DDR_BASE + 0x08) 	//SDRAM Bank Config Register
#define SDREF           	(volatile Uint32*) (DDR_BASE + 0x0C) 	//SDRAM Refresh Control Register
#define SDTIM           	(volatile Uint32*) (DDR_BASE + 0x10) 	//SDRAM Timing Register
#define VBUSP            	(volatile Uint32*) (DDR_BASE + 0x20) 	//VBUSM Burst Priority Register
#define PERFCNT1		(volatile Uint32*) (DDR_BASE + 0x40)	//Performance Counter Register 1
#define PERFCNT2		(volatile Uint32*) (DDR_BASE + 0x44)	//Performance Counter Register 2
#define PERFCNTCFG		(volatile Unit32*) (DDR_BASE + 0x48)    //Performance Counter Config Register
#define PERFCNTMSTREGSEL        (volatile Uint32*) (DDR_BASE + 0x4C)	//Performance Counter Master Region Select Register
#define INTRAW			(volatile Uint32*) (DDR_BASE + 0xC0)    //Interrupt Raw Register
#define INTMASK                 (volatile Uint32*) (DDR_BASE + 0xC4)    //Interrupt Masked Register
#define INTMASKSET              (volatile Uint32*) (DDR_BASE + 0xC8)    //Interrupt Mask Set Register
#define INTMASKCLR        	(volatile Uint32*) (DDR_BASE + 0xCC)    //Interrupt Mask Clear Register
#define DDRPHYREV               (volatile Uint32*) (DDR_BASE + 0xE0)    //DDR PHY ID and Revision Register
#define DDRCTL                  (volatile Uint32*) (DDR_BASE + 0xE4)    //DDR PHY Control Register
#define PHYSTAT                 (volatile Uint32*) (DDR_BASE + 0xE8)    //DDR PHY Status Register

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         25 Jun 2004 10:31:05    1881             xkeshavm      */
/*                                                                  */
/* Command header files added to develop test cases                 */
/********************************************************************/ 
