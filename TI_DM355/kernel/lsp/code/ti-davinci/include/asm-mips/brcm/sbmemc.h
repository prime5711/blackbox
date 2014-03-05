/*
 * BCM47XX Sonics SiliconBackplane DDR/SDRAM controller core hardware definitions.
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

#ifndef	_SBMEMC_H
#define	_SBMEMC_H

#ifdef _LANGUAGE_ASSEMBLY

#define	MEMC_CONTROL		0x00
#define	MEMC_CONFIG		0x04
#define	MEMC_REFRESH		0x08
#define	MEMC_BISTSTAT		0x0c
#define	MEMC_MODEBUF		0x10
#define	MEMC_BKCLS		0x14
#define	MEMC_PRIORINV		0x18
#define	MEMC_DRAMTIM		0x1c
#define	MEMC_INTSTAT		0x20
#define	MEMC_INTMASK		0x24
#define	MEMC_INTINFO		0x28
#define	MEMC_NCDLCTL		0x30
#define	MEMC_RDNCDLCOR		0x34
#define	MEMC_WRNCDLCOR		0x38
#define	MEMC_MISCDLYCTL		0x3c
#define	MEMC_DQSGATENCDL	0x40
#define	MEMC_SPARE		0x44
#define	MEMC_TPADDR		0x48
#define	MEMC_TPDATA		0x4c
#define	MEMC_BARRIER		0x50
#define	MEMC_CORE		0x54


#else

/* Sonics side: MEMC core registers */
typedef volatile struct sbmemcregs {
	uint32	control;
	uint32	config;
	uint32	refresh;
	uint32	biststat;
	uint32	modebuf;
	uint32	bkcls;
	uint32	priorinv;
	uint32	dramtim;
	uint32	intstat;
	uint32	intmask;
	uint32	intinfo;
	uint32	reserved1;
	uint32	ncdlctl;
	uint32	rdncdlcor;
	uint32	wrncdlcor;
	uint32	miscdlyctl;
	uint32	dqsgatencdl;
	uint32	spare;
	uint32	tpaddr;
	uint32	tpdata;
	uint32	barrier;
	uint32	core;
} sbmemcregs_t;

#endif

/* MEMC Core Init values (OCP ID 0x80f) */

/* For SDR: */
#define MEMC_SD_CONFIG_INIT	0x00048000
#define MEMC_SD_DRAMTIM_INIT	0x000754da
#define MEMC_SD_RDNCDLCOR_INIT	0x00000000
#define MEMC_SD_WRNCDLCOR_INIT	0x49351200
#define MEMC_SD1_WRNCDLCOR_INIT	0x14500200	/* For corerev 1 (4712) */
#define MEMC_SD_MISCDLYCTL_INIT	0x00061c1b
#define MEMC_SD1_MISCDLYCTL_INIT 0x00021416	/* For corerev 1 (4712) */
#define MEMC_SD_CONTROL_INIT0	0x00000002
#define MEMC_SD_CONTROL_INIT1	0x00000008
#define MEMC_SD_CONTROL_INIT2	0x00000004
#define MEMC_SD_CONTROL_INIT3	0x00000010
#define MEMC_SD_CONTROL_INIT4	0x00000001
#define MEMC_SD_MODEBUF_INIT	0x00000000
#define MEMC_SD_REFRESH_INIT	0x0000840f


/* This is for SDRM8X8X4 */
#define	MEMC_SDR_INIT		0x0008
#define	MEMC_SDR_MODE		0x32
#define	MEMC_SDR_NCDL		0x00020032
#define	MEMC_SDR1_NCDL		0x0002020f	/* For corerev 1 (4712) */

/* For DDR: */
#define MEMC_CONFIG_INIT	0x00048000
#define MEMC_DRAMTIM_INIT	0x000754d9
#define MEMC_RDNCDLCOR_INIT	0x00000000
#define MEMC_WRNCDLCOR_INIT	0x49351200
#define MEMC_1_WRNCDLCOR_INIT	0x14500200
#define MEMC_DQSGATENCDL_INIT	0x00030000
#define MEMC_MISCDLYCTL_INIT	0x21061c1b
#define MEMC_1_MISCDLYCTL_INIT	0x21021400
#define MEMC_NCDLCTL_INIT	0x00002001
#define MEMC_CONTROL_INIT0	0x00000002
#define MEMC_CONTROL_INIT1	0x00000008
#define MEMC_MODEBUF_INIT0	0x00004000
#define MEMC_CONTROL_INIT2	0x00000010
#define MEMC_MODEBUF_INIT1	0x00000100
#define MEMC_CONTROL_INIT3	0x00000010
#define MEMC_CONTROL_INIT4	0x00000008
#define MEMC_REFRESH_INIT	0x0000840f
#define MEMC_CONTROL_INIT5	0x00000004
#define MEMC_MODEBUF_INIT2	0x00000000
#define MEMC_CONTROL_INIT6	0x00000010
#define MEMC_CONTROL_INIT7	0x00000001


/* This is for DDRM16X16X2 */
#define	MEMC_DDR_INIT		0x0009
#define	MEMC_DDR_MODE		0x62
#define	MEMC_DDR_NCDL		0x0005050a
#define	MEMC_DDR1_NCDL		0x00000a0a	/* For corerev 1 (4712) */

/* mask for SDR/DDR calibration registers */
#define MEMC_RDNCDLCOR_RD_MASK	0x000000ff
#define MEMC_WRNCDLCOR_WR_MASK	0x000000ff
#define MEMC_DQSGATENCDL_G_MASK	0x000000ff

/* masks for MiscDlyCtl registers */
#define MEMC_MISC_SM_MASK	0x30000000
#define MEMC_MISC_SM_SHIFT	28
#define MEMC_MISC_SD_MASK	0x0f000000
#define MEMC_MISC_SD_SHIFT	24

/* hw threshhold for calculating wr/rd for SDR MEMC */
#define MEMC_CD_THRESHOLD	128

/* Low bit of init register says if MEMC is DDR or SDR */
#define MEMC_CONFIG_DDR		0x00000001

#endif	/* _SBMEMC_H */
