/*
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

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/*----------------------- define TRUE, FALSE, NULL, bool ----------------*/
#ifdef __cplusplus

#ifndef FALSE
#define FALSE	false
#endif
#ifndef TRUE
#define TRUE	true
#endif

#else /* !__cplusplus */

#if defined(_WIN32)

typedef	unsigned char	bool;

#else

#if defined(MACOSX) && defined(KERNEL)
#include <IOKit/IOTypes.h>
#else
typedef	int	bool;
#endif

#endif

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1

#ifndef NULL
#define	NULL 0
#endif

#endif

#endif /* __cplusplus */

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif

/*----------------------- define uchar, ushort, uint, ulong ----------------*/

typedef unsigned char uchar;

#if defined(_WIN32) || defined(PMON) || defined(__MRC__) || defined(V2_HAL) || defined(_CFE_)

#ifndef V2_HAL
typedef unsigned short	ushort;
#endif

typedef unsigned int	uint;
typedef unsigned long	ulong;

#else

/* pick up ushort & uint from standard types.h */
#if defined(linux) && defined(__KERNEL__)
#include <linux/types.h>	/* sys/types.h and linux/types.h are oil and water */
#else
#include <sys/types.h>
#if !defined(TARGETENV_sun4) && !defined(linux)
typedef unsigned long	ulong;
#endif /* TARGETENV_sun4 */
#endif
#if defined(PMON)
typedef unsigned int	uint;
typedef unsigned long long       uint64;
#endif

#endif /* WIN32 || PMON || .. */

/*----------------------- define [u]int8/16/32/64 --------------------------*/


#ifdef V2_HAL
#include <bcmos.h>
#else
typedef signed char	int8;
typedef signed short	int16;
typedef signed int	int32;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
#endif	/* V2_HAL */

typedef float		float32;
typedef double		float64;

/*
 * abstracted floating point type allows for compile time selection of
 * single or double precision arithmetic.  Compiling with -DFLOAT32
 * selects single precision; the default is double precision.
 */

#if defined(FLOAT32)
typedef float32 float_t;
#else /* default to double precision floating point */
typedef float64 float_t;
#endif /* FLOAT32 */

#ifdef _MSC_VER	    /* Microsoft C */
typedef signed __int64	int64;
typedef unsigned __int64 uint64;

#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
/* gcc understands signed/unsigned 64 bit types, but complains in ANSI mode */
typedef signed long long int64;
typedef unsigned long long uint64;

#elif defined(__ICL) && !defined(__STDC__)
/* ICL accepts unsigned 64 bit type only, and complains in ANSI mode */
typedef unsigned long long uint64;

#endif /* _MSC_VER */


/*----------------------- define PTRSZ, INLINE --------------------------*/

#define	PTRSZ	sizeof (char*)

#ifndef INLINE

#ifdef _MSC_VER

#define INLINE __inline

#elif __GNUC__

#define INLINE __inline__

#else

#define INLINE

#endif /* _MSC_VER */

#endif /* INLINE */

#endif /* _TYPEDEFS_H_ */
