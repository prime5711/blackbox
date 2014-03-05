/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: tistdtypes.h  												      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _TISTDTYPES_H_
#define _TISTDTYPES_H_

/*
    The purpose of this header file is to consolidate all the primitive "C"
    data types into one file. This file is expected to be included in the
    basic types file exported by other software components, for example CSL.
 */

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

#ifndef TRUE

typedef int		Bool;
#define TRUE		((Bool) 1)
#define FALSE		((Bool) 0)

#endif

typedef int             Int;
typedef unsigned int    Uns;    /* deprecated type */
typedef char            Char;
typedef char *          String;
typedef void *          Ptr;

/* unsigned quantities */
typedef unsigned int   	Uint32;
typedef unsigned short 	Uint16;
typedef unsigned char   Uint8;

/* signed quantities */
typedef int             Int32;
typedef short           Int16;
typedef char            Int8;

/* volatile unsigned quantities */
typedef volatile unsigned int       VUint32;
typedef volatile unsigned short     VUint16;
typedef volatile unsigned char      VUint8;

/* volatile signed quantities */
typedef volatile int                VInt32;
typedef volatile short              VInt16;
typedef volatile char               VInt8;

#endif /* _TI_STD_TYPES */


#endif /* _TISTDTYPES_H_ */

