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
	.globl	fs_swap_a_b
fs_swap_a_b:
	mov	a, exp_a
	xch	a, exp_b
	mov	exp_a, a
	// is there a faster way to swap these 2 bits???
	// this trick with psw can play havoc with the resiter bank setting
	// mov	a, psw
	// swap	a		// depends on sign bits in psw.1 & psw.5
	// mov	psw, a
	 mov	c, sign_a
	 rlc	a
	 mov	c, sign_b
	 mov	sign_a, c
	 rrc	a
	 mov	sign_b, c
	mov	a, r4
	xch	a, r7
	mov	r4, a
	mov	a, r3
	xch	a, r6
	mov	r3, a
	mov	a, r2
	xch	a, r5
	mov	r2, a
	ret
	_endasm;
}

#endif

