/*
 * Copyright (C) 2004 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/serial_reg.h>
#include <linux/interrupt.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/time.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmnvram.h>
#include <asm/brcm/sbconfig.h>
#include <asm/brcm/sbextif.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/sbmips.h>
#include <asm/brcm/bcmtime.h>

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock

extern int panic_timeout;
static int watchdog = 0;
static u8 *mcr = NULL;

void __init bcm947xx_time_init(void)
{
	unsigned int hz;
	extifregs_t *eir;

	/*
	 * Use deterministic values for initial counter interrupt
	 * so that calibrate delay avoids encountering a counter wrap.
	 */
	write_c0_count(0);
	write_c0_compare(0xffff);

	if (!(hz = sb_mips_clock(sbh)))
		hz = 100000000;

#if defined(CONFIG_BCM94702_CPCI) || defined(CONFIG_BCM94704_CPCI)
	/* Init RTC */

	rtc17xx_tod_init();
	rtc17xx_tod_print();
	/* Use RTC from local bus */
	rtc_get_time = rtc17xx_get_time;
	rtc_set_time = rtc17xx_set_time;
#endif
	printk("CPU: BCM%04x rev %d at %d MHz\n", sb_chip(sbh),
	       sb_chiprev(sbh), (hz + 500000) / 1000000);

	/* Set MIPS counter frequency for fixed_rate_gettimeoffset() */
	mips_hpt_frequency = hz / 2;

	/* Set watchdog interval in ms */
	watchdog = simple_strtoul(nvram_safe_get("watchdog"), NULL, 0);

	/* Set panic timeout in seconds */
	panic_timeout = watchdog / 1000;

	/* Setup blink */
	if ((eir = sb_setcore(sbh, SB_EXTIF, 0))) {
		sbconfig_t *sb =
		    (sbconfig_t *) ((unsigned int) eir + SBCONFIGOFF);
		unsigned long base =
		    EXTIF_CFGIF_BASE(sb_base
				     (readl((void *) (&sb->sbadmatch1))));
		mcr = (u8 *) ioremap_nocache(base + UART_MCR, 1);
	}
}

static irqreturn_t
bcm947xx_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{

	/* Generic MIPS timer code */
	timer_interrupt(irq, dev_id, regs);

	/* Set the watchdog timer to reset after the specified number of ms */
	if (watchdog > 0)
		sb_watchdog(sbh, WATCHDOG_CLOCK / 1000 * watchdog);

#ifdef	CONFIG_HWSIM
	(*((int *) 0xa0000f1c))++;
#else
	/* Blink one of the LEDs in the external UART */
	if (mcr && !(jiffies % (HZ / 2)))
		writeb(readb(mcr) ^ UART_MCR_OUT2, mcr);
#endif
	return IRQ_HANDLED;
}

static struct irqaction bcm947xx_timer_irqaction = {
	bcm947xx_timer_interrupt,
	SA_INTERRUPT | SA_NODELAY,	/* no timer interrupt threading */
	CPU_MASK_NONE,
	"timer",
	NULL,
	NULL
};

void __init bcm947xx_timer_setup(struct irqaction *irq)
{
	/* Enable the timer interrupt */
	setup_irq(7, &bcm947xx_timer_irqaction);
}
