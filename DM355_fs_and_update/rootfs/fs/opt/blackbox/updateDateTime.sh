#!/bin/sh

Config_file="/mnt/mmc/configuration/nc100_blackbox.conf"

if [ -f /mnt/mmc/update/date_time.cfg ]
then
	echo " Force to set system time..."
	tmpdatetime=$(grep -v ^# /mnt/mmc/update/date_time.cfg | grep [0-9][0-9][0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9]_[0-9][0-9] | cut -d' ' -f1)
	
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
		
			rm /mnt/mmc/update/date_time.cfg
		else
			echo "Invalid date&time info..."
		fi
	else
		echo "Invalid date&time info..."
	fi

#	if [ -f $Config_file ]
#	then
#		mount -t jffs2 /dev/mtdblock5 /mnt/nand
#    		echo "Configuration file update - $Config_file"
#    		cp $Config_file /mnt/nand/configuration/
#    		sync
#		umount /mnt/nand
#	else
#    		echo "Configuration file does not exist - $Config_file"
#	fi
fi
