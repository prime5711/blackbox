./killall.sh
umount /mnt/mmc
rmmod g_serial.ko
rmmod g_file_storage.ko

insmod g_file_storage.ko file=/dev/mmcblk0p1 removable=1
