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

	push	hl
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

; void *memcpy(void *dest, const void *source, int count)
_memcpy::
	push	de
	push	bc
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	l,8(ix)
	ld	h,9(ix)
	ld	e,10(ix)
	ld	d,11(ix)
	ld	c,12(ix)
	ld	b,13(ix)

	inc	b
	inc	c
	push	hl

	jr	2$
1$:
	ld	a,(de)
	ld	(hl),a
	inc	de
	inc	hl
2$:
	dec	c
	jr	nz,1$
	dec	b
	jr	nz,1$	

	pop	hl
	pop	ix
	pop	bc
	pop	de
	ret

; int strcmp(const char *s1, const char *s2) 
_strcmp::
	push	de
	push	ix
	ld	ix,#0
	add	ix,sp
	ld	e,6(ix)
	ld	d,7(ix)
	ld	l,8(ix)
	ld	h,9(ix)

	jr	1$
2$:	
	ld	a,(de)
	sub	(hl)
	jr	nz,4$
	;; A == 0
	cp	(hl)
	jr	z,3$
1$:	
	inc	de
	inc	hl
	jr	2$

3$:
	ld	hl,#0
	jr	5$
4$:
	ld	hl,#1
	jr	nc,5$
	ld	hl,#-1
5$:
	pop	ix
	pop	de
	ret
	