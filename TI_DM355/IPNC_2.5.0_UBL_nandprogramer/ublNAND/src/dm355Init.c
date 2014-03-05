/* --------------------------------------------------------------------------
    FILE        : dm355_evm.c 				                             	 	        
    PURPOSE     : Platform initialization file
    PROJECT     : DaVinci User Boot-Loader and Flasher
    AUTHOR      : Daniel Allred
    DATE	    : Jan-22-2007  
 
    HISTORY
 	     v1.00 completion (with support for DM6441 and DM6441_LV)							 						      
 	          Daniel Allred - Jan-22-2007
         v1.11 - DJA - 07-Mar-2007
 	          Made all DDR and PLL global values as const variables.
 	          Added code to set 1394 MDSTAT, as per workaround from U-boot
 	          and code to init all power domains to known state.
         v1.13 - DJA - 04-Jun-2007
	          Fix for DDRPHYCR misread from user's guide.
 	     
 ----------------------------------------------------------------------------- */

#include "dm355_evm.h"
#include "uart.h"

int test_mode=0;
#define PRESS_TIME 1000
// ---------------------------------------------------------------------------
// Global Memory Timing and PLL Settings
// ---------------------------------------------------------------------------
#if defined(DM6441_LV)
    // For Micron MT47H64M16BT-37E @ 135 MHz   
    const Uint8 DDR_NM = 0;
    const Uint8 DDR_CL = 3;
    const Uint8 DDR_IBANK = 3;
    const Uint8 DDR_PAGESIZE = 2;
    const Uint8 DDR_T_RFC = 17;
    const Uint8 DDR_T_RP = 2;
    const Uint8 DDR_T_RCD = 2;
    const Uint8 DDR_T_WR = 2;
    const Uint8 DDR_T_RAS = 5;
    const Uint8 DDR_T_RC = 7;
    const Uint8 DDR_T_RRD = 1;
    const Uint8 DDR_T_WTR = 1;
    const Uint8 DDR_T_XSNR = 18;
    const Uint8 DDR_T_XSRD = 199;
    const Uint8 DDR_T_RTP = 1;
    const Uint8 DDR_T_CKE = 2;
    const Uint16 DDR_RR = 1264;
    const Uint8 DDR_Board_Delay = 3;
    const Uint8 DDR_READ_Latency = 5;
    
    const Uint32 PLL2_Mult = 20;
    const Uint32 PLL2_Div1 = 10;
    const Uint32 PLL2_Div2 = 2;
#else
    // For Micron MT47H64M16BT-37E @ 162 MHz
    const Uint8 DDR_NM = 0;
    const Uint8 DDR_CL = 3;
    const Uint8 DDR_IBANK = 3;
    const Uint8 DDR_PAGESIZE = 2;
    const Uint8 DDR_T_RFC = 20;
    const Uint8 DDR_T_RP = 2;
    const Uint8 DDR_T_RCD = 2;
    const Uint8 DDR_T_WR = 2;
    const Uint8 DDR_T_RAS = 6;
    const Uint8 DDR_T_RC = 8;
    const Uint8 DDR_T_RRD = 2;
    const Uint8 DDR_T_WTR = 1;
    const Uint8 DDR_T_XSNR = 22;
    const Uint8 DDR_T_XSRD = 199;
    const Uint8 DDR_T_RTP = 1;
    const Uint8 DDR_T_CKE = 2;
    const Uint16 DDR_RR = 1053;
    const Uint8 DDR_Board_Delay = 3;
    const Uint8 DDR_READ_Latency = 5; 
    
    const Uint32 PLL2_Mult = 24;
    const Uint32 PLL2_Div1 = 12;
    const Uint32 PLL2_Div2 = 2;
    
    // 567 verison - only use this with older silicon/EVMs
    // For Micron MT47H64M16BT-37E @ 189 MHz
    /*const Uint8 DDR_NM = 0;
    const Uint8 DDR_CL = 4;
    const Uint8 DDR_IBANK = 3;
    const Uint8 DDR_PAGESIZE = 2;
    const Uint8 DDR_T_RFC = 24;
    const Uint8 DDR_T_RP = 2;
    const Uint8 DDR_T_RCD = 2;
    const Uint8 DDR_T_WR = 2;
    const Uint8 DDR_T_RAS = 7;
    const Uint8 DDR_T_RC = 10;
    const Uint8 DDR_T_RRD = 2;
    const Uint8 DDR_T_WTR = 1;
    const Uint8 DDR_T_XSNR = 25;
    const Uint8 DDR_T_XSRD = 199;
    const Uint8 DDR_T_RTP = 1;
    const Uint8 DDR_T_CKE = 2;
    const Uint16 DDR_RR = 1477;
    const Uint8 DDR_Board_Delay = 2;
    const Uint8 DDR_READ_Latency = 5;
    
    const Uint32 PLL2_Mult = 14;
    const Uint32 PLL2_Div1 = 7;
    const Uint32 PLL2_Div2 = 1;*/
#endif

// Set CPU clocks
#if defined(DM6441_LV)
    const Uint32 PLL1_Mult = 15;  // DSP=405 MHz
#elif defined(DM6441)
    const Uint32 PLL1_Mult = 19;  // DSP=513 MHz  
#else
    const Uint32 PLL1_Mult = 22;  // DSP=594 MHz
    // 567 version - only use this with older silicon/EVMs
    // Uint32 PLL1_Mult = 21;
#endif

#define W_SETUP       	0x1	    //0~f         
#define W_STROBE        0x2	    //0~3f
#define W_HOLD        	0x1	    //0~7
#define R_SETUP       	0x1	    //0~f         
#define R_STROBE      	0x2	    //0~3f
#define R_HOLD        	0x1     //0~7

#define TA            	3     //0~3
#define A_SIZE        	1     //1:16 bit 0:8bit
#define DM9000_TIMING  		W_SETUP<<26 | W_STROBE<<20 | W_HOLD <<17 | R_SETUP<<13 | R_STROBE<<7 | R_HOLD <<4 | TA<<2 | A_SIZE

    
// ---------------------------------------------------------
// End of global PLL and Memory settings
// ---------------------------------------------------------
    

#define MAX_IMAGE_SIZE      (0x02000000)

/* Set details of RAM */
#define RAM_START_ADDR      (0x80000000)
#define RAM_END_ADDR        (0x8FFFFFFF)
#define MAXSTRLEN 256
// Send specified number of bytes 

Int32 GetStringLen(Uint8* seq)
{
    Int32 i = 0;
    while ((seq[i] != 0) && (i<MAXSTRLEN)){ i++;}
    if (i == MAXSTRLEN)
        return -1;
    else
        return i;
}

Uint32 UARTSendData(Uint8* seq, Bool includeNull)
{
	Uint32 status = 0;
    Int32 i,numBytes;
	Uint32 timerStatus = 0x1000000;
	
	numBytes = includeNull?(GetStringLen(seq)+1):(GetStringLen(seq));
	
	for(i=0;i<numBytes;i++) {
		/* Enable Timer one time */
		//TIMER0Start();
		do{
			status = (UART0->LSR)&(0x60);
			//timerStatus = TIMER0Status();
            timerStatus--;
		} while (!status && timerStatus);

		if(timerStatus == 0)
			return E_TIMEOUT;
		
		// Send byte 
		(UART0->THR) = seq[i];
	}
	return E_PASS;
}

Uint32 UARTSendInt(Uint32 value)
{
	char seq[9];
	Uint32 i,shift,temp;

	for( i = 0; i < 8; i++)
	{
		shift = ((7-i)*4);
		temp = ((value>>shift) & (0x0000000F));
		if (temp > 9)
		{
			temp = temp + 7;
        }
		seq[i] = temp + 48;	
	}
	seq[8] = 0;
	return UARTSendData((Uint8 *)seq, FALSE);
}

void wait( Uint32 count )
{
    Uint32 i; 
    for ( i = 0; i < count; i++){}
}

Uint8 PSC_use_interrupt( Uint8 id )
{
    /* Interrupts: 0-1, 5-6, 9-10, 13-15, 17, 26, 40 */
    if ( ( id == 0 ) || ( id == 1 ) || ( id == 5 ) || ( id == 6 )
        || ( id == 9 ) || ( id == 10 ) || ( ( id >= 13 ) && ( id <= 15 ) )
        || ( id == 17 ) || ( id == 26 ) || ( id == 40 ) )
        return 1;
    else
        return 0;
}

void LPSCTransition(Uint8 module, Uint8 state)
{
    Uint8 set_interrupt;
    /*
     *  Step 0 - Ignore request if the state is already set as is
     */
	if (((PSC->MDSTAT[module]) & 0x1F) == state)	
        return;
    /*
     *  Step 1 - Wait for PTSTAT.GOSTAT to clear
     */

    while (PSC->PTSTAT & 0x00000001);

    /*
     *  Step 2 - Set MDCTLx.NEXT to new state
     */

	PSC->MDCTL[module] = ((PSC->MDCTL[module]) & (0xFFFFFFE0)) | (state);

    /*
     *  Step 2.5 - Enable interrutps if it is needed
     */
    set_interrupt = PSC_use_interrupt( module );

    if ( set_interrupt )
    {    
        PSC->MDCTL[module] |= 0x0200;
    } 

    /*
     *  Step 3 - Start power transition ( set PTCMD.GO to 1 )
     */

	PSC->PTCMD |= 0x00000001;


    /*
     *  Step 4 - Wait for PTSTAT.GOSTAT to clear
     */

	while ((PSC->PTSTAT) & 0x00000001);

    /*
     *  Step 5 - Clear Interrupt
     */

    if ( set_interrupt )
    {    
        PSC->MDCTL[module] &= ~0x0200;
    } 

}
	
void DM355EvmInit()
{
	unsigned int status,i;
	// Mask all interrupts
	AINTC->INTCTL = 0x4;
	AINTC->EABASE = 0x0;
	AINTC->EINT0 = 0x0;
	AINTC->EINT1 = 0x0;		
   
    AINTC->FIQ0 = 0xffffffff;
    AINTC->FIQ1 = 0xffffffff;
    AINTC->IRQ0 = 0xffffffff;
    AINTC->IRQ1 = 0xffffffff;

	
    PSCInit();     
#if 0
    SYSTEM->PINMUX[0] = 0x00007F55;  // All Video Inputs
    SYSTEM->PINMUX[1] = 0x00145555;  // All Video Outputs
    SYSTEM->PINMUX[2] = 0x00000004;  // EMIFA
    SYSTEM->PINMUX[3] = 0x1BFF55FF;  // SPI0, SPI1, UART1, I2C, SD0, SD1, McBSP0, CLKOUTs
    SYSTEM->PINMUX[4] = 0x00000000;  // MMC/SD0 instead of MS, SPI0
#endif
	SYSTEM->PINMUX[0] = 0x00005F50;  // All Video Inputs
    SYSTEM->PINMUX[1] = 0x00145555;  // All Video Outputs
    SYSTEM->PINMUX[2] = 0x00000C00;  // EMIFA
    SYSTEM->PINMUX[3] = 0x00610000;  // SPI0, SPI1, UART1, I2C, SD0, SD1, McBSP0, CLKOUTs
    SYSTEM->PINMUX[4] = 0x00000003;  // MMC/SD0 instead of MS, SPI0 select gio 102.103
	/******************* UART Setup **************************/
	
	/******************* System PLL Setup ********************/
	PLL1Init();
	
	/******************* DDR PLL Setup ***********************/	
	PLL2Init();

	/******************* DDR2 Timing Setup *******************/
	DDR2Init();
			
	/******************* AEMIF Setup *************************/
	         	/* set GIO10 output	*/
    *((volatile unsigned int *) GIO_DIR01) &= ~(1<<10);  
	*((volatile unsigned int *) GIO_SET_DATA01) |= (1<<10); // output Hi                    	 
//    *((volatile unsigned int *) GIO_CLR_DATA01) |= (1<<10); // output low	 
		         	/* set GIO00 output	*/
    *((volatile unsigned int *) GIO_DIR01) &= ~(1<<0);  
	*((volatile unsigned int *) GIO_SET_DATA01) |= (1<<0); // output Hi  
	// Handled in NOR or NAND init
	AEMIFInit();     
	      	
    EVMDM355_I2C_init( );
	UARTInit();
	/* Added Check  is Diagnostic  mode ?*/
	for(i=0l;i<PRESS_TIME;i++){
		status= *((volatile unsigned int *) GIO_IN_DATA01) & (1<<2);
    	if (status ==4)
			break;
        waitloop(1000);
    }
	test_mode=0;
	if(i==PRESS_TIME)
		test_mode=1;
    /******************* IRQ Vector Table Setup **************/
    //IVTInit();
}

void PSCInit()
{
    Uint32 i;

#if 0
    // Put the C64x+ Core into reset (if it's on)
	PSC->MDCTL[LPSC_DSP] &= (~0x00000100);
	PSC->PTCMD |= 0x00000002;
	while ((PSC->PTSTAT) & (0x00000002));
	while ((PSC->MDSTAT[LPSC_DSP]) & (0x00000100));
	
    for( i = LPSC_VPSS_MAST ; i < LPSC_1394 ; i++ )
        PSC->MDCTL[i] |= 0x03; // Enable

	// Do this for enabling a WDT initiated reset this is a workaround
	// for a chip bug.  Not required under normal situations 
	// Copied from U-boot boards/DaVinci/platform.S and convereted to C
	//      LDR R6, P1394
	//      MOV R10, #0x0	
	//      STR R10, [R6]        
    PSC->MDCTL[LPSC_1394] = 0x0;
    
    for( i = LPSC_USB ; i < LPSC_DSP ; i++ )
        PSC->MDCTL[i] |= 0x03; // Enable
    
    PSC->MDCTL[LPSC_IMCOP] = 0x03;

    // Set EMURSTIE to 1 on the following
    PSC->MDCTL[LPSC_VPSS_SLV]   |= 0x0203;
    PSC->MDCTL[LPSC_EMAC0]      |= 0x0203;
    PSC->MDCTL[LPSC_EMAC1]      |= 0x0203;
    PSC->MDCTL[LPSC_MDIO]       |= 0x0203;
    PSC->MDCTL[LPSC_USB]        |= 0x0203;
    PSC->MDCTL[LPSC_ATA]        |= 0x0203;
    PSC->MDCTL[LPSC_VLYNQ]      |= 0x0203;
    PSC->MDCTL[LPSC_HPI]        |= 0x0203;
    PSC->MDCTL[LPSC_DDR2]       |= 0x0203;
    PSC->MDCTL[LPSC_AEMIF]      |= 0x0203;
    PSC->MDCTL[LPSC_MMCSD]      |= 0x0203;
    PSC->MDCTL[LPSC_MEMSTK]     |= 0x0203;
    PSC->MDCTL[LPSC_ASP]        |= 0x0203;
    PSC->MDCTL[LPSC_GPIO]       |= 0x0203;
    PSC->MDCTL[LPSC_IMCOP]      |= 0x0203;

    // Do Always-On Power Domain Transitions
    PSC->PTCMD |= 0x00000001;
    while ((PSC->PTSTAT) & 0x00000001);
    
    // DO DSP Power Domain Transitions
    PSC->PTCMD |= 0x00000002;
	w hile ((PSC->PTSTAT) & (0x00000002));
	
	
	// Clear EMURSTIE to 0 on the following
	PSC->MDCTL[LPSC_VPSS_SLV]   &= 0x0003;
    PSC->MDCTL[LPSC_EMAC0]      &= 0x0003;
    PSC->MDCTL[LPSC_EMAC1]      &= 0x0003;
    PSC->MDCTL[LPSC_MDIO]       &= 0x0003;
    PSC->MDCTL[LPSC_USB]        &= 0x0003;
    PSC->MDCTL[LPSC_ATA]        &= 0x0003;
    PSC->MDCTL[LPSC_VLYNQ]      &= 0x0003;
    PSC->MDCTL[LPSC_HPI]        &= 0x0003;
    PSC->MDCTL[LPSC_DDR2]       &= 0x0003;
    PSC->MDCTL[LPSC_AEMIF]      &= 0x0003;
    PSC->MDCTL[LPSC_MMCSD]      &= 0x0003;
    PSC->MDCTL[LPSC_MEMSTK]     &= 0x0003;
    PSC->MDCTL[LPSC_ASP]        &= 0x0003;
    PSC->MDCTL[LPSC_GPIO]       &= 0x0003;
    PSC->MDCTL[LPSC_IMCOP]      &= 0x0003;    
#endif
    /*
     *  Enable all non-reserved power modules
     *  Reserved: 39
     */
    for ( i = 0 ; i <= 38 ; i++ )
        LPSCTransition( i , 3 );
    LPSCTransition( 40 , 3 );
}

void PLL2Init()
{	
        
#if 0
	// Set PLL2 clock input to external osc. 
	PLL2->PLLCTL &= (~0x00000100);
	
	// Clear PLLENSRC bit and clear PLLEN bit for Bypass mode 
	PLL2->PLLCTL &= (~0x00000021);

	// Wait for PLLEN mux to switch 
	waitloop(32*(PLL1_Mult/2));
	
	PLL2->PLLCTL &= (~0x00000008);          // Put PLL into reset
	PLL2->PLLCTL |= (0x00000010);           // Disable the PLL
	PLL2->PLLCTL &= (~0x00000002);          // Power-up the PLL
	PLL2->PLLCTL &= (~0x00000010);          // Enable the PLL
	
	// Set PLL multipliers and divisors 
	PLL2->PLLM      = PLL2_Mult-1;     // 27  Mhz * (23+1) = 648 MHz 
	PLL2->PLLDIV1   = PLL2_Div1-1;     // 648 MHz / (11+1) = 54  MHz
	PLL2->PLLDIV2   = PLL2_Div2-1;     // 648 MHz / (1+1 ) = 324 MHz (the PHY DDR rate)
		
	PLL2->PLLDIV2 |= (0x00008000);          // Enable DDR divider	
	PLL2->PLLDIV1 |= (0x00008000);          // Enable VPBE divider	
	PLL2->PLLCMD |= 0x00000001;             // Tell PLL to do phase alignment
	while ((PLL2->PLLSTAT) & 0x1);          // Wait until done
	waitloop(256*(PLL1_Mult/2));

	PLL2->PLLCTL |= (0x00000008);           // Take PLL out of reset	
	waitloop(2000*(PLL1_Mult/2));                       // Wait for locking
	
	PLL2->PLLCTL |= (0x00000001);           // Switch out of bypass mode
#endif
    /* per GEL file */
    PLL2->PLLCTL &= ~0x0100;             // Onchip Oscillator
//    *pll_ctl |= 0x0100;            // External Oscillator

    PLL2->PLLCTL &= ~0x0020;             // Clear PLLENSRC
    PLL2->PLLCTL &= ~0x0001;             // Set PLL in bypass
    waitloop( 150 );

    PLL2->PLLCTL |= 0x0008;              // Put PLL in reset 

    PLL2->PLLCTL |= 0x0010;              // Disable PLL
    PLL2->PLLCTL &= ~0x0002;             // Power up PLL
    PLL2->PLLCTL &= ~0x0010;             // Enable PLL
    waitloop( 150 );                   // Wait for PLL to stabilize

    PLL2->PLLM    = 113;                 // PLL2 24 * (113 + 1) / (7+1) = 342MHz
    PLL2->PLLDIV1 |= 0x8000;          // SYSCLK1 = 342MHz / 2 = 171MHz (DDR2)

    PLL2->PLLCMD  |= 0x0001;             // Set GOSET
    while( ( PLL2->PLLSTAT & 1 ) != 0 ); // Wait until complete

    waitloop( 2000 );
    PLL2->PLLCTL &= ~0x0008;             // Release PLL from reset

    waitloop( 2000 );
    PLL2->PLLCTL |= 0x0001;              // Enable PLL

}

void DDR2Init()
{
	
#if 0
	Int32 tempVTP;
	// Set the DDR2 to enable
	LPSCTransition(LPSC_DDR2, PSC_ENABLE);
		
	// For Micron MT47H64M16BT-37E @ 162 MHz
	// Setup the read latency (CAS Latency + 3 = 6 (but write 6-1=5))
	DDR->DDRPHYCR = (0x50006400) | DDR_READ_Latency;
	// Set TIMUNLOCK bit, CAS LAtency 3, 8 banks, 1024-word page size 
	//DDR->SDBCR = 0x00138632;
	DDR->SDBCR = 0x00138000 |
	             (DDR_NM << 14)   |
	             (DDR_CL << 9)    |
	             (DDR_IBANK << 4) |
	             (DDR_PAGESIZE <<0);
	
	// Program timing registers 
	//DDR->SDTIMR = 0x28923211;
	DDR->SDTIMR = (DDR_T_RFC << 25) |              
                  (DDR_T_RP << 22)  |
                  (DDR_T_RCD << 19) |
                  (DDR_T_WR << 16)  |
                  (DDR_T_RAS << 11) |
                  (DDR_T_RC << 6)   |
                  (DDR_T_RRD << 3)  |
                  (DDR_T_WTR << 0);
                  
	//DDR->SDTIMR2 = 0x0016C722;
	DDR->SDTIMR2 = (DDR_T_XSNR << 16) |
                   (DDR_T_XSRD << 8)  |
                   (DDR_T_RTP << 5)   |
                   (DDR_T_CKE << 0);
    
    
    // Clear the TIMUNLOCK bit 
	DDR->SDBCR &= (~0x00008000);
	
	// Set the refresh rate
	DDR->SDRCR = DDR_RR;
	
	// Dummy write/read to apply timing settings
	DDRMem[0] = DDR_TEST_PATTERN;
	if (DDRMem[0] == DDR_TEST_PATTERN)
          UARTSendInt(DDRMem[0]);
	
	// Set the DDR2 to syncreset
	LPSCTransition(LPSC_DDR2, PSC_SYNCRESET);

	// Set the DDR2 to enable
	LPSCTransition(LPSC_DDR2, PSC_ENABLE);
			 
	/***************** DDR2 VTP Calibration ****************/
	DDR->VTPIOCR = 0x201F;        // Clear calibration start bit
	DDR->VTPIOCR = 0xA01F;        // Set calibration start bit 
	
	waitloop(11*33);              // Wait for calibration to complete 
		 
	SYSTEM->DDRVTPER = 0x1;       // DDRVTPR Enable register
	
	tempVTP = 0x3FF & DDRVTPR;    // Read calibration data
	
	// Write calibration data to VTP Control register 
	DDR->VTPIOCR = ((DDR->VTPIOCR) & 0xFFFFFC00) | tempVTP; 
	
	// Clear calibration enable bit
	DDR->VTPIOCR = (DDR->VTPIOCR) & (~0x00002000);
	
	// DDRVTPR Enable register - disable DDRVTPR access 
	SYSTEM->DDRVTPER = 0x0;
#endif
    /* per GEL file */
    LPSCTransition( 13, 3 );
    SYSTEM->VTPIOCR &= 0xFFFFDF3F;// Clear bit CLRZ & PWRDN & LOCK bit(bit 13/6/7)
    SYSTEM->VTPIOCR |=  0x00002000;  // Set bit CLRZ (bit 13)

    while(!(SYSTEM->VTPIOCR & 0x8000));

    SYSTEM->VTPIOCR |= 0x00004000;  // Set bit VTP_IO_READY(bit 14)
    SYSTEM->VTPIOCR |= 0x00000180;  // Set bit LOCK(bit 7) and  PWRSAVE  (bit 8)
    SYSTEM->VTPIOCR |= 0x00000040;  // Powerdown VTP as it is locked  (bit 6)

	waitloop(11*33);              // Wait for calibration to complete 

    /* Turn on DDR2 PHY in PSC */
    //LPSCTransition( 13, 3 );

    /* DDR2 controller initialization */
    DDR->DDRPHYCR = 0x51006494;   //External DQS gating enabled

    DDR->SDRCR = 0x00000535;        //Program SDRAM Refresh Control Register

    DDR->PBBPR = 0x000000FE;       //VBUSM Burst Priority Register, pr_old_count = 0xFE
    DDR->SDBCR = 0x0000C632;       //Program SDRAM Bank Config Register

    DDR->SDTIMR0 = 0x2A923249;        //Program SDRAM Timing Control Register1
    DDR->SDTIMR1 = 0x3c17C763;        //Program SDRAM Timing Control Register2

    DDR->SDBCR   = 0x00004632;       //Program SDRAM Bank Config Register
}

void PLL1Init()
{
#if 0
    //594 version
    //Uint32 PLL1_Mult = 22;
        
    //567 version - use with 189 MHZ DDR
    //Uint32 PLL1_Mult = 21;
            
	// Set PLL2 clock input to internal osc. 
	PLL1->PLLCTL &= (~0x00000100);	
	
	// Clear PLLENSRC bit and clear PLLEN bit for Bypass mode 
	PLL1->PLLCTL &= (~0x00000021);

	// Wait for PLLEN mux to switch 
	waitloop(32);
	
	PLL1->PLLCTL &= (~0x00000008);     // Put PLL into reset
	PLL1->PLLCTL |= (0x00000010);      // Disable the PLL
	PLL1->PLLCTL &= (~0x00000002);     // Power-up the PLL
	PLL1->PLLCTL &= (~0x00000010);     // Enable the PLL
	
	// Set PLL multipliers and divisors 
	PLL1->PLLM = PLL1_Mult - 1;        // 27Mhz * (21+1) = 594 MHz 
			
	PLL1->PLLCMD |= 0x00000001;		// Tell PLL to do phase alignment
	while ((PLL1->PLLSTAT) & 0x1);	// Wait until done
	
	waitloop(256);
	PLL1->PLLCTL |= (0x00000008);		//	Take PLL out of reset	
	waitloop(2000);				// Wait for locking
	
	PLL1->PLLCTL |= (0x00000001);		// Switch out of bypass mode
    /* per GEL file */
    PLL1->PLLCTL &= ~0x0100;             // Onchip Oscillator
//    *pll_ctl |= 0x0100;            // External Oscillator

    PLL1->PLLCTL &= ~0x0020;             // Clear PLLENSRC
    PLL1->PLLCTL &= ~0x0001;             // Set PLL in bypass
    wait( 150 );

    PLL1->PLLCTL &= ~0x0008;             // Put PLL in reset

    PLL1->PLLCTL |= 0x0010;              // Disable PLL
    PLL1->PLLCTL &= ~0x0002;             // Power up PLL
    PLL1->PLLCTL &= ~0x0010;             // Enable PLL
    wait( 150 );                    // Wait for PLL to stabilize

    PLL1->PLLM = 143;                 // PLL1 24 * (143 + 1) / (7 + 1) = 432MHz
    PLL1->PLLDIV1 |= 0x8000;          // SYSCLK1 = 432MHz / 2 = 216MHz (ARM)
    PLL1->PLLDIV2 |= 0x8000;          // SYSCLK2 = 432MHz / 4 = 108MHz (Peripherals)
    PLL1->PLLDIV3 = 0x8000 | 15;      // SYSCLK3 = 432MHz / 16 = 27MHz (VENC)
    PLL1->PLLDIV4 = 0x8000 | 3;       // SYSCLK4 = 432MHz / 4 = 108MHz (VPSS)
    wait (100 );

    SYSTEM->MISC &= 0xFFFFFFD;       // Enable high frequency operation

    PLL1->PLLCMD |= 0x0001;              // Set GOSET
    while( ( PLL1->PLLSTAT & 1 ) != 0 ); // Wait until complete

    wait( 2000 );
    PLL1->PLLCTL |= 0x0008;              // Release PLL from reset

    wait( 2000 );
    PLL1->PLLCTL |= 0x0001;              // Enable PLL
#endif

    PLL1->PLLCTL &= ~0x0100;             // Onchip Oscillator
//    *pll_ctl |= 0x0100;            // External Oscillator

    PLL1->PLLCTL &= ~0x0020;             // Clear PLLENSRC
    PLL1->PLLCTL &= ~0x0001;             // Set PLL in bypass
    waitloop(150 );

    PLL1->PLLCTL |= 0x0008;             // Put PLL in reset

    PLL1->PLLCTL |= 0x0010;              // Disable PLL
    PLL1->PLLCTL &= ~0x0002;             // Power up PLL
    PLL1->PLLCTL &= ~0x0010;             // Enable PLL
    waitloop( 150 );                    // Wait for PLL to stabilize
#if 1   /* for 216 MHZ */
    PLL1->PLLM = 143;                 // PLL1 24 * (143 + 1) / (7 + 1) = 432MHz
    PLL1->PLLDIV1 |= 0x8000;          // SYSCLK1 = 432MHz / 2 = 216MHz (ARM)
    PLL1->PLLDIV2 |= 0x8000;          // SYSCLK2 = 432MHz / 4 = 108MHz (Peripherals)
    PLL1->PLLDIV3 = 0x8000 | 15;      // SYSCLK3 = 432MHz / 16 = 27MHz (VENC)
    PLL1->PLLDIV4 = 0x8000 | 3;       // SYSCLK4 = 432MHz / 4 = 108MHz (VPSS)
#else

	PLL1->PLLM = 179;                 // PLL1 24 * (179 + 1) / (7 + 1) = 540MHz
    PLL1->PLLDIV1 |= 0x8000;          // SYSCLK1 = 540MHz / 2 = 270MHz (ARM)
    PLL1->PLLDIV2 |= 0x8000;          // SYSCLK2 = 540MHz / 4 = 135MHz (Peripherals)
    PLL1->PLLDIV3 = 0x8000 | 19;      // SYSCLK3 = 540MHz / 18 = 27MHz (VENC)
    PLL1->PLLDIV4 = 0x8000 | 3;       // SYSCLK4 = 540MHz / 4 = 135MHz (VPSS)

#endif
    waitloop (100 );

    SYSTEM->MISC &= 0xFFFFFFD;       // Enable high frequency operation

    PLL1->PLLCMD |= 0x0001;              // Set GOSET
    while( ( PLL1->PLLSTAT & 1 ) != 0 ); // Wait until complete

    waitloop( 2000 );
    PLL1->PLLCTL &= ~0x0008;              // Release PLL from reset

    waitloop( 2000 );
    PLL1->PLLCTL = 0x0001;              // Enable PLLLL1->PLLDIV1 |= 0x00008000;         // Enable SYSCLK1 (DDR)

}
void AEMIFInit()
{     
#if 0
   AEMIF->AWCCR     = 0x00000000;
   AEMIF->AB1CR     = 0x3FFFFFFD;
   AEMIF->AB2CR     = 0x3FFFFFFD;
   AEMIF->AB3CR     = 0x3FFFFFFD;
   AEMIF->AB4CR     = 0x3FFFFFFD;
   AEMIF->NANDFCR   = 0x00000000;
#endif
   /* No extended wait cycles */
    //AEMIF->AWCCR = 0;
    AEMIF->AWCCR = 0xff;

    /* Setup CE0 - 8-bit NAND */
    //AEMIF->AB1CR = 0x04422318;//0x00400204;  // Setup=0, Strobe=4, Hold=0, TA=1, 8-bit
    AEMIF->AB1CR = 0x40400204;  // Setup=0, Strobe=4, Hold=0, TA=1, 8-bit
    AEMIF->NANDFCR |= 1;

    /* Setup CE1 - 16-bit normal async, 9.26ns/cycle */
	AEMIF->AB2CR =DM9000_TIMING;
//	AEMIF->AB2CR = 0x00300185;  // Setup=0, Strobe=3, Hold=0, TA=1, 16-bit
//    AEMIF->AB2CR = 0x00500285;  // Setup=0, Strobe=5, Hold=0, TA=1, 16-bit
//    AEMIF->AB2CR = 0x00a00505;  // Setup=0, Strobe=A, Hold=0, TA=1, 16-bit
    AEMIF->NANDFCR &= ~2;
}
 
void UARTInit()
{		
    // The DM644x pin muxing registers must be set for UART0 use. 
	//SYSTEM->PINMUX[3] =  0x1BFFFFFF;  /* per GEL file */
	
#if 0
	// Set DLAB bit - allows setting of clock divisors 
	UART0->LCR |= 0x80;
	
	//divider = 27000000/(16*115200) = 14.64 => 15 = 0x0F (2% error is OK)
	UART0->DLL = 0x0d;
	UART0->DLH = 0x00; 

    // Enable, clear and reset FIFOs	
	UART0->FCR = 0x07;
	
	// Disable autoflow control 
	UART0->MCR = 0x00;
	
	// Enable receiver, transmitter, st to run. 
	UART0->PWREMU_MGNT |= 0x6001;
	//UART0->PWREMU_MGNT |= 0x8001;

	// Set word length to 8 bits, clear DLAB bit 
	UART0->LCR = 0x03;

	// Disable the timer 
	TIMER0->TCR = 0x00000000;
	// Set to 64-bit GP Timer mode, enable TIMER12 & TIMER34
	TIMER0->TGCR = 0x00000003;

	// Reset timers to zero 
	TIMER0->TIM34 = 0x00000000;
	TIMER0->TIM12 = 0x00000000;
	
	// Set timer period (5 second timeout = (27000000 * 5) cycles = 0x080BEFC0) 
	TIMER0->PRD34 = 0x00000000;
	TIMER0->PRD12 = 0x080BEFC0;
#endif
    /* copy from SDI example code */
 
    UART0->PWREMU_MGNT = 0;         // Reset UART TX & RX components

        wait( 100 );

        UART0->DLL = 0xd;               // Set baud rate
        UART0->DLH = 0;

        UART0->FCR = 0x0007;            // Clear UART TX & RX FIFOs
        UART0->FCR = 0x0000;            // Non-FIFO mode
        UART0->IER = 0x0007;            // Enable interrupts
        UART0->LCR = 0x0003;            // 8-bit words,
                                                    // 1 STOP bit generated,
                                                    // No Parity, No Stick paritiy,
                                                    // No Break control
        UART0->MCR = 0x0000;            // RTS & CTS disabled,
                                                    // Loopback mode disabled,
                                                    // Autoflow disabled

        UART0->PWREMU_MGNT = 0xE001;    // Enable TX & RX componenets

    	// Disable the timer 
        TIMER0->TCR = 0x00000000;
        // Set to 64-bit GP Timer mode, enable TIMER12 & TIMER34
        TIMER0->TGCR = 0x00000003;

        // Reset timers to zero 
        TIMER0->TIM34 = 0x00000000;
        TIMER0->TIM12 = 0x00000000;
	
        // Set timer period (5 second timeout = (24000000 * 5) cycles = 0x07270E00) 
        TIMER0->PRD34 = 0x00000000;
        TIMER0->PRD12 = 0x07270E00;
}

#define I2C_BASE                0x01C21000
#define I2C_OAR                 *( volatile Uint32* )( I2C_BASE + 0x00 )
#define I2C_ICIMR               *( volatile Uint32* )( I2C_BASE + 0x04 )
#define I2C_ICSTR               *( volatile Uint32* )( I2C_BASE + 0x08 )
#define I2C_ICCLKL              *( volatile Uint32* )( I2C_BASE + 0x0C )
#define I2C_ICCLKH              *( volatile Uint32* )( I2C_BASE + 0x10 )
#define I2C_ICCNT               *( volatile Uint32* )( I2C_BASE + 0x14 )
#define I2C_ICDRR               *( volatile Uint32* )( I2C_BASE + 0x18 )
#define I2C_ICSAR               *( volatile Uint32* )( I2C_BASE + 0x1C )
#define I2C_ICDXR               *( volatile Uint32* )( I2C_BASE + 0x20 )
#define I2C_ICMDR               *( volatile Uint32* )( I2C_BASE + 0x24 )
#define I2C_ICIVR               *( volatile Uint32* )( I2C_BASE + 0x28 )
#define I2C_ICEMDR              *( volatile Uint32* )( I2C_BASE + 0x2C )
#define I2C_ICPSC               *( volatile Uint32* )( I2C_BASE + 0x30 )
#define I2C_ICPID1              *( volatile Uint32* )( I2C_BASE + 0x34 )
#define I2C_ICPID2              *( volatile Uint32* )( I2C_BASE + 0x38 )

#define ICMDR_IRS                       0x0020
Int16 EVMDM355_I2C_init( )
{
    I2C_ICMDR   = 0;                // Reset I2C
    I2C_ICPSC   = 26;               // Config prescaler for 27MHz
    I2C_ICCLKL  = 20;               // Config clk LOW for 20kHz
    I2C_ICCLKH  = 20;               // Config clk HIGH for 20kHz
    I2C_ICMDR  |= ICMDR_IRS;        // Release I2C from reset
    return 0;
}

#if 0
void IVTInit()
{
	VUint32 *ivect;
    extern Uint32 __IVT;
    
	if (gBootMode == NON_SECURE_NOR)
	{
		ivect = &(__IVT);
		*ivect++ = 0xEAFFFFFE;  /* Reset @ 0x00*/
	}
	else
	{
		ivect = &(__IVT) + 4;
	}
	*ivect++ = 0xEAFFFFFE;  /* Undefined Address @ 0x04 */
	*ivect++ = 0xEAFFFFFE;  /* Software Interrupt @0x08 */
	*ivect++ = 0xEAFFFFFE;  /* Pre-Fetch Abort @ 0x0C */
	*ivect++ = 0xEAFFFFFE;  /* Data Abort @ 0x10 */
	*ivect++ = 0xEAFFFFFE;  /* Reserved @ 0x14 */
	*ivect++ = 0xEAFFFFFE;  /* IRQ @ 0x18 */
	*ivect   = 0xEAFFFFFE;	/* FIQ @ 0x1C */
}
#endif
