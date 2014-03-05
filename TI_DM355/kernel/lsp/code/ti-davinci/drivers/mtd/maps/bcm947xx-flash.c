/*
 * Flash mapping for BCM947XX boards
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. ( 2.6 kernel modification )
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

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/config.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmnvram.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/sbconfig.h>
#include <asm/brcm/sbchipc.h>
#include <asm/brcm/sbutils.h>

/* Global SB handle */
extern void *bcm947xx_sbh;
extern spinlock_t bcm947xx_sbh_lock;

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock

#define WINDOW_ADDR 0x1fc00000
#define WINDOW_SIZE 0x400000
#define BUSWIDTH 2

static struct mtd_info *bcm947xx_mtd;

static void bcm947xx_map_write(struct map_info *map, const map_word datum,
			       unsigned long offs)
{
	if (map_bankwidth_is_1(map))
		writeb(datum.x[0], (void *) (map->map_priv_1 + offs));
	else if (map_bankwidth_is_2(map))
		writew(datum.x[0], (void *) (map->map_priv_1 + offs));
	else if (map_bankwidth_is_4(map))
		writel(datum.x[0], (void *) (map->map_priv_1 + offs));
	else if (map_bankwidth_is_large(map))
		memcpy_toio(map->map_priv_1 + offs, (void *) datum.x[0],
			    map->bankwidth);
	mb();
}

static map_word bcm947xx_map_read(struct map_info *map, unsigned long offs)
{
	map_word r;

	if (map_bankwidth_is_1(map)) {
		u16 val;
		if (map->map_priv_2 == 1)
			r.x[0] = readb((void *) (map->map_priv_1 + offs));
		else {
			val =
			    readw((void *) (map->map_priv_1 +
					    (offs & ~1)));
			if (offs & 1)
				r.x[0] = ((val >> 8) & 0xff);
			else
				r.x[0] = (val & 0xff);
		}
	} else if (map_bankwidth_is_2(map))
		r.x[0] = readw((void *) (map->map_priv_1 + offs));
	else if (map_bankwidth_is_4(map))
		r.x[0] = readl((void *) (map->map_priv_1 + offs));
	else if (map_bankwidth_is_large(map))
		memcpy_fromio(r.x, map->map_priv_1 + offs, map->bankwidth);
	return r;
}

static void bcm947xx_map_copy_from(struct map_info *map, void *to,
				   unsigned long from, ssize_t len)
{
	from += map->map_priv_1;
	while (len--)
		*(char *) to++ = readb((void *) (from++));
}

static void bcm947xx_map_copy_to(struct map_info *map, unsigned long to,
				 const void *from, ssize_t len)
{
	to += map->map_priv_1;
	while (len--)
		writeb(*(char *) from++, (void *) (to++));
}

static struct map_info bcm947xx_map = {
	.name = "Physically mapped flash",
	.size = WINDOW_SIZE,
	.bankwidth = BUSWIDTH,
	.read = bcm947xx_map_read,
	.write = bcm947xx_map_write,
	.copy_from = bcm947xx_map_copy_from,
	.copy_to = bcm947xx_map_copy_to
};

#ifdef CONFIG_MTD_PARTITIONS

/*
 * CFE 1.1.0 flash map:
 *
 * flash2.boot   offset 00000000 size 256KB
 * flash2.trx    offset 00080000 size 3552KB
 * flash2.nvram  offset 003F8000 size 32KB
 */

static struct mtd_partition bcm947xx_mtd_parts[] __initdata = {
	{
	 .name = "cfe",
	 .size = 0x080000,
	 .offset = 0x000000,
	 .mask_flags = MTD_WRITEABLE}, {
					.name = "User FS",
					.size = 0x360000,
					.offset = 0x080000}, {
							      .name =
							      "nvram",
							      .size =
							      0x020000,
							      .offset =
							      0x3e0000}
};

#define NUM_PARTS (sizeof(bcm947xx_mtd_parts)/sizeof(struct mtd_partition))
#endif

#if LINUX_VERSION_CODE < 0x20212 && defined(MODULE)
#define init_bcm947xx_map init_module
#define cleanup_bcm947xx_map cleanup_module
#endif

int __init init_bcm947xx_map(void)
{
	ulong flags;
	uint coreidx;
	chipcregs_t *cc;
	uint32 fltype;
	uint window_addr = 0, window_size = 0;
	int ret = 0;

	spin_lock_irqsave(&sbh_lock, flags);
	coreidx = sb_coreidx(sbh);

	/* Check strapping option if Chipcommon exists */
	if ((cc = sb_setcore(sbh, SB_CC, 0))) {
		fltype =
		    readl((void *) (&cc->capabilities)) & CAP_FLASH_MASK;
		if (fltype == PFLASH) {
			bcm947xx_map.map_priv_2 = 1;
			window_addr = 0x1b000000;
			bcm947xx_map.size = window_size = 4 * 1024 * 1024;
			if ((readl((void *) (&cc->parallelflashconfig)) &
			     CC_CFG_DS) == 0)
				bcm947xx_map.bankwidth = 1;
		}
	} else {
		fltype = PFLASH;
		bcm947xx_map.map_priv_2 = 0;
		window_addr = WINDOW_ADDR;
		window_size = WINDOW_SIZE;
	}

	sb_setcoreidx(sbh, coreidx);
	spin_unlock_irqrestore(&sbh_lock, flags);

	if (fltype != PFLASH) {
		printk(KERN_ERR "pflash: found no supported devices\n");
		ret = -ENODEV;
		goto fail;
	}

	bcm947xx_map.map_priv_1 =
	    (unsigned long) ioremap(window_addr, window_size);
	if (!bcm947xx_map.map_priv_1) {
		printk(KERN_ERR "pflash: ioremap failed\n");
		ret = -EIO;
		goto fail;
	}

	if (!(bcm947xx_mtd = do_map_probe("cfi_probe", &bcm947xx_map))) {
		printk(KERN_ERR "pflash: cfi_probe failed\n");
		ret = -ENXIO;
		goto fail;
	}

	bcm947xx_mtd->owner = THIS_MODULE;

	printk(KERN_NOTICE "Flash device: 0x%x at 0x%x\n",
	       bcm947xx_mtd->size, window_addr);

#ifdef CONFIG_MTD_PARTITIONS
	ret =
	    add_mtd_partitions(bcm947xx_mtd, bcm947xx_mtd_parts,
			       NUM_PARTS);
	if (ret) {
		printk(KERN_ERR "pflash: add_mtd_partitions failed\n");
		goto fail;
	}
#endif

	return 0;

      fail:
	if (bcm947xx_mtd)
		map_destroy(bcm947xx_mtd);
	if (bcm947xx_map.map_priv_1)
		iounmap((void *) bcm947xx_map.map_priv_1);
	bcm947xx_map.map_priv_1 = 0;
	return ret;
}

static void __exit cleanup_bcm947xx_map(void)
{
#ifdef CONFIG_MTD_PARTITIONS
	del_mtd_partitions(bcm947xx_mtd);
#endif
	map_destroy(bcm947xx_mtd);
	iounmap((void *) bcm947xx_map.map_priv_1);
	bcm947xx_map.map_priv_1 = 0;
}

module_init(init_bcm947xx_map);
module_exit(cleanup_bcm947xx_map);
