;-------------------------------------------------------------------------
;   _mulint.s - routine for multiplication of 16 bit (unsigned) int
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

	.module _mulint

;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl __mulint_PARM_2
	.globl __mulint

;--------------------------------------------------------
; overlayable function parameters in zero page
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
__mulint_PARM_2:
	.ds 2

;--------------------------------------------------------
; local aliases
;--------------------------------------------------------
	.define res0 "___SDCC_m6502_ret0"
	.define res1 "___SDCC_m6502_ret1"
	.define P1   "___SDCC_m6502_ret2"
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

__mulint:
	eor #0xff
	sta *P1+0
	txa
	eor #0xff
	sta *P1+1

	ldx #0
	stx *res0
;	stx *res1

	ldy #8
loop1:
	lsr *P1+0
	bcs skip1
	lda *res0
	adc *__mulint_PARM_2+0
	sta *res0
	txa
	adc	*__mulint_PARM_2+1
	tax
skip1:
	asl	*__mulint_PARM_2+0
	rol	*__mulint_PARM_2+1
    dey
    bne loop1

	txa
;	ldy #8
loop2:
	lsr *P1+1
	bcs skip2
	adc *__mulint_PARM_2+1
skip2:
	asl *__mulint_PARM_2+1
	bne loop2
;	dey
;	bne loop2

	tax
;	sta *res1
	lda *res0
;	ldx *res1
	rts
