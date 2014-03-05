/*
 * timer functions declarations
 *
 * Copyright (C) 2005  MontaVista Inc.
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
#ifndef _bcmtime_h_
#define _bcmtime_h_

extern void bcm947xx_time_init(void);
extern void bcm947xx_timer_setup(struct irqaction *irq);
extern void (*board_timer_setup) (struct irqaction * irq);

#if defined(CONFIG_BCM94702_CPCI) || defined(CONFIG_BCM94704_CPCI)
extern int rtc17xx_tod_init(void);
extern int rtc17xx_tod_print(void);
extern unsigned long rtc17xx_get_time(void);
extern int rtc17xx_set_time(unsigned long sec);
#endif

#endif
