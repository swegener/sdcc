;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:31 2000

;--------------------------------------------------------
	.module _modulong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __modulong_PARM_2
	.globl __modulong
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
__modulong_sloc0_1_0:
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
__modulong_PARM_2:
	.ds	0x0004
__modulong_a_1_1:
	.ds	0x0004
__modulong_count_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_modulong$0$0 ==.
;	_modulong.c 29
;	-----------------------------------------
;	 function _modulong
;	-----------------------------------------
__modulong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_modulong.c 51
	push	acc
	push	b
	push	dph
	push	dpl
	mov	dptr,#__modulong_a_1_1
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
;	_modulong.c 31
	mov	dptr,#__modulong_count_1_1
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_modulong.c 34
	mov	dptr,#__modulong_a_1_1
	movx	a,@dptr
	mov	__modulong_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(__modulong_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(__modulong_sloc0_1_0 + 2),a
	inc	dptr
	movx	a,@dptr
	mov	(__modulong_sloc0_1_0 + 3),a
	mov	r6,#0x00
00103$:
	mov	dptr,#__modulong_PARM_2
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
	mov  r2,a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r3,a
	jz	00119$
	ljmp	00117$
00119$:
;	_modulong.c 35
	mov	a,r7
	add	a,acc
	mov	r5,a
	mov	a,r0
	rlc	a
	mov	r3,a
	mov	a,r1
	rlc	a
	mov	r4,a
	mov	a,r2
	rlc	a
	mov	r2,a
	mov	dptr,#__modulong_PARM_2
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
;	_modulong.c 36
	clr	c
	mov	a,__modulong_sloc0_1_0
	subb	a,r5
	mov	a,(__modulong_sloc0_1_0 + 1)
	subb	a,r3
	mov	a,(__modulong_sloc0_1_0 + 2)
	subb	a,r4
	mov	a,(__modulong_sloc0_1_0 + 3)
	subb	a,r2
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00102$
00120$:
;	_modulong.c 38
	clr	c
	mov	a,r2
	rrc	a
	mov	r2,a
	mov	a,r4
	rrc	a
	mov	r4,a
	mov	a,r3
	rrc	a
	mov	r3,a
	mov	a,r5
	rrc	a
; Peephole 100   removed redundant mov
	mov  r5,a
	mov  dptr,#__modulong_PARM_2
	movx @dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
;	_modulong.c 39
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00102$:
;	_modulong.c 41
	inc	r6
	mov	dptr,#__modulong_count_1_1
	mov	a,r6
	movx	@dptr,a
	ljmp	00103$
;	_modulong.c 44
00117$:
	mov	dptr,#__modulong_count_1_1
	movx	a,@dptr
	mov	__modulong_sloc0_1_0,a
00108$:
;	_modulong.c 45
	mov	dptr,#__modulong_a_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#__modulong_PARM_2
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
	mov	r2,a
	clr	c
	mov	a,r3
	subb	a,r7
	mov	a,r4
	subb	a,r0
	mov	a,r5
	subb	a,r1
	mov	a,r6
	subb	a,r2
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00121$:
;	_modulong.c 46
	mov	dptr,#__modulong_a_1_1
	clr	c
	mov	a,r3
	subb	a,r7
	movx	@dptr,a
	mov	a,r4
	subb	a,r0
	inc	dptr
	movx	@dptr,a
	mov	a,r5
	subb	a,r1
	inc	dptr
	movx	@dptr,a
	mov	a,r6
	subb	a,r2
	inc	dptr
	movx	@dptr,a
00107$:
;	_modulong.c 48
	mov	dptr,#__modulong_PARM_2
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
; Peephole 106   removed redundant mov 
	mov  r5,a
	clr  c
	rrc	a
	mov	r5,a
	mov	a,r4
	rrc	a
	mov	r4,a
	mov	a,r3
	rrc	a
	mov	r3,a
	mov	a,r2
	rrc	a
; Peephole 100   removed redundant mov
	mov  r2,a
	mov  dptr,#__modulong_PARM_2
	movx @dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_modulong.c 49
	mov	r2,__modulong_sloc0_1_0
	dec	__modulong_sloc0_1_0
	mov	a,r2
	jz	00122$
	ljmp	00108$
00122$:
;	_modulong.c 51
	mov	dptr,#__modulong_a_1_1
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
00111$:
	C$_modulong.c$52$1$1 ==.
	XG$_modulong$0$0 ==.
	ret
	.area	CSEG    (CODE)
