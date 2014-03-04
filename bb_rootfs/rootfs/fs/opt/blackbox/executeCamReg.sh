#!/bin/sh
cat /mnt/mmc/camera_reg.txt|grep 0x*|grep ",0x"|awk '{split($1, subchar,","); printf("./execute_i2c 0x5d w 0x%x 0x%x\n", subchar[1], subchar[2]);}'|sh
cat /mnt/mmc/camera_reg.txt|grep 0x*|grep ",0x"|awk '{split($1, subchar,","); printf("./execute_i2c 0x5d r 0x%x\n", subchar[1]);}'|sh
