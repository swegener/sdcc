        .area   _CODE

; This multiplication routine is similar to the one
; from Rodnay Zaks, "Programming the Z80".

; Now replaced by a builtin for code generation, but
; still called from some asm files in this directory.
__muluchar_rrx_s::
        ld      hl, #2+1
        add     hl, sp
        ld      e, (hl)
        dec     hl
        ld      h, (hl)
        ld      l, #0
        ld      d, l
        ld      b, #8
muluchar_rrx_s_loop:
        add     hl, hl
        jr      nc, muluchar_rrx_s_noadd
        add     hl, de
muluchar_rrx_s_noadd:
        djnz    muluchar_rrx_s_loop
        ret

; operands have different sign

__mulsuchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      c,(hl)
        ld      b, #0
        jr      signexte

__muluschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      c,(hl)
        ld      b, #0
        dec     hl
        ld      e,(hl)
        jr      signexte

;; Originally from GBDK by Pascal Felber.

__mulschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__mulschar_rrx_hds::
        ;; Need to sign extend before going in.
        ld      c,l

        ld      a,l
        rla
        sbc     a,a
        ld      b,a
signexte:
        ld      a,e
        rla
        sbc     a,a
        ld      d,a

        jp      __mul16

