;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:12 2000

;--------------------------------------------------------
	.module _iscntrl
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _iscntrl
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
	G$iscntrl$0$0 ==.
;	_iscntrl.c 27
;	-----------------------------------------
;	 function iscntrl
;	-----------------------------------------
_iscntrl:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_iscntrl.c 32
	mov	r2,dpl
;	_iscntrl.c 30
	clr	c
	mov	a,#0x1f
	subb	a,r2
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00101$
00107$:
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r2,#0x7f,00102$
; Peephole 201   removed redundant sjmp
00108$:
00109$:
00101$:
;	_iscntrl.c 31
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_iscntrl.c 32
	mov	dpl,#0x00
00104$:
	C$_iscntrl.c$33$1$1 ==.
	XG$iscntrl$0$0 ==.
	ret
	.area	CSEG    (CODE)
