;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:18 2000

;--------------------------------------------------------
	.module malloc
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _init_dynamic_memory_PARM_2
	.globl _free
	.globl _malloc
	.globl _init_dynamic_memory
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
Fmalloc$FIRST_MEMORY_HEADER_PTR$0$0==.
_FIRST_MEMORY_HEADER_PTR:
	.ds	0x0002
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
_init_dynamic_memory_PARM_2:
	.ds	0x0002
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
_malloc_size_1_1:
	.ds	0x0002
_malloc_current_header_1_1:
	.ds	0x0002
	.area _DUMMY
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
	mov	r2,dpl
	mov	r3,dph
;	malloc.c 53
	mov	a,r2
	orl	a,r3
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00106$:
;	malloc.c 55
	inc	r2
	cjne	r2,#0x00,00107$
	inc	r3
00107$:
;	malloc.c 56
	dec	_init_dynamic_memory_PARM_2
	mov	a,#0xff
	cjne	a,_init_dynamic_memory_PARM_2,00108$
	dec	(_init_dynamic_memory_PARM_2 + 1)
00108$:
00102$:
;	malloc.c 58
	mov	_FIRST_MEMORY_HEADER_PTR,r2
	mov	(_FIRST_MEMORY_HEADER_PTR + 1),r3
;	malloc.c 60
	mov	r4,_init_dynamic_memory_PARM_2
	mov	r5,(_init_dynamic_memory_PARM_2 + 1)
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
	mov	_malloc_size_1_1,dpl
	mov	(_malloc_size_1_1 + 1),dph
;	malloc.c 71
	mov	r4,_malloc_size_1_1
	mov	r5,(_malloc_size_1_1 + 1)
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
	add	a,_malloc_size_1_1
	mov	_malloc_size_1_1,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,(_malloc_size_1_1 + 1)
	mov	(_malloc_size_1_1 + 1),a
;	malloc.c 73
	mov	r4,_FIRST_MEMORY_HEADER_PTR
	mov	r5,(_FIRST_MEMORY_HEADER_PTR + 1)
;	malloc.c 74
00108$:
;	malloc.c 84
	mov	dpl,r4
	mov	dph,r5
	movx	a,@dptr
	mov	_malloc_current_header_1_1,a
	inc	dptr
	movx	a,@dptr
	mov	(_malloc_current_header_1_1 + 1),a
	mov	r0,_malloc_current_header_1_1
	mov	r1,(_malloc_current_header_1_1 + 1)
;	malloc.c 85
	mov	ar2,r4
	mov	ar3,r5
	clr	c
	mov	a,r0
	subb	a,r2
	mov	r0,a
	mov	a,r1
	subb	a,r3
	mov	r1,a
;	malloc.c 86
	mov	a,#0x04
	add	a,r4
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	clr	c
	mov	a,r0
	subb	a,r6
	mov	r0,a
	mov	a,r1
	subb	a,r7
	mov	r1,a
	clr	c
	mov	a,r0
	subb	a,_malloc_size_1_1
	mov	a,r1
	subb	a,(_malloc_size_1_1 + 1)
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00109$
00123$:
;	malloc.c 87
	mov	r4,_malloc_current_header_1_1
	mov	r5,(_malloc_current_header_1_1 + 1)
;	malloc.c 88
	mov	dpl,r4
	mov	dph,r5
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
	mov	a,r6
	orl	a,r7
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00111$
00125$:
;	malloc.c 92
	mov	dpl,r2
	mov	dph,r3
	mov	a,_malloc_size_1_1
	movx	@dptr,a
	inc	dptr
	mov	a,(_malloc_size_1_1 + 1)
	movx	@dptr,a
;	malloc.c 93
	mov	a,#0x06
	add	a,r4
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r5
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
	ljmp	00114$
00111$:
;	malloc.c 95
	mov	a,r6
	add	a,r4
	mov	r2,a
	mov	a,r7
	addc	a,r5
	mov	r3,a
;	malloc.c 96
	mov	dpl,r2
	mov	dph,r3
	mov	a,_malloc_current_header_1_1
	movx	@dptr,a
	inc	dptr
	mov	a,(_malloc_current_header_1_1 + 1)
	movx	@dptr,a
;	malloc.c 97
	mov	a,#0x02
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;	malloc.c 98
	mov	dpl,r4
	mov	dph,r5
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	malloc.c 99
	mov	a,_malloc_current_header_1_1
	orl	a,(_malloc_current_header_1_1 + 1)
; Peephole 110   removed ljmp by inverse jump logic
	jz  00113$
00126$:
	mov	dpl,r2
	mov	dph,r3
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
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
00113$:
;	malloc.c 100
	mov	a,#0x04
	add	a,r2
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	dph,a
	mov	a,_malloc_size_1_1
	movx	@dptr,a
	inc	dptr
	mov	a,(_malloc_size_1_1 + 1)
	movx	@dptr,a
;	malloc.c 101
	mov	a,#0x06
	add	a,r2
	mov	r2,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
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
	mov	r2,dpl
	mov	r3,dph
;	malloc.c 107
	mov	a,r2
	orl	a,r3
	jnz	00113$
	ljmp	00108$
00113$:
;	malloc.c 109
	mov	a,r2
	add	a,#0xfa
	mov	r2,a
	mov	a,r3
	addc	a,#0xff
	mov	r3,a
;	malloc.c 110
	mov	a,#0x02
	add	a,r2
	mov	r4,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r5,a
	mov	dpl,r4
	mov	dph,r5
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
; Peephole 135   removed redundant mov
	mov  r5,a
	orl  a,r4
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00114$:
;	malloc.c 112
;	malloc.c 113
	mov	dpl,r2
	mov	dph,r3
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dpl,r4
	mov	dph,r5
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
;	malloc.c 114
	mov	a,r6
	orl	a,r7
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00115$:
	mov	a,#0x02
	add	a,r6
	mov	dpl,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r7
	mov	dph,a
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00104$:
;	malloc.c 116
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
00108$:
	C$malloc.c$118$2$1 ==.
	XG$free$0$0 ==.
	ret
	.area	CSEG    (CODE)
