;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strpbrk
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strpbrk_PARM_2
	.globl _strpbrk
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
_strpbrk_PARM_2:
	.ds	0x0003
_strpbrk_string_1_1:
	.ds	0x0003
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
	G$strpbrk$0$0 ==.
;	_strpbrk.c 27
;	-----------------------------------------
;	 function strpbrk
;	-----------------------------------------
_strpbrk:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strpbrk.c 0
	mov	_strpbrk_string_1_1,dpl
	mov	(_strpbrk_string_1_1 + 1),dph
	mov	(_strpbrk_string_1_1 + 2),b
;	_strpbrk.c 34
	mov	r5,_strpbrk_PARM_2
	mov	r6,(_strpbrk_PARM_2 + 1)
	mov	r7,(_strpbrk_PARM_2 + 2)
00103$:
	mov	dpl,_strpbrk_string_1_1
	mov	dph,(_strpbrk_string_1_1 + 1)
	mov	b,(_strpbrk_string_1_1 + 2)
	lcall	__gptrget
; Peephole 166   removed redundant mov
	mov  r0,a
	mov  ar1,r0 
; Peephole 110   removed ljmp by inverse jump logic
	jz  00105$
00111$:
;	_strpbrk.c 35
	mov	_strchr_PARM_2,r1
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	_strchr
	mov	r0,dpl
	mov	r1,dph
	mov	r2,b
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r0
	orl	a,r1
	orl	a,r2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00112$:
;	_strpbrk.c 36
	mov	dpl,_strpbrk_string_1_1
	mov	dph,(_strpbrk_string_1_1 + 1)
	mov	b,(_strpbrk_string_1_1 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	_strpbrk.c 39
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00106$:
	C$_strpbrk.c$40$1$1 ==.
	XG$strpbrk$0$0 ==.
	ret
	.area	CSEG    (CODE)
