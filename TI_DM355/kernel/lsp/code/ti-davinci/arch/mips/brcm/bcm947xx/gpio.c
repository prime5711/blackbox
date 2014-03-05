/*
 * GPIO char driver
 *
 * Copyright (C) 2004 Broadcom Corporation
 * Copyright (C) 2005 MontaVista Inc. ( 2.6 kernel modifications )
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
 *
 */

#include <linux/module.h>
#include <linux/major.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/devfs_fs_kernel.h>
#include <asm/uaccess.h>

#include <asm/brcm/typedefs.h>
#include <asm/brcm/bcmutils.h>
#include <asm/brcm/sbutils.h>
#include <asm/brcm/bcmdevs.h>

static void *gpio_sbh;
static int gpio_major;
static int gpio_open(struct inode *inode, struct file *file)
{
	if (MINOR(inode->i_rdev) > 3)
		return -ENODEV;

	return 0;
}

static int gpio_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t
gpio_read(struct file *file, char *buf, size_t count, loff_t * ppos)
{
	u32 val;

	switch (MINOR(file->f_dentry->d_inode->i_rdev)) {
	case 0:
		val = sb_gpioin(gpio_sbh);
		break;
	case 1:
		val = sb_gpioout(gpio_sbh, 0, 0);
		break;
	case 2:
		val = sb_gpioouten(gpio_sbh, 0, 0);
		break;
	case 3:
		val = sb_gpiocontrol(gpio_sbh, 0, 0);
		break;
	default:
		return -ENODEV;
	}

	if (put_user(val, (u32 *) buf))
		return -EFAULT;

	return sizeof(val);
}

static ssize_t
gpio_write(struct file *file, const char *buf, size_t count, loff_t * ppos)
{
	u32 val;

	if (get_user(val, (u32 *) buf))
		return -EFAULT;

	switch (MINOR(file->f_dentry->d_inode->i_rdev)) {
	case 0:
		return -EACCES;
	case 1:
		sb_gpioout(gpio_sbh, ~0, val);
		break;
	case 2:
		sb_gpioouten(gpio_sbh, ~0, val);
		break;
	case 3:
		sb_gpiocontrol(gpio_sbh, ~0, val);
		break;
	default:
		return -ENODEV;
	}

	return sizeof(val);
}

static struct file_operations gpio_fops = {
      owner:THIS_MODULE,
      open:gpio_open,
      release:gpio_release,
      read:gpio_read,
      write:gpio_write,
};

static int __init gpio_init(void)
{
	if (!(gpio_sbh = sb_kattach()))
		return -ENODEV;

	sb_gpiosetcore(gpio_sbh);

	if ((gpio_major = register_chrdev(0, "gpio", &gpio_fops)) < 0)
		return gpio_major;

	devfs_mk_dir("gpio");
	devfs_mk_cdev(MKDEV(gpio_major, 0), S_IFCHR | S_IRUGO | S_IWUGO,
		      "gpio/in");
	devfs_mk_cdev(MKDEV(gpio_major, 1), S_IFCHR | S_IRUGO | S_IWUGO,
		      "gpio/out");
	devfs_mk_cdev(MKDEV(gpio_major, 2), S_IFCHR | S_IRUGO | S_IWUGO,
		      "gpio/outen");
	devfs_mk_cdev(MKDEV(gpio_major, 3), S_IFCHR | S_IRUGO | S_IWUGO,
		      "gpio/control");

	return 0;
}

static void __exit gpio_exit(void)
{
	devfs_remove("gpio/control");
	devfs_remove("gpio/outen");
	devfs_remove("gpio/out");
	devfs_remove("gpio/in");
	devfs_remove("gpio");
	unregister_chrdev(gpio_major, "gpio");
	sb_detach(gpio_sbh);
}

module_init(gpio_init);
module_exit(gpio_exit);
