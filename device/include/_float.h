/*-------------------------------------------------------------------------
  Include file for floating point routines.
  
   Written By - Johan Knol, johan.knol@iduna.nl
    
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

#ifndef _FLOAT_H
#define _FLOAT_H

// some usefull constants
#define UCHAR_MAX 255
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UINT_MAX 65535
#define SINT_MIN (-32768)
#define SINT_MAX 32767
#define ULONG_MAX 4294967295
#define SLONG_MIN (-2147483648)
#define SLONG_MAX 2147483647

#define M_E 2.7182818284590452354 /* e */
#define M_LOG2E 1.4426950408889634074 /* log_2 e */
#define M_LOG10E 0.43429448190325182765 /* log_10 e */
#define M_LN2 0.69314718055994530942 /* log_e 2 */
#define M_LN10 2.30258509299404568402 /* log_e 10 */
#define M_PI 3.14159265358979323846 /* pi */
#define M_PI_2 1.57079632679489661923 /* pi/2 */
#define M_PI_4 0.78539816339744830962 /* pi/4 */
#define M_1_PI 0.31830988618379067154 /* 1/pi */
#define M_2_PI 0.63661977236758134308 /* 2/pi */
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#define M_SQRT2 1.41421356237309504880 /* sqrt(2) */
#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */

// the conversion routines (internal declares )
#if 0
unsigned char __fs2uchar(float);
unsigned int __fs2uint(float);
unsigned long __fs2ulong(float);
signed char __fs2schar(float);
signed int __fs2sint(float);
signed long __fs2slong(float);
float __uchar2fs(unsigned char);
float __uint2fs(unsigned int);
float __ulong2fs(unsigned long);
float __schar2fs(signed char);
float __sint2fs(signed int);
float __slong2fs(signed long);

// the math routines
float __fsadd(float, float);
float __fssub(float, float);
float __fsmul(float, float);
float __fsdiv(float, float);
float __fsmod(float, float);

// the logic routines
float __fseq(float, float);
float __fsneq(float, float);
float __fslt(float, float);
float __fslteq(float, float);
float __fsgt(float, float);
float __fsgteq(float, float);
#endif

//float _fssqrt(float);
//float _fslog(float);
//float _fsexp(float);
//float _fssin(float);
//float _fscos(float);
//float _fstan(float);

#endif _FLOAT_H
