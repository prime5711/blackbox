cmd_arch/arm/kernel/entry-common.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,arch/arm/kernel/.entry-common.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__ -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float    -c -o arch/arm/kernel/entry-common.o arch/arm/kernel/entry-common.S

deps_arch/arm/kernel/entry-common.o := \
  arch/arm/kernel/entry-common.S \
    $(wildcard include/config/ltt.h) \
    $(wildcard include/config/critical/irqsoff/timing.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/oabi/compat.h) \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/alignment/trap.h) \
    $(wildcard include/config/mcount.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/smp.h) \
  include/asm/unistd.h \
  include/linux/linkage.h \
  include/asm/linkage.h \
  arch/arm/kernel/entry-header.S \
    $(wildcard include/config/arch/xxxx.h) \
    $(wildcard include/config/frame/pointer.h) \
  include/asm/assembler.h \
  include/asm/constants.h \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/asm/hardware.h \
  include/asm/arch/hardware.h \
  include/asm/sizes.h \
  include/asm/memory.h \
    $(wildcard include/config/discontigmem.h) \
  include/linux/compiler.h \
  include/asm/arch/memory.h \
  include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/cpu/minicache.h) \
    $(wildcard include/config/debug/slab.h) \
  include/asm/arch/io.h \
  include/asm/types.h \
  include/asm/arch/irqs.h \
  include/asm/arch/entry-macro.S \
  arch/arm/kernel/calls.S \

arch/arm/kernel/entry-common.o: $(deps_arch/arm/kernel/entry-common.o)

$(deps_arch/arm/kernel/entry-common.o):
