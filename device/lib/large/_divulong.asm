;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:23 2000

;--------------------------------------------------------
	.module _divulong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divulong_PARM_2
	.globl __divulong
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
__divulong_sloc0_1_0:
	.ds	0x0004
__divulong_sloc1_1_0:
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
__divulong_PARM_2:
	.ds	0x0004
__divulong_a_1_1:
	.ds	0x0004
__divulong_reste_1_1:
	.ds	0x0004
__divulong_count_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_divulong$0$0 ==.
;	_divulong.c 27
;	-----------------------------------------
;	 function _divulong
;	-----------------------------------------
__divulong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divulong.c 48
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#__divulong_a_1_1
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
;	_divulong.c 29
	mov	dptr,#__divulong_reste_1_1
	clr	a
	inc	dptr
	inc	dptr
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
	lcall	__decdptr
	movx	@dptr,a
;	_divulong.c 30
	mov	dptr,#__divulong_count_1_1
	mov	a,#0x20
	movx	@dptr,a
;	_divulong.c 33
	mov	dptr,#__divulong_PARM_2
	movx	a,@dptr
	mov	__divulong_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(__divulong_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(__divulong_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(__divulong_sloc0_1_0 + 3),a
00105$:
;	_divulong.c 35
	mov	dptr,#__divulong_a_1_1
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
; Peephole 105   removed redundant mov
	mov  r1,a
	rl	a
	anl	a,#0x01
	mov	r2,a
;	_divulong.c 36
	mov	a,r6
	add	a,acc
	mov	r6,a
	mov	a,r7
	rlc	a
	mov	r4,a
	mov	a,r0
	rlc	a
	mov	r5,a
	mov	a,r1
	rlc	a
	mov	r3,a
	mov	dptr,#__divulong_a_1_1
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	_divulong.c 37
	push	ar6
	push	ar4
	push	ar5
	push	ar3
	mov	dptr,#__divulong_reste_1_1
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,r7
	add	a,acc
	mov	__divulong_sloc1_1_0,a
	mov	a,r0
	rlc	a
	mov	(__divulong_sloc1_1_0 + 1),a
	mov	a,r1
	rlc	a
	mov	(__divulong_sloc1_1_0 + 2),a
	mov	a,r3
	rlc	a
	mov	(__divulong_sloc1_1_0 + 3),a
	mov	dptr,#__divulong_reste_1_1
	mov	a,__divulong_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 3)
	movx	@dptr,a
;	_divulong.c 38
	pop	ar3
	pop	ar5
	pop	ar4
	pop	ar6
	mov	a,r2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_divulong.c 39
	mov	dptr,#__divulong_reste_1_1
	mov	a,#0x01
	orl	a,__divulong_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divulong_sloc1_1_0 + 3)
	movx	@dptr,a
00102$:
;	_divulong.c 41
	mov	dptr,#__divulong_reste_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	clr	c
	mov	a,r2
	subb	a,__divulong_sloc0_1_0
	mov	a,r7
	subb	a,(__divulong_sloc0_1_0 + 1)
	mov	a,r0
	subb	a,(__divulong_sloc0_1_0 + 2)
	mov	a,r1
	subb	a,(__divulong_sloc0_1_0 + 3)
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00106$
00115$:
;	_divulong.c 42
	mov	dptr,#__divulong_reste_1_1
	clr	c
	mov	a,r2
	subb	a,__divulong_sloc0_1_0
	movx	@dptr,a
	mov	a,r7
	subb	a,(__divulong_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,r0
	subb	a,(__divulong_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,r1
	subb	a,(__divulong_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_divulong.c 44
	mov	dptr,#__divulong_a_1_1
	mov	a,#0x01
	orl	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
00106$:
;	_divulong.c 46
	mov	dptr,#__divulong_count_1_1
	movx	a,@dptr
	add	a,#0xff
; Peephole 100   removed redundant mov
	mov  r2,a
	mov  dptr,#__divulong_count_1_1
	movx @dptr,a
	mov	a,r2
	jz	00116$
	ljmp	00105$
00116$:
;	_divulong.c 48
	mov	dptr,#__divulong_a_1_1
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
00108$:
	C$_divulong.c$49$1$1 ==.
	XG$_divulong$0$0 ==.
	ret
	.area	CSEG    (CODE)
