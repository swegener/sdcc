;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:32 2000

;--------------------------------------------------------
	.module _mululong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __mululong_PARM_2
	.globl __mululong
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
__mululong_PARM_2:
	.ds	0x0004
__mululong_a_1_1:
	.ds	0x0004
__mululong_t_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_mululong$0$0 ==.
;	_mululong.c 59
;	-----------------------------------------
;	 function _mululong
;	-----------------------------------------
__mululong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_mululong.c 86
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#__mululong_a_1_1
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
;	_mululong.c 63
	mov	dptr,#(__mululong_a_1_1)
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#(0x0002 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r3,a
	mov	b,r3
	mov	a,r2
	mul	ab
	mov	r3,a
	mov	r4,b
	mov	dptr,#(0x0002 + __mululong_t_1_1)
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_mululong.c 64
	mov	dptr,#(__mululong_PARM_2)
	movx	a,@dptr
	mov	r3,a
	mov	b,r3
	mov	a,r2
	mul	ab
	mov	r4,a
	mov	r5,b
	mov	dptr,#(__mululong_t_1_1)
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_mululong.c 65
	mov	dptr,#(0x0003 + __mululong_t_1_1)
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#(0x0003 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r5,a
;	_mululong.c 66
	mov	b,r3
	mov	a,r5
	mul	ab
; Peephole 105   removed redundant mov
	mov  r5,a
	add	a,r4
; Peephole 100   removed redundant mov
	mov  r4,a
	mov  dptr,#(0x0003 + __mululong_t_1_1)
	movx @dptr,a
;	_mululong.c 67
	mov	dptr,#(0x0003 + __mululong_t_1_1)
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#(0x0002 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r5,a
;	_mululong.c 68
	mov	dptr,#(0x0001 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r6,a
	mov	b,r6
	mov	a,r5
	mul	ab
; Peephole 105   removed redundant mov
	mov  r5,a
	add	a,r4
; Peephole 100   removed redundant mov
	mov  r4,a
	mov  dptr,#(0x0003 + __mululong_t_1_1)
	movx @dptr,a
;	_mululong.c 69
	mov	dptr,#(0x0002 + __mululong_t_1_1)
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#(0x0002 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r6,a
	mov	b,r3
	mov	a,r6
	mul	ab
	mov	r6,a
	mov	r7,b
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	mov	dptr,#(0x0002 + __mululong_t_1_1)
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_mululong.c 71
	mov	dptr,#(0x0002 + __mululong_t_1_1)
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#(0x0001 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#(0x0001 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r7,a
	mov	b,r7
	mov	a,r6
	mul	ab
	mov	r7,a
	mov	r6,b
	mov	a,r7
	add	a,r4
	mov	r4,a
	mov	a,r6
	addc	a,r5
	mov	r5,a
	mov	dptr,#(0x0002 + __mululong_t_1_1)
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_mululong.c 73
	mov	dptr,#(0x0001 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r4,a
;	_mululong.c 74
	mov	dptr,#(0x0002 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r5,a
	mov	b,r5
	mov	a,r4
	mul	ab
; Peephole 100   removed redundant mov
	mov  r4,a
	mov  dptr,#(0x0003 + __mululong_a_1_1)
	movx @dptr,a
;	_mululong.c 75
	mov	dptr,#(0x0001 + __mululong_a_1_1)
	movx	a,@dptr
	mov	r4,a
;	_mululong.c 76
	mov	b,r3
	mov	a,r4
	mul	ab
	mov	r3,a
	mov	r5,b
	mov	dptr,#(0x0001 + __mululong_a_1_1)
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_mululong.c 78
;	_mululong.c 79
	mov	dptr,#(0x0003 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r3,a
	mov	b,r3
	mov	a,r2
	mul	ab
; Peephole 100   removed redundant mov
	mov  r3,a
	mov  dptr,#(0x0003 + __mululong_PARM_2)
	movx @dptr,a
;	_mululong.c 81
	mov	dptr,#(0x0001 + __mululong_PARM_2)
	movx	a,@dptr
	mov	r3,a
	mov	b,r3
	mov	a,r2
	mul	ab
	mov	r2,a
	mov	r4,b
	mov	dptr,#(0x0001 + __mululong_PARM_2)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_mululong.c 82
	mov	dptr,#(__mululong_PARM_2)
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_mululong.c 83
	mov	dptr,#(__mululong_a_1_1)
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_mululong.c 84
	mov	dptr,#(__mululong_t_1_1)
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
	mov	dptr,#__mululong_a_1_1
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
	mov	r1,a
	mov	a,r6
	add	a,r2
	mov	r2,a
	mov	a,r7
	addc	a,r3
	mov	r3,a
	mov	a,r0
	addc	a,r4
	mov	r4,a
	mov	a,r1
	addc	a,r5
	mov	r5,a
	mov	dptr,#(__mululong_t_1_1)
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
;	_mululong.c 86
	mov	dptr,#__mululong_PARM_2
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
	mov	r1,a
	mov	a,r6
	add	a,r2
	mov	r2,a
	mov	a,r7
	addc	a,r3
	mov	r3,a
	mov	a,r0
	addc	a,r4
	mov	r4,a
	mov	a,r1
	addc	a,r5
; Peephole 191   removed redundant mov
	mov  r5,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r4
00101$:
	C$_mululong.c$87$1$1 ==.
	XG$_mululong$0$0 ==.
	ret
	.area	CSEG    (CODE)
