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

// float long __ulong2fs (float x)
static void dummy(void) _naked
{
	_asm
	.globl	___ulong2fs
___ulong2fs:
	mov	r4, a
	mov	r3, b
	mov	r2, dph
	mov	r1, dpl
	mov	a, #158
	.globl	ulong2fs_doit
ulong2fs_doit:
	clr	sign_a
long2fs_doit:
	mov	exp_a, a
	lcall	fs_normalize_a
	ljmp	fs_round_and_return
	_endasm;
}

#else



/*
** libgcc support for software floating point.
** Copyright (C) 1991 by Pipeline Associates, Inc.  All rights reserved.
** Permission is granted to do *anything* you want with this file,
** commercial or otherwise, provided this message remains intact.  So there!
** I would appreciate receiving any updates/patches/changes that anyone
** makes, and am willing to be the repository for said changes (am I
** making a big mistake?).
**
** Pat Wood
** Pipeline Associates, Inc.
** pipeline!phw@motown.com or
** sun!pipeline!phw or
** uunet!motown!pipeline!phw
*/

/* (c)2000/2001: hacked a little by johan.knol@iduna.nl for sdcc */

union float_long
  {
    float f;
    long l;
  };

float __ulong2fs (unsigned long a )
{
  int exp = 24 + EXCESS;
  volatile union float_long fl;

  if (!a)
    {
      return 0.0;
    }

  while (a & NORM) 
    {
      // we lose accuracy here
      a >>= 1;
      exp++;
    }
  
  while (a < HIDDEN)
    {
      a <<= 1;
      exp--;
    }

#if 1
  if ((a&0x7fffff)==0x7fffff) {
    a=0;
    exp++;
  }
#endif

  a &= ~HIDDEN ;
  /* pack up and go home */
  fl.l = PACK(0,(unsigned long)exp, a);

  return (fl.f);
}

#endif

