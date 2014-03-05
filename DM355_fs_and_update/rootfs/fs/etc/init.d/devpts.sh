#! /bin/sh

# chkconfig: S 35 0

set -e

. /etc/default/devpts

[ "$(uname -s)" = "Linux" ] || exit 0

#
#	First find out if devpts is available. Also check if devfs
#	is already mounted - in that case we don't want to use devpts.
#

if ! grep -qis '[<[:space:]]/dev[>[:space:]].*devfs' /proc/mounts &&
	grep -qis '[<[:space:]]devpts' /proc/filesystems
then
	#
	#	Create mountpoint and multiplexor device.
	#
	if [ ! -d /dev/pts ] ; then
		mkdir /dev/pts && chmod 0755 /dev/pts
	fi
	if [ ! -c /dev/ptmx ] ; then
		mknod /dev/ptmx c 5 2 && chmod 0666 /dev/ptmx
	fi

	#
	#	Mount /dev/pts if needed.
	#
	if ! grep -qis '/dev/pts' /proc/mounts
	then
		mount -t devpts devpts /dev/pts -ogid=${TTYGRP},mode=${TTYMODE}
	fi
fi
