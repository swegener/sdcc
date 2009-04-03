; Copyright (C) 2008-2009
; Philipp Klaus Krause

; This file is part of the sdcc Library.  This library is free
; software; you can redistribute it and/or modify it under the
; terms of the GNU General Public License as published by the
; Free Software Foundation; either version 2, or (at your option)
; any later version.

; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License along
; with this library; see the file COPYING.  If not, write to the Free
; Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
; USA.

; As a special exception, you may use this file as part of a free software
; library without restriction.  Specifically, if other files instantiate
; templates or use macros or inline functions from this file, or you compile
; this file and link it with other files to produce an executable, this
; file does not by itself cause the resulting executable to be covered by
; the GNU General Public License.  This exception does not however
; invalidate any other reasons why the executable file might be covered by
; the GNU General Public License.

        .area   _CODE

	.globl _memmove

; The Z80 has the ldir and lddr instructions, which are perfect for implementing memmove().

_memmove:
	push	ix
	ld	ix, #0
	add	ix, sp
	ld	c, 8(ix)
	ld	b, 9(ix)
	ld	a, c
	or	a, b
	jr	Z, memmove_end
	ld	e, 4(ix)
	ld	d, 5(ix)
	ld	l, 6(ix)
	ld	h, 7(ix)
	ld	a, l
	sbc	hl, de	; or above cleared carry.
	ld	l, a
	ld	h, 7(ix)
	jr	NC, memmove_up
memmove_down:
	add	hl, bc
	dec	hl
	ld	a, e
	add	a, c
	ld	e, a
	ld	a, d
	adc	a, b
	ld	d, a
	dec	de
	lddr
	jr	memmove_end
memmove_up:
	ldir
memmove_end:
	ld	l, 4(ix)
	ld	h, 5(ix)
	pop	ix
	ret

