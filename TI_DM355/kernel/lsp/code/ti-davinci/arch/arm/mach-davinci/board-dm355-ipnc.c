/*
 * TI DaVinci DM355 IPNetCam board
 *
 * Derived from: arch/arm/mach-davinci/board-evm.c
 * Copyright (C) 2006 Texas Instruments.
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

/**************************************************************************
 * Included Files
 **************************************************************************/


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/root_dev.h>
#include <linux/dma-mapping.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <linux/davinci_mmc.h>
#include <linux/nand_davinci.h>

#if defined(CONFIG_USB_MUSB_HDRC) || defined(CONFIG_USB_MUSB_HDRC_MODULE)
#include <linux/usb_musb.h>
#endif

#include <asm/setup.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/gio.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
#include <asm/arch/irqs.h>
#include <asm/arch/hardware.h>
#include <asm/arch/edma.h>
#include <linux/kgdb.h>
#include "clock.h"

/**************************************************************************
 * Definitions
 **************************************************************************/

static struct plat_serial8250_port serial_platform_data[] = {
	{
		.membase = (char *) IO_ADDRESS(DAVINCI_UART0_BASE),
		.mapbase = (unsigned long) DAVINCI_UART0_BASE,
		.irq = IRQ_UARTINT0,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype = UPIO_MEM,
		.regshift = 2,
		.uartclk = 24000000,
        	.line  = 0,

	},
	{
		.membase = (char *) IO_ADDRESS(DAVINCI_UART1_BASE),
		.mapbase = (unsigned long) DAVINCI_UART1_BASE,
		.irq = IRQ_UARTINT1,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype = UPIO_MEM,
		.regshift = 2,
		.uartclk = 24000000,
        	.line  = 1,
	},
	{
		.membase = (char *) IO_ADDRESS(DM355_UART2_BASE),
		.mapbase = (unsigned long) DM355_UART2_BASE,
		.irq = IRQ_DM355_UARTINT2,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype = UPIO_MEM,
		.regshift = 2,
		.uartclk = 24000000,
        	.line  = 2,
	},
	{
		.flags = 0
	},
};

static struct platform_device serial_device = {
	.name = "serial8250",
	.id = 0,
	.dev = {
			.platform_data = serial_platform_data,
		},
};

/**************************************************************************
 * Public Functions
 **************************************************************************/
int cpu_type(void)
{
	return MACH_TYPE_DAVINCI_DM355_IPNC;
}

extern void davinci_serial_init(struct platform_device *pdev);


/*
 * USB
 */
#if defined(CONFIG_USB_MUSB_HDRC) || defined(CONFIG_USB_MUSB_HDRC_MODULE)

static struct musb_hdrc_platform_data usb_data = {
#if     defined(CONFIG_USB_MUSB_OTG)
        /* OTG requires a Mini-AB connector */
        .mode           = MUSB_OTG,
#elif   defined(CONFIG_USB_MUSB_PERIPHERAL)
        .mode           = MUSB_PERIPHERAL,
#elif   defined(CONFIG_USB_MUSB_HOST)
        .mode           = MUSB_HOST,
#endif
        /* irlml6401 switches 5V */
        .power          = 255,          /* sustains 3.0+ Amps (!) */
        .potpgt         = 4,            /* ~8 msec */

        /* REVISIT multipoint is a _chip_ capability; not board specific */
        .multipoint     = 1,
};

static struct resource usb_resources [] = {
	{
		/* physical address */
		.start          = DAVINCI_USB_OTG_BASE,
		.end            = DAVINCI_USB_OTG_BASE + 0x5ff,
		.flags          = IORESOURCE_MEM,
	},
	{
		.start          = IRQ_USBINT,
		.flags          = IORESOURCE_IRQ,
	},
};

static u64 usb_dmamask = DMA_32BIT_MASK;

static struct platform_device usb_dev = {
        .name           = "musb_hdrc",
        .id             = -1,
        .dev = {
                .platform_data  = &usb_data,
                .dma_mask               = &usb_dmamask,
                .coherent_dma_mask      = DMA_32BIT_MASK,
        },
        .resource       = usb_resources,
        .num_resources  = ARRAY_SIZE(usb_resources),
};

static inline void setup_usb(void)
{
        /* REVISIT:  everything except platform_data setup should be
         * shared between all DaVinci boards using the same core.
         */
        int status;

        status = platform_device_register(&usb_dev);
        if (status != 0)
                pr_debug("setup_usb --> %d\n", status);
        else
                board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_USB, 1);
}

#else
static inline void setup_usb(void)
{
        /* NOP */
}
#endif  /* CONFIG_USB_MUSB_HDRC */



#if defined (CONFIG_MTD_NAND_DAVINCI) || defined(CONFIG_MTD_NAND_DAVINCI_MODULE)

/*
 * The NAND partition map used by UBL/U-Boot is a function of the NAND block
 * size.  We support NAND components with either a 128KB or 256KB block size.
 */

 //  #define CONFIG_MACH_DAVINCI_DM355_IPNC_WIFI_MTD
#define NAND_IS_K9F1G08

#define NAND_BLOCK_SIZE (SZ_16K)
static struct mtd_partition nand_partitions[] = {
#if 0
	/* bootloader (UBL, U-Boot, BBT) in sectors: 0 - 14 */
	{
	      .name		= "bootloader",
	      .offset		= 0,
	      .size		= 24*NAND_BLOCK_SIZE,
	      .mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	/* bootloader params in the next sector 15 */
	{
	      .name		= "params",
	      .offset		= MTDPART_OFS_APPEND,
	      .size		= 104*NAND_BLOCK_SIZE,
	      .mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
#else
	/* bootloader (UBL, U-Boot, BBT) in sectors: 0 - 14 */
	{
	      .name		= "bootloader",
	      .offset		= 0,
	      .size			= SZ_2M,
	      .mask_flags	= 0//MTD_WRITEABLE/* force read-only */
	},
#endif
	/* kernel_0 */
	{
	      .name		= "kernel_0",
	      .offset		= MTDPART_OFS_APPEND,
            #ifdef CONFIG_MACH_DAVINCI_DM355_IPNC_WIFI_MTD
	      .size			= SZ_2_5M,
            #else
	      .size			= SZ_2M,
            #endif
	      .mask_flags	= 0//MTD_WRITEABLE
	},
	/* kernel_1 */
	{
	      .name		= "kernel_1",
	      .offset		= MTDPART_OFS_APPEND,
            #ifdef CONFIG_MACH_DAVINCI_DM355_IPNC_WIFI_MTD
	      .size			= SZ_1_5M,
            #else
	      .size			= SZ_2M,
            #endif
	      .mask_flags	= 0//MTD_WRITEABLE
	},
#if 0	// ramdisk  for  root filesystem
	{
	      .name		= "filesystem",
	      .offset		= MTDPART_OFS_APPEND,
	      .size		= SZ_16M+SZ_4M,
	      .mask_flags	= 0
	},
#endif
	/* filesystem_0 */
	{
	      .name		= "filesystem_0",
	      .offset		= MTDPART_OFS_APPEND,
	      .size			= SZ_8M + SZ_2M,
	      .mask_flags	= 0//MTD_WRITEABLE
	},
	/* filesystem_1 */
	{
	      .name		= "filesystem_1",
	      .offset		= MTDPART_OFS_APPEND,
	      .size			= SZ_8M + SZ_2M,
	      .mask_flags	= 0//MTD_WRITEABLE
	},
    /*
    {
	      .name		= "data",	//"data"  6 MBytes
	      .offset		= MTDPART_OFS_APPEND,
	      //Yun .size			= MTDPART_SIZ_FULL,
	      .size			= SZ_2M + SZ_2M + SZ_2M,
	      .mask_flags	= 0
	},
    */
//  	{
//  	      .name		= "data",	//"spare"  32 MBytes
//  	      .offset		= MTDPART_OFS_APPEND,
//  	      .size			= MTDPART_SIZ_FULL,
//  	      .mask_flags	= 0
//  	},
	{
	      .name		= "data",	//"spare"  32 MBytes
	      .offset		= MTDPART_OFS_APPEND,
		  #ifdef NAND_IS_K9F1G08 //128Mbyte
		  //shcho : 2014-02-25 : booting?쒓컙??以꾩씠湲??꾪빐??: 留롮? ?곸뿭?먯꽌 JFFS2 ECC error
//  	      .size			= SZ_101M,
	      .size			= SZ_8M+SZ_2M,
		  #else // K9F5608 32MByte
	      .size			= SZ_5M,
		  #endif
	      .mask_flags	= 0
	},
	{
	      .name		= "bbt",	//"spare"  1M MBytes
	      .offset		= MTDPART_OFS_APPEND,
	      .size			= MTDPART_SIZ_FULL,
	      .mask_flags	= 0
	},


};

/* flash bbt decriptors */
static uint8_t nand_davinci_bbt_pattern[] = { 'B', 'b', 't', '0' };
static uint8_t nand_davinci_mirror_pattern[] = { '1', 't', 'b', 'B' };

static struct nand_bbt_descr nand_davinci_bbt_main_descr = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
	    | NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs = 0,
	.len = 4,
	.veroffs = 5,
	.maxblocks = 4,
	.pattern = nand_davinci_bbt_pattern
};

static struct nand_bbt_descr nand_davinci_bbt_mirror_descr = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
	    | NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs = 0,
	.len = 4,
	.veroffs = 5,
	.maxblocks = 4,
	.pattern = nand_davinci_mirror_pattern
};

static struct nand_davinci_platform_data nand_data = {
	.options	= 0,
	.eccmode	= NAND_ECC_HW10_512,//NAND_ECC_NONE,//NAND_ECC_HW3_512,//NAND_ECC_SOFT,//NAND_ECC_HW10_512,
	.cle_mask	= 0x10,
	.ale_mask	= 0x08,
	.bbt_td		= &nand_davinci_bbt_main_descr,
	.bbt_md		= &nand_davinci_bbt_mirror_descr,
	.parts		= nand_partitions,
	.nr_parts	= ARRAY_SIZE(nand_partitions),
};


static struct resource nand_resources[] = {
	[0] = {		/* First memory resource is AEMIF control registers */
	.start	= DM355_ASYNC_EMIF_CNTRL_BASE,
		.end	= DM355_ASYNC_EMIF_CNTRL_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] ={		/* Second memory resource is NAND I/O window */
		.start	= DAVINCI_ASYNC_EMIF_DATA_CE0_BASE,
		.end	= DAVINCI_ASYNC_EMIF_DATA_CE0_BASE + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},	
};

static struct platform_device nand_device = {
	.name		= "nand_davinci",
	.id		= 0,
	.dev		= {
		.platform_data	= &nand_data
	},

	.num_resources	= ARRAY_SIZE(nand_resources),
	.resource	= nand_resources,
};

static void setup_nand(void)
{
	void __iomem *pinmux2 =
		(void __iomem *) IO_ADDRESS(DAVINCI_SYSTEM_MODULE_BASE + 0x8);

	/* Configure the pin multiplexing to enable all of the EMIF signals */
	__raw_writel(0x00000C40, pinmux2);
}

#else
#define setup_nand()
#endif

#if defined(CONFIG_DM9000) || defined(CONFIG_DM9000_MODULE)

#define ETH_PHYS	(DAVINCI_ASYNC_EMIF_DATA_CE1_BASE)
#define ETH_PHYS_DATA	(ETH_PHYS+16)
#define ETH_IRQ_USE	(IRQ_DM355_GPIO9)


static struct resource dm9000_resources[] = {
	[0] = {
		.start	= ETH_PHYS,
		.end	= ETH_PHYS + 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= ETH_PHYS_DATA,
		.end	= ETH_PHYS_DATA + 1,
		
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= ETH_IRQ_USE,
		.end	= ETH_IRQ_USE,
		.flags	= (IORESOURCE_IRQ | IRQT_RISING),
	},
};

static struct platform_device dm9000_device = {
	.name		= "dm9000",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(dm9000_resources),
	.resource	= dm9000_resources,
};

static void setup_ethernet(void)
{
	u32 gpio = 9; 
	/* Enable interrupt generation from GPIO Bank 0 (GPIO0-GPIO15) */
	gpio_interrupt_bank_enable(gpio);

	/* Configure GPIO1 as an input */
	gpio_set_direction(gpio, GIO_DIR_INPUT);

	/* Configure GPIO1 to generate an interrupt on rising edge only */
	gpio_interrupt_enable(gpio, TRIGGER_RISING_EDGE);
	gpio_interrupt_disable(gpio, TRIGGER_FALLING_EDGE);

}
#else
#define setup_ethernet()
#endif

#if defined(CONFIG_MMC_DAVINCI) || defined(CONFIG_MMC_DAVINCI_MODULE)
static struct resource mmc0_resources[] = {
	[0] = {			/* registers */
		.start	= DM355_MMC0_BASE,
		.end	= DM355_MMC0_BASE + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {			/* interrupt */
		.start	= IRQ_DM355_MMCINT0,
		.end	= IRQ_DM355_MMCINT0,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {			/* dma rx */
		.start	= DM355_DMA_MMC0RXEVT,
		.end	= DM355_DMA_MMC0RXEVT,
		.flags	= IORESOURCE_DMA,
	},
	[3] = {			/* dma tx */
		.start	= DM355_DMA_MMC0TXEVT,
		.end	= DM355_DMA_MMC0TXEVT,
		.flags	= IORESOURCE_DMA,
	},
};

static struct davinci_mmc_platform_data mmc0_platform_data = {
	.mmc_clk = "MMCSDCLK0",
	.rw_threshold = 32,
	.use_4bit_mode = 1,
};

static struct platform_device mmc0_device = {
	.name = "mmc",
	.id = 0,
	.dev = {
		.platform_data = &mmc0_platform_data,
		},
	.num_resources = ARRAY_SIZE(mmc0_resources),
	.resource = mmc0_resources,
};

static void setup_mmc(void)
{
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_MMC_SD0, 1);
}
#else
#define setup_mmc()
#endif

static struct platform_device *dm355_evm_devices[] __initdata =
{
	&serial_device,
#if defined (CONFIG_MTD_NAND_DAVINCI) || defined(CONFIG_MTD_NAND_DAVINCI_MODULE)
	&nand_device,
#endif
#if defined(CONFIG_DM9000) || defined(CONFIG_DM9000_MODULE)
	&dm9000_device,
#endif
#if defined(CONFIG_MMC_DAVINCI) || defined(CONFIG_MMC_DAVINCI_MODULE)
	&mmc0_device,
#endif
};

static void board_init(void)
{
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_VPSSMSTR, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_VPSSSLV, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPCC, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPTC0, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPTC1, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_GPIO, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_McBSP1, 1);
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_SPI, 1);
	// add for uart 0
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_UART0, 1);

	/* Turn on WatchDog timer LPSC.  Needed for RESET to work */
	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TIMER2, 1);
	davinci_serial_init(&serial_device);
	davinci_writel(0x56837210, DAVINCI_VPSS_REGS_BASE + 0x810);
	davinci_writel(0x7b3c0004, DAVINCI_VPSS_REGS_BASE + 0x814);

}

static void __init
davinci_map_io(void)
{
	davinci_map_common_io();

#ifdef CONFIG_KGDB_8250
        early_serial_setup((struct uart_port *)&serial_platform_data[kgdb8250_ttyS]);
        kgdb8250_add_platform_port(kgdb8250_ttyS, &serial_platform_data[kgdb8250_ttyS]);
#endif

	/* Board-specific initialization */
	board_init ();
}


#define REG_PLLCTL1				__REG(0x01c40800)
#define REG_PLLCTL1_PLL_DIV4	__REG(0x01c40800+0x160)

#define REG_PLLCTL2			__REG(0x01c40c00)
#define REG_PLLCTL2_PLLM	__REG(0x01c40c00+0x110)

#define REG_PINMUX0			__REG(0x01c40000)	
#define REG_PINMUX1			__REG(0x01c40004)	
#define REG_PINMUX2			__REG(0x01c40008)	
#define REG_PINMUX3			__REG(0x01c4000c)	
#define REG_PINMUX4			__REG(0x01c40010)	

static void setup_nc100m_board(void)
{
	/* DDR Clock */
	//unsigned int pllc2_pllm = REG_PLLCTL2_PLLM;
	
	//REG_PLLCTL2_PLLM = 0x90;	//216 Mhz

	//don't uncomment
	//msleep(100);
	//printk("\nPLLM : [0x%x] ===> [0x90], DDR clock to 216 Mhz\n", pllc2_pllm);

	//unsigned int pll2_div4 = REG_PLLCTL1_PLL_DIV4;
	
	//REG_PLLCTL2_PLLM = 0x90;	//216 Mhz

	//don't uncomment
	//msleep(100);

	//REG_PLLCTL1_PLL_DIV4 = 0x8001;

	//printk("\nPLL1 DIV4 : [0x%x]=======> 0x8001 \n", pll2_div4);

	/* GIO 94 ~ GIO 101 [set to gpio] */
	REG_PINMUX0 &= ~(1 << 9);// 94,95
	REG_PINMUX0 &= ~(1 << 8);// 96,97
	/* if need spi1.. just modify this */
	REG_PINMUX0 &= ~(3 << 6);// 98
	REG_PINMUX0 &= ~(3 << 4);// 99
	REG_PINMUX0 &= ~(3 << 2);// 100
	REG_PINMUX0 &= ~(3 << 0);// 101

	/* nand control */
	// REG_PINMUX2

	/* GIO 8~11, 19 ~ GIO 28 [set to gpio] */
	REG_PINMUX3 &= ~(1 << 0);// 30 - mode 1
	REG_PINMUX3 &= ~(1 << 1);// 29 - mode 0
	REG_PINMUX3 &= ~(1 << 2);// 28 
	REG_PINMUX3 &= ~(1 << 3);// 27
	REG_PINMUX3 &= ~(1 << 4);// 26
	REG_PINMUX3 &= ~(1 << 5);// 25
	REG_PINMUX3 &= ~(1 << 6);// 24
	REG_PINMUX3 &= ~(1 << 7);// 23 - boot led
	REG_PINMUX3 &= ~(3 << 8);// 22
	REG_PINMUX3 &= ~(3 << 10);// 21
	REG_PINMUX3 &= ~(3 << 12);// 20
	REG_PINMUX3 &= ~(3 << 14);// 19
	REG_PINMUX3 &= ~(1 << 18);// 16 sd_en
	REG_PINMUX3 &= ~(1 << 23);// 11
	REG_PINMUX3 &= ~(1 << 24);// 10
	REG_PINMUX3 &= ~(3 << 25);// 9
	REG_PINMUX3 &= ~(1 << 27);// 8

	/* booting led on */
	gpio_set_direction(23, GIO_DIR_OUTPUT);

	gpio_set_value(23, GIO_STATE_LOW);
	
	/* sd enable */
	gpio_set_direction(16, GIO_DIR_OUTPUT);

	gpio_set_value(16, GIO_STATE_HIGH);

}

static __init void davinci_init(void)
{
	setup_ethernet(); // ignored ...
	setup_mmc();
	setup_nand();

	setup_nc100m_board();

	platform_add_devices(dm355_evm_devices,
		ARRAY_SIZE(dm355_evm_devices));

}

extern void davinci_irq_init(void);
extern struct sys_timer davinci_timer;

MACHINE_START(DAVINCI_EVM, "DaVinci DM355 IPNetCam")
    MAINTAINER("MontaVista Software")
    BOOT_MEM(DAVINCI_DDR_BASE, IO_PHYS, IO_VIRT)
    BOOT_PARAMS(0x80000100)
    MAPIO(davinci_map_io)
    INITIRQ(davinci_irq_init)
    .timer = &davinci_timer,
    INIT_MACHINE(davinci_init)
    MACHINE_END
EXPORT_SYMBOL(cpu_type);
