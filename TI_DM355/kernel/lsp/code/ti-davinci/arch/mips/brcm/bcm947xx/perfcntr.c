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
 */

#include <asm/mipsregs.h>

/*
 * BCM4710 performance counter register select values
 * No even-odd control-counter mapping, just counters
 */
#define PERF_DCACHE_HIT 	0
#define PERF_DCACHE_MISS	1
#define PERF_ICACHE_HIT 	2
#define PERF_ICACHE_MISS	3
#define PERF_ICOUNT		4

/*
 * Move from Coprocessor 0 Register 25 Select n
 * data <- CPR[0,25,n]
 * GPR[1] <- data
 */
#define read_bcm4710_perf_cntr(n)				\
({ int __res;							\
        __asm__ __volatile__(					\
	".set\tnoreorder\n\t"                                   \
	".set\tnoat\n\t"                                        \
	".word\t"STR(0x4001c800|(n))"\n\t"			\
	"move\t%0,$1\n\t"                                       \
	".set\tat\n\t"                                          \
	".set\treorder"                                         \
	:"=r" (__res));                                         \
	__res;})

asmlinkage unsigned int read_perf_cntr(unsigned int counter)
{
	switch (counter) {
	case PERF_DCACHE_HIT:
		return read_bcm4710_perf_cntr(PERF_DCACHE_HIT);
	case PERF_DCACHE_MISS:
		return read_bcm4710_perf_cntr(PERF_DCACHE_MISS);
	case PERF_ICACHE_HIT:
		return read_bcm4710_perf_cntr(PERF_ICACHE_HIT);
	case PERF_ICACHE_MISS:
		return read_bcm4710_perf_cntr(PERF_ICACHE_MISS);
	case PERF_ICOUNT:
		return read_bcm4710_perf_cntr(PERF_ICOUNT);
	}
	return 0;
}

asmlinkage void write_perf_cntr(unsigned int counter, unsigned int val)
{
}

asmlinkage unsigned int read_perf_cntl(unsigned int counter)
{
	return 0;
}

asmlinkage void write_perf_cntl(unsigned int counter, unsigned int val)
{
}
