;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:14 2000

;--------------------------------------------------------
	.module _moduint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __moduint_PARM_2
	.globl __moduint
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
__moduint_PARM_2:
	.ds	0x0002
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
	G$_moduint$0$0 ==.
;	_moduint.c 30
;	-----------------------------------------
;	 function _moduint
;	-----------------------------------------
__moduint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_moduint.c 52
	mov	r2,dpl
	mov	r3,dph
;	_moduint.c 32
	mov	r4,#0x00
;	_moduint.c 35
	mov	r5,#0x00
00103$:
	mov	a,(__moduint_PARM_2 + 1)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00117$
00119$:
;	_moduint.c 36
	mov	r6,__moduint_PARM_2
	mov	a,(__moduint_PARM_2 + 1)
	xch	a,r6
	add	a,acc
	xch	a,r6
	rlc	a
	mov	r7,a
	mov	__moduint_PARM_2,r6
	mov	(__moduint_PARM_2 + 1),r7
;	_moduint.c 37
	clr	c
	mov	a,r2
	subb	a,r6
	mov	a,r3
	subb	a,r7
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00102$
00120$:
;	_moduint.c 39
	mov	__moduint_PARM_2,r6
	mov	a,r7
	clr	c
	rrc	a
	xch	a,__moduint_PARM_2
	rrc	a
	xch	a,__moduint_PARM_2
	mov	(__moduint_PARM_2 + 1),a
;	_moduint.c 40
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00102$:
;	_moduint.c 42
	inc	r5
	mov	ar4,r5
;	_moduint.c 45
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00117$:
00108$:
;	_moduint.c 46
	clr	c
	mov	a,r2
	subb	a,__moduint_PARM_2
	mov	a,r3
	subb	a,(__moduint_PARM_2 + 1)
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00121$:
;	_moduint.c 47
	clr	c
	mov	a,r2
	subb	a,__moduint_PARM_2
	mov	r2,a
	mov	a,r3
	subb	a,(__moduint_PARM_2 + 1)
	mov	r3,a
00107$:
;	_moduint.c 49
	mov	a,(__moduint_PARM_2 + 1)
	clr	c
	rrc	a
	xch	a,__moduint_PARM_2
	rrc	a
	xch	a,__moduint_PARM_2
	mov	(__moduint_PARM_2 + 1),a
;	_moduint.c 50
	mov	ar5,r4
	dec	r4
	mov	a,r5
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00108$
00122$:
;	_moduint.c 52
	mov	dpl,r2
	mov	dph,r3
00111$:
	C$_moduint.c$53$1$1 ==.
	XG$_moduint$0$0 ==.
	ret
	.area	CSEG    (CODE)
