/************************************************************************/
/* FILE:	nandProgrmmer.c			 				                    */
/* PURPOSE: Implementation of Nand programmer for DM350             	*/
/* PROJECT: DM350 boot-loader 		                                    */
/* AUTHOR: 				  				                                */
/* DATE: May-3-2006                                                     */
/************************************************************************/
#include <stdio.h>
#include <tistdtypes.h>
#include <nand.h>

#define _UBLFILE_		// write ubl binary file to nand flash. #define UBL must be uncommented
#define _APPFILE_		//write application bin file to flash. You will require to uncomment #define APP
#define _DIAGFILE_		//write application bin file to flash. You will require to uncomment #define APP

#define UBL				// writes UBL to NAND.
#define APP				// writes UBOOT (application) to NAND
#define DIAG			// Diagnostic mode

extern NAND_INFO gNandInfo;
extern Uint32 gTotalBlocks;

#define UBL_START_BLOCK_NUM 1 
#define UBL_END_BLOCK_NUM   7

#define NAND_TOTAL_BLOCKS 2048 //4096  //Appro modify
/* searches in Block 8,9 */
#define APP_DESC_START_BLOCK_NUM 8
#define APP_DESC_END_BLOCK_NUM   9 

#define DIAG_DESC_START_BLOCK_NUM 5
#define DIAG_DESC_END_BLOCK_NUM   7 

/* Some of the parameters are modified runtime */
unsigned int ubldesc [] = {
0xA1ACED00, //Magic number 
0x00000020, // Entry point for the UBL
0x00000007, // No. of pages for the UBL code
0x1, // Starting block of UBL in NAND
0x1 // Starting page of UBL in NAND
};

/* Some of the parameters are modified runtime */
unsigned int appdesc [] = {
0xB1ACED55, //Magic number 
0x0000000A, // Start block in NAND
0x81080000, // Start address in DDR
0x81080000, // Entry point
0x00000200, // No. of pages for the complete Application
0x00000000  //compression flag
};

/* Some of the parameters are modified runtime */
unsigned int diagdesc [] = {
0xC1ACED55, //Magic number 
0x00000018, // Start block in NAND
0x81000000, // Start address in DDR
0x81000040, // Entry point
0x00000200, // No. of pages for the complete Application
0x00000000  //compression flag
};

Uint8 NANDCheckBadBlock(Uint32 blockNum, Uint32 pageSize)
{
	Uint8 nandRx[PAGE_SIZE_SPARE];
	Uint8 error=(Uint8)E_FAIL;

	error = NANDEraseBlock(blockNum);        
	if ( error != E_PASS )
    	printf(" NANDEraseBlock fails on block %d\n", blockNum);
    else
	   // printf(" NANDEraseBlock done on block %d\n", blockNum);                 
	if(error !=E_PASS && error != E_ERROR)
		return E_FAIL;
	if((error=NANDReadPage(blockNum,0,nandRx,TRUE))!=E_PASS)
	{
		printf(" NANDReadPage fails on block %d\n",blockNum);
		return E_FAIL;
	}
    else 
    	//printf(" NANDReadPage done on block %d\n",blockNum);

	if(gNandInfo.page_size == 528)
	{
		if(nandRx[pageSize+5]!=0xFF)
		{
			printf(" Block number %d is a BAD BLOCK\n",blockNum);
			return E_FAIL;
		}
	}
	else
	{					
		if((nandRx[pageSize]!=0xFF) || (nandRx[pageSize+5]!=0xFF))
		{
			printf(" Block number %d is a BAD BLOCK",blockNum);
			return E_FAIL;
		}			
	}
	return E_PASS;
}

Uint8 NANDVerifyPage(Uint8 *RxData, Uint8 *TxData, Uint32 pageSize)
{
	Uint32 i;	
	for ( i = 0; i< pageSize; i++ )
	{
    	if ( RxData[i] != TxData[i] )
        {
           	printf(" RxData[%d]=0x%x is not consistent with TxData[%d]=0x%x\n",i,RxData[i],i,TxData[i]);
	        return E_FAIL;
	   	}
    }
	return E_PASS;
}

/* function call for programming the NAND memory*/	
Uint8 NANDProg() {
	Uint8 nandTx[PAGE_SIZE_SPARE],nandRx[PAGE_SIZE_SPARE]; //Buffer for receiving/transmitting the page content
	Uint8 error=(Uint8)E_FAIL;
	Uint16 numPage=0;
	Uint32 blockNum = 0, page=0,pageSize=0, nextBlock=0;
	Uint32 entryPoint,count;
#ifdef UBL
	Uint8 *ubldatPtr= NULL; 			/* pointer for an UBL application 	  */
#endif
#ifdef APP
	Uint8 *appdatPtr= NULL;
#endif
#ifdef DIAG
	Uint8 *diagdatPtr= NULL;
#endif
    Bool  bSwitched = FALSE;
		
#ifdef _UBLFILE_
	//Added code to read UBL from bin file and write to flash
	////////////////////////////////////
	FILE 	*fPtr = NULL;
	Int8	fileName[256] = {0};
	Uint8 	*ubldat = NULL;
	unsigned int fileSize = 0;
	///////////////////////////////////
#endif // _UBLFILE_

#ifdef _APPFILE_ 
	//Added code to read app from bin file and write to flash
	////////////////////////////////////
	FILE 	*fwtrPtr = NULL;
	Int8	wtrFileName[256] = {0};
	unsigned int wtrFileSize = 0;
	Uint8 	*appdat = NULL;
#endif

#ifdef  _DIAGFILE_
	//Added code to read app from bin file and write to flash
	////////////////////////////////////
	FILE 	*fwtrPtr1 = NULL;
	Int8	wtrFileName1[256] = {0};
	unsigned int wtrFileSize1 = 0;
	Uint8 	*appdat1 = NULL;
#endif


	printf("Started Nand programming\n");

	/*Initialize NAND and get details such as page size etc*/
	if((error=NANDInit())!=E_PASS)
		return E_FAIL;	
	printf("NANDInit() Successful\n"); 
#if 0
	if((error=NANDReadPage(10,0,nandRx,TRUE))!=E_PASS)
	{
		printf(" NANDReadPage fails on block %d\n",blockNum);
		return E_FAIL;
	}
	if((error=NANDCheckBadBlock(20, 512)) == E_FAIL){
		printf(" NANDReadPage fails on block %d\n",blockNum);
		return E_FAIL;
	}

	if((error=NANDWritePage(20, 0, nandRx, TRUE))!=E_PASS)
    {
		printf(" NANDWriteBlock fails on block %x\n",blockNum);
		goto Retry_UBL_program;
    }
	while (1);
#endif

#if 1
	for (count = 0; count < NAND_TOTAL_BLOCKS; count++)
	{
		error = NANDEraseBlock(count);  
    }
#endif


#ifdef _UBLFILE_
	//Read UBL binary from file instead of reading from buffer
	/////////////////////////////////////////////////////////
	printf("Enter the UBL File Name\n");
	scanf("%s", fileName);

	fflush(stdin);

	// Open an File from the hard drive
	fPtr = fopen(fileName, "rb");
	if(fPtr == NULL)
	{
		printf(" File %s Open failed\n", fileName);
		return E_FAIL;
	}
	fileSize = 0;
	// Read file size
	fseek(fPtr,0,SEEK_END);
	fileSize = ftell(fPtr);
	if(fileSize == 0)
	{
		printf(" File read failed.. Closing APP\n");
		fclose (fPtr);
		return E_FAIL;
	}
	fseek(fPtr,0,SEEK_SET);
	ubldat = (Uint8 *)malloc(fileSize);
	if(!ubldat)
		return E_FAIL;
	if(fileSize != fread(ubldat, 1, fileSize, fPtr))
	{
		printf(" Warning !!! File Size mismatch\n");
		fclose(fPtr);
		return E_FAIL;
	}
	if(fPtr)
		fclose (fPtr);
	///////////////////////////////////////////////////////////////
#endif	//_UBLFILE_

    if ( ! gTotalBlocks )
    {
        gTotalBlocks = NAND_TOTAL_BLOCKS;
    }
//	for (count = 0; count < gTotalBlocks; count++)
	for (count = 0; count < 40; count++)
	{
		if((error=NANDCheckBadBlock(count, pageSize)) == E_FAIL)
			printf("Block %d is a bad block\n", count);
	}
	for (count = 2040; count < 2048; count++)
	{
		if((error=NANDCheckBadBlock(count, pageSize)) == E_FAIL)
			printf("Block %d is a bad block\n", count);
	}
#ifdef UBL 
	memset(nandTx, 0, PAGE_SIZE_SPARE);
	/* writing 0xff to reserve area of the 8 bit NAND flash for bad block identification */
	if(gNandInfo.page_size == 2112)
	{
		pageSize=2048;
		//wordsPerpage=512;
		nandTx[pageSize] = 0xFF;	/* If not BAD block */
		nandTx[pageSize+5] = 0xFF;
	}
	else 
	{
		pageSize=512;
		//wordsPerpage=128;
		nandTx[pageSize+5] = 0xFF;  /* If not BAD block */
	}
	
	// Determine # of pages.
	ubldesc[2] = fileSize/pageSize;
	if((ubldesc[2]*pageSize) < fileSize)
		ubldesc[2]++;
	printf("UBL: Number of pages: %d\n", ubldesc[2]);
	entryPoint = ubldesc[1];
	if((entryPoint < 0x0020) || (entryPoint> 0x781c)) 
	{
		printf(" Entry point is not in a valid range  ");
		return E_FAIL;
	}
	/* the total number of pages required for programming the UBL code */
    numPage = ubldesc[2];
	if(((numPage*pageSize)> 30720) || (numPage== 0))
		return E_FAIL;
	/* UBL Descriptor copy starts now */
	printf("UBL Descriptor copy starts now \n");

    if ( gNandInfo.dev_code == 0xD3 && gNandInfo.page_cnt == 128 )
    {
        printf("This is MT29F16G08QAA NAND. Change the blocksize!\n");
        /* RBL doesn't support this device. Let's pretend the block size is 
           2KB for writing UBL
         */
        gNandInfo.blk_shift = 22;
        gNandInfo.page_cnt  = 64;
        bSwitched = TRUE;
    }
Retry_UBL_program:

	blockNum++;
	if(blockNum > UBL_END_BLOCK_NUM)
		return E_FAIL;
	/* UBL programming starts now  */
	ubldesc[3] = blockNum;
	printf(" BlockNum: %d\n", blockNum);

	for(count=0;count<5;count++)  /* copy the UBL descriptor to NAND */
		((Uint32 *)nandTx)[count] = ubldesc[count];		
	
	if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
		goto Retry_UBL_program;
	if((error=NANDWritePage(blockNum, 0, nandTx, TRUE))!=E_PASS)
    {
		printf(" NANDWriteBlock fails on block %x\n",blockNum);
		goto Retry_UBL_program;
    }
    else
    	//printf(" NANDWriteBlock done on block %x\n",blockNum);
	waitloop(200);
	if((error=NANDReadPage(blockNum, 0, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
	{
		printf(" NANDReadPage fails on block %x\n",blockNum);
		goto Retry_UBL_program;
	} 
    else
    	//printf(" NANDReadPage done on block %x\n",blockNum);
	if((error=NANDVerifyPage(nandRx, nandTx, pageSize)) == E_FAIL)
		goto Retry_UBL_program;	
	printf("UBL descriptor is successfully programmed, BlockNum : %d\n",blockNum);	

   	/* UBL code copy starts now */
	printf("UBL code copy starts now \n");

	/* Actual  UBL programming starts ... */
	// blockNum is already set to be 1 or where ever the descriptor got copied.
	page = ubldesc[4];  // starting page no. of UBL
	for(count = 0; count < numPage; count++) 
	{
		/* if page goes beyond max number of pages, increment block number and set page number to '0' */
		if(page >= gNandInfo.page_cnt) 
		{
			blockNum++;
			page = 0;
			if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
				goto Retry_UBL_program;
		}
		ubldatPtr = &ubldat[count*pageSize];
		if((error=NANDWritePage(blockNum,page,(Uint8*)ubldatPtr,TRUE))!= E_PASS)
			goto Retry_UBL_program; 
		else
    		//printf(" NANDWriteBlock done on block %x\n",blockNum);
		waitloop(200);
		if((error=NANDReadPage(blockNum, page, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
        {
            printf(" NANDReadPage fails 2 on block %d\n",blockNum);
			goto Retry_UBL_program;
        } 
        else
            //printf(" NANDReadPage done on block %d\n",blockNum);
		if((error=NANDVerifyPage(nandRx, ubldatPtr, pageSize)) == E_FAIL)
			goto Retry_UBL_program;	
		page++;		
	}  
	free(ubldat);
	/* Completes the UBL programming  */
	printf("UBL code programming is completed\n"); 
#endif

#ifdef _APPFILE_
	//Read U-Boot binary from file instead of reading from buffer
	/////////////////////////////////////////////////////////
	printf("Enter the UBOOT File Name\n");
	scanf("%s", wtrFileName);
	fflush(stdin);
	fwtrPtr = fopen(wtrFileName, "rb");
	if(fwtrPtr == NULL)
	{
		printf(" File %s Open failed\n", wtrFileName);
		return E_FAIL;
	}
	wtrFileSize = 0;
	// Read file size
	fseek(fwtrPtr,0,SEEK_END);
	wtrFileSize = ftell(fwtrPtr);
	if(wtrFileSize == 0)
	{
		printf(" File read failed.. Closing APP\n");
		fclose (fwtrPtr);
		return E_FAIL;
	}
	fseek(fwtrPtr,0,SEEK_SET);
	appdat = (Uint8 *)malloc(wtrFileSize);
	if(appdat == NULL)
	{
		printf(" Unable to allocate memory %d\n",wtrFileSize);
		fclose(fwtrPtr);
		return E_FAIL;
	}		
	if(wtrFileSize != fread(appdat, 1, wtrFileSize, fwtrPtr))
	{
		printf(" Warning !!! File Size mismatch\n");
		free(appdat);
		fclose(fwtrPtr);
		return E_FAIL;
	}
	if(fwtrPtr)
		fclose (fwtrPtr);
#endif
   
#ifdef APP
    if ( bSwitched )
    {
        printf("This is MT29F16G08QAA NAND. Change the blocksize back!\n");
        /* RBL doesn't support this device. Let's pretend the block size is 
           2KB for writing UBL
         */
        gNandInfo.blk_shift = 23;
        gNandInfo.page_cnt  = 128;
        /* keep the first block from being erased */
        NANDSetBadBlock(0);
    }
	/* writing 0xff to reserve area of the 8 bit NAND flash for bad block identification */
	if(gNandInfo.page_size == 2112)
	{
		pageSize=2048;
		//wordsPerpage=512;
		nandTx[pageSize] = 0xFF;	/* If not BAD block */
		nandTx[pageSize+5] = 0xFF;
	}
	else 
	{
		pageSize=512;
		//wordsPerpage=128;
		nandTx[pageSize+5] = 0xFF;  /* If not BAD block */
	}
	// Determine # of pages
	appdesc[4] = wtrFileSize/pageSize;
	if((appdesc[4]*pageSize) < wtrFileSize)
		appdesc[4]++;
	printf("U-Boot: Number of pages: %d\n", appdesc[4]);

	// Check the entry addresses TODO
	blockNum = APP_DESC_START_BLOCK_NUM-1; // Refer to UBL code.
	/* the total number of pages required for programming the U-Boot code */
    numPage = appdesc[4];

	/* U-Boot Descriptor copy starts now */
	printf("U-Boot Descriptor copy starts now \n");
	memset(nandTx, 0, PAGE_SIZE_SPARE);

Retry_program:

	blockNum++;
	if(blockNum > APP_DESC_END_BLOCK_NUM)
		return E_FAIL;
	printf(" BlockNum: %d\n", blockNum);

	for(count=0;count<6;count++)  /* copy the U-Boot descriptor to NAND */
		((Uint32 *)nandTx)[count] = appdesc[count];

	if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
	{
		printf("NANDCheckBadBlock() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program;
	}	
	if((error=NANDWritePage(blockNum, 0, nandTx,TRUE))!=E_PASS)
    {
		printf("NANDWritePage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program;
	}	
    else
    	printf(" NANDWriteBlock done on block %d\n",blockNum);

	waitloop(200);

	if((error=NANDReadPage(blockNum, 0, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
	{
		printf("NANDReadPage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program;
	} 
    else
    	//printf(" NANDReadPage done on block %d\n",blockNum);
	if((error = NANDVerifyPage(nandRx, nandTx, pageSize)) == E_FAIL)
	{
		printf("NANDVerifyPage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program;
	}
	printf("UBOOT descriptor is successfully programmed ,BlockNum : %d\n",blockNum);
	
	/* Calculate the total number of pages required for burning the actual application */
    numPage = appdesc[4];
	blockNum = appdesc[1]-1; // Block number from the descriptor.
	page = 0;
	nextBlock = 1;
	/* Actual application programming started now */
	printf("UBOOT programming starts now pages %d, blockNum %d\n", numPage, blockNum);

	for(count = 0; count < numPage; count++) 
	{
Retry_UBoot_Program:
		if(nextBlock)
		{
			blockNum++;
			if(blockNum > 20)
				return E_FAIL;
			page = 0;
			if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
				goto Retry_UBoot_Program;
			nextBlock = 0;
		}	
		/* if page goes beyond max number of pages, increment block number and set page number to '0' */
		if(page >= gNandInfo.page_cnt)
		{
			nextBlock = 1;
			goto Retry_UBoot_Program;
		}
		appdatPtr = &appdat[count*pageSize];
		if((error=NANDWritePage(blockNum,page,(Uint8*)appdatPtr,TRUE))!= E_PASS)
		{
			nextBlock = 1;
			goto Retry_UBoot_Program;
		}
		else
	    	//printf(" NANDWriteBlock done on block %d\n",blockNum);
		waitloop(200);

		if((error=NANDReadPage(blockNum, page, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
        {
            printf(" NANDReadPage fails on block %d\n",blockNum);
			nextBlock = 1;
			goto Retry_UBoot_Program;
        } 
        else
            //printf(" NANDReadPage done on block %d\n",blockNum);
		if((error=NANDVerifyPage(nandRx, appdatPtr, pageSize)) == E_FAIL)
		{
			nextBlock = 1;
			goto Retry_UBoot_Program;
		}
		page++;		
	}  
	free(appdat);
	/* Completes the U-Boot programming  */
	printf("UBOOT programming is completed \n");
#endif

#ifdef _DIAGFILE_
	//Read U-Boot binary from file instead of reading from buffer
	/////////////////////////////////////////////////////////
	printf("Enter the Diagnostic File Name\n");
	scanf("%s", wtrFileName1);
	fflush(stdin);
	fwtrPtr1 = fopen(wtrFileName1, "rb");
	if(fwtrPtr1 == NULL)
	{
		printf(" File %s Open failed\n", wtrFileName1);
		return E_FAIL;
	}
	wtrFileSize1 = 0;
	// Read file size
	fseek(fwtrPtr1,0,SEEK_END);
	wtrFileSize1 = ftell(fwtrPtr1);
	if(wtrFileSize1 == 0)
	{
		printf(" File read failed.. Closing APP\n");
		fclose (fwtrPtr1);
		return E_FAIL;
	}
	fseek(fwtrPtr1,0,SEEK_SET);
	appdat1 = (Uint8 *)malloc(wtrFileSize1);
	if(appdat1 == NULL)
	{
		printf(" Unable to allocate memory %d\n",wtrFileSize1);
		fclose(fwtrPtr1);
		return E_FAIL;
	}		
	if(wtrFileSize1 != fread(appdat1, 1, wtrFileSize1, fwtrPtr1))
	{
		printf(" Warning !!! File Size mismatch\n");
		free(appdat1);
		fclose(fwtrPtr1);
		return E_FAIL;
	}
	if(fwtrPtr1)
		fclose (fwtrPtr1);
#endif
   
#ifdef DIAG
    if ( bSwitched )
    {
        printf("This is MT29F16G08QAA NAND. Change the blocksize back!\n");
        /* RBL doesn't support this device. Let's pretend the block size is 
           2KB for writing UBL
         */
        gNandInfo.blk_shift = 23;
        gNandInfo.page_cnt  = 128;
        /* keep the first block from being erased */
        NANDSetBadBlock(0);
    }
	/* writing 0xff to reserve area of the 8 bit NAND flash for bad block identification */
	if(gNandInfo.page_size == 2112)
	{
		pageSize=2048;
		//wordsPerpage=512;
		nandTx[pageSize] = 0xFF;	/* If not BAD block */
		nandTx[pageSize+5] = 0xFF;
	}
	else 
	{
		pageSize=512;
		//wordsPerpage=128;
		nandTx[pageSize+5] = 0xFF;  /* If not BAD block */
	}
	// Determine # of pages
	diagdesc[4] = wtrFileSize1/pageSize;
	if((diagdesc[4]*pageSize) < wtrFileSize1)
		diagdesc[4]++;
	printf("Diagnostic: Number of pages: %d\n", diagdesc[4]);

	// Check the entry addresses TODO
	blockNum = DIAG_DESC_START_BLOCK_NUM-1; // Refer to UBL code.
	/* the total number of pages required for programming the U-Boot code */
    numPage = diagdesc[4];

	/* U-Boot Descriptor copy starts now */
	printf("Diagnostic Descriptor copy starts now \n");
	memset(nandTx, 0, PAGE_SIZE_SPARE);

Retry_program1:

	blockNum++;
	if(blockNum > DIAG_DESC_END_BLOCK_NUM)
		return E_FAIL;
	printf(" BlockNum: %d\n", blockNum);

	for(count=0;count<6;count++)  /* copy the U-Boot descriptor to NAND */
		((Uint32 *)nandTx)[count] = diagdesc[count];

	if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
	{
		printf("NANDCheckBadBlock() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program1;
	}	
	if((error=NANDWritePage(blockNum, 0, nandTx,TRUE))!=E_PASS)
    {
		printf("NANDWritePage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program1;
	}	
    else
    	printf(" NANDWriteBlock done on block %d\n",blockNum);

	waitloop(200);

	if((error=NANDReadPage(blockNum, 0, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
	{
		printf("NANDReadPage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program1;
	} 
    else
    	//printf(" NANDReadPage done on block %d\n",blockNum);
	if((error = NANDVerifyPage(nandRx, nandTx, pageSize)) == E_FAIL)
	{
		printf("NANDVerifyPage() failed on block %d\nU-Boot Descriptor cannot be written, exiting\n", blockNum);
		goto Retry_program1;
	}
	printf("Diagnostic descriptor is successfully programmed ,BlockNum : %d\n",blockNum);
	
	/* Calculate the total number of pages required for burning the actual application */
    numPage = diagdesc[4];
	blockNum = diagdesc[1]-1; // Block number from the descriptor.
	page = 0;
	nextBlock = 1;
	/* Actual application programming started now */
	printf("Diagnostic programming starts now pages %d, blockNum %d\n", numPage, blockNum);

	for(count = 0; count < numPage; count++) 
	{
Retry_Diag_program:
		if(nextBlock)
		{
			blockNum++;
			if(blockNum > 128)
				return E_FAIL;
			page = 0;
			if((error=NANDCheckBadBlock(blockNum, pageSize)) == E_FAIL)
				goto Retry_Diag_program;
			nextBlock = 0;
		}	
		/* if page goes beyond max number of pages, increment block number and set page number to '0' */
		if(page >= gNandInfo.page_cnt)
		{
			nextBlock = 1;
			goto Retry_Diag_program;
		}
		diagdatPtr = &appdat1[count*pageSize];
		if((error=NANDWritePage(blockNum,page,(Uint8*)diagdatPtr,TRUE))!= E_PASS)
		{
			nextBlock = 1;
			goto Retry_Diag_program;
		}
		else
	    	//printf(" NANDWriteBlock done on block %d\n",blockNum);
		waitloop(200);

		if((error=NANDReadPage(blockNum, page, nandRx,TRUE))!=E_PASS)  	/* to verify if the data is proper */
        {
            printf(" NANDReadPage fails on block %d\n",blockNum);
			nextBlock = 1;
			goto Retry_Diag_program;
        } 
        else
            //printf(" NANDReadPage done on block %d\n",blockNum);
		if((error=NANDVerifyPage(nandRx, diagdatPtr, pageSize)) == E_FAIL)
		{
			nextBlock = 1;
			goto Retry_Diag_program;
		}
		page++;		
	}  
	free(appdat1);
	/* Completes the U-Boot programming  */
	printf("Diagnostic programming is completed \n");
#endif
	return E_PASS;
}		


