/*
 * Driver-specific socket ioctls
 * used by BSD, Linux, and PSOS
 * Broadcom BCM44XX 10/100Mbps Ethernet Device Driver
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

#ifndef _etsockio_h_
#define _etsockio_h_

/* THESE MUST BE CONTIGUOUS AND CONSISTENT WITH VALUES IN ETC.H */


#if defined(linux)
#define SIOCSETCUP		(SIOCDEVPRIVATE + 0)
#define SIOCSETCDOWN		(SIOCDEVPRIVATE + 1)
#define SIOCSETCLOOP		(SIOCDEVPRIVATE + 2)
#define SIOCGETCDUMP		(SIOCDEVPRIVATE + 3)
#define SIOCSETCSETMSGLEVEL	(SIOCDEVPRIVATE + 4)
#define SIOCSETCPROMISC		(SIOCDEVPRIVATE + 5)
#define SIOCSETCTXDOWN		(SIOCDEVPRIVATE + 6)	/* obsolete */
#define SIOCSETCSPEED		(SIOCDEVPRIVATE + 7)
#define SIOCTXGEN		(SIOCDEVPRIVATE + 8)
#define SIOCGETCPHYRD		(SIOCDEVPRIVATE + 9)
#define SIOCSETCPHYWR		(SIOCDEVPRIVATE + 10)

#else				/* !linux */

#define SIOCSETCUP		_IOWR('e', 130 + 0, struct ifreq)
#define SIOCSETCDOWN		_IOWR('e', 130 + 1, struct ifreq)
#define SIOCSETCLOOP		_IOWR('e', 130 + 2, struct ifreq)
#define SIOCGETCDUMP		_IOWR('e', 130 + 3, struct ifreq)
#define SIOCSETCSETMSGLEVEL	_IOWR('e', 130 + 4, struct ifreq)
#define SIOCSETCPROMISC		_IOWR('e', 130 + 5, struct ifreq)
#define SIOCSETCTXDOWN		_IOWR('e', 130 + 6, struct ifreq)	/* obsolete */
#define SIOCSETCSPEED		_IOWR('e', 130 + 7, struct ifreq)
#define SIOCTXGEN		_IOWR('e', 130 + 8, struct ifreq)

#endif

/* arg to SIOCTXGEN */
struct txg {
	uint32 num;		/* number of frames to send */
	uint32 delay;		/* delay in microseconds between sending each */
	uint32 size;		/* size of ether frame to send */
	uchar buf[1514];	/* starting ether frame data */
};

#endif
