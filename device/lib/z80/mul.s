	;; Originally from GBDK by Pascal Felber.
	
	.area	_CODE

__mulschar_rrx_s::
        ld      hl,#2
        add     hl,sp

        ld      e,(hl)
        inc     hl
        ld      l,(hl)                

        ;; Fall through
__mulschar_rrx_hds::
        ;; Need to sign extend before going in.
        ld      c,l
        
        ld      a,l
        rla
        sbc     a,a
        ld      b,a

        ld      a,e
        rla
        sbc     a,a
        ld      d,a

        jp      .mul16

__muluchar_rrx_s::
        ld      hl,#2
        add     hl,sp

        ld      e,(hl)

        inc     hl
        ld      c,(hl)                

        ;; Clear the top
        xor     a
        ld      d,a
        ld      b,a
        
        jp      .mulu16
        
__mulsint_rrx_s::
__muluint_rrx_s::
        ld      hl,#2
        add     hl,sp
        
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      a,(hl)
        inc     hl
        ld      h,(hl)
        ld      l,a

        ;; Fall through
        
__muluchar_rrx_hds::
__mulsint_rrx_hds::
__muluint_rrx_hds::
	;; Parameters:
	;;	HL, DE (left, right irrelivent)
	ld	b,h
	ld	c,l
	
	;; 16-bit multiplication
	;; 
	;; Entry conditions
	;;   BC = multiplicand
	;;   DE = multiplier
	;; 
	;; Exit conditions
	;;   DE = less significant word of product
	;;
	;; Register used: AF,BC,DE,HL
.mul16:
.mulu16:
	LD	HL,#0x00	; Product = 0
	LD	A,#15		; Count = bit length - 1
	;; Shift-and-add algorithm
	;; If MSB of multiplier is 1, add multiplicand to partial product
	;; Shift partial product, multiplier left 1 bit
.mlp:
	SLA	E		; Shift multiplier left 1 bit
	RL	D
	jp	NC,.mlp1	; Jump if MSB of multiplier = 0
	ADD	HL,BC		; Add multiplicand to partial product
.mlp1:
	ADD	HL,HL		; Shift partial product left
	DEC	A
	jp	NZ,.mlp		; Continue until count = 0
	;; Add multiplicand one last time if MSB of multiplier is 1
	BIT	7,D		; Get MSB of multiplier
	JR	Z,.mend		; Exit if MSB of multiplier is 0
	ADD	HL,BC		; Add multiplicand to product
.mend:
				; HL = result
	ret

