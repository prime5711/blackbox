/*
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

#ifndef _epivers_h_
#define _epivers_h_

#ifdef	linux
#include <linux/config.h>
#endif

/* Vendor Name, ASCII, 32 chars max */
#ifdef COMPANYNAME
#define	HPNA_VENDOR 		COMPANYNAME
#else
#define	HPNA_VENDOR 		"Broadcom Corporation"
#endif

/* Driver Date, ASCII, 32 chars max */
#define HPNA_DRV_BUILD_DATE	__DATE__

/* Hardware Manufacture Date, ASCII, 32 chars max */
#define HPNA_HW_MFG_DATE	"Not Specified"

/* See documentation for Device Type values, 32 values max */
#ifndef	HPNA_DEV_TYPE

#if	defined(CONFIG_BRCM_VJ)
#define HPNA_DEV_TYPE		{ CDCF_V0_DEVICE_DISPLAY }

#elif	defined(CONFIG_BCRM_93725)
#define HPNA_DEV_TYPE		{ CDCF_V0_DEVICE_CM_BRIDGE, CDCF_V0_DEVICE_DISPLAY }

#else
#define HPNA_DEV_TYPE		{ CDCF_V0_DEVICE_PCINIC }

#endif

#endif				/* !HPNA_DEV_TYPE */


#define	EPI_MAJOR_VERSION	3

#define	EPI_MINOR_VERSION	50

#define	EPI_RC_NUMBER		21

#define	EPI_INCREMENTAL_NUMBER	0

#define	EPI_BUILD_NUMBER	0

#define	EPI_VERSION		3,50,21,0

#define	EPI_VERSION_NUM		0x03321500

/* Driver Version String, ASCII, 32 chars max */
#define	EPI_VERSION_STR		"3.50.21.0"
#define	EPI_ROUTER_VERSION_STR	"3.51.21.0"

#endif				/* _epivers_h_ */
