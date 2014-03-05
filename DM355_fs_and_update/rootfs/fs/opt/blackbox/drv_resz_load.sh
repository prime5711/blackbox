#!/bin/sh

module="csl"

# invoke insmod with all arguments we got
insmod ./$module.ko || exit 1

# retrieve major number
major=$(awk '$2=="csl" {print $1}' /proc/devices)

rm -f /dev/${module}
mknod /dev/${module} c $major 0






