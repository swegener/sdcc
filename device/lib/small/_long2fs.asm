;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _long2fs
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___long2fs
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
___long2fs_a1_1_1:
	.ds	0x0004
___long2fs_sign_1_1:
	.ds	0x0004
___long2fs_fl_1_1:
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
	G$__long2fs$0$0 ==.
;	_long2fs.c 80
;	-----------------------------------------
;	 function __long2fs
;	-----------------------------------------
___long2fs:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_long2fs.c 108
	mov	___long2fs_a1_1_1,dpl
	mov	(___long2fs_a1_1_1 + 1),dph
	mov	(___long2fs_a1_1_1 + 2),b
	mov	(___long2fs_a1_1_1 + 3),a
;	_long2fs.c 82
	clr	a
	mov	(___long2fs_sign_1_1 + 3),a
	mov	(___long2fs_sign_1_1 + 2),a
	mov	(___long2fs_sign_1_1 + 1),a
	mov	___long2fs_sign_1_1,a
;	_long2fs.c 86
	mov	a,___long2fs_a1_1_1
	orl	a,(___long2fs_a1_1_1 + 1)
	orl	a,(___long2fs_a1_1_1 + 2)
	orl	a,(___long2fs_a1_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00114$:
;	_long2fs.c 88
	mov	___long2fs_fl_1_1,#0x00
	mov	(___long2fs_fl_1_1 + 1),#0x00
	mov	(___long2fs_fl_1_1 + 2),#0x00
	mov	(___long2fs_fl_1_1 + 3),#0x00
;	_long2fs.c 89
	mov	dpl,___long2fs_fl_1_1
	mov	dph,(___long2fs_fl_1_1 + 1)
	mov	b,(___long2fs_fl_1_1 + 2)
	mov	a,(___long2fs_fl_1_1 + 3)
	ljmp	00108$
00102$:
;	_long2fs.c 92
	mov	a,(___long2fs_a1_1_1 + 3)
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00113$
00115$:
;	_long2fs.c 94
	mov	___long2fs_sign_1_1,#0x00
	mov	(___long2fs_sign_1_1 + 1),#0x00
	mov	(___long2fs_sign_1_1 + 2),#0x00
	mov	(___long2fs_sign_1_1 + 3),#0x80
;	_long2fs.c 95
	clr	c
	clr	a
	subb	a,___long2fs_a1_1_1
	mov	___long2fs_a1_1_1,a
	clr	a
	subb	a,(___long2fs_a1_1_1 + 1)
	mov	(___long2fs_a1_1_1 + 1),a
	clr	a
	subb	a,(___long2fs_a1_1_1 + 2)
	mov	(___long2fs_a1_1_1 + 2),a
	clr	a
	subb	a,(___long2fs_a1_1_1 + 3)
	mov	(___long2fs_a1_1_1 + 3),a
;	_long2fs.c 98
00113$:
	mov	r6,#0x96
	mov	r7,#0x00
00105$:
	clr	c
	mov	a,___long2fs_a1_1_1
	subb	a,#0x00
	mov	a,(___long2fs_a1_1_1 + 1)
	subb	a,#0x00
	mov	a,(___long2fs_a1_1_1 + 2)
	subb	a,#0x80
	mov	a,(___long2fs_a1_1_1 + 3)
	subb	a,#0x00
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00107$
00116$:
;	_long2fs.c 100
	mov	a,___long2fs_a1_1_1
	add	a,acc
	mov	___long2fs_a1_1_1,a
	mov	a,(___long2fs_a1_1_1 + 1)
	rlc	a
	mov	(___long2fs_a1_1_1 + 1),a
	mov	a,(___long2fs_a1_1_1 + 2)
	rlc	a
	mov	(___long2fs_a1_1_1 + 2),a
	mov	a,(___long2fs_a1_1_1 + 3)
	rlc	a
	mov	(___long2fs_a1_1_1 + 3),a
;	_long2fs.c 101
	dec	r6
	cjne	r6,#0xff,00117$
	dec	r7
00117$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00107$:
;	_long2fs.c 104
	anl	(___long2fs_a1_1_1 + 2),#0x7f
;	_long2fs.c 106
	mov	r0,#0x00
	mov	r1,#0x00
	mov	ar0,r6
	mov	a,r7
	anl	a,#0x01
	mov	c,acc.0
	xch	a,r0
	rrc	a
	xch	a,r0
	rrc	a
	xch	a,r0
	mov	r1,a
	mov	r7,#0x00
	mov	r6,#0x00
	mov	r2,___long2fs_sign_1_1
	mov	r3,(___long2fs_sign_1_1 + 1)
	mov	r4,(___long2fs_sign_1_1 + 2)
	mov	r5,(___long2fs_sign_1_1 + 3)
	mov	a,r2
	orl	ar6,a
	mov	a,r3
	orl	ar7,a
	mov	a,r4
	orl	ar0,a
	mov	a,r5
	orl	ar1,a
	mov	a,___long2fs_a1_1_1
	orl	ar6,a
	mov	a,(___long2fs_a1_1_1 + 1)
	orl	ar7,a
	mov	a,(___long2fs_a1_1_1 + 2)
	orl	ar0,a
	mov	a,(___long2fs_a1_1_1 + 3)
	orl	ar1,a
	mov	___long2fs_fl_1_1,r6
	mov	(___long2fs_fl_1_1 + 1),r7
	mov	(___long2fs_fl_1_1 + 2),r0
	mov	(___long2fs_fl_1_1 + 3),r1
;	_long2fs.c 108
	mov	dpl,___long2fs_fl_1_1
	mov	dph,(___long2fs_fl_1_1 + 1)
	mov	b,(___long2fs_fl_1_1 + 2)
	mov	a,(___long2fs_fl_1_1 + 3)
00108$:
	C$_long2fs.c$109$1$1 ==.
	XG$__long2fs$0$0 ==.
	ret
	.area	CSEG    (CODE)
