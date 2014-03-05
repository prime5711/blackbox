/*
 *
 * Copyright (C) 2004 Texas Instruments.
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 Modifications:
 ver. 1.0: Mar 2007, Suresh Rajashekara (Based on the file davinci.c by
 *                                       Swaminathan S)
 *
 */

#include <common.h>
#include <i2c.h>

#define PLL0_PLLM   *(volatile unsigned int *)0x01C40910
#define PLL1_PLLM   *(volatile unsigned int *)0x01C40D10
#define PLL1_DIV1   *(volatile unsigned char *)0x01C40D18

void davinci_hd_psc_enable(void);

/*******************************************
 Routine: delay
 Description:  Delay function
*******************************************/
static inline void delay (unsigned long loops)
{
    __asm__ volatile ("1:\n"
		      "subs %0, %1, #1\n"
		      "bne 1b":"=r" (loops):"0" (loops));
}

/*******************************************
 Routine: board_init
 Description:  Board Initialization routine
*******************************************/
int board_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;

	/* Arch Number. __Need to register__ */
    gd->bd->bi_arch_number = 1500;
	/* 1500 is a random number chosen at the time of development. We have
	   not yet registered ourselves with the ARM development community. Once
	   thats done, please change the number to the one supplied by the ARM
	   development community and replace it with a macro.*/

      	/* Adress of boot parameters */
    gd->bd->bi_boot_params = LINUX_BOOT_PARAM_ADDR;

	/* Configure MUX settings ? */

	/* Power on required peripherals */
    davinci_hd_psc_enable();

    inittimer ();

    return 0;
}

#define PTCMD               *( volatile unsigned int* )( 0x01C41120 )
#define PTSTAT              *( volatile unsigned int* )( 0x01C41128 )
#define PDSTAT              *( volatile unsigned int* )( 0x01C41200 )
#define PDCTL               *( volatile unsigned int* )( 0x01C41300 )

/* PSC Registers */
#define PSC_ADDR            0x01C41000

#define PTCMD               ( PSC_ADDR + 0x120 ) /* Power domain transition
						  * commmand register */
#define PTSTAT              ( PSC_ADDR + 0x128 ) /* Power domain transition status
						  * register */

/**************************************
 Routine: davinci_hd_psc_enable
 Description:  Enable PSC domains
**************************************/
void davinci_hd_psc_enable ( void )
{
    unsigned int alwaysOnPdNum = 0, dspPdNum = 1, i;
    int waiting;
    unsigned int state;

	/* Note this function assumes that the Power Domains are already on */

    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*14) = *(unsigned int*) (PSC_ADDR+0xA00+4*14) | 0x003; /* EMAC */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*20) = *(unsigned int*) (PSC_ADDR+0xA00+4*20) | 0x003; /* DDR2 */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*21) = *(unsigned int*) (PSC_ADDR+0xA00+4*21) | 0x003; /* EMIFA */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*26) = *(unsigned int*) (PSC_ADDR+0xA00+4*26) | 0x003; /* UART0 */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*31) = *(unsigned int*) (PSC_ADDR+0xA00+4*31) | 0x003; /* I2C */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*35) = *(unsigned int*) (PSC_ADDR+0xA00+4*34) | 0x003; /* TIMER0 */
    *(volatile unsigned int*) (PSC_ADDR+0xA00+4*35) = *(unsigned int*) (PSC_ADDR+0xA00+4*35) | 0x003; /* TIMER1 */

	/* Set PTCMD.GO to 0x1 to initiate the state transtion for Modules in
	 * the ALWAYSON Power Domain */
    *(volatile unsigned int*) PTCMD = (1<<alwaysOnPdNum);

	/* Wait for PTSTAT.GOSTAT0 to clear to 0x0 */
    while(! (((*(volatile unsigned int*) PTSTAT >> alwaysOnPdNum) & 0x00000001) == 0));

	/* Enable GIO3.3V cells used for EMAC (???) */
#define VDD3P3V_PWDN        0x01c40048
    *(volatile unsigned int*) VDD3P3V_PWDN = 0;

#define PINMUX0     0x01C40000
#define PINMUX1     0x01C40004

	/* Select UART function on UART0 */
    *(volatile unsigned int *)PINMUX0 &= ~(0x0000003f << 18);
    *(volatile unsigned int *)PINMUX1 &= ~(0x00000003);

	/* Enable AEMIF pins */
    *(volatile unsigned int*) PINMUX0 &= ~(0x00000007);
}

/******************************
 Routine: misc_init_r
 Description:  Misc. init
******************************/
int misc_init_r (void)
{
    char temp[20];
    char rtcdata[10] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    char emac_read_addr [10] = { 0x7f, 0 }, i= 0;
    int clk = 0;

    clk = ((PLL1_PLLM + 1) * 27) / (PLL1_DIV1 + 1);

    printf ("ARM Clock :- %dMHz\n", ((((PLL0_PLLM + 1) * 27 ) / 2)) );
    printf ("DDR Clock :- %dMHz\n", (clk/2));

    i2c_write (0x50, 0x00, 1, emac_read_addr, 2); /* ?? */
    i2c_read (0x50, 0x00, 1, emac_read_addr, 6);
    temp[0] = (emac_read_addr[0] & 0xF0) >> 4;
    temp[1] = (emac_read_addr[0] & 0x0F);
    temp[2] = ':';
    temp[3] = (emac_read_addr[1] & 0xF0) >> 4;
    temp[4] = (emac_read_addr[1] & 0x0F);
    temp[5] = ':';
    temp[6] = (emac_read_addr[2] & 0xF0) >> 4;
    temp[7] = (emac_read_addr[2] & 0x0F);
    temp[8] = ':';
    temp[9] = (emac_read_addr[3] & 0xF0) >> 4;
    temp[10]= (emac_read_addr[3] & 0x0F);
    temp[11]= ':';
    temp[12]= (emac_read_addr[4] & 0xF0) >> 4;
    temp[13]= (emac_read_addr[4] & 0x0F);
    temp[14]= ':';
    temp[15]= (emac_read_addr[5] & 0xF0) >> 4;
    temp[16]= (emac_read_addr[5] & 0x0F);
                                                                               
    for (i = 0; i < 17; i++)
    {
        if (temp[i] == ':')
	    continue;
        else if (temp[i] >= 0 && temp[i] <= 9)
	    temp[i] = temp[i] + 48;
        else
	    temp[i] = temp[i] + 87;
    }
                   
    temp [17] = 0;                                                            
    if ((emac_read_addr [0] != 0xFF) ||
	(emac_read_addr [1] != 0xFF) ||	
	(emac_read_addr [2] != 0xFF) ||	
	(emac_read_addr [3] != 0xFF) ||	
	(emac_read_addr [4] != 0xFF) ||	
	(emac_read_addr [5] != 0xFF))
    { 
	setenv ("ethaddr", temp);
    }

    return (0);
}

/******************************
 Routine: dram_init
 Description:  Memory Info
******************************/
int dram_init (void)
{
    DECLARE_GLOBAL_DATA_PTR;

    gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
    gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

    return 0;
}

