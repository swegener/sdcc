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

// long __fs2slong (float x)
static void dummy(void) _naked
{
	_asm
	.globl	___fs2slong
___fs2slong:
	lcall	fsgetarg
	clr	c
	mov	a, #158
	subb	a, exp_a
	jc	fs2slong_maxval		//  |x| >= 2^32
fs2slong_int_ok:
	mov	r1, #0
	lcall	fs_rshift_a
	jnb	sign_a, fs2slong_pos
fs2slong_neg:
	mov	a, r1
	cpl	a
	add	a, #1
	mov	dpl, a
	mov	a, r2
	cpl	a
	addc	a, #0
	mov	dph, a
	mov	a, r3
	cpl	a
	addc	a, #0
	mov	b, a
	mov	a, r4
	cpl	a
	addc	a, #0
	jnb	acc.7, fs2slong_maxval_neg  // x < -0x80000000
	ret
fs2slong_pos:
	mov	a, r4
	jb	acc.7, fs2slong_maxval_pos  //  x > 0x7FFFFFFF
	mov	dpl, r1
	mov	dph, r2
	mov	b, r3
	ret
fs2slong_maxval:
	jnb	sign_a, fs2slong_maxval_pos
fs2slong_maxval_neg:
	clr	a
	mov	dpl, a
	mov	dph, a
	mov	b, a
	ret
fs2slong_maxval_pos:
	mov	a, #0xFF
	mov	dpl, a
	mov	dph, a
	mov	b, a
	mov	a, #0x7F
	ret
	_endasm;
}

#else


/* convert float to signed long */
signed long __fs2slong (float f) {

  if (!f)
    return 0;

  if (f<0) {
    return -__fs2ulong(-f);
  } else {
    return __fs2ulong(f);
  }
}

#endif
