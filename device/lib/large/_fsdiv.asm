;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:26 2000

;--------------------------------------------------------
	.module _fsdiv
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fsdiv_PARM_2
	.globl ___fsdiv
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
___fsdiv_sloc0_1_0:
	.ds	0x0001
___fsdiv_sloc1_1_0:
	.ds	0x0004
___fsdiv_sloc2_1_0:
	.ds	0x0004
___fsdiv_sloc3_1_0:
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
___fsdiv_PARM_2:
	.ds	0x0004
___fsdiv_a1_1_1:
	.ds	0x0004
___fsdiv_fl1_1_1:
	.ds	0x0004
___fsdiv_fl2_1_1:
	.ds	0x0004
___fsdiv_result_1_1:
	.ds	0x0004
___fsdiv_mask_1_1:
	.ds	0x0004
___fsdiv_mant1_1_1:
	.ds	0x0004
___fsdiv_mant2_1_1:
	.ds	0x0004
___fsdiv_exp_1_1:
	.ds	0x0002
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fsdiv$0$0 ==.
;	_fsdiv.c 73
;	-----------------------------------------
;	 function __fsdiv
;	-----------------------------------------
___fsdiv:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fsdiv.c 138
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fsdiv_a1_1_1
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
;	_fsdiv.c 82
	mov	dptr,#___fsdiv_a1_1_1
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
	mov	dptr,#(___fsdiv_fl1_1_1)
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
;	_fsdiv.c 83
	mov	dptr,#___fsdiv_PARM_2
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
	mov	dptr,#(___fsdiv_fl2_1_1)
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
;	_fsdiv.c 86
	mov	dptr,#(___fsdiv_fl1_1_1)
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
	mov	ar2,r4
	mov	a,r5
	mov	c,acc.7
	xch	a,r2
	rlc	a
	xch	a,r2
	rlc	a
	xch	a,r2
	anl	a,#0x01
	jnb	acc.0,00120$
	orl	a,#0xfe
00120$:
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r4,a
	mov	r5,a
	mov	dptr,#___fsdiv_exp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_fsdiv.c 87
	mov	dptr,#(___fsdiv_fl2_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	ar2,r6
	mov	a,r7
	mov	c,acc.7
	xch	a,r2
	rlc	a
	xch	a,r2
	rlc	a
	xch	a,r2
	anl	a,#0x01
	jnb	acc.0,00121$
	orl	a,#0xfe
00121$:
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	r3,#0x00
	mov	dptr,#___fsdiv_exp_1_1
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#___fsdiv_exp_1_1
	clr	c
	mov	a,r0
	subb	a,r2
	movx	@dptr,a
	mov	a,r1
	subb	a,r3
	inc	dptr
	movx	@dptr,a
;	_fsdiv.c 88
	mov	dptr,#___fsdiv_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#___fsdiv_exp_1_1
	mov	a,#0x7e
	add	a,r2
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
;	_fsdiv.c 91
	mov	dptr,#(___fsdiv_fl1_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r1,a
	rl	a
	anl	a,#0x01
	mov	___fsdiv_sloc0_1_0,a
	mov	dptr,#(___fsdiv_fl2_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsdiv_sloc1_1_0 + 3),a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
	xrl	a,___fsdiv_sloc0_1_0
	mov	___fsdiv_sloc1_1_0,a
;	_fsdiv.c 94
	mov	dptr,#(___fsdiv_fl2_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	a,___fsdiv_sloc2_1_0
	orl	a,(___fsdiv_sloc2_1_0 + 1)
	orl	a,(___fsdiv_sloc2_1_0 + 2)
	orl	a,(___fsdiv_sloc2_1_0 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00122$:
;	_fsdiv.c 96
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x80
	mov	a,#0xbf
	ljmp	00112$
00102$:
;	_fsdiv.c 99
	mov	dptr,#(___fsdiv_fl1_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	a,___fsdiv_sloc2_1_0
	orl	a,(___fsdiv_sloc2_1_0 + 1)
	orl	a,(___fsdiv_sloc2_1_0 + 2)
	orl	a,(___fsdiv_sloc2_1_0 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00123$:
;	_fsdiv.c 100
; Peephole 3.a   changed mov to clr
; Peephole 3.b   changed mov to clr
; Peephole 3.b   changed mov to clr
	clr  a
	mov  dpl,a
	mov  dph,a
	mov  b,a
	ljmp	00112$
00104$:
;	_fsdiv.c 103
	mov	dptr,#(___fsdiv_fl1_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	anl	(___fsdiv_sloc2_1_0 + 2),#0x7f
	mov	(___fsdiv_sloc2_1_0 + 3),#0x00
	mov	dptr,#___fsdiv_mant1_1_1
	mov	a,___fsdiv_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	movx	@dptr,a
	mov	a,#0x80
	orl	a,(___fsdiv_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 104
	mov	dptr,#(___fsdiv_fl2_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	anl	(___fsdiv_sloc2_1_0 + 2),#0x7f
	mov	(___fsdiv_sloc2_1_0 + 3),#0x00
	mov	dptr,#___fsdiv_mant2_1_1
	mov	a,___fsdiv_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	movx	@dptr,a
	mov	a,#0x80
	orl	a,(___fsdiv_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 107
	mov	dptr,#___fsdiv_mant2_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	clr	c
	mov	a,___fsdiv_sloc3_1_0
	subb	a,___fsdiv_sloc2_1_0
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	subb	a,(___fsdiv_sloc2_1_0 + 1)
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	subb	a,(___fsdiv_sloc2_1_0 + 2)
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	xrl	a,#0x80
	mov	b,(___fsdiv_sloc2_1_0 + 3)
	xrl	b,#0x80
	subb	a,b
	jc	00124$
	ljmp	00106$
00124$:
;	_fsdiv.c 109
	mov	dptr,#___fsdiv_mant1_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	a,___fsdiv_sloc3_1_0
	add	a,acc
	mov	___fsdiv_sloc3_1_0,a
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	mov	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 110
	mov	dptr,#___fsdiv_exp_1_1
	movx	a,@dptr
	add	a,#0xff
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0xff
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	dptr,#___fsdiv_exp_1_1
	mov	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
00106$:
;	_fsdiv.c 114
	mov	dptr,#___fsdiv_mask_1_1
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
	inc	dptr
	mov	a,#0x01
	movx	@dptr,a
;	_fsdiv.c 115
	mov	dptr,#___fsdiv_result_1_1
	clr	a
	inc	dptr
	inc	dptr
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
;	_fsdiv.c 116
00109$:
	mov	dptr,#___fsdiv_mask_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	a,___fsdiv_sloc3_1_0
	orl	a,(___fsdiv_sloc3_1_0 + 1)
	orl	a,(___fsdiv_sloc3_1_0 + 2)
	orl	a,(___fsdiv_sloc3_1_0 + 3)
	jnz	00125$
	ljmp	00111$
00125$:
;	_fsdiv.c 118
	mov	dptr,#___fsdiv_mant2_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	clr	c
	mov	a,___fsdiv_sloc2_1_0
	subb	a,___fsdiv_sloc3_1_0
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	subb	a,(___fsdiv_sloc3_1_0 + 1)
	mov	a,(___fsdiv_sloc2_1_0 + 2)
	subb	a,(___fsdiv_sloc3_1_0 + 2)
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	xrl	a,#0x80
	mov	b,(___fsdiv_sloc3_1_0 + 3)
	xrl	b,#0x80
	subb	a,b
	jnc	00126$
	ljmp	00108$
00126$:
;	_fsdiv.c 120
	mov	dptr,#___fsdiv_mask_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_result_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	dptr,#___fsdiv_result_1_1
	mov	a,___fsdiv_sloc3_1_0
	orl	a,___fsdiv_sloc2_1_0
	movx	@dptr,a
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	orl	a,(___fsdiv_sloc2_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	orl	a,(___fsdiv_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	orl	a,(___fsdiv_sloc2_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsdiv.c 121
	mov	dptr,#___fsdiv_mant2_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	clr	c
	mov	a,___fsdiv_sloc2_1_0
	subb	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	subb	a,(___fsdiv_sloc3_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsdiv_sloc2_1_0 + 2)
	subb	a,(___fsdiv_sloc3_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	subb	a,(___fsdiv_sloc3_1_0 + 3)
	inc	dptr
	movx	@dptr,a
00108$:
;	_fsdiv.c 123
	mov	dptr,#___fsdiv_mant1_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	a,___fsdiv_sloc3_1_0
	add	a,acc
	mov	___fsdiv_sloc3_1_0,a
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	rlc	a
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_mant1_1_1
	mov	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 124
	mov	dptr,#___fsdiv_mask_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 106   removed redundant mov 
	mov  (___fsdiv_sloc3_1_0 + 3),a
	clr  c
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,___fsdiv_sloc3_1_0
	rrc	a
; Peephole 100   removed redundant mov
	mov  ___fsdiv_sloc3_1_0,a
	mov  dptr,#___fsdiv_mask_1_1
	movx @dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
	ljmp	00109$
00111$:
;	_fsdiv.c 128
	mov	dptr,#___fsdiv_result_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_result_1_1
	mov	a,#0x01
	add	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsdiv_sloc3_1_0 + 1)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsdiv_sloc3_1_0 + 2)
	inc	dptr
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsdiv_sloc3_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsdiv.c 131
	mov	dptr,#___fsdiv_exp_1_1
	movx	a,@dptr
	add	a,#0x01
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0x00
	movx	@dptr,a
;	_fsdiv.c 132
	mov	dptr,#___fsdiv_result_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsdiv_sloc3_1_0 + 3),a
	mov	c,acc.7
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	rrc	a
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,___fsdiv_sloc3_1_0
	rrc	a
; Peephole 100   removed redundant mov
	mov  ___fsdiv_sloc3_1_0,a
	mov  dptr,#___fsdiv_result_1_1
	movx @dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 134
	mov	dptr,#___fsdiv_result_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_result_1_1
	mov	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	mov	a,#0x7f
	anl	a,(___fsdiv_sloc3_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 137
	mov	dptr,#___fsdiv_exp_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	mov	(___fsdiv_sloc3_1_0 + 2),___fsdiv_sloc3_1_0
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	anl	a,#0x01
	mov	c,acc.0
	xch	a,(___fsdiv_sloc3_1_0 + 2)
	rrc	a
	xch	a,(___fsdiv_sloc3_1_0 + 2)
	rrc	a
	xch	a,(___fsdiv_sloc3_1_0 + 2)
	mov	(___fsdiv_sloc3_1_0 + 3),a
	mov	(___fsdiv_sloc3_1_0 + 1),#0x00
	mov	___fsdiv_sloc3_1_0,#0x00
	mov	___fsdiv_sloc2_1_0,___fsdiv_sloc1_1_0
; Peephole 3.c   changed mov to clr
; Peephole 3.b   changed mov to clr
	clr  a
	mov  (___fsdiv_sloc2_1_0 + 1),a
	mov  (___fsdiv_sloc2_1_0 + 2),a
	mov  (___fsdiv_sloc2_1_0 + 3),a
	mov  a,___fsdiv_sloc2_1_0
	orl	___fsdiv_sloc3_1_0,a
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	orl	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,(___fsdiv_sloc2_1_0 + 2)
	orl	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	orl	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#___fsdiv_result_1_1
	movx	a,@dptr
	mov	___fsdiv_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc2_1_0 + 3),a
	mov	a,___fsdiv_sloc2_1_0
	orl	___fsdiv_sloc3_1_0,a
	mov	a,(___fsdiv_sloc2_1_0 + 1)
	orl	(___fsdiv_sloc3_1_0 + 1),a
	mov	a,(___fsdiv_sloc2_1_0 + 2)
	orl	(___fsdiv_sloc3_1_0 + 2),a
	mov	a,(___fsdiv_sloc2_1_0 + 3)
	orl	(___fsdiv_sloc3_1_0 + 3),a
	mov	dptr,#(___fsdiv_fl1_1_1)
	mov	a,___fsdiv_sloc3_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsdiv_sloc3_1_0 + 3)
	movx	@dptr,a
;	_fsdiv.c 138
	mov	dptr,#(___fsdiv_fl1_1_1)
	movx	a,@dptr
	mov	___fsdiv_sloc3_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsdiv_sloc3_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 191   removed redundant mov
	mov  (___fsdiv_sloc3_1_0 + 3),a
	mov  dpl,___fsdiv_sloc3_1_0
	mov  dph,(___fsdiv_sloc3_1_0 + 1)
	mov  b,(___fsdiv_sloc3_1_0 + 2)
00112$:
	C$_fsdiv.c$139$1$1 ==.
	XG$__fsdiv$0$0 ==.
	ret
	.area	CSEG    (CODE)
