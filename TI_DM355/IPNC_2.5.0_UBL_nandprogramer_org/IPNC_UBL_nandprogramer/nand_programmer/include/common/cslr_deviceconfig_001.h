#ifndef _CSLR_DEVICECONFIG_1_H_
#define _CSLR_DEVICECONFIG_1_H_
/*********************************************************************
 * Copyright (C) 2003-2004 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_deviceconfig_1.h
 * 
 * \brief This file contains the Register Desciptions for DeviceConfig
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PINMUX0;
    volatile Uint32 PINMUX1;
    volatile Uint32 BOOTCFG;
} CSL_DeviceconfigRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* PINMUX0 */

#define CSL_DEVICECONFIG_PINMUX0_EMACEN_MASK (0x80000000u)
#define CSL_DEVICECONFIG_PINMUX0_EMACEN_SHIFT (0x0000001Fu)
#define CSL_DEVICECONFIG_PINMUX0_EMACEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_1394EN_MASK (0x40000000u)
#define CSL_DEVICECONFIG_PINMUX0_1394EN_SHIFT (0x0000001Eu)
#define CSL_DEVICECONFIG_PINMUX0_1394EN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_HPIEN_MASK (0x20000000u)
#define CSL_DEVICECONFIG_PINMUX0_HPIEN_SHIFT (0x0000001Du)
#define CSL_DEVICECONFIG_PINMUX0_HPIEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_CFLDEN_MASK (0x08000000u)
#define CSL_DEVICECONFIG_PINMUX0_CFLDEN_SHIFT (0x0000001Bu)
#define CSL_DEVICECONFIG_PINMUX0_CFLDEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_CWEN_MASK (0x04000000u)
#define CSL_DEVICECONFIG_PINMUX0_CWEN_SHIFT (0x0000001Au)
#define CSL_DEVICECONFIG_PINMUX0_CWEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_LFLDEN_MASK (0x02000000u)
#define CSL_DEVICECONFIG_PINMUX0_LFLDEN_SHIFT (0x00000019u)
#define CSL_DEVICECONFIG_PINMUX0_LFLDEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_LOEEN_MASK (0x01000000u)
#define CSL_DEVICECONFIG_PINMUX0_LOEEN_SHIFT (0x00000018u)
#define CSL_DEVICECONFIG_PINMUX0_LOEEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_RGB888_MASK (0x00800000u)
#define CSL_DEVICECONFIG_PINMUX0_RGB888_SHIFT (0x00000017u)
#define CSL_DEVICECONFIG_PINMUX0_RGB888_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_ATAEN_MASK (0x00020000u)
#define CSL_DEVICECONFIG_PINMUX0_ATAEN_SHIFT (0x00000011u)
#define CSL_DEVICECONFIG_PINMUX0_ATAEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_HDIREN_MASK (0x00010000u)
#define CSL_DEVICECONFIG_PINMUX0_HDIREN_SHIFT (0x00000010u)
#define CSL_DEVICECONFIG_PINMUX0_HDIREN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_VLYNQEN_MASK (0x00008000u)
#define CSL_DEVICECONFIG_PINMUX0_VLYNQEN_SHIFT (0x0000000Fu)
#define CSL_DEVICECONFIG_PINMUX0_VLYNQEN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_VLSCREN_MASK (0x00004000u)
#define CSL_DEVICECONFIG_PINMUX0_VLSCREN_SHIFT (0x0000000Eu)
#define CSL_DEVICECONFIG_PINMUX0_VLSCREN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_VLYNQWD_MASK (0x00003000u)
#define CSL_DEVICECONFIG_PINMUX0_VLYNQWD_SHIFT (0x0000000Cu)
#define CSL_DEVICECONFIG_PINMUX0_VLYNQWD_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_AECS5_MASK (0x00000800u)
#define CSL_DEVICECONFIG_PINMUX0_AECS5_SHIFT (0x0000000Bu)
#define CSL_DEVICECONFIG_PINMUX0_AECS5_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_AECS4_MASK (0x00000400u)
#define CSL_DEVICECONFIG_PINMUX0_AECS4_SHIFT (0x0000000Au)
#define CSL_DEVICECONFIG_PINMUX0_AECS4_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_AEAW_MASK (0x0000001Fu)
#define CSL_DEVICECONFIG_PINMUX0_AEAW_SHIFT (0x00000000u)
#define CSL_DEVICECONFIG_PINMUX0_AEAW_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX0_RESETVAL (0x00000000u)

/* PINMUX1 */

#define CSL_DEVICECONFIG_PINMUX1_TIMIN_MASK (0x00040000u)
#define CSL_DEVICECONFIG_PINMUX1_TIMIN_SHIFT (0x00000012u)
#define CSL_DEVICECONFIG_PINMUX1_TIMIN_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_CLK1_MASK (0x00020000u)
#define CSL_DEVICECONFIG_PINMUX1_CLK1_SHIFT (0x00000011u)
#define CSL_DEVICECONFIG_PINMUX1_CLK1_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_CLK0_MASK (0x00010000u)
#define CSL_DEVICECONFIG_PINMUX1_CLK0_SHIFT (0x00000010u)
#define CSL_DEVICECONFIG_PINMUX1_CLK0_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_MCBSP_MASK (0x00000400u)
#define CSL_DEVICECONFIG_PINMUX1_MCBSP_SHIFT (0x0000000Au)
#define CSL_DEVICECONFIG_PINMUX1_MCBSP_RESETVAL (0x00000000u)

/*----MCBSP Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_MCBSP_MCBSP_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_MCBSP_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_MSTK_MASK (0x00000200u)
#define CSL_DEVICECONFIG_PINMUX1_MSTK_SHIFT (0x00000009u)
#define CSL_DEVICECONFIG_PINMUX1_MSTK_RESETVAL (0x00000000u)

/*----MSTK Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_MSTK_SD_PIN_ENABLE (0x00000000u)
#define CSL_DEVICECONFIG_PINMUX1_MSTK_MS_PIN_ENABLE (0x00000001u)

#define CSL_DEVICECONFIG_PINMUX1_SPI_MASK (0x00000100u)
#define CSL_DEVICECONFIG_PINMUX1_SPI_SHIFT (0x00000008u)
#define CSL_DEVICECONFIG_PINMUX1_SPI_RESETVAL (0x00000000u)

/*----SPI Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_SPI_SPI_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_SPI_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_I2C_MASK (0x00000080u)
#define CSL_DEVICECONFIG_PINMUX1_I2C_SHIFT (0x00000007u)
#define CSL_DEVICECONFIG_PINMUX1_I2C_RESETVAL (0x00000000u)

/*----I2C Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_I2C_I2C_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_I2C_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_PWM2_MASK (0x00000040u)
#define CSL_DEVICECONFIG_PINMUX1_PWM2_SHIFT (0x00000006u)
#define CSL_DEVICECONFIG_PINMUX1_PWM2_RESETVAL (0x00000000u)

/*----PWM2 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_PWM2_PWM2_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_PWM2_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_PWM1_MASK (0x00000020u)
#define CSL_DEVICECONFIG_PINMUX1_PWM1_SHIFT (0x00000005u)
#define CSL_DEVICECONFIG_PINMUX1_PWM1_RESETVAL (0x00000000u)

/*----PWM1 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_PWM1_PWM1_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_PWM1_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_PWM0_MASK (0x00000010u)
#define CSL_DEVICECONFIG_PINMUX1_PWM0_SHIFT (0x00000004u)
#define CSL_DEVICECONFIG_PINMUX1_PWM0_RESETVAL (0x00000000u)

/*----PWM0 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_PWM0_PWM0_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_PWM0_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_U2FLO_MASK (0x00000008u)
#define CSL_DEVICECONFIG_PINMUX1_U2FLO_SHIFT (0x00000003u)
#define CSL_DEVICECONFIG_PINMUX1_U2FLO_RESETVAL (0x00000000u)

/*----U2FLO Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_U2FLO_UART2AUTOFLOW_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_U2FLO_CI_CCD_PIN_ENABLE (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_UART2_MASK (0x00000004u)
#define CSL_DEVICECONFIG_PINMUX1_UART2_SHIFT (0x00000002u)
#define CSL_DEVICECONFIG_PINMUX1_UART2_RESETVAL (0x00000000u)

/*----UART2 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_UART2_UART2_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_UART2_CI_CCD_PIN_ENABLE (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_UART1_MASK (0x00000002u)
#define CSL_DEVICECONFIG_PINMUX1_UART1_SHIFT (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_UART1_RESETVAL (0x00000000u)

/*----UART1 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_UART1_UART1_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_UART1_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_UART0_MASK (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_UART0_SHIFT (0x00000000u)
#define CSL_DEVICECONFIG_PINMUX1_UART0_RESETVAL (0x00000000u)

/*----UART0 Tokens----*/
#define CSL_DEVICECONFIG_PINMUX1_UART0_UART0_PIN_ENABLE (0x00000001u)
#define CSL_DEVICECONFIG_PINMUX1_UART0_GPIO_PIN (0x00000000u)

#define CSL_DEVICECONFIG_PINMUX1_RESETVAL (0x00000000u)

/* BOOTCFG */

#define CSL_DEVICECONFIG_BOOTCFG_DSPBOOT_MASK (0x00000100u)
#define CSL_DEVICECONFIG_BOOTCFG_DSPBOOT_SHIFT (0x00000008u)
#define CSL_DEVICECONFIG_BOOTCFG_DSPBOOT_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_BOOTCFG_BOOTSEL_MASK (0x00000080u)
#define CSL_DEVICECONFIG_BOOTCFG_BOOTSEL_SHIFT (0x00000007u)
#define CSL_DEVICECONFIG_BOOTCFG_BOOTSEL_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_BOOTCFG_CE0_WIDTH_MASK (0x00000040u)
#define CSL_DEVICECONFIG_BOOTCFG_CE0_WIDTH_SHIFT (0x00000006u)
#define CSL_DEVICECONFIG_BOOTCFG_CE0_WIDTH_RESETVAL (0x00000000u)

#define CSL_DEVICECONFIG_BOOTCFG_AEAW_MASK (0x0000001Fu)
#define CSL_DEVICECONFIG_BOOTCFG_AEAW_SHIFT (0x00000000u)
#define CSL_DEVICECONFIG_BOOTCFG_AEAW_RESETVAL (0x00000000u)

#define CSL_DeviceConfig_BOOTCFG_RESETVAL (0x00000000u)

#endif

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 2         27 Jul 2004 12:52:07    2195             xkeshavm      */
/*                                                                  */
/* To change base addr of pinmux and added more function |Ds       */
/********************************************************************/ 