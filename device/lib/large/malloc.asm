;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:35 2000

;--------------------------------------------------------
	.module malloc
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _free
	.globl _malloc
	.globl _init_dynamic_memory
	.globl _init_dynamic_memory_PARM_2
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
_malloc_sloc0_1_0:
	.ds	0x0002
_malloc_sloc1_1_0:
	.ds	0x0002
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
Fmalloc$FIRST_MEMORY_HEADER_PTR$0$0==.
_FIRST_MEMORY_HEADER_PTR:
	.ds	0x0002
_init_dynamic_memory_PARM_2:
	.ds	0x0002
_init_dynamic_memory_array_1_1:
	.ds	0x0002
_malloc_size_1_1:
	.ds	0x0002
_malloc_current_header_1_1:
	.ds	0x0002
_free_p_1_1:
	.ds	0x0002
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$init_dynamic_memory$0$0 ==.
;	malloc.c 31
;	-----------------------------------------
;	 function init_dynamic_memory
;	-----------------------------------------
_init_dynamic_memory:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	malloc.c 63
	push	dph
	push	dpl
	mov	dptr,#_init_dynamic_memory_array_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	malloc.c 53
	mov	dptr,#_init_dynamic_memory_array_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r3,a
	orl  a,r2
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00106$:
;	malloc.c 55
	mov	dptr,#_init_dynamic_memory_array_1_1
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
;	malloc.c 56
	mov	dptr,#_init_dynamic_memory_PARM_2
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	dec	r2
	cjne	r2,#0xff,00107$
	dec	r3
00107$:
	mov	dptr,#_init_dynamic_memory_PARM_2
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
00102$:
;	malloc.c 58
	mov	dptr,#_init_dynamic_memory_array_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_FIRST_MEMORY_HEADER_PTR
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	malloc.c 60
	mov	dptr,#_init_dynamic_memory_PARM_2
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r4
	add	a,r2
	mov	r4,a
	mov	a,r5
	addc	a,r3
	mov	r5,a
	mov	a,r4
	add	a,#0xfa
	mov	r4,a
	mov	a,r5
	addc	a,#0xff
	mov	r5,a
	mov	dpl,r2
	mov	dph,r3
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 61
	mov	dpl,r4
	mov	dph,r5
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
;	malloc.c 62
	mov	a,#0x02
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
;	malloc.c 63
	mov	a,#0x04
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
00103$:
	C$malloc.c$64$1$1 ==.
	XG$init_dynamic_memory$0$0 ==.
	ret
	G$malloc$0$0 ==.
;	malloc.c 66
;	-----------------------------------------
;	 function malloc
;	-----------------------------------------
_malloc:
;	malloc.c 0
	push	dph
	push	dpl
	mov	dptr,#_malloc_size_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	malloc.c 71
	mov	dptr,#_malloc_size_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	ar4,r2
	mov	ar5,r3
	mov	r6,#0x00
	mov	r7,#0x00
	clr	c
	mov	a,#0xf9
	subb	a,r4
	mov	a,#0xff
	subb	a,r5
; Peephole 180   changed mov to clr
	clr  a
	subb	a,r6
; Peephole 180   changed mov to clr
	clr  a
	subb	a,r7
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00102$
00122$:
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	ljmp	00114$
00102$:
;	malloc.c 72
	mov	a,#0x06
	add	a,r2
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r3,a
	mov	dptr,#_malloc_size_1_1
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	malloc.c 73
	mov	dptr,#_FIRST_MEMORY_HEADER_PTR
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_malloc_current_header_1_1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 74
00108$:
;	malloc.c 84
	mov	dptr,#_malloc_current_header_1_1
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dpl,r4
	mov	dph,r5
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	ar0,r6
	mov	ar1,r7
;	malloc.c 85
	push	ar6
	push	ar7
	mov	ar6,r4
	mov	ar7,r5
	clr	c
	mov	a,r0
	subb	a,r6
	mov	r0,a
	mov	a,r1
	subb	a,r7
	mov	r1,a
;	malloc.c 86
	mov	a,#0x04
	add	a,r4
	mov	_malloc_sloc0_1_0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	(_malloc_sloc0_1_0 + 1),a
	mov	dpl,_malloc_sloc0_1_0
	mov	dph,(_malloc_sloc0_1_0 + 1)
	movx	a,@dptr
	mov	_malloc_sloc1_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_malloc_sloc1_1_0 + 1),a
	clr	c
	mov	a,r0
	subb	a,_malloc_sloc1_1_0
	mov	r0,a
	mov	a,r1
	subb	a,(_malloc_sloc1_1_0 + 1)
	mov	r1,a
	clr	c
	mov	a,r0
	subb	a,r2
	mov	a,r1
	subb	a,r3
	clr	a
	rlc	a
	pop	ar7
	pop	ar6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00123$:
;	malloc.c 87
	mov	dptr,#_malloc_current_header_1_1
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
;	malloc.c 88
	mov	dpl,r6
	mov	dph,r7
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r1,a
	orl  a,r0
	jz	00124$
	ljmp	00108$
00124$:
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	ljmp	00114$
00109$:
;	malloc.c 90
	mov	a,_malloc_sloc1_1_0
	orl	a,(_malloc_sloc1_1_0 + 1)
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00111$
00125$:
;	malloc.c 92
	mov	dpl,_malloc_sloc0_1_0
	mov	dph,(_malloc_sloc0_1_0 + 1)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	malloc.c 93
	mov	a,#0x06
	add	a,r4
	mov	r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	r1,a
	mov	dpl,r0
	mov	dph,r1
	ljmp	00114$
00111$:
;	malloc.c 95
	mov	a,_malloc_sloc1_1_0
	add	a,r4
	mov	r0,a
	mov	a,(_malloc_sloc1_1_0 + 1)
	addc	a,r5
	mov	r1,a
;	malloc.c 96
	mov	dpl,r0
	mov	dph,r1
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
;	malloc.c 97
	mov	a,#0x02
	add	a,r0
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r1
	mov	dph,a
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 98
	mov	dpl,r4
	mov	dph,r5
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
;	malloc.c 99
	mov	a,r6
	orl	a,r7
; Peephole 110   removed ljmp by inverse jump logic
	jz  00113$
00126$:
	mov	dpl,r0
	mov	dph,r1
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,#0x02
	add	a,r4
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	dph,a
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
00113$:
;	malloc.c 100
	mov	a,#0x04
	add	a,r0
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r1
	mov	dph,a
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	malloc.c 101
	mov	a,#0x06
	add	a,r0
	mov	r0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r1
	mov	r1,a
	mov	dpl,r0
	mov	dph,r1
00114$:
	C$malloc.c$102$1$1 ==.
	XG$malloc$0$0 ==.
	ret
	G$free$0$0 ==.
;	malloc.c 104
;	-----------------------------------------
;	 function free
;	-----------------------------------------
_free:
;	malloc.c 0
	push	dph
	push	dpl
	mov	dptr,#_free_p_1_1
	pop	acc
	movx	@dptr,a
	pop	acc
	inc	dptr
	movx	@dptr,a
;	malloc.c 107
	mov	dptr,#_free_p_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r3,a
	orl  a,r2
	jnz	00113$
	ljmp	00108$
00113$:
;	malloc.c 109
	mov	a,r2
	add	a,#0xfa
	mov	r4,a
	mov	a,r3
	addc	a,#0xff
	mov	r5,a
	mov	dptr,#_free_p_1_1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 110
	mov	a,#0x02
	add	a,r4
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	dph,a
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r7,a
	orl  a,r6
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00114$:
;	malloc.c 113
	mov	dpl,r4
	mov	dph,r5
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dpl,r6
	mov	dph,r7
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 114
	mov	a,r4
	orl	a,r5
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00115$:
	mov	a,#0x02
	add	a,r4
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	dph,a
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00104$:
;	malloc.c 116
	mov	a,#0xfe
	add	a,r2
	mov	dpl,a
	mov	a,#0xff
	addc	a,r3
	mov	dph,a
; Peephole 101   removed redundant mov
; Peephole 180   changed mov to clr
	clr  a
	movx @dptr,a
	inc  dptr
	movx @dptr,a
00108$:
	C$malloc.c$118$2$1 ==.
	XG$free$0$0 ==.
	ret
	.area	CSEG    (CODE)
