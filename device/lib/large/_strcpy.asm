;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:33 2000

;--------------------------------------------------------
	.module _strcpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcpy_PARM_2
	.globl _strcpy
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
_strcpy_sloc0_1_0:
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
_strcpy_PARM_2:
	.ds	0x0003
_strcpy_d_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strcpy$0$0 ==.
;	_strcpy.c 27
;	-----------------------------------------
;	 function strcpy
;	-----------------------------------------
_strcpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcpy.c 35
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strcpy_d_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strcpy.c 31
	mov	dptr,#_strcpy_d_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	_strcpy.c 33
	mov	dptr,#_strcpy_PARM_2
	movx	a,@dptr
	mov	_strcpy_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strcpy_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strcpy_sloc0_1_0 + 2),a
00101$:
	mov	ar0,r2
	mov	ar1,r3
	mov	ar5,r4
	inc	r2
	cjne	r2,#0x00,00108$
	inc	r3
00108$:
	mov	dpl,_strcpy_sloc0_1_0
	mov	dph,(_strcpy_sloc0_1_0 + 1)
	mov	b,(_strcpy_sloc0_1_0 + 2)
	inc	_strcpy_sloc0_1_0
	clr	a
	cjne	a,_strcpy_sloc0_1_0,00109$
	inc	(_strcpy_sloc0_1_0 + 1)
00109$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r6,a
	mov  dpl,r0
	mov  dph,r1
	mov  b,r5
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00110$:
;	_strcpy.c 35
	mov	dptr,#_strcpy_d_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
00104$:
	C$_strcpy.c$36$1$1 ==.
	XG$strcpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
