;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strrchr
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strrchr_PARM_2
	.globl _strrchr
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
_strrchr_PARM_2:
	.ds	0x0001
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
	G$strrchr$0$0 ==.
;	_strrchr.c 27
;	-----------------------------------------
;	 function strrchr
;	-----------------------------------------
_strrchr:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strrchr.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strrchr.c 32
	mov	ar5,r2
	mov	ar6,r3
	mov	ar7,r4
;	_strrchr.c 34
00101$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	inc	r2
	cjne	r2,#0x00,00116$
	inc	r3
00116$:
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00117$:
;	_strrchr.c 37
00105$:
	dec	r2
	cjne	r2,#0xff,00118$
	dec	r3
00118$:
	mov	a,r2
	cjne	a,ar5,00119$
	mov	a,r3
	cjne	a,ar6,00119$
	mov	a,r4
	cjne	a,ar7,00119$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00119$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,_strrchr_PARM_2,00105$
; Peephole 201   removed redundant sjmp
00120$:
00121$:
00107$:
;	_strrchr.c 40
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,_strrchr_PARM_2,00109$
; Peephole 201   removed redundant sjmp
00122$:
00123$:
;	_strrchr.c 41
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00109$:
;	_strrchr.c 43
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00110$:
	C$_strrchr.c$44$1$1 ==.
	XG$strrchr$0$0 ==.
	ret
	.area	CSEG    (CODE)
