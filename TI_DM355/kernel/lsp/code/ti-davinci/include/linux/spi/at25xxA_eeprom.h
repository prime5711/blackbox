#ifndef DM355_SPI_EEPROM_H
#define DM355_SPI_EEPROM_H

#include <linux/cache.h>
/*
 *  EEPROM op-codes
 */
#define DM355_EEPROM_READ	0x03	/* read */
#define DM355_EEPROM_WRITE	0x02	/* write */
#define DM355_EEPROM_WREN	0x06	/* write enable */
#define DM355_EEPROM_WRDIS	0x04	/* write disable */
#define DM355_EEPROM_RDSTAT	0x05	/* read status register */
#define DM355_EEPROM_WRSTAT	0x01	/* write status register */

#define SPI_BUFFER_SIZE SMP_CACHE_BYTES
#define DAVINCI_SPI_TX_CMD_SIZE 3

#define DAVINCI_SPI_EEPROM_NAME "at25xxA_spi_eeprom"

struct davinci_eeprom_info {
        unsigned int eeprom_size;
        unsigned int page_size;
	unsigned int page_mask;
	unsigned long chip_sel;
        struct spi_device *spi;
	struct dm355_spi *spi_data;

	struct semaphore lock;
        char tx_buffer[SPI_BUFFER_SIZE+DAVINCI_SPI_TX_CMD_SIZE];
        char rx_buffer[SPI_BUFFER_SIZE];
};

#endif /*DM355_SPI_EEPROM_H */
