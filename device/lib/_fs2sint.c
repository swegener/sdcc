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

// int __fs2sint (float x)
static void dummy(void) _naked
{
	_asm
	.globl	___fs2sint
___fs2sint:
	lcall	___fs2slong
	jnb	sign_a, fs2sint_pos
fs2sint_neg:
	cpl	a
	jnz	fs2sint_maxval_neg
	mov	a, b
	cpl	a
	jnz	fs2sint_maxval_neg
	mov	a, dph
	jnb	acc.7, fs2sint_maxval_neg
	ret
fs2sint_maxval_neg:
	mov	dptr, #0x8000
	ret
fs2sint_pos:
	jnz	fs2sint_maxval_pos
	mov	a, b
	jnz	fs2sint_maxval_pos
	mov	a, dph
	jb	acc.7, fs2sint_maxval_pos
	ret
fs2sint_maxval_pos:
	mov	dptr, #0x7FFF
	ret
	_endasm;
}


#else



/* convert float to signed int */
signed int __fs2sint (float f) {
  signed long sl=__fs2slong(f);
  if (sl>=INT_MAX)
    return INT_MAX;
  if (sl<=INT_MIN) 
    return -INT_MIN;
  return sl;
}

#endif

