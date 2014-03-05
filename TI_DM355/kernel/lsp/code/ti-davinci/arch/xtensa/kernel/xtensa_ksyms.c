/*
 * arch/xtensa/kernel/xtensa_ksyms.c
 *
 * Export Xtensa-specific functions for loadable modules.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005  Tensilica Inc.
 *
 * Joe Taylor <joe@tensilica.com>
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/in6.h>
#include <linux/pci.h>
#include <linux/ide.h>

#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/semaphore.h>
#ifdef CONFIG_BLK_DEV_FD
#include <asm/floppy.h>
#endif
#ifdef CONFIG_NET
#include <net/checksum.h>
#endif /* CONFIG_NET */


/*
 * String functions
 */
EXPORT_SYMBOL(memcmp);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(memchr);
EXPORT_SYMBOL(strcat);
EXPORT_SYMBOL(strchr);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strpbrk);
EXPORT_SYMBOL(strncat);
EXPORT_SYMBOL(strnlen);
EXPORT_SYMBOL(strrchr);
EXPORT_SYMBOL(strstr);

EXPORT_SYMBOL(enable_irq);
EXPORT_SYMBOL(disable_irq);
EXPORT_SYMBOL(kernel_thread);

/*
 * gcc internal math functions
 */
extern long long __ashrdi3(long long, int);
extern long long __ashldi3(long long, int);
extern long long __lshrdi3(long long, int);
extern int __divsi3(int, int);
extern int __modsi3(int, int);
extern long long __muldi3(long long, long long);
extern int __mulsi3(int, int);
extern unsigned int __udivsi3(unsigned int, unsigned int);
extern unsigned int __umodsi3(unsigned int, unsigned int);
extern unsigned long long __umoddi3(unsigned long long, unsigned long long);
extern unsigned long long __udivdi3(unsigned long long, unsigned long long);

EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__modsi3);
EXPORT_SYMBOL(__muldi3);
EXPORT_SYMBOL(__mulsi3);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(__umodsi3);
EXPORT_SYMBOL(__udivdi3);
EXPORT_SYMBOL(__umoddi3);

/*
 * Semaphore operations
 */
EXPORT_SYMBOL(__compat_down);
EXPORT_SYMBOL(__compat_down_interruptible);
EXPORT_SYMBOL(__compat_down_trylock);
EXPORT_SYMBOL(__compat_up);

#ifdef CONFIG_NET
/*
 * Networking support
 */
EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_generic);
#endif /* CONFIG_NET */

/*
 * Architecture-specific symbols
 */
EXPORT_SYMBOL(__xtensa_copy_user);

/*
 * Kernel hacking ...
 */

#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_DUMMY_CONSOLE)
// FIXME EXPORT_SYMBOL(screen_info);
#endif

EXPORT_SYMBOL(get_wchan);

EXPORT_SYMBOL(outsb);
EXPORT_SYMBOL(outsw);
EXPORT_SYMBOL(outsl);
EXPORT_SYMBOL(insb);
EXPORT_SYMBOL(insw);
EXPORT_SYMBOL(insl);