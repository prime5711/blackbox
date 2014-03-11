#/bin/sh
# NC1000S version initialize...

DEFUALT_VERSION='0.001'

Kernel_image_file='/mnt/mmc/update/uImage_NC1000S_2.0M_2.0M_2013_1129_2212_rev304'
Rootfs_image_file='/mnt/mmc/update/nc1000s_fs_wizwin_1202_0005.bin.gz'
PC_Viwer_file='/mnt/mmc/update/PC_Viewer.exe_'
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
	flash_eraseall /dev/mtd2
	nandwrite -p /dev/mtd1 $Kernel_image_file
	nandwrite -p /dev/mtd2 $Kernel_image_file
	sync
}

Update_NC1000S_rootfs()
{
	flash_eraseall /dev/mtd3
	flash_eraseall /dev/mtd4
	nandwrite -p /dev/mtd3 $Rootfs_image_file
	nandwrite -p /dev/mtd4 $Rootfs_image_file
	sync
}

Update_NC1000W_kenel()
{
	flash_eraseall /dev/mtd1
	nandwrite -p /dev/mtd1 $Kernel_image_file
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
        cp  $PC_Viwer_file /mnt/nand/pc_viewer/PC_Viewer.exe
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
	echo "Change to NC1000S..."

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

	# NC100W base...
	if [ -n "$NC1000W_KERNEL_SIZE" ]
	then 
		echo "NC1000W update to NC1000S"
		Update_NC1000W_kenel
		
		UBOOTBootcmdKernel="nand read 0x80700000 0x200000 0x200000"		
		UBOOTBootcmdRootFs="nand read 0x82000000 0x600000 0xA00000"
				
		UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
		
		echo "$UBOOTBootcmd"
		
		/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd
		
		sync
            
		reboot &
		sleep 10
		
	else
    	SvnRevision="/opt/blackbox/svnversion"

		if [ -f $SvnRevision ]
			then 
				echo "check NC1000S version..."
				CUR_SVN_REVISION=$(grep -v ^# $SvnRevision | grep SVN_REVERSION | cut -d'=' -f2)
				echo "Version : $CUR_SVN_REVISION"
				
				if [ $CUR_SVN_REVISION != "WIZWIN_1202_0005" ] 
				then 
					echo "NC1000S update to NC1000S"
					Update_NC1000S_kenel
					Update_NC1000S_rootfs
            
					UBOOTBootcmdKernel="nand read 0x80700000 0x200000 0x200000"
					UBOOTBootcmdRootFs="nand read 0x82000000 0x600000 0xA00000"
        
					UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
					/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd
            
					InitSystem
            
					sync
            
					reboot &
					sleep 10
				fi
		else 
			echo "NC1000S update to NC1000S"
			Update_NC1000S_kenel
			Update_NC1000S_rootfs
	
			UBOOTBootcmdKernel="nand read 0x80700000 0x200000 0x200000"
			UBOOTBootcmdRootFs="nand read 0x82000000 0x600000 0xA00000"
        
			UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
			/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd

			InitSystem
			sync
		
			reboot &
			sleep 10
		fi
	fi 	


	killall LEDControlForUpdate.sh
	echo 0 > /proc/gio/gio20
	echo 0 > /proc/gio/gio23
	echo 0 > /proc/gio/gio27
	echo 0 > /proc/gio/gio24
	echo 0 > /proc/gio/gio19
else 
	echo "update files no exist"
fi 



