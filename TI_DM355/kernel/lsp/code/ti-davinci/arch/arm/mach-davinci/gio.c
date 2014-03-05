/*****************************************************************************/
/*                                                                           */
/*  Copyright (c) 2003 - 2004, MultiMedia Solutions Ltd.                     */
/*  All rights reserved.                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  Name:           gio.c                                                    */
/*  Autor:          Dinko Mironov                                            */
/*  Description:    GIO Support                                              */
/*  Project:        Monta Vista Linux - DaVinci                              */
/*                                                                           */
/*****************************************************************************/
/*  History:                                                                 */
/*  Version Date        Author          Reason                               */
/*                                                                           */
/*  1.0     05.06.2006  Dinko           Ported to DaVinci                    */
/*  1.1     19.01.2007  Sudhakar        Modifed                              */
/*  1.2     31.07.2007  Sandeep			Ported to DM355						 */
/*                                                                           */
/*****************************************************************************/

/**************************************************************************
 * Included Files
 **************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/cpu.h>
#include <asm/arch/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/gio.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/arch/irqs.h>
#include <asm/arch/hardware.h>

static struct proc_dir_entry *gio_procdir;
// Assuming 104 is the max for DM644x and DM350
static struct proc_dir_entry *gio_proc[104];

static struct gpio_controller *__iomem gpio2controller(unsigned gpio)
{
	void *__iomem ptr = NULL;

	if (cpu_is_davinci_dm355())
	{
		if (gpio < 32)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10);
		else if (gpio < 64)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38);
		else if (gpio < 96)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60);
		else if (gpio < DM355_NUM_GIOS)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88);
		else
			ptr = NULL;
	}
	else if (cpu_is_davinci_dm6443())
	{
		if (gpio < 32)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10);
		else if (gpio < 64)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38);
		else if (gpio < DM644x_NUM_GIOS)
			ptr = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60);
		else
			ptr = NULL;
	}
	return ptr;
}

/*
 * Assuming the pin is muxed as a gpio output, set its output value.
 */
int __gpio_set(unsigned gpio, int value)
{
        struct gpio_controller  *__iomem g = gpio2controller(gpio);

        if (!g)
                return -EINVAL;
        __raw_writel(gpio_mask(gpio), value ? &g->set_data : &g->clr_data);
        return 0;
}
EXPORT_SYMBOL(__gpio_set);


/*
 * Read the pin's value (works even if it's not set up as output);
 * returns zero/nonzero.
 *
 * Note that changes are synched to the GPIO clock, so reading values back
 * right after you've set them may give old values.
 */
int __gpio_get(unsigned gpio)
{
        struct gpio_controller  *__iomem g = gpio2controller(gpio);

        if (!g)
                return -EINVAL;
        return !!(gpio_mask(gpio) & __raw_readl(&g->in_data));
}
EXPORT_SYMBOL(__gpio_get);

/*
 * We expect board setup code to handle all gpio direction setup,
 * so there's no need to let driver modules do it.
 *
 * That same board setup code must also set PINMUX0 and PINMUX1 as
 * needed, and enable the GPIO clock.
 */
int gpio_set_direction(unsigned gpio, int is_in)
{
	struct gpio_controller  *__iomem g = gpio2controller(gpio);
	u32                     temp;
	u32                     mask;

	if (!g)
		return -EINVAL;

	mask = gpio_mask(gpio);
	temp = __raw_readl(&g->dir);
	if (is_in)
		temp |= mask;
	else
		temp &= ~mask;
	__raw_writel(temp, &g->dir);
	return 0;
}
EXPORT_SYMBOL(gpio_set_direction);

int gpio_interrupt_bank_enable(int gpio)
{
	void *__iomem ptr1;
	u32 temp = 0;
	u32 mask = 0;
	u32 gpio_bank;
	u32 max_gpio = 0;

	if (cpu_is_davinci_dm355())
		max_gpio = DM355_NUM_GIOS;
	else if (cpu_is_davinci_dm6443())
		max_gpio = DM644x_NUM_GIOS;

	if (gpio >= max_gpio)
		return -EINVAL;
	gpio_bank = gpio/16;

	ptr1 = (void *__iomem) IO_ADDRESS(DAVINCI_GPIO_BASE + 0x8);
	mask = (1 << gpio_bank);
	temp = __raw_readl(ptr1);
	temp |= mask;
	__raw_writel(temp, ptr1);
	return 0;
}
EXPORT_SYMBOL(gpio_interrupt_bank_enable);

int gpio_interrupt_enable(unsigned gpio, int trigger)
{
	struct gpio_controller  *__iomem g = gpio2controller(gpio);
	u32 mask;
	if (!g)
		return -EINVAL;

	if (trigger & ~(TRIGGER_RISING_EDGE|TRIGGER_FALLING_EDGE))
		return -EINVAL;

	mask = gpio_mask(gpio);

	if (trigger & TRIGGER_RISING_EDGE)
			__raw_writel(mask, &g->set_rising);
	if (trigger & TRIGGER_FALLING_EDGE)
			__raw_writel(mask, &g->set_falling);
	return 0;
}
EXPORT_SYMBOL(gpio_interrupt_enable);

int gpio_interrupt_disable(unsigned gpio, int trigger)
{
	struct gpio_controller  *__iomem g = gpio2controller(gpio);
	u32 mask;
	if (!g)
		return -EINVAL;

	if (trigger & ~(TRIGGER_RISING_EDGE|TRIGGER_FALLING_EDGE))
		return -EINVAL;

	mask = gpio_mask(gpio);

	if (trigger & TRIGGER_RISING_EDGE)
			__raw_writel(mask, &g->clr_rising);
	if (trigger & TRIGGER_FALLING_EDGE)
			__raw_writel(mask, &g->clr_falling);
	return 0;
}
EXPORT_SYMBOL(gpio_interrupt_disable);

#if CONFIG_PROC_FS

#define MAX_BUF		4*1024

static int proc_gio(char *page, char **start, off_t off,
		    int count, int *eof, void *data)
{
	int len = 0;
	char buffer[MAX_BUF];
	volatile unsigned long *reg;

	if (!count)
		return 0;

	reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE);
	len += sprintf(&buffer[len], "GPIO Module:\n");
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x PID                \n", reg,
		    (int)reg[0]);
	reg += 2;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x BINTEN             \n", reg,
		    (int)reg[0]);

	//BANK 0 AND BANK 1
	len += sprintf(&buffer[len], "GPIO Bank0 and Bank1:\n");
	reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x10);
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Direction          \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Output Data        \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Data           \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Input Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Rising edge   \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Rising edge \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Falling edge  \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Falling edge\n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Interrupt Status   \n", reg,
		    (int)reg[0]);

	//BANK 2 AND BANK 3
	len += sprintf(&buffer[len], "GPIO Bank2 and Bank3:\n");
	reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x38);
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Direction          \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Output Data        \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Data           \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Input Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Rising edge   \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Rising edge \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Falling edge  \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Falling edge\n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Interrupt Status   \n", reg,
		    (int)reg[0]);

	//BANK 4 AND BANK 5 for DM355 and BANK 4 for DM6446
	if (cpu_is_davinci_dm355())
		len += sprintf(&buffer[len], "GPIO Bank4 and Bank5:\n");
	else
		len += sprintf(&buffer[len], "GPIO Bank4:\n");
	reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x60);
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Direction          \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Output Data        \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Data           \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Input Data         \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Rising edge   \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Rising edge \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Set Falling edge  \n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Falling edge\n", reg,
		    (int)reg[0]);
	reg++;
	len +=
	    sprintf(&buffer[len], "  0x%p: 0x%08x Interrupt Status   \n", reg,
		    (int)reg[0]);

	//BANK 6
	if (cpu_is_davinci_dm355()) 
	{
		len += sprintf(&buffer[len], "GPIO Bank6:\n");
		reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE + 0x88);
		len +=
		    sprintf(&buffer[len], "  0x%p: 0x%08x Direction          \n", reg,
			    (int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Output Data        \n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Set Data           \n", reg,
			    (int)reg[0]);
		reg++;
		len +=
		    sprintf(&buffer[len], "  0x%p: 0x%08x Clear Data         \n", reg,
			    (int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Input Data         \n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Set Rising edge   \n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Clear Rising edge \n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Set Falling edge  \n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Clear Falling edge\n", reg,
		    	(int)reg[0]);
		reg++;
		len +=
	    	sprintf(&buffer[len], "  0x%p: 0x%08x Interrupt Status   \n", reg,
		    	(int)reg[0]);
	}
	if (count > len)
		count = len;
	memcpy(page, &buffer[off], count);

	*eof = 1;
	*start = NULL;
	return len;
}

static int proc_gio_read(char *page, char **start, off_t off,
			 int count, int *eof, void *data)
{
	int len;

	if (count == 0)
		return 0;

	gpio_set_direction((int)data, GIO_DIR_INPUT);
	len = sprintf(page, "%d\n", gpio_get_value((int)data));

	*eof = 1;
	*start = 0;

	return len;
}

static int proc_gio_write(struct file *file, const char __user * buffer,
			  unsigned long count, void *data)
{
	int i;

	if (!count)
		return 0;

	gpio_set_direction((int)data, GIO_DIR_OUTPUT);

	for (i = 0; i < count; i++) {
		if (buffer[i] == '0')
			gpio_set_value((int)data, 0);
		if (buffer[i] == '1')
			gpio_set_value((int)data, 1);
	}

	return count;
}

int __init gio_proc_client_create(void)
{
	int i;
	char name[16];
	struct proc_dir_entry *ent;
	u32 max_gpio = 0;

	if (cpu_is_davinci_dm355())
		max_gpio = DM355_NUM_GIOS;
	else if (cpu_is_davinci_dm6443())
		max_gpio = DM644x_NUM_GIOS;

	gio_procdir = proc_mkdir("gio", 0);
	if (gio_procdir == NULL) {
		printk(KERN_ERR "gio: failed to register proc directory gio\n");
		return -ENOMEM;
	}

	ent = create_proc_read_entry("registers", 0, gio_procdir, proc_gio, 0);
	if (ent == NULL) {
		printk(KERN_ERR
		       "gio: failed to register proc device: registers\n");
		return -ENOMEM;
	}

	for (i = 0; i < max_gpio; i++) {
		sprintf(name, "gio%d", i);
		gio_proc[i] = create_proc_entry(name, 0, gio_procdir);
		if (gio_proc[i] == NULL) {
			printk(KERN_ERR
			       "gio: failed to register proc device: %s\n",
			       name);
			return -ENOMEM;
		}
		gio_proc[i]->data = (void *)i;
		gio_proc[i]->read_proc = proc_gio_read;
		gio_proc[i]->write_proc = proc_gio_write;
	}

	return 0;
}

#else				/* CONFIG_PROC_FS */

#define gio_proc_client_create() do {} while(0);

#endif				/* CONFIG_PROC_FS */

struct timer_list timer_booting; 

static int timer_cnt = 0; 

static void booting_timer_func(unsigned int data)
{
	add_timer(&timer_booting); 
	if (timer_cnt > 40) {
		del_timer(&timer_booting); 
		gpio_set_value(25, GIO_STATE_LOW); 
	}
	else {
		timer_booting.expires = jiffies + (HZ >> 1); 
		add_timer(&timer_booting); 
		gpio_set_value(25, (timer_cnt & 0x01) ? GIO_STATE_HIGH : GIO_STATE_LOW); 
	}
	timer_cnt++; 
}

static int __init booting_timer_init(void)
{
	gpio_set_direction(25, GIO_DIR_OUTPUT);
	gpio_set_value(25, GIO_STATE_HIGH);
	init_timer(&timer_booting);
	 
	timer_booting.expires = jiffies + (HZ >> 1);
	timer_booting.data = 0;
	timer_booting.function = &booting_timer_func;
	 
	add_timer(&timer_booting);
	 
	return 0;
}


module_init(gio_proc_client_create);
module_init(booting_timer_init);

