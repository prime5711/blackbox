/*
 * Early initialization code for BCM94710 boards
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. ( 2.6 kernel modifications )
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
#include <linux/types.h>
#include <asm/bootinfo.h>
#include <asm/io.h>
#include <linux/serial_reg.h>

void __init prom_init(void)
{
	unsigned long mem, before, offset;

	mips_machgroup = MACH_GROUP_BRCM;
	mips_machtype = MACH_BCM947XX;

	/* Figure out memory size by finding aliases.
	 *
	 * We assume that there will be no more than 128 MB of memory,
	 * and that the memory size will be a multiple of 1 MB.
	 *
	 * We set 'before' to be the amount of memory before this
	 * function, i.e. one MB less than the number  of MB of memory that we
	 * *know* we have.  And we set 'offset' to be the address of 'prominit'
	 * minus 'before', so that KSEG0 or KSEG1 base + offset < 1 MB.
	 * This prevents us from overrunning 128 MB and causing a bus error.
	 */
	before = ((unsigned long) &prom_init) & (127 << 20);
	offset = ((unsigned long) &prom_init) - before;
	for (mem = before + (1 << 20); mem < (128 << 20); mem += (1 << 20))
		if (*(unsigned long *) (offset + mem) ==
		    *(unsigned long *) (prom_init)) {
			/*
			 * We may already be well past the end of memory at
			 * this point, so we'll have to compensate for it.
			 */
			mem -= before;
			break;
		}
	add_memory_region(0, mem, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}

/* simple debug assistance*/
#define CCORE_BASE 0xb8000300
void prom_putchar(char c)
{
	while (!(readb((void *) (CCORE_BASE + UART_LSR)) & UART_LSR_THRE));
	writeb(c, (void *) (CCORE_BASE + UART_TX));
}

/*writes char on nonboard display*/
#define   LED_BASE 0xba00c000
void wr_LED(char c)
{
	volatile u_long *p = (volatile u_long *) LED_BASE;
	*p = (c << 24) | (' ' << 16) | (' ' << 8) | ' ';
}
