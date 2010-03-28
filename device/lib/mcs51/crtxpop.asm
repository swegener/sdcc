;--------------------------------------------------------------------------
;  crtxpop.asm - C run-time: pop registers (not bits) from xstack
;
;  Copyright (C) 2009, Maarten Brock
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
;  might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

;--------------------------------------------------------
; overlayable bit register bank
;--------------------------------------------------------
	.area BIT_BANK	(REL,OVR,DATA)
bits:
	.ds 1

	ar0 = 0x00
	ar1 = 0x01

	.area HOME    (CODE)

; Pop registers r0..r7 from xstack
; Expect mask in B
__sdcc_xpop_regs::
	mov	a,r0
	mov	r0,_spx
	jbc	B.0,00101$	;if B(0)=0 then
	dec	r0
	movx	a,@r0		;pop R0
	dec	_spx
00101$:
	push	acc
	mov	r1,#0x08
00102$:
	mov	a,B
00103$:
	jz	00104$		;if B=0 we are done
	dec	r1
	add	a,acc		;if B(n) then
	jnc	00103$
	mov	B,a
	dec	r0		;pop Rn
	movx	a,@r0
	mov	@r1,a
	sjmp	00102$
00104$:
	mov	_spx,r0
	pop	ar0
	ret
