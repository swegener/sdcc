;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:17 2000

;--------------------------------------------------------
	.module _ulong2fs
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___ulong2fs
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
___ulong2fs_sign_1_1:
	.ds	0x0004
___ulong2fs_fl_1_1:
	.ds	0x0004
___ulong2fs_a1_1_1:
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
	G$__ulong2fs$0$0 ==.
;	_ulong2fs.c 80
;	-----------------------------------------
;	 function __ulong2fs
;	-----------------------------------------
___ulong2fs:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_ulong2fs.c 109
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_ulong2fs.c 82
	clr	a
	mov	(___ulong2fs_sign_1_1 + 3),a
	mov	(___ulong2fs_sign_1_1 + 2),a
	mov	(___ulong2fs_sign_1_1 + 1),a
	mov	___ulong2fs_sign_1_1,a
;	_ulong2fs.c 85
	mov	___ulong2fs_a1_1_1,r2
	mov	(___ulong2fs_a1_1_1 + 1),r3
	mov	(___ulong2fs_a1_1_1 + 2),r4
	mov	(___ulong2fs_a1_1_1 + 3),r5
;	_ulong2fs.c 87
	mov	a,r2
	orl	a,r3
	orl	a,r4
	orl	a,r5
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00114$:
;	_ulong2fs.c 89
	mov	___ulong2fs_fl_1_1,#0x00
	mov	(___ulong2fs_fl_1_1 + 1),#0x00
	mov	(___ulong2fs_fl_1_1 + 2),#0x00
	mov	(___ulong2fs_fl_1_1 + 3),#0x00
;	_ulong2fs.c 90
	mov	dpl,___ulong2fs_fl_1_1
	mov	dph,(___ulong2fs_fl_1_1 + 1)
	mov	b,(___ulong2fs_fl_1_1 + 2)
	mov	a,(___ulong2fs_fl_1_1 + 3)
	ljmp	00108$
00102$:
;	_ulong2fs.c 93
	mov	a,r5
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00113$
00115$:
;	_ulong2fs.c 95
	mov	___ulong2fs_sign_1_1,#0x00
	mov	(___ulong2fs_sign_1_1 + 1),#0x00
	mov	(___ulong2fs_sign_1_1 + 2),#0x00
	mov	(___ulong2fs_sign_1_1 + 3),#0x80
;	_ulong2fs.c 96
	clr	c
	clr	a
	subb	a,r2
	mov	___ulong2fs_a1_1_1,a
	clr	a
	subb	a,r3
	mov	(___ulong2fs_a1_1_1 + 1),a
	clr	a
	subb	a,r4
	mov	(___ulong2fs_a1_1_1 + 2),a
	clr	a
	subb	a,r5
	mov	(___ulong2fs_a1_1_1 + 3),a
;	_ulong2fs.c 99
00113$:
	mov	r2,#0x96
	mov	r3,#0x00
00105$:
	clr	c
	mov	a,___ulong2fs_a1_1_1
	subb	a,#0x00
	mov	a,(___ulong2fs_a1_1_1 + 1)
	subb	a,#0x00
	mov	a,(___ulong2fs_a1_1_1 + 2)
	subb	a,#0x80
	mov	a,(___ulong2fs_a1_1_1 + 3)
	subb	a,#0x00
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00107$
00116$:
;	_ulong2fs.c 101
	mov	a,___ulong2fs_a1_1_1
	add	a,acc
	mov	___ulong2fs_a1_1_1,a
	mov	a,(___ulong2fs_a1_1_1 + 1)
	rlc	a
	mov	(___ulong2fs_a1_1_1 + 1),a
	mov	a,(___ulong2fs_a1_1_1 + 2)
	rlc	a
	mov	(___ulong2fs_a1_1_1 + 2),a
	mov	a,(___ulong2fs_a1_1_1 + 3)
	rlc	a
	mov	(___ulong2fs_a1_1_1 + 3),a
;	_ulong2fs.c 102
	dec	r2
	cjne	r2,#0xff,00117$
	dec	r3
00117$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00107$:
;	_ulong2fs.c 105
	anl	(___ulong2fs_a1_1_1 + 2),#0x7f
;	_ulong2fs.c 107
	mov	r4,#0x00
	mov	r5,#0x00
	mov	ar4,r2
	mov	a,r3
	anl	a,#0x01
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	r5,a
	mov	r3,#0x00
	mov	r2,#0x00
	mov	r6,___ulong2fs_sign_1_1
	mov	r7,(___ulong2fs_sign_1_1 + 1)
	mov	r0,(___ulong2fs_sign_1_1 + 2)
	mov	r1,(___ulong2fs_sign_1_1 + 3)
	mov	a,r6
	orl	ar2,a
	mov	a,r7
	orl	ar3,a
	mov	a,r0
	orl	ar4,a
	mov	a,r1
	orl	ar5,a
	mov	a,___ulong2fs_a1_1_1
	orl	ar2,a
	mov	a,(___ulong2fs_a1_1_1 + 1)
	orl	ar3,a
	mov	a,(___ulong2fs_a1_1_1 + 2)
	orl	ar4,a
	mov	a,(___ulong2fs_a1_1_1 + 3)
	orl	ar5,a
	mov	___ulong2fs_fl_1_1,r2
	mov	(___ulong2fs_fl_1_1 + 1),r3
	mov	(___ulong2fs_fl_1_1 + 2),r4
	mov	(___ulong2fs_fl_1_1 + 3),r5
;	_ulong2fs.c 109
	mov	dpl,___ulong2fs_fl_1_1
	mov	dph,(___ulong2fs_fl_1_1 + 1)
	mov	b,(___ulong2fs_fl_1_1 + 2)
	mov	a,(___ulong2fs_fl_1_1 + 3)
00108$:
	C$_ulong2fs.c$110$1$1 ==.
	XG$__ulong2fs$0$0 ==.
	ret
	.area	CSEG    (CODE)
