/*
 * Broadcom SiliconBackplane chipcommon serial flash interface
 *
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

#ifndef _sflash_h_
#define _sflash_h_

#include <asm/brcm/typedefs.h>
#include <asm/brcm/sbchipc.h>

/* GPIO based bank selection (1 GPIO bit) */
#define SFLASH_MAX_BANKS	1
#define SFLASH_GPIO_SHIFT	2
#define SFLASH_GPIO_MASK	((SFLASH_MAX_BANKS - 1) << SFLASH_GPIO_SHIFT)

struct sflash_bank {
	uint offset;					/* Byte offset */
	uint erasesize;					/* Block size */
	uint numblocks;					/* Number of blocks */
	uint size;					/* Total bank size in bytes */
};

struct sflash {
	struct sflash_bank banks[SFLASH_MAX_BANKS];	/* GPIO selectable banks */
	uint32 type;					/* Type */
	uint size;					/* Total array size in bytes */
};

/* Utility functions */
extern int sflash_poll(chipcregs_t *cc, uint offset);
extern int sflash_read(chipcregs_t *cc, uint offset, uint len, uchar *buf);
extern int sflash_write(chipcregs_t *cc, uint offset, uint len, const uchar *buf);
extern int sflash_erase(chipcregs_t *cc, uint offset);
extern struct sflash * sflash_init(chipcregs_t *cc);

#endif /* _sflash_h_ */
