;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:30 2000

;--------------------------------------------------------
	.module _memset
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memset_PARM_3
	.globl _memset_PARM_2
	.globl _memset
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
_memset_PARM_2:
	.ds	0x0001
_memset_PARM_3:
	.ds	0x0002
_memset_buf_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$memset$0$0 ==.
;	_memset.c 27
;	-----------------------------------------
;	 function memset
;	-----------------------------------------
_memset:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memset.c 40
	push	b
	push	dph
	push	dpl
	mov	dptr,#_memset_buf_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_memset.c 33
	mov	dptr,#_memset_buf_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	_memset.c 35
	mov	dptr,#_memset_PARM_2
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_memset_PARM_3
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
00101$:
	mov	ar0,r6
	mov	ar1,r7
	dec	r6
	cjne	r6,#0xff,00108$
	dec	r7
00108$:
	mov	a,r0
	orl	a,r1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_memset.c 36
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__gptrput
;	_memset.c 37
	inc	r2
	cjne	r2,#0x00,00110$
	inc	r3
00110$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_memset.c 40
	mov	dptr,#_memset_buf_1_1
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
	C$_memset.c$41$1$1 ==.
	XG$memset$0$0 ==.
	ret
	.area	CSEG    (CODE)
