;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:09 2000

;--------------------------------------------------------
	.module _divuint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divuint_PARM_2
	.globl __divuint
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
__divuint_PARM_2:
	.ds	0x0002
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area	ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area	BSEG    (BIT)
__divuint_c_1_1:
	.ds	0x0001
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
	G$_divuint$0$0 ==.
;	_divuint.c 28
;	-----------------------------------------
;	 function _divuint
;	-----------------------------------------
__divuint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divuint.c 49
	mov	r2,dpl
	mov	r3,dph
;	_divuint.c 30
	mov	r4,#0x00
	mov	r5,#0x00
;	_divuint.c 34
	mov	r6,#0x10
00105$:
;	_divuint.c 36
	mov	a,r3
	rl	a
	anl	a,#0x01
	mov	r7,a
;	_divuint.c 37
	mov	a,r3
	xch	a,r2
	add	a,acc
	xch	a,r2
	rlc	a
	mov	r3,a
;	_divuint.c 38
	mov	a,r5
	xch	a,r4
	add	a,acc
	xch	a,r4
	rlc	a
	mov	r5,a
;	_divuint.c 39
	mov	a,r7
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_divuint.c 40
	orl	ar4,#0x01
00102$:
;	_divuint.c 42
	clr	c
	mov	a,r4
	subb	a,__divuint_PARM_2
	mov	a,r5
	subb	a,(__divuint_PARM_2 + 1)
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00106$
00115$:
;	_divuint.c 43
	clr	c
	mov	a,r4
	subb	a,__divuint_PARM_2
	mov	r4,a
	mov	a,r5
	subb	a,(__divuint_PARM_2 + 1)
	mov	r5,a
;	_divuint.c 45
	orl	ar2,#0x01
00106$:
;	_divuint.c 47
; Peephole 132   changed ljmp to sjmp
; Peephole 205   optimized misc jump sequence
	djnz r6,00105$
00116$:
00117$:
;	_divuint.c 49
	mov	dpl,r2
	mov	dph,r3
00108$:
	C$_divuint.c$50$1$1 ==.
	XG$_divuint$0$0 ==.
	ret
	.area	CSEG    (CODE)
