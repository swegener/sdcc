;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:33 2000

;--------------------------------------------------------
	.module _strcmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcmp_PARM_2
	.globl _strcmp
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
_strcmp_sloc0_1_0:
	.ds	0x0001
_strcmp_sloc1_1_0:
	.ds	0x0003
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
_strcmp_PARM_2:
	.ds	0x0003
_strcmp_src_1_1:
	.ds	0x0003
_strcmp_ret_1_1:
	.ds	0x0002
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strcmp$0$0 ==.
;	_strcmp.c 27
;	-----------------------------------------
;	 function strcmp
;	-----------------------------------------
_strcmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcmp.c 42
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strcmp_src_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strcmp.c 34
	mov	dptr,#_strcmp_src_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#_strcmp_PARM_2
	movx	a,@dptr
	mov	_strcmp_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strcmp_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strcmp_sloc1_1_0 + 2),a
00102$:
	mov	dpl,_strcmp_sloc1_1_0
	mov	dph,(_strcmp_sloc1_1_0 + 1)
	mov	b,(_strcmp_sloc1_1_0 + 2)
	lcall	__gptrget
	mov	_strcmp_sloc0_1_0,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 106   removed redundant mov 
	mov  r1,a
	clr  c
	subb	a,_strcmp_sloc0_1_0
; Peephole 166   removed redundant mov
	mov  r1,a
	mov  ar0,r1 
	rlc	a
	subb	a,acc
	mov	r5,a
	mov	dptr,#_strcmp_ret_1_1
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	mov	a,r1
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00117$:
	mov	a,_strcmp_sloc0_1_0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00118$:
;	_strcmp.c 35
	inc	r2
	cjne	r2,#0x00,00119$
	inc	r3
00119$:
	mov	dptr,#_strcmp_src_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	_strcmp_sloc1_1_0
	clr	a
	cjne	a,_strcmp_sloc1_1_0,00120$
	inc	(_strcmp_sloc1_1_0 + 1)
00120$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00104$:
;	_strcmp.c 37
	mov	dptr,#_strcmp_src_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	mov	a,r5
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00108$
00121$:
;	_strcmp.c 38
	mov	dptr,#_strcmp_ret_1_1
; Peephole 101   removed redundant mov
	mov  a,#0xff
	movx @dptr,a
	inc  dptr
	movx @dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	_strcmp.c 39
	clr	c
; Peephole 180   changed mov to clr
	clr  a
	subb	a,r0
; Peephole 159   avoided xrl during execution
	mov  a,#(0x00 ^ 0x80)
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00122$:
;	_strcmp.c 40
	mov	dptr,#_strcmp_ret_1_1
	clr	a
	inc	dptr
	movx	@dptr,a
	lcall	__decdptr
	mov	a,#0x01
	movx	@dptr,a
00109$:
;	_strcmp.c 42
	mov	dptr,#_strcmp_ret_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
00110$:
	C$_strcmp.c$43$1$1 ==.
	XG$strcmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
