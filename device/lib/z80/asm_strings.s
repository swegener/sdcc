	;; Implementation of some string functions in
	;; assembler.

	;; Why - because I want a better dhrystone score :)

; char *strcpy(char *dest, const char *source)
_strcpy::
	push	de
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	l,6(ix)
	ld	h,7(ix)
	ld	e,8(ix)
	ld	d,9(ix)

	push	de
1$:	
	ld	a,(de)
	ld	(hl),a
	inc	hl
	inc	de
	or	a,a
	jr	nz,1$

	pop	hl
	pop	ix
	pop	de
	ret

	