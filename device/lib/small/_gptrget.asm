;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:12 2000

;--------------------------------------------------------
	.module _gptrget
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __gptrget
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; special function bits 
;--------------------------------------------------------
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area	DSEG    (DATA)
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG	(OVR,DATA)
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area	ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area	BSEG    (BIT)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_gptrget$0$0 ==.
;	_gptrget.c 26
;	-----------------------------------------
;	 function _gptrget
;	-----------------------------------------
__gptrget:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_gptrget.c 81
	    ;   save values passed
		xch    a,r0
		push   acc
	    ;
	    ;   depending on the pointer type
	    ;
	        mov     a,b
	        jz      00001$
		dec     a
		jz      00002$
	        dec     a
	        jz      00003$
		dec     a
		jz      00004$
	    ;
	    ;   any other value for type 
	    ;   return xFF
		mov     a,#0xff
		sjmp    00005$
	    ;
	    ;   Pointer to data space
	    ;   
 00001$:
		mov     r0,dpl     ; use only low order address
		mov     a,@r0
	        sjmp    00005$
	    ;
	    ;   pointer to xternal data
	    ;
 00002$:
	        movx    a,@dptr
	        sjmp    00005$
;
;   pointer to code area
;   
 00003$:
		clr     a
	        movc    a,@a+dptr
	        sjmp    00005$
;
;   pointer to xternal stack
;
 00004$:
		mov     r0,dpl
	        movx    a,@r0
;
;   restore and return
;
 00005$:
	        mov     r0,a	
	        pop     acc
		xch     a,r0
00101$:
	C$_gptrget.c$83$1$1 ==.
	XG$_gptrget$0$0 ==.
	ret
	.area	CSEG    (CODE)
