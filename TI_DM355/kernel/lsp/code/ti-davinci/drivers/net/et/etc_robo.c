/*
 * BCM53xx RoboSwitch utility functions
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
#include <asm/brcm/osl.h>
#include <asm/brcm/sbutils.h>

#include <asm/brcm/proto/ethernet.h>
#include <et_dbg.h>

/* Private state per RoboSwitch */
typedef struct {
	void *sbh;		/* SiliconBackplane handle */
	uint coreidx;		/* Current core index */
	uint32 ssl, clk, mosi, miso;	/* GPIO mapping */
	int cid, page;		/* Current chip ID and page */
} robo_info_t;


/* Forward declarations */
robo_info_t *robo_attach(void *sbh, uint32 ssl, uint32 clk, uint32 mosi,
			 uint32 miso);
void robo_detach(robo_info_t * robo);
static void robo_enable(robo_info_t * robo);
void
robo_wreg(robo_info_t * robo, uint8 cid, uint8 page,
	  uint8 addr, uint8 * buf, uint len);
void
robo_rreg(robo_info_t * robo, uint8 cid, uint8 page,
	  uint8 addr, uint8 * buf, uint len);
void robo_rvmii(robo_info_t * robo, uint8 cid);



/* Get access to the RoboSwitch */
robo_info_t *robo_attach(void *sbh, uint32 ssl, uint32 clk, uint32 mosi,
			 uint32 miso)
{
	robo_info_t *robo;

	/* Allocate private state */
	if (!(robo = MALLOC(sizeof(robo_info_t)))) {
		ET_ERROR(("robo_attach: out of memory"));
		return NULL;
	}
	bzero((char *) robo, sizeof(robo_info_t));
	robo->sbh = sbh;
	robo->ssl = ssl;
	robo->clk = clk;
	robo->mosi = mosi;
	robo->miso = miso;
	robo->cid = robo->page = -1;

	/* Initialize GPIO outputs */
	sb_gpioout(robo->sbh, robo->ssl | robo->clk | robo->mosi,
		   robo->ssl);
	sb_gpioouten(robo->sbh,
		     robo->ssl | robo->clk | robo->mosi | robo->miso,
		     robo->ssl | robo->clk | robo->mosi);

	return robo;
}

/* Release access to the RoboSwitch */
void robo_detach(robo_info_t * robo)
{
	/* Disable GPIO outputs */
	sb_gpioouten(robo->sbh, robo->ssl | robo->clk | robo->mosi, 0);

	/* Free private state */
	MFREE(robo, sizeof(robo_info_t));
}

/* Enable serial access to the chip */
static void robo_enable(robo_info_t * robo)
{
	void *regs;

	/* Save current core index */
	robo->coreidx = sb_coreidx(robo->sbh);

	/* Switch to GPIO core for faster access */
	regs = sb_gpiosetcore(robo->sbh);
	ASSERT(regs);
}

/* Disable serial access to the chip */
static void robo_disable(robo_info_t * robo)
{
	/* Switch back to original core */
	sb_setcoreidx(robo->sbh, robo->coreidx);
}

/* Write a byte stream to the chip */
static void robo_write(robo_info_t * robo, uint8 * buf, uint len)
{
	uint i;
	uint8 mask;

	for (i = 0; i < len; i++) {
		/* Bit bang from MSB to LSB */
		for (mask = 0x80; mask; mask >>= 1) {
			/* Clock low */
			sb_gpioout(robo->sbh, robo->clk, 0);
			OSL_DELAY(10);

			/* Output on rising edge */
			if (mask & buf[i])
				sb_gpioout(robo->sbh, robo->mosi,
					   robo->mosi);
			else
				sb_gpioout(robo->sbh, robo->mosi, 0);

			/* Clock high */
			sb_gpioout(robo->sbh, robo->clk, robo->clk);
			OSL_DELAY(10);
		}
	}
}

/* Handy macros for writing fixed length values */
#define robo_write8(robo, b) { uint8 val = (uint8) (b); robo_write((robo), &val, sizeof(val)); }
#define robo_write16(robo, w) { uint16 val = (uint16) (w); robo_write((robo), &val, sizeof(val)); }
#define robo_write32(robo, l) { uint32 val = (uint32) (l); robo_write((robo), &val, sizeof(val)); }

/* Read a byte stream from the chip */
static void robo_read(robo_info_t * robo, uint8 * buf, uint len)
{
	uint i, timeout;
	uint8 rack, mask, byte;

	/* Timeout after 100 tries without RACK */
	for (i = 0, rack = 0, timeout = 100; i < len && timeout;) {
		/* Bit bang from MSB to LSB */
		for (mask = 0x80, byte = 0; mask; mask >>= 1) {
			/* Clock low */
			sb_gpioout(robo->sbh, robo->clk, 0);
			OSL_DELAY(10);

			/* Sample on rising edge */
			if (sb_gpioin(robo->sbh) & robo->miso)
				byte |= mask;

			/* Clock high */
			sb_gpioout(robo->sbh, robo->clk, robo->clk);
			OSL_DELAY(10);
		}

		/* RACK when bit 0 is high */
		if (!rack) {
			rack = (byte & 1);
			timeout--;
		} else {
			buf[i] = byte;
			i++;
		}
	}

	if (timeout == 0) {
		ET_ERROR(("robo_read: timeout"));
	}
}

/* Handy macros for reading fixed length values */
#define robo_read8(robo) { uint8 val; robo_read((robo), &val, sizeof(val)); val; }
#define robo_read16(robo) { uint16 val; robo_read((robo), &val, sizeof(val)); val; }
#define robo_read32(robo) { uint32 val; robo_read((robo), &val, sizeof(val)); val; }

/* Select new chip and page */
static void robo_select(robo_info_t * robo, uint8 cid, uint8 page)
{
	/* Chip and page already selected */
	if (robo->cid == (int) cid && robo->page == (int) page)
		return;
	robo->cid = (int) cid;
	robo->page = (int) page;

	/* Enable CS */
	sb_gpioout(robo->sbh, robo->ssl, 0);
	OSL_DELAY(10);

	/* Select new chip */
	robo_write8(robo, 0x61 | ((cid & 0x7) << 1));

	/* Select new page */
	robo_write8(robo, 0xff);
	robo_write8(robo, page);

	/* Disable CS */
	sb_gpioout(robo->sbh, robo->ssl, robo->ssl);
	OSL_DELAY(10);
}

/* Write chip register */
void
robo_wreg(robo_info_t * robo, uint8 cid, uint8 page, uint8 addr,
	  uint8 * buf, uint len)
{
	robo_enable(robo);

	/* Select chip and page */
	robo_select(robo, cid, page);

	/* Enable CS */
	sb_gpioout(robo->sbh, robo->ssl, 0);
	OSL_DELAY(10);

	/* Write */
	robo_write8(robo, 0x61 | ((cid & 0x7) << 1));
	robo_write8(robo, addr);
	robo_write(robo, buf, len);

	/* Disable CS */
	sb_gpioout(robo->sbh, robo->ssl, robo->ssl);
	OSL_DELAY(10);

	robo_disable(robo);
}

/* Read chip register */
void
robo_rreg(robo_info_t * robo, uint8 cid, uint8 page, uint8 addr,
	  uint8 * buf, uint len)
{
	robo_enable(robo);

	/* Select chip and page */
	robo_select(robo, cid, page);

	/* Enable CS */
	sb_gpioout(robo->sbh, robo->ssl, 0);
	OSL_DELAY(10);

	/* Fast read */
	robo_write8(robo, 0x10 | ((cid & 0x7) << 1));
	robo_write8(robo, addr);
	robo_read(robo, buf, len);

	/* Disable CS */
	sb_gpioout(robo->sbh, robo->ssl, robo->ssl);
	OSL_DELAY(10);

	robo_disable(robo);
}

/* Enable reverse MII mode */
void robo_rvmii(robo_info_t * robo, uint8 cid)
{
	uint8 mii;

	/* MII port state override (page 0 register 14) */
	robo_rreg(robo, cid, 0, 14, &mii, sizeof(mii));

	/* Bit 4 enables reverse MII mode */
	if (mii & 0x10)
		return;
	mii |= 0x10;
	robo_wreg(robo, cid, 0, 14, &mii, sizeof(mii));

	/* Read back */
	robo_rreg(robo, cid, 0, 14, &mii, sizeof(mii));
	if (!(mii & 0x10)) {
		ET_ERROR(("robo_rvmii: error enabling mode"));
	}
}
