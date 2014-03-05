/*
 * DM355 SPI controller driver with Interrupt.
*/


#include "dm355_spi_master.h"

#if defined(CONFIG_SPI_DEBUG)
#define DEBUG_SPI(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define DEBUG_SPI(fmt,arg...)
#endif

#define ENTER DEBUG_SPI("[Ei %s-%d] \n", __FUNCTION__, __LINE__);
#define EXIT  DEBUG_SPI("[Ex %s-%d] \n", __FUNCTION__, __LINE__);


/* operating momde selection from kconfig */
dm355_spi_config_t dm355_spi_config;

#define dm355_SPI_RX_BUF(type) 					  \
static inline void dm355_spi_rx_buf_##type(u32 data, struct dm355_spi *dm355_spi) \
{									  \
	type * rx = dm355_spi->rx;					  \
	*rx++ = (type)data;						  \
	dm355_spi->rx = rx;						  \
}

#define dm355_SPI_TX_BUF(type)				\
static inline u32 dm355_spi_tx_buf_##type(struct dm355_spi *dm355_spi)	\
{								\
	u32 data;						\
	const type * tx = dm355_spi->tx;			\
	data = *tx++;						\
	dm355_spi->tx = tx;					\
	return data;						\
}

dm355_SPI_RX_BUF(u8);
dm355_SPI_RX_BUF(u16);
dm355_SPI_TX_BUF(u8);
dm355_SPI_TX_BUF(u16);

#define SP0_MUX_SEL_LINE \
PINMUX3 |= 0x10000000;\
PINMUX4 &= 0xfffffffc

#define SP1_MUX_SEL_LINE \
PINMUX3 |= 0x0f800000

#define SP2_MUX_SEL_LINE \
PINMUX0 |= 0xeA

/*
 * Interface to control the chip select signal
 */
static void dm355_spi_chipselect(struct spi_device *spi, int value)
{
        struct dm355_spi *dm355_spi;
        ENTER dm355_spi = spi_master_get_devdata(spi->master);
        EXIT
            /* board specific chip select logic decides the polarity and cs line for the controller */
            if (value == BITBANG_CS_ACTIVE)
                return;
        else
                return;
}

/**
 * dm355_spi_setup_transfer - This functions will determine transfer method
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function determines data transfer method (8/16/32 bit transfer).
 * It will also set the SPI Clock Control register according to 
 * SPI slave device freq.
 */
static int dm355_spi_setup_transfer(struct spi_device *spi,
                                    struct spi_transfer *t)
{

        struct dm355_spi *dm355_spi;
        u8 bits_per_word = 0;
        u32 hz = 0;
        int i = 0;

        ENTER dm355_spi = spi_master_get_devdata(spi->master);

        spi->controller_data = NULL;
        spi->controller_data = &dm355_spi_config;

        if (t) {
                bits_per_word = t->bits_per_word;
                hz = t->speed_hz;
        }

        /* if bits_per_word is not set then set it default */
        if (!bits_per_word)
                bits_per_word = spi->bits_per_word;

        /* Assign function pointer to appropriate transfer method 8bit/16bit or 32bit transfer */
        if (bits_per_word <= 8 && bits_per_word >= 2) {
                DEBUG_SPI("BITS PER WORD = %d\n", bits_per_word);
                dm355_spi->get_rx = dm355_spi_rx_buf_u8;
                dm355_spi->get_tx = dm355_spi_tx_buf_u8;
                dm355_spi->slave[spi->chip_select].bytes_per_word = 1;
        } else if (bits_per_word <= 16 && bits_per_word >= 2) {
                DEBUG_SPI("BITS PER WORD = %d\n", bits_per_word);
                dm355_spi->get_rx = dm355_spi_rx_buf_u16;
                dm355_spi->get_tx = dm355_spi_tx_buf_u16;
                dm355_spi->slave[spi->chip_select].bytes_per_word = 2;
        } else
                return ERROR;


        if (!hz) {
                hz = spi->max_speed_hz;
                if (!hz) {
                        hz = 2000000;   /* defaulting to 2Mhz */
                        printk
                            ("[SPI] -> Slave device speed not set correctly. Trying with %dHz\n",
                             hz);
                }
        }

/**************************************************************************/

        for (i = 0; i < 4; i++) {
                dm355_spi->base->SPIFMT[i] &= ~(SPI_SPIFMT_CHARLEN_MASK);
                dm355_spi->base->SPIFMT[i] |= bits_per_word;
        }

        EXIT return 0;
}


/**
 * dm355_spi_setup - This functions will set default transfer method
 * @spi: spi device on which data transfer to be done
 *
 * This functions sets the default transfer method.
 */

static int dm355_spi_setup(struct spi_device *spi)
{
        int retval;
        struct dm355_spi *dm355_spi;

        ENTER dm355_spi = spi_master_get_devdata(spi->master);

        /*if bits per word length is zero then set it default 8 */
        if (!spi->bits_per_word)
                spi->bits_per_word = 8;

        dm355_spi->slave[spi->chip_select].cmd_to_write = 0;

        retval = dm355_spi_setup_transfer(spi, NULL);

        EXIT return retval;
}


/**
 * dm355_spi_bufs - functions which will handle transfer data
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function will put data to be transferred into data register
 * of SPI controller and then wait untill the completion will be marked
 * by the IRQ Handler.
 */
#define BIT_CLEAR_TIME_OUT	10000

/* #define SPI_PROFILE 1 */

#ifdef SPI_PROFILE

static unsigned int spi_prof_start;
static unsigned int spi_prof_end;
static unsigned int spi_avg_latency;

#define rdtscl(dest) \
	    __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest))

static void calculate_spi_latency(void)
{
        unsigned int diff = spi_prof_end - spi_prof_start;

        if (diff < 0) {
                printk("Count register wraparound not supported.\n");
        } else {
                if (spi_avg_latency == 0) {
                        spi_avg_latency = diff;
                } else {
                        spi_avg_latency += diff;
                        spi_avg_latency /= 2;
                }
        }
}

void spi_start_profile(void)
{
        rdtscl(spi_prof_start);
}

void spi_end_profile(void)
{
        rdtscl(spi_prof_end);
        calculate_spi_latency();
        DEBUG_SPI("[SPI-PROFILE] Throughput = %d\n", spi_avg_latency);
}

unsigned int spi_get_avg_latency()
{
        return spi_avg_latency;
}
#endif                          /* SPI_PROFILE */


static int dm355_spi_bufs(struct spi_device *spi, struct spi_transfer *t)
{
        struct dm355_spi *dm355_spi;
        int i, intStatus = 0;
        u8 conv = 1;
        u32 tx_data = 0;
        u32 data1_reg_val = 0;

        dm355_spi_config_t *spi_cfg;
        u32 rx_data = 0;
        u32 sPIPC0 = 0;
        volatile u32 buf_val, flg_val;

        ENTER dm355_spi = spi_master_get_devdata(spi->master);

        dm355_spi->tx = t->tx_buf;
        dm355_spi->rx = t->rx_buf;

        //DEBUG_SPI("tx_buf value = %x\n",*(u16 *)dm355_spi->tx);

        /* convert len to words bbased on bits_per_word */
        conv = dm355_spi->slave[spi->chip_select].bytes_per_word;

        dm355_spi->count = t->len / conv;

        DEBUG_SPI("chipselect=%d, bytes_per_word=%d, t->len=%d, conv=%d\n",
              spi->chip_select,
              dm355_spi->slave[spi->chip_select].bytes_per_word, t->len,
              conv);

        INIT_COMPLETION(dm355_spi->done);


        /*configuraton parameter for SPI */

        spi_cfg = (dm355_spi_config_t *) spi->controller_data;

        for (i = 0; i < 4; i++) {

                if (spi_cfg->phaseIn)
                        dm355_spi->base->SPIFMT[i] |=
                            SPI_SPIFMT_PHASE_MASK;
                else
                        dm355_spi->base->SPIFMT[i] &=
                            ~(SPI_SPIFMT_PHASE_MASK);

                if (spi_cfg->clkHigh)
                        dm355_spi->base->SPIFMT[i] |=
                            SPI_SPIFMT_POLARITY_MASK;
                else
                        dm355_spi->base->SPIFMT[i] &=
                            ~(SPI_SPIFMT_POLARITY_MASK);

                if (spi_cfg->lsbFirst)
                        dm355_spi->base->SPIFMT[i] |=
                            SPI_SPIFMT_SHIFTDIR_MASK;
                else
                        dm355_spi->base->SPIFMT[i] &=
                            ~(SPI_SPIFMT_SHIFTDIR_MASK);

        }

        /*Enable SPI */
        dm355_spi->base->SPIGCR1 |= SPI_SPIGCR1_SPIENA_MASK;


        /*Clock internal */
        if (spi_cfg->clkInternal)
                dm355_spi->base->SPIGCR1 |= SPI_SPIGCR1_CLKMOD_MASK;
        else
                dm355_spi->base->SPIGCR1 &= ~(SPI_SPIGCR1_CLKMOD_MASK);

        /* master mode default */
        dm355_spi->base->SPIGCR1 |= SPI_SPIGCR1_MASTER_MASK;

        /*CD default = 0xFF */
        dm355_spi->base->SPIDEF |= CS_DEFAULT;

        if (spi_cfg->intrLevel)
                dm355_spi->base->SPILVL = SPI_INTLVL_1;
        else
                dm355_spi->base->SPILVL = SPI_INTLVL_0;


        switch (spi_cfg->pinOpModes) {

        case SPI_OPMODE_3PIN:
                sPIPC0 |= (SPI_SPIPC0_DIFUN_DI << SPI_SPIPC0_DIFUN_SHIFT)
                    | (SPI_SPIPC0_DOFUN_DO << SPI_SPIPC0_DOFUN_SHIFT)
                    | (SPI_SPIPC0_CLKFUN_CLK << SPI_SPIPC0_CLKFUN_SHIFT);

                dm355_spi->base->SPIPC0 = sPIPC0;

                break;

        case SPI_OPMODE_SPISCS_4PIN:
                sPIPC0 |= (SPI_SPIPC0_DIFUN_DI << SPI_SPIPC0_DIFUN_SHIFT)
                    | (SPI_SPIPC0_DOFUN_DO << SPI_SPIPC0_DOFUN_SHIFT)
                    | (SPI_SPIPC0_CLKFUN_CLK << SPI_SPIPC0_CLKFUN_SHIFT)
                    | (SPI_SPIPC0_EN1FUN_EN1 << SPI_SPIPC0_EN1FUN_SHIFT)
                    | (SPI_SPIPC0_EN0FUN_EN0 << SPI_SPIPC0_EN0FUN_SHIFT);

                dm355_spi->base->SPIPC0 = sPIPC0;

                break;

        case SPI_OPMODE_SPIENA_4PIN:
                sPIPC0 |= (SPI_SPIPC0_DIFUN_DI << SPI_SPIPC0_DIFUN_SHIFT)
                    | (SPI_SPIPC0_DOFUN_DO << SPI_SPIPC0_DOFUN_SHIFT)
                    | (SPI_SPIPC0_CLKFUN_CLK << SPI_SPIPC0_CLKFUN_SHIFT)
                    | (SPI_SPIPC0_SPIENA << SPI_SPIPC0_SPIENA_SHIFT);

                dm355_spi->base->SPIPC0 = sPIPC0;
                break;

        default:
                return -1;
        }

        if (spi_cfg->loopBack)
                dm355_spi->base->SPIGCR1 |= SPI_SPIGCR1_LOOPBACK_MASK;
        else
                dm355_spi->base->SPIGCR1 &= ~(SPI_SPIGCR1_LOOPBACK_MASK);


        /*Put delay val if required */
        dm355_spi->base->SPIDELAY = 0
                | ( 8 << 24 )   // C2TDELAY
                | ( 8 << 16 );  // T2CDELAY

                data1_reg_val |=
                    spi_cfg->csHold << SPI_SPIDAT1_CSHOLD_SHIFT;
                data1_reg_val |=
            /*spi->chip_select*/2 << SPI_SPIDAT1_CSNR_SHIFT;
        /* Determine the command to execute READ or WRITE */
        if (t->tx_buf) {
                u8 flag = 0;

                /* set the flag for DM355_EEPROM_READ */
                if ( t->len == 3 && ((u8 *)t->tx_buf)[0] == 0x3 )
                    flag = 1;
                dm355_spi->base->SPIINT &= ~(SPI_SPIINT_MASKALL);

                i = 0;
                dm355_spi->base->SPIBUF;
                while (1) {
                        buf_val = dm355_spi->base->SPIBUF;
                        if ((buf_val & SPI_SPIBUF_TXFULL_MASK) == FALSE) {
                                tx_data = dm355_spi->get_tx(dm355_spi);
                                DEBUG_SPI("data to be TX = 0x%x\n", tx_data);
                                i++;
         
                                /* READ is treated differenetly here when count to */
                                if ( i  > dm355_spi->count )
                                    dm355_spi->base->SPIDAT1 = (data1_reg_val & 0x0ffcffff) | tx_data ;
                                else
                                    dm355_spi->base->SPIDAT1 = data1_reg_val  | tx_data ;

                                DEBUG_SPI
                                    ("\n***I have transmited...on iteration %d and buf_val = 0x%8x***\n",
                                     i-1, buf_val);

                                while ( (flg_val=dm355_spi->base->SPIBUF) & ( 0x80000000 ) );

                                tx_data = dm355_spi->base->SPIBUF; 

                                if ( (i > dm355_spi->count) || (i == 3 && flag == 1))
                                break;
                        }
                }
        } else {
                DEBUG_SPI("In receive\n");
		
		#ifdef CONFIG_SPI_INTERRUPT
			spi_cfg->op_mode = 1;
		#else
			spi_cfg->op_mode = 0;
		#endif		

                if (!spi_cfg->op_mode) {        /*In Polling mode receive */

                        i = 0;
                        while (1) {

                                flg_val = dm355_spi->base->SPIFLG;
                                buf_val = dm355_spi->base->SPIBUF;

                                DEBUG_SPI
                                    ("flg_val = 0x%8x & buf_val = 0x%8x\n",
                                     flg_val, buf_val);

                                if ((buf_val & 0x20000000) == 0) {
                                        if ( ++i <= dm355_spi->count )
                                            dm355_spi->base->SPIDAT1 = data1_reg_val;
                                        else 
                                            dm355_spi->base->SPIDAT1 = ( data1_reg_val & 0x0ffcffff);

                                        while ( (buf_val = dm355_spi->base->SPIBUF) & 0x80000000 );

                                        rx_data = dm355_spi->base->SPIBUF;

                                        dm355_spi->get_rx(rx_data,
                                                          dm355_spi);

                                        DEBUG_SPI
                                            ("I have got it... RCD = 0x%x after %d iteration buf_val = 0x%8x\nflg_val = 0x%8x\n",
                                             rx_data, i, buf_val, flg_val);

                                        if ( i > dm355_spi->count  )
                                        break;
                                }

                        }
                } else {        /*Receive in Interrupt mode */

                        for ( i = 0; i<= dm355_spi->count; i++)
                        {
                            if ( i == dm355_spi->count )
                                dm355_spi->base->SPIDAT1 = ( data1_reg_val & 0x0ffcffff);
                            else {
                         
                        dm355_spi->base->SPIINT |=
                            SPI_SPIFLG_BITERRFLG_MASK |
                            SPI_SPIFLG_OVRNINTFLG_MASK |
                            SPI_SPIFLG_RXINTFLAG_MASK;

                                dm355_spi->base->SPIDAT1 = ( data1_reg_val );

                                while ( (flg_val = dm355_spi->base->SPIINT) & SPI_SPIFLG_RXINTFLAG_MASK );
                            }
                        }
                }
        }

        /* Check for bit error, desync error,parity error,timeout error and
           receive overflow errors */
        intStatus = dm355_spi->base->SPIFLG;

        if ((intStatus & SPI_SPIINT_TIMEOUT_INTR) ==
            SPI_SPIINT_TIMEOUT_INTR) {
                printk("SPI Time-out Error\n");
                return SPI_TIMEOUT_ERR;
        }

        /* De-Synchronization error, holds only in master mode */
        else if ((intStatus & SPI_SPIINT_DESYNC_INTR) ==
                 SPI_SPIINT_DESYNC_INTR) {
                printk("SPI Desynchronization Error\n");
                return SPI_DESYNC_ERR;
        }

        /* Bit error error */
        else if ((intStatus & SPI_SPIINT_BITERR_INTR) ==
                 SPI_SPIINT_BITERR_INTR) {
                printk("SPI Bit error\n");
                return SPI_BIT_ERR;
        /**/}


        /* SPI Framework maintains the count only in bytes so convert back to bytes */
        dm355_spi->count *= conv;

        EXIT

        return t->len;
}



/**
 * dm355_spi_irq - probe function for SPI Master Controller
 * @irq: IRQ number for this SPI Master
 * @context_data: structure for SPI Master controller dm355_spi
 * @ptregs: 
 *
 * ISR will determine that interrupt arrives either for READ or WRITE command.
 * According to command it will do the appropriate action. It will check 
 * transfer length and if it is not zero then dispatch transfer command again.
 * If transfer length is zero then it will indicate the COMPLETION so that
 * dm355_spi_bufs function can go ahead.
 */
irqreturn_t dm355_spi_irq(s32 irq, void *context_data,
                          struct pt_regs * ptregs)
{
        struct dm355_spi *dm355_spi = context_data;
        volatile u32 intStatus = 0;
        u32 rx_data = 0;

        DEBUG_SPI("IN_IRQ\n");

        intStatus = dm355_spi->base->SPIFLG;

        DEBUG_SPI("received interrupt = 0x%8x\n", intStatus);

        while (intStatus != 0) {
                /*Time out Error */
                if ((intStatus & SPI_SPIINT_TIMEOUT_INTR) ==
                    SPI_SPIINT_TIMEOUT_INTR) {
                        printk("SPI Time-out Error\n");
                        return SPI_TIMEOUT_ERR;
                }

                /* De-Synchronization interrupt, holds only in master mode */
                else if ((intStatus & SPI_SPIINT_DESYNC_INTR) ==
                         SPI_SPIINT_DESYNC_INTR) {
                        printk("SPI Desynchronization Error\n");
                        return SPI_DESYNC_ERR;
                }

                /* Bit error Interrupt */
                else if ((intStatus & SPI_SPIINT_BITERR_INTR) ==
                         SPI_SPIINT_BITERR_INTR) {
                        printk("SPI Bit error\n");
                        return SPI_BIT_ERR;
                /**/}

                /*Receive Interrupt */
                else if (intStatus & SPI_SPIINT_RX_INTR) {

                        rx_data = dm355_spi->base->SPIBUF;
                        dm355_spi->get_rx(rx_data, dm355_spi);
                        DEBUG_SPI("IRQ rx_data = 0x16%x\n", rx_data);

                        /*Disable Receive Interrupt */
                        dm355_spi->base->SPIINT = ~SPI_SPIINT_RX_INTR;
                        return IRQ_HANDLED;
                }

        }
        return -1;

}





/**
 * dm355_spi_probe - probe function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * According to Linux Deviced Model this function will be invoked by Linux
 * with plateform_device struct which contains the device specific info
 * like bus_num, max_chipselect (how many slave devices can be connected),
 * clock freq. of SPI controller, SPI controller's memory range, IRQ number etc.
 *
 * According to Linux Deviced Model this function will be invoked by Linux
 * with plateform_device struct which contains the device specific info
 * like bus_num, max_chipselect (how many slave devices can be connected),
 * clock freq. of SPI controller, SPI controller's memory range, IRQ number etc.
 * This info will be provided by board specific code which will reside in 
 * linux-2.6.10/arch/mips/mips-boards/dm355_dm355/dm355_yamuna code.
 * This function will map the SPI controller's memory, register IRQ,
 * Reset SPI controller and setting its registers to default value.
 * It will invoke spi_bitbang_start to create work queue so that client driver
 * can register transfer method to work queue.
 */

static int dm355_spi_probe(struct device *d)
{
        struct platform_device *dev =
            container_of(d, struct platform_device, dev);
        struct spi_master *master;
        struct dm355_spi *dm355_spi;
        struct dm355_spi_platform_data *pdata;
        struct resource *r;
        int i = 0, ret = 0, prescale = 0, clk_freq = 0;;


        /* Get resources(memory, IRQ) associated with the device */
        master = spi_alloc_master(&dev->dev, sizeof(struct dm355_spi));

        if (master == NULL) {
                ret = -ENOMEM;
                goto err;
        }


        dev_set_drvdata(&(dev)->dev, (master));

        pdata = dev->dev.platform_data;

        if (pdata == NULL) {
                ret = -ENODEV;
                goto free_master;
        }

        r = platform_get_resource(dev, IORESOURCE_MEM, 0);

        if (r == NULL) {
                ret = -ENODEV;
                goto free_master;
        }

        dm355_spi = spi_master_get_devdata(master);
        dm355_spi->bitbang.master = spi_master_get(master);
        dm355_spi->bitbang.chipselect = dm355_spi_chipselect;
        dm355_spi->bitbang.setup_transfer = dm355_spi_setup_transfer;
        dm355_spi->bitbang.txrx_bufs = dm355_spi_bufs;
        dm355_spi->sysclk = pdata->sysclk;
        dm355_spi->activate_cs = pdata->activate_cs;
        dm355_spi->deactivate_cs = pdata->deactivate_cs;
        dm355_spi->get_rx = dm355_spi_rx_buf_u8;
        dm355_spi->get_tx = dm355_spi_tx_buf_u8;

        dm355_spi->bitbang.master->setup = dm355_spi_setup;
        init_completion(&dm355_spi->done);

        ENTER
            dm355_spi->base = (struct dm355_spi_reg __iomem *) ioremap(r->start, (r->end - r->start));

        if (dm355_spi->base == NULL) {
                ret = -ENOMEM;
                goto put_master;
        }

        dm355_spi->irq = platform_get_irq(dev, 0);

        if (dm355_spi->irq < 0) {
                ret = -ENXIO;
                goto unmap_io;
        }
	
        /* Register for SPI Interrupt */
        ret = request_irq(dm355_spi->irq, dm355_spi_irq,
                          SA_INTERRUPT, "dm_spi", dm355_spi);

        if (ret != 0) {
                DEBUG_SPI("request_irq fails\n");
                goto unmap_io;
        }

        master->bus_num = pdata->bus_num;
        master->num_chipselect = pdata->max_chipselect;

        /* SPI controller initializations */

        //SP0_MUX_SEL_LINE;

        //mdelay(500);

        clk_freq = 216000000 / 2;

        prescale = (clk_freq / SPI_BUS_FREQ) + 1;


        /*Reset In/OUT SPI modle */
        (dm355_spi->base->SPIGCR0) = 0x0;
        udelay(100);
        (dm355_spi->base->SPIGCR0) = 0x1;

        for (i = 0; i < 4; i++) {
                dm355_spi->base->SPIFMT[i] &= 0xFFFF00FF;       /* clear the old value */

                dm355_spi->base->SPIFMT[i] |= (prescale << 8);  /* clear the old value */
        }


        ret = dm355_spi_bitbang_start(&dm355_spi->bitbang);

        if (ret != 0)
                goto free_irq;


        printk(KERN_INFO
               "%s: dm355 SPI Controller driver at 0x%p (irq = %d)\n",
               dev->dev.bus_id, dm355_spi->base, dm355_spi->irq);

        EXIT return ret;

      free_irq:
        free_irq(dm355_spi->irq, dm355_spi);
      unmap_io:

        iounmap(dm355_spi->base);
      put_master:
        spi_master_put(master);
      free_master:
        kfree(master);
      err:
        return ret;
}



/**
 * dm355_spi_remove - remove function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * This function will do the reverse action of dm355_spi_probe function
 * It will free the IRQ and SPI controller's memory region. 
 * It will also call spi_bitbang_stop to destroy the work queue which was
 * created by spi_bitbang_start.
 */
static int __devexit dm355_spi_remove(struct device *d)
{
        struct platform_device *dev =
            container_of(d, struct platform_device, dev);
        struct dm355_spi *dm355_spi;
        struct spi_master *master;

        master = dev_get_drvdata(&(dev)->dev);

        dm355_spi = spi_master_get_devdata(master);

        dm355_spi_bitbang_stop(&dm355_spi->bitbang);
        free_irq(dm355_spi->irq, dm355_spi);
        iounmap(dm355_spi->base);

	spi_master_put(dm355_spi->bitbang.master);

        return 0;
}

static struct device_driver dm355_spi_driver = {
        .name = "dm_spi",
        .bus = &platform_bus_type,
        .probe = dm355_spi_probe,
        .remove = __devexit_p(dm355_spi_remove),
};


static int __init dm355_spi_init(void)
{
        return driver_register(&dm355_spi_driver);
}

static void __exit dm355_spi_exit(void)
{
        driver_unregister(&dm355_spi_driver);
}


module_init(dm355_spi_init);
module_exit(dm355_spi_exit);

MODULE_AUTHOR("Dhruval Shah & Varun Shah");
MODULE_DESCRIPTION("DM355 SPI Master Controller Driver");
MODULE_LICENSE("GPL");
