;--------------------------------------------------------------------------
;  divsigned.s
;
;  Copyright (C) 2000, Michael Hope
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2.1, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

        .area   _CODE

__divsint_rrx_s::
        pop     af
        pop     hl
        pop     de
        push    de
        push    hl
        push    af

        jp      __div16

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
__div_signexte::
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
;        jr      C,.exit         ; Exit if divide by zero
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

