;-------------------------------------------------------------------------
;   _fslt.s - routine for floating point comparison
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

	.module _fslt

;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl ___fslt

;--------------------------------------------------------
; local aliases
;--------------------------------------------------------
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

___fslt:
	ldy	#0x00
	sty	*res0
	jsr 	___fs_unpack_2P

	lda 	*exp1
	ora 	*exp2
	bne 	not_denorm
	ora	*(___fslt_PARM_1 + 0)
	ora	*(___fslt_PARM_2 + 0)
	ora	*(___fslt_PARM_1 + 1)
	ora	*(___fslt_PARM_2 + 1)
	ora	*(___fslt_PARM_1 + 2)
	ora	*(___fslt_PARM_2 + 2)
	beq 	ret
not_denorm:
	lda	*s1
	eor 	*s2
	beq 	same_sign
	lda 	*s1
	beq 	ret
	lda 	#0x01
ret:
	rts

same_sign:
	lda 	*s1
	beq 	both_pos
	inc 	*res0
both_pos:
	sec
	lda	*___fslt_PARM_1
	sbc	*___fslt_PARM_2
	sta	*___fslt_PARM_1
	lda	*(___fslt_PARM_1 + 1)
	sbc	*(___fslt_PARM_2 + 1)
	sta	*(___fslt_PARM_1 + 1)
	lda	*(___fslt_PARM_1 + 2)
	sbc	*(___fslt_PARM_2 + 2)
	sta	*(___fslt_PARM_1 + 2)
	lda	*(___fslt_PARM_1 + 3)
	sbc	*(___fslt_PARM_2 + 3)
	sta	*(___fslt_PARM_1 + 3)
	bvc	00150$
	bpl	00149$
	bmi	00109$
00150$:
	bpl	00109$
00149$:
	iny
00109$:
	lda	*___fslt_PARM_1
	ora	*(___fslt_PARM_1 + 1)
	ora	*(___fslt_PARM_1 + 2)
	ora	*(___fslt_PARM_1 + 3)
	beq 	ret
	tya
	eor 	*res0
	rts
