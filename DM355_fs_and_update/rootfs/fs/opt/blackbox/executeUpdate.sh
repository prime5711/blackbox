#!/bin/sh
# for blackbox update


OLD_VERSION=`cat /proc/mtd | grep mtd1 | grep 200000`

KernelVersionFile='/mnt/nand/Info/KernelVersion'
RootFsVersionFile='/mnt/nand/Info/RootFsVersion'

OldKernelVersion=
OldRootFsVersion=

UpdateDir='/mnt/mmc/update'
UpdateInfoFile="$UpdateDir/update_info.cfg"

UpdateKernelImageFile=
UpdateRootFsImageFile=

update_type=

kernel_version=
root_fs_version=

kernel_image_name=
root_fs_image_name=

kernel_md5=
root_fs_md5=

ExitUpdateOnError()
{
	umount /mnt/mmc
	exit 1;
}

CheckUpdateInfo()
{
    
	echo "Checking Update..."
	
	mount /mnt/mmc

	mkdir -p /mnt/mmc/Info
	mkdir -p /mnt/mmc/update
	mkdir -p /mnt/mmc/pc_viewer
	mkdir -p /mnt/mmc/configuration

# force to set datetime - update time - Start             
	/opt/blackbox/updateDateTime.sh
# force to set datetime - update time - End

# process dev_prescript.sh - Start
	if [ -f /mnt/mmc/update/dev_prescript.sh ]
	then
		echo " Processing dev_prescript.sh ..."
		/mnt/mmc/update/dev_prescript.sh
		rm /mnt/mmc/update/dev_prescript.sh
	fi
# process dev_prescript.sh - End

	mount -t jffs2 /dev/mtdblock5 /mnt/nand
	
	OldKernelVersion=$(grep -v ^# $KernelVersionFile | grep kernel_version | cut -d'=' -f2)		
	OldRootFsVersion=$(grep -v ^# $RootFsVersionFile | grep root_fs_version | cut -d'=' -f2)
			
	echo "Current Kernel Version : $OldKernelVersion"
	echo "Current Root File System Version : $OldRootFsVersion"

# Control for each version - Start
	VersionCtrlFile="version_ctrl_$OldRootFsVersion.sh";
	echo "Version control file : $VersionCtrlFile"
	
	if [ -f "/opt/blackbox/version_ctrl/$VersionCtrlFile" ]
	then
		if [ -f "/mnt/nand/version_ctrl/$VersionCtrlFile" ]
		then
			echo "Already executed...."
		else
			mkdir -p /mnt/nand/version_ctrl
			cp /opt/blackbox/version_ctrl/$VersionCtrlFile /mnt/nand/version_ctrl/$VersionCtrlFile
		
			/mnt/nand/version_ctrl/$VersionCtrlFile
		fi
	fi
# Control for each version - End
	
	umount /mnt/nand

	
	
	echo "kernel_version=$OldKernelVersion" > /mnt/mmc/Info/KernelVersion
	echo "root_fs_version=$OldRootFsVersion" > /mnt/mmc/Info/RootFsVersion

	if [ -f $UpdateInfoFile ]
	then		
		echo "Check update information..."
		
		update_type=$(grep -v ^# $UpdateInfoFile | grep update_type | cut -d'=' -f2)
		
		kernel_version=$(grep -v ^# $UpdateInfoFile | grep kernel_version | cut -d'=' -f2)
		root_fs_version=$(grep -v ^# $UpdateInfoFile | grep root_fs_version | cut -d'=' -f2)

		kernel_image_name=$(grep -v ^# $UpdateInfoFile | grep kernel_image_name | cut -d'=' -f2)
		root_fs_image_name=$(grep -v ^# $UpdateInfoFile | grep root_fs_image_name | cut -d'=' -f2)

		kernel_md5=$(grep -v ^# $UpdateInfoFile | grep kernel_md5 | cut -d'=' -f2)
		root_fs_md5=$(grep -v ^# $UpdateInfoFile | grep root_fs_md5 | cut -d'=' -f2)
		
		UpdateKernelImageFile="$UpdateDir/$kernel_image_name"
		UpdateRootFsImageFile="$UpdateDir/$root_fs_image_name"
		
		if [ $update_type = 'blackbox_kernel' ]
		then
			echo "Update Type : $update_type"
			
			if [ -f $UpdateKernelImageFile ]
			then
				echo "New Kernel Image File : $UpdateKernelImageFile"			
			else
				echo "There is no kernel image file"
				ExitUpdateOnError
			fi

			MajorOldKernelVersion=$(echo $OldKernelVersion | cut -d'.' -f1)
			MinorOldKernelVersion=$(echo $OldKernelVersion | cut -d'.' -f2)
			
			MajorNewKernelVersion=$(echo $kernel_version | cut -d'.' -f1)
			MinorNewKernelVersion=$(echo $kernel_version | cut -d'.' -f2)

			if [ $MajorNewKernelVersion -gt $MajorOldKernelVersion -o $kernel_version = '0.123' ]
			then
				echo "New Kernel Version :  $kernel_version"
			elif [ $MajorNewKernelVersion -eq $MajorOldKernelVersion ]
			then
				if [ $MinorNewKernelVersion -gt $MinorOldKernelVersion ]
				then
					echo "New Kernel Version :  $kernel_version"
				else
					echo "Current Kernel Version is newer or same as new one."
					ExitUpdateOnError
				fi
			else
				echo "Current Kernel Version is newer or same as new one."
				ExitUpdateOnError			
			fi
			
		elif [ $update_type = 'blackbox_rootfs' ]
		then
			echo "Update Type : $update_type"

			if [ -f $UpdateRootFsImageFile ]
			then
				echo "New Root File System Image File : $UpdateRootFsImageFile"			
			else
				echo "There is no root file system image file"
				ExitUpdateOnError
			fi

			MajorOldRootFsVersion=$(echo $OldRootFsVersion | cut -d'.' -f1)
			MinorOldRootFsVersion=$(echo $OldRootFsVersion | cut -d'.' -f2)
			
			MajorNewRootFsVersion=$(echo $root_fs_version | cut -d'.' -f1)
			MinorNewRootFsVersion=$(echo $root_fs_version | cut -d'.' -f2)

			if [ $MajorNewRootFsVersion -gt $MajorOldRootFsVersion -o $root_fs_version = '0.123' ]
			then
				echo "New Root File System Version :  $root_fs_version"
			elif [ $MajorOldRootFsVersion -eq $MajorNewRootFsVersion ]
			then
				if [ $MinorNewRootFsVersion -gt $MinorOldRootFsVersion ]
				then
					echo "New Root File System Version :  $root_fs_version"
				else
					echo "Current Root File System Version is newer or same as new one."
					ExitUpdateOnError
				fi
			else
				echo "Current Root File System Version is newer or same as new one."
				ExitUpdateOnError			
			fi
		elif [ $update_type = 'blackbox_both' ]
		then
			echo "Update Type : $update_type"

			if [ -f $UpdateKernelImageFile ]
			then
				echo "New Kernel Image File : $UpdateKernelImageFile"			
			else
				echo "There is no kernel image file"
				ExitUpdateOnError
			fi

			if [ -f $UpdateRootFsImageFile ]
			then
				echo "New Root File System Image File : $UpdateRootFsImageFile"			
			else
				echo "There is no root file system image file"
				ExitUpdateOnError
			fi

			MajorOldKernelVersion=$(echo $OldKernelVersion | cut -d'.' -f1)
			MinorOldKernelVersion=$(echo $OldKernelVersion | cut -d'.' -f2)
			
			MajorNewKernelVersion=$(echo $kernel_version | cut -d'.' -f1)
			MinorNewKernelVersion=$(echo $kernel_version | cut -d'.' -f2)

			if [ $MajorNewKernelVersion -gt $MajorOldKernelVersion -o $kernel_version = '0.123' ]
			then
				echo "New Kernel Version :  $kernel_version"
			elif [ $MajorNewKernelVersion -eq $MajorOldKernelVersion ]
			then
				if [ $MinorNewKernelVersion -gt $MinorOldKernelVersion ]
				then
					echo "New Kernel Version :  $kernel_version"
				else
					echo "Current Kernel Version is newer or same as new one."
					ExitUpdateOnError
				fi
			else
				echo "Current Kernel Version is newer or same as new one."
				ExitUpdateOnError			
			fi
			
			MajorOldRootFsVersion=$(echo $OldRootFsVersion | cut -d'.' -f1)
			MinorOldRootFsVersion=$(echo $OldRootFsVersion | cut -d'.' -f2)
			
			MajorNewRootFsVersion=$(echo $root_fs_version | cut -d'.' -f1)
			MinorNewRootFsVersion=$(echo $root_fs_version | cut -d'.' -f2)

			if [ $MajorNewRootFsVersion -gt $MajorOldRootFsVersion -o $root_fs_version = '0.123' ]
			then
				echo "New Root File System Version :  $root_fs_version"
			elif [ $MajorOldRootFsVersion -eq $MajorNewRootFsVersion ]
			then
				if [ $MinorNewRootFsVersion -gt $MinorOldRootFsVersion ]
				then
					echo "New Root File System Version :  $root_fs_version"
				else
					echo "Current Root File System Version is newer or same as new one."
					ExitUpdateOnError
				fi
			else
				echo "Current Root File System Version is newer or same as new one."
				ExitUpdateOnError			
			fi			
		else
			echo "Wrong type..."
			ExitUpdateOnError
		fi

	else
		echo "There is no update_info file"

		# process dev_postscript.sh - Start
		if [ -f /mnt/mmc/update/dev_postscript.sh ]
		then
			echo " Processing dev_postscript.sh ..."
			/mnt/mmc/update/dev_postscript.sh
			rm /mnt/mmc/update/dev_postscript.sh
		fi
		# process dev_postscript.sh - End

		exit  1
	fi
}

CheckMD5()
{
	if [ $update_type = 'blackbox_kernel' ]
	then				
		MD5CheckSum=$(md5sum $UpdateKernelImageFile | cut -d' ' -f1)
		
		if [ $MD5CheckSum = $kernel_md5 ]
		then
			echo "The kernel image file is ok"
		else
			echo "The kernel image file is currupted"
			ExitUpdateOnError
		fi
		
	elif [ $update_type = 'blackbox_rootfs' ]
	then
		MD5CheckSum=$(md5sum $UpdateRootFsImageFile | cut -d' ' -f1)
		
		if [ $MD5CheckSum = $root_fs_md5 ]
		then
			echo "The root file system image file is ok"
		else
			echo "The root file system image file is currupted"
			ExitUpdateOnError
		fi
	elif [ $update_type = 'blackbox_both' ]
	then
		MD5CheckSum=$(md5sum $UpdateKernelImageFile | cut -d' ' -f1)
		
		if [ $MD5CheckSum = $kernel_md5 ]
		then
			echo "The kernel image file is ok"
		else
			echo "The kernel image file is currupted"
			ExitUpdateOnError
		fi

		MD5CheckSum=$(md5sum $UpdateRootFsImageFile | cut -d' ' -f1)
		
		if [ $MD5CheckSum = $root_fs_md5 ]
		then
			echo "The root file system image file is ok"
		else
			echo "The root file system image file is currupted"
			ExitUpdateOnError
		fi
	fi
}

ControlLedOn()
{
	/opt/blackbox/LEDControlForUpdate.sh &
}

ControlLedOff()
{
	killall LEDControlForUpdate.sh
	echo 0 > /proc/gio/gio20
	echo 0 > /proc/gio/gio23
	echo 0 > /proc/gio/gio27
	echo 0 > /proc/gio/gio24
}

StartUpdate()
{
	echo "Start update process..."
	
	UBOOTBootcmd=$(/opt/blackbox/fw_printenv | grep "bootcmd=" | cut -d'=' -f2)
	
	UBOOTBootcmdKernel=$(echo $UBOOTBootcmd  | cut -d';' -f2)
	UBOOTBootcmdRootFs=$(echo $UBOOTBootcmd  | cut -d';' -f1)
	UBOOTBootcmdOther=$(echo $UBOOTBootcmd  | cut -d';' -f3)
	
	NandCurrentKernelAddr=$(echo $UBOOTBootcmdKernel  | cut -d' ' -f4)
	NandCurrentRootFsAddr=$(echo $UBOOTBootcmdRootFs | cut -d' ' -f4)
	
	NandNewKernelAddr=
	NandNewRootFsAddr=
	
	MTDNewKernelPartition=
	MTDNewRootFsPartition=
	
	if [ $update_type = 'blackbox_kernel' ]
	then				
		if [ $NandCurrentKernelAddr = '0x200000' ]
		then
			NandNewKernelAddr='0x400000'
			MTDNewKernelPartition='/dev/mtd2'
		elif [ $NandCurrentKernelAddr = '0x400000' ]
		then
			NandNewKernelAddr='0x200000'
			MTDNewKernelPartition='/dev/mtd1'
		else
			echo "Can not recognize Nand for kernel..."
			ExitUpdateOnError
		fi
		
		ControlLedOn
		
		flash_eraseall $MTDNewKernelPartition
		nandwrite -p $MTDNewKernelPartition $UpdateKernelImageFile
		
		UBOOTBootcmdKernel=$(echo $UBOOTBootcmdKernel | sed s/$NandCurrentKernelAddr/$NandNewKernelAddr/g)
		
		UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
		
		/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd

		mount -t jffs2 /dev/mtdblock5 /mnt/nand

		echo "kernel_version=$kernel_version" > /mnt/nand/Info/KernelVersion
		echo "kernel_version=$kernel_version" > /mnt/mmc/Info/KernelVersion
		
		umount /mnt/nand
	elif [ $update_type = 'blackbox_rootfs' ]
	then
		if [ $NandCurrentRootFsAddr = '0x600000' ]
		then
			NandNewRootFsAddr='0x1000000'
			MTDNewRootFsPartition='/dev/mtd4'
		elif [ $NandCurrentRootFsAddr = '0x1000000' ]
		then
			NandNewRootFsAddr='0x600000'
			MTDNewRootFsPartition='/dev/mtd3'
		else
			echo "Can not recognize Nand for root file system..."
			ExitUpdateOnError
		fi

		ControlLedOn
		
		flash_eraseall $MTDNewRootFsPartition
		nandwrite -p $MTDNewRootFsPartition $UpdateRootFsImageFile

		UBOOTBootcmdRootFs=$(echo $UBOOTBootcmdRootFs | sed s/$NandCurrentRootFsAddr/$NandNewRootFsAddr/g)

		UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
		
		/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd
		
		mount -t jffs2 /dev/mtdblock5 /mnt/nand
		echo "root_fs_version=$root_fs_version" > /mnt/nand/Info/RootFsVersion
		echo "root_fs_version=$root_fs_version" > /mnt/mmc/Info/RootFsVersion

		umount /mnt/nand


	elif [ $update_type = 'blackbox_both' ]
	then
		if [ $NandCurrentKernelAddr = '0x200000' ]
		then
			NandNewKernelAddr='0x400000'
			MTDNewKernelPartition='/dev/mtd2'
		elif [ $NandCurrentKernelAddr = '0x400000' ]
		then
			NandNewKernelAddr='0x200000'
			MTDNewKernelPartition='/dev/mtd1'
		else
			echo "Can not recognize Nand for kernel..."
			ExitUpdateOnError
		fi

		if [ $NandCurrentRootFsAddr = '0x600000' ]
		then
			NandNewRootFsAddr='0x1000000'
			MTDNewRootFsPartition='/dev/mtd4'
		elif [ $NandCurrentRootFsAddr = '0x1000000' ]
		then
			NandNewRootFsAddr='0x600000'
			MTDNewRootFsPartition='/dev/mtd3'
		else
			echo "Can not recognize Nand for root file system..."
			ExitUpdateOnError
		fi

		ControlLedOn
		
		flash_eraseall $MTDNewKernelPartition
		nandwrite -p $MTDNewKernelPartition $UpdateKernelImageFile

		flash_eraseall $MTDNewRootFsPartition
		nandwrite -p $MTDNewRootFsPartition $UpdateRootFsImageFile

		UBOOTBootcmdKernel=$(echo $UBOOTBootcmdKernel | sed s/$NandCurrentKernelAddr/$NandNewKernelAddr/g)
		UBOOTBootcmdRootFs=$(echo $UBOOTBootcmdRootFs | sed s/$NandCurrentRootFsAddr/$NandNewRootFsAddr/g)
		
		UBOOTBootcmd="$UBOOTBootcmdRootFs;$UBOOTBootcmdKernel;$UBOOTBootcmdOther"
		
		/opt/blackbox/fw_setenv bootcmd $UBOOTBootcmd

		mount -t jffs2 /dev/mtdblock5 /mnt/nand

		echo "kernel_version=$kernel_version" > /mnt/nand/Info/KernelVersion
		echo "root_fs_version=$root_fs_version" > /mnt/nand/Info/RootFsVersion

		echo "kernel_version=$kernel_version" > /mnt/mmc/Info/KernelVersion
		echo "root_fs_version=$kernel_version" > /mnt/mmc/Info/RootFsVersion

		umount /mnt/nand
	fi

# process dev_postscript.sh - Start
	if [ -f /mnt/mmc/update/dev_postscript.sh ]
	then
		echo " Processing dev_postscript.sh ..."
		/mnt/mmc/update/dev_postscript.sh
		rm /mnt/mmc/update/dev_postscript.sh
	fi
# process dev_postscript.sh - End

}

StartUpdate_wifi()
{
	echo "Start update process..."
	
	UBOOTBootcmd=$(/opt/blackbox/fw_printenv | grep bootcmd | cut -d'=' -f2)
	
	UBOOTBootcmdKernel=$(echo $UBOOTBootcmd  | cut -d';' -f2)
	UBOOTBootcmdRootFs=$(echo $UBOOTBootcmd  | cut -d';' -f1)
	UBOOTBootcmdOther=$(echo $UBOOTBootcmd  | cut -d';' -f3)
	
	NandCurrentKernelAddr=$(echo $UBOOTBootcmdKernel  | cut -d' ' -f4)
	NandCurrentRootFsAddr=$(echo $UBOOTBootcmdRootFs | cut -d' ' -f4)
	
	NandNewKernelAddr=
	NandNewRootFsAddr=
	
	MTDNewKernelPartition=
	MTDNewRootFsPartition=
	
	if [ $update_type = 'blackbox_kernel' ]
	then				
		ControlLedOn
		
		flash_eraseall /dev/mtd2
		nandwrite -p /dev/mtd2 $UpdateKernelImageFile
		
		mount -t jffs2 /dev/mtdblock5 /mnt/nand

		echo "kernel_version=$kernel_version" > /mnt/nand/Info/KernelVersion
		echo "kernel_version=$kernel_version" > /mnt/mmc/Info/KernelVersion
		
		umount /mnt/nand
	elif [ $update_type = 'blackbox_rootfs' ]
	then
		ControlLedOn
		
		flash_eraseall /dev/mtd3
		nandwrite -p /dev/mtd3 $UpdateRootFsImageFile

		mount -t jffs2 /dev/mtdblock5 /mnt/nand
		echo "root_fs_version=$root_fs_version" > /mnt/nand/Info/RootFsVersion
		echo "root_fs_version=$root_fs_version" > /mnt/mmc/Info/RootFsVersion

		umount /mnt/nand


	elif [ $update_type = 'blackbox_both' ]
	then
		ControlLedOn
		
		flash_eraseall /dev/mtd2
		nandwrite -p /dev/mtd2 $UpdateKernelImageFile

		flash_eraseall /dev/mtd3
		nandwrite -p /dev/mtd3 $UpdateRootFsImageFile

		mount -t jffs2 /dev/mtdblock5 /mnt/nand

		echo "kernel_version=$kernel_version" > /mnt/nand/Info/KernelVersion
		echo "root_fs_version=$root_fs_version" > /mnt/nand/Info/RootFsVersion

		echo "kernel_version=$kernel_version" > /mnt/mmc/Info/KernelVersion
		echo "root_fs_version=$kernel_version" > /mnt/mmc/Info/RootFsVersion

		umount /mnt/nand
	fi

# process dev_postscript.sh - Start
	if [ -f /mnt/mmc/update/dev_postscript.sh ]
	then
		echo " Processing dev_postscript.sh ..."
		/mnt/mmc/update/dev_postscript.sh
		rm /mnt/mmc/update/dev_postscript.sh
	fi
# process dev_postscript.sh - End

}


CompleteUpdate()
{
	rm -f $UpdateKernelImageFile
	rm -f $UpdateRootFsImageFile

	sync
	sync
	ControlLedOff
	
	umount /mnt/mmc
	
	LoopCount=0
	
	while [ $LoopCount -lt 5 ]
	do
	    echo 1 > /proc/gio/gio23
	    echo 1 > /proc/gio/gio24
	    sleep 1
	    echo 0 > /proc/gio/gio23
	    echo 0 > /proc/gio/gio24
	    sleep 1
	    LoopCount=`expr $LoopCount + 1`
	done
	
	reboot
}


if [ -z "$OLD_VERSION" ] ; then
    echo "WiFi Version=============================="
    echo "WiFi Version=============================="

    CheckUpdateInfo
    CheckMD5
    StartUpdate_wifi
    CompleteUpdate

    reboot

else
    echo "OLD Version=============================="
    echo "OLD Version=============================="

    CheckUpdateInfo
    CheckMD5
    StartUpdate
    CompleteUpdate
fi



