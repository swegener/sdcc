;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:14 2000

;--------------------------------------------------------
	.module _muluint
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __muluint_PARM_2
	.globl __muluint
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
__muluint_PARM_2:
	.ds	0x0002
__muluint_a_1_1:
	.ds	0x0002
__muluint_t_1_1:
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
	G$_muluint$0$0 ==.
;	_muluint.c 32
;	-----------------------------------------
;	 function _muluint
;	-----------------------------------------
__muluint:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_muluint.c 51
	mov	__muluint_a_1_1,dpl
	mov	(__muluint_a_1_1 + 1),dph
;	_muluint.c 44
;	_muluint.c 45
;	_muluint.c 48
	mov	b,__muluint_PARM_2
	mov	a,__muluint_a_1_1
	mul	ab
	mov	__muluint_t_1_1,a
	mov	(__muluint_t_1_1 + 1),b
;	_muluint.c 49
	mov	b,0x0001 + __muluint_PARM_2
	mov	a,__muluint_a_1_1
	mul	ab
	mov	r2,a
	mov	b,__muluint_PARM_2
	mov	a,0x0001 + __muluint_a_1_1
	mul	ab
; Peephole 105   removed redundant mov
	mov  r3,a
	add	a,r2
	add	a,0x0001 + __muluint_t_1_1
	mov	0x0001 + __muluint_t_1_1,a
;	_muluint.c 51
	mov	dpl,__muluint_t_1_1
	mov	dph,(__muluint_t_1_1 + 1)
00101$:
	C$_muluint.c$52$1$1 ==.
	XG$_muluint$0$0 ==.
	ret
	.area	CSEG    (CODE)
