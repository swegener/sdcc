/*-------------------------------------------------------------------------
  stdint.h - ISO C99 7.18 Integer types <stdint.h>

             Written By -  Maarten Brock, sourceforge.brock@dse.nl (2005)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
-------------------------------------------------------------------------*/

#ifndef _STDINT_H
#define _STDINT_H       1

/* Exact integral types.  */

/* Signed.  */

typedef signed char             int8_t;
typedef short int               int16_t;
typedef long int                int32_t;

/* Unsigned.  */
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned long int       uint32_t;


/* Small types.  */

/* Signed.  */
typedef signed char             int_least8_t;
typedef short int               int_least16_t;
typedef long int                int_least32_t;

/* Unsigned.  */
typedef unsigned char           uint_least8_t;
typedef unsigned short int      uint_least16_t;
typedef unsigned long int       uint_least32_t;


/* Fast types.  */

/* Signed.  */
typedef signed char             int_fast8_t;
typedef int                     int_fast16_t;
typedef long int                int_fast32_t;

/* Unsigned.  */
typedef unsigned char           uint_fast8_t;
typedef unsigned int            uint_fast16_t;
typedef unsigned long int       uint_fast32_t;


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

/* Unsigned.  */
# define UINT8_C(c)     c ## U
# define UINT16_C(c)    c ## U
# define UINT32_C(c)    c ## UL

/* Maximal type.  */
# define INTMAX_C(c)    c ## L
# define UINTMAX_C(c)   c ## UL


#endif /* stdint.h */
