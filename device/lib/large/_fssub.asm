;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:28 2000

;--------------------------------------------------------
	.module _fssub
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fssub_PARM_2
	.globl ___fssub
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
___fssub_PARM_2:
	.ds	0x0004
___fssub_a1_1_1:
	.ds	0x0004
___fssub_fl1_1_1:
	.ds	0x0004
___fssub_fl2_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$__fssub$0$0 ==.
;	_fssub.c 81
;	-----------------------------------------
;	 function __fssub
;	-----------------------------------------
___fssub:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fssub.c 96
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#___fssub_a1_1_1
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
;	_fssub.c 85
	mov	dptr,#___fssub_a1_1_1
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
	mov	dptr,#(___fssub_fl1_1_1)
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
;	_fssub.c 86
	mov	dptr,#___fssub_PARM_2
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
	mov	dptr,#(___fssub_fl2_1_1)
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
;	_fssub.c 89
	mov	dptr,#(___fssub_fl2_1_1)
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
00109$:
;	_fssub.c 90
	mov	dptr,#(___fssub_fl1_1_1)
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
	ljmp	00105$
00102$:
;	_fssub.c 91
	mov	dptr,#(___fssub_fl1_1_1)
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
00110$:
;	_fssub.c 92
	mov	dptr,#(___fssub_fl2_1_1)
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
	cpl	acc.7
; Peephole 191   removed redundant mov
	mov  r5,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r4
	ljmp	00105$
00104$:
;	_fssub.c 95
	mov	dptr,#(___fssub_fl2_1_1)
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
	xrl	ar5,#0x80
	mov	dptr,#(___fssub_fl2_1_1)
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
;	_fssub.c 96
	mov	dptr,#(___fssub_fl1_1_1)
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
	mov	dptr,#(___fssub_fl2_1_1)
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
	mov	dptr,#___fsadd_PARM_2
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fsadd
	C$_fssub.c$97$1$1 ==.
	XG$__fssub$0$0 ==.
; Peephole 156   removed misc moves via dph, dpl, b, a before return
00105$:
	ret
	.area	CSEG    (CODE)
