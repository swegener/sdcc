        ;; Originally from GBDK by Pascal Felber.

        .area   _CODE

__divuint_rrx_s::
        pop     af
        pop     hl
        pop     de
        push    de
        push    hl
        push    af

        jr      __divu16

__divsuchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)
        ld      h,#0

        jr      signexte

__modsuchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)
        ld      h,#0

        call    signexte

        ex      de,hl
        ret

__divuschar_rrx_s::
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

        jr      __div16

__divschar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__divschar_rrx_hds::
__div8::
        ld      a,l             ; Sign extend
        rlca
        sbc     a
        ld      h,a
signexte:
        ld      a,e             ; Sign extend
        rlca
        sbc     a
        ld      d,a
        ; Fall through to __div16

        ;; signed 16-bit division
        ;;
        ;; Entry conditions
        ;;   HL = dividend
        ;;   DE = divisor
        ;;
        ;; Exit conditions
        ;;   HL = quotient
        ;;   DE = remainder
        ;;   If divisor is non-zero, carry=0
        ;;   If divisor is 0, carry=1 and both quotient and remainder are 0
        ;;
        ;; Register used: AF,B,DE,HL
__divsint_rrx_hds::
__div16::
        ;; Determine sign of quotient by xor-ing high bytes of dividend
        ;;  and divisor. Quotient is positive if signs are the same, negative
        ;;  if signs are different
        ;; Remainder has same sign as dividend
        ld      a,h             ; Get high byte of dividend
        xor     d               ; Xor with high byte of divisor
        rla                     ; Sign of quotient goes into the carry
        ld      a,h             ; Get high byte of dividend
        push    af              ; Save sign of both quotient and reminder

        ;; Take absolute value of dividend
        rla
        jr      NC,.chkde       ; Jump if dividend is positive
        sub     a               ; Substract dividend from 0
        sub     l
        ld      l,a
        sbc     a               ; Propagate borrow (A=0xFF if borrow)
        sub     h
        ld      h,a
        ;; Take absolute value of divisor
.chkde:
        bit     7,d
        jr      Z,.dodiv        ; Jump if divisor is positive
        sub     a               ; Substract divisor from 0
        sub     e
        ld      e,a
        sbc     a               ; Propagate borrow (A=0xFF if borrow)
        sub     d
        ld      d,a
        ;; Divide absolute values
.dodiv:
        call    __divu16
        jr      C,.exit         ; Exit if divide by zero
        ;; Negate quotient if it is negative
        pop     af              ; recover sign of quotient
        jr      NC,.dorem       ; Jump if quotient is positive
        ld      b,a
        sub     a               ; Substract quotient from 0
        sub     l
        ld      l,a
        sbc     a               ; Propagate borrow (A=0xFF if borrow)
        sub     h
        ld      h,a
        ld      a,b
.dorem:
        ;; Negate remainder if it is negative
        rla
        ret     NC              ; Return if remainder is positive
        sub     a               ; Substract remainder from 0
        sub     e
        ld      e,a
        sbc     a               ; Propagate remainder (A=0xFF if borrow)
        sub     d
        ld      d,a
        ret
.exit:
        pop     af

.dividebyzero:
        ld      h,d             ; Divide by zero error: D=E=0
        ld      l,e
        scf                     ; Set carry, invalid result
        ret

        ;; Unsigned
__divuchar_rrx_s::
        ld      hl,#2+1
        add     hl,sp

        ld      e,(hl)
        dec     hl
        ld      l,(hl)

        ;; Fall through
__divuchar_rrx_hds::
__divu8::
        ld      h,#0x00
        ld      d,h
        ; Fall through to divu16

        ;; unsigned 16-bit division
        ;; Restructured on April 2009 by Marco Bodrato(http://bodrato.it/ )
        ;;
        ;; Entry conditions
        ;;   HL = dividend
        ;;   DE = divisor
        ;;
        ;; Exit conditions
        ;;   HL = quotient
        ;;   DE = remainder
        ;;   If divisor is non-zero, carry=0
        ;;   If divisor is 0, carry=1 and both quotient and remainder are 0
        ;;
        ;; Register used: AF,B,DE,HL
__divuint_rrx_hds::
__divu16::
        ;; Check for division by zero
        ld      a,e
        or      d
        jr      Z,.dividebyzero ; Branch if divisor is non-zero
        ;; Two algorithms: one assumes divisor <2^7, the second
        ;; assumes divisor >=2^7; choose the applicable one.
        and     #0x80
        jr      NZ,.morethan7bits
        or      d
        jr      NZ,.morethan7bits
        ;; Both algorithms "rotate" 24 bits (H,L,A) but roles change.

        ;; unsigned 16/7-bit division
        ;; Rewrote on April 2009 by Marco Bodrato ( http://bodrato.it/ )
.atmost7bits:
        ld      b,#16           ; bits in dividend and possible quotient
        ;; Carry cleared by AND/OR, this "0" bit will pass trough HL.[*]
        adc     hl,hl
.dvloop7:
        ;; HL holds both dividend and quotient. While we shift a bit from
        ;;  MSB of dividend, we shift next bit of quotient in from carry.
        ;; A holds remainder.
        rla

        ;; If remainder is >= divisor, next bit of quotient is 1.  We try
        ;;  to compute the difference.
        sub     a,e
        jr      NC,.nodrop7     ; Jump if remainder is >= dividend
        add     a,e             ; Otherwise, restore remainder
        ;; The add above sets the carry, because sbc a,e did set it.
.nodrop7:
        ccf                     ; Complement borrow so 1 indicates a
                                ;  successful substraction (this is the
                                ;  next bit of quotient)
        adc     hl,hl
        djnz    .dvloop7
        ;; Carry now contains the same value it contained before
        ;; entering .dvloop7[*]: "0" = valid result.
        ld      e,a             ; DE = remainder, HL = quotient
        ret

.morethan7bits:
        ld      b,#9            ; at most 9 bits in quotient.
        ld      a,l             ; precompute the first 7 shifts, by
        ld      l,h             ;  doing 8
        ld      h,#0
        rr      l               ;  undoing 1
.dvloop:
        ;; Cleaned up on April 2009 by Marco Bodrato(http://bodrato.it/ )
        ;; Shift next bit of quotient into bit 0 of dividend
        ;; Shift next MSB of dividend into LSB of remainder
        ;; A holds both dividend and quotient. While we shift a bit from
        ;;  MSB of dividend, we shift next bit of quotient in from carry
        ;; HL holds remainder
        adc     hl,hl           ; HL < 2^(7+9), no carry, ever.

        ;; If remainder is >= divisor, next bit of quotient is 1. We try
        ;;  to compute the difference.
        sbc     hl,de
        jr      NC,.nodrop      ; Jump if remainder is >= dividend
        add     hl,de           ; Otherwise, restore remainder
	;; The add above sets the carry, because sbc hl,de did set it.
.nodrop:
        ccf                     ; Complement borrow so 1 indicates a
                                ;  successful substraction (this is the
                                ;  next bit of quotient)
        rla
        djnz    .dvloop
        ;; Take care of the ninth quotient bit! after the loop B=0.
        rl      b               ; BA = quotient
        ;; Carry now contains "0" = valid result.
        ld      d,b
        ld      e,a             ; DE = quotient, HL = remainder
        ex      de,hl           ; HL = quotient, DE = remainder
        ret

