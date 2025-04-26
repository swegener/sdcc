;-------------------------------------------------------------------------
;   _fsadd.s - routine for floating point addition
;
;   Copyright (C) 2025, Gabriele Gorla
;
;   This library is free software; you can redistribute it and/or modify it
;   under the terms of the GNU General Public License as published by the
;   Free Software Foundation; either version 2, or (at your option) any
;   later version.
;
;   This library is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this library; see the file COPYING. If not, write to the
;   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;   As a special exception, if you link this library with other files,
;   some of which are compiled with SDCC, to produce an executable,
;   this library does not by itself cause the resulting executable to
;   be covered by the GNU General Public License. This exception does
;   not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;-------------------------------------------------------------------------

	.module _fsadd
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl ___fsadd

;--------------------------------------------------------
; ZP ram data
;--------------------------------------------------------
	.area ZP      (PAG)

;--------------------------------------------------------
; overlayable items in ram
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)

;--------------------------------------------------------
; local aliases
;--------------------------------------------------------
	.define sh1   "___SDCC_m6502_ret0"
	.define sh2   "___SDCC_m6502_ret1"
	.define res0  "___SDCC_m6502_ret0"
	.define res1  "___SDCC_m6502_ret1"
	.define res2  "___SDCC_m6502_ret2"
	.define res3  "___SDCC_m6502_ret3"
	.define s1    "___SDCC_m6502_ret4"
	.define exp1  "___SDCC_m6502_ret5"
	.define s2    "___SDCC_m6502_ret6"
	.define exp2  "___SDCC_m6502_ret7"
		
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;------------------------------------------------------------

___fsadd:
; check if PARM_1 is zero
	lda	*(___fsadd_PARM_1 + 3)
	and	#0x7F
	ora	*(___fsadd_PARM_1 + 2)
;	ora	*(___fsadd_PARM_1 + 1)
;	ora	*___fsadd_PARM_1
	bne	p1_not0
	
; return PARM_2
ret_p2:
	lda	*(___fsadd_PARM_2 + 3)
	sta	*___SDCC_m6502_ret3
	lda	*(___fsadd_PARM_2 + 2)
	sta	*___SDCC_m6502_ret2
	ldx	*(___fsadd_PARM_2 + 1)
	lda	*___fsadd_PARM_2
	rts
p1_not0:

; check if PARM_2 is zero
	lda	*(___fsadd_PARM_2 + 3)
	and	#0x7F
	ora	*(___fsadd_PARM_2 + 2)
;	ora	*(___fsadd_PARM_2 + 1)
;	ora	*___fsadd_PARM_2
	bne	p2_not0

; return PARM_1
ret_p1:
	lda	*(___fsadd_PARM_1 + 3)
	sta	*___SDCC_m6502_ret3
	lda	*(___fsadd_PARM_1 + 2)
	sta	*___SDCC_m6502_ret2
	ldx	*(___fsadd_PARM_1 + 1)
	lda	*___fsadd_PARM_1
	rts
p2_not0:

	ldy	#0x00
	jsr	___fs_unpack_2P
	
; compute exponent difference
	sec
	lda	*exp1
	sbc	*exp2
;	sta	expd

;   lda expd
 ; a=expd, y=0
	bpl	pos_diff
    ; negative exponent difference
    ; expd=-eppd
    eor	#0xff
	clc
	adc	#0x01
	; check for overflow
;	cmp	#26
;	bmi	skip_p2
;	jmp	ret_p2
skip_p2:
	sty	*sh2
	sta	*sh1
	clc
	adc	*exp1
	sta	*exp1
	jmp	done    
pos_diff:
	; check for overflow
;	cmp	#26
;	bmi	skip_p1
;	jmp	ret_p1
skip_p1:
	sty	*sh1
	sta	*sh2

done:
	ldx	#0x01
; PARM_1 is now mantissa
	asl	*(___fsadd_PARM_1 + 0)
	rol	*(___fsadd_PARM_1 + 1)
 	rol	*(___fsadd_PARM_1 + 2)
	stx	*(___fsadd_PARM_1 + 3)
	
; PARM_2 is now mantissa
	asl	*(___fsadd_PARM_2 + 0)
	rol	*(___fsadd_PARM_2 + 1)
 	rol	*(___fsadd_PARM_2 + 2)
	stx	*(___fsadd_PARM_2 + 3)

; shift param 1 if necessary
	ldy	*sh1
	beq	00110$
00266$:
	lsr	*(___fsadd_PARM_1 + 3)
	ror	*(___fsadd_PARM_1 + 2)
	ror	*(___fsadd_PARM_1 + 1)
	ror	*___fsadd_PARM_1
	dey
	bne	00266$
00110$:

; shift param 2 if necessary
	ldy	*sh2
	beq	00269$
00268$:
	lsr	*(___fsadd_PARM_2 + 3)
	ror	*(___fsadd_PARM_2 + 2)
	ror	*(___fsadd_PARM_2 + 1)
	ror	*___fsadd_PARM_2
	dey
	bne	00268$
00269$:

; Y is still 0
; rneg and exp2 share the same location
;	sty	*rneg  ; Y is still 0
	ldx	#0x00   ; rneg is in X
	lda	*s1
	and	*s2
	beq	not_both_negative
;	sta	*rneg
	tax            ; rneg is in X
	bne	end
not_both_negative:
	lda	*s1
	beq	skip2
; Y is still 0
	sec
	tya
	sbc	*___fsadd_PARM_1
	sta	*___fsadd_PARM_1
	tya
	sbc	*(___fsadd_PARM_1 + 1)
	sta	*(___fsadd_PARM_1 + 1)
	tya
	sbc	*(___fsadd_PARM_1 + 2)
	sta	*(___fsadd_PARM_1 + 2)
	tya
	sbc	*(___fsadd_PARM_1 + 3)
	sta	*(___fsadd_PARM_1 + 3)  
skip2:
	lda	*s2
	beq	end	
	sec
    tya
	sbc	*___fsadd_PARM_2
	sta	*___fsadd_PARM_2
	tya
	sbc	*(___fsadd_PARM_2 + 1)
	sta	*(___fsadd_PARM_2 + 1)
	tya
	sbc	*(___fsadd_PARM_2 + 2)
	sta	*(___fsadd_PARM_2 + 2)
	tya
	sbc	*(___fsadd_PARM_2 + 3)
	sta	*(___fsadd_PARM_2 + 3)
end:

; sh1 and sh2 are no longer needed
; reuse res0 and res1 for the result

; add the mantissa
	clc
	lda	*___fsadd_PARM_1
	adc	*___fsadd_PARM_2
	sta	*res0
	lda	*(___fsadd_PARM_1 + 1)
	adc	*(___fsadd_PARM_2 + 1)
	sta	*res1
	lda	*(___fsadd_PARM_1 + 2)
	adc	*(___fsadd_PARM_2 + 2)
	sta	*res2
	lda	*(___fsadd_PARM_1 + 3)
	adc	*(___fsadd_PARM_2 + 3)
	sta	*res3

; is result 0?
;   lda	*res3
	ora	*res2
	ora	*res1
	ora	*res0
	bne	res_not_zero
	jmp	___fs_ret_zero  

res_not_zero:
	bit	*res3
	bpl	res_pos
    ; result is negative
	ldx	#0x80
;    stx *rneg
	sec
	tya
	sbc	*res0
	sta	*res0
	tya
	sbc	*res1
	sta	*res1
	tya
	sbc	*res2
	sta	*res2
	tya
	sbc	*res3
	sta	*res3  
res_pos:


normalize:
	lda	*res3
	bne	norm_done
	asl	*res0
	rol	*res1
	rol	*res2
	rol	*res3
	dec	*exp1
	bne	normalize
	jmp	___fs_ret_zero

norm_done:

round:
	lda	*res3
	and	#0xfe
	beq	end_round
	lda	*res0
	and	#0x01
	beq	add_end
	inc	*res0
	bne	add_end
	inc	*res1
	bne	add_end
	inc	*res2
	bne	add_end
	inc	*res3
add_end:

	lsr	*res3
	ror	*res2
	ror	*res1
	ror	*res0
	inc	*exp1
	bne	round
;    	lda	*rneg
	txa     ; rneg is in X
	jmp 	___fs_ret_inf ; return infinite

end_round:
    
	lsr	*res2
	ror	*res1
	ror	*res0

	stx	*s1
	jmp	___fs_pack_ret

