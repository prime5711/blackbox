#
# (C) Copyright 2002
# Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
# David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
#
# (C) Copyright 2003
# Texas Instruments, <www.ti.com>
# Swaminathan <swami.iyer@ti.com>
#
# Davinci EVM board (ARM925EJS) cpu
# see http://www.ti.com/ for more information on Texas Instruments
#
# Davinci EVM has 1 bank of 256 MB DDR RAM 
# Physical Address:
# 8000'0000 to 9000'0000
#
#
# Linux-Kernel is expected to be at 8000'8000, entry 8000'8000
# (mem base + reserved)
#
# we load ourself to 8100 '0000
#
#

#Provide a atleast 16MB spacing between us and the Linux Kernel image
TEXT_BASE = 0x81080000
BOARDLIBS = drivers/nand/libnand.a
