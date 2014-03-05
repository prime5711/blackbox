/*
 * Copyright (C) 2005-2006 by Texas Instruments
 *
 * This file is part of the Inventra Controller Driver for Linux.
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * The Inventra Controller Driver for Linux is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
// #include <linux/clk.h>
#include <asm/hardware/clock.h>

#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/memory.h>
#include <asm/mach-types.h>
#ifdef CONFIG_ARCH_DAVINCI355
#include <asm/arch/gio.h>
#include <asm/arch/cpu.h>
#endif
// #ifdef CONFIG_USB_MUSB_HDRC_HCD
#include <linux/usb.h>
#include "../core/hcd.h"
// #endif

#include "musbdefs.h"
// #ifdef CONFIG_USB_MUSB_HDRC_HCD
#include "musb_host.h"
// #endif

#ifdef CONFIG_ARCH_DAVINCI

#if defined (CONFIG_MACH_DAVINCI_EVM) || (CONFIG_MACH_DAVINCI_HD_EVM)        
#include <asm/arch/i2c-client.h>
#endif

#include "davinci.h"
#endif

#ifdef CONFIG_USB_TI_CPPI_DMA
#include "cppi_dma.h"
#endif
/*
 * USB
 */
//#if defined(CONFIG_USB_MUSB_HDRC) || defined(CONFIG_USB_MUSB_HDRC_MODULE)

static struct musb_hdrc_platform_data usb_data = {
#if     defined(CONFIG_USB_MUSB_OTG)
	/* OTG requires a Mini-AB connector */
	.mode = MUSB_OTG,
#elif   defined(CONFIG_USB_MUSB_PERIPHERAL)
	.mode = MUSB_PERIPHERAL,
#elif   defined(CONFIG_USB_MUSB_HOST)
	.mode = MUSB_HOST,
#endif
	/* irlml6401 switches 5V */
	.power = 255,           /* sustains 3.0+ Amps (!) */
	.potpgt = 4,            /* ~8 msec */

/* REVISIT multipoint is a _chip_ capability; not board specific */
	.multipoint = 1,
};

static struct resource usb_resources[] = { {
		/* physical address */
		.start = DAVINCI_USB_OTG_BASE,
		.end = DAVINCI_USB_OTG_BASE + 0x5ff,
		.flags = IORESOURCE_MEM,
	}, {
		.start = IRQ_USBINT,
		.flags = IORESOURCE_IRQ,
#ifdef CONFIG_ARCH_DAVINCI646x
	}, {
		.start = IRQ_USBDMAINT,
		.flags = IORESOURCE_IRQ,
#endif
	},
};

static u64 usb_dmamask = DMA_32BIT_MASK;

struct platform_device usb_dev = {
	.name = "musb_hdrc",
	.id = -1,
	.dev = {
		.platform_data = &usb_data,
		.dma_mask = &usb_dmamask,
		.coherent_dma_mask = DMA_32BIT_MASK,
	},
	.resource = usb_resources,
	.num_resources = ARRAY_SIZE(usb_resources),
};


static inline void phy_on(void)
{
#ifdef CONFIG_MACH_DAVINCI_EVM
	/* start the on-chip PHY and its PLL */
	__raw_writel(USBPHY_SESNDEN | USBPHY_VBDTCTEN | USBPHY_PHYPLLON,
		IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
#ifdef CONFIG_ARCH_DAVINCI646x
	__raw_writel(0, IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
#ifdef CONFIG_ARCH_DAVINCI355
	/* start the on-chip PHY and its PLL; Also swap USB_DP and USB_DM signals */
	__raw_writel(USBPHY_SESNDEN | USBPHY_VBDTCTEN | USBPHY_PHYPLLON,
		IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
	while ((__raw_readl(IO_ADDRESS(USBPHY_CTL_PADDR))
			& USBPHY_PHYCLKGD) == 0)
		cpu_relax();
}

static inline void phy_off(void)
{
	/* powerdown the on-chip PHY and its oscillator */
#if defined(CONFIG_MACH_DAVINCI_EVM) || defined(CONFIG_ARCH_DAVINCI355)
	__raw_writel(USBPHY_OSCPDWN | USBPHY_PHYSPDWN,
		     IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
#ifdef CONFIG_ARCH_DAVINCI646x
	__raw_writel(USBPHY_PHYSPDWN, IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
}

void musb_platform_enable(struct musb *musb)
{
	u32 tmp, old, val;

	/* workaround:  setup irqs through both register sets */
	tmp = (musb->wEndMask & DAVINCI_USB_TX_ENDPTS_MASK)
	    << DAVINCI_USB_TXINT_SHIFT;
	musb_writel(musb->ctrl_base, DAVINCI_USB_INT_MASK_SET_REG, tmp);
	old = tmp;
	tmp = (musb->wEndMask & (0xfffe & DAVINCI_USB_RX_ENDPTS_MASK))
	    << DAVINCI_USB_RXINT_SHIFT;
	musb_writel(musb->ctrl_base, DAVINCI_USB_INT_MASK_SET_REG, tmp);
	tmp |= old;

	val = ~MGC_M_INTR_SOF;
	tmp |= ((val & 0x01ff) << DAVINCI_USB_USBINT_SHIFT);
	musb_writel(musb->ctrl_base, DAVINCI_USB_INT_MASK_SET_REG, tmp);
}

/*
 * Disable the HDRC and flush interrupts
 */
void musb_platform_disable(struct musb *musb)
{
	/* because we don't set CTRLR.UINT, "important" to:
	 *  - not read/write INTRUSB/INTRUSBE
	 *  - (except during initial setup, as workaround)
	 *  - use INTSETR/INTCLRR instead
	 */
	musb_writel(musb->ctrl_base, DAVINCI_USB_INT_MASK_CLR_REG,
		    DAVINCI_USB_USBINT_MASK
		    | DAVINCI_USB_TXINT_MASK | DAVINCI_USB_RXINT_MASK);
	musb_writeb(musb->pRegs, MGC_O_HDRC_DEVCTL, 0);
	musb_writel(musb->ctrl_base, DAVINCI_USB_EOI_REG, 0);
}

/* REVISIT this file shouldn't modify the OTG state machine ...
 *
 * The OTG infrastructure needs updating, to include things like
 * offchip DRVVBUS support and replacing MGC_OtgMachineInputs with
 * musb struct members (so e.g. vbus_state vanishes).
 */
#if defined (CONFIG_MACH_DAVINCI_EVM) || (CONFIG_MACH_DAVINCI_HD_EVM)
static int vbus_state = -1;

/* I2C operations are always synchronous, and require a task context.
 * With unloaded systems, using the shared workqueue seems to suffice
 * to satisfy the 100msec A_WAIT_VRISE timeout...
 */
static void evm_deferred_drvvbus(void *_musb)
{
	int is_on = vbus_state;

#ifdef CONFIG_MACH_DAVINCI_EVM
	davinci_i2c_expander_op(0x3a, USB_DRVVBUS, !is_on);
#endif
#ifdef CONFIG_MACH_DAVINCI_HD_EVM
	davinci_i2c_expander_op(0x3a, USB_FB, !is_on);
#endif
	vbus_state = is_on;
}

DECLARE_WORK(evm_vbus_work, evm_deferred_drvvbus, 0);


/* VBUS SWITCHING IS BOARD-SPECIFIC */
void davinci_vbus_power(struct musb *musb, int is_on, int sleeping)
{
	if (is_on)
		is_on = 1;

	DBG(1, "vbus_state=>%d\n", is_on);

	if (is_on) {
		MUSB_HST_MODE(musb);
	} else {
		MUSB_DEV_MODE(musb);
	}

#if defined (CONFIG_MACH_DAVINCI_EVM) || (CONFIG_MACH_DAVINCI_HD_EVM)        
	if (machine_is_davinci_evm()) {
#ifdef CONFIG_USB_MUSB_OTG

		/* modified EVM board switching VBUS with GPIO(6) not I2C
		 * NOTE:  PINMUX0.RGB888 (bit23) must be clear
		 */
		if (!is_on) {
			REG_DVEVM_GPIO45_SET |= DAVINCI_VBUS_OFF;
		} else {
			REG_DVEVM_GPIO45_CLR |= DAVINCI_VBUS_ON;
		}
#else
		/* Do not turn off the USB bus.  This results in the IDE HDD
		 * getting reset.
		 */
		vbus_state = is_on;
		DBG(2, "VBUS power %s, %s\n", is_on ? "on" : "off",
		    sleeping ? "immediate" : "deferred");
		schedule_work(&evm_vbus_work);
		return;
#endif
	}
#endif
}
#endif

static irqreturn_t davinci_interrupt(int irq, void *__hci, struct pt_regs *r)
{
	unsigned long flags;
	irqreturn_t retval = IRQ_NONE;
	struct musb *musb = __hci;
	void *__iomem tibase = musb->ctrl_base;
	u32 tmp;

	spin_lock_irqsave(&musb->Lock, flags);

#ifdef CONFIG_USB_TI_CPPI_DMA
	/* CPPI interrupts share the same IRQ line, but have their own
	 * mask, state, and EIO registers.
	 */
	{
		u32 cppi_tx = musb_readl(tibase, DAVINCI_TXCPPI_MASKED_REG);
		u32 cppi_rx = musb_readl(tibase, DAVINCI_RXCPPI_MASKED_REG);

		if (cppi_tx || cppi_rx) {
			DBG(4, "<== CPPI IRQ t%x r%x\n", cppi_tx, cppi_rx);
			cppi_completion(musb, cppi_rx, cppi_tx);
			retval = IRQ_HANDLED;
		}
	}
#endif

	/* NOTE: DaVinci shadows the Mentor IRQs; don't manage them through
	 * the mentor registers (except for setup), use the TI ones and EOI.
	 */

	/* ack and handle non-CPPI interrupts */
	tmp = musb_readl(tibase, DAVINCI_USB_INT_SRC_MASKED_REG);
	musb_writel(tibase, DAVINCI_USB_INT_SRC_CLR_REG, tmp);

	musb->int_rx = (tmp & DAVINCI_USB_RXINT_MASK)
	    >> DAVINCI_USB_RXINT_SHIFT;
	musb->int_tx = (tmp & DAVINCI_USB_TXINT_MASK)
	    >> DAVINCI_USB_TXINT_SHIFT;
	musb->int_usb = (tmp & DAVINCI_USB_USBINT_MASK)
	    >> DAVINCI_USB_USBINT_SHIFT;
	musb->int_regs = r;

	if (tmp & (1 << (8 + DAVINCI_USB_USBINT_SHIFT))) {
		int drvvbus = musb_readl(tibase, DAVINCI_USB_STAT_REG);

		/* NOTE:  this must complete poweron within 100 msec */
		DBG(1, "drvvbus Interrupt\n");
#ifndef CONFIG_ARCH_DAVINCI355
#ifdef CONFIG_USB_MUSB_OTG
		davinci_vbus_power(musb, drvvbus, drvvbus);
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
		/* In host mode manipulate vbus based on core request but
		 * keep the session on.
		 */
		davinci_vbus_power(musb, drvvbus, 1);
#endif
#endif
		drvvbus = 0;
		retval = IRQ_HANDLED;
	}

	if (musb->int_tx || musb->int_rx || musb->int_usb)
		retval |= musb_interrupt(musb);

	/* irq stays asserted until EOI is written */
	musb_writel(tibase, DAVINCI_USB_EOI_REG, 0);

	spin_unlock_irqrestore(&musb->Lock, flags);

	/* REVISIT we sometimes get unhandled IRQs with CPPI
	 * (minimally, host TX).  not clear why...
	 */
	if (retval != IRQ_HANDLED)
		DBG(5, "unhandled? %08x\n", tmp);
	return IRQ_HANDLED;
}

int __init musb_platform_init(struct musb *musb)
{
	void *__iomem tibase = musb->ctrl_base;
	u32 revision, phystatus;
#ifdef CONFIG_ARCH_DAVINCI
	struct clk *clkp;
	
	clkp = clk_get (NULL, "USBCLK");
	if (IS_ERR(clkp))
		return -ENODEV;

	musb->clock = clkp;	
	clk_use (clkp);
	if(clk_enable (clkp) != 0)
		return -ENODEV;
#endif
	musb->pRegs += DAVINCI_BASE_OFFSET;

	/* returns zero if e.g. not clocked */
	revision = musb_readl(tibase, DAVINCI_USB_VERSION_REG);
	if (revision == 0)
		return -ENODEV;

	/* note that transceiver issues make us want to charge
	 * VBUS only when the PHY PLL is not active.
	 */
#if defined(CONFIG_MACH_DAVINCI_EVM) || defined (CONFIG_MACH_DAVINCI_HD_EVM)
#ifdef CONFIG_USB_MUSB_OTG
	/* clear EMACEN to enble OTG GPIO 16 for VBus power control */
	/* Set GPIO Direction */
	REG_DVEVM_GPIO45_DIR &= ~(DVEVM_GPIO45_DIR_OUT);
	DAVINCI_PINMUX0 &= ~(0x80000000);
#endif
	evm_vbus_work.data = musb;
#endif
#ifdef CONFIG_ARCH_DAVINCI355
	gpio_set_direction(2, GIO_DIR_OUTPUT);
#ifdef CONFIG_USB_MUSB_HDRC_HCD
	__gpio_set(2, 1);
#else
	__gpio_set(2, 0);
#endif
	 __raw_writel( __raw_readl(IO_ADDRESS(DEEPSLEEP_REG)) & 0xfffffff0,
			IO_ADDRESS(DEEPSLEEP_REG));
#endif
	/* reset the controller */
	musb_writel(tibase, DAVINCI_USB_CTRL_REG, 0x1);
	phy_on();

	msleep(5);
	phystatus = __raw_readl(IO_ADDRESS(USBPHY_CTL_PADDR));
#ifdef CONFIG_ARCH_DAVINCI646x
#ifdef CONFIG_USB_MUSB_HDRC_HCD
	__raw_writel(phystatus | USBPHY_SESSION_VBUS | USBPHY_NDATAPOL,
		IO_ADDRESS(USBPHY_CTL_PADDR));
#else
	__raw_writel (phystatus | USBPHY_SESSION_VBUS | USBPHY_NDATAPOL |
			 USBPHY_PERI_USBID, IO_ADDRESS(USBPHY_CTL_PADDR));
#endif
#endif

	/* NOTE:  irqs are in mixed mode, not bypass to pure-musb */
	pr_debug("DaVinci OTG revision %08x phy %03x control %02x\n",
		 revision,
		 musb_readl((void *__iomem)IO_ADDRESS(USBPHY_CTL_PADDR), 0x00),
		 musb_readb(tibase, DAVINCI_USB_CTRL_REG));

	musb->isr = davinci_interrupt;
	return 0;
}

int musb_platform_exit(struct musb *musb)
{
	phy_off();
#ifdef CONFIG_MACH_DAVINCI_EVM
	davinci_vbus_power(musb, 0 /*off */ , 1);
#ifdef CONFIG_USB_MUSB_OTG
	/* Set EMACEN to enable OTG GPIO 16 for Emac control */
	/* Set GPIO Direction */
	DAVINCI_PINMUX0 |= (0x80000000);
#endif
#endif
#if	defined (CONFIG_MACH_DAVINCI_HD_EVM) && defined(CONFIG_USB_MUSB_HDRC_HCD)
	davinci_vbus_power(musb, 0 /*off */ , 1);
	//DAVINCI_PINMUX0 |= 0x10000000;
	//VDD3P3V_PWDN |= 0x10000000;
#endif	
	clk_disable (musb->clock);
	clk_unuse (musb->clock);

	return 0;
}
