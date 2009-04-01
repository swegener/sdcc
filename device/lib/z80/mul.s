        .area   _CODE

__mulint_rrx_s::
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
__mulint_rrx_hds::
	;; Parameters:
	;;	HL, DE (left, right irrelevant)
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
__mul16::
        ld      l,#0
        ld      a,b
        ld      b,#16

        ;; Optimise for the case when this side has 8 bits of data or
        ;; less.  This is often the case with support address calls.
        or      a
        jr      NZ,2$

        ld      b,#8
        ld      a,c
1$:
        ;; Taken from z88dk, which originally borrowed from the
        ;; Spectrum rom.
        add     hl,hl
2$:
        rl      c
        rla                     ;DLE 27/11/98
        jr      NC,3$
        add     hl,de
3$:
        djnz    1$
        ret

