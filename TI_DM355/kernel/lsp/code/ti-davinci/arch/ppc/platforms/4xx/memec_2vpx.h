/*
 * memec_2vpx.h
 *
 * Include file that defines the Memec Virtex-II Pro 2VP7/2VP4 development board
 *
 * Author: MontaVista Software, Inc.
 *         source@mvista.com
 *
 * 2002-2004 (c) MontaVista, Software, Inc.  This file is licensed under the
 * terms of the GNU General Public License version 2.  This program is licensed
 * "as is" without any warranty of any kind, whether express or implied.
 */

#ifdef __KERNEL__
#ifndef __MEMEC_2VPX_H__
#define __MEMEC_2VPX_H__

/* 2VP7/2VP4 have a Xilinx Virtex-II Pro processor */
#include <platforms/4xx/virtex.h>

#ifndef __ASSEMBLY__

#include <linux/types.h>

typedef struct board_info {
	unsigned int	 bi_memsize;		/* DRAM installed, in bytes */
	unsigned char	 bi_enetaddr[6];	/* Local Ethernet MAC address */
	unsigned int	 bi_intfreq;		/* Processor speed, in Hz */
	unsigned int	 bi_busfreq;		/* PLB Bus speed, in Hz */
	unsigned int	 bi_pci_busfreq;	/* PCI Bus speed, in Hz */
} bd_t;

/* Some 4xx parts use a different timebase frequency from the internal clock.
*/
#define bi_tbfreq bi_intfreq

#endif /* !__ASSEMBLY__ */

/* We don't need anything mapped.  Size of zero will accomplish that. */
#define PPC4xx_ONB_IO_PADDR	0u
#define PPC4xx_ONB_IO_VADDR	0u
#define PPC4xx_ONB_IO_SIZE	0u

#define PPC4xx_MACHINE_NAME "Memec 2VP4/P7 FG456/FF672"
#define XILINX_SYS_ID_STR "Memec Virtex-II Pro 2VP7/2VP4 development board\n"

#endif				/* __MEMEC_2VPX_H__ */
#endif				/* __KERNEL__ */
