/*
 * include/asm-xtensa/sigcontext.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2003 Tensilica Inc.
 */

#ifndef _XTENSA_SIGCONTEXT_H
#define _XTENSA_SIGCONTEXT_H

#include <asm/cpu/config.h>
#include <asm/coprocessor.h>

struct _cpstate {
	unsigned char _cpstate[XTENSA_CP_EXTRA_SIZE];
} __attribute__ ((aligned (XTENSA_CP_EXTRA_ALIGN)));


struct sigcontext {
	unsigned long	oldmask;

	/* CPU registers */

	unsigned long sc_pc;
	unsigned long sc_ps;
	unsigned long sc_lbeg;
	unsigned long sc_lend;
	unsigned long sc_lcount;
	unsigned long sc_sar;
	unsigned long sc_acclo;
	unsigned long sc_acchi;
	unsigned long sc_a[16];
};

#endif /* _XTENSA_SIGCONTEXT_H */