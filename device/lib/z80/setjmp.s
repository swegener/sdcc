;--------------------------------------------------------------------------
;  abs.s
;
;  Copyright (C) 2011, Philipp Klaus Krause
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2.1, or (at your option) any
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

	.globl ___setjmp

___setjmp:
	pop	hl
	pop	iy
	push	af
	push	hl
	ld	0(iy), l
	ld	1(iy), h
	xor	a, a
	ld	l, a
	ld	h, a
	ld	hl, #0
	add	hl, sp
	ld	2(iy), l
	ld	3(iy), h
	ld	l, a
	ld	h, a
	ret

.globl _longjmp

_longjmp:
	pop	af
	pop	iy
	pop	de

	; Ensure that return value is non-zero.
	ld	a, e
	or	a, d
	jr	NZ, jump
	ld	de, #1
jump:

	ld	l, 0(iy)
	ld	h, 1(iy)
	push	hl
	ld	l, 2(iy)
	ld	h, 3(iy)
	pop	iy
	ld	sp, hl
	inc	sp
	inc	sp
	ex	de, hl
	jp	(iy)

