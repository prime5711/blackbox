/*
 * include/asm-mips/tx4939/ide.h
 *
 * ide supplement routines
 *
 * (C) Copyright TOSHIBA CORPORATION SEMICONDUCTOR COMPANY 2005
 *
 * Author: source@mvista.com,
 *               Hiroshi DOYU <Hiroshi_DOYU@montavista.co.jp>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */

#ifndef __ASM_MACH_TX4939_IDE_H
#define __ASM_MACH_TX4939_IDE_H

#ifdef __KERNEL__

#ifndef MAX_HWIFS
# ifdef CONFIG_BLK_DEV_IDEPCI
#define MAX_HWIFS	10
# else
#define MAX_HWIFS	6
# endif
#endif

/*
 * Our Physical Region Descriptor (PRD) table should be large enough
 * to handle the biggest I/O request we are likely to see.  Since requests
 * can have no more than 256 sectors, and since the typical blocksize is
 * two or more sectors, we could get by with a limit of 128 entries here for
 * the usual worst case.  Most requests seem to include some contiguous blocks,
 * further reducing the number of table entries required.
 *
 * The driver reverts to PIO mode for individual requests that exceed
 * this limit (possible with 512 byte blocksizes, eg. MSDOS f/s), so handling
 * 100% of all crazy scenarios here is not necessary.
 *
 * As it turns out though, we must allocate a full 4KB page for this,
 * so the two PRD tables (ide0 & ide1) will each get half of that,
 * allowing each to have about 256 entries (8 bytes each) from this.
 */

#ifdef PRD_ENTRIES
#undef PRD_ENTRIES
#endif

#define PRD_BYTES       8
#define PRD_ENTRIES     (PAGE_SIZE / (2 * PRD_BYTES))


#define ide_ack_intr(hwif) (hwif->hw.ack_intr ? hwif->hw.ack_intr(hwif) : 1)
#define IDE_ARCH_ACK_INTR

#define __ide_mm_insw	readsw
#define __ide_mm_insl	readsl
#define __ide_mm_outsw	writesw
#define __ide_mm_outsl	writesl

#ifndef CONFIG_CPU_LITTLE_ENDIAN
/*
 * Only for the Big Endian systems, do not do the swapping.
 * We cannot turn off the CONFIG_SWAP_IO_SPACE since the
 * other subsystems need it. Hence we need this approach for
 * IDE only.
 * Furthermore, since the big endian mode of TX4939 is more specific,
 * so add more the following to include/asm-mips/ide.h.
 */
#ifdef inw
#undef inw
#endif
#ifdef outw
#undef outw
#endif
#ifdef inl
#undef inl
#endif
#ifdef outl
#undef outl
#endif

#define inw(port)		__raw_inw(port & ~0x1)
#define inl(port)		__raw_inl(port)
#define outw(val, port)		__raw_outw(val, port & ~0x01)
#define outl(val, port)		__raw_outl(val, port)

#endif /* CONFIG_LITTLE_ENDIAN */
#endif /* __KERNEL__ */

#define IS_IDE_TX4939 (HWIF(drive)->chipset == ide_tx4939)

#endif /* __ASM_MACH_TX4939_IDE_H */

