;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:11 2000

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
	.ds	0x0004
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
___fsdiv_PARM_2:
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
___fsdiv_sign_1_1:
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
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fsdiv.c 82
	mov	___fsdiv_fl1_1_1,r2
	mov	(___fsdiv_fl1_1_1 + 1),r3
	mov	(___fsdiv_fl1_1_1 + 2),r4
	mov	(___fsdiv_fl1_1_1 + 3),r5
;	_fsdiv.c 83
	mov	___fsdiv_fl2_1_1,___fsdiv_PARM_2
	mov	(___fsdiv_fl2_1_1 + 1),(___fsdiv_PARM_2 + 1)
	mov	(___fsdiv_fl2_1_1 + 2),(___fsdiv_PARM_2 + 2)
	mov	(___fsdiv_fl2_1_1 + 3),(___fsdiv_PARM_2 + 3)
;	_fsdiv.c 86
	mov	r2,(___fsdiv_fl1_1_1 + 2)
	mov	a,(___fsdiv_fl1_1_1 + 3)
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
	mov	___fsdiv_exp_1_1,r2
	mov	(___fsdiv_exp_1_1 + 1),#0x00
;	_fsdiv.c 87
	mov	r2,(___fsdiv_fl2_1_1 + 2)
	mov	a,(___fsdiv_fl2_1_1 + 3)
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
	clr	c
	mov	a,___fsdiv_exp_1_1
	subb	a,r2
	mov	___fsdiv_exp_1_1,a
	mov	a,(___fsdiv_exp_1_1 + 1)
	subb	a,r3
	mov	(___fsdiv_exp_1_1 + 1),a
;	_fsdiv.c 88
	mov	a,#0x7e
	add	a,___fsdiv_exp_1_1
	mov	___fsdiv_exp_1_1,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsdiv_exp_1_1 + 1)
	mov	(___fsdiv_exp_1_1 + 1),a
;	_fsdiv.c 91
	mov	a,(___fsdiv_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
	mov	r2,a
	mov	a,(___fsdiv_fl2_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r3,a
	xrl	a,ar2
	mov	___fsdiv_sign_1_1,a
;	_fsdiv.c 94
	mov	a,___fsdiv_fl2_1_1
	orl	a,(___fsdiv_fl2_1_1 + 1)
	orl	a,(___fsdiv_fl2_1_1 + 2)
	orl	a,(___fsdiv_fl2_1_1 + 3)
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
	mov	a,___fsdiv_fl1_1_1
	orl	a,(___fsdiv_fl1_1_1 + 1)
	orl	a,(___fsdiv_fl1_1_1 + 2)
	orl	a,(___fsdiv_fl1_1_1 + 3)
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
	mov	r3,___fsdiv_fl1_1_1
	mov	r0,(___fsdiv_fl1_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsdiv_fl1_1_1 + 2)
	mov	r1,a
	mov	r2,#0x00
	mov	___fsdiv_mant1_1_1,r3
	mov	(___fsdiv_mant1_1_1 + 1),r0
	mov	a,#0x80
	orl	a,r1
	mov	(___fsdiv_mant1_1_1 + 2),a
	mov	(___fsdiv_mant1_1_1 + 3),r2
;	_fsdiv.c 104
	mov	r2,___fsdiv_fl2_1_1
	mov	r3,(___fsdiv_fl2_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsdiv_fl2_1_1 + 2)
	mov	r0,a
	mov	r1,#0x00
	mov	___fsdiv_mant2_1_1,r2
	mov	(___fsdiv_mant2_1_1 + 1),r3
	mov	a,#0x80
	orl	a,r0
	mov	(___fsdiv_mant2_1_1 + 2),a
	mov	(___fsdiv_mant2_1_1 + 3),r1
;	_fsdiv.c 107
	clr	c
	mov	a,___fsdiv_mant1_1_1
	subb	a,___fsdiv_mant2_1_1
	mov	a,(___fsdiv_mant1_1_1 + 1)
	subb	a,(___fsdiv_mant2_1_1 + 1)
	mov	a,(___fsdiv_mant1_1_1 + 2)
	subb	a,(___fsdiv_mant2_1_1 + 2)
	mov	a,(___fsdiv_mant1_1_1 + 3)
	xrl	a,#0x80
	mov	b,(___fsdiv_mant2_1_1 + 3)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00106$
00124$:
;	_fsdiv.c 109
	mov	a,___fsdiv_mant1_1_1
	add	a,acc
	mov	___fsdiv_mant1_1_1,a
	mov	a,(___fsdiv_mant1_1_1 + 1)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 1),a
	mov	a,(___fsdiv_mant1_1_1 + 2)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 2),a
	mov	a,(___fsdiv_mant1_1_1 + 3)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 3),a
;	_fsdiv.c 110
	dec	___fsdiv_exp_1_1
	mov	a,#0xff
	cjne	a,___fsdiv_exp_1_1,00125$
	dec	(___fsdiv_exp_1_1 + 1)
00125$:
00106$:
;	_fsdiv.c 114
	mov	___fsdiv_mask_1_1,#0x00
	mov	(___fsdiv_mask_1_1 + 1),#0x00
	mov	(___fsdiv_mask_1_1 + 2),#0x00
	mov	(___fsdiv_mask_1_1 + 3),#0x01
;	_fsdiv.c 115
	clr	a
	mov	(___fsdiv_result_1_1 + 3),a
	mov	(___fsdiv_result_1_1 + 2),a
	mov	(___fsdiv_result_1_1 + 1),a
	mov	___fsdiv_result_1_1,a
;	_fsdiv.c 116
00109$:
	mov	a,___fsdiv_mask_1_1
	orl	a,(___fsdiv_mask_1_1 + 1)
	orl	a,(___fsdiv_mask_1_1 + 2)
	orl	a,(___fsdiv_mask_1_1 + 3)
	jnz	00126$
	ljmp	00111$
00126$:
;	_fsdiv.c 118
	clr	c
	mov	a,___fsdiv_mant1_1_1
	subb	a,___fsdiv_mant2_1_1
	mov	a,(___fsdiv_mant1_1_1 + 1)
	subb	a,(___fsdiv_mant2_1_1 + 1)
	mov	a,(___fsdiv_mant1_1_1 + 2)
	subb	a,(___fsdiv_mant2_1_1 + 2)
	mov	a,(___fsdiv_mant1_1_1 + 3)
	xrl	a,#0x80
	mov	b,(___fsdiv_mant2_1_1 + 3)
	xrl	b,#0x80
	subb	a,b
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00108$
00127$:
;	_fsdiv.c 120
	mov	r2,___fsdiv_result_1_1
	mov	r3,(___fsdiv_result_1_1 + 1)
	mov	r0,(___fsdiv_result_1_1 + 2)
	mov	r1,(___fsdiv_result_1_1 + 3)
	mov	a,___fsdiv_mask_1_1
	orl	a,r2
	mov	___fsdiv_result_1_1,a
	mov	a,(___fsdiv_mask_1_1 + 1)
	orl	a,r3
	mov	(___fsdiv_result_1_1 + 1),a
	mov	a,(___fsdiv_mask_1_1 + 2)
	orl	a,r0
	mov	(___fsdiv_result_1_1 + 2),a
	mov	a,(___fsdiv_mask_1_1 + 3)
	orl	a,r1
	mov	(___fsdiv_result_1_1 + 3),a
;	_fsdiv.c 121
	clr	c
	mov	a,___fsdiv_mant1_1_1
	subb	a,___fsdiv_mant2_1_1
	mov	___fsdiv_mant1_1_1,a
	mov	a,(___fsdiv_mant1_1_1 + 1)
	subb	a,(___fsdiv_mant2_1_1 + 1)
	mov	(___fsdiv_mant1_1_1 + 1),a
	mov	a,(___fsdiv_mant1_1_1 + 2)
	subb	a,(___fsdiv_mant2_1_1 + 2)
	mov	(___fsdiv_mant1_1_1 + 2),a
	mov	a,(___fsdiv_mant1_1_1 + 3)
	subb	a,(___fsdiv_mant2_1_1 + 3)
	mov	(___fsdiv_mant1_1_1 + 3),a
00108$:
;	_fsdiv.c 123
	mov	a,___fsdiv_mant1_1_1
	add	a,acc
	mov	___fsdiv_mant1_1_1,a
	mov	a,(___fsdiv_mant1_1_1 + 1)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 1),a
	mov	a,(___fsdiv_mant1_1_1 + 2)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 2),a
	mov	a,(___fsdiv_mant1_1_1 + 3)
	rlc	a
	mov	(___fsdiv_mant1_1_1 + 3),a
;	_fsdiv.c 124
	clr	c
	mov	a,(___fsdiv_mask_1_1 + 3)
	rrc	a
	mov	(___fsdiv_mask_1_1 + 3),a
	mov	a,(___fsdiv_mask_1_1 + 2)
	rrc	a
	mov	(___fsdiv_mask_1_1 + 2),a
	mov	a,(___fsdiv_mask_1_1 + 1)
	rrc	a
	mov	(___fsdiv_mask_1_1 + 1),a
	mov	a,___fsdiv_mask_1_1
	rrc	a
	mov	___fsdiv_mask_1_1,a
	ljmp	00109$
00111$:
;	_fsdiv.c 128
	inc	___fsdiv_result_1_1
	clr	a
	cjne	a,___fsdiv_result_1_1,00128$
	inc	(___fsdiv_result_1_1 + 1)
	cjne	a,(___fsdiv_result_1_1 + 1),00128$
	inc	(___fsdiv_result_1_1 + 2)
	cjne	a,(___fsdiv_result_1_1 + 2),00128$
	inc	(___fsdiv_result_1_1 + 3)
00128$:
;	_fsdiv.c 131
	inc	___fsdiv_exp_1_1
	clr	a
	cjne	a,___fsdiv_exp_1_1,00129$
	inc	(___fsdiv_exp_1_1 + 1)
00129$:
;	_fsdiv.c 132
	mov	a,(___fsdiv_result_1_1 + 3)
	mov	c,acc.7
	rrc	a
	mov	(___fsdiv_result_1_1 + 3),a
	mov	a,(___fsdiv_result_1_1 + 2)
	rrc	a
	mov	(___fsdiv_result_1_1 + 2),a
	mov	a,(___fsdiv_result_1_1 + 1)
	rrc	a
	mov	(___fsdiv_result_1_1 + 1),a
	mov	a,___fsdiv_result_1_1
	rrc	a
	mov	___fsdiv_result_1_1,a
;	_fsdiv.c 134
	mov	r2,___fsdiv_result_1_1
	mov	r3,(___fsdiv_result_1_1 + 1)
	mov	r0,(___fsdiv_result_1_1 + 2)
	mov	r1,(___fsdiv_result_1_1 + 3)
	mov	___fsdiv_result_1_1,r2
	mov	(___fsdiv_result_1_1 + 1),r3
	mov	a,#0x7f
	anl	a,r0
	mov	(___fsdiv_result_1_1 + 2),a
	mov	(___fsdiv_result_1_1 + 3),r1
;	_fsdiv.c 137
	mov	r2,___fsdiv_exp_1_1
	mov	r3,(___fsdiv_exp_1_1 + 1)
	mov	r0,#0x00
	mov	r1,#0x00
	mov	(___fsdiv_sloc0_1_0 + 2),r2
	mov	a,r3
	anl	a,#0x01
	mov	c,acc.0
	xch	a,(___fsdiv_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsdiv_sloc0_1_0 + 2)
	rrc	a
	xch	a,(___fsdiv_sloc0_1_0 + 2)
	mov	(___fsdiv_sloc0_1_0 + 3),a
	mov	(___fsdiv_sloc0_1_0 + 1),#0x00
	mov	___fsdiv_sloc0_1_0,#0x00
	mov	r2,___fsdiv_sign_1_1
; Peephole 3.c   changed mov to clr
; Peephole 3.b   changed mov to clr
	clr  a
	mov  r3,a
	mov  r0,a
	mov  r1,a
	mov  a,r2
	orl	___fsdiv_sloc0_1_0,a
	mov	a,r3
	orl	(___fsdiv_sloc0_1_0 + 1),a
	mov	a,r0
	orl	(___fsdiv_sloc0_1_0 + 2),a
	mov	a,r1
	orl	(___fsdiv_sloc0_1_0 + 3),a
	mov	r2,___fsdiv_result_1_1
	mov	r3,(___fsdiv_result_1_1 + 1)
	mov	r0,(___fsdiv_result_1_1 + 2)
	mov	r1,(___fsdiv_result_1_1 + 3)
	mov	a,___fsdiv_sloc0_1_0
	orl	ar2,a
	mov	a,(___fsdiv_sloc0_1_0 + 1)
	orl	ar3,a
	mov	a,(___fsdiv_sloc0_1_0 + 2)
	orl	ar0,a
	mov	a,(___fsdiv_sloc0_1_0 + 3)
	orl	ar1,a
	mov	___fsdiv_fl1_1_1,r2
	mov	(___fsdiv_fl1_1_1 + 1),r3
	mov	(___fsdiv_fl1_1_1 + 2),r0
	mov	(___fsdiv_fl1_1_1 + 3),r1
;	_fsdiv.c 138
	mov	dpl,___fsdiv_fl1_1_1
	mov	dph,(___fsdiv_fl1_1_1 + 1)
	mov	b,(___fsdiv_fl1_1_1 + 2)
	mov	a,(___fsdiv_fl1_1_1 + 3)
00112$:
	C$_fsdiv.c$139$1$1 ==.
	XG$__fsdiv$0$0 ==.
	ret
	.area	CSEG    (CODE)
