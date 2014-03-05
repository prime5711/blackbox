
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/spi/spi.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <asm/uaccess.h>

#include <linux/spi/at25xxA_eeprom.h> 
#include <linux/spi/dm355_spi_master.h>

static struct davinci_eeprom_info *spi_eeprom_info;

/*
 *  Utilities functions
 */
struct davinci_eeprom_info dm355_spi_eeprom_info = {
	.eeprom_size = 8192,
	.page_size = 32,
	.page_mask = 0x001F,
	.chip_sel = SCS0_SELECT,
};

#if 0
static int spi_generic_eeprom_open(struct inode * inode, struct file * file)
{
	file->private_data = spi_eeprom_info;
	return 0;
}

static int spi_generic_eeprom_ioctl(struct inode *inode, struct file *file,
                    unsigned int cmd, unsigned long arg)
{
	return 0;
}
#endif
static int spi_generic_config(dm355_spi_config_t *spi_cfg)
{
	if (spi_cfg == NULL) {
		printk("spi_cfg = NULL\n");
		return -1;
	}

	spi_cfg->clkInternal = TRUE;
	spi_cfg->csHold = TRUE;
	spi_cfg->intrLevel = FALSE;
	spi_cfg->pinOpModes = SPI_OPMODE_SPISCS_4PIN;
	spi_cfg->clkHigh = FALSE;
	spi_cfg->lsbFirst = FALSE;
	spi_cfg->parityEnable = TRUE;
	spi_cfg->oddParity = FALSE;
	spi_cfg->phaseIn = TRUE;
	spi_cfg->op_mode = TRUE; /*Polling mode -> FALSE, Interrupt mode -> TRUE */
	spi_cfg->loopBack = FALSE; /* loopback is disabled */

}
static int spi_generic_eeprom_read(struct mtd_info *mtd, loff_t from, 
			size_t count, size_t *retlen, u_char *buf)
{
	u8 *tx_ptr, *rx_ptr;
	int rx_cnt;
	unsigned int addr;
	unsigned long flags;
	struct spi_transfer x[2];
	struct spi_message msg;
	struct davinci_eeprom_info *priv_dat = mtd->priv;

	addr = (u16)from;
	*retlen = 0;
	if(addr > priv_dat->eeprom_size)
		return -EINVAL;

	if (count > priv_dat->eeprom_size)
		return -EINVAL;

	if((addr + count) > priv_dat->eeprom_size)
		return -EINVAL;

	memset(x, 0, sizeof x);
	down(&priv_dat->lock);
	x[0].tx_buf = tx_ptr = priv_dat->tx_buffer;

	tx_ptr[0] = DM355_EEPROM_READ;

	x[0].len = 3;

	/* Handle data return from EEPROM */
	x[1].rx_buf = rx_ptr = priv_dat->rx_buffer;

	while (count > 0) {
		if (likely(count > SPI_BUFFER_SIZE))
			rx_cnt = SPI_BUFFER_SIZE;
		else
			rx_cnt = count;

		spi_message_init(&msg);
		/* setup read command */
		tx_ptr[1] = (addr >> 8) & 0xFF;
		tx_ptr[2] = (addr & 0xFF);

		local_irq_save(flags);
		spi_message_add_tail(&x[0], &msg);

		/* read the eeprom */
		x[1].len = rx_cnt;
		spi_message_add_tail(&x[1], &msg);
		local_irq_restore(flags);

		spi_sync(priv_dat->spi, &msg);

		//spi_read(priv_dat->spi, rx_ptr, rx_cnt);
		memcpy(buf, rx_ptr, rx_cnt);

		buf += rx_cnt;
		count -= rx_cnt;
		addr += rx_cnt;
		*retlen += rx_cnt;
	}
	up(&priv_dat->lock);

	return 0;
}

static int spi_generic_eeprom_write (struct mtd_info *mtd, loff_t to, 
				size_t count, size_t *retlen, const u_char *buf)
{
	char *ptr;
	int status;
	int tx_cnt;
	unsigned int  addr;
	struct spi_transfer xfer[2];
	struct spi_message msg;
	struct davinci_eeprom_info *priv_dat = mtd->priv;
	unsigned long flags;

	addr = (u16)(to);
	*retlen = 0;
	memset(xfer, 0, sizeof xfer);
	if(addr > priv_dat->eeprom_size)
		return -EINVAL;

	if (count > priv_dat->eeprom_size)
		return -EINVAL;

	if((addr + count) > priv_dat->eeprom_size)
		return -EINVAL;

	down(&priv_dat->lock);
	while(count > 0) {
		xfer[0].tx_buf = ptr = priv_dat->tx_buffer;

		/* set write enable */
		ptr[0] = DM355_EEPROM_WREN;
		spi_write(priv_dat->spi, ptr, 1);

		spi_message_init(&msg);
		/* set the write command */
		ptr[0] = DM355_EEPROM_WRITE;
		ptr[1] = (addr >> 8) & 0xFF;
		ptr[2] = (addr & 0xFF);
		xfer[0].len = DAVINCI_SPI_TX_CMD_SIZE;
		local_irq_save(flags);
		//spi_message_add_tail(&xfer[0], &msg);

		/* figure out the max transfer within a page */
		tx_cnt = priv_dat->page_size - (addr & priv_dat->page_mask);
		//if((tx_cnt + DAVINCI_SPI_TX_CMD_SIZE) > SPI_BUFFER_SIZE)
		//	tx_cnt = SPI_BUFFER_SIZE - DAVINCI_SPI_TX_CMD_SIZE;

		if (count < tx_cnt)
			tx_cnt = count;

		ptr = &priv_dat->tx_buffer[DAVINCI_SPI_TX_CMD_SIZE];
		//xfer[1].tx_buf = ptr;
		xfer[0].len += tx_cnt;
		memcpy(ptr, buf, tx_cnt);
		//spi_write(priv_dat->spi, ptr, tx_cnt + DAVINCI_SPI_TX_CMD_SIZE);
		spi_message_add_tail(&xfer[0], &msg);
		local_irq_restore(flags);
		status = spi_sync(priv_dat->spi, &msg);

		count -= tx_cnt;
		buf += tx_cnt;
		addr += tx_cnt;
		*retlen += tx_cnt;
	}
	up(&priv_dat->lock);

	return (0);
}

#if 0
static int spi_generic_eeprom_release(struct inode * inode, struct file * file)
{
	return 0;
}

struct file_operations spi_eeprom_ops = {

	.owner = THIS_MODULE,
	.open = spi_generic_eeprom_open,
	.ioctl = spi_generic_eeprom_ioctl,
	.read = spi_generic_eeprom_read,
	.write = spi_generic_eeprom_write,
	.release = spi_generic_eeprom_release,

};
#endif

static int spi_eeprom_generic_erase(struct mtd_info *mtd, 
				struct erase_info *instr)
{
	*mtd;
	*instr;
}

static struct mtd_info davinci_at25;
static int __devinit eeprom_probe(struct spi_device *spi)
{
	int ret;
	static struct mtd_info *mtd;

	mtd = &davinci_at25;
	memset(mtd, 0, sizeof(struct mtd_info));

	/* are there any id we need to read? */
	spi_eeprom_info = &dm355_spi_eeprom_info;
	spi_eeprom_info->spi = spi;
	spi_generic_config(spi->controller_data);
	spi_eeprom_info->spi_data = spi_master_get_devdata(spi->master);
	init_MUTEX(&spi_eeprom_info->lock);

	mtd->priv = spi_eeprom_info;
	mtd->size = spi_eeprom_info->eeprom_size;
	mtd->flags = MTD_CAP_RAM;
	mtd->read = spi_generic_eeprom_read;
	mtd->write = spi_generic_eeprom_write;
	mtd->erase = spi_eeprom_generic_erase;
	mtd->type = MTD_RAM;
	mtd->name = "spi_eeprom";
	mtd->erasesize = 0x10;

	ret = add_mtd_device(mtd);
	//ret = register_chrdev(SPI_EEPROM_MAJOR, DAVINCI_SPI_EEPROM_NAME, 
	//			&spi_eeprom_ops);
	if (ret < 0) {
		printk("at25xxA_spi_eeprom device register failed\n");
	}
	printk("at25xxA_spi_eeprom device register Succeeded\n");
	return ret;
}

static struct spi_driver spi_eeprom_driver = {
        .driver = {
                .name           = DAVINCI_SPI_EEPROM_NAME,
                .bus            = &spi_bus_type,
                .owner          = THIS_MODULE,
        },
        .probe          = eeprom_probe,
	/* since eeprom is soldered on, we don't need to worry about remove */
};

static int __init spi_eeprom_init(void)
{
	printk("at25xxA_spi_eeprom init \n");
        return spi_register_driver(&spi_eeprom_driver);
}
module_init(spi_eeprom_init);

static void __exit spi_eeprom_exit(void)
{
	printk("at25xxA_spi_eeprom exit \n");
        spi_unregister_driver(&spi_eeprom_driver);
}
module_exit(spi_eeprom_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steve Chen");
MODULE_DESCRIPTION("SPI EEPROM driver");

