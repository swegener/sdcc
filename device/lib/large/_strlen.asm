;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:33 2000

;--------------------------------------------------------
	.module _strlen
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strlen
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
_strlen_str_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strlen$0$0 ==.
;	_strlen.c 27
;	-----------------------------------------
;	 function strlen
;	-----------------------------------------
_strlen:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strlen.c 34
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strlen_str_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strlen.c 31
	mov	r2,#0x00
	mov	r3,#0x00
	mov	dptr,#_strlen_str_1_1
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
00101$:
	mov	ar7,r4
	mov	ar0,r5
	mov	ar1,r6
	inc	r4
	cjne	r4,#0x00,00108$
	inc	r5
00108$:
	mov	dptr,#_strlen_str_1_1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	mov	dpl,r7
	mov	dph,r0
	mov	b,r1
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00109$:
;	_strlen.c 32
	inc	r2
	cjne	r2,#0x00,00110$
	inc	r3
00110$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	_strlen.c 34
	mov	dpl,r2
	mov	dph,r3
00104$:
	C$_strlen.c$35$1$1 ==.
	XG$strlen$0$0 ==.
	ret
	.area	CSEG    (CODE)
