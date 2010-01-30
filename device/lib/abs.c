/*-------------------------------------------------------------------------
   abs.c: computes absolute value of an integer.

   Copyright (C) 2004 - Maarten Brock, sourceforge.brock@dse.nl

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
-------------------------------------------------------------------------*/

#include <stdlib.h>

#if defined (SDCC_mcs51)

#if defined(SDCC)
 #include <sdcc-lib.h>
#endif

static void dummy(void) __naked
{
	__asm
	.globl	_abs
_abs:
	mov	a, dph
	jnb	acc.7, 00001$
	clr	c
	clr	a
	subb	a,dpl
	mov	dpl,a
	clr	a
	subb	a,dph
	mov	dph,a
00001$:
	_RETURN
	__endasm;
}

#else

int abs(int j)
{
	return (j < 0) ? -j : j;
}

#endif

//END OF MODULE
