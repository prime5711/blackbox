/*****************************************************************************/
/*                                                                           */
/*  Copyright (c) 2003 - 2004, MultiMedia Solutions Ltd.                     */
/*  All rights reserved.                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  Name:           gio.h                                                    */
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
/*                                                                           */
/*****************************************************************************/
#ifndef	__DAVINCI_GPIO_H
#define	__DAVINCI_GPIO_H

/*
 * basic gpio routines
 *
 * GPIOs are numbered 0..(DM644x_NUM_GIOS-1).  For documentation, and maybe
 * for later updates, code should write GPIO(N) or:
 *  - GPIOV18(N) for 1.8V pins, N in 0..53; same as GPIO(0)..GPIO(53)
 *  - GPIOV33(N) for 3.3V pins, N in 0..17; same as GPIO(54)..GPIO(70)
 *
 */
#define DM644x_NUM_GIOS                71
#define DM355_NUM_GIOS                104

/* Control Direction GIO */
#define GIO_DIR_OUTPUT  0
#define GIO_DIR_INPUT   1

/* Control Output GIO State */
#define GIO_STATE_LOW   0
#define GIO_STATE_HIGH  1

#define TRIGGER_RISING_EDGE		1
#define TRIGGER_FALLING_EDGE	2

// TODO, do we need this for DM355
#define	GPIO(X)		(X)		/* 0 <= X <= 71 */
#define	GPIOV18(X)	(X)		/* 1.8V i/o; 0 <= X <= 53 */
#define	GPIOV33(X)	((X)+54)	/* 3.3V i/o; 0 <= X <= 17 */

struct gpio_controller {
	u32	dir;
	u32	out_data;
	u32	set_data;
	u32	clr_data;
	u32	in_data;
	u32	set_rising;
	u32	clr_rising;
	u32	set_falling;
	u32	clr_falling;
	u32	intstat;
};

static inline u32 gpio_mask(unsigned gpio)
{
	return 1 << (gpio % 32);
}

extern int __gpio_set(unsigned gpio, int value);
extern int __gpio_get(unsigned gpio);

static inline int gpio_set(unsigned gpio)
{
	return __gpio_set(gpio, 1);
}

static inline int gpio_clear(unsigned gpio)
{
	return __gpio_set(gpio, 0);
}

static inline int gpio_set_value(unsigned gpio, int value)
{
	return __gpio_set(gpio, value);
}

/* Returns zero or nonzero, or negative on error; works for gpios
 * configured as inputs OR as outputs.
 *
 * NOTE: changes in reported values are synchronized to the GPIO clock.
 * This is most easily seen after calls to gpio_set() and gpio_clear(),
 * where set-then-get might return the old value.
 */
static inline int gpio_get_value(unsigned gpio)
{
	return __gpio_get(gpio);
}

/* powerup default direction is IN */
extern int gpio_set_direction(unsigned gpio, int is_in);
extern int gpio_interrupt_bank_enable(int gpio);
extern int gpio_interrupt_enable(unsigned gpio, int trigger);
extern int gpio_interrupt_disable(unsigned gpio, int trigger);

/* NOTE:  currently there's no "claim/release" mechanism for GPIOs,
 * so drivers arguing over them will get errors much like they will
 * when the pin isn't muxed properly as gpio ...
 */
#endif	/* __DAVINCI_GPIO_H */
