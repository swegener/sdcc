;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

;--------------------------------------------------------
	.module _strtok
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strtok_PARM_2
	.globl _strtok
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
_strtok_PARM_2:
	.ds	0x0003
_strtok_str_1_1:
	.ds	0x0003
_strtok_s_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strtok$0$0 ==.
;	_strtok.c 32
;	-----------------------------------------
;	 function strtok
;	-----------------------------------------
_strtok:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strtok.c 0
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strtok_str_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strtok.c 40
	mov	dptr,#_strtok_str_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	a,r2
	orl	a,r3
	orl	a,r4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_strtok.c 41
	mov	dptr,#_strtok_s_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
00102$:
;	_strtok.c 43
	mov	dptr,#_strtok_s_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	_strtok.c 45
00105$:
	mov	dptr,#_strtok_s_1_1
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
	jnz	00115$
	ljmp	00107$
00115$:
;	_strtok.c 46
	push	ar2
	push	ar3
	push	ar4
	mov	dptr,#_strtok_PARM_2
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_strchr_PARM_2
	mov	a,r0
	movx	@dptr,a
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	_strchr
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r2
	orl	a,r3
	orl	a,r4
	pop	ar4
	pop	ar3
	pop	ar2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00116$:
;	_strtok.c 47
	mov	dptr,#_strtok_s_1_1
	mov	a,#0x01
	add	a,r5
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r6
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
; Peephole 180   changed mov to clr
	clr  a
	lcall	__gptrput
;	_strtok.c 48
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00104$:
;	_strtok.c 50
	mov	dptr,#_strtok_s_1_1
	mov	a,#0x01
	add	a,r5
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r6
	inc	dptr
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	ljmp	00105$
00107$:
;	_strtok.c 52
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00108$:
	C$_strtok.c$53$1$1 ==.
	XG$strtok$0$0 ==.
	ret
	.area	CSEG    (CODE)
