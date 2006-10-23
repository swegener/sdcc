; /*-------------------------------------------------------------------------
; 
;   crtxclear.asm :- C run-time: clear XSEG
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

	.area CSEG    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	
	.area GSINIT4 (CODE)

__mcs51_genXRAMCLEAR::
	mov	r0,#l_PSEG
	mov	a,r0
	jz	00005$
	mov	r1,#s_PSEG
	mov	__XPAGE,#(s_PSEG >> 8)
	clr     a
00004$:	movx	@r1,a
	inc	r1
	djnz	r0,00004$
00005$:
	mov	r0,#l_XSEG
	mov	a,r0
	orl	a,#(l_XSEG >> 8)
	jz	00007$
	mov	r1,#((l_XSEG + 255) >> 8)
	mov	dptr,#s_XSEG
	clr     a
00006$:	movx	@dptr,a
	inc	dptr
	djnz	r0,00006$
	djnz	r1,00006$
00007$:

