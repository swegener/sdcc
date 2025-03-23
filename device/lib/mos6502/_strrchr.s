;-------------------------------------------------------------------------
;   _strrchar.s - standard C library function
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
	.module _strrchr
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl _strrchr_PARM_2
	.globl _strrchr

;--------------------------------------------------------
; overlayable function parameters in zero page
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
_strrchr_PARM_2:
	.ds 2

;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

_strrchr:
	jsr _strlen
    ; strlen is in xy
    ; ptr to end of string is in DPTR+y
    lda #0x00
    cmp _strrchr_PARM_2
    beq found    
    cpy #0x00
    bne skip_zero
    cpx #0x00
    beq not_found
skip_zero:
    inx
;    inc *DPTR+1
loop:
    dey
    lda [DPTR],y
    cmp _strrchr_PARM_2
    beq found
    cpy #0x00
    bne loop
    dec *DPTR+1
    dex
    bne loop

not_found:
	lda	#0x00
	tax
	rts
	
found:
    tya
    ldx *DPTR+1
    clc
    adc *DPTR+0
    bcc no_carry
    inx
no_carry:
	rts
	
