;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:17 2000

;--------------------------------------------------------
	.module _strstr
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strstr_PARM_2
	.globl _strstr
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
_strstr_PARM_2:
	.ds	0x0003
_strstr_cp_1_1:
	.ds	0x0003
_strstr_s1_1_1:
	.ds	0x0003
_strstr_s2_1_1:
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
	G$strstr$0$0 ==.
;	_strstr.c 27
;	-----------------------------------------
;	 function strstr
;	-----------------------------------------
_strstr:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strstr.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strstr.c 32
	mov	_strstr_cp_1_1,r2
	mov	(_strstr_cp_1_1 + 1),r3
	mov	(_strstr_cp_1_1 + 2),r4
;	_strstr.c 36
	mov	_strstr_s2_1_1,_strstr_PARM_2
	mov	(_strstr_s2_1_1 + 1),(_strstr_PARM_2 + 1)
	mov	(_strstr_s2_1_1 + 2),(_strstr_PARM_2 + 2)
	mov	dpl,_strstr_s2_1_1
	mov	dph,(_strstr_s2_1_1 + 1)
	mov	b,(_strstr_s2_1_1 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00122$
00123$:
;	_strstr.c 37
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	ljmp	00113$
;	_strstr.c 39
00122$:
00110$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
	jnz	00124$
	ljmp	00112$
00124$:
;	_strstr.c 42
;	_strstr.c 44
	mov	_strstr_s1_1_1,r2
	mov	(_strstr_s1_1_1 + 1),r3
	mov	(_strstr_s1_1_1 + 2),r4
	mov	r0,_strstr_s2_1_1
	mov	r1,(_strstr_s2_1_1 + 1)
	mov	r5,(_strstr_s2_1_1 + 2)
00105$:
	mov	dpl,_strstr_s1_1_1
	mov	dph,(_strstr_s1_1_1 + 1)
	mov	b,(_strstr_s1_1_1 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00125$:
	mov	dpl,r0
	mov	dph,r1
	mov	b,r5
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00126$:
	clr	c
	mov	a,r6
	subb	a,r7
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00107$
00127$:
;	_strstr.c 45
	inc	_strstr_s1_1_1
	clr	a
	cjne	a,_strstr_s1_1_1,00128$
	inc	(_strstr_s1_1_1 + 1)
00128$:
	inc	r0
	cjne	r0,#0x00,00129$
	inc	r1
00129$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00107$:
;	_strstr.c 47
	mov	dpl,r0
	mov	dph,r1
	mov	b,r5
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00109$
00130$:
;	_strstr.c 48
	mov	dpl,_strstr_cp_1_1
	mov	dph,(_strstr_cp_1_1 + 1)
	mov	b,(_strstr_cp_1_1 + 2)
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00109$:
;	_strstr.c 50
	inc	r2
	cjne	r2,#0x00,00131$
	inc	r3
00131$:
	mov	_strstr_cp_1_1,r2
	mov	(_strstr_cp_1_1 + 1),r3
	mov	(_strstr_cp_1_1 + 2),r4
	ljmp	00110$
00112$:
;	_strstr.c 53
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00113$:
	C$_strstr.c$54$1$1 ==.
	XG$strstr$0$0 ==.
	ret
	.area	CSEG    (CODE)
