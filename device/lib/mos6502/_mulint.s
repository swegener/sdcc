;-------------------------------------------------------------------------
;   _mulint.s - routine for multiplication of 32 bit (unsigned) long
;
;   Copyright (C) 2009, Ullrich von Bassewitz
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

	.module _mulint

	.globl __mulint_PARM_2
	.globl __mulint

	.globl ___SDCC_m6502_ret0
	.globl ___SDCC_m6502_ret1
	.globl ___SDCC_m6502_ret2
	.globl ___SDCC_m6502_ret3
	.globl ___SDCC_m6502_ret4

	.area XSEG
__mulint_PARM_2:
	.ds 2

	.area CSEG

__mulint:
	sta	*___SDCC_m6502_ret0
	stx	*___SDCC_m6502_ret1
	lda	__mulint_PARM_2+0
	sta	*___SDCC_m6502_ret2
	lda	__mulint_PARM_2+1
	sta	*___SDCC_m6502_ret3

	lda	#0
	sta	*___SDCC_m6502_ret4
	ldy	#16

	lsr	*___SDCC_m6502_ret1
	ror	*___SDCC_m6502_ret0
00001$:
	bcc	00002$
	clc
	adc	*___SDCC_m6502_ret2
	tax
	lda	*___SDCC_m6502_ret3
	adc	*___SDCC_m6502_ret4
	sta	*___SDCC_m6502_ret4
	txa
00002$:
	ror	*___SDCC_m6502_ret4
	ror	a
	ror	*___SDCC_m6502_ret1
	ror	*___SDCC_m6502_ret0
	dey
	bne	00001$

	lda	*___SDCC_m6502_ret0
	ldx	*___SDCC_m6502_ret1
	rts
