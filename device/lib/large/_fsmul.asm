;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:28 2000

;--------------------------------------------------------
	.module _fsmul
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fsmul_PARM_2
	.globl ___fsmul
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
___fsmul_sloc0_1_0:
	.ds	0x0004
___fsmul_sloc1_1_0:
	.ds	0x0004
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
___fsmul_PARM_2:
	.ds	0x0004
___fsmul_a1_1_1:
	.ds	0x0004
___fsmul_fl1_1_1:
	.ds	0x0004
___fsmul_fl2_1_1:
	.ds	0x0004
___fsmul_result_1_1:
	.ds	0x0004
___fsmul_exp_1_1:
	.ds	0x0002
___fsmul_sign_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fsmul$0$0 ==.
;	_fsmul.c 81
;	-----------------------------------------
;	 function __fsmul
;	-----------------------------------------
___fsmul:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fsmul.c 125
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fsmul_a1_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 88
	mov	dptr,#___fsmul_a1_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#(___fsmul_fl1_1_1)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_fsmul.c 89
	mov	dptr,#___fsmul_PARM_2
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#(___fsmul_fl2_1_1)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_fsmul.c 91
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r2
	orl	a,r3
	orl	a,r4
	orl	a,r5
; Peephole 110   removed ljmp by inverse jump logic
	jz  00101$
00114$:
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r2
	orl	a,r3
	orl	a,r4
	orl	a,r5
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00115$:
00101$:
;	_fsmul.c 92
; Peephole 3.a   changed mov to clr
; Peephole 3.b   changed mov to clr
; Peephole 3.b   changed mov to clr
	clr  a
	mov  dpl,a
	mov  dph,a
	mov  b,a
	ljmp	00107$
00102$:
;	_fsmul.c 95
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r5,a
	rl	a
	anl	a,#0x01
	mov	r2,a
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r1,a
	rl	a
	anl	a,#0x01
	mov	r6,a
	mov	dptr,#___fsmul_sign_1_1
	mov	a,r6
	xrl	a,ar2
	movx	@dptr,a
;	_fsmul.c 96
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	___fsmul_sloc0_1_0,(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	mov	c,acc.7
	xch	a,___fsmul_sloc0_1_0
	rlc	a
	xch	a,___fsmul_sloc0_1_0
	rlc	a
	xch	a,___fsmul_sloc0_1_0
	anl	a,#0x01
	jnb	acc.0,00116$
	orl	a,#0xfe
00116$:
	mov	(___fsmul_sloc0_1_0 + 1),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc0_1_0 + 2),a
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	r2,___fsmul_sloc0_1_0
	mov	r6,(___fsmul_sloc0_1_0 + 1)
	mov	a,r2
	mov	b,#0x00
	add	a,#0x82
	mov	r2,a
	mov	a,b
	addc	a,#0xff
	mov	r6,a
	mov	dptr,#___fsmul_exp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
;	_fsmul.c 97
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	___fsmul_sloc0_1_0,(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	mov	c,acc.7
	xch	a,___fsmul_sloc0_1_0
	rlc	a
	xch	a,___fsmul_sloc0_1_0
	rlc	a
	xch	a,___fsmul_sloc0_1_0
	anl	a,#0x01
	jnb	acc.0,00117$
	orl	a,#0xfe
00117$:
	mov	(___fsmul_sloc0_1_0 + 1),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc0_1_0 + 2),a
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	r2,___fsmul_sloc0_1_0
	mov	r6,(___fsmul_sloc0_1_0 + 1)
	mov	___fsmul_sloc0_1_0,r2
	mov	(___fsmul_sloc0_1_0 + 1),#0x00
	mov	dptr,#___fsmul_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#___fsmul_exp_1_1
	mov	a,___fsmul_sloc0_1_0
	add	a,r2
	movx	@dptr,a
	mov	a,(___fsmul_sloc0_1_0 + 1)
	addc	a,r6
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 99
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	anl	(___fsmul_sloc0_1_0 + 2),#0x7f
	mov	(___fsmul_sloc0_1_0 + 3),#0x00
	orl	(___fsmul_sloc0_1_0 + 2),#0x80
	mov	dptr,#(___fsmul_fl1_1_1)
	mov	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 100
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	anl	(___fsmul_sloc0_1_0 + 2),#0x7f
	mov	(___fsmul_sloc0_1_0 + 3),#0x00
	orl	(___fsmul_sloc0_1_0 + 2),#0x80
	mov	dptr,#(___fsmul_fl2_1_1)
	mov	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 103
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	___fsmul_sloc0_1_0,(___fsmul_sloc0_1_0 + 1)
	mov	(___fsmul_sloc0_1_0 + 1),(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	mov	(___fsmul_sloc0_1_0 + 2),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	___fsmul_sloc1_1_0,(___fsmul_sloc1_1_0 + 1)
	mov	(___fsmul_sloc1_1_0 + 1),(___fsmul_sloc1_1_0 + 2)
	mov	a,(___fsmul_sloc1_1_0 + 3)
	mov	(___fsmul_sloc1_1_0 + 2),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#__mulslong_PARM_2
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
	mov	dpl,___fsmul_sloc0_1_0
	mov	dph,(___fsmul_sloc0_1_0 + 1)
	mov	b,(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	lcall	__mulslong
	mov	___fsmul_sloc1_1_0,dpl
	mov	(___fsmul_sloc1_1_0 + 1),dph
	mov	(___fsmul_sloc1_1_0 + 2),b
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 104
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	(___fsmul_sloc1_1_0 + 1),#0x00
	mov	(___fsmul_sloc1_1_0 + 2),#0x00
	mov	(___fsmul_sloc1_1_0 + 3),#0x00
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	___fsmul_sloc0_1_0,(___fsmul_sloc0_1_0 + 1)
	mov	(___fsmul_sloc0_1_0 + 1),(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	mov	(___fsmul_sloc0_1_0 + 2),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	dptr,#__mululong_PARM_2
	mov	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 3)
	movx	@dptr,a
	mov	dpl,___fsmul_sloc1_1_0
	mov	dph,(___fsmul_sloc1_1_0 + 1)
	mov	b,(___fsmul_sloc1_1_0 + 2)
	mov	a,(___fsmul_sloc1_1_0 + 3)
	lcall	__mululong
	mov	___fsmul_sloc1_1_0,dpl
	mov	(___fsmul_sloc1_1_0 + 1),dph
	mov	(___fsmul_sloc1_1_0 + 2),b
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	___fsmul_sloc1_1_0,(___fsmul_sloc1_1_0 + 1)
	mov	(___fsmul_sloc1_1_0 + 1),(___fsmul_sloc1_1_0 + 2)
	mov	(___fsmul_sloc1_1_0 + 2),(___fsmul_sloc1_1_0 + 3)
	mov	(___fsmul_sloc1_1_0 + 3),#0x00
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	add	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 1)
	addc	a,(___fsmul_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 2)
	addc	a,(___fsmul_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 3)
	addc	a,(___fsmul_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 105
	mov	dptr,#(___fsmul_fl2_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	(___fsmul_sloc1_1_0 + 1),#0x00
	mov	(___fsmul_sloc1_1_0 + 2),#0x00
	mov	(___fsmul_sloc1_1_0 + 3),#0x00
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	___fsmul_sloc0_1_0,(___fsmul_sloc0_1_0 + 1)
	mov	(___fsmul_sloc0_1_0 + 1),(___fsmul_sloc0_1_0 + 2)
	mov	a,(___fsmul_sloc0_1_0 + 3)
	mov	(___fsmul_sloc0_1_0 + 2),a
	rlc	a
	subb	a,acc
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	dptr,#__mululong_PARM_2
	mov	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc0_1_0 + 3)
	movx	@dptr,a
	mov	dpl,___fsmul_sloc1_1_0
	mov	dph,(___fsmul_sloc1_1_0 + 1)
	mov	b,(___fsmul_sloc1_1_0 + 2)
	mov	a,(___fsmul_sloc1_1_0 + 3)
	lcall	__mululong
	mov	___fsmul_sloc1_1_0,dpl
	mov	(___fsmul_sloc1_1_0 + 1),dph
	mov	(___fsmul_sloc1_1_0 + 2),b
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	___fsmul_sloc1_1_0,(___fsmul_sloc1_1_0 + 1)
	mov	(___fsmul_sloc1_1_0 + 1),(___fsmul_sloc1_1_0 + 2)
	mov	(___fsmul_sloc1_1_0 + 2),(___fsmul_sloc1_1_0 + 3)
	mov	(___fsmul_sloc1_1_0 + 3),#0x00
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	add	a,___fsmul_sloc0_1_0
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 1)
	addc	a,(___fsmul_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 2)
	addc	a,(___fsmul_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsmul_sloc1_1_0 + 3)
	addc	a,(___fsmul_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 107
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsmul_sloc1_1_0 + 3),a
	jb	acc.7,00118$
	ljmp	00105$
00118$:
;	_fsmul.c 110
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,#0x80
	add	a,___fsmul_sloc1_1_0
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 1)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 2)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 111
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	___fsmul_sloc1_1_0,(___fsmul_sloc1_1_0 + 1)
	mov	(___fsmul_sloc1_1_0 + 1),(___fsmul_sloc1_1_0 + 2)
	mov	(___fsmul_sloc1_1_0 + 2),(___fsmul_sloc1_1_0 + 3)
	mov	(___fsmul_sloc1_1_0 + 3),#0x00
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
	ljmp	00106$
00105$:
;	_fsmul.c 116
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,#0x40
	add	a,___fsmul_sloc1_1_0
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 1)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 2)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_sloc1_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsmul.c 117
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	a,(___fsmul_sloc1_1_0 + 1)
	mov	c,acc.7
	xch	a,___fsmul_sloc1_1_0
	rlc	a
	xch	a,___fsmul_sloc1_1_0
	rlc	a
	xch	a,___fsmul_sloc1_1_0
	anl	a,#0x01
	mov	(___fsmul_sloc1_1_0 + 1),a
	mov	a,(___fsmul_sloc1_1_0 + 2)
	add	a,acc
	orl	a,(___fsmul_sloc1_1_0 + 1)
	mov	(___fsmul_sloc1_1_0 + 1),a
	mov	a,(___fsmul_sloc1_1_0 + 3)
	mov	c,acc.7
	xch	a,(___fsmul_sloc1_1_0 + 2)
	rlc	a
	xch	a,(___fsmul_sloc1_1_0 + 2)
	rlc	a
	xch	a,(___fsmul_sloc1_1_0 + 2)
	anl	a,#0x01
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 118
	mov	dptr,#___fsmul_exp_1_1
	movx	a,@dptr
	add	a,#0xff
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0xff
	mov	r6,a
	mov	dptr,#___fsmul_exp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
00106$:
;	_fsmul.c 121
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	mov	a,#0x7f
	anl	a,(___fsmul_sloc1_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 124
	mov	dptr,#___fsmul_sign_1_1
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00119$:
	mov	___fsmul_sloc1_1_0,#0x00
	mov	(___fsmul_sloc1_1_0 + 1),#0x00
	mov	(___fsmul_sloc1_1_0 + 2),#0x00
	mov	(___fsmul_sloc1_1_0 + 3),#0x80
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00109$:
	clr	a
	mov	(___fsmul_sloc1_1_0 + 3),a
	mov	(___fsmul_sloc1_1_0 + 2),a
	mov	(___fsmul_sloc1_1_0 + 1),a
	mov	___fsmul_sloc1_1_0,a
00110$:
	mov	dptr,#___fsmul_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	___fsmul_sloc0_1_0,r2
	mov	(___fsmul_sloc0_1_0 + 1),r6
	mov	(___fsmul_sloc0_1_0 + 2),#0x00
	mov	(___fsmul_sloc0_1_0 + 3),#0x00
	mov	(___fsmul_sloc0_1_0 + 2),___fsmul_sloc0_1_0
	mov	a,(___fsmul_sloc0_1_0 + 1)
	anl	a,#0x01
	mov	c,acc.0
	xch	a,(___fsmul_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsmul_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsmul_sloc0_1_0 + 2)
	mov	(___fsmul_sloc0_1_0 + 3),a
; Peephole 3.c   changed mov to clr
	clr  a
	mov  (___fsmul_sloc0_1_0 + 1),a
; Peephole 105   removed redundant mov
	mov  ___fsmul_sloc0_1_0,a
	orl	___fsmul_sloc1_1_0,a
	mov	a,(___fsmul_sloc0_1_0 + 1)
	orl	(___fsmul_sloc1_1_0 + 1),a
	mov	a,(___fsmul_sloc0_1_0 + 2)
	orl	(___fsmul_sloc1_1_0 + 2),a
	mov	a,(___fsmul_sloc0_1_0 + 3)
	orl	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#___fsmul_result_1_1
	movx	a,@dptr
	mov	___fsmul_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	a,___fsmul_sloc0_1_0
	orl	___fsmul_sloc1_1_0,a
	mov	a,(___fsmul_sloc0_1_0 + 1)
	orl	(___fsmul_sloc1_1_0 + 1),a
	mov	a,(___fsmul_sloc0_1_0 + 2)
	orl	(___fsmul_sloc1_1_0 + 2),a
	mov	a,(___fsmul_sloc0_1_0 + 3)
	orl	(___fsmul_sloc1_1_0 + 3),a
	mov	dptr,#(___fsmul_fl1_1_1)
	mov	a,___fsmul_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsmul_sloc1_1_0 + 3)
	movx	@dptr,a
;	_fsmul.c 125
	mov	dptr,#(___fsmul_fl1_1_1)
	movx	a,@dptr
	mov	___fsmul_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsmul_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 191   removed redundant mov
	mov  (___fsmul_sloc1_1_0 + 3),a
	mov  dpl,___fsmul_sloc1_1_0
	mov  dph,(___fsmul_sloc1_1_0 + 1)
	mov  b,(___fsmul_sloc1_1_0 + 2)
00107$:
	C$_fsmul.c$126$1$1 ==.
	XG$__fsmul$0$0 ==.
	ret
	.area	CSEG    (CODE)
