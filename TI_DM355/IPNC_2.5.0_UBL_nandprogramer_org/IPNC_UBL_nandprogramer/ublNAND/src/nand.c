/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: nand.c  												  */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 UBL boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/
#include <bootloader.h>
#include <nand.h>
#include <cslr_gpio.h>

NAND_INFO	gNandInfo;
extern Uint8 gDMA;


/* Details of NAND device sizes supported */
const NAND_DEVICE_INFO   gNandDevInfo[] = {
/*  devid,p_cnt,p_byte1,blk_shift, addr_cycles */
    {0xE3, 16, 	512+16, 12, 		3},	 /* 4 MB */
    {0xE5, 16, 	512+16, 12, 		3},	 /* 4 MB */
    {0xE6, 16, 	512+16, 12, 		3},	 /* 8 MB */
    {0x39, 16, 	512+16, 12, 		3},	 /* 8 MB */
    {0x6B, 16, 	512+16, 12, 		3},	 /* 8 MB */    
    {0x73, 32, 	512+16, 13, 		3},	 /* 16 MB */
    {0x33, 32, 	512+16, 13, 		3},	 /* 16 MB */
    {0x75, 32, 	512+16, 13, 		3},	 /* 32 MB */
    {0x35, 32, 	512+16, 13, 		3},	 /* 32 MB */
	{0x43, 32, 	512+16, 12, 		3},  /* 16 MB 0x1243 */
	{0x45, 32, 	512+16, 12, 		3},  /* 32 MB 0x1245 */
	{0x53, 32, 	512+16, 12, 		3},  /* 16 MB 0x1253 */
	{0x55, 32, 	512+16, 12, 		3},  /* 32 MB 0x1255 */
    {0x76, 32, 	512+16, 13, 		4},	 /* 64 MB */
    {0x36, 32, 	512+16, 13, 		4},	 /* 64 MB */
    {0x79, 32, 	512+16, 13, 		4},	 /* 128 MB */
  	{0x71, 32, 	512+16, 13, 		4},	 /* 256 MB */
	{0x46, 32, 	512+16, 13, 		4},  /* 64 MB 0x1346 */
	{0x56, 32, 	512+16, 13, 		4},  /* 64 MB 0x1356 */
	{0x74, 32, 	512+16, 13, 		4},  /* 128 MB 0x1374 */   
    {0xF1, 64, 	2048+64,22, 		4},  /* 128 MB - Big Block */
    {0xA1, 64, 	2048+64,22, 		4},	 /* 128 MB - Big Block */
 	{0xAA, 64, 	2048+64,22, 		5},	 /* 256 MB - Big Block */
 	{0xDA, 64, 	2048+64,22, 		5},	 /* 256 MB - Big Block */
	{0xAC, 64, 	2048+64,22, 		5},  /* 512 MB - Big Block */
 	{0xDC, 64, 	2048+64,22, 		5},	 /* 512 MB - Big Block */
	{0xB1, 64, 	2048+64,22, 		4},  /* 128 MB - Big Block 0x22B1 */
  	{0xC1, 64, 	2048+64,22, 		4},  /* 128 MB - Big Block 0x22C1 */
  	{0xDC, 64, 	2048+64,22, 		5},  /* Micron  MT29F4G08AAA */
  	{0xD3, 64,	2048+64,22, 		5},  /* Micron  MT29F16G08FAA */
  	{0xD3, 128,	2048+64,23, 		5}   /* Micron  MT29F16G08QAA */
};    

/* Initialze NAND interface and find the details of the NAND used */
Uint8 NANDInit() { 
	Uint32 error;

	/* Aysnchronous bank 1 is used for NAND CS2 
	   Following are the default settings
		CSL_EMIF_0_REGS->AWCCR = 0x80; 
	    CSL_EMIF_0_REGS->AB1CR = 0x3FFFFFFC;

		Only 8 bit NAND supported. So, 4 bit ECC mode used.
	*/

    /* configuring the CS2(bit no.0) for NAND) */
    CSL_EMIF_0_REGS->NANDFCR =  0x1; 
	
	/* Send reset command to NAND */
	*NAND_CMD_ADDR = NAND_RST;

	waitloop(10);
	if(NANDWaitRdy() != E_PASS) {
		error = E_FAIL;
	} else {
		/* Find details of the NAND used */
		error = NANDGetDetails();                
	}
	
	return error;
}
                                         
Uint8 NANDReadNBytes(Uint32 numBytes, Uint8* destAddr) {
	Uint32 i;
	Uint8 status = 0;
	volatile Uint32 count = DMA_TIMEOUT;	

	if (gDMA == 1) {
		/* Use EDMA */
		CSL_EDMACC_0_REGS->PARAMENTRY[0].OPT = 0x00100004;
		CSL_EDMACC_0_REGS->PARAMENTRY[0].SRC = 0x02000000;
		CSL_EDMACC_0_REGS->PARAMENTRY[0].SRC_DST_BIDX = 0x00080000;
		CSL_EDMACC_0_REGS->PARAMENTRY[0].LINK_BCNTRLD = 0x0;
		CSL_EDMACC_0_REGS->PARAMENTRY[0].SRC_DST_CIDX = 0x0;
		CSL_EDMACC_0_REGS->PARAMENTRY[0].CCNT = 1;              

		CSL_EDMACC_0_REGS->PARAMENTRY[0].A_B_CNT = ((numBytes/8)<<16) | (0x8);

		CSL_EDMACC_0_REGS->PARAMENTRY[0].DST = (Uint32)((&destAddr[0]));

		CSL_EDMACC_0_REGS->SHADOW[0].ICR = 0xFFFFFFFF;				
		CSL_EDMACC_0_REGS->SHADOW[0].EESR |= 0x1;
		CSL_EDMACC_0_REGS->SHADOW[0].ESR  |= 0x1;
	    
		/* check for the status to be ready */
		do { 
			status = ((CSL_EDMACC_0_REGS->SHADOW[0].IPR) & (0x00000001));
			count --;
		} while (!status && count > 0);
		
		if(count == 0)
			return E_TIMEOUT;
		else
			return E_PASS;

	} else {
		/* Support for 8-bit NAND only */
	    for (i=0; i < numBytes; i++) {
			/* NANDRead done directly without checking for nand_width */
			*destAddr = (*NAND_DATA_ADDR);
	    	destAddr++;
	    }
		return E_PASS;
	}
}     
                           
/* Get details of the NAND flash used from the id and the table of NAND */
Uint8 NANDGetDetails()
{
	Uint32 deviceID,extID,i;
	Uint32 foundNAND = NAND_NOT_FOUND;
	
	/* read device ID */
	*NAND_CMD_ADDR =  NAND_RDID;/* Read ID Command */
	*NAND_ALE_ADDR =  NAND_RDIDADD;/* Send ID address */
	waitloop(10);

	// EM_CE0 is always driven low. This is required for some manufacturers
	// Enabling GIO37
#if 1
	*PINMUX2 |= 0x40; 
	//drive GIO37 low to keep the EM_CS low during tREAD
	// GIO37 config bank bit11 = 0 for output
	CSL_GPIO_REGS->BANK[1].DIR &= 0xFFFFFFDF;
	// Clear GIO37
	CSL_GPIO_REGS->BANK[1].CLR_DATA |= 0x20; 
#endif	
	/* Always reading the ID alone in 8 bit mode */
	i			= (*NAND_DATA_ADDR);
	deviceID 	= (*NAND_DATA_ADDR);
	extID		= (*NAND_DATA_ADDR);
	extID   	= (*NAND_DATA_ADDR);

	/* Search for the NAND details in the table */
	for(i=0;(i<MAX_NAND_SUPPORTED) && (foundNAND == NAND_NOT_FOUND);i++) {
		if(deviceID == gNandDevInfo[i].dev_id) {
			foundNAND = NAND_FOUND;
			gNandInfo.dev_code = deviceID;
			gNandInfo.page_cnt = gNandDevInfo[i].p_cnt;
			gNandInfo.page_size = gNandDevInfo[i].p_byte1;
			gNandInfo.blk_shift = gNandDevInfo[i].blk_shift;
			gNandInfo.addr_cycles = gNandDevInfo[i].addr_cycles;
			if(gNandInfo.page_size > 528) {
				gNandInfo.page_shift = 16;
				gNandInfo.big_block = 1;
			}
			else {
				gNandInfo.page_shift = 8;
				gNandInfo.big_block = 0;
			}

            if ( deviceID == 0xD3 )
            {
                if ((extID >>= 4) & 2 )
                {
                    /* this is MT29F16G08QAA with block size as 216KB */      
                    UARTSendData((Uint8*)"this is MT29F16G08QAA device\r\n", FALSE);
                    gNandInfo.page_cnt  = 128;
                    gNandInfo.blk_shift = 23;
                }
                else
                {
                    UARTSendData((Uint8*)" this is MT29F16G08FAA device \r\n", FALSE);
                }
            }
		}
	}
	if(foundNAND == NAND_NOT_FOUND) {
		/* Default configuration */
		/* For SAMSUNG/RENESAS and other. We dont support Toshiba */
		gNandInfo.dev_code 		= UNKNOWN_NAND;
		gNandInfo.page_cnt 		= 64;
		gNandInfo.page_size 	= 2112;
		gNandInfo.blk_shift 	= 22;
		gNandInfo.big_block 	= 1;
		gNandInfo.addr_cycles 	= 5;
		gNandInfo.page_shift 	= 16;
	}    
	
	return E_PASS;
}

/* Routine to read a page from NAND */	
Uint8 NANDReadPage(Uint32 blkAddr, Uint32 page, Uint8 *dest) {
	Uint32 i;
	Uint32 addr;
	Uint16 syndrome10[8];
	
	*NAND_CMD_ADDR = NAND_LO_PAGE;
	
	/* Send address of the block + page to be read */
	if(gNandInfo.addr_cycles == 5) {
		addr = (blkAddr << (gNandInfo.blk_shift-16)) | page;
		*NAND_ALE_ADDR = 0x00;
		*NAND_ALE_ADDR = 0x00;
		*NAND_ALE_ADDR = (addr & 0xff);
		*NAND_ALE_ADDR = ((addr & 0xff00)>>8);
		*NAND_ALE_ADDR = ((addr & 0xff0000)>>16);
	}
	else if(gNandInfo.addr_cycles == 4) {
		addr = (blkAddr << gNandInfo.blk_shift) | (page << gNandInfo.page_shift);
		*NAND_ALE_ADDR = (addr & 0xff);
		*NAND_ALE_ADDR = ((addr & 0xff00)>>8);
		*NAND_ALE_ADDR = ((addr & 0xff0000)>>16);
		*NAND_ALE_ADDR = ((addr & 0xff000000)>>24);				
	}
	else {
		addr = (blkAddr << gNandInfo.blk_shift) | (page << gNandInfo.page_shift);
		*NAND_ALE_ADDR = (addr & 0xff);
		*NAND_ALE_ADDR = ((addr & 0xff00)>>8);
		*NAND_ALE_ADDR = ((addr & 0xff0000)>>16);
	}
	
	if(gNandInfo.big_block == 1)	/* Big block NAND */
		*NAND_CMD_ADDR = NAND_READ_30H;

	/* Wait for data to be available */
	waitloop(10);
	if(NANDWaitRdy())
		return E_FAIL;

	if(gDMA == 1) {
		/* EDMACC is used when dma = 1 */
		/* Register settings for TPCC. Using DMA region 0, Channel 11*/ 
		CSL_EDMACC_0_REGS->DRA[0].DRAE  |= 0xFFFFFFFF;
		CSL_EDMACC_0_REGS->DRA[0].DRAEH |= 0xFFFFFFFF;
	}

	/* Read the data to the destination buffer and detect error*/
	if (gNandInfo.page_size == 528) {
		NANDECC4BitStart();
		if (NANDReadNBytes(512,dest) != E_PASS)
			return E_FAIL;
		NANDECC4BitStop();

		// Read ECC from Spare area
		if (NANDReadNBytes(16,&dest[512]) != E_PASS)
			return E_FAIL;			
		NANDECC4Bit8to10Syndrome(&dest[518], &syndrome10[0]);
		if (NANDECCFindAndCorrectError(&syndrome10[0], &dest[0]) != E_PASS)
			return E_FAIL;
	} 
	else if (gNandInfo.page_size == 2112) {
		for(i = 0;i<4;i++) {
			NANDECC4BitStart();
			if (NANDReadNBytes(512,&dest[512*i]) != E_PASS)
				return E_FAIL;
			NANDECC4BitStop();

#if 1
			if (NANDReadNBytes(16,&dest[2048 +(16*i)]) != E_PASS)
				return E_FAIL;
			NANDECC4Bit8to10Syndrome(&dest[2048 + (16*i) + 6], &syndrome10[0]);
			if (NANDECCFindAndCorrectError(&syndrome10[0], &dest[512*i]) != E_PASS)
				return E_FAIL;
#endif
		}                                       
	}	

	/* Wait NAND to be set to ready */
	if(NANDWaitRdyReg() != E_PASS)
		return E_FAIL;	
		
	return E_PASS;
}

/* Wait for the status to be ready 
Polling for the pad_wait_i(bit 0) of NANDFSR */
Uint8 NANDWaitRdy() {
	volatile Uint32 cnt;
	Uint32 status;
  	cnt = NAND_TIMEOUT;
  	
  	do {
    	status = (CSL_EMIF_0_REGS->NANDFSR & 1);
    	cnt--;
  	} while((cnt>0) && !status);

  	if(cnt == 0)
  		return E_FAIL;

 	return E_PASS;
}


/* Wait for the status to be ready in register 
 There were some problems reported in DM320 with Ready/Busy pin
 not working with all NANDs. So this check has also been added */
Uint8 NANDWaitRdyReg() {
	volatile Uint32 cnt;
	Uint32 status;
    *NAND_CMD_ADDR = NAND_STATUS;
    status = *NAND_DATA_ADDR;
    cnt = NAND_TIMEOUT;

  	do {
	    *NAND_CMD_ADDR = NAND_STATUS;
	    status = *NAND_DATA_ADDR;
    	cnt--;
  	} while((cnt>0) && !status);
    
	if(cnt == 0)
		return E_FAIL;  		

	return E_PASS;
}


// Start 4 Bit ECC detection
void NANDECC4BitStart(void) {
	// bit[5,4] = [00] for NAND in CS2
	// Set bit12 for 4bit_ecc_start
	CSL_EMIF_0_REGS->NANDFCR |= 0x1000;
}

// Stop 4 bit ECC by reading NAND4BITECC
void NANDECC4BitStop(void) {
	Uint32 temp;
	temp = CSL_EMIF_0_REGS->NAND4BITECC1;
}

//for read used in RBL 
//input : 10 syndrome8 values 
//output : 8 syndrom10 values
void NANDECC4Bit8to10Syndrome(Uint8* syndrome8, Uint16* syndrome10) {
	syndrome10[0] = ((syndrome8[1]& 0x3)<<8  | syndrome8[0] );
	syndrome10[1] = ((syndrome8[2]& 0xF)<<6  |(syndrome8[1]& 0xFC)>>2 );
	syndrome10[2] = ((syndrome8[3]& 0x3F)<<4 |(syndrome8[2]& 0xF0)>>4 );
	syndrome10[3] = ( syndrome8[4]<<2        |(syndrome8[3]& 0xC0)>>6 );
	syndrome10[4] = ((syndrome8[6]& 0x3)<<8  | syndrome8[5] );
	syndrome10[5] = ((syndrome8[7]& 0xF)<<6  |(syndrome8[6]& 0xFC)>>2 );
	syndrome10[6] = ((syndrome8[8]& 0x3F)<<4 |(syndrome8[7]& 0xF0)>>4 );
	syndrome10[7] = ( syndrome8[9]<<2        |(syndrome8[8]& 0xC0)>>6 );
}

//for write.used in NAND programmer
//input : 8 syndrome10 values 
//output : 10 syndrom8 values
void NANDECC4Bit10to8Syndrome(Uint16* syndrome10, Uint8* syndrome8) {
//Convert eight 10-bit codes to ten 8-bit words:
	syndrome8[0] =   syndrome10[0] & 0xFF;
	syndrome8[1] = ((syndrome10[1] & 0x3F) << 2) | ((syndrome10[0] & 0x300) >> 8);
	syndrome8[2] = ((syndrome10[2] & 0x0F) << 4) | ((syndrome10[1] & 0x3C0) >> 6);
	syndrome8[3] = ((syndrome10[3] & 0x03) << 6) | ((syndrome10[2] & 0x3F0) >> 4);
	syndrome8[4] = ((syndrome10[3] & 0x3FC)>> 2);
	syndrome8[5] =   syndrome10[4] & 0xFF;
	syndrome8[6] = ((syndrome10[5] & 0x3F) << 2) | ((syndrome10[4] & 0x300) >> 8);
	syndrome8[7] = ((syndrome10[6] & 0x0F) << 4) | ((syndrome10[5] & 0x3C0) >> 6);
	syndrome8[8] = ((syndrome10[7] & 0x03) << 6) | ((syndrome10[6] & 0x3F0) >> 4);
	syndrome8[9] = ((syndrome10[7] & 0x3FC)>> 2);
}

// Finds the error using the parity bits in spare area and
// syndromes generated. Then correct it.
Uint8 NANDECCFindAndCorrectError(Uint16* syndrome10, 
								  Uint8* bytesToCorrect) {

   Uint8  status, numE;
   Int32 i;
   Uint32 temp;
   Uint16 addOffset, corrValue;

   volatile Uint32 cnt;

   // Load the syndrome10 (from 7 to 0) values 
   for(i=7;i>=0;i--)
		CSL_EMIF_0_REGS->NAND4BITECCLOAD = syndrome10[i];
   
   // Read the EMIF status and version (dummy call) 
   temp = CSL_EMIF_0_REGS->ERCSR;

   // Check if error is detected
   if( ((CSL_EMIF_0_REGS->NAND4BITECC1&0x3FF03FF)| 
     	(CSL_EMIF_0_REGS->NAND4BITECC2&0x3FF03FF)| 
		(CSL_EMIF_0_REGS->NAND4BITECC3&0x3FF03FF)| 
		(CSL_EMIF_0_REGS->NAND4BITECC4&0x3FF03FF)  ) == 0) {
   		return E_PASS;
   }

   // Calcuate the address if error detected
   // Set bit13 of NANDFCR
	CSL_EMIF_0_REGS->NANDFCR |= 0x2000;
	
	// bit 11:8 gives the corr_state. After correction the states can
	// be 0000, 0001, 0010, 0011. All other states are work-in-progress
	// So, checking for bit 11:10
  	cnt = NAND_TIMEOUT;
  	do {
    	status = (CSL_EMIF_0_REGS->NANDFSR & 0xC00);
    	cnt--;
  	} while((cnt>0) && status);

	// After bit 11:10 become 00
	if( ((CSL_EMIF_0_REGS->NANDFSR & 0xF00)>>8) == 1)
		return E_FAIL;
	else {
		// Error detected and address calculated
		// Number of errors corrected 17:16
		numE = (CSL_EMIF_0_REGS->NANDFSR & 0x30000) >> 16;
		// bit 9:0
		addOffset = 519 - CSL_EMIF_0_REGS->NANDERRADD1 & (0x3FF);
		corrValue = CSL_EMIF_0_REGS->NANDERRVAL1 & (0x3FF);
		bytesToCorrect[addOffset] ^= (Uint8)corrValue;

		if(numE == 0)
			return E_PASS;
		else {
			// bit 25:16
			addOffset = 519 - ( (CSL_EMIF_0_REGS->NANDERRADD1 & (0x3FF0000))>>16 );
			corrValue = CSL_EMIF_0_REGS->NANDERRVAL1 & (0x3FF);
			bytesToCorrect[addOffset] ^= (Uint8)corrValue;				

			if(numE == 1)
				return E_PASS;
			else {
				// bit 9:0
				addOffset = 519 - CSL_EMIF_0_REGS->NANDERRADD2 & (0x3FF);
				corrValue = CSL_EMIF_0_REGS->NANDERRVAL2 & (0x3FF);
				bytesToCorrect[addOffset] ^= (Uint8)corrValue;
				
				if (numE == 2)
					return E_PASS;
				else {
					// bit 25:16
					addOffset = 519 - ( (CSL_EMIF_0_REGS->NANDERRADD2 & (0x3FF0000)>>16) );
					corrValue = CSL_EMIF_0_REGS->NANDERRVAL2 & (0x3FF);
					bytesToCorrect[addOffset] ^= (Uint8)corrValue;
				}
			}
		}
	}

	return E_PASS;
}




