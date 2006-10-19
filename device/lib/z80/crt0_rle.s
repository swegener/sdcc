        .area   CODE

        ;; Special RLE decoder used for initing global data
__initrleblock::
        ;; Pull the destination address out
        ld      c,l
        ld      b,h

        ;; Pop the return address
        pop     hl
1$:
        ;; Fetch the run
        ld      e,(hl)
        inc     hl
        ;; Negative means a run
        bit     7,e
        jp      Z,2$
        ;; Code for expanding a run
        ld      a,(hl)
        inc     hl
3$:
        ld      (bc),a
        inc     bc
        inc     e
        jp      NZ,3$
        jp      1$
2$:
        ;; Zero means end of a block
        xor     a
        or      e
        jp      Z,4$
        ;; Code for expanding a block
5$:
        ld      a,(hl)
        inc     hl
        ld      (bc),a
        inc     bc
        dec     e
        jp      NZ,5$
        jp      1$
4$:
        ;; Push the return address back onto the stack
        push    hl
        ret

