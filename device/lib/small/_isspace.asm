;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.1.9Ga Sun Jan 16 17:31:13 2000

;--------------------------------------------------------
	.module _isspace
;--------------------------------------------------------
; publics variables in this module
;--------------------------------------------------------
	.globl _isspace
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
	G$isspace$0$0 ==.
;	_isspace.c 24
;	-----------------------------------------
;	 function isspace
;	-----------------------------------------
_isspace:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	_isspace.c 34
	mov	r2,dpl
;	_isspace.c 27
	cjne	r2,#0x20,00115$
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00115$:
;	_isspace.c 28
	cjne	r2,#0x0c,00116$
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00116$:
;	_isspace.c 29
	cjne	r2,#0x0a,00117$
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00117$:
;	_isspace.c 30
	cjne	r2,#0x0d,00118$
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00118$:
;	_isspace.c 31
	cjne	r2,#0x09,00119$
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00119$:
;	_isspace.c 32
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne r2,#0x0b,00102$
; Peephole 201   removed redundant sjmp
00120$:
00121$:
00101$:
;	_isspace.c 33
	mov	dpl,#0x01
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00102$:
;	_isspace.c 34
	mov	dpl,#0x00
00108$:
	C$_isspace.c$35$1$1 ==.
	XG$isspace$0$0 ==.
	ret
	.area	CSEG    (CODE)
