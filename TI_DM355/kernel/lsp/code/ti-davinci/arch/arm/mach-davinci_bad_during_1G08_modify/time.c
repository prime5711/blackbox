/*
 * linux/arch/arm/mach-davinci/time.c
 *
 * DaVinci timer subsystem
 *
 * Author: MontaVista Software, Inc. <source@mvista.com>
 *
 * Copyright 2005 (c) MontaVista Software, Inc. This file is licensed
 * under the terms of the GNU General Public License version 2. This
 * program is licensed "as is" without any warranty of any kind,
 * whether express or implied.
 *
 */
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/hrtime.h>

#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/system.h>
#include <asm/leds.h>
#include <asm/irq.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include <asm/arch/timex.h>
#include <asm/arch/irqs.h>
#include <asm/errno.h>
#include <asm/hrtime.h> /* for cycles-to-nsec macros */

enum {
	T0_BOT = 0, T0_TOP, T1_BOT, T1_TOP, NUM_TIMERS,
};
#define IS_TIMER1(id)    (id & 0x2)
#define IS_TIMER0(id)    (!IS_TIMER1(id))
#define IS_TIMER_TOP(id) ((id & 0x1))
#define IS_TIMER_BOT(id) (!IS_TIMER_TOP(id))

int timer_irqs[NUM_TIMERS] = {
	IRQ_TINT0_TINT12,
	IRQ_TINT0_TINT34,
	IRQ_TINT1_TINT12,
	IRQ_TINT1_TINT34,
};

/*
 * This driver configures the 2 64-bit DaVinci timers as 4 independent
 * 32-bit timers used as follows:
 *
 * T0_BOT: Timer 0, bottom:  free-running counter, used for cycle counter
 * T0_TOP: Timer 0, top   :  high-res timer programmable timer
 * T1_BOT: Timer 1, bottom:  reserved for DSP
 * T1_TOP: Timer 1, top   :  Linux system tick
 */
#define TID_SYSTEM  T1_TOP
#define TID_FREERUN T0_BOT
#define TID_HRT     T0_TOP

/* timer regs */
typedef struct davinci_timer_regs_s {
	unsigned int pid12;		/* 0x0 */
	unsigned int emumgt_clksped;	/* 0x4 */
	unsigned int gpint_en;		/* 0x8 */
	unsigned int gpdir_dat;		/* 0xC */
	unsigned int tim12;		/* 0x10 */
	unsigned int tim34;		/* 0x14 */
	unsigned int prd12;		/* 0x18 */
	unsigned int prd34;		/* 0x1C */
	unsigned int tcr;		/* 0x20 */
	unsigned int tgcr;		/* 0x24 */
	unsigned int wdtcr;		/* 0x28 */
	unsigned int tlgc;		/* 0x2C */
	unsigned int tlmr;		/* 0x30 */
} davinci_timer_regs_t;

typedef struct davinci_timer_s {
	char *name;
	unsigned int id;
	unsigned long period;
	unsigned long opts;
	davinci_timer_regs_t *regs;
	struct irqaction irqaction;
} davinci_timer_t;
static davinci_timer_t davinci_timers[];

/* values for 'opts' field of davinci_timer_t */
#define TIMER_DISABLED   0x00
#define TIMER_ONE_SHOT   0x01
#define TIMER_CONTINUOUS 0x02

#define davinci_timer_base(id) \
  (IS_TIMER1(id) ? \
   (volatile davinci_timer_regs_t*)IO_ADDRESS(DAVINCI_TIMER1_BASE) :  \
   (volatile davinci_timer_regs_t*)IO_ADDRESS(DAVINCI_TIMER0_BASE))

static int davinci_timer32_config(davinci_timer_t *t) {
	volatile davinci_timer_regs_t *regs = t->regs;
	u32 enamode_shift, reset_shift;
	int ret = 0;

	if (IS_TIMER_BOT(t->id)) {
		regs->prd12 = t->period;
		enamode_shift = 6;
		reset_shift = 0;
	}
	else {
		regs->prd34 = t->period;
		enamode_shift = 22;
		reset_shift = 1;
	}

	/* reset timer */
	regs->tgcr &= ~(0x1 << reset_shift);

	/* Register interrupt */
	if (t->irqaction.handler != NULL) {
		ret = setup_irq(timer_irqs[t->id], &t->irqaction);
	}

	/* Set enable mode */
	if (t->opts & TIMER_ONE_SHOT) {
		regs->tcr |= 0x1 << enamode_shift;
	}
	else if (t->opts & TIMER_CONTINUOUS) {
		regs->tcr |= 0x2 << enamode_shift;
	}
	else { /* TIMER_DISABLED */
		regs->tcr &= ~(0x3 << enamode_shift);
	}

	/* unreset */
	regs->tgcr |= (0x1 << reset_shift);

	return ret;
}

static inline u32 davinci_timer32_read(davinci_timer_t *t) {
	volatile davinci_timer_regs_t *regs = t->regs;

	if IS_TIMER_TOP(t->id) {
		return regs->tim34;
	}
	else {
		return regs->tim12;
	}
}

/*
 * Last processed system timer interrupt
 */
static unsigned long davinci_timer32_last = 0;
static irqreturn_t system_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	unsigned long now, latency;

	write_seqlock(&xtime_lock);
	now = davinci_timer32_read(&davinci_timers[TID_FREERUN]);
	latency = davinci_timer32_read(&davinci_timers[TID_SYSTEM]);
	davinci_timer32_last = now - latency;

	/* Do the Linux timer operations */
	timer_tick(regs);
	write_sequnlock(&xtime_lock);

	return IRQ_HANDLED;
}

unsigned long davinci_gettimeoffset(void)
{
	unsigned long now, elapsed, nsec;

	now = davinci_timer32_read(&davinci_timers[TID_FREERUN]);
	elapsed = now - davinci_timer32_last;

	nsec = arch_cycle_to_nsec(elapsed);
	return nsec / 1000;
}

static irqreturn_t freerun_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
	/* TODO: keep track of roll-overs for 64-bit cycle-count */
	return IRQ_HANDLED;
}

cycles_t davinci_get_cycles(void) {
	return davinci_timer32_read(&davinci_timers[TID_FREERUN]);
}

#ifdef CONFIG_HIGH_RES_TIMERS
int
schedule_hr_timer_int(unsigned long ref_jiffies, int ref_cycles)
{
	unsigned long temp_cycles, jiffies_f = jiffies;
	davinci_timer_t *t = &davinci_timers[TID_HRT];

	BUG_ON(ref_cycles < 0);

	/*
	 * Get offset from last jiffy
	 */
	temp_cycles = (ref_jiffies - jiffies_f) * arch_cycles_per_jiffy +
	    ref_cycles - get_arch_cycles(jiffies_f);

	if ((long) (ref_jiffies - jiffies_f) <= 0 && (long) temp_cycles < 0)
		return -ETIME;

	t->period = temp_cycles;
	t->opts   = TIMER_ONE_SHOT;
	davinci_timer32_config(t);

	return 0;
}

int
get_arch_cycles(unsigned long ref_jiffies)
{
	extern unsigned long do_getmachinecycles(void);
	int ret;
	unsigned now;
	unsigned temp_jiffies;
	unsigned diff_jiffies;

	do {
		/* snapshot jiffies */
		temp_jiffies = jiffies;
		barrier();

		/* calculate cycles since the current jiffy */
		now = davinci_timer32_read(&davinci_timers[TID_FREERUN]);
		ret = now - davinci_timer32_last;

		/* compensate for ref_jiffies in the past */
		if (unlikely(diff_jiffies = jiffies - ref_jiffies))
			ret += diff_jiffies * arch_cycles_per_jiffy;

		barrier();
		/* repeat if we didn't have a consistent view of the world */
	} while (unlikely(temp_jiffies != jiffies));

	return ret;
}

static irqreturn_t
hr_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	do_hr_timer_int();

	return IRQ_HANDLED;
}

static int
hr_timer_init(void)
{
	int ret = 0;

	/* Initialized by init of davinci_timers[] array */

	return ret;
}

__initcall(hr_timer_init);
#endif /* CONFIG_HIGH_RES_TIMERS */

static davinci_timer_t davinci_timers[NUM_TIMERS] = {
	[TID_SYSTEM] = {
		.name      = "system tick",
		.period    = ((CLOCK_TICK_RATE / HZ) - 1),
		.opts      = TIMER_CONTINUOUS,
		.irqaction = {
			.flags   = SA_INTERRUPT | SA_NODELAY,
			.handler = system_timer_interrupt,
		}
	},
	[TID_FREERUN] = {
		.name       = "free-run counter",
		.period     = 0xffffffff,
		.opts       = TIMER_CONTINUOUS,
		.irqaction = {
			.flags   = SA_INTERRUPT,
			.handler = freerun_interrupt,
		}
	},
#ifdef CONFIG_HIGH_RES_TIMERS
	[TID_HRT] = {
		.name       = "high-res timer",
		.opts       = TIMER_DISABLED,
		.period     = 0,
		.irqaction = {
			.flags = SA_INTERRUPT | SA_NODELAY,
			.handler = hr_timer_interrupt,
		}
	},
#endif
};

void __init davinci_timer_init(void)
{
	volatile davinci_timer_regs_t *t0 = davinci_timer_base(T0_BOT);
	volatile davinci_timer_regs_t *t1 = davinci_timer_base(T1_BOT);
	int i;

	/* Disabled, Internal clock source */
	t0->tcr = 0x0;
	t1->tcr = 0x0;

	/* reset both timers, no pre-scaler for timer34 */
	t0->tgcr = 0;
	t1->tgcr = 0;

	/* Set both timers to unchained 32-bit */
	t0->tgcr |= 0x4;
	t1->tgcr |= 0x4;

	/* Unreset timers */
	t0->tgcr |= 0x3;
	t1->tgcr |= 0x3;

	/* Init both counters to zero */
	t0->tim12 = 0;
	t0->tim34 = 0;
	t1->tim12 = 0;
	t1->tim34 = 0;

	for(i=0; i<sizeof(davinci_timers)/sizeof(davinci_timer_t); i++) {
		davinci_timer_t *t = &davinci_timers[i];

		if (t->name) {
			t->id = i;
			t->regs =
			    (davinci_timer_regs_t *)davinci_timer_base(t->id);
			t->irqaction.name = t->name;
			t->irqaction.dev_id = (void *)t;

			davinci_timer32_config(&davinci_timers[i]);
		}
	}
}

struct sys_timer davinci_timer = {
	.init   = davinci_timer_init,
	.offset = davinci_gettimeoffset,
};


void davinci_watchdog_reset(void) {
	volatile davinci_timer_regs_t *davinci_wdt = 
		(volatile davinci_timer_regs_t *)IO_ADDRESS(DAVINCI_WDOG_BASE);

	davinci_wdt->tcr = 0x0;		/* disable timer */
	davinci_wdt->tgcr = 0x0;	/* reset timer */
        davinci_wdt->tgcr = 0x8;	/* configure timer2 as 64-bit */
        davinci_wdt->tgcr |= 0x3;	/* release timer from reset */
	davinci_wdt->tim12 = 0;		/* clear counter and period regs */
	davinci_wdt->tim34 = 0;
	davinci_wdt->prd12 =  0;
	davinci_wdt->prd34 =  0;
	davinci_wdt->wdtcr |= 0x4000;	/* enable watchdog timer */

	/* put watchdog in pre-active state */
	davinci_wdt->wdtcr = 0xA5C64000;

	/* put watchdog in active state */
	davinci_wdt->wdtcr = 0xDA7E4000;	

	/* write an invalid value to the WDKEY field to trigger 
	 * a watchdog reset */
	davinci_wdt->wdtcr = 0x00004000;
}

u32 davinci_timer_read(int clock_id) {
	davinci_timer_t *t = &davinci_timers[clock_id];

	return davinci_timer32_read(t);
}
