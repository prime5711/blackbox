#/bin/sh

DEFUALT_VERSION='0.001'

Kernel_image_file='/mnt/mmc/update/uImage_NC1000W_blackbox_2.5M_1.5M_2014_0312_0126_rev310_5608_no_connect_nc1000w'
Rootfs_image_file='/mnt/mmc/update/nc1000w_fs_2012_1128_1529.bin.gz'
Wifi_Kernel_image_file='/mnt/mmc/update/uImage_bbt_ecc_none_new_final'
Wifi_Rootfs_file_1='/mnt/mmc/update/nc1000w_WifiRootFs_20120502_2013.tar'
Wifi_Rootfs_file_2='/mnt/mmc/update/rtl8192_8188CU_20120502_2016.tar'
PC_Viwer_file='/mnt/mmc/update/GreenCam.exe_'
Config_file='/mnt/mmc/update/nc100_blackbox.conf'

Script_file='/mnt/mmc/update/dev_postscript.sh.bak'



echo "Start update process..."

NandNewKernelAddr=
NandNewKernelSize=
NandNewRootFsAddr=

MTDNewKernelPartition=
MTDNewRootFsPartition=
	

Update_NC1000S_kenel()
{
	flash_eraseall /dev/mtd1
	nandwrite -p /dev/mtd1 $Kernel_image_file

	NandNewKernelAddr='0x200000'
	MTDNewKernelPartition='/dev/mtd1'
	sync
}

Update_NC1000S_rootfs()
{
	flash_eraseall /dev/mtd3
	nandwrite -p /dev/mtd3 $Rootfs_image_file
	NandNewRootFsAddr='0x600000'
	MTDNewRootFsPartition='/dev/mtd3'
	sync
}

Update_NC1000W_kenel()
{
	flash_eraseall /dev/mtd1
	flash_eraseall /dev/mtd2
	nandwrite -p /dev/mtd2 $Kernel_image_file
	nandwrite -p /dev/mtd1 $Wifi_Kernel_image_file

	NandNewKernelAddr='0x480000'
	sync
}

Update_NC1000W_rootfs()
{
	flash_eraseall /dev/mtd3
	nandwrite -p /dev/mtd3 $Rootfs_image_file

    flash_eraseall -j /dev/mtd4
    mount -t jffs2 /dev/mtdblock4 /mnt/nand
    cd /mnt/nand
    tar xvf $Wifi_Rootfs_file_1
    sync

    cd /mnt/nand/lib/modules/2.6.32-rc2-davinci1/kernel/drivers/net/wireless
   	tar xvf $Wifi_Rootfs_file_2

	sync

    cd /opt/blackbox

    umount /mnt/nand

	sync
}



CheckDebugBootenv()
{

	tmp=$(/opt/blackbox/fw_printenv | grep "k_update_2632" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "k_update_2632 boot env delete"
	else 
		echo "k_update_2632 boot env none"
	fi 

	tmp=$(/opt/blackbox/fw_printenv | grep "k_update_2610" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "k_update_2610 boot env delete"
	else 
		echo "k_update_2610 boot env none"
	fi 

	tmp=$(/opt/blackbox/fw_printenv | grep "bootargs_2632" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "bootargs_2632 boot env delete"
	else 
		echo "bootargs_2632 boot env none"
	fi 

	tmp=$(/opt/blackbox/fw_printenv | grep "bootcmd_2632" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "bootcmd_2632 boot env delete"
	else 
		echo "bootcmd_2632 boot env none"
	fi 

	tmp=$(/opt/blackbox/fw_printenv | grep "bootargs_2610" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "bootargs_2610 boot env delete"
	else 
		echo "bootargs_2610 boot env none"
	fi 

	tmp=$(/opt/blackbox/fw_printenv | grep "bootcmd_2610" | cut -d'=' -f1)
	if [ -n $tmp ]
	then 
		/opt/blackbox/fw_setenv $tmp
		echo "bootcmd_2610 boot env delete"
	else 
		echo "bootcmd_2610 boot env none"
	fi 
}

InitSystem()
{
    # mdt5 initiallize
    flash_eraseall -j /dev/mtd5
    mount -t jffs2 /dev/mtdblock5 /mnt/nand


    mkdir -p /mnt/nand/configuration/
    mkdir -p /mnt/nand/pc_viewer/
    mkdir -p /mnt/nand/Info/

    if [ -f $Config_file ]
    then
        cp $Config_file /mnt/nand/configuration/
        sync
    else
        echo "Configuration file does not exist - $Config_file"
    fi

    if [ -f $PC_Viwer_file ]
    then
        cp  $PC_Viwer_file /mnt/nand/pc_viewer/GreenCam.exe
        sync
    else
        echo "PC Viewer file does not exist -  $PC_Viwer_file"
    fi

    echo "kernel_version=$DEFUALT_VERSION" > /mnt/nand/Info/KernelVersion
    echo "root_fs_version=$DEFUALT_VERSION" > /mnt/nand/Info/RootFsVersion

    sync

    umount /mnt/nand
}

if [ -f $Kernel_image_file -a -f $Rootfs_image_file ] 
then 
	echo "change to NaviCam NC1000W..."

	/opt/blackbox/LEDControlForUpdate.sh &

	CheckDebugBootenv

    cp /mnt/mmc/update/dev_prescript.sh /mnt/mmc/update/dev_prescript.sh.bak

    if [ -f $Script_file ]
    then
        cp $Script_file /mnt/mmc/update/dev_postscript.sh
    fi

	UBOOTBootcmd=$(/opt/blackbox/fw_printenv | grep "bootcmd=" | cut -d'=' -f2)

	UBOOTBootcmdKernel=$(echo $UBOOTBootcmd  | cut -d';' -f2)
	UBOOTBootcmdRootFs=$(echo $UBOOTBootcmd  | cut -d';' -f1)
	UBOOTBootcmdOther=$(echo $UBOOTBootcmd  | cut -d';' -f3)


	NC1000W_KERNEL_SIZE=`cat /proc/mtd | grep mtd2 | grep 180000`

	if [ -n "$NC1000W_KERNEL_SIZE" ]
	then 
		echo "NC1000W update NaviCam NC1000W"
		
    	SvnRevision="/opt/blackbox/svnversion"

		if [ -f $SvnRevision ]
			then 
				echo "check NC1000W"
#				CUR_SVN_REVISION=$(grep -v ^# $SvnRevision | grep SVN_REVERSION | cut -d'=' -f2)


#				if [ $CUR_SVN_REVISION != "GREENCAM_226" ] 
#				then 


				UNAME_VERSION_MATCH=`uname -v | grep 310`

				if [ -z "$UNAME_VERSION_MATCH" ] 
				then
					echo "NC1000W update to NaviCam NC1000W"
					Update_NC1000W_kenel
					Update_NC1000W_rootfs
            
					UBOOTBootcmdKernel="nand read 0x80700000 0x480000 0x180000"
					UBOOTBootcmdRootFs="nand read 0x82000000 0x600000 0xA00000"
        
					UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
					/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd
            
					InitSystem
            
					sync
            
					reboot &
					sleep 10
				fi
		else 
			echo "NC1000S update to NaviCam NC1000W"
			Update_NC1000W_kenel
			Update_NC1000W_rootfs
	
			UBOOTBootcmdKernel="nand read 0x80700000 0x480000 0x180000"
			UBOOTBootcmdRootFs="nand read 0x82000000 0x600000 0xA00000"

			UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
			/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd

			InitSystem
			sync
		
			reboot &
			sleep 10
		fi
	else 
		echo "change NC1000S to NC1000W patition !!!"
		Update_NC1000S_kenel
		UBOOTBootcmdKernel="nand read 0x80700000 0x200000 0x200000"

		UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
		/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd

		sync

		reboot &
		sleep 10

	fi 	


	killall LEDControlForUpdate.sh
	echo 0 > /proc/gio/gio20
	echo 0 > /proc/gio/gio23
	echo 0 > /proc/gio/gio27
	echo 0 > /proc/gio/gio24
else 
	echo "update files no exist"
fi 



