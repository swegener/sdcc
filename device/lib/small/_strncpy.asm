;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strncpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strncpy_PARM_3
	.globl _strncpy_PARM_2
	.globl _strncpy
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
_strncpy_PARM_2:
	.ds	0x0003
_strncpy_PARM_3:
	.ds	0x0002
_strncpy_d1_1_1:
	.ds	0x0003
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
	G$strncpy$0$0 ==.
;	_strncpy.c 27
;	-----------------------------------------
;	 function strncpy
;	-----------------------------------------
_strncpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strncpy.c 36
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strncpy.c 32
	mov	_strncpy_d1_1_1,r2
	mov	(_strncpy_d1_1_1 + 1),r3
	mov	(_strncpy_d1_1_1 + 2),r4
;	_strncpy.c 34
00101$:
	mov	r5,_strncpy_PARM_3
	mov	r0,(_strncpy_PARM_3 + 1)
	dec	_strncpy_PARM_3
	mov	a,#0xff
	cjne	a,_strncpy_PARM_3,00108$
	dec	(_strncpy_PARM_3 + 1)
00108$:
	mov	a,r5
	orl	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_strncpy.c 35
	mov	ar5,r2
	mov	ar0,r3
	mov	ar1,r4
	inc	r2
	cjne	r2,#0x00,00110$
	inc	r3
00110$:
	mov	dpl,_strncpy_PARM_2
	mov	dph,(_strncpy_PARM_2 + 1)
	mov	b,(_strncpy_PARM_2 + 2)
	inc	_strncpy_PARM_2
	clr	a
	cjne	a,_strncpy_PARM_2,00111$
	inc	(_strncpy_PARM_2 + 1)
00111$:
	lcall	__gptrget
; Peephole 191   removed redundant mov
	mov  r6,a
	mov  dpl,r5
	mov  dph,r0
	mov  b,r1
	lcall	__gptrput
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_strncpy.c 36
	mov	dpl,_strncpy_d1_1_1
	mov	dph,(_strncpy_d1_1_1 + 1)
	mov	b,(_strncpy_d1_1_1 + 2)
00104$:
	C$_strncpy.c$37$1$1 ==.
	XG$strncpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
