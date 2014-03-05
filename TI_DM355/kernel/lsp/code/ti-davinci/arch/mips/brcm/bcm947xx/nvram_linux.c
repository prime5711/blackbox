/*
 * NVRAM variable manipulation (Linux kernel half)
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. (Modifocations for 2.6 kernel).
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

#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/bootmem.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mtd/mtd.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmendian.h>
#include <asm/brcm/bcmnvram.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/sbconfig.h>
#include <asm/brcm/sbchipc.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/sbmips.h>
#include <asm/brcm/sflash.h>

/* In BSS to minimize text size and page aligned so it can be mmap()-ed */
static char nvram_buf[NVRAM_SPACE] __attribute__ ((aligned(PAGE_SIZE)));

#ifdef MODULE

#define early_nvram_get(name) nvram_get(name)

#else				/* !MODULE */

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock
#define KB * 1024
#define MB * 1024 * 1024

/* Probe for NVRAM header */
static void __init early_nvram_init(void)
{
	struct nvram_header *header;
	chipcregs_t *cc;
	struct sflash *info = NULL;
	int i, bytes, len;
	long offset = -1;

	if ((cc = sb_setcore(sbh, SB_CC, 0)) != NULL) {
		switch (readl((void *) (&cc->capabilities)) &
			CAP_FLASH_MASK) {
		case PFLASH:
			/* Should figure out if it is really 4MB or not! */
			offset = 4 MB - NVRAM_SPACE;
			break;

		case SFLASH_ST:
		case SFLASH_AT:
			if ((info = sflash_init(cc)) == NULL)
				return;
			/* Start at end of serial flash */
			offset = info->size - NVRAM_SPACE;
			break;

		case FLASH_NONE:
		default:
			return;
		}
	} else {
		/* extif assumed, Start at 4 MB */
		offset = 4 MB - NVRAM_SPACE;
	}

	while (offset >= 0) {
		/* Windowed flash access */
		if (!info) {
			/* There are two places one might access the
			 * NVRAM area: via the boot ROM address space
			 * (0x1fc00000) and via the extif interface
			 * (0x1b000000).
			 *
			 * On the BCM94704, the NVRAM variables are
			 * always stored in the Intel flash, which is
			 * always accessible via the extif interface,
			 * and is accessible via the boot ROM area
			 * when JP1001 is jumpered.
			 *
			 * The default value for CONFIG_BCM947XX_NVRAM_BASE
			 * (0x1b000000) is the extif address of the
			 * Intel flash on BCM94704 boards, which makes
			 * the NVRAM available irrespective of the
			 * setting of JP1001.
			 *
			 * (If the boot flash address 0x1fc00000 is used
			 * here, the Intel flash is NOT accessed when
			 * JP1001 is not jumpered, and in that case we
			 * can't find our environment!)
			 */
			header = (struct nvram_header *)
			    KSEG1ADDR(CONFIG_BCM947XX_NVRAM_BASE + offset);

			if (header->magic == NVRAM_MAGIC) {
#ifdef CONFIG_CPU_LITTLE_ENDIAN
				memcpy(nvram_buf, header, NVRAM_SPACE);
#else
				u32 *src = (u32 *) header;
				u32 *dst = (u32 *) nvram_buf;

				for (i = 0;
				     i < sizeof(struct nvram_header);
				     i += 4) {
					*dst++ = *src++;
				}

				for (; i < header->len && i < NVRAM_SPACE;
				     i += 4) {
					*dst++ = ltoh32(*src++);
				}
#endif
				return;
			}
		}

		/* Serial flash access */
		else {
			header = (struct nvram_header *) nvram_buf;
			/* Load header */
			i = 0;
			len = sizeof(struct nvram_header);
			while (i < sizeof(struct nvram_header)) {
				if ((bytes =
				     sflash_read(cc, offset + i, len - i,
						 &nvram_buf[i])) < 0)
					break;
				i += bytes;
			}
			/* Load the rest */
			if (header->magic == NVRAM_MAGIC) {
				i = sizeof(struct nvram_header);
				len = MIN(header->len, NVRAM_SPACE);
				while (i < len) {
					if ((bytes =
					     sflash_read(cc, offset + i,
							 len - i,
							 &nvram_buf[i])) <
					    0)
						break;
					i += bytes;
				}
				return;
			}
		}

		/* Try embedded NVRAM at 4 KB and 1 KB as last resorts */
		if (offset == 1 KB)
			break;
		else if (offset == 4 KB)
			offset = 1 KB;
		else if (offset < 256 KB)
			offset = 4 KB;
		else
			offset -= 256 KB;
	}

#ifdef USE_TOD_CLOCK_SRAM
	printk("reading TOD nvram\n");
	/* current CFE stores in clock nvram */
	{
		header = (struct nvram_header *) 0xba00e800;
		u32 *src = (u32 *) header;
		u32 *dst = (u32 *) nvram_buf;

		for (i = 0; i < sizeof(struct nvram_header); i += 4)
			*dst++ = *src++;

		for (; i < header->len && i < NVRAM_SPACE; i += 4) {
			*dst++ = ltoh32(*src++);
		}
		return;
	}
#endif
}

/* Early (before mm or mtd) read-only access to NVRAM */
static char *__init early_nvram_get(const char *name)
{
	char *var, *value, *end, *eq;

	if (!name)
		return NULL;

	if (!nvram_buf[0])
		early_nvram_init();

	/* Look for name=value and return value */
	var = &nvram_buf[sizeof(struct nvram_header)];
	end = nvram_buf + sizeof(nvram_buf) - 2;
	end[0] = end[1] = '\0';
	for (; *var; var = value + strlen(value) + 1) {
		if (!(eq = strchr(var, '=')))
			break;
		value = eq + 1;
		if ((eq - var) == strlen(name) &&
		    strncmp(var, name, (eq - var)) == 0)
			return value;
	}

	return NULL;
}

#endif				/* !MODULE */

/* Globals */
static spinlock_t nvram_lock = SPIN_LOCK_UNLOCKED;
static struct semaphore nvram_sem;
static unsigned long nvram_offset = 0;
static int nvram_major = -1;
static struct mtd_info *nvram_mtd = NULL;

int _nvram_read(void *buf)
{
	struct nvram_header *header = (struct nvram_header *) buf;
	size_t len;

	if (!nvram_mtd ||
	    MTD_READ(nvram_mtd, nvram_mtd->size - NVRAM_SPACE,
		     NVRAM_SPACE, &len, buf) ||
	    len != NVRAM_SPACE || header->magic != NVRAM_MAGIC) {
		/* Maybe we can recover some data from early initialization */
		memcpy(buf, nvram_buf, NVRAM_SPACE);
	}
#ifndef CONFIG_CPU_LITTLE_ENDIAN
	/* data is written little-endian (but header is not) */
	{
		u32 *dst = (u32 *) buf;
		int i;
		for (i = 0; i < NVRAM_SPACE; i += 4) {
			if (i > 4)
				*dst = ltoh32(*dst);
			dst++;
		}
	}
#endif

	return 0;
}

struct nvram_tuple *_nvram_realloc(struct nvram_tuple *t, const char *name,
				   const char *value)
{
	if ((nvram_offset + strlen(value) + 1) > NVRAM_SPACE)
		return NULL;

	if (!t) {
		if (!
		    (t =
		     kmalloc(sizeof(struct nvram_tuple) + strlen(name) + 1,
			     GFP_ATOMIC)))
			return NULL;

		/* Copy name */
		t->name = (char *) &t[1];
		strcpy(t->name, name);

		t->value = NULL;
	}

	/* Copy value */
	if (!t->value || strcmp(t->value, value)) {
		t->value = &nvram_buf[nvram_offset];
		strcpy(t->value, value);
		nvram_offset += strlen(value) + 1;
	}

	return t;
}

void _nvram_free(struct nvram_tuple *t)
{
	if (!t)
		nvram_offset = 0;
	else
		kfree(t);
}

int nvram_set(const char *name, const char *value)
{
	unsigned long flags;
	int ret;
	struct nvram_header *header;

	spin_lock_irqsave(&nvram_lock, flags);
	if ((ret = _nvram_set(name, value))) {
		/* Consolidate space and try again */
		if ((header = kmalloc(NVRAM_SPACE, GFP_ATOMIC))) {
			if (_nvram_commit(header) == 0)
				ret = _nvram_set(name, value);
			kfree(header);
		}
	}
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

char *real_nvram_get(const char *name)
{
	unsigned long flags;
	char *value;

	spin_lock_irqsave(&nvram_lock, flags);
	value = _nvram_get(name);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return value;
}

char *nvram_get(const char *name)
{
	if (nvram_major >= 0)
		return real_nvram_get(name);
	else
		return early_nvram_get(name);
}

int nvram_unset(const char *name)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_unset(name);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

static void erase_callback(struct erase_info *done)
{
	wait_queue_head_t *wait_q = (wait_queue_head_t *) done->priv;
	wake_up(wait_q);
}

#ifndef CONFIG_CPU_LITTLE_ENDIAN
void swap_nvram_data(struct nvram_header *header, int size)
{
	int i;
	u32 *ptr;

	ptr = (u32 *) header;
	for (i = 0; i < size; i += 4) {
		if (i > 4)
			*ptr = htol32(*ptr);
		ptr++;
	}
}
#endif

int nvram_commit(void)
{
	char *buf;
	size_t erasesize, len;
	unsigned int i;
	int ret, hlen;
	struct nvram_header *header;
	unsigned long flags;
	u_int32_t offset;
	DECLARE_WAITQUEUE(wait, current);
	wait_queue_head_t wait_q;
	struct erase_info erase;

	if (!nvram_mtd) {
		printk("nvram_commit: NVRAM not found\n");
		return -ENODEV;
	}

	if (in_interrupt()) {
		printk("nvram_commit: not committing in interrupt\n");
		return -EINVAL;
	}

	/* Backup sector blocks to be erased */
	erasesize = ROUNDUP(NVRAM_SPACE, nvram_mtd->erasesize);
	if (!(buf = kmalloc(erasesize, GFP_KERNEL))) {
		printk("nvram_commit: out of memory\n");
		return -ENOMEM;
	}

	down(&nvram_sem);

	offset = nvram_mtd->size - erasesize;
	i = erasesize - NVRAM_SPACE;
	ret = MTD_READ(nvram_mtd, offset, i, &len, buf);
	if (ret || len != i) {
		printk("nvram_commit: read error\n");
		ret = -EIO;
		goto done;

	}

	/* Regenerate NVRAM */
	header = (struct nvram_header *) (buf + erasesize - NVRAM_SPACE);
	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_commit(header);
	spin_unlock_irqrestore(&nvram_lock, flags);
	if (ret)
		goto done;

	hlen = header->len;

#ifndef CONFIG_CPU_LITTLE_ENDIAN
	swap_nvram_data(header, NVRAM_SPACE);
#endif

	/* Erase sector blocks */
	init_waitqueue_head(&wait_q);
	for (; offset < nvram_mtd->size - NVRAM_SPACE + hlen;
	     offset += nvram_mtd->erasesize) {
		erase.mtd = nvram_mtd;
		erase.addr = offset;
		erase.len = nvram_mtd->erasesize;
		erase.callback = erase_callback;
		erase.priv = (u_long) & wait_q;

		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&wait_q, &wait);

		/* Unlock sector blocks */
		if (nvram_mtd->unlock)
			nvram_mtd->unlock(nvram_mtd, offset,
					  nvram_mtd->erasesize);

		if ((ret = MTD_ERASE(nvram_mtd, &erase))) {
			set_current_state(TASK_RUNNING);
			remove_wait_queue(&wait_q, &wait);
			printk("nvram_commit: erase error\n");
			goto done;
		}

		/* Wait for erase to finish */
		schedule();
		remove_wait_queue(&wait_q, &wait);
	}

	/* Write partition up to end of data area */
	offset = nvram_mtd->size - erasesize;
	i = erasesize - NVRAM_SPACE + hlen;
	ret = MTD_WRITE(nvram_mtd, offset, i, &len, buf);
	if (ret || len != i) {
		printk("nvram_commit: write error\n");
		ret = -EIO;
		goto done;
	}

	offset = nvram_mtd->size - erasesize;
	ret = MTD_READ(nvram_mtd, offset, 4, &len, buf);

      done:
	up(&nvram_sem);
	kfree(buf);
	return ret;
}

int nvram_getall(char *buf, int count)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_getall(buf, count);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

EXPORT_SYMBOL(nvram_get);
EXPORT_SYMBOL(nvram_getall);
EXPORT_SYMBOL(nvram_set);
EXPORT_SYMBOL(nvram_unset);
EXPORT_SYMBOL(nvram_commit);

static void dev_nvram_exit(void)
{
	int order = 0;
	struct page *page, *end;

#ifdef CONFIG_MTD
	if (nvram_mtd)
		put_mtd_device(nvram_mtd);
#endif

	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++)
		ClearPageReserved(page);

	_nvram_exit();
}

static int __init dev_nvram_init(void)
{
	int order = 0, ret = 0;
	struct page *page, *end;
	unsigned int i;

	/* Allocate and reserve memory to mmap() */
	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++)
		SetPageReserved(page);

#ifdef CONFIG_MTD
	/* Find associated MTD device */
	for (i = 0; i < MAX_MTD_DEVICES; i++) {
		nvram_mtd = get_mtd_device(NULL, i);
		if (nvram_mtd) {
			if (!strcmp(nvram_mtd->name, "nvram") &&
			    nvram_mtd->size >= NVRAM_SPACE)
				break;
			put_mtd_device(nvram_mtd);
		}
	}
	if (i >= MAX_MTD_DEVICES)
		nvram_mtd = NULL;
#endif

	/* Initialize hash table lock */
	spin_lock_init(&nvram_lock);

	/* Initialize commit semaphore */
	init_MUTEX(&nvram_sem);

	/* Initialize hash table */
	if ((ret = _nvram_init()) != 0)
		dev_nvram_exit();

	return ret;
}

module_init(dev_nvram_init);
module_exit(dev_nvram_exit);
