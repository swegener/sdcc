;-------------------------------------------------------------------------
;   _floatlib.s - common code for floating point support
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

	.module _floatlib
	
;--------------------------------------------------------
; exported symbols
;--------------------------------------------------------
	.globl _float_PARM_1
	.globl ___fsadd_PARM_1
	.globl ___fssub_PARM_1
	.globl ___fsmul_PARM_1
	.globl _fabsf_PARM_1

	.globl _float_PARM_2
	.globl ___fsadd_PARM_2
	.globl ___fssub_PARM_2
	.globl ___fsmul_PARM_2
	
	.globl ___fs_ret_zero
	.globl ___fs_ret_inf
	.globl ___fs_pack_ret
	.globl ___fs_unpack_2P
	.globl ___fs_unpack_1P

;--------------------------------------------------------
; overlayable items in ram
;--------------------------------------------------------
	.area	OSEG    (PAG, OVR)
_float_PARM_1:
_fabsf_PARM_1:
___fsadd_PARM_1:
___fssub_PARM_1:
___fsmul_PARM_1:
	.ds 4
	
_float_PARM_2:
___fsadd_PARM_2:
___fssub_PARM_2:
___fsmul_PARM_2:
	.ds 4

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

___fs_unpack_2P:	
	lda	*(_float_PARM_2 + 2) ; get exp2
	rol	a
	lda	*(_float_PARM_2 + 3)
	rol	a
	sta	*exp2
	tya			; get sign2 - sign is 0x80
	ror	a
	sta	*s2

___fs_unpack_1P:
	lda	*(_float_PARM_1 + 2) ; get exp1
	rol	a
	lda	*(_float_PARM_1 + 3)
	rol	a
	sta	*exp1
	tya			; get sign1 - sign is 0x80 
	ror	a
	sta	*s1
	rts
	
___fs_ret_zero:
	lda	#0x00
	tax
	sta	*___SDCC_m6502_ret2
	sta	*___SDCC_m6502_ret3
	rts
	
___fs_ret_inf:
	ora	#0x7f		; A contains sign
	sta	*___SDCC_m6502_ret3
	lda	#0x80
	sta	*___SDCC_m6502_ret2
	lda	#0x00
	tax
	rts

___fs_pack_ret:
	lda	*exp1
	lsr	a
	ora	*s1
	sta	*___SDCC_m6502_ret3
	lda	#0x00
	ror	a
	ora	*___SDCC_m6502_ret2
	sta	*___SDCC_m6502_ret2
	ldx	*res1
	lda	*res0
	rts
