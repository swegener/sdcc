;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:23 2000

;--------------------------------------------------------
	.module _divuint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divuint_PARM_2
	.globl __divuint
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
__divuint_c_1_1:
	.ds	0x0001
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area	XSEG    (XDATA)
__divuint_PARM_2:
	.ds	0x0002
__divuint_a_1_1:
	.ds	0x0002
__divuint_reste_1_1:
	.ds	0x0002
__divuint_count_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_divuint$0$0 ==.
;	_divuint.c 28
;	-----------------------------------------
;	 function _divuint
;	-----------------------------------------
__divuint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divuint.c 49
	push	dph
	push	dpl
	mov	dptr,#__divuint_a_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_divuint.c 30
	mov	dptr,#__divuint_reste_1_1
	clr	a
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
;	_divuint.c 31
	mov	dptr,#__divuint_count_1_1
	mov	a,#0x10
	movx	@dptr,a
;	_divuint.c 34
	mov	dptr,#__divuint_PARM_2
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
00105$:
;	_divuint.c 36
	mov	dptr,#__divuint_a_1_1
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r5,a
	rl	a
	anl	a,#0x01
	mov	r6,a
;	_divuint.c 37
	mov	a,r5
	xch	a,r4
	add	a,acc
	xch	a,r4
	rlc	a
	mov	r5,a
	mov	dptr,#__divuint_a_1_1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_divuint.c 38
	mov	dptr,#__divuint_reste_1_1
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r0,a
	xch	a,r7
	add	a,acc
	xch	a,r7
	rlc	a
	mov	r0,a
	mov	dptr,#__divuint_reste_1_1
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
;	_divuint.c 39
	mov	a,r6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_divuint.c 40
	mov	dptr,#__divuint_reste_1_1
	mov	a,#0x01
	orl	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
00102$:
;	_divuint.c 42
	mov	dptr,#__divuint_reste_1_1
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	clr	c
	mov	a,r6
	subb	a,r2
	mov	a,r7
	subb	a,r3
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00106$
00115$:
;	_divuint.c 43
	mov	dptr,#__divuint_reste_1_1
	clr	c
	mov	a,r6
	subb	a,r2
	movx	@dptr,a
	mov	a,r7
	subb	a,r3
	inc	dptr
	movx	@dptr,a
;	_divuint.c 45
	mov	dptr,#__divuint_a_1_1
	mov	a,#0x01
	orl	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
00106$:
;	_divuint.c 47
	mov	dptr,#__divuint_count_1_1
	movx	a,@dptr
	add	a,#0xff
; Peephole 100   removed redundant mov
	mov  r4,a
	mov  dptr,#__divuint_count_1_1
	movx @dptr,a
	mov	a,r4
	jz	00116$
	ljmp	00105$
00116$:
;	_divuint.c 49
	mov	dptr,#__divuint_a_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
00108$:
	C$_divuint.c$50$1$1 ==.
	XG$_divuint$0$0 ==.
	ret
	.area	CSEG    (CODE)
