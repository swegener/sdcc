;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

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
_strstr_sloc0_1_0:
	.ds	0x0003
_strstr_sloc1_1_0:
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
_strstr_PARM_2:
	.ds	0x0003
_strstr_str1_1_1:
	.ds	0x0003
_strstr_cp_1_1:
	.ds	0x0003
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
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strstr_str1_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strstr.c 32
	mov	dptr,#_strstr_str1_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#_strstr_cp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
;	_strstr.c 36
	mov	dptr,#_strstr_PARM_2
	movx	a,@dptr
	mov	_strstr_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_strstr_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_strstr_sloc0_1_0 + 2),a
	mov	dpl,_strstr_sloc0_1_0
	mov	dph,(_strstr_sloc0_1_0 + 1)
	mov	b,(_strstr_sloc0_1_0 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
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
	mov  r0,a
	jnz	00124$
	ljmp	00112$
00124$:
;	_strstr.c 44
	mov	_strstr_sloc1_1_0,r2
	mov	(_strstr_sloc1_1_0 + 1),r3
	mov	(_strstr_sloc1_1_0 + 2),r4
	mov	r6,_strstr_sloc0_1_0
	mov	r7,(_strstr_sloc0_1_0 + 1)
	mov	r5,(_strstr_sloc0_1_0 + 2)
00105$:
	mov	dpl,_strstr_sloc1_1_0
	mov	dph,(_strstr_sloc1_1_0 + 1)
	mov	b,(_strstr_sloc1_1_0 + 2)
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00125$:
	mov	dpl,r6
	mov	dph,r7
	mov	b,r5
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r1,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00126$:
	clr	c
	mov	a,r0
	subb	a,r1
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00107$
00127$:
;	_strstr.c 45
	inc	_strstr_sloc1_1_0
	clr	a
	cjne	a,_strstr_sloc1_1_0,00128$
	inc	(_strstr_sloc1_1_0 + 1)
00128$:
	inc	r6
	cjne	r6,#0x00,00129$
	inc	r7
00129$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00107$:
;	_strstr.c 47
	mov	dpl,r6
	mov	dph,r7
	mov	b,r5
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r6,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00109$
00130$:
;	_strstr.c 48
	mov	dptr,#_strstr_cp_1_1
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	mov	dpl,r6
	mov	dph,r0
	mov	b,r1
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00109$:
;	_strstr.c 50
	inc	r2
	cjne	r2,#0x00,00131$
	inc	r3
00131$:
	mov	dptr,#_strstr_cp_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
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
