;--------------------------------------------------------------------------
;  mul.s
;
;  Copyright (C) 2000, Michael Hope
;  Copyright (C) 2021, Sebastian 'basxto' Riedel (sdcc@basxto.de)
;  Copyright (c) 2021, Philipp Klaus Krause
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
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

        ;; Originally from GBDK by Pascal Felber.

        .area   _CODE

.globl	__mulsuchar
.globl	__muluschar
.globl	__mulschar
.globl	__muluchar
.globl	__mulint

; operands with different sign

__mulsuchar:
	ld	c, a
	ld	b, #0

        jr      signexte

__muluschar:
	ld	c, e
	ld	b, #0
	ld	e, a

        jr      signexte

__mulschar:
	; Sign-extend before going in.
	ld	c,a

        rla
        sbc     a,a
        ld      b,a
signexte:
        ld      a,e
        rla
        sbc     a,a
        ld      d,a

        jp      .mul16

__muluchar:
	ld	c, a

	; Clear the top
	ld	d, #0
	ld	b, d

__mulint:
        ;; 16-bit multiplication
        ;;
        ;; Entry conditions
        ;;   BC = multiplicand
        ;;   DE = multiplier
        ;;
        ;; Exit conditions
        ;;   BC = less significant word of product
        ;;
        ;; Register used: AF,BC,DE,HL
.mul16:
        ld      hl,#0
        ld      a,b
        ; ld c,c
        ld      b,#16

        ;; Optimise for the case when this side has 8 bits of data or
        ;; less.  This is often the case with support address calls.
        or      a
        jp      NZ,1$

        ld      b,#8
        ld      a,c
1$:
        ;; Taken from z88dk, which originally borrowed from the
        ;; Spectrum rom.
        add     hl,hl
        rl      c
        rla                     ;DLE 27/11/98
        jp      NC,2$
        add     hl,de
2$:
        dec     b
        jr      NZ,1$

        ;; Return in bc
        ld      c,l
        ld      b,h

        ret

