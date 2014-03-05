/*
 * Linux device driver private data structure
 * Broadcom BCM53XX, BCM44XX and BCM47XX 10/100Mbps Ethernet Device Driver
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

#ifndef _et_linux_priv_h_
#define _et_linux_priv_h_
#include <etc.h>

typedef struct et_info {
	etc_info_t *etc;	/* pointer to common os-independent data */
	struct net_device *dev;	/* backpoint to device */
	struct pci_dev *pdev;	/* backpoint to pci_dev */
	spinlock_t lock;	/* per-device perimeter lock */
	struct sk_buff_head txq;	/* send queue */
	void *regsva;		/* opaque chip registers virtual address */
	struct timer_list timer;	/* one second watchdog timer */
	struct net_device_stats stats;	/* stat counter reporting structure */
	int events;		/* bit channel between isr and dpc */
	struct tasklet_struct tasklet;	/* dpc tasklet */
	ulong flags;		/* current irq flags */
	struct et_info *next;	/* pointer to next et_info_t
				   in chain */
} et_info_t;

#endif				/* _et_linux_priv_h_ */
