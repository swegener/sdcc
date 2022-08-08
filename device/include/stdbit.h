/*-------------------------------------------------------------------------
   stdbitint.h - ISO CX 7.18 Bit and byte utilitites <stdbit.h>

   Copyright (C) 2022, Philipp Klaus Krause, krauseph@informatik.uni-freiburg.de

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
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

#ifndef _STDBIT_H
#define _STDBIT_H 1

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
  typedef unsigned int size_t;
#endif

#ifndef __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED
#define __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED

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

#endif // __SPECIFIED_WIDTH_INTEGER_TYPES_DEFINED

// C2X 7.18.2 Endian

#define __STDC_ENDIAN_LITTLE__ 0
#define __STDC_ENDIAN_BIG__ 1

#if defined(__SDCC_hc08) || defined(__SDCC_s08) || defined(__SDCC_stm8)
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_BIG__
#else
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__
#endif

// C2X 7.18.6 Count Trailing Ones
int_fast8_t __stdc_count_trailing_onesull(unsigned long long value); // Todo: Use _BitInt(8) here once all ports support it, so we avoid integer promotion for some cases.
#define stdc_count_trailing_ones(value) __stdc_count_trailing_onesull(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_count_trailing_onesuc(value) ((int)(stdc_count_trailing_ones((unsigned char)(value)))
#define stdc_count_trailing_onesus(value) ((int)(stdc_count_trailing_ones((unsigned short)(value)))
#define stdc_count_trailing_onesui(value) ((int)(stdc_count_trailing_ones((unsigned int)(value)))
#define stdc_count_trailing_onesul(value) ((int)(stdc_count_trailing_ones((unsigned long)(value)))
#define stdc_count_trailing_onesull(value) ((int)(stdc_count_trailing_ones((unsigned long long)(value)))

// C2X 7.18.5 Count Trailing Zeros
#define stdc_count_trailing_zeros(value) stdc_count_trailing_ones((typeof(value))~(value))
#define stdc_count_trailing_zerosuc(value) ((int)(stdc_count_trailing_ones((unsigned char)(~(value))))
#define stdc_count_trailing_zerosus(value) ((int)(stdc_count_trailing_ones((unsigned short)(~(value))))
#define stdc_count_trailing_zerosui(value) ((int)(stdc_count_trailing_ones((unsigned int)(~(value))))
#define stdc_count_trailing_zerosul(value) ((int)(stdc_count_trailing_ones((unsigned long)(~(value))))
#define stdc_count_trailing_zerosull(value) ((int)(stdc_count_trailing_ones((unsigned long long)(~(value))))

// C2X 7.18.10 First Trailing One
int_fast8_t __stdc_first_trailing_oneull(unsigned long long value); // Todo: Use _BitInt(8) here once all ports support it, so we avoid integer promotion for some cases.
#define stdc_first_trailing_one(value) __stdc_first_trailing_oneull(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_first_trailing_oneuc(value) ((int)(stdc_first_trailing_one((unsigned char)(value)))
#define stdc_first_trailing_oneus(value) ((int)(stdc_first_trailing_one((unsigned short)(value)))
#define stdc_first_trailing_oneui(value) ((int)(stdc_first_trailing_one((unsigned int)(value)))
#define stdc_first_trailing_oneul(value) ((int)(stdc_first_trailing_one((unsigned long)(value)))
#define stdc_first_trailing_oneull(value) ((int)(stdc_first_trailing_one((unsigned long long)(value)))

// C2X 7.18.9 First Trailing Zero
#define stdc_first_trailing_zero(value) __stdc_first_trailing_oneull((unsigned long long)~(value)) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_first_trailing_zerouc(value) ((int)(stdc_first_trailing_zero((unsigned char)(value)))
#define stdc_first_trailing_zerous(value) ((int)(stdc_first_trailing_zero((unsigned short)(value)))
#define stdc_first_trailing_zeroui(value) ((int)(stdc_first_trailing_zero((unsigned int)(value)))
#define stdc_first_trailing_zeroul(value) ((int)(stdc_first_trailing_zero((unsigned long)(value)))
#define stdc_first_trailing_zeroull(value) ((int)(stdc_first_trailing_zero((unsigned long long)(value)))

// C2X 7.18.11 Count Ones
int_fast8_t __stdc_count_onesull(unsigned long long value); // Todo: Use _BitInt(8) here once all ports support it, so we avoid integer promotion for some cases.
#define stdc_count_ones(value) __stdc_count_onesull(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_count_onesuc(value) ((int)(stdc_count_ones((unsigned char)(value)))
#define stdc_count_onesus(value) ((int)(stdc_count_ones((unsigned short)(value)))
#define stdc_count_onesui(value) ((int)(stdc_count_ones((unsigned int)(value)))
#define stdc_count_onesul(value) ((int)(stdc_count_ones((unsigned long)(value)))
#define stdc_count_onesull(value) ((int)(stdc_count_ones((unsigned long long)(value)))

// C2X 7.18.12 Count Zeros
#define stdc_count_zeros(value) stdc_count_ones((typeof(value))~(value))
#define stdc_count_zerosuc(value) ((int)(stdc_count_ones((unsigned char)(~(value))))
#define stdc_count_zerosus(value) ((int)(stdc_count_ones((unsigned short)(~(value))))
#define stdc_count_zerosui(value) ((int)(stdc_count_ones((unsigned int)(~(value))))
#define stdc_count_zerosul(value) ((int)(stdc_count_ones((unsigned long)(~(value))))
#define stdc_count_zerosull(value) ((int)(stdc_count_ones((unsigned long long)(~(value))))

// C2X 7.18.13 Single-bit Check
#define stdc_has_single_bit(value) ((_Bool)(stdc_count_ones(value) == 1))
#define stdc_has_single_bituc(value) stdc_has_single_bit((unsigned char)(value))
#define stdc_has_single_bitus(value) stdc_has_single_bit((unsigned short)(value))
#define stdc_has_single_bitui(value) stdc_has_single_bit((unsigned int)(value))
#define stdc_has_single_bitul(value) stdc_has_single_bit((unsigned long)(value))
#define stdc_has_single_bitull(value) stdc_has_single_bit((unsigned long long)(value))

// C2X 7.18.14 Bit Width
int_fast8_t __stdc_bit_widthull(unsigned long long value); // Todo: Use _BitInt(8) here once all ports support it, so we avoid integer promotion for some cases.
#define stdc_bit_width(value) __stdc_bit_widthull(value) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_bit_widthuc(value) ((int)(stdc_bit_width((unsigned char)(value)))
#define stdc_bit_widthus(value) ((int)(stdc_bit_width((unsigned short)(value)))
#define stdc_bit_widthui(value) ((int)(stdc_bit_width((unsigned int)(value)))
#define stdc_bit_widthul(value) ((int)(stdc_bit_width((unsigned long)(value)))
#define stdc_bit_widthull(value) ((int)(stdc_bit_width((unsigned long long)(value)))

// C2X 7.18.15 Bit Floor
#define stdc_bit_floor(value) ((typeof(value))(1ull << stdc_bit_width(value)) >> 1)
#define stdc_bit_flooruc(value) ((unsigned char)(1 << stdc_bit_width(value)((unsigned char)(value))) >> 1)
#define stdc_bit_floorus(value) ((unsigned short)(1 << stdc_bit_width((unsigned short)(value))) >> 1)
#define stdc_bit_floorui(value) (1u << stdc_bit_width((unsigned int)(value) >> 1)
#define stdc_bit_floorul(value) (1ul << stdc_bit_width((unsigned long)(value) >> 1)
#define stdc_bit_floorull(value) (1ull << stdc_bit_width((unsigned long long)(value) >> 1)

// C2X 7.18.16 Bit Ceiling
unsigned long long __stdc_bit_ceilull(unsigned long long value);
#define stdc_bit_ceil(value) ((typeof(value))(__stdc_bit_ceilull(value))) // Todo: Use some speed-optimized variants here later. Via _Generic or sizeof.
#define stdc_bit_ceiluc(value) (stdc_bit_ceil(value)((unsigned char)(value))))
#define stdc_bit_ceilus(value) (stdc_bit_ceil(value)((unsigned short)(value))))
#define stdc_bit_ceilui(value) (stdc_bit_ceil(value)((unsigned int)(value))))
#define stdc_bit_ceilul(value) (stdc_bit_ceil(value)((unsigned long)(value))))
#define stdc_bit_ceilull(value) (stdc_bit_ceil(value)((unsigned long long)(value))))

#endif

