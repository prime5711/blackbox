cmd_mvl_patches/dummy.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,mvl_patches/.dummy.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os     -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi  -msoft-float -Uarm -Wdeclaration-after-statement    -DKBUILD_BASENAME=dummy  -c -o mvl_patches/.tmp_dummy.o mvl_patches/dummy.c

deps_mvl_patches/dummy.o := \
  mvl_patches/dummy.c \

mvl_patches/dummy.o: $(deps_mvl_patches/dummy.o)

$(deps_mvl_patches/dummy.o):
