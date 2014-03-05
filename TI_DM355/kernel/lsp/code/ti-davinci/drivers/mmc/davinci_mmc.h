/*
 *  linux/drivers/mmc/davinci.h
 *
 *  BRIEF MODULE DESCRIPTION
 *      DAVINCI MMC register and other definitions
 *
 *  Copyright (C) 2006 Texas Instruments.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 Modifications:
 ver. 1.0: Oct 2005, Purushotam Kumar   Initial version
 ver  1.2: Jan  2006, Purushotam Kumar   Added hot card remove insert support

 *
 */

#ifndef DAVINCI_MMC_H_
#define DAVINCI_MMC_H_

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/

typedef struct {
	unsigned short mmc_ctl;
	unsigned char rsvd0[2];
	unsigned short mmc_clk;
	unsigned char rsvd1[2];
	unsigned short mmc_st0;
	unsigned char rsvd2[2];
	unsigned short mmc_st1;
	unsigned char rsvd3[2];
	unsigned short mmc_im;
	unsigned char rsvd4[2];
	unsigned short mmc_tor;
	unsigned char rsvd5[2];
	unsigned short mmc_tod;
	unsigned char rsvd6[2];
	unsigned short mmc_blen;
	unsigned char rsvd7[2];
	unsigned short mmc_nblk;
	unsigned char rsvd8[2];
	unsigned short mmc_nblc;
	unsigned char rsvd9[2];
	unsigned int mmc_drr;
	unsigned int mmc_dxr;
	unsigned int mmc_cmd;
	unsigned int mmc_arghl;
	unsigned int mmc_rsp01;
	unsigned int mmc_rsp23;
	unsigned int mmc_rsp45;
	unsigned int mmc_rsp67;
	unsigned short mmc_drsp;
	unsigned char rsvd10[2];
	unsigned short mmc_etok;
	unsigned char rsvd11[2];
	unsigned short mmc_cidx;
	unsigned char rsvd12[2];
	unsigned short mmc_ckc;
	unsigned char rsvd13[2];
	unsigned short mmc_torc;
	unsigned char rsvd14[2];
	unsigned short mmc_todc;
	unsigned char rsvd15[2];
	unsigned short mmc_blnc;
	unsigned char rsvd16[2];
	unsigned short sdio_ctl;
	unsigned char rsvd17[2];
	unsigned short sdio_st0;
	unsigned char rsvd18[2];
	unsigned short sdio_en;
	unsigned char rsvd19[2];
	unsigned short sdio_st;
	unsigned char rsvd20[2];
	unsigned short mmc_fifo_ctl;
} mmcsd_regs_base;

#define mmcsd_regs(host) ((host)->base)

/*
 * Command types
 */
#define DAVINCI_MMC_CMDTYPE_BC	0
#define DAVINCI_MMC_CMDTYPE_BCR	1
#define DAVINCI_MMC_CMDTYPE_AC	2
#define DAVINCI_MMC_CMDTYPE_ADTC	3
#define EDMA_MAX_LOGICAL_CHA_ALLOWED 1

typedef struct {
	unsigned char cnt_chanel;
	unsigned int chanel_num[EDMA_MAX_LOGICAL_CHA_ALLOWED];
} edma_ch_mmcsd;

struct mmc_davinci_host {
	struct resource *reg_res;
	spinlock_t mmc_lock;
	int is_card_busy;
	int is_card_detect_progress;
	int is_card_initialized;
	int is_card_removed;
	int is_init_progress;
	int is_req_queued_up;
	struct mmc_host *que_mmc_host;
	struct mmc_request *que_mmc_request;
	struct mmc_command *cmd;
	struct mmc_data *data;
	struct mmc_host *mmc;
	struct clk *clk;
	volatile mmcsd_regs_base *base;
	unsigned int rw_threshold;
	unsigned int option_read;
	unsigned int option_write;
	int flag_sd_mmc;
	int irq;
	unsigned char bus_mode;

#define DAVINCI_MMC_DATADIR_NONE	0
#define DAVINCI_MMC_DATADIR_READ	1
#define DAVINCI_MMC_DATADIR_WRITE	2
	unsigned char data_dir;
	u32 *buffer;
	u32 bytes_left;

	int use_dma;
	int do_dma;
	unsigned int dma_rx_event;
	unsigned int dma_tx_event;

	struct timer_list timer;
	unsigned int is_core_command;
	unsigned int cmd_code;
	unsigned int old_card_state;
	unsigned int new_card_state;

	unsigned char sd_support;

	edma_ch_mmcsd edma_ch_details;

	unsigned int sg_len;
	int sg_idx;
	unsigned int buffer_bytes_left;
};

typedef enum {
	MMCSD_EVENT_EOFCMD = (1 << 2),
	MMCSD_EVENT_READ = (1 << 10),
	MMCSD_EVENT_WRITE = (1 << 9),
	MMCSD_EVENT_ERROR_CMDCRC = (1 << 7),
	MMCSD_EVENT_ERROR_DATACRC = ((1 << 6) | (1 << 5)),
	MMCSD_EVENT_ERROR_CMDTIMEOUT = (1 << 4),
	MMCSD_EVENT_ERROR_DATATIMEOUT = (1 << 3),
	MMCSD_EVENT_CARD_EXITBUSY = (1 << 1),
	MMCSD_EVENT_BLOCK_XFERRED = (1 << 0)
} mmcsdevent;

#define MMCSD_EVENT_TIMEOUT_ERROR \
 (MMCSD_EVENT_ERROR_DATATIMEOUT | MMCSD_EVENT_ERROR_CMDTIMEOUT )
#define MMCSD_EVENT_CRC_ERROR \
 (MMCSD_EVENT_ERROR_DATACRC | MMCSD_EVENT_ERROR_CMDCRC)
#define MMCSD_EVENT_ERROR \
 (MMCSD_EVENT_TIMEOUT_ERROR | MMCSD_EVENT_CRC_ERROR)

static void init_mmcsd_host(struct mmc_davinci_host *host);

static void davinci_fifo_data_trans(struct mmc_davinci_host *host);

static void mmc_davinci_sg_to_buf(struct mmc_davinci_host *host);

static int mmc_davinci_send_dma_request(struct mmc_davinci_host *host,
					struct mmc_request *req);

static void mmc_davinci_xfer_done(struct mmc_davinci_host *host,
				  struct mmc_data *data);

static int mmc_davinci_get_ro(struct mmc_host *mmc);

static void davinci_abort_dma(struct mmc_davinci_host *host);

#endif				/* DAVINCI_MMC_H_ */
