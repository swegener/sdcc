;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:25 2000

;--------------------------------------------------------
	.module _fsadd
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fsadd_PARM_2
	.globl ___fsadd
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
___fsadd_sloc0_1_0:
	.ds	0x0004
___fsadd_sloc1_1_0:
	.ds	0x0002
___fsadd_sloc2_1_0:
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
___fsadd_PARM_2:
	.ds	0x0004
___fsadd_a1_1_1:
	.ds	0x0004
___fsadd_mant1_1_1:
	.ds	0x0004
___fsadd_mant2_1_1:
	.ds	0x0004
___fsadd_fl1_1_1:
	.ds	0x0004
___fsadd_fl2_1_1:
	.ds	0x0004
___fsadd_exp1_1_1:
	.ds	0x0002
___fsadd_exp2_1_1:
	.ds	0x0002
___fsadd_sign_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fsadd$0$0 ==.
;	_fsadd.c 17
;	-----------------------------------------
;	 function __fsadd
;	-----------------------------------------
___fsadd:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fsadd.c 101
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fsadd_a1_1_1
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
;	_fsadd.c 22
	mov	dptr,#___fsadd_sign_1_1
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
;	_fsadd.c 24
	mov	dptr,#___fsadd_a1_1_1
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
	mov	dptr,#(___fsadd_fl1_1_1)
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
;	_fsadd.c 25
	mov	dptr,#___fsadd_PARM_2
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
	mov	dptr,#(___fsadd_fl2_1_1)
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
;	_fsadd.c 28
	mov	dptr,#(___fsadd_fl1_1_1)
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
00146$:
;	_fsadd.c 29
	mov	dptr,#(___fsadd_fl2_1_1)
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
; Peephole 191   removed redundant mov
	mov  r5,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r4
	ljmp	00128$
00102$:
;	_fsadd.c 30
	mov	dptr,#(___fsadd_fl2_1_1)
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
	jnz  00104$
00147$:
;	_fsadd.c 31
	mov	dptr,#(___fsadd_fl1_1_1)
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
; Peephole 191   removed redundant mov
	mov  r5,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r4
	ljmp	00128$
00104$:
;	_fsadd.c 33
	mov	dptr,#(___fsadd_fl1_1_1)
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
	jnb	acc.0,00148$
	orl	a,#0xfe
00148$:
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r4,a
	mov	r5,a
	mov	dptr,#___fsadd_exp1_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_fsadd.c 34
	mov	dptr,#(___fsadd_fl2_1_1)
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
	jnb	acc.0,00149$
	orl	a,#0xfe
00149$:
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	dptr,#___fsadd_exp2_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_fsadd.c 36
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,#0x19
	add	a,r2
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r3,a
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	clr	c
	mov	a,r2
	subb	a,r0
	mov	a,r3
	xrl	a,#0x80
	mov	b,r1
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00106$
00150$:
;	_fsadd.c 37
	mov	dptr,#(___fsadd_fl1_1_1)
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
; Peephole 191   removed redundant mov
	mov  r1,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r0
	lcall	___long2fs
	mov	r2,dpl
	mov	r3,dph
	mov	r0,b
; Peephole 191   removed redundant mov
	mov  r1,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r0
	ljmp	00128$
00106$:
;	_fsadd.c 38
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,#0x19
	add	a,r2
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r3,a
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	clr	c
	mov	a,r2
	subb	a,r0
	mov	a,r3
	xrl	a,#0x80
	mov	b,r1
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00108$
00151$:
;	_fsadd.c 39
	mov	dptr,#(___fsadd_fl2_1_1)
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
; Peephole 191   removed redundant mov
	mov  r1,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r0
	lcall	___long2fs
	mov	r2,dpl
	mov	r3,dph
	mov	r0,b
; Peephole 191   removed redundant mov
	mov  r1,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r0
	ljmp	00128$
00108$:
;	_fsadd.c 42
	mov	dptr,#(___fsadd_fl1_1_1)
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
	mov	r1,a
	anl	ar0,#0x7f
	mov	r1,#0x00
	orl	ar0,#0x80
	mov	a,r1
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	mov	r1,a
	mov	a,r3
	rr	a
	rr	a
	anl	a,#0x3f
	orl	a,r0
	mov	r0,a
	mov	a,r3
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	mov	r3,a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
;	_fsadd.c 43
	mov	dptr,#(___fsadd_fl2_1_1)
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
	mov	r1,a
	anl	ar0,#0x7f
	mov	r1,#0x00
	orl	ar0,#0x80
	mov	a,r1
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	mov	r1,a
	mov	a,r3
	rr	a
	rr	a
	anl	a,#0x3f
	orl	a,r0
	mov	r0,a
	mov	a,r3
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	mov	r3,a
	mov	dptr,#___fsadd_mant2_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
;	_fsadd.c 45
	mov	dptr,#(___fsadd_fl1_1_1)
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
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00110$
00152$:
;	_fsadd.c 46
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	clr	c
	clr	a
	subb	a,___fsadd_sloc0_1_0
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
00110$:
;	_fsadd.c 47
	mov	dptr,#(___fsadd_fl2_1_1)
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc0_1_0 + 3),a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00153$:
;	_fsadd.c 48
	mov	dptr,#___fsadd_mant2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	dptr,#___fsadd_mant2_1_1
	clr	c
	clr	a
	subb	a,___fsadd_sloc0_1_0
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
00112$:
;	_fsadd.c 50
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc1_1_0 + 1),a
	clr	c
	mov	a,___fsadd_sloc0_1_0
	subb	a,___fsadd_sloc1_1_0
	mov	a,(___fsadd_sloc0_1_0 + 1)
	xrl	a,#0x80
	mov	b,(___fsadd_sloc1_1_0 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	00154$
	ljmp	00114$
00154$:
;	_fsadd.c 52
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc1_1_0 + 1),a
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	clr	c
	mov	a,___fsadd_sloc0_1_0
	subb	a,___fsadd_sloc1_1_0
	mov	___fsadd_sloc1_1_0,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	subb	a,(___fsadd_sloc1_1_0 + 1)
	mov	(___fsadd_sloc1_1_0 + 1),a
	mov	dptr,#___fsadd_mant2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	b,___fsadd_sloc1_1_0
	inc	b
	mov	a,(___fsadd_sloc0_1_0 + 3)
	rlc	a
	mov	ov,c
	sjmp	00156$
00155$:
	mov	c,ov
	mov	a,(___fsadd_sloc0_1_0 + 3)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 2),a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 1),a
	mov	a,___fsadd_sloc0_1_0
	rrc	a
	mov	___fsadd_sloc0_1_0,a
00156$:
	djnz	b,00155$
	mov	dptr,#___fsadd_mant2_1_1
	mov	a,___fsadd_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 3)
	movx	@dptr,a
	ljmp	00115$
00114$:
;	_fsadd.c 56
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc1_1_0 + 1),a
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	clr	c
	mov	a,___fsadd_sloc0_1_0
	subb	a,___fsadd_sloc1_1_0
	mov	___fsadd_sloc1_1_0,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	subb	a,(___fsadd_sloc1_1_0 + 1)
	mov	(___fsadd_sloc1_1_0 + 1),a
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	b,___fsadd_sloc1_1_0
	inc	b
	mov	a,(___fsadd_sloc0_1_0 + 3)
	rlc	a
	mov	ov,c
	sjmp	00158$
00157$:
	mov	c,ov
	mov	a,(___fsadd_sloc0_1_0 + 3)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 2),a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	rrc	a
	mov	(___fsadd_sloc0_1_0 + 1),a
	mov	a,___fsadd_sloc0_1_0
	rrc	a
	mov	___fsadd_sloc0_1_0,a
00158$:
	djnz	b,00157$
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc0_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 57
	mov	dptr,#___fsadd_exp2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc1_1_0 + 1),a
	mov	dptr,#___fsadd_exp1_1_1
	mov	a,___fsadd_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc1_1_0 + 1)
	movx	@dptr,a
00115$:
;	_fsadd.c 59
	mov	dptr,#___fsadd_mant2_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc0_1_0
	add	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	addc	a,(___fsadd_sloc2_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	addc	a,(___fsadd_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsadd_sloc0_1_0 + 3)
	addc	a,(___fsadd_sloc2_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsadd.c 61
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00119$
00159$:
;	_fsadd.c 63
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	clr	c
	clr	a
	subb	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc2_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	clr	a
	subb	a,(___fsadd_sloc2_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsadd.c 64
	mov	dptr,#___fsadd_sign_1_1
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
	mov	a,#0x80
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00119$:
;	_fsadd.c 66
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,___fsadd_sloc2_1_0
	orl	a,(___fsadd_sloc2_1_0 + 1)
	orl	a,(___fsadd_sloc2_1_0 + 2)
	orl	a,(___fsadd_sloc2_1_0 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00121$
00160$:
;	_fsadd.c 67
; Peephole 3.a   changed mov to clr
; Peephole 3.b   changed mov to clr
; Peephole 3.b   changed mov to clr
	clr  a
	mov  dpl,a
	mov  dph,a
	mov  b,a
	ljmp	00128$
;	_fsadd.c 70
00121$:
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
	anl	a,#0xe0
; Peephole 162   removed sjmp by inverse jump logic
	jz   00162$
00161$:
	ljmp	00123$
00162$:
;	_fsadd.c 72
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,___fsadd_sloc2_1_0
	add	a,acc
	mov	___fsadd_sloc2_1_0,a
	mov	a,(___fsadd_sloc2_1_0 + 1)
	rlc	a
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	a,(___fsadd_sloc2_1_0 + 2)
	rlc	a
	mov	(___fsadd_sloc2_1_0 + 2),a
	mov	a,(___fsadd_sloc2_1_0 + 3)
	rlc	a
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 73
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	add	a,#0xff
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0xff
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	dptr,#___fsadd_exp1_1_1
	mov	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	ljmp	00121$
00123$:
;	_fsadd.c 77
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00125$
00163$:
;	_fsadd.c 79
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
	mov	c,acc.7
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,(___fsadd_sloc2_1_0 + 2)
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 2),a
	mov	a,(___fsadd_sloc2_1_0 + 1)
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	a,___fsadd_sloc2_1_0
	rrc	a
; Peephole 100   removed redundant mov
	mov  ___fsadd_sloc2_1_0,a
	mov  dptr,#___fsadd_mant1_1_1
	movx @dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 80
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	add	a,#0x01
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0x00
	movx	@dptr,a
00125$:
;	_fsadd.c 84
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,___fsadd_sloc2_1_0
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00130$
00164$:
	clr	a
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	(___fsadd_sloc2_1_0 + 2),a
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	___fsadd_sloc2_1_0,#0x20
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00130$:
	clr	a
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	(___fsadd_sloc2_1_0 + 2),a
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	___fsadd_sloc2_1_0,#0x1f
00131$:
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc2_1_0
	add	a,___fsadd_sloc0_1_0
	movx	@dptr,a
	mov	a,(___fsadd_sloc2_1_0 + 1)
	addc	a,(___fsadd_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsadd_sloc2_1_0 + 2)
	addc	a,(___fsadd_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,(___fsadd_sloc2_1_0 + 3)
	addc	a,(___fsadd_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_fsadd.c 87
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00127$
00165$:
;	_fsadd.c 89
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
	mov	c,acc.7
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,(___fsadd_sloc2_1_0 + 2)
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 2),a
	mov	a,(___fsadd_sloc2_1_0 + 1)
	rrc	a
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	a,___fsadd_sloc2_1_0
	rrc	a
; Peephole 100   removed redundant mov
	mov  ___fsadd_sloc2_1_0,a
	mov  dptr,#___fsadd_mant1_1_1
	movx @dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 90
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	add	a,#0x01
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0x00
	movx	@dptr,a
00127$:
;	_fsadd.c 94
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	a,(___fsadd_sloc2_1_0 + 1)
	mov	c,acc.7
	xch	a,___fsadd_sloc2_1_0
	rlc	a
	xch	a,___fsadd_sloc2_1_0
	rlc	a
	xch	a,___fsadd_sloc2_1_0
	rlc	a
	xch	a,___fsadd_sloc2_1_0
	rlc	a
	xch	a,___fsadd_sloc2_1_0
	anl	a,#0x03
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	a,(___fsadd_sloc2_1_0 + 2)
	add	a,acc
	add	a,acc
	orl	a,(___fsadd_sloc2_1_0 + 1)
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	a,(___fsadd_sloc2_1_0 + 3)
	mov	c,acc.7
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rlc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rlc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rlc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rlc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	anl	a,#0x03
	jnb	acc.1,00166$
	orl	a,#0xfc
00166$:
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 97
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	mov	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	mov	a,#0x7f
	anl	a,(___fsadd_sloc2_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 100
	mov	dptr,#___fsadd_exp1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	mov	(___fsadd_sloc2_1_0 + 2),___fsadd_sloc2_1_0
	mov	a,(___fsadd_sloc2_1_0 + 1)
	anl	a,#0x01
	mov	c,acc.0
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rrc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	rrc	a
	xch	a,(___fsadd_sloc2_1_0 + 2)
	mov	(___fsadd_sloc2_1_0 + 3),a
	mov	(___fsadd_sloc2_1_0 + 1),#0x00
	mov	___fsadd_sloc2_1_0,#0x00
	mov	dptr,#___fsadd_sign_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	a,___fsadd_sloc0_1_0
	orl	___fsadd_sloc2_1_0,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	orl	(___fsadd_sloc2_1_0 + 1),a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	orl	(___fsadd_sloc2_1_0 + 2),a
	mov	a,(___fsadd_sloc0_1_0 + 3)
	orl	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#___fsadd_mant1_1_1
	movx	a,@dptr
	mov	___fsadd_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	a,___fsadd_sloc0_1_0
	orl	___fsadd_sloc2_1_0,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	orl	(___fsadd_sloc2_1_0 + 1),a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	orl	(___fsadd_sloc2_1_0 + 2),a
	mov	a,(___fsadd_sloc0_1_0 + 3)
	orl	(___fsadd_sloc2_1_0 + 3),a
	mov	dptr,#(___fsadd_fl1_1_1)
	mov	a,___fsadd_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsadd_sloc2_1_0 + 3)
	movx	@dptr,a
;	_fsadd.c 101
	mov	dptr,#(___fsadd_fl1_1_1)
	movx	a,@dptr
	mov	___fsadd_sloc2_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsadd_sloc2_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 191   removed redundant mov
	mov  (___fsadd_sloc2_1_0 + 3),a
	mov  dpl,___fsadd_sloc2_1_0
	mov  dph,(___fsadd_sloc2_1_0 + 1)
	mov  b,(___fsadd_sloc2_1_0 + 2)
00128$:
	C$_fsadd.c$102$1$1 ==.
	XG$__fsadd$0$0 ==.
	ret
	.area	CSEG    (CODE)
