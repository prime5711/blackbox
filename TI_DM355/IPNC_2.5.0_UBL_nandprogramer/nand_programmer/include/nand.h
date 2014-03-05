/************************************/
/* 	nand1.h          				*/
/*	Definitions for boot loader	    */
/*  DaVinci ROM boot-loader 		*/
/*  Vignesh LA						*/
/*  Nov-02-2004                     */
/************************************/
/* 
----------------------------------------------------------------------
Has 2 additional functions which are not present in the driver 
   used in the RBL. 
   NAND_writePage()
   NAND_eraseBlock()
   These are not required in RBL and are used for writing the UBL
   for test purposes
   
   Vignesh - 19th Nov,2004
----------------------------------------------------------------------
*/
#ifndef _NAND_H_
#define _NAND_H_

#include <tistdtypes.h>
#include <cslr_emif.h>
#include <cslr_edma.h>
#include <error.h> 

/* NAND FLASH ADDRESS */
#define NANDFLASH_BASE 0x02000000
#define NAND_DATA_ADDR  *( volatile Uint8* )( NANDFLASH_BASE + 0x00 )	
#define NAND_ALE_ADDR	((volatile Uint8*)0x02000008)	
#define NAND_CMD_ADDR	((volatile Uint8*)0x02000010)

#define NAND_TYPE_MASK_0X00000020 (0x00000020)  

/* NAND FLASH COMMANDS */
/* Commands for write and erase not included */
#define NAND_LO_PAGE		0
#define NAND_HI_PAGE		1
#define NAND_READ_30H		0x30
#define	NAND_RDID			0x90
#define NAND_RDIDADD		0x00
#define	NAND_RST			0xFF
#define NAND_RDY			0x40
#define	NAND_STATUS			0x70
#define	NAND_PGRM_START	    0x80
#define NAND_PGRM_END	    0x10
#define NAND_RDY			0x40
#define	NAND_PGM_FAIL	    0x01
#define	NAND_BERASEC1	    0x60
#define	NAND_BERASEC2	    0xD0
#define	NAND_STATUS			0x70

#define NAND_2048 1			//Appro modify
#ifdef NAND_512
#define PAGE_SIZE_SPARE   (528)
#define BYTES_PER_PAGE	  (512)
#elif NAND_2048
#define PAGE_SIZE_SPARE     (2112)
#define BYTES_PER_PAGE		(2048)
#else
#define PAGE_SIZE_SPARE   (528)
#define BYTES_PER_PAGE	  (512)
#endif
                                   
#define MAX_NAND_SUPPORTED	(30) 			/* Change in accordance with the device info */
#define UNKNOWN_NAND		(0xFF)			/* Unknown device id */
#define NAND_NOT_FOUND      (0)
#define NAND_FOUND          (1)

typedef struct  {
    Uint8 dev_id;                 /* DeviceID */
    Uint8 p_cnt;                  /* page count per block */
    Uint16 p_byte1;               /* byte count per page(include redundant parts)*/
    Uint8 blk_shift;			  /* Number of bits by which block address is to be shifted */
    Uint8 addr_cycles;			  /* Number of address cycles */
} NAND_DEVICE_INFO;

/***    NAND_info structure */
typedef struct  {
    Uint8 dev_code;                /* NAND_DevTable index */
    Uint8 page_cnt;                /* page count per block */
    Uint8 addr_cycles;			   /* Number of address cycles */
    Uint8 big_block;			   /* 1 - Big block device, 0 - small block device */
    Uint16 page_size;			   /* Number of bytes in a page including extra data 264/528 */
    Uint8 blk_shift;			   /* Number of bits by which block address is to be shifted */
    Uint8 page_shift;			   /* Number of bits by which 	page address is to be shifted */
} NAND_INFO;

/* Prototypes of functions for NAND flash */
void NANDConfig();
Uint8 NANDInit();
Uint8 NANDGetDetails();
Uint8 NANDReadPage(Uint32 blkAddr, Uint32 page, Uint8 *dest, Bool bECC4);
Uint8 NANDWaitRdy();                                         
Uint8 NANDWaitRdyReg();
Uint8 NANDReadNBytes(Uint32 numBytes, Uint8* destAddr);     

//NAND ECC functions - Only 4 bit ECC supported
// Start 4 Bit ECC detection
void NANDECC4BitStart(void);
// Stop 4 bit ECC by reading NAND4BITECC
void NANDECC4BitStop(void);

//for read used in RBL 
//input : 10 syndrome8 values 
//output : 8 syndrom10 values
void NANDECC4Bit8to10Syndrome(Uint8* syndrome8, Uint16* syndrome10); 

//for write.used in NAND programmer
//input : 8 syndrome10 values 
//output : 10 syndrom8 values
void NANDECC4Bit10to8Syndrome(Uint16* syndrome10, Uint8* syndrome8); 

// Finds the error using the parity bits in spare area and
// syndromes generated. Then correct it.
Uint8 NANDECCFindAndCorrectError(Uint16* syndrome10, 
								  Uint8* bytesToCorrect);

Uint32 NANDWritePage();
Uint32 NANDEraseBlock();

#endif //_NAND_H_

/* 
Only 4 bit ECC used as 16 bit NANDs not supported
For writes:
  1. Set the 4bit_ecc_start bit in the NANDFCR to 1.
  2. Write 518 bytes of data to the NAND Flash.
  3. Read the parity from the NAND4BITECC1-4 registers.
  4. Convert the 10-bit parity values to 8-bits. All 10-bit parity values 
  	 can be concatenated together with 4bit_ecc_val1 as lsb and 4bit_ecc_val8
  	 as msb. Then the concatenated value can be broken down into ten 8-bit values.
  5. Store the parity to spare location in the NAND Flash.

For reads:
  1. Set the 4bit_ecc_start bit in the NANDFCR to 1.
  2. Read 518 bytes of data from the NAND Flash.
  3. Clear the 4bit_ecc_start bit in the NANDFCR by reading any of the NAND4BITECC1-4.
  4. Read the parity stored in the spare location in the NAND Flash.
  5. Convert the 8-bit parity values to 10-bits. Reverse of the conversion that was done during
     writes.
  6. Write the parity values in the NAND4BITECCLOAD. Write each parity value one at a 
     time starting from 4bit_ecc_val8 to 4bit_ecc_val1.
  7. Perform a dummy read to the EMIF Revision Code and Status register. This is required to
     ensure time for syndrome calculation after writing the ECC values in step 6.
  8. Read the syndrome from the NAND4BITECC1-4. A syndrome value of 0 means no bit errors. 
     If the syndrome is non-zero continue to step 8.
  9. Set the addr_calc_st bit in the NANDFCR to 1.
 10. Start another read from NAND if required (a new thread from step 1).
 11. Wait for the corr_state field in the NANDFSR to be equal to 0x1, 0x2, or
     0x3.
 12. The number of errors can be read from the err_num field in the NAND Flash Status
     register.
 13. Read the error address from the NANDFERRADD1-2 registers. Address for the
     errored word is equal to (total_words_read + 7 - address_value). For 518 bytes, the
     address will be equal to (525 - address_value).
 14. Read the error value from the NANDFERRVAL1-2 registers. Errors can be
     corrected by XORing the errored word with the error value from the the NANDERRVAL1-2
*/

