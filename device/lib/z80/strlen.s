        .area   _CODE

	.globl _strlen

; The Z80 has the cpir instruction, which is perfect for implementing strlen().

_strlen:
	push	ix
	ld	ix, #0
	add	ix, sp
	ld	l, 4(ix)
	ld	h, 5(ix)
	xor	a, a
	ld	b, a
	ld	c, a
	cpir
	ld	hl, #-1
	sbc	hl, bc	; C flag still cleared from xor above.
	pop	ix
	ret

