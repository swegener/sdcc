;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _memcmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _memcmp_PARM_3
	.globl _memcmp_PARM_2
	.globl _memcmp
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
_memcmp_PARM_2:
	.ds	0x0003
_memcmp_PARM_3:
	.ds	0x0002
_memcmp_buf1_1_1:
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
	G$memcmp$0$0 ==.
;	_memcmp.c 27
;	-----------------------------------------
;	 function memcmp
;	-----------------------------------------
_memcmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_memcmp.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_memcmp.c 33
	mov	a,_memcmp_PARM_3
	orl	a,(_memcmp_PARM_3 + 1)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00113$:
;	_memcmp.c 34
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	ljmp	00107$
;	_memcmp.c 36
00104$:
	mov	a,_memcmp_PARM_3
	add	a,#0xff
	mov	r5,a
	mov	a,(_memcmp_PARM_3 + 1)
	addc	a,#0xff
	mov	r6,a
	mov	_memcmp_PARM_3,r5
	mov	(_memcmp_PARM_3 + 1),r6
	mov	a,r5
	orl	a,r6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00114$:
	mov	_memcmp_buf1_1_1,r2
	mov	(_memcmp_buf1_1_1 + 1),r3
	mov	(_memcmp_buf1_1_1 + 2),r4
	mov	r0,_memcmp_PARM_2
	mov	r1,(_memcmp_PARM_2 + 1)
	mov	r5,(_memcmp_PARM_2 + 2)
	mov	dpl,r0
	mov	dph,r1
	mov	b,r5
	lcall	__gptrget
	mov	r6,a
	mov	dpl,_memcmp_buf1_1_1
	mov	dph,(_memcmp_buf1_1_1 + 1)
	mov	b,(_memcmp_buf1_1_1 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar6,00106$
; Peephole 201   removed redundant sjmp
00115$:
00116$:
;	_memcmp.c 37
	inc	r2
	cjne	r2,#0x00,00117$
	inc	r3
00117$:
;	_memcmp.c 38
	mov	a,#0x01
	add	a,r0
	mov	_memcmp_PARM_2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r1
	mov	(_memcmp_PARM_2 + 1),a
	mov	(_memcmp_PARM_2 + 2),r5
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00106$:
;	_memcmp.c 41
	mov	dpl,_memcmp_PARM_2
	mov	dph,(_memcmp_PARM_2 + 1)
	mov	b,(_memcmp_PARM_2 + 2)
	lcall	__gptrget
	mov	r5,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 106   removed redundant mov 
	mov  r2,a
	clr  c
	subb	a,r5
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	subb	a,#0x00
	mov	dph,a
00107$:
	C$_memcmp.c$42$1$1 ==.
	XG$memcmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
