cmd_arch/arm/mm/proc-arm926.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,arch/arm/mm/.proc-arm926.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__ -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float    -c -o arch/arm/mm/proc-arm926.o arch/arm/mm/proc-arm926.S

deps_arch/arm/mm/proc-arm926.o := \
  arch/arm/mm/proc-arm926.S \
    $(wildcard include/config/cpu/arm926/cpu/idle.h) \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
    $(wildcard include/config/cpu/cache/round/robin.h) \
    $(wildcard include/config/cpu/icache/disable.h) \
    $(wildcard include/config/cpu/dcache/disable.h) \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
  include/asm/assembler.h \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \
  include/asm/pgtable.h \
  include/asm/memory.h \
    $(wildcard include/config/discontigmem.h) \
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
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/debug/slab.h) \
  include/asm/sizes.h \
  include/asm/arch/hardware.h \
  include/asm/arch/io.h \
  include/asm/types.h \
  include/asm/proc-fns.h \
    $(wildcard include/config/cpu/32.h) \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/asm/arch/vmalloc.h \
  include/asm/procinfo.h \
  include/asm/hardware.h \
  arch/arm/mm/proc-macros.S \
  include/asm/constants.h \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \

arch/arm/mm/proc-arm926.o: $(deps_arch/arm/mm/proc-arm926.o)

$(deps_arch/arm/mm/proc-arm926.o):
