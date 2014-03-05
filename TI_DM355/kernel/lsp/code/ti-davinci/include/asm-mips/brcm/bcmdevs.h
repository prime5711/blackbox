/*
 * Broadcom device-specific manifest constants.
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

#ifndef	_BCMDEVS_H
#define	_BCMDEVS_H

/* Known PCI vendor Id's */
#define	VENDOR_EPIGRAM		0xfeda
#define	VENDOR_BROADCOM		0x14e4
#define	VENDOR_3COM		0x10b7
#define	VENDOR_NETGEAR		0x1385
#define	VENDOR_DIAMOND		0x1092
#define	VENDOR_DELL		0x1028
#define	VENDOR_HP		0x0e11
#define	VENDOR_APPLE		0x106b

/* PCI Device Id's */
#define	BCM4210_DEVICE_ID	0x1072	/* never used */
#define	BCM4211_DEVICE_ID	0x4211
#define	BCM4230_DEVICE_ID	0x1086	/* never used */
#define	BCM4231_DEVICE_ID	0x4231

#define	BCM4410_DEVICE_ID	0x4410	/* bcm44xx family pci iline */
#define	BCM4430_DEVICE_ID	0x4430	/* bcm44xx family cardbus iline */
#define	BCM4412_DEVICE_ID	0x4412	/* bcm44xx family pci enet */
#define	BCM4432_DEVICE_ID	0x4432	/* bcm44xx family cardbus enet */

#define	BCM3352_DEVICE_ID	0x3352	/* bcm3352 device id */
#define	BCM3360_DEVICE_ID	0x3360	/* bcm3360 device id */

#define	EPI41210_DEVICE_ID	0xa0fa	/* bcm4210 */
#define	EPI41230_DEVICE_ID	0xa10e	/* bcm4230 */

#define	BCM47XX_ILINE_ID	0x4711	/* 47xx iline20 */
#define	BCM47XX_V90_ID		0x4712	/* 47xx v90 codec */
#define	BCM47XX_ENET_ID		0x4713	/* 47xx enet */
#define	BCM47XX_EXT_ID		0x4714	/* 47xx external i/f */
#define	BCM47XX_USB_ID		0x4715	/* 47xx usb */
#define	BCM47XX_USBH_ID		0x4716	/* 47xx usb host */
#define	BCM47XX_USBD_ID		0x4717	/* 47xx usb device */
#define	BCM47XX_IPSEC_ID	0x4718	/* 47xx ipsec */

#define	BCM4710_DEVICE_ID	0x4710	/* 4710 primary function 0 */

#define	BCM4610_DEVICE_ID	0x4610	/* 4610 primary function 0 */
#define	BCM4610_ILINE_ID	0x4611	/* 4610 iline100 */
#define	BCM4610_V90_ID		0x4612	/* 4610 v90 codec */
#define	BCM4610_ENET_ID		0x4613	/* 4610 enet */
#define	BCM4610_EXT_ID		0x4614	/* 4610 external i/f */
#define	BCM4610_USB_ID		0x4615	/* 4610 usb */

#define	BCM4402_DEVICE_ID	0x4402	/* 4402 primary function 0 */
#define	BCM4402_ENET_ID		0x4402	/* 4402 enet */
#define	BCM4402_V90_ID		0x4403	/* 4402 v90 codec */

#define	BCM4301_DEVICE_ID	0x4301	/* 4301 primary function 0 */
#define	BCM4301_D11B_ID		0x4301	/* 4301 802.11b */

#define	BCM4307_DEVICE_ID	0x4307	/* 4307 primary function 0 */
#define	BCM4307_V90_ID		0x4305	/* 4307 v90 codec */
#define	BCM4307_ENET_ID		0x4306	/* 4307 enet */
#define	BCM4307_D11B_ID		0x4307	/* 4307 802.11b */

#define	BCM4306_DEVICE_ID	0x4306	/* 4306 chipcommon chipid */
#define	BCM4306_D11G_ID		0x4320	/* 4306 802.11g */
#define	BCM4306_D11G_ID2	0x4325
#define	BCM4306_D11A_ID		0x4321	/* 4306 802.11a */
#define	BCM4306_UART_ID		0x4322	/* 4306 uart */
#define	BCM4306_V90_ID		0x4323	/* 4306 v90 codec */
#define	BCM4306_D11DUAL_ID	0x4324	/* 4306 dual A+B */

#define	BCM4309_PKG_ID		1	/* 4309 package id */

#define	BCM4303_D11B_ID		0x4303	/* 4303 802.11b */
#define	BCM4303_PKG_ID		2	/* 4303 package id */

#define	BCM4310_DEVICE_ID	0x4310	/* 4310 chipcommon chipid */
#define	BCM4310_D11B_ID		0x4311	/* 4310 802.11b */
#define	BCM4310_UART_ID		0x4312	/* 4310 uart */
#define	BCM4310_ENET_ID		0x4313	/* 4310 enet */
#define	BCM4310_USB_ID		0x4315	/* 4310 usb */

#define	BCM4704_DEVICE_ID	0x4704	/* 4704 chipcommon chipid */
#define	BCM4704_V90_ID		0x4705	/* 4704 802.11b */
#define	BCM4704_ENET_ID		0x4706	/* 4704 enet (Use 47XX_ENET_ID instead!) */
#define	BCM4704_USB_ID		0x4707	/* 4704 usb */
#define	BCM4704_IPSEC_ID	0x4708	/* 4704 ipsec */

#define	BCM4712_DEVICE_ID	0x4712	/* 4712 chipcommon chipid */
#define	BCM4712_MIPS_ID		0x4720	/* 4712 base devid */
#define	BCM4712SMALL_PKG_ID	1	/* 200pin 4712 package id */

#define	BCM4317_DEVICE_ID	0x4317	/* 4317 chip common chipid */
#define	BCM4317_D11B_ID		0x4317	/* 4317 802.11b */

#define	SDIOH_FPGA_ID		0x4380	/* sdio host fpga */

#define BCM5365_DEVICE_ID       0x5365	/* 5365 chipcommon chipid */
#define BCM5365_ENET_ID         0x0806	/* 5365 enet */


/* PCMCIA vendor Id's */

#define	VENDOR_BROADCOM_PCMCIA	0x02d0

/* SDIO vendor Id's */
#define	VENDOR_BROADCOM_SDIO	0x00BF


/* boardflags */
#define	BFL_BTCOEXIST		0x0001	/* This board implements Bluetooth coexistance */
#define	BFL_PACTRL		0x0002	/* This board has GPIO 9 controlling the PA */
#define	BFL_AIRLINEMODE		0x0004	/* This board implements GPIO 13 radio disable indication */
#define	BFL_ENETSPI		0x0010	/* This board has ephy roboswitch spi */
#define	BFL_CCKHIPWR		0x0040	/* Can do high-power CCK transmission */
#define	BFL_ENETADM		0x0080	/* This board has ADMtek switch */
#define	BFL_ENETVLAN		0x0100	/* This board can do vlan */

/* board specific GPIO assignment, gpio 0-3 are also customer-configurable led */
#define BOARD_GPIO_HWRAD_B	0x010	/* bit 4 is HWRAD input on 4301 */
#define	BOARD_GPIO_BTC_IN	0x080	/* bit 7 is BT Coexistance Input */
#define	BOARD_GPIO_BTC_OUT	0x100	/* bit 8 is BT Coexistance Out */
#define	BOARD_GPIO_PACTRL	0x200	/* bit 9 controls the PA on new 4306 boards */
#define	PCI_CFG_GPIO_SCS	0x10	/* PCI config space bit 4 for 4306c0 slow clock source */
#define PCI_CFG_GPIO_HWRAD	0x20	/* PCI config space GPIO 13 for hw radio disable */
#define PCI_CFG_GPIO_XTAL	0x40	/* PCI config space GPIO 14 for Xtal powerup */
#define PCI_CFG_GPIO_PLL	0x80	/* PCI config space GPIO 15 for PLL powerdown */

/* Bus types */
#define	SB_BUS			0	/* Silicon Backplane */
#define	PCI_BUS			1	/* PCI target */
#define	PCMCIA_BUS		2	/* PCMCIA target */
#define SDIO_BUS		3	/* SDIO target */

/* Reference Board Types */

#define	BU4710_BOARD		0x0400
#define	VSIM4710_BOARD		0x0401
#define	QT4710_BOARD		0x0402

#define	BU4610_BOARD		0x0403
#define	VSIM4610_BOARD		0x0404

#define	BU4307_BOARD		0x0405
#define	BCM94301CB_BOARD	0x0406
#define	BCM94301PC_BOARD	0x0406	/* PCMCIA 5V card */
#define	BCM94301MP_BOARD	0x0407
#define	BCM94307MP_BOARD	0x0408
#define	BCMAP4307_BOARD		0x0409

#define	BU4309_BOARD		0x040a
#define	BCM94309CB_BOARD	0x040b
#define	BCM94309MP_BOARD	0x040c
#define	BCM4309AP_BOARD		0x040d

#define	BCM94302MP_BOARD	0x040e

#define	VSIM4310_BOARD		0x040f
#define	BU4711_BOARD		0x0410
#define	BCM94310U_BOARD		0x0411
#define	BCM94310AP_BOARD	0x0412
#define	BCM94310MP_BOARD	0x0414

#define	BU4306_BOARD		0x0416
#define	BCM94306CB_BOARD	0x0417
#define	BCM94306MP_BOARD	0x0418

#define	BCM94710D_BOARD		0x041a
#define	BCM94710R1_BOARD	0x041b
#define	BCM94710R4_BOARD	0x041c
#define	BCM94710AP_BOARD	0x041d


#define	BU2050_BOARD		0x041f


#define	BCM94309G_BOARD		0x0421

#define	BCM94301PC3_BOARD	0x0422	/* PCMCIA 3.3V card */

#define	BU4704_BOARD		0x0423
#define	BU4702_BOARD		0x0424

#define	BCM94306PC_BOARD	0x0425	/* PCMCIA 3.3V 4306 card */



#define	BCM94702MN_BOARD	0x0428

/* BCM4702 1U CompactPCI Board */
#define	BCM94702CPCI_BOARD	0x0429

/* BCM4702 with BCM95380 VLAN Router */
#define	BCM95380RR_BOARD	0x042a

/* cb4306 with SiGe PA */
#define	BCM94306CBSG_BOARD	0x042b

/* mp4301 with 2050 radio */
#define	BCM94301MPL_BOARD	0x042c

/* cb4306 with SiGe PA */
#define	PCSG94306_BOARD		0x042d

/* bu4704 with sdram */
#define	BU4704SD_BOARD		0x042e

/* Dual 11a/11g Router */
#define	BCM94704AGR_BOARD	0x042f

/* 11a-only minipci */
#define	BCM94308MP_BOARD	0x0430



/* BCM5365K Sentry-5 Evaluation Platform */
#define BCM95365K_BOARD         0x0444

/* BCM5365P Sentry-5 PCI NIC Evaluation Platform */
#define BCM95365P_BOARD         0x0445

/* BCM5365R Sentry-5 Router/AP Reference Platform */
#define BCM95365R_BOARD         0x0446

#endif				/* _BCMDEVS_H */