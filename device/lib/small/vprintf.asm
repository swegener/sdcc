;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:21 2000

;--------------------------------------------------------
	.module vprintf
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _lsd
	.globl _lower_case
	.globl _output_to_string
	.globl _vprintf_PARM_2
	.globl _vsprintf_PARM_3
	.globl _vsprintf_PARM_2
	.globl _radix
	.globl _value
	.globl _output_ptr
	.globl _vsprintf
	.globl _vprintf
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
G$output_ptr$0$0==.
_output_ptr:
	.ds	0x0003
G$value$0$0==.
_value:
	.ds	0x0005
G$radix$0$0==.
_radix:
	.ds	0x0001
_vsprintf_PARM_2:
	.ds	0x0003
_vsprintf_PARM_3:
	.ds	0x0001
_vsprintf_width_1_1:
	.ds	0x0001
_vsprintf_length_1_1:
	.ds	0x0001
_vsprintf_c_1_1:
	.ds	0x0001
_vsprintf_sloc0_1_0:
	.ds	0x0001
_vsprintf_sloc1_1_0:
	.ds	0x0001
_vsprintf_sloc2_1_0:
	.ds	0x0001
_vsprintf_sloc3_1_0:
	.ds	0x0001
_vsprintf_sloc4_1_0:
	.ds	0x0001
_vsprintf_sloc5_1_0:
	.ds	0x0001
_vsprintf_sloc6_1_0:
	.ds	0x0001
_vsprintf_sloc7_1_0:
	.ds	0x0003
_vprintf_PARM_2:
	.ds	0x0001
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
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
G$output_to_string$0$0==.
_output_to_string:
	.ds	0x0001
G$lower_case$0$0==.
_lower_case:
	.ds	0x0001
G$lsd$0$0==.
_lsd:
	.ds	0x0001
_vsprintf_left_justify_1_1:
	.ds	0x0001
_vsprintf_zero_padding_1_1:
	.ds	0x0001
_vsprintf_prefix_sign_1_1:
	.ds	0x0001
_vsprintf_prefix_space_1_1:
	.ds	0x0001
_vsprintf_signed_argument_1_1:
	.ds	0x0001
_vsprintf_char_argument_1_1:
	.ds	0x0001
_vsprintf_long_argument_1_1:
	.ds	0x0001
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
	Fvprintf$output_char$0$0 ==.
;	vprintf.c 63
;	-----------------------------------------
;	 function output_char
;	-----------------------------------------
_output_char:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
	push	_bp
	mov	_bp,sp
;	vprintf.c 0
	mov	r2,dpl
;	vprintf.c 65
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _output_to_string,00102$
00107$:
;	vprintf.c 67
	mov	r3,_output_ptr
	mov	r4,(_output_ptr + 1)
	mov	r5,(_output_ptr + 2)
	inc	_output_ptr
	clr	a
	cjne	a,_output_ptr,00108$
	inc	(_output_ptr + 1)
00108$:
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,r2
	lcall	__gptrput
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00102$:
;	vprintf.c 71
	mov	dpl,r2
	lcall	_putchar
00104$:
	mov	sp,_bp
	pop	_bp
	C$vprintf.c$73$1$1 ==.
	XFvprintf$output_char$0$0 ==.
	ret
	Fvprintf$output_digit$0$0 ==.
;	vprintf.c 77
;	-----------------------------------------
;	 function output_digit
;	-----------------------------------------
_output_digit:
	push	_bp
	mov	_bp,sp
;	vprintf.c 79
	mov	r2,dpl
	clr	c
	mov	a,#0x09
	subb	a,r2
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r3,a
	cjne	a,#0x01,00109$
00109$:
	clr	a
	rlc	a
; Peephole 105   removed redundant mov
	mov  r3,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00110$:
	mov	a,#0x30
	add	a,r2
	mov	r3,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00103$:
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _lower_case,00105$
00111$:
	mov	a,#0x57
	add	a,r2
	mov	r4,a
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
	mov	a,#0x37
	add	a,r2
	mov	r4,a
00106$:
	mov	ar3,r4
00104$:
	mov	dpl,r3
	lcall	_output_char
00101$:
	mov	sp,_bp
	pop	_bp
	C$vprintf.c$80$1$1 ==.
	XFvprintf$output_digit$0$0 ==.
	ret
	Fvprintf$output_2digits$0$0 ==.
;	vprintf.c 84
;	-----------------------------------------
;	 function output_2digits
;	-----------------------------------------
_output_2digits:
	push	_bp
	mov	_bp,sp
;	vprintf.c 87
	mov	r2,dpl
;	vprintf.c 86
	mov	a,r2
	swap	a
	anl	a,#0x0f
	mov	dpl,a
	push	ar2
	lcall	_output_digit
	pop	ar2
;	vprintf.c 87
	mov	a,#0x0f
	anl	a,r2
	mov	dpl,a
	lcall	_output_digit
00101$:
	mov	sp,_bp
	pop	_bp
	C$vprintf.c$88$1$1 ==.
	XFvprintf$output_2digits$0$0 ==.
	ret
	Fvprintf$calculate_digit$0$0 ==.
;	vprintf.c 92
;	-----------------------------------------
;	 function calculate_digit
;	-----------------------------------------
_calculate_digit:
;	vprintf.c 96
	mov	r2,#0x20
00103$:
	cjne	r2,#0x00,00112$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00112$:
;	vprintf.c 115
	  clr  c
	  mov  a,_value+0
	  rlc  a
	  mov  _value+0,a
	  mov  a,_value+1
	  rlc  a
	  mov  _value+1,a
	  mov  a,_value+2
	  rlc  a
	  mov  _value+2,a
	  mov  a,_value+3
	  rlc  a
	  mov  _value+3,a
	  mov  a,_value+4
	  rlc  a
	  mov  _value+4,a
;	vprintf.c 117
	clr	c
	mov	a,0x0004 + _value
	subb	a,_radix
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00105$
00113$:
;	vprintf.c 119
	clr	c
	mov	a,0x0004 + _value
	subb	a,_radix
	mov	0x0004 + _value,a
;	vprintf.c 120
	mov	a,#0x01
	add	a,_value
	mov	_value,a
00105$:
;	vprintf.c 96
	dec	r2
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00107$:
	C$vprintf.c$123$1$1 ==.
	XFvprintf$calculate_digit$0$0 ==.
	ret
	G$vsprintf$0$0 ==.
;	vprintf.c 127
;	-----------------------------------------
;	 function vsprintf
;	-----------------------------------------
_vsprintf:
;	vprintf.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	vprintf.c 141
	mov	_output_ptr,r2
	mov	(_output_ptr + 1),r3
	mov	(_output_ptr + 2),r4
;	vprintf.c 142
	mov	a,r2
	orl	a,r3
	orl	a,r4
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00102$
00262$:
;	vprintf.c 144
	clr	_output_to_string
; Peephole 132   changed ljmp to sjmp
	sjmp 00237$
00102$:
;	vprintf.c 148
	setb	_output_to_string
;	vprintf.c 151
00237$:
00193$:
	mov	r2,_vsprintf_PARM_2
	mov	r3,(_vsprintf_PARM_2 + 1)
	mov	r4,(_vsprintf_PARM_2 + 2)
	mov	a,#0x01
	add	a,r2
	mov	r5,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r3
	mov	r6,a
	mov	ar7,r4
	mov	_vsprintf_PARM_2,r5
	mov	(_vsprintf_PARM_2 + 1),r6
	mov	(_vsprintf_PARM_2 + 2),r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 166   removed redundant mov
	mov  r2,a
	mov  _vsprintf_c_1_1,r2 
	jnz	00263$
	ljmp	00195$
00263$:
;	vprintf.c 153
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x25,00264$
	sjmp	00265$
00264$:
	ljmp	00191$
00265$:
;	vprintf.c 155
	clr	_vsprintf_left_justify_1_1
;	vprintf.c 156
	clr	_vsprintf_zero_padding_1_1
;	vprintf.c 157
	clr	_vsprintf_prefix_sign_1_1
;	vprintf.c 158
	clr	_vsprintf_prefix_space_1_1
;	vprintf.c 159
	clr	_vsprintf_signed_argument_1_1
;	vprintf.c 160
	mov	_radix,#0x00
;	vprintf.c 161
	clr	_vsprintf_char_argument_1_1
;	vprintf.c 162
	clr	_vsprintf_long_argument_1_1
;	vprintf.c 163
	mov	_vsprintf_width_1_1,#0x00
;	vprintf.c 167
	mov	_vsprintf_PARM_2,r5
	mov	(_vsprintf_PARM_2 + 1),r6
	mov	(_vsprintf_PARM_2 + 2),r7
00104$:
	mov	dpl,_vsprintf_PARM_2
	mov	dph,(_vsprintf_PARM_2 + 1)
	mov	b,(_vsprintf_PARM_2 + 2)
	inc	_vsprintf_PARM_2
	clr	a
	cjne	a,_vsprintf_PARM_2,00266$
	inc	(_vsprintf_PARM_2 + 1)
00266$:
	lcall	__gptrget
	mov	_vsprintf_c_1_1,a
;	vprintf.c 169
	mov	dpl,_vsprintf_c_1_1
	lcall	_isdigit
	mov	r1,dpl
	mov	a,r1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00267$:
;	vprintf.c 171
	mov	b,#0x0a
	mov	a,_vsprintf_width_1_1
	mul	ab
	mov	r5,a
	mov	a,_vsprintf_c_1_1
	add	a,#0xd0
; Peephole 105   removed redundant mov
	mov  r6,a
	add	a,r5
;	vprintf.c 173
; Peephole 105   removed redundant mov
	mov  _vsprintf_width_1_1,a
	jnz	00268$
	mov	a,#0x01
	sjmp	00269$
00268$:
	clr	a
00269$:
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00270$:
;	vprintf.c 176
	setb	_vsprintf_zero_padding_1_1
;	vprintf.c 178
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00108$:
;	vprintf.c 181
	mov	dpl,_vsprintf_c_1_1
	push	ar5
	push	ar6
	push	ar7
	push	ar1
	lcall	_islower
	mov	r2,dpl
	pop	ar1
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r2
	cjne	a,#0x01,00271$
00271$:
	cpl	c
	mov	_lower_case,c
;	vprintf.c 182
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _lower_case,00110$
00272$:
;	vprintf.c 184
	mov	a,#0xdf
	anl	a,_vsprintf_c_1_1
	mov	_vsprintf_c_1_1,a
00110$:
;	vprintf.c 187
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x20,00273$
	mov	a,#0x01
	sjmp	00274$
00273$:
	clr	a
00274$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc0_1_0,a
	jz	00275$
	ljmp	00113$
00275$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x2b,00276$
	mov	a,#0x01
	sjmp	00277$
00276$:
	clr	a
00277$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc1_1_0,a
	jz	00278$
	ljmp	00112$
00278$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x2d,00279$
	mov	a,#0x01
	sjmp	00280$
00279$:
	clr	a
00280$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc2_1_0,a
	jz	00281$
	ljmp	00111$
00281$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x42,00282$
	mov	a,#0x01
	sjmp	00283$
00282$:
	clr	a
00283$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc3_1_0,a
	jz	00284$
	ljmp	00114$
00284$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x43,00285$
	mov	a,#0x01
	sjmp	00286$
00285$:
	clr	a
00286$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc4_1_0,a
	jz	00287$
	ljmp	00116$
00287$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x44,00288$
	mov	a,#0x01
	sjmp	00289$
00288$:
	clr	a
00289$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc5_1_0,a
	jz	00290$
	ljmp	00137$
00290$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x49,00291$
	mov	a,#0x01
	sjmp	00292$
00291$:
	clr	a
00292$:
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc6_1_0,a
	jz	00293$
	ljmp	00137$
00293$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x4c,00294$
	mov	a,#0x01
	sjmp	00295$
00294$:
	clr	a
00295$:
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00115$
00296$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x4f,00297$
	ljmp	00139$
00297$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x50,00298$
	ljmp	00133$
00298$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x53,00299$
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00299$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x55,00300$
	ljmp	00140$
00300$:
	mov	a,_vsprintf_c_1_1
	cjne	a,#0x58,00301$
	ljmp	00141$
00301$:
	ljmp	00142$
;	vprintf.c 190
00111$:
	setb	_vsprintf_left_justify_1_1
;	vprintf.c 191
	ljmp	00104$
;	vprintf.c 193
00112$:
	setb	_vsprintf_prefix_sign_1_1
;	vprintf.c 194
	ljmp	00104$
;	vprintf.c 196
00113$:
	setb	_vsprintf_prefix_space_1_1
;	vprintf.c 197
	ljmp	00104$
;	vprintf.c 199
00114$:
	setb	_vsprintf_char_argument_1_1
;	vprintf.c 200
	ljmp	00104$
;	vprintf.c 202
00115$:
	setb	_vsprintf_long_argument_1_1
;	vprintf.c 203
	ljmp	00104$
;	vprintf.c 206
00116$:
	mov	a,_vsprintf_PARM_3
	add	a,#0xff
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	dpl,@r0
	lcall	_output_char
;	vprintf.c 207
	ljmp	00143$
;	vprintf.c 210
00117$:
	mov	a,_vsprintf_PARM_3
	add	a,#0xfd
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	ar2,@r0
	inc	r0
	mov	ar5,@r0
	inc	r0
	mov	ar6,@r0
	dec	r0
	dec	r0
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
;	vprintf.c 212
	mov	dpl,r2
	mov	dph,r5
	mov	b,r6
	lcall	_strlen
	mov	r2,dpl
	mov	r5,dph
	mov	_vsprintf_length_1_1,r2
;	vprintf.c 213
; Peephole 112   removed ljmp by inverse jump logic
	jb   _vsprintf_left_justify_1_1,00124$
00302$:
	clr	c
	mov	a,_vsprintf_length_1_1
	subb	a,_vsprintf_width_1_1
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00124$
00303$:
;	vprintf.c 215
	clr	c
	mov	a,_vsprintf_width_1_1
	subb	a,_vsprintf_length_1_1
	mov	_vsprintf_width_1_1,a
;	vprintf.c 216
	mov	r5,_vsprintf_width_1_1
00118$:
	mov	ar6,r5
	dec	r5
	mov	_vsprintf_width_1_1,r5
	cjne	r6,#0x00,00304$
	mov	a,#0x01
	sjmp	00305$
00304$:
	clr	a
00305$:
; Peephole 105   removed redundant mov
	mov  r7,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00124$
00306$:
;	vprintf.c 218
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,#0x20
	lcall	_output_char
	pop	ar7
	pop	ar6
	pop	ar5
;	vprintf.c 222
; Peephole 132   changed ljmp to sjmp
	sjmp 00118$
00124$:
	mov	r5,_value
	mov	r6,(_value + 1)
	mov	r7,(_value + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  _vsprintf_sloc6_1_0,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00126$
00307$:
;	vprintf.c 223
	mov	a,#0x01
	add	a,r5
	mov	_vsprintf_sloc7_1_0,a
; Peephole 180   changed mov to clr
	clr  a
	addc	a,r6
	mov	(_vsprintf_sloc7_1_0 + 1),a
	mov	(_vsprintf_sloc7_1_0 + 2),r7
	mov	_value,_vsprintf_sloc7_1_0
	mov	(_value + 1),(_vsprintf_sloc7_1_0 + 1)
	mov	(_value + 2),(_vsprintf_sloc7_1_0 + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r2,a
	push	ar2
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r2
	lcall	_output_char
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar2
; Peephole 132   changed ljmp to sjmp
	sjmp 00124$
00126$:
;	vprintf.c 225
	jb	_vsprintf_left_justify_1_1,00308$
	ljmp	00143$
00308$:
	clr	c
	mov	a,_vsprintf_length_1_1
	subb	a,_vsprintf_width_1_1
	jc	00309$
	ljmp	00143$
00309$:
;	vprintf.c 227
	clr	c
	mov	a,_vsprintf_width_1_1
	subb	a,_vsprintf_length_1_1
	mov	_vsprintf_width_1_1,a
;	vprintf.c 228
	mov	r2,_vsprintf_width_1_1
00127$:
	mov	ar5,r2
	dec	r2
	mov	_vsprintf_width_1_1,r2
	cjne	r5,#0x00,00310$
	mov	a,#0x01
	sjmp	00311$
00310$:
	clr	a
00311$:
; Peephole 105   removed redundant mov
	mov  r6,a
	jz	00312$
	ljmp	00143$
00312$:
;	vprintf.c 230
	push	ar2
	push	ar5
	push	ar6
	mov	dpl,#0x20
	lcall	_output_char
	pop	ar6
	pop	ar5
	pop	ar2
;	vprintf.c 236
; Peephole 132   changed ljmp to sjmp
	sjmp 00127$
00133$:
	mov	a,_vsprintf_PARM_3
	add	a,#0xfd
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	ar2,@r0
	inc	r0
	mov	ar5,@r0
	inc	r0
	mov	ar6,@r0
	dec	r0
	dec	r0
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
;	vprintf.c 238
	clr	c
	mov	a,#0x03
	subb	a,0x0002 + _value
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00200$
00313$:
	mov	r2,#0x04
; Peephole 132   changed ljmp to sjmp
	sjmp 00201$
00200$:
	mov	r2,0x0002 + _value
00201$:
	mov	a,r2
; Peephole 180   changed mov to clr
; Peephole 186   optimized movc sequence
	mov  dptr,#_memory_id
	movc a,@a+dptr
	mov	r2,a
	mov	dpl,r2
	lcall	_output_char
;	vprintf.c 239
	mov	dpl,#0x3a
	lcall	_output_char
;	vprintf.c 240
	mov	a,0x0002 + _value
; Peephole 110   removed ljmp by inverse jump logic
	jz  00135$
00314$:
	mov	a,0x0002 + _value
	cjne	a,#0x03,00315$
; Peephole 132   changed ljmp to sjmp
	sjmp 00135$
00315$:
;	vprintf.c 241
	mov	dpl,0x0001 + _value
	lcall	_output_2digits
00135$:
;	vprintf.c 242
	mov	dpl,_value
	lcall	_output_2digits
;	vprintf.c 243
;	vprintf.c 247
; Peephole 132   changed ljmp to sjmp
	sjmp 00143$
00137$:
	setb	_vsprintf_signed_argument_1_1
;	vprintf.c 248
	mov	_radix,#0x0a
;	vprintf.c 249
;	vprintf.c 252
; Peephole 132   changed ljmp to sjmp
	sjmp 00143$
00139$:
	mov	_radix,#0x08
;	vprintf.c 253
;	vprintf.c 256
; Peephole 132   changed ljmp to sjmp
	sjmp 00143$
00140$:
	mov	_radix,#0x0a
;	vprintf.c 257
;	vprintf.c 260
; Peephole 132   changed ljmp to sjmp
	sjmp 00143$
00141$:
	mov	_radix,#0x10
;	vprintf.c 261
;	vprintf.c 265
; Peephole 132   changed ljmp to sjmp
	sjmp 00143$
00142$:
	mov	dpl,_vsprintf_c_1_1
	lcall	_output_char
;	vprintf.c 267
00143$:
;	vprintf.c 269
	mov	a,_radix
	jnz	00316$
	ljmp	00193$
00316$:
;	vprintf.c 275
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_char_argument_1_1,00152$
00317$:
;	vprintf.c 277
	mov	a,_vsprintf_PARM_3
	add	a,#0xff
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	ar2,@r0
	mov	a,r2
	rlc	a
	subb	a,acc
	mov	r5,a
	mov	r6,a
	mov	r7,a
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
	mov	(_value + 3),r7
;	vprintf.c 278
; Peephole 132   changed ljmp to sjmp
; Peephole 164   removed sjmp by inverse jump logic
	jb   _vsprintf_signed_argument_1_1,00153$
00318$:
;	vprintf.c 280
	mov	0x0001 + _value,#0x00
;	vprintf.c 281
	mov	0x0002 + _value,#0x00
;	vprintf.c 282
	mov	0x0003 + _value,#0x00
; Peephole 132   changed ljmp to sjmp
	sjmp 00153$
00152$:
;	vprintf.c 285
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_long_argument_1_1,00149$
00319$:
;	vprintf.c 287
	mov	a,_vsprintf_PARM_3
	add	a,#0xfc
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	ar2,@r0
	inc	r0
	mov	ar5,@r0
	inc	r0
	mov	ar6,@r0
	inc	r0
	mov	ar7,@r0
	dec	r0
	dec	r0
	dec	r0
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
	mov	(_value + 3),r7
; Peephole 132   changed ljmp to sjmp
	sjmp 00153$
00149$:
;	vprintf.c 291
	mov	a,_vsprintf_PARM_3
	add	a,#0xfe
	mov	r0,a
	mov	_vsprintf_PARM_3,r0
	mov	ar2,@r0
	inc	r0
	mov	ar5,@r0
	dec	r0
	mov	a,r5
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
	mov	(_value + 3),r7
;	vprintf.c 292
; Peephole 112   removed ljmp by inverse jump logic
	jb   _vsprintf_signed_argument_1_1,00153$
00320$:
;	vprintf.c 294
	mov	0x0002 + _value,#0x00
;	vprintf.c 295
	mov	0x0003 + _value,#0x00
00153$:
;	vprintf.c 299
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_signed_argument_1_1,00158$
00321$:
;	vprintf.c 301
	mov	a,(_value + 3)
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00155$
00322$:
;	vprintf.c 302
	clr	c
	clr	a
	subb	a,_value
	mov	r2,a
	clr	a
	subb	a,(_value + 1)
	mov	r5,a
	clr	a
	subb	a,(_value + 2)
	mov	r6,a
	clr	a
	subb	a,(_value + 3)
	mov	r7,a
	mov	_value,r2
	mov	(_value + 1),r5
	mov	(_value + 2),r6
	mov	(_value + 3),r7
; Peephole 132   changed ljmp to sjmp
	sjmp 00158$
00155$:
;	vprintf.c 304
	clr	_vsprintf_signed_argument_1_1
00158$:
;	vprintf.c 307
	mov	_vsprintf_length_1_1,#0x00
;	vprintf.c 308
	setb	_lsd
;	vprintf.c 309
	mov	r2,#0x00
00162$:
	mov	a,_value
; Peephole 162   removed sjmp by inverse jump logic
	jz   00324$
00323$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00163$
00324$:
	mov	a,0x0001 + _value
; Peephole 162   removed sjmp by inverse jump logic
	jz   00326$
00325$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00163$
00326$:
;	vprintf.c 310
	mov	a,0x0002 + _value
; Peephole 162   removed sjmp by inverse jump logic
	jz   00328$
00327$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00163$
00328$:
	mov	a,0x0003 + _value
	jnz	00329$
	mov	a,#0x01
	sjmp	00330$
00329$:
	clr	a
00330$:
	mov	r5,a
	mov	_vsprintf_length_1_1,r2
	mov	a,r5
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00164$
00331$:
00163$:
;	vprintf.c 312
	mov	0x0004 + _value,#0x00
;	vprintf.c 313
	push	ar2
	lcall	_calculate_digit
	pop	ar2
;	vprintf.c 327
	  jb   _lsd,1$
	  pop  b                ; b = <lsd>
	  mov  a,_value+4       ; a = <msd>
	  swap a
	  orl  b,a              ; b = <msd><lsd>
	  push b
	  sjmp 2$
1$:
	  mov  a,_value+4       ; a = <lsd>
	  push acc
2$:
;	vprintf.c 329
	inc	r2
;	vprintf.c 330
; Peephole 167   removed redundant bit moves (c not set to _lsd)
	cpl  _lsd 
; Peephole 132   changed ljmp to sjmp
	sjmp 00162$
00164$:
;	vprintf.c 333
	mov	a,_vsprintf_width_1_1
; Peephole 162   removed sjmp by inverse jump logic
	jz   00333$
00332$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00166$
00333$:
;	vprintf.c 338
	mov	_vsprintf_width_1_1,#0x01
00166$:
;	vprintf.c 342
; Peephole 112   removed ljmp by inverse jump logic
	jb   _vsprintf_zero_padding_1_1,00171$
00334$:
;	vprintf.c 344
	mov	a,#0x01
	add	a,_vsprintf_length_1_1
	mov	r2,a
	mov	r5,_vsprintf_width_1_1
00167$:
	clr	c
	mov	a,r2
	subb	a,r5
	clr	a
	rlc	a
	mov	r2,a
	mov	_vsprintf_width_1_1,r5
	mov	a,r2
; Peephole 110   removed ljmp by inverse jump logic
	jz  00171$
00335$:
;	vprintf.c 346
	push	ar5
	mov	dpl,#0x20
	lcall	_output_char
	pop	ar5
;	vprintf.c 347
	dec	r5
; Peephole 132   changed ljmp to sjmp
	sjmp 00167$
00171$:
;	vprintf.c 351
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_signed_argument_1_1,00180$
00336$:
;	vprintf.c 353
	mov	dpl,#0x2d
	lcall	_output_char
;	vprintf.c 355
	dec	_vsprintf_width_1_1
; Peephole 132   changed ljmp to sjmp
	sjmp 00258$
00180$:
;	vprintf.c 357
	mov	a,_vsprintf_length_1_1
; Peephole 110   removed ljmp by inverse jump logic
	jz  00258$
00337$:
;	vprintf.c 360
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_prefix_sign_1_1,00175$
00338$:
;	vprintf.c 362
	mov	dpl,#0x2b
	lcall	_output_char
;	vprintf.c 364
	dec	_vsprintf_width_1_1
; Peephole 132   changed ljmp to sjmp
	sjmp 00258$
00175$:
;	vprintf.c 366
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_prefix_space_1_1,00258$
00339$:
;	vprintf.c 368
	mov	dpl,#0x20
	lcall	_output_char
;	vprintf.c 370
	dec	_vsprintf_width_1_1
;	vprintf.c 375
00258$:
	mov	r2,_vsprintf_width_1_1
00182$:
	mov	ar5,r2
	dec	r2
	clr	c
	mov	a,_vsprintf_length_1_1
	subb	a,r5
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00260$
00340$:
;	vprintf.c 377
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _vsprintf_zero_padding_1_1,00202$
00341$:
	mov	r5,#0x30
; Peephole 132   changed ljmp to sjmp
	sjmp 00203$
00202$:
	mov	r5,#0x20
00203$:
	push	ar2
	mov	dpl,r5
	lcall	_output_char
	pop	ar2
;	vprintf.c 381
; Peephole 132   changed ljmp to sjmp
	sjmp 00182$
00260$:
	mov	r2,_vsprintf_length_1_1
00185$:
	mov	ar5,r2
	dec	r2
	mov	a,r5
	jnz	00342$
	ljmp	00193$
00342$:
;	vprintf.c 383
; Peephole 167   removed redundant bit moves (c not set to _lsd)
	cpl  _lsd 
;	vprintf.c 398
	  jb   _lsd,3$
	  pop  acc              ; a = <msd><lsd>
	  nop                   ; to disable the "optimizer"
	  push acc
	  swap a
	  anl  a,#0x0F          ; a = <msd>
	  sjmp 4$
3$:
	  pop  acc
	  anl  a,#0x0F          ; a = <lsd>
4$:
	  mov  _value+4,a
;	vprintf.c 400
	mov	dpl,0x0004 + _value
	push	ar2
	lcall	_output_digit
	pop	ar2
; Peephole 132   changed ljmp to sjmp
	sjmp 00185$
00191$:
;	vprintf.c 407
	mov	dpl,_vsprintf_c_1_1
	lcall	_output_char
	ljmp	00193$
00195$:
;	vprintf.c 413
; Peephole 111   removed ljmp by inverse jump logic
	jnb  _output_to_string,00198$
00343$:
	mov	dpl,#0x00
	lcall	_output_char
00198$:
	C$vprintf.c$414$1$1 ==.
	XG$vsprintf$0$0 ==.
	ret
	G$vprintf$0$0 ==.
;	vprintf.c 418
;	-----------------------------------------
;	 function vprintf
;	-----------------------------------------
_vprintf:
;	vprintf.c 420
	mov	_vsprintf_PARM_2,dpl
	mov	(_vsprintf_PARM_2 + 1),dph
	mov	(_vsprintf_PARM_2 + 2),b
	mov	_vsprintf_PARM_3,_vprintf_PARM_2
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
	lcall	_vsprintf
00101$:
	C$vprintf.c$421$1$1 ==.
	XG$vprintf$0$0 ==.
	ret
	.area	CSEG    (CODE)
Fvprintf$memory_id$0$0 == .
_memory_id:
	.ascii /IXCP-/
	.byte	0
