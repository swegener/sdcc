;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:11 2000

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
___fsadd_PARM_2:
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
___fsadd_sloc0_1_0:
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
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fsadd.c 22
	clr	a
	mov	(___fsadd_sign_1_1 + 3),a
	mov	(___fsadd_sign_1_1 + 2),a
	mov	(___fsadd_sign_1_1 + 1),a
	mov	___fsadd_sign_1_1,a
;	_fsadd.c 24
	mov	___fsadd_fl1_1_1,r2
	mov	(___fsadd_fl1_1_1 + 1),r3
	mov	(___fsadd_fl1_1_1 + 2),r4
	mov	(___fsadd_fl1_1_1 + 3),r5
;	_fsadd.c 25
	mov	___fsadd_fl2_1_1,___fsadd_PARM_2
	mov	(___fsadd_fl2_1_1 + 1),(___fsadd_PARM_2 + 1)
	mov	(___fsadd_fl2_1_1 + 2),(___fsadd_PARM_2 + 2)
	mov	(___fsadd_fl2_1_1 + 3),(___fsadd_PARM_2 + 3)
;	_fsadd.c 28
	mov	a,___fsadd_fl1_1_1
	orl	a,(___fsadd_fl1_1_1 + 1)
	orl	a,(___fsadd_fl1_1_1 + 2)
	orl	a,(___fsadd_fl1_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00146$:
;	_fsadd.c 29
	mov	dpl,___fsadd_fl2_1_1
	mov	dph,(___fsadd_fl2_1_1 + 1)
	mov	b,(___fsadd_fl2_1_1 + 2)
	mov	a,(___fsadd_fl2_1_1 + 3)
	ljmp	00128$
00102$:
;	_fsadd.c 30
	mov	a,___fsadd_fl2_1_1
	orl	a,(___fsadd_fl2_1_1 + 1)
	orl	a,(___fsadd_fl2_1_1 + 2)
	orl	a,(___fsadd_fl2_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00147$:
;	_fsadd.c 31
	mov	dpl,___fsadd_fl1_1_1
	mov	dph,(___fsadd_fl1_1_1 + 1)
	mov	b,(___fsadd_fl1_1_1 + 2)
	mov	a,(___fsadd_fl1_1_1 + 3)
	ljmp	00128$
00104$:
;	_fsadd.c 33
	mov	r2,(___fsadd_fl1_1_1 + 2)
	mov	a,(___fsadd_fl1_1_1 + 3)
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
	mov	___fsadd_exp1_1_1,r2
	mov	(___fsadd_exp1_1_1 + 1),#0x00
;	_fsadd.c 34
	mov	r2,(___fsadd_fl2_1_1 + 2)
	mov	a,(___fsadd_fl2_1_1 + 3)
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
	mov	___fsadd_exp2_1_1,r2
	mov	(___fsadd_exp2_1_1 + 1),#0x00
;	_fsadd.c 36
	mov	a,#0x19
	add	a,___fsadd_exp2_1_1
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsadd_exp2_1_1 + 1)
	mov	r3,a
	clr	c
	mov	a,r2
	subb	a,___fsadd_exp1_1_1
	mov	a,r3
	xrl	a,#0x80
	mov	b,(___fsadd_exp1_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00106$
00150$:
;	_fsadd.c 37
	mov	dpl,___fsadd_fl1_1_1
	mov	dph,(___fsadd_fl1_1_1 + 1)
	mov	b,(___fsadd_fl1_1_1 + 2)
	mov	a,(___fsadd_fl1_1_1 + 3)
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
	mov	a,#0x19
	add	a,___fsadd_exp1_1_1
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsadd_exp1_1_1 + 1)
	mov	r3,a
	clr	c
	mov	a,r2
	subb	a,___fsadd_exp2_1_1
	mov	a,r3
	xrl	a,#0x80
	mov	b,(___fsadd_exp2_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00108$
00151$:
;	_fsadd.c 39
	mov	dpl,___fsadd_fl2_1_1
	mov	dph,(___fsadd_fl2_1_1 + 1)
	mov	b,(___fsadd_fl2_1_1 + 2)
	mov	a,(___fsadd_fl2_1_1 + 3)
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
	mov	r2,___fsadd_fl1_1_1
	mov	r3,(___fsadd_fl1_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsadd_fl1_1_1 + 2)
	mov	r0,a
	mov	r1,#0x00
	orl	ar0,#0x80
	mov	(___fsadd_mant1_1_1 + 2),r0
	mov	a,r1
	anl	a,#0x03
	mov	c,acc.0
	xch	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 3),a
	mov	a,r3
	rr	a
	rr	a
	anl	a,#0x3f
	orl	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	___fsadd_mant1_1_1,r2
	mov	a,r3
	anl	a,#0x03
	mov	c,acc.0
	xch	a,___fsadd_mant1_1_1
	rrc	a
	xch	a,___fsadd_mant1_1_1
	rrc	a
	xch	a,___fsadd_mant1_1_1
	rrc	a
	xch	a,___fsadd_mant1_1_1
	rrc	a
	xch	a,___fsadd_mant1_1_1
	mov	(___fsadd_mant1_1_1 + 1),a
;	_fsadd.c 43
	mov	r2,___fsadd_fl2_1_1
	mov	r3,(___fsadd_fl2_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsadd_fl2_1_1 + 2)
	mov	r0,a
	mov	r1,#0x00
	orl	ar0,#0x80
	mov	(___fsadd_mant2_1_1 + 2),r0
	mov	a,r1
	anl	a,#0x03
	mov	c,acc.0
	xch	a,(___fsadd_mant2_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant2_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant2_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant2_1_1 + 2)
	rrc	a
	xch	a,(___fsadd_mant2_1_1 + 2)
	mov	(___fsadd_mant2_1_1 + 3),a
	mov	a,r3
	rr	a
	rr	a
	anl	a,#0x3f
	orl	a,(___fsadd_mant2_1_1 + 2)
	mov	(___fsadd_mant2_1_1 + 2),a
	mov	___fsadd_mant2_1_1,r2
	mov	a,r3
	anl	a,#0x03
	mov	c,acc.0
	xch	a,___fsadd_mant2_1_1
	rrc	a
	xch	a,___fsadd_mant2_1_1
	rrc	a
	xch	a,___fsadd_mant2_1_1
	rrc	a
	xch	a,___fsadd_mant2_1_1
	rrc	a
	xch	a,___fsadd_mant2_1_1
	mov	(___fsadd_mant2_1_1 + 1),a
;	_fsadd.c 45
	mov	a,(___fsadd_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00110$
00152$:
;	_fsadd.c 46
	clr	c
	clr	a
	subb	a,___fsadd_mant1_1_1
	mov	___fsadd_mant1_1_1,a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 1)
	mov	(___fsadd_mant1_1_1 + 1),a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 2),a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 3)
	mov	(___fsadd_mant1_1_1 + 3),a
00110$:
;	_fsadd.c 47
	mov	a,(___fsadd_fl2_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00153$:
;	_fsadd.c 48
	clr	c
	clr	a
	subb	a,___fsadd_mant2_1_1
	mov	___fsadd_mant2_1_1,a
	clr	a
	subb	a,(___fsadd_mant2_1_1 + 1)
	mov	(___fsadd_mant2_1_1 + 1),a
	clr	a
	subb	a,(___fsadd_mant2_1_1 + 2)
	mov	(___fsadd_mant2_1_1 + 2),a
	clr	a
	subb	a,(___fsadd_mant2_1_1 + 3)
	mov	(___fsadd_mant2_1_1 + 3),a
00112$:
;	_fsadd.c 50
	clr	c
	mov	a,___fsadd_exp2_1_1
	subb	a,___fsadd_exp1_1_1
	mov	a,(___fsadd_exp2_1_1 + 1)
	xrl	a,#0x80
	mov	b,(___fsadd_exp1_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00114$
00154$:
;	_fsadd.c 52
	clr	c
	mov	a,___fsadd_exp1_1_1
	subb	a,___fsadd_exp2_1_1
	mov	r2,a
	mov	a,(___fsadd_exp1_1_1 + 1)
	subb	a,(___fsadd_exp2_1_1 + 1)
	mov	r3,a
	mov	b,r2
	inc	b
	mov	a,(___fsadd_mant2_1_1 + 3)
	rlc	a
	mov	ov,c
	sjmp	00156$
00155$:
	mov	c,ov
	mov	a,(___fsadd_mant2_1_1 + 3)
	rrc	a
	mov	(___fsadd_mant2_1_1 + 3),a
	mov	a,(___fsadd_mant2_1_1 + 2)
	rrc	a
	mov	(___fsadd_mant2_1_1 + 2),a
	mov	a,(___fsadd_mant2_1_1 + 1)
	rrc	a
	mov	(___fsadd_mant2_1_1 + 1),a
	mov	a,___fsadd_mant2_1_1
	rrc	a
	mov	___fsadd_mant2_1_1,a
00156$:
	djnz	b,00155$
; Peephole 132   changed ljmp to sjmp
	sjmp 00115$
00114$:
;	_fsadd.c 56
	clr	c
	mov	a,___fsadd_exp2_1_1
	subb	a,___fsadd_exp1_1_1
	mov	r2,a
	mov	a,(___fsadd_exp2_1_1 + 1)
	subb	a,(___fsadd_exp1_1_1 + 1)
	mov	r3,a
	mov	b,r2
	inc	b
	mov	a,(___fsadd_mant1_1_1 + 3)
	rlc	a
	mov	ov,c
	sjmp	00158$
00157$:
	mov	c,ov
	mov	a,(___fsadd_mant1_1_1 + 3)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 3),a
	mov	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,(___fsadd_mant1_1_1 + 1)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,___fsadd_mant1_1_1
	rrc	a
	mov	___fsadd_mant1_1_1,a
00158$:
	djnz	b,00157$
;	_fsadd.c 57
	mov	___fsadd_exp1_1_1,___fsadd_exp2_1_1
	mov	(___fsadd_exp1_1_1 + 1),(___fsadd_exp2_1_1 + 1)
00115$:
;	_fsadd.c 59
	mov	a,___fsadd_mant2_1_1
	add	a,___fsadd_mant1_1_1
	mov	___fsadd_mant1_1_1,a
	mov	a,(___fsadd_mant2_1_1 + 1)
	addc	a,(___fsadd_mant1_1_1 + 1)
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,(___fsadd_mant2_1_1 + 2)
	addc	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,(___fsadd_mant2_1_1 + 3)
	addc	a,(___fsadd_mant1_1_1 + 3)
;	_fsadd.c 61
; Peephole 105   removed redundant mov
	mov  (___fsadd_mant1_1_1 + 3),a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00119$
00159$:
;	_fsadd.c 63
	clr	c
	clr	a
	subb	a,___fsadd_mant1_1_1
	mov	___fsadd_mant1_1_1,a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 1)
	mov	(___fsadd_mant1_1_1 + 1),a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 2),a
	clr	a
	subb	a,(___fsadd_mant1_1_1 + 3)
	mov	(___fsadd_mant1_1_1 + 3),a
;	_fsadd.c 64
	mov	___fsadd_sign_1_1,#0x00
	mov	(___fsadd_sign_1_1 + 1),#0x00
	mov	(___fsadd_sign_1_1 + 2),#0x00
	mov	(___fsadd_sign_1_1 + 3),#0x80
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00119$:
;	_fsadd.c 66
	mov	a,___fsadd_mant1_1_1
	orl	a,(___fsadd_mant1_1_1 + 1)
	orl	a,(___fsadd_mant1_1_1 + 2)
	orl	a,(___fsadd_mant1_1_1 + 3)
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
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	a,r1
	anl	a,#0xe0
; Peephole 162   removed sjmp by inverse jump logic
	jz   00162$
00161$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00123$
00162$:
;	_fsadd.c 72
	mov	a,___fsadd_mant1_1_1
	add	a,acc
	mov	___fsadd_mant1_1_1,a
	mov	a,(___fsadd_mant1_1_1 + 1)
	rlc	a
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,(___fsadd_mant1_1_1 + 2)
	rlc	a
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,(___fsadd_mant1_1_1 + 3)
	rlc	a
	mov	(___fsadd_mant1_1_1 + 3),a
;	_fsadd.c 73
	dec	___fsadd_exp1_1_1
	mov	a,#0xff
	cjne	a,___fsadd_exp1_1_1,00163$
	dec	(___fsadd_exp1_1_1 + 1)
00163$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00123$:
;	_fsadd.c 77
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	a,r1
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00125$
00164$:
;	_fsadd.c 79
	mov	a,(___fsadd_mant1_1_1 + 3)
	mov	c,acc.7
	rrc	a
	mov	(___fsadd_mant1_1_1 + 3),a
	mov	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,(___fsadd_mant1_1_1 + 1)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,___fsadd_mant1_1_1
	rrc	a
	mov	___fsadd_mant1_1_1,a
;	_fsadd.c 80
	inc	___fsadd_exp1_1_1
	clr	a
	cjne	a,___fsadd_exp1_1_1,00165$
	inc	(___fsadd_exp1_1_1 + 1)
00165$:
00125$:
;	_fsadd.c 84
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	a,r2
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00130$
00166$:
	mov	r2,#0x20
	mov	r3,#0x00
	mov	r0,#0x00
	mov	r1,#0x00
; Peephole 132   changed ljmp to sjmp
	sjmp 00131$
00130$:
	mov	r2,#0x1f
	mov	r3,#0x00
	mov	r0,#0x00
	mov	r1,#0x00
00131$:
	mov	a,r2
	add	a,___fsadd_mant1_1_1
	mov	___fsadd_mant1_1_1,a
	mov	a,r3
	addc	a,(___fsadd_mant1_1_1 + 1)
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,r0
	addc	a,(___fsadd_mant1_1_1 + 2)
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,r1
	addc	a,(___fsadd_mant1_1_1 + 3)
	mov	(___fsadd_mant1_1_1 + 3),a
;	_fsadd.c 87
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	a,r1
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.6,00127$
00167$:
;	_fsadd.c 89
	mov	a,(___fsadd_mant1_1_1 + 3)
	mov	c,acc.7
	rrc	a
	mov	(___fsadd_mant1_1_1 + 3),a
	mov	a,(___fsadd_mant1_1_1 + 2)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	a,(___fsadd_mant1_1_1 + 1)
	rrc	a
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,___fsadd_mant1_1_1
	rrc	a
	mov	___fsadd_mant1_1_1,a
;	_fsadd.c 90
	inc	___fsadd_exp1_1_1
	clr	a
	cjne	a,___fsadd_exp1_1_1,00168$
	inc	(___fsadd_exp1_1_1 + 1)
00168$:
00127$:
;	_fsadd.c 94
	mov	a,(___fsadd_mant1_1_1 + 1)
	mov	c,acc.7
	xch	a,___fsadd_mant1_1_1
	rlc	a
	xch	a,___fsadd_mant1_1_1
	rlc	a
	xch	a,___fsadd_mant1_1_1
	rlc	a
	xch	a,___fsadd_mant1_1_1
	rlc	a
	xch	a,___fsadd_mant1_1_1
	anl	a,#0x03
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,(___fsadd_mant1_1_1 + 2)
	add	a,acc
	add	a,acc
	orl	a,(___fsadd_mant1_1_1 + 1)
	mov	(___fsadd_mant1_1_1 + 1),a
	mov	a,(___fsadd_mant1_1_1 + 3)
	mov	c,acc.7
	xch	a,(___fsadd_mant1_1_1 + 2)
	rlc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rlc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rlc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	rlc	a
	xch	a,(___fsadd_mant1_1_1 + 2)
	anl	a,#0x03
	jnb	acc.1,00169$
	orl	a,#0xfc
00169$:
	mov	(___fsadd_mant1_1_1 + 3),a
;	_fsadd.c 97
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	___fsadd_mant1_1_1,r2
	mov	(___fsadd_mant1_1_1 + 1),r3
	mov	a,#0x7f
	anl	a,r0
	mov	(___fsadd_mant1_1_1 + 2),a
	mov	(___fsadd_mant1_1_1 + 3),r1
;	_fsadd.c 100
	mov	r2,___fsadd_exp1_1_1
	mov	r3,(___fsadd_exp1_1_1 + 1)
	mov	r0,#0x00
	mov	r1,#0x00
	mov	(___fsadd_sloc0_1_0 + 2),r2
	mov	a,r3
	anl	a,#0x01
	mov	c,acc.0
	xch	a,(___fsadd_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsadd_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsadd_sloc0_1_0 + 2)
	mov	(___fsadd_sloc0_1_0 + 3),a
	mov	(___fsadd_sloc0_1_0 + 1),#0x00
	mov	___fsadd_sloc0_1_0,#0x00
	mov	r2,___fsadd_sign_1_1
	mov	r3,(___fsadd_sign_1_1 + 1)
	mov	r0,(___fsadd_sign_1_1 + 2)
	mov	r1,(___fsadd_sign_1_1 + 3)
	mov	a,r2
	orl	___fsadd_sloc0_1_0,a
	mov	a,r3
	orl	(___fsadd_sloc0_1_0 + 1),a
	mov	a,r0
	orl	(___fsadd_sloc0_1_0 + 2),a
	mov	a,r1
	orl	(___fsadd_sloc0_1_0 + 3),a
	mov	r2,___fsadd_mant1_1_1
	mov	r3,(___fsadd_mant1_1_1 + 1)
	mov	r0,(___fsadd_mant1_1_1 + 2)
	mov	r1,(___fsadd_mant1_1_1 + 3)
	mov	a,___fsadd_sloc0_1_0
	orl	ar2,a
	mov	a,(___fsadd_sloc0_1_0 + 1)
	orl	ar3,a
	mov	a,(___fsadd_sloc0_1_0 + 2)
	orl	ar0,a
	mov	a,(___fsadd_sloc0_1_0 + 3)
	orl	ar1,a
	mov	___fsadd_fl1_1_1,r2
	mov	(___fsadd_fl1_1_1 + 1),r3
	mov	(___fsadd_fl1_1_1 + 2),r0
	mov	(___fsadd_fl1_1_1 + 3),r1
;	_fsadd.c 101
	mov	dpl,___fsadd_fl1_1_1
	mov	dph,(___fsadd_fl1_1_1 + 1)
	mov	b,(___fsadd_fl1_1_1 + 2)
	mov	a,(___fsadd_fl1_1_1 + 3)
00128$:
	C$_fsadd.c$102$1$1 ==.
	XG$__fsadd$0$0 ==.
	ret
	.area	CSEG    (CODE)
