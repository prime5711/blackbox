/*
 * Low-Level PCI and SB support for BCM47xx (Linux support code)
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. ( 2.6 kernel modification )
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
#include <linux/pci.h>
#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/sbconfig.h>
#include <asm/brcm/sbpci.h>
#include <asm/brcm/pcicfg.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/bcmdevs.h>
#include <asm/brcm/bcmnvram.h>

/* Global SB handle */
extern void *bcm947xx_sbh;
extern spinlock_t bcm947xx_sbh_lock;

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock

static u32 pci_iobase = 0x100;
static u32 pci_membase = SB_PCI_DMA;

static void bcm947xx_fixup(struct pci_dev *d)
{
	struct pci_bus *b;
	struct list_head *ln;
	struct resource *res;
	int pos, size;
	u32 *base;

	b = d->bus;

	/* Fix up external PCI */
	if (b->number != 0) {
		for (ln = b->devices.next; ln != &b->devices;
		     ln = ln->next) {
			d = pci_dev_b(ln);
			/* Fix up resource bases */
			for (pos = 0; pos < 4; pos++) {
				res = &d->resource[pos];
				base =
				    (res->
				     flags & IORESOURCE_IO) ? &pci_iobase :
				    &pci_membase;
				if (res->end) {
					size = res->end - res->start + 1;
					if (*base & (size - 1))
						*base =
						    (*base +
						     size) & ~(size - 1);
					res->start = *base;
					res->end = res->start + size - 1;
					*base += size;
					pci_write_config_dword(d,
							       PCI_BASE_ADDRESS_0
							       +
							       (pos << 2),
							       res->start);
				}
				/* Fix up PCI bridge BAR0 only */
				if (b->number == 1
				    && PCI_SLOT(d->devfn) == 0)
					break;
			}
			/* Fix up interrupt lines */
			if (pci_find_device(VENDOR_BROADCOM, SB_PCI, NULL))
				d->irq =
				    (pci_find_device
				     (VENDOR_BROADCOM, SB_PCI, NULL))->irq;
			pci_write_config_byte(d, PCI_INTERRUPT_LINE,
					      d->irq);
		}
	}
}

DECLARE_PCI_FIXUP_FINAL(PCI_ANY_ID, PCI_ANY_ID, bcm947xx_fixup);

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

int __init pcibios_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	u8 irq;
	pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &irq);
	dev->irq = irq + 4;
	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);
	return dev->irq;
}

static void __init pcibios_fixup_resources(struct pci_dev *dev)
{
	ulong flags;
	uint coreidx;

	if (dev->bus->number == 0) {

		/*
		 * Chipcommon, RAM controller and PCI bridge must not be reset!
		 */
		if (dev->device == SB_MIPS ||
		    dev->device == SB_MIPS33 ||
		    dev->device == SB_EXTIF ||
		    dev->device == SB_MEMC ||
		    dev->device == SB_PCI || dev->device == SB_CC)
			return;

		spin_lock_irqsave(&sbh_lock, flags);
		coreidx = sb_coreidx(sbh);
		if (!sb_setcoreidx(sbh, PCI_SLOT(dev->devfn)))
			return;

		/*
		 * The USB core requires a special bit to be set during core
		 * reset to enable host (OHCI) mode. Resetting the SB core here
		 * is a hack for compatibility with vanilla usb-ohci so that it
		 * does not have to know about SB.  A driver that wants to  use
		 * the  USB core in device mode should know about SB and should
		 * reset the bit back to 0.
		 */
		if (sb_coreid(sbh) == SB_USB) {
			sb_core_disable(sbh, sb_coreflags(sbh, 0, 0));
			sb_core_reset(sbh, 1 << 29);
		} else
			sb_core_reset(sbh, 0);

		sb_setcoreidx(sbh, coreidx);
		spin_unlock_irqrestore(&sbh_lock, flags);

		return;
	}

	if (dev->bus->number != 1 || PCI_SLOT(dev->devfn) != 0)
		return;

	printk("PCI: Fixing up bridge\n");

	/* Enable PCI bridge bus mastering */
	pci_set_master(dev);

	/* Enable PCI bridge BAR1 prefetch and burst */
	pci_write_config_dword(dev, PCI_BAR1_CONTROL, 0x3);
}

DECLARE_PCI_FIXUP_FINAL(PCI_ANY_ID, PCI_ANY_ID, pcibios_fixup_resources);
