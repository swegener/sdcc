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

;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl __mullong_PARM_2
	.globl __mullong_PARM_1
	.globl __mullong

;--------------------------------------------------------
; overlayable items in zero page
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
_mullong_tmp:
	.ds 3

;--------------------------------------------------------
; function parameters
;--------------------------------------------------------
	.area XSEG
__mullong_PARM_1:
	.ds 4
__mullong_PARM_2:
	.ds 4

;--------------------------------------------------------
; code
;--------------------------------------------------------
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
        sta     *_mullong_tmp+2
        sta     *_mullong_tmp+1
        sta     *_mullong_tmp+0

        ldy     #32
next_bit:
	lsr     *_mullong_tmp+2
        ror     *_mullong_tmp+1
        ror     *_mullong_tmp+0
        ror     a
        ror     *___SDCC_m6502_ret3
        ror     *___SDCC_m6502_ret2
        ror     *___SDCC_m6502_ret1
        ror     *___SDCC_m6502_ret0
        bcc     skip
        clc
        adc     __mullong_PARM_2+0
        tax
        lda     __mullong_PARM_2+1
        adc     *_mullong_tmp+0
        sta     *_mullong_tmp+0
        lda     __mullong_PARM_2+2
        adc     *_mullong_tmp+1
        sta     *_mullong_tmp+1
        lda     __mullong_PARM_2+3
        adc     *_mullong_tmp+2
        sta     *_mullong_tmp+2
        txa
skip:
        dey
        bpl	next_bit
        ldx	*___SDCC_m6502_ret1
        lda	*___SDCC_m6502_ret0
	rts
