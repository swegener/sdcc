;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:15 2000

;--------------------------------------------------------
	.module _strchr
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _strchr_PARM_2
	.globl _strchr
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
_strchr_PARM_2:
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
	G$strchr$0$0 ==.
;	_strchr.c 27
;	-----------------------------------------
;	 function strchr
;	-----------------------------------------
_strchr:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_strchr.c 0
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	_strchr.c 32
00102$:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
; Peephole 105   removed redundant mov
	mov  r5,a
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00113$:
	mov	a,r5
	cjne	a,_strchr_PARM_2,00114$
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00114$:
;	_strchr.c 33
	inc	r2
	cjne	r2,#0x00,00115$
	inc	r3
00115$:
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00104$:
;	_strchr.c 35
	mov	a,r5
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,_strchr_PARM_2,00106$
; Peephole 201   removed redundant sjmp
00116$:
00117$:
;	_strchr.c 36
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
;	_strchr.c 37
; Peephole 181   used 16 bit load of dptr
	mov  dptr,#0x0000
	mov	b,#0x00
00107$:
	C$_strchr.c$38$1$1 ==.
	XG$strchr$0$0 ==.
	ret
	.area	CSEG    (CODE)
