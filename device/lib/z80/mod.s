        .area   _CODE

__moduchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__moduchar_rrx_hds::
        call    __divu8

        ex      de,hl

        ret

__moduint_rrx_s::
        pop     af
        pop     hl
        pop     de
        push    de
        push    hl
        push    af

        ;; Fall through
__moduint_rrx_hds::
        call    __divu16

        ex      de,hl

        ret

__moduschar_rrx_s::
        ld      hl,#2+1
        ld      d, h
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ld      a,l             ; Sign extend
        rlca
        sbc     a
        ld      h,a

        call    __div16

        ex      de,hl

        ret

