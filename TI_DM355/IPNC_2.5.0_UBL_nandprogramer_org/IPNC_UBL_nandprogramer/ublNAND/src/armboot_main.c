/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: armboot_main.c												  */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 UBL boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#include <bootloader.h>
#include <cslr_gpio.h>
#include <cslr_tmr.h>
#include <cslr_psc.h>


Uint32 gEntryPoint;
Uint8 gToggle = 0;
extern int test_mode;


Uint32 LPSCEnable(Uint8 PdNum, Uint8 LPSCNUM){
	Uint8 domainOn = 0;
	volatile Uint32 count;
 
  	/* NEXT=0x3, Enable LPSC Module N */   
  	CSL_PSC_0_REGS->MDCTL[LPSCNUM] = (CSL_PSC_0_REGS->MDCTL[LPSCNUM]&0xFFFFFFF8) | (0x3);

  	/* Write value of PDSTAT for Power Domain N
  	   Has no effect if domain is already powered up */
  	domainOn = (CSL_PSC_0_REGS->PDSTAT[PdNum] & 0x00000001);
        
    /* If domain is not powered, power up domain */
    if(domainOn == 0) 
	{
		CSL_PSC_0_REGS->PDCTL[PdNum] |= 0x1;
		
        CSL_PSC_0_REGS->PTCMD = (1<<PdNum);    
        
        /* Wait for EPCpr BIT TO BE SET. If Timeout, continue */                                        
	    count = TIMEOUT_COUNT;
	    while( (((CSL_PSC_0_REGS->EPCPR >> PdNum) & 0x00000001) == 0) && (count>0)){
	   		count --;
	   	}

        /* Write to EPCGOOD Bit */           
        CSL_PSC_0_REGS->PDCTL[PdNum] |= 0x00000100; 

        /* Wait for GOSTAT = NO TRANSITION from PTSTAT. If Timeout, continue */
	    count = TIMEOUT_COUNT;
		while( (((CSL_PSC_0_REGS->PTSTAT >> PdNum) & 0x00000001) != 0) && (count>0)){
	   		count --;
	   	}

	} else {  
	  	/* Program goctl to start transition sequence for LPSCs */
  	    CSL_PSC_0_REGS->PTCMD = (1<<PdNum);
	  
  	    /* Wait for GOSTAT = NO TRANSITION from LPSC. If timeout, continue */
	    count = TIMEOUT_COUNT;
        while( (((CSL_PSC_0_REGS->PTSTAT >> PdNum) & 0x00000001) != 0) && (count>0)){
	   		count --;
	   	}

	}
  
  	/* Wait for MODSTAT = ENABLE from LPSC. If timeout, continue */
    count = TIMEOUT_COUNT;
    while( ((CSL_PSC_0_REGS->MDSTAT[LPSCNUM] & 0x0000001F) != 0x3) && (count>0)){
   		count --;
   	}
	return E_PASS;  	
}

void EnableICache() {
    /* Flush I-Cache */ 
    asm("   STMFD   SP!, {R0-R1}");
    asm("	MOV     R0, #0");  
    asm("	MCR     P15, #0, R0, C7, C5, #0");
    asm("	NOP");
	asm("	MRC   P15, #0, R1, C1, C0, #0");
 	asm("	MOV   R0,#0x1000");
	asm("	ORR   R1, R1, R0");                           
	asm("	MCR   P15, #0, R1, C1, C0, #0");
   	asm("	NOP");   
   	asm("   LDMFD  SP!, {R0-R1}");                      
}

void DisableICache() {
    /* Flush I-Cache */ 
    asm("   STMFD   SP!, {R0-R1}");
    asm("	MOV     R0, #0");  
    asm("	MCR     P15, #0, R0, C7, C5, #0");
    asm("	NOP");
	asm("	MRC   P15, #0, R1, C1, C0, #0");
 	asm("	MOV   R0,#0x1000");
	asm("	BIC   R1, R1, R0");                  
	asm("	MCR   P15, #0, R1, C1, C0, #0");
   	asm("	NOP");   
   	asm("   LDMFD  SP!, {R0-R1}");                      

}

void waitloop (Uint32 count) {
  volatile Uint32 counter;
  for (counter=0; counter<count; counter++) {
    asm("\tnop");
  }
}


Int main()
{
    volatile Uint32 counter;

	/* Clear all interrupts (FIQ and IRQ) and disable all interrupts */
	//CSL_INTC1_0_REGS->INTCTL = 0x0; /* settinf IDMODE = 0 for immediate disable */
	//CSL_INTC1_0_REGS->EINT0 = 0x0; /* 0-31 INT */
	//CSL_INTC1_0_REGS->EINT1 = 0x0; /* 32-63 INT */

	// Enable DDR input Powerdown feature */
	// Set bit 6 of PHY control MMR1
	//*PHY_CTRL1 |= 0x40;

	// Power on GPIO Module
	//LPSCEnable(0,LPSC_GPIO);

	// configure pll,cpu clock, DDR
	//DM350Init_ARM_216_DDR2_162MHz();
	//DM350Init_ARM_135_VPSS_135_DDR2_133MHz();

   	/* copy the application from NAND to DDRAM */
    DM355EvmInit();
    //EVMDM355_I2C_init();
    //UARTInit();
    //UARTSendData((Uint8*)"\r\nDM350 Inialization is done\r\n", FALSE);
#if 1
  	if (NANDBoot() != E_PASS) 
    {
        UARTSendData((Uint8*)"UBL: NANDBoot() failed\r\n", FALSE);
		return E_FAIL;  
    }
#endif
	// disabling all the interrupts again 
	//CSL_INTC1_0_REGS->EINT1 = 0;
	if(!test_mode){
    	UARTSendData((Uint8*)"UBL: booting to U-Boot\r\n", FALSE);
	}else{
		UARTSendData((Uint8*)"UBL: booting to Diagnostic\r\n", FALSE);
	}
	//UARTSendInt(gEntryPoint);
	return E_PASS;  
}


