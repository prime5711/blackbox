#/bin/sh
# NC1000S version initialize...

DEFUALT_VERSION='0.001'

Kernel_image_file='/mnt/mmc/update/uImage_NC1000S_2.0M_2.0M_2014_0307_2346_rev306_mtd_11'
Rootfs_image_file='/mnt/mmc/update/nc1000s_fs_kurefa_wiz_no_mtd5_jffs2_system_server_2013_0307.bin.gz'
#    PC_Viwer_file='/mnt/mmc/update/PC_Viewer.exe_'
#      Config_file='/mnt/mmc/update/nc100_blackbox.conf'
      Script_file='/mnt/mmc/update/dev_postscript.sh.bak'



echo "Start update process..."

UNAME_VERISON_MATCH=$( uname -v | grep "306 Fri Mar 7 23:46:45 KST 2014")

if [ -n $UNAME_VERISON_MATCH ]
then
	echo "Non Match"
else
	echo "Match"

fi
