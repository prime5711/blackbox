/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2005, (C) Copyright 2005 Texas Instruments.  All rights reserved.*/ 
/*                                                                            */
/*  FILE 	: ublDM355.cmd						      						  */
/*  PURPOSE	: Linker file						      						  */
/*  PROJECT	: DM350 User boot-loader    	                                  */
/*  AUTHOR	: 																  */
/******************************************************************************/



MEMORY
{
	BOOT_IRAM_INIT       org= 0x00000020 len= 0x00000130  /* Boot Code */
	BOOT_IRAM_CODE       org= 0x00000150 len= 0x00003600 /* UBL Code  */
	
	DDR_EMIF      		 org= 0x80000000 len= 0x10000000 /* Application code */
}


SECTIONS
{
	.boot		>   BOOT_IRAM_INIT
	.initTCM	>   BOOT_IRAM_CODE
	.text		>   BOOT_IRAM_CODE
	.cinit	 	>   BOOT_IRAM_CODE	
	.const	 	>	BOOT_IRAM_CODE
	.bss		>	BOOT_IRAM_CODE
	.data		>	BOOT_IRAM_CODE
	.stack      >   BOOT_IRAM_CODE
	.sysmem		>	BOOT_IRAM_CODE
}





