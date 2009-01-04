        .area   _CODE

	.globl _memmove

; The Z80 has the ldir and lddr instructions, which are perfect for implementing memmove().

_memmove:
	push	ix
	ld	ix, #0
	add	ix, sp
	ld	c, 8(ix)
	ld	b, 9(ix)
	ld	a, c
	or	a, b
	jr	Z, memmove_end
	ld	e, 4(ix)
	ld	d, 5(ix)
	ld	l, 6(ix)
	ld	h, 7(ix)
	ld	a, l
	sbc	hl, de	; or above cleared carry.
	ld	l, a
	ld	h, 7(ix)
	jr	NC, memmove_up
memmove_down:
	add	hl, bc
	dec	hl
	ld	a, e
	add	a, c
	ld	e, a
	ld	a, d
	adc	a, b
	ld	d, a
	dec	de
	lddr
	jr	memmove_end
memmove_up:
	ldir
memmove_end:
	ld	l, 4(ix)
	ld	h, 5(ix)
	pop	ix
	ret

