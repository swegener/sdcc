;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:29 2000

;--------------------------------------------------------
	.module _ispunct
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _ispunct
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
_ispunct_c_1_1:
	.ds	0x0001
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$ispunct$0$0 ==.
;	_ispunct.c 25
;	-----------------------------------------
;	 function ispunct
;	-----------------------------------------
_ispunct:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_ispunct.c 34
	push	dpl
	mov	dptr,#_ispunct_c_1_1
	pop	acc
	movx	@dptr,a
;	_ispunct.c 28
	mov	dptr,#_ispunct_c_1_1
	movx	a,@dptr
	mov	r2,a
	push	ar2
	mov	dpl,r2
	lcall	_isprint
	mov	r3,dpl
	pop	ar2
	mov	a,r3
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_ispunct.c 29
	push	ar2
	mov	dpl,r2
	lcall	_islower
	mov	r3,dpl
	pop	ar2
	mov	a,r3
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00115$:
;	_ispunct.c 30
	push	ar2
	mov	dpl,r2
	lcall	_isupper
	mov	r3,dpl
	pop	ar2
	mov	a,r3
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00116$:
;	_ispunct.c 31
	cjne	r2,#0x20,00117$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00117$:
;	_ispunct.c 32
	mov	dpl,r2
	lcall	_isdigit
	mov	r2,dpl
	mov	a,r2
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00118$:
;	_ispunct.c 33
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00102$:
;	_ispunct.c 34
	mov	dpl,#0x00
00107$:
	C$_ispunct.c$35$1$1 ==.
	XG$ispunct$0$0 ==.
	ret
	.area	CSEG    (CODE)
