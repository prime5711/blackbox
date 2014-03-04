#!/bin/sh
# for initializing blackbox 

if [ $# -eq 0 ]
then
	echo "######## Please input the date & time information ########"
	echo "Usage : initSystem.sh YYYY_MM_DD_hh_mm_ss"
	echo "        ex ) 2010/05/03 - 15:16:00"
	echo "        system $ ./initSystem.sh 2010_05_03_15_16_00"
#	exit 1
else
	echo " Force to set system time..."
	tmpdatetime=$(echo $1 | grep [0-9][0-9][0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9] | cut -d' ' -f1)
	
	if [ $tmpdatetime ]
	then
		year=$(echo $tmpdatetime | cut -d'_' -f1)
		month=$(echo $tmpdatetime | cut -d'_' -f2)
		day=$(echo $tmpdatetime | cut -d'_' -f3)
		hour=$(echo $tmpdatetime | cut -d'_' -f4)
		minute=$(echo $tmpdatetime | cut -d'_' -f5)
		second=$(echo $tmpdatetime | cut -d'_' -f6)
		
		if [ $year -a $month -a $day -a $hour -a $minute -a $second ]
		then
			echo "set system time to ... $year/$month/$day $hour/$minute/$second ..."
			datetime="$month$day$hour$minute$year.$second"
			
			date $datetime
			hwclock -w
		else
			echo "Invalid time info..."
			exit 1
		fi
	else
		echo "Invalid time info..."
		exit 1
	fi
fi

flash_eraseall -j /dev/mtd5
mount -t jffs2 /dev/mtdblock5 /mnt/nand

mkdir -p /mnt/nand/configuration
mkdir -p /mnt/nand/pc_viewer
mkdir -p /mnt/nand/Info

if [ -f /mnt/mmc/default/nc100_blackbox.conf ]
then
	cp /mnt/mmc/default/nc100_blackbox.conf /mnt/nand/configuration/
else
	echo "Configuration file does not exist - /mnt/mmc/default/nc100_blackbox.conf"
fi

if [ -f /mnt/mmc/default/NaviCAM.exe ]
then
	cp /mnt/mmc/default/NaviCAM.exe /mnt/nand/pc_viewer/
else
	echo "PC Viewer file does not exist - /mnt/mmc/default/NaviCAM.exe"
fi

echo "kernel_version=0.001" > /mnt/nand/Info/KernelVersion
echo "root_fs_version=0.001" > /mnt/nand/Info/RootFsVersion

umount /mnt/nand