cmd_fs/yaffs2/yaffs_packedtags2.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,fs/yaffs2/.yaffs_packedtags2.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os     -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi  -msoft-float -Uarm -Wdeclaration-after-statement    -DKBUILD_BASENAME=yaffs_packedtags2 -DKBUILD_MODNAME=yaffs -c -o fs/yaffs2/.tmp_yaffs_packedtags2.o fs/yaffs2/yaffs_packedtags2.c

deps_fs/yaffs2/yaffs_packedtags2.o := \
  fs/yaffs2/yaffs_packedtags2.c \
  fs/yaffs2/yaffs_packedtags2.h \
  fs/yaffs2/yaffs_guts.h \
    $(wildcard include/config/yaffs/no/yaffs1.h) \
    $(wildcard include/config/yaffs/unicode.h) \
    $(wildcard include/config/yaffs/yaffs2.h) \
    $(wildcard include/config/yaffs/wince.h) \
    $(wildcard include/config/yaffs/tnode/list/debug.h) \
    $(wildcard include/config/yaffs/short/names/in/ram.h) \
  fs/yaffs2/devextras.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
  include/linux/compiler-gcc3.h \
    $(wildcard include/config/superh.h) \
  include/linux/compiler-gcc.h \
  include/asm/posix_types.h \
  include/asm/types.h \
  include/linux/list.h \
  include/linux/prefetch.h \
  include/asm/processor.h \
  include/asm/atomic.h \
    $(wildcard include/config/smp.h) \
  include/asm/system.h \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/preempt/rt.h) \
    $(wildcard include/config/critical/irqsoff/timing.h) \
    $(wildcard include/config/ilatency.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  include/linux/ilatency.h \
  include/linux/linkage.h \
  include/asm/linkage.h \
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
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  include/asm/procinfo.h \
  include/asm/cache.h \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/security.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  include/asm/ioctl.h \
  include/linux/wait.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/critical/timing.h) \
  include/linux/thread_info.h \
  include/linux/bitops.h \
  include/asm/bitops.h \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
  /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/../lib/gcc/armv5tl-montavista-linuxeabi/3.4.3/include/stdarg.h \
  include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
  include/linux/stringify.h \
  include/linux/rt_lock.h \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/rt/deadlock/detect.h) \
  include/asm/current.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/cache.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/preempt/rcu.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/.h) \
    $(wildcard include/config/numa.h) \
  include/linux/gfp.h \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
  include/linux/numa.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/hotplug/cpu.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
  include/asm/string.h \
  include/linux/smp.h \
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
  include/linux/seqlock.h \
  include/linux/stat.h \
  include/asm/stat.h \
  include/linux/time.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
    $(wildcard include/config/sysfs.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/kref.h \
  include/linux/kobject_uevent.h \
    $(wildcard include/config/kobject/uevent.h) \
  include/linux/radix-tree.h \
  include/linux/prio_tree.h \
  include/linux/audit.h \
    $(wildcard include/config/audit.h) \
  include/asm/semaphore.h \
  include/asm/locks.h \
  include/linux/semaphore.h \
  include/linux/quota.h \
  include/linux/errno.h \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/fcntl.h \
  include/asm/fcntl.h \
  include/linux/err.h \
  fs/yaffs2/yportenv.h \
    $(wildcard include/config/yaffs/direct.h) \
    $(wildcard include/config/yaffs/util.h) \
  fs/yaffs2/moduleconfig.h \
    $(wildcard include/config/h//.h) \
    $(wildcard include/config/yaffs/fs.h) \
    $(wildcard include/config/yaffs/yaffs1.h) \
    $(wildcard include/config/yaffs/does/ecc.h) \
    $(wildcard include/config/yaffs/ecc/wrong/order.h) \
    $(wildcard include/config/yaffs/disable/chunk/erased/check.h) \
  include/linux/version.h \
  include/linux/mm.h \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/debug/pagealloc.h) \
  include/linux/sched.h \
    $(wildcard include/config/preempt/softirqs.h) \
    $(wildcard include/config/preempt/hardirqs.h) \
    $(wildcard include/config/preempt/bkl.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/preempt/trace.h) \
    $(wildcard include/config/latency/trace.h) \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/mcount.h) \
    $(wildcard include/config/wakeup/timing.h) \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/magic/sysrq.h) \
  include/asm/param.h \
  include/asm/arch/param.h \
  include/linux/capability.h \
  include/linux/timex.h \
    $(wildcard include/config/time/interpolation.h) \
  include/asm/timex.h \
  include/asm/arch/timex.h \
    $(wildcard include/config/arch/davinci355.h) \
  include/linux/jiffies.h \
  include/asm/div64.h \
  include/linux/rbtree.h \
  include/asm/mmu.h \
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
  include/linux/completion.h \
  include/linux/pid.h \
  include/linux/param.h \
  include/linux/timer.h \
  include/linux/aio.h \
  include/linux/workqueue.h \
  include/linux/aio_abi.h \
  include/asm/pgtable.h \
  include/asm/arch/vmalloc.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/swap.h) \
  include/linux/vmalloc.h \
  fs/yaffs2/yaffs_ecc.h \
  fs/yaffs2/yaffs_tagsvalidity.h \

fs/yaffs2/yaffs_packedtags2.o: $(deps_fs/yaffs2/yaffs_packedtags2.o)

$(deps_fs/yaffs2/yaffs_packedtags2.o):
