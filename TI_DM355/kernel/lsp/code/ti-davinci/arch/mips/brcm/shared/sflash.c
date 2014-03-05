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

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/osl.h>
#include <asm/brcm/sbchipc.h>
#include <asm/brcm/sflash.h>

/* Private global state */
static struct sflash sflash;

/* Select the appropriate bank */
static void sflash_select(chipcregs_t * cc, uint bank)
{
	uint32 val;

	ASSERT(bank < SFLASH_MAX_BANKS);

	/* Enable output */
	val = R_REG(&cc->gpioouten);
	if ((val & SFLASH_GPIO_MASK) != SFLASH_GPIO_MASK) {
		val |= SFLASH_GPIO_MASK;
		W_REG(&cc->gpioouten, val);
	}

	/* Select bank */
	val = R_REG(&cc->gpioout);
	if ((val & SFLASH_GPIO_MASK) != (bank << SFLASH_GPIO_SHIFT)) {
		val &= ~SFLASH_GPIO_MASK;
		val |= (bank << SFLASH_GPIO_SHIFT);
		W_REG(&cc->gpioout, val);
	}
}

/* Select a serial flash bank */
static struct sflash_bank *sflash_bank(chipcregs_t * cc, uint offset)
{
	uint bank;

	ASSERT(offset < sflash.size);

	/* Find bank */
	for (bank = 0; bank < SFLASH_MAX_BANKS; bank++) {
		if (offset >= sflash.banks[bank].offset &&
		    offset < (sflash.banks[bank].offset +
			      sflash.banks[bank].size))
			break;
	}

	/* Select bank */
	sflash_select(cc, bank);

	return &sflash.banks[bank];
}

/* Issue a serial flash command */
static INLINE void sflash_cmd(chipcregs_t * cc, uint opcode)
{
	W_REG(&cc->flashcontrol, SFLASH_START | opcode);
	while (R_REG(&cc->flashcontrol) & SFLASH_BUSY);
}

/* Initialize serial flash access */
struct sflash *sflash_init(chipcregs_t * cc)
{
	uint bank;
	uint32 id, id2;

	bzero(&sflash, sizeof(sflash));

	sflash.type = R_REG(&cc->capabilities) & CAP_FLASH_MASK;

	for (bank = 0; bank < SFLASH_MAX_BANKS; bank++) {
		/* Select bank */
		sflash_select(cc, bank);

		switch (sflash.type) {
		case SFLASH_ST:
			/* Probe for ST in this bank */
			sflash_cmd(cc, SFLASH_ST_DP);
			sflash_cmd(cc, SFLASH_ST_RES);
			id = R_REG(&cc->flashdata);
			switch (id) {
			case 0x11:
				/* ST M25P20 2 Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 64 * 1024;
				sflash.banks[bank].numblocks = 4;
				break;
			case 0x12:
				/* ST M25P40 4 Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 64 * 1024;
				sflash.banks[bank].numblocks = 8;
				break;
			case 0x13:
				/* ST M25P80 8 Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 64 * 1024;
				sflash.banks[bank].numblocks = 16;
				break;
			case 0xbf:
				W_REG(&cc->flashaddress, 1);
				sflash_cmd(cc, SFLASH_ST_RES);
				id2 = R_REG(&cc->flashdata);
				if (id2 == 0x44) {
					/* SST M25VF80 4 Mbit Serial Flash */
					sflash.banks[bank].offset =
					    sflash.size;
					sflash.banks[bank].erasesize =
					    64 * 1024;
					sflash.banks[bank].numblocks = 8;
				}
				break;
			}
			break;

		case SFLASH_AT:
			/* Probe for Atmel in this bank */
			sflash_cmd(cc, SFLASH_AT_STATUS);
			id = R_REG(&cc->flashdata) & 0x3c;
			switch (id) {
			case 0x2c:
				/* Atmel AT45DB161 16Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 512;
				sflash.banks[bank].numblocks = 4096;
				break;
			case 0x34:
				/* Atmel AT45DB321 32Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 512;
				sflash.banks[bank].numblocks = 8192;
				break;
			case 0x3c:
				/* Atmel AT45DB642 64Mbit Serial Flash */
				sflash.banks[bank].offset = sflash.size;
				sflash.banks[bank].erasesize = 1024;
				sflash.banks[bank].numblocks = 8192;
				break;
			}
			break;
		}

		sflash.banks[bank].size +=
		    sflash.banks[bank].erasesize *
		    sflash.banks[bank].numblocks;
		sflash.size += sflash.banks[bank].size;
	}

	return sflash.size ? &sflash : NULL;
}

/* Read len bytes starting at offset into buf. Returns number of bytes read. */
int sflash_read(chipcregs_t * cc, uint offset, uint len, uchar * buf)
{
	struct sflash_bank *bank;
	int ret = 0;
	uint32 page, byte, mask;

	if (!len)
		return 0;

	if ((offset + len) > sflash.size)
		return -22;

	bank = sflash_bank(cc, offset);
	switch (sflash.type) {
	case SFLASH_ST:
		ret = 1;
		W_REG(&cc->flashaddress, offset - bank->offset);
		sflash_cmd(cc, SFLASH_ST_READ);
		*buf = R_REG(&cc->flashdata);
		break;
	case SFLASH_AT:
		mask = bank->erasesize - 1;
		page = ((offset - bank->offset) & ~mask) << 1;
		byte = (offset - bank->offset) & mask;
		ret = 1;
		W_REG(&cc->flashaddress, page | byte);
		sflash_cmd(cc, SFLASH_AT_READ);
		*buf = R_REG(&cc->flashdata);
		break;
	}

	return ret;
}

/* Poll for command completion. Returns zero when complete. */
int sflash_poll(chipcregs_t * cc, uint offset)
{
	struct sflash_bank *bank;

	if (offset >= sflash.size)
		return -22;

	bank = sflash_bank(cc, offset);
	switch (sflash.type) {
	case SFLASH_ST:
		/* Check for ST Write In Progress bit */
		sflash_cmd(cc, SFLASH_ST_RDSR);
		return R_REG(&cc->flashdata) & SFLASH_ST_WIP;
	case SFLASH_AT:
		/* Check for Atmel Ready bit */
		sflash_cmd(cc, SFLASH_AT_STATUS);
		return !(R_REG(&cc->flashdata) & SFLASH_AT_READY);
	}

	return 0;
}

/* Write len bytes starting at offset into buf. Returns number of bytes
 * written. Caller should poll for completion.
 */
int sflash_write(chipcregs_t * cc, uint offset, uint len,
		 const uchar * buf)
{
	struct sflash_bank *bank;
	int ret = 0;
	uint32 page, byte, mask;

	if (!len)
		return 0;
	if ((offset + len) > sflash.size)
		return -22;

	bank = sflash_bank(cc, offset);
	switch (sflash.type) {
	case SFLASH_ST:
		ret = 1;
		/* Enable writes */
		sflash_cmd(cc, SFLASH_ST_WREN);
		W_REG(&cc->flashaddress, offset);
		W_REG(&cc->flashdata, *buf);
		/* Page program */
		sflash_cmd(cc, SFLASH_ST_PP);
		break;
	case SFLASH_AT:
		mask = bank->erasesize - 1;
		page = ((offset - bank->offset) & ~mask) << 1;
		byte = (offset - bank->offset) & mask;
		/* Read main memory page into buffer 1 */
		if (byte || len < bank->erasesize) {
			W_REG(&cc->flashaddress, page);
			sflash_cmd(cc, SFLASH_AT_BUF1_LOAD);
			/* 250 us for AT45DB321B */
			SPINWAIT(sflash_poll(cc, offset), 1000);
			ASSERT(!sflash_poll(cc, offset));
		}
		/* Write into buffer 1 */
		for (ret = 0; ret < len && byte < bank->erasesize; ret++) {
			W_REG(&cc->flashaddress, byte++);
			W_REG(&cc->flashdata, *buf++);
			sflash_cmd(cc, SFLASH_AT_BUF1_WRITE);
		}
		/* Write buffer 1 into main memory page */
		W_REG(&cc->flashaddress, page);
		sflash_cmd(cc, SFLASH_AT_BUF1_PROGRAM);
		break;
	}

	return ret;
}

/* Erase a region. Returns number of bytes scheduled for erasure.
 * Caller should poll for completion.
 */
int sflash_erase(chipcregs_t * cc, uint offset)
{
	struct sflash_bank *bank;

	if (offset >= sflash.size)
		return -22;

	bank = sflash_bank(cc, offset);
	switch (sflash.type) {
	case SFLASH_ST:
		sflash_cmd(cc, SFLASH_ST_WREN);
		W_REG(&cc->flashaddress, offset);
		sflash_cmd(cc, SFLASH_ST_SE);
		return bank->erasesize;
	case SFLASH_AT:
		W_REG(&cc->flashaddress, offset << 1);
		sflash_cmd(cc, SFLASH_AT_PAGE_ERASE);
		return bank->erasesize;
	}

	return 0;
}
