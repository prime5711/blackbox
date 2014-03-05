cmd_arch/arm/kernel/vmlinux.lds := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -E -Wp,-MD,arch/arm/kernel/.vmlinux.lds.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude   -DTEXTADDR=0xC0008000 -DDATAADDR= -P -C -Uarm -D__ASSEMBLY__ -o arch/arm/kernel/vmlinux.lds arch/arm/kernel/vmlinux.lds.S

deps_arch/arm/kernel/vmlinux.lds := \
  arch/arm/kernel/vmlinux.lds.S \
    $(wildcard include/config/xip/kernel.h) \
  include/asm-generic/vmlinux.lds.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \

arch/arm/kernel/vmlinux.lds: $(deps_arch/arm/kernel/vmlinux.lds)

$(deps_arch/arm/kernel/vmlinux.lds):
