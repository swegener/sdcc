/*-------------------------------------------------------------------------
  float.h - ANSI functions forward declarations
 
	Adopted for pic16 port library by Vangelis Rokas
		[vrokas at otenet.gr] (2004)

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#ifndef __FLOAT_H
#define __FLOAT_H 1

#include <limits.h>

#define FLT_RADIX       2
#define FLT_MANT_DIG    24
#define FLT_EPSILON     1.192092896E-07F
#define FLT_DIG         6
#define FLT_MIN_EXP     (-125)
#define FLT_MIN         1.175494351E-38F
#define FLT_MIN_10_EXP  (-37)
#define FLT_MAX_EXP     (+128)
#define FLT_MAX         3.402823466E+38F
#define FLT_MAX_10_EXP  (+38)

/* the following deal with IEEE single-precision numbers */
#define EXCESS		126
#define SIGNBIT		((unsigned long)0x80000000)
#define HIDDEN		(unsigned long)(1ul << 23)
#define SIGN(fp)	(((unsigned long)(fp) >> (8*sizeof(fp)-1)) & 1)
#define EXP(fp)		(((unsigned long)(fp) >> 23) & (unsigned int) 0x00FF)
#define MANT(fp)	(((fp) & (unsigned long)0x007FFFFF) | HIDDEN)
#define NORM            0xff000000
#define PACK(s,e,m)	((s) | ((unsigned long)(e) << 23) | (m))


#if 0
#define reentrant

float __uchar2fs (unsigned char) reentrant;
float __schar2fs (signed char) reentrant;
float __uint2fs (unsigned int) reentrant;
float __sint2fs (signed int) reentrant;
float __ulong2fs (unsigned long) reentrant;
float __slong2fs (signed long) reentrant;
unsigned char __fs2uchar (float) reentrant;
signed char __fs2schar (float) reentrant;
unsigned int __fs2uint (float) reentrant;
signed int __fs2sint (float) reentrant;
unsigned long __fs2ulong (float) reentrant;
signed long __fs2slong (float) reentrant;

float __fsadd (float, float) reentrant;
float __fssub (float, float) reentrant;
float __fsmul (float, float) reentrant;
float __fsdiv (float, float) reentrant;

char __fslt (float, float) reentrant;
char __fseq (float, float) reentrant;
char __fsgt (float, float) reentrant;

#endif

#endif





