;-------------------------------------------------------------------------
;   _fsmul.s - routine for floating point multiplication
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

	.module _fsmul
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl ___fsmul
	
;--------------------------------------------------------
; overlayable function parameters in zero page
;--------------------------------------------------------
	
	.define mres1 "___SDCC_m6502_ret0"
	.define mres2 "___SDCC_m6502_ret1"
	.define mres3 "___SDCC_m6502_ret2"
	.define mres0 "___SDCC_m6502_ret3"
	.define s1    "___SDCC_m6502_ret4"
	.define exp1  "___SDCC_m6502_ret5"
	.define s2    "___SDCC_m6502_ret6"
	.define exp2  "___SDCC_m6502_ret7"
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

___fsmul:
; check if PARM_1 is zero
	lda	*(___fsmul_PARM_1 + 3)
	and	#0x7F
	ora	*(___fsmul_PARM_1 + 2)
;	ora	*(___fsmul_PARM_1 + 1)
;	ora	*___fsmul_PARM_1
	beq	ret_zero

;p1_not0:
	lda	*(___fsmul_PARM_2 + 3)
	and	#0x7F
	ora	*(___fsmul_PARM_2 + 2)
;	ora	*(___fsmul_PARM_2 + 1)
;	ora	*___fsmul_PARM_2
	beq	ret_zero
;	jmp ___fs_ret_zero

;p2_not0:
	ldy	#0x00
	jsr ___fs_unpack_2P
	
	; compute sign
	lda *s1
	eor *s2
	sta *s1
	
	; s2 is now free
	sty *s2
	sec
    lda *exp1
    sbc #126    ; excess
;    sta *exp1
    bcs no_borrow
    dec *s2
no_borrow:
	clc
;	lda *exp1
	adc *exp2
	sta *exp1
	bcc no_carry
	inc *s2
no_carry:
	
	; add hidden bit
	lda *(___fsmul_PARM_1 + 2)
	ora #0x80
	sta *(___fsmul_PARM_1 + 2)
	lda *(___fsmul_PARM_2 + 2)
	ora #0x80
	sta *(___fsmul_PARM_2 + 2)
	
; no need to blank the MSBs as the multiplication routine does not use them
	
;__mul24:
	ldx #0x00
	stx *mres0
	stx *mres1
	stx *mres2
	stx *mres3
	
loop2:
	ldy #8
loop1:
	lsr *___fsmul_PARM_2+0,x
	bcc skip_add1
	clc
	lda *mres1
	adc *___fsmul_PARM_1+0
	sta *mres1
	lda *mres2
	adc *___fsmul_PARM_1+1
	sta *mres2
	lda *mres3
	adc *___fsmul_PARM_1+2
	sta *mres3
skip_add1:
    ror *mres3
    ror *mres2
    ror *mres1
    ror *mres0
    dey
    bne loop1
    inx 
    cpx #3
    bne loop2
    
; round
     bit *mres3
     bmi end
     jmp end7
     bpl not_24  ; MSB not set
     asl *mres0
     bcc end     ; MSB set and rounding will not change the result
	 jsr add3
     jmp end     ; MSB set
ret_zero:
	jmp ___fs_ret_zero
not_24:
     bit *mres0
     bvc end7    ; adding 0x40 will not change the result - shift 7
     bpl need_add
     lda #0x80  ; res0 is 01xx xxxx adding 0x40 produces 10xx xxxx
     sta *mres0
     bne end7
need_add:
     clc
     lda *mres0
     adc #0x40
     sta *mres0
     jsr add3
     bit *mres3
     bpl end7
     lda *mres0
     adc #0x40
     sta *mres0
     jsr add3
     bpl end7
     bmi end
	
end7:
    asl *mres0
    rol *mres1
    rol *mres2
    rol *mres3
    sec
    lda *exp1
    sbc #1
    sta *exp1
    bcs no_borrow2
    dec *s2
no_borrow2:

end:
    lda *s2
;    bmi res_zero
    bmi ret_zero
    bne res_inf
    lda *___SDCC_m6502_ret2
    and #0x7f
    sta *___SDCC_m6502_ret2
	jmp ___fs_pack_ret

;res_zero:
;    jmp ___fs_ret_zero
    
res_inf:
    lda *s1
    jmp ___fs_ret_inf

add3:
     lda *mres1
     adc #0
     sta *mres1
     lda *mres2
     adc #0
     sta *mres2
     lda *mres3
     adc #0
     sta *mres3
     rts
	
