/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 MontaVista Software Inc.
 * Author:  source@mvista.com
 */
#ifndef __ASM_MACH_BCM947XX_MANGLE_PORT_H
#define __ASM_MACH_BCM947XX_MANGLE_PORT_H
#ifndef CONFIG_CPU_LITTLE_ENDIAN
#define __swizzle_addr_b(port)	((port) ^ 3)
#else
#define __swizzle_addr_b(port)	(port)
#endif
#define __swizzle_addr_w(port)	(port)
#define __swizzle_addr_l(port)	(port)
#define __swizzle_addr_q(port)	(port)

#endif /* __ASM_MACH_BCM947XX_MANGLE_PORT_H */
