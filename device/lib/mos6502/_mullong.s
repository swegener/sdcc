;-------------------------------------------------------------------------
;   _mullong.s - routine for multiplication of 32 bit (unsigned) long
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

	.module _mullong

	.globl __mullong_PARM_2
	.globl __mullong_PARM_1
	.globl __mullong


	.globl ___SDCC_m6502_ret0
	.globl ___SDCC_m6502_ret1
	.globl ___SDCC_m6502_ret2
	.globl ___SDCC_m6502_ret3
	.globl ___SDCC_m6502_ret4
	.globl ___SDCC_m6502_ret5
	.globl ___SDCC_m6502_ret6
	.globl ___SDCC_m6502_ret7

	.area XSEG
__mullong_PARM_1:
	.ds 4
__mullong_PARM_2:
	.ds 4

	.area CSEG

__mullong:

        ldx	__mullong_PARM_1+3
        stx	*___SDCC_m6502_ret3
        ldx	__mullong_PARM_1+2
        stx	*___SDCC_m6502_ret2
        ldx	__mullong_PARM_1+1
        stx	*___SDCC_m6502_ret1
        ldx	__mullong_PARM_1+0
        stx	*___SDCC_m6502_ret0
        lda     #0
        sta     *___SDCC_m6502_ret7
        sta     *___SDCC_m6502_ret6
        sta     *___SDCC_m6502_ret5
        ldy     #32
00001$:
	lsr     *___SDCC_m6502_ret7
        ror     *___SDCC_m6502_ret6
        ror     *___SDCC_m6502_ret5
        ror     a
        ror     *___SDCC_m6502_ret3
        ror     *___SDCC_m6502_ret2
        ror     *___SDCC_m6502_ret1
        ror     *___SDCC_m6502_ret0
        bcc     00002$
        clc
        adc     __mullong_PARM_2+0
        tax
        lda     __mullong_PARM_2+1
        adc     *___SDCC_m6502_ret5
        sta     *___SDCC_m6502_ret5
        lda     __mullong_PARM_2+2
        adc     *___SDCC_m6502_ret6
        sta     *___SDCC_m6502_ret6
        lda     __mullong_PARM_2+3
        adc     *___SDCC_m6502_ret7
        sta     *___SDCC_m6502_ret7
        txa
00002$:
        dey
        bpl	00001$
        ldx	*___SDCC_m6502_ret1
        lda	*___SDCC_m6502_ret0
	rts
