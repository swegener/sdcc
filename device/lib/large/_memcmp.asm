;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:30 2000

;--------------------------------------------------------
	.module _memcmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memcmp_PARM_3
	.globl _memcmp_PARM_2
	.globl _memcmp
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
_memcmp_PARM_2:
	.ds	0x0003
_memcmp_PARM_3:
	.ds	0x0002
_memcmp_buf1_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$memcmp$0$0 ==.
;	_memcmp.c 27
;	-----------------------------------------
;	 function memcmp
;	-----------------------------------------
_memcmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memcmp.c 0
	push	b
	push	dph
	push	dpl
	mov	dptr,#_memcmp_buf1_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_memcmp.c 33
	mov	dptr,#_memcmp_PARM_3
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r3,a
	orl  a,r2
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00112$
00113$:
;	_memcmp.c 34
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	ljmp	00107$
;	_memcmp.c 36
00112$:
	mov	dptr,#_memcmp_buf1_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#_memcmp_PARM_2
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
00104$:
	mov	dptr,#_memcmp_PARM_3
	movx	a,@dptr
	add	a,#0xff
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0xff
	mov	r1,a
	mov	dptr,#_memcmp_PARM_3
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	a,r0
	orl	a,r1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00114$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r0,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r1,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar0,00106$
; Peephole 201   removed redundant sjmp
00115$:
00116$:
;	_memcmp.c 37
	inc	r2
	cjne	r2,#0x00,00117$
	inc	r3
00117$:
	mov	dptr,#_memcmp_buf1_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_memcmp.c 38
	inc	r5
	cjne	r5,#0x00,00118$
	inc	r6
00118$:
	ljmp	00104$
00106$:
;	_memcmp.c 41
	mov	dptr,#_memcmp_buf1_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r5,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 106   removed redundant mov 
	mov  r2,a
	clr  c
	subb	a,r5
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	subb	a,#0x00
	mov	r5,a
	mov	dpl,r2
	mov	dph,r5
00107$:
	C$_memcmp.c$42$1$1 ==.
	XG$memcmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
