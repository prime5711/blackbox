/*
 * linux/sound/oss/davinci-aic26.h
 *
 * Glue driver for AIC33 for Davinci processors
 *
 * Copyright (C) 2006 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * History:
 *  -------
 *  2005-10-18 Rishi Bhattacharya - Support for AIC33 codec and Davinci DM644x Processor
 */

#ifndef __ASM_ARCH_AIC26_H
#define __ASM_ARCH_AIC26_H



//Page 0 Registers
#define AIC26_DAT_BAT1  0x80A0  //0x05
#define AIC26_DAT_BAT2  0x80C0  //0x06
#define AIC26_DAT_AUX   0x80E0  //0x07
#define AIC26_DAT_TMP1  0x8120  //0x09
#define AIC26_DAT_TMP2  0x8140  //0x0A

//Page 1 Registers
#define AIC26_ADC      0x8800  //0x00
#define AIC26_STA      0x8820  //0x01
#define AIC26_REF      0x8860  //0x03
#define AIC26_RST      0x8880  //0x04

//Page 2 Registers
#define AIC26_AUD_CNT1 0x9000  //0x00
#define AIC26_ADC_GAIN 0x9020  //0x01
#define AIC26_DAC_GAIN 0x9040  //0x02
#define AIC26_SIDETONE 0x9060  //0x03
#define AIC26_AUD_CNT2 0x9080  //0x04
#define AIC26_PWR_CNT  0x90A0  //0x05
#define AIC26_AUD_CNT3 0x90C0  //0x06
#define AIC26_BBFC_LN0 0x90E0  //0x07
#define AIC26_BBFC_LN1 0x9100  //0x08
#define AIC26_BBFC_LN2 0x9120  //0x09
#define AIC26_BBFC_LN3 0x9140  //0x0A
#define AIC26_BBFC_LN4 0x9160  //0x0B
#define AIC26_BBFC_LN5 0x9180  //0x0C
#define AIC26_BBFC_LD1 0x91A0  //0x0D
#define AIC26_BBFC_LD2 0x91C0  //0x0E
#define AIC26_BBFC_LD4 0x91E0  //0x0F
#define AIC26_BBFC_LD5 0x9200  //0x10
#define AIC26_BBFC_RN0 0x9220  //0x11
#define AIC26_BBFC_RN1 0x9240  //0x12
#define AIC26_BBFC_RN2 0x9260  //0x13
#define AIC26_BBFC_RN3 0x9280  //0x14
#define AIC26_BBFC_RN4 0x92A0  //0x15
#define AIC26_BBFC_RN5 0x92C0  //0x16
#define AIC26_BBFC_RD1 0x92E0  //0x17
#define AIC26_BBFC_RD2 0x9300  //0x18
#define AIC26_BBFC_RD4 0x9320  //0x19
#define AIC26_BBFC_RD5 0x9340  //0x1A
#define AIC26_PLL_PRG1 0x9360  //0x1B
#define AIC26_PLL_PRG2 0x9380  //0x1C
#define AIC26_AUD_CNT4 0x93A0  //0x1D
#define AIC26_AUD_CNT5 0x93C0  //0x1E


// PLL Programming registerA 3
#define PLL_ENABLE              0x80



// Codec Datapath setup register 7
#define FS_REF_44_1             0x80
#define FS_REF_DEFAULT_48       0x00
#define ADC_DUAL_RATE_MODE      0x40
#define DAC_DUAL_RATE_MODE      0x20
#define LDAC_LCHAN              0x08
#define RDAC_RCHAN              0x02



//MICBIAS control register 25
#define MICBIAS_OUTPUT_2_0V     0x40
#define MICBIAS_OUTPUT_2_5V     0x80
#define MICBIAS_OUTPUT_AVDD     0xC0


//any to LOP/M Volume control
#define LOPM_ON                 0x80



#define LIV_MAX                         0x0077
#define LIV_MIN                         0x0000

#define LHV_MAX                         0x0077
#define LHV_MIN                         0x0000

#define LIG_MAX							0x0077
#define LIG_MIN							0x0000

#define LOG_MAX							0x007f
#define LOG_MIN							0x0000

#endif				/* __ASM_ARCH_AIC26_H */
