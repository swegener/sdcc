;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:12 2000

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
___fsmul_PARM_2:
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
___fsmul_sloc0_1_0:
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
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fsmul.c 88
	mov	___fsmul_fl1_1_1,r2
	mov	(___fsmul_fl1_1_1 + 1),r3
	mov	(___fsmul_fl1_1_1 + 2),r4
	mov	(___fsmul_fl1_1_1 + 3),r5
;	_fsmul.c 89
	mov	___fsmul_fl2_1_1,___fsmul_PARM_2
	mov	(___fsmul_fl2_1_1 + 1),(___fsmul_PARM_2 + 1)
	mov	(___fsmul_fl2_1_1 + 2),(___fsmul_PARM_2 + 2)
	mov	(___fsmul_fl2_1_1 + 3),(___fsmul_PARM_2 + 3)
;	_fsmul.c 91
	mov	a,___fsmul_fl1_1_1
	orl	a,(___fsmul_fl1_1_1 + 1)
	orl	a,(___fsmul_fl1_1_1 + 2)
	orl	a,(___fsmul_fl1_1_1 + 3)
; Peephole 110   removed ljmp by inverse jump logic
	jz  00101$
00114$:
	mov	a,___fsmul_fl2_1_1
	orl	a,(___fsmul_fl2_1_1 + 1)
	orl	a,(___fsmul_fl2_1_1 + 2)
	orl	a,(___fsmul_fl2_1_1 + 3)
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
	mov	a,(___fsmul_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
	mov	r2,a
	mov	a,(___fsmul_fl2_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r3,a
	xrl	a,ar2
	mov	___fsmul_sign_1_1,a
;	_fsmul.c 96
	mov	r3,(___fsmul_fl1_1_1 + 2)
	mov	a,(___fsmul_fl1_1_1 + 3)
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	xch	a,r3
	anl	a,#0x01
	jnb	acc.0,00116$
	orl	a,#0xfe
00116$:
	mov	r4,a
	rlc	a
	subb	a,acc
	mov	r5,a
	mov	r6,a
	mov	a,r3
	mov	b,#0x00
	add	a,#0x82
	mov	___fsmul_exp_1_1,a
	mov	a,b
	addc	a,#0xff
	mov	(___fsmul_exp_1_1 + 1),a
;	_fsmul.c 97
	mov	r3,(___fsmul_fl2_1_1 + 2)
	mov	a,(___fsmul_fl2_1_1 + 3)
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	xch	a,r3
	anl	a,#0x01
	jnb	acc.0,00117$
	orl	a,#0xfe
00117$:
	mov	r4,a
	rlc	a
	subb	a,acc
	mov	r7,a
	mov	r0,a
	mov	r4,#0x00
	mov	a,r3
	add	a,___fsmul_exp_1_1
	mov	___fsmul_exp_1_1,a
	mov	a,r4
	addc	a,(___fsmul_exp_1_1 + 1)
	mov	(___fsmul_exp_1_1 + 1),a
;	_fsmul.c 99
	mov	r3,___fsmul_fl1_1_1
	mov	r4,(___fsmul_fl1_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsmul_fl1_1_1 + 2)
	mov	r1,a
	mov	r2,#0x00
	orl	ar1,#0x80
	mov	___fsmul_fl1_1_1,r3
	mov	(___fsmul_fl1_1_1 + 1),r4
	mov	(___fsmul_fl1_1_1 + 2),r1
	mov	(___fsmul_fl1_1_1 + 3),r2
;	_fsmul.c 100
	mov	r2,___fsmul_fl2_1_1
	mov	r3,(___fsmul_fl2_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fsmul_fl2_1_1 + 2)
	mov	r4,a
	mov	r1,#0x00
	orl	ar4,#0x80
	mov	___fsmul_fl2_1_1,r2
	mov	(___fsmul_fl2_1_1 + 1),r3
	mov	(___fsmul_fl2_1_1 + 2),r4
	mov	(___fsmul_fl2_1_1 + 3),r1
;	_fsmul.c 103
	mov	r2,(___fsmul_fl1_1_1 + 1)
	mov	r3,(___fsmul_fl1_1_1 + 2)
	mov	a,(___fsmul_fl1_1_1 + 3)
	mov	r4,a
	rlc	a
	subb	a,acc
	mov	r1,a
	mov	__mulslong_PARM_2,(___fsmul_fl2_1_1 + 1)
	mov	(__mulslong_PARM_2 + 1),(___fsmul_fl2_1_1 + 2)
	mov	a,(___fsmul_fl2_1_1 + 3)
	mov	(__mulslong_PARM_2 + 2),a
	rlc	a
	subb	a,acc
	mov	(__mulslong_PARM_2 + 3),a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r1
	lcall	__mulslong
	mov	___fsmul_result_1_1,dpl
	mov	(___fsmul_result_1_1 + 1),dph
	mov	(___fsmul_result_1_1 + 2),b
	mov	(___fsmul_result_1_1 + 3),a
;	_fsmul.c 104
	mov	r2,___fsmul_fl1_1_1
	mov	r3,#0x00
	mov	r4,#0x00
	mov	r1,#0x00
	mov	__mululong_PARM_2,(___fsmul_fl2_1_1 + 1)
	mov	(__mululong_PARM_2 + 1),(___fsmul_fl2_1_1 + 2)
	mov	(__mululong_PARM_2 + 2),(___fsmul_fl2_1_1 + 3)
	mov	(__mululong_PARM_2 + 3),#0x00
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r1
	lcall	__mululong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r1,a
	mov	ar2,r3
	mov	ar3,r4
	mov	ar4,r1
	mov	r1,#0x00
	mov	a,r2
	add	a,___fsmul_result_1_1
	mov	___fsmul_result_1_1,a
	mov	a,r3
	addc	a,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),a
	mov	a,r4
	addc	a,(___fsmul_result_1_1 + 2)
	mov	(___fsmul_result_1_1 + 2),a
	mov	a,r1
	addc	a,(___fsmul_result_1_1 + 3)
	mov	(___fsmul_result_1_1 + 3),a
;	_fsmul.c 105
	mov	r2,___fsmul_fl2_1_1
	mov	r3,#0x00
	mov	r4,#0x00
	mov	r1,#0x00
	mov	__mululong_PARM_2,(___fsmul_fl1_1_1 + 1)
	mov	(__mululong_PARM_2 + 1),(___fsmul_fl1_1_1 + 2)
	mov	(__mululong_PARM_2 + 2),(___fsmul_fl1_1_1 + 3)
	mov	(__mululong_PARM_2 + 3),#0x00
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r1
	lcall	__mululong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r1,a
	mov	ar2,r3
	mov	ar3,r4
	mov	ar4,r1
	mov	r1,#0x00
	mov	a,r2
	add	a,___fsmul_result_1_1
	mov	___fsmul_result_1_1,a
	mov	a,r3
	addc	a,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),a
	mov	a,r4
	addc	a,(___fsmul_result_1_1 + 2)
	mov	(___fsmul_result_1_1 + 2),a
	mov	a,r1
	addc	a,(___fsmul_result_1_1 + 3)
;	_fsmul.c 107
; Peephole 105   removed redundant mov
	mov  (___fsmul_result_1_1 + 3),a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00105$
00118$:
;	_fsmul.c 110
	mov	a,#0x80
	add	a,___fsmul_result_1_1
	mov	___fsmul_result_1_1,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 2)
	mov	(___fsmul_result_1_1 + 2),a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 3)
	mov	(___fsmul_result_1_1 + 3),a
;	_fsmul.c 111
	mov	___fsmul_result_1_1,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),(___fsmul_result_1_1 + 2)
	mov	(___fsmul_result_1_1 + 2),(___fsmul_result_1_1 + 3)
	mov	(___fsmul_result_1_1 + 3),#0x00
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	_fsmul.c 116
	mov	a,#0x40
	add	a,___fsmul_result_1_1
	mov	___fsmul_result_1_1,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 2)
	mov	(___fsmul_result_1_1 + 2),a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(___fsmul_result_1_1 + 3)
	mov	(___fsmul_result_1_1 + 3),a
;	_fsmul.c 117
	mov	a,(___fsmul_result_1_1 + 1)
	mov	c,acc.7
	xch	a,___fsmul_result_1_1
	rlc	a
	xch	a,___fsmul_result_1_1
	rlc	a
	xch	a,___fsmul_result_1_1
	anl	a,#0x01
	mov	(___fsmul_result_1_1 + 1),a
	mov	a,(___fsmul_result_1_1 + 2)
	add	a,acc
	orl	a,(___fsmul_result_1_1 + 1)
	mov	(___fsmul_result_1_1 + 1),a
	mov	a,(___fsmul_result_1_1 + 3)
	mov	c,acc.7
	xch	a,(___fsmul_result_1_1 + 2)
	rlc	a
	xch	a,(___fsmul_result_1_1 + 2)
	rlc	a
	xch	a,(___fsmul_result_1_1 + 2)
	anl	a,#0x01
	mov	(___fsmul_result_1_1 + 3),a
;	_fsmul.c 118
	dec	___fsmul_exp_1_1
	mov	a,#0xff
	cjne	a,___fsmul_exp_1_1,00119$
	dec	(___fsmul_exp_1_1 + 1)
00119$:
00106$:
;	_fsmul.c 121
	anl	(___fsmul_result_1_1 + 2),#0x7f
;	_fsmul.c 124
	mov	a,___fsmul_sign_1_1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00120$:
	mov	___fsmul_sloc0_1_0,#0x00
	mov	(___fsmul_sloc0_1_0 + 1),#0x00
	mov	(___fsmul_sloc0_1_0 + 2),#0x00
	mov	(___fsmul_sloc0_1_0 + 3),#0x80
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00109$:
	clr	a
	mov	(___fsmul_sloc0_1_0 + 3),a
	mov	(___fsmul_sloc0_1_0 + 2),a
	mov	(___fsmul_sloc0_1_0 + 1),a
	mov	___fsmul_sloc0_1_0,a
00110$:
	mov	r2,___fsmul_exp_1_1
	mov	r3,(___fsmul_exp_1_1 + 1)
	mov	r4,#0x00
	mov	r1,#0x00
	mov	ar4,r2
	mov	a,r3
	anl	a,#0x01
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	r1,a
; Peephole 3.c   changed mov to clr
	clr  a
	mov  r3,a
	mov  r2,a
	mov  a,___fsmul_sloc0_1_0
	orl	ar2,a
	mov	a,(___fsmul_sloc0_1_0 + 1)
	orl	ar3,a
	mov	a,(___fsmul_sloc0_1_0 + 2)
	orl	ar4,a
	mov	a,(___fsmul_sloc0_1_0 + 3)
	orl	ar1,a
	mov	a,___fsmul_result_1_1
	orl	ar2,a
	mov	a,(___fsmul_result_1_1 + 1)
	orl	ar3,a
	mov	a,(___fsmul_result_1_1 + 2)
	orl	ar4,a
	mov	a,(___fsmul_result_1_1 + 3)
	orl	ar1,a
	mov	___fsmul_fl1_1_1,r2
	mov	(___fsmul_fl1_1_1 + 1),r3
	mov	(___fsmul_fl1_1_1 + 2),r4
	mov	(___fsmul_fl1_1_1 + 3),r1
;	_fsmul.c 125
	mov	dpl,___fsmul_fl1_1_1
	mov	dph,(___fsmul_fl1_1_1 + 1)
	mov	b,(___fsmul_fl1_1_1 + 2)
	mov	a,(___fsmul_fl1_1_1 + 3)
00107$:
	C$_fsmul.c$126$1$1 ==.
	XG$__fsmul$0$0 ==.
	ret
	.area	CSEG    (CODE)
