;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:31 2000

;--------------------------------------------------------
	.module _moduint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __moduint_PARM_2
	.globl __moduint
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
__moduint_PARM_2:
	.ds	0x0002
__moduint_a_1_1:
	.ds	0x0002
__moduint_count_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_moduint$0$0 ==.
;	_moduint.c 30
;	-----------------------------------------
;	 function _moduint
;	-----------------------------------------
__moduint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_moduint.c 52
	push	dph
	push	dpl
	mov	dptr,#__moduint_a_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_moduint.c 32
	mov	dptr,#__moduint_count_1_1
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	_moduint.c 35
	mov	dptr,#__moduint_a_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	r4,#0x00
00103$:
	mov	dptr,#__moduint_PARM_2
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r6,a
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00117$
00119$:
;	_moduint.c 36
	mov	a,r6
	xch	a,r5
	add	a,acc
	xch	a,r5
	rlc	a
	mov	r6,a
	mov	dptr,#__moduint_PARM_2
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
;	_moduint.c 37
	clr	c
	mov	a,r2
	subb	a,r5
	mov	a,r3
	subb	a,r6
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00102$
00120$:
;	_moduint.c 39
	mov	a,r6
	clr	c
	rrc	a
	xch	a,r5
	rrc	a
	xch	a,r5
	mov	r6,a
	mov	dptr,#__moduint_PARM_2
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
;	_moduint.c 40
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00102$:
;	_moduint.c 42
	inc	r4
	mov	dptr,#__moduint_count_1_1
	mov	a,r4
	movx	@dptr,a
	ljmp	00103$
;	_moduint.c 45
00117$:
	mov	dptr,#__moduint_count_1_1
	movx	a,@dptr
	mov	r2,a
00108$:
;	_moduint.c 46
	mov	dptr,#__moduint_a_1_1
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#__moduint_PARM_2
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	clr	c
	mov	a,r3
	subb	a,r5
	mov	a,r4
	subb	a,r6
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00121$:
;	_moduint.c 47
	mov	dptr,#__moduint_a_1_1
	clr	c
	mov	a,r3
	subb	a,r5
	movx	@dptr,a
	mov	a,r4
	subb	a,r6
	inc	dptr
	movx	@dptr,a
00107$:
;	_moduint.c 49
	mov	dptr,#__moduint_PARM_2
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r4,a
	clr	c
	rrc	a
	xch	a,r3
	rrc	a
	xch	a,r3
	mov	r4,a
	mov	dptr,#__moduint_PARM_2
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_moduint.c 50
	mov	ar3,r2
	dec	r2
	mov	a,r3
	jz	00122$
	ljmp	00108$
00122$:
;	_moduint.c 52
	mov	dptr,#__moduint_a_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
00111$:
	C$_moduint.c$53$1$1 ==.
	XG$_moduint$0$0 ==.
	ret
	.area	CSEG    (CODE)
