;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

;--------------------------------------------------------
	.module _strspn
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strspn_PARM_2
	.globl _strspn
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
_strspn_sloc0_1_0:
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
_strspn_PARM_2:
	.ds	0x0003
_strspn_string_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strspn$0$0 ==.
;	_strspn.c 27
;	-----------------------------------------
;	 function strspn
;	-----------------------------------------
_strspn:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strspn.c 43
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strspn_string_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strspn.c 35
	mov	dptr,#_strspn_PARM_2
	movx	a,@dptr
	mov	_strspn_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strspn_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strspn_sloc0_1_0 + 2),a
	mov	r5,#0x00
	mov	r6,#0x00
	mov	dptr,#_strspn_string_1_1
	movx	a,@dptr
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
00104$:
	mov	dpl,r7
	mov	dph,r0
	mov	b,r1
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00112$:
;	_strspn.c 36
	mov	dptr,#_strchr_PARM_2
	mov	a,r2
	movx	@dptr,a
	push	ar5
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	dpl,_strspn_sloc0_1_0
	mov	dph,(_strspn_sloc0_1_0 + 1)
	mov	b,(_strspn_sloc0_1_0 + 2)
	lcall	_strchr
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r2
	orl	a,r3
	orl	a,r4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00113$:
;	_strspn.c 37
	inc	r5
	cjne	r5,#0x00,00114$
	inc	r6
00114$:
;	_strspn.c 40
	inc	r7
	cjne	r7,#0x00,00115$
	inc	r0
00115$:
	mov	dptr,#_strspn_string_1_1
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	ljmp	00104$
00106$:
;	_strspn.c 43
	mov	dptr,#_strspn_string_1_1
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
00107$:
	C$_strspn.c$44$1$1 ==.
	XG$strspn$0$0 ==.
	ret
	.area	CSEG    (CODE)
