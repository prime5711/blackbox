#!/bin/sh
#./killall.sh
rmmod cmemk.ko
rmmod dm350mmap.ko
rmmod user_gpio.ko


#insmod cmemk.ko phys_start=0x85000000 phys_end=0x88000000 pools=1x21233664,10x1048576,3x3145728,32x131072,1x4153344
#insmod cmemk.ko phys_start=0x85000000 phys_end=0x88000000 pools=1x1048576,1x1048576,1x21825790,1x4388608,8x1048576,3x3145728,32x131072 
#insmod cmemk.ko phys_start=0x85000000 phys_end=0x88000000 pools=1x21233664,8x1048576,3x3145728,32x131072,1x4153344
#insmod cmemk.ko phys_start=0x85000000 phys_end=0x88000000 pools=1x21233664,1x11548,1x512,1x4153344,1x7680,10x4,1x81920,1x38400,2x691200,2x2457600
#insmod cmemk.ko phys_start=0x84000000 phys_end=0x88000000 pools=8x4,1x512,1x7680,1x11548,1x38400,1x81920,2x691200,2x2457600,1x4153344,1x37748736
#insmod cmemk.ko phys_start=0x84000000 phys_end=0x88000000 pools=8x4,1x512,1x7680,1x11548,1x38400,1x81920,2x691200,2x2457600,1x4153344,1x55705600
#insmod cmemk.ko phys_start=0x84000000 phys_end=0x88000000 pools=9x4096,1x8192,1x12288,1x40960,1x81920,2x692224,2x2457600,1x4153344,1x55705600
insmod cmemk.ko phys_start=0x84E00000 phys_end=0x88000000 pools=9x4096,3x8192,1x12288,2x20480,1x28672,2x40960,1x81920,1x126976,2x692224,2x2621440,1x4411392,1x40108032

./mapdmaq

insmod dm350mmap.ko
rm -f /dev/dm350mmap
mknod /dev/dm350mmap c `awk "\\$2==\"dm350mmap\" {print \\$1}" /proc/devices` 0

insmod user_gpio.ko
rm -f /dev/user_gpio
mknod /dev/user_gpio c `awk "\\$2==\"user_gpio\" {print \\$1}" /proc/devices` 0

rm -f /dev/ttygserial
mknod /dev/ttygserial c 127 0


./drv_resz_reload.sh

echo 0 >> /proc/gio/gio23
