/**********************************************************************************/
/* File: nandProg_main.c			 				                              */
/* Purpose:  main function for nand programmer 									  */
/* Project:  DM350 boot-loader		 		                                      */
/* Authror: 																	  */
/* Date : May-2-2006	                                                      	  */
/**********************************************************************************/

#include <error.h>
#include <tistdtypes.h>
#include <stdio.h>


void main()
{
	Uint8 result=0;	
//	NandPrg();
  /* call nand programmer function*/
  result=NANDProg();
  if(result!= E_PASS) 	 
		printf("NAND test failed and hence NAND cannot be programmed\n"); 	
	else
		printf(" NAND flash programming is completed \n");

}

	  
	


