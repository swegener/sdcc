;-------------------------------------------------------------------------
;   ldiv.s - implementation on std C library ldiv
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

	.module ldiv
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl _ldiv

;--------------------------------------------------------
; local aliases
;--------------------------------------------------------
	.define res0 "__divslong_PARM_1+0"
	.define res1 "__divslong_PARM_1+1"
	.define res2 "___SDCC_m6502_ret2"
	.define res3 "___SDCC_m6502_ret3"
	.define den  "__divslong_PARM_2"
	.define rem  "___SDCC_m6502_ret4"
	.define s1   "___SDCC_m6502_ret0"
	.define s2   "___SDCC_m6502_ret1"

;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
_ldiv:
	jsr	___sdivmod32
	lda	*s1
	bpl	rempos
; neg res
	sec
	ldx	#0x00
	sbc	*rem+0
    sta *rem+0
	txa
	sbc	*rem+1
    sta *rem+1
	txa
	sbc	*rem+2
	sta	*rem+2
	txa
	sbc	*rem+3
	sta	*rem+3
rempos:	
	lda	*s1
	eor	*s2
	bpl	pos
; neg res
	sec
	txa
	sbc	*res0
	tay
	txa
	sbc	*res1
	tax
	lda	#0x00
	sbc	*res2
	sta	*res2
	lda	#0x00
	sbc	*res3
	sta	*res3
	tya
	rts
pos:
	lda	*res0
	ldx	*res1
	rts

