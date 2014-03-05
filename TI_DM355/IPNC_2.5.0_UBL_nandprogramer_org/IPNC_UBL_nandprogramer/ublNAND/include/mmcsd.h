/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */ 
/*   Created 2006, (C) Copyright 2006 Texas Instruments.  All rights reserved.*/ 
/*																			  */
/*  FILE 	: mmcsd.h													      */
/*	PURPOSE	: MMC/SD driver APIs				                              */
/*  PROJECT	: DM350 UBL boot-loader                                           */
/*  AUTHOR	: Vignesh LA	                                                  */
/*  DATE	: Feb-14-2006                                                     */
/******************************************************************************/

#ifndef _MMCSD_H_
#define _MMCSD_H_

#include <cslr_mmcsd.h>

#define MMCSD_STUFF_BITS          0x00000000u
#define MMCSD_VDD_32_34           0x00300000u
#define MMCSD_VDD_32_33           0x00100000u
#define MMCSD_VDD_27_36           0x00FF8000u
#define MMCSD_BSYEXP		       0x0100

/**
    \brief Card Lock Structure as per MMC/SD Specifications
*/
typedef enum {
  MMCSD_SET_PASSWD=1, 
  MMCSD_CLR_PASSWD=2,
  MMCSD_LOCK_UNLOCK=4, 
  MMCSD_FORCED_ERASE=8
} MMCSD_CARD_LOCK_MODE;

/* Response Macros */
#define MMCSD_RSPNONE                   (0x0000u)
#define MMCSD_RSP1                      (0x0200u)
#define MMCSD_RSP2                      (0x0400u)
#define MMCSD_RSP3                      (0x0600u)
#define MMCSD_RSP4                       MMCSD_RSP1
#define MMCSD_RSP5                       MMCSD_RSP1
#define MMCSD_RSP6                       MMCSD_RSP1

/* Command Macros */
#define MMCSD_CMD0                      (0x0000u)
#define MMCSD_CMD1                      (0x0001u)
#define MMCSD_CMD2                      (0x0002u)
#define MMCSD_CMD3                      (0x0003u)
#define MMCSD_CMD4                      (0x0004u)
#define MMCSD_CMD5                      (0x0005u)
#define MMCSD_CMD6                      (0x0006u)
#define MMCSD_CMD7                      (0x0007u)
#define MMCSD_CMD8                      (0x0008u)
#define MMCSD_CMD9                      (0x0009u)
#define MMCSD_CMD10                     (0x000Au)
#define MMCSD_CMD11                     (0x000Bu)
#define MMCSD_CMD12                     (0x000Cu)
#define MMCSD_CMD13                     (0x000Du)
#define MMCSD_CMD14                     (0x000Eu)
#define MMCSD_CMD15                     (0x000Fu)
#define MMCSD_CMD16                     (0x0010u)
#define MMCSD_CMD17                     (0x0011u)
#define MMCSD_CMD18                     (0x0012u)
#define MMCSD_CMD19                     (0x0013u)
#define MMCSD_CMD20                     (0x0014u)
#define MMCSD_CMD21                     (0x0015u)
#define MMCSD_CMD22                     (0x0016u)
#define MMCSD_CMD23                     (0x0017u)
#define MMCSD_CMD24                     (0x0018u)
#define MMCSD_CMD25                     (0x0019u)
#define MMCSD_CMD26                     (0x001Au)
#define MMCSD_CMD27                     (0x001Bu)
#define MMCSD_CMD28                     (0x001Cu)
#define MMCSD_CMD29                     (0x001Du)
#define MMCSD_CMD30                     (0x001Eu)
#define MMCSD_CMD31                     (0x001Fu)
#define MMCSD_CMD32                     (0x0020u)
#define MMCSD_CMD33                     (0x0021u)
#define MMCSD_CMD34                     (0x0022u)
#define MMCSD_CMD35                     (0x0023u)
#define MMCSD_CMD36                     (0x0024u)
#define MMCSD_CMD37                     (0x0025u)
#define MMCSD_CMD38                     (0x0026u)
#define MMCSD_CMD39                     (0x0027u)
#define MMCSD_CMD40                     (0x0028u)
#define MMCSD_CMD41                     (0x0029u)
#define MMCSD_CMD42                     (0x002Au)
#define MMCSD_CMD43                     (0x002Bu)
#define MMCSD_CMD44                     (0x002Cu)
#define MMCSD_CMD45                     (0x002Du)
#define MMCSD_CMD46                     (0x002Eu)
#define MMCSD_CMD47                     (0x002Fu)
#define MMCSD_CMD48                     (0x0030u)
#define MMCSD_CMD49                     (0x0031u)
#define MMCSD_CMD50                     (0x0032u)
#define MMCSD_CMD51                     (0x0033u)
#define MMCSD_CMD52                     (0x0034u)
#define MMCSD_CMD53                     (0x0035u)
#define MMCSD_CMD54                     (0x0036u)
#define MMCSD_CMD55                     (0x0037u)
#define MMCSD_CMD56                     (0x0038u)
#define MMCSD_CMD57                     (0x0039u)
#define MMCSD_CMD58                     (0x003Au)
#define MMCSD_CMD59                     (0x003Bu)
#define MMCSD_CMD60                     (0x003Cu)
#define MMCSD_CMD61                     (0x003Du)
#define MMCSD_CMD62                     (0x003Eu)
#define MMCSD_CMD63                     (0x003Fu)
#define MMCSD_CMD64                     (0x0040u)

/* Commands and their responses */
/* MMC and SD */
#define MMCSD_GO_IDLE_STATE                   (MMCSD_CMD0 | MMCSD_RSPNONE)

#define MMCSD_ALL_SEND_CID                    (MMCSD_CMD2 | MMCSD_RSP2 )
#define MMCSD_SET_DSR                         (MMCSD_CMD4 | MMCSD_RSPNONE)
#define MMCSD_SELECT_CARD                     (MMCSD_CMD7 | MMCSD_RSP1)
#define MMCSD_DESELECT_CARD                   (MMCSD_CMD7 )
#define MMCSD_SEND_CSD                        (MMCSD_CMD9 | MMCSD_RSP2)
#define MMCSD_SEND_CID                        (MMCSD_CMD10| MMCSD_RSP2)
#define MMCSD_SEND_STATUS                     (MMCSD_CMD13 | MMCSD_RSP1)
#define MMCSD_GO_INACTIVE_STATE               (MMCSD_CMD15 | MMCSD_RSPNONE)
#define MMCSD_APP_CMD                         (MMCSD_CMD55 | MMCSD_RSP1 )
#define MMCSD_STOP_TRANSMISSION               (MMCSD_CMD12 | MMCSD_RSP1 | MMCSD_BSYEXP)
#define MMCSD_READ_MULTIPLE_BLOCK             (MMCSD_CMD18 | MMCSD_RSP1)
#define MMCSD_WRITE_MULTIPLE_BLOCK            (MMCSD_CMD25 | MMCSD_RSP1 ) /*| MMCSD_BSYEXP)*/

/* Common to SPI & MMC */
#define MMCSD_SET_BLOCKLEN                    (MMCSD_CMD16 | MMCSD_RSP1 )
#define MMCSD_PROGRAM_CSD                     (MMCSD_CMD27 | MMCSD_RSP1 | MMCSD_BSYEXP) /* MMC-bsy, SPI-bsy optional */
#define MMCSD_SET_WRITE_PROT                  (MMCSD_CMD28 | MMCSD_RSP1 | MMCSD_BSYEXP)
#define MMCSD_CLR_WRITE_PROT                  (MMCSD_CMD29 | MMCSD_RSP1 | MMCSD_BSYEXP)
#define MMCSD_SEND_WRITE_PROT                 (MMCSD_CMD30 | MMCSD_RSP1)
#define MMCSD_READ_SINGLE_BLOCK               (MMCSD_CMD17 | MMCSD_RSP1 )
#define MMCSD_WRITE_BLOCK                     (MMCSD_CMD24 | MMCSD_RSP1 )/*| MMC_BSYEXP)*/
#define MMCSD_READ_OCR                        (MMCSD_CMD58 | MMCSD_RSP3 )
#define MMCSD_CRC_ON_OFF                      (MMCSD_CMD59 | MMCSD_RSP1)
  
/* Command parameters */
#define MMCCMD_REG_INITCK             (0x4000u)
#define MMCCMD_REG_PPLEN              (0x0080u)
#define MMCSD_PPLEN			       	  (0x0080)
#define MMCSD_STAT0_DATDNE 			  (0x0001)     ///< Data Done Status


#define MMCSD_OP_TIMEOUT           2048
#define MMC_SEND_OP_COND           (MMCSD_CMD1 | MMCSD_RSP3 )
#define MMC_SET_RELATIVE_ADDR      (MMCSD_CMD3 | MMCSD_RSP1)

#define SD_APP_OP_COND             (MMCSD_CMD41 | MMCSD_RSP3 )
#define SD_SEND_RELATIVE_ADDR      (MMCSD_CMD3 | MMCSD_RSP6 | MMCSD_PPLEN)
#define SD_SET_BUS_WIDTH           (MMCSD_CMD6  | MMCSD_RSP1 | MMCSD_PPLEN)

/**
    \brief Card States as per MMC/SD Specifications
*/
typedef enum {
  MMCSD_STATE_IDLE=0, 
  MMCSD_STATE_READY,
  MMCSD_STATE_IDENT, 
  MMCSD_STATE_STBY, 
  MMCSD_STATE_TRAN,
  MMCSD_STATE_DATA, 
  MMCSD_STATE_RCV, 
  MMCSD_STATE_PRG, 
  MMCSD_STATE_DIS,
  MMCSD_STATE_INA
} MMCSD_CARD_STATE;


/**
    \brief Response information received from MMCSD
*/
typedef struct {
  Uint16 response[8];           ///< Response of the command
  Uint8 dataResponse;           ///< Data response
  Uint8 errorToken;             ///< Error Token
  Uint8 commandIdk;             ///< Command Index
} MMCSDResponseData;

/**
    \brief MMC-SD: Native mode Card CSD Register Information, required by the driver
*/
typedef struct {
  Uint8  permWriteProtect;
  Uint8  tmpWriteProtect;
  Uint8  writeBlkPartial;
  Uint16 writeBlkLenBytes;
  Uint8  wpGrpEnable;
  Uint8  wpGrpSize;                ///> Extracting 7 bits: For MMC - 5 bits reqd; For SD - 7 bits reqd. (have to be taken care by user)
  Uint8  dsrImp;
  Uint8  readBlkMisalign;
  Uint8  writeBlkMisalign;
  Uint8  readBlkPartial;
  Uint16 readBlkLenBytes;
  Uint8  sysSpecVersion;        ///> These bits are reserved in the case of SD card
} MMCSDCSDRegInfo;

/**
    \brief MMC-SD: Card Status Register \n
    In native mode, all the fields will be set. \n 
    In SPI mode, the error flags will contain the entire response.
*/
typedef struct {
  Uint8      appSpecific;
  Uint8      ready;
  MMCSD_CARD_STATE  currentState;
  Uint8      eraseReset;
  Uint8      eccDisabled;
  Uint8      wpEraseSkip;
  Uint16     errorFlags;
  Bool       multiMediaCard;
} MMCSDCardStatusReg;

/* Function Prototypes */
void MMCSDClearResponse();
Uint8 MMCSDSendCmd(Uint32 command, Uint32 argument, Bool checkStatus);
Uint8 MMCSDGetResponse();
Uint8 MMCSDInitCard(void);
Uint8 MMCSDSingleBlkRead( Uint32 cardMemAddr, Uint32 *dest, Uint32 blklength);
Uint8 MMCSDMultipleBlkRead( Uint32 cardMemAddr, Uint32 *dest, Uint32 datalength);
Uint8 MMCSDReadNWords(Uint32 *data, Uint32 numofBytes); 
void   MMCSDBoot(Uint8 retry);
#endif /* _MMCSD_H_ */

