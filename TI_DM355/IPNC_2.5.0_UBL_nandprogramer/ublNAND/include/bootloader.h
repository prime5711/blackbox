/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: bootloader.h  											      */
/*	PURPOSE	: Linker file						                              */
/*  PROJECT	: DM350 UBL boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/

#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_
//#define PROFILE

#include <cslr_intc.h>
#include <cslr_psc.h>  

#include <error.h>

#ifdef PROFILE      
#include <cslr_tmr.h>
#endif

#include <tistdtypes.h>

/* searches in Block 8,9 */
#define APP_DESC_START_BLOCK_NUM 8
#define APP_DESC_END_BLOCK_NUM 9 


#define APP_MAGIC_NUMBER_VALID (0xB1ACED00)
#define APP_MAGIC_SAFE		   (0x00)		/* Safe boot mode */
#define APP_MAGIC_DMA          (0x11)		/* DMA boot mode  */
#define APP_MAGIC_IC  		   (0x22)		/* I Cache boot mode */
#define APP_MAGIC_FAST  	   (0x33)		/* Fast EMIF boot mode */
#define APP_MAGIC_DMA_IC   	   (0x44)	    /* DMA + I Cache boot mode */
#define APP_MAGIC_DMA_IC_FAST  (0x55)	    /* DMA + I Cache + Fast EMIF boot mode */

#define BL_MEMDDR	(0x80000000) /* starting address in DDR where main application will be copied */

/* PSC constants */

#define LPSC_GPIO			26      /*GPIO LPSC*/
#define LPSC_MMCSD0			15      /*MMCSD0 LPSC*/

#define PD0	0		/*ALWAYS ON POWER DOMAIN*/

/* System module registers */
#define PINMUX0			((volatile Uint32*)0x01C40000)
#define PINMUX1	        ((volatile Uint32*)0x01C40004)
#define PINMUX2         ((volatile Uint32*)0x01C40008)
#define PINMUX3         ((volatile Uint32*)0x01C4000C)
#define PINMUX4			((volatile Uint32*)0x01C40010)
#define BOOTCFG 		((volatile Uint32*)0x01C40014)
#define PHY_CTRL1       ((volatile Uint32*)0x020000E4)
#define IVT_FIQ_ADD     ((volatile Uint32*)0x1C)

#define BL_MEM 					(0x80000000) 	/* starting address in IRAM where User boot loader will be copied */
#define BOOTCFG_MASK_0X000000C0 (0x000000C0)
#define BOOTCFG_MASK_0x00000100 (0x00000100) /* For FASTBOOT*/


typedef struct {
	Uint32 block;		/* starting block number where actual application is stored */
	Uint32 destination;	/* Application copy start address in DDR	*/
	Uint32 entrypoint;	/* Entry point of the main application */
	Uint32 pages;		/* Application size in page numbers */
	Uint32 compression;	/* flag for de-compressing the application */
} APP_BOOT;



/* Function prototypes */
Uint32 LPSCEnable(Uint8 PdNum, Uint8 LPSCNUM);
void EnableICache();
void DisableICache();
void waitloop (Uint32 count);
Uint8 MagicSwitch(Uint32 magicNum);
#endif //_BOOTLOADER_H_



