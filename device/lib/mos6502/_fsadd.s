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
	.globl ___fsadd_PARM_2
	.globl ___fsadd_PARM_1
	.globl ___fssub_PARM_2
	.globl ___fssub_PARM_1
	.globl ___fsadd
;--------------------------------------------------------
; ZP ram data
;--------------------------------------------------------
	.area ZP      (PAG)
;--------------------------------------------------------
; overlayable items in ram
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
___fs_PARM_1:
___fsadd_PARM_1:
___fssub_PARM_1:
	.ds 4
	
___fs_PARM_2:
___fsadd_PARM_2:
___fssub_PARM_2:
	.ds 4
	
;--------------------------------------------------------
; uninitialized external ram data
;--------------------------------------------------------
	.area BSS

;--------------------------------------------------------
; local aliases
;--------------------------------------------------------
	.define sh1   "___SDCC_m6502_ret0"
	.define sh2   "___SDCC_m6502_ret1"
	.define res2  "___SDCC_m6502_ret2"
	.define res3  "___SDCC_m6502_ret3"
	.define s1    "___SDCC_m6502_ret4"
	.define exp1  "___SDCC_m6502_ret5"
	.define s2    "___SDCC_m6502_ret6"
	.define exp2  "___SDCC_m6502_ret7"
	.define rneg  "___SDCC_m6502_ret7"
		
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;------------------------------------------------------------
;	../_fsadd.c: 173: float __fsadd (float f1, float f2) __SDCC_FLOAT_NONBANKED

___fsadd:

; check if PARM_1 is zero
	lda	*(___fsadd_PARM_1 + 3)
	and	#0x7F
	ora	*(___fsadd_PARM_1 + 2)
	ora	*(___fsadd_PARM_1 + 1)
	ora	*___fsadd_PARM_1
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
	ora	*(___fsadd_PARM_2 + 1)
	ora	*___fsadd_PARM_2
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
	
; get exp1
	lda	*(___fsadd_PARM_1 + 0x0002)
	rol	a
	lda	*(___fsadd_PARM_1 + 0x0003)
	rol	a
	sta *exp1
; get sign1 - sign is 0x80
	tya
	ror a
	sta *s1
	
; get exp2
	lda	*(___fsadd_PARM_2 + 0x0002)
	rol	a
	lda	*(___fsadd_PARM_2 + 0x0003)
	rol	a
	sta *exp2
; get sign2 - sign is 0x80
	tya
	ror a
	sta *s2
	
; compute exponent difference
	sec
	lda *exp1
	sbc *exp2
;	sta expd

;   lda expd
 ; a=expd, y=0
    bpl pos_diff
    ; negative expenent difference
    ; expd=-eppd
    eor	#0xff
	clc
	adc	#0x01
	; check for overflow
;	cmp #26
;	bmi skip_p2
;	jmp ret_p2
skip_p2:
    sty *sh2
    sta *sh1
    clc
    adc *exp1
    sta *exp1
    jmp done    
pos_diff:
	; check for overflow
;	cmp #26
;	bmi skip_p1
;	jmp ret_p1
skip_p1:
    sty *sh1
    sta *sh2
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
	sty	*rneg
	lda *s1
	and *s2
	beq not_both_negative
	sta *rneg
	bne end
not_both_negative:
    lda *s1
    beq skip2
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
    lda *s2
    beq end	
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

; add the mantissa
	clc
	lda	*___fsadd_PARM_1
	adc	*___fsadd_PARM_2
	sta	*___fsadd_PARM_1
	lda	*(___fsadd_PARM_1 + 1)
	adc	*(___fsadd_PARM_2 + 1)
	sta	*(___fsadd_PARM_1 + 1)
	lda	*(___fsadd_PARM_1 + 2)
	adc	*(___fsadd_PARM_2 + 2)
	sta	*(___fsadd_PARM_1 + 2)
	lda	*(___fsadd_PARM_1 + 3)
	adc	*(___fsadd_PARM_2 + 3)
	sta	*(___fsadd_PARM_1 + 3)

; is result 0?
;   lda	*(___fsadd_PARM_1 + 3)
	ora	*(___fsadd_PARM_1 + 2)
	ora	*(___fsadd_PARM_1 + 1)
	ora	*___fsadd_PARM_1
	bne	res_not_zero
; return 0
ret_zero:
	; Y is still 0
    ;    tya
    tax
	sta	*___SDCC_m6502_ret2
	sta	*___SDCC_m6502_ret3
	rts
	
res_not_zero:
    bit *(___fsadd_PARM_1 + 3)
    bpl res_pos
    ; result is negative
    ldx #0x80
    stx *rneg
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
res_pos:


normalize:
	lda	*(___fsadd_PARM_1 + 3)
    bne norm_done
;	../_fsadd.c: 236: mant1 <<= 1;
	asl	*___fsadd_PARM_1
	rol	*(___fsadd_PARM_1 + 1)
	rol	*(___fsadd_PARM_1 + 2)
	rol	*(___fsadd_PARM_1 + 3)
;	../_fsadd.c: 237: exp1--;
    dec *exp1
	bne	normalize
	lda #0x00
	jmp ret_zero
norm_done:

round:
	lda	*(___fsadd_PARM_1 + 3)
	and #0xfe
	beq	end_round
	lda	*___fsadd_PARM_1
	and	#0x01
	beq	add_end
	inc	*___fsadd_PARM_1
	bne	add_end
	inc	*(___fsadd_PARM_1 + 1)
	bne	add_end
	inc	*(___fsadd_PARM_1 + 2)
	bne	add_end
	inc	*(___fsadd_PARM_1 + 3)
add_end:

;	../_fsadd.c: 244: mant1 >>= 1;
	lsr	*(___fsadd_PARM_1 + 3)
	ror	*(___fsadd_PARM_1 + 2)
	ror	*(___fsadd_PARM_1 + 1)
	ror	*___fsadd_PARM_1
;	../_fsadd.c: 245: exp1++;
    inc *exp1
    bne round
    
; return infinite
	lda	#0x7f
	ora *rneg
	sta	*___SDCC_m6502_ret3
	lda	#0x80
	sta	*___SDCC_m6502_ret2
	lda	#0x00
	tax
	rts
end_round:
    
	lsr	*(___fsadd_PARM_1 + 2)
	ror	*(___fsadd_PARM_1 + 1)
	ror	*___fsadd_PARM_1

; pack
    lda *exp1
    lsr a
    ora *rneg
	sta	*___SDCC_m6502_ret3
    lda #0x00
    ror a
    ora *(___fsadd_PARM_1 + 2)
	sta	*___SDCC_m6502_ret2
    ldx *(___fsadd_PARM_1 + 1)
    lda *(___fsadd_PARM_1 + 0)
	rts

