;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

;--------------------------------------------------------
	.module _strncpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strncpy_PARM_3
	.globl _strncpy_PARM_2
	.globl _strncpy
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
_strncpy_sloc0_1_0:
	.ds	0x0003
_strncpy_sloc1_1_0:
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
_strncpy_PARM_2:
	.ds	0x0003
_strncpy_PARM_3:
	.ds	0x0002
_strncpy_d_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strncpy$0$0 ==.
;	_strncpy.c 27
;	-----------------------------------------
;	 function strncpy
;	-----------------------------------------
_strncpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strncpy.c 36
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strncpy_d_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strncpy.c 32
	mov	dptr,#_strncpy_d_1_1
	movx	a,@dptr
	mov	_strncpy_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strncpy_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strncpy_sloc0_1_0 + 2),a
;	_strncpy.c 34
	mov	r5,_strncpy_sloc0_1_0
	mov	r6,(_strncpy_sloc0_1_0 + 1)
	mov	r7,(_strncpy_sloc0_1_0 + 2)
	mov	dptr,#_strncpy_PARM_2
	movx	a,@dptr
	mov	_strncpy_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strncpy_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strncpy_sloc1_1_0 + 2),a
	mov	dptr,#_strncpy_PARM_3
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
00101$:
	mov	ar2,r3
	mov	ar0,r4
	dec	r3
	cjne	r3,#0xff,00108$
	dec	r4
00108$:
	mov	dptr,#_strncpy_d_1_1
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	mov	a,r2
	orl	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_strncpy.c 35
	push	ar3
	push	ar4
	mov	ar2,r5
	mov	ar0,r6
	mov	ar1,r7
	inc	r5
	cjne	r5,#0x00,00110$
	inc	r6
00110$:
	mov	dpl,_strncpy_sloc1_1_0
	mov	dph,(_strncpy_sloc1_1_0 + 1)
	mov	b,(_strncpy_sloc1_1_0 + 2)
	inc	_strncpy_sloc1_1_0
	clr	a
	cjne	a,_strncpy_sloc1_1_0,00111$
	inc	(_strncpy_sloc1_1_0 + 1)
00111$:
	lcall	__gptrget
; Peephole 191   removed redundant mov
	mov  r3,a
	mov  dpl,r2
	mov  dph,r0
	mov  b,r1
	lcall	__gptrput
	pop	ar4
	pop	ar3
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_strncpy.c 36
	mov	dpl,_strncpy_sloc0_1_0
	mov	dph,(_strncpy_sloc0_1_0 + 1)
	mov	b,(_strncpy_sloc0_1_0 + 2)
00104$:
	C$_strncpy.c$37$1$1 ==.
	XG$strncpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
