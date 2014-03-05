/*
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
 * ST M48TXX, Dallas/Maxim DS17xx Real time clock Driver
 *
 * The M48T59 and DS1743 RTC's support an 8K NVRAM starting at offset
 * DS1743_BASE_ADDR and continuing for 8184 bytes; beyond that lies
 * the Time-Of-Day (TOD) registers which are used to set and get the
 * internal timekeeping functions.
 *
 * This module implements Y2K compliance by taking full year numbers
 * and translating back and forth from the TOD 2-digit year.
 *
 * NOTE: for proper interaction with an operating system, the TOD should
 * be used to store Universal Coordinated Time (GMT) and timezone
 * conversions should be used.
 *
 * Here is a diagram of the memory layout:
 *
 * +---------------------------------------------+ BASE
 * | Non-volatile memory                         | .
 * |                                             | .
 * | (8184 bytes of Non-volatile memory)         | .
 * |                                             | .
 * +---------------------------------------------+ BASE + 8K - 8
 * | Control                                     |
 * +---------------------------------------------+ BASE + 8K - 7
 * | Seconds                                     |
 * +---------------------------------------------+ BASE + 8K - 6
 * | Minutes                                     |
 * +---------------------------------------------+ BASE + 8K - 5
 * | Hours                                       |
 * +---------------------------------------------+ BASE + 8k - 4
 * | Day                                         |
 * +---------------------------------------------+ BASE + 8K - 3
 * | Date                                        |
 * +---------------------------------------------+ BASE + 8K - 2
 * | Month                                       |
 * +---------------------------------------------+ BASE + 8K - 1
 * | Year                                        |
 * +---------------------------------------------+
 *
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/rtc.h>

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/time.h>
#include <asm/mipsregs.h>
#include <asm/gdb-stub.h>

#include <asm/brcm/bcm4710.h>

#define YEAR		(0x7)
#define MONTH		(0x6)
#define DAY		(0x5)
#define DAY_OF_WEEK	(0x4)
#define HOUR		(0x3)
#define MINUTE		(0x2)
#define SECOND		(0x1)
#define CONTROL 	(0x0)

/*
 * Protection: if PLD for EXTIF decode doesn't support write lockout,
 * then these are null macros.
 */
#define SYS_TOD_UNPROTECT(x)
#define SYS_TOD_PROTECT(x)

static unsigned char *rtc17xxBaseAddr;

#if defined(CONFIG_BCM94704_CPCI) && !defined(CONFIG_CPU_LITTLE_ENDIAN)

static unsigned char be_map[8] = { 3, 2, 1, 0, 7, 6, 5, 4 };

#define RTC17XX_ADDR(off) \
	(((volatile unsigned char *)(rtc17xxBaseAddr + 0x1ff8))[be_map[off]])

#else

/* NOTE: On M48T59 RTC chips, we don't use the alarm feature */
#define RTC17XX_ADDR(off) \
	(((volatile unsigned char *)(rtc17xxBaseAddr + 0x1ff8))[off])

#endif

/*
 * rtc17xx_tod_set
 */

static int to_bcd(int value)
{
	return value / 10 * 16 + value % 10;
}

static int from_bcd(int value)
{
	return value / 16 * 10 + value % 16;
}

static int day_of_week(int y, int m, int d)
{				/* 0-6 ==> Sun-Sat */
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

int rtc17xx_tod_set(int year,	/* 1980-2079 */
		    int month,	/* 01-12 */
		    int day,	/* 01-31 */
		    int hour,	/* 00-23 */
		    int minute,	/* 00-59 */
		    int second)
{				/* 00-59 */
	SYS_TOD_UNPROTECT();
	RTC17XX_ADDR(CONTROL) |= 0x80;	/* Set WRITE bit */

	RTC17XX_ADDR(YEAR) = to_bcd(year % 100);
	RTC17XX_ADDR(MONTH) = to_bcd(month);
	RTC17XX_ADDR(DAY) = to_bcd(day);
	RTC17XX_ADDR(DAY_OF_WEEK) = day_of_week(year, month, day) + 1;
	RTC17XX_ADDR(HOUR) = to_bcd(hour);
	RTC17XX_ADDR(MINUTE) = to_bcd(minute);
	RTC17XX_ADDR(SECOND) = to_bcd(second);

	RTC17XX_ADDR(CONTROL) &= ~0x80;	/* Clear WRITE bit */
	SYS_TOD_PROTECT();
	return 0;
}

int rtc17xx_tod_get(int *year,	/* 1980-2079 */
		    int *month,	/* 01-12 */
		    int *day,	/* 01-31 */
		    int *hour,	/* 00-23 */
		    int *minute,	/* 00-59 */
		    int *second)
{				/* 00-59 */
	int y;

	SYS_TOD_UNPROTECT();
	RTC17XX_ADDR(CONTROL) |= 0x40;	/* Set READ bit */
	SYS_TOD_PROTECT();

	y = from_bcd(RTC17XX_ADDR(YEAR));
	*year = y < 80 ? 2000 + y : 1900 + y;
	*month = from_bcd(RTC17XX_ADDR(MONTH));
	*day = from_bcd(RTC17XX_ADDR(DAY) & 0x3f);
	/*
	 * Note: MSB of day-of-week reg contains battery-life bit.  It
	 * should always be '1'.  If it's zero, either the battery is dead
	 * or we're not really reading the chip.
	 */
	/* day_of_week = RTC17XX_ADDR(DAY_OF_WEEK) & 0xf; */
	*hour = from_bcd(RTC17XX_ADDR(HOUR));
	*minute = from_bcd(RTC17XX_ADDR(MINUTE));
	*second = from_bcd(RTC17XX_ADDR(SECOND) & 0x7f);

	RTC17XX_ADDR(CONTROL) &= ~0x40;	/* Clear READ bit */

	return 0;
}

int rtc17xx_tod_print(void)
{
	int year, month, day, hour, minute, second;
	rtc17xx_tod_get(&year,	/* 1980-2079 */
			&month,	/* 01-12 */
			&day,	/* 01-31 */
			&hour,	/* 00-23 */
			&minute,	/* 00-59 */
			&second);	/* 00-59 */

	printk(KERN_NOTICE "RTC: %d/%d/%d, %d:%.2d:%.2d\n",
	       month, day, year, hour, minute, second);
	return 0;
}

int rtc17xx_tod_get_second(void)
{
	return from_bcd(RTC17XX_ADDR(SECOND) & 0x7f);
}

unsigned long rtc17xx_get_time(void)
{
	unsigned int year, mon, day, hour, min, sec;

	rtc17xx_tod_get(&year,	/* 1980-2079 */
			&mon,	/* 01-12 */
			&day,	/* 01-31 */
			&hour,	/* 00-23 */
			&min,	/* 00-59 */
			&sec);	/* 00-59 */

	return mktime(year, mon, day, hour, min, sec);
}

int rtc17xx_set_time(unsigned long sec)
{
	struct rtc_time tm;
	to_tm(sec, &tm);

	rtc17xx_tod_set(tm.tm_year,	/* 1980-2079 */
			tm.tm_mon + 1,	/* 01-12 */
			tm.tm_mday,	/* 01-31 */
			tm.tm_hour,	/* 00-23 */
			tm.tm_min,	/* 00-59 */
			tm.tm_sec);	/* 00-59 */

	return 0;
}

int rtc17xx_tod_init(void)
{
#ifdef CONFIG_BCM5XXX_FPGA
	rtc17xxBaseAddr = ioremap_nocache(0x1b240000, 0x1fff);
#elif defined(CONFIG_BCM94702_CPCI)
	rtc17xxBaseAddr =
	    ioremap_nocache(BCM94702_CPCI_NVRAM_ADDR, 0x1fff);
#elif defined(CONFIG_BCM94704_CPCI)
	/* rtc17xxBaseAddr = ioremap_nocache(BCM94704_CPCI_NVRAM_ADDR, 0x2000); */
	rtc17xxBaseAddr = (unsigned char *) BCM94704_CPCI_NVRAM_ADDR;
#endif

	SYS_TOD_UNPROTECT();
	RTC17XX_ADDR(CONTROL) = 0;
	SYS_TOD_PROTECT();

	/* Start the clock if the oscillator is not running */
	if (RTC17XX_ADDR(SECOND) & 0x80)
		return rtc17xx_tod_set(2000, 1, 1, 0, 0, 0);
	else
		return 0;
}
