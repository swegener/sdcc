/* Floating point library in optimized assembly for 8051
 * Copyright (c) 2004, Paul Stoffregen, paul@pjrc.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */



#define SDCC_FLOAT_LIB
#include <float.h>


#ifdef FLOAT_ASM_MCS51

static void dummy(void) _naked
{
	_asm
	.globl	fs_rshift_a
fs_rshift_a:
	jz	00020$
	mov	r0, a
	add	a, exp_a	// adjust exponent
	jnc	00001$
	mov	a, #255		// don't roll over
00001$:
	mov	exp_a, a
#ifdef FLOAT_SHIFT_SPEEDUP
	mov	a, r0
	add	a, #248
	jnc	00003$
	xch	a, r4
	xch	a, r3
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r4
	//mov	r1, ar2		// avoid dependence on register bank
	//mov	r2, ar3
	//mov	r3, ar4
	//mov	r4, #0
	add	a, #248
	jnc	00003$
	xch	a, r3
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r3
	//mov	r1, ar2
	//mov	r2, ar3
	//mov	r3, #0
	add	a, #248
	jnc	00003$
	xch	a, r2
	mov	r1, a
	clr	a
	xch	a, r2
	//mov	r1, ar2
	//mov	r2, #0
	add	a, #248
	jnc	00003$
	mov	r1, #0
	ret
00003$:
	add	a, #8
	jz	00020$
	mov	r0, a
#endif
00005$:
	clr	c
	mov	a, r4
	rrc	a
	mov	r4, a
	mov	a, r3
	rrc	a
	mov	r3, a
	mov	a, r2
	rrc	a
	mov	r2, a
	mov	a, r1
	rrc	a
	mov	r1, a
	djnz	r0, 00005$
00020$:
	ret
	_endasm;
}

#endif

