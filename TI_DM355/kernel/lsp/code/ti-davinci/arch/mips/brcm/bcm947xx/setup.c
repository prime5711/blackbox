/*
 * Generic setup routines for Broadcom MIPS boards
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. ( 2.6 kernel modification )
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

#include <linux/module.h>
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/reboot.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmdevs.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/bcmnvram.h>
#include <asm/brcm/sbmips.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/bcmtime.h>

/* Global SB handle */
void *bcm947xx_sbh = NULL;
DEFINE_SPINLOCK(bcm947xx_sbh_lock);
EXPORT_SYMBOL(bcm947xx_sbh);
EXPORT_SYMBOL(bcm947xx_sbh_lock);

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock

void bcm947xx_machine_restart(char *command)
{
	printk("Please stand by while rebooting the system...\n");

	/* Set the watchdog timer to reset immediately */
	cli();
	sb_watchdog(sbh, 1);
	while (1);
}

void bcm947xx_machine_halt(void)
{
	printk("System halted\n");

	/* Disable interrupts and watchdog and spin forever */
	cli();
	sb_watchdog(sbh, 0);
	while (1);
}

#ifdef CONFIG_SERIAL_8250
static void __init serial_setup(void *sbh)
{
	sb_serial_init(sbh, NULL /*serial_add */ );

}
#endif				/* CONFIG_SERIAL_8250 */

static int __init brcm_setup(void)
{
	char *value;

	/* Get global SB handle */
	sbh = sb_kattach();

	/* Initialize clocks and interrupts */
	sb_mips_init(sbh);

#ifdef CONFIG_SERIAL_8250
	/* Initialize UARTs */
	serial_setup(sbh);
#endif

	/* Override default command line arguments */
	value = nvram_get("kernel_args");
	if (value && strlen(value) && strncmp(value, "empty", 5))
		strncpy(arcs_cmdline, value, sizeof(arcs_cmdline));

	/* Generic setup */
	_machine_restart = bcm947xx_machine_restart;
	_machine_halt = bcm947xx_machine_halt;
	_machine_power_off = bcm947xx_machine_halt;

	board_time_init = bcm947xx_time_init;
	board_timer_setup = bcm947xx_timer_setup;
	return 0;
}

early_initcall(brcm_setup);

const char *get_system_type(void)
{
	switch (sb_chip(sbh)) {
	case BCM5365_DEVICE_ID:
		return "Broadcom BCM95365";
	default:
		return "Broadcom BCM947XX";
	}
}

void __init bus_error_init(void)
{
}
