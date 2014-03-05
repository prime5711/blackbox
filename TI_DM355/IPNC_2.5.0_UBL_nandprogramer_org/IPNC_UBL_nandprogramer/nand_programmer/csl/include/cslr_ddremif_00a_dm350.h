#ifndef _CSLR_DDR_1_H_
#define _CSLR_DDR_1_H_
/*********************************************************************
 * Copyright (C) 2003-2005 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_ddr_1.h
 * 
 * \brief This file contains the Register Desciptions for DDR
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 EMIDREV;
    volatile Uint32 SDRSTAT;
    volatile Uint32 SDCR1;
    volatile Uint32 SDRCR;
    volatile Uint32 SDTIMR1;
    volatile Uint32 SDTIMR2;
    volatile Uint8 RSVD0[4];
    volatile Uint32 SDCR2;
    volatile Uint32 VBPR;
    volatile Uint8 RSVD1[4];
    volatile Uint32 VBCFG1;
    volatile Uint32 VBCFG2;
    volatile Uint8 RSVD2[16];
    volatile Uint32 PERFC1R;
    volatile Uint32 PERFC2R;
    volatile Uint32 PCCR;
    volatile Uint32 PCMRSR;
    volatile Uint8 RSVD3[112];
    volatile Uint32 EIRR;
    volatile Uint32 EIMR;
    volatile Uint32 EIMSR;
    volatile Uint32 EIMCR;
    volatile Uint8 RSVD4[16];
    volatile Uint32 DDRPHYREV;
    volatile Uint32 DDRPHYCR1;
    volatile Uint32 DDRPHYCR2;
    volatile Uint32 DDRPHYCR3;
} CSL_DdrRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* EMIDREV */

#define CSL_DDR_EMIDREV_MID_MASK         (0x3FFF0000u)
#define CSL_DDR_EMIDREV_MID_SHIFT        (0x00000010u)
#define CSL_DDR_EMIDREV_MID_RESETVAL     (0x00000031u)

#define CSL_DDR_EMIDREV_MAJREV_MASK      (0x0000FF00u)
#define CSL_DDR_EMIDREV_MAJREV_SHIFT     (0x00000008u)
#define CSL_DDR_EMIDREV_MAJREV_RESETVAL  (0x00000003u)

#define CSL_DDR_EMIDREV_MINREV_MASK      (0x000000FFu)
#define CSL_DDR_EMIDREV_MINREV_SHIFT     (0x00000000u)
#define CSL_DDR_EMIDREV_MINREV_RESETVAL  (0x00000019u)

#define CSL_DDR_EMIDREV_RESETVAL         (0x00310319u)

/* SDRSTAT */

#define CSL_DDR_SDRSTAT_BE_MASK          (0x80000000u)
#define CSL_DDR_SDRSTAT_BE_SHIFT         (0x0000001Fu)
#define CSL_DDR_SDRSTAT_BE_RESETVAL      (0x00000000u)

#define CSL_DDR_SDRSTAT_DCLKMD_MASK      (0x40000000u)
#define CSL_DDR_SDRSTAT_DCLKMD_SHIFT     (0x0000001Eu)
#define CSL_DDR_SDRSTAT_DCLKMD_RESETVAL  (0x00000001u)

#define CSL_DDR_SDRSTAT_FASTINIT_MASK    (0x20000000u)
#define CSL_DDR_SDRSTAT_FASTINIT_SHIFT   (0x0000001Du)
#define CSL_DDR_SDRSTAT_FASTINIT_RESETVAL (0x00000000u)

#define CSL_DDR_SDRSTAT_PHYRDY_MASK      (0x00000004u)
#define CSL_DDR_SDRSTAT_PHYRDY_SHIFT     (0x00000002u)
#define CSL_DDR_SDRSTAT_PHYRDY_RESETVAL  (0x00000000u)

#define CSL_DDR_SDRSTAT_RESETVAL         (0x40000000u)

/* SDCR1 */

#define CSL_DDR_SDCR1_DDR2TERM1_MASK     (0x08000000u)
#define CSL_DDR_SDCR1_DDR2TERM1_SHIFT    (0x0000001Bu)
#define CSL_DDR_SDCR1_DDR2TERM1_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_IBANKPOS_MASK      (0x04000000u)
#define CSL_DDR_SDCR1_IBANKPOS_SHIFT     (0x0000001Au)
#define CSL_DDR_SDCR1_IBANKPOS_RESETVAL  (0x00000000u)

#define CSL_DDR_SDCR1_MSDRAMEN_MASK      (0x02000000u)
#define CSL_DDR_SDCR1_MSDRAMEN_SHIFT     (0x00000019u)
#define CSL_DDR_SDCR1_MSDRAMEN_RESETVAL  (0x00000000u)

#define CSL_DDR_SDCR1_DDRDRIVE1_MASK     (0x01000000u)
#define CSL_DDR_SDCR1_DDRDRIVE1_SHIFT    (0x00000018u)
#define CSL_DDR_SDCR1_DDRDRIVE1_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_BOOTUNLOCK_MASK    (0x00800000u)
#define CSL_DDR_SDCR1_BOOTUNLOCK_SHIFT   (0x00000017u)
#define CSL_DDR_SDCR1_BOOTUNLOCK_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_DDR2DDQS_MASK      (0x00400000u)
#define CSL_DDR_SDCR1_DDR2DDQS_SHIFT     (0x00000016u)
#define CSL_DDR_SDCR1_DDR2DDQS_RESETVAL  (0x00000000u)

#define CSL_DDR_SDCR1_DDR2TERM0_MASK     (0x00200000u)
#define CSL_DDR_SDCR1_DDR2TERM0_SHIFT    (0x00000015u)
#define CSL_DDR_SDCR1_DDR2TERM0_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_DDR2_EN_MASK       (0x00100000u)
#define CSL_DDR_SDCR1_DDR2_EN_SHIFT      (0x00000014u)
#define CSL_DDR_SDCR1_DDR2_EN_RESETVAL   (0x00000000u)

#define CSL_DDR_SDCR1_DISDDRDLL_MASK     (0x00080000u)
#define CSL_DDR_SDCR1_DISDDRDLL_SHIFT    (0x00000013u)
#define CSL_DDR_SDCR1_DISDDRDLL_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_DDRDRIVE0_MASK     (0x00040000u)
#define CSL_DDR_SDCR1_DDRDRIVE0_SHIFT    (0x00000012u)
#define CSL_DDR_SDCR1_DDRDRIVE0_RESETVAL (0x00000000u)

#define CSL_DDR_SDCR1_DDR_EN_MASK        (0x00020000u)
#define CSL_DDR_SDCR1_DDR_EN_SHIFT       (0x00000011u)
#define CSL_DDR_SDCR1_DDR_EN_RESETVAL    (0x00000000u)

#define CSL_DDR_SDCR1_SDR_EN_MASK        (0x00010000u)
#define CSL_DDR_SDCR1_SDR_EN_SHIFT       (0x00000010u)
#define CSL_DDR_SDCR1_SDR_EN_RESETVAL    (0x00000000u)

#define CSL_DDR_SDCR1_TIMUNLOCK_MASK     (0x00008000u)
#define CSL_DDR_SDCR1_TIMUNLOCK_SHIFT    (0x0000000Fu)
#define CSL_DDR_SDCR1_TIMUNLOCK_RESETVAL (0x00000000u)

/*----TIMUNLOCK Tokens----*/
#define CSL_DDR_SDCR1_TIMUNLOCK_UNLOCKTIM (0x00000001u)

#define CSL_DDR_SDCR1_NM_MASK            (0x00004000u)
#define CSL_DDR_SDCR1_NM_SHIFT           (0x0000000Eu)
#define CSL_DDR_SDCR1_NM_RESETVAL        (0x00000000u)

#define CSL_DDR_SDCR1_CL_MASK            (0x00000E00u)
#define CSL_DDR_SDCR1_CL_SHIFT           (0x00000009u)
#define CSL_DDR_SDCR1_CL_RESETVAL        (0x00000000u)

/*----CL Tokens----*/
#define CSL_DDR_SDCR1_CL_CASLAT2         (0x00000002u)
#define CSL_DDR_SDCR1_CL_CASLAT3         (0x00000003u)
#define CSL_DDR_SDCR1_CL_CASLAT4         (0x00000004u)
#define CSL_DDR_SDCR1_CL_CASLAT5         (0x00000005u)

#define CSL_DDR_SDCR1_IBANK_MASK         (0x00000070u)
#define CSL_DDR_SDCR1_IBANK_SHIFT        (0x00000004u)
#define CSL_DDR_SDCR1_IBANK_RESETVAL     (0x00000002u)

/*----IBANK Tokens----*/
#define CSL_DDR_SDCR1_IBANK_SD1BANK      (0x00000000u)
#define CSL_DDR_SDCR1_IBANK_SD2BANK      (0x00000001u)
#define CSL_DDR_SDCR1_IBANK_SD4BANK      (0x00000002u)
#define CSL_DDR_SDCR1_IBANK_SD8BANK      (0x00000003u)

#define CSL_DDR_SDCR1_EBANK_MASK         (0x00000008u)
#define CSL_DDR_SDCR1_EBANK_SHIFT        (0x00000003u)
#define CSL_DDR_SDCR1_EBANK_RESETVAL     (0x00000000u)

/*----EBANK Tokens----*/
#define CSL_DDR_SDCR1_EBANK_CS0          (0x00000000u)
#define CSL_DDR_SDCR1_EBANK_CS0_CS1      (0x00000001u)

#define CSL_DDR_SDCR1_PAGESIZE_MASK      (0x00000007u)
#define CSL_DDR_SDCR1_PAGESIZE_SHIFT     (0x00000000u)
#define CSL_DDR_SDCR1_PAGESIZE_RESETVAL  (0x00000000u)

/*----PAGESIZE Tokens----*/
#define CSL_DDR_SDCR1_PAGESIZE_P256      (0x00000000u)
#define CSL_DDR_SDCR1_PAGESIZE_P512      (0x00000001u)
#define CSL_DDR_SDCR1_PAGESIZE_P1024     (0x00000002u)
#define CSL_DDR_SDCR1_PAGESIZE_P2048     (0x00000003u)

#define CSL_DDR_SDCR1_RESETVAL           (0x00000020u)

/* SDRCR */

#define CSL_DDR_SDRCR_LPMODEN_MASK       (0x80000000u)
#define CSL_DDR_SDRCR_LPMODEN_SHIFT      (0x0000001Fu)
#define CSL_DDR_SDRCR_LPMODEN_RESETVAL   (0x00000000u)

#define CSL_DDR_SDRCR_MCLK_STOP_MASK     (0x40000000u)
#define CSL_DDR_SDRCR_MCLK_STOP_SHIFT    (0x0000001Eu)
#define CSL_DDR_SDRCR_MCLK_STOP_RESETVAL (0x00000000u)

#define CSL_DDR_SDRCR_SRPD_MASK          (0x00800000u)
#define CSL_DDR_SDRCR_SRPD_SHIFT         (0x00000017u)
#define CSL_DDR_SDRCR_SRPD_RESETVAL      (0x00000000u)

#define CSL_DDR_SDRCR_RR_MASK            (0x0000FFFFu)
#define CSL_DDR_SDRCR_RR_SHIFT           (0x00000000u)
#define CSL_DDR_SDRCR_RR_RESETVAL        (0x00000000u)

#define CSL_DDR_SDRCR_RESETVAL           (0x00000000u)

/* SDTIMR1 */

#define CSL_DDR_SDTIMR1_T_RFC_MASK       (0xFE000000u)
#define CSL_DDR_SDTIMR1_T_RFC_SHIFT      (0x00000019u)
#define CSL_DDR_SDTIMR1_T_RFC_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR1_T_RP_MASK        (0x01C00000u)
#define CSL_DDR_SDTIMR1_T_RP_SHIFT       (0x00000016u)
#define CSL_DDR_SDTIMR1_T_RP_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR1_T_RCD_MASK       (0x00380000u)
#define CSL_DDR_SDTIMR1_T_RCD_SHIFT      (0x00000013u)
#define CSL_DDR_SDTIMR1_T_RCD_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR1_T_WR_MASK        (0x00070000u)
#define CSL_DDR_SDTIMR1_T_WR_SHIFT       (0x00000010u)
#define CSL_DDR_SDTIMR1_T_WR_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR1_T_RAS_MASK       (0x0000F800u)
#define CSL_DDR_SDTIMR1_T_RAS_SHIFT      (0x0000000Bu)
#define CSL_DDR_SDTIMR1_T_RAS_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR1_T_RC_MASK        (0x000007C0u)
#define CSL_DDR_SDTIMR1_T_RC_SHIFT       (0x00000006u)
#define CSL_DDR_SDTIMR1_T_RC_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR1_T_RRD_MASK       (0x00000038u)
#define CSL_DDR_SDTIMR1_T_RRD_SHIFT      (0x00000003u)
#define CSL_DDR_SDTIMR1_T_RRD_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR1_T_WTR_MASK       (0x00000003u)
#define CSL_DDR_SDTIMR1_T_WTR_SHIFT      (0x00000000u)
#define CSL_DDR_SDTIMR1_T_WTR_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR1_RESETVAL         (0x00000000u)

/* SDTIMR2 */

#define CSL_DDR_SDTIMR2_TRASMAX_MASK     (0x78000000u)
#define CSL_DDR_SDTIMR2_TRASMAX_SHIFT    (0x0000001Bu)
#define CSL_DDR_SDTIMR2_TRASMAX_RESETVAL (0x00000000u)

#define CSL_DDR_SDTIMR2_TXP_MASK         (0x06000000u)
#define CSL_DDR_SDTIMR2_TXP_SHIFT        (0x00000019u)
#define CSL_DDR_SDTIMR2_TXP_RESETVAL     (0x00000000u)

#define CSL_DDR_SDTIMR2_T_ODT_MASK       (0x01800000u)
#define CSL_DDR_SDTIMR2_T_ODT_SHIFT      (0x00000017u)
#define CSL_DDR_SDTIMR2_T_ODT_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_T_XSNR_MASK      (0x007F0000u)
#define CSL_DDR_SDTIMR2_T_XSNR_SHIFT     (0x00000010u)
#define CSL_DDR_SDTIMR2_T_XSNR_RESETVAL  (0x00000000u)

#define CSL_DDR_SDTIMR2_T_XSRD_MASK      (0x0000FF00u)
#define CSL_DDR_SDTIMR2_T_XSRD_SHIFT     (0x00000008u)
#define CSL_DDR_SDTIMR2_T_XSRD_RESETVAL  (0x00000000u)

#define CSL_DDR_SDTIMR2_T_RTP_MASK       (0x000000E0u)
#define CSL_DDR_SDTIMR2_T_RTP_SHIFT      (0x00000005u)
#define CSL_DDR_SDTIMR2_T_RTP_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_T_CKE_MASK       (0x0000001Fu)
#define CSL_DDR_SDTIMR2_T_CKE_SHIFT      (0x00000000u)
#define CSL_DDR_SDTIMR2_T_CKE_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_RESETVAL         (0x00000000u)

/* SDCR2 */

#define CSL_DDR_SDCR2_PASR_MASK          (0x00070000u)
#define CSL_DDR_SDCR2_PASR_SHIFT         (0x00000010u)
#define CSL_DDR_SDCR2_PASR_RESETVAL      (0x00000000u)

#define CSL_DDR_SDCR2_ROWSIZE_MASK       (0x00000007u)
#define CSL_DDR_SDCR2_ROWSIZE_SHIFT      (0x00000000u)
#define CSL_DDR_SDCR2_ROWSIZE_RESETVAL   (0x00000000u)

#define CSL_DDR_SDCR2_RESETVAL           (0x00000000u)

/* VBPR */

#define CSL_DDR_VBPR_PROC_MASK           (0x000000FFu)
#define CSL_DDR_VBPR_PROC_SHIFT          (0x00000000u)
#define CSL_DDR_VBPR_PROC_RESETVAL       (0x000000FFu)

#define CSL_DDR_VBPR_RESETVAL            (0x000000FFu)

/* VBCFG1 */

#define CSL_DDR_VBCFG1_BUS_MASK          (0xC0000000u)
#define CSL_DDR_VBCFG1_BUS_SHIFT         (0x0000001Eu)
#define CSL_DDR_VBCFG1_BUS_RESETVAL      (0x00000001u)

#define CSL_DDR_VBCFG1_STATFIFO_MASK     (0x00FF0000u)
#define CSL_DDR_VBCFG1_STATFIFO_SHIFT    (0x00000010u)
#define CSL_DDR_VBCFG1_STATFIFO_RESETVAL (0x00000007u)

#define CSL_DDR_VBCFG1_WRFIFO_MASK       (0x0000FF00u)
#define CSL_DDR_VBCFG1_WRFIFO_SHIFT      (0x00000008u)
#define CSL_DDR_VBCFG1_WRFIFO_RESETVAL   (0x0000000Bu)

#define CSL_DDR_VBCFG1_CMDFIFO_MASK      (0x000000FFu)
#define CSL_DDR_VBCFG1_CMDFIFO_SHIFT     (0x00000000u)
#define CSL_DDR_VBCFG1_CMDFIFO_RESETVAL  (0x00000007u)

#define CSL_DDR_VBCFG1_RESETVAL          (0x40070B07u)

/* VBCFG2 */

#define CSL_DDR_VBCFG2_RREGFIFO_MASK     (0x00FF0000u)
#define CSL_DDR_VBCFG2_RREGFIFO_SHIFT    (0x00000010u)
#define CSL_DDR_VBCFG2_RREGFIFO_RESETVAL (0x00000002u)

#define CSL_DDR_VBCFG2_RDFIFO_MASK       (0x0000FF00u)
#define CSL_DDR_VBCFG2_RDFIFO_SHIFT      (0x00000008u)
#define CSL_DDR_VBCFG2_RDFIFO_RESETVAL   (0x00000011u)

#define CSL_DDR_VBCFG2_RDCMDFIFO_MASK    (0x000000FFu)
#define CSL_DDR_VBCFG2_RDCMDFIFO_SHIFT   (0x00000000u)
#define CSL_DDR_VBCFG2_RDCMDFIFO_RESETVAL (0x00000016u)

#define CSL_DDR_VBCFG2_RESETVAL          (0x00021116u)

/* PERFC1R */

#define CSL_DDR_PERFC1R_COUNTER1_MASK    (0xFFFFFFFFu)
#define CSL_DDR_PERFC1R_COUNTER1_SHIFT   (0x00000000u)
#define CSL_DDR_PERFC1R_COUNTER1_RESETVAL (0x00000000u)

#define CSL_DDR_PERFC1R_RESETVAL         (0x00000000u)

/* PERFC2R */

#define CSL_DDR_PERFC2R_COUNTER2_MASK    (0xFFFFFFFFu)
#define CSL_DDR_PERFC2R_COUNTER2_SHIFT   (0x00000000u)
#define CSL_DDR_PERFC2R_COUNTER2_RESETVAL (0x00000000u)

#define CSL_DDR_PERFC2R_RESETVAL         (0x00000000u)

/* PCCR */

#define CSL_DDR_PCCR_CNT2MSTEN_MASK      (0x80000000u)
#define CSL_DDR_PCCR_CNT2MSTEN_SHIFT     (0x0000001Fu)
#define CSL_DDR_PCCR_CNT2MSTEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT2REGEN_MASK      (0x40000000u)
#define CSL_DDR_PCCR_CNT2REGEN_SHIFT     (0x0000001Eu)
#define CSL_DDR_PCCR_CNT2REGEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT2CFG_MASK        (0x000F0000u)
#define CSL_DDR_PCCR_CNT2CFG_SHIFT       (0x00000010u)
#define CSL_DDR_PCCR_CNT2CFG_RESETVAL    (0x00000001u)

/*----CNT2CFG Tokens----*/
#define CSL_DDR_PCCR_CNT2CFG_SDACC       (0x00000000u)
#define CSL_DDR_PCCR_CNT2CFG_SDACT       (0x00000001u)
#define CSL_DDR_PCCR_CNT2CFG_TOTRD       (0x00000002u)
#define CSL_DDR_PCCR_CNT2CFG_TOTWR       (0x00000003u)
#define CSL_DDR_PCCR_CNT2CFG_COMFULL     (0x00000004u)
#define CSL_DDR_PCCR_CNT2CFG_WRDATFULL   (0x00000005u)
#define CSL_DDR_PCCR_CNT2CFG_RDDATFULL   (0x00000006u)
#define CSL_DDR_PCCR_CNT2CFG_WRSTATFULL  (0x00000007u)
#define CSL_DDR_PCCR_CNT2CFG_PRIELEV     (0x00000008u)
#define CSL_DDR_PCCR_CNT2CFG_CMDPND      (0x00000009u)

#define CSL_DDR_PCCR_CNT1MSTEN_MASK      (0x00008000u)
#define CSL_DDR_PCCR_CNT1MSTEN_SHIFT     (0x0000000Fu)
#define CSL_DDR_PCCR_CNT1MSTEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT1REGEN_MASK      (0x00004000u)
#define CSL_DDR_PCCR_CNT1REGEN_SHIFT     (0x0000000Eu)
#define CSL_DDR_PCCR_CNT1REGEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT1CFG_MASK        (0x0000000Fu)
#define CSL_DDR_PCCR_CNT1CFG_SHIFT       (0x00000000u)
#define CSL_DDR_PCCR_CNT1CFG_RESETVAL    (0x00000000u)

/*----CNT1CFG Tokens----*/
#define CSL_DDR_PCCR_CNT1CFG_SDACC       (0x00000000u)
#define CSL_DDR_PCCR_CNT1CFG_SDACT       (0x00000001u)
#define CSL_DDR_PCCR_CNT1CFG_TOTRD       (0x00000002u)
#define CSL_DDR_PCCR_CNT1CFG_TOTWR       (0x00000003u)
#define CSL_DDR_PCCR_CNT1CFG_COMFULL     (0x00000004u)
#define CSL_DDR_PCCR_CNT1CFG_WRDATFULL   (0x00000005u)
#define CSL_DDR_PCCR_CNT1CFG_RDDATFULL   (0x00000006u)
#define CSL_DDR_PCCR_CNT1CFG_WRSTATFULL  (0x00000007u)
#define CSL_DDR_PCCR_CNT1CFG_PRIELEV     (0x00000008u)
#define CSL_DDR_PCCR_CNT1CFG_CMDPND      (0x00000009u)

#define CSL_DDR_PCCR_RESETVAL            (0x00010000u)

/* PCMRSR */

#define CSL_DDR_PCMRSR_MSTID2_MASK       (0xFF000000u)
#define CSL_DDR_PCMRSR_MSTID2_SHIFT      (0x00000018u)
#define CSL_DDR_PCMRSR_MSTID2_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_REGID2_MASK       (0x000F0000u)
#define CSL_DDR_PCMRSR_REGID2_SHIFT      (0x00000010u)
#define CSL_DDR_PCMRSR_REGID2_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_MSTID1_MASK       (0x0000FF00u)
#define CSL_DDR_PCMRSR_MSTID1_SHIFT      (0x00000008u)
#define CSL_DDR_PCMRSR_MSTID1_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_REGID1_MASK       (0x0000000Fu)
#define CSL_DDR_PCMRSR_REGID1_SHIFT      (0x00000000u)
#define CSL_DDR_PCMRSR_REGID1_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_RESETVAL          (0x00000000u)

/* EIRR */

#define CSL_DDR_EIRR_LT_MASK             (0x00000004u)
#define CSL_DDR_EIRR_LT_SHIFT            (0x00000002u)
#define CSL_DDR_EIRR_LT_RESETVAL         (0x00000000u)

#define CSL_DDR_EIRR_RESETVAL            (0x00000000u)

/* EIMR */

#define CSL_DDR_EIMR_LTM_MASK            (0x00000004u)
#define CSL_DDR_EIMR_LTM_SHIFT           (0x00000002u)
#define CSL_DDR_EIMR_LTM_RESETVAL        (0x00000000u)

#define CSL_DDR_EIMR_RESETVAL            (0x00000000u)

/* EIMSR */

#define CSL_DDR_EIMSR_LTMSET_MASK        (0x00000004u)
#define CSL_DDR_EIMSR_LTMSET_SHIFT       (0x00000002u)
#define CSL_DDR_EIMSR_LTMSET_RESETVAL    (0x00000000u)

#define CSL_DDR_EIMSR_RESETVAL           (0x00000000u)

/* EIMCR */

#define CSL_DDR_EIMCR_LTMCLR_MASK        (0x00000004u)
#define CSL_DDR_EIMCR_LTMCLR_SHIFT       (0x00000002u)
#define CSL_DDR_EIMCR_LTMCLR_RESETVAL    (0x00000000u)

#define CSL_DDR_EIMCR_RESETVAL           (0x00000000u)

/* DDRPHYREV */

#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYREV_RESETVAL       (0x00000000u)

/* DDRPHYCR1 */

#define CSL_DDR_DDRPHYCR1_DDRPHYCTRL_MASK (0xFFFFFFF8u)
#define CSL_DDR_DDRPHYCR1_DDRPHYCTRL_SHIFT (0x00000003u)
#define CSL_DDR_DDRPHYCR1_DDRPHYCTRL_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYCR1_RDLATENCY_MASK (0x00000007u)
#define CSL_DDR_DDRPHYCR1_RDLATENCY_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYCR1_RDLATENCY_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYCR1_RESETVAL       (0x00000000u)

/* DDRPHYCR2 */

#define CSL_DDR_DDRPHYCR2_DDRPHYCTRL_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYCR2_DDRPHYCTRL_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYCR2_DDRPHYCTRL_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYCR2_RESETVAL       (0x00000000u)

/* DDRPHYCR3 */

#define CSL_DDR_DDRPHYCR3_DDRPHYCTRL_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYCR3_DDRPHYCTRL_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYCR3_DDRPHYCTRL_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYCR3_RESETVAL       (0x00000000u)

#endif
