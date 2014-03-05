/*
 * Linux OS Independent Layer
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
 */

#define LINUX_OSL

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmendian.h>
#include <asm/brcm/linuxver.h>
#include <asm/brcm/linux_osl.h>
#include <asm/brcm/bcmutils.h>
#include <linux/delay.h>
#ifdef mips
#include <asm/paccess.h>
#endif
#include <asm/brcm/pcicfg.h>
#include <asm/brcm/sbpci.h>

#define PCI_CFG_RETRY 10

void *osl_pktget(void *drv, uint len, bool send)
{
	struct sk_buff *skb;

	if ((skb = dev_alloc_skb(len)) == NULL)
		return (NULL);

	skb_put(skb, len);

	/* ensure the cookie field is cleared */
	PKTSETCOOKIE(skb, NULL);

	return ((void *) skb);
}

void osl_pktfree(void *p)
{
	struct sk_buff *skb, *nskb;

	skb = (struct sk_buff *) p;

	/* perversion: we use skb->next to chain multi-skb packets */
	while (skb) {
		nskb = skb->next;
		skb->next = NULL;
		if (skb->destructor) {
			/* cannot kfree_skb() on hard IRQ (net/core/skbuff.c) if destructor exists */
			dev_kfree_skb_any(skb);
		} else {
			/* can free immediately (even in_irq()) if destructor does not exist */
			dev_kfree_skb(skb);
		}
		skb = nskb;
	}
}

uint32 osl_pci_read_config(void *loc, uint offset, uint size)
{
	struct pci_dev *pdev;
	uint val;
	uint retry = PCI_CFG_RETRY;

	/* only 4byte access supported */
	ASSERT(size == 4);

	pdev = (struct pci_dev *) loc;
	do {
		sbpci_read_config_dword(pdev, offset, &val);
		if (val != 0xffffffff)
			break;
	} while (retry--);

	return (val);
}

void osl_pci_write_config(void *loc, uint offset, uint size, uint val)
{
	struct pci_dev *pdev;
	uint retry = PCI_CFG_RETRY;

	/* only 4byte access supported */
	ASSERT(size == 4);

	pdev = (struct pci_dev *) loc;

	do {
		sbpci_write_config_dword(pdev, offset, val);
		if (offset != PCI_BAR0_WIN)
			break;
		if (osl_pci_read_config(loc, offset, size) == val)
			break;
	} while (retry--);

}

void osl_pcmcia_read_attr(void *osh, uint offset, void *buf, int size)
{
	ASSERT(0);
}

void osl_pcmcia_write_attr(void *osh, uint offset, void *buf, int size)
{
	ASSERT(0);
}

void osl_assert(char *exp, char *file, int line)
{
	char tempbuf[255];

	sprintf(tempbuf, "assertion \"%s\" failed: file \"%s\", line %d\n",
		exp, file, line);
	panic(tempbuf);
}

/*
 * BINOSL selects the slightly slower function-call-based binary compatible osl.
 */
#ifdef BINOSL

int osl_printf(const char *format, ...)
{
	va_list args;
	char buf[1024];
	int len;

	/* sprintf into a local buffer because there *is* no "vprintk()".. */
	va_start(args, format);
	len = vsprintf(buf, format, args);
	va_end(args);

	if (len > sizeof(buf)) {
		printk("osl_printf: buffer overrun\n");
		return (0);
	}

	return (printk(buf));
}

int osl_sprintf(char *buf, const char *format, ...)
{
	va_list args;
	int rc;

	va_start(args, format);
	rc = vsprintf(buf, format, args);
	va_end(args);
	return (rc);
}

int osl_strcmp(const char *s1, const char *s2)
{
	return (strcmp(s1, s2));
}

int osl_strncmp(const char *s1, const char *s2, uint n)
{
	return (strncmp(s1, s2, n));
}

int osl_strlen(char *s)
{
	return (strlen(s));
}

char *osl_strcpy(char *d, const char *s)
{
	return (strcpy(d, s));
}

char *osl_strncpy(char *d, const char *s, uint n)
{
	return (strncpy(d, s, n));
}

void bcopy(const void *src, void *dst, int len)
{
	memcpy(dst, src, len);
}

int bcmp(const void *b1, const void *b2, int len)
{
	return (memcmp(b1, b2, len));
}

void bzero(void *b, int len)
{
	memset(b, '\0', len);
}

void *osl_malloc(uint size)
{
	return (kmalloc(size, GFP_ATOMIC));
}

void osl_mfree(void *addr, uint size)
{
	kfree(addr);
}

uint32 osl_readl(volatile uint32 * r)
{
	return (readl(r));
}

uint16 osl_readw(volatile uint16 * r)
{
	return (readw(r));
}

uint8 osl_readb(volatile uint8 * r)
{
	return (readb(r));
}

void osl_writel(uint32 v, volatile uint32 * r)
{
	writel(v, r);
}

void osl_writew(uint16 v, volatile uint16 * r)
{
	writew(v, r);
}

void osl_writeb(uint8 v, volatile uint8 * r)
{
	writeb(v, r);
}

void *osl_uncached(void *va)
{
#ifdef mips
	return ((void *) KSEG1ADDR(va));
#else
	return ((void *) va);
#endif
}

uint osl_getcycles(void)
{
	uint cycles;

#if defined(mips)
	cycles = read_c0_count() * 2;
#elif defined(__i386__)
	rdtscl(cycles);
#else
	cycles = 0;
#endif
	return cycles;
}

void *osl_reg_map(uint32 pa, uint size)
{
	return (ioremap_nocache((unsigned long) pa, (unsigned long) size));
}

void osl_reg_unmap(void *va)
{
	iounmap(va);
}

int osl_busprobe(uint32 * val, uint32 addr)
{
#ifdef mips
	return get_dbe(*val, (uint32 *) addr);
#else
	*val = readl(addr);
	return 0;
#endif
}

void *osl_dma_alloc_consistent(void *dev, uint size, ulong * pap)
{
	return (pci_alloc_consistent
		((struct pci_dev *) dev, size, (dma_addr_t *) pap));
}

void osl_dma_free_consistent(void *dev, void *va, uint size, ulong pa)
{
	pci_free_consistent((struct pci_dev *) dev, size, va,
			    (dma_addr_t) pa);
}

uint osl_dma_map(void *dev, void *va, uint size, int direction)
{
	int dir;

	dir =
	    (direction == DMA_TX) ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE;
	return (pci_map_single(dev, va, size, dir));
}

void osl_dma_unmap(void *dev, uint pa, uint size, int direction)
{
	int dir;

	dir =
	    (direction == DMA_TX) ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE;
	pci_unmap_single(dev, (uint32) pa, size, dir);
}

void osl_delay(uint usec)
{
	udelay(usec);
}

uchar *osl_pktdata(void *drv, void *skb)
{
	return (((struct sk_buff *) skb)->data);
}

uint osl_pktlen(void *drv, void *skb)
{
	return (((struct sk_buff *) skb)->len);
}

void *osl_pktnext(void *drv, void *skb)
{
	return (((struct sk_buff *) skb)->next);
}

void osl_pktsetnext(void *skb, void *x)
{
	((struct sk_buff *) skb)->next = (struct sk_buff *) x;
}

void osl_pktsetlen(void *drv, void *skb, uint len)
{
	__skb_trim((struct sk_buff *) skb, len);
}

uchar *osl_pktpush(void *drv, void *skb, int bytes)
{
	return (skb_push((struct sk_buff *) skb, bytes));
}

uchar *osl_pktpull(void *drv, void *skb, int bytes)
{
	return (skb_pull((struct sk_buff *) skb, bytes));
}

void *osl_pktdup(void *drv, void *skb)
{
	return (skb_clone((struct sk_buff *) skb, GFP_ATOMIC));
}

void *osl_pktcookie(void *skb)
{
	return ((void *) ((struct sk_buff *) skb)->csum);
}

void osl_pktsetcookie(void *skb, void *x)
{
	((struct sk_buff *) skb)->csum = (uint) x;
}

void *osl_pktlink(void *skb)
{
	return (((struct sk_buff *) skb)->prev);
}

void osl_pktsetlink(void *skb, void *x)
{
	((struct sk_buff *) skb)->prev = (struct sk_buff *) x;
}

#endif
