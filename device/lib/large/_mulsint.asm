;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:31 2000

;--------------------------------------------------------
	.module _mulsint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __mulsint_PARM_2
	.globl __mulsint
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
__mulsint_PARM_2:
	.ds	0x0002
__mulsint_a_1_1:
	.ds	0x0002
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_mulsint$0$0 ==.
;	_mulsint.c 26
;	-----------------------------------------
;	 function _mulsint
;	-----------------------------------------
__mulsint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_mulsint.c 0
	push	dph
	push	dpl
	mov	dptr,#__mulsint_a_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_mulsint.c 30
	mov	dptr,#__mulsint_a_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r3,a
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00106$
00113$:
	clr	c
	clr	a
	subb	a,r2
	mov	r4,a
	clr	a
	subb	a,r3
	mov	r5,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
	mov	ar4,r2
	mov	ar5,r3
00107$:
	mov	ar2,r4
	mov	ar3,r5
;	_mulsint.c 31
	mov	dptr,#__mulsint_PARM_2
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r5,a
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00114$:
	clr	c
	clr	a
	subb	a,r4
	mov	r7,a
	clr	a
	subb	a,r5
	mov	r0,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
	mov	ar7,r4
	mov	ar0,r5
00109$:
	mov	dptr,#__muluint_PARM_2
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	push	ar6
	mov	dpl,r2
	mov	dph,r3
	lcall	__muluint
	mov	r2,dpl
	mov	r3,dph
	pop	ar6
;	_mulsint.c 33
	mov	dptr,#__mulsint_a_1_1
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
; Peephole 105   removed redundant mov
	mov  r5,a
	rlc	a
	clr	a
	rlc	a
	mov	r4,a
	mov	a,r6
	xrl	a,ar4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00115$:
;	_mulsint.c 34
	clr	c
	clr	a
	subb	a,r2
	mov	r4,a
	clr	a
	subb	a,r3
	mov	r6,a
	mov	dpl,r4
	mov	dph,r6
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_mulsint.c 36
	mov	dpl,r2
	mov	dph,r3
00104$:
	C$_mulsint.c$37$1$1 ==.
	XG$_mulsint$0$0 ==.
	ret
	.area	CSEG    (CODE)
