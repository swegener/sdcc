;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:21 2000

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
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area _DUMMY
	.area	OSEG	(OVR,DATA)
_strcat_PARM_2:
	.ds	0x0003
_strcat_dst_1_1:
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
	mov	_strcat_dst_1_1,dpl
	mov	(_strcat_dst_1_1 + 1),dph
	mov	(_strcat_dst_1_1 + 2),b
;	_strcat.c 33
	mov	r5,_strcat_dst_1_1
	mov	r6,(_strcat_dst_1_1 + 1)
	mov	r7,(_strcat_dst_1_1 + 2)
00101$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r0,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00111$
00113$:
;	_strcat.c 34
	inc	r5
	cjne	r5,#0x00,00114$
	inc	r6
00114$:
;	_strcat.c 36
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00111$:
00104$:
	mov	ar3,r5
	mov	ar4,r6
	mov	ar2,r7
	inc	r5
	cjne	r5,#0x00,00115$
	inc	r6
00115$:
	mov	dpl,_strcat_PARM_2
	mov	dph,(_strcat_PARM_2 + 1)
	mov	b,(_strcat_PARM_2 + 2)
	inc	_strcat_PARM_2
	clr	a
	cjne	a,_strcat_PARM_2,00116$
	inc	(_strcat_PARM_2 + 1)
00116$:
	lcall	__gptrget
; Peephole 190   removed redundant mov
; Peephole 191   removed redundant mov
	mov  r0,a
	mov  dpl,r3
	mov  dph,r4
	mov  b,r2
	lcall __gptrput
; Peephole 109   removed ljmp by inverse jump logic
	jnz  00104$
00117$:
;	_strcat.c 38
	mov	dpl,_strcat_dst_1_1
	mov	dph,(_strcat_dst_1_1 + 1)
	mov	b,(_strcat_dst_1_1 + 2)
00107$:
	C$_strcat.c$40$1$1 ==.
	XG$strcat$0$0 ==.
	ret
	.area	CSEG    (CODE)
