#ifndef _CSLR_EDMATC1_001_H_
#define _CSLR_EDMATC1_001_H_
/*********************************************************************
 * Copyright (C) 2003-2004 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_edmatc1_001.h
 * 
 * \brief This file contains the Register Desciptions for EDMATC1
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure for DFIREG 
\**************************************************************************/
typedef struct  {
    volatile Uint32 DFOPT;
    volatile Uint32 DFSRC;
    volatile Uint32 DFCNT;
    volatile Uint32 DFDST;
    volatile Uint32 DFBIDX;
    volatile Uint32 DFMPPRXY;
    volatile Uint8 RSVD0[40];
} CSL_Edmatc1DfiregRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 REV;
    volatile Uint32 TCCFG;
    volatile Uint8 RSVD0[248];
    volatile Uint32 TPTCSTAT;
    volatile Uint32 INTSTAT;
    volatile Uint32 INTEN;
    volatile Uint32 INTCLR;
    volatile Uint32 INTCMD;
    volatile Uint8 RSVD1[12];
    volatile Uint32 ERRSTAT;
    volatile Uint32 ERREN;
    volatile Uint32 ERRCLR;
    volatile Uint32 ERRDET;
    volatile Uint32 ERRCMD;
    volatile Uint8 RSVD2[12];
    volatile Uint32 RDRATE;
    volatile Uint8 RSVD3[188];
    volatile Uint32 POPT;
    volatile Uint32 PSRC;
    volatile Uint32 PCNT;
    volatile Uint32 PDST;
    volatile Uint32 PBIDX;
    volatile Uint32 PMPPRXY;
    volatile Uint8 RSVD4[40];
    volatile Uint32 SAOPT;
    volatile Uint32 SASRC;
    volatile Uint32 SACNT;
    volatile Uint32 SADST;
    volatile Uint32 SABIDX;
    volatile Uint32 SAMPPRXY;
    volatile Uint32 SACNTRLD;
    volatile Uint32 SASRCBREF;
    volatile Uint32 SADSTBREF;
    volatile Uint8 RSVD5[28];
    volatile Uint32 DFCNTRLD;
    volatile Uint32 DFSRCBREF;
    volatile Uint32 DFDSTBREF;
    volatile Uint8 RSVD6[116];
    CSL_Edmatc1DfiregRegs DFIREG[4];
} CSL_Edmatc1Regs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* REV */

#define CSL_EDMATC1_REV_TYPE_MASK        (0x00FF0000u)
#define CSL_EDMATC1_REV_TYPE_SHIFT       (0x00000010u)
#define CSL_EDMATC1_REV_TYPE_RESETVAL    (0x00000006u)

#define CSL_EDMATC1_REV_CLASS_MASK       (0x0000FF00u)
#define CSL_EDMATC1_REV_CLASS_SHIFT      (0x00000008u)
#define CSL_EDMATC1_REV_CLASS_RESETVAL   (0x00000004u)

#define CSL_EDMATC1_REV_REV_MASK         (0x000000FFu)
#define CSL_EDMATC1_REV_REV_SHIFT        (0x00000000u)
#define CSL_EDMATC1_REV_REV_RESETVAL     (0x00000001u)

#define CSL_EDMATC1_REV_RESETVAL         (0x00060401u)

/* TCCFG */

#define CSL_EDMATC1_TCCFG_DREGDEPTH_MASK (0x00000300u)
#define CSL_EDMATC1_TCCFG_DREGDEPTH_SHIFT (0x00000008u)
#define CSL_EDMATC1_TCCFG_DREGDEPTH_RESETVAL (0x00000002u)

/*----DREGDEPTH Tokens----*/
#define CSL_EDMATC1_TCCFG_DREGDEPTH_2ENTRY (0x00000001u)

#define CSL_EDMATC1_TCCFG_BUSWIDTH_MASK  (0x00000030u)
#define CSL_EDMATC1_TCCFG_BUSWIDTH_SHIFT (0x00000004u)
#define CSL_EDMATC1_TCCFG_BUSWIDTH_RESETVAL (0x00000001u)

/*----BUSWIDTH Tokens----*/
#define CSL_EDMATC1_TCCFG_BUSWIDTH_64BIT (0x00000001u)

#define CSL_EDMATC1_TCCFG_FIFOSIZE_MASK  (0x00000007u)
#define CSL_EDMATC1_TCCFG_FIFOSIZE_SHIFT (0x00000000u)
#define CSL_EDMATC1_TCCFG_FIFOSIZE_RESETVAL (0x00000003u)

/*----FIFOSIZE Tokens----*/
#define CSL_EDMATC1_TCCFG_FIFOSIZE_128BYTE (0x00000002u)

#define CSL_EDMATC1_TCCFG_RESETVAL       (0x00000213u)

/* TPTCSTAT */

#define CSL_EDMATC1_TPTCSTAT_DFSTRT_MASK (0x00003000u)
#define CSL_EDMATC1_TPTCSTAT_DFSTRT_SHIFT (0x0000000Cu)
#define CSL_EDMATC1_TPTCSTAT_DFSTRT_RESETVAL (0x00000000u)

#define CSL_EDMATC1_TPTCSTAT_ATCV_MASK   (0x00000100u)
#define CSL_EDMATC1_TPTCSTAT_ATCV_SHIFT  (0x00000008u)
#define CSL_EDMATC1_TPTCSTAT_ATCV_RESETVAL (0x00000000u)

/*----ATCV Tokens----*/
#define CSL_EDMATC1_TPTCSTAT_ATCV_IDLE   (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_ATCV_BUSY   (0x00000001u)

#define CSL_EDMATC1_TPTCSTAT_DSTACT_MASK (0x00000070u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_SHIFT (0x00000004u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_RESETVAL (0x00000000u)

/*----DSTACT Tokens----*/
#define CSL_EDMATC1_TPTCSTAT_DSTACT_EMPTY (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_1TR  (0x00000001u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_2TR  (0x00000002u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_3TR  (0x00000003u)
#define CSL_EDMATC1_TPTCSTAT_DSTACT_4TR  (0x00000004u)

#define CSL_EDMATC1_TPTCSTAT_WSACTV_MASK (0x00000004u)
#define CSL_EDMATC1_TPTCSTAT_WSACTV_SHIFT (0x00000002u)
#define CSL_EDMATC1_TPTCSTAT_WSACTV_RESETVAL (0x00000000u)

/*----WSACTV Tokens----*/
#define CSL_EDMATC1_TPTCSTAT_WSACTV_NONE (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_WSACTV_PEND (0x00000001u)

#define CSL_EDMATC1_TPTCSTAT_SRCACTV_MASK (0x00000002u)
#define CSL_EDMATC1_TPTCSTAT_SRCACTV_SHIFT (0x00000001u)
#define CSL_EDMATC1_TPTCSTAT_SRCACTV_RESETVAL (0x00000000u)

/*----SRCACTV Tokens----*/
#define CSL_EDMATC1_TPTCSTAT_SRCACTV_IDLE (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_SRCACTV_BUSY (0x00000001u)

#define CSL_EDMATC1_TPTCSTAT_PROGBUSY_MASK (0x00000001u)
#define CSL_EDMATC1_TPTCSTAT_PROGBUSY_SHIFT (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_PROGBUSY_RESETVAL (0x00000000u)

/*----PROGBUSY Tokens----*/
#define CSL_EDMATC1_TPTCSTAT_PROGBUSY_IDLE (0x00000000u)
#define CSL_EDMATC1_TPTCSTAT_PROGBUSY_BUSY (0x00000001u)

#define CSL_EDMATC1_TPTCSTAT_RESETVAL    (0x00000000u)

/* INTSTAT */

#define CSL_EDMATC1_INTSTAT_TRDONE_MASK  (0x00000002u)
#define CSL_EDMATC1_INTSTAT_TRDONE_SHIFT (0x00000001u)
#define CSL_EDMATC1_INTSTAT_TRDONE_RESETVAL (0x00000000u)

/*----TRDONE Tokens----*/
#define CSL_EDMATC1_INTSTAT_TRDONE_NONE  (0x00000000u)
#define CSL_EDMATC1_INTSTAT_TRDONE_DONE  (0x00000001u)

#define CSL_EDMATC1_INTSTAT_PROGEMPTY_MASK (0x00000001u)
#define CSL_EDMATC1_INTSTAT_PROGEMPTY_SHIFT (0x00000000u)
#define CSL_EDMATC1_INTSTAT_PROGEMPTY_RESETVAL (0x00000000u)

/*----PROGEMPTY Tokens----*/
#define CSL_EDMATC1_INTSTAT_PROGEMPTY_NONE (0x00000000u)
#define CSL_EDMATC1_INTSTAT_PROGEMPTY_EMPTY (0x00000001u)

#define CSL_EDMATC1_INTSTAT_RESETVAL     (0x00000000u)

/* INTEN */

#define CSL_EDMATC1_INTEN_TRDONE_MASK    (0x00000002u)
#define CSL_EDMATC1_INTEN_TRDONE_SHIFT   (0x00000001u)
#define CSL_EDMATC1_INTEN_TRDONE_RESETVAL (0x00000000u)

/*----TRDONE Tokens----*/
#define CSL_EDMATC1_INTEN_TRDONE_DISABLE (0x00000000u)
#define CSL_EDMATC1_INTEN_TRDONE_ENABLE  (0x00000001u)

#define CSL_EDMATC1_INTEN_PROGEMPTY_MASK (0x00000001u)
#define CSL_EDMATC1_INTEN_PROGEMPTY_SHIFT (0x00000000u)
#define CSL_EDMATC1_INTEN_PROGEMPTY_RESETVAL (0x00000000u)

/*----PROGEMPTY Tokens----*/
#define CSL_EDMATC1_INTEN_PROGEMPTY_DISABLE (0x00000000u)
#define CSL_EDMATC1_INTEN_PROGEMPTY_ENABLE (0x00000001u)

#define CSL_EDMATC1_INTEN_RESETVAL       (0x00000000u)

/* INTCLR */

#define CSL_EDMATC1_INTCLR_TRDONE_MASK   (0x00000002u)
#define CSL_EDMATC1_INTCLR_TRDONE_SHIFT  (0x00000001u)
#define CSL_EDMATC1_INTCLR_TRDONE_RESETVAL (0x00000000u)

/*----TRDONE Tokens----*/
#define CSL_EDMATC1_INTCLR_TRDONE_CLEAR  (0x00000001u)

#define CSL_EDMATC1_INTCLR_PROGEMPTY_MASK (0x00000001u)
#define CSL_EDMATC1_INTCLR_PROGEMPTY_SHIFT (0x00000000u)
#define CSL_EDMATC1_INTCLR_PROGEMPTY_RESETVAL (0x00000000u)

/*----PROGEMPTY Tokens----*/
#define CSL_EDMATC1_INTCLR_PROGEMPTY_CLEAR (0x00000001u)

#define CSL_EDMATC1_INTCLR_RESETVAL      (0x00000000u)

/* INTCMD */

#define CSL_EDMATC1_INTCMD_SET_MASK      (0x00000002u)
#define CSL_EDMATC1_INTCMD_SET_SHIFT     (0x00000001u)
#define CSL_EDMATC1_INTCMD_SET_RESETVAL  (0x00000000u)

/*----SET Tokens----*/
#define CSL_EDMATC1_INTCMD_SET_SET       (0x00000001u)

#define CSL_EDMATC1_INTCMD_EVAL_MASK     (0x00000001u)
#define CSL_EDMATC1_INTCMD_EVAL_SHIFT    (0x00000000u)
#define CSL_EDMATC1_INTCMD_EVAL_RESETVAL (0x00000000u)

/*----EVAL Tokens----*/
#define CSL_EDMATC1_INTCMD_EVAL_EVAL     (0x00000001u)

#define CSL_EDMATC1_INTCMD_RESETVAL      (0x00000000u)

/* ERRSTAT */

#define CSL_EDMATC1_ERRSTAT_MMRAERR_MASK (0x00000008u)
#define CSL_EDMATC1_ERRSTAT_MMRAERR_SHIFT (0x00000003u)
#define CSL_EDMATC1_ERRSTAT_MMRAERR_RESETVAL (0x00000000u)

/*----MMRAERR Tokens----*/
#define CSL_EDMATC1_ERRSTAT_MMRAERR_NONE (0x00000000u)
#define CSL_EDMATC1_ERRSTAT_MMRAERR_ERROR (0x00000001u)

#define CSL_EDMATC1_ERRSTAT_TRERR_MASK   (0x00000004u)
#define CSL_EDMATC1_ERRSTAT_TRERR_SHIFT  (0x00000002u)
#define CSL_EDMATC1_ERRSTAT_TRERR_RESETVAL (0x00000000u)

/*----TRERR Tokens----*/
#define CSL_EDMATC1_ERRSTAT_TRERR_NONE   (0x00000000u)
#define CSL_EDMATC1_ERRSTAT_TRERR_ERROR  (0x00000001u)
#define CSL_EDMATC1_ERRSTAT__RESV_NONE   (0x00000000u)
#define CSL_EDMATC1_ERRSTAT__RESV_ERROR  (0x00000001u)

#define CSL_EDMATC1_ERRSTAT_BUSERR_MASK  (0x00000001u)
#define CSL_EDMATC1_ERRSTAT_BUSERR_SHIFT (0x00000000u)
#define CSL_EDMATC1_ERRSTAT_BUSERR_RESETVAL (0x00000000u)

/*----BUSERR Tokens----*/
#define CSL_EDMATC1_ERRSTAT_BUSERR_NONE  (0x00000000u)
#define CSL_EDMATC1_ERRSTAT_BUSERR_ERROR (0x00000001u)

#define CSL_EDMATC1_ERRSTAT_RESETVAL     (0x00000000u)

/* ERREN */

#define CSL_EDMATC1_ERREN_MMRAERR_MASK   (0x00000008u)
#define CSL_EDMATC1_ERREN_MMRAERR_SHIFT  (0x00000003u)
#define CSL_EDMATC1_ERREN_MMRAERR_RESETVAL (0x00000000u)

/*----MMRAERR Tokens----*/
#define CSL_EDMATC1_ERREN_MMRAERR_DISABLE (0x00000000u)
#define CSL_EDMATC1_ERREN_MMRAERR_ENABLE (0x00000001u)

#define CSL_EDMATC1_ERREN_TRERR_MASK     (0x00000004u)
#define CSL_EDMATC1_ERREN_TRERR_SHIFT    (0x00000002u)
#define CSL_EDMATC1_ERREN_TRERR_RESETVAL (0x00000000u)

/*----TRERR Tokens----*/
#define CSL_EDMATC1_ERREN_TRERR_DISABLE  (0x00000000u)
#define CSL_EDMATC1_ERREN_TRERR_ENABLE   (0x00000001u)
#define CSL_EDMATC1_ERREN__RESV_DISABLE  (0x00000000u)
#define CSL_EDMATC1_ERREN__RESV_ENABLE   (0x00000001u)

#define CSL_EDMATC1_ERREN_BUSERR_MASK    (0x00000001u)
#define CSL_EDMATC1_ERREN_BUSERR_SHIFT   (0x00000000u)
#define CSL_EDMATC1_ERREN_BUSERR_RESETVAL (0x00000000u)

/*----BUSERR Tokens----*/
#define CSL_EDMATC1_ERREN_BUSERR_DISABLE (0x00000000u)
#define CSL_EDMATC1_ERREN_BUSERR_ENABLE  (0x00000001u)

#define CSL_EDMATC1_ERREN_RESETVAL       (0x00000000u)

/* ERRCLR */

#define CSL_EDMATC1_ERRCLR_MMRAERR_MASK  (0x00000008u)
#define CSL_EDMATC1_ERRCLR_MMRAERR_SHIFT (0x00000003u)
#define CSL_EDMATC1_ERRCLR_MMRAERR_RESETVAL (0x00000000u)

/*----MMRAERR Tokens----*/
#define CSL_EDMATC1_ERRCLR_MMRAERR_CLEAR (0x00000001u)

#define CSL_EDMATC1_ERRCLR_TRERR_MASK    (0x00000004u)
#define CSL_EDMATC1_ERRCLR_TRERR_SHIFT   (0x00000002u)
#define CSL_EDMATC1_ERRCLR_TRERR_RESETVAL (0x00000000u)

/*----TRERR Tokens----*/
#define CSL_EDMATC1_ERRCLR_TRERR_CLEAR   (0x00000001u)
#define CSL_EDMATC1_ERRCLR__RESV_CLEAR   (0x00000001u)

#define CSL_EDMATC1_ERRCLR_BUSERR_MASK   (0x00000001u)
#define CSL_EDMATC1_ERRCLR_BUSERR_SHIFT  (0x00000000u)
#define CSL_EDMATC1_ERRCLR_BUSERR_RESETVAL (0x00000000u)

/*----BUSERR Tokens----*/
#define CSL_EDMATC1_ERRCLR_BUSERR_CLEAR  (0x00000001u)

#define CSL_EDMATC1_ERRCLR_RESETVAL      (0x00000000u)

/* ERRDET */

#define CSL_EDMATC1_ERRDET_TCCHEN_MASK   (0x00020000u)
#define CSL_EDMATC1_ERRDET_TCCHEN_SHIFT  (0x00000011u)
#define CSL_EDMATC1_ERRDET_TCCHEN_RESETVAL (0x00000000u)

#define CSL_EDMATC1_ERRDET_TCINTEN_MASK  (0x00010000u)
#define CSL_EDMATC1_ERRDET_TCINTEN_SHIFT (0x00000010u)
#define CSL_EDMATC1_ERRDET_TCINTEN_RESETVAL (0x00000000u)

#define CSL_EDMATC1_ERRDET_TCC_MASK      (0x00003F00u)
#define CSL_EDMATC1_ERRDET_TCC_SHIFT     (0x00000008u)
#define CSL_EDMATC1_ERRDET_TCC_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_ERRDET_STAT_MASK     (0x0000000Fu)
#define CSL_EDMATC1_ERRDET_STAT_SHIFT    (0x00000000u)
#define CSL_EDMATC1_ERRDET_STAT_RESETVAL (0x00000000u)

/*----STAT Tokens----*/
#define CSL_EDMATC1_ERRDET_STAT_NONE     (0x00000000u)
#define CSL_EDMATC1_ERRDET_STAT_READ_ADDRESS (0x00000001u)
#define CSL_EDMATC1_ERRDET_STAT_READ_PRIVILEGE (0x00000002u)
#define CSL_EDMATC1_ERRDET_STAT_READ_TIMEOUT (0x00000003u)
#define CSL_EDMATC1_ERRDET_STAT_READ_DATA (0x00000004u)
#define CSL_EDMATC1_ERRDET_STAT_READ_EXCLUSIVE (0x00000007u)
#define CSL_EDMATC1_ERRDET_STAT_WRITE_ADDRESS (0x00000009u)
#define CSL_EDMATC1_ERRDET_STAT_WRITE_PRIVILEGE (0x0000000Au)
#define CSL_EDMATC1_ERRDET_STAT_WRITE_TIMEOUT (0x0000000Bu)
#define CSL_EDMATC1_ERRDET_STAT_WRITE_DATA (0x0000000Cu)
#define CSL_EDMATC1_ERRDET_STAT_WRITE_EXCLUSIVE (0x0000000Fu)

#define CSL_EDMATC1_ERRDET_RESETVAL      (0x00000000u)

/* ERRCMD */

#define CSL_EDMATC1_ERRCMD_SET_MASK      (0x00000002u)
#define CSL_EDMATC1_ERRCMD_SET_SHIFT     (0x00000001u)
#define CSL_EDMATC1_ERRCMD_SET_RESETVAL  (0x00000000u)

/*----SET Tokens----*/
#define CSL_EDMATC1_ERRCMD_SET_SET       (0x00000001u)

#define CSL_EDMATC1_ERRCMD_EVAL_MASK     (0x00000001u)
#define CSL_EDMATC1_ERRCMD_EVAL_SHIFT    (0x00000000u)
#define CSL_EDMATC1_ERRCMD_EVAL_RESETVAL (0x00000000u)

/*----EVAL Tokens----*/
#define CSL_EDMATC1_ERRCMD_EVAL_EVAL     (0x00000001u)

#define CSL_EDMATC1_ERRCMD_RESETVAL      (0x00000000u)

/* RDRATE */

#define CSL_EDMATC1_RDRATE_RDRATE_MASK   (0x00000007u)
#define CSL_EDMATC1_RDRATE_RDRATE_SHIFT  (0x00000000u)
#define CSL_EDMATC1_RDRATE_RDRATE_RESETVAL (0x00000000u)

/*----RDRATE Tokens----*/
#define CSL_EDMATC1_RDRATE_RDRATE_AFAP   (0x00000000u)
#define CSL_EDMATC1_RDRATE_RDRATE_4CYCLE (0x00000001u)
#define CSL_EDMATC1_RDRATE_RDRATE_8CYCLE (0x00000002u)
#define CSL_EDMATC1_RDRATE_RDRATE_16CYCLE (0x00000003u)
#define CSL_EDMATC1_RDRATE_RDRATE_32CYCLE (0x00000004u)

#define CSL_EDMATC1_RDRATE_RESETVAL      (0x00000000u)

/* POPT */

#define CSL_EDMATC1_POPT_TCCHEN_MASK     (0x00400000u)
#define CSL_EDMATC1_POPT_TCCHEN_SHIFT    (0x00000016u)
#define CSL_EDMATC1_POPT_TCCHEN_RESETVAL (0x00000000u)

/*----TCCHEN Tokens----*/
#define CSL_EDMATC1_POPT_TCCHEN_DISABLE  (0x00000000u)
#define CSL_EDMATC1_POPT_TCCHEN_ENABLE   (0x00000001u)

#define CSL_EDMATC1_POPT_TCINTEN_MASK    (0x00100000u)
#define CSL_EDMATC1_POPT_TCINTEN_SHIFT   (0x00000014u)
#define CSL_EDMATC1_POPT_TCINTEN_RESETVAL (0x00000000u)

/*----TCINTEN Tokens----*/
#define CSL_EDMATC1_POPT_TCINTEN_DISABLE (0x00000000u)
#define CSL_EDMATC1_POPT_TCINTEN_ENABLE  (0x00000001u)

#define CSL_EDMATC1_POPT_TCC_MASK        (0x0003F000u)
#define CSL_EDMATC1_POPT_TCC_SHIFT       (0x0000000Cu)
#define CSL_EDMATC1_POPT_TCC_RESETVAL    (0x00000000u)

#define CSL_EDMATC1_POPT_FWID_MASK       (0x00000700u)
#define CSL_EDMATC1_POPT_FWID_SHIFT      (0x00000008u)
#define CSL_EDMATC1_POPT_FWID_RESETVAL   (0x00000000u)

/*----FWID Tokens----*/
#define CSL_EDMATC1_POPT_FWID_8BIT       (0x00000000u)
#define CSL_EDMATC1_POPT_FWID_16BIT      (0x00000001u)
#define CSL_EDMATC1_POPT_FWID_32BIT      (0x00000002u)
#define CSL_EDMATC1_POPT_FWID_64BIT      (0x00000003u)
#define CSL_EDMATC1_POPT_FWID_128BIT     (0x00000004u)
#define CSL_EDMATC1_POPT_FWID_256BIT     (0x00000005u)

#define CSL_EDMATC1_POPT_PRI_MASK        (0x00000070u)
#define CSL_EDMATC1_POPT_PRI_SHIFT       (0x00000004u)
#define CSL_EDMATC1_POPT_PRI_RESETVAL    (0x00000000u)

#define CSL_EDMATC1_POPT_DAM_MASK        (0x00000002u)
#define CSL_EDMATC1_POPT_DAM_SHIFT       (0x00000001u)
#define CSL_EDMATC1_POPT_DAM_RESETVAL    (0x00000000u)

/*----DAM Tokens----*/
#define CSL_EDMATC1_POPT_DAM_INCR        (0x00000000u)
#define CSL_EDMATC1_POPT_DAM_FIFO        (0x00000001u)

#define CSL_EDMATC1_POPT_SAM_MASK        (0x00000001u)
#define CSL_EDMATC1_POPT_SAM_SHIFT       (0x00000000u)
#define CSL_EDMATC1_POPT_SAM_RESETVAL    (0x00000000u)

/*----SAM Tokens----*/
#define CSL_EDMATC1_POPT_SAM_INCR        (0x00000000u)
#define CSL_EDMATC1_POPT_SAM_FIFO        (0x00000001u)

#define CSL_EDMATC1_POPT_RESETVAL        (0x00000000u)

/* PSRC */

#define CSL_EDMATC1_PSRC_SADDR_MASK      (0xFFFFFFFFu)
#define CSL_EDMATC1_PSRC_SADDR_SHIFT     (0x00000000u)
#define CSL_EDMATC1_PSRC_SADDR_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_PSRC_RESETVAL        (0x00000000u)

/* PCNT */

#define CSL_EDMATC1_PCNT_BCNT_MASK       (0xFFFF0000u)
#define CSL_EDMATC1_PCNT_BCNT_SHIFT      (0x00000010u)
#define CSL_EDMATC1_PCNT_BCNT_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_PCNT_ACNT_MASK       (0x0000FFFFu)
#define CSL_EDMATC1_PCNT_ACNT_SHIFT      (0x00000000u)
#define CSL_EDMATC1_PCNT_ACNT_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_PCNT_RESETVAL        (0x00000000u)

/* PDST */

#define CSL_EDMATC1_PDST_DADDR_MASK      (0xFFFFFFFFu)
#define CSL_EDMATC1_PDST_DADDR_SHIFT     (0x00000000u)
#define CSL_EDMATC1_PDST_DADDR_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_PDST_RESETVAL        (0x00000000u)

/* PBIDX */

#define CSL_EDMATC1_PBIDX_DBIDX_MASK     (0xFFFF0000u)
#define CSL_EDMATC1_PBIDX_DBIDX_SHIFT    (0x00000010u)
#define CSL_EDMATC1_PBIDX_DBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_PBIDX_SBIDX_MASK     (0x0000FFFFu)
#define CSL_EDMATC1_PBIDX_SBIDX_SHIFT    (0x00000000u)
#define CSL_EDMATC1_PBIDX_SBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_PBIDX_RESETVAL       (0x00000000u)

/* PMPPRXY */

#define CSL_EDMATC1_PMPPRXY_PRIV_MASK    (0x00000100u)
#define CSL_EDMATC1_PMPPRXY_PRIV_SHIFT   (0x00000008u)
#define CSL_EDMATC1_PMPPRXY_PRIV_RESETVAL (0x00000000u)

/*----PRIV Tokens----*/
#define CSL_EDMATC1_PMPPRXY_PRIV_USER    (0x00000000u)
#define CSL_EDMATC1_PMPPRXY_PRIV_SUPERVISOR (0x00000001u)

#define CSL_EDMATC1_PMPPRXY_PRIVID_MASK  (0x0000000Fu)
#define CSL_EDMATC1_PMPPRXY_PRIVID_SHIFT (0x00000000u)
#define CSL_EDMATC1_PMPPRXY_PRIVID_RESETVAL (0x00000000u)

#define CSL_EDMATC1_PMPPRXY_RESETVAL     (0x00000000u)

/* SAOPT */

#define CSL_EDMATC1_SAOPT_TCCHEN_MASK    (0x00400000u)
#define CSL_EDMATC1_SAOPT_TCCHEN_SHIFT   (0x00000016u)
#define CSL_EDMATC1_SAOPT_TCCHEN_RESETVAL (0x00000000u)

/*----TCCHEN Tokens----*/
#define CSL_EDMATC1_SAOPT_TCCHEN_DISABLE (0x00000000u)
#define CSL_EDMATC1_SAOPT_TCCHEN_ENABLE  (0x00000001u)

#define CSL_EDMATC1_SAOPT_TCINTEN_MASK   (0x00100000u)
#define CSL_EDMATC1_SAOPT_TCINTEN_SHIFT  (0x00000014u)
#define CSL_EDMATC1_SAOPT_TCINTEN_RESETVAL (0x00000000u)

/*----TCINTEN Tokens----*/
#define CSL_EDMATC1_SAOPT_TCINTEN_DISABLE (0x00000000u)
#define CSL_EDMATC1_SAOPT_TCINTEN_ENABLE (0x00000001u)

#define CSL_EDMATC1_SAOPT_TCC_MASK       (0x0003F000u)
#define CSL_EDMATC1_SAOPT_TCC_SHIFT      (0x0000000Cu)
#define CSL_EDMATC1_SAOPT_TCC_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_SAOPT_FWID_MASK      (0x00000700u)
#define CSL_EDMATC1_SAOPT_FWID_SHIFT     (0x00000008u)
#define CSL_EDMATC1_SAOPT_FWID_RESETVAL  (0x00000000u)

/*----FWID Tokens----*/
#define CSL_EDMATC1_SAOPT_FWID_8BIT      (0x00000000u)
#define CSL_EDMATC1_SAOPT_FWID_16BIT     (0x00000001u)
#define CSL_EDMATC1_SAOPT_FWID_32BIT     (0x00000002u)
#define CSL_EDMATC1_SAOPT_FWID_64BIT     (0x00000003u)
#define CSL_EDMATC1_SAOPT_FWID_128BIT    (0x00000004u)
#define CSL_EDMATC1_SAOPT_FWID_256BIT    (0x00000005u)

#define CSL_EDMATC1_SAOPT_PRI_MASK       (0x00000070u)
#define CSL_EDMATC1_SAOPT_PRI_SHIFT      (0x00000004u)
#define CSL_EDMATC1_SAOPT_PRI_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_SAOPT_DAM_MASK       (0x00000002u)
#define CSL_EDMATC1_SAOPT_DAM_SHIFT      (0x00000001u)
#define CSL_EDMATC1_SAOPT_DAM_RESETVAL   (0x00000000u)

/*----DAM Tokens----*/
#define CSL_EDMATC1_SAOPT_DAM_INCR       (0x00000000u)
#define CSL_EDMATC1_SAOPT_DAM_FIFO       (0x00000001u)

#define CSL_EDMATC1_SAOPT_SAM_MASK       (0x00000001u)
#define CSL_EDMATC1_SAOPT_SAM_SHIFT      (0x00000000u)
#define CSL_EDMATC1_SAOPT_SAM_RESETVAL   (0x00000000u)

/*----SAM Tokens----*/
#define CSL_EDMATC1_SAOPT_SAM_INCR       (0x00000000u)
#define CSL_EDMATC1_SAOPT_SAM_FIFO       (0x00000001u)

#define CSL_EDMATC1_SAOPT_RESETVAL       (0x00000000u)

/* SASRC */

#define CSL_EDMATC1_SASRC_SADDR_MASK     (0xFFFFFFFFu)
#define CSL_EDMATC1_SASRC_SADDR_SHIFT    (0x00000000u)
#define CSL_EDMATC1_SASRC_SADDR_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SASRC_RESETVAL       (0x00000000u)

/* SACNT */

#define CSL_EDMATC1_SACNT_BCNT_MASK      (0xFFFF0000u)
#define CSL_EDMATC1_SACNT_BCNT_SHIFT     (0x00000010u)
#define CSL_EDMATC1_SACNT_BCNT_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_SACNT_ACNT_MASK      (0x0000FFFFu)
#define CSL_EDMATC1_SACNT_ACNT_SHIFT     (0x00000000u)
#define CSL_EDMATC1_SACNT_ACNT_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_SACNT_RESETVAL       (0x00000000u)

/* SADST */

#define CSL_EDMATC1_SADST_RESETVAL       (0x00000000u)

/* SABIDX */

#define CSL_EDMATC1_SABIDX_DBIDX_MASK    (0xFFFF0000u)
#define CSL_EDMATC1_SABIDX_DBIDX_SHIFT   (0x00000010u)
#define CSL_EDMATC1_SABIDX_DBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SABIDX_SBIDX_MASK    (0x0000FFFFu)
#define CSL_EDMATC1_SABIDX_SBIDX_SHIFT   (0x00000000u)
#define CSL_EDMATC1_SABIDX_SBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SABIDX_RESETVAL      (0x00000000u)

/* SAMPPRXY */

#define CSL_EDMATC1_SAMPPRXY_PRIV_MASK   (0x00000100u)
#define CSL_EDMATC1_SAMPPRXY_PRIV_SHIFT  (0x00000008u)
#define CSL_EDMATC1_SAMPPRXY_PRIV_RESETVAL (0x00000000u)

/*----PRIV Tokens----*/
#define CSL_EDMATC1_SAMPPRXY_PRIV_USER   (0x00000000u)
#define CSL_EDMATC1_SAMPPRXY_PRIV_SUPERVISOR (0x00000001u)

#define CSL_EDMATC1_SAMPPRXY_PRIVID_MASK (0x0000000Fu)
#define CSL_EDMATC1_SAMPPRXY_PRIVID_SHIFT (0x00000000u)
#define CSL_EDMATC1_SAMPPRXY_PRIVID_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SAMPPRXY_RESETVAL    (0x00000000u)

/* SACNTRLD */

#define CSL_EDMATC1_SACNTRLD_ACNTRLD_MASK (0x0000FFFFu)
#define CSL_EDMATC1_SACNTRLD_ACNTRLD_SHIFT (0x00000000u)
#define CSL_EDMATC1_SACNTRLD_ACNTRLD_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SACNTRLD_RESETVAL    (0x00000000u)

/* SASRCBREF */

#define CSL_EDMATC1_SASRCBREF_SADDRBREFG_MASK (0xFFFFFFFFu)
#define CSL_EDMATC1_SASRCBREF_SADDRBREFG_SHIFT (0x00000000u)
#define CSL_EDMATC1_SASRCBREF_SADDRBREFG_RESETVAL (0x00000000u)

#define CSL_EDMATC1_SASRCBREF_RESETVAL   (0x00000000u)

/* SADSTBREF */

#define CSL_EDMATC1_SADSTBREF_RESETVAL   (0x00000000u)

/* DFCNTRLD */

#define CSL_EDMATC1_DFCNTRLD_ACNTRLD_MASK (0x0000FFFFu)
#define CSL_EDMATC1_DFCNTRLD_ACNTRLD_SHIFT (0x00000000u)
#define CSL_EDMATC1_DFCNTRLD_ACNTRLD_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFCNTRLD_RESETVAL    (0x00000000u)

/* DFSRCBREF */

#define CSL_EDMATC1_DFSRCBREF_RESETVAL   (0x00000000u)

/* DFDSTBREF */

#define CSL_EDMATC1_DFDSTBREF_DADDRBREF_MASK (0xFFFFFFFFu)
#define CSL_EDMATC1_DFDSTBREF_DADDRBREF_SHIFT (0x00000000u)
#define CSL_EDMATC1_DFDSTBREF_DADDRBREF_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFDSTBREF_RESETVAL   (0x00000000u)

/* DFOPT */

#define CSL_EDMATC1_DFOPT_TCCHEN_MASK    (0x00400000u)
#define CSL_EDMATC1_DFOPT_TCCHEN_SHIFT   (0x00000016u)
#define CSL_EDMATC1_DFOPT_TCCHEN_RESETVAL (0x00000000u)

/*----TCCHEN Tokens----*/
#define CSL_EDMATC1_DFOPT_TCCHEN_DISABLE (0x00000000u)
#define CSL_EDMATC1_DFOPT_TCCHEN_ENABLE  (0x00000001u)

#define CSL_EDMATC1_DFOPT_TCINTEN_MASK   (0x00100000u)
#define CSL_EDMATC1_DFOPT_TCINTEN_SHIFT  (0x00000014u)
#define CSL_EDMATC1_DFOPT_TCINTEN_RESETVAL (0x00000000u)

/*----TCINTEN Tokens----*/
#define CSL_EDMATC1_DFOPT_TCINTEN_DISABLE (0x00000000u)
#define CSL_EDMATC1_DFOPT_TCINTEN_ENABLE (0x00000001u)

#define CSL_EDMATC1_DFOPT_TCC_MASK       (0x0003F000u)
#define CSL_EDMATC1_DFOPT_TCC_SHIFT      (0x0000000Cu)
#define CSL_EDMATC1_DFOPT_TCC_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_DFOPT_FWID_MASK      (0x00000700u)
#define CSL_EDMATC1_DFOPT_FWID_SHIFT     (0x00000008u)
#define CSL_EDMATC1_DFOPT_FWID_RESETVAL  (0x00000000u)

/*----FWID Tokens----*/
#define CSL_EDMATC1_DFOPT_FWID_8BIT      (0x00000000u)
#define CSL_EDMATC1_DFOPT_FWID_16BIT     (0x00000001u)
#define CSL_EDMATC1_DFOPT_FWID_32BIT     (0x00000002u)
#define CSL_EDMATC1_DFOPT_FWID_64BIT     (0x00000003u)
#define CSL_EDMATC1_DFOPT_FWID_128BIT    (0x00000004u)
#define CSL_EDMATC1_DFOPT_FWID_256BIT    (0x00000005u)

#define CSL_EDMATC1_DFOPT_PRI_MASK       (0x00000070u)
#define CSL_EDMATC1_DFOPT_PRI_SHIFT      (0x00000004u)
#define CSL_EDMATC1_DFOPT_PRI_RESETVAL   (0x00000000u)

#define CSL_EDMATC1_DFOPT_DAM_MASK       (0x00000002u)
#define CSL_EDMATC1_DFOPT_DAM_SHIFT      (0x00000001u)
#define CSL_EDMATC1_DFOPT_DAM_RESETVAL   (0x00000000u)

/*----DAM Tokens----*/
#define CSL_EDMATC1_DFOPT_DAM_INCR       (0x00000000u)
#define CSL_EDMATC1_DFOPT_DAM_FIFO       (0x00000001u)

#define CSL_EDMATC1_DFOPT_SAM_MASK       (0x00000001u)
#define CSL_EDMATC1_DFOPT_SAM_SHIFT      (0x00000000u)
#define CSL_EDMATC1_DFOPT_SAM_RESETVAL   (0x00000000u)

/*----SAM Tokens----*/
#define CSL_EDMATC1_DFOPT_SAM_INCR       (0x00000000u)
#define CSL_EDMATC1_DFOPT_SAM_FIFO       (0x00000001u)

#define CSL_EDMATC1_DFOPT_RESETVAL       (0x00000000u)

/* DFSRC */

#define CSL_EDMATC1_DFSRC_RESETVAL       (0x00000000u)

/* DFCNT */

#define CSL_EDMATC1_DFCNT_BCNT_MASK      (0xFFFF0000u)
#define CSL_EDMATC1_DFCNT_BCNT_SHIFT     (0x00000010u)
#define CSL_EDMATC1_DFCNT_BCNT_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_DFCNT_ACNT_MASK      (0x0000FFFFu)
#define CSL_EDMATC1_DFCNT_ACNT_SHIFT     (0x00000000u)
#define CSL_EDMATC1_DFCNT_ACNT_RESETVAL  (0x00000000u)

#define CSL_EDMATC1_DFCNT_RESETVAL       (0x00000000u)

/* DFDST */

#define CSL_EDMATC1_DFDST_DADDR_MASK     (0xFFFFFFFFu)
#define CSL_EDMATC1_DFDST_DADDR_SHIFT    (0x00000000u)
#define CSL_EDMATC1_DFDST_DADDR_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFDST_RESETVAL       (0x00000000u)

/* DFBIDX */

#define CSL_EDMATC1_DFBIDX_DBIDX_MASK    (0xFFFF0000u)
#define CSL_EDMATC1_DFBIDX_DBIDX_SHIFT   (0x00000010u)
#define CSL_EDMATC1_DFBIDX_DBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFBIDX_SBIDX_MASK    (0x0000FFFFu)
#define CSL_EDMATC1_DFBIDX_SBIDX_SHIFT   (0x00000000u)
#define CSL_EDMATC1_DFBIDX_SBIDX_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFBIDX_RESETVAL      (0x00000000u)

/* DFMPPRXY */

#define CSL_EDMATC1_DFMPPRXY_PRIV_MASK   (0x00000100u)
#define CSL_EDMATC1_DFMPPRXY_PRIV_SHIFT  (0x00000008u)
#define CSL_EDMATC1_DFMPPRXY_PRIV_RESETVAL (0x00000000u)

/*----PRIV Tokens----*/
#define CSL_EDMATC1_DFMPPRXY_PRIV_USER   (0x00000000u)
#define CSL_EDMATC1_DFMPPRXY_PRIV_SUPERVISOR (0x00000001u)

#define CSL_EDMATC1_DFMPPRXY_PRIVID_MASK (0x0000000Fu)
#define CSL_EDMATC1_DFMPPRXY_PRIVID_SHIFT (0x00000000u)
#define CSL_EDMATC1_DFMPPRXY_PRIVID_RESETVAL (0x00000000u)

#define CSL_EDMATC1_DFMPPRXY_RESETVAL    (0x00000000u)

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 3          Dec 14:17:11 7         4658             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL Dec6 2004 Release                               */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:43    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
