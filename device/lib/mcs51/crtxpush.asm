; /*-------------------------------------------------------------------------
;
;   crtxpush.asm :- C run-time: push registers (not R0) to xstack
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

; Push registers r1..r7 & bits on xstack
; Expect allocation size in ACC and mask in B
__sdcc_xpush_regs::
	mov	r0,_spx
	add	a,_spx
	mov	_spx,a
	push	ar1
	jbc	B.0,00100$	;if B(0)=0 then
	mov	a,bits		;push bits
	movx	@r0,a
	inc	r0
00100$:
	jbc	B.1,00101$	;if B(1)=0 then
	mov	a,r1		;push R1
	movx	@r0,a
	inc	r0
00101$:
	mov	r1,#0x01
00102$:
	mov	a,B
00103$:
	jz	00104$		;if B=0 we are done
	inc	r1
	add	a,acc		;if B(9-n)=1 then
	jnc	00103$
	mov	B,a
	mov	a,@r1		;push Rn
	movx	@r0,a
	inc	r0
	sjmp	00102$
00104$:
	pop	ar1
	ret
