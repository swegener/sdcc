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

static void dummy(void) __naked
{
	// arg1: passed in a,b,dph,dpl
	// arg2: passed on stack
	__asm
	.globl	fsgetargs
fsgetargs:
	// extract the two inputs, placing them into:
	//      sign     exponent   mantiassa
	//      ----     --------   ---------
	//  a:  sign_a   exp_a     r4/r3/r2
	//  b:  sign_b   exp_b     r7/r6/r5
	//
	mov	r2, dpl
	mov	r3, dph
	mov	c, b.7
	rlc	a
	mov	sign_a, c
	jz	00001$
	setb	b.7
00001$:
	mov	exp_a, a
	mov	r4, b
	// now extract the 2nd parameter from the stack
	mov	a, sp
	add	a, #249
	mov	r0, a
	mov	a, @r0
	mov	r5, a
	inc	r0
	mov	a, @r0
	mov	r6, a
	inc	r0
	mov	b, @r0
	inc	r0
	mov	a, @r0
	mov	c, b.7
	rlc	a
	mov	sign_b, c
	jz	00002$
	setb	b.7
00002$:
	mov	exp_b, a
	mov	r7, b
	ret
	__endasm;
}

#endif
