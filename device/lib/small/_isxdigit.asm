;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _isxdigit
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _isxdigit
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
	G$isxdigit$0$0 ==.
;	_isxdigit.c 24
;	-----------------------------------------
;	 function isxdigit
;	-----------------------------------------
_isxdigit:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_isxdigit.c 31
	mov	r2,dpl
;	_isxdigit.c 27
	cjne	r2,#0x30,00113$
00113$:
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00105$
00114$:
	clr	c
	mov	a,#0x39
	subb	a,r2
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00101$
00115$:
00105$:
;	_isxdigit.c 28
	cjne	r2,#0x61,00116$
00116$:
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00107$
00117$:
	clr	c
	mov	a,#0x66
	subb	a,r2
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00101$
00118$:
00107$:
;	_isxdigit.c 29
	cjne	r2,#0x41,00119$
00119$:
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00120$:
	clr	c
	mov	a,#0x46
	subb	a,r2
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00102$
00121$:
00101$:
;	_isxdigit.c 30
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00102$:
;	_isxdigit.c 31
	mov	dpl,#0x00
00108$:
	C$_isxdigit.c$33$1$1 ==.
	XG$isxdigit$0$0 ==.
	ret
	.area	CSEG    (CODE)
