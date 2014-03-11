#/bin/sh
# NC1000S version initialize...

DEFUALT_VERSION='0.001'

Kernel_image_file='/mnt/mmc/update/uImage_NC1000S_2.0M_2.0M_2014_0307_2346_rev306_mtd_11'
Rootfs_image_file='/mnt/mmc/update/nc1000s_fs_kurefa_wiz_no_mtd5_jffs2_system_server_2013_0307.bin.gz'
#    PC_Viwer_file='/mnt/mmc/update/PC_Viewer.exe_'
#      Config_file='/mnt/mmc/update/nc100_blackbox.conf'
      Script_file='/mnt/mmc/update/dev_postscript.sh.bak'



echo "Start update process..."

cp /mnt/mmc/update/dev_prescript.sh /mnt/mmc/update/dev_prescript.sh.bak

if [ -f $Script_file ]
then
	cp $Script_file /mnt/mmc/update/dev_postscript.sh
fi


UNAME_VERSION_MATCH=`uname -v | grep 306`

if [ -z "$UNAME_VERSION_MATCH" ]
then

if [ -f $Kernel_image_file -a -f $Rootfs_image_file ] 
then 
	echo "Change to NC1000S_japan_Release_2014_0307_1G08U0B"

	/opt/blackbox/LEDControlForUpdate.sh &

    cp /mnt/mmc/update/dev_prescript.sh /mnt/mmc/update/dev_prescript.sh.bak

    if [ -f $Script_file ]
    then
        cp $Script_file /mnt/mmc/update/dev_postscript.sh
    fi

	flash_eraseall /dev/mtd1
	flash_eraseall /dev/mtd2
	nandwrite -p /dev/mtd1 $Kernel_image_file
	nandwrite -p /dev/mtd2 $Kernel_image_file
	sync

	flash_eraseall /dev/mtd3
	flash_eraseall /dev/mtd4
	nandwrite -p /dev/mtd3 $Rootfs_image_file
	nandwrite -p /dev/mtd4 $Rootfs_image_file
	sync

	reboot &
	sleep 10


	killall LEDControlForUpdate.sh
	echo 0 > /proc/gio/gio20
	echo 0 > /proc/gio/gio23
	echo 0 > /proc/gio/gio27
	echo 0 > /proc/gio/gio24
else 
	echo "update files no exist"
fi 

else
	echo "Current Kernel Version is same as update version"
fi



