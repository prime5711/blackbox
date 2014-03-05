/*
 * Platform device support for TI SoCs.
 *
 */
#include <linux/config.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/spi/spi.h>
#include <linux/spi/dm355_spi.h>
#include <linux/spi/dm355_spi_master.h>
#include <asm/arch/hardware.h>
#include <linux/spi/at25xxA_eeprom.h>
/*#include <pal.h>*/
#include <linux/device.h>

#define SPI_MASTER_TEST_MEMORY_DRIVER 1
#define DM355_SPI_INIT_SPMODE 0 

/*Board specific addresses of SPI*/
#define SPI0_ADDRESS (0x01c66000)
#define SPI1_ADDRESS (0x01c66800)
#define SPI2_ADDRESS (0x01c67800)
#define SPI_SIZE (0x7FF)

/*Interrupt lines*/
#define SPI0_INTR_LINE 42

static struct dm355_spi_platform_data dm355_spi_platform_data =
{
        .initial_spmode         = DM355_SPI_INIT_SPMODE,
        .bus_num                = 0,
        .max_chipselect         = 2,        
        .activate_cs            = NULL,
        .deactivate_cs          = NULL, /* four our controller cs is taken care by framelength */
//        .sysclk                 =67.5*1000*1000,
        .sysclk                 =108*1000*1000,
};

static struct resource dm355_spi_resources[] = { 
        [0] = {
                .start          = SPI0_ADDRESS,
                .end            = SPI0_ADDRESS + SPI_SIZE,
                .flags          = IORESOURCE_MEM,
        },
        [1] = {
                .start          = SPI0_INTR_LINE,
                .end            = SPI0_INTR_LINE,
                .flags          = IORESOURCE_IRQ,
        },
};

static struct platform_device dm355_spi_device = {
        .name           = "dm_spi",
        .id             = 0,
        .num_resources  = ARRAY_SIZE(dm355_spi_resources),
        .resource       = dm355_spi_resources,
};


#if defined (CONFIG_DM355_SPI_EEPROM)

struct sst_eeprom_platform_data {
      int data;
};
static struct sst_eeprom_platform_data eeprom1_info = {
                // ... if you want something
};

extern struct davinci_eeprom_info dm355_spi_eeprom_info;


static struct spi_board_info eeprom_spi_board_info[] = {
        {
                .modalias       = DAVINCI_SPI_EEPROM_NAME,
                .platform_data  = &dm355_spi_eeprom_info,
                .mode           = SPI_MODE_0,
                .irq            = 0,
                .max_speed_hz   = 2*1000*1000 /* max sample rate at 3V */ ,
                .bus_num        = 65535,
                .chip_select    = 0,
        },
};   
#else
struct spi_platform_data {
      int data;
};

static struct spi_platform_data eeprom1_info = {
                // ... if you want something
};    
#endif    

/*Put slave specific information in this array.*/
/*For more information refer the table at the end of file tnetd84xx_spi_cs.c*/
static struct spi_board_info dm355_spi_board_info[] = {
#if defined (CONFIG_LOOPBACK_SPI_MODULE) || defined (CONFIG_LOOPBACK_SPI)
{
	.modalias		= "LOOPB", 
	.platform_data		= &eeprom1_info,
	.mode			= SPI_MODE_0,
	.irq			= 0, 
	.max_speed_hz		= 2*1000*1000 /* max sample rate at 3V */ ,
	.bus_num		= 0,
	.chip_select		= 0,
},
#endif
#if defined (CONFIG_DLCD_SPI) || defined (CONFIG_DLCD_SPI_MODULE)
{
	.modalias		= "DLCD_SPI",
	.platform_data		= &eeprom1_info,
	.mode			= SPI_MODE_0,
	.irq			= 0, 
	.max_speed_hz		= 2*1000*1000 /* max sample rate at 3V */ ,
	.bus_num		= 0,
	.chip_select		= 0,
},
#endif
#if defined (CONFIG_SOUND_DM355_TLV320AIC26) 
{
	.modalias		= "AIC26",
	.platform_data		= &eeprom1_info,
	.mode			= SPI_MODE_0,
	.irq			= 0, 
	.max_speed_hz		= 2*1000*1000 /* max sample rate at 3V */ ,
	.bus_num		= 65535, ///2,
	.chip_select		= 0, ///1,
},
#endif
};

/* 
 * This function initializes the GPIOs used by the SPI module 
 * and it also registers the spi mastere device with the platform 
 * and the spi slave devices with the spi bus 
 */
static int __init dm355_spi_board_init(void)
{

	int ret = 0;
 //       printk("%s:%s:%d dm355_spi_board_init \n",__FILE__,__FUNCTION__,__LINE__);
    	/* Register the slave devices present in the board with SPI subsytem */
	ret = spi_register_board_info(dm355_spi_board_info, 
			ARRAY_SIZE(dm355_spi_board_info));
	if (ret) {
		printk(KERN_ERR "Failed to spi_register_board_info.\n");
		return ret;
	}		
#if defined (CONFIG_DM355_SPI_EEPROM)
	ret = spi_register_board_info(eeprom_spi_board_info, 
			ARRAY_SIZE(eeprom_spi_board_info));
	if (ret) {
		printk(KERN_ERR "Failed to spi_register_board_info.\n");
		return ret;
	}				
#endif			
 //       printk("%s:%s:%d REG BOARD  ret %d\n",__FILE__,__FUNCTION__,__LINE__,ret);
        dm355_spi_device.dev.platform_data = &dm355_spi_platform_data;

	/* Register the master controller with platform */
	ret =  platform_device_register(&dm355_spi_device);
  //      printk("%s:%s:%d REGISTER DEVICE ret %d\n",__FILE__,__FUNCTION__,__LINE__,ret);
	if (ret) {
		printk(KERN_ERR "Failed to platform_device_register.\n");
		return ret;
	}		
	
	return 0;
}

static void __exit dm355_spi_board_exit(void)
{
//	int ret = 0;
        printk("%s:%s:%d dm355_spi_board_exit \n",__FILE__,__FUNCTION__,__LINE__);
	 /* nothing to be done */
}

module_init(dm355_spi_board_init);
module_exit(dm355_spi_board_exit);


