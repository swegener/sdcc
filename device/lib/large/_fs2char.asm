;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:23 2000

;--------------------------------------------------------
	.module _fs2char
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fs2char
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
___fs2char_sloc0_1_0:
	.ds	0x0004
___fs2char_sloc1_1_0:
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
___fs2char_a1_1_1:
	.ds	0x0004
___fs2char_fl1_1_1:
	.ds	0x0004
___fs2char_exp_1_1:
	.ds	0x0002
___fs2char_l_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fs2char$0$0 ==.
;	_fs2char.c 73
;	-----------------------------------------
;	 function __fs2char
;	-----------------------------------------
___fs2char:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fs2char.c 96
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fs2char_a1_1_1
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
;	_fs2char.c 79
	mov	dptr,#___fs2char_a1_1_1
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
	mov	dptr,#(___fs2char_fl1_1_1)
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
;	_fs2char.c 81
	mov	dptr,#(___fs2char_fl1_1_1)
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
00120$:
;	_fs2char.c 82
	mov	dpl,#0x00
	ljmp	00110$
00102$:
;	_fs2char.c 84
	mov	dptr,#(___fs2char_fl1_1_1)
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
	jnb	acc.0,00121$
	orl	a,#0xfe
00121$:
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r4,a
	mov	r5,a
	mov	a,r2
	mov	b,#0x00
	add	a,#0x6a
	mov	r2,a
	mov	a,b
	addc	a,#0xff
	mov	r3,a
	mov	dptr,#___fs2char_exp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	_fs2char.c 85
	mov	dptr,#(___fs2char_fl1_1_1)
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
	anl	ar6,#0x7f
	mov	r7,#0x00
	mov	dptr,#___fs2char_l_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	mov	a,#0x80
	orl	a,r6
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
;	_fs2char.c 87
	mov	dptr,#___fs2char_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	clr	c
; Peephole 180   changed mov to clr
	clr  a
	subb	a,r2
; Peephole 159   avoided xrl during execution
	mov  a,#(0x00 ^ 0x80)
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00104$
00122$:
;	_fs2char.c 88
	mov	dptr,#(___fs2char_fl1_1_1)
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
; Peephole 105   removed redundant mov
	mov  r7,a
	rl	a
	anl	a,#0x01
	mov	r2,a
	orl	ar2,#0x7f
	mov	dpl,r2
	ljmp	00110$
00104$:
;	_fs2char.c 91
	mov	dptr,#___fs2char_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r0,a
	jb	acc.7,00123$
	ljmp	00106$
00123$:
	mov	dptr,#___fs2char_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	clr	c
	mov	a,#0xe7
	subb	a,r2
; Peephole 159   avoided xrl during execution
	mov  a,#(0xff ^ 0x80)
	mov	b,r0
	xrl	b,#0x80
	subb	a,b
	jc	00124$
	ljmp	00106$
00124$:
	mov	dptr,#___fs2char_l_1_1
	movx	a,@dptr
	mov	___fs2char_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc0_1_0 + 3),a
	mov	a,___fs2char_sloc0_1_0
	orl	a,(___fs2char_sloc0_1_0 + 1)
	orl	a,(___fs2char_sloc0_1_0 + 2)
	orl	a,(___fs2char_sloc0_1_0 + 3)
	jnz	00125$
	ljmp	00106$
00125$:
;	_fs2char.c 92
	mov	dptr,#___fs2char_exp_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	clr	c
	clr	a
	subb	a,r2
	mov	___fs2char_sloc0_1_0,a
	clr	a
	subb	a,r0
	mov	(___fs2char_sloc0_1_0 + 1),a
	mov	dptr,#___fs2char_l_1_1
	movx	a,@dptr
	mov	___fs2char_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 3),a
	mov	b,___fs2char_sloc0_1_0
	inc	b
	mov	a,(___fs2char_sloc1_1_0 + 3)
	rlc	a
	mov	ov,c
	sjmp	00127$
00126$:
	mov	c,ov
	mov	a,(___fs2char_sloc1_1_0 + 3)
	rrc	a
	mov	(___fs2char_sloc1_1_0 + 3),a
	mov	a,(___fs2char_sloc1_1_0 + 2)
	rrc	a
	mov	(___fs2char_sloc1_1_0 + 2),a
	mov	a,(___fs2char_sloc1_1_0 + 1)
	rrc	a
	mov	(___fs2char_sloc1_1_0 + 1),a
	mov	a,___fs2char_sloc1_1_0
	rrc	a
	mov	___fs2char_sloc1_1_0,a
00127$:
	djnz	b,00126$
	mov	dptr,#___fs2char_l_1_1
	mov	a,___fs2char_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fs2char_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fs2char_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fs2char_sloc1_1_0 + 3)
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
;	_fs2char.c 94
	mov	dpl,#0x00
	ljmp	00110$
00107$:
;	_fs2char.c 96
	mov	dptr,#(___fs2char_fl1_1_1)
	movx	a,@dptr
	mov	___fs2char_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  (___fs2char_sloc1_1_0 + 3),a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00128$:
	mov	dptr,#___fs2char_l_1_1
	movx	a,@dptr
	mov	___fs2char_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 3),a
	clr	c
	clr	a
	subb	a,___fs2char_sloc1_1_0
	mov	___fs2char_sloc1_1_0,a
	clr	a
	subb	a,(___fs2char_sloc1_1_0 + 1)
	mov	(___fs2char_sloc1_1_0 + 1),a
	clr	a
	subb	a,(___fs2char_sloc1_1_0 + 2)
	mov	(___fs2char_sloc1_1_0 + 2),a
	clr	a
	subb	a,(___fs2char_sloc1_1_0 + 3)
	mov	(___fs2char_sloc1_1_0 + 3),a
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00112$:
	mov	dptr,#___fs2char_l_1_1
	movx	a,@dptr
	mov	___fs2char_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fs2char_sloc1_1_0 + 3),a
00113$:
	mov	dpl,___fs2char_sloc1_1_0
	mov	dph,(___fs2char_sloc1_1_0 + 1)
	mov	b,(___fs2char_sloc1_1_0 + 2)
	mov	a,(___fs2char_sloc1_1_0 + 3)
00110$:
	C$_fs2char.c$97$1$1 ==.
	XG$__fs2char$0$0 ==.
	ret
	.area	CSEG    (CODE)
