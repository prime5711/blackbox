#
# Makefile for Broadcom shared driver code
#
# Copyright (C) 2004 Broadcom Corporation
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
#

O_TARGET	:= hnd.o

HND_OBJS	:= bcmutils.o hnddma.o linux_osl.o sbutils.o bcmsrom.o

ifneq ($(CONFIG_BCM947XX),y)
HND_OBJS	+= nvramstubs.o
endif

export-objs	:= shared_ksyms.o
obj-y		:= shared_ksyms.o $(HND_OBJS)
obj-m		:= $(O_TARGET)

SRCBASE		:= $(TOPDIR)/arch/mips/brcm/s5
EXTRA_CFLAGS	+= -I$(TOPDIR)/arch/mips/brcm/s5

vpath %.c $(SRCBASE)

include $(TOPDIR)/Rules.make

ifeq ($(wildcard $(SRCBASE)/bcmutils.c),)
bcmutils.o: $(SRCBASE)/shared/bcmutils.o
	cp $< $@
endif

ifeq ($(wildcard $(SRCBASE)/hnddma.c),)
hnddma.o: $(SRCBASE)/shared/hnddma.o
	cp $< $@
endif

ifeq ($(wildcard $(SRCBASE)/linux_osl.c),)
linux_osl.o: $(SRCBASE)/shared/linux_osl.o
	cp $< $@
endif

ifeq ($(wildcard $(SRCBASE)/sbutils.c),)
sbutils.o: $(SRCBASE)/shared/sbutils.o
	cp $< $@
endif

ifeq ($(wildcard $(SRCBASE)/bcmsrom.c),)
bcmsrom.o: $(SRCBASE)/shared/bcmsrom.o
        cp $< $@
endif

ifeq ($(wildcard $(SRCBASE)/nvramstubs.c),)
nvramstubs.o: $(SRCBASE)/shared/nvramstubs.o
	cp $< $@
endif

shared_ksyms.c: shared_ksyms.sh $(HND_OBJS)
	sh -e $< $(HND_OBJS) > $@
