;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:21 2000

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
__assert_PARM_2:
	.ds	0x0003
__assert_PARM_3:
	.ds	0x0002
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
;	assert.c 6
	mov	r2,#__str_0
	mov	r3,#(__str_0 >> 8)
	mov	r4,#0x02
	push	__assert_PARM_2
	push	(__assert_PARM_2 + 1)
	push	(__assert_PARM_2 + 2)
	push	__assert_PARM_3
	push	(__assert_PARM_3 + 1)
	push	ar2
	push	ar3
	push	ar4
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
