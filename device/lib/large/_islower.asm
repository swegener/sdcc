;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:29 2000

;--------------------------------------------------------
	.module _islower
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _islower
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
_islower_c_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$islower$0$0 ==.
;	_islower.c 25
;	-----------------------------------------
;	 function islower
;	-----------------------------------------
_islower:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_islower.c 30
	push	dpl
	mov	dptr,#_islower_c_1_1
	pop	acc
	movx	@dptr,a
;	_islower.c 28
	mov	dptr,#_islower_c_1_1
	movx	a,@dptr
	mov	r2,a
	cjne	r2,#0x61,00108$
00108$:
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00109$:
	clr	c
	mov	a,#0x7a
	subb	a,r2
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00110$:
;	_islower.c 29
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_islower.c 30
	mov	dpl,#0x00
00104$:
	C$_islower.c$31$1$1 ==.
	XG$islower$0$0 ==.
	ret
	.area	CSEG    (CODE)
