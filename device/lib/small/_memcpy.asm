;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _memcpy
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memcpy_PARM_3
	.globl _memcpy_PARM_2
	.globl _memcpy
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
_memcpy_PARM_2:
	.ds	0x0003
_memcpy_PARM_3:
	.ds	0x0002
_memcpy_dst_1_1:
	.ds	0x0003
_memcpy_ret_1_1:
	.ds	0x0003
_memcpy_s_1_1:
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
	G$memcpy$0$0 ==.
;	_memcpy.c 27
;	-----------------------------------------
;	 function memcpy
;	-----------------------------------------
_memcpy:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memcpy.c 44
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_memcpy.c 33
	mov	_memcpy_ret_1_1,r2
	mov	(_memcpy_ret_1_1 + 1),r3
	mov	(_memcpy_ret_1_1 + 2),r4
;	_memcpy.c 34
	mov	_memcpy_dst_1_1,r2
	mov	(_memcpy_dst_1_1 + 1),r3
	mov	(_memcpy_dst_1_1 + 2),r4
;	_memcpy.c 35
;	_memcpy.c 40
	mov	r3,_memcpy_dst_1_1
	mov	r4,(_memcpy_dst_1_1 + 1)
	mov	r2,(_memcpy_dst_1_1 + 2)
	mov	_memcpy_s_1_1,_memcpy_PARM_2
	mov	(_memcpy_s_1_1 + 1),(_memcpy_PARM_2 + 1)
	mov	(_memcpy_s_1_1 + 2),(_memcpy_PARM_2 + 2)
00101$:
	mov	r5,_memcpy_PARM_3
	mov	r0,(_memcpy_PARM_3 + 1)
	dec	_memcpy_PARM_3
	mov	a,#0xff
	cjne	a,_memcpy_PARM_3,00108$
	dec	(_memcpy_PARM_3 + 1)
00108$:
	mov	a,r5
	orl	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_memcpy.c 41
	mov	ar5,r3
	mov	ar0,r4
	mov	ar1,r2
	inc	r3
	cjne	r3,#0x00,00110$
	inc	r4
00110$:
	mov	dpl,_memcpy_s_1_1
	mov	dph,(_memcpy_s_1_1 + 1)
	mov	b,(_memcpy_s_1_1 + 2)
	inc	_memcpy_s_1_1
	clr	a
	cjne	a,_memcpy_s_1_1,00111$
	inc	(_memcpy_s_1_1 + 1)
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
;	_memcpy.c 44
	mov	dpl,_memcpy_ret_1_1
	mov	dph,(_memcpy_ret_1_1 + 1)
	mov	b,(_memcpy_ret_1_1 + 2)
00104$:
	C$_memcpy.c$45$1$1 ==.
	XG$memcpy$0$0 ==.
	ret
	.area	CSEG    (CODE)
