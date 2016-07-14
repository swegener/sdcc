;--------------------------------------------------------------------------
;  memcpy.s
;
;  Copyright (C) 2016, Philipp Klaus Krause
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

; This is a partially unrolled version of memcpy(), to reduce the count overhead.

	.globl _memcpy

	.area CODE

_memcpy:

	ldw	x, (7, sp)
	jreq	end
	ldw	y, (3, sp)

	srlw	x
	jrnc even

odd:
	incw	x
	ldw	(7, sp), x
	ldw	x, (5, sp)
	jra	odd_loop

even:
	ldw	(7, sp), x
	ldw	x, (5, sp)

loop:
	ld	a, (x)
	ld	(y), a
	incw	x
	incw	y
odd_loop:
	ld	a, (x)
	ld	(y), a
	incw	x
	incw	y

	pushw	x
	ldw	x, (9, sp)
	decw	x
	ldw	(9, sp), x
	popw	x
	jrne	loop

end:
	ldw	x, (3, sp)
	ret

