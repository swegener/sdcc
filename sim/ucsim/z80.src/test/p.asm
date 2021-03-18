	org 0

	ld	hl,0xfff0
	ld	sp,hl

	ld	a,50
	ld	c,8
	call	do_test

	ld 	a,0xa0
	ld 	c,0x30
	call 	do_test
	halt
	
do_test:
	ld 	hl,1000
	ld 	(hl),a
	
	;; old, C=1
	scf
	ld  	a,0		; 0x3e 0x00
	out 	(c),a
	sbc 	a,(hl)		; 0x9e
	call 	save
	
	;; old, C=0
	scf
	ccf
	ld 	a,0
	out 	(c),a
	sbc 	a,(hl)
	call 	save
	
	;; new, C=1
	scf
	sbc 	a,a		; 0x9f
	out 	(c),a
	sub 	a,(hl)		; 0x96
	call	save
	
	;; new, C=0
	scf
	ccf
	sbc 	a,a
	out 	(c),a
	sub 	a,(hl)
	call 	save
	
	ld 	a,(hl)

	ret
	
getf:
	push hl
	push af
	pop hl
	ld a,l
	pop hl
	ret
	
save:
	ld e,a
	call getf
	ld d,a
	inc c
	out (c),e
	inc c
	out (c),d
	ld a,6
	add a,c
	ld c,a
	ret
