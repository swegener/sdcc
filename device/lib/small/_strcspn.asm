;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:16 2000

;--------------------------------------------------------
	.module _strcspn
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strcspn_PARM_2
	.globl _strcspn
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
_strcspn_PARM_2:
	.ds	0x0003
_strcspn_string_1_1:
	.ds	0x0003
_strcspn_count_1_1:
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
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG (CODE)
	G$strcspn$0$0 ==.
;	_strcspn.c 27
;	-----------------------------------------
;	 function strcspn
;	-----------------------------------------
_strcspn:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strcspn.c 42
	mov	_strcspn_string_1_1,dpl
	mov	(_strcspn_string_1_1 + 1),dph
	mov	(_strcspn_string_1_1 + 2),b
;	_strcspn.c 35
	mov	r5,_strcspn_PARM_2
	mov	r6,(_strcspn_PARM_2 + 1)
	mov	r7,(_strcspn_PARM_2 + 2)
	clr	a
	mov	(_strcspn_count_1_1 + 1),a
	mov	_strcspn_count_1_1,a
00104$:
	mov	dpl,_strcspn_string_1_1
	mov	dph,(_strcspn_string_1_1 + 1)
	mov	b,(_strcspn_string_1_1 + 2)
	lcall	__gptrget
; Peephole 166   removed redundant mov
	mov  r0,a
	mov  ar1,r0 
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00112$:
;	_strcspn.c 36
	mov	_strchr_PARM_2,r1
	push	ar5
	push	ar6
	push	ar7
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	_strchr
	mov	r0,dpl
	mov	r1,dph
	mov	r2,b
	pop	ar7
	pop	ar6
	pop	ar5
	mov	a,r0
	orl	a,r1
	orl	a,r2
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00106$
00113$:
;	_strcspn.c 39
	inc	_strcspn_count_1_1
	clr	a
	cjne	a,_strcspn_count_1_1,00114$
	inc	(_strcspn_count_1_1 + 1)
00114$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00106$:
;	_strcspn.c 42
	mov	dpl,_strcspn_count_1_1
	mov	dph,(_strcspn_count_1_1 + 1)
00107$:
	C$_strcspn.c$43$1$1 ==.
	XG$strcspn$0$0 ==.
	ret
	.area	CSEG    (CODE)
