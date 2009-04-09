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
        call    __divu16

        ex      de,hl

        ret

__moduschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        ld      d, #0
        dec     hl
        ld      l,(hl)

        ld      a,l             ; Sign extend
        rlca
        sbc     a
        ld      h,a

        call    __div16

        ex      de,hl

        ret

