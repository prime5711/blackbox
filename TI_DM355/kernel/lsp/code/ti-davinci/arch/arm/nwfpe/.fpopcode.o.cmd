cmd_arch/arm/nwfpe/fpopcode.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,arch/arm/nwfpe/.fpopcode.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os     -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi  -msoft-float -Uarm -Wdeclaration-after-statement    -DKBUILD_BASENAME=fpopcode -DKBUILD_MODNAME=nwfpe -c -o arch/arm/nwfpe/.tmp_fpopcode.o arch/arm/nwfpe/fpopcode.c

deps_arch/arm/nwfpe/fpopcode.o := \
  arch/arm/nwfpe/fpopcode.c \
    $(wildcard include/config/fpe/nwfpe/xp.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  arch/arm/nwfpe/fpa11.h \
  include/linux/thread_info.h \
  include/linux/bitops.h \
  include/asm/types.h \
  include/asm/bitops.h \
  include/asm/system.h \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/smp.h) \
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
  include/linux/compiler.h \
  include/linux/compiler-gcc3.h \
    $(wildcard include/config/superh.h) \
  include/linux/compiler-gcc.h \
  include/asm/arch/memory.h \
  include/asm/sizes.h \
  include/asm/arch/hardware.h \
  include/asm/arch/io.h \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  arch/arm/nwfpe/fpsr.h \
  arch/arm/nwfpe/milieu.h \
  arch/arm/nwfpe/ARM-gcc.h \
  arch/arm/nwfpe/softfloat.h \
  arch/arm/nwfpe/fpopcode.h \
  arch/arm/nwfpe/fpmodule.h \
    $(wildcard include/config/cpu.h) \
  arch/arm/nwfpe/fpmodule.inl \

arch/arm/nwfpe/fpopcode.o: $(deps_arch/arm/nwfpe/fpopcode.o)

$(deps_arch/arm/nwfpe/fpopcode.o):
