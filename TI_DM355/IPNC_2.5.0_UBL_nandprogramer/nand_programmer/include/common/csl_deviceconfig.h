#ifndef __CSL_DEVICECONFIG_H__
#define __CSL_DEVICECONFIG_H__

#define AEMIF_ADDRESS_WIDTH AEMIF_A_W
#define VLYNQ_WIDTH VLYNQ_WORD
//PIN MUX 0

//Enable EMAC Module Pins
#define CSL_PINMUX0_EMAC_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_EMACEN, 1) 

//DISABLE EMAC Module Pins
#define CSL_PINMUX0_EMAC_DISABLE     	CSL_FMK(DEVICECONFIG_PINMUX0_EMACEN, 0) 

//Enable 1394 Module Pins(if EMAC not enable)
#define CSL_PINMUX0_1394_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_1394EN, 1) 

//Disable 1394 Module Pins(if EMAC not enable)
#define CSL_PINMUX0_1394_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_1394EN, 0) 

//Enable UHPI Module Pins
#define CSL_PINMUX0_HPI_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_HPIEN, 1) 

//Disable UHPI Module Pins
#define CSL_PINMUX0_HPI_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_HPIEN, 0) 

//Enable CCD C_FIELD pin function on GPIO4
#define CSL_PINMUX0_CFL_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_CFLDEN, 1) 

//Disable CCD C_FIELD pin function on GPIO4
#define CSL_PINMUX0_CFL_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_CFLDEN, 0) 

//Enable CCD_C_WEN pin function on GPIO3
#define CSL_PINMUX0_CW_ENABLE      		CSL_FMK(DEVICECONFIG_PINMUX0_CWEN, 1) 

//Disable CCD_C_WEN pin function on GPIO3
#define CSL_PINMUX0_CW_DISABLE     		CSL_FMK(DEVICECONFIG_PINMUX0_CWEN, 0) 

//Enable LCD_FIELD pin function on GPIO1
#define CSL_PINMUX0_LCD_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_LFLDEN, 1) 

//Disable LCD_FIELD pin function on GPIO1
#define CSL_PINMUX0_LCD_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_LFLDEN, 0) 

//Enable LCD_OE pin function on GPIO1
#define CSL_PINMUX0_LOE_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_LOEEN, 1) 

//Disable LCD_OE pin function on GPIO1
#define CSL_PINMUX0_LOE_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_LOEEN, 0) 

//Enable VideoOut extended RGB888 mode outputs
#define CSL_PINMUX0_RGB888_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_RGB888, 1) 

//Disable VideoOut extended RGB888 mode outputs
#define CSL_PINMUX0_RGB888_DISABLE     	CSL_FMK(DEVICECONFIG_PINMUX0_RGB888, 0) 

//Enable ATA Module pins
#define CSL_PINMUX0_ATA_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_ATAEN, 1) 

//Disable ATA Module pins
#define CSL_PINMUX0_ATA_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_ATAEN, 0) 

//Enable ATADIR DIR output
#define CSL_PINMUX0_HDIREN_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_HDIREN, 1) 

//Disable ATADIR DIR output
#define CSL_PINMUX0_HDIREN_DISABLE     	CSL_FMK(DEVICECONFIG_PINMUX0_HDIREN, 0) 

//Enable VLYNQ Module Pins
#define CSL_PINMUX0_VLYNQ_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_VLYNQEN, 1) 

//Disable VLYNQ Module Pins
#define CSL_PINMUX0_VLYNQ_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_VLYNQEN, 0) 

//Enable VLYNQ scrun PIN Pins
#define CSL_PINMUX0_VLYNQ_SCRUN_ENABLE  CSL_FMK(DEVICECONFIG_PINMUX0_VLSCREN, 1) 

//DISABLE VLYNQ scrun PIN Pins
#define CSL_PINMUX0_VLYNQ_SCRUN_DISABLE  CSL_FMK(DEVICECONFIG_PINMUX0_VLSCREN, 0) 

//Set VLYNQ data width
#define CSL_PINMUX0_VLYNQWD			    CSL_FMK(DEVICECONFIG_PINMUX0_VLYNQWD, VLYNQ_WORD) 

//Enable AEMIF ECS5 Chip select
#define CSL_PINMUX0_AECS5_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_AECS5, 1) 

//Disable AEMIF ECS5 Chip select
#define CSL_PINMUX0_AECS5_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_AECS5, 0) 

//Enable AEMIF ECS4 Chip select
#define CSL_PINMUX0_AECS4_ENABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_AECS4, 1) 

//DISABLE AEMIF ECS4 Chip select
#define CSL_PINMUX0_AECS4_DISABLE      	CSL_FMK(DEVICECONFIG_PINMUX0_AECS4, 0) 

//AEMIF address width selection
#define CSL_PINMUX0_AEMIF_ADDR_WIDTH  	CSL_FMK(DEVICECONFIG_PINMUX0_AEAW, AEMIF_ADDRESS_WIDTH) 


//PINMUX1 Selection

//Enable TIM_IN on GPIO[49]
#define CSL_PINMUX1_TIM_IN_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_TIMIN, 1) 

//Disable TIM_IN on GPIO[49]
#define CSL_PINMUX1_TIM_IN_DISABLE 		CSL_FMK(DEVICECONFIG_PINMUX1_TIMIN, 0) 

//Enable CLKOUT1  on GPIO[49]
#define CSL_PINMUX1_CLK1_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_CLK1, 1) 

//Disable CLKOUT1  on GPIO[49]
#define CSL_PINMUX1_CLK1_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_CLK1, 0) 

//Enable CLKOUT0  on GPIO[48]
#define CSL_PINMUX1_CLK0_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_CLK0, 1) 

//Disable CLKOUT0  on GPIO[48]
#define CSL_PINMUX1_CLK0_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_CLK0, 0) 

//Enable McBSP
#define CSL_PINMUX1_McBSP_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_MCBSP, 1) 

//Disable McBSP
#define CSL_PINMUX1_McBSP_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_MCBSP, 0) 

//Enable Memory Stick
#define CSL_PINMUX1_MSTK_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_MSTK, 1) 

//Disable Memory Stick
#define CSL_PINMUX1_MSTK_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_MSTK, 0) 

//Enable SPI
#define CSL_PINMUX1_SPI_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_SPI, 1) 

//DISABLE SPI
#define CSL_PINMUX1_SPI_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_SPI, 0) 

//Enable I2C
#define CSL_PINMUX1_I2C_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_I2C, 1) 

//Disable I2C
#define CSL_PINMUX1_I2C_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_I2C, 0) 

//Enable PWM2
#define CSL_PINMUX1_PWM2_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM2, 1) 

//Disable PWM2
#define CSL_PINMUX1_PWM2_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM2, 0) 

//Enable PWM1
#define CSL_PINMUX1_PWM1_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM1, 1) 

//Disable PWM1
#define CSL_PINMUX1_PWM1_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM1, 0) 

//Enable PWM0
#define CSL_PINMUX1_PWM0_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM0, 1) 

//Disable PWM0
#define CSL_PINMUX1_PWM0_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_PWM0, 0) 

//Enable U2FLO
#define CSL_PINMUX1_U2FLO_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_U2FLO, 1) 

//Disable U2FLO
#define CSL_PINMUX1_U2FLO_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_U2FLO, 0) 

//Enable UART2
#define CSL_PINMUX1_UART2_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_UART2, 1) 

//Disable UART2
#define CSL_PINMUX1_UART2_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_UART2, 0) 

//Enable UART1
#define CSL_PINMUX1_UART1_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_UART1, 1) 

//Disable UART1
#define CSL_PINMUX1_UART1_DISABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_UART1, 0) 

//Enable UART0
#define CSL_PINMUX1_UART_0_ENABLE  		CSL_FMK(DEVICECONFIG_PINMUX1_UART0, 1) 

//Disable UART0
#define CSL_PINMUX1_UART_0_DISABLE 		CSL_FMK(DEVICECONFIG_PINMUX1_UART0, 0) 

typedef enum {
pinMuxReg0,
pinMuxReg1
}pinMuxReg;
#endif


/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         25 Jun 2004 10:31:03    1881             xkeshavm      */
/*                                                                  */
/* Command header files added to develop test cases                 */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 2         27 Jul 2004 12:52:06    2195             xkeshavm      */
/*                                                                  */
/* To change base addr of pinmux and added more function |Ds       */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 3          Sep 09:52:17 9         2632             xkeshavm      */
/*                                                                  */
/* To remove csl suffix                                             */
/********************************************************************/ 
