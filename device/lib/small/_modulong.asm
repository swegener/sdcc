;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:14 2000

;--------------------------------------------------------
	.module _modulong
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl __modulong_PARM_2
	.globl __modulong
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
__modulong_PARM_2:
	.ds	0x0004
__modulong_count_1_1:
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
	G$_modulong$0$0 ==.
;	_modulong.c 29
;	-----------------------------------------
;	 function _modulong
;	-----------------------------------------
__modulong:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_modulong.c 51
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	_modulong.c 31
	mov	__modulong_count_1_1,#0x00
;	_modulong.c 34
	mov	__modulong_count_1_1,#0x00
00103$:
	mov	a,(__modulong_PARM_2 + 3)
	rl	a
	anl	a,#0x01
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00117$
00119$:
;	_modulong.c 35
	mov	a,__modulong_PARM_2
	add	a,acc
	mov	r0,a
	mov	a,(__modulong_PARM_2 + 1)
	rlc	a
	mov	r1,a
	mov	a,(__modulong_PARM_2 + 2)
	rlc	a
	mov	r6,a
	mov	a,(__modulong_PARM_2 + 3)
	rlc	a
	mov	r7,a
	mov	__modulong_PARM_2,r0
	mov	(__modulong_PARM_2 + 1),r1
	mov	(__modulong_PARM_2 + 2),r6
	mov	(__modulong_PARM_2 + 3),r7
;	_modulong.c 36
	clr	c
	mov	a,r2
	subb	a,r0
	mov	a,r3
	subb	a,r1
	mov	a,r4
	subb	a,r6
	mov	a,r5
	subb	a,r7
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00102$
00120$:
;	_modulong.c 38
	clr	c
	mov	a,r7
	rrc	a
	mov	(__modulong_PARM_2 + 3),a
	mov	a,r6
	rrc	a
	mov	(__modulong_PARM_2 + 2),a
	mov	a,r1
	rrc	a
	mov	(__modulong_PARM_2 + 1),a
	mov	a,r0
	rrc	a
	mov	__modulong_PARM_2,a
;	_modulong.c 39
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00102$:
;	_modulong.c 41
	inc	__modulong_count_1_1
	ljmp	00103$
;	_modulong.c 44
00117$:
	mov	r6,__modulong_count_1_1
00108$:
;	_modulong.c 45
	clr	c
	mov	a,r2
	subb	a,__modulong_PARM_2
	mov	a,r3
	subb	a,(__modulong_PARM_2 + 1)
	mov	a,r4
	subb	a,(__modulong_PARM_2 + 2)
	mov	a,r5
	subb	a,(__modulong_PARM_2 + 3)
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00121$:
;	_modulong.c 46
	clr	c
	mov	a,r2
	subb	a,__modulong_PARM_2
	mov	r2,a
	mov	a,r3
	subb	a,(__modulong_PARM_2 + 1)
	mov	r3,a
	mov	a,r4
	subb	a,(__modulong_PARM_2 + 2)
	mov	r4,a
	mov	a,r5
	subb	a,(__modulong_PARM_2 + 3)
	mov	r5,a
00107$:
;	_modulong.c 48
	clr	c
	mov	a,(__modulong_PARM_2 + 3)
	rrc	a
	mov	(__modulong_PARM_2 + 3),a
	mov	a,(__modulong_PARM_2 + 2)
	rrc	a
	mov	(__modulong_PARM_2 + 2),a
	mov	a,(__modulong_PARM_2 + 1)
	rrc	a
	mov	(__modulong_PARM_2 + 1),a
	mov	a,__modulong_PARM_2
	rrc	a
	mov	__modulong_PARM_2,a
;	_modulong.c 49
	mov	ar7,r6
	dec	r6
	mov	a,r7
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00108$
00122$:
;	_modulong.c 51
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
00111$:
	C$_modulong.c$52$1$1 ==.
	XG$_modulong$0$0 ==.
	ret
	.area	CSEG    (CODE)
