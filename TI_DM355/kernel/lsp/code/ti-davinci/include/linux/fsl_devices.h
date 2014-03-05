/*
 * include/linux/fsl_devices.h
 *
 * Definitions for any platform device related flags or structures for
 * Freescale processor devices
 *
 * Maintainer: Kumar Gala (kumar.gala@freescale.com)
 *
 * Copyright 2004 Freescale Semiconductor, Inc
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifdef __KERNEL__
#ifndef _FSL_DEVICE_H_
#define _FSL_DEVICE_H_

#include <linux/types.h>

/*
 * Some conventions on how we handle peripherals on Freescale chips
 *
 * unique device: a platform_device entry in fsl_plat_devs[] plus
 * associated device information in its platform_data structure.
 *
 * A chip is described by a set of unique devices.
 *
 * Each sub-arch has its own master list of unique devices and
 * enumerates them by enum fsl_devices in a sub-arch specific header
 *
 * The platform data structure is broken into two parts.  The
 * first is device specific information that help identify any
 * unique features of a peripheral.  The second is any
 * information that may be defined by the board or how the device
 * is connected externally of the chip.
 *
 * naming conventions:
 * - platform data structures: <driver>_platform_data
 * - platform data device flags: FSL_<driver>_DEV_<FLAG>
 * - platform data board flags: FSL_<driver>_BRD_<FLAG>
 *
 */

struct gianfar_platform_data {
	/* device specific information */
	u32 device_flags;

	/* board specific information */
	u32 board_flags;
	u32 bus_id;
	u32 phy_id;
	u8 mac_addr[6];
};

struct gianfar_mdio_data {
	/* board specific information */
	int irq[32];
};

/* Flags related to gianfar device features */
#define FSL_GIANFAR_DEV_HAS_GIGABIT		0x00000001
#define FSL_GIANFAR_DEV_HAS_COALESCE		0x00000002
#define FSL_GIANFAR_DEV_HAS_RMON		0x00000004
#define FSL_GIANFAR_DEV_HAS_MULTI_INTR		0x00000008
#define FSL_GIANFAR_DEV_HAS_CSUM		0x00000010
#define FSL_GIANFAR_DEV_HAS_VLAN		0x00000020
#define FSL_GIANFAR_DEV_HAS_EXTENDED_HASH	0x00000040
#define FSL_GIANFAR_DEV_HAS_PADDING		0x00000080

/* Flags in gianfar_platform_data */
#define FSL_GIANFAR_BRD_HAS_PHY_INTR	0x00000001 /* set or use a timer */
#define FSL_GIANFAR_BRD_IS_REDUCED	0x00000002 /* Set if RGMII, RMII */
#define FSL_GIANFAR_BRD_PHY_ANEG	0x00000004 /* always use autonegotiation */

struct fsl_i2c_platform_data {
	/* device specific information */
	u32 device_flags;
};

/* Flags related to I2C device features */
#define FSL_I2C_DEV_SEPARATE_DFSRR	0x00000001
#define FSL_I2C_DEV_CLOCK_5200		0x00000002


enum fsl_usb2_operating_modes {
	FSL_USB2_MPH_HOST,
	FSL_USB2_DR_HOST,
	FSL_USB2_DR_DEVICE,
	FSL_USB2_DR_OTG,
};

enum fsl_usb2_phy_modes {
	FSL_USB2_PHY_NONE,
	FSL_USB2_PHY_ULPI,
	FSL_USB2_PHY_UTMI,
	FSL_USB2_PHY_UTMI_WIDE,
	FSL_USB2_PHY_SERIAL,
};

struct fsl_usb2_platform_data {
	/* board specific information */
	enum fsl_usb2_operating_modes operating_mode;
	enum fsl_usb2_phy_modes phy_mode;
	unsigned int port_enables;
};

/* Flags in fsl_usb2_mph_platform_data */
#define FSL_USB2_PORT0_ENABLED	0x00000001
#define FSL_USB2_PORT1_ENABLED	0x00000002

/* Ethernet interface (phy management and speed)
*/
typedef enum enet_interface {
	ENET_10_MII,       /* 10 Base T,   MII interface */
	ENET_10_RMII,      /* 10 Base T,  RMII interface */
	ENET_10_RGMII,     /* 10 Base T, RGMII interface */
	ENET_100_MII,      /* 100 Base T,   MII interface */
	ENET_100_RMII,     /* 100 Base T,  RMII interface */
	ENET_100_RGMII,    /* 100 Base T, RGMII interface */
	ENET_1000_GMII,    /* 1000 Base T,  GMII interface */
	ENET_1000_RGMII,   /* 1000 Base T, RGMII interface */
	ENET_1000_TBI,     /* 1000 Base T,   TBI interface */
	ENET_1000_RTBI     /* 1000 Base T,  RTBI interface */
} enet_interface_e;

struct ucc_geth_platform_data {
	/* device specific information */
	u32					device_flags;
	u32					phy_reg_addr;

	/* board specific information */
	u32					board_flags;
	u8					rx_clock;
	u8					tx_clock;
	u32					phy_id;
	enet_interface_e	phy_interface;
	u32					phy_interrupt;
	u8					mac_addr[6];
};

/* Flags related to UCC Gigabit Ethernet device features */
#define FSL_UGETH_DEV_HAS_GIGABIT	0x00000001
#define FSL_UGETH_DEV_HAS_COALESCE	0x00000002
#define FSL_UGETH_DEV_HAS_RMON		0x00000004

/* Flags in ucc_geth_platform_data */
#define FSL_UGETH_BRD_HAS_PHY_INTR	0x00000001	/* if not set use a timer */

#endif				/* _FSL_DEVICE_H_ */
#endif				/* __KERNEL__ */
