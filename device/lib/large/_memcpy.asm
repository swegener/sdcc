;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:30 2000

;--------------------------------------------------------
	.module _memcpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memcpy_PARM_3
	.globl _memcpy_PARM_2
	.globl _memcpy
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
_memcpy_sloc0_1_0:
	.ds	0x0003
_memcpy_sloc1_1_0:
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
_memcpy_PARM_2:
	.ds	0x0003
_memcpy_PARM_3:
	.ds	0x0002
_memcpy_dst_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$memcpy$0$0 ==.
;	_memcpy.c 27
;	-----------------------------------------
;	 function memcpy
;	-----------------------------------------
_memcpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memcpy.c 44
	push	b
	push	dph
	push	dpl
	mov	dptr,#_memcpy_dst_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_memcpy.c 33
	mov	dptr,#_memcpy_dst_1_1
	movx	a,@dptr
	mov	_memcpy_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_memcpy_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_memcpy_sloc1_1_0 + 2),a
;	_memcpy.c 35
	mov	dptr,#_memcpy_PARM_2
	movx	a,@dptr
	mov	_memcpy_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_memcpy_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_memcpy_sloc0_1_0 + 2),a
;	_memcpy.c 40
	mov	r0,_memcpy_sloc1_1_0
	mov	r1,(_memcpy_sloc1_1_0 + 1)
	mov	r5,(_memcpy_sloc1_1_0 + 2)
	mov	dptr,#_memcpy_PARM_3
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
00101$:
	mov	ar2,r3
	mov	ar6,r4
	dec	r3
	cjne	r3,#0xff,00108$
	dec	r4
00108$:
	mov	a,r2
	orl	a,r6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_memcpy.c 41
	push	ar3
	push	ar4
	mov	ar2,r0
	mov	ar6,r1
	mov	ar7,r5
	inc	r0
	cjne	r0,#0x00,00110$
	inc	r1
00110$:
	mov	dpl,_memcpy_sloc0_1_0
	mov	dph,(_memcpy_sloc0_1_0 + 1)
	mov	b,(_memcpy_sloc0_1_0 + 2)
	inc	_memcpy_sloc0_1_0
	clr	a
	cjne	a,_memcpy_sloc0_1_0,00111$
	inc	(_memcpy_sloc0_1_0 + 1)
00111$:
	lcall	__gptrget
; Peephole 191   removed redundant mov
	mov  r3,a
	mov  dpl,r2
	mov  dph,r6
	mov  b,r7
	lcall	__gptrput
	pop	ar4
	pop	ar3
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_memcpy.c 44
	mov	dpl,_memcpy_sloc1_1_0
	mov	dph,(_memcpy_sloc1_1_0 + 1)
	mov	b,(_memcpy_sloc1_1_0 + 2)
00104$:
	C$_memcpy.c$45$1$1 ==.
	XG$memcpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
