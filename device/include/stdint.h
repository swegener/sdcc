/*-------------------------------------------------------------------------
   stdint.h - ISO C99 7.18 Integer types <stdint.h>

   Copyright (C) 2005, Maarten Brock, sourceforge.brock@dse.nl
   Copyright (C) 2011, Philipp Klaus Krause, pkk@spth.de

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef _STDINT_H
#define _STDINT_H       1

/* Exact integral types.  */

#if defined (SDCC_z80) || defined (SDCC_z180) || defined (SDCC_r2k) || defined (SDCC_gbz80)
#if __STDC_VERSION__ >= 199901L
#define __SDCC_LONGLONG
#endif
#endif

/* Signed.  */

typedef signed char             int8_t;
typedef short int               int16_t;
typedef long int                int32_t;
#ifdef __SDCC_LONGLONG
typedef long long int           int64_t;
#endif

/* Unsigned.  */
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned long int       uint32_t;
#ifdef __SDCC_LONGLONG
typedef unsigned long long int  uint64_t;
#endif

/* Small types.  */

/* Signed.  */
typedef signed char             int_least8_t;
typedef short int               int_least16_t;
typedef long int                int_least32_t;
#ifdef __SDCC_LONGLONG
typedef long long int           int_least64_t;
#endif

/* Unsigned.  */
typedef unsigned char           uint_least8_t;
typedef unsigned short int      uint_least16_t;
typedef unsigned long int       uint_least32_t;
#ifdef __SDCC_LONGLONG
typedef unsigned long long int  uint_least64_t;
#endif

/* Fast types.  */

/* Signed.  */
typedef signed char             int_fast8_t;
typedef int                     int_fast16_t;
typedef long int                int_fast32_t;
#ifdef __SDCC_LONGLONG
typedef long long int           int_fast64_t;
#endif

/* Unsigned.  */
typedef unsigned char           uint_fast8_t;
typedef unsigned int            uint_fast16_t;
typedef unsigned long int       uint_fast32_t;
#ifdef __SDCC_LONGLONG
typedef unsigned long long int  uint_fast64_t;
#endif

/* Types for `void *' pointers.  */
#if defined (SDCC_mcs51) || defined (SDCC_ds390)
  typedef long int              intptr_t;
  typedef unsigned long int     uintptr_t;
#else
  typedef int                   intptr_t;
  typedef unsigned int          uintptr_t;
#endif


/* Largest integral types.  */
typedef long int                intmax_t;
typedef unsigned long int       uintmax_t;


/* Limits of integral types.  */

/* Minimum of signed integral types.  */
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647L-1)
/* Maximum of signed integral types.  */
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647L)

/* Maximum of unsigned integral types.  */
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295UL)

/* Minimum of signed integral types having a minimum size.  */
# define INT_LEAST8_MIN         (-128)
# define INT_LEAST16_MIN        (-32767-1)
# define INT_LEAST32_MIN        (-2147483647L-1)
/* Maximum of signed integral types having a minimum size.  */
# define INT_LEAST8_MAX         (127)
# define INT_LEAST16_MAX        (32767)
# define INT_LEAST32_MAX        (2147483647L)

/* Maximum of unsigned integral types having a minimum size.  */
# define UINT_LEAST8_MAX        (255)
# define UINT_LEAST16_MAX       (65535)
# define UINT_LEAST32_MAX       (4294967295UL)

/* Minimum of fast signed integral types having a minimum size.  */
# define INT_FAST8_MIN          (-128)
# define INT_FAST16_MIN         (-32767-1)
# define INT_FAST32_MIN         (-2147483647L-1)

/* Maximum of fast signed integral types having a minimum size.  */
# define INT_FAST8_MAX          (127)
# define INT_FAST16_MAX         (32767)
# define INT_FAST32_MAX         (2147483647L)

/* Maximum of fast unsigned integral types having a minimum size.  */
# define UINT_FAST8_MAX         (255)
# define UINT_FAST16_MAX        (65535)
# define UINT_FAST32_MAX        (4294967295UL)

/* Values to test for integral types holding `void *' pointer.  */
#if defined (SDCC_mcs51) || defined (SDCC_ds390)
# define INTPTR_MIN             (-2147483647L-1)
# define INTPTR_MAX             (2147483647L)
# define UINTPTR_MAX            (4294967295UL)
#else
# define INTPTR_MIN             (-32767-1)
# define INTPTR_MAX             (32767)
# define UINTPTR_MAX            (65535)
#endif

/* Minimum for largest signed integral type.  */
# define INTMAX_MIN             (-__INT32_C(-2147483647L)-1)
/* Maximum for largest signed integral type.  */
# define INTMAX_MAX             (__INT32_C(2147483647L))

/* Maximum for largest unsigned integral type.  */
# define UINTMAX_MAX            (__UINT32_C(4294967295UL))


/* Limits of other integer types.  */

/* Limits of `ptrdiff_t' type.  */
#if defined (SDCC_mcs51) || defined (SDCC_ds390)
# define PTRDIFF_MIN           (-2147483647L-1)
# define PTRDIFF_MAX           (2147483647L)
#else
# define PTRDIFF_MIN           (-32767-1)
# define PTRDIFF_MAX           (32767)
#endif

/* Limit of `size_t' type.  */
# define SIZE_MAX               (65535)

/* Signed.  */
# define INT8_C(c)      c
# define INT16_C(c)     c
# define INT32_C(c)     c ## L
# define INT64_C(c)     c ## LL

/* Unsigned.  */
# define UINT8_C(c)     c ## U
# define UINT16_C(c)    c ## U
# define UINT32_C(c)    c ## UL
# define UINT64_C(c)    c ## ULL

/* Maximal type.  */
# define INTMAX_C(c)    c ## L
# define UINTMAX_C(c)   c ## UL


#endif /* stdint.h */
