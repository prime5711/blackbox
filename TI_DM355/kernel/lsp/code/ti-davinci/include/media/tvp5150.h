/*                                                                        
    tvp5150.h - definition for tvp5150 inputs                           
                                                                        
    Copyright (C) 2006 Hans Verkuil (hverkuil@xs4all.nl)                
                                                                        
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or   
    (at your option) any later version.                                 
                                                                        
    This program is distributed in the hope that it will be useful,     
    but WITHOUT ANY WARRANTY; without even the implied warranty of      
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
    GNU General Public License for more details.                        
                                                                        
    You should have received a copy of the GNU General Public License   
    along with this program; if not, write to the Free Software         
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.           
*/                                                                      
                                                                        
//#ifndef _TVP5150_H_                                                     
//#define _TVP5150_H_                                                     

#ifndef TVP5150_H                                                     
#define TVP5150_H     
                                                                        
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#define TRUE 1
#define FALSE 0                                            
                                    
                                                                        
/* TVP5150 HW outputs */                                                                               

/* analog muxing mode */
#define    TVP5150_AMUX_COMPOSITE  0
#define    TVP5150_AMUX_SVIDEO     1

/* TVP5150 HW inputs */     
//#define		TVP5150_ANALOG_CH0  		0
//#define   TVP5150_SVIDEO  				1
//#define   TVP5150_ANALOG_CH1  		2
//#define   TVP5150_BLACK_SCREEN  	8


typedef enum {
	TVP5150_MODE_INV = -1,
	TVP5150_MODE_AUTO = 0,	/* autoswitch mode (default)   */
	TVP5150_MODE_NTSC = 1,	/* (M, J) NTSC      525-line   */
	TVP5150_MODE_PAL = 2,	/* (B, D, G, H, I, N) PAL      */
	TVP5150_MODE_PAL_M = 3,	/* (M) PAL          525-line   */
	TVP5150_MODE_PAL_CN = 4,	/* (Combination-N) PAL         */
	TVP5150_MODE_NTSC_443 = 5,	/* NTSC 4.43        525-line   */
	TVP5150_MODE_SECAM = 6,	/* SECAM                       */
	TVP5150_MODE_PAL_60 = 7,	/* PAL 60          525-line    */
	TVP5150_MODE_AUTO_SQP = 8,	/* autoswitch mode (default)   */
	TVP5150_MODE_NTSC_SQP = 9,	/* (M, J) NTSC      525-line   */
	TVP5150_MODE_PAL_SQP = 0xA,	/* (B, D, G, H, I, N) PAL      */
	TVP5150_MODE_PAL_M_SQP = 0xB,	/* (M) PAL          525-line   */
	TVP5150_MODE_PAL_CN_SQP = 0xC,	/* (Combination-N) PAL         */
	TVP5150_MODE_NTSC_443_SQP = 0xD,	/* NTSC 4.43 525-line   */
	TVP5150_MODE_SECAM_SQP = 0xE,	/* SECAM                       */
	TVP5150_MODE_PAL_60_SQP = 0xF,	/* PAL 60          525-line    */
} tvp5150_mode;


/* Enum for TVP5146 output data width */
typedef enum tvp5150_datawidth {
	TVP5150_WIDTH_16BIT,
	TVP5150_WIDTH_8BIT
}tvp5150_datawidth_t;


typedef struct {
	tvp5150_mode mode;
	int amuxmode;
	int enablebt656sync;
	tvp5150_datawidth_t data_width;
}tvp5150_params;



#ifdef __KERNEL__

typedef unsigned int tvp5150_cmd;

/* commands for setup the decoder */
#define TVP5150_SET_AMUXMODE        4
#define TVP5150_SET_BRIGHTNESS      5
#define TVP5150_SET_CONTRAST        6
#define TVP5150_SET_HUE             7
#define TVP5150_SET_SATURATION      8
#define TVP5150_SET_AGC             9
#define TVP5150_SET_VIDEOSTD        10
#define TVP5150_CLR_LOSTLOCK        11
#define TVP5150_CONFIG              12
#define TVP5150_RESET               13
#define TVP5150_POWERDOWN           14

#define TVP5150_GET_STATUS          15
#define TVP5150_GET_STD             16
#define TVP5150_INIT                17
#define TVP5150_CLEANUP             18

#define TVP5150_READ_ALL            19
#define TVP5150_TEST								20
#define TVP5150_ROMWRITE						21

//#define TVP5150_I2C_ADDR (0xBA >> 1)  // Pull up
#define TVP5150_I2C_ADDR (0xB8 >> 1)  // YIN7/I2C Slave Address Pull down in DM355  org code

//#define TVP5150_I2C_ADDR (0x90 >>1) //for test

/* Defines for TVP5150 register address */
#define TVP5150_OUTPUT_CONTROL     	(0x03)  // Miscellaneous Control Register


#define TVP5146_I2C_REGISTERED			(1)
#define TVP5146_I2C_UNREGISTERED		(0)





#define TVP5150_I2C_REGISTERED			(1)
#define TVP5150_I2C_UNREGISTERED		(0)

extern int tvp5150_ctrl(tvp5150_cmd cmd, void *arg);
                                                                        
#endif    
#endif   
                             
