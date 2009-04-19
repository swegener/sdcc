        ;;
__rrulong_rrx_s::
        ld      hl,#2+4
        add     hl,sp

        ld      a,(hl)
        or      a,a
	pop     bc
	pop     hl
	pop     de
	push    de
	push    hl
	push    bc
        ret     Z
        ld      b,a
        ld      a,e
1$:
        srl     d
        rra
        rr	h
        rr      l

        djnz    1$
        ld      e,a
        ret

__rrslong_rrx_s::
        ld      hl,#2+4
        add     hl,sp

        ld      a,(hl)
        or      a,a
	pop     bc
	pop     hl
	pop     de
	push    de
	push    hl
	push    bc
        ret     Z
        ld      b,a
        ld      a,e
2$:
        sra     d
        rra
        rr	h
        rr      l

        djnz    2$
        ld      e,a
        ret

__rlslong_rrx_s::
__rlulong_rrx_s::
        ld      hl,#2+4
        add     hl,sp

        ld      a,(hl)
        or      a,a
	pop     bc
	pop     hl
	pop     de
	push    de
	push    hl
	push    bc
        ret     Z
        ld      b,a
        ld      a,e
3$:
        add     hl,hl
        rla
        rl	d

        djnz    3$
        ld      e,a
        ret
