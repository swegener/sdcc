;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:22 2000

;--------------------------------------------------------
	.module _atol
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _atol
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
_atol_sloc0_1_0:
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
_atol_s_1_1:
	.ds	0x0003
_atol_rv_1_1:
	.ds	0x0004
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$atol$0$0 ==.
;	_atol.c 25
;	-----------------------------------------
;	 function atol
;	-----------------------------------------
_atol:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_atol.c 0
	push	b
	push	dph
	push	dpl
	mov	dptr,#_atol_s_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_atol.c 27
	mov	dptr,#_atol_rv_1_1
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
;	_atol.c 31
	mov	dptr,#_atol_s_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
00107$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00133$:
;	_atol.c 32
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00134$:
	clr	c
	mov	a,r5
	xrl	a,#0x80
	subb	a,#0xb0
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00135$:
;	_atol.c 33
00102$:
;	_atol.c 34
	cjne	r5,#0x2d,00136$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00136$:
	cjne	r5,#0x2b,00137$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00137$:
;	_atol.c 36
	inc	r2
	cjne	r2,#0x00,00138$
	inc	r3
00138$:
	mov	dptr,#_atol_s_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00109$:
;	_atol.c 39
	mov	dptr,#_atol_s_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r5,a
	cjne	r5,#0x2d,00139$
	mov	a,#0x01
	sjmp	00140$
00139$:
	clr	a
00140$:
;	_atol.c 40
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00110$
00141$:
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r5,#0x2b,00131$
; Peephole 201   removed redundant sjmp
00142$:
00143$:
00110$:
	mov	dptr,#_atol_s_1_1
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_atol.c 42
00131$:
	mov	dptr,#_atol_s_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
00115$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
	jnz	00144$
	ljmp	00117$
00144$:
	clr	c
	mov	a,r5
	xrl	a,#0x80
	subb	a,#0xb0
	jnc	00145$
	ljmp	00117$
00145$:
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
	jnc	00146$
	ljmp	00117$
00146$:
;	_atol.c 43
	push	ar6
	mov	dptr,#_atol_rv_1_1
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
	mov	r6,a
	mov	dptr,#__mulslong_PARM_2
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
	mov	a,#0x0a
	movx	@dptr,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	mov	dpl,r7
	mov	dph,r0
	mov	b,r1
	mov	a,r6
	lcall	__mulslong
	mov	_atol_sloc0_1_0,dpl
	mov	(_atol_sloc0_1_0 + 1),dph
	mov	(_atol_sloc0_1_0 + 2),b
	mov	(_atol_sloc0_1_0 + 3),a
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	a,r5
	add	a,#0xd0
	mov	r5,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r6,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r7,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r0,a
	mov	dptr,#_atol_rv_1_1
	mov	a,r5
	add	a,_atol_sloc0_1_0
	movx	@dptr,a
	mov	a,r6
	addc	a,(_atol_sloc0_1_0 + 1)
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	addc	a,(_atol_sloc0_1_0 + 2)
	inc	dptr
	movx	@dptr,a
	mov	a,r0
	addc	a,(_atol_sloc0_1_0 + 3)
	inc	dptr
	movx	@dptr,a
;	_atol.c 44
	inc	r2
	cjne	r2,#0x00,00147$
	inc	r3
00147$:
	mov	dptr,#_atol_s_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	pop	ar6
	ljmp	00115$
00117$:
;	_atol.c 47
	mov	dptr,#_atol_s_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	mov	a,r6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00120$
00148$:
	mov	dptr,#_atol_rv_1_1
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
	clr	c
	clr	a
	subb	a,r2
	mov	r2,a
	clr	a
	subb	a,r3
	mov	r3,a
	clr	a
	subb	a,r4
	mov	r4,a
	clr	a
	subb	a,r5
	mov	r5,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00120$:
	mov	dptr,#_atol_rv_1_1
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
00121$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
00118$:
	C$_atol.c$48$1$1 ==.
	XG$atol$0$0 ==.
	ret
	.area	CSEG    (CODE)
