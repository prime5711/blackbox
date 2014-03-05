#ifndef _ASM_X8664_PERCPU_H_
#define _ASM_X8664_PERCPU_H_
#include <linux/compiler.h>

/* Same as asm-generic/percpu.h, except that we store the per cpu offset
   in the PDA. Longer term the PDA and every per cpu variable
   should be just put into a single section and referenced directly
   from %gs */

#ifdef CONFIG_SMP

#include <asm/pda.h>

#define __per_cpu_offset(cpu) (cpu_pda[cpu].data_offset)
#define __my_cpu_offset() read_pda(data_offset)

/* Separate out the type, so (int[3], foo) works. */
#define DEFINE_PER_CPU(type, name) \
    __attribute__((__section__(".data.percpu"))) __typeof__(type) per_cpu__##name
#define DEFINE_PER_CPU_LOCKED(type, name) \
    __attribute__((__section__(".data.percpu"))) spinlock_t per_cpu_lock__##name##_locked = SPIN_LOCK_UNLOCKED; \
    __attribute__((__section__(".data.percpu"))) __typeof__(type) per_cpu__##name##_locked
 
/* var is in discarded region: offset to particular copy we want */
#define per_cpu(var, cpu) (*RELOC_HIDE(&per_cpu__##var, __per_cpu_offset(cpu)))
#define __get_cpu_var(var) (*RELOC_HIDE(&per_cpu__##var, __my_cpu_offset()))

#define per_cpu_lock(var, cpu) \
	(*RELOC_HIDE(&per_cpu_lock__##var##_locked, __per_cpu_offset(cpu)))
#define per_cpu_var_locked(var, cpu) \
		(*RELOC_HIDE(&per_cpu__##var##_locked, __per_cpu_offset(cpu)))
#define __get_cpu_lock(var, cpu) \
		per_cpu_lock(var, cpu)
#define __get_cpu_var_locked(var, cpu) \
		per_cpu_var_locked(var, cpu)

/* A macro to avoid #include hell... */
#define percpu_modcopy(pcpudst, src, size)			\
do {								\
	unsigned int __i;					\
	for (__i = 0; __i < NR_CPUS; __i++)			\
		if (cpu_possible(__i))				\
			memcpy((pcpudst)+__per_cpu_offset(__i),	\
			       (src), (size));			\
} while (0)

extern void setup_per_cpu_areas(void);

#else /* ! SMP */

#define DEFINE_PER_CPU(type, name) \
    __typeof__(type) per_cpu__##name

#define DEFINE_PER_CPU_LOCKED(type, name) \
	spinlock_t per_cpu_lock__##name##_locked = SPIN_LOCK_UNLOCKED; \
	__typeof__(type) per_cpu__##name##_locked

#define per_cpu(var, cpu)			(*((void)cpu, &per_cpu__##var))
#define __get_cpu_var(var)			per_cpu__##var
#define __get_cpu_lock(var, cpu)		per_cpu_lock__##var##_locked
#define __get_cpu_var_locked(var, cpu)		per_cpu__##var##_locked

#endif	/* SMP */

#define DECLARE_PER_CPU(type, name) extern __typeof__(type) per_cpu__##name

#define EXPORT_PER_CPU_SYMBOL(var) EXPORT_SYMBOL(per_cpu__##var)
#define EXPORT_PER_CPU_SYMBOL_GPL(var) EXPORT_SYMBOL_GPL(per_cpu__##var)

#endif /* _ASM_X8664_PERCPU_H_ */
