#!/bin/sh
#
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

cat <<EOF
#include <linux/config.h>
#include <linux/module.h>
extern void bcm_ctype; EXPORT_SYMBOL(bcm_ctype);
EOF

for file in $* ; do
    ${NM} $file | sed -ne 's/[0-9A-Fa-f]* T \([^ ]*\)/extern void \1; EXPORT_SYMBOL(\1);/p'
done
