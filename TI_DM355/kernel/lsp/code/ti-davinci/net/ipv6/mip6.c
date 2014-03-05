/*
 * Copyright (C)2003 Helsinki University of Technology
 * Copyright (C)2003 USAGI/WIDE Project
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * Authors:
 *	Noriaki TAKAMIYA @USAGI
 *	Masahide NAKAMURA @USAGI
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ip6_checksum.h>
#include <net/sock.h>
#include <net/ipv6.h>
#include <net/mip6.h>
#include <net/protocol.h>
#include <net/xfrm.h>

struct mip6_be_rate_limiter {
	spinlock_t lock;
	struct timeval stamp;
	int iif;
	struct in6_addr src;
	struct in6_addr dst;
};

static struct mip6_be_rate_limiter mip6_be_rl = {
	.lock = SPIN_LOCK_UNLOCKED
};

static inline int mip6_be_rl_allow(struct sk_buff *skb)
{
	struct inet6_skb_parm *opt = (struct inet6_skb_parm *)skb->cb;
	struct destopt_hao *hao = (struct destopt_hao *)(skb->nh.raw + opt->hao);
	int allow = 0;

	spin_lock_bh(&mip6_be_rl.lock);
	if (mip6_be_rl.stamp.tv_sec != skb->stamp.tv_sec ||
	    mip6_be_rl.stamp.tv_usec != skb->stamp.tv_usec ||
	    mip6_be_rl.iif != opt->iif ||
	    ipv6_addr_cmp(&mip6_be_rl.src, &hao->addr) ||
	    ipv6_addr_cmp(&mip6_be_rl.dst, &skb->nh.ipv6h->daddr)) {
		mip6_be_rl.stamp.tv_sec = skb->stamp.tv_sec;
		mip6_be_rl.stamp.tv_usec = skb->stamp.tv_usec;
		mip6_be_rl.iif = opt->iif;
		ipv6_addr_copy(&mip6_be_rl.src, &hao->addr);
		ipv6_addr_copy(&mip6_be_rl.dst, &skb->nh.ipv6h->daddr);
		allow = 1;
	}
	spin_unlock_bh(&mip6_be_rl.lock);
	return allow;
}


unsigned inline int calc_padlen(unsigned int len, unsigned int n)
{
	return (n - len + 16) & 0x7;
}

static inline void *mip6_padn(__u8 *data, __u8 padlen)
{
	if (!data)
		return NULL;
	if (padlen == 1) {
		data[0] = MIP6_OPT_PAD_1;
	} else if (padlen > 1) {
		data[0] = MIP6_OPT_PAD_N;
		data[1] = padlen - 2;
		if (padlen > 2)
			memset(data+2, 0, data[1]);
	}
	return data + padlen;
}

/*****************************
 * Mobility Header
 *****************************/

static int _mh_len(int type)
{
	int len = 0;

	switch (type) {
	case IP6_MH_TYPE_BRR:
		len = 0;
		break;
	case IP6_MH_TYPE_HOTI:
	case IP6_MH_TYPE_COTI:
	case IP6_MH_TYPE_BU:
	case IP6_MH_TYPE_BACK:
		len = 1;
		break;
	case IP6_MH_TYPE_HOT:
	case IP6_MH_TYPE_COT:
	case IP6_MH_TYPE_BERROR:
		len = 2;
		break;
	}
	return len;
}

static inline void mip6_param_prob(struct sk_buff *skb, int code, int pos)
{
	icmpv6_send(skb, ICMPV6_PARAMPROB, code, pos, skb->dev);
}

int mip6_mh_filter(struct sock *sk, struct sk_buff *skb)
{
	struct ip6_mh *mh;
	int mhlen;

	if (!pskb_may_pull(skb, (skb->h.raw - skb->data) + 8) ||
	    !pskb_may_pull(skb, (skb->h.raw - skb->data) + ((skb->h.raw[1] + 1) << 3)))
		return -1;

	mh = (struct ip6_mh *)skb->h.raw;

	/* check mobility header length */
	if (mh->ip6mh_hdrlen < _mh_len(mh->ip6mh_type)) {
		LIMIT_NETDEBUG(
			printk(KERN_DEBUG "mip6: MH message too short: %d vs >=%d\n",
			       mh->ip6mh_hdrlen, _mh_len(mh->ip6mh_type)));
		mip6_param_prob(skb, 0, (&mh->ip6mh_hdrlen) - skb->nh.raw);
		return -1;
	}
	mhlen = (mh->ip6mh_hdrlen + 1) << 3;

	/* verify checksum is correct */
	if (skb->ip_summed == CHECKSUM_HW) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		if (csum_ipv6_magic(&skb->nh.ipv6h->saddr,
				    &skb->nh.ipv6h->daddr,
				    mhlen, IPPROTO_MH,
				    skb->csum)) {
			LIMIT_NETDEBUG(
				printk(KERN_DEBUG "mip6: MH hw checksum failed\n"));
			skb->ip_summed = CHECKSUM_NONE;
		}
	}
	if (skb->ip_summed == CHECKSUM_NONE) {
		if (csum_ipv6_magic(&skb->nh.ipv6h->saddr,
				    &skb->nh.ipv6h->daddr,
				    mhlen, IPPROTO_MH,
				    skb_checksum(skb, 0, mhlen, 0))) {
			LIMIT_NETDEBUG(
				printk(KERN_DEBUG "mip6: MH checksum failed [%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x > %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x]\n",
				       NIP6(skb->nh.ipv6h->saddr),
				       NIP6(skb->nh.ipv6h->daddr)));
 			return -1;
			return -1;
		}
		skb->ip_summed = CHECKSUM_UNNECESSARY;
	}

	/* payload proto */
	if (mh->ip6mh_proto != IPPROTO_NONE) {
		LIMIT_NETDEBUG(
			printk(KERN_DEBUG "mip6: MH invalid payload proto = %d\n",
			       mh->ip6mh_proto));
		mip6_param_prob(skb, 0, (&mh->ip6mh_proto) - skb->nh.raw);
		return -1;
	}

	return 0;
}

/*****************************
 * Destination Options Header
 *****************************/

static int mip6_destopt_input(struct xfrm_state *x, struct xfrm_decap_state *decap, struct sk_buff *skb)
{
	struct ipv6hdr *iph = skb->nh.ipv6h;
	struct inet6_skb_parm *opt = (struct inet6_skb_parm *)skb->cb;
	struct destopt_hao *hao = (struct destopt_hao *)(skb->nh.raw + opt->hao);
	struct in6_addr tmp_addr;
	struct ipv6_destopt_hdr *destopt = (struct ipv6_destopt_hdr *)(skb->nh.raw + opt->dsthao);

	/* Verifying care-of address */
	if (ipv6_addr_cmp(&iph->saddr, (struct in6_addr *)x->coaddr) &&
	    !ipv6_addr_any((struct in6_addr *)x->coaddr)) {
		LIMIT_NETDEBUG(
			printk(KERN_DEBUG
			       "mip6: hao mismatch care-of address:"
			       " input = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
			       " kern = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
			       " hoa = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n",
			       NIP6(iph->saddr),
			       NIP6(*(struct in6_addr *)x->coaddr),
			       NIP6(hao->addr)));
		return -ENOENT;
	}

	/* Replacing address */
	memcpy(&tmp_addr, &iph->saddr, sizeof(tmp_addr));
	memcpy(&iph->saddr, &hao->addr, sizeof(iph->saddr));
	memcpy(&hao->addr, &tmp_addr, sizeof(hao->addr));

	return destopt->nexthdr;
}

/* Send a Binding Error for packets with home address option, which SPD dropped  */
static int mip6_destopt_post_reject(struct xfrm_state *x, struct xfrm_decap_state *decap, struct sk_buff *skb, struct flowi *fl)
{
	struct inet6_skb_parm *opt = (struct inet6_skb_parm *)skb->cb;
	struct destopt_hao *hao = (struct destopt_hao *)(skb->nh.raw + opt->hao);
	if ((fl->proto != IPPROTO_MH || fl->fl_mh_type > IP6_MH_TYPE_MAX) && 
	    mip6_be_rl_allow(skb))
		xfrm_send_mip6notify(fl->proto == IPPROTO_MH ?
				     IP6_MH_BES_UNKNOWN_MH :
				     IP6_MH_BES_UNKNOWN_HAO, opt->iif,
				     (xfrm_address_t *)&hao->addr,
				     (xfrm_address_t *)&skb->nh.ipv6h->saddr, 
				     (xfrm_address_t *)&skb->nh.ipv6h->daddr);
	return 0;
}

int mip6_destopt_place_find(struct sk_buff *skb, u8 **nexthdr)
{
	u16 offset = sizeof(struct ipv6hdr);
	struct ipv6_opt_hdr *exthdr = (struct ipv6_opt_hdr*)(skb->nh.ipv6h + 1);
	unsigned int packet_len = skb->tail - skb->nh.raw;
	int found_rhdr = 0;

	*nexthdr = &skb->nh.ipv6h->nexthdr;

	while (offset + 1 <= packet_len) {

		switch (**nexthdr) {
		case NEXTHDR_HOP:
			break;
		case NEXTHDR_ROUTING:
			found_rhdr = 1;
			break;
		case NEXTHDR_DEST:
			/*
			 * HAO MUST NOT appear more than once.
			 * XXX: It is better to try to find by the end of
			 * XXX: packet if HAO exists.
			 */
			if (ipv6_find_tlv(skb, offset, IPV6_TLV_HAO) >= 0) {
				LIMIT_NETDEBUG(
					printk(KERN_WARNING "mip6: hao exists already, override\n"));
				return offset;
			}

			if (found_rhdr)
				return offset;

			break;
		case NEXTHDR_FRAGMENT:
		case NEXTHDR_ESP:
		case NEXTHDR_AUTH:
		default:
			return offset;
		}

		offset += ipv6_optlen(exthdr);
		*nexthdr = &exthdr->nexthdr;
		exthdr = (struct ipv6_opt_hdr*)(skb->nh.raw + offset);
	}

#ifdef CONFIG_IPV6_MIP6_DEBUG
	printk(KERN_DEBUG "%s: nothing\n", __FUNCTION__);
#endif
	return offset;
}

/* Destination Option Header is inserted.
 * IP Header's src address is replaced with Home Address Option in
 * Destination Option Header.
 */
static int mip6_destopt_output(struct sk_buff *pskb)
{
	struct dst_entry *dst = pskb->dst;
	struct xfrm_state *x = dst->xfrm;
	struct ipv6hdr *iph;
	struct ipv6_destopt_hdr *dstopt;
	struct destopt_hao *hao;
	u8 nexthdr;
	int len;

	iph = (struct ipv6hdr *)pskb->data;
	iph->payload_len = htons(pskb->len - sizeof(*iph));

	nexthdr = *pskb->nh.raw;
	*pskb->nh.raw = IPPROTO_DSTOPTS;

	dstopt = (struct ipv6_destopt_hdr *)pskb->h.raw;
	dstopt->nexthdr = nexthdr;

	hao = mip6_padn((char *)(dstopt + 1),
			calc_padlen(sizeof(*dstopt), 6));

	hao->type = IPV6_TLV_HAO;
	hao->length = sizeof(*hao) - 2;
	BUG_TRAP(hao->length == 16);

	len = ((char *)hao - (char *)dstopt) + sizeof(*hao);

	/* Replacing address */
	memcpy(&hao->addr, &iph->saddr, sizeof(hao->addr));
	memcpy(&iph->saddr, x->coaddr, sizeof(iph->saddr));

	BUG_TRAP(len == x->props.header_len);
	dstopt->hdrlen = (x->props.header_len >> 3) - 1;

	return 0;
}

static int mip6_destopt_init_state(struct xfrm_state *x, void *args)
{
	u32 spi;

	if (x->id.spi) {
		printk(KERN_INFO "%s: spi is not 0: %u\n", __FUNCTION__,
		       x->id.spi);
		return -EINVAL;
	}
	if (x->props.mode != 0) {
		printk(KERN_INFO "%s: state's mode is not 0: %d\n",
		       __FUNCTION__, x->props.mode);
		return -EINVAL;
	}

	spi = xfrm6_tunnel_alloc_spi((xfrm_address_t *)&x->props.saddr);
	if (spi == 0) {
		printk(KERN_INFO "%s: failed to alloc spi\n", __FUNCTION__);
		return -EINVAL;
	}
	x->id.spi = spi;

	x->props.header_len = 24; /* XXX: */

	return 0;
}

static void mip6_destopt_destroy(struct xfrm_state *x)
{
	if (x->id.spi != 0) {
		xfrm6_tunnel_free_spi((xfrm_address_t *)&x->props.saddr);
		x->id.spi = 0;
	}
}

/*****************************
 * Routing Header type 2
 *****************************/

static int mip6_rthdr_input(struct xfrm_state *x, struct xfrm_decap_state *decap, struct sk_buff *skb)
{
	struct rt2_hdr *rt2 = (struct rt2_hdr *)skb->h.raw;

	/* Verifying care-of address */
	if (ipv6_addr_cmp(&rt2->addr, (struct in6_addr *)x->coaddr) &&
	    !ipv6_addr_any((struct in6_addr *)x->coaddr)) {
 		LIMIT_NETDEBUG(
 			printk(KERN_DEBUG "rt2 mismatch care-of address:"
 			       " input = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
 			       " kernel = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
 			       " hoa = %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n",
 			       NIP6(rt2->addr),
 			       NIP6(*(struct in6_addr *)x->coaddr),
 			       NIP6(skb->nh.ipv6h->daddr)));
		return -ENOENT;
	}

	return rt2->rt_hdr.nexthdr;
}

int mip6_rthdr_place_find(struct sk_buff *skb, u8 **nexthdr)
{
	u16 offset = sizeof(struct ipv6hdr);
	struct ipv6_opt_hdr *exthdr = (struct ipv6_opt_hdr*)(skb->nh.ipv6h + 1);
	unsigned int packet_len = skb->tail - skb->nh.raw;
	int found_rhdr = 0;

	*nexthdr = &skb->nh.ipv6h->nexthdr;

	while (offset + 1 <= packet_len) {

		switch (**nexthdr) {

		case NEXTHDR_HOP:
			break;
		case NEXTHDR_ROUTING:
			if (offset + 3 <= packet_len) {
				struct ipv6_rt_hdr *rt;
				rt = (struct ipv6_rt_hdr *)(skb->nh.raw + offset);
				if (rt->type != 0)
					return offset;
			}
			found_rhdr = 1;
			break;
		case NEXTHDR_DEST:
			if (ipv6_find_tlv(skb, offset, IPV6_TLV_HAO) >= 0)
				return offset;

			if (found_rhdr)
				return offset;

			break;
		default:
			return offset;
		}

		offset += ipv6_optlen(exthdr);
		*nexthdr = &exthdr->nexthdr;
		exthdr = (struct ipv6_opt_hdr*)(skb->nh.raw + offset);
	}

	MIP6_DBG("%s: nothing\n", __FUNCTION__);

	return offset;
}

/* Routing Header type 2 is inserted.
 * IP Header's dst address is replaced with Routing Header's Home Address.
 */
static int mip6_rthdr_output(struct sk_buff *pskb)
{
	struct dst_entry *dst = pskb->dst;
	struct xfrm_state *x = dst->xfrm;
	struct ipv6hdr *iph;
	struct rt2_hdr *rt2;
	u8 nexthdr;

	iph = (struct ipv6hdr *)pskb->data;
	iph->payload_len = htons(pskb->len - sizeof(*iph));

	nexthdr = *pskb->nh.raw;
	*pskb->nh.raw = IPPROTO_ROUTING;

	rt2 = (struct rt2_hdr *)pskb->h.raw;
	rt2->rt_hdr.nexthdr = nexthdr;
	rt2->rt_hdr.hdrlen = (x->props.header_len >> 3) - 1;
	rt2->rt_hdr.type = IPV6_SRCRT_TYPE_2;
	rt2->rt_hdr.segments_left = 1;
	memset(&rt2->reserved, 0, sizeof(rt2->reserved));

	BUG_TRAP(rt2->rt_hdr.hdrlen == 2);

	/* Replacing address */
	memcpy(&rt2->addr, &iph->daddr, sizeof(rt2->addr));
	memcpy(&iph->daddr, x->coaddr, sizeof(iph->daddr));

	return 0;
}

static int mip6_rthdr_init_state(struct xfrm_state *x, void *args)
{
	u32 spi;

	if (x->id.spi) {
		printk(KERN_INFO "%s: spi is not 0: %u\n", __FUNCTION__,
		       x->id.spi);
		return -EINVAL;
	}
	if (x->props.mode != 0) {
		printk(KERN_INFO "%s: state's mode is not 0: %d\n",
		       __FUNCTION__, x->props.mode);
		return -EINVAL;
	}

	spi = xfrm6_tunnel_alloc_spi((xfrm_address_t *)&x->props.saddr);
	if (spi == 0) {
		printk(KERN_INFO "%s: failed to alloc spi\n", __FUNCTION__);
		return -EINVAL;
	}
	x->id.spi = spi;

	x->props.header_len = sizeof(struct rt2_hdr);

	return 0;
}

static void mip6_rthdr_destroy(struct xfrm_state *x)
{
	if (x->id.spi != 0) {
		xfrm6_tunnel_free_spi((xfrm_address_t *)&x->props.saddr);
		x->id.spi = 0;
	}
}

/*****************************
 * Module part
 *****************************/
static struct xfrm_type mip6_destopt_type =
{
	.description	= "MIP6",
	.owner		= THIS_MODULE,
	.proto	     	= IPPROTO_DSTOPTS,
	.init_state	= mip6_destopt_init_state,
	.destructor	= mip6_destopt_destroy,
	.input		= mip6_destopt_input,
	.post_input	= NULL,
	.post_reject    = mip6_destopt_post_reject,
	.output		= mip6_destopt_output
};

static struct xfrm_type mip6_rthdr_type =
{
	.description	= "MIP6",
	.owner		= THIS_MODULE,
	.proto	     	= IPPROTO_ROUTING,
	.init_state	= mip6_rthdr_init_state,
	.destructor	= mip6_rthdr_destroy,
	.input		= mip6_rthdr_input,
	.post_input	= NULL,
	.output		= mip6_rthdr_output
};

int __init mip6_init(void)
{
	printk(KERN_INFO "Mobile IPv6\n");

	if (xfrm_register_type(&mip6_destopt_type, AF_INET6) < 0) {
		printk(KERN_INFO "%s: can't add xfrm type(destopt)\n", __FUNCTION__);
		goto mip6_destopt_xfrm_fail;
	}
	if (xfrm_register_type(&mip6_rthdr_type, AF_INET6) < 0) {
		printk(KERN_INFO "%s: can't add xfrm type(rthdr)\n", __FUNCTION__);
		goto mip6_rthdr_xfrm_fail;
	}
	return 0;

 mip6_rthdr_xfrm_fail:
	xfrm_unregister_type(&mip6_destopt_type, AF_INET6);
 mip6_destopt_xfrm_fail:
	return -EAGAIN;
}

void __exit mip6_fini(void)
{
	if (xfrm_unregister_type(&mip6_rthdr_type, AF_INET6) < 0)
		printk(KERN_INFO "%s: can't remove xfrm type(rthdr)\n", __FUNCTION__);
	if (xfrm_unregister_type(&mip6_destopt_type, AF_INET6) < 0)
		printk(KERN_INFO "%s: can't remove xfrm type(destopt)\n", __FUNCTION__);
}
