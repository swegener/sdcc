;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:14 2000

;--------------------------------------------------------
	.module _modslong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __modslong_PARM_2
	.globl __modslong
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
__modslong_PARM_2:
	.ds	0x0004
__modslong_sloc0_1_0:
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
	G$_modslong$0$0 ==.
;	_modslong.c 24
;	-----------------------------------------
;	 function _modslong
;	-----------------------------------------
__modslong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_modslong.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_modslong.c 28
; Peephole 105   removed redundant mov
	mov  r5,a
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00113$:
	clr	c
	clr	a
	subb	a,r2
	mov	__modslong_sloc0_1_0,a
	clr	a
	subb	a,r3
	mov	(__modslong_sloc0_1_0 + 1),a
	clr	a
	subb	a,r4
	mov	(__modslong_sloc0_1_0 + 2),a
	clr	a
	subb	a,r5
	mov	(__modslong_sloc0_1_0 + 3),a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
	mov	__modslong_sloc0_1_0,r2
	mov	(__modslong_sloc0_1_0 + 1),r3
	mov	(__modslong_sloc0_1_0 + 2),r4
	mov	(__modslong_sloc0_1_0 + 3),r5
00107$:
	mov	r2,__modslong_sloc0_1_0
	mov	r3,(__modslong_sloc0_1_0 + 1)
	mov	r4,(__modslong_sloc0_1_0 + 2)
	mov	r5,(__modslong_sloc0_1_0 + 3)
;	_modslong.c 29
	mov	a,(__modslong_PARM_2 + 3)
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00114$:
	clr	c
	clr	a
	subb	a,__modslong_PARM_2
	mov	__modslong_sloc0_1_0,a
	clr	a
	subb	a,(__modslong_PARM_2 + 1)
	mov	(__modslong_sloc0_1_0 + 1),a
	clr	a
	subb	a,(__modslong_PARM_2 + 2)
	mov	(__modslong_sloc0_1_0 + 2),a
	clr	a
	subb	a,(__modslong_PARM_2 + 3)
	mov	(__modslong_sloc0_1_0 + 3),a
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
	mov	__modslong_sloc0_1_0,__modslong_PARM_2
	mov	(__modslong_sloc0_1_0 + 1),(__modslong_PARM_2 + 1)
	mov	(__modslong_sloc0_1_0 + 2),(__modslong_PARM_2 + 2)
	mov	(__modslong_sloc0_1_0 + 3),(__modslong_PARM_2 + 3)
00109$:
	mov	__modulong_PARM_2,__modslong_sloc0_1_0
	mov	(__modulong_PARM_2 + 1),(__modslong_sloc0_1_0 + 1)
	mov	(__modulong_PARM_2 + 2),(__modslong_sloc0_1_0 + 2)
	mov	(__modulong_PARM_2 + 3),(__modslong_sloc0_1_0 + 3)
	push	ar6
	push	ar7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	pop	ar7
	pop	ar6
;	_modslong.c 31
	mov	a,r7
	xrl	a,ar6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00115$:
;	_modslong.c 32
	clr	c
	clr	a
	subb	a,r2
	mov	dpl,a
	clr	a
	subb	a,r3
	mov	dph,a
	clr	a
	subb	a,r4
	mov	b,a
	clr	a
	subb	a,r5
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_modslong.c 34
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
00104$:
	C$_modslong.c$35$1$1 ==.
	XG$_modslong$0$0 ==.
	ret
	.area	CSEG    (CODE)
