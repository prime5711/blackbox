/*****************************************************************************/
/*                                                                           */
/*  Copyright (c) 2003 - 2004, MultiMedia Solutions Ltd.                     */
/*  All rights reserved.                                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  Name:           pinmux.c                                                 */
/*  Autor:          Dinko Mironov                                            */
/*  Description:    Pin Mux Support                                          */
/*  Project:        Monta Vista Linux - DaVinci                              */
/*                                                                           */
/*****************************************************************************/
/*  History:                                                                 */
/*  Version Date        Author          Reason                               */
/*                                                                           */
/*  1.0     05.06.2006  Dinko           Ported to DaVinci                    */
/*                                                                           */
/*****************************************************************************/

/**************************************************************************
 * Included Files
 **************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include <asm/arch/io.h>
#include <asm/arch/hardware.h>

typedef struct {
	unsigned long AEAW:5;
	unsigned long reserved0:5;
	unsigned long AECS4:1;
	unsigned long AECS5:1;
	unsigned long VLYNQWD:2;
	unsigned long VLSCREN:1;
	unsigned long VLYNQEN:1;
	unsigned long HDIREN:1;
	unsigned long ATAEN:1;
	unsigned long reserved1:4;
	unsigned long RGB666:1;
	unsigned long RGB888:1;
	unsigned long LOEEN:1;
	unsigned long LFLDEN:1;
	unsigned long CWEN:1;
	unsigned long CFLDEN:1;
	unsigned long reserved2:3;
	unsigned long EMACEN:1;
} pinmux_reg0_t;

typedef struct {
	unsigned long UART0:1;
	unsigned long UART1:1;
	unsigned long UART2:1;
	unsigned long U2FLO:1;
	unsigned long PWM0:1;
	unsigned long PWM1:1;
	unsigned long PWM2:1;
	unsigned long I2C:1;
	unsigned long SPI:1;
	unsigned long MSTK:1;
	unsigned long McBSP:1;
	unsigned long reserved2:5;
	unsigned long CLK0:1;
	unsigned long CLK1:1;
	unsigned long TIMIN:1;
} pinmux_reg1_t;

pinmux_reg0_t *pinmux_reg0;
pinmux_reg1_t *pinmux_reg1;

#if CONFIG_PROC_FS

char pinmux_gio52[32] =
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
};
char pinmux_gio53[32] =
    { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
};
char pinmux_gio[32] =
    { 2, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22
};

#define MAX_BUF		4096

static int proc_pinmux(char *page, char **start, off_t off,
		       int count, int *eof, void *data)
{
	int len = 0;
	char buffer[MAX_BUF];
	unsigned long *pmux_reg;

	pmux_reg = (unsigned long *)pinmux_reg0;
	len += sprintf(&buffer[len], "PINMUX0 = 0x%08x\n", (int)*pmux_reg);
	if (pinmux_gio53[pinmux_reg0->AEAW]) {
		len +=
		    sprintf(&buffer[len],
			    "  %2d   AEAW     AEMIF address width selection "
			    "on ", pinmux_reg0->AEAW);
		len += sprintf(&buffer[len], "GPIO53,");
	}
	if (pinmux_gio52[pinmux_reg0->AEAW]) {
		len +=
		    sprintf(&buffer[len],
			    "  %2d   AEAW     AEMIF address width selection "
			    "on ", pinmux_reg0->AEAW);
		len += sprintf(&buffer[len], "GPIO52,");
	}
	if (pinmux_gio[pinmux_reg0->AEAW]) {
		len +=
		    sprintf(&buffer[len],
			    "  %2d   AEAW     Number of address bits enabled "
			    "on the AEMIF - ", pinmux_reg0->AEAW);
		len +=
		    sprintf(&buffer[len], "%d", pinmux_gio[pinmux_reg0->AEAW]);
	}
	len += sprintf(&buffer[len], "\n");
	len +=
	    sprintf(&buffer[len],
		    "  %2d   AECS4    Enable AEMIF ECS4 chip select\n",
		    pinmux_reg0->AECS4);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   AECS5    Enable AEMIF ECS5 chip select\n",
		    pinmux_reg0->AECS5);
	len +=
	    sprintf(&buffer[len], "  %2d   VLYNQWD  Sets VLYNQ data width\n",
		    pinmux_reg0->VLYNQWD);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   VLSCREN  Enable VLYNQ SCRUN pin function\n",
		    pinmux_reg0->VLSCREN);
	len +=
	    sprintf(&buffer[len], "  %2d   VLYNQEN  Enable VLYNQ module pins\n",
		    pinmux_reg0->VLYNQEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   HDIREN   Enable ATADIR DIR output on "
		    "GPIO42/SPI_EN1\n", pinmux_reg0->HDIREN);
	len +=
	    sprintf(&buffer[len], "  %2d   ATAEN    Enable ATA module pins\n",
		    pinmux_reg0->ATAEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   RGB666   Enable VideoOut extended RGB666 mode "
		    "outputs on GIOs\n", pinmux_reg0->RGB666);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   RGB888   Enable VideoOut extended RGB888 mode "
		    "outputs on GIOs\n", pinmux_reg0->RGB888);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   LOEEN    Enable LCD_OE pin function on GPIO0\n",
		    pinmux_reg0->LOEEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   LFLDEN   Enable LCD_FIELD pin function on GPIO1\n",
		    pinmux_reg0->LFLDEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   CWEN     Enable CCD _C_WEN function on GPIO3/B0\n",
		    pinmux_reg0->CWEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   CFLDEN   Enable CCD C_FIELD pin function on "
		    "GPIO4/R0\n", pinmux_reg0->CFLDEN);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   EMACEN   Enable EMAC and MDIO function on "
		    "default GPIO3V[0:16] pins.\n", pinmux_reg0->EMACEN);

	pmux_reg = (unsigned long *)pinmux_reg1;
	len += sprintf(&buffer[len], "PINMUX1 = 0x%08x\n", (int)*pmux_reg);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   UART0    Enable UART0 on GPIO[35:36]\n",
		    pinmux_reg1->UART0);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   UART1    Enable UART1 on ATA DMACK and DMARQ\n",
		    pinmux_reg1->UART1);
	len +=
	    sprintf(&buffer[len], "  %2d   UART2    Enable UART2\n",
		    pinmux_reg1->UART2);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   U2FLO    Enable UART2 flow control on Upper "
		    "CCD/Chroma signals\n", pinmux_reg1->U2FLO);
	len +=
	    sprintf(&buffer[len], "  %2d   PWM0     Enable PWM0 on GPIO45\n",
		    pinmux_reg1->PWM0);
	len +=
	    sprintf(&buffer[len], "  %2d   PWM1     Enable PWM1 on GPIO46/R2\n",
		    pinmux_reg1->PWM1);
	len +=
	    sprintf(&buffer[len], "  %2d   PWM2     Enable PWM2 on GPIO47/B2\n",
		    pinmux_reg1->PWM2);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   I2C      Enable I2C on GPIO[43:44]\n",
		    pinmux_reg1->I2C);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   SPI      Enable SPI on GPIO[37,39,40,41,42]\n",
		    pinmux_reg1->SPI);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   MSTK     Enable Memory Stick/MMC/SD\n",
		    pinmux_reg1->MSTK);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   McBSP    Enable McBSP function on default "
		    "GPIO[29:34] pins\n", pinmux_reg1->McBSP);
	len +=
	    sprintf(&buffer[len], "  %2d   CLK0     Enable CLKOUT0 on GPIO48\n",
		    pinmux_reg1->CLK0);
	len +=
	    sprintf(&buffer[len], "  %2d   CLK1     Enable CLKOUT1 on GPIO49\n",
		    pinmux_reg1->CLK1);
	len +=
	    sprintf(&buffer[len],
		    "  %2d   TIMIN    Enable TIM_IN on GPIO49 / CLKOUT1\n",
		    pinmux_reg1->TIMIN);

	if (count > len)
		count = len;
	memcpy(page, &buffer[off], count);

	*eof = 1;
	*start = NULL;
	return len;
}

int __init pinmux_proc_client_create(void)
{
	create_proc_read_entry("pinmux", 0, NULL, proc_pinmux, NULL);

	pinmux_reg0 =
	    (pinmux_reg0_t *) IO_ADDRESS(DAVINCI_SYSTEM_MODULE_BASE + 0x00);
	pinmux_reg1 =
	    (pinmux_reg1_t *) IO_ADDRESS(DAVINCI_SYSTEM_MODULE_BASE + 0x04);

	return 0;
}

#else				/* CONFIG_PROC_FS */

#define pinmux_proc_client_create() do {} while(0);

#endif				/* CONFIG_PROC_FS */

module_init(pinmux_proc_client_create);
