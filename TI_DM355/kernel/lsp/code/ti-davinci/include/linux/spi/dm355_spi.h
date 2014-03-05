
#ifndef __AVALANCHE_SPI_H_
#define __AVALANCHE_SPI_H_

#define AVALANCHE_SPI_SIZE				0xFF
#define AVALANCHE_SPI_INIT_SPMODE			0

/* Chip selects available on the spi IP */
#define AVALANCHE_SPI_CS0		0
#define AVALANCHE_SPI_CS1		1
#define AVALANCHE_SPI_CS2		2
#define AVALANCHE_SPI_CS3		3


struct spi_cmd_t {
	unsigned int addr;
	unsigned int data;
};

struct ctlr_cs_sel_t {
        u8 cs;
        u8 pol;
};

struct dm355_spi_platform_data {
        /* initial SPMODE value */
        u32 initial_spmode;
        /* board specific information */
        u16 bus_num;            /* id for controller */
        u16 max_chipselect;
        int (*activate_cs) (u8 cs, u8 polarity,
                            struct ctlr_cs_sel_t * ctlr_cs_sel);
        int (*deactivate_cs) (u8 cs, u8 polarity,
                              struct ctlr_cs_sel_t * ctlr_cs_sel);
        u32 sysclk;
};

#endif                          /* __AVALANCHE_SPI_H_ */
