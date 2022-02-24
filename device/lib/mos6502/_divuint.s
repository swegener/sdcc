;-------------------------------------------------------------------------
;   _divuint.s - routine for division of 16 bit unsigned int
;
;   Copyright (C) 1998, Ullrich von Bassewitz
;   Copyright (C) 2022, Gabriele Gorla
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

	.module _divuint

;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl __divuint_PARM_2
	.globl __divuint

;--------------------------------------------------------
; function parameters
;--------------------------------------------------------
	.area BSS
__divuint_PARM_2:
	.ds 2

;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

__divuint:
	sta	*___SDCC_m6502_ret0+0
	stx	*___SDCC_m6502_ret0+1

	lda	#0
	sta	*___SDCC_m6502_ret2+1
	ldy	#16
;	ldx	__divuint_PARM_2+1
;	beq	div16x8
next_bit:
	asl	*___SDCC_m6502_ret0+0
	rol	*___SDCC_m6502_ret0+1
	rol	a
	rol	*___SDCC_m6502_ret2+1

	tax
	cmp	__divuint_PARM_2+0
	lda	*___SDCC_m6502_ret2+1
	sbc	__divuint_PARM_2+1
	bcc	L1
	sta	*___SDCC_m6502_ret2+1
	txa
	sbc	__divuint_PARM_2+0
	tax
	inc	*___SDCC_m6502_ret0+0
L1:
	txa
	dey
	bne	next_bit
	sta	*___SDCC_m6502_ret2+0

	lda	*___SDCC_m6502_ret0+0
	ldx	*___SDCC_m6502_ret0+1
	rts

;div16x8:
;LL0:
;	asl	*___SDCC_m6502_ret0+0
;	rol	*___SDCC_m6502_ret0+1
;	rol	a
;	bcs	LL1
;	cmp	__divuint_PARM_2+0
;	bcc	LL2
;LL1:
;	sbc	__divuint_PARM_2+0
;	inc	*___SDCC_m6502_ret0+0
;LL2:
;	dey
;	bne	LL0
;	sta	*___SDCC_m6502_ret2+0
;
;	lda	*___SDCC_m6502_ret0+0
;	ldx	*___SDCC_m6502_ret0+1
;	rts
