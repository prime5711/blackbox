#
# Makefile for the linux kernel.
#
#


# Common objects
obj-y 				:= time.o irq.o dma.o serial.o clock.o pinmux.o gio.o io.o id.o

# Board specific
obj-$(CONFIG_MACH_DAVINCI_EVM)		+= board-evm.o i2c-emac.o
obj-$(CONFIG_MACH_DAVINCI_DM355_EVM)	+= board-dm355-evm.o
obj-$(CONFIG_MACH_DAVINCI_DM355_IPNC)	+= board-dm355-ipnc.o
obj-$(CONFIG_DAVINCI_MCBSP)		+= mcbsp.o
obj-$(CONFIG_DAVINCI_I2C_EXPANDER)	+= i2c-client.o
obj-$(CONFIG_DM355_SPI)			+= spi_platform_init.o

ifeq ($(CONFIG_LEDS),y)
obj-$(CONFIG_MACH_DAVINCI_EVM)	+= leds-evm.o
endif
