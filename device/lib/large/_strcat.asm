;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:40 2000

;--------------------------------------------------------
	.module _strcat
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcat_PARM_2
	.globl _strcat
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
_strcat_sloc0_1_0:
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
_strcat_PARM_2:
	.ds	0x0003
_strcat_dst_1_1:
	.ds	0x0003
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strcat$0$0 ==.
;	_strcat.c 26
;	-----------------------------------------
;	 function strcat
;	-----------------------------------------
_strcat:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcat.c 38
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strcat_dst_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strcat.c 31
	mov	dptr,#_strcat_dst_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	_strcat.c 33
00101$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00111$
00113$:
;	_strcat.c 34
	inc	r2
	cjne	r2,#0x00,00114$
	inc	r3
00114$:
;	_strcat.c 36
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00111$:
	mov	dptr,#_strcat_PARM_2
	movx	a,@dptr
	mov	_strcat_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strcat_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strcat_sloc0_1_0 + 2),a
00104$:
	mov	ar0,r2
	mov	ar1,r3
	mov	ar5,r4
	inc	r2
	cjne	r2,#0x00,00115$
	inc	r3
00115$:
	mov	dpl,_strcat_sloc0_1_0
	mov	dph,(_strcat_sloc0_1_0 + 1)
	mov	b,(_strcat_sloc0_1_0 + 2)
	inc	_strcat_sloc0_1_0
	clr	a
	cjne	a,_strcat_sloc0_1_0,00116$
	inc	(_strcat_sloc0_1_0 + 1)
00116$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r6,a
	mov  dpl,r0
	mov  dph,r1
	mov  b,r5
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00117$:
;	_strcat.c 38
	mov	dptr,#_strcat_dst_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
00107$:
	C$_strcat.c$40$1$1 ==.
	XG$strcat$0$0 ==.
	ret
	.area	CSEG    (CODE)
