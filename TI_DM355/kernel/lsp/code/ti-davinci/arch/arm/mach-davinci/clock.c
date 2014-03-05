/*
 * linux/arch/arm/mach-davinci/clock.c
 *
 * TI DaVinci clock config file
 *
 * Copyright (C) 2006 Texas Instruments.
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 *
 */

/**************************************************************************
 * Included Files
 **************************************************************************/

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/root_dev.h>

#include <asm/setup.h>
#include <asm/semaphore.h>
#include <asm/hardware/clock.h>
#include <asm/io.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/arch/hardware.h>
#include <asm/arch/cpu.h>
#include "clock.h"

#define DAVINCI_MAX_CLK 12

#define PLL1_PLLM   __REG(0x01c40910)
#define PLL2_PLLM   __REG(0x01c40D10)
#define PTCMD       __REG(0x01C41120)
#define PDSTAT      __REG(0x01C41200)
#define PDCTL1      __REG(0x01C41304)
#define EPCPR       __REG(0x01C41070)
#define PTSTAT      __REG(0x01C41128)

#define MDSTAT  IO_ADDRESS(0x01C41800)
#define MDCTL   IO_ADDRESS(0x01C41A00)
#define VDD3P3V_PWDN  __REG(0x01C40048)

#define PINMUX0     __REG(0x01c40000)
#define PINMUX1     __REG(0x01c40004)
#define PINMUX3     __REG(0x01C4000C)
#define PINMUX4     __REG(0x01c40010)

static LIST_HEAD(clocks);
static DECLARE_MUTEX(clocks_sem);
static DEFINE_RAW_SPINLOCK(clockfw_lock);
static unsigned int commonrate;
static unsigned int armrate;
static unsigned int fixedrate;

/**************************************
 Routine: board_setup_psc
 Description:  Enable/Disable a PSC domain
**************************************/

void board_setup_psc(unsigned int domain, unsigned int id, char enable)
{
	volatile unsigned int *mdstat = (unsigned int *)((int)MDSTAT + 4 * id);
	volatile unsigned int *mdctl = (unsigned int *)((int)MDCTL + 4 * id);

	if (enable) {
		*mdctl |= 0x00000003;	/* Enable Module */
	} else {
		*mdctl &= 0xFFFFFFF2;	/* Disable Module */
	}

	if ((PDSTAT & 0x00000001) == 0) {
		PDCTL1 |= 0x1;
		PTCMD = (1 << domain);
		while ((((EPCPR >> domain) & 1) == 0)) ;

		PDCTL1 |= 0x100;
		while (!(((PTSTAT >> domain) & 1) == 0)) ;
	} else {
		PTCMD = (1 << domain);
		while (!(((PTSTAT >> domain) & 1) == 0)) ;
	}

	if (enable) {
		while (!((*mdstat & 0x0000001F) == 0x3)) ;
	} else {
		while (!((*mdstat & 0x0000001F) == 0x2)) ;
	}
}

static int board_setup_peripheral(unsigned int id)
{
	switch (id) {
	case DAVINCI_LPSC_ATA:
		if (cpu_is_davinci_dm6443()) {
			/* DAVINCI_LPSC_ATA */
			PINMUX0 |= (1 << 17) | (1 << 16);
		}
		else if (cpu_is_davinci_dm355()) {
			/* DAVINCI_LPSC_PWM3 */
			PINMUX1 |=  0x3ca000;
			PINMUX1 &= ~0x005000;
		}
		break;
	case DAVINCI_LPSC_MMC_SD0:
		if (cpu_is_davinci_dm6443()) {
			/* VDD power manipulations are done in U-Boot for CPMAC
			 * so applies to MMC as well
			 */
			/*Set up the pull register for MMC */
			VDD3P3V_PWDN = 0x0;
			PINMUX1 &= (~(1 << 9));
		}
		else if (cpu_is_davinci_dm355())
			PINMUX4 &= (~(1 << 2));
		break;
	case DAVINCI_LPSC_MMC_SD1:
		if (cpu_is_davinci_dm355()) {
			PINMUX3 &= ~0x0000ff00;
			PINMUX3 |=  0x000055c0;
		}
		break;
	case DAVINCI_LPSC_I2C:
		if (cpu_is_davinci_dm6443())
			PINMUX1 |= (1 << 7);
		else if (cpu_is_davinci_dm355()) {
			PINMUX3 |= (1 << 19);
			PINMUX3 |= (1 << 20);
		}
		break;
	case DAVINCI_LPSC_McBSP0:
		if (cpu_is_davinci_dm6443())
			PINMUX1 |= (1 << 10);
		else if (cpu_is_davinci_dm355())
			PINMUX3 |= 0x3F;
		break;
	case DAVINCI_LPSC_PWM0:
		if (cpu_is_davinci_dm6443())
			PINMUX1 |= (1 << 4);
		else if (cpu_is_davinci_dm355()) {
			PINMUX1 |=  0x2;
			PINMUX1 &= ~0x1;
		}
		break;
	case DAVINCI_LPSC_PWM1:
		if (cpu_is_davinci_dm6443())
			PINMUX1 |= (1 << 5);
		else if (cpu_is_davinci_dm355()) {
			PINMUX1 |=  0x8;
			PINMUX1 &= ~0x4;
		}
		break;
	case DAVINCI_LPSC_PWM2:
		if (cpu_is_davinci_dm6443())
			PINMUX1 |= (1 << 6);
		else if (cpu_is_davinci_dm355()) {
			PINMUX1 |=  0xaa0;
			PINMUX1 &= ~0x550;
		}
		break;
	default:
		break;
	}
}
struct clk *clk_get(struct device *dev, const char *id)
{
	struct clk *p, *clk = ERR_PTR(-ENOENT);

	down(&clocks_sem);
	list_for_each_entry(p, &clocks, node) {
		if (strcmp(id, p->name) == 0 && try_module_get(p->owner)) {
			clk = p;
			break;
		}
	}

	up(&clocks_sem);

	return clk;
}

EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
	if (clk && !IS_ERR(clk))
		module_put(clk->owner);
}

EXPORT_SYMBOL(clk_put);

int __clk_enable(struct clk *clk)
{
	if (clk->flags & ALWAYS_ENABLED)
		return 0;

	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, clk->lpsc, 1);
	return 0;
}

void __clk_disable(struct clk *clk)
{
	if (clk->usecount)
		return;

	board_setup_psc(DAVINCI_GPSC_ARMDOMAIN, clk->lpsc, 0);
}

void __clk_unuse(struct clk *clk)
{
	if (clk->usecount > 0) {
		--clk->usecount;
	}
}

int __clk_use(struct clk *clk)
{
	int ret = 0;

	clk->usecount++;

	return ret;
}

int clk_enable(struct clk *clk)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&clockfw_lock, flags);
	ret = __clk_enable(clk);
	spin_unlock_irqrestore(&clockfw_lock, flags);
	board_setup_peripheral(clk->lpsc);
	return ret;
}

EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	unsigned long flags;

	spin_lock_irqsave(&clockfw_lock, flags);
	__clk_disable(clk);
	spin_unlock_irqrestore(&clockfw_lock, flags);
}

EXPORT_SYMBOL(clk_disable);

int clk_use(struct clk *clk)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&clockfw_lock, flags);
	ret = __clk_use(clk);
	spin_unlock_irqrestore(&clockfw_lock, flags);
	return ret;
}

EXPORT_SYMBOL(clk_use);

void clk_unuse(struct clk *clk)
{
	unsigned long flags;

	spin_lock_irqsave(&clockfw_lock, flags);
	__clk_unuse(clk);
	spin_unlock_irqrestore(&clockfw_lock, flags);
}

EXPORT_SYMBOL(clk_unuse);

unsigned long clk_get_rate(struct clk *clk)
{
	return *(clk->rate);
}

EXPORT_SYMBOL(clk_get_rate);

int clk_register(struct clk *clk)
{
	down(&clocks_sem);
	list_add(&clk->node, &clocks);
	up(&clocks_sem);
	return 0;
}

EXPORT_SYMBOL(clk_register);

void clk_unregister(struct clk *clk)
{
	down(&clocks_sem);
	list_del(&clk->node);
	up(&clocks_sem);
}

EXPORT_SYMBOL(clk_unregister);

static struct clk davinci_clks[] = {
	{
		.name = "ARMCLK",
		.rate = &armrate,
		.lpsc = -1,
		.flags = ALWAYS_ENABLED,
	},
	{
		.name = "UART0",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_UART0,
		.usecount = 1,
	},
	{
		.name = "UART1",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_UART1,
		.usecount = 1,
	},
	{
		.name = "UART2",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_UART2,
		.usecount = 1,
	},
	{
		.name = "EMACCLK",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_EMAC_WRAPPER,
	},
	{
		.name = "I2CCLK",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_I2C,
	},
	{
		.name = "IDECLK",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_ATA,
	},
	{
		.name = "McBSPCLK0",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_McBSP0,
	},
	{
		.name = "McBSPCLK1",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_McBSP1,
	},
	{
		.name = "MMCSDCLK0",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_MMC_SD0,
	},
	{
		.name = "MMCSDCLK1",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_MMC_SD1,
	},
	{
		.name = "SPICLK",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_SPI,
	},
	{
		.name = "gpio",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_GPIO,
	 },
	{
		.name = "AEMIFCLK",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_AEMIF,
		.usecount = 1,
	},
	{
		.name = "PWM0_CLK",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_PWM0,
	},
	{
		.name = "PWM1_CLK",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_PWM1,
	},
	{
		.name = "PWM2_CLK",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_PWM2,
	},
	{
		.name = "PWM3_CLK",
		.rate = &fixedrate,
		.lpsc = DAVINCI_LPSC_PWM3,
	},
	{
		.name = "USBCLK",
		.rate = &commonrate,
		.lpsc = DAVINCI_LPSC_USB,
	},
};

void davinci_clk_init(void)
{
	struct clk *clkp;
	int count = 0;

	if (cpu_is_davinci_dm355()) {
		/*
		 * FIXME
		 * We're assuming a 24MHz reference, but the DM355 also
		 * supports a 36MHz reference.
		 */
		unsigned long postdiv;

		/*
		 * Read the PLL1 POSTDIV register to determine if the post
		 * divider is /1 or /2
		 */
		postdiv = (davinci_readl(DAVINCI_PLL_CNTRL0_BASE + 0x128)
			& 0x1f) + 1;

		fixedrate = 24000000;
		armrate = (PLL1_PLLM + 1)*(fixedrate/(16*postdiv));
		commonrate = armrate/2;
	}
	else {
		fixedrate = 27000000;
		armrate = (PLL1_PLLM + 1)*(fixedrate/2);
		commonrate = armrate/3;
	}

	for (clkp = davinci_clks; count < ARRAY_SIZE(davinci_clks);
		count++, clkp++)
	{
		clk_register(clkp);

		/* Turn on clocks that have been enabled in the
		 * table above */
		if (clkp->usecount) {
			clk_enable(clkp);
		}
	}
}
