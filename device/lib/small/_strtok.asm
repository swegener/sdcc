;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:17 2000

;--------------------------------------------------------
	.module _strtok
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strtok_PARM_2
	.globl _strtok
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
_strtok_s_1_1:
	.ds	0x0003
_strtok_PARM_2:
	.ds	0x0003
_strtok_s1_1_1:
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
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strtok$0$0 ==.
;	_strtok.c 32
;	-----------------------------------------
;	 function strtok
;	-----------------------------------------
_strtok:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strtok.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strtok.c 40
	mov	a,r2
	orl	a,r3
	orl	a,r4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00114$:
;	_strtok.c 41
	mov	_strtok_s_1_1,r2
	mov	(_strtok_s_1_1 + 1),r3
	mov	(_strtok_s_1_1 + 2),r4
00102$:
;	_strtok.c 43
	mov	_strtok_s1_1_1,_strtok_s_1_1
	mov	(_strtok_s1_1_1 + 1),(_strtok_s_1_1 + 1)
	mov	(_strtok_s1_1_1 + 2),(_strtok_s_1_1 + 2)
;	_strtok.c 45
	mov	r5,_strtok_PARM_2
	mov	r6,(_strtok_PARM_2 + 1)
	mov	r7,(_strtok_PARM_2 + 2)
00105$:
	mov	r0,_strtok_s_1_1
	mov	r1,(_strtok_s_1_1 + 1)
	mov	r2,(_strtok_s_1_1 + 2)
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
	jnz	00115$
	ljmp	00107$
00115$:
;	_strtok.c 46
	mov	r3,_strtok_s_1_1
	mov	r4,(_strtok_s_1_1 + 1)
	mov	r0,(_strtok_s_1_1 + 2)
	mov	dpl,r3
	mov	dph,r4
	mov	b,r0
	lcall	__gptrget
	mov	_strchr_PARM_2,a
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	_strchr
	mov	r3,dpl
	mov	r4,dph
	mov	r0,b
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r3
	orl	a,r4
	orl	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00116$:
;	_strtok.c 47
	mov	r3,_strtok_s_1_1
	mov	r4,(_strtok_s_1_1 + 1)
	mov	r5,(_strtok_s_1_1 + 2)
	inc	_strtok_s_1_1
	clr	a
	cjne	a,_strtok_s_1_1,00117$
	inc	(_strtok_s_1_1 + 1)
00117$:
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
; Peephole 180   changed mov to clr
	clr  a
	lcall	__gptrput
;	_strtok.c 48
	mov	dpl,_strtok_s1_1_1
	mov	dph,(_strtok_s1_1_1 + 1)
	mov	b,(_strtok_s1_1_1 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00104$:
;	_strtok.c 50
	inc	_strtok_s_1_1
	clr	a
	cjne	a,_strtok_s_1_1,00118$
	inc	(_strtok_s_1_1 + 1)
00118$:
	ljmp	00105$
00107$:
;	_strtok.c 52
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00108$:
	C$_strtok.c$53$1$1 ==.
	XG$strtok$0$0 ==.
	ret
	.area	CSEG    (CODE)
