#!/bin/sh

DATE_DATA="/tmp/date"

CUR_DATE=$(date +"%Y%m%d")
BEFORE_DATE=$(grep -v ^# $DATE_DATA | tr -d '[:alpha:]')

echo "CURRENT=$CUR_DATE"
echo "BEFORE=$BEFORE_DATE"


if [ -z $BEFORE_DATE ]
then
	echo "$CUR_DATE" > /tmp/date
	echo "new date $CUR_DATE"
	exit
fi

DIFF_DATE=$(($CUR_DATE - $BEFORE_DATE))
echo "$CUR_DATE - $BEFORE_DATE = $DIFF_DATE"

if [ $DIFF_DATE -gt 2 ]
then 
	echo "$CUR_DATE" > /tmp/date
	echo "$CUR_DATE - $BEFORE_DATE = $DIFF_DATE" >> /tmp/datelog
	echo "$CUR_DATE"

	killall -9 SystemWatcher
	killall -2 system_server
	sleep 2

	/opt/blackbox/killall.sh
	sleep 1

	rmmod cmemk.ko
	insmod cmemk.ko phys_start=0x84E00000 phys_end=0x88000000 pools=9x4096,3x8192,1x12288,2x20480,1x28672,2x40960,1x81920,1x126976,2x692224,2x2621440,1x4411392,1x40108032
	/opt/blackbox/autorun.sh
fi 

