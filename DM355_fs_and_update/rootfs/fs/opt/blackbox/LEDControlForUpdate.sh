#!/bin/sh
# Control LEDs for Update

while [ 1 ]
do
    echo 0 > /proc/gio/gio20
    echo 0 > /proc/gio/gio23
    echo 1 > /proc/gio/gio27
    echo 1 > /proc/gio/gio24
    sleep 1
    echo 0 > /proc/gio/gio27
    echo 0 > /proc/gio/gio24
    echo 1 > /proc/gio/gio20
    echo 1 > /proc/gio/gio23
    sleep 1
done
