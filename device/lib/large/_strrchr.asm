;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:34 2000

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
_strrchr_sloc0_1_0:
	.ds	0x0001
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
_strrchr_PARM_2:
	.ds	0x0001
_strrchr_string_1_1:
	.ds	0x0003
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
	push	b
	push	dph
	push	dpl
	mov	dptr,#_strrchr_string_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	_strrchr.c 32
	mov	dptr,#_strrchr_string_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
;	_strrchr.c 34
	mov	ar5,r2
	mov	ar6,r3
	mov	ar7,r4
00101$:
	push	ar2
	push	ar3
	push	ar4
	mov	ar0,r5
	mov	ar1,r6
	mov	ar2,r7
	inc	r5
	cjne	r5,#0x00,00116$
	inc	r6
00116$:
	mov	dptr,#_strrchr_string_1_1
	mov	a,r5
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	lcall	__gptrget
	mov	r0,a
	pop	ar4
	pop	ar3
	pop	ar2
	mov	a,r0
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00101$
00117$:
;	_strrchr.c 37
	mov	dptr,#_strrchr_PARM_2
	movx	a,@dptr
	mov	_strrchr_sloc0_1_0,a
00105$:
	mov	dptr,#_strrchr_string_1_1
	movx	a,@dptr
	add	a,#0xff
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0xff
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	mov	dptr,#_strrchr_string_1_1
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	mov	a,r6
	cjne	a,ar2,00118$
	mov	a,r7
	cjne	a,ar3,00118$
	mov	a,r0
	cjne	a,ar4,00118$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00118$:
	push	ar2
	push	ar3
	push	ar4
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r2,a
	cjne	a,_strrchr_sloc0_1_0,00119$
	mov	a,#0x01
	sjmp	00120$
00119$:
	clr	a
00120$:
	pop	ar4
	pop	ar3
	pop	ar2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00105$
00121$:
00107$:
;	_strrchr.c 40
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,_strrchr_sloc0_1_0,00109$
; Peephole 201   removed redundant sjmp
00122$:
00123$:
;	_strrchr.c 41
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
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
