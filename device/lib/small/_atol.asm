;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:08 2000

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
_atol_s_1_1:
	.ds	0x0003
_atol_rv_1_1:
	.ds	0x0004
_atol_sign_1_1:
	.ds	0x0001
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
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_atol.c 27
	clr	a
	mov	(_atol_rv_1_1 + 3),a
	mov	(_atol_rv_1_1 + 2),a
	mov	(_atol_rv_1_1 + 1),a
	mov	_atol_rv_1_1,a
;	_atol.c 31
	mov	ar1,r2
	mov	ar5,r3
	mov	ar6,r4
00107$:
	mov	dpl,r1
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00133$:
;	_atol.c 32
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00134$:
	clr	c
	mov	a,r7
	xrl	a,#0x80
	subb	a,#0xb0
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00135$:
;	_atol.c 33
00102$:
;	_atol.c 34
	cjne	r7,#0x2d,00136$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00136$:
	cjne	r7,#0x2b,00137$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00137$:
;	_atol.c 36
	inc	r1
	cjne	r1,#0x00,00138$
	inc	r5
00138$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00109$:
;	_atol.c 39
	mov	ar2,r1
	mov	ar3,r5
	mov	ar4,r6
	cjne	r7,#0x2d,00139$
	mov	a,#0x01
	sjmp	00140$
00139$:
	clr	a
00140$:
;	_atol.c 40
; Peephole 166   removed redundant mov
	mov  r5,a
	mov  _atol_sign_1_1,r5 
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00110$
00141$:
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r7,#0x2b,00131$
; Peephole 201   removed redundant sjmp
00142$:
00143$:
00110$:
	inc	r2
	cjne	r2,#0x00,00144$
	inc	r3
00144$:
;	_atol.c 42
00131$:
	mov	_atol_s_1_1,r2
	mov	(_atol_s_1_1 + 1),r3
	mov	(_atol_s_1_1 + 2),r4
00115$:
	mov	dpl,_atol_s_1_1
	mov	dph,(_atol_s_1_1 + 1)
	mov	b,(_atol_s_1_1 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
	jnz	00145$
	ljmp	00117$
00145$:
	clr	c
	mov	a,r5
	xrl	a,#0x80
	subb	a,#0xb0
	jnc	00146$
	ljmp	00117$
00146$:
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
	jnc	00147$
	ljmp	00117$
00147$:
;	_atol.c 43
	clr	a
	mov	(__mulslong_PARM_2 + 3),a
	mov	(__mulslong_PARM_2 + 2),a
	mov	(__mulslong_PARM_2 + 1),a
	mov	__mulslong_PARM_2,#0x0a
	push	ar5
	mov	dpl,_atol_rv_1_1
	mov	dph,(_atol_rv_1_1 + 1)
	mov	b,(_atol_rv_1_1 + 2)
	mov	a,(_atol_rv_1_1 + 3)
	lcall	__mulslong
	mov	r7,dpl
	mov	r0,dph
	mov	r1,b
	mov	r6,a
	pop	ar5
	mov	a,r5
	add	a,#0xd0
	mov	r5,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r3,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r4,a
	mov	a,r5
	add	a,r7
	mov	_atol_rv_1_1,a
	mov	a,r2
	addc	a,r0
	mov	(_atol_rv_1_1 + 1),a
	mov	a,r3
	addc	a,r1
	mov	(_atol_rv_1_1 + 2),a
	mov	a,r4
	addc	a,r6
	mov	(_atol_rv_1_1 + 3),a
;	_atol.c 44
	inc	_atol_s_1_1
	clr	a
	cjne	a,_atol_s_1_1,00148$
	inc	(_atol_s_1_1 + 1)
00148$:
	ljmp	00115$
00117$:
;	_atol.c 47
	mov	a,_atol_sign_1_1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00120$
00149$:
	clr	c
	clr	a
	subb	a,_atol_rv_1_1
	mov	r2,a
	clr	a
	subb	a,(_atol_rv_1_1 + 1)
	mov	r3,a
	clr	a
	subb	a,(_atol_rv_1_1 + 2)
	mov	r4,a
	clr	a
	subb	a,(_atol_rv_1_1 + 3)
	mov	r5,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00120$:
	mov	r2,_atol_rv_1_1
	mov	r3,(_atol_rv_1_1 + 1)
	mov	r4,(_atol_rv_1_1 + 2)
	mov	r5,(_atol_rv_1_1 + 3)
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
