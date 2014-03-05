#ifndef _AEMIF_H_
#define _AEMIF_H_

#define AEMIF_ADDR  			0x01E00000

#define RCSR  		    	(volatile Uint32*) (AEMIF_ADDR + 0x00)	//Revision Code and Status Register
#define WAITCFG         	(volatile Uint32*) (AEMIF_ADDR + 0x04) 	//Async Wait Cycle Config Register
#define ACFG2  			(volatile Uint32*) (AEMIF_ADDR + 0x10)	//Async Bank1 Config Register
#define ACFG3	 		(volatile Uint32*) (AEMIF_ADDR + 0x14)	//Async Bank2 Config Register
#define ACFG4	 		(volatile Uint32*) (AEMIF_ADDR + 0x18)	//Async Bank3 Config Register
#define ACFG5	 		(volatile Uint32*) (AEMIF_ADDR + 0x1C)	//Async Bank4 Config Register
#define AINTRAW			(volatile Uint32*) (AEMIF_ADDR + 0x40)	//Interrpt Raw Register
#define AINTMASK		(volatile Uint32*) (AEMIF_ADDR + 0x44)	//Interrupt Masked Register
#define AINTMASKSET		(volatile Uint32*) (AEMIF_ADDR + 0x48)	//Interrupt Mask Set Register
#define INTMASKCLEAR		(volatile Uint32*) (AEMIF_ADDR + 0x4C)	//Interrupt Mask Clear Register
#define NANDCTL			(volatile Uint32*) (AEMIF_ADDR + 0x60)	//NAND Flash Control Register
#define NANDSTAT		(volatile Uint32*) (AEMIF_ADDR + 0x64)	//NAND Flash Status Register
#define NANDECC2		(volatile Uint32*) (AEMIF_ADDR + 0x70)	//NAND Flash 1 ECC Register
#define NANDECC3		(volatile Uint32*) (AEMIF_ADDR + 0x74)	//NAND Flash 2 ECC Register
#define NANDECC4		(volatile Uint32*) (AEMIF_ADDR + 0x78)	//NAND Flash 3 ECC Register
#define NANDECC5		(volatile Uint32*) (AEMIF_ADDR + 0x7C)	//NAND Flash 4 ECC Register

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         25 Jun 2004 10:31:02    1881             xkeshavm      */
/*                                                                  */
/* Command header files added to develop test cases                 */
/********************************************************************/ 
