;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:12 2000

;--------------------------------------------------------
	.module _fsneq
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fsneq_PARM_2
	.globl ___fsneq
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
___fsneq_PARM_2:
	.ds	0x0004
___fsneq_fl1_1_1:
	.ds	0x0004
___fsneq_fl2_1_1:
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
	G$__fsneq$0$0 ==.
;	_fsneq.c 73
;	-----------------------------------------
;	 function __fsneq
;	-----------------------------------------
___fsneq:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fsneq.c 87
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fsneq.c 77
	mov	___fsneq_fl1_1_1,r2
	mov	(___fsneq_fl1_1_1 + 1),r3
	mov	(___fsneq_fl1_1_1 + 2),r4
	mov	(___fsneq_fl1_1_1 + 3),r5
;	_fsneq.c 78
	mov	___fsneq_fl2_1_1,___fsneq_PARM_2
	mov	(___fsneq_fl2_1_1 + 1),(___fsneq_PARM_2 + 1)
	mov	(___fsneq_fl2_1_1 + 2),(___fsneq_PARM_2 + 2)
	mov	(___fsneq_fl2_1_1 + 3),(___fsneq_PARM_2 + 3)
;	_fsneq.c 80
	mov	a,(___fsneq_fl1_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00111$:
	mov	a,(___fsneq_fl2_1_1 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00112$:
;	_fsneq.c 82
	mov	r2,___fsneq_fl1_1_1
	mov	r3,(___fsneq_fl1_1_1 + 1)
	mov	r4,(___fsneq_fl1_1_1 + 2)
	mov	a,#0x80
	xrl	a,(___fsneq_fl1_1_1 + 3)
	mov	r5,a
	mov	___fsneq_fl1_1_1,r2
	mov	(___fsneq_fl1_1_1 + 1),r3
	mov	(___fsneq_fl1_1_1 + 2),r4
	mov	(___fsneq_fl1_1_1 + 3),r5
;	_fsneq.c 83
	mov	r2,___fsneq_fl2_1_1
	mov	r3,(___fsneq_fl2_1_1 + 1)
	mov	r4,(___fsneq_fl2_1_1 + 2)
	mov	a,#0x80
	xrl	a,(___fsneq_fl2_1_1 + 3)
	mov	r5,a
	mov	___fsneq_fl2_1_1,r2
	mov	(___fsneq_fl2_1_1 + 1),r3
	mov	(___fsneq_fl2_1_1 + 2),r4
	mov	(___fsneq_fl2_1_1 + 3),r5
00102$:
;	_fsneq.c 85
	mov	a,___fsneq_fl1_1_1
; Peephole 132   changed ljmp to sjmp
; Peephole 193   optimized misc jump sequence
	cjne a,___fsneq_fl2_1_1,00105$
	mov  a,(___fsneq_fl1_1_1 + 1)
	cjne a,(___fsneq_fl2_1_1 + 1),00105$
	mov  a,(___fsneq_fl1_1_1 + 2)
	cjne a,(___fsneq_fl2_1_1 + 2),00105$
	mov  a,(___fsneq_fl1_1_1 + 3)
	cjne a,(___fsneq_fl2_1_1 + 3),00105$
; Peephole 201   removed redundant sjmp
00113$:
00114$:
;	_fsneq.c 86
	mov	dpl,#0x00
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	_fsneq.c 87
	mov	dpl,#0x01
00106$:
	C$_fsneq.c$88$1$1 ==.
	XG$__fsneq$0$0 ==.
	ret
	.area	CSEG    (CODE)
