/* --------------------------------------------------------------------------
    FILE        : dm355_evm.h
    PURPOSE     : Platform init header file
    PROJECT     : DaVinci CCS NAND Flashing Utility
    AUTHOR      : Hui Geng
    DATE	    : 06-Jun-2007  
 
    HISTORY
 	     v1.00 - DJA - 04-Jun-2007
 	        Completion (with support for DM6441 and DM6441_LV)
 ----------------------------------------------------------------------------- */


#ifndef _DM355EVM_H_
#define _DM355EVM_H_

#include "tistdtypes.h" 

#define DM355_EVM
//Return type defines 
#define E_PASS    0x00000000u
#define E_FAIL    0x00000001u
#define E_TIMEOUT 0x00000002u 

// BIT mask defines
//  BIT masks
#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

/* -------------------------------------------------------------------------- *
 *    System Control Module register structure - See sprue14.pdf, Chapter 10  *
 *       for more details.                                                    *
 * -------------------------------------------------------------------------- */ 
typedef struct _sys_module_regs_
{
	VUint32 PINMUX[5];         //0x00
	VUint32 BOOTCFG;           //0x14
	VUint32 ARM_INTMUX;        //0x18
	VUint32 EDMA_EVTMUX;       //0x1C
	VUint32 DDR_SLEW;          //0x20
	VUint32 CLKOUT;            //0x24
	VUint32 DEVICE_ID;         //0x28
	VUint32 VDAC_CONFIG;       //0x2C
	VUint32 TIMER64_CTL;       //0x30
	VUint32 USB_PHY_CTRL;      //0x34
	VUint32 MISC;              //0x38
	VUint32 MSTPRI[2];         //0x3C
	VUint32 VPSS_CLKCTL;       //0x44
	VUint32 DEEPSLEEP;         //0x48
	VUint32 DFT_ENABLE;        //0x4C
	VUint32 DEBOUNCE[8];       //0x50
	VUint32 VTPIOCR;           //0x70
} sysModuleRegs;

#define SYSTEM ((sysModuleRegs*) 0x01C40000)
static inline int DataBusWidth(void)
{
     return 0x02; //( ( (SYSTEM->BOOTCFG) >> 3) & 0x1 )?0x01:0x02;
}

/* -------------------------------------------------------------------------- *
 *    ARM Interrupt Controller register structure - See sprue26.pdf for more  *
 *       details.                                                             *
 * -------------------------------------------------------------------------- */
typedef struct _aintc_regs_
{
    VUint32 FIQ0;
    VUint32 FIQ1;
    VUint32 IRQ0;
    VUint32 IRQ1;
    VUint32 FIQENTRY;
    VUint32 IRQENTRY;
    VUint32 EINT0;
    VUint32 EINT1;
    VUint32 INTCTL;
    VUint32 EABASE;
    VUint8 RSVD0[8];
    VUint32 INTPRI0;
    VUint32 INTPRI1;
    VUint32 INTPRI2;
    VUint32 INTPRI3;
    VUint32 INTPRI4;
    VUint32 INTPRI5;
    VUint32 INTPRI6;
    VUint32 INTPRI7;
} aintcRegs;

#define AINTC ((aintcRegs*) 0x01C48000)

/* -------------------------------------------------------------------------- *
 *    PLL Register structure - See sprue14.pdf, Chapter 6 for more details.   *
 * -------------------------------------------------------------------------- */
typedef struct _PLL_regs_
{
	VUint32 PID;
	VUint8 RSVD0[224];
	VUint32 RSTYPE;
	VUint8 RSVD1[24];	
	VUint32 PLLCTL;
	VUint8 RSVD2[12];	
	VUint32 PLLM;
	VUint8 RSVD3[4];
	VUint32 PLLDIV1;
	VUint32 PLLDIV2;
	VUint32 PLLDIV3;	
	VUint8 RSVD4[4]; 
	VUint32 POSTDIV;
	VUint32 BPDIV;
	VUint8 RSVD5[8];	
	VUint32 PLLCMD;
	VUint32 PLLSTAT;
	VUint32 ALNCTL;
	VUint32 DCHANGE;
	VUint32 CKEN;
	VUint32 CKSTAT;
	VUint32 SYSTAT;
	VUint8 RSVD6[12];
	VUint32 PLLDIV4;
	VUint32 PLLDIV5;
} PLLRegs;

#define PLL1 ((PLLRegs*) 0x01C40800)
#define PLL2 ((PLLRegs*) 0x01C40C00)

/* -------------------------------------------------------------------------- *
 *    Power/Sleep Ctrl Register structure - See sprue14.pdf, Chapter 7        * 
 *       for more details.                                                    *
 * -------------------------------------------------------------------------- */
typedef struct _PSC_regs_
{
	VUint32 PID;        // 0x000
	VUint8 RSVD0[12];   // 0x004
	VUint32 GBLCTL;     // 0x010
	VUint8 RSVD1[4];    // 0x014
	VUint32 INTEVAL;    // 0x018
	VUint8 RSVD2[36];	// 0x01C
	VUint32 MERRPR0;    // 0x040
	VUint32 MERRPR1;    // 0x044
	VUint8 RSVD3[8];    // 0x048
	VUint32 MERRCR0;    // 0x050
	VUint32 MERRCR1;    // 0x054
	VUint8 RSVD4[8];    // 0x058
	VUint32 PERRPR;     // 0x060
	VUint8 RSVD5[4];    // 0x064
	VUint32 PERRCR;     // 0x068
	VUint8 RSVD6[4];    // 0x06C
	VUint32 EPCPR;      // 0x070
	VUint8 RSVD7[4];    // 0x074
	VUint32 EPCCR;      // 0x078
	VUint8 RSVD8[132];  // 0x07C
	VUint32 RAILSTAT;   // 0x100
	VUint32 RAILCTL;    // 0x104
	VUint32 RAILSEL;    // 0x108
	VUint8 RSVD9[20];   // 0x10C
	VUint32 PTCMD;      // 0x120
	VUint8 RSVD10[4];   // 0x124
	VUint32 PTSTAT;     // 0x128
	VUint8 RSVD11[212]; // 0x12C
	VUint32 PDSTAT0;    // 0x200
	VUint32 PDSTAT1;    // 0x204
	VUint8 RSVD12[248]; // 0x208
	VUint32 PDCTL0;     // 0x300
	VUint32 PDCTL1;     // 0x304
	VUint8 RSVD13[536]; // 0x308
	VUint32 MCKOUT0;    // 0x520
	VUint32 MCKOUT1;    // 0x524
	VUint8 RSVD14[728]; // 0x528
	VUint32 MDSTAT[41]; // 0x800
	VUint8 RSVD15[348]; // 0x8A4
	VUint32 MDCTL[42];  // 0xA00
} PSCRegs;

#define PSC ((PSCRegs*) 0x01C41000)

/* PSC constants */
#define LPSC_VPSS_MAST      0
#define LPSC_VPSS_SLV       1
#define LPSC_TPCC        	2
#define LPSC_TPTC0       	3
#define LPSC_TPTC1       	4
#define LPSC_EMAC0       	5
#define LPSC_EMAC1       	6
#define LPSC_MDIO           7
#define LPSC_1394           8
#define LPSC_USB            9
#define LPSC_ATA            10
#define LPSC_VLYNQ          11
#define LPSC_HPI            12
#define LPSC_DDR2           13
#define LPSC_AEMIF			14
#define LPSC_MMCSD          15
#define LPSC_MEMSTK         16
#define LPSC_ASP            17
#define LPSC_I2C            18
#define LPSC_UART0          19
#define LPSC_GPIO           26
#define LPSC_TIMER0         27
#define LPSC_ARM            31
#define LPSC_DSP            39
#define LPSC_IMCOP          40

#define EMURSTIE_MASK       0x00000200

#define PSC_ENABLE          0x3
#define PSC_DISABLE         0x2
#define PSC_SYNCRESET       0x1
#define PSC_SWRSTDISABLE    0x0

	
 
/* -------------------------------------------------------------------------- *
 *    DDR2 Memory Ctrl Register structure - See sprue22b.pdf for more details.*
 * -------------------------------------------------------------------------- */
typedef struct _DDR2_MEM_CTL_REGS_
{
	VUint32 EIDRR;	    	//0x00
	VUint32 SDRSTAT;		//0x04
	VUint32 SDBCR;			//0x08
	VUint32 SDRCR;			//0x0C
	VUint32 SDTIMR0;		//0x10
	VUint32 SDTIMR1;		//0x14
	VUint8 RSVD1[8];		//0x18 
	VUint32 PBBPR;			//0x20
	VUint8 RSVD2[156];	    //0x24 
	VUint32 IRR;			//0xC0
	VUint32 IMR;			//0xC4
	VUint32 IMSR;			//0xC8
	VUint32 IMCR;			//0xCC
	VUint8 RSVD3[20];	//0xD0
	VUint32 DDRPHYCR;		//0xE4
	VUint8 RSVD4[8];		//0xE8
	VUint32 VTPIOCR;		//0xF0
} DDR2Regs;

#define DDRVTPR (*((VUint32*) 0x01C42030))
#define DDR ((DDR2Regs*) 0x20000000)
#define DDR_TEST_PATTERN (0xA55AA55Au)
#define DDR_RAM_SIZE (0x10000000u)


/* -------------------------------------------------------------------------- *
 *    AEMIF Register structure - See sprue20a.pdf for more details.           *
 * -------------------------------------------------------------------------- */
typedef struct _emif_regs_
{
    VUint32 ERCSR;      // 0x00
    VUint32 AWCCR;      // 0x04
    VUint32 SDBCR;      // 0x08
    VUint32 SDRCR;      // 0x0C
    VUint32 AB1CR;      // 0x10
    VUint32 AB2CR;      // 0x14
    VUint32 AB3CR;
    VUint32 AB4CR;
    VUint32 SDTIMR;     // 0x20
    VUint32 DDRSR;
    VUint32 DDRPHYCR;
    VUint32 DDRPHYSR;
    VUint32 TOTAR;      // 0x30
    VUint32 TOTACTR;
    VUint32 DDRPHYID_REV;
    VUint32 SDSRETR;
    VUint32 EIRR;       // 0x40
    VUint32 EIMR;
    VUint32 EIMSR;
    VUint32 EIMCR;
    VUint32 IOCTRLR;    // 0x50
    VUint32 IOSTATR;
    VUint8 RSVD0[8];
    VUint32 NANDFCR;    // 0x60
    VUint32 NANDFSR;
    VUint8 RSVD1[8];
    VUint32 NANDF1ECC;  // 0x70
    VUint32 NANDF2ECC;
    VUint32 NANDF3ECC;
    VUint32 NANDF4ECC;
    VUint8 RSVD2[4];    // 0x80
    VUint32 IODFTECR;
    VUint32 IODFTGCR;
    VUint8 RSVD3[4];
    VUint32 IODFTMRLR;  // 0x90
    VUint32 IODFTMRMR;
    VUint32 IODFTMRMSBR;
    VUint8 RSVD4[20];
    VUint32 MODRNR;     // 0xB0
} emifRegs;

#define AEMIF ((emifRegs*) 0x01E10000)

/* -------------------------------------------------------------------------- *
 *    UART Register structure - See sprue33.pdf for more details.             *
 * -------------------------------------------------------------------------- */
typedef struct _uart_regs_
{
	VUint32 RBR;
	VUint32 IER;
	VUint32 IIR;
	VUint32 LCR;
	VUint32 MCR;
	VUint32 LSR;
	VUint32 MSR;
	VUint32 SCR;
	VUint8 DLL;
	VUint8 RSVDO[3];
	VUint8 DLH;
	VUint8 RSVD1[3];	
	VUint32 PID1;
	VUint32 PID2;
	VUint32 PWREMU_MGNT;
} uartRegs;

#define THR RBR
#define FCR IIR

#define UART0  ((uartRegs*) 0x01C20400)//((uartRegs*) 0x01C20000)

/* -------------------------------------------------------------------------- *
 *    Timer Register structure - See sprue26.pdf for more details.             *
 * -------------------------------------------------------------------------- */
typedef struct _timer_regs_
{
    VUint32 PID12;
    VUint32 EMUMGT_CLKSPD;
    VUint32 GPINT_GPEN;
    VUint32 GPTDAT_GPDIR;
    VUint32 TIM12;
    VUint32 TIM34;
    VUint32 PRD12;
    VUint32 PRD34;
    VUint32 TCR;
    VUint32 TGCR;
    VUint32 WDTCR;
    VUint8 RSVD0[8];
    VUint32 REL12;
    VUint32 REL34;
    VUint32 CAP12;
    VUint32 CAP34;
    VUint32 INTCTL_STAT;

} timerRegs;

#define TIMER0 ((timerRegs*) 0x01C21400)

/* -------------------------------------------------------------------------- *
 *    SPI Register structure 
 * -------------------------------------------------------------------------- */
typedef struct _spi_regs_
{
    VUint32 SPIGCR0;              //0x00
    VUint32 SPIGCR1;              //0x04
    VUint32 SPIINT;               //0x08
    VUint32 SPILVL;               //0x0C
    VUint32 SPIFLG;               //0x10
    VUint32 SPIPC0;               //0x14
    VUint8  RSVD0[4];
    VUint32 SPIPC2;               //0x1C
    VUint8  RSVD1[28];
    VUint32 SPIDAT1;              //0x3C
    VUint32 SPIBUF;               //0x40
    VUint32 SPIEMU;               //0x44
    VUint32 SPIDELAY;             //0x48
    VUint32 SPIDEF;               //0x4C
    VUint32 SPIFMT0;              //0x50
    VUint32 SPIFMT1;              //0x54
    VUint32 SPIFMT2;              //0x58
    VUint32 SPIFMT3;              //0x5C
    VUint32 INTVECT0;             //0x60
    VUint32 INTVECT1;             //0x64

} spiRegs;

#define SPI0  ((spiRegs*) 0x01C66000)
#define SPI1  ((spiRegs*) 0x01C66800)
#define SPI2  ((spiRegs*) 0x01C67800)


/* GIO register */
#define	GIO_BINTEN		        0x01C67008		/* GPIO Interrupt Per-Bank Enable Register */
#define GIO_DIR01			    0x01C67010		
#define GIO_OUT_DATA01			0x01C67014		
#define GIO_SET_DATA01			0x01C67018		
#define GIO_CLR_DATA01			0x01C6701C
#define GIO_IN_DATA01			0x01C67020		
#define	GIO_SET_RIS_TRIG01	0x01C67024		    
#define	GIO_SET_FAL_TRIG01	0x01C6702c

//Timer inline functions
static inline void TIMER0Start(void)
{
     AINTC->IRQ1 |= 0x00000001;
     TIMER0->TGCR  = 0x00000000;
     TIMER0->TIM12 = 0x0;
     TIMER0->TCR   = 0x00400040;
     TIMER0->TGCR  = 0x00000003;
}

static inline int TIMER0Status(void)
{
     return ((AINTC->IRQ1)&1);
}

// Function Prototypes

// Execute LPSC state transition
void LPSCTransition(Uint8 module, Uint8 state);

// Initialization prototypes
void DM355EvmInit(void);
void PSCInit(void);
void UARTInit(void);
Int16 EVMDM355_I2C_init( );
void PLL1Init(void);
void PLL2Init(void);
void DDR2Init(void);
void AEMIFInit(void);
Int32 GetStringLen(Uint8* seq);
Uint32 UARTSendData(Uint8* seq, Bool includeNull);
Uint32 UARTSendInt(Uint32 value);
//void IVTInit(void);
  
#define CPLD_PAGE_SIZE   0x8000
#endif // End _DM355EVM_H_
