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

// char __fs2schar (float x)
static void dummy(void) __naked
{
	__asm
	.globl	___fs2schar
___fs2schar:
	lcall	___fs2slong
	jnz	fs2schar_not_zero
	mov	a, dpl
	orl	a, dph
	orl	a, b
	jnz	fs2schar_clr_a
	ret
fs2schar_clr_a:
	clr a
fs2schar_not_zero:
	jnb	sign_a, fs2schar_pos
fs2schar_neg:
	cpl	a
	jnz	fs2schar_maxval_neg
	mov	a, b
	cpl	a
	jnz	fs2schar_maxval_neg
	mov	a, dph
	cpl	a
	jnz	fs2schar_maxval_neg
	mov	a, dpl
	jnb	acc.7, fs2schar_maxval_neg
	ret
fs2schar_maxval_neg:
	mov	dpl, #0x80
	ret
fs2schar_pos:
	jnz	fs2schar_maxval_pos
	mov	a, b
	jnz	fs2schar_maxval_pos
	mov	a, dph
	jnz	fs2schar_maxval_pos
	mov	a, dpl
	jb	acc.7, fs2schar_maxval_pos
	ret
fs2schar_maxval_pos:
	mov	dpl, #0x7F
	ret
	__endasm;
}

#else

/* convert float to signed char */
signed char __fs2schar (float f)
{
  signed long sl=__fs2slong(f);
  if (sl>=CHAR_MAX)
    return CHAR_MAX;
  if (sl<=CHAR_MIN)
    return -CHAR_MIN;
  return sl;
}

#endif
