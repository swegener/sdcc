;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:30:25 2000

;--------------------------------------------------------
	.module _atoi
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _atoi
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
_atoi_s_1_1:
	.ds	0x0003
_atoi_rv_1_1:
	.ds	0x0002
_atoi_sign_1_1:
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
	G$atoi$0$0 ==.
;	_atoi.c 25
;	-----------------------------------------
;	 function atoi
;	-----------------------------------------
_atoi:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_atoi.c 0
	mov	_atoi_s_1_1,dpl
	mov	(_atoi_s_1_1 + 1),dph
	mov	(_atoi_s_1_1 + 2),b
;	_atoi.c 27
	clr	a
	mov	(_atoi_rv_1_1 + 1),a
	mov	_atoi_rv_1_1,a
;	_atoi.c 31
	mov	r7,_atoi_s_1_1
	mov	r0,(_atoi_s_1_1 + 1)
	mov	r1,(_atoi_s_1_1 + 2)
00107$:
	mov	dpl,r7
	mov	dph,r0
	mov	b,r1
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00133$:
;	_atoi.c 32
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r2
	xrl	b,#0x80
	subb	a,b
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00134$:
	clr	c
	mov	a,r2
	xrl	a,#0x80
	subb	a,#0xb0
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00135$:
;	_atoi.c 33
00102$:
;	_atoi.c 34
	cjne	r2,#0x2d,00136$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00136$:
	cjne	r2,#0x2b,00137$
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00137$:
;	_atoi.c 36
	inc	r7
	cjne	r7,#0x00,00138$
	inc	r0
00138$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00109$:
;	_atoi.c 39
	mov	_atoi_s_1_1,r7
	mov	(_atoi_s_1_1 + 1),r0
	mov	(_atoi_s_1_1 + 2),r1
	cjne	r2,#0x2d,00139$
	mov	a,#0x01
	sjmp	00140$
00139$:
	clr	a
00140$:
;	_atoi.c 40
; Peephole 166   removed redundant mov
	mov  r3,a
	mov  _atoi_sign_1_1,r3 
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00110$
00141$:
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r2,#0x2b,00131$
; Peephole 201   removed redundant sjmp
00142$:
00143$:
00110$:
	inc	_atoi_s_1_1
	clr	a
	cjne	a,_atoi_s_1_1,00144$
	inc	(_atoi_s_1_1 + 1)
00144$:
;	_atoi.c 42
00131$:
	mov	r2,_atoi_s_1_1
	mov	r3,(_atoi_s_1_1 + 1)
	mov	r7,(_atoi_s_1_1 + 2)
00115$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
	jnz	00145$
	ljmp	00117$
00145$:
	clr	c
	mov	a,r0
	xrl	a,#0x80
	subb	a,#0xb0
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00117$
00146$:
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x39 ^ 0x80)
	mov	b,r0
	xrl	b,#0x80
	subb	a,b
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00117$
00147$:
;	_atoi.c 43
	clr	a
	mov	(__mulsint_PARM_2 + 1),a
	mov	__mulsint_PARM_2,#0x0a
	push	ar2
	push	ar3
	push	ar7
	push	ar0
	mov	dpl,_atoi_rv_1_1
	mov	dph,(_atoi_rv_1_1 + 1)
	lcall	__mulsint
	mov	r1,dpl
	mov	r4,dph
	pop	ar0
	pop	ar7
	pop	ar3
	pop	ar2
	mov	a,r0
	add	a,#0xd0
	mov	r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,#0xff
	mov	r5,a
	mov	a,r0
	add	a,r1
	mov	_atoi_rv_1_1,a
	mov	a,r5
	addc	a,r4
	mov	(_atoi_rv_1_1 + 1),a
;	_atoi.c 44
	inc	r2
	cjne	r2,#0x00,00148$
	inc	r3
00148$:
	ljmp	00115$
00117$:
;	_atoi.c 47
	mov	a,_atoi_sign_1_1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00120$
00149$:
	clr	c
	clr	a
	subb	a,_atoi_rv_1_1
	mov	r2,a
	clr	a
	subb	a,(_atoi_rv_1_1 + 1)
	mov	r3,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00121$
00120$:
	mov	r2,_atoi_rv_1_1
	mov	r3,(_atoi_rv_1_1 + 1)
00121$:
	mov	dpl,r2
	mov	dph,r3
00118$:
	C$_atoi.c$48$1$1 ==.
	XG$atoi$0$0 ==.
	ret
	.area	CSEG    (CODE)
