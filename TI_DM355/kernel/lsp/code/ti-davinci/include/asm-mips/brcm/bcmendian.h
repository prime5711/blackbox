/*
 * local version of endian.h - byte order defines
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

#ifndef _BCMENDIAN_H_
#define _BCMENDIAN_H_

#include <asm/brcm/typedefs.h>

#if defined(CONFIG_BCM947XX) && !defined(CONFIG_CPU_LITTLE_ENDIAN)
#define IL_BIGENDIAN
#endif

/* Byte swap a 16 bit value */
#define BCMSWAP16(val) \
	((uint16)( \
		(((uint16)(val) & (uint16)0x00ffU) << 8) | \
		(((uint16)(val) & (uint16)0xff00U) >> 8) ))

/* Byte swap a 32 bit value */
#define BCMSWAP32(val) \
	((uint32)( \
		(((uint32)(val) & (uint32)0x000000ffUL) << 24) | \
		(((uint32)(val) & (uint32)0x0000ff00UL) <<  8) | \
		(((uint32)(val) & (uint32)0x00ff0000UL) >>  8) | \
		(((uint32)(val) & (uint32)0xff000000UL) >> 24) ))

static INLINE uint16 bcmswap16(uint16 val)
{
	return BCMSWAP16(val);
}

static INLINE uint32 bcmswap32(uint32 val)
{
	return BCMSWAP32(val);
}

/* buf	- start of buffer of shorts to swap */
/* len  - byte length of buffer */
static INLINE void bcmswap16_buf(uint16 * buf, uint len)
{
	len = len / 2;

	while (len--) {
		*buf = bcmswap16(*buf);
		buf++;
	}
}

#ifndef hton16
#ifndef IL_BIGENDIAN
#define HTON16(i) BCMSWAP16(i)
#define	hton16(i) bcmswap16(i)
#define	hton32(i) bcmswap32(i)
#define	ntoh16(i) bcmswap16(i)
#define	ntoh32(i) bcmswap32(i)
#define ltoh16(i) (i)
#define ltoh32(i) (i)
#define htol16(i) (i)
#define htol32(i) (i)
#else
#define HTON16(i) (i)
#define	hton16(i) (i)
#define	hton32(i) (i)
#define	ntoh16(i) (i)
#define	ntoh32(i) (i)
#define	ltoh16(i) bcmswap16(i)
#define	ltoh32(i) bcmswap32(i)
#define htol16(i) bcmswap16(i)
#define htol32(i) bcmswap32(i)
#endif
#endif

#ifndef IL_BIGENDIAN
#define ltoh16_buf(buf, i)
#define htol16_buf(buf, i)
#else
#define ltoh16_buf(buf, i) bcmswap16_buf((uint16*)buf, i)
#define htol16_buf(buf, i) bcmswap16_buf((uint16*)buf, i)
#endif

/*
 * load 16-bit value from unaligned little endian byte array.
 */
static INLINE uint16 ltoh16_ua(uint8 * bytes)
{
	return (bytes[1] << 8) + bytes[0];
}

/*
 * load 32-bit value from unaligned little endian byte array.
 */
static INLINE uint32 ltoh32_ua(uint8 * bytes)
{
	return (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) +
	    bytes[0];
}

/*
 * load 16-bit value from unaligned big (network) endian byte array.
 */
static INLINE uint16 ntoh16_ua(uint8 * bytes)
{
	return (bytes[0] << 8) + bytes[1];
}

/*
 * load 32-bit value from unaligned big (network) endian byte array.
 */
static INLINE uint32 ntoh32_ua(uint8 * bytes)
{
	return (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) +
	    bytes[3];
}

#endif				/* _BCMENDIAN_H_ */
