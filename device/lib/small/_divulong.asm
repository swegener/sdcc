;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:09 2000

;--------------------------------------------------------
	.module _divulong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divulong_PARM_2
	.globl __divulong
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
__divulong_PARM_2:
	.ds	0x0004
__divulong_a_1_1:
	.ds	0x0004
__divulong_reste_1_1:
	.ds	0x0004
__divulong_count_1_1:
	.ds	0x0001
__divulong_c_1_1:
	.ds	0x0001
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
	G$_divulong$0$0 ==.
;	_divulong.c 27
;	-----------------------------------------
;	 function _divulong
;	-----------------------------------------
__divulong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divulong.c 48
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_divulong.c 29
	mov	r6,#0x00
	mov	r7,#0x00
	mov	r0,#0x00
	mov	r1,#0x00
;	_divulong.c 33
	mov	__divulong_count_1_1,#0x20
00105$:
;	_divulong.c 35
	mov	a,r5
	rl	a
	anl	a,#0x01
	mov	__divulong_c_1_1,a
;	_divulong.c 36
	mov	a,r2
	add	a,acc
	mov	r2,a
	mov	a,r3
	rlc	a
	mov	r3,a
	mov	a,r4
	rlc	a
	mov	r4,a
	mov	a,r5
	rlc	a
	mov	r5,a
;	_divulong.c 37
	mov	a,r6
	add	a,acc
	mov	r6,a
	mov	a,r7
	rlc	a
	mov	r7,a
	mov	a,r0
	rlc	a
	mov	r0,a
	mov	a,r1
	rlc	a
	mov	r1,a
;	_divulong.c 38
	mov	a,__divulong_c_1_1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_divulong.c 39
	mov	__divulong_reste_1_1,r6
	mov	(__divulong_reste_1_1 + 1),r7
	mov	(__divulong_reste_1_1 + 2),r0
	mov	(__divulong_reste_1_1 + 3),r1
	mov	a,#0x01
	orl	a,__divulong_reste_1_1
	mov	r6,a
	mov	r7,(__divulong_reste_1_1 + 1)
	mov	r0,(__divulong_reste_1_1 + 2)
	mov	r1,(__divulong_reste_1_1 + 3)
00102$:
;	_divulong.c 41
	clr	c
	mov	a,r6
	subb	a,__divulong_PARM_2
	mov	a,r7
	subb	a,(__divulong_PARM_2 + 1)
	mov	a,r0
	subb	a,(__divulong_PARM_2 + 2)
	mov	a,r1
	subb	a,(__divulong_PARM_2 + 3)
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00106$
00115$:
;	_divulong.c 42
	clr	c
	mov	a,r6
	subb	a,__divulong_PARM_2
	mov	r6,a
	mov	a,r7
	subb	a,(__divulong_PARM_2 + 1)
	mov	r7,a
	mov	a,r0
	subb	a,(__divulong_PARM_2 + 2)
	mov	r0,a
	mov	a,r1
	subb	a,(__divulong_PARM_2 + 3)
	mov	r1,a
;	_divulong.c 44
	mov	__divulong_a_1_1,r2
	mov	(__divulong_a_1_1 + 1),r3
	mov	(__divulong_a_1_1 + 2),r4
	mov	(__divulong_a_1_1 + 3),r5
	mov	a,#0x01
	orl	a,__divulong_a_1_1
	mov	r2,a
	mov	r3,(__divulong_a_1_1 + 1)
	mov	r4,(__divulong_a_1_1 + 2)
	mov	r5,(__divulong_a_1_1 + 3)
00106$:
;	_divulong.c 46
	djnz	__divulong_count_1_1,00116$
	sjmp	00117$
00116$:
	ljmp	00105$
00117$:
;	_divulong.c 48
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
00108$:
	C$_divulong.c$49$1$1 ==.
	XG$_divulong$0$0 ==.
	ret
	.area	CSEG    (CODE)
