;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strcmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcmp_PARM_2
	.globl _strcmp
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
_strcmp_PARM_2:
	.ds	0x0003
_strcmp_ret_1_1:
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
	G$strcmp$0$0 ==.
;	_strcmp.c 27
;	-----------------------------------------
;	 function strcmp
;	-----------------------------------------
_strcmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcmp.c 42
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strcmp.c 34
	mov	r5,_strcmp_PARM_2
	mov	r6,(_strcmp_PARM_2 + 1)
	mov	r7,(_strcmp_PARM_2 + 2)
00102$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r0,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 106   removed redundant mov 
	mov  r1,a
	clr  c
	subb	a,r0
; Peephole 166   removed redundant mov
	mov  r1,a
	mov  _strcmp_ret_1_1,r1 
	rlc	a
	subb	a,acc
	mov	(_strcmp_ret_1_1 + 1),a
	mov	a,r1
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00117$:
	mov	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00118$:
;	_strcmp.c 35
	inc	r2
	cjne	r2,#0x00,00119$
	inc	r3
00119$:
	inc	r5
	cjne	r5,#0x00,00120$
	inc	r6
00120$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00104$:
;	_strcmp.c 37
	mov	a,(_strcmp_ret_1_1 + 1)
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00108$
00121$:
;	_strcmp.c 38
	mov	_strcmp_ret_1_1,#0xff
	mov	(_strcmp_ret_1_1 + 1),#0xff
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	_strcmp.c 39
	clr	c
; Peephole 180   changed mov to clr
	clr  a
	subb	a,_strcmp_ret_1_1
; Peephole 159   avoided xrl during execution
	mov  a,#(0x00 ^ 0x80)
	mov	b,(_strcmp_ret_1_1 + 1)
	xrl	b,#0x80
	subb	a,b
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00122$:
;	_strcmp.c 40
	clr	a
	mov	(_strcmp_ret_1_1 + 1),a
	mov	_strcmp_ret_1_1,#0x01
00109$:
;	_strcmp.c 42
	mov	dpl,_strcmp_ret_1_1
	mov	dph,(_strcmp_ret_1_1 + 1)
00110$:
	C$_strcmp.c$43$1$1 ==.
	XG$strcmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
