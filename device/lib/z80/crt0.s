	;; Generic crt0.s for a Z80
        .module crt0
       	.globl	_main

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
	.area	_CODE
        .area   _GSINIT
        .area   _GSFINAL
        
	.area	_DATA
        .area   _BSS

        .area   _CODE
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

        ;; Special RLE decoder used for initing global data
__initrleblock::
        ;; Pop the return address
        pop     hl
        ;; Save registers
        push    bc
        push    de

        ;; Pull the destination address out
        ld      c,(hl)
        inc     hl
        ld      b,(hl)
        inc     hl
1$:
        ;; Fetch the run
        ld      e,(hl)
        inc     hl
        ;; Negative means a run
        bit     7,e
        jp      z,2$
        ;; Code for expanding a run
        ld      a,(hl)
        inc     hl
3$:
        ld      (bc),a
        inc     bc
        inc     e
        jp      nz,3$
        jp      1$
2$:
        ;; Zero means end of a block
        xor     a
        or      e
        jp      z,4$
        ;; Code for expanding a block
5$:     
        ld      a,(hl)        
        inc     hl
        ld      (bc),a
        inc     bc
        dec     e
        jp      nz,5$
        jp      1$
4$:     
        pop     de
        pop     bc

        ;; Push the return address back onto the stack
        push    hl
        ret

        .area   _GSINIT
gsinit::	

        .area   _GSFINAL
        ret
