;--------------------------------------------------------------------------
;  _modsint.s
;
;  Copyright (C) 2014, Krzysztof Nikiel, Ben Shi
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

	.globl __modsint

	.area CODE
__modsint:
	ldw	x, (#3, sp)
	ldw	y, (#5, sp)
	ld	a, xh
	cpw	x, #0x0000
	jrsge	__modsint_1
	negw	x
__modsint_1:
	cpw	y, #0x0000
	jrsge	__modsint_2
	negw	y
__modsint_2:
	divw	x, y
	and	a, #0x80
	jreq	__modsint_3
	negw	y
__modsint_3:
	ldw	x, y
	ret
