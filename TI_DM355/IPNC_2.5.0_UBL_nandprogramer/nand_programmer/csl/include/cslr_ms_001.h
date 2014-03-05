#ifndef _CSLR_MS_1_H_
#define _CSLR_MS_1_H_
/*********************************************************************
 * Copyright (C) 2003-2004 Texas Instruments Incorporated. 
 * All Rights Reserved 
 *********************************************************************/
 /** \file cslr_ms_1.h
 * 
 * \brief This file contains the Register Desciptions for MS
 * 
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 MSCMD;
    volatile Uint32 MSCDAT;
    volatile Uint32 MSSTAT;
    volatile Uint32 MSSYST;
} CSL_MsRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* MSCMD */

#define CSL_MS_MSCMD_TPC_MASK            (0xF0000000u)
#define CSL_MS_MSCMD_TPC_SHIFT           (0x0000001Cu)
#define CSL_MS_MSCMD_TPC_RESETVAL        (0x00000000u)

#define CSL_MS_MSCMD_DSL_MASK            (0x04000000u)
#define CSL_MS_MSCMD_DSL_SHIFT           (0x0000001Au)
#define CSL_MS_MSCMD_DSL_RESETVAL        (0x00000000u)

/*----DSL Tokens----*/
#define CSL_MS_MSCMD_DSL_MSFIFO          (0x00000000u)
#define CSL_MS_MSCMD_DSL_MSEXTMEM        (0x00000001u)

#define CSL_MS_MSCMD_DSZ_MASK            (0x03FF0000u)
#define CSL_MS_MSCMD_DSZ_SHIFT           (0x00000010u)
#define CSL_MS_MSCMD_DSZ_RESETVAL        (0x00000000u)

#define CSL_MS_MSCMD_RESETVAL            (0x00000000u)

/* MSCDAT */

#define CSL_MS_MSCDAT_DATA_MASK          (0xFFFFFFFFu)
#define CSL_MS_MSCDAT_DATA_SHIFT         (0x00000000u)
#define CSL_MS_MSCDAT_DATA_RESETVAL      (0x00000000u)

#define CSL_MS_MSCDAT_RESETVAL           (0x00000000u)

/* MSSTAT */

#define CSL_MS_MSSTAT_DRQ_MASK           (0x40000000u)
#define CSL_MS_MSSTAT_DRQ_SHIFT          (0x0000001Eu)
#define CSL_MS_MSSTAT_DRQ_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_MSINT_MASK         (0x20000000u)
#define CSL_MS_MSSTAT_MSINT_SHIFT        (0x0000001Du)
#define CSL_MS_MSSTAT_MSINT_RESETVAL     (0x00000000u)

#define CSL_MS_MSSTAT_RDY_MASK           (0x10000000u)
#define CSL_MS_MSSTAT_RDY_SHIFT          (0x0000001Cu)
#define CSL_MS_MSSTAT_RDY_RESETVAL       (0x00000001u)

#define CSL_MS_MSSTAT_CRC_MASK           (0x02000000u)
#define CSL_MS_MSSTAT_CRC_SHIFT          (0x00000019u)
#define CSL_MS_MSSTAT_CRC_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_TOE_MASK           (0x01000000u)
#define CSL_MS_MSSTAT_TOE_SHIFT          (0x00000018u)
#define CSL_MS_MSSTAT_TOE_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_EMP_MASK           (0x00200000u)
#define CSL_MS_MSSTAT_EMP_SHIFT          (0x00000015u)
#define CSL_MS_MSSTAT_EMP_RESETVAL       (0x00000001u)

#define CSL_MS_MSSTAT_FUL_MASK           (0x00100000u)
#define CSL_MS_MSSTAT_FUL_SHIFT          (0x00000014u)
#define CSL_MS_MSSTAT_FUL_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_CED_MASK           (0x00080000u)
#define CSL_MS_MSSTAT_CED_SHIFT          (0x00000013u)
#define CSL_MS_MSSTAT_CED_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_ERR_MASK           (0x00040000u)
#define CSL_MS_MSSTAT_ERR_SHIFT          (0x00000012u)
#define CSL_MS_MSSTAT_ERR_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_BRQ_MASK           (0x00020000u)
#define CSL_MS_MSSTAT_BRQ_SHIFT          (0x00000011u)
#define CSL_MS_MSSTAT_BRQ_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_CNK_MASK           (0x00010000u)
#define CSL_MS_MSSTAT_CNK_SHIFT          (0x00000010u)
#define CSL_MS_MSSTAT_CNK_RESETVAL       (0x00000000u)

#define CSL_MS_MSSTAT_RESETVAL           (0x10200000u)

/* MSSYST */

#define CSL_MS_MSSYST_RST_MASK           (0x80000000u)
#define CSL_MS_MSSYST_RST_SHIFT          (0x0000001Fu)
#define CSL_MS_MSSYST_RST_RESETVAL       (0x00000000u)

#define CSL_MS_MSSYST_SRAC_MASK          (0x40000000u)
#define CSL_MS_MSSYST_SRAC_SHIFT         (0x0000001Eu)
#define CSL_MS_MSSYST_SRAC_RESETVAL      (0x00000001u)

#define CSL_MS_MSSYST_INTEN_MASK         (0x20000000u)
#define CSL_MS_MSSYST_INTEN_SHIFT        (0x0000001Du)
#define CSL_MS_MSSYST_INTEN_RESETVAL     (0x00000000u)

#define CSL_MS_MSSYST_NOCRC_MASK         (0x10000000u)
#define CSL_MS_MSSYST_NOCRC_SHIFT        (0x0000001Cu)
#define CSL_MS_MSSYST_NOCRC_RESETVAL     (0x00000000u)

#define CSL_MS_MSSYST_INTCLR_MASK        (0x08000000u)
#define CSL_MS_MSSYST_INTCLR_SHIFT       (0x0000001Bu)
#define CSL_MS_MSSYST_INTCLR_RESETVAL    (0x00000001u)

#define CSL_MS_MSSYST_MSIEN_MASK         (0x04000000u)
#define CSL_MS_MSSYST_MSIEN_SHIFT        (0x0000001Au)
#define CSL_MS_MSSYST_MSIEN_RESETVAL     (0x00000000u)

#define CSL_MS_MSSYST_FCLR_MASK          (0x02000000u)
#define CSL_MS_MSSYST_FCLR_SHIFT         (0x00000019u)
#define CSL_MS_MSSYST_FCLR_RESETVAL      (0x00000000u)

#define CSL_MS_MSSYST_FDIR_MASK          (0x01000000u)
#define CSL_MS_MSSYST_FDIR_SHIFT         (0x00000018u)
#define CSL_MS_MSSYST_FDIR_RESETVAL      (0x00000000u)

#define CSL_MS_MSSYST_DAM_MASK           (0x00800000u)
#define CSL_MS_MSSYST_DAM_SHIFT          (0x00000017u)
#define CSL_MS_MSSYST_DAM_RESETVAL       (0x00000000u)

#define CSL_MS_MSSYST_DRM_MASK           (0x00400000u)
#define CSL_MS_MSSYST_DRM_SHIFT          (0x00000016u)
#define CSL_MS_MSSYST_DRM_RESETVAL       (0x00000000u)

#define CSL_MS_MSSYST_DRQSL_MASK         (0x00200000u)
#define CSL_MS_MSSYST_DRQSL_SHIFT        (0x00000015u)
#define CSL_MS_MSSYST_DRQSL_RESETVAL     (0x00000000u)

#define CSL_MS_MSSYST_REI_MASK           (0x00100000u)
#define CSL_MS_MSSYST_REI_SHIFT          (0x00000014u)
#define CSL_MS_MSSYST_REI_RESETVAL       (0x00000001u)

#define CSL_MS_MSSYST_REO_MASK           (0x00080000u)
#define CSL_MS_MSSYST_REO_SHIFT          (0x00000013u)
#define CSL_MS_MSSYST_REO_RESETVAL       (0x00000000u)

#define CSL_MS_MSSYST_BSY_MASK           (0x00070000u)
#define CSL_MS_MSSYST_BSY_SHIFT          (0x00000010u)
#define CSL_MS_MSSYST_BSY_RESETVAL       (0x00000005u)

#define CSL_MS_MSSYST_RESETVAL           (0x48150000u)

#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 5         14 Jan 2005 13:32:32    5888             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL0.57 JAN 2005 Release and built the library for ARM and DSP*/
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:47:52    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
