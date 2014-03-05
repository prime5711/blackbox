cmd_lib/errno.o := /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-gcc -Wp,-MD,lib/.errno.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os     -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=aapcs -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi  -msoft-float -Uarm -Wdeclaration-after-statement    -DKBUILD_BASENAME=errno -DKBUILD_MODNAME=errno -c -o lib/.tmp_errno.o lib/errno.c

deps_lib/errno.o := \
  lib/errno.c \

lib/errno.o: $(deps_lib/errno.o)

$(deps_lib/errno.o):
