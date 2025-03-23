;-------------------------------------------------------------------------
;   _strchr.s - standard C library function
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
	.module _strchr
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl _strchr_PARM_2
	.globl _strchr

;--------------------------------------------------------
; overlayable function parameters in zero page
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
_strchr_PARM_2:
	.ds 2

;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

_strchr:
	sta	*DPTR+0
	stx	*DPTR+1

	ldy	#0x00
;	ldx	#0x00
loop:
	lda	[DPTR],y
	beq	not_found
    cmp _strchr_PARM_2
    beq found
	iny
	bne	loop
	inc	*DPTR+1
;	inx
	bne	loop
not_found:
    ldx _strchr_PARM_2
    beq found
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
	
