/*****************************************************\
 *  Copyright 2004, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/
/*   ==========================================================
 *   @file  csl_edmaAux.h
 *
 *   @path  $(CSLPATH)\src\edma
 *
 *   @desc  EDMA module
 *   Modification : 15th July 2004
 *   Modified function 
 *   CSL_edmaChannelMapParamEntry to default the trigger word to 7
 *
 */
#ifndef _CSL_EDMAAUX_H_
#define _CSL_EDMAAUX_H_

/** 
@addtogroup CSL_EDMA_FUNCTION_CHANNEL_INTERNAL 
@{
*/
//pragma CODE_SECTION (CSL_edmaChannelQueryParamMap, ".text:csl_section:edma");
/*
 * ======================================================
 *   @func   CSL_edmaChannelQueryParamMap
 * ======================================================
 */
/** @brief 
 *	   Queries the parameter Entry associated with the channel
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret  CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** Gets the parameter entry to which he channel is mapped to.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), @a CAL_edmaChannelOpen() must be 
 * called successfully in that order before @a CSL_edmaChannelQueryParamMap() can 
 * be called.  
 *
 * @b Example:
   \code
    // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
    Uint32 paramMap;
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);

    // Channel 0 is opened which is in the global region, hence the shadow Handle
    // is NULL    
    paramMap = CSL_edmaChannelQueryParamMap(edmaHndl);
                            
	\endcode
 * 
 * @return Status of the operation
 * 
 */
CSL_IDEF_INLINE Uint32  CSL_edmaChannelQueryParamMap(
	/**  a handle to the requested channel instance of the specified EDMA
     *   obtained  through @a CSL_edmaOpen() call */
    CSL_EdmaChanHandle         hEdma
    )
{

	Uint32 paramEntry;
	if (hEdma->chaNum  < CSL_EDMA_NUM_DMACH){
#if CSL_EDMA_CHMAPEXIST	
		paramEntry = CSL_FEXT(hEdma->ccregs->DCHMAP[hEdma->chaNum], EDMACC_DCHMAP_PAENTRY);			
#endif		
	} else
		paramEntry = CSL_FEXT(hEdma->ccregs->QCHMAP[hEdma->chaNum - CSL_EDMA_NUM_DMACH], EDMACC_QCHMAP_PAENTRY);
	return paramEntry;
} 
/* top level control internal API */
/*
 * ======================================================
 *   @func   CSL_edmaHwChannelEnable
 * ======================================================
 */
/** @brief 
 *	   Enables the channel
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret  CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API enables the event associated with the Channel.  
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit(). CSL_edmaChannelOpen() must be 
 * called before usage of this API.
 *
 * @b Example:
   \code
    // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
   status = CSL_edmaHwChannelEnable(edmaHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaChannelEnable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaChannelEnable(
    /**  a handle to the EDMA channel instance obtained  through 
         the @a CSL_edmaChannelOpen() call */
    CSL_EdmaChanHandle         hEdma
)
{
	/* Ensure this is a normal channel and not a QDMA channel */
	if (hEdma->region == -1) {
		if (hEdma->chaNum >= CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->QEESR = (1 << (hEdma->chaNum - CSL_EDMA_NUM_DMACH));
		else if (hEdma->chaNum < 32)
			hEdma->ccregs->EESR = (1 << hEdma->chaNum);
		else 
			hEdma->ccregs->EESRH = (1 << (hEdma->chaNum - 32));
	} else {
		if (hEdma->chaNum >= CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->SHADOW[hEdma->region].QEESR = (1 << (hEdma->chaNum - CSL_EDMA_NUM_DMACH));
		else if (hEdma->chaNum < 32)
			hEdma->ccregs->SHADOW[hEdma->region].EESR = (1 << hEdma->chaNum);
		else 
			hEdma->ccregs->SHADOW[hEdma->region].EESRH = (1 << (hEdma->chaNum - 32));
	}
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaHwChannelDisable
 * ======================================================
 */
/** @brief 
 *	   Disables the channel
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret  CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API disabled the event associated with the Channel.  
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit(). CSL_edmaChannelOpen() must be 
 * called before usage of this API.
 *
 * @b Example:
   \code
    // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
   status = CSL_edmaHwChannelDisable(edmaHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaChannelDisable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaChannelDisable(
    /**  a handle to the requested channel instance of the specified EDMA
     *   obtained  through the @a CSL_edmaOpen() call */
    CSL_EdmaChanHandle         hEdma
)
{
	/* Ensure this is a normal channel and not a QDMA channel */
	if (hEdma->region == -1) {
		if (hEdma->chaNum >= CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->QEECR = (1 << (hEdma->chaNum - CSL_EDMA_NUM_DMACH));
		else if (hEdma->chaNum < 32)
			hEdma->ccregs->EECR = (1 << hEdma->chaNum);
		else 
			hEdma->ccregs->EECRH = (1 << (hEdma->chaNum - 32));
	} else {
		if (hEdma->chaNum >= CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->SHADOW[hEdma->region].QEECR = (1 << (hEdma->chaNum - CSL_EDMA_NUM_DMACH));
		else if (hEdma->chaNum < 32)
			hEdma->ccregs->SHADOW[hEdma->region].EECR = (1 << hEdma->chaNum);
		else 
			hEdma->ccregs->SHADOW[hEdma->region].EECRH = (1 << (hEdma->chaNum - 32));
	}
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaChannelEventSet
 * ======================================================
 */
/** @brief 
 *	   Manually triggers an EDMA event by setting the ER register
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API manually set the event for the channel 
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaChannelOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    
     // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
   status = CSL_edmaHwEventSet(edmaHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
 
//#pragma CODE_SECTION (CSL_edmaChannelEventSet, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaChannelEventSet(
    /**  a handle to the requested channel instance of the specified EDMA
     *   obtained  through the @a CSL_edmaOpen() call */
    CSL_EdmaChanHandle         hEdma
)
{		
	if (hEdma->region == -1)
	{
		if (hEdma->chaNum < 32)
			hEdma->ccregs->ESR = (1 << hEdma->chaNum);
		else if (hEdma->chaNum < CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->ESRH = (1 << (hEdma->chaNum - 32));
		else 	
			return CSL_ESYS_INVPARAMS;	
	} else {
		if (hEdma->chaNum < 32)
			hEdma->ccregs->SHADOW[hEdma->region].ESR = (1 << hEdma->chaNum);
		else if (hEdma->chaNum < CSL_EDMA_NUM_DMACH)
			hEdma->ccregs->SHADOW[hEdma->region].ESRH = (1 << (hEdma->chaNum - 32));
		else 	
			return CSL_ESYS_INVPARAMS;	
    }			
	return CSL_SOK;
}
/*
 * ======================================================
 *   @func   CSL_edmaChannelMapParamEntry
 * ======================================================
 */
/** @brief 
 *	   Maps the channel to a given parameter entry
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */  
/** This API maps the channel to paRam entry.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaChannelOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
         
      // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
   status = CSL_edmaChannelMapParamEntry(edmaHndl, 6);
   
   \endcode
 *
 * @return returns CSL status. 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 * By default sets the trigger word to 7.
 *
 */
//#pragma CODE_SECTION (CSL_edmaChannelMapParamEntry, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status CSL_edmaChannelMapParamEntry(
	CSL_EdmaChanHandle hEdma,
	Uint32 paramEntry
)
{
	if (hEdma->chaNum  < CSL_EDMA_NUM_DMACH) {
#if CSL_EDMA_CHMAPEXIST		
		CSL_FINS(hEdma->ccregs->DCHMAP[hEdma->chaNum], EDMACC_DCHMAP_PAENTRY,paramEntry);			
#endif		
	} else
		hEdma->ccregs->QCHMAP[hEdma->chaNum-CSL_EDMA_NUM_DMACH] = CSL_FMK(EDMACC_QCHMAP_PAENTRY,paramEntry) |  CSL_FMK(EDMACC_QCHMAP_TRWORD,7);	
	return CSL_SOK;
}
/*
 * ======================================================
 *   @func   CSL_edmaChannelSetTrigWord
 * ======================================================
 */
/** @brief 
 *	   Sets the triger word 
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API maps the channel to paRam entry.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaChannelOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
     // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
                            
   // Setting the Trigger word to be te last one                                                      
   status = CSL_edmaChannelSetTrigWord(edmaHndl, 7);
   
   \endcode
 *
 * @return returns CSL status. 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaChannelSetTrigWord, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status 	CSL_edmaChannelSetTrigWord(
	CSL_EdmaChanHandle hEdma,
	Uint32		       trigWord
)
{
	if (hEdma->chaNum  < CSL_EDMA_NUM_DMACH)
		return CSL_ESYS_INVPARAMS;		
	else
		CSL_FINS(hEdma->ccregs->QCHMAP[hEdma->chaNum - CSL_EDMA_NUM_DMACH], EDMACC_QCHMAP_TRWORD, trigWord);
	return CSL_SOK;
}
/*
 * ======================================================
 *   @func   CSL_edmaChannelSetEvtQue
 * ======================================================
 */
/** @brief 
 *	   Sets the Channel to the Event Queue specified  
 */
 /* ======================================================
 *   @arg  hCh
 *		Channel Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
	
/** This API maps the channel to specified Event queue
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaChannelOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
       // Initialization

	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    CSL_EdmaHandle     hModHndl;          
    CSL_Status modStatus;
   
    // Initialize
    modStatus = CSL_edmaInit(NULL);
    ...
    // Open Module    
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Open Channel 0
	chParam.regionNum = CSL_EDMA_REGION_GLOBAL;
    chParam.chaNum = CSL_EDMA_CHA0;
    edmaHndl = CSL_edmaChannelOpen(&edmaObj,
                            CSL_EDMA_0,
                            &chParam,
                            &edmaStatus);
   
   // EDMA Chanbnel 0 is submitted to Event queue 3                                                     
   status = CSL_edmaChannelSetEvtQue(edmaHndl, CSL_EDMA_EVT_QUE3);
   
   \endcode
 *
 * @return returns CSL status. 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaChannelSetEvtQue, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status CSL_edmaChannelSetEvtQue(
	CSL_EdmaChanHandle hEdma,
	CSL_EdmaEventQueue evtQue
)
{
	Uint32 _cha, _qNumIndex, _qchMap;
	if (hEdma->chaNum >= CSL_EDMA_NUM_DMACH) {
    /* 	Extracting the Channel Number in case of QDMA */
		_cha = hEdma->chaNum - CSL_EDMA_NUM_DMACH;
    /*  Channel to Event Queue mapping */
    	CSL_FINSR(hEdma->ccregs->QDMAQNUM,_cha*4 + 2,_cha*4,evtQue);
    } else {
    /*  Finding out the relevant DMAQNUM register and the correct bit positions to 
        write into */ 
		_qNumIndex = hEdma->chaNum >> 3;
		_qchMap = hEdma->chaNum - (_qNumIndex * 8);
		CSL_FINSR(hEdma->ccregs->DMAQNUM[_qNumIndex],_qchMap*4+2,_qchMap*4,evtQue);
    }
	return CSL_SOK;
}
/**
@}
*/
/** 
@addtogroup CSL_EDMA_FUNCTION_MODULE_INTERNAL    
@{
*/
/*
 * ======================================================
 *   @func   CSL_edmaGetRevision
 * ======================================================
 */
/** @brief 
 *	   Queries module revision number
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @ret  Uint32 - Revision number
 *
 * ======================================================    
 */       

//pragma CODE_SECTION (CSL_edmaGetRevision, ".text:csl_section:edma");
/** Gets the status of the different operations or the current setup of EDMA 
 *	module.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetRevision() can 
 * be called.  
 * @b Example:
   \code
   CSL_EdmaHandle     edmaHndl;          
   CSL_Status modStatus;
   // Initialize
   
   modStatus = CSL_edmaInit(NULL);
   ...
   
   edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
    revision = CSL_edmaGetRevision(hModHndl);

	\endcode
 * 
 * @return Revision of Module
 * 
 */
CSL_IDEF_INLINE Uint32  CSL_edmaGetRevision(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod
    )
{
	return (hMod->REV);
}
/*
 * ======================================================
 *   @func   CSL_edmaGetEventsMissed
 * ======================================================
 */
/** @brief 
 *	   Queries all the events missed.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  Uint32
 *		Since there may be upto 64 EDMA channels + upto 8 QDMA channels,
 *     this points to an  array of 3, 32 bit elements to hold 
 *     all 64 + 8 event missed status bits. 
 *     missed [0] - holds status from EMR
 *     missed [1] - holds status from EMRH
 *     missed [2] - holds status from QEMR
 *   @ret CSL_Status - CSL_SOK 
 *
 * ======================================================    
 */ 
//#pragma CODE_SECTION (CSL_edmaGetEventsMissed, ".text:csl_section:edma");
/** Gets the status of the missed events.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetEventsMissed() can 
 * be called.  
 * @b Example:
   \code
   CSL_EdmaHandle     edmaHndl;          
   CSL_Status modStatus;
   Uint32 missedEdma,missedEdmaHi,missedQdma;
   // Initialize
   
   modStatus = CSL_edmaInit(NULL);
   ...
   
   edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Get missed events 
    edmaStatus = CSL_edmaGetEventsMissed(hMod, &missedEdma, &missedEdmaHi, 
    &missedQdma);

                            
	\endcode
 * 
 * @return CSL  Status
 * 
 */
CSL_IDEF_INLINE CSL_Status  CSL_edmaGetEventsMissed(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
	/**  a place holder for 0-31 events */
    CSL_BitMask32          *missedLo,
    /**  a place holder for 32-63 events */
    CSL_BitMask32          *missedHi,
    /**  a place holder for QDMA events */
    CSL_BitMask32          *missedQdma
    )
{
    /* Reading the missed Status registers */
	*missedLo    = hMod->EMR;
	*missedHi    = hMod->EMRH	;
	*missedQdma  = hMod->QEMR	;

    /* Clearing the status registers */
	hMod->EMCR = *missedLo;
	hMod->EMCRH	= *missedHi;
	hMod->QEMCR	= *missedQdma;
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaGetControllerError
 * ======================================================
 */
/** @brief 
 *	   Queries the Channel controller Error Status
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  ccStat
 *		this points to a structure allocated by the user 
 *      program that can hold the queried status values.
 *   @ret CSL_Status - CSL_SOK 
 *
 * ======================================================    
 */       

/** Gets the status of the controller Error.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetControllerError() can 
 * be called.  
 * @b Example:
   \code

	CSL_EdmaHandle     edmaHndl;          
	CSL_Status modStatus;
	CSL_EdmaccStatus ccError;
    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Get Controller Error
    edmaStatus = CSL_edmaGetControllerError(hMod,&ccError);
	\endcode
 * 
 * @return CSL  Status
 * 
 */

//#pragma CODE_SECTION (CSL_edmaGetControllerError, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaGetControllerError(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  a place holder for the Controller Error */
    CSL_EdmaccStatus  *ccStat
    )
{ 
	ccStat->errorMask = hMod->CCERR & 0xFF ; 
	ccStat->exceedTcc = (Bool)(CSL_FEXT(hMod->CCERR, EDMACC_CCERR_TCCERR)) ;
	/* Clears the errors */
	hMod->CCERRCLR = ccStat->exceedTcc << 16 | ccStat->errorMask;
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaGetIntrPendStatus
 * ======================================================
 */
/** @brief 
 *	   Queries the interrupt status of the interrupts.
 *     This is read from the Global Channel Register.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  intrLo
 *		status 0-31 of the interrupts 
 *   @arg  intrHi
 *		status 32-63 of the interrupts 
 *   @ret CSL_Status - CSL_SOK 
 *
 * ======================================================    
 */       

/** Gets the interrupt pend status.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetIntrPendStatus() can 
 * be called.  
 * @b Example:
   \code
	CSL_EdmaHandle     edmaHndl;          
	CSL_Status modStatus;
	Uint32 edmaIntrLo,edmaIntrHi;
    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Get interrupt pend status 
    edmaStatus = CSL_edmaGetIntrPendStatus(hMod,&edmaIntrLo. &edmaIntrHi);
                            
	\endcode
 * 
 * @return CSL  Status
 * 
 */

//#pragma CODE_SECTION (CSL_edmaGetIntrPendStatus, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaGetIntrPendStatus(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  place holder for the interrupt status 0-31*/
    CSL_BitMask32           *intrLo,
	/**  place holder for the interrupt status 32-63*/
    CSL_BitMask32           *intrHi
     )
{
	*intrLo = hMod->IPR;
	*intrHi = hMod->IPRH;
	return CSL_SOK;
}   
#if CSL_EDMA_MEMPROTECT 
/*
 * ======================================================
 *   @func   CSL_edmaGetMemoryProtectionAttrib
 * ======================================================
 */
/** @brief 
 *	   Queries the Memory Access rights of the specified region. This 
 *     may be global/view.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  region
 *		region queried. This may refer to the global channel register space 
 *      or a specific region/view.
 *   @ret  CSL_BitMask32 - MPPAG/MPPA[0/1../n] value
 *
 * ======================================================    
 */ 
/** Gets the memory protection attributes.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetMemoryProtectAttrib() can 
 * be called.  
 * @b Example:
   \code
	CSL_EdmaHandle     edmaHndl;          
	CSL_Status modStatus;
	Uint32 edmaIntrLo,edmaIntrHi;
	Uint32 memProt;
    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	
	// Get memory protection attributes 
    memProt = CSL_edmaGetMemoryProtectAttrib(hMod,CSL_EDMA_REGION_2);

                            
	\endcode
 * 
 * @return Memory protection attributes
 * 
 */
//#pragma CODE_SECTION (CSL_edmaGetMemoryProtectionAttrib, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_BitMask32  CSL_edmaGetMemoryProtectionAttrib(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
        /**  interrupt queried */
    Int        region
    )
{
	CSL_BitMask32 mppa;
	if (region < 0)
		mppa = hMod->MPPAG;
	else
		mppa = hMod->MPPA[region];			
	return mppa;
}
//#pragma CODE_SECTION (CSL_edmaGetMemoryFaultError, ".text:csl_section:edma");
/*
 * ======================================================
 *   @func   CSL_edmaGetMemoryFaultError
 * ======================================================
 */
/** @brief 
 *	   Queries the Channel controllers memory fault error and the
 *     Error attributes
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  memFault
 *		this points to a structure alloctaed by the user 
 *      program that can hold the queried the memory fault status values.
 *   @ret CSL_Status - CSL_SOK 
 *
 * ======================================================    
 */    
/** Gets the memory fault status details, clears the fault.
 * 
 * <b> Usage Constraints: </b>
 * All @a CSL_edmaInit(), @a CSL_edmaOpen(), must be 
 * called successfully in that order before @a CSL_edmaGetMemoryFaultError() can 
 * be called.  
 * @b Example:
   \code

	CSL_EdmaMemFaultStatus memFault;
	CSL_EdmaHandle     edmaHndl;          
	CSL_Status modStatus;

    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Get memory protection attributes 
    memStatus CSL_edmaGetMemoryFaultError(hMod,&memFault);
                            
	\endcode
 * 
 * @return CSL Status.
 * 
 */
CSL_IDEF_INLINE CSL_Status  CSL_edmaGetMemoryFaultError(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  a place holder for the memory fault Error status */
    CSL_EdmaMemFaultStatus  *memFault
    )
{ 
	/* Reading the Memory Fault Status registers */
	memFault->addr = hMod->MPFAR; 
	memFault->errorMask =  CSL_FEXTR(hMod->MPFSR, 8, 0);
	memFault->fid = CSL_FEXTR(hMod->MPFSR, 12, 9);
	
	/* Clearing the Memory Fault Error */
	//CSL_FINS(hMod->MPFCR,EDMACC_MPFCR_MPFCLR,1);
	hMod->MPFCR = 1;
	return CSL_SOK;		
}


#endif
/*
 * ======================================================
 *   @func   CSL_edmaSetMemoryProtectionAttrib
 * ======================================================
 */
/** @brief 
 *	   Sets specified memory protection attributes for the specifed region.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  edmaRegion
 *		region - this may be specified as one of the many view supported by the 
 *      chip or the global memory protection attributes themselves.
 *   @arg  mpa
 *	  	Value to be programmed into the MPPAG/MPPA[0/1/2/../n].
 *      This is a Bitmask of the protection attributes. 
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API sets the memory protection attributes for the specified region.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    
	CSL_EdmaHandle     edmaHndl;          
	CSL_Status modStatus;

    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Module Control API 
	modStatus = CSL_edmaSetMemoryProtectionAttrib(hModHndl,CSL_EDMA_REGION_0,
		(CSL_EDMA_MEMACCESS_UX|CSL_EDMA_MEMACCESS_UW|CSL_EDMA_MEMACCESS_UR|CSL_EDMA_MEMACCESS_AID2));
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
#if CSL_EDMA_MEMPROTECT 
//#pragma CODE_SECTION (CSL_edmaSetMemoryProtectionAttrib, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaSetMemoryProtectionAttrib(
    /**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  region needing the Memory protection */
    Int        edmaRegion,
    /**  bitMask of the Memory Protection Attributes */
    CSL_BitMask32          mpa
    )
{    
	if (edmaRegion == CSL_EDMA_REGION_GLOBAL)
		hMod->MPPAG = mpa;
	else 
	    hMod->MPPA[edmaRegion] = mpa;
	return CSL_SOK;
}    
#endif
/*
 * ======================================================
 *   @func   CSL_edmaDmaRegionAccessEnable
 * ======================================================
 */
/** @brief 
 *	   Enables shadow region access bits DRAE,DRAEH,QRAE
 */
  /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  edmaRegion
 *		region - this may be specified as one of the many view supported by the 
 *      chip or the global memory protection attributes themselves.
 *   @arg access
 *		region bits to be programmed
 *   @arg  drae 
 *	  	Bitmask to be enabled in DRAE
 *   @arg  draeh
 *	  	Bitmask to be enabled in DRAEH
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */

/** This API sets the DRAE/DRAEH BitMask.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code

    
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;

    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Module Control API 
	status = CSL_edmaDmaRegionAccessEnable(hModHndl,CSL_EDMA_REGION_0,0x000000FF, 0x000000FF);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
 
//#pragma CODE_SECTION (CSL_edmaDmaRegionAccessEnable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaDmaRegionAccessEnable(
    /**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  region needing the Memory protection */
    Int        edmaRegion,
    /**  drae bits to be ebledan */
    CSL_BitMask32          drae,
    /**  draeh bits to be enables */
    CSL_BitMask32          draeh
)
{
	int cs;
	if (edmaRegion == CSL_EDMA_REGION_GLOBAL)
		return CSL_ESYS_INVPARAMS;
	cs = _CSL_beginCriticalSection()	;
	hMod->DRA[edmaRegion].DRAE |= drae;
	hMod->DRA[edmaRegion].DRAEH |= draeh;
	_CSL_endCriticalSection(cs)	;
	return CSL_SOK;
}  
/*
 * ======================================================
 *   @func   CSL_edmaDmaRegionAccessDisable
 * ======================================================
 */
/** @brief 
 *	   Disables shadow region access bits DRAE,DRAEH,QRAE
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  edmaRegion
 *		region - this may be specified as one of the many view supported by the 
 *      chip or the global memory protection attributes themselves.
 *   @arg access
 *		region bits to be programmed
 *   @arg  drae 
 *	  	Bitmask to be disabled in DRAE
 *   @arg  draeh
 *	  	Bitmask to be disabled in DRAEH
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
  
/** This API clears the DRAE/DRAEH BitMask.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
      
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;

    // Initialize
   
    modStatus = CSL_edmaInit(NULL);
    ...
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	// Module Control API 
	status = CSL_edmaDmaRegionAccessDisable(hModHndl,CSL_EDMA_REGION_0,0x000000FF,0x000000FF);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaDmaRegionAccessDisable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaDmaRegionAccessDisable(
    /**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  region needing the Memory protection */
    Int        edmaRegion,
    /**  drae bits to be ebledan */
    CSL_BitMask32          drae,
    /**  draeh bits to be enables */
    CSL_BitMask32          draeh
    )
{
	int cs;
	if (edmaRegion == CSL_EDMA_REGION_GLOBAL)
		return CSL_ESYS_INVPARAMS;
	cs = _CSL_beginCriticalSection()	;	
	hMod->DRA[edmaRegion].DRAE &= ~drae;
	hMod->DRA[edmaRegion].DRAEH &= ~draeh;
	_CSL_endCriticalSection(cs)	;
 	return CSL_SOK;
}    
/*
 * ======================================================
 *   @func   CSL_edmaQdmaRegionAccessDisable
 * ======================================================
 */
/** @brief 
 *	   Disables shadow region access bits DRAE,DRAEH,QRAE
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  edmaRegion
 *		region - this may be specified as one of the many view supported by the 
 *      chip or the global memory protection attributes themselves.
 *   @arg  qrae 
 *	  	Bitmask to be disabled in QRAE
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
/** This API sets the QRAE BitMask.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
  
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);	// Module Control API 
	// ALL 8 Channels allowed
	status = CSL_edmaQdmaRegionAccessEnable(hModHndl,CSL_EDMA_REGION_0,0x000000FF);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaQdmaRegionAccessEnable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaQdmaRegionAccessEnable(
    /**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  region needing the Memory protection */
    Int                    edmaRegion,
    /**  drae bits to be ebledan */
    CSL_BitMask32          qrae
)
{
	int cs;
	if (edmaRegion == CSL_EDMA_REGION_GLOBAL)
		return CSL_ESYS_INVPARAMS;
	cs = _CSL_beginCriticalSection()	;
	hMod->QRAE[edmaRegion] |= qrae;
	_CSL_endCriticalSection(cs)	;
	return CSL_SOK;
} 
/*
 * ======================================================
 *   @func   CSL_edmaQdmaRegionAccessEnable
 * ======================================================
 */
/** @brief 
 *	   Enables shadow region access bits DRAE,DRAEH,QRAE
 */
  /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  edmaRegion
 *		region - this may be specified as one of the many view supported by the 
 *      chip or the global memory protection attributes themselves.
 *   @arg access
 *		region bits to be programmed
 *   @arg  qrae 
 *	  	Bitmask to be enabled in QRAE
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================
 */
   
/** This API clears the QRAE BitMask.
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code

	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	// Module Control API 
	// ALL 8 Channels disallowed
	status = CSL_edmaQdmaRegionAccessDisable(hModHndl,CSL_EDMA_REGION_0,0x000000FF);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaQdmaRegionAccessDisable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaQdmaRegionAccessDisable(
    /**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  region needing the Memory protection */
    Int        edmaRegion,
    /**  drae bits to be ebledan */
    CSL_BitMask32          qrae
    )
{
	int cs;
	if (edmaRegion == CSL_EDMA_REGION_GLOBAL)
		return CSL_ESYS_INVPARAMS;
	cs = _CSL_beginCriticalSection()	;	
	hMod->QRAE[edmaRegion] &= ~qrae;
	_CSL_endCriticalSection(cs)	;
 	return CSL_SOK;
}    
/*
 * ======================================================
 *   @func   CSL_edmaEventQueuePrioritySet
 * ======================================================
 */
/** @brief 
 *	   Sets the specified priority for the specified queue.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  que
 *		Event queue
 *   @arg  pri
 *	  	Priority needed to be set for above Event queue.
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */
/** This API sets the Queue priority level to the appropriate value 
 * 
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code

	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
    status = CSL_edmaEventQueuePrioritySet(hModHndl,CSL_EDMA_EVT_QUE0,
		CSL_EDMA_VBUSM_PRI2);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaEventQueuePrioritySet, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaEventQueuePrioritySet(
	/**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  Queue for which priority level is required to be set */
    CSL_EdmaEventQueue         que,
    /**  bitMask of the Memory Protection Attributes */
    CSL_EdmaEventQueuePri      pri
    )
{
	int cs;
	cs = _CSL_beginCriticalSection()	;	
	CSL_FINSR(hMod->QUEPRI, (4*que + 2),(4*que),pri);
	_CSL_endCriticalSection(cs);
	return CSL_SOK;
} 
/*
 * ======================================================
 *   @func   CSL_EdmaEventQueueTcMap
 * ======================================================
 */
/** @brief 
 *	   Maps the specified transfer controller number to the specified queue.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  que
 *		Event queue
 *   @arg  tc
 *	  	transfer controller number
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */       
/** This API does the Event Queue to TC Mapping
 * 
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...
   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
    
	status = CSL_edmaEventQueueTcMap(hModHndl,CSL_EDMA_EVT_QUE0,
		CSL_EDMA_TC2);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaEventQueueTcMap, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaEventQueueTcMap(
	/**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  Queue for which priority level is required to be set */
    CSL_EdmaEventQueue     que,
    /**  bitMask of the Memory Protection Attributes */
    CSL_EdmaTc             tc
    )
{
	int cs;
	cs = _CSL_beginCriticalSection()	;	
	CSL_FINSR(hMod->QUETCMAP, (4*que + 2),(4*que),tc);
	_CSL_endCriticalSection(cs);
	return CSL_SOK;
}    
/*
 * ======================================================
 *   @func   CSL_EdmaEventQueueThresholdSet
 * ======================================================
 */
/** @brief 
 *	   Sets the specified threshold for the specified queue.
 */
 /* ======================================================
 *   @arg  hMod
 *		module Handle
 *   @arg  que
 *		event queue
 *   @arg  threshold
 *	  	target threshold value for the above queue.
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */
/** This API sets the Queue threshold to the appropriate level
 * 
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
   
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
    
	status = CSL_edmaEventQueueThresholdSet(hModHndl,CSL_EDMA_EVT_QUE0,
		CSL_EDMA_QUETHRESH_10);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK
 *
 */
//#pragma CODE_SECTION (CSL_edmaEventQueueThresholdSet, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaEventQueueThresholdSet(
	/**  module handle to EDMA */
    CSL_EdmaHandle		   hMod,
    /**  Queue for which priority level is required to be set */
    CSL_EdmaEventQueue         que,
    /**  Threshold to be set */
    CSL_EdmaEventQueueThr      threshold
    )
{
	int cs;
	cs = _CSL_beginCriticalSection()	;	
	if (que < ((CSL_EdmaEventQueue)4))
		CSL_FINSR(hMod->QWMTHRA, (8*que + 4),(8*que),threshold);			
	 else 
		CSL_FINSR(hMod->QWMTHRB, (8 * (que-4) + 4),(8 * (que-4)),threshold);
	_CSL_endCriticalSection(cs);	
	return CSL_SOK;
}    
/*
 * ======================================================
 *   @func   CSL_edmaErrorEval
 * ======================================================
 */
/** @brief 
 *	   Sets EVAL bit of the EEVAL register in the Global register space.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */ 

/** This API enables enables evaluation of errros for the specified view/shadow Region.
 * 
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
   
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Module Control API 
	status = CSL_edmaErrorEval(hModHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_INVPARAMS
 *
 */
//#pragma CODE_SECTION (CSL_edmaErrorEval, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaErrorEval(
	/**  module handle to EDMA */
    CSL_EdmaHandle		   hMod
    )
{
//	CSL_FINST(hMod->EEVAL,EDMACC_EEVAL_EVAL,EVAL);
    hMod->EEVAL = 1; 
	return CSL_SOK;
}    
/*
 * ======================================================
 *   @func   CSL_edmaInterruptEnable
 * ======================================================
 */
/** @brief 
 *	   Enables the specified interrupt in the Global Channel register space.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  intrLo
 *		interrupt 0-31 (BitMask32) to be enabled
 *   @arg  intrHi
 *		interrupt 32-63 (BitMask32) to be enabled
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */
/** This API enables the interrupts as specified in the BitMask
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
   
	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);

    status = CSL_edmaInterruptEnable(hModHndl,0x000000E0, 0x0);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaInterruptEnable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaInterruptEnable( 
    /**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  interrupt to be enabled 0-31*/
	CSL_BitMask32          intrLo,
	/**  interrupt to be enabled 32-63*/
	CSL_BitMask32          intrHi
)
{
	hMod->IESR = intrLo;
	hMod->IESRH = intrHi;
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaInterruptDisable
 * ======================================================
 */
/** @brief 
 *	   Disables the specified interrupt in the Global Channel register space.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  intrLo
 *		interrupt 0-31 (BitMask32) to be disabled
 *   @arg  intrHi
 *		interrupt 32-63 (BitMask32) to be disabled
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */ 
/** This API disables the interrupts as specified
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code

	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);

	// Interrupt 5,6,7 disabled
    status = CSL_edmaInterruptDisable(hModHndl,0x000000E0, 0x0);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaInterruptDisable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaInterruptDisable(
    /**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  interrupt to be disabled */
    CSL_BitMask32          intrLo,
    /**  interrupt to be disabled */
    CSL_BitMask32          intrHi
)
{
	hMod->IECR  = intrLo;
	hMod->IECRH = intrHi;
	return CSL_SOK;	
}
/*
 * ======================================================
 *   @func   CSL_edmaInterruptClear
 * ======================================================
 */
/** @brief 
 *	   Clears the specified interrupt in the Global Channel register space.
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  intrLo
 *		interrupt 0-31 (BitMask32) to be cleared
 *   @arg  intrHi
 *		interrupt 32-63 (BitMask32) to be cleared
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */ 
/** This API clears the interrupt as specified 
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    

	CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
    // 5,6,7 cleared 
    status = CSL_edmaInterruptClear(hModHndl,0x000000E0,0);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaInterruptClear, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaInterruptClear(
	/**  module handle to EDMA */
    CSL_EdmaHandle         hMod,
    /**  interrupt to be cleared 0- 31 */
    CSL_BitMask32          intrLo,
    /**  interrupt to be cleared 32 - 63 */
    CSL_BitMask32          intrHi
)
{
	hMod->ICR = intrLo;
	hMod->ICRH = intrHi;
	return CSL_SOK;	
}
/*
 * ======================================================
 *   @func   CSL_edmaInterruptEval
 * ======================================================
 */
/** @brief 
 *	   If any enabled interrutps are pending, the tpcc_intN 
 *   output signal to be pulsed.  
 */
 /* ======================================================
 *   @arg  hMod
 *		Module Handle
 *   @arg  NULL
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */ 
/**
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    
    CSL_EdmaHandle     hModHndl;          
	CSL_Status modStatus;
    // Initialize   
    modStatus = CSL_edmaInit(NULL);
    ...   
    hModHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	

    status = CSL_edmaInterruptEval(hModHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
CSL_IDEF_INLINE CSL_Status  CSL_edmaInterruptEval(
    /**  a handle to the requested channel instance of the specified EDMA
     *   obtained  through the @a CSL_edmaOpen() call */
    CSL_EdmaHandle         hMod
)
{
//	CSL_FINST(hMod->IEVAL,EDMACC_IEVAL_EVAL,EVAL);
    hMod->IEVAL = 1;
	return CSL_SOK; 
}

/**
@}
*/

/** 
@addtogroup CSL_EDMA_FUNCTION_SHADOW_INTERNAL 
@{
*/
/*
 * ======================================================
 *   @func   CSL_edmaRegionInterruptEnable
 * ======================================================
 */
/** @brief 
 *	   Enables specified interrupt in the Shadow Region .
 */
 /* ======================================================
 *   @arg  hSh
 *		Shadow Region Handle
 *   @arg  intrLo
 *		interrupt 0-31 to be enabled
 *   @arg  intrHi
 *		interrupt 32-63 to be enabled
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 * 
 * ======================================================   
 */   
/** This API enables the interrupts for the shadow region. 
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaRegionOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
  	CSL_EdmaObj ModuleObj;
	CSL_EdmaHandle hModHndl;
	CSL_EdmaRegionParam regParam;
	CSL_EdmaRegionObj RegionObj;
	CSL_EdmaRegionHndl shHndl;
	CSL_Status status;
	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    // Initialization
	CSL_edmaInit(NULL);
	
	// Open	Module
	edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Region Open 
	regParam.regionNum = CSL_EDMA_REGION_0;
	shHndl = CSL_edmaRegionOpen(&RegionObj,CSL_EDMA_0,&regParam,&status);
        
	status = CSL_edmaRegionInterruptEnable(shHndl,0x0000ffff,0xffff0000);
   
   \endcode
 *
 * @return returns CSL status 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaRegionInterruptEnable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaRegionInterruptEnable(
    /**  a handle to the shadow region already opened */
    CSL_EdmaRegionHandle       hSh,
    /**  interrupt to be enabled - Lo*/
    CSL_BitMask32              intrLo,
    /**  interrupt to be enabled -Hi */
    CSL_BitMask32              intrHi    
)
{
	
	hSh->shregs->IESR = intrLo;
	hSh->shregs->IESRH = intrHi;
	return CSL_SOK;		
}
/*
 * ======================================================
 *   @func   CSL_edmaRegionInterruptDisable
 * ======================================================
 */
/** @brief 
 *	   Disabled specified interrupt in the Shadow Region .
 */
 /* ======================================================
 *   @arg  hSh
 *		Shadow Region Handle
 *   @arg  intrLo
 *		interrupt to be disabled(0-31)
 *   @arg  intrHi
 *		interrupt to be disabled(32-63) 
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================   
 */   

/** This API disables the interrupts for the shadow region. 
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaRegionOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
 	CSL_EdmaObj ModuleObj;
	CSL_EdmaHandle hModHndl;
	CSL_EdmaRegionParam regParam;
	CSL_EdmaRegionObj RegionObj;
	CSL_EdmaRegionHndl shHndl;
	CSL_Status status;
	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    // Initialization
	CSL_edmaInit(NULL);
	
	// Open	Module
	edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Region Open 
	regParam.regionNum = CSL_EDMA_REGION_0;
	shHndl = CSL_edmaRegionOpen(&RegionObj,CSL_EDMA_0,&regParam,&status);
   
	status = CSL_edmaRegionInterruptEnable(shHndl,0x0000ffff,0xffff0000);
   
   \endcode
 *
 * @return returns CSL status 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaRegionInterruptDisable, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaRegionInterruptDisable(
    /**  a handle to the shadow region already opened */
    CSL_EdmaRegionHandle       hSh,
    /**  interrupt to be enabled - Lo*/
    CSL_BitMask32              intrLo,
    /**  interrupt to be enabled -Hi */
    CSL_BitMask32              intrHi    
)
{
	hSh->shregs->IECR = intrLo;
	hSh->shregs->IECRH = intrHi;
	return CSL_SOK;	
}
/*
 * ======================================================
 *   @func   CSL_edmaRegionInterruptClear
 * ======================================================
 */
/** @brief 
 *	   Clears specified interrupts in the Shadow Region .
 */
 /* ======================================================
 *   @arg  hSh
 *		Shadow Region Handle
 *   @arg  clrLo
 *		interrupt to be cleared(0-31)
 *   @arg  clrHi
 *		interrupt to be cleared(32-63) 
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================   
 */ 
/** This API clears the interrupts for the shadow region. 
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaRegionOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    	CSL_EdmaObj ModuleObj;
	CSL_EdmaHandle hModHndl;
	CSL_EdmaRegionParam regParam;
	CSL_EdmaRegionObj RegionObj;
	CSL_EdmaRegionHndl shHndl;
	CSL_Status status;
	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    // Initialization
	CSL_edmaInit(NULL);
	
	// Open	Module
	edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Region Open 
	regParam.regionNum = CSL_EDMA_REGION_0;
	shHndl = CSL_edmaRegionOpen(&RegionObj,CSL_EDMA_0,&regParam,&status);
	...
	status = CSL_edmaRegionInterruptClear(shHndl,0x0000ffff,0xffff0000);
   
   \endcode
 *
 * @return returns CSL status 
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaRegionInterruptClear, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaRegionInterruptClear(
    /**  a handle to the shadow region already opened */
    CSL_EdmaRegionHandle       hSh,
    /**  interrupt to be enabled */
    CSL_BitMask32              intrLo,
    /**  interrupt to be enabled */
    CSL_BitMask32              intrHi
)
{

	hSh->shregs->ICR = intrLo;
	hSh->shregs->ICRH = intrHi ;
	return CSL_SOK;
}
/*
 * ======================================================
 *   @func   CSL_edmaRegionIntrEval
 * ======================================================
 */
/** @brief 
 *	 Sets EVAL bit of the IEVAL register in the specifed shadow region 
 *   register space.
 */
 /* ======================================================
 *   @arg  hSh
 *		Shadow Region Handle
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */   

/** This API enables evaluation of any interrupts in the shadow region
 *
 * <b> Usage Constraints: </b>
 * The CSL system as well as EDMA must be succesfully initialized
 * via @a CSL_sysInit() and @a CSL_edmaInit().
 * CSL_edmaRegionOpen() must be called before usage of this API.
 *
 * @b Example:
   \code
    
  	CSL_EdmaObj ModuleObj;
	CSL_EdmaHandle hModHndl;
	CSL_EdmaRegionParam regParam;
	CSL_EdmaRegionObj RegionObj;
	CSL_EdmaRegionHndl shHndl;
	CSL_Status status;
	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	
    // Initialization
	CSL_edmaInit(NULL);
	
	// Open	Module
	edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Region Open 
	regParam.regionNum = CSL_EDMA_REGION_0;
	shHndl = CSL_edmaRegionOpen(&RegionObj,CSL_EDMA_0,&regParam,&status);
	...

	status = CSL_edmaRegionInterruptEval(shHndl);
   
   \endcode
 *
 * @return returns status whether the command passed/failed.
 * Possible return values - CSL_SOK, CSL_ESYS_BADHANDLE 
 *
 */
//#pragma CODE_SECTION (CSL_edmaRegionInterruptEval, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaRegionInterruptEval(
	/**  module handle to EDMA */
    CSL_EdmaRegionHandle		   hSh    
    )
{
	hSh->shregs->IEVAL = 1;
	return CSL_SOK;
}
/*
 * ======================================================
 *   @func   CSL_edmaGetRegionIntrPendStatus
 * ======================================================
 */
/** @brief 
 *   Queries the pend status of all the interrupt channels in the 
 *   shadow region.
 */
 /* ======================================================
 *   @arg  hSh
 *		Shadow Region Handle
 *   @arg  *intrLo
 *      Place holder for interrupt Status 0-31
 *   @arg  *intrHi
 *      Place holder for interrupt Status 32-63
 *   @ret CSL_Status - CSL_SOK/CSL_ESYS_BADHANDLE
 *
 * ======================================================    
 */   
/** Queries the interrupt Pend status in the shadow region.
 *
 * <b> Usage Constraints: </b>
 * @a CSL_edmaInit(), CSL_edmaRegionOpen must have been called successfully before this function can 
 * be called.The user must allocate space for this and this API fills in the system 
 * specified fields.
 * 
 *
 * @b Example:
 * \code
	CSL_EdmaObj ModuleObj;
	CSL_EdmaHandle hModHndl;
	CSL_EdmaRegionParam regParam;
	CSL_EdmaRegionObj RegionObj;
	CSL_EdmaRegionHndl shHndl;
	CSL_Status status;
	CSL_EdmaChanObj ChObj;
	CSL_EdmaChanHandle chHndl;
	CSL_BitMask32 intrPend[2];
	
    // Initialization
	CSL_edmaInit(NULL);
	
	// Open	Module
	edmaHndl = CSL_edmaOpen(NULL,CSL_EDMA_0,NULL,&modStatus);
	
	// Region Open 
	regParam.regionNum = CSL_EDMA_REGION_0;
	shHndl = CSL_edmaRegionOpen(&RegionObj,CSL_EDMA_0,&regParam,&status);
	
	// Query the shadow region 
	CSL_edmaGetRegionIntrPendStatus(shHndl, &intrPend[0], &intrPend[1]);
	
   \endcode
 *
 * @return CSL Status.
 *
 */
//#pragma CODE_SECTION (CSL_edmaGetRegionIntrPendStatus, ".text:csl_section:edma");
CSL_IDEF_INLINE CSL_Status  CSL_edmaGetRegionIntrPendStatus(
	/**  module handle to EDMA */
    CSL_EdmaRegionHandle       hSh,
    /**  interrupt queried */
    CSL_BitMask32              *intrLo,
    /**  interrupt queried */
    CSL_BitMask32              *intrHi
    )
{
    if (intrLo!=NULL)
    	*intrLo = hSh->shregs->IPR;
    if (intrHi!=NULL)
    	*intrHi = hSh->shregs->IPRH;	
	return CSL_SOK;
}  
/**
@}
*/
#endif
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 2          Aug 10:08:58 9         2272             xkeshavm      */
/*                                                                  */
/* To upload API CSL 0.51 Release                                   */
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:03    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
