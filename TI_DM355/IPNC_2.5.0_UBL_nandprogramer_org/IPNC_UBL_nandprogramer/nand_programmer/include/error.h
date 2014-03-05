/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: error.h  												      	  */
/*	PURPOSE	: Error codes file						                              */
/*  PROJECT	: DM350 ROM boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Jan-30-2006                                                     */
/******************************************************************************/
#ifndef _ERROR_H_
#define _ERROR_H_
                     
#define TIMEOUT_COUNT 	10240  
#define SECURE_TIMEOUT 	0xFFFF0000
#define NAND_TIMEOUT 	10240
#define DMA_TIMEOUT 	0xFFFF0000
                     
#define E_PASS     0
#define E_FAIL     1
#define E_TIMEOUT  2
#define E_DEVICE   3
#define E_ERROR	   4

#endif/*_ERROR_H_*/
