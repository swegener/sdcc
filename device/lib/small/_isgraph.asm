;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _isgraph
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _isgraph
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
	G$isgraph$0$0 ==.
;	_isgraph.c 25
;	-----------------------------------------
;	 function isgraph
;	-----------------------------------------
_isgraph:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_isgraph.c 30
	mov	r2,dpl
;	_isgraph.c 28
	cjne	r2,#0x21,00108$
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
;	_isgraph.c 29
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_isgraph.c 30
	mov	dpl,#0x00
00104$:
	C$_isgraph.c$31$1$1 ==.
	XG$isgraph$0$0 ==.
	ret
	.area	CSEG    (CODE)
