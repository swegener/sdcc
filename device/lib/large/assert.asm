;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:40 2000

;--------------------------------------------------------
	.module assert
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __assert
	.globl __assert_PARM_3
	.globl __assert_PARM_2
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
__assert_PARM_2:
	.ds	0x0003
__assert_PARM_3:
	.ds	0x0002
__assert_expr_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$_assert$0$0 ==.
;	assert.c 4
;	-----------------------------------------
;	 function _assert
;	-----------------------------------------
__assert:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	assert.c 0
	push	b
	push	dph
	push	dpl
	mov	dptr,#__assert_expr_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	assert.c 7
	mov	dptr,#__assert_expr_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	assert.c 6
	mov	r5,#__str_0
	mov	r6,#(__str_0 >> 8)
	mov	r7,#0x02
	mov	dptr,#__assert_PARM_2
	movx	a,@dptr
;  Peephole 100.a   removed redundant mov
	push	acc
	inc	dptr
	movx	a,@dptr
;  Peephole 100.a   removed redundant mov
	push	acc
	inc	dptr
	movx	a,@dptr
;  Peephole 100.a   removed redundant mov
	push	acc
	mov	dptr,#__assert_PARM_3
	movx	a,@dptr
;  Peephole 100.a   removed redundant mov
	push	acc
	inc	dptr
	movx	a,@dptr
;  Peephole 100.a   removed redundant mov
	push	acc
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	_printf
	mov	a,sp
	add	a,#0xfa
	mov	sp,a
;	assert.c 8
00102$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00104$:
	C$assert.c$9$1$1 ==.
	XG$_assert$0$0 ==.
	ret
	.area	CSEG    (CODE)
Fassert$_str_0$0$0 == .
__str_0:
	.ascii /Assert(%s) failed at line %u in file %s./
	.byte 0x0a
	.ascii //
	.byte	0
