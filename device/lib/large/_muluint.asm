;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:31 2000

;--------------------------------------------------------
	.module _muluint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __muluint_PARM_2
	.globl __muluint
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
__muluint_PARM_2:
	.ds	0x0002
__muluint_a_1_1:
	.ds	0x0002
__muluint_t_1_1:
	.ds	0x0002
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_muluint$0$0 ==.
;	_muluint.c 32
;	-----------------------------------------
;	 function _muluint
;	-----------------------------------------
__muluint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_muluint.c 51
	push	dph
	push	dpl
	mov	dptr,#__muluint_a_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_muluint.c 38
;	_muluint.c 39
;	_muluint.c 48
	mov	dptr,#(__muluint_a_1_1)
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#(__muluint_PARM_2)
	movx	a,@dptr
	mov	r3,a
	mov	b,r3
	mov	a,r2
	mul	ab
	mov	r4,a
	mov	r5,b
	mov	dptr,#(__muluint_t_1_1)
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	_muluint.c 49
	mov	dptr,#(0x0001 + __muluint_t_1_1)
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#(0x0001 + __muluint_PARM_2)
	movx	a,@dptr
	mov	r5,a
	mov	b,r5
	mov	a,r2
	mul	ab
	mov	r2,a
	mov	dptr,#(0x0001 + __muluint_a_1_1)
	movx	a,@dptr
	mov	r5,a
	mov	b,r3
	mov	a,r5
	mul	ab
; Peephole 105   removed redundant mov
	mov  r3,a
	add	a,r2
	add	a,r4
; Peephole 100   removed redundant mov
	mov  r4,a
	mov  dptr,#(0x0001 + __muluint_t_1_1)
	movx @dptr,a
;	_muluint.c 51
	mov	dptr,#(__muluint_t_1_1)
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
00101$:
	C$_muluint.c$52$1$1 ==.
	XG$_muluint$0$0 ==.
	ret
	.area	CSEG    (CODE)
