/******************************************************************************
            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION           
                                                                             
   Property of Texas Instruments  
   For  Unrestricted  Internal  Use  Only 
   Unauthorized reproduction and/or distribution is strictly prohibited.  
   This product  is  protected  under  copyright  law  and  trade  secret law 
   as an unpublished work.  

   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.
                                                           
   Filename       	: DM350_emif.cmd

   Description    	: Link command file for the Nand Programmer 
   
   Project        	: DM350   

  *******************************************************************************/
-f 0
-c                                         /* LINK USING C CONVENTIONS      */
-stack  0x10000
-heap   0x40000                             /* HEAP AREA SIZE                */

__I_STACK_SIZE = 0x800;		/* IRQ Stack */
__S_STACK_SIZE = 0x800;		/* SUPERVISOR Stack */
__F_STACK_SIZE = 0x800;		/* FIQ Stack */
__U_STACK_SIZE = 0x800;		/* UNDEF Stack */
__Y_STACK_SIZE = 0x800;		/* SYSYEM Stack */
__A_STACK_SIZE = 0x800;		/* ABORT Stack */

/* SPECIFY THE SYSTEM MEMORY MAP */
MEMORY
{		 
	IRAM                org= 0x00000000 len= 0x00007FFF
	ENTRY_POINT         org= 0x80000000 len= 0x00000200 	
	DDR_EMIF			org= 0x80000200 len= 0x10000000 
}


SECTIONS
{
   	.bss:csl_section:_CSL_intcEventHandlerRecord > IRAM
	{. = align(8);}
   	.bss:csl_section:intc  > IRAM
   	.text:csl_section:intc > IRAM 
    
    .entryPoint >   ENTRY_POINT
	.cinit		>	DDR_EMIF
   	.text		>	DDR_EMIF
	.const      >   DDR_EMIF 
    .data       >   DDR_EMIF    
	.bss        >   DDR_EMIF
	.sysmem		>	DDR_EMIF
	.stack		>	DDR_EMIF
	.sstack		> 	DDR_EMIF
	.istack		> 	DDR_EMIF
	.fstack		> 	DDR_EMIF
	.ustack		> 	DDR_EMIF
	.ystack		> 	DDR_EMIF
	.astack		> 	DDR_EMIF
	
     
}
