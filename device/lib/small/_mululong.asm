;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:15 2000

;--------------------------------------------------------
	.module _mululong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __mululong_PARM_2
	.globl __mululong
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
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
__mululong_PARM_2:
	.ds	0x0004
__mululong_a_1_1:
	.ds	0x0004
__mululong_t_1_1:
	.ds	0x0004
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
	G$_mululong$0$0 ==.
;	_mululong.c 59
;	-----------------------------------------
;	 function _mululong
;	-----------------------------------------
__mululong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_mululong.c 86
	mov	__mululong_a_1_1,dpl
	mov	(__mululong_a_1_1 + 1),dph
	mov	(__mululong_a_1_1 + 2),b
	mov	(__mululong_a_1_1 + 3),a
;	_mululong.c 63
	mov	b,0x0002 + __mululong_PARM_2
	mov	a,__mululong_a_1_1
	mul	ab
	mov	0x0002 + __mululong_t_1_1,a
	mov	(0x0002 + __mululong_t_1_1 + 1),b
;	_mululong.c 64
	mov	b,__mululong_PARM_2
	mov	a,__mululong_a_1_1
	mul	ab
	mov	__mululong_t_1_1,a
	mov	(__mululong_t_1_1 + 1),b
;	_mululong.c 65
;	_mululong.c 66
	mov	b,__mululong_PARM_2
	mov	a,0x0003 + __mululong_a_1_1
	mul	ab
; Peephole 105   removed redundant mov
	mov  r2,a
	add	a,0x0003 + __mululong_t_1_1
	mov	0x0003 + __mululong_t_1_1,a
;	_mululong.c 67
;	_mululong.c 68
	mov	b,0x0001 + __mululong_PARM_2
	mov	a,0x0002 + __mululong_a_1_1
	mul	ab
; Peephole 105   removed redundant mov
	mov  r2,a
	add	a,0x0003 + __mululong_t_1_1
	mov	0x0003 + __mululong_t_1_1,a
;	_mululong.c 69
	mov	b,__mululong_PARM_2
	mov	a,0x0002 + __mululong_a_1_1
	mul	ab
	mov	r2,a
	mov	r3,b
	mov	a,r2
	add	a,0x0002 + __mululong_t_1_1
	mov	r2,a
	mov	a,r3
	addc	a,(0x0002 + __mululong_t_1_1 + 1)
	mov	r3,a
	mov	0x0002 + __mululong_t_1_1,r2
	mov	(0x0002 + __mululong_t_1_1 + 1),r3
;	_mululong.c 71
	mov	b,0x0001 + __mululong_PARM_2
	mov	a,0x0001 + __mululong_a_1_1
	mul	ab
	mov	r2,a
	mov	r3,b
	mov	a,r2
	add	a,0x0002 + __mululong_t_1_1
	mov	r2,a
	mov	a,r3
	addc	a,(0x0002 + __mululong_t_1_1 + 1)
	mov	r3,a
	mov	0x0002 + __mululong_t_1_1,r2
	mov	(0x0002 + __mululong_t_1_1 + 1),r3
;	_mululong.c 73
;	_mululong.c 74
	mov	b,0x0002 + __mululong_PARM_2
	mov	a,0x0001 + __mululong_a_1_1
	mul	ab
	mov	0x0003 + __mululong_a_1_1,a
;	_mululong.c 75
;	_mululong.c 76
	mov	b,__mululong_PARM_2
	mov	a,0x0001 + __mululong_a_1_1
	mul	ab
	mov	0x0001 + __mululong_a_1_1,a
	mov	(0x0001 + __mululong_a_1_1 + 1),b
;	_mululong.c 78
;	_mululong.c 79
	mov	b,0x0003 + __mululong_PARM_2
	mov	a,__mululong_a_1_1
	mul	ab
	mov	0x0003 + __mululong_PARM_2,a
;	_mululong.c 81
	mov	b,0x0001 + __mululong_PARM_2
	mov	a,__mululong_a_1_1
	mul	ab
	mov	0x0001 + __mululong_PARM_2,a
	mov	(0x0001 + __mululong_PARM_2 + 1),b
;	_mululong.c 82
;	_mululong.c 83
;	_mululong.c 84
; Peephole 3.c   changed mov to clr
	clr  a
	mov  __mululong_PARM_2,a
; Peephole 105   removed redundant mov
	mov  __mululong_a_1_1,a
	add	a,__mululong_t_1_1
	mov	r2,a
	mov	a,(__mululong_a_1_1 + 1)
	addc	a,(__mululong_t_1_1 + 1)
	mov	r3,a
	mov	a,(__mululong_a_1_1 + 2)
	addc	a,(__mululong_t_1_1 + 2)
	mov	r4,a
	mov	a,(__mululong_a_1_1 + 3)
	addc	a,(__mululong_t_1_1 + 3)
	mov	r5,a
	mov	__mululong_t_1_1,r2
	mov	(__mululong_t_1_1 + 1),r3
	mov	(__mululong_t_1_1 + 2),r4
	mov	(__mululong_t_1_1 + 3),r5
;	_mululong.c 86
	mov	a,__mululong_PARM_2
	add	a,r2
	mov	dpl,a
	mov	a,(__mululong_PARM_2 + 1)
	addc	a,r3
	mov	dph,a
	mov	a,(__mululong_PARM_2 + 2)
	addc	a,r4
	mov	b,a
	mov	a,(__mululong_PARM_2 + 3)
	addc	a,r5
00101$:
	C$_mululong.c$87$1$1 ==.
	XG$_mululong$0$0 ==.
	ret
	.area	CSEG    (CODE)
