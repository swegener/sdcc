	.area _CODE
_putchar::
	push	ix
	ld	ix,#0
	add	ix,sp

	ld	a,4(ix)
	out	(0xff),a

	pop	ix
	ret
	
			
