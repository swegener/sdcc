        .area   _CODE

__modschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__modschar_rrx_hds::
        call    __div8

        ex	de,hl
        ret

__modsint_rrx_s::
        pop     af
        pop     hl
        pop     de
        push    de
        push    hl
        push    af

        ;; Fall through
__modsint_rrx_hds::
        call    __div16

        ex      de,hl

        ret

