;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _isprint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _isprint
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
	.area _DUMMY
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
	G$isprint$0$0 ==.
;	_isprint.c 24
;	-----------------------------------------
;	 function isprint
;	-----------------------------------------
_isprint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_isprint.c 29
	mov	r2,dpl
;	_isprint.c 27
	cjne	r2,#0x20,00108$
00108$:
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00109$:
	clr	c
	mov	a,#0x7e
	subb	a,r2
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00110$:
;	_isprint.c 28
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_isprint.c 29
	mov	dpl,#0x00
00104$:
	C$_isprint.c$30$1$1 ==.
	XG$isprint$0$0 ==.
	ret
	.area	CSEG    (CODE)
