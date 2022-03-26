;--------------------------------------------------------------------------
;  abs.s
;
;  Copyright (C) 2010, Philipp Klaus Krause
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.area   _CODE

	.globl _abs
; 12B; 44T for nonnegative arguments, 40T for negative.
_abs:
	ld	de, 2 (sp)
	ld	hl, #0
	sub	hl, de
	ret	P
	ex	de, hl
	ret

; 13B; 46T for nonnegative arguments, 42T for negative.
;_abs:
;	ld	de, 2 (sp)
;	xor	a, a
;	ld	l, a
;	ld	h, a
;	sbc	hl, de
;	ret	P
;	ex	de, hl
;	ret

; 16B; 54T for nonegative arguments, 72T for negative:
;_abs:
;	pop	de
;	pop	hl
;	push	hl
;	push	de
;	bit	7, h
;	ret	Z
;	xor	a, a
;	ld	e, a
;	ld	d, a
;	ex	de, hl
;	sbc	hl, de
;	ret

