;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:09 2000

;--------------------------------------------------------
	.module _divsint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __divsint_PARM_2
	.globl __divsint
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
__divsint_PARM_2:
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
	G$_divsint$0$0 ==.
;	_divsint.c 26
;	-----------------------------------------
;	 function _divsint
;	-----------------------------------------
__divsint:
	clr	ea
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_divsint.c 0
	mov	r2,dpl
	mov	r3,dph
;	_divsint.c 33
	mov	a,r3
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r4,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00113$:
	clr	c
	clr	a
	subb	a,r2
	mov	r5,a
	clr	a
	subb	a,r3
	mov	r6,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
	mov	ar5,r2
	mov	ar6,r3
00107$:
	mov	ar2,r5
	mov	ar3,r6
;	_divsint.c 34
	mov	a,(__divsint_PARM_2 + 1)
	rlc	a
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00114$:
	clr	c
	clr	a
	subb	a,__divsint_PARM_2
	mov	r6,a
	clr	a
	subb	a,(__divsint_PARM_2 + 1)
	mov	r7,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
	mov	r6,__divsint_PARM_2
	mov	r7,(__divsint_PARM_2 + 1)
00109$:
	mov	__divuint_PARM_2,r6
	mov	(__divuint_PARM_2 + 1),r7
	push	ar4
	push	ar5
	mov	dpl,r2
	mov	dph,r3
	lcall	__divuint
	mov	r2,dpl
	mov	r3,dph
	pop	ar5
	pop	ar4
;	_divsint.c 35
	mov	a,r5
	xrl	a,ar4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00115$:
;	_divsint.c 37
	clr	c
	clr	a
	subb	a,r2
	mov	dpl,a
	clr	a
	subb	a,r3
	mov	dph,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	_divsint.c 41
	mov	dpl,r2
	mov	dph,r3
00104$:
	setb	ea
	C$_divsint.c$43$1$1 ==.
	XG$_divsint$0$0 ==.
	ret
	.area	CSEG    (CODE)
