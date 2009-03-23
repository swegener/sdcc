/*-------------------------------------------------------------------------
  limits.h - ANSI defines constants for sizes of integral types

        Adopted for the pic16 port by Vangelis Rokas
                [ vrokas at otenet.gr ] 2004

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

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

#ifndef __LIMITS_H
#define __LIMITS_H 1

#define CHAR_BIT      8    /* bits in a char */
#define SCHAR_MAX   127
#define SCHAR_MIN  -128
#define UCHAR_MAX   0xff
#define UCHAR_MIN   0
#ifdef SDCC_CHAR_UNSIGNED
#define CHAR_MAX    UCHAR_MAX
#define CHAR_MIN    UCHAR_MIN
#else
#define CHAR_MAX    SCHAR_MAX
#define CHAR_MIN    SCHAR_MIN
#endif
#define INT_MIN    -32768
#define INT_MAX     32767
#define SHRT_MAX    INT_MAX
#define SHRT_MIN    INT_MIN
#define UINT_MAX    0xffff
#define UINT_MIN    0
#define USHRT_MAX   UINT_MAX
#define USHRT_MIN   UINT_MIN
#define LONG_MIN   -2147483648
#define LONG_MAX    2147483647
#define ULONG_MAX   0xffffffff
#define ULONG_MIN   0

#endif
