;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:12 2000

;--------------------------------------------------------
	.module _fssub
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl ___fssub_PARM_2
	.globl ___fssub
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
___fssub_PARM_2:
	.ds	0x0004
___fssub_fl1_1_1:
	.ds	0x0004
___fssub_fl2_1_1:
	.ds	0x0004
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
	G$__fssub$0$0 ==.
;	_fssub.c 81
;	-----------------------------------------
;	 function __fssub
;	-----------------------------------------
___fssub:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_fssub.c 96
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_fssub.c 85
	mov	___fssub_fl1_1_1,r2
	mov	(___fssub_fl1_1_1 + 1),r3
	mov	(___fssub_fl1_1_1 + 2),r4
	mov	(___fssub_fl1_1_1 + 3),r5
;	_fssub.c 86
	mov	___fssub_fl2_1_1,___fssub_PARM_2
	mov	(___fssub_fl2_1_1 + 1),(___fssub_PARM_2 + 1)
	mov	(___fssub_fl2_1_1 + 2),(___fssub_PARM_2 + 2)
	mov	(___fssub_fl2_1_1 + 3),(___fssub_PARM_2 + 3)
;	_fssub.c 89
	mov	a,___fssub_fl2_1_1
	orl	a,(___fssub_fl2_1_1 + 1)
	orl	a,(___fssub_fl2_1_1 + 2)
	orl	a,(___fssub_fl2_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00109$:
;	_fssub.c 90
	mov	dpl,___fssub_fl1_1_1
	mov	dph,(___fssub_fl1_1_1 + 1)
	mov	b,(___fssub_fl1_1_1 + 2)
	mov	a,(___fssub_fl1_1_1 + 3)
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00102$:
;	_fssub.c 91
	mov	a,___fssub_fl1_1_1
	orl	a,(___fssub_fl1_1_1 + 1)
	orl	a,(___fssub_fl1_1_1 + 2)
	orl	a,(___fssub_fl1_1_1 + 3)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00110$:
;	_fssub.c 92
	mov	a,(___fssub_fl2_1_1 + 3)
	cpl	acc.7
	mov	dpl,___fssub_fl2_1_1
	mov	dph,(___fssub_fl2_1_1 + 1)
	mov	b,(___fssub_fl2_1_1 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00104$:
;	_fssub.c 95
	mov	r2,___fssub_fl2_1_1
	mov	r3,(___fssub_fl2_1_1 + 1)
	mov	r4,(___fssub_fl2_1_1 + 2)
	mov	a,#0x80
	xrl	a,(___fssub_fl2_1_1 + 3)
	mov	r5,a
	mov	___fssub_fl2_1_1,r2
	mov	(___fssub_fl2_1_1 + 1),r3
	mov	(___fssub_fl2_1_1 + 2),r4
	mov	(___fssub_fl2_1_1 + 3),r5
;	_fssub.c 96
	mov	___fsadd_PARM_2,___fssub_fl2_1_1
	mov	(___fsadd_PARM_2 + 1),(___fssub_fl2_1_1 + 1)
	mov	(___fsadd_PARM_2 + 2),(___fssub_fl2_1_1 + 2)
	mov	(___fsadd_PARM_2 + 3),(___fssub_fl2_1_1 + 3)
	mov	dpl,___fssub_fl1_1_1
	mov	dph,(___fssub_fl1_1_1 + 1)
	mov	b,(___fssub_fl1_1_1 + 2)
	mov	a,(___fssub_fl1_1_1 + 3)
	lcall	___fsadd
	C$_fssub.c$97$1$1 ==.
	XG$__fssub$0$0 ==.
; Peephole 156   removed misc moves via dph, dpl, b, a before return
00105$:
	ret
	.area	CSEG    (CODE)
