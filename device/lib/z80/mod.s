        .area   _CODE

__moduchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__moduchar_rrx_hds::
        ld      c,l
        call    __divu8

        ld      l,e
        ld      h,d

        ret

__moduint_rrx_s::
        ld      hl,#2+3
        add     hl,sp

        ld      d,(hl)
        dec     hl
        ld      e,(hl)
        dec     hl
        ld      a,(hl)
        dec     hl
        ld      l,(hl)
        ld      h,a
        ;; Fall through

__moduint_rrx_hds::
        ld      b,h
        ld      c,l

        call    __divu16

        ld      l,e
        ld      h,d

        ret

__moduschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        ld      d, #0
        dec     hl
        ld      c,(hl)

        ld      a,c             ; Sign extend
        rlca
        sbc     a
        ld      b,a

        call    __div16

        ld      l,e
        ld      h,d

        ret

