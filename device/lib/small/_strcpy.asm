;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strcpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcpy_PARM_2
	.globl _strcpy
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
_strcpy_PARM_2:
	.ds	0x0003
_strcpy_d_1_1:
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
	G$strcpy$0$0 ==.
;	_strcpy.c 27
;	-----------------------------------------
;	 function strcpy
;	-----------------------------------------
_strcpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcpy.c 35
	mov	_strcpy_d_1_1,dpl
	mov	(_strcpy_d_1_1 + 1),dph
	mov	(_strcpy_d_1_1 + 2),b
;	_strcpy.c 33
	mov	r5,_strcpy_d_1_1
	mov	r6,(_strcpy_d_1_1 + 1)
	mov	r7,(_strcpy_d_1_1 + 2)
00101$:
	mov	ar3,r5
	mov	ar4,r6
	mov	ar2,r7
	inc	r5
	cjne	r5,#0x00,00108$
	inc	r6
00108$:
	mov	dpl,_strcpy_PARM_2
	mov	dph,(_strcpy_PARM_2 + 1)
	mov	b,(_strcpy_PARM_2 + 2)
	inc	_strcpy_PARM_2
	clr	a
	cjne	a,_strcpy_PARM_2,00109$
	inc	(_strcpy_PARM_2 + 1)
00109$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r0,a
	mov  dpl,r3
	mov  dph,r4
	mov  b,r2
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00110$:
;	_strcpy.c 35
	mov	dpl,_strcpy_d_1_1
	mov	dph,(_strcpy_d_1_1 + 1)
	mov	b,(_strcpy_d_1_1 + 2)
00104$:
	C$_strcpy.c$36$1$1 ==.
	XG$strcpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
