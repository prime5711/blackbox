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

#include <asm/asm.h>
#include <asm/mipsregs.h>
#include <asm/regdef.h>
#include <asm/stackframe.h>

/*
 *	MIPS IRQ	Source
 *	--------	------
 *	       0	Software (ignored)
 *	       1	Software (ignored)
 *	       2	Combined hardware interrupt (hw0)
 *	       3	Hardware
 *	       4	Hardware
 *	       5	Hardware
 *	       6	Hardware
 *	       7	R4k timer
 */

	.text
	.set	noreorder
	.set	noat
	.align	5
	NESTED(brcmIRQ, PT_SIZE, sp)
	SAVE_ALL
	CLI
	.set	at
	.set	noreorder

	jal	brcm_irq_dispatch
	 move	a0, sp

	j	ret_from_irq
	 nop

	END(brcmIRQ)
