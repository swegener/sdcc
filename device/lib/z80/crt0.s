	;; Generic crt0.s for a Z80
	.globl	__main

	.area _HEADER (ABS)
	;; Reset vector
	.org 	0
	jp	init

	.org	0x38
	reti
	
	.org	0x150
init:
	;; Stack at the top of memory.
	ld	sp,#0xffff        

	;; Use _main instead of main to bypass sdcc's intelligence
	call	__main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_CODE
	.area	_DATA

	.area	_CODE
_exit::
	;; Exit - special code to the emulator
	ld	a,#0
	out	(1),a
1$:
	halt
	jr	1$
