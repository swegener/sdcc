;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

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
_strpbrk_sloc0_1_0:
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
_strpbrk_PARM_2:
	.ds	0x0003
_strpbrk_string_1_1:
	.ds	0x0003
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
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strpbrk_string_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strpbrk.c 34
	mov	dptr,#_strpbrk_PARM_2
	movx	a,@dptr
	mov	_strpbrk_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strpbrk_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strpbrk_sloc0_1_0 + 2),a
	mov	dptr,#_strpbrk_string_1_1
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
00103$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00105$
00111$:
;	_strpbrk.c 35
	mov	dptr,#_strchr_PARM_2
	mov	a,r0
	movx	@dptr,a
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,_strpbrk_sloc0_1_0
	mov	dph,(_strpbrk_sloc0_1_0 + 1)
	mov	b,(_strpbrk_sloc0_1_0 + 2)
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
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
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
