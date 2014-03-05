/*
 * linux/drivers/mmc/davinci.c
 *
 * TI DaVinci MMC controller file
 *
 * Copyright (C) 2006 Texas Instruments.
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
 ver. 1.0: Oct 2005, Purushotam Kumar   Initial version
 ver 1.1:  Nov  2005, Purushotam Kumar  Solved bugs
 ver 1.2:  Jan  2066, Purushotam Kumar   Added card remove insert support
 -
 *
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/version.h>
/* DEBUG check shall be made before kernel.h or device.h is included */
#ifdef CONFIG_MMC_DEBUG
#define DEBUG
#endif
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/blkdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/hardware.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/protocol.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <asm/arch/irqs.h>
#include <asm/arch/hardware.h>
#include <asm/hardware/clock.h>
#include <asm/io.h>

//#include "davinci_mmc.h"
#include <linux/davinci_mmc.h>	/* for platform data */
#include <asm/arch/edma.h>

#define	DAVINCI_MMC_REG_CTL	0x00
#define	DAVINCI_MMC_REG_CLK	0x04
#define	DAVINCI_MMC_REG_ST0	0x08
#define	DAVINCI_MMC_REG_ST1	0x0c
#define	DAVINCI_MMC_REG_IM	0x10
#define	DAVINCI_MMC_REG_TOR	0x14
#define	DAVINCI_MMC_REG_TOD	0x18
#define	DAVINCI_MMC_REG_BLEN	0x1c
#define	DAVINCI_MMC_REG_NBLK	0x20
#define	DAVINCI_MMC_REG_NBLC	0x24
#define	DAVINCI_MMC_REG_DRR	0x28
#define	DAVINCI_MMC_REG_DXR	0x2c
#define	DAVINCI_MMC_REG_CMD	0x30
#define	DAVINCI_MMC_REG_ARGHL	0x34
#define	DAVINCI_MMC_REG_RSP01	0x38
#define	DAVINCI_MMC_REG_RSP23	0x3c
#define	DAVINCI_MMC_REG_RSP45	0x40
#define	DAVINCI_MMC_REG_RSP67	0x44
#define	DAVINCI_MMC_REG_DRSP	0x48
#define	DAVINCI_MMC_REG_ETOK	0x4c
#define	DAVINCI_MMC_REG_CIDX	0x50
#define	DAVINCI_MMC_REG_CKC	0x54
#define	DAVINCI_MMC_REG_TORC	0x58
#define	DAVINCI_MMC_REG_TODC	0x5c
#define	DAVINCI_MMC_REG_BLNC	0x60
#define	DAVINCI_SDIO_REG_CTL	0x64
#define	DAVINCI_SDIO_REG_ST0	0x68
#define	DAVINCI_SDIO_REG_EN	0x6c
#define	DAVINCI_SDIO_REG_ST	0x70
#define	DAVINCI_MMC_REG_FIFO_CTL	0x74

/* DAVINCI_MMCCTL definitions */
#define MMCCTL_DATRST         (1 << 0)
#define MMCCTL_CMDRST         (1 << 1)
#define MMCCTL_WIDTH_4_BIT    (1 << 2)
#define MMCCTL_DATEG_DISABLED (0 << 6)
#define MMCCTL_DATEG_RISING   (1 << 6)
#define MMCCTL_DATEG_FALLING  (2 << 6)
#define MMCCTL_DATEG_BOTH     (3 << 6)
#define MMCCTL_PERMDR_LE      (0 << 9)
#define MMCCTL_PERMDR_BE      (1 << 9)
#define MMCCTL_PERMDX_LE      (0 << 10)
#define MMCCTL_PERMDX_BE      (1 << 10)

/* DAVINCI_MMCCLK definitions */
#define MMCCLK_CLKEN          (1 << 8)
#define MMCCLK_CLKRT_MASK     (0xFF << 0)

#define BIT(x) (1UL << (x))

/* IRQ bit definitions, for DAVINCI_MMCST0 and DAVINCI_MMCIM */
#define MMCST0_DATDNE         BIT(0)	/* data done */
#define MMCST0_BSYDNE         BIT(1)	/* busy done */
#define MMCST0_RSPDNE         BIT(2)	/* command done */
#define MMCST0_TOUTRD         BIT(3)	/* data read timeout */
#define MMCST0_TOUTRS         BIT(4)	/* command response timeout */
#define MMCST0_CRCWR          BIT(5)	/* data write CRC error */
#define MMCST0_CRCRD          BIT(6)	/* data read CRC error */
#define MMCST0_CRCRS          BIT(7)	/* command response CRC error */
#define MMCST0_DXRDY          BIT(9)	/* data transmit ready (fifo empty) */
#define MMCST0_DRRDY          BIT(10)	/* data receive ready (data in fifo)*/
#define MMCST0_DATED          BIT(11)	/* DAT3 edge detect */
#define MMCST0_TRNDNE         BIT(12)	/* transfer done */

/* DAVINCI_MMCST1 definitions */
#define MMCST1_BUSY           (1 << 0)

/* DAVINCI_MMCCMD definitions */
#define MMCCMD_CMD_MASK       (0x3F << 0)
#define MMCCMD_PPLEN          (1 << 7)
#define MMCCMD_BSYEXP         (1 << 8)
#define MMCCMD_RSPFMT_MASK    (3 << 9)
#define MMCCMD_RSPFMT_NONE    (0 << 9)
#define MMCCMD_RSPFMT_R1456   (1 << 9)
#define MMCCMD_RSPFMT_R2      (2 << 9)
#define MMCCMD_RSPFMT_R3      (3 << 9)
#define MMCCMD_DTRW           (1 << 11)
#define MMCCMD_STRMTP         (1 << 12)
#define MMCCMD_WDATX          (1 << 13)
#define MMCCMD_INITCK         (1 << 14)
#define MMCCMD_DCLR           (1 << 15)
#define MMCCMD_DMATRIG        (1 << 16)

/* DAVINCI_MMCFIFOCTL definitions */
#define MMCFIFOCTL_FIFORST    (1 << 0)
#define MMCFIFOCTL_FIFODIR_WR (1 << 1)
#define MMCFIFOCTL_FIFODIR_RD (0 << 1)
#define MMCFIFOCTL_FIFOLEV    (1 << 2) /* 0 = 128 bits, 1 = 256 bits */
#define MMCFIFOCTL_ACCWD_4    (0 << 3) /* access width of 4 bytes    */
#define MMCFIFOCTL_ACCWD_3    (1 << 3) /* access width of 3 bytes    */
#define MMCFIFOCTL_ACCWD_2    (2 << 3) /* access width of 2 bytes    */
#define MMCFIFOCTL_ACCWD_1    (3 << 3) /* access width of 1 byte     */

#define DAVINCI_MMC_READW(host, reg) \
	__raw_readw((host)->virt_base + DAVINCI_MMC_REG_##reg)
#define DAVINCI_MMC_WRITEW(host, reg, val) \
	__raw_writew((val), (host)->virt_base + DAVINCI_MMC_REG_##reg)
#define DAVINCI_MMC_READL(host, reg) \
	__raw_readl((host)->virt_base + DAVINCI_MMC_REG_##reg)
#define DAVINCI_MMC_WRITEL(host, reg, val) \
	__raw_writel((val), (host)->virt_base + DAVINCI_MMC_REG_##reg)
/*
 * Command types
 */
#define DAVINCI_MMC_CMDTYPE_BC		0
#define DAVINCI_MMC_CMDTYPE_BCR		1
#define DAVINCI_MMC_CMDTYPE_AC		2
#define DAVINCI_MMC_CMDTYPE_ADTC	3
//#define EDMA_MAX_LOGICAL_CHA_ALLOWED	1

#define DAVINCI_MMC_EVENT_BLOCK_XFERRED		(1 <<  0)
#define DAVINCI_MMC_EVENT_CARD_EXITBUSY		(1 <<  1)
#define DAVINCI_MMC_EVENT_EOFCMD		(1 <<  2)
#define DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT	(1 <<  3)
#define DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT	(1 <<  4)
#define DAVINCI_MMC_EVENT_ERROR_DATACRC		((1 << 6) | (1 << 5))
#define DAVINCI_MMC_EVENT_ERROR_CMDCRC		(1 <<  7)
#define DAVINCI_MMC_EVENT_WRITE			(1 <<  9)
#define DAVINCI_MMC_EVENT_READ			(1 << 10)

#define DAVINCI_MMC_EVENT_TIMEOUT_ERROR \
 (DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT | DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT)
#define DAVINCI_MMC_EVENT_CRC_ERROR \
 (DAVINCI_MMC_EVENT_ERROR_DATACRC | DAVINCI_MMC_EVENT_ERROR_CMDCRC)
#define DAVINCI_MMC_EVENT_ERROR \
 (DAVINCI_MMC_EVENT_TIMEOUT_ERROR | DAVINCI_MMC_EVENT_CRC_ERROR)
#define DRIVER_NAME "mmc"
#define RSP_TYPE(x)	((x) & ~(MMC_RSP_BUSY|MMC_RSP_OPCODE))
#define MAX_C_CNT	((1 << 16) - 1)

#define NR_SG		16

static unsigned rw_threshold = 32;

module_param(rw_threshold, uint, S_IRUGO);
MODULE_PARM_DESC(rw_threshold,
		"Read/Write threshold. Default = 32");

static unsigned __initdata use_dma = 1;
module_param(use_dma, uint, 0);
MODULE_PARM_DESC(use_dma, "Whether to use DMA or not. Default = 1");

struct edma_ch_mmcsd {
	unsigned char cnt_chanel;
	unsigned int chanel_num[NR_SG-1];
};

struct mmc_davinci_host {
	struct resource 	*reg_res;
	spinlock_t		mmc_lock;
	int			is_card_busy;
	int			is_card_detect_progress;
	int			is_card_initialized;
	int			is_card_removed;
	int			is_init_progress;
	int			is_req_queued_up;
	struct mmc_host		*que_mmc_host;
	struct mmc_request	*que_mmc_request;
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	struct mmc_host		*mmc;
	struct device		*dev;
	struct clk		*clk;
	struct resource		*mem_res;
	void __iomem		*virt_base;
	unsigned int		phys_base;
	unsigned int		rw_threshold;
	unsigned int		option_read;
	unsigned int		option_write;
	int			flag_sd_mmc;
	int			irq;
	unsigned char		bus_mode;

#define DAVINCI_MMC_DATADIR_NONE	0
#define DAVINCI_MMC_DATADIR_READ	1
#define DAVINCI_MMC_DATADIR_WRITE	2
	unsigned char		data_dir;
	u32			*buffer;
	u32			bytes_left;

	int			use_dma;
	int			do_dma;
	unsigned int		dma_rx_event;
	unsigned int		dma_tx_event;

	struct timer_list	timer;
	unsigned int		is_core_command;
	unsigned int		cmd_code;
	unsigned int		old_card_state;
	unsigned int		new_card_state;
	/* Scatterlist DMA uses one or more parameter RAM entries:
	 * the main one (associated with rxdma or txdma) plus zero or
	 * more links.  The entries for a given transfer differ only
	 * by memory buffer (address, length) and link field.
	 */
	unsigned char		sd_support;

	struct edma_ch_mmcsd	edma_ch_details;

	unsigned int		sg_len;
	int			sg_idx;
	unsigned int		buffer_bytes_left;
};


extern void davinci_clean_channel(int ch_no);

/* MMCSD Init clock in Hz in opendain mode */
#define DAVINCI_MMC_INIT_CLOCK 		200000

/* This macro could not be defined to 0 (ZERO) or -ve value.
 * This value is multiplied to "HZ"
 * while requesting for timer interrupt every time for probing card.
 */
#define MULTIPLIER_TO_HZ 1

/* PIO only */
static void mmc_davinci_sg_to_buf(struct mmc_davinci_host *host)
{
	struct scatterlist *sg;

	sg = host->data->sg + host->sg_idx;
	host->buffer_bytes_left = sg->length;
	host->buffer = page_address(sg->page) + sg->offset;
	if (host->buffer_bytes_left > host->bytes_left)
		host->buffer_bytes_left = host->bytes_left;
}

static void davinci_fifo_data_trans(struct mmc_davinci_host *host,
					unsigned int n)
{
	u8 *p;
	unsigned int i;

	//dev_info(mmc_dev(host->mmc), "davinci_fifo_data_trans(%d)\n",n);

	if (host->buffer_bytes_left == 0) {
		host->sg_idx++;
		BUG_ON(host->sg_idx == host->sg_len);
		mmc_davinci_sg_to_buf(host);
	}

	p = host->buffer;
	if (n > host->buffer_bytes_left)
		n = host->buffer_bytes_left;
	host->buffer_bytes_left -= n;
	host->bytes_left -= n;

	/* NOTE:  we never transfer more than rw_threshold bytes
	 * to/from the fifo here; there's no I/O overlap.
	 * This also assumes that access width( i.e. ACCWD) is 4 bytes
	 */
	if (host->data_dir == DAVINCI_MMC_DATADIR_WRITE) {
		for (i = 0; i < (n >> 2); i++) {
			//writel(*((u32 *)p), host->base + DAVINCI_MMCDXR);
			DAVINCI_MMC_WRITEL(host, DXR, *((u32 *)p));
			p = p + 4;
		}
		if (n & 3) {
			outsb_p(host->virt_base + DAVINCI_MMC_REG_DXR, p, (n & 3));
			p = p + (n & 3);
		}
	} else {
		for (i = 0; i < (n >> 2); i++) {
			//*((u32 *)p) = readl(host->base + DAVINCI_MMCDRR);
			*((u32 *)p) = DAVINCI_MMC_READL(host, DRR);
			p  = p + 4;
		}
		if (n & 3) {
			insb_p(host->virt_base + DAVINCI_MMC_REG_DRR, p, (n & 3));
			p = p + (n & 3);
		}
	}
	host->buffer = p;
}

static void mmc_davinci_start_command(struct mmc_davinci_host *host,
		struct mmc_command *cmd)
{
	u32 cmdreg = 0;
	u32 resptype = 0;
	u32 cmdtype = 0;
	unsigned long flags;

	host->cmd = cmd;

	resptype = 0;
	cmdtype = 0;

	switch (RSP_TYPE(mmc_resp_type(cmd))) {
	case RSP_TYPE(MMC_RSP_R1):
		/* resp 1, resp 1b */
		resptype = 1;
		break;
	case RSP_TYPE(MMC_RSP_R2):
		resptype = 2;
		break;
	case RSP_TYPE(MMC_RSP_R3):
		resptype = 3;
		break;
	default:
		break;
	}

	/* Protocol layer does not provide command type, but our hardware
	 * needs it!
	 * any data transfer means adtc type (but that information is not
	 * in command structure, so we flagged it into host struct.)
	 * However, telling bc, bcr and ac apart based on response is
	 * not foolproof:
	 * CMD0  = bc  = resp0  CMD15 = ac  = resp0
	 * CMD2  = bcr = resp2  CMD10 = ac  = resp2
	 *
	 * Resolve to best guess with some exception testing:
	 * resp0 -> bc, except CMD15 = ac
	 * rest are ac, except if opendrain
	 */

	if (host->data_dir)
		cmdtype = DAVINCI_MMC_CMDTYPE_ADTC;
	else if (resptype == 0 && cmd->opcode != 15)
		cmdtype = DAVINCI_MMC_CMDTYPE_BC;
	else if (host->bus_mode == MMC_BUSMODE_OPENDRAIN)
		cmdtype = DAVINCI_MMC_CMDTYPE_BCR;
	else
		cmdtype = DAVINCI_MMC_CMDTYPE_AC;

	/*
	 * Set command Busy or not
	 * Linux core sending BUSY which is not defined for cmd 24
	 * as per mmc standard
	 */
	if (cmd->flags & MMC_RSP_BUSY)
		if (cmd->opcode != 24)
			cmdreg = cmdreg | (1 << 8);

	/* Set command index */
	cmdreg |= cmd->opcode;

	/* Setting initialize clock */
	if (cmd->opcode == 0)
		cmdreg = cmdreg | (1 << 14);

	/* Set for generating DMA Xfer event */
	if ((host->do_dma == 1) && (host->data != NULL)
	    && ((cmd->opcode == 18) || (cmd->opcode == 25)
		|| (cmd->opcode == 24) || (cmd->opcode == 17)))
		cmdreg = cmdreg | (1 << 16);

	/* Setting whether command involves data transfer or not */
	if (cmdtype == DAVINCI_MMC_CMDTYPE_ADTC)
		cmdreg = cmdreg | (1 << 13);

	/* Setting whether stream or block transfer */
	if (cmd->flags & MMC_DATA_STREAM)
		cmdreg = cmdreg | (1 << 12);

	/* Setting whether data read or write */
	if (host->data_dir == DAVINCI_MMC_DATADIR_WRITE)
		cmdreg = cmdreg | (1 << 11);

	/* Setting response type */
	cmdreg = cmdreg | (resptype << 9);

	if (host->bus_mode == MMC_BUSMODE_PUSHPULL)
		cmdreg = cmdreg | (1 << 7);

	/* set Command timeout */
	DAVINCI_MMC_WRITEW(host, TOR, 0xFFFF);

	/* Enable interrupt */
	if (host->data_dir == DAVINCI_MMC_DATADIR_WRITE) {
		if (host->do_dma != 1)
			DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_WRITE |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT |
				      DAVINCI_MMC_EVENT_BLOCK_XFERRED));
		else
			DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT |
				      DAVINCI_MMC_EVENT_BLOCK_XFERRED));
	} else if (host->data_dir == DAVINCI_MMC_DATADIR_READ) {
		if (host->do_dma != 1)
			DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_READ |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT |
				      DAVINCI_MMC_EVENT_BLOCK_XFERRED));
		else
			DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT |
				      DAVINCI_MMC_EVENT_BLOCK_XFERRED));
	} else
		DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT));

	/*
	 * It is required by controoler b4 WRITE command that
	 * FIFO should be populated with 32 bytes
	 */
	if ((host->data_dir == DAVINCI_MMC_DATADIR_WRITE) &&
	    (cmdtype == DAVINCI_MMC_CMDTYPE_ADTC) && (host->do_dma != 1))
		davinci_fifo_data_trans(host, 32);

	if (cmd->opcode == 7) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_removed = 0;
		host->new_card_state = 1;
		host->is_card_initialized = 1;
		host->old_card_state = host->new_card_state;
		host->is_init_progress = 0;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
	}
	if (cmd->opcode == 1 || cmd->opcode == 41) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_initialized = 0;
		host->is_init_progress = 1;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
	}

	host->is_core_command = 1;
	DAVINCI_MMC_WRITEL(host, ARGHL, cmd->arg);
	DAVINCI_MMC_WRITEL(host, CMD, cmdreg);
}

/*----------------------------------------------------------------------*/

/* DMA infrastructure */

static void davinci_abort_dma(struct mmc_davinci_host *host)
{
	int sync_dev = 0;

	if (host->data_dir == DAVINCI_MMC_DATADIR_READ)
		sync_dev = host->dma_tx_event;
	else
		sync_dev = host->dma_rx_event;

	davinci_stop_dma(sync_dev);
	davinci_clean_channel(sync_dev);

}


static void mmc_davinci_dma_cb(int lch, u16 ch_status, void *data)
{
	if (DMA_COMPLETE != ch_status) {
		struct mmc_davinci_host *host = (struct mmc_davinci_host *)data;
		dev_warn(host->mmc->dev, "[DMA FAILED]");
		davinci_abort_dma(host);
	}
}

static void davinci_reinit_chan(struct mmc_davinci_host *host)
{
	davinci_stop_dma(host->dma_tx_event);
	davinci_clean_channel(host->dma_tx_event);

	davinci_stop_dma(host->dma_rx_event);
	davinci_clean_channel(host->dma_rx_event);
}

static int mmc_davinci_send_dma_request(struct mmc_davinci_host *host,
					struct mmc_request *req)
{
	unsigned	sync_dev;
	u16	acnt = 4;
	u16	bcnt = rw_threshold >> 2;
	u16	ccnt = 0;
	u32		src_port = 0;
	u32		dst_port = 0;
	s16		src_bidx, dst_bidx;
	s16		src_cidx, dst_cidx;
	enum address_mode mode_src, mode_dst;
	enum fifo_width fifo_width_src, fifo_width_dst;
	unsigned short bcntrld;
	enum sync_dimension sync_mode;
	edmacc_paramentry_regs temp;
	int edma_chan_num;
	struct mmc_data *data = host->data;
	struct scatterlist *sg = &data->sg[0];
	unsigned			count = sg_dma_len(sg);
	const unsigned	shift = ffs(rw_threshold) - 1;

	//printk("mmc_davinci_send_dma_request\n");

	/*
	 * A-B Sync transfer:  each DMA request is for one "frame" of
	 * rw_threshold bytes, broken into "acnt"-size chunks repeated
	 * "bcnt" times.  Each segment needs "ccnt" such frames; since
	 * we tell the block layer our mmc->max_seg_size limit, we can
	 * trust (later) that it's within bounds.
	 *
	 * The FIFOs are read/written in 4-byte chunks (acnt == 4) and
	 * EDMA will optimize memory operations to use larger bursts.
	 */

	count = sg_dma_len(sg);

	if (count > host->bytes_left)
		count = host->bytes_left;

#if 0
	if(count & (rw_threshold - 1))
	{
		acnt = count;
		bcnt = 1;
		ccnt = 1;
	}
	else
	{
		ccnt = count >> shift;
	}
#else
	ccnt = count >> shift;
#endif

	if (host->data_dir == DAVINCI_MMC_DATADIR_WRITE) {
		/*AB Sync Transfer */
		sync_dev = host->dma_tx_event;

		src_port = (unsigned int)sg_dma_address(sg);
		mode_src = INCR;
		fifo_width_src = W8BIT;	/* It's not cared as modeDsr is INCR */
		src_bidx = acnt;
		src_cidx = acnt * bcnt;
		dst_port = host->phys_base + DAVINCI_MMC_REG_DXR;
		mode_dst = INCR;
		fifo_width_dst = W8BIT;	/* It's not cared as modeDsr is INCR */
		dst_bidx = 0;
		dst_cidx = 0;
		bcntrld = 8;
		sync_mode = ABSYNC;
	} else {
		sync_dev = host->dma_rx_event;

		src_port = host->phys_base + DAVINCI_MMC_REG_DRR;
		mode_src = INCR;
		fifo_width_src = W8BIT;
		src_bidx = 0;
		src_cidx = 0;
		dst_port = (unsigned int)sg_dma_address(sg);
		mode_dst = INCR;
		fifo_width_dst = W8BIT;	/* It's not cared as modeDsr is INCR */
		dst_bidx = acnt;
		dst_cidx = acnt * bcnt;
		bcntrld = 8;
		sync_mode = ABSYNC;
	}

	davinci_set_dma_src_params(sync_dev, src_port, mode_src,
				   fifo_width_src);
	davinci_set_dma_dest_params(sync_dev, dst_port, mode_dst,
				    fifo_width_dst);
	davinci_set_dma_src_index(sync_dev, src_bidx, src_cidx);
	davinci_set_dma_dest_index(sync_dev, dst_bidx, dst_cidx);
	davinci_set_dma_transfer_params(sync_dev, acnt, bcnt, ccnt, bcntrld,
					sync_mode);

	davinci_get_dma_params(sync_dev, &temp);

	//printk("opt = [0x%x]\n", temp.opt);
	/* don't bother with irqs or chaining */
	temp.opt |= sync_dev << 12;
#if 0
	if (sync_dev == host->dma_tx_event) {
		if (host->option_write == 0) {
			host->option_write = temp.opt;
		} else {
			temp.opt = host->option_write;
			davinci_set_dma_params(sync_dev, &temp);
		}
	}
	if (sync_dev == host->dma_rx_event) {
		if (host->option_read == 0) {
			host->option_read = temp.opt;
		} else {
			temp.opt = host->option_read;
			davinci_set_dma_params(sync_dev, &temp);
		}
	}
#endif
	temp.opt &= ~TCINTEN;
	davinci_set_dma_params(sync_dev, &temp);

	if (host->sg_len > 1)
	{

		unsigned			link;
		int				slot;
		unsigned			sg_len = host->sg_len - 1;
		unsigned			bytes_left = host->bytes_left - count;

		count = sg_dma_len(sg);

		/* We know sg_len and ccnt will never be out of range because
		 * we told the block layer to ensure that it only hands us one
		 * scatterlist segment per EDMA PARAM entry.  Update the PARAM
		 * entries needed for each segment of this scatterlist.
		 */
		davinci_get_dma_params(sync_dev, &temp);

		for (link = 0 ; sg_len > 0 && bytes_left ; link++, sg_len--)
		{
			sg = &data->sg[link+1];
			slot = host->edma_ch_details.chanel_num[link];
			count = sg_dma_len(sg);

			if(link > 0)
				davinci_get_dma_params(host->edma_ch_details.chanel_num[link-1], &temp);

			// in davinci_dma_link_lch
			//temp.link_bcntrld = sg_len
			//		? (host->edma_ch_details.chanel_num[link] << 5)
			//		: 0xffff;

			//if(sg_len == 0)
			//	temp.link_bcntrld = 0xffff;

			if (count > bytes_left)
				count = bytes_left;
			bytes_left -= count;

			temp.ccnt = count >> shift;

			if (sync_dev == host->dma_tx_event)
				temp.src = (unsigned int)sg_dma_address(sg);
			else
				temp.dst = (unsigned int)sg_dma_address(sg);

			//printk("#########slot = %d, sync_dev = %d, sg_len= %d, count = %d, bytes_left = %d\n", slot, sync_dev, sg_len, count, bytes_left);

			if(bytes_left == 0)
			{
				temp.opt |= TCINTEN;
			}
			
			davinci_set_dma_params(slot, &temp);

			if(link > 0)
				davinci_dma_link_lch(host->edma_ch_details.chanel_num[link-1], slot);
		}

		davinci_dma_link_lch(sync_dev, host->edma_ch_details.chanel_num[0]);
	}


	davinci_start_dma(sync_dev);
	return 0;
}

static int mmc_davinci_start_dma_transfer(struct mmc_davinci_host *host,
					  struct mmc_request *req)
{
	int i;
	int mask = rw_threshold - 1;
	struct mmc_data *data = host->data;

	//printk("mmc_davinci_start_dma_transfer\n");
	host->sg_len = dma_map_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
				((data->flags & MMC_DATA_WRITE)
				? DMA_TO_DEVICE
				: DMA_FROM_DEVICE));

	/* no individual DMA segment should need a partial FIFO */
	for (i = 0; i < host->sg_len; i++) {
		if (sg_dma_len(data->sg + i) & mask) {
			dma_unmap_sg(mmc_dev(host->mmc),
					data->sg, data->sg_len,
					(data->flags & MMC_DATA_WRITE)
					? DMA_TO_DEVICE
					: DMA_FROM_DEVICE);
			return -1;
		}
	}

	host->do_dma = 1;

	mmc_davinci_send_dma_request(host, req);

	return 0;

}

static int davinci_release_dma_channels(struct mmc_davinci_host *host)
{
	int i;
	
	davinci_free_dma(host->dma_tx_event);
	davinci_free_dma(host->dma_rx_event);

	for(i = 0 ; i <host->edma_ch_details.cnt_chanel ; i++)
	{
		davinci_free_dma(host->edma_ch_details.chanel_num[i]);
	}

	host->edma_ch_details.cnt_chanel = 0;

	return 0;
}

static int davinci_acquire_dma_channels(struct mmc_davinci_host *host)
{
	int edma_chan_num, tcc = 0, r, sync_dev, i;
	enum dma_event_q queue_no = EVENTQ_0;

	/* Acquire master DMA write channel */
	r = davinci_request_dma(host->dma_tx_event, "MMC_WRITE",
				     mmc_davinci_dma_cb, host,
				     &edma_chan_num, &tcc, queue_no);
	if (r) {
		dev_warn(host->mmc->dev,
			 "MMC: davinci_request_dma() failed with %d\n", r);
		return r;
	}

	/* Acquire master DMA read channel */
	r = davinci_request_dma(host->dma_rx_event, "MMC_READ",
				     mmc_davinci_dma_cb, host,
				     &edma_chan_num, &tcc, queue_no);
	if (r) {
		dev_warn(host->mmc->dev,
			 "MMC: davinci_request_dma() failed with %d\n", r);
		goto free_master_write;
	}

	host->edma_ch_details.cnt_chanel = 0;

	/* currently data Writes are done using single block mode,
	 * so no DMA slave write channel is required for now */

	/* Create a DMA slave read channel
	 * (assuming max segments handled is 2) */

	for(i = 0 ; i < (NR_SG - 1) ; i++)
	{
		sync_dev = host->dma_rx_event;
		r = davinci_request_dma(DAVINCI_EDMA_PARAM_ANY, "LINK",
					     NULL, NULL, &edma_chan_num,
					     &sync_dev, queue_no);
		if (r) {
			if(i == 0)
			{
				dev_warn(host->mmc->dev,
					 "MMC: davinci_request_dma() failed with %d\n", r);
				goto free_master_read;
			}
			else
				break;
		}

		host->edma_ch_details.cnt_chanel++;
		host->edma_ch_details.chanel_num[i] = edma_chan_num;
	}

	printk("Acquired channel num = [%d]\n", host->edma_ch_details.cnt_chanel);

	return 0;

free_master_read:
	davinci_free_dma(host->dma_rx_event);
free_master_write:
	davinci_free_dma(host->dma_tx_event);

	return r;
}

/*----------------------------------------------------------------------*/

static void
mmc_davinci_prepare_data(struct mmc_davinci_host *host, struct mmc_request *req)
{
	int fifo_lev = (rw_threshold == 32) ? MMCFIFOCTL_FIFOLEV : 0;
	int timeout;
	struct mmc_data *data = req->data;

	//printk("mmc_davinci_prepare_data\n");
	
	host->data = data;
	if (data == NULL) {
		host->data_dir = DAVINCI_MMC_DATADIR_NONE;
		writel(0, host->virt_base + DAVINCI_MMC_REG_BLEN);
		writel(0, host->virt_base + DAVINCI_MMC_REG_NBLK);
		//printk("mmc_davinci_prepare_data........DATA=0x%x, REQ_DATA=0x%x\n", (unsigned int) data, (unsigned int) req->data);
		return;
	}

	dev_dbg(mmc_dev(host->mmc), "%s %s, %d blocks of %d bytes\n",
		(data->flags & MMC_DATA_STREAM) ? "stream" : "block",
		(data->flags & MMC_DATA_WRITE) ? "write" : "read",
		data->blocks, data->blksz);
	dev_dbg(mmc_dev(host->mmc), "  DTO %d cycles + %d ns\n",
		data->timeout_clks, data->timeout_ns);

	/* Convert ns to clock cycles by assuming 20MHz frequency
	 * 1 cycle at 20MHz = 500 ns
	 */
	timeout = data->timeout_clks + data->timeout_ns / 500;
	if (timeout > 0xffff)
		timeout = 0xffff;

	writel(timeout, host->virt_base + DAVINCI_MMC_REG_TOD);
	writel(data->blocks, host->virt_base + DAVINCI_MMC_REG_NBLK);
	writel(data->blksz, host->virt_base + DAVINCI_MMC_REG_BLEN);

	/* Configure the FIFO */
	switch (data->flags & MMC_DATA_WRITE) {
	case MMC_DATA_WRITE:
		host->data_dir = DAVINCI_MMC_DATADIR_WRITE;
		writel(fifo_lev | MMCFIFOCTL_FIFODIR_WR | MMCFIFOCTL_FIFORST,
			host->virt_base + DAVINCI_MMC_REG_FIFO_CTL);
		writel(fifo_lev | MMCFIFOCTL_FIFODIR_WR,
			host->virt_base + DAVINCI_MMC_REG_FIFO_CTL);
		break;

	default:
		host->data_dir = DAVINCI_MMC_DATADIR_READ;
		writel(fifo_lev | MMCFIFOCTL_FIFODIR_RD | MMCFIFOCTL_FIFORST,
			host->virt_base + DAVINCI_MMC_REG_FIFO_CTL);
		writel(fifo_lev | MMCFIFOCTL_FIFODIR_RD,
			host->virt_base + DAVINCI_MMC_REG_FIFO_CTL);
		break;
	}

	host->buffer = NULL;
	host->sg_len = data->sg_len;
	host->bytes_left = data->blocks * data->blksz;

	/* For now we try to use DMA whenever we won't need partial FIFO
	 * reads or writes, either for the whole transfer (as tested here)
	 * or for any individual scatterlist segment (tested when we call
	 * start_dma_transfer).
	 *
	 * While we *could* change that, unusual block sizes are rarely
	 * used.  The occasional fallback to PIO should't hurt.
	 */
	if (host->use_dma && (host->bytes_left & (rw_threshold - 1)) == 0
			&& mmc_davinci_start_dma_transfer(host, req) == 0) {
		/* zero this to ensure we take no PIO paths */
		host->bytes_left = 0;
	} else {
		/* Revert to CPU Copy */
		//printk("Revert to CPU Copy\n");
		host->sg_idx = 0;
		host->do_dma = 0;
		mmc_davinci_sg_to_buf(host);
	}
}

static void mmc_davinci_request(struct mmc_host *mmc, struct mmc_request *req)
{
	struct mmc_davinci_host *host = mmc_priv(mmc);
	unsigned long timeout = jiffies + msecs_to_jiffies(900);
	u32 mmcst1 = 0;
	unsigned long flags;
	//printk("mmc_davinci_request...req->data... [0x%x]\n", (unsigned int) req->data);

	/* Card may still be sending BUSY after a previous operation,
	 * typically some kind of write.  If so, we can't proceed yet.
	 */
	while (time_before(jiffies, timeout)) {
		mmcst1  = readl(host->virt_base + DAVINCI_MMC_REG_ST1);
		if (!(mmcst1 & MMCST1_BUSY))
			break;
		cpu_relax();
	}
	if (mmcst1 & MMCST1_BUSY) {
		dev_err(mmc_dev(host->mmc), "still BUSY? bad ... \n");
		req->cmd->error = -ETIMEDOUT;
		mmc_request_done(mmc, req);
		return;
	}

	if (!host->is_card_detect_progress) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_busy = 1;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
		host->do_dma = 0;
		mmc_davinci_prepare_data(host, req);
		mmc_davinci_start_command(host, req->cmd);
	} else {
		/* Queue up the request as card dectection is being excuted */
		host->que_mmc_host = mmc;
		host->que_mmc_request = req;
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_req_queued_up = 1;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
	}
}

static unsigned int calculate_freq_for_card(struct mmc_davinci_host *host,
			unsigned int mmc_req_freq)
{
	unsigned int mmc_freq, cpu_arm_clk, mmc_push_pull;

	cpu_arm_clk = clk_get_rate(host->clk);

	if (cpu_arm_clk > (2 * mmc_req_freq)) {
		mmc_push_pull =
		    ((unsigned int)cpu_arm_clk / (2 * mmc_req_freq)) - 1;
	} else
		mmc_push_pull = 0;

	mmc_freq = (unsigned int)cpu_arm_clk / (2 * (mmc_push_pull + 1));

	if (mmc_freq > mmc_req_freq)
		mmc_push_pull = mmc_push_pull + 1;

	return mmc_push_pull;
}

#if 1
static void mmc_davinci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	unsigned int open_drain_freq = 0, cpu_arm_clk = 0;
	unsigned int mmc_push_pull_freq = 0;
	struct mmc_davinci_host *host = mmc_priv(mmc);

	//cpu_arm_clk = host->mmc_input_clk;
	cpu_arm_clk = clk_get_rate(host->clk);

	dev_dbg(mmc_dev(host->mmc),
		"clock %dHz busmode %d powermode %d Vdd %04x\n",
		ios->clock, ios->bus_mode, ios->power_mode,
		ios->vdd);
	if (ios->bus_width == MMC_BUS_WIDTH_4) {
		dev_dbg(mmc_dev(host->mmc), "Enabling 4 bit mode\n");
		writel(readl(host->virt_base + DAVINCI_MMC_REG_CTL) | MMCCTL_WIDTH_4_BIT,
			host->virt_base + DAVINCI_MMC_REG_CTL);
	} else {
		dev_dbg(mmc_dev(host->mmc), "Disabling 4 bit mode\n");
		writel(readl(host->virt_base + DAVINCI_MMC_REG_CTL) & ~MMCCTL_WIDTH_4_BIT,
			host->virt_base + DAVINCI_MMC_REG_CTL);
	}

	if (ios->bus_mode == MMC_BUSMODE_OPENDRAIN) {
		u32 temp;
		open_drain_freq = ((unsigned int)cpu_arm_clk
				/ (2 * DAVINCI_MMC_INIT_CLOCK)) - 1;
		temp = readl(host->virt_base + DAVINCI_MMC_REG_CLK) & ~0xFF;
		temp |= open_drain_freq;
		writel(temp, host->virt_base + DAVINCI_MMC_REG_CLK);
	} else {
		u32 temp;
		mmc_push_pull_freq = calculate_freq_for_card(host, ios->clock);

		temp = readl(host->virt_base + DAVINCI_MMC_REG_CLK) & ~MMCCLK_CLKEN;
		writel(temp, host->virt_base + DAVINCI_MMC_REG_CLK);

		udelay(10);

		temp = readl(host->virt_base + DAVINCI_MMC_REG_CLK) & ~MMCCLK_CLKRT_MASK;
		temp |= mmc_push_pull_freq;
		writel(temp, host->virt_base + DAVINCI_MMC_REG_CLK);

		writel(temp | MMCCLK_CLKEN, host->virt_base + DAVINCI_MMC_REG_CLK);

		udelay(10);
	}

	host->bus_mode = ios->bus_mode;
	if (ios->power_mode == MMC_POWER_UP) {
		unsigned long timeout = jiffies + msecs_to_jiffies(50);
		int lose = 1;

		/* Send clock cycles, poll completion */
		writel(0, host->virt_base + DAVINCI_MMC_REG_ARGHL);
		writel(MMCCMD_INITCK, host->virt_base + DAVINCI_MMC_REG_CMD);
		while (time_before(jiffies, timeout)) {
			u32 tmp = readl(host->virt_base + DAVINCI_MMC_REG_ST0);

			if (tmp & MMCST0_RSPDNE) {
				lose = 0;
				break;
			}
			cpu_relax();
		}
		if (lose)
			dev_warn(mmc_dev(host->mmc), "powerup timeout\n");
	}

	/* FIXME on power OFF, reset things ... */
}

#else
static void mmc_davinci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	unsigned short status;
	unsigned int open_drain_freq, cpu_arm_clk;
	unsigned int mmc_push_pull_freq;
	u16 reg;
	struct mmc_davinci_host *host = mmc_priv(mmc);

	cpu_arm_clk = clk_get_rate(host->clk);
	dev_dbg(host->mmc->dev, "clock %dHz busmode %d powermode %d \
			Vdd %d.%02d\n",
		ios->clock, ios->bus_mode, ios->power_mode,
		ios->vdd / 100, ios->vdd % 100);
	if (ios->bus_width == MMC_BUS_WIDTH_4) {
		dev_dbg(host->mmc->dev, "\nEnabling 4 bit mode\n");
		reg = DAVINCI_MMC_READW(host, CTL);
		reg  |= (1 << 2);
		DAVINCI_MMC_WRITEW(host, CTL, reg);
	} else {
		dev_dbg(host->mmc->dev, "Disabling 4 bit mode\n");
		reg = DAVINCI_MMC_READW(host, CTL);
		reg &= ~(1 << 2);
		DAVINCI_MMC_WRITEW(host, CTL, reg);
	}

	if (ios->bus_mode == MMC_BUSMODE_OPENDRAIN) {
		open_drain_freq =
		    ((unsigned int)(cpu_arm_clk + 2*DAVINCI_MMC_INIT_CLOCK - 1)
			/ (2 * DAVINCI_MMC_INIT_CLOCK)) - 1;
		if (open_drain_freq > 0xff)
			open_drain_freq = 0xff;
		DAVINCI_MMC_WRITEW(host, CLK, open_drain_freq | 0x100);

	} else {
		mmc_push_pull_freq = calculate_freq_for_card(host, ios->clock);
		reg = DAVINCI_MMC_READW(host, CLK);
		reg &= ~(0x100);
		DAVINCI_MMC_WRITEW(host, CLK, reg);
		udelay(10);
		DAVINCI_MMC_WRITEW(host, CLK, mmc_push_pull_freq | 0x100);
		udelay(10);
	}
	host->bus_mode = ios->bus_mode;
	if (ios->power_mode == MMC_POWER_UP) {
		/* Send clock cycles, poll completion */
		reg = DAVINCI_MMC_READW(host, IM);
		DAVINCI_MMC_WRITEW(host, IM, 0);
		DAVINCI_MMC_WRITEL(host, ARGHL, 0x0);
		DAVINCI_MMC_WRITEL(host, CMD, 0x4000);
		status = 0;
		while (!(status & (DAVINCI_MMC_EVENT_EOFCMD)))
			status = DAVINCI_MMC_READW(host, ST0);

		DAVINCI_MMC_WRITEW(host, IM, reg);
	}
}
#endif

static void mmc_davinci_xfer_done(struct mmc_davinci_host *host,
				  struct mmc_data *data)
{
	unsigned long flags;
	host->data = NULL;
	host->data_dir = DAVINCI_MMC_DATADIR_NONE;
	if (data->error == MMC_ERR_NONE)
		data->bytes_xfered += data->blocks * data->blksz;

	if (host->do_dma) {
		davinci_abort_dma(host);

		dma_unmap_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
			     (data->flags & MMC_DATA_WRITE)
			     ? DMA_TO_DEVICE
			     : DMA_FROM_DEVICE);
		host->do_dma = 0;
	}

	if (data->error == MMC_ERR_TIMEOUT) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_busy = 0;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
		mmc_request_done(host->mmc, data->mrq);
		writel(0, host->virt_base + DAVINCI_MMC_REG_IM);
		return;
	}

	if (!data->stop) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_busy = 0;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
		mmc_request_done(host->mmc, data->mrq);
		writel(0, host->virt_base + DAVINCI_MMC_REG_IM);
		return;
	}
	mmc_davinci_start_command(host, data->stop);
}

static void mmc_davinci_cmd_done(struct mmc_davinci_host *host,
				 struct mmc_command *cmd)
{
	unsigned long flags;
	host->cmd = NULL;

	if (!cmd) {
		dev_warn(host->mmc->dev, "%s(): No cmd ptr\n", __func__);
		return;
	}

	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136) {
			/* response type 2 */
			cmd->resp[3] = DAVINCI_MMC_READL(host, RSP01);
			cmd->resp[2] = DAVINCI_MMC_READL(host, RSP23);
			cmd->resp[1] = DAVINCI_MMC_READL(host, RSP45);
			cmd->resp[0] = DAVINCI_MMC_READL(host, RSP67);
		} else {
		/* response types 1, 1b, 3, 4, 5, 6 */
		cmd->resp[0] = DAVINCI_MMC_READL(host, RSP67);
		}
	}

	if (host->data == NULL || cmd->error != MMC_ERR_NONE) {
		if (cmd->error == MMC_ERR_TIMEOUT)
			cmd->mrq->cmd->retries = 0;
		spin_lock_irqsave(&host->mmc_lock, flags);
		host->is_card_busy = 0;
		spin_unlock_irqrestore(&host->mmc_lock, flags);
		mmc_request_done(host->mmc, cmd->mrq);
		writel(0, host->virt_base + DAVINCI_MMC_REG_IM);
	}

}

static irqreturn_t mmc_davinci_irq(int irq, void *dev_id, struct pt_regs *regs)
{
	struct mmc_davinci_host *host = (struct mmc_davinci_host *)dev_id;
	u16 status;
	int end_command = 0;
	int end_transfer = 0;
	unsigned long flags;
	u16 reg;

	if (host->is_core_command) {
		if (host->cmd == NULL && host->data == NULL) {
			status = DAVINCI_MMC_READW(host, ST0);
			dev_dbg(host->mmc->dev, "Spurious interrupt 0x%04x\n",
				status);
			/* Disable the interrupt from mmcsd */
			DAVINCI_MMC_WRITEW(host, IM, 0);
			return IRQ_HANDLED;
		}
	}

	status = DAVINCI_MMC_READW(host, ST0);
	if (status == 0)
		return IRQ_HANDLED;

	if (host->is_core_command) {
		if (host->is_card_initialized) {
			if (host->new_card_state == 0) {
				if (host->cmd) {
					host->cmd->error |= MMC_ERR_TIMEOUT;
					mmc_davinci_cmd_done(host, host->cmd);
				}
				dev_dbg(host->mmc->dev,
					"From code segment excuted when card \
					removed\n");
				return IRQ_HANDLED;
			}
		}

		while (status != 0) {
			if (host->data_dir == DAVINCI_MMC_DATADIR_WRITE) {
				if (status & DAVINCI_MMC_EVENT_WRITE) {
					/* Buffer almost empty */
					if (host->bytes_left > 0)
						davinci_fifo_data_trans(host, host->bytes_left);
				}
			}

			if (host->data_dir == DAVINCI_MMC_DATADIR_READ)
				if (status & DAVINCI_MMC_EVENT_READ)
					/* Buffer almost empty */
					if (host->bytes_left > 0)
						davinci_fifo_data_trans(host, host->bytes_left);

			if (status & DAVINCI_MMC_EVENT_BLOCK_XFERRED) {
				/* Block sent/received */
				if (host->data != NULL) {
					if (host->do_dma == 1)
						end_transfer = 1;
					else {
						/* if datasize<32 no RX ints
						 * are generated */
						if (host->bytes_left > 0)
							davinci_fifo_data_trans(host, host->bytes_left);
						end_transfer = 1;
					}
				} else
					dev_warn(host->mmc->dev,
						 "TC:host->data is NULL\n");
			}

			if (status & DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT) {
				/* Data timeout */
				if ((host->data) &&
						(host->new_card_state != 0)) {
					host->data->error |= MMC_ERR_TIMEOUT;
					spin_lock_irqsave(&host->mmc_lock,
							flags);
					host->is_card_removed = 1;
					host->new_card_state = 0;
					host->is_card_initialized = 0;
					spin_unlock_irqrestore(&host->mmc_lock,
							       flags);
					dev_dbg(host->mmc->dev,
						"MMCSD: Data timeout, CMD%d \
						and status is %x\n",
						host->cmd->opcode, status);

					if (host->cmd)
						host->cmd->error |=
						    MMC_ERR_TIMEOUT;
					end_transfer = 1;
				}
			}

			if (status & DAVINCI_MMC_EVENT_ERROR_DATACRC) {
				/* DAT line portion is diabled and in reset
				 * state */
				reg = DAVINCI_MMC_READW(host, CTL);
				reg |= (1 << 1);
				DAVINCI_MMC_WRITEW(host, CTL, reg);
				udelay(10);
				reg = DAVINCI_MMC_READW(host, CTL);
				reg &= ~(1 << 1);
				DAVINCI_MMC_WRITEW(host, CTL, reg);

				/* Data CRC error */
				if (host->data) {
					host->data->error |= MMC_ERR_BADCRC;
					dev_dbg(host->mmc->dev,
						"MMCSD: Data CRC error, bytes \
						left %d\n",
						host->bytes_left);
					end_transfer = 1;
				} else
					dev_dbg(host->mmc->dev,
						"MMCSD: Data CRC error\n");
			}

			if (status & DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT) {
				if (host->do_dma)
					davinci_abort_dma(host);

				/* Command timeout */
				if (host->cmd) {
					/* Timeouts are normal in case of
					 * MMC_SEND_STATUS
					 */
					if (host->cmd->opcode !=
					    MMC_ALL_SEND_CID) {
						dev_dbg(host->mmc->dev,
							"MMCSD: Command \
							timeout, CMD%d and \
							status is %x\n",
							host->cmd->opcode,
							status);
						spin_lock_irqsave(
								&host->mmc_lock,
								  flags);
						host->new_card_state = 0;
						host->is_card_initialized = 0;
						spin_unlock_irqrestore
						    (&host->mmc_lock, flags);
					}
					host->cmd->error |= MMC_ERR_TIMEOUT;
					end_command = 1;

				}
			}

			if (status & DAVINCI_MMC_EVENT_ERROR_CMDCRC) {
				/* Command CRC error */
				dev_dbg(host->mmc->dev, "Command CRC error\n");
				if (host->cmd) {
					/* Ignore CMD CRC errors during high
					 * speed operation */
					if (host->mmc->ios.clock <= 25000000) {
						host->cmd->error |=
						    MMC_ERR_BADCRC;
					}
					end_command = 1;
				}
			}

			if (status & DAVINCI_MMC_EVENT_EOFCMD)
				end_command = 1;

			if (host->data == NULL) {
				status = DAVINCI_MMC_READW(host, ST0);
				if (status != 0) {
					dev_dbg(host->mmc->dev,
						"Status is %x at end of ISR \
						when host->data is NULL",
						status);
					status = 0;

				}
			} else
				status = DAVINCI_MMC_READW(host, ST0);
		}

		if (end_command)
		mmc_davinci_cmd_done(host, host->cmd);
	if (end_transfer)
			mmc_davinci_xfer_done(host, host->data);
	} else {
		if (host->cmd_code == 13) {
			if (status & DAVINCI_MMC_EVENT_EOFCMD) {
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->new_card_state = 1;
				spin_unlock_irqrestore(&host->mmc_lock, flags);

			} else {
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->is_card_removed = 1;
				host->new_card_state = 0;
				host->is_card_initialized = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			}

			spin_lock_irqsave(&host->mmc_lock, flags);
			host->is_card_detect_progress = 0;
			spin_unlock_irqrestore(&host->mmc_lock, flags);

			if (host->is_req_queued_up) {
				mmc_davinci_request(host->que_mmc_host,
						    host->que_mmc_request);
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->is_req_queued_up = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			}

		}

		if (host->cmd_code == 1 || host->cmd_code == 55) {
			if (status & DAVINCI_MMC_EVENT_EOFCMD) {
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->is_card_removed = 0;
				host->new_card_state = 1;
				host->is_card_initialized = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			} else {

				spin_lock_irqsave(&host->mmc_lock, flags);
				host->is_card_removed = 1;
				host->new_card_state = 0;
				host->is_card_initialized = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			}

			spin_lock_irqsave(&host->mmc_lock, flags);
			host->is_card_detect_progress = 0;
			spin_unlock_irqrestore(&host->mmc_lock, flags);

			if (host->is_req_queued_up) {
				mmc_davinci_request(host->que_mmc_host,
						    host->que_mmc_request);
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->is_req_queued_up = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			}
		}

		if (host->cmd_code == 0) {
			if (status & DAVINCI_MMC_EVENT_EOFCMD) {
				host->is_core_command = 0;

				if (host->flag_sd_mmc) {
					host->flag_sd_mmc = 0;
					host->cmd_code = 1;
					/* Issue cmd1 */
					DAVINCI_MMC_WRITEL(host, ARGHL,
							0x80300000);
					DAVINCI_MMC_WRITEL(host, CMD,
							0x00000601);
				} else {
					host->flag_sd_mmc = 1;
					host->cmd_code = 55;
					/* Issue cmd55 */
					DAVINCI_MMC_WRITEL(host, ARGHL, 0x0);
					DAVINCI_MMC_WRITEL(host, CMD,
					    ((0x0 | (1 << 9) | 55)));
				}

				dev_dbg(host->mmc->dev,
					"MMC-Probing mmc with cmd%d\n",
					host->cmd_code);
			} else {
				spin_lock_irqsave(&host->mmc_lock, flags);
				host->new_card_state = 0;
				host->is_card_initialized = 0;
				host->is_card_detect_progress = 0;
				spin_unlock_irqrestore(&host->mmc_lock, flags);
			}
		}
	}
	return IRQ_HANDLED;

}

static int mmc_davinci_get_ro(struct mmc_host *mmc)
{
	return 0;
}

static struct mmc_host_ops mmc_davinci_ops = {
	.request = mmc_davinci_request,
	.set_ios = mmc_davinci_set_ios,
	.get_ro = mmc_davinci_get_ro
};

void mmc_check_card(unsigned long data)
{
	struct mmc_davinci_host *host = (struct mmc_davinci_host *)data;
	unsigned long flags;
	struct mmc_card *card = NULL;

	if (host->mmc && host->mmc->card_selected)
		card = host->mmc->card_selected;

	if ((!host->is_card_detect_progress) || (!host->is_init_progress)) {
		if (host->is_card_initialized) {
			host->is_core_command = 0;
			host->cmd_code = 13;
			spin_lock_irqsave(&host->mmc_lock, flags);
			host->is_card_detect_progress = 1;
			spin_unlock_irqrestore(&host->mmc_lock, flags);
			/* Issue cmd13 */
			DAVINCI_MMC_WRITEL(host, ARGHL, (card && (card->state
							 & MMC_STATE_SDCARD))
					? (card->rca << 16) : 0x10000);
			DAVINCI_MMC_WRITEL(host, CMD, 0x0000028D);
		} else {
			host->is_core_command = 0;
			host->cmd_code = 0;
			spin_lock_irqsave(&host->mmc_lock, flags);
			host->is_card_detect_progress = 1;
			spin_unlock_irqrestore(&host->mmc_lock, flags);
			/* Issue cmd0 */
			DAVINCI_MMC_WRITEL(host, ARGHL, 0);
			DAVINCI_MMC_WRITEL(host, CMD, 0x4000);
		}
		DAVINCI_MMC_WRITEW(host, IM, (DAVINCI_MMC_EVENT_EOFCMD |
				      DAVINCI_MMC_EVENT_ERROR_CMDCRC |
				      DAVINCI_MMC_EVENT_ERROR_DATACRC |
				      DAVINCI_MMC_EVENT_ERROR_CMDTIMEOUT |
				      DAVINCI_MMC_EVENT_ERROR_DATATIMEOUT));

	}
}

static void init_mmcsd_host(struct mmc_davinci_host *host)
{
	u16 reg;
	/* CMD line portion is disabled and in reset state */
	reg = DAVINCI_MMC_READW(host, CTL);
	reg |= 0x1;
	DAVINCI_MMC_WRITEW(host, CTL, reg);
	/* DAT line portion is disabled and in reset state */
	reg = DAVINCI_MMC_READW(host, CTL);
	reg |= (1 << 1);
	DAVINCI_MMC_WRITEW(host, CTL, reg);
	udelay(10);

	DAVINCI_MMC_WRITEW(host, CLK, 0x0);
	reg = DAVINCI_MMC_READW(host, CLK);
	reg |= (1 << 8);
	DAVINCI_MMC_WRITEW(host, CLK, reg);

	DAVINCI_MMC_WRITEW(host, TOR, 0xFFFF);
	DAVINCI_MMC_WRITEW(host, TOD, 0xFFFF);

	reg = DAVINCI_MMC_READW(host, CTL);
	reg &= ~(0x1);
	DAVINCI_MMC_WRITEW(host, CTL, reg);
	reg = DAVINCI_MMC_READW(host, CTL);
	reg &= ~(1 << 1);
	DAVINCI_MMC_WRITEW(host, CTL, reg);
	udelay(10);
}

static void davinci_mmc_check_status(unsigned long data)
{
	unsigned long flags;
	struct mmc_davinci_host *host = (struct mmc_davinci_host *)data;
	if (!host->is_card_busy) {
		if (host->old_card_state ^ host->new_card_state) {
			davinci_reinit_chan(host);
			init_mmcsd_host(host);
			mmc_detect_change(host->mmc, 0);
			spin_lock_irqsave(&host->mmc_lock, flags);
			host->old_card_state = host->new_card_state;
			spin_unlock_irqrestore(&host->mmc_lock, flags);
		} else {
			mmc_check_card(data);
		}

	}
	mod_timer(&host->timer, jiffies + MULTIPLIER_TO_HZ * HZ);
}

static int davinci_mmcsd_probe(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct davinci_mmc_platform_data *minfo = pdev->dev.platform_data;
	struct mmc_host *mmc;
	struct mmc_davinci_host *host = NULL;
	struct resource *res;
	int ret = 0;
	int irq;
	//printk("[ENTER] : davinci_mmcsd_probe#################\n");
	
	if (minfo == NULL) {
		dev_err(&pdev->dev, "platform data missing\n");
		return -ENXIO;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (res == NULL || irq < 0)
		return -ENXIO;

	res = request_mem_region(res->start, res->end - res->start + 1,
				 pdev->name);
	if (res == NULL)
		return -EBUSY;

	mmc = mmc_alloc_host(sizeof(struct mmc_davinci_host), &pdev->dev);
	if (mmc == NULL) {
		ret = -ENOMEM;
		goto err_free_mem_region;
	}

	host = mmc_priv(mmc);
	host->mmc = mmc;

	spin_lock_init(&host->mmc_lock);

	host->mem_res = res;
	host->irq = irq;

	host->phys_base = host->mem_res->start;
	host->virt_base = (void __iomem *) IO_ADDRESS(host->phys_base);

	host->use_dma = 0;

	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (res > 0) {
		host->dma_rx_event = res->start;
		res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
		if (res > 0) {
			host->dma_tx_event = res->start;
			host->use_dma = 1;
		} else
			host->dma_rx_event = 0;
	}

	host->clk = clk_get(&pdev->dev, minfo->mmc_clk);
	if (IS_ERR(host->clk)) {
		ret = -ENODEV;
		goto err_free_mmc_host;
	}

	clk_enable(host->clk);

	init_mmcsd_host(host);

	if (minfo->use_4bit_mode) {
		dev_info(mmc->dev, "Supporting 4-bit mode\n");
		mmc->caps |= MMC_CAP_4_BIT_DATA;
	} else {
		dev_info(mmc->dev, "Not Supporting 4-bit mode\n");
	}

	host->rw_threshold = minfo->rw_threshold;
	//printk("==============> RW_THRESHOLD = [%d]\n", host->rw_threshold);
	mmc->ops = &mmc_davinci_ops;
	mmc->f_min = 312500;
#define CONFIG_MMC_HIGHSPEED	
#ifdef CONFIG_MMC_HIGHSPEED
	mmc->f_max = 50000000;
#else
	mmc->f_max = 25000000;
#endif
	mmc->ocr_avail = MMC_VDD_32_33|MMC_VDD_33_34;

	mmc->caps |= MMC_CAP_MULTIWRITE;
	


	if (host->use_dma) {
		dev_info(mmc->dev, "Using DMA mode\n");
		ret = davinci_acquire_dma_channels(host);
		if (ret)
			goto err_release_clk;
	} else {
		dev_info(mmc->dev, "Not Using DMA mode\n");
	}


	// 2,2,256 - mv 2.6.18 - slow... ...888 or 8816
	mmc->max_phys_segs = host->edma_ch_details.cnt_chanel + 1;//8;
	mmc->max_hw_segs = mmc->max_phys_segs;//8;
	//mmc->max_sectors = 8;//256;//8; //256

	/* Restrict the max size of seg we can handle */
	//mmc->max_seg_size = mmc->max_sectors * 512;
	mmc->max_seg_size	= MAX_C_CNT * host->rw_threshold;
	//mmc->max_seg_size = (1<<16 -1) * 32;//mmc->max_sectors * 512;

	/* MMC/SD controller limits for multiblock requests */
	mmc->max_blk_size	= 4095;  /* BLEN is 11 bits */
	mmc->max_blk_count	= 65535; /* NBLK is 16 bits */
	mmc->max_req_size	= mmc->max_blk_size * mmc->max_blk_count;

	dev_info(mmc->dev, "max_phys_segs=%d\n", mmc->max_phys_segs);
	dev_info(mmc->dev, "max_hw_segs=%d\n", mmc->max_hw_segs);
	//dev_info(mmc->dev, "max_sect=%d\n", mmc->max_sectors);
	dev_info(mmc->dev, "max_seg_size=%d\n", mmc->max_seg_size);


	host->sd_support = 1;
	ret = request_irq(host->irq, mmc_davinci_irq, 0, DRIVER_NAME, host);

	if (ret)
		goto err_release_dma;

	host->dev = &pdev->dev;
	dev_set_drvdata(dev, host);
	mmc_add_host(mmc);

	init_timer(&host->timer);
	host->timer.data = (unsigned long)host;
	host->timer.function = davinci_mmc_check_status;
	host->timer.expires = jiffies + MULTIPLIER_TO_HZ * HZ;
	add_timer(&host->timer);

	//printk("[LEAVE] : SUCCESS!!! #################\n");
	return 0;

err_release_dma:
	davinci_release_dma_channels(host);
err_release_clk:
	clk_disable(host->clk);
	clk_put(host->clk);
err_free_mmc_host:
	mmc_free_host(mmc);
err_free_mem_region:
	release_mem_region(res->start, res->end - res->start + 1);

	printk("[LEAVE] : ERROR davinci_mmcsd_probe#################[%d]\n", ret);

	return ret;
}

static int davinci_mmcsd_remove(struct device *dev)
{
	struct mmc_davinci_host *host = dev_get_drvdata(dev);
	unsigned long flags;

	if (host) {
		spin_lock_irqsave(&host->mmc_lock, flags);
		del_timer(&host->timer);
		spin_unlock_irqrestore(&host->mmc_lock, flags);

		mmc_remove_host(host->mmc);
		dev_set_drvdata(dev, NULL);
		free_irq(host->irq, host);
		davinci_release_dma_channels(host);
		clk_disable(host->clk);
		clk_put(host->clk);
		release_resource(host->mem_res);
		kfree(host->mem_res);
		mmc_free_host(host->mmc);
	}

	return 0;
}

#ifdef CONFIG_PM
static int davinci_mmcsd_suspend(struct device *dev, u32 state, u32 level)
{
	struct mmc_host *mmc = dev_get_drvdata(dev);
	int ret = 0;

	if (mmc && level == SUSPEND_DISABLE)
		ret = mmc_suspend_host(mmc, state);

	return ret;
}

static int davinci_mmcsd_resume(struct device *dev, u32 level)
{
	struct mmc_host *mmc = dev_get_drvdata(dev);
	int ret = 0;

	if (mmc && level == RESUME_ENABLE)
		ret = mmc_resume_host(mmc);

	return ret;
}

#else

#define davinci_mmcsd_suspend	NULL
#define davinci_mmcsd_resume	NULL

#endif

static struct device_driver davinci_mmcsd_driver = {
	.name = DRIVER_NAME,
	.bus = &platform_bus_type,
	.probe = davinci_mmcsd_probe,
	.remove = davinci_mmcsd_remove,
	.suspend = davinci_mmcsd_suspend,
	.resume = davinci_mmcsd_resume,
};

static int davinci_mmcsd_init(void)
{
	return driver_register(&davinci_mmcsd_driver);
}

static void __exit davinci_mmcsd_exit(void)
{
	driver_unregister(&davinci_mmcsd_driver);
}

module_init(davinci_mmcsd_init);
module_exit(davinci_mmcsd_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MMCSD driver for Davinci MMC controller");
