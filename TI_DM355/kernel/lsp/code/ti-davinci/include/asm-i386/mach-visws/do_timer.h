/* defines for inline arch setup functions */

#include <asm/fixmap.h>
#include "cobalt.h"

static inline void do_timer_interrupt_hook(struct pt_regs *regs)
{
	/* Clear the interrupt */
	co_cpu_write(CO_CPU_STAT,co_cpu_read(CO_CPU_STAT) & ~CO_STAT_TIMEINTR);

#ifdef CONFIG_HIGH_RES_TIMERS
        { 
		long arch_cycles = get_arch_cycles(jiffies);

		/*
		 * We use unsigned here to correct a little problem when
		 * the TSC is reset during the SMP sync TSC stuff at
		 * boot time.  The unsigned on the compare will force
		 * the code into a loop updating the "stake"
		 * (last_update) until we get a positive result.  By
		 * using unsigned we don't incure any additional over
		 * head while still traping the problem of a negative
		 * return.
		 */
		if ((unsigned)arch_cycles < arch_cycles_per_jiffy) { 
			do_hr_timer_int();
			return;
		}
		discipline_PIT_timer();
		do{
			do_timer(regs);
			stake_cpuctr();
		}while ((unsigned)get_arch_cycles(jiffies) > arch_cycles_per_jiffy);
	}
#else
	do_timer(regs);
#endif
#ifndef CONFIG_SMP
	update_process_times(user_mode(regs));
#endif
/*
 * In the SMP case we use the local APIC timer interrupt to do the
 * profiling, except when we simulate SMP mode on a uniprocessor
 * system, in that case we have to call the local interrupt handler.
 */
#ifndef CONFIG_X86_LOCAL_APIC
	profile_tick(CPU_PROFILING, regs);
#else
	if (!using_apic_timer)
		smp_local_timer_interrupt(regs);
#endif
}

static inline int do_timer_overflow(int count)
{
	int i;

	spin_lock(&i8259A_lock);
	/*
	 * This is tricky when I/O APICs are used;
	 * see do_timer_interrupt().
	 */
	i = inb(0x20);
	spin_unlock(&i8259A_lock);
	
	/* assumption about timer being IRQ0 */
	if (i & 0x01) {
		/*
		 * We cannot detect lost timer interrupts ... 
		 * well, that's why we call them lost, don't we? :)
		 * [hmm, on the Pentium and Alpha we can ... sort of]
		 */
		count -= LATCH;
	} else {
		printk("do_slow_gettimeoffset(): hardware timer problem?\n");
	}
	return count;
}
