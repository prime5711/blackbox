/*
 * Copyright (C) 2005-2006 by Texas Instruments
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 */

#ifndef __MUSB_HDRDF_H__
#define __MUSB_HDRDF_H__

/*
 * DM350-specific definitions
 */

/* Integrated highspeed/otg PHY */
#ifdef CONFIG_ARCH_DM350
#define USBPHY_DATAPOL_INV (1 << 11)
#define USBPHY_CLKSCR_12MH  (1 << 9)
#endif
#define	USBPHY_CTL_PADDR	(DM350_SYSTEM_MODULE_BASE + 0x34)
#define	USBPHY_PHYCLKGD		(1 << 8)
#define	USBPHY_SESNDEN		(1 << 7)	/* v(sess_end) comparator */
#define	USBPHY_VBDTCTEN		(1 << 6)	/* v(bus) comparator */
#define	USBPHY_PHYPLLON		(1 << 4)	/* override pll suspend */

#ifndef CONFIG_ARCH_DM350
#define	USBPHY_CLKO1SEL		(1 << 3)
#endif

#define	USBPHY_OSCPDWN		(1 << 2)
#define	USBPHY_PHYPDWN		(1 << 0)

/* For now include usb OTG module registers here */
#define DM64XX_USB_VERSION_REG		0x00
#define DM64XX_USB_CTRL_REG		0x04
#define DM64XX_USB_STAT_REG		0x08
#define DM64XX_RNDIS_REG		0x10
#define DM64XX_AUTOREQ_REG		0x14
#define DM64XX_USB_INT_SOURCE_REG	0x20
#define DM64XX_USB_INT_SET_REG		0x24
#define DM64XX_USB_INT_SRC_CLR_REG	0x28
#define DM64XX_USB_INT_MASK_REG	0x2c
#define DM64XX_USB_INT_MASK_SET_REG	0x30
#define DM64XX_USB_INT_MASK_CLR_REG	0x34
#define DM64XX_USB_INT_SRC_MASKED_REG	0x38
#define DM64XX_USB_EOI_REG		0x3c
#define DM64XX_USB_EOI_INTVEC		0x40

/* CPPI related registers */
#define DM64XX_TXCPPI_CTRL_REG		0x80
#define DM64XX_TXCPPI_TEAR_REG		0x84
#define DM64XX_CPPI_EOI_REG		0x88
#define DM64XX_CPPI_INTVEC_REG		0x8c
#define DM64XX_TXCPPI_MASKED_REG	0x90
#define DM64XX_TXCPPI_RAW_REG		0x94
#define DM64XX_TXCPPI_INTENAB_REG	0x98
#define DM64XX_TXCPPI_INTCLR_REG	0x9c

#define DM64XX_RXCPPI_CTRL_REG		0xC0
#define DM64XX_RXCPPI_MASKED_REG	0xD0
#define DM64XX_RXCPPI_RAW_REG		0xD4
#define DM64XX_RXCPPI_INTENAB_REG	0xD8
#define DM64XX_RXCPPI_INTCLR_REG	0xDC

#define DM64XX_RXCPPI_BUFCNT0_REG	0xE0
#define DM64XX_RXCPPI_BUFCNT1_REG	0xE4
#define DM64XX_RXCPPI_BUFCNT2_REG	0xE8
#define DM64XX_RXCPPI_BUFCNT3_REG	0xEC

/* CPPI state RAM entries */
#define DM64XX_CPPI_STATERAM_BASE_OFFSET   0x100

#define DM64XX_TXCPPI_STATERAM_OFFSET(channelNum) \
	(DM64XX_CPPI_STATERAM_BASE_OFFSET +       ((channelNum)* 0x40))
#define DM64XX_RXCPPI_STATERAM_OFFSET(channelNum) \
	(DM64XX_CPPI_STATERAM_BASE_OFFSET + 0x20 +((channelNum)* 0x40))

/* CPPI masks */
#define DM64XX_DMA_CTRL_ENABLE		1
#define DM64XX_DMA_CTRL_DISABLE	0

#define DM64XX_DMA_ALL_CHANNELS_ENABLE	0xF
#define DM64XX_DMA_ALL_CHANNELS_DISABLE 0xF

/* REVISIT relying on "volatile" here is wrong ... */

/* define structures of Rx/Tx stateRam entries */
struct cppi_tx_stateram {
	volatile u32 headPtr;
	volatile u32 sopDescPtr;
	volatile u32 currDescPtr;
	volatile u32 currBuffPtr;
	volatile u32 flags;
	volatile u32 remLength;
	volatile u32 dummy;
	volatile u32 completionPtr;
};

struct cppi_rx_stateram {
	volatile u32 buffOffset;
	volatile u32 headPtr;
	volatile u32 sopDescPtr;
	volatile u32 currDescPtr;
	volatile u32 currBuffPtr;
	volatile u32 pktLength;
	volatile u32 byteCount;
	volatile u32 completionPtr;
};

#define DM64XX_USB_TX_ENDPTS_MASK	0x1f		/* ep0 + 4 tx */
#define DM64XX_USB_RX_ENDPTS_MASK	0x1e		/* 4 rx */

#define DM64XX_USB_USBINT_SHIFT	16
#define DM64XX_USB_TXINT_SHIFT		0
#define DM64XX_USB_RXINT_SHIFT		8

#define DM64XX_USB_USBINT_MASK		0x01ff0000	/* 8 Mentor, DRVVBUS */
#define DM64XX_USB_TXINT_MASK \
	(DM64XX_USB_TX_ENDPTS_MASK << DM64XX_USB_TXINT_SHIFT)
#define DM64XX_USB_RXINT_MASK \
	(DM64XX_USB_RX_ENDPTS_MASK << DM64XX_USB_RXINT_SHIFT)

#define DM64XX_BASE_OFFSET		0x400

#endif	/* __MUSB_HDRDF_H__ */
