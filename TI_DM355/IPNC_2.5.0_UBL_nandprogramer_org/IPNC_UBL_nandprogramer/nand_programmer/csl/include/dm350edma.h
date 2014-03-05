#ifndef _DM350EDMA_H_
#define _DM350EDMA_H_

#define CSL_IDEF_INLINE static
/**************************************************************************\
* Davinci EDMA file
\**************************************************************************/

/**************************************************************************\
*  DaVinci Configuration 
*  
\**************************************************************************/
#define CSL_EDMA_NUM_DMACH            64
#define CSL_EDMA_NUM_QDMACH            8
#define CSL_EDMA_NUM_PARAMENTRY      128
#define CSL_EDMA_NUM_EVQUE             2
#define CSL_EDMA_CHMAPEXIST            0
#define CSL_EDMA_NUMINTCH			  64  
#define CSL_EDMA_NUM_REGIONS	       4  
#define CSL_EDMA_MEMPROTECT            0  

/** @brief Number of Generic Channel instances */
#define CSL_EDMA_CHA_CNT              72




/** @brief Enumerations for EDMA channels
*
*  There are 68 EDMA channels - 64 EDMA Channels and 8 QDMA Channels. 
*  The enum values indicate the number of the channel.  
*  This is passed as a parameter in @a CSL_dmaOpen() to indicate the
*  channel instance that the user wants to acquire.  The CSL_DMA_CHA_ANY
*  indicates the user is willing to use any available generic channel.
*  
*
*/

// MB (06/08) Hooked up 3PDMA events from Chap 5 DM350, Table 5-8

	
#define     CSL_EDMA_TINT6		   		      0  /**< Channel 0 */
#define     CSL_EDMA_TINT7		   		      1  /**< Channel 1 */
#define     CSL_EDMA_McBSP0XEVT    		      2  /**< Channel 2 */
#define     CSL_EDMA_McBSP0REVT	   		      3  /**< Channel 3 */
#define     CSL_EDMA_EVT1			   	      4  /**< Channel 4 */
#define     CSL_EDMA_EVT2	 		   	      5  /**< Channel 5 */
#define     CSL_EDMA_EVT3			   	      6  /**< Channel 6 */
#define     CSL_EDMA_EVT4	   		          7  /**< Channel 7 */
#define     CSL_EDMA_McBSP1XEVT		   		  8  /**< Channel 8 */ //McBSP1XEVT or TINT4
#define     CSL_EDMA_TINT4		   		      8  /**< Channel 8 */ //McBSP1XEVT or TINT4
#define     CSL_EDMA_McBSP1REVT		   		  9  /**< Channel 9 */ //McBSP1REVT or TINT5
#define     CSL_EDMA_TINT5		   		      9  /**< Channel 9 */ //McBSP1REVT or TINT5
#define     CSL_EDMA_SPI2XEVT	   		      10 /**< Channel 10*/
#define     CSL_EDMA_SPI2REVT	   		      11 /**< Channel 11*/
#define     CSL_EDMA_IMXINT		   		      12 /**< Channel 12*/
#define     CSL_EDMA_SEQINT		   		      13 /**< Channel 13*/
#define     CSL_EDMA_SPI1XEVT	   		      14 /**< Channel 14*/
#define     CSL_EDMA_SPI1REVT	   		      15 /**< Channel 15*/
#define     CSL_EDMA_SPI0XEVT	   		      16 /**< Channel 16*/
#define     CSL_EDMA_SPI0REVT		 	      17 /**< Channel 17*/
#define     CSL_EDMA_URXEVT0		 	      18 /**< Channel 18*/
#define     CSL_EDMA_UTXEVT0		   	      19 /**< Channel 19*/
#define     CSL_EDMA_URXEVT1		   	      20 /**< Channel 20*/
#define     CSL_EDMA_UTXEVT1		   	      21 /**< Channel 21*/
#define     CSL_EDMA_URXEVT2		   	      22 /**< Channel 22*/
#define     CSL_EDMA_UTXEVT2		   	      23 /**< Channel 23*/
#define     CSL_EDMA_RTOINT		   		      24 /**< Channel 24*/
#define     CSL_EDMA_GPINT9                   25 /**< Channel 25*/
#define     CSL_EDMA_MMC0RXEVT	   		      26 /**< Channel 26*/ //MMC0 RXEVT or MSEVT
#define     CSL_EDMA_MSEVT	   				  26 /**< Channel 26*/ //MMC0 RXEVT or MSEVT
#define     CSL_EDMA_MMC0TXEVT	   		      27 /**< Channel 27*/
#define     CSL_EDMA_ICREVT		   		      28 /**< Channel 28*/
#define     CSL_EDMA_ICXEVT		   		      29 /**< Channel 29*/
#define     CSL_EDMA_MMC1RXEVT	   		      30 /**< Channel 30*/
#define     CSL_EDMA_MMC1TXEVT	 		      31 /**< Channel 31*/
#define     CSL_EDMA_GPINT0		   		      32 /**< Channel 32*/
#define     CSL_EDMA_GPINT1		   		      33 /**< Channel 33*/
#define     CSL_EDMA_GPINT2		   		      34 /**< Channel 34*/
#define     CSL_EDMA_GPINT3		   		      35 /**< Channel 35*/
#define     CSL_EDMA_GPINT4		   		      36 /**< Channel 36*/
#define     CSL_EDMA_GPINT5		   		      37 /**< Channel 37*/
#define     CSL_EDMA_GPINT6		   		      38 /**< Channel 38*/
#define     CSL_EDMA_GPINT7		   		      39 /**< Channel 39*/
#define     CSL_EDMA_GPBNKINT0	   		      40 /**< Channel 40*/
#define     CSL_EDMA_GPBNKINT1	   		      41 /**< Channel 41*/
#define     CSL_EDMA_GPBNKINT2	   		      42 /**< Channel 42*/
#define     CSL_EDMA_GPBNKINT3	   		      43 /**< Channel 43*/
#define     CSL_EDMA_GPBNKINT4		   	      44 /**< Channel 44*/
#define     CSL_EDMA_GPBNKINT5 		   	      45 /**< Channel 45*/
#define     CSL_EDMA_GPBNKINT6	   		      46 /**< Channel 46*/
#define     CSL_EDMA_GPINT8		   		      47 /**< Channel 47*/
#define     CSL_EDMA_TINT0		   		      48 /**< Channel 48*/
#define     CSL_EDMA_TINT1		   		      49 /**< Channel 49*/
#define     CSL_EDMA_TINT2		   		      50 /**< Channel 50*/
#define     CSL_EDMA_TINT3		   		      51 /**< Channel 51*/
#define     CSL_EDMA_PWM0		   		      52 /**< Channel 52*/
#define     CSL_EDMA_PWM1		   		      53 /**< Channel 53*/
#define     CSL_EDMA_PWM2		   		      54 /**< Channel 54*/
#define     CSL_EDMA_PWM3		   		      55 /**< Channel 54*/
#define     CSL_EDMA_VLCDINT	   		      56 /**< Channel 56*/
#define     CSL_EDMA_BIMINT		   		      57 /**< Channel 57*/
#define     CSL_EDMA_DCTINT		   		      58 /**< Channel 58*/
#define     CSL_EDMA_QIQINT		   		      59 /**< Channel 59*/
#define     CSL_EDMA_BPSINT		   		      60 /**< Channel 60*/
#define     CSL_EDMA_VLCDERRINT	   		      61 /**< Channel 61*/
#define     CSL_EDMA_RCNTINT	   		      62/**< Channel 62*/
#define     CSL_EDMA_COPCINT	   		      63 /**< Channel 63*/
#define     CSL_EDMA_QCHA0		   		      64 /**< QDMA Channel 0*/
#define     CSL_EDMA_QCHA1		   		      65 /**< QDMA Channel 1*/
#define     CSL_EDMA_QCHA2    	   		      66 /**< QDMA Channel 2*/
#define     CSL_EDMA_QCHA3		   		      67  /**< QDMA Channel 3*/
#define     CSL_EDMA_QCHA4		   		      68  /**< QDMA Channel 4*/
#define     CSL_EDMA_QCHA5		   		      69  /**< QDMA Channel 5*/
#define     CSL_EDMA_QCHA6		   		      70  /**< QDMA Channel 6*/
#define     CSL_EDMA_QCHA7		   		      71  /**< QDMA Channel 7*/
  

/** @brief Enumerations for EDMA Event Queues
*
*  2 Event Queues
*
*/
typedef enum {
	CSL_EDMA_EVT_QUE_DEFAULT   = 			   0, /**< Queue 0 is default */  
	CSL_EDMA_EVT_QUE0 		   = 			   0, /**< Queue 0 */  
	CSL_EDMA_EVT_QUE1 		   = 			   1  /**< Queue 1 */  
	
} CSL_EdmaEventQueue;
/** @brief Enumerations for EDMA Transfer Controllers
*
*  2 Transfer Controllers. One to one mapping exists
*  between Event Queues and Transfer Controllers.
*
*/
typedef enum {
	CSL_EDMA_TC0 		   = 			   0, /**< TC 0 */  
	CSL_EDMA_TC1 		   = 			   1  /**< TC 1 */  

} CSL_EdmaTc;

/** @brief Enumeration for EDMA Regions 
*
*  
*/

#define 	CSL_EDMA_REGION_GLOBAL  -1   /**< Global Region */
#define 	CSL_EDMA_REGION_0  0         /**< EDMA Region 0 : Shadow Region for ARM*/
#define 	CSL_EDMA_REGION_1  1         /**< EDMA Region 1 : Shadow Region for GEM*/
#define 	CSL_EDMA_REGION_2  2         /**< EDMA Region 2 : Shadow Region for Sequencer*/
#define 	CSL_EDMA_REGION_3  3          /**< EDMA Region 3: Unused Shadow Region / Might take off from PDK CSL */

#endif  /* _DM35064PLUS_H_ */

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 4         14 Jan 2005 13:32:40    5888             xkeshavm      */
/*                                                                  */
/* Uploaded the CSL0.57 JAN 2005 Release and built the library for ARM and DSP*/
/********************************************************************/ 
/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */

/* 1         13 Nov 2005 12:48:09    401              x0029935      */
/*                                                                  */
/* Modified from davicni CSL files to Dm350                         */
/* First CSL files build up fro Dm350                               */
/********************************************************************/ 
