;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strncat
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strncat_PARM_3
	.globl _strncat_PARM_2
	.globl _strncat
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
_strncat_PARM_2:
	.ds	0x0003
_strncat_PARM_3:
	.ds	0x0002
_strncat_front_1_1:
	.ds	0x0003
_strncat_start_1_1:
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
	G$strncat$0$0 ==.
;	_strncat.c 27
;	-----------------------------------------
;	 function strncat
;	-----------------------------------------
_strncat:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strncat.c 44
	mov	_strncat_front_1_1,dpl
	mov	(_strncat_front_1_1 + 1),dph
	mov	(_strncat_front_1_1 + 2),b
;	_strncat.c 33
	mov	_strncat_start_1_1,_strncat_front_1_1
	mov	(_strncat_start_1_1 + 1),(_strncat_front_1_1 + 1)
	mov	(_strncat_start_1_1 + 2),(_strncat_front_1_1 + 2)
;	_strncat.c 35
	mov	r0,_strncat_front_1_1
	mov	r1,(_strncat_front_1_1 + 1)
	mov	r5,(_strncat_front_1_1 + 2)
00101$:
	mov	dpl,r0
	mov	dph,r1
	mov	b,r5
	inc	r0
	cjne	r0,#0x00,00116$
	inc	r1
00116$:
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00117$:
;	_strncat.c 37
	mov	a,r0
	add	a,#0xff
	mov	_strncat_front_1_1,a
	mov	a,r1
	addc	a,#0xff
	mov	(_strncat_front_1_1 + 1),a
	mov	(_strncat_front_1_1 + 2),r5
;	_strncat.c 39
	mov	r5,_strncat_front_1_1
	mov	r6,(_strncat_front_1_1 + 1)
	mov	r7,(_strncat_front_1_1 + 2)
00106$:
	mov	r2,_strncat_PARM_3
	mov	r0,(_strncat_PARM_3 + 1)
	dec	_strncat_PARM_3
	mov	a,#0xff
	cjne	a,_strncat_PARM_3,00118$
	dec	(_strncat_PARM_3 + 1)
00118$:
	mov	a,r2
	orl	a,r0
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00119$:
;	_strncat.c 40
	mov	ar2,r5
	mov	ar0,r6
	mov	ar1,r7
	inc	r5
	cjne	r5,#0x00,00120$
	inc	r6
00120$:
	mov	_strncat_front_1_1,r5
	mov	(_strncat_front_1_1 + 1),r6
	mov	(_strncat_front_1_1 + 2),r7
	mov	dpl,_strncat_PARM_2
	mov	dph,(_strncat_PARM_2 + 1)
	mov	b,(_strncat_PARM_2 + 2)
	inc	_strncat_PARM_2
	clr	a
	cjne	a,_strncat_PARM_2,00121$
	inc	(_strncat_PARM_2 + 1)
00121$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r3,a
	mov  dpl,r2
	mov  dph,r0
	mov  b,r1
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00106$
00122$:
;	_strncat.c 41
	mov	dpl,_strncat_start_1_1
	mov	dph,(_strncat_start_1_1 + 1)
	mov	b,(_strncat_start_1_1 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	_strncat.c 43
	mov	dpl,_strncat_front_1_1
	mov	dph,(_strncat_front_1_1 + 1)
	mov	b,(_strncat_front_1_1 + 2)
; Peephole 180   changed mov to clr
	clr  a
	lcall	__gptrput
;	_strncat.c 44
	mov	dpl,_strncat_start_1_1
	mov	dph,(_strncat_start_1_1 + 1)
	mov	b,(_strncat_start_1_1 + 2)
00109$:
	C$_strncat.c$45$1$1 ==.
	XG$strncat$0$0 ==.
	ret
	.area	CSEG    (CODE)
