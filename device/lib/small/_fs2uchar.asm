;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:10 2000

;--------------------------------------------------------
	.module _fs2uchar
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fs2uchar
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
___fs2uchar_fl1_1_1:
	.ds	0x0004
___fs2uchar_exp_1_1:
	.ds	0x0002
___fs2uchar_l_1_1:
	.ds	0x0004
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
	G$__fs2uchar$0$0 ==.
;	_fs2uchar.c 73
;	-----------------------------------------
;	 function __fs2uchar
;	-----------------------------------------
___fs2uchar:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fs2uchar.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fs2uchar.c 79
	mov	___fs2uchar_fl1_1_1,r2
	mov	(___fs2uchar_fl1_1_1 + 1),r3
	mov	(___fs2uchar_fl1_1_1 + 2),r4
	mov	(___fs2uchar_fl1_1_1 + 3),r5
;	_fs2uchar.c 81
	mov	a,___fs2uchar_fl1_1_1
	orl	a,(___fs2uchar_fl1_1_1 + 1)
	orl	a,(___fs2uchar_fl1_1_1 + 2)
	orl	a,(___fs2uchar_fl1_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00120$:
;	_fs2uchar.c 82
	mov	dpl,#0x00
	ljmp	00110$
00102$:
;	_fs2uchar.c 84
	mov	r2,(___fs2uchar_fl1_1_1 + 2)
	mov	a,(___fs2uchar_fl1_1_1 + 3)
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
	mov	___fs2uchar_exp_1_1,a
	mov	a,b
	addc	a,#0xff
	mov	(___fs2uchar_exp_1_1 + 1),a
;	_fs2uchar.c 85
	mov	r2,___fs2uchar_fl1_1_1
	mov	r3,(___fs2uchar_fl1_1_1 + 1)
	mov	a,#0x7f
	anl	a,(___fs2uchar_fl1_1_1 + 2)
	mov	r6,a
	mov	r7,#0x00
	mov	___fs2uchar_l_1_1,r2
	mov	(___fs2uchar_l_1_1 + 1),r3
	mov	a,#0x80
	orl	a,r6
	mov	(___fs2uchar_l_1_1 + 2),a
	mov	(___fs2uchar_l_1_1 + 3),r7
;	_fs2uchar.c 87
	clr	c
; Peephole 180   changed mov to clr
	clr  a
	subb	a,___fs2uchar_exp_1_1
; Peephole 159   avoided xrl during execution
	mov  a,#(0x00 ^ 0x80)
	mov	b,(___fs2uchar_exp_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00104$
00122$:
;	_fs2uchar.c 88
	mov	a,(___fs2uchar_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
	mov	r2,a
	mov	a,#0x7f
	orl	a,r2
	mov	dpl,a
	ljmp	00110$
00104$:
;	_fs2uchar.c 91
	mov	a,(___fs2uchar_exp_1_1 + 1)
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00106$
00123$:
	clr	c
	mov	a,#0xe7
	subb	a,___fs2uchar_exp_1_1
; Peephole 159   avoided xrl during execution
	mov  a,#(0xff ^ 0x80)
	mov	b,(___fs2uchar_exp_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00106$
00124$:
	mov	a,___fs2uchar_l_1_1
	orl	a,(___fs2uchar_l_1_1 + 1)
	orl	a,(___fs2uchar_l_1_1 + 2)
	orl	a,(___fs2uchar_l_1_1 + 3)
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00125$:
;	_fs2uchar.c 92
	clr	c
	clr	a
	subb	a,___fs2uchar_exp_1_1
	mov	r2,a
	clr	a
	subb	a,(___fs2uchar_exp_1_1 + 1)
	mov	r3,a
	mov	b,r2
	inc	b
	mov	a,(___fs2uchar_l_1_1 + 3)
	rlc	a
	mov	ov,c
	sjmp	00127$
00126$:
	mov	c,ov
	mov	a,(___fs2uchar_l_1_1 + 3)
	rrc	a
	mov	(___fs2uchar_l_1_1 + 3),a
	mov	a,(___fs2uchar_l_1_1 + 2)
	rrc	a
	mov	(___fs2uchar_l_1_1 + 2),a
	mov	a,(___fs2uchar_l_1_1 + 1)
	rrc	a
	mov	(___fs2uchar_l_1_1 + 1),a
	mov	a,___fs2uchar_l_1_1
	rrc	a
	mov	___fs2uchar_l_1_1,a
00127$:
	djnz	b,00126$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
;	_fs2uchar.c 94
	mov	dpl,#0x00
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00107$:
;	_fs2uchar.c 96
	mov	a,(___fs2uchar_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00128$:
	mov	r2,___fs2uchar_l_1_1
	clr	c
	clr	a
	subb	a,r2
	mov	r2,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00112$:
	mov	r2,___fs2uchar_l_1_1
00113$:
	mov	dpl,r2
00110$:
	C$_fs2uchar.c$97$1$1 ==.
	XG$__fs2uchar$0$0 ==.
	ret
	.area	CSEG    (CODE)
