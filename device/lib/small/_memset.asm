;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:14 2000

;--------------------------------------------------------
	.module _memset
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memset_PARM_3
	.globl _memset_PARM_2
	.globl _memset
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
_memset_PARM_2:
	.ds	0x0001
_memset_PARM_3:
	.ds	0x0002
_memset_buf_1_1:
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
	G$memset$0$0 ==.
;	_memset.c 27
;	-----------------------------------------
;	 function memset
;	-----------------------------------------
_memset:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memset.c 40
	mov	_memset_buf_1_1,dpl
	mov	(_memset_buf_1_1 + 1),dph
	mov	(_memset_buf_1_1 + 2),b
;	_memset.c 33
	mov	r5,_memset_buf_1_1
	mov	r6,(_memset_buf_1_1 + 1)
	mov	r7,(_memset_buf_1_1 + 2)
;	_memset.c 35
	mov	r0,_memset_PARM_3
	mov	r1,(_memset_PARM_3 + 1)
00101$:
	mov	ar2,r0
	mov	ar3,r1
	dec	r0
	cjne	r0,#0xff,00108$
	dec	r1
00108$:
	mov	a,r2
	orl	a,r3
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_memset.c 36
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,_memset_PARM_2
	lcall	__gptrput
;	_memset.c 37
	inc	r5
	cjne	r5,#0x00,00110$
	inc	r6
00110$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_memset.c 40
	mov	dpl,_memset_buf_1_1
	mov	dph,(_memset_buf_1_1 + 1)
	mov	b,(_memset_buf_1_1 + 2)
00104$:
	C$_memset.c$41$1$1 ==.
	XG$memset$0$0 ==.
	ret
	.area	CSEG    (CODE)
