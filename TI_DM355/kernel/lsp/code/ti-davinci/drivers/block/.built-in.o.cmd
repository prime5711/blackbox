cmd_drivers/block/built-in.o :=  /media/workspace/svn/TI_DM355_NC1000/trunk/cross_compiler/mv_pro_4.0.1/montavista/pro/devkit/arm/v5t_le/bin/arm_v5t_le-ld -EL   -r -o drivers/block/built-in.o drivers/block/elevator.o drivers/block/ll_rw_blk.o drivers/block/ioctl.o drivers/block/genhd.o drivers/block/scsi_ioctl.o drivers/block/noop-iosched.o drivers/block/as-iosched.o drivers/block/rd.o drivers/block/loop.o drivers/block/sbull/built-in.o