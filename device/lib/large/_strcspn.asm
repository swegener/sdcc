;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:33 2000

;--------------------------------------------------------
	.module _strcspn
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcspn_PARM_2
	.globl _strcspn
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
_strcspn_sloc0_1_0:
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
_strcspn_PARM_2:
	.ds	0x0003
_strcspn_string_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strcspn$0$0 ==.
;	_strcspn.c 27
;	-----------------------------------------
;	 function strcspn
;	-----------------------------------------
_strcspn:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcspn.c 42
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strcspn_string_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strcspn.c 35
	mov	dptr,#_strcspn_PARM_2
	movx	a,@dptr
	mov	_strcspn_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strcspn_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strcspn_sloc0_1_0 + 2),a
	mov	dptr,#_strcspn_string_1_1
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	r0,#0x00
	mov	r1,#0x00
00104$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r2,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00112$:
;	_strcspn.c 36
	mov	dptr,#_strchr_PARM_2
	mov	a,r2
	movx	@dptr,a
	push	ar5
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	dpl,_strcspn_sloc0_1_0
	mov	dph,(_strcspn_sloc0_1_0 + 1)
	mov	b,(_strcspn_sloc0_1_0 + 2)
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
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00106$
00113$:
;	_strcspn.c 39
	inc	r0
	cjne	r0,#0x00,00114$
	inc	r1
00114$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00106$:
;	_strcspn.c 42
	mov	dpl,r0
	mov	dph,r1
00107$:
	C$_strcspn.c$43$1$1 ==.
	XG$strcspn$0$0 ==.
	ret
	.area	CSEG    (CODE)
