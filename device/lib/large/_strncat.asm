;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:33 2000

;--------------------------------------------------------
	.module _strncat
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strncat_PARM_3
	.globl _strncat_PARM_2
	.globl _strncat
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
_strncat_sloc0_1_0:
	.ds	0x0003
_strncat_sloc1_1_0:
	.ds	0x0003
_strncat_sloc2_1_0:
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
_strncat_PARM_2:
	.ds	0x0003
_strncat_PARM_3:
	.ds	0x0002
_strncat_front_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strncat$0$0 ==.
;	_strncat.c 27
;	-----------------------------------------
;	 function strncat
;	-----------------------------------------
_strncat:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strncat.c 44
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strncat_front_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strncat.c 33
	mov	dptr,#_strncat_front_1_1
	movx	a,@dptr
	mov	_strncat_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strncat_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strncat_sloc0_1_0 + 2),a
;	_strncat.c 35
00101$:
	mov	dptr,#_strncat_front_1_1
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x01
	add	a,r5
	mov	r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r6
	mov	r1,a
	mov	ar2,r7
	mov	dptr,#_strncat_front_1_1
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00116$:
;	_strncat.c 37
	mov	a,r0
	add	a,#0xff
	mov	r4,a
	mov	a,r1
	addc	a,#0xff
	mov	r3,a
	mov	dptr,#_strncat_front_1_1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
;	_strncat.c 39
	mov	_strncat_sloc2_1_0,r4
	mov	(_strncat_sloc2_1_0 + 1),r3
	mov	(_strncat_sloc2_1_0 + 2),r2
	mov	dptr,#_strncat_PARM_2
	movx	a,@dptr
	mov	_strncat_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strncat_sloc1_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strncat_sloc1_1_0 + 2),a
	mov	dptr,#_strncat_PARM_3
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
00106$:
	mov	ar1,r5
	mov	ar2,r0
	dec	r5
	cjne	r5,#0xff,00117$
	dec	r0
00117$:
	mov	a,r1
	orl	a,r2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00118$:
;	_strncat.c 40
	mov	r2,_strncat_sloc2_1_0
	mov	r3,(_strncat_sloc2_1_0 + 1)
	mov	r4,(_strncat_sloc2_1_0 + 2)
	inc	_strncat_sloc2_1_0
	clr	a
	cjne	a,_strncat_sloc2_1_0,00119$
	inc	(_strncat_sloc2_1_0 + 1)
00119$:
	mov	dptr,#_strncat_front_1_1
	mov	a,_strncat_sloc2_1_0
	movx	@dptr,a
	inc	dptr
	mov	a,(_strncat_sloc2_1_0 + 1)
	movx	@dptr,a
	inc	dptr
	mov	a,(_strncat_sloc2_1_0 + 2)
	movx	@dptr,a
	mov	dpl,_strncat_sloc1_1_0
	mov	dph,(_strncat_sloc1_1_0 + 1)
	mov	b,(_strncat_sloc1_1_0 + 2)
	inc	_strncat_sloc1_1_0
	clr	a
	cjne	a,_strncat_sloc1_1_0,00120$
	inc	(_strncat_sloc1_1_0 + 1)
00120$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r1,a
	mov  dpl,r2
	mov  dph,r3
	mov  b,r4
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00106$
00121$:
;	_strncat.c 41
	mov	dpl,_strncat_sloc0_1_0
	mov	dph,(_strncat_sloc0_1_0 + 1)
	mov	b,(_strncat_sloc0_1_0 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	_strncat.c 43
	mov	dptr,#_strncat_front_1_1
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
; Peephole 180   changed mov to clr
	clr  a
	lcall	__gptrput
;	_strncat.c 44
	mov	dpl,_strncat_sloc0_1_0
	mov	dph,(_strncat_sloc0_1_0 + 1)
	mov	b,(_strncat_sloc0_1_0 + 2)
00109$:
	C$_strncat.c$45$1$1 ==.
	XG$strncat$0$0 ==.
	ret
	.area	CSEG    (CODE)
