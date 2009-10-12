; /*-------------------------------------------------------------------------
;
;   crtxpop.asm :- C run-time: pop registers (not bits) from xstack
;
;    This library is free software; you can redistribute it and/or modify it
;    under the terms of the GNU Library General Public License as published by the
;    Free Software Foundation; either version 2, or (at your option) any
;    later version.
;
;    This library is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU Library General Public License for more details.
;
;    You should have received a copy of the GNU Library General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;
;    In other words, you are welcome to use, share and improve this program.
;    You are forbidden to forbid anyone else to use, share and improve
;    what you give them.   Help stamp out software-hoarding!
; -------------------------------------------------------------------------*/

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
