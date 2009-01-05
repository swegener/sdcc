        .area   _CODE

__divschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__divschar_rrx_hds::
        ld      c,l

        call    __div8

        ld      l,c
        ld      h,b

        ret

__modschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__modschar_rrx_hds::
        ld      c,l

        call    __div8

        ld      l,e
        ld      h,d

        ret

__divsint_rrx_s::
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
__divsint_rrx_hds::
        ld      b,h
        ld      c,l

        call    __div16

        ld      l,c
        ld      h,b

        ret

__modsint_rrx_s::
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
__modsint_rrx_hds::
        ld      b,h
        ld      c,l

        call    __div16

        ld      l,e
        ld      h,d

        ret

