        .area   _CODE

__divsint_rrx_s::
        pop     af
        pop     hl
        pop     de
        push    de
        push    hl
        push    af

        jp      __div16

