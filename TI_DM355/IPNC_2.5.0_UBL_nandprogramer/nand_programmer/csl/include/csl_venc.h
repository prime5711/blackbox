/** @mainpage VENC CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the VENC module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given VENC module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular VENC Module. The CSL developer should use his discretion designing
 * new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
 *    -# VENC_SPEC, VENC Module Specifications DocumentVersion 1.00.w.04  
 *
 */

/** @file csl_venc.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 10/3/2004
 *    - reason: Created the sources
 *
 *
 * @date 3rd October, 2004
 * @author Santosh Narayanan.
 *
 */
#ifndef _CSL_VENC_H_
#define _CSL_VENC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_venc.h>

/**************************************************************************\
* VENC global macro declarations
\**************************************************************************/

/** Constants for passing parameters to the VENC HwSetup function.
 */

/** Analog mode video */
#define CSL_VENC_ANALOG_MODE     (0)
/** Digital mode video */
#define CSL_VENC_DIGITAL_MODE     (1)
/** Mixed mode - both analog as well as digital output mode */
#define CSL_VENC_MIXED_MODE        (2)

/**************************************************************************\
* VENC global typedef declarations
\**************************************************************************/

/** @brief This object contains the reference to the instance of VENC opened
 *  using the @a CSL_vencOpen().
 *
 *  The pointer to this, is passed to all VENC CSL APIs.
 */
typedef struct CSL_VencObj {
	/** This is the mode which the CSL instance is opened     */
	CSL_OpenMode openMode;
	/** This is a unique identifier to the instance of VENC being
	 *  referred to by this object
	 */
	CSL_Uid uid;
	/** This is the variable that contains the current state of a
	 *  resource being shared by current instance of VENC with
     *  other peripherals
     */
	CSL_Xio xio;
	/** This is a pointer to the registers of the instance of VENC
     *  referred to by this object
     */
	CSL_VencRegsOvly regs;
	/** This is the instance of VENC being referred to by this object  */
	CSL_VencNum perNum;
}CSL_VencObj;

typedef struct CSL_VencObj *CSL_VencHandle;

/** @brief Analog Video Mode Basic Configuration structure
*
* All the basic fields needed for Analog Video mode configuration.
 */
typedef struct CSL_VencAnalogVideoConfig_{
       /** Scan mode: 0==> Interlace, 1==> Non-interlace*/
	Uint16 scanMode;   
	/** Non-interlace line number select */
	Uint16 itlcl;
	/** Non-standard interlace mode: 0==>Progressive, 1==>Interlace */
	Uint16 nsit;
	/** Blanking mode: 0==> Normal, 1==> Force blanking */
	Uint16 blankMode;
	/** Composite Output Enable: 0==> Fixed L level output, 
	      1==> Normal composite output */
	Uint16 compositeOut;
	/** DAC3 output select */
	Uint16 da3s;
	/** DAC2 output select */
	Uint16 da2s;
	/** DAC1 output select */
	Uint16 da1s;
	/** DAC0 output select */
	Uint16 da0s;
	/* Analog RGB matrix 0 */
	Uint16 analogRGBMat0;	
       /* Analog RGB matrix 0 */
	Uint16 analogRGBMat1;	
       /* Analog RGB matrix 2 */
	Uint16 analogRGBMat2;	
	/* Analog RGB matrix 3 */
	Uint16 analogRGBMat3;	
       /* Analog RGB matrix 4 */
	Uint16 analogRGBMat4;	
   
}CSL_VencAnalogVideoConfig;

/** @brief Analog Video Mode Extended Configuration structure
*
* Additional fields that are useful for Analog Video mode configuration.
 */
typedef struct CSL_VencAnalogVideoExtConfig_{
	/** Video Attribute Data Register 0 */
	Uint16 atr0;
	/** Video Attribute Data Register 1 */
	Uint16 atr1;
	/** Video Attribute Data Register 2 */
	Uint16 atr2;
	/** YOUT DC level */
	Uint16 ylvl;
	/** COUT DC level */
	Uint16 clvl;
	/** Sub-carrier inital phase value */
	Uint16 scsd;
	/* Closed caption default data */
	Uint16 cadf;
	/* Closed caption field select */
	Uint16 capf;
	/* Closed caption data 0 (Odd field) */
	Uint16 cado0;
	/* Closed caption data 1 (Odd field) */
	Uint16 cado1;
	/* Closed caption data 0 (Even field) */
	Uint16 cade0;
	/* Closed caption data 1 (Even field) */
	Uint16 cade1;
}CSL_VencAnalogVideoExtConfig;
	

/** @brief Video Processing structure.
*
* All fields needed for Video Processing for the analog mode.
 */
typedef struct CSL_VencVideoProcConfig_{

	/** Select C Prefilter: 0==>No filter, 1==> 1+1, 2==> 1+2+1 */
	Uint16 pfltc;
	/** Select Y Prefilter: 0==>No filter, 1==> 1+1, 2==> 1+2+1 */
	Uint16 pflty;
	/** Prefilter sampling frequency: 0==>ENC clock/2, 1==>ENC clock */
	Uint16 pfltr;
	/** Color bar type: 0==>75%, 1==>100% */
	Uint16 cbtyp;
	/** Color bar mode: 0==>Normal output, 1==> Color bar output */
	Uint16 cbmd;
	/** Input video attenuation control for RGB */
	Uint16 atrgb;
	/** Input video attenuation control for YCbCr */
	Uint16 atycc;
	/** Input video attenuation control for composite */
	Uint16 atcom;
	/** DAC x2 up-sampling enable */
	Uint16 daups;
	/** C signal up-sampling enable */
	Uint16 cups;
	/** Y signal up-sampling enable */
	Uint16 yups;
	
}CSL_VencVideoProcConfig;


/** @brief Horizontal & Vertical parameter configuration structure.
*
* All fields pertaining to horizontal and vertical sync, interval, start and delay.
 */
typedef struct CSL_VencHorzVertConfig_{

	/** Horizontal sync pulse width */
	Uint16 hspls;
	/** Vertical sync pulse width */
	Uint16 vspls;
	/** Horizontal interval */
	Uint16 hint;
	/** Horizontal valid data start position */
	Uint16 hstart;
	/** Horizontal data valid range */
	Uint16 hvalid;
	/** Vertical interval */
	Uint16 vint;
	/** Vertical valid data start position */
	Uint16 vstart;
	/** Vertical valid start position for even field */
	Uint16 vstarta;
	/** Vertical data valid range */
	Uint16 vvalid;
	/** Output delay of horz. sync signal */
	Uint16 hsdly;
	/** Output delay of vertical sync signal */
	Uint16 vsdly;
}CSL_VencHorzVertConfig;

/** @brief Digtial Video Config structure.
*
* All basic fields needed for digital mode video.
 */
typedef struct CSL_VencDigitalConfig_{

	/** Digital Video output Mode */
       Uint16  videoMode;
	/** VCLK output polarity */
	Uint16  vclkp;
	/** VCLK output enable */
	Uint16  vclke;
	/** VCLK pin output enable */
	Uint16  vclkz;
	/** Digital data output mode */
	Uint16  digDataOpMode;
	/** YOUT/COUT pin output interchange */
	Uint16  ycswap;
	/** YOUT/COUT pin output level */
	Uint16  ycol;
	/** YOUT/COUT pin output mode */
	Uint16  ycomd;
	/** YOUT/COUT pin direction */
	Uint16  ycdir;
	/** Digital RGB Matrix 0 */
	Uint16 digRGBMat0;
	/** Digital RGB Matrix 1 */
	Uint16 digRGBMat1;
	/** Digital RGB Matrix 2 */
	Uint16 digRGBMat2;
	/** Digital RGB Matrix 3 */
	Uint16 digRGBMat3;
	/** Digital RGB Matrix 4 */
	Uint16 digRGBMat4;

}CSL_VencDigitalConfig;

/** @brief YCC Control structure.
*
* All fields needed for YCC control.
 */
typedef struct CSL_VencYccCtlConfig_{

	/** YC output order */
	Uint16 ycp;
	/** REC656 mode */
	Uint16 r656;

}CSL_VencYccCtlConfig;

/** @brief  RGB Control structure.
*
* All fields needed for RGB control.
 */
typedef struct CSL_VencRgbCtlConfig_{

    /** RGB latch setting: 0==>Normal output, 1==> Latch mode */
	Uint16 rgblat;   
	/** Iron man type RGB output: 0==> Normal, 1==> Iron-man type */
	Uint16 ironm;
	/** Swap order of data in Iron Man mode: 0==>Normal, 1==> Data swap */
	Uint16 irswp;
	/** Iron Man 9 bit mode: 0==> 8 bit, 1==> 9 bit */
	Uint16 ir9;
	/** RGB LPF sampling frequency: 0==> ENC clock/2, 1==> ENC clock */
	Uint16 dfltr;
	/** RGB LPF Select: 0==>No filter, 1==> 1+2+1, 2==> 1+2+4+2+1 */
	Uint16 dflts;
	/** RGB output order (Line Id = 0) */
	Uint16 rgbef;
	/** RGB output order (Line Id = 1) */
	Uint16 rgbof;
	/** Upper Clip level for RGB output */
	Uint16 uclip;
	/** Offset level for RGB output */
	Uint16 ofst;

}CSL_VencRgbCtlConfig;

/** @brief  SYNC Control structure.
*
* All fields needed for SYNC control.
 */
typedef struct CSL_VencSyncCtlConfig_{

    /** OSD Vsync delay: 0==> Not delay, 1==>Delay 0.5H */
	Uint16 ovd;   
	/** External field detection mode */
	Uint16 exfmd;
	/** External field input inversion: 0==> non-inverse, 1==> inverse */
	Uint16 exfiv;
	/** External sync select: 0==> HSYNC/VSYNC pin, 1==> CCD sync signal */
	Uint16 exsync;
	/** External vertical sync input polarity: 0==>Active H, 1==> Active L */
	Uint16 exviv;
	/** External horx. sync input polarity: 0==>Active H, 1==> Active L */
	Uint16 exhiv;
	/** Composite signal output polarity: 0==> Active H, 1==> Active L */
	Uint16 csp;
	/** Composite signal output enable: 0==> Off, 1==> On */
	Uint16 cse;
	/** Output sync select: 0==>Normal, 1==> Sync pulse width proc. mode */
	Uint16 sysw;
	/** Vertical sync output signal: 0==> Vertical sync signal, 
	      1==> Composite sync signal */
	Uint16 vsyncs;
	/** Vertical sync output polarity: 0==> Active H, 1==> Active L */
	Uint16 vpl;
	/** Horizontal sync output polarity: 0==> Active H, 1==> Active L */
	Uint16 hpl;
	/** Vertical Sync ouput enable: 0==> Off, 1==> On */
	Uint16 syev;
	/** Horizontal Sync ouput enable: 0==> Off, 1==> On */
	Uint16 syeh;
	/** Horz/Vert sync I/O control */
	Uint16 sydir;

}CSL_VencSyncCtlConfig;

/** @brief  LINE Control structure.
*
* All fields needed for LINE control.
 */
typedef struct CSL_VencLineCtlConfig_{

	/** External Line Id signal polarity: 0==> Non-inverse, 1==>Inverse */
	Uint16 exidp;
	/** External Line Id signal input mode: 0==> Internal Id line,
	      1==> External Id line */
	Uint16 exide;
	/** Vertical data valid start position mode: 0==> Normal mode, 
	1==> Field mode */
	Uint16 vstf;
	/** Vertical culling line position */
	Uint16 vclid;
	/** Vertical culling counter reset mode: 0==> Reset to 0, 
	      1==> Reset to Random value */
	Uint16 vclrd;
	/** Digital output vertical culling: 0==> No culling, 1==>5/6 culling */
	Uint16 vcl56;
	/** Digital output field hold: 0==>Normal, 1==>Output hold */
	Uint16 hldf;
	/** Digital output line hold: 0==>Normal, 1==>Output hold */
	Uint16 hldl;
	/** Start line Id control in even field: 0==> Line Id=0, 1==> Line Id=1 */
	Uint16 linid;
	/** DCLK pattern switching by culling line Id: 0==>Off, 1==>On */
	Uint16 dckclp;
	/** DCLK polarity inversion by culling line Id: 0==>Off, 1==>On */
	Uint16 dckcli;
	/** RGB output order switching by culling line Id: 0==>Off, 1==>On */
	Uint16 rgbcl;
	/** Culling line Id toggle position (Odd field) */
	Uint16 clof;
	/** Culling line Id toggle position (even field) */
	Uint16 clef;
	/** Culling line Id inversion  interval */
	Uint16 culi;
	/** Horizontal valid culling mode: 0==>Normal, 1==> Horz. culling mode */
	Uint16 hcm;
	/** Horizontal valid culling pattern bit width */
	Uint16 hcpw;
	/** Horizontal valid culling pattern */
	Uint16 hcpt;

}CSL_VencLineCtlConfig;

/** @brief  LCD output config structure.
*
* All fields needed for LCD output configuration.
 */
typedef struct CSL_VencLcdOutConfig_{

	/** GIO39 output signal select: 0==>Field Id, 1==>LCD_OE */
	Uint16 fids;
	/** Field Id output polarity: 0==>Non-inverse, 1==> Inverse */
	Uint16 fidp;
	/** PWM output pulse polarity: 0==>Active H, 1==>Active L */
	Uint16 pwmp;
	/** PWM output control: 0==>Off, 1==>On */
	Uint16 pwme;
	/** LCD AC output control: 0==>Off, 1==>On */
	Uint16 ace;
	/** Bright output polarity: 0==>Non-inverse, 1==>Inverse */
	Uint16 brp;
	/** Bright output control: 0==>Off, 1==>On */
	Uint16 bre;
	/** LCD OE output polarity: 0==>Active H, 1==>Active L */
	Uint16 oep;
	/** LCD OE output polarity */
	Uint16 oee;
	/** Bright pulse start position */
	Uint16 brts;
	/** Bright pulse width */
	Uint16 brtw;
	/** LCD AC toggle interval */
	Uint16 actf;
	/** LCD AC toggle horizontal position */
	Uint16 acth;
	/** PWM output period */
	Uint16 pwmop;
	/** PWM output pulse width */
	Uint16 pwmw;

}CSL_VencLcdOutConfig;

/** @brief  DCLK config structure.
*
* All fields needed for DCLK configuration.
 */
typedef struct CSL_VencDclkConfig_{

       /** Internal DCLK mode */
	Uint16 dckim;	
	/** DCLK output offset */
	Uint16 dofst;
	/** DCLK pattern mode: 0==>Level, 1==> Enable */
	Uint16 dckec;
	/** DCLK mask control: 0==>Off, 1==>On */
	Uint16 dckme;
	/** DCLK output divide: 0==>Divide by 1, 1==>Divide by 2 */
	Uint16 dckoh;
	/** Internal DCLK output divide: 0==>Divide by 1, 1==>Divide by 2 */
	Uint16 dckih;
	/** DCLK pattern valid bit width */
	Uint16 dckpw;
	/** DCLK pattern 0 */
	Uint16 dclkptn0;
	/** DCLK pattern 1 */
	Uint16 dclkptn1;
	/** DCLK pattern 2 */
	Uint16 dclkptn2;
	/** DCLK pattern 3 */
	Uint16 dclkptn3;
       /** DCLK pattern 0 (auxiliary) */
	Uint16 dclkptn0a;
	/** DCLK pattern 1 (auxiliary) */
	Uint16 dclkptn1a;
	/** DCLK pattern 2 (auxiliary) */
	Uint16 dclkptn2a;
	/** DCLK pattern 3 (auxiliary) */
	Uint16 dclkptn3a;
       /** Horizontal DCLK mask start position */
	Uint16 dchs;
	/** Horizontal DCLK (auxiliary) mask start position */
	Uint16 dchsa;   
	/** Horizontal DCLK mask range */
	Uint16 dclkhr;
	/** DCLK vertical mask start position */
	Uint16 dclkvs;
	/** DCLK vertical mask range */
	Uint16 dcvr;

}CSL_VencDclkConfig;

/** @brief  EPSON LCD config structure.
*
* All fields needed for EPSON LCD configuration.
 */
typedef struct CSL_VencEpsonLcdConfig_{

	/** Low pulse number of YSCL: 0==>one pulse, 1==>two pulses */
	Uint16 yscls;
	/** XINH signal select: 0==>DC output, 1==>TG controlled */
	Uint16 xinhs;
	/** XINH DC level: 0==>L level output, 1==>H level output */
	Uint16 xinhl;
	/** SYS function applied field ID select */
	Uint16 sysfid;
	/** SYS function enable: 0==>Off, 1==>On */
	Uint16 sysse;
	/** GCP pattern select */
	Uint16 gcps;

}CSL_VencEpsonLcdConfig;

/** @brief  CASIO LCD config structure.
*
* All fields needed for CASIO LCD configuration.
 */
typedef struct CSL_VencCasioLcdConfig_{

	/** RIT port control: 0==>L level output, 1==>H level output */
	Uint16 crit;
	/** STBYB port control: 0==> L level output, 1==> H level output */
	Uint16 cstb;
	/** GRES port control: 0==>Normal, 1==>Fixed L level */
	Uint16 cgres;
	/** Display mode: 0==>Normal, 1==>Vertical reverse mode */
	Uint16 cdm;

}CSL_VencCasioLcdConfig;

/** @brief  UDISP LCD config structure.
*
* All fields needed for UDISP LCD configuration.
 */
typedef struct CSL_VencUdispLcdConfig_{

	/**Frame start pulse width */
	Uint16 fsw;

}CSL_VencUdispLcdConfig;

/** @brief  STN LCD config structure.
*
* All fields needed for STN LCD configuration.
 */
typedef struct CSL_VencStnLcdConfig_{

      /** FRC table rotation enable */
	Uint16 frcrot;  
	/**Frame start pulse width */
	Uint16 sdw;

}CSL_VencStnLcdConfig;

/** @brief  RAM config structure.
*
* All fields needed for RAM configuration.
 */
typedef struct CSL_VencRamConfig_{

	/*GCP/FRC table RAM address */
	Uint16 ramAddr;
	/** RAM data port */
	Uint16 ramPort;

}CSL_VencRamConfig;


/** @brief Composite mode config structure.
*
* All fields needed for composite video mode.
*/

typedef struct CSL_VencCompositeConfig_{

	/** Delay adjustment of Y signal in composite signal */
	Uint16 ycdly;
	/** Composite video level */
	Uint16 cvlvl;
	/** Composite setup: 0==>0%, 1==>7.5% */
	Uint16 cstup;
	/** Blanking shape disable: 0==>Enable, 1==>Disable */
	Uint16 cbls;
	/** Chroma signal low-pass filter select: 0==>1.5MHz cutoff, 1==>3MHz cutoff */
	Uint16 crcut;
	/** Blanking build-up time for composite output */
	Uint16 cbbld;
	/** Sync build-up time for composite output */
	Uint16 csbld;
	/** Equalizing pulse width offset */
	Uint16 cepw;
	/** Field sync pulse width offset */
	Uint16 cfsw;
	/** Line sync pulse width offset */
	Uint16 clsw;
	/** Burst end position offset */
	Uint16 cbse;
	/** Burst start position offset */
	Uint16 cbst;
	/** Front porch position offset */
	Uint16 cfpw;
	/** Line blanking end position offset */
	Uint16 clbi; 

}CSL_VencCompositeConfig;

/** @brief Component mode config structure
*
*  All fields needed for component video mode.
*/

typedef struct CSL_VencComponentConfig_{

      /** RGB select for component output */
      Uint16  mrgb;	  
      /** Delay adjustment of Y signal for component output */
      Uint16  mydly;
      /** Sync on Pr (or R) */
      Uint16  msyr;
      /** Sync on Pb (or B) */
      Uint16  msyb;
      /** Sync on Y (or G) */
      Uint16  msyg;
      /** Chroma level for component YPbPr */
      Uint16  mclvl;
      /** Luma level for component YPbPr */
      Uint16  mylvl;
      /** Setup for component YPbPr */
      Uint16 mstup;
      /** Blanking shape disable */
      Uint16 mbls;
      /** Blanking build up time for component output */
      Uint16 mbbld;
      /** Sync build up time for component output */
      Uint16 msbld;
      /** Equalizing pulse width offset */
      Uint16 mepw;
      /** Field sync pulse width offset */
      Uint16 mfsw;
      /** Line sync pulse width offset */
      Uint16 mlsw;	  
      /** Front porch position offset */
      Uint16 cfpw;
      /** Line blanking end position offset */
      Uint16 clbi;
      	  
}CSL_VencComponentConfig;

/** @brief OSD Config structure
*
* All fields pertaining to OSD configuration.
*/

typedef struct CSL_VencOSDConfig_{

	  /** OSD clock pattern bit width */
	  Uint16 ocpw;
	  /** OSD clock pattern */
	  Uint16 ocpt;
	  /** OSD horizontal sync advance */
	  Uint16 ohad;

}CSL_VencOSDConfig;

/** @brief This has all the fields required to configure VENC at Power Up
 *  (After a Hardware Reset) or a Soft Reset
 *
 *  This structure is used to setup or obtain existing setup of
 *  VENC using @a CSL_vencHwSetup() function.
 */
typedef struct CSL_VencHwSetup_ {

       /** Video output mode: 0==> Analog, 1==> Digital */
	Uint16 videoOutMode;   
	/** Structure for Analog Video mode configuration */
	CSL_VencAnalogVideoConfig *vencAnalogVideoConfig;
	/** Structure for Digital Video mode configuration */
	CSL_VencDigitalConfig *vencDigitalConfig;
}CSL_VencHwSetup;


/** @brief Enumeration for queries passed to @a CSL_vencGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of VENC.
 */
typedef enum {
	 /** Get the VENC digital video mode  (response type: @a Uint16 *) */
	 CSL_VENC_QUERY_VIDEO_MODE = 1,
	 /** Get  info whether configured as master or slave (response type: @a Uint16 *)    */
	 CSL_VENC_QUERY_MASTERSLAVE ,
	 /** Get info whether in HDTV or SDTV mode (response type: @a Uint16 *)  */
	 CSL_VENC_QUERY_HDTV_MODE,
	 /** Get info regarding TV format (response type: @a Uint16 *)  */
	 CSL_VENC_QUERY_TV_FORMAT,
	 /** Get the information regarding video timing(response type: @a Uint16 *)    */
	 CSL_VENC_QUERY_VIDEO_TIMING, 
	 /** Get the video status information (response type: @a (CSL_VencVideoStatus *)) */
	 CSL_VENC_QUERY_VIDEO_STATUS
} CSL_VencHwStatusQuery;

/** @brief Enumeration for queries passed to @a CSL_vencHwControl()
 *
 * This is used to select the commands to control the operations
 * existing setup of VENC. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration.
 */
 typedef enum {
	/** Set the NTSC/PAL mode (TV format) as specified by argument : argument @a (Uint16 *)	 */
	CSL_VENC_CMD_SET_TV_FORMAT = 1,
	/** Set the master or slave mode as specified by argument : argument @a (Uint16 *)	 */
	CSL_VENC_CMD_SET_MASTER_OR_SLAVE ,
	/** Set the video timing mode: argument @a (Uint16 *)	 */
	CSL_VENC_CMD_VIDEO_TIMING ,
	/** Enable the VENC module : no argument */
	CSL_VENC_CMD_ENABLE,
	/** Reset the VENC module : no argument */
	CSL_VENC_CMD_RESET,
	/** Set the HDTV Mode : argument @a (Uint16*) */
	CSL_VENC_CMD_SET_HDTV_MODE,
	/** Set the DAC test mode : argument @a (CSL_VencDacTest *) */
	CSL_VENC_CMD_DAC_TEST

} CSL_VencHwControlCmd;

/** @brief DAC Test structure.
*
* All fields needed for DAC Test.
 */
typedef struct CSL_VencDacTest_{

       /** DAC 3 power-down: 0==>Normal, 1==>Power-down mode */
	Uint16  dapd3;
	/** DAC 2 power-down: 0==>Normal, 1==>Power-down mode */
	Uint16  dapd2;
	/** DAC 1 power-down: 0==>Normal, 1==>Power-down mode */
	Uint16  dapd1;
	/** DAC 0 power-down: 0==>Normal, 1==>Power-down mode */
	Uint16  dapd0;
	/** DAC DC output mode: 0==>Normal, 1==>DC output mode */
	Uint16  dacdc;
	/** DC level control */
	Uint16  dalvl;
	
}CSL_VencDacTest;

/** @brief  Video status structure.
*
* All fields needed for Video status query.
 */
typedef struct CSL_VencVideoStatus_{

	/** Closed caption status (even field) */
	Uint16 caest;
	/** Closed caption status (odd field) */
	Uint16 caost;
	/** Field ID monitor */
	Uint16 fidst;
	/** UDisplay "Balance signal" monitor */
	Uint16 udbal;
	/** UDisplay "Full" signal monitor */
	Uint16 udful;

}CSL_VencVideoStatus;

/**************************************************************************\
* VENC global function declarations
\**************************************************************************/

/**  This function is idempotent in that calling it many times is same as
 *   calling it once. This function initializes the VENC CSL data structures.
 *
 * <b> Usage Constraints: </b>
 * CSL system initialization must be successfully completed by invoking
 * @a CSL_sysInit() before calling this function. This function should be
 * called before using any of the CSL APIs
 *
 * @b Example:
 * @verbatim


   ...
   CSL_sysInit();
   if (CSL_SOK != CSL_vencInit()) {
       return;
   }
   @endverbatim
 *
 * @return returns the status of the operation
 *
 */
CSL_Status  CSL_vencInit(
    void
);


/** The open call sets up the data structures for the particular instance of
 *  VENC device. The device can be re-opened anytime after it has been normally
 *  closed if so required. VENC Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as VENC must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_vencInit() before calling this
 *  function. Memory for the @a CSL_vencObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

	  CSL_VencObj     vencObj;
	  CSL_Status       status;
 		...
	  hVenc = CSL_vencOpen(&vencObj,
                          CSL_VENC_0,
                          CSL_EXCLUSIVE,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_VencHandle to the requested instance of
 * VENC if the call is successful, otherwise, a @a NULL is returned.
 *
 */
CSL_VencHandle CSL_vencOpen (
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencObj              *hVencObj,
    /** Instance of VENC to which a handle is requested
	 */
    CSL_VencNum              vencNum,
    /** Specifies if VENC should be opened with exclusive or
     *  shared access to the associate pins
	 */
    CSL_OpenMode            openMode,
    /** This returns the status (success/errors) of the call
	 */
    CSL_Status              *status
	);

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_vencInit() and @a CSL_vencOpen() must be called successfully
 * in that order before @a CSL_vencClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_VencHandle hVenc;
   ...
   CSL_vencClose(hVenc);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vencClose(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc
);


/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_VencHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencHwSetup hwSetup = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_VencHwSetup(hVenc, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencHwSetup                        *setup
);


/** Control operations for the VENC.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_VencHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called successfully
 *  in that order before @a CSL_vencHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_VencHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_VencHandle hVenc;
       CSL_Status status;
       ...
       status = CSL_vencHwControl(hVenc,
                                    CSL_VENC_CMD_ENABLE,
                                   &command);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vencHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** The command to this API indicates the action to be taken
	 */
    CSL_VencHwControlCmd                   cmd,
    /** An optional argument @a void* casted
	 */
    void                                     *arg
	);

/** This function is used to read the current device configuration, status flags
 *  and the value present associated registers. Following table details the various
 *  status queries supported and the associated data structure to record the response.
 *  User should allocate memory for the said data type and pass its pointer as an
 *  unadorned void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the response,
 *  refer to @a CSL_VencHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called successfully
 *  in that order before @a CSL_vencGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_VencHwStatusQuery
 *
 * @b Example:
 * @verbatim
	  CSL_VencHandle hVenc;
	  CSL_Status status;
	  Uint16  *response;
	   ...
	  status = CSL_vencGetHwStatus(hVenc,
                           CSL_VENC_QUERY_VIDEO_STATUS,
                                  &response);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_vencGetHwStatus(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** The query to this API which indicates the status
     *  to be returned
	 */
    CSL_VencHwStatusQuery                  query,
    /** Placeholder to return the status. @a void* casted */
    void                                    *response
	);

/**This function pertains to the configuration of device registers that  control
 *  the video processing functions like filter parameter selection, delay 
 *  adjustment, attenuation control, and sampling control. This function 
 *  initializes the device registers with the appropriate values
 *  provided through the VideoProcConfig Data structure. 
 *  For information passed through the VideoProcConfig Data structure refer
 *  @a CSL_VencVideoProcConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencVideoProcConfig vencVideoProcConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencVideoProcSetup(hVenc, &vencVideoProcConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencVideoProcSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencVideoProcConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the horizontal and vertical functions like sync pulse width, delay, start 
 *  and interval. This function initializes the device registers with the 
 *  appropriate values provided through the HorzVertConfig Data structure. 
 *  For information passed through the HorzVertConfig Data structure refer
 *  @a CSL_VencHorzVertConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencHorzVertConfig vencHorzVertConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencHorzVertSetup(hVenc, &vencHorzVertConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencHorzVertSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencHorzVertConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the sync functions in SYNCCTL register. This function initializes the 
 *  device registers with the appropriate values provided through the 
 *  SyncCtlConfig Data structure. 
 *  For information passed through the SyncCtlConfig Data structure refer
 *  @a CSL_VencSyncCtlConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencSyncCtlConfig vencSyncCtlConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencSyncCtlSetup(hVenc, &vencSyncCtlConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencSyncCtlSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencSyncCtlConfig  *setup
	);

/**This function pertains to the configuration of device registers that 
 *  configure some additional parameters that are useful in the analog mode
 *  of operation. This function initializes the device registers with the 
 *  appropriate values provided through the vencAnalogExtConfig Data structure.
 *  For information passed through the vencAnalogExtConfig Data structure refer
 *  @a CSL_VencAnalogVideoExtConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencAnalogVideoExtConfig vencAnalogExtConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencAnalogVideoExtSetup(hVenc, &vencAnalogExtConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencAnalogVideoExtSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencAnalogVideoExtConfig *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the YCbCr functions in YCCCTL register. This function initializes the 
 *  device registers with the appropriate values provided through the 
 *  YccCtlConfig Data structure. 
 *  For information passed through the YccCtlConfig Data structure refer
 *  @a CSL_VencYccCtlConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencYccCtlConfig vencYccCtlConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencYccCtlSetup(hVenc, &vencYccCtlConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencYccCtlSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencYccCtlConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the RGB functions in RGBCTL register. This function initializes the 
 *  device registers with the appropriate values provided through the 
 *  RgbCtlConfig Data structure. 
 *  For information passed through the RgbCtlConfig Data structure refer
 *  @a CSL_VencRgbCtlConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencRgbCtlConfig vencRgbCtlConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencRgbCtlSetup(hVenc, &vencRgbCtlConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencRgbCtlSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencRgbCtlConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the line control and culling functions in LINECTL & CULLLINE register. This 
 *  function initializes the device registers with the appropriate values provided
 *  through the LineCtlConfig Data structure. 
 *  For information passed through the LineCtlConfig Data structure refer
 *  @a CSL_VencLineCtlConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencLineCtlConfig vencLineCtlConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencLineCtlSetup(hVenc, &vencLineCtlConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencLineCtlSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencLineCtlConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the LCD Output signal. This function initializes the device registers with 
 *  the appropriate values provided through the LcdOutConfig Data structure. 
 *  For information passed through the LcdOutConfig Data structure refer
 *  @a CSL_VencLcdOutConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencLcdOutConfig vencLcdOutConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencLcdOutSetup(hVenc, &vencLcdOutConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencLcdOutSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencLcdOutConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the DCLK. This function initializes the device registers with 
 *  the appropriate values provided through the DclkConfig Data structure. 
 *  For information passed through the DclkConfig Data structure refer
 *  @a CSL_VencDclkConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencDclkConfig vencDclkConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencDclkSetup(hVenc, &vencDclkConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencDclkSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencDclkConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the EPSON LCD. This function initializes the device registers with 
 *  the appropriate values provided through the EpsonLcdConfig structure. 
 *  For information passed through the EpsonLcdConfig Data structure refer
 *  @a CSL_VencEpsonLcdConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencEpsonLcdConfig vencEpsonLcdConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencEpsonLcdSetup(hVenc, &vencEpsonLcdConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencEpsonLcdSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencEpsonLcdConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the CASIO LCD. This function initializes the device registers with 
 *  the appropriate values provided through the CasioLcdConfig structure. 
 *  For information passed through the CasioLcdConfig Data structure refer
 *  @a CSL_VencCasioLcdConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencCasioLcdConfig vencCasioLcdConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencCasioLcdSetup(hVenc, &vencCasioLcdConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencCasioLcdSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencCasioLcdConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the UDISP LCD. This function initializes the device registers with 
 *  the appropriate values provided through the UdispLcdConfig structure. 
 *  For information passed through the UdispLcdConfig Data structure refer
 *  @a CSL_VencUdispLcdConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencUdispLcdConfig vencUdispLcdConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencUdispLcdSetup(hVenc, &vencUdispLcdConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencUdispLcdSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencUdispLcdConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the STN LCD. This function initializes the device registers with 
 *  the appropriate values provided through the StnLcdConfig structure. 
 *  For information passed through the StnLcdConfig Data structure refer
 *  @a CSL_VencStnLcdConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencStnLcdConfig vencStnLcdConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencStnLcdSetup(hVenc, &vencStnLcdConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencStnLcdSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencStnLcdConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the GCP/FRC table RAM. This function initializes the device registers with 
 *  the appropriate values provided through the RamConfig structure. 
 *  For information passed through the RamConfig Data structure refer
 *  @a CSL_VencRamConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencRamConfig vencRamConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencRamSetup(hVenc, &vencRamConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencRamSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencRamConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the OSD clock. This function initializes the device registers with 
 *  the appropriate values provided through the OSDConfig structure. 
 *  For information passed through the OSDConfig Data structure refer
 *  @a CSL_VencOSDConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencOSDConfig vencOSDConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencOSDSetup(hVenc, &vencOSDConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencOSDSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencOSDConfig  *setup
	);


/**This function pertains to the configuration of device registers that  control
 *  the CVBS mode (composite video mode). This function initializes the device 
 *  registers with the appropriate values provided through the CompositeConfig 
 *  structure. 
 *  For information passed through the CompositeConfig Data structure refer
 *  @a CSL_VencCompositeConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencCompositeConfig vencCompositeConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencCompositeSetup(hVenc, &vencCompositeConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencCompositeSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencCompositeConfig  *setup
	);

/**This function pertains to the configuration of device registers that  control
 *  the component video mode. This function initializes the device  registers with 
 *  the appropriate values provided through the ComponentConfig structure. 
 *  For information passed through the ComponentConfig Data structure refer
 *  @a CSL_VencComponentConfig.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_vencInit() and @a CSL_vencOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_VencHandle hVenc;
     CSL_VencComponentConfig vencComponentConfig = CSL_VENC_HWSETUP_DEFAULTS;
     CSL_vencComponentSetup(hVenc, &vencComponentConfig);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_vencComponentSetup(
    /** Pointer to the object that holds reference to the
     *  instance of VENC requested after the call
	 */
    CSL_VencHandle                         hVenc,
    /** Pointer to setup structure which contains the
     *  information to program VENC to a useful state
	 */
    CSL_VencComponentConfig  *setup
	);


#ifdef __cplusplus
}
#endif


#endif

