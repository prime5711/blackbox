/* ld script to make ARM Linux kernel
 * taken from the i386 version by Russell King
 * Written by Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */
/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.10_mvl401
 * Mon Sep  9 00:14:01 2013
 */






/*
 * Code maturity level options
 */





/*
 * General setup
 */
/*
 * Loadable module support
 */
/*
 * System type and features
 */
/*
 * TI DaVinci Implementations
 */

/*
 * DaVinci Core Type
 */



/*
 * DaVinci Board Type
 */





/*
 * DaVinci Options
 */





/*
 * Processor Type
 */
/*
 * Processor Features
 */
/*
 * PCCARD (PCMCIA/CardBus) support
 */


/*
 * PC-card bridges
 */

/*
 * At least one math emulation must be selected
 */
/*
 * Device Drivers
 */

/*
 * Generic Driver Options
 */




/*
 * Parallel port support
 */


/*
 * Memory Technology Devices (MTD)
 */
/*
 * User Modules And Translation Layers
 */







/*
 * RAM/ROM/Flash chip drivers
 */
/*
 * Mapping drivers for chip access
 */



/*
 * Self-contained MTD device drivers
 */
/*
 * Disk-On-Chip Device Drivers
 */




/*
 * NAND Flash Device Drivers
 */







/*
 * Plug and Play support
 */

/*
 * Block devices
 */
/*
 * IO Schedulers
 */





/*
 * sbull Driver
 */


/*
 * Multi-device support (RAID and LVM)
 */




/*
 * Networking support
 */


/*
 * Networking options
 */
/*
 * IP: Virtual Server Configuration
 */





/*
 * IP: Netfilter Configuration
 */
/*
 * SCTP Configuration (EXPERIMENTAL)
 */
/*
 * QoS and/or fair queueing
 */



/*
 * Network testing
 */
/*
 * PHY device support
 */

/*
 * Ethernet (10 or 100Mbit)
 */


/*
 * Broadcom network devices
 */


/*
 * Ethernet (1000 Mbit)
 */

/*
 * Ethernet (10000 Mbit)
 */

/*
 * Token Ring devices
 */

/*
 * Wireless LAN (non-hamradio)
 */



/*
 * Obsolete Wireless cards support (pre-802.11)
 */





/*
 * Wan interfaces
 */






/*
 * ATA/ATAPI/MFM/RLL support
 */


/*
 * SCSI device support
 */



/*
 * SCSI support type (disk, tape, CD-ROM)
 */






/*
 * Some SCSI devices (e.g. CD jukebox) support multiple LUNs
 */




/*
 * SCSI Transport Attributes
 */



/*
 * SCSI low-level drivers
 */



/*
 * Fusion MPT device support
 */

/*
 * IEEE 1394 (FireWire) support
 */

/*
 * I2O device support
 */

/*
 * ISDN subsystem
 */


/*
 * Input device support
 */


/*
 * Userland interfaces
 */







/*
 * Input I/O drivers
 */







/*
 * Input Device Drivers
 */






/*
 * Character devices
 */





/*
 * Serial drivers
 */





/*
 * Non-8250 serial port support
 */






/*
 * IPMI
 */


/*
 * Watchdog Cards
 */



/*
 * Watchdog Device Drivers
 */



/*
 * USB-based Watchdog Cards
 */
/*
 * Ftape, the floppy tape device driver
 */






/*
 * I2C support
 */



/*
 * I2C Algorithms
 */






/*
 * I2C Hardware Bus support
 */
/*
 * Hardware Sensors Chip support
 */
/*
 * Other I2C Chip support
 */
/*
 * SPI support
 */



/*
 * SPI Master Controller Drivers
 */




/*
 * SPI Protocol Masters
 */

/*
 * Multimedia devices
 */


/*
 * Video For Linux
 */

/*
 * Video Adapters
 */
/*
 * Radio Adapters
 */


/*
 * Digital Video Broadcasting Devices
 */



/*
 * File systems
 */
/*
 * CD-ROM/DVD Filesystems
 */



/*
 * DOS/FAT/NT Filesystems
 */







/*
 * Pseudo filesystems
 */
/*
 * Miscellaneous filesystems
 */
/*
 * Network File Systems
 */
/*
 * Partition Types
 */
/*
 * Native Language Support
 */
/*
 * Profiling support
 */


/*
 * Graphics support
 */






/*
 * Console display driver support
 */







/*
 * Logo configuration
 */


/*
 * Sound
 */


/*
 * Advanced Linux Sound Architecture
 */


/*
 * Open Sound System
 */
/*
 * DaVinci Audio Options
 */



/*
 * Misc devices
 */

/*
 * Multimedia Capabilities Port drivers
 */

/*
 * USB support
 */



/*
 * Miscellaneous USB options
 */
/*
 * USB Host Controller Drivers
 */




/*
 * DaVinci DM355 USB support
 */
/*
 * USB Device Class drivers
 */






/*
 * NOTE: USB_STORAGE enables SCSI, and 'SCSI disk support' may also be needed; see USB_STORAGE Help for more information
 */


/*
 * USB Input Devices
 */


/*
 * USB HID Boot Protocol drivers
 */
/*
 * USB Imaging devices
 */




/*
 * USB Multimedia devices
 */
/*
 * USB Network Adapters
 */






/*
 * USB port drivers
 */

/*
 * USB Serial Converter support
 */


/*
 * USB Miscellaneous drivers
 */
/*
 * USB ATM/DSL drivers
 */

/*
 * USB Gadget Support
 */


/*
 * Synchronous Serial Interfaces (SSI)
 */

/*
 * MMC/SD Card support
 */







/*
 * MontaVista System tools
 */


/*
 * Kernel hacking
 */
/*
 * Security options
 */



/*
 * Cryptographic options
 */
/*
 * Library routines
 */







/*
 * Fast Real-Time Domain
 */


/*
 * Fast Real-Time Domain Advanced Options
 */



OUTPUT_ARCH(arm)
ENTRY(stext)

jiffies = jiffies_64;



SECTIONS
{
 . = 0xC0008000;
 .init : { /* Init code and data		*/
  _stext = .;
   _sinittext = .;
   *(.init.text)
   _einittext = .;
  __proc_info_begin = .;
   *(.proc.info.init)
  __proc_info_end = .;
  __arch_info_begin = .;
   *(.arch.info)
  __arch_info_end = .;
  __tagtable_begin = .;
   *(.taglist)
  __tagtable_end = .;
  . = ALIGN(16);
  __setup_start = .;
   *(.init.setup)
  __setup_end = .;
  __early_begin = .;
   *(__early_param)
  __early_end = .;
  __initcall_start = .;
   *(.initcall1.init)
   *(.initcall2.init)
   *(.initcall3.init)
   *(.initcall4.init)
   *(.initcall5.init)
   *(.initcall6.init)
   *(.initcall7.init)
  __initcall_end = .;
  __con_initcall_start = .;
   *(.con_initcall.init)
  __con_initcall_end = .;
  __security_initcall_start = .;
   *(.security_initcall.init)
  __security_initcall_end = .;
  . = ALIGN(32);
  __initramfs_start = .;
   usr/built-in.o(.init.ramfs)
  __initramfs_end = .;
  . = ALIGN(64);
  __per_cpu_start = .;
   *(.data.percpu)
  __per_cpu_end = .;

  __init_begin = _stext;
  *(.init.data)
  . = ALIGN(4096);
  __init_end = .;

 }

 /DISCARD/ : { /* Exit code and data		*/
  *(.exit.text)
  *(.exit.data)
  *(.exitcall.exit)
 }

 .text : { /* Real text segment		*/
  _text = .; /* Text and read-only data	*/
   *(.text)
   __sched_text_start = .; *(.sched.text) __sched_text_end = .;
   __lock_text_start = .; *(.spinlock.text) __lock_text_end = .;
   *(.fixup)
   *(.gnu.warning)
   *(.rodata)
   *(.rodata.*)
   *(.glue_7)
   *(.glue_7t)
  *(.got) /* Global offset table		*/
 }

 . = ALIGN(16);
 __ex_table : { /* Exception table		*/
  __start___ex_table = .;
   *(__ex_table)
  __stop___ex_table = .;
 }

 .rodata : AT(ADDR(.rodata) - 0) { *(.rodata) *(.rodata.*) *(__vermagic) } .rodata1 : AT(ADDR(.rodata1) - 0) { *(.rodata1) } .pci_fixup : AT(ADDR(.pci_fixup) - 0) { __start_pci_fixups_early = .; *(.pci_fixup_early) __end_pci_fixups_early = .; __start_pci_fixups_header = .; *(.pci_fixup_header) __end_pci_fixups_header = .; __start_pci_fixups_final = .; *(.pci_fixup_final) __end_pci_fixups_final = .; __start_pci_fixups_enable = .; *(.pci_fixup_enable) __end_pci_fixups_enable = .; } __ksymtab : AT(ADDR(__ksymtab) - 0) { __start___ksymtab = .; *(__ksymtab) __stop___ksymtab = .; } __ksymtab_gpl : AT(ADDR(__ksymtab_gpl) - 0) { __start___ksymtab_gpl = .; *(__ksymtab_gpl) __stop___ksymtab_gpl = .; } __kcrctab : AT(ADDR(__kcrctab) - 0) { __start___kcrctab = .; *(__kcrctab) __stop___kcrctab = .; } __kcrctab_gpl : AT(ADDR(__kcrctab_gpl) - 0) { __start___kcrctab_gpl = .; *(__kcrctab_gpl) __stop___kcrctab_gpl = .; } __ksymtab_strings : AT(ADDR(__ksymtab_strings) - 0) { *(__ksymtab_strings) } __param : AT(ADDR(__param) - 0) { __start___param = .; *(__param) __stop___param = .; }

 _etext = .; /* End of text and rodata section */





 . = ALIGN(8192);
 __data_loc = .;


 .data : AT(__data_loc) {
  __data_start = .; /* address in memory */

  /*
		 * first, the init task union, aligned
		 * to an 8192 byte boundary.
		 */
  *(.init.task)
  . = ALIGN(4096);
  __nosave_begin = .;
  *(.data.nosave)
  . = ALIGN(4096);
  __nosave_end = .;

  /*
		 * then the cacheline aligned data
		 */
  . = ALIGN(32);
  *(.data.cacheline_aligned)

  /*
		 * and the usual data section
		 */
  *(.data)
  CONSTRUCTORS

  _edata = .;
 }

 .bss : {
  __bss_start = .; /* BSS				*/
  *(.bss)
  *(COMMON)
  _end = .;
 }
     /* Stabs debugging sections.	*/
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .comment 0 : { *(.comment) }
}
