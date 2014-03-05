/*
 * Misc useful routines to access NIC SROM
 *
 * Copyright (C) 2004 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef	_bcmsrom_h_
#define	_bcmsrom_h_

extern int srom_var_init(uint bus, void *curmap, void *osh, char **vars,
			 int *count);

extern int srom_read(uint bus, void *curmap, void *osh, uint byteoff,
		     uint nbytes, uint16 * buf);

extern int srom_parsecis(uint8 * cis, char **vars, int *count);

#endif				/* _bcmsrom_h_ */
