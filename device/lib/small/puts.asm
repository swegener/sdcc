;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:21 2000

;--------------------------------------------------------
	.module puts
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _puts
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
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$puts$0$0 ==.
;	puts.c 27
;	-----------------------------------------
;	 function puts
;	-----------------------------------------
_puts:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	puts.c 35
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	puts.c 30
	mov	r5,#0x00
	mov	r6,#0x00
00101$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00108$:
;	puts.c 31
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	inc	r2
	cjne	r2,#0x00,00109$
	inc	r3
00109$:
	lcall	__gptrget
	mov	r7,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	mov	dpl,r7
	lcall	_putchar
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	puts.c 32
	inc	r5
	cjne	r5,#0x00,00110$
	inc	r6
00110$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	puts.c 34
	push	ar5
	push	ar6
	mov	dpl,#0x0a
	lcall	_putchar
	pop	ar6
	pop	ar5
;	puts.c 35
	mov	a,#0x01
	add	a,r5
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r6
	mov	dph,a
00104$:
	C$puts.c$36$1$1 ==.
	XG$puts$0$0 ==.
	ret
	.area	CSEG    (CODE)
