;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:23 2000

;--------------------------------------------------------
	.module _divslong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divslong_PARM_2
	.globl __divslong
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
__divslong_sloc0_1_0:
	.ds	0x0004
__divslong_sloc1_1_0:
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
__divslong_PARM_2:
	.ds	0x0004
__divslong_a_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_divslong$0$0 ==.
;	_divslong.c 25
;	-----------------------------------------
;	 function _divslong
;	-----------------------------------------
__divslong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divslong.c 0
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#__divslong_a_1_1
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
;	_divslong.c 29
	mov	dptr,#__divslong_a_1_1
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
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00106$
00113$:
	clr	c
	clr	a
	subb	a,r2
	mov	r6,a
	clr	a
	subb	a,r3
	mov	r7,a
	clr	a
	subb	a,r4
	mov	r0,a
	clr	a
	subb	a,r5
	mov	r1,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
	mov	ar6,r2
	mov	ar7,r3
	mov	ar0,r4
	mov	ar1,r5
00107$:
	mov	__divslong_sloc0_1_0,r6
	mov	(__divslong_sloc0_1_0 + 1),r7
	mov	(__divslong_sloc0_1_0 + 2),r0
	mov	(__divslong_sloc0_1_0 + 3),r1
;	_divslong.c 30
	mov	dptr,#__divslong_PARM_2
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
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00114$:
	clr	c
	clr	a
	subb	a,r6
	mov	__divslong_sloc1_1_0,a
	clr	a
	subb	a,r7
	mov	(__divslong_sloc1_1_0 + 1),a
	clr	a
	subb	a,r0
	mov	(__divslong_sloc1_1_0 + 2),a
	clr	a
	subb	a,r1
	mov	(__divslong_sloc1_1_0 + 3),a
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
	mov	__divslong_sloc1_1_0,r6
	mov	(__divslong_sloc1_1_0 + 1),r7
	mov	(__divslong_sloc1_1_0 + 2),r0
	mov	(__divslong_sloc1_1_0 + 3),r1
00109$:
	mov	dptr,#__divulong_PARM_2
	mov	a,__divslong_sloc1_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(__divslong_sloc1_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divslong_sloc1_1_0 + 2)
	movx	@dptr,a
	inc	dptr
	mov	a,(__divslong_sloc1_1_0 + 3)
	movx	@dptr,a
	push	ar2
	mov	dpl,__divslong_sloc0_1_0
	mov	dph,(__divslong_sloc0_1_0 + 1)
	mov	b,(__divslong_sloc0_1_0 + 2)
	mov	a,(__divslong_sloc0_1_0 + 3)
	lcall	__divulong
	mov	r3,dpl
	mov	r4,dph
	mov	r5,b
	mov	r6,a
	pop	ar2
;	_divslong.c 31
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	mov	dptr,#__divslong_a_1_1
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
; Peephole 105   removed redundant mov
	mov  r3,a
	rlc	a
	clr	a
	rlc	a
	mov	r7,a
	mov	a,r2
	xrl	a,ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00115$:
;	_divslong.c 32
	clr	c
	clr	a
	subb	a,r3
	mov	__divslong_sloc1_1_0,a
	clr	a
	subb	a,r4
	mov	(__divslong_sloc1_1_0 + 1),a
	clr	a
	subb	a,r5
	mov	(__divslong_sloc1_1_0 + 2),a
	clr	a
	subb	a,r6
; Peephole 191   removed redundant mov
	mov  (__divslong_sloc1_1_0 + 3),a
	mov  dpl,__divslong_sloc1_1_0
	mov  dph,(__divslong_sloc1_1_0 + 1)
	mov  b,(__divslong_sloc1_1_0 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_divslong.c 34
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,r6
00104$:
	C$_divslong.c$35$1$1 ==.
	XG$_divslong$0$0 ==.
	ret
	.area	CSEG    (CODE)
