	;; Generic crt0.s for a Z80
	.globl	__main

	.area _HEADER (ABS)
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
	jp	__int
	
	.org	0x150
init:
	;; Stack at the top of memory.
	ld	sp,#0xffff        

	ld	a,#0x01
	out	(0x09),a
	ld	a,#0xEF
	out	(0x07),a
	
	;; Use _main instead of main to bypass sdcc's intelligence
	ei
	call	__main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_CODE
	.area	_DATA

__ticks:
	.ds	2
	.area	_CODE
__int:
	push	af
	push	hl
	ld	hl,#__ticks
	inc	(hl)
	jr	nz,1$
	inc	hl
	inc	(hl)
1$:
	pop	hl
	pop	af
	ei
	ret

_clock::
	ld	hl,#__ticks
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a
	ret
	
_getsp::
	ld	hl,#0
	add	hl,sp
	ret
	
_exit::
	;; Exit - special code to the emulator
	ld	a,#0
	out	(1),a
1$:
	halt
	jr	1$
