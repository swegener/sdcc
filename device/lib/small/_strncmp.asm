;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strncmp
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strncmp_PARM_3
	.globl _strncmp_PARM_2
	.globl _strncmp
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
_strncmp_PARM_2:
	.ds	0x0003
_strncmp_PARM_3:
	.ds	0x0002
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
	G$strncmp$0$0 ==.
;	_strncmp.c 27
;	-----------------------------------------
;	 function strncmp
;	-----------------------------------------
_strncmp:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strncmp.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strncmp.c 33
	mov	a,_strncmp_PARM_3
	orl	a,(_strncmp_PARM_3 + 1)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00114$
00115$:
;	_strncmp.c 34
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	ljmp	00108$
;	_strncmp.c 36
00114$:
	mov	r5,_strncmp_PARM_2
	mov	r6,(_strncmp_PARM_2 + 1)
	mov	r7,(_strncmp_PARM_2 + 2)
00105$:
	mov	a,_strncmp_PARM_3
	add	a,#0xff
	mov	r0,a
	mov	a,(_strncmp_PARM_3 + 1)
	addc	a,#0xff
	mov	r1,a
	mov	_strncmp_PARM_3,r0
	mov	(_strncmp_PARM_3 + 1),r1
	mov	a,r0
	orl	a,r1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00116$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00117$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r1,a
	mov	a,r0
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,ar1,00107$
; Peephole 201   removed redundant sjmp
00118$:
00119$:
;	_strncmp.c 37
	inc	r2
	cjne	r2,#0x00,00120$
	inc	r3
00120$:
;	_strncmp.c 38
	inc	r5
	cjne	r5,#0x00,00121$
	inc	r6
00121$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00107$:
;	_strncmp.c 41
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
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
00108$:
	C$_strncmp.c$42$1$1 ==.
	XG$strncmp$0$0 ==.
	ret
	.area	CSEG    (CODE)
