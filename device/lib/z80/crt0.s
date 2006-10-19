	;; Generic crt0.s for a Z80
        .module crt0
       	.globl	_main

	.area HEADER (ABS)
	;; Reset vector
	.org 	0
	jp	init

	.org	0x08
	reti
	.org	0x10
	reti
	.org	0x18
	reti
	.org	0x20
	reti
	.org	0x28
	reti
	.org	0x30
	reti
	.org	0x38
	reti
	
	.org	0x100
init:
	;; Stack at the top of memory.
	ld	sp,#0xffff        

        ;; Initialise global variables
        call    gsinit
	call	_main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	HOME
	.area	CODE
        .area   GSINIT
        .area   GSFINAL
        
	.area	DATA
        .area   BSS
        .area   HEAP

        .area   CODE
__clock::
	ld	a,#2
        rst     0x08
	ret
	
_exit::
	;; Exit - special code to the emulator
	ld	a,#0
        rst     0x08
1$:
	halt
	jr	1$

        .area   GSINIT
gsinit::	

        .area   GSFINAL
        ret
