;-------------------------------------------------------------------------
;   _divulong.s - routine for 32 bit unsigned long division
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

	.module _divulong

;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl __divulong_PARM_2
	.globl __divulong_PARM_1
	.globl __divulong
	.globl ___udiv32

;--------------------------------------------------------
; overlayable items in zero page
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
_divulong_tmp:
	.ds 4

;--------------------------------------------------------
; function parameters
;--------------------------------------------------------
	.area XSEG
__divulong_PARM_1:
	.ds 4
__divulong_PARM_2:
	.ds 4

;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG

__divulong:
        ldx	__divulong_PARM_1+3
        stx	*___SDCC_m6502_ret0+3
        ldx	__divulong_PARM_1+2
        stx	*___SDCC_m6502_ret0+2
        ldx	__divulong_PARM_1+1
        stx	*___SDCC_m6502_ret0+1
        ldx	__divulong_PARM_1+0
        stx	*___SDCC_m6502_ret0+0

        ldx	__divulong_PARM_2+3
        stx	*___SDCC_m6502_ret4+3
        ldx	__divulong_PARM_2+2
        stx	*___SDCC_m6502_ret4+2
        ldx	__divulong_PARM_2+1
        stx	*___SDCC_m6502_ret4+1
        ldx	__divulong_PARM_2+0
        stx	*___SDCC_m6502_ret4+0

___udiv32:
	lda     #0
        sta     *_divulong_tmp+0
        sta     *_divulong_tmp+1
        sta     *_divulong_tmp+2
        sta     *_divulong_tmp+3
        ldy     #32
L0:
	asl     *___SDCC_m6502_ret0+0
        rol     *___SDCC_m6502_ret0+1
        rol     *___SDCC_m6502_ret0+2
        rol     *___SDCC_m6502_ret0+3
        rol     a
        rol     *_divulong_tmp+1
        rol     *_divulong_tmp+2
        rol     *_divulong_tmp+3

; Do a subtraction. we do not have enough space to store the intermediate
; result, so we may have to do the subtraction twice.
        tax
        cmp     *___SDCC_m6502_ret4+0
        lda     *_divulong_tmp+1
        sbc     *___SDCC_m6502_ret4+1
        lda     *_divulong_tmp+2
        sbc     *___SDCC_m6502_ret4+2
        lda     *_divulong_tmp+3
        sbc     *___SDCC_m6502_ret4+3
        bcc     L1

; Overflow, do the subtraction again, this time store the result
        sta     *_divulong_tmp+3	; We have the high byte already
        txa
        sbc     *___SDCC_m6502_ret4+0	; byte 0
        tax
        lda     *_divulong_tmp+1
        sbc     *___SDCC_m6502_ret4+1
        sta     *_divulong_tmp+1	; byte 1
        lda     *_divulong_tmp+2
        sbc     *___SDCC_m6502_ret4+2
        sta     *_divulong_tmp+2 	; byte 2
        inc     *___SDCC_m6502_ret0+0	; Set result bit
L1:
	txa
        dey
        bne     L0
        sta     *_divulong_tmp+0
	lda	*___SDCC_m6502_ret0+0
	ldx	*___SDCC_m6502_ret0+1
        rts
