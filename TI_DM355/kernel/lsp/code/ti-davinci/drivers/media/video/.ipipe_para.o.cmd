cmd_drivers/media/video/ipipe_para.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,drivers/media/video/.ipipe_para.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os     -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi  -msoft-float -Uarm -Wdeclaration-after-statement -I/media/workspace/svn/TI_DM355_NC1000/trunk/kernel/lsp/code/ti-davinci/drivers/media/dvb/dvb-core   -DKBUILD_BASENAME=ipipe_para -DKBUILD_MODNAME=dm355_ipipe_driver -c -o drivers/media/video/.tmp_ipipe_para.o drivers/media/video/ipipe_para.c

deps_drivers/media/video/ipipe_para.o := \
  drivers/media/video/ipipe_para.c \
    $(wildcard include/config/mach/davinci/dm355/ipnc.h) \
  drivers/media/video/ipipe_para.h \
  include/asm-arm/arch-davinci/dm355_ipipe_hw.h \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/rt.h) \
  /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/../lib/gcc/armv5tl-montavista-linuxeabi/3.4.3/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
  include/linux/compiler-gcc3.h \
    $(wildcard include/config/superh.h) \
  include/linux/compiler-gcc.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  include/asm/posix_types.h \
  include/asm/types.h \
  include/linux/bitops.h \
  include/asm/bitops.h \
  include/asm/system.h \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/critical/irqsoff/timing.h) \
    $(wildcard include/config/ilatency.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  include/linux/ilatency.h \
  include/asm/proc-fns.h \
    $(wildcard include/config/cpu/32.h) \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/asm/cpu-single.h \
  include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/cpu/minicache.h) \
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/debug/slab.h) \
  include/asm/glue.h \
    $(wildcard include/config/cpu/abrt/lv4t.h) \
    $(wildcard include/config/cpu/abrt/ev4.h) \
    $(wildcard include/config/cpu/abrt/ev4t.h) \
    $(wildcard include/config/cpu/abrt/ev5tj.h) \
    $(wildcard include/config/cpu/abrt/ev5t.h) \
    $(wildcard include/config/cpu/abrt/ev6.h) \
  include/asm/memory.h \
    $(wildcard include/config/discontigmem.h) \
  include/asm/arch/memory.h \
  include/asm/sizes.h \
  include/asm/arch/hardware.h \
  include/asm/arch/io.h \
  include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
  include/asm/io.h \
  include/asm-arm/arch-davinci/dm355_ipipe.h \
  include/linux/ioctl.h \
  include/asm/ioctl.h \
  include/linux/completion.h \
  include/linux/wait.h \
  include/linux/list.h \
  include/linux/prefetch.h \
  include/asm/processor.h \
  include/asm/atomic.h \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  include/asm/procinfo.h \
  include/asm/cache.h \
  include/linux/spinlock.h \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/spinlock.h) \
  include/linux/preempt.h \
    $(wildcard include/config/critical/timing.h) \
  include/linux/thread_info.h \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  include/linux/stringify.h \
  include/linux/rt_lock.h \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/rt/deadlock/detect.h) \
  include/asm/current.h \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/hotplug/cpu.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
  include/asm/string.h \
  include/linux/hardirq.h \
    $(wildcard include/config/preempt/bkl.h) \
  include/linux/smp_lock.h \
    $(wildcard include/config/lock/kernel.h) \
  include/linux/sched.h \
    $(wildcard include/config/preempt/softirqs.h) \
    $(wildcard include/config/preempt/hardirqs.h) \
    $(wildcard include/config/preempt/trace.h) \
    $(wildcard include/config/latency/trace.h) \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/mcount.h) \
    $(wildcard include/config/wakeup/timing.h) \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/magic/sysrq.h) \
  include/asm/param.h \
  include/asm/arch/param.h \
  include/linux/capability.h \
  include/linux/timex.h \
    $(wildcard include/config/time/interpolation.h) \
  include/linux/time.h \
  include/linux/seqlock.h \
  include/asm/timex.h \
  include/asm/arch/timex.h \
    $(wildcard include/config/arch/davinci355.h) \
  include/linux/jiffies.h \
  include/asm/div64.h \
  include/linux/rbtree.h \
  include/asm/semaphore.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/asm/locks.h \
  include/linux/semaphore.h \
  include/asm/mmu.h \
  include/linux/smp.h \
  include/linux/sem.h \
    $(wildcard include/config/sysvipc/semmni.h) \
    $(wildcard include/config/sysvipc/semmsl.h) \
    $(wildcard include/config/sysvipc.h) \
  include/linux/ipc.h \
  include/asm/ipcbuf.h \
  include/asm/sembuf.h \
  include/linux/signal.h \
  include/asm/signal.h \
  include/asm/sigcontext.h \
  include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/resource.h \
  include/asm/resource.h \
  include/linux/securebits.h \
  include/linux/fs_struct.h \
  include/linux/pid.h \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/.h) \
  include/linux/gfp.h \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
  include/linux/cache.h \
  include/linux/numa.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
  include/asm/topology.h \
  include/asm-generic/topology.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/kmalloc_sizes.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/large/allocs.h) \
  include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/param.h \
  include/linux/timer.h \
  include/linux/aio.h \
  include/linux/workqueue.h \
  include/linux/aio_abi.h \
  include/asm/hardirq.h \
  include/asm/irq.h \
  include/asm/arch/irqs.h \
  include/linux/irq_cpustat.h \

drivers/media/video/ipipe_para.o: $(deps_drivers/media/video/ipipe_para.o)

$(deps_drivers/media/video/ipipe_para.o):
