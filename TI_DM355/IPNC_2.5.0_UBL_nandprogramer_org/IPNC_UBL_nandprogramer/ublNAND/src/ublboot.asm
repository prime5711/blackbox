;****************************************************************************
;* UBLBOOT.ASM
;*
;* THIS MODULE PERFORMS THE FOLLOWING ACTIONS:
;*   ALLOCATES THE STACK AND INITIALIZES THE STACK POINTER
;*   INITIALIZE CLOCK CONTROLLER
;*   COPY THE IVT to the IVT LOCATION
;*   MAKE VINTH = 0 TO MAKE IVT AT 0X0
;*
;* THIS MODULE DEFINES THE FOLLOWING GLOBAL SYMBOLS:
;*   1) _c_intDM355     BOOT ROUTINE
;*   2) _gEntryPoint    ENTRY POINT FOR THE APP
;*
;****************************************************************************
	.sect ".boot"

	.state32

	.global _c_intDM355
	.ref	_main
	.global	_gEntryPoint

;***************************************************************
;* FUNCTION DEF: _c_intDM355                                    
;***************************************************************
_c_intDM355: .asmfunc

	;write to enable DTCM
	; C9_DTCM_ENBL = 0x00008019 for Davinci(16K RAM + 16K ROM)
	; C9_DTCM_ENBL = 0x0001001D for DM355 (32K RAM + 32K ROM)
		LDR R0 , C9_DTCM_ENBL
	MCR p15, #00, R0, c9 , c1, #0
	NOP

	;*------------------------------------------------------
	;* SET TO FIQ  MODE
	;*------------------------------------------------------
	; TINT1 is used to toggle GPIO61 and hence FIQ stack is being setup
	MRS	r0, cpsr
	BIC	r0, r0, #0x1F	; CLEAR MODES
	ORR	r0, r0, #0x11	; SET FIQ mode
	MSR	cpsr, r0

 	;*------------------------------------------------------
	;* INITIALIZE THE FIQ  MODE STACK                      
	;*------------------------------------------------------
	LDR     SP, f_stack
	LDR     R0, f_STACK_SIZE
	ADD	SP, SP, R0

	;*------------------------------------------------------
	;* SET TO SUPERVISOR  MODE
	;*------------------------------------------------------
	MRS	r0, cpsr
	BIC	r0, r0, #0x1F	; CLEAR MODES
	ORR	r0, r0, #0x13	; SET SUPERVISOR mode
	MSR	cpsr, r0       
	NOP
	NOP	

	;*------------------------------------------------------
	;* DISABLE IRQ and FIQ
	;*------------------------------------------------------
	MRS     r0, cpsr
	ORR     r0, r0, #0xC0  ; Disable IRQ and FIQ
	MSR     cpsr, r0
	NOP
	NOP
	                             
	;*------------------------------------------------------
	;* INITIALIZE SUPERVISOR STACK
	;*------------------------------------------------------
	LDR     SP, c_stack
	LDR     R0, c_STACK_SIZE
	ADD	SP, SP, R0

    ;*------------------------------------------------------
    ;* PERFORM AUTO-INITIALIZATION.  IF CINIT IS -1, THEN
    ;* THERE IS NONE.
    ;*------------------------------------------------------
_b1_:	LDR     R0, c_cinit
        BL      auto_init

	;*------------------------------------------------------
	;* Set interrupt table to location 0x00000000
	;*------------------------------------------------------
	MRC	p15, #0, r1, c1, c0, #0
 	MOV	r0,#0x2000
	BIC	r1, r1, r0                    
   	NOP                                 
	NOP
	MCR	p15, #0, r1, c1, c0, #0
	NOP
	NOP                                 
	
	; Branch to main
	BL _main
	
	NOP
	NOP
	
	;give control to user boot loader
	LDR R0, _entryPoint
	NOP
	NOP
	LDR	R1, [R0]
	NOP
	NOP
	MOV PC, R1
	NOP
	NOP           
	.endasmfunc
	
	.align 4
                        
;***************************************************************************
;*  PROCESS INITIALIZATION TABLE.
;*
;*  THE TABLE CONSISTS OF A SEQUENCE OF RECORDS OF THE FOLLOWING FORMAT:
;*                                                                          
;*       .word  <length of data (bytes)>
;*       .word  <address of variable to initialize>                         
;*       .word  <data>
;*                                                                          
;*  THE INITIALIZATION TABLE IS TERMINATED WITH A ZERO LENGTH RECORD.
;*  USED directly from the RTS source without any modification
;*                                                                          
;***************************************************************************

tbl_addr: .set    R0
var_addr: .set    R1
length:   .set    R2
data:     .set    R3

auto_init:
	B	rec_chk

    ;*------------------------------------------------------
	;* PROCESS AN INITIALIZATION RECORD
    ;*------------------------------------------------------
record:	LDR	var_addr, [tbl_addr], #4   

    ;*------------------------------------------------------
	;* COPY THE INITIALIZATION DATA
    ;*------------------------------------------------------
	TST	var_addr, #3		   ; SEE IF DEST IS ALIGNED
	BNE     _bcopy			   ; IF NOT, COPY BYTES
	SUBS	length, length, #4 ; IF length <= 3, ALSO
	BMI     _bcont			   ; COPY BYTES

_wcopy:	LDR	data, [tbl_addr], #4  	   
	STR	data, [var_addr], #4	   ; COPY A WORD OF DATA
	SUBS	length, length, #4	   
	BPL	_wcopy			   ;
_bcont:	ADDS	length, length, #4	   
	BEQ	_cont			   

_bcopy:	LDRB	data, [tbl_addr], #1       
	STRB	data, [var_addr], #1       ; COPY A BYTE OF DATA
	SUBS	length, length, #1	   
	BNE	_bcopy                     

_cont:	ANDS	length, tbl_addr, #0x3 ; MAKE SURE THE ADDRESS
	RSBNE	length, length, #0x4       ; IS WORD ALIGNED
	ADDNE	tbl_addr, tbl_addr, length 

rec_chk:LDR	length, [tbl_addr], #4 ; PROCESS NEXT
	CMP	length, #0                 ; RECORD IF LENGTH IS
	BNE	record                     ; NONZERO

	MOV	PC, LR

	.endasmfunc


;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************
_entryPoint		.field	_gEntryPoint, 32
f_stack		    .long    0x7FA0    
f_STACK_SIZE  	.long    0x5C ;Last 4 bytes not used for stack as requried by mmcsd   
c_stack		    .long    0x7E00    
c_STACK_SIZE  	.long    0x1A0
c_cinit       	.long    cinit
C9_DTCM_ENBL 	.long    0x0001001D  

        .sect   ".cinit"
        .align  4
        .field          4,32
        .field          _stkchk_called+0,32
        .field          0,32

        .bss    _stkchk_called,4,4

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global	cinit
	.global _stkchk_called

.end
