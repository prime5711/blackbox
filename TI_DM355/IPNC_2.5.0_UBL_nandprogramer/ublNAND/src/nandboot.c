/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: nandboot.c	  												  */
/*	PURPOSE	: NAND boot file						                              */
/*  PROJECT	: DM355 UBL boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#include <bootloader.h>
#include <nand.h>

extern NAND_INFO	gNandInfo;			 /* information about the NAND flash present */
extern Uint32 gEntryPoint;
extern int test_mode;
Uint8 gDMA = 0;
Uint8 copygDMA;
Uint8 buff[2112];

Uint8 MagicSwitch(Uint32 magicNum) {

	switch(magicNum) {
		case APP_MAGIC_SAFE:   
		    gDMA = 0;
		    DisableICache();
			// For 8 bit NAND Only. Disable Emif Fast mode
			CSL_EMIF_0_REGS->AB1CR = 0x3FFFFFFC;
			break;
			
		case APP_MAGIC_DMA:
			gDMA = 1;		
			DisableICache();
			break;
			
		case APP_MAGIC_IC:              
			gDMA = 0;
			EnableICache();			
			break;
			
		case APP_MAGIC_FAST:
			gDMA = 0;
		    // For 8 bit NAND Only. Enable EMIF Fast mode
			CSL_EMIF_0_REGS->AWCCR = 0x200000FF;
			CSL_EMIF_0_REGS->AB1CR = 0x00262200;
			DisableICache();
			break;			
			
		case APP_MAGIC_DMA_IC:
			gDMA = 1;
			EnableICache();
			break;
			
		case APP_MAGIC_DMA_IC_FAST:
			gDMA = 1;
		    // For 8 bit NAND Only. enable EMIF Fast Mode
			CSL_EMIF_0_REGS->AWCCR = 0x100000FF;
			CSL_EMIF_0_REGS->AB1CR = 0x00262200;
 			EnableICache();
			break;		
			
		default:          
			return E_FAIL;
	}	

	return E_PASS;
}

/* Function to find out where the APP is and copy to IRAM */
Uint8 NANDBoot() {

    Uint32 endBlockNum;
	Uint32 validMagicNum;  /* verifying the magic number stored in an application descriptor            */
	Uint8 *rxBuf;		   /* receive buffer where the data read from the block N0:49/50 of Nand device */
	APP_BOOT AppBootDesc ; /* structure for holding details about Application stored in NAND            */
    Uint32 numPage;		   /* Number of pages based on the applicaion size                              */ 
    Uint8 flag=0,cnt;          /* flag to verify certain conditions                                         */
	Uint8 badBlockdata1,badBlockdata2;  /* Verifying the bad block locations                            */
	Uint32 count,blockNum;
	Uint32 magicNum;
	Uint32 pageSize,page;
	Uint32 readError=E_FAIL;

	rxBuf = (Uint8*)BL_MEMDDR;

	/* Block number for reading the application descriptor from NAND */ 
	blockNum = APP_DESC_START_BLOCK_NUM;	
	endBlockNum   = APP_DESC_END_BLOCK_NUM;
	validMagicNum = APP_MAGIC_NUMBER_VALID;
	if(test_mode==1){
		blockNum = 5;	
		endBlockNum   = 7;
		validMagicNum = 0xC1ACED00;
	}

	/* initialize NAND flash interface and get details about NAND flash */
   readError=NANDInit();
   if(readError!=E_PASS)
     return E_FAIL;

   	/* Calculate the total number of pages based on an applicaton size */    						
	if(gNandInfo.page_size == 2112)
		pageSize = 2048;
    else 
		pageSize = 512;

	
	// When GIO0 is 1 => Normal NAND
	// When GIO0 is 0 => Fast NAND
	// GIO0 is sampled at reset and reflected in the BOOTCFG register
	//fastBoot = ((*BOOTCFG) & BOOTCFG_MASK_0x00000100);

    //UARTSendData((Uint8*)"\r\nread 49 and 50 blocks\r\n", FALSE);
	NAND_startAgain:
	/* read the application descriptor from the block 49 or 50 of Page 0 */
		for(count = blockNum; count <= endBlockNum;count++){				   
			readError = NANDReadPage(count,0,rxBuf);
			if(readError!= E_PASS){
				if(count<endBlockNum)
				 continue;
				 else
					return E_FAIL;
			}
		    

			badBlockdata1= rxBuf[pageSize];
	        badBlockdata2= rxBuf[pageSize+5];

      /* For a small block NAND device, the 517th locations should hold 0xFF value, 
	   	else for a big block NAND device, the 2048th and 2053rd locations should 
	    hold 0xFF value */
#if 0 	
 	  if(badBlockdata2!=0xFF) /* Bad block is found */
	    continue;  
			
	  if(pageSize==2048){
	    if(badBlockdata1 !=0xFF)/* Bad block is found */
          continue;
	  }
#endif		
			magicNum = *(((Uint32 *)rxBuf));

			/* Magic number found */
			if((magicNum & 0xFFFFFF00) == validMagicNum){
				if(test_mode)
					UARTSendData((Uint8*)"UBL: detected valid Diagnostic magic number\r\n", FALSE);
				else
                	UARTSendData((Uint8*)"UBL: detected valid U-BOOT magic number\r\n", FALSE);
				flag=1;
				break;
			}else
				continue;			
 		}

	/* applicaton descriptor is not found in NAND */
		if(flag!=1)
        {
            UARTSendData((Uint8*)"UBL: Failed to read app descriptor\r\n", FALSE);
			return E_FAIL;
        }
	
	/* Assign application descriptor to a structure */
	AppBootDesc.block = *(((Uint32 *)(&rxBuf[4])));      /* The first "long" is the starting block number where actual application is stored */
	AppBootDesc.destination = *(((Uint32 *)(&rxBuf[8])));	 /* The second "long" is the Application copy start address in DDR */
 	AppBootDesc.entrypoint = *(((Uint32 *)(&rxBuf[12])));	 /* The third "long" is the Entry point of the main application */
	AppBootDesc.pages = *(((Uint32 *)(&rxBuf[16])));	 /* The fourth "long" is the Application size in bytes */
	AppBootDesc.compression = *(((Uint32 *)(&rxBuf[20])));	 /* The fourth "long" is the flag for de-compressing the application */


	/* Assigning application copy start address in DDR */
	rxBuf = (Uint8*)AppBootDesc.destination;

	/* initialize block and page number to be used for read operation */
	blockNum = AppBootDesc.block;
	numPage = AppBootDesc.pages;  // application size in pages
	page = 0;    //Assume that the application starts from page-0 of the block



NAND_retry:
	if(MagicSwitch(magicNum & 0xFF) != E_PASS)
    {
        UARTSendData((Uint8*)"UBL: Wrong magic number\r\n", FALSE);
		return E_FAIL;
    }

	/* Actual application copy starts from NAND to DDR memory  */
	for(count=0;count<numPage;count++){
		/* if page goes beyond max number of pages increment block number and set page number to '0' */
		if(page >= gNandInfo.page_cnt){
			page = 0;
			blockNum++;
		}	
NEXT_BLOCK:				
	
		readError = NANDReadPage(blockNum,page,(&rxBuf[count*pageSize])); /* Copy the data */
	
		if(readError != E_PASS){
			if(magicNum == APP_MAGIC_SAFE) {
				if(blockNum == APP_DESC_END_BLOCK_NUM) {
					return E_FAIL; /* NAND boot failed and return fail to main */
				} else {
					/* Search for the Magic number in next block */
					blockNum++;
					goto NAND_startAgain;
				}
			}
		    else {
		        magicNum = APP_MAGIC_SAFE;
		        page = 0;
				goto NAND_retry;
			}
		}		

	/* Do Bad block management. Will check if the block being read is bad block and appropriately skip them */
#if 0
    	if(page == 0){	
		  cnt = 0;
		  copygDMA = gDMA;
          gDMA = 0;
          while(cnt < 2){
		    
		    readError = NANDReadPage(blockNum,cnt++,(&buff[0])); /* Copy the data */
			if(readError != E_PASS)
			  return E_FAIL;

		    badBlockdata1= buff[pageSize];
		 	badBlockdata2= buff[pageSize+5];

    		/* For a small block NAND device, the 517th locations should hold 0xFF value, 
	    	 else for a big block NAND device, the 2048th and 2053rd locations should 
		     hold 0xFF value */
 		    if(badBlockdata2!=0xFF) /* Bad block is found */{
			  blockNum++;	/* Else perform the SKIP method */
			  goto NEXT_BLOCK;
			}
			
			if(pageSize==2048){
			  if(badBlockdata1 !=0xFF)/* Bad block is found */{
			    blockNum++;	/* Else perform the SKIP method */
			    goto NEXT_BLOCK;
			  }
			}            
          }  
		  /* block is qualified */
          gDMA = copygDMA;
		  page++;
		  continue;		    
		}  
#endif
		/* copy the remaining pages of the block */
		page++;	

	  } /* application is copied completely to DDRAM */


	/* (DM350_TO)De-compression should start */
	/*if(	AppBootDesc.compression == 1)
	{
		// De-compress the data and copy to DDR memory	
	}
    */
	
	gEntryPoint = AppBootDesc.entrypoint;
	return E_PASS;
}

