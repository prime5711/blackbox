/*
 * Hardware-specific MIB definition for
 * Broadcom Home Networking Division
 * BCM44XX and BCM47XX 10/100 Mbps Ethernet cores.
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

#ifndef _bcmenetmib_h_
#define _bcmenetmib_h_

/* cpp contortions to concatenate w/arg prescan */
#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif				/* PAD */

/*
 * EMAC MIB Registers
 */
typedef volatile struct {
	uint32 tx_good_octets;
	uint32 tx_good_pkts;
	uint32 tx_octets;
	uint32 tx_pkts;
	uint32 tx_broadcast_pkts;
	uint32 tx_multicast_pkts;
	uint32 tx_len_64;
	uint32 tx_len_65_to_127;
	uint32 tx_len_128_to_255;
	uint32 tx_len_256_to_511;
	uint32 tx_len_512_to_1023;
	uint32 tx_len_1024_to_max;
	uint32 tx_jabber_pkts;
	uint32 tx_oversize_pkts;
	uint32 tx_fragment_pkts;
	uint32 tx_underruns;
	uint32 tx_total_cols;
	uint32 tx_single_cols;
	uint32 tx_multiple_cols;
	uint32 tx_excessive_cols;
	uint32 tx_late_cols;
	uint32 tx_defered;
	uint32 tx_carrier_lost;
	uint32 tx_pause_pkts;
	uint32 PAD[8];

	uint32 rx_good_octets;
	uint32 rx_good_pkts;
	uint32 rx_octets;
	uint32 rx_pkts;
	uint32 rx_broadcast_pkts;
	uint32 rx_multicast_pkts;
	uint32 rx_len_64;
	uint32 rx_len_65_to_127;
	uint32 rx_len_128_to_255;
	uint32 rx_len_256_to_511;
	uint32 rx_len_512_to_1023;
	uint32 rx_len_1024_to_max;
	uint32 rx_jabber_pkts;
	uint32 rx_oversize_pkts;
	uint32 rx_fragment_pkts;
	uint32 rx_missed_pkts;
	uint32 rx_crc_align_errs;
	uint32 rx_undersize;
	uint32 rx_crc_errs;
	uint32 rx_align_errs;
	uint32 rx_symbol_errs;
	uint32 rx_pause_pkts;
	uint32 rx_nonpause_pkts;
} bcmenetmib_t;

#endif				/* _bcmenetmib_h_ */
