;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:27 2000

;--------------------------------------------------------
	.module _fsgt
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fsgt_PARM_2
	.globl ___fsgt
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
___fsgt_sloc0_1_0:
	.ds	0x0004
___fsgt_sloc1_1_0:
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
___fsgt_PARM_2:
	.ds	0x0004
___fsgt_a1_1_1:
	.ds	0x0004
___fsgt_fl1_1_1:
	.ds	0x0004
___fsgt_fl2_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fsgt$0$0 ==.
;	_fsgt.c 73
;	-----------------------------------------
;	 function __fsgt
;	-----------------------------------------
___fsgt:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fsgt.c 87
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fsgt_a1_1_1
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
;	_fsgt.c 77
	mov	dptr,#___fsgt_a1_1_1
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
	mov	dptr,#(___fsgt_fl1_1_1)
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
;	_fsgt.c 78
	mov	dptr,#___fsgt_PARM_2
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
	mov	dptr,#(___fsgt_fl2_1_1)
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
;	_fsgt.c 80
	mov	dptr,#(___fsgt_fl1_1_1)
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
; Peephole 105   removed redundant mov
	mov  r5,a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
	jnz	00111$
	ljmp	00102$
00111$:
	mov	dptr,#(___fsgt_fl2_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r0,a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
	jnz	00112$
	ljmp	00102$
00112$:
;	_fsgt.c 82
	mov	dptr,#(___fsgt_fl1_1_1)
	movx	a,@dptr
	mov	___fsgt_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 3),a
	xrl	(___fsgt_sloc0_1_0 + 3),#0x80
	mov	dptr,#(___fsgt_fl1_1_1)
	mov	a,___fsgt_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 3)
	movx	@dptr,a
;	_fsgt.c 83
	mov	dptr,#(___fsgt_fl2_1_1)
	movx	a,@dptr
	mov	___fsgt_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 3),a
	xrl	(___fsgt_sloc0_1_0 + 3),#0x80
	mov	dptr,#(___fsgt_fl2_1_1)
	mov	a,___fsgt_sloc0_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(___fsgt_sloc0_1_0 + 3)
	movx	@dptr,a
00102$:
;	_fsgt.c 85
	mov	dptr,#(___fsgt_fl1_1_1)
	movx	a,@dptr
	mov	___fsgt_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc0_1_0 + 3),a
	mov	dptr,#(___fsgt_fl2_1_1)
	movx	a,@dptr
	mov	___fsgt_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc1_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(___fsgt_sloc1_1_0 + 3),a
	clr	c
	mov	a,___fsgt_sloc1_1_0
	subb	a,___fsgt_sloc0_1_0
	mov	a,(___fsgt_sloc1_1_0 + 1)
	subb	a,(___fsgt_sloc0_1_0 + 1)
	mov	a,(___fsgt_sloc1_1_0 + 2)
	subb	a,(___fsgt_sloc0_1_0 + 2)
	mov	a,(___fsgt_sloc1_1_0 + 3)
	xrl	a,#0x80
	mov	b,(___fsgt_sloc0_1_0 + 3)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00105$
00113$:
;	_fsgt.c 86
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	_fsgt.c 87
	mov	dpl,#0x00
00106$:
	C$_fsgt.c$88$1$1 ==.
	XG$__fsgt$0$0 ==.
	ret
	.area	CSEG    (CODE)
