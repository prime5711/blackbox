cmd_arch/arm/boot/compressed/head.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,arch/arm/boot/compressed/.head.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__ -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float    -c -o arch/arm/boot/compressed/head.o arch/arm/boot/compressed/head.S

deps_arch/arm/boot/compressed/head.o := \
  arch/arm/boot/compressed/head.S \
    $(wildcard include/config/debug/dc21285/port.h) \
    $(wildcard include/config/debug/icedcc.h) \
    $(wildcard include/config/footbridge.h) \
    $(wildcard include/config/arch/rpc.h) \
    $(wildcard include/config/arch/integrator.h) \
    $(wildcard include/config/arch/pxa.h) \
    $(wildcard include/config/arch/sa1100.h) \
    $(wildcard include/config/debug/ll/ser3.h) \
    $(wildcard include/config/arch/ixp4xx.h) \
    $(wildcard include/config/arch/ixp2000.h) \
    $(wildcard include/config/arch/lh7a40x.h) \
    $(wildcard include/config/arch/omap.h) \
    $(wildcard include/config/omap/ll/debug/uart2.h) \
    $(wildcard include/config/omap/ll/debug/uart3.h) \
    $(wildcard include/config/arch/iop331.h) \
    $(wildcard include/config/arch/s3c2410.h) \
    $(wildcard include/config/s3c2410/lowlevel/uart/port.h) \
    $(wildcard include/config/zboot/rom.h) \
    $(wildcard include/config/mcount.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \

arch/arm/boot/compressed/head.o: $(deps_arch/arm/boot/compressed/head.o)

$(deps_arch/arm/boot/compressed/head.o):
