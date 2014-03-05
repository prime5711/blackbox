/*
 * Common [OS-independent] portion of
 * Broadcom Home Networking Division 10/100 Mbit/s Ethernet
 * Device Driver.
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

#include <asm/brcm/osl.h>
#include <asm/brcm/bcmendian.h>
#include <asm/brcm/proto/ethernet.h>
#include <asm/brcm/bcmenetmib.h>
#include <asm/brcm/bcmenetrxh.h>
#include <et_dbg.h>
#include <etc.h>
#include <et_export.h>
#include <asm/brcm/bcmutils.h>

int et_msg_level = 0;

/* local prototypes */
static void etc_loopback(etc_info_t * etc, int on);

/* find the chip opsvec for this chip */
struct chops *etc_chipmatch(uint vendor, uint device)
{
	{
		extern struct chops bcm47xx_et_chops;
		if (bcm47xx_et_chops.id(vendor, device))
			return (&bcm47xx_et_chops);
	}
	return (NULL);
}

void *etc_attach(void *et, uint vendor, uint device, uint unit, void *pdev,
		 void *regsva)
{
	etc_info_t *etc;

	ET_TRACE(("et%d: etc_attach: vendor 0x%x device 0x%x\n", unit,
		  vendor, device));

	/* some code depends on packed structures */
	ASSERT(sizeof(struct ether_addr) == ETHER_ADDR_LEN);
	ASSERT(sizeof(struct ether_header) == ETHER_HDR_LEN);

	/* allocate etc_info_t state structure */
	if ((etc = (etc_info_t *) MALLOC(sizeof(etc_info_t))) == NULL) {
		ET_ERROR(("et%d: etc_attach malloc failed\n", unit));
		return (NULL);
	}
	bzero((char *) etc, sizeof(etc_info_t));

	/* perform any osl specific init */
	osl_init();

	etc->et = et;
	etc->unit = unit;
	etc->vendorid = (uint16) vendor;
	etc->deviceid = (uint16) device;
	etc->forcespeed = ET_AUTO;
	etc->linkstate = FALSE;

	/* set chip opsvec */
	etc->chops = etc_chipmatch(vendor, device);
	ASSERT(etc->chops);

	/* chip attach */
	if ((etc->ch = (*etc->chops->attach) (etc, pdev, regsva)) == NULL) {
		ET_ERROR(("et%d: chipattach error\n", unit));
		goto fail;
	}

	return ((void *) etc);

      fail:
	etc_detach(etc);
	return (NULL);
}

void etc_detach(etc_info_t * etc)
{
	if (etc == NULL)
		return;

	/* free chip private state */
	if (etc->ch) {
		(*etc->chops->detach) (etc->ch);
		etc->chops = etc->ch = NULL;
	}

	MFREE(etc, sizeof(etc_info_t));
}

void etc_reset(etc_info_t * etc)
{
	ET_TRACE(("et%d: etc_reset\n", etc->unit));

	etc->reset++;

	/* reset the chip */
	(*etc->chops->reset) (etc->ch);

	/* free any posted tx packets */
	(*etc->chops->txreclaim) (etc->ch, TRUE);

#ifdef DMA
	/* free any posted rx packets */
	(*etc->chops->rxreclaim) (etc->ch);
#endif
}

void etc_init(etc_info_t * etc)
{
	ET_TRACE(("et%d: etc_init\n", etc->unit));

	ASSERT(etc->pioactive == NULL);
	ASSERT(!ETHER_ISNULLADDR(&etc->cur_etheraddr));
	ASSERT(!ETHER_ISMULTI(&etc->cur_etheraddr));

	/* init the chip */
	(*etc->chops->init) (etc->ch, TRUE);
}

/* mark interface up */
void etc_up(etc_info_t * etc)
{
	etc->up = TRUE;

	et_init(etc->et);
}

/* mark interface down */
uint etc_down(etc_info_t * etc, int reset)
{
	uint callback;

	callback = 0;

	etc->up = FALSE;
	if (reset)
		et_reset(etc->et);

	/* suppress link state changes during power management mode changes */
	if (etc->linkstate) {
		etc->linkstate = FALSE;
		if (!etc->pm_modechange)
			et_link_down(etc->et);
	}

	return (callback);
}

/* common ioctl handler.  return: 0=ok, -1=error */
int etc_ioctl(etc_info_t * etc, int cmd, void *arg)
{
	int error;
	int val;
	int *vec = (int *) arg;

	error = 0;

	val = arg ? *(int *) arg : 0;

	ET_TRACE(("et%d: etc_ioctl: cmd 0x%x\n", etc->unit, cmd));

	switch (cmd) {
	case ETCUP:
		et_up(etc->et);
		break;

	case ETCDOWN:
		et_down(etc->et, TRUE);
		break;

	case ETCLOOP:
		etc_loopback(etc, val);
		break;

	case ETCDUMP:
		if (et_msg_level & 0x10000)
			bcmdumplog((uchar *) arg, 4096);
		break;

	case ETCSETMSGLEVEL:
		et_msg_level = val;
		break;

	case ETCPROMISC:
		etc_promisc(etc, val);
		break;

	case ETCSPEED:
		if ((val != ET_AUTO) && (val != ET_10HALF)
		    && (val != ET_10FULL)
		    && (val != ET_100HALF) && (val != ET_100FULL))
			goto err;
		etc->forcespeed = val;

		/* explicitly reset the phy */
		(*etc->chops->phyreset) (etc->ch, etc->phyaddr);

		/* request restart autonegotiation if we're reverting to adv mode */
		if ((etc->forcespeed == ET_AUTO) & etc->advertise)
			etc->needautoneg = TRUE;

		et_init(etc->et);
		break;

	case ETCPHYRD:
		if (vec)
			vec[1] =
			    (*etc->chops->phyrd) (etc->ch, etc->phyaddr,
						  vec[0]);
		break;

	case ETCPHYWR:
		if (vec)
			(*etc->chops->phywr) (etc->ch, etc->phyaddr,
					      vec[0], (uint16) vec[1]);
		break;

	default:
	      err:
		error = -1;
	}

	return (error);
}

/* called once per second */
void etc_watchdog(etc_info_t * etc)
{
	uint16 status;
	uint16 adv;
	uint16 lpa;

	etc->now++;

	if (etc->phyaddr == PHY_NOMDC) {
		etc->linkstate = TRUE;
		etc->speed = 100;
		etc->duplex = 1;
		return;
	}

	status = (*etc->chops->phyrd) (etc->ch, etc->phyaddr, 1);
	adv = (*etc->chops->phyrd) (etc->ch, etc->phyaddr, 4);
	lpa = (*etc->chops->phyrd) (etc->ch, etc->phyaddr, 5);

	/* check for bad mdio read */
	if (status == 0xffff) {
		ET_ERROR(("et%d: etc_watchdog: bad mdio read: phyaddr %d mdcport %d\n", etc->unit, etc->phyaddr, etc->mdcport));
		return;
	}

	/* monitor link state */
	if (!etc->linkstate && (status & STAT_LINK)) {
		etc->linkstate = TRUE;

		if (etc->pm_modechange)
			etc->pm_modechange = FALSE;
		else
			et_link_up(etc->et);
	} else if (etc->linkstate && !(status & STAT_LINK)) {
		etc->linkstate = FALSE;
		if (!etc->pm_modechange)
			et_link_down(etc->et);
	}

	/* update current speed and duplex */
	if ((adv & ADV_100FULL) && (lpa & LPA_100FULL)) {
		etc->speed = 100;
		etc->duplex = 1;
	} else if ((adv & ADV_100HALF) && (lpa & LPA_100HALF)) {
		etc->speed = 100;
		etc->duplex = 0;
	} else if ((adv & ADV_10FULL) && (lpa & LPA_10FULL)) {
		etc->speed = 10;
		etc->duplex = 1;
	} else {
		etc->speed = 10;
		etc->duplex = 0;
	}

	/* keep emac txcontrol duplex bit consistent with current phy duplex */
	(*etc->chops->duplexupd) (etc->ch);

	/* check for remote fault error */
	if (status & STAT_REMFAULT) {
		ET_ERROR(("et%d: remote fault\n", etc->unit));
	}

	/* check for jabber error */
	if (status & STAT_JAB) {
		ET_ERROR(("et%d: jabber\n", etc->unit));
	}

	/*
	 * Read chip MIB counters occasionally before the 16-bit ones can wrap.
	 * We don't use the high-rate MIB counters.
	 */
	if ((etc->now % 30) == 0)
		(*etc->chops->statsupd) (etc->ch);
}

static void etc_loopback(etc_info_t * etc, int on)
{
	ET_TRACE(("et%d: etc_loopback: %d\n", etc->unit, on));

	etc->loopbk = (bool) on;
	et_init(etc->et);
}

void etc_promisc(etc_info_t * etc, uint on)
{
	ET_TRACE(("et%d: etc_promisc: %d\n", etc->unit, on));

	etc->promisc = (bool) on;
	et_init(etc->et);
}

uint etc_totlen(etc_info_t * etc, void *p)
{
	uint total;

	total = 0;
	for (; p; p = PKTNEXT(etc->et, p))
		total += PKTLEN(etc->et, p);
	return (total);
}
