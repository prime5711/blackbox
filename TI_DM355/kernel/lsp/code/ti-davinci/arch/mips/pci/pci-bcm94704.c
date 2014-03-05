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

#include <linux/config.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/paccess.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/sbconfig.h>
#include <asm/brcm/sbpci.h>
#include <asm/brcm/pcicfg.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/bcmdevs.h>
#include <asm/brcm/bcmnvram.h>

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock

static int
bcm947xx_pci_read_config(struct pci_bus *bus, unsigned int devfn,
			 int where, int size, u32 * value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_read_config(sbh, bus->number, PCI_SLOT(devfn),
			      PCI_FUNC(devfn), where, value, size);
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;

}

static int
bcm947xx_pci_write_config(struct pci_bus *bus, unsigned int devfn,
			  int where, int size, u32 value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_write_config(sbh, bus->number, PCI_SLOT(devfn),
			       PCI_FUNC(devfn), where, &value, size);
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_read_config_byte(struct pci_dev *dev, int where, u8 * value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_read_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			      PCI_FUNC(dev->devfn), where, value,
			      sizeof(*value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_read_config_word(struct pci_dev *dev, int where, u16 * value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_read_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			      PCI_FUNC(dev->devfn), where, value,
			      sizeof(*value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_read_config_dword(struct pci_dev *dev, int where, u32 * value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_read_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			      PCI_FUNC(dev->devfn), where, value,
			      sizeof(*value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_write_config_byte(struct pci_dev *dev, int where, u8 value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_write_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			       PCI_FUNC(dev->devfn), where, &value,
			       sizeof(value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_write_config_word(struct pci_dev *dev, int where, u16 value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_write_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			       PCI_FUNC(dev->devfn), where, &value,
			       sizeof(value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

int sbpci_write_config_dword(struct pci_dev *dev, int where, u32 value)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&sbh_lock, flags);
	ret =
	    sbpci_write_config(sbh, dev->bus->number, PCI_SLOT(dev->devfn),
			       PCI_FUNC(dev->devfn), where, &value,
			       sizeof(value));
	spin_unlock_irqrestore(&sbh_lock, flags);
	return ret ? PCIBIOS_DEVICE_NOT_FOUND : PCIBIOS_SUCCESSFUL;
}

static struct pci_ops pcibios_ops = {
	.read = bcm947xx_pci_read_config,
	.write = bcm947xx_pci_write_config
};

static struct resource bcm947xx_mem_resource = {
	.name = "bcm947xx mem",
	.start = SB_PCI_MEM,
	.end = SB_PCI_MEM + 0x04000000,
	.flags = IORESOURCE_MEM,
};

static struct resource bcm947xx_io_resource = {
	.name = "bcm947xx io",
	.start = 0x1000,
	.end = 0x1000 + 0x4000,
	.flags = IORESOURCE_IO,
};

static struct pci_controller bcm947xx_controller = {
	.pci_ops = &pcibios_ops,
	.mem_resource = &bcm947xx_mem_resource,
	.io_resource = &bcm947xx_io_resource,
};

static int __init bcm947xx_pcibios_init(void)
{
	ulong flags;
	extern int pci_probe_only;

	pci_probe_only = 1;
	if (!(sbh = sb_kattach()))
		panic("sb_kattach failed");
	spin_lock_init(&sbh_lock);
	spin_lock_irqsave(&sbh_lock, flags);
	sbpci_init(sbh);
	spin_unlock_irqrestore(&sbh_lock, flags);
	set_io_port_base((unsigned long)
			 ioremap_nocache(SB_PCI_MEM, 0x04000000));
	register_pci_controller(&bcm947xx_controller);
	return 0;
}

arch_initcall(bcm947xx_pcibios_init);
