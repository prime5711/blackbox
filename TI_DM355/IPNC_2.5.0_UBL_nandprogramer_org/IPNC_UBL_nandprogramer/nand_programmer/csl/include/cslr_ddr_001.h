#ifndef _CSLR_DDR_1_H_
#define _CSLR_DDR_1_H_
/*********************************************************************
 * Copyright (C) 2003-2004 Texas Instruments Incorporated. 
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
    volatile Uint32 ERCSR;
    volatile Uint32 SDRSTAT;
    volatile Uint32 SDBCR;
    volatile Uint32 SDRCR;
    volatile Uint32 SDTIMR;
    volatile Uint32 SDTIMR2;
    volatile Uint8 RSVD0[8];
    volatile Uint32 VBPR;
    volatile Uint8 RSVD1[4];
    volatile Uint32 VBCFG1;
    volatile Uint32 VBCFG2;
    volatile Uint8 RSVD2[16];
    volatile Uint32 PERFC1R;
    volatile Uint32 PERFC2R;
    volatile Uint32 PCCR;
    volatile Uint32 PCMRSR;
    volatile Uint8 RSVD3[48];
    volatile Uint32 ASYNCCS2CR;
    volatile Uint32 ASYNCCS3CR;
    volatile Uint32 ASYNCCS4CR;
    volatile Uint32 ASYNCCS5CR;
    volatile Uint8 RSVD4[16];
    volatile Uint32 AWCCR;
    volatile Uint8 RSVD5[28];
    volatile Uint32 IRR;
    volatile Uint32 IMR;
    volatile Uint32 IMSR;
    volatile Uint32 IMCR;
    volatile Uint8 RSVD6[16];
    volatile Uint32 DDRPHYREV;
    volatile Uint32 DDRPHYCR;
    volatile Uint32 DDRPHYSR;
    volatile Uint8 RSVD7[4];
    volatile Uint32 VTPCTRL;
    volatile Uint32 VTPSTAT;
} CSL_DdrRegs;
/* Pratheesh G: 06/28/2004
 * Following lines are added to ovecome the limitation of ASHA in representing
 * registers whose interpretation can change dynamically based on one of the
 * field, here SSEL */
#define SBSCS2CR ASYNCCS2CR
#define SBSCS3CR ASYNCCS3CR
#define SBSCS4CR ASYNCCS4CR
#define SBSCS5CR ASYNCCS5CR
/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* ERCSR */

#define CSL_DDR_ERCSR_MID_MASK           (0x3FFF0000u)
#define CSL_DDR_ERCSR_MID_SHIFT          (0x00000010u)
#define CSL_DDR_ERCSR_MID_RESETVAL       (0x00000031u)

#define CSL_DDR_ERCSR_MAJREV_MASK        (0x0000FF00u)
#define CSL_DDR_ERCSR_MAJREV_SHIFT       (0x00000008u)
#define CSL_DDR_ERCSR_MAJREV_RESETVAL    (0x00000003u)

#define CSL_DDR_ERCSR_MINREV_MASK        (0x000000FFu)
#define CSL_DDR_ERCSR_MINREV_SHIFT       (0x00000000u)
#define CSL_DDR_ERCSR_MINREV_RESETVAL    (0x00000004u)

#define CSL_DDR_ERCSR_RESETVAL           (0x00310304u)

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

/* SDBCR */

#define CSL_DDR_SDBCR_BOOTUNLOCK_MASK    (0x00800000u)
#define CSL_DDR_SDBCR_BOOTUNLOCK_SHIFT   (0x00000017u)
#define CSL_DDR_SDBCR_BOOTUNLOCK_RESETVAL (0x00000000u)

#define CSL_DDR_SDBCR_DDR2TERM_MASK      (0x00200000u)
#define CSL_DDR_SDBCR_DDR2TERM_SHIFT     (0x00000015u)
#define CSL_DDR_SDBCR_DDR2TERM_RESETVAL  (0x00000000u)

/*----DDR2TERM Tokens----*/
#define CSL_DDR_SDBCR_DDR2TERM_TERM75    (0x00000000u)
#define CSL_DDR_SDBCR_DDR2TERM_TERM150   (0x00000001u)

#define CSL_DDR_SDBCR_DDR2_EN_MASK       (0x00100000u)
#define CSL_DDR_SDBCR_DDR2_EN_SHIFT      (0x00000014u)
#define CSL_DDR_SDBCR_DDR2_EN_RESETVAL   (0x00000001u)

#define CSL_DDR_SDBCR_DISDDRDLL_MASK     (0x00080000u)
#define CSL_DDR_SDBCR_DISDDRDLL_SHIFT    (0x00000013u)
#define CSL_DDR_SDBCR_DISDDRDLL_RESETVAL (0x00000000u)

#define CSL_DDR_SDBCR_DDR_DR_MASK        (0x00040000u)
#define CSL_DDR_SDBCR_DDR_DR_SHIFT       (0x00000012u)
#define CSL_DDR_SDBCR_DDR_DR_RESETVAL    (0x00000000u)

/*----DDR_DR Tokens----*/
#define CSL_DDR_SDBCR_DDR_DR_NORM        (0x00000000u)
#define CSL_DDR_SDBCR_DDR_DR_WEAK        (0x00000001u)

#define CSL_DDR_SDBCR_DDR_EN_MASK        (0x00020000u)
#define CSL_DDR_SDBCR_DDR_EN_SHIFT       (0x00000011u)
#define CSL_DDR_SDBCR_DDR_EN_RESETVAL    (0x00000001u)

#define CSL_DDR_SDBCR_SDR_EN_MASK        (0x00010000u)
#define CSL_DDR_SDBCR_SDR_EN_SHIFT       (0x00000010u)
#define CSL_DDR_SDBCR_SDR_EN_RESETVAL    (0x00000001u)

#define CSL_DDR_SDBCR_TIMUNLOCK_MASK     (0x00008000u)
#define CSL_DDR_SDBCR_TIMUNLOCK_SHIFT    (0x0000000Fu)
#define CSL_DDR_SDBCR_TIMUNLOCK_RESETVAL (0x00000000u)

/*----TIMUNLOCK Tokens----*/
#define CSL_DDR_SDBCR_TIMUNLOCK_UNLOCKTIM (0x00000001u)

#define CSL_DDR_SDBCR_NM_MASK            (0x00004000u)
#define CSL_DDR_SDBCR_NM_SHIFT           (0x0000000Eu)
#define CSL_DDR_SDBCR_NM_RESETVAL        (0x00000000u)

#define CSL_DDR_SDBCR_CL_MASK            (0x00000E00u)
#define CSL_DDR_SDBCR_CL_SHIFT           (0x00000009u)
#define CSL_DDR_SDBCR_CL_RESETVAL        (0x00000005u)

/*----CL Tokens----*/
#define CSL_DDR_SDBCR_CL_CASLAT2         (0x00000002u)
#define CSL_DDR_SDBCR_CL_CASLAT3         (0x00000003u)
#define CSL_DDR_SDBCR_CL_CASLAT4         (0x00000004u)
#define CSL_DDR_SDBCR_CL_CASLAT5         (0x00000005u)

#define CSL_DDR_SDBCR_IBANK_MASK         (0x00000070u)
#define CSL_DDR_SDBCR_IBANK_SHIFT        (0x00000004u)
#define CSL_DDR_SDBCR_IBANK_RESETVAL     (0x00000002u)

/*----IBANK Tokens----*/
#define CSL_DDR_SDBCR_IBANK_SD1BANK      (0x00000000u)
#define CSL_DDR_SDBCR_IBANK_SD2BANK      (0x00000001u)
#define CSL_DDR_SDBCR_IBANK_SD4BANK      (0x00000002u)
#define CSL_DDR_SDBCR_IBANK_SD8BANK      (0x00000003u)

#define CSL_DDR_SDBCR_EBANK_MASK         (0x00000008u)
#define CSL_DDR_SDBCR_EBANK_SHIFT        (0x00000003u)
#define CSL_DDR_SDBCR_EBANK_RESETVAL     (0x00000000u)

/*----EBANK Tokens----*/
#define CSL_DDR_SDBCR_EBANK_CS0          (0x00000000u)
#define CSL_DDR_SDBCR_EBANK_CS0_CS1      (0x00000001u)

#define CSL_DDR_SDBCR_PAGESIZE_MASK      (0x00000007u)
#define CSL_DDR_SDBCR_PAGESIZE_SHIFT     (0x00000000u)
#define CSL_DDR_SDBCR_PAGESIZE_RESETVAL  (0x00000000u)

/*----PAGESIZE Tokens----*/
#define CSL_DDR_SDBCR_PAGESIZE_P256      (0x00000000u)
#define CSL_DDR_SDBCR_PAGESIZE_P512      (0x00000001u)
#define CSL_DDR_SDBCR_PAGESIZE_P1024     (0x00000002u)
#define CSL_DDR_SDBCR_PAGESIZE_P2048     (0x00000003u)

#define CSL_DDR_SDBCR_RESETVAL           (0x00130A20u)

/* SDRCR */

#define CSL_DDR_SDRCR_SR_MASK            (0x80000000u)
#define CSL_DDR_SDRCR_SR_SHIFT           (0x0000001Fu)
#define CSL_DDR_SDRCR_SR_RESETVAL        (0x00000000u)

/*----SR Tokens----*/
#define CSL_DDR_SDRCR_SR_SELFREF         (0x00000001u)

#define CSL_DDR_SDRCR_PEN_MASK           (0x40000000u)
#define CSL_DDR_SDRCR_PEN_SHIFT          (0x0000001Eu)
#define CSL_DDR_SDRCR_PEN_RESETVAL       (0x00000000u)

/*----PEN Tokens----*/
#define CSL_DDR_SDRCR_PEN_DISABLE        (0x00000000u)
#define CSL_DDR_SDRCR_PEN_ENABLE         (0x00000001u)

#define CSL_DDR_SDRCR_RR_MASK            (0x0000FFFFu)
#define CSL_DDR_SDRCR_RR_SHIFT           (0x00000000u)
#define CSL_DDR_SDRCR_RR_RESETVAL        (0x00000300u)

#define CSL_DDR_SDRCR_RESETVAL           (0x00000300u)

/* SDTIMR */

#define CSL_DDR_SDTIMR_T_RFC_MASK        (0xF8000000u)
#define CSL_DDR_SDTIMR_T_RFC_SHIFT       (0x0000001Bu)
#define CSL_DDR_SDTIMR_T_RFC_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR_T_RP_MASK         (0x07000000u)
#define CSL_DDR_SDTIMR_T_RP_SHIFT        (0x00000018u)
#define CSL_DDR_SDTIMR_T_RP_RESETVAL     (0x00000000u)

#define CSL_DDR_SDTIMR_T_RCD_MASK        (0x00700000u)
#define CSL_DDR_SDTIMR_T_RCD_SHIFT       (0x00000014u)
#define CSL_DDR_SDTIMR_T_RCD_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR_T_WR_MASK         (0x00070000u)
#define CSL_DDR_SDTIMR_T_WR_SHIFT        (0x00000010u)
#define CSL_DDR_SDTIMR_T_WR_RESETVAL     (0x00000000u)

#define CSL_DDR_SDTIMR_T_RAS_MASK        (0x0000F000u)
#define CSL_DDR_SDTIMR_T_RAS_SHIFT       (0x0000000Cu)
#define CSL_DDR_SDTIMR_T_RAS_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR_T_RC_MASK         (0x00000F00u)
#define CSL_DDR_SDTIMR_T_RC_SHIFT        (0x00000008u)
#define CSL_DDR_SDTIMR_T_RC_RESETVAL     (0x00000000u)

#define CSL_DDR_SDTIMR_T_RRD_MASK        (0x00000070u)
#define CSL_DDR_SDTIMR_T_RRD_SHIFT       (0x00000004u)
#define CSL_DDR_SDTIMR_T_RRD_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR_T_WTR_MASK        (0x00000003u)
#define CSL_DDR_SDTIMR_T_WTR_SHIFT       (0x00000000u)
#define CSL_DDR_SDTIMR_T_WTR_RESETVAL    (0x00000000u)

#define CSL_DDR_SDTIMR_RESETVAL          (0x00000000u)

/* SDTIMR2 */

#define CSL_DDR_SDTIMR2_T_ODT_MASK       (0x00600000u)
#define CSL_DDR_SDTIMR2_T_ODT_SHIFT      (0x00000015u)
#define CSL_DDR_SDTIMR2_T_ODT_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_T_SXNR_MASK      (0x001F0000u)
#define CSL_DDR_SDTIMR2_T_SXNR_SHIFT     (0x00000010u)
#define CSL_DDR_SDTIMR2_T_SXNR_RESETVAL  (0x00000000u)

#define CSL_DDR_SDTIMR2_T_SXRD_MASK      (0x0000FF00u)
#define CSL_DDR_SDTIMR2_T_SXRD_SHIFT     (0x00000008u)
#define CSL_DDR_SDTIMR2_T_SXRD_RESETVAL  (0x00000000u)

#define CSL_DDR_SDTIMR2_T_RTP_MASK       (0x000000E0u)
#define CSL_DDR_SDTIMR2_T_RTP_SHIFT      (0x00000005u)
#define CSL_DDR_SDTIMR2_T_RTP_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_T_CKE_MASK       (0x0000001Fu)
#define CSL_DDR_SDTIMR2_T_CKE_SHIFT      (0x00000000u)
#define CSL_DDR_SDTIMR2_T_CKE_RESETVAL   (0x00000000u)

#define CSL_DDR_SDTIMR2_RESETVAL         (0x00000000u)

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

#define CSL_DDR_PCCR_CNT2CFG_MASK        (0x3FFF0000u)
#define CSL_DDR_PCCR_CNT2CFG_SHIFT       (0x00000010u)
#define CSL_DDR_PCCR_CNT2CFG_RESETVAL    (0x00000000u)

/*----CNT2CFG Tokens----*/
#define CSL_DDR_PCCR_CNT2CFG_SDACC       (0x00000000u)
#define CSL_DDR_PCCR_CNT2CFG_SDACT       (0x00000001u)
#define CSL_DDR_PCCR_CNT2CFG_TOTRD       (0x00000002u)
#define CSL_DDR_PCCR_CNT2CFG_TOTWR       (0x00000003u)
#define CSL_DDR_PCCR_CNT2CFG_COMFULL     (0x00000004u)
#define CSL_DDR_PCCR_CNT2CFG_WRDATFULL   (0x00000005u)
#define CSL_DDR_PCCR_CNT2CFG_RDDATFULL   (0x00000006u)
#define CSL_DDR_PCCR_CNT2CFG_WRSTATFULL  (0x00000007u)
#define CSL_DDR_PCCR_CNT2CFG_TOTPARERR   (0x00000008u)
#define CSL_DDR_PCCR_CNT2CFG_PRIELEV     (0x00000009u)
#define CSL_DDR_PCCR_CNT2CFG_PCHRGELEV   (0x0000000Au)
#define CSL_DDR_PCCR_CNT2CFG_CMDPND      (0x0000000Bu)

#define CSL_DDR_PCCR_CNT1MSTEN_MASK      (0x00008000u)
#define CSL_DDR_PCCR_CNT1MSTEN_SHIFT     (0x0000000Fu)
#define CSL_DDR_PCCR_CNT1MSTEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT1REGEN_MASK      (0x00004000u)
#define CSL_DDR_PCCR_CNT1REGEN_SHIFT     (0x0000000Eu)
#define CSL_DDR_PCCR_CNT1REGEN_RESETVAL  (0x00000000u)

#define CSL_DDR_PCCR_CNT1CFG_MASK        (0x00003FFFu)
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
#define CSL_DDR_PCCR_CNT1CFG_TOTPARERR   (0x00000008u)
#define CSL_DDR_PCCR_CNT1CFG_PRIELEV     (0x00000009u)
#define CSL_DDR_PCCR_CNT1CFG_PCHRGELEV   (0x0000000Au)
#define CSL_DDR_PCCR_CNT1CFG_CMDPND      (0x0000000Bu)

#define CSL_DDR_PCCR_RESETVAL            (0x00000000u)

/* PCMRSR */

#define CSL_DDR_PCMRSR_MSTID2_MASK       (0x01F80000u)
#define CSL_DDR_PCMRSR_MSTID2_SHIFT      (0x00000013u)
#define CSL_DDR_PCMRSR_MSTID2_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_REGID2_MASK       (0x00070000u)
#define CSL_DDR_PCMRSR_REGID2_SHIFT      (0x00000010u)
#define CSL_DDR_PCMRSR_REGID2_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_MSTID1_MASK       (0x000001F8u)
#define CSL_DDR_PCMRSR_MSTID1_SHIFT      (0x00000003u)
#define CSL_DDR_PCMRSR_MSTID1_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_REGID1_MASK       (0x00000007u)
#define CSL_DDR_PCMRSR_REGID1_SHIFT      (0x00000000u)
#define CSL_DDR_PCMRSR_REGID1_RESETVAL   (0x00000000u)

#define CSL_DDR_PCMRSR_RESETVAL          (0x00000000u)

/* ASYNCCS2CR */

#define CSL_DDR_ASYNCCS2CR_SSEL_MASK     (0x80000000u)
#define CSL_DDR_ASYNCCS2CR_SSEL_SHIFT    (0x0000001Fu)
#define CSL_DDR_ASYNCCS2CR_SSEL_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS2CR_SS_MASK       (0x40000000u)
#define CSL_DDR_ASYNCCS2CR_SS_SHIFT      (0x0000001Eu)
#define CSL_DDR_ASYNCCS2CR_SS_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS2CR_BWEM_MASK     (0x20000000u)
#define CSL_DDR_ASYNCCS2CR_BWEM_SHIFT    (0x0000001Du)
#define CSL_DDR_ASYNCCS2CR_BWEM_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS2CR_EW_MASK       (0x10000000u)
#define CSL_DDR_ASYNCCS2CR_EW_SHIFT      (0x0000001Cu)
#define CSL_DDR_ASYNCCS2CR_EW_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS2CR_WSETUP_MASK   (0x0F000000u)
#define CSL_DDR_ASYNCCS2CR_WSETUP_SHIFT  (0x00000018u)
#define CSL_DDR_ASYNCCS2CR_WSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS2CR_WSTROBE_MASK  (0x00FC0000u)
#define CSL_DDR_ASYNCCS2CR_WSTROBE_SHIFT (0x00000012u)
#define CSL_DDR_ASYNCCS2CR_WSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS2CR_WHOLD_MASK    (0x00038000u)
#define CSL_DDR_ASYNCCS2CR_WHOLD_SHIFT   (0x0000000Fu)
#define CSL_DDR_ASYNCCS2CR_WHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS2CR_RSETUP_MASK   (0x00007800u)
#define CSL_DDR_ASYNCCS2CR_RSETUP_SHIFT  (0x0000000Bu)
#define CSL_DDR_ASYNCCS2CR_RSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS2CR_RSTROBE_MASK  (0x000007E0u)
#define CSL_DDR_ASYNCCS2CR_RSTROBE_SHIFT (0x00000005u)
#define CSL_DDR_ASYNCCS2CR_RSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS2CR_RHOLD_MASK    (0x0000001Cu)
#define CSL_DDR_ASYNCCS2CR_RHOLD_SHIFT   (0x00000002u)
#define CSL_DDR_ASYNCCS2CR_RHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS2CR_ASIZE_MASK    (0x00000003u)
#define CSL_DDR_ASYNCCS2CR_ASIZE_SHIFT   (0x00000000u)
#define CSL_DDR_ASYNCCS2CR_ASIZE_RESETVAL (0x00000000u)

/*----ASIZE Tokens----*/
#define CSL_DDR_ASYNCCS2CR_ASIZE_DATA8   (0x00000000u)
#define CSL_DDR_ASYNCCS2CR_ASIZE_DATA16  (0x00000001u)
#define CSL_DDR_ASYNCCS2CR_ASIZE_DATA32  (0x00000002u)
#define CSL_DDR_ASYNCCS2CR_ASIZE_DATA64  (0x00000003u)

#define CSL_DDR_ASYNCCS2CR_RESETVAL      (0x0FFFFFFCu)

/* SBSCS2CR */

#define CSL_DDR_SBSCS2CR_SSEL_MASK       (0x80000000u)
#define CSL_DDR_SBSCS2CR_SSEL_SHIFT      (0x0000001Fu)
#define CSL_DDR_SBSCS2CR_SSEL_RESETVAL   (0x00000000u)

#define CSL_DDR_SBSCS2CR_CSEXT_MASK      (0x00000200u)
#define CSL_DDR_SBSCS2CR_CSEXT_SHIFT     (0x00000009u)
#define CSL_DDR_SBSCS2CR_CSEXT_RESETVAL  (0x00000000u)

#define CSL_DDR_SBSCS2CR_READEN_MASK     (0x00000100u)
#define CSL_DDR_SBSCS2CR_READEN_SHIFT    (0x00000008u)
#define CSL_DDR_SBSCS2CR_READEN_RESETVAL (0x00000000u)

#define CSL_DDR_SBSCS2CR_WRLAT_MASK      (0x000000C0u)
#define CSL_DDR_SBSCS2CR_WRLAT_SHIFT     (0x00000006u)
#define CSL_DDR_SBSCS2CR_WRLAT_RESETVAL  (0x00000000u)

/*----WRLAT Tokens----*/
#define CSL_DDR_SBSCS2CR_WRLAT_WRLAT0    (0x00000000u)
#define CSL_DDR_SBSCS2CR_WRLAT_WRLAT1    (0x00000001u)
#define CSL_DDR_SBSCS2CR_WRLAT_WRLAT2    (0x00000002u)
#define CSL_DDR_SBSCS2CR_WRLAT_WRLAT3    (0x00000003u)

#define CSL_DDR_SBSCS2CR_RDLAT_MASK      (0x0000000Cu)
#define CSL_DDR_SBSCS2CR_RDLAT_SHIFT     (0x00000002u)
#define CSL_DDR_SBSCS2CR_RDLAT_RESETVAL  (0x00000000u)

/*----RDLAT Tokens----*/
#define CSL_DDR_SBSCS2CR_RDLAT_RDLAT0    (0x00000000u)
#define CSL_DDR_SBSCS2CR_RDLAT_RDLAT1    (0x00000001u)
#define CSL_DDR_SBSCS2CR_RDLAT_RDLAT2    (0x00000002u)
#define CSL_DDR_SBSCS2CR_RDLAT_RDLAT3    (0x00000003u)

#define CSL_DDR_SBSCS2CR_SBSIZE_MASK     (0x00000003u)
#define CSL_DDR_SBSCS2CR_SBSIZE_SHIFT    (0x00000000u)
#define CSL_DDR_SBSCS2CR_SBSIZE_RESETVAL (0x00000000u)

/*----SBSIZE Tokens----*/
#define CSL_DDR_SBSCS2CR_SBSIZE_DATA8    (0x00000000u)
#define CSL_DDR_SBSCS2CR_SBSIZE_DATA16   (0x00000001u)
#define CSL_DDR_SBSCS2CR_SBSIZE_DATA32   (0x00000002u)
#define CSL_DDR_SBSCS2CR_SBSIZE_DATA64   (0x00000003u)

#define CSL_DDR_SBSCS2CR_RESETVAL        (0x00000000u)

/* ASYNCCS3CR */

#define CSL_DDR_ASYNCCS3CR_SSEL_MASK     (0x80000000u)
#define CSL_DDR_ASYNCCS3CR_SSEL_SHIFT    (0x0000001Fu)
#define CSL_DDR_ASYNCCS3CR_SSEL_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS3CR_SS_MASK       (0x40000000u)
#define CSL_DDR_ASYNCCS3CR_SS_SHIFT      (0x0000001Eu)
#define CSL_DDR_ASYNCCS3CR_SS_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS3CR_BWEM_MASK     (0x20000000u)
#define CSL_DDR_ASYNCCS3CR_BWEM_SHIFT    (0x0000001Du)
#define CSL_DDR_ASYNCCS3CR_BWEM_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS3CR_EW_MASK       (0x10000000u)
#define CSL_DDR_ASYNCCS3CR_EW_SHIFT      (0x0000001Cu)
#define CSL_DDR_ASYNCCS3CR_EW_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS3CR_WSETUP_MASK   (0x0F000000u)
#define CSL_DDR_ASYNCCS3CR_WSETUP_SHIFT  (0x00000018u)
#define CSL_DDR_ASYNCCS3CR_WSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS3CR_WSTROBE_MASK  (0x00FC0000u)
#define CSL_DDR_ASYNCCS3CR_WSTROBE_SHIFT (0x00000012u)
#define CSL_DDR_ASYNCCS3CR_WSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS3CR_WHOLD_MASK    (0x00038000u)
#define CSL_DDR_ASYNCCS3CR_WHOLD_SHIFT   (0x0000000Fu)
#define CSL_DDR_ASYNCCS3CR_WHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS3CR_RSETUP_MASK   (0x00007800u)
#define CSL_DDR_ASYNCCS3CR_RSETUP_SHIFT  (0x0000000Bu)
#define CSL_DDR_ASYNCCS3CR_RSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS3CR_RSTROBE_MASK  (0x000007E0u)
#define CSL_DDR_ASYNCCS3CR_RSTROBE_SHIFT (0x00000005u)
#define CSL_DDR_ASYNCCS3CR_RSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS3CR_RHOLD_MASK    (0x0000001Cu)
#define CSL_DDR_ASYNCCS3CR_RHOLD_SHIFT   (0x00000002u)
#define CSL_DDR_ASYNCCS3CR_RHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS3CR_ASIZE_MASK    (0x00000003u)
#define CSL_DDR_ASYNCCS3CR_ASIZE_SHIFT   (0x00000000u)
#define CSL_DDR_ASYNCCS3CR_ASIZE_RESETVAL (0x00000000u)

/*----ASIZE Tokens----*/
#define CSL_DDR_ASYNCCS3CR_ASIZE_DATA8   (0x00000000u)
#define CSL_DDR_ASYNCCS3CR_ASIZE_DATA16  (0x00000001u)
#define CSL_DDR_ASYNCCS3CR_ASIZE_DATA32  (0x00000002u)
#define CSL_DDR_ASYNCCS3CR_ASIZE_DATA64  (0x00000003u)

#define CSL_DDR_ASYNCCS3CR_RESETVAL      (0x0FFFFFFCu)

/* SBSCS3CR */

#define CSL_DDR_SBSCS3CR_SSEL_MASK       (0x80000000u)
#define CSL_DDR_SBSCS3CR_SSEL_SHIFT      (0x0000001Fu)
#define CSL_DDR_SBSCS3CR_SSEL_RESETVAL   (0x00000000u)

#define CSL_DDR_SBSCS3CR_CSEXT_MASK      (0x00000200u)
#define CSL_DDR_SBSCS3CR_CSEXT_SHIFT     (0x00000009u)
#define CSL_DDR_SBSCS3CR_CSEXT_RESETVAL  (0x00000000u)

#define CSL_DDR_SBSCS3CR_READEN_MASK     (0x00000100u)
#define CSL_DDR_SBSCS3CR_READEN_SHIFT    (0x00000008u)
#define CSL_DDR_SBSCS3CR_READEN_RESETVAL (0x00000000u)

#define CSL_DDR_SBSCS3CR_WRLAT_MASK      (0x000000C0u)
#define CSL_DDR_SBSCS3CR_WRLAT_SHIFT     (0x00000006u)
#define CSL_DDR_SBSCS3CR_WRLAT_RESETVAL  (0x00000000u)

/*----WRLAT Tokens----*/
#define CSL_DDR_SBSCS3CR_WRLAT_WRLAT0    (0x00000000u)
#define CSL_DDR_SBSCS3CR_WRLAT_WRLAT1    (0x00000001u)
#define CSL_DDR_SBSCS3CR_WRLAT_WRLAT2    (0x00000002u)
#define CSL_DDR_SBSCS3CR_WRLAT_WRLAT3    (0x00000003u)

#define CSL_DDR_SBSCS3CR_RDLAT_MASK      (0x0000000Cu)
#define CSL_DDR_SBSCS3CR_RDLAT_SHIFT     (0x00000002u)
#define CSL_DDR_SBSCS3CR_RDLAT_RESETVAL  (0x00000000u)

/*----RDLAT Tokens----*/
#define CSL_DDR_SBSCS3CR_RDLAT_RDLAT0    (0x00000000u)
#define CSL_DDR_SBSCS3CR_RDLAT_RDLAT1    (0x00000001u)
#define CSL_DDR_SBSCS3CR_RDLAT_RDLAT2    (0x00000002u)
#define CSL_DDR_SBSCS3CR_RDLAT_RDLAT3    (0x00000003u)

#define CSL_DDR_SBSCS3CR_SBSIZE_MASK     (0x00000003u)
#define CSL_DDR_SBSCS3CR_SBSIZE_SHIFT    (0x00000000u)
#define CSL_DDR_SBSCS3CR_SBSIZE_RESETVAL (0x00000000u)

/*----SBSIZE Tokens----*/
#define CSL_DDR_SBSCS3CR_SBSIZE_DATA8    (0x00000000u)
#define CSL_DDR_SBSCS3CR_SBSIZE_DATA16   (0x00000001u)
#define CSL_DDR_SBSCS3CR_SBSIZE_DATA32   (0x00000002u)
#define CSL_DDR_SBSCS3CR_SBSIZE_DATA64   (0x00000003u)

#define CSL_DDR_SBSCS3CR_RESETVAL        (0x00000000u)

/* ASYNCCS4CR */

#define CSL_DDR_ASYNCCS4CR_SSEL_MASK     (0x80000000u)
#define CSL_DDR_ASYNCCS4CR_SSEL_SHIFT    (0x0000001Fu)
#define CSL_DDR_ASYNCCS4CR_SSEL_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS4CR_SS_MASK       (0x40000000u)
#define CSL_DDR_ASYNCCS4CR_SS_SHIFT      (0x0000001Eu)
#define CSL_DDR_ASYNCCS4CR_SS_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS4CR_BWEM_MASK     (0x20000000u)
#define CSL_DDR_ASYNCCS4CR_BWEM_SHIFT    (0x0000001Du)
#define CSL_DDR_ASYNCCS4CR_BWEM_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS4CR_EW_MASK       (0x10000000u)
#define CSL_DDR_ASYNCCS4CR_EW_SHIFT      (0x0000001Cu)
#define CSL_DDR_ASYNCCS4CR_EW_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS4CR_WSETUP_MASK   (0x0F000000u)
#define CSL_DDR_ASYNCCS4CR_WSETUP_SHIFT  (0x00000018u)
#define CSL_DDR_ASYNCCS4CR_WSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS4CR_WSTROBE_MASK  (0x00FC0000u)
#define CSL_DDR_ASYNCCS4CR_WSTROBE_SHIFT (0x00000012u)
#define CSL_DDR_ASYNCCS4CR_WSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS4CR_WHOLD_MASK    (0x00038000u)
#define CSL_DDR_ASYNCCS4CR_WHOLD_SHIFT   (0x0000000Fu)
#define CSL_DDR_ASYNCCS4CR_WHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS4CR_RSETUP_MASK   (0x00007800u)
#define CSL_DDR_ASYNCCS4CR_RSETUP_SHIFT  (0x0000000Bu)
#define CSL_DDR_ASYNCCS4CR_RSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS4CR_RSTROBE_MASK  (0x000007E0u)
#define CSL_DDR_ASYNCCS4CR_RSTROBE_SHIFT (0x00000005u)
#define CSL_DDR_ASYNCCS4CR_RSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS4CR_RHOLD_MASK    (0x0000001Cu)
#define CSL_DDR_ASYNCCS4CR_RHOLD_SHIFT   (0x00000002u)
#define CSL_DDR_ASYNCCS4CR_RHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS4CR_ASIZE_MASK    (0x00000003u)
#define CSL_DDR_ASYNCCS4CR_ASIZE_SHIFT   (0x00000000u)
#define CSL_DDR_ASYNCCS4CR_ASIZE_RESETVAL (0x00000000u)

/*----ASIZE Tokens----*/
#define CSL_DDR_ASYNCCS4CR_ASIZE_DATA8   (0x00000000u)
#define CSL_DDR_ASYNCCS4CR_ASIZE_DATA16  (0x00000001u)
#define CSL_DDR_ASYNCCS4CR_ASIZE_DATA32  (0x00000002u)
#define CSL_DDR_ASYNCCS4CR_ASIZE_DATA64  (0x00000003u)

#define CSL_DDR_ASYNCCS4CR_RESETVAL      (0x0FFFFFFCu)

/* SBSCS4CR */

#define CSL_DDR_SBSCS4CR_SSEL_MASK       (0x80000000u)
#define CSL_DDR_SBSCS4CR_SSEL_SHIFT      (0x0000001Fu)
#define CSL_DDR_SBSCS4CR_SSEL_RESETVAL   (0x00000000u)

#define CSL_DDR_SBSCS4CR_CSEXT_MASK      (0x00000200u)
#define CSL_DDR_SBSCS4CR_CSEXT_SHIFT     (0x00000009u)
#define CSL_DDR_SBSCS4CR_CSEXT_RESETVAL  (0x00000000u)

#define CSL_DDR_SBSCS4CR_READEN_MASK     (0x00000100u)
#define CSL_DDR_SBSCS4CR_READEN_SHIFT    (0x00000008u)
#define CSL_DDR_SBSCS4CR_READEN_RESETVAL (0x00000000u)

#define CSL_DDR_SBSCS4CR_WRLAT_MASK      (0x000000C0u)
#define CSL_DDR_SBSCS4CR_WRLAT_SHIFT     (0x00000006u)
#define CSL_DDR_SBSCS4CR_WRLAT_RESETVAL  (0x00000000u)

/*----WRLAT Tokens----*/
#define CSL_DDR_SBSCS4CR_WRLAT_WRLAT0    (0x00000000u)
#define CSL_DDR_SBSCS4CR_WRLAT_WRLAT1    (0x00000001u)
#define CSL_DDR_SBSCS4CR_WRLAT_WRLAT2    (0x00000002u)
#define CSL_DDR_SBSCS4CR_WRLAT_WRLAT3    (0x00000003u)

#define CSL_DDR_SBSCS4CR_RDLAT_MASK      (0x0000000Cu)
#define CSL_DDR_SBSCS4CR_RDLAT_SHIFT     (0x00000002u)
#define CSL_DDR_SBSCS4CR_RDLAT_RESETVAL  (0x00000000u)

/*----RDLAT Tokens----*/
#define CSL_DDR_SBSCS4CR_RDLAT_RDLAT0    (0x00000000u)
#define CSL_DDR_SBSCS4CR_RDLAT_RDLAT1    (0x00000001u)
#define CSL_DDR_SBSCS4CR_RDLAT_RDLAT2    (0x00000002u)
#define CSL_DDR_SBSCS4CR_RDLAT_RDLAT3    (0x00000003u)

#define CSL_DDR_SBSCS4CR_SBSIZE_MASK     (0x00000003u)
#define CSL_DDR_SBSCS4CR_SBSIZE_SHIFT    (0x00000000u)
#define CSL_DDR_SBSCS4CR_SBSIZE_RESETVAL (0x00000000u)

/*----SBSIZE Tokens----*/
#define CSL_DDR_SBSCS4CR_SBSIZE_DATA8    (0x00000000u)
#define CSL_DDR_SBSCS4CR_SBSIZE_DATA16   (0x00000001u)
#define CSL_DDR_SBSCS4CR_SBSIZE_DATA32   (0x00000002u)
#define CSL_DDR_SBSCS4CR_SBSIZE_DATA64   (0x00000003u)

#define CSL_DDR_SBSCS4CR_RESETVAL        (0x00000000u)

/* ASYNCCS5CR */

#define CSL_DDR_ASYNCCS5CR_SSEL_MASK     (0x80000000u)
#define CSL_DDR_ASYNCCS5CR_SSEL_SHIFT    (0x0000001Fu)
#define CSL_DDR_ASYNCCS5CR_SSEL_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS5CR_SS_MASK       (0x40000000u)
#define CSL_DDR_ASYNCCS5CR_SS_SHIFT      (0x0000001Eu)
#define CSL_DDR_ASYNCCS5CR_SS_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS5CR_BWEM_MASK     (0x20000000u)
#define CSL_DDR_ASYNCCS5CR_BWEM_SHIFT    (0x0000001Du)
#define CSL_DDR_ASYNCCS5CR_BWEM_RESETVAL (0x00000000u)

#define CSL_DDR_ASYNCCS5CR_EW_MASK       (0x10000000u)
#define CSL_DDR_ASYNCCS5CR_EW_SHIFT      (0x0000001Cu)
#define CSL_DDR_ASYNCCS5CR_EW_RESETVAL   (0x00000000u)

#define CSL_DDR_ASYNCCS5CR_WSETUP_MASK   (0x0F000000u)
#define CSL_DDR_ASYNCCS5CR_WSETUP_SHIFT  (0x00000018u)
#define CSL_DDR_ASYNCCS5CR_WSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS5CR_WSTROBE_MASK  (0x00FC0000u)
#define CSL_DDR_ASYNCCS5CR_WSTROBE_SHIFT (0x00000012u)
#define CSL_DDR_ASYNCCS5CR_WSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS5CR_WHOLD_MASK    (0x00038000u)
#define CSL_DDR_ASYNCCS5CR_WHOLD_SHIFT   (0x0000000Fu)
#define CSL_DDR_ASYNCCS5CR_WHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS5CR_RSETUP_MASK   (0x00007800u)
#define CSL_DDR_ASYNCCS5CR_RSETUP_SHIFT  (0x0000000Bu)
#define CSL_DDR_ASYNCCS5CR_RSETUP_RESETVAL (0x0000000Fu)

#define CSL_DDR_ASYNCCS5CR_RSTROBE_MASK  (0x000007E0u)
#define CSL_DDR_ASYNCCS5CR_RSTROBE_SHIFT (0x00000005u)
#define CSL_DDR_ASYNCCS5CR_RSTROBE_RESETVAL (0x0000003Fu)

#define CSL_DDR_ASYNCCS5CR_RHOLD_MASK    (0x0000001Cu)
#define CSL_DDR_ASYNCCS5CR_RHOLD_SHIFT   (0x00000002u)
#define CSL_DDR_ASYNCCS5CR_RHOLD_RESETVAL (0x00000007u)

#define CSL_DDR_ASYNCCS5CR_ASIZE_MASK    (0x00000003u)
#define CSL_DDR_ASYNCCS5CR_ASIZE_SHIFT   (0x00000000u)
#define CSL_DDR_ASYNCCS5CR_ASIZE_RESETVAL (0x00000000u)

/*----ASIZE Tokens----*/
#define CSL_DDR_ASYNCCS5CR_ASIZE_DATA8   (0x00000000u)
#define CSL_DDR_ASYNCCS5CR_ASIZE_DATA16  (0x00000001u)
#define CSL_DDR_ASYNCCS5CR_ASIZE_DATA32  (0x00000002u)
#define CSL_DDR_ASYNCCS5CR_ASIZE_DATA64  (0x00000003u)

#define CSL_DDR_ASYNCCS5CR_RESETVAL      (0x0FFFFFFCu)

/* SBSCS5CR */

#define CSL_DDR_SBSCS5CR_SSEL_MASK       (0x80000000u)
#define CSL_DDR_SBSCS5CR_SSEL_SHIFT      (0x0000001Fu)
#define CSL_DDR_SBSCS5CR_SSEL_RESETVAL   (0x00000000u)

#define CSL_DDR_SBSCS5CR_CSEXT_MASK      (0x00000200u)
#define CSL_DDR_SBSCS5CR_CSEXT_SHIFT     (0x00000009u)
#define CSL_DDR_SBSCS5CR_CSEXT_RESETVAL  (0x00000000u)

#define CSL_DDR_SBSCS5CR_READEN_MASK     (0x00000100u)
#define CSL_DDR_SBSCS5CR_READEN_SHIFT    (0x00000008u)
#define CSL_DDR_SBSCS5CR_READEN_RESETVAL (0x00000000u)

#define CSL_DDR_SBSCS5CR_WRLAT_MASK      (0x000000C0u)
#define CSL_DDR_SBSCS5CR_WRLAT_SHIFT     (0x00000006u)
#define CSL_DDR_SBSCS5CR_WRLAT_RESETVAL  (0x00000000u)

/*----WRLAT Tokens----*/
#define CSL_DDR_SBSCS5CR_WRLAT_WRLAT0    (0x00000000u)
#define CSL_DDR_SBSCS5CR_WRLAT_WRLAT1    (0x00000001u)
#define CSL_DDR_SBSCS5CR_WRLAT_WRLAT2    (0x00000002u)
#define CSL_DDR_SBSCS5CR_WRLAT_WRLAT3    (0x00000003u)

#define CSL_DDR_SBSCS5CR_RDLAT_MASK      (0x0000000Cu)
#define CSL_DDR_SBSCS5CR_RDLAT_SHIFT     (0x00000002u)
#define CSL_DDR_SBSCS5CR_RDLAT_RESETVAL  (0x00000000u)

/*----RDLAT Tokens----*/
#define CSL_DDR_SBSCS5CR_RDLAT_RDLAT0    (0x00000000u)
#define CSL_DDR_SBSCS5CR_RDLAT_RDLAT1    (0x00000001u)
#define CSL_DDR_SBSCS5CR_RDLAT_RDLAT2    (0x00000002u)
#define CSL_DDR_SBSCS5CR_RDLAT_RDLAT3    (0x00000003u)

#define CSL_DDR_SBSCS5CR_SBSIZE_MASK     (0x00000003u)
#define CSL_DDR_SBSCS5CR_SBSIZE_SHIFT    (0x00000000u)
#define CSL_DDR_SBSCS5CR_SBSIZE_RESETVAL (0x00000000u)

/*----SBSIZE Tokens----*/
#define CSL_DDR_SBSCS5CR_SBSIZE_DATA8    (0x00000000u)
#define CSL_DDR_SBSCS5CR_SBSIZE_DATA16   (0x00000001u)
#define CSL_DDR_SBSCS5CR_SBSIZE_DATA32   (0x00000002u)
#define CSL_DDR_SBSCS5CR_SBSIZE_DATA64   (0x00000003u)

#define CSL_DDR_SBSCS5CR_RESETVAL        (0x00000000u)

/* AWCCR */

#define CSL_DDR_AWCCR_WP_MASK            (0x40000000u)
#define CSL_DDR_AWCCR_WP_SHIFT           (0x0000001Eu)
#define CSL_DDR_AWCCR_WP_RESETVAL        (0x00000001u)

#define CSL_DDR_AWCCR_TA_MASK            (0x00000700u)
#define CSL_DDR_AWCCR_TA_SHIFT           (0x00000008u)
#define CSL_DDR_AWCCR_TA_RESETVAL        (0x00000003u)

#define CSL_DDR_AWCCR_MEWC_MASK          (0x000000FFu)
#define CSL_DDR_AWCCR_MEWC_SHIFT         (0x00000000u)
#define CSL_DDR_AWCCR_MEWC_RESETVAL      (0x00000080u)

#define CSL_DDR_AWCCR_RESETVAL           (0x40000380u)

/* IRR */

#define CSL_DDR_IRR_LT_MASK              (0x00000004u)
#define CSL_DDR_IRR_LT_SHIFT             (0x00000002u)
#define CSL_DDR_IRR_LT_RESETVAL          (0x00000000u)

#define CSL_DDR_IRR_PE_MASK              (0x00000002u)
#define CSL_DDR_IRR_PE_SHIFT             (0x00000001u)
#define CSL_DDR_IRR_PE_RESETVAL          (0x00000000u)

#define CSL_DDR_IRR_AT_MASK              (0x00000001u)
#define CSL_DDR_IRR_AT_SHIFT             (0x00000000u)
#define CSL_DDR_IRR_AT_RESETVAL          (0x00000000u)

#define CSL_DDR_IRR_RESETVAL             (0x00000000u)

/* IMR */

#define CSL_DDR_IMR_LTM_MASK             (0x00000004u)
#define CSL_DDR_IMR_LTM_SHIFT            (0x00000002u)
#define CSL_DDR_IMR_LTM_RESETVAL         (0x00000000u)

#define CSL_DDR_IMR_PEM_MASK             (0x00000002u)
#define CSL_DDR_IMR_PEM_SHIFT            (0x00000001u)
#define CSL_DDR_IMR_PEM_RESETVAL         (0x00000000u)

#define CSL_DDR_IMR_ATM_MASK             (0x00000001u)
#define CSL_DDR_IMR_ATM_SHIFT            (0x00000000u)
#define CSL_DDR_IMR_ATM_RESETVAL         (0x00000000u)

#define CSL_DDR_IMR_RESETVAL             (0x00000000u)

/* IMSR */

#define CSL_DDR_IMSR_LTMSET_MASK         (0x00000004u)
#define CSL_DDR_IMSR_LTMSET_SHIFT        (0x00000002u)
#define CSL_DDR_IMSR_LTMSET_RESETVAL     (0x00000000u)

#define CSL_DDR_IMSR_PEMSET_MASK         (0x00000002u)
#define CSL_DDR_IMSR_PEMSET_SHIFT        (0x00000001u)
#define CSL_DDR_IMSR_PEMSET_RESETVAL     (0x00000000u)

#define CSL_DDR_IMSR_ATMSET_MASK         (0x00000001u)
#define CSL_DDR_IMSR_ATMSET_SHIFT        (0x00000000u)
#define CSL_DDR_IMSR_ATMSET_RESETVAL     (0x00000000u)

#define CSL_DDR_IMSR_RESETVAL            (0x00000000u)

/* IMCR */

#define CSL_DDR_IMCR_LTMCLR_MASK         (0x00000004u)
#define CSL_DDR_IMCR_LTMCLR_SHIFT        (0x00000002u)
#define CSL_DDR_IMCR_LTMCLR_RESETVAL     (0x00000000u)

#define CSL_DDR_IMCR_PEMCLR_MASK         (0x00000002u)
#define CSL_DDR_IMCR_PEMCLR_SHIFT        (0x00000001u)
#define CSL_DDR_IMCR_PEMCLR_RESETVAL     (0x00000000u)

#define CSL_DDR_IMCR_ATMCLR_MASK         (0x00000001u)
#define CSL_DDR_IMCR_ATMCLR_SHIFT        (0x00000000u)
#define CSL_DDR_IMCR_ATMCLR_RESETVAL     (0x00000000u)

#define CSL_DDR_IMCR_RESETVAL            (0x00000000u)

/* DDRPHYREV */

#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYREV_DDRPHYIDREV_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYREV_RESETVAL       (0x00000000u)

/* DDRPHYCR */

#define CSL_DDR_DDRPHYCR_DDRPHYCTRL_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYCR_DDRPHYCTRL_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYCR_DDRPHYCTRL_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYCR_RESETVAL        (0x00000000u)

/* DDRPHYSR */

#define CSL_DDR_DDRPHYSR_DDRPHYSTAT_MASK (0xFFFFFFFFu)
#define CSL_DDR_DDRPHYSR_DDRPHYSTAT_SHIFT (0x00000000u)
#define CSL_DDR_DDRPHYSR_DDRPHYSTAT_RESETVAL (0x00000000u)

#define CSL_DDR_DDRPHYSR_RESETVAL        (0x00000000u)

/* VTPCTRL */

#define CSL_DDR_VTPCTRL_IOCTRL_MASK      (0x0000FFFFu)
#define CSL_DDR_VTPCTRL_IOCTRL_SHIFT     (0x00000000u)
#define CSL_DDR_VTPCTRL_IOCTRL_RESETVAL  (0x00000000u)

#define CSL_DDR_VTPCTRL_RESETVAL         (0x00000000u)

/* VTPSTAT */

#define CSL_DDR_VTPSTAT_IOCTRL_MASK      (0x0000000Fu)
#define CSL_DDR_VTPSTAT_IOCTRL_SHIFT     (0x00000000u)
#define CSL_DDR_VTPSTAT_IOCTRL_RESETVAL  (0x00000000u)

#define CSL_DDR_VTPSTAT_RESETVAL         (0x00000000u)

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 3          Aug 10:09:15 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:41    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
