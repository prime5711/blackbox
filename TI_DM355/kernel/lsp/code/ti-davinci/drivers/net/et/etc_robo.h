/*
 * BCM53xx RoboSwitch utility functions
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

#ifndef _robo_h_
#define _robo_h_

extern void *robo_attach(void *sbh, uint32 ssl, uint32 clk, uint32 mosi,
			 uint32 miso);
extern void robo_detach(void *robo);
extern void robo_rreg(void *robo, uint8 cid, uint8 page, uint8 addr,
		      uint8 * buf, uint len);
extern void robo_wreg(void *robo, uint8 cid, uint8 page, uint8 addr,
		      uint8 * buf, uint len);
extern void robo_rvmii(void *robo, uint8 cid);

#endif				/* _robo_h_ */
