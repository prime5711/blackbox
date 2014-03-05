/** @mainpage OSD CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the  OSD module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given OSD module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular OSD Module. The CSL developer should use his discretion designing
 * new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
 *
 */

/** @file csl_osd.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 1/10/2004
 *    - reason: Created the sources
 *
 *
 * @date 7th Oct, 2004
 * @author Sandeep Tiwari
 *
 */
#ifndef _CSL_OSD_H_
#define _CSL_OSD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_osd.h>

/****** ********************************************************************\
*		OSD global macro declarations
\**************************************************************************/

/** Constants for passing parameters to the OSD Hardware Setup function
 */

/** For indicating that the OSD data i/p format is cb cr  */
#define CSL_OSD_FORMAT_CBCR                          (0)
/** For indicating that the OSD data i/p format is cr cb   */
#define CSL_OSD_FORMAT_CRCB                          (1)
/** For using ROM look up table    */
#define CSL_OSD_LUT_ROM                          (0)
/** For using RAM look up table   */
#define CSL_OSD_LUT_RAM                          (1)
/** configure as field mode  */
#define CSL_OSD_FIELD_MODE                       (0)
/** configure as frame mode   */
#define CSL_OSD_FRAME_MODE                       (1)
/** configure as no zoom   */
#define CSL_OSD_NO_ZOOM                         (0)
/** configure as 1x zoom   */
#define CSL_OSD_ZOOM_1X                         (0)
/** configure as 2x zoom   */
#define CSL_OSD_ZOOM_2X                         (1)
/** configure as 4X zoom   */
#define CSL_OSD_ZOOM_4X                         (2)
/** bitmap mode   */
#define CSL_OSD_BMP                             (0)
/** RGB 16 bit mode (R-5,G-6,B-5) */
#define CSL_OSD_RGB_16BIT                       (1)
/** bitmap is 1-bit mode   */
#define CSL_OSD_BMP_1BIT                        (0) 
/** bitmap is 2-bit mode   */
#define CSL_OSD_BMP_2BIT                        (1)      
/** bitmap mode is 4-bit mode  */
#define CSL_OSD_BMP_4BIT                        (2)
/** bitmap mode is 8-bit mode  */
#define CSL_OSD_BMP_8BIT                        (3) 

/** Parameters used in selection of window */

/** OSD Video Window 0 ID */
#define CSL_OSD_VID_WIN0                    (0)			
/** OSD Video Window 1 ID */
#define	CSL_OSD_VID_WIN1                    (1)
/** OSD Bitmap Window 0 ID */
#define CSL_OSD_BMP_WIN0                    (2)
/** OSD Bitmap Window 1 ID */
#define CSL_OSD_BMP_WIN1		    (3)
/** OSD Hardware Cursor Window ID */
#define CSL_OSD_CURSOR			    (4)
/** Number of Hardware Windows Supported by OSD */
#define CSL_OSD_WIN_MAX  		(5)


    
/**************************************************************************\
* OSD global typedef declarations
\**************************************************************************/

/** @brief This object contains the reference to the instance of OSD opened
 *  using the @a CSL_osdOpen().
 *
 *  The pointer to this, is passed to all OSD CSL APIs.
 */
typedef struct CSL_OsdObj {
	/** This is the mode which the CSL instance is opened     */
	CSL_OpenMode openMode;
	/** This is a unique identifier to the instance of OSD being
	 *  referred to by this object
	 */
	CSL_Uid uid;
	/** This is the variable that contains the current state of a
	 *  resource being shared by current instance of OSD with
     *  other peripherals
     */
	CSL_Xio xio;
	/** This is a pointer to the registers of the instance of OSD
     *  referred to by this object
     */
	CSL_OsdRegsOvly regs;
	/** This is the instance of OSD being referred to by this object  */
	CSL_OsdNum perNum;
}CSL_OsdObj;

typedef struct CSL_OsdObj *CSL_OsdHandle;


/** @brief Prototype for OSD Hw setup
*/

typedef struct CSL_OsdHwSetup_{ 
        /** Background CLUT Selection: 0==>RAM, 1==>ROM */
        Uint16 bclut;
        /** CLUT RAM for Y */
        Uint16 y[256];
        /** CLUT RAM for Cb */
        Uint16 cb[256];
        /** CLUT RAM for Cr */
        Uint16 cr[256];
        /** Base Pixel X */
        Uint16 basepx;
        /** Base Pixel Y */
        Uint16 basepy; 

}CSL_OsdHwSetup;

/** @brief Enumeration for queries passed to @a CSL_osdGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of OSD.
 */

typedef enum {
      
	 /** Get OSD window data address (response type: @a (CSL_GetWinAddr *)) */ 
         CSL_OSD_QUERY_WIN_ADDR=1,
			 
         /** Get OSD video window 0, ping pong buffer address 
             (response type: @a (Uint32*) */ 
         CSL_OSD_QUERY_PINGPONG_BUFFER_ADDR,
         
         /** Get the busy status of the CLUT (response type: @a (Uint16*)) */
         CSL_OSD_QUERY_CLUT_BUSY
         
	      
} CSL_OsdHwStatusQuery;


/** @brief Enumeration for queries passed to @a CSL_osdHwControl()
 *
 * This is used to select the commands to control the operations
 * existing setup of OSD. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration.
 */
 typedef enum {
         
         /** Sets parameters which will affect all OSD windows:
             argument @a (CSL_OsdModeData *) */ 
         CSL_OSD_CMD_SET_ALL_WIN=1,
         
         /** Sets parameters which will affect both OSD video windows:
             argument @a (CSL_OsdVideoModeData *) */ 
         CSL_OSD_CMD_SET_VIDEO_WIN,
         
         /** Sets parameters which will affect both OSD bitmap window:
             argument @a (CSL_OsdBitmapModeData *) */ 
         CSL_OSD_CMD_SET_BITMAP_WIN,

         /** Configures OSD window 'WINID': argument @a (CSL_OsdWinConfig *) */                 
         CSL_OSD_CMD_SET_WIN_CONFIG,

         /** Configures parameters specific to OSD bitmap windows:
             argument @a (CSL_OsdBmpWinConfig *) */       
         CSL_OSD_CMD_SET_BMP_WIN_CONFIG,
         
         /** Set rectangular cursor parameters: argument @a (CSL_OsdCursorConfig *) */       
         CSL_OSD_CMD_SET_CURSOR_CONFIG,
         
         /** Sets OSD window start position: argument @a (CSL_OsdStart*) */
         CSL_OSD_CMD_WIN_START,
            
         /** Sets OSD window horizontal & vertical size:
             argument @a (CSL_OsdSize*) */       
         CSL_OSD_CMD_WIN_SIZE,

         /** Enable(show)/disable(hide) OSD window: argument @a (CSL_OsdWinEnable *) */                        
         CSL_OSD_CMD_OSDWIN_SHOW,

         /** sets OSD window data address: argument @a (CSL_OsdWinAddr*) */             
         CSL_OSD_CMD_SET_WIN_ADDR,
         
         /** sets OSD LUT in RAM: argument @a (CSL_OsdRamLut*) */
         CSL_OSD_CMD_SET_RAM_LUT,
         
         /** Sets OSD video window 0 ping pong buffer address:
             argument @a (CSL_OsdPingAddr *) */
         CSL_OSD_CMD_SET_PINGPONG_BUFFER_ADDR,
                    
         /** Switches OSD video window 0 address:
             argument @a (CSL_OsdPingBuff *) */
         CSL_OSD_CMD_SWITCH_TO_PINGPONG,
         
         /** Sets OSD 1,2,4-bit bitmap color pallette: 
             argument @a (CSL_OsdBmpColor *) */                 
         CSL_OSD_CMD_SET_BMP_COLOR

              
  } CSL_OsdHwControlCmd;

 /**
  * @brief Set OSD bitmap window mode parameters
  * @ this routine set parameters which will affect both OSD bitmap windows
  */

 typedef struct CSL_OsdBitmapModeData {
	 /** Vertical Expansion enable */
	 Uint16 vertexpand;
	 /** Horizontal expansion enable */
	 Uint16 horzexpand;

 }CSL_OsdBitmapModeData;

 /**
  * @brief Set OSD 1,2,4-bit bitmap color pallatte
  */

 typedef struct CSL_OsdBmpColor {

	  /** Window identifier */
	  Uint16 winID; 
	  /** Bitmap width */
	  Uint16 bmpwidth;
	  /** Bitmap value */
	  Uint16 bmpvalue;
	  /** LUT colot */
	  Uint16 lutcolor;

 }CSL_OsdBmpColor;

 /**
  * @brief sets OSD bitmap window configuration
  */

 typedef struct CSL_OsdBmpWinConfig {
	  /** Window identifier */
	  Uint16 winID;
	  /** RGB input select: 0==> Bitmap input, 1==> 16-bit RGB mode */
	  Uint16 inputMode;
	  /** CLUT select: 0==> ROM look-up table, 1==> RAM look-up table */
	  Uint16 selectLut;
	  /** Bitmap bit width */
	  Uint16 bitMapWidth;
	  /** Blending ratio */
	  Uint16 blendRatio;
	  /** Transparency enable: 0==> Disable, 1==> Enable */
	  Uint16 transparencyEnable;
	  /** Window 1 attribute: 0==> OSD window 1, 1==> Attribute window */
	  Uint16 attrWinModeEnable;
	  /** Bitmap bit width OASW = 0 */
	  Uint16 attrWinBlinkInterval;

 }CSL_OsdBmpWinConfig;

 /**
  * @brief set OSD cursor configuration
  */
 typedef struct CSL_OsdCursorConfig {
        /**rectangular cursor horizontal line width */
	 Uint16 pixelWidthH;
	/**rectangular cursor vertical line width */
	 Uint16 pixelHeightV;
	/**CLUT select:0==>ROM-look-up-table,1==>RAM-look-up-table*/
	Uint16 selectlut;
	/**rectangular cursor color palette address */
	Uint16 color;

 }CSL_OsdCursorConfig;

 /**
  *  @brief   Set all OSD window parameters
  *  @ All parameters which affect all OSD windows.
  */

 typedef struct CSL_OsdModeData {
           /** Selects the Cb/Cr or Cr/Cb format */
	    Uint16 cbcr;
	    /** Background color LUT select */	   
	    Uint16 lut;
	    /** Background color */	
	    Uint16 backgrcolor;
	    /** Field signal inversion select */	
	    Uint16 fieldsgInv;
	    /** Base Pixel in X */	
	    Uint16 basepx;
	    /** Base Pixel (Line) in Y */	
           Uint16 basepy;       
	    /** OSD clock select: 0==> VENC clk, 1==>VENC clk/2 */
	    Uint16 osdclk;	

 }CSL_OsdModeData ;

 /**
   *  @brief Set OSD video window ,ping pong buffer address
   */

 typedef struct CSL_OsdPingAddr {
         /** Ping Pong buffer address */	  
	  Uint32 address;

 }CSL_OsdPingAddr;

 /**
   *  @brief Setup OSD LUT in RAM
   */

 typedef struct CSL_OsdRamLut {
        /** Entry number in LUT */
        Uint16 entryNum;
	    /** LUT entries */	   
	    Uint16  lut[3]; 

 }CSL_OsdRamLut;

 /**
  * @brief Set OSD video window mode parameters
  * @ this routine sets parameters which will affect both OSD video
  *  windows
  */

 typedef struct CSL_OsdVideoModeData {

	 /** Expansion filter enable: 0==>Off, 1==>On */
	  Uint16 expfilter;
	 /** Video window vertical expansion enable: 0==> x1, 1==> x 6/5 */
	  Uint16 vertexpand;
	 /** Video window horizontal expansion enable: 0==> x1, 1==> x 9/8 */ 
	  Uint16 horzexpand;
	 /** Video window 0/1 expansion filter coeff. inverse: 0==> Normal,
	        1==> Inverse */
	  Uint16 expnfiltcoeff;
	 /** Video window RGB mode enable: 0==>Disable, 1==>Enable */
	  Uint16 rgben;
	 /** Video window select for RGB: 0==>Video Window 0, 1==> Video Window 1 */
	 Uint16 rgbwin;

 }CSL_OsdVideoModeData;

 /**
   *  @brief Set OSD window data address
   */

 typedef struct CSL_OsdWinAddr {

        /** Window identifier */
	 Uint16 winID;
	 /** OSD window data address */	
	 Uint32 address;			

 }CSL_OsdWinAddr;

 /**
   *  @brief set OSD window configuration	     
   */
 
 typedef struct CSL_OsdWinConfig {
 
	/**	window offset address */  
       Uint16 offset;
	/**window identifier */   
	Uint16  winID;
	/** Window Display mode: 0==> Field mode, 1==> Frame mode */
	Uint16  displaymode;
	/** Horizontal direction zoom */
	Uint16  hzoom;
	/** Vertical direction zoom */
	Uint16  vzoom;

 }CSL_OsdWinConfig;

 /**
   *  @brief  Set OSD window horizontal, vertical Size
   */

 typedef struct CSL_OsdSize {

        /**  window ID - identifies window */
	 Uint16  winID;
	 /** Window width */	
	 Uint16 width;
	 /** Window height */
	 Uint16 height;

 }CSL_OsdSize;

 /**
  *   @brief Set OSD window start position
  */

 typedef struct CSL_OsdStart {
              /** window identifier */
	        Uint16  winID;
	       /** windowX starting position */		  
		Uint16 startX;
	       /** window Y starting position */	
		Uint16 startY;

 }CSL_OsdStart;

 /**
   *  @brief Ping Pong Buffer toggle select structure
   */

 typedef struct CSL_OsdPingBuff {

	   /** Ping Pong buffer toggle select */
	   Uint16 pingPongBuff;

 }CSL_OsdPingBuff;

 /**
   *  @brief Enable/ disable OSD window
   */

 typedef struct CSL_OsdWinEnable {
	   /** Set image display for Video window 0 */  	  
	    Uint16 v0Enable;
	    /** Set image display for Video window 1 */  	  
	    Uint16 v1Enable;
	    /** OSD Window 0 active */	
	    Uint16 o0Enable;
	    /** OSD Window 1 active */		
	    Uint16 o1Enable;
	    /** Rectangular cursor active */	
	    Uint16 rectCurEnable;

 }CSL_OsdWinEnable;

 /**
   * @brief Get OSD window data address
   * return realtive SDRAM address
   */
 
 typedef struct CSL_GetWinAddr {
         /** Window identifier */
	  Uint16  winID;
          /** Windows address */
         Uint32  winAddr;		  

 }CSL_GetWinAddr;


 /**************************************************************************\
* OSD global function declarations
\**************************************************************************/

/**  This function is idempotent in that calling it many times is same as
 *   calling it once. This function initializes the OSD CSL data structures.
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
   if (CSL_SOK != CSL_osdInit()) {
       return;
   }
   @endverbatim
 *
 * @return returns the status of the operation
 *
 */
CSL_Status  CSL_osdInit(
   void
);


/** The open call sets up the data structures for the particular instance of
 *  OSD device. The device can be re-opened anytime after it has been normally
 *  closed if so required. OSD Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as osd must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_osdInit() before calling this
 *  function. Memory for the @a CSL_osdObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

	  CSL_OsdObj     osdObj;
	  CSL_OsdHwSetup osdSetup;
	  CSL_Status       status;
 		...
	  hOsd = CSL_osdOpen(&osdObj,
                          CSL_OSD_0,
                          CSL_EXCLUSIVE,
                          &osdSetup,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_OsdHandle to the requested instance of
 * OSD if the call is successful, otherwise, a @a NULL is returned.
 *
 */
CSL_OsdHandle CSL_osdOpen (
    /** Pointer to the object that holds reference to the
     *  instance of OSD requested after the call
     */
    CSL_OsdObj              *hOsdObj,
    /** Instance of OSD to which a handle is requested
     */
    CSL_OsdNum               osdNum,
    /** Specifies if OSD should be opened with exclusive or
     *  shared access to the associate pins
     */
    CSL_OpenMode                openMode,
    /** This returns the status (success/errors) of the call
     */
    CSL_Status                  *status
);

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_osdInit() and @a CSL_osdOpen() must be called successfully
 * in that order before @a CSL_osdClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_OsdHandle hOsd;
   ...
   CSL_osdClose(hOsd);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_osdClose(
    /** Pointer to the object that holds reference to the
     *  instance of OSD requested after the call
     */
    CSL_OsdHandle                         hOsd
);


/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_OsdHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_osdInit() and @a CSL_osdOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_OsdHandle hOsd;
     CSL_OsdHwSetup hwSetup = CSL_OSD_HWSETUP_DEFAULTS;
     CSL_osdHwSetup(hOsd, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_osdHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of OSD requested after the call
     */
    CSL_OsdHandle                         hOsd,
    /** Pointer to setup structure which contains the
     *  information to program OSD to a useful state
     */
    CSL_OsdHwSetup                        *setup
);



/** Control operations for the OSD.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_OsdHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_osdInit() and @a CSL_osdOpen() must be called successfully
 *  in that order before @a CSL_osdHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_OsdHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_OsdHandle hOsd;
       CSL_Status status;
       ...
       status = CSL_osdHwControl(hOsd,
                                    CSL_OSD_CMD_START,
                                   &command);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_osdHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of OSD requested after the call
     */
    CSL_OsdHandle                        hOsd,
    /** The command to this API indicates the action to be taken
     */
    CSL_OsdHwControlCmd                  cmd,
    /** An optional argument @a void* casted
     */
    void                                 *arg
);


/** This function is used to read the current device configuration, status flags
 *  and the value present associated registers. Following table details the various
 *  status queries supported and the associated data structure to record the response.
 *  User should allocate memory for the said data type and pass its pointer as an
 *  unadorned void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the response,
 *  refer to @a CSL_OsdHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_osdInit() and @a CSL_osdOpen() must be called successfully
 *  in that order before @a CSL_osdGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_OsdHwStatusQuery
 *
 * @b Example:
 * @verbatim
	  CSL_OsdHandle hOsd;
	  CSL_Status status;
	  Uint16  *response;
	   ...
	  status = CSL_osdGetHwStatus(hOsd,
                           CSL_OSD_QUERY_BUS_BUSY,
                                  &response);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_osdGetHwStatus(
    /** Pointer to the object that holds reference to the
     *  instance of OSD requested after the call
     */
    CSL_OsdHandle                         hOsd,
    /** The query to this API which indicates the status
     *  to be returned
     */
    CSL_OsdHwStatusQuery                  query,
    /** Placeholder to return the status. @a void* casted */
    void                                    *response
);

#ifdef __cplusplus
}
#endif


#endif





