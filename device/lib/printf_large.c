/*-------------------------------------------------------------------------
  printf_large.c - formatted output conversion

             Written By - Martijn van Balen aed@iae.nl (1999)
             Added %f By - johan.knol@iduna.nl (2000)
             Refactored by - Maarten Brock (2004)

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

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#ifdef SDCC_STACK_AUTO
  #ifdef ASM_ALLOWED
    /* Eventually we should get rid of ASM_ALLOWED completely as it   */
    /* prevents portability, clobbers this source and brings only 2%  */
    /* optimization. A better alternative is a completely handcrafted */
    /* asm version if needed. */
    #undef ASM_ALLOWED
  #endif
#endif

#if defined(SDCC_mcs51)
  #if defined(SDCC_STACK_AUTO)
    #if defined(SDCC_USE_XSTACK)
      #define NEAR pdata
    #else
      //strange enough "idata" doesn't work
      #define NEAR data
    #endif
  #elif defined(SDCC_MODEL_LARGE)
    #define NEAR xdata
  #else
    #define NEAR data
  #endif
#else
  #define NEAR
#endif

#if defined(__ds390)
#define USE_FLOATS 1
#endif

#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#define PTR value.ptr

#ifdef SDCC_ds390
#define NULL_STRING "<NULL>"
#define NULL_STRING_LENGTH 6
#endif

/****************************************************************************/

//typedef char * ptr_t;
#define ptr_t char *

#ifdef toupper
#undef toupper
#endif

//#define toupper(c) ((c)&=~0x20)
#define toupper(c) ((c)&=0xDF)

typedef union
{
  unsigned char  byte[5];
  long           l;
  unsigned long  ul;
  float          f;
  char           *ptr;
} value_t;

static const char memory_id[] = "IXCP-";

#ifndef SDCC_STACK_AUTO
  static BOOL lower_case;
  static pfn_outputchar output_char;
  static void* p;

  #ifdef ASM_ALLOWED
    static bool   lsd;

    /* this one NEEDS to be in data */
    static data value_t value;
  #else
    static value_t value;
  #endif
#endif

/****************************************************************************/

#ifdef SDCC_STACK_AUTO
  static void output_digit( unsigned char n, BOOL lower_case, pfn_outputchar output_char, void* p )
#else
  static void output_digit( unsigned char n )
#endif
  {
    output_char( n <= 9 ? '0'+n :
               (lower_case ? n+(char)('a'-10) : n+(char)('A'-10)), p );
  }

/*--------------------------------------------------------------------------*/

#ifdef SDCC_STACK_AUTO
  #define OUTPUT_2DIGITS( B )	output_2digits( B, lower_case, output_char, p )
  static void output_2digits( unsigned char b, BOOL lower_case, pfn_outputchar output_char, void* p )
  {
    output_digit( b>>4,   lower_case, output_char, p );
    output_digit( b&0x0F, lower_case, output_char, p );
  }
#else
  #define OUTPUT_2DIGITS( B )	output_2digits( B )
  static void output_2digits( unsigned char b )
  {
    output_digit( b>>4   );
    output_digit( b&0x0F );
  }
#endif

/*--------------------------------------------------------------------------*/

#if defined ASM_ALLOWED
static void calculate_digit( unsigned char radix )
{
  unsigned char i;

  for( i = 32; i != 0; i-- )
  {
_asm
  clr  c
  mov  a,_value+0
  rlc  a
  mov  _value+0,a
  mov  a,_value+1
  rlc  a
  mov  _value+1,a
  mov  a,_value+2
  rlc  a
  mov  _value+2,a
  mov  a,_value+3
  rlc  a
  mov  _value+3,a
  mov  a,_value+4
  rlc  a
  mov  _value+4,a
_endasm;
    if (radix <= value.byte[4] )
    {
      value.byte[4] -= radix;
      value.byte[0]++;
    }
  }
}
#elif defined SDCC_STACK_AUTO
static void calculate_digit( value_t* value, unsigned char radix )
{
  unsigned char i;

  for( i = 32; i != 0; i-- )
  {
    value->byte[4] = (value->byte[4] << 1) | ((value->ul >> 31) & 0x01);
    value->ul <<= 1;

    if (radix <= value->byte[4] )
    {
      value->byte[4] -= radix;
      value->byte[0]++;
    }
  }
}
#else
static void calculate_digit( unsigned char radix )
{
  unsigned char i;

  for( i = 32; i != 0; i-- )
  {
    value.byte[4] = (value.byte[4] << 1) | ((value.ul >> 31) & 0x01);
    value.ul <<= 1;

    if (radix <= value.byte[4] )
    {
      value.byte[4] -= radix;
      value.byte[0]++;
    }
  }
}
#endif

#if USE_FLOATS

/* This is a very inefficient but direct approach, since we have no math
   library yet (e.g. log()).
   It does most of the modifiers, but has some restrictions. E.g. the
   abs(float) shouldn't be bigger than an unsigned long (that's
   about 4294967295), but still makes it usefull for most real-life
   applications.
*/

#define DEFAULT_FLOAT_PRECISION 6

#ifdef SDCC_STACK_AUTO
#define OUTPUT_FLOAT(F, W, D, L, Z, S, P)	output_float(F, W, D, L, Z, S, P, output_char, p)
static int output_float (float f, unsigned char reqWidth,
                         signed char reqDecimals,
                         BOOL left, BOOL zero, BOOL sign, BOOL space,
                         pfn_outputchar output_char, void* p)
#else
#define OUTPUT_FLOAT(F, W, D, L, Z, S, P)	output_float(F, W, D, L, Z, S, P)
static int output_float (float f, unsigned char reqWidth,
                         signed char reqDecimals,
                         BOOL left, BOOL zero, BOOL sign, BOOL space)
#endif
{
  char negative=0;
  unsigned long integerPart;
  float decimalPart;
  char fpBuffer[128];
  char fpBI=0, fpBD;
  unsigned char minWidth, i;
  int charsOutputted=0;

  // save the sign
  if (f<0) {
    negative=1;
    f=-f;
  }

  if (f>0x00ffffff) {
    // this part is from Frank van der Hulst
    signed char exp;

    for (exp = 0; f >= 10.0; exp++) f /=10.0;
    for (       ; f < 1.0;   exp--) f *=10.0;

    if (negative) {
      output_char ('-', p);
      charsOutputted++;
    } else {
      if (sign) {
        output_char ('+', p);
        charsOutputted++;
      }
    }
    charsOutputted += OUTPUT_FLOAT(f, 0, reqDecimals, 0, 0, 0, 0);
    output_char ('e', p);
    charsOutputted++;
    if (exp<0) {
      output_char ('-', p);
      charsOutputted++;
      exp = -exp;
    }
    output_char ('0'+exp/10, p);
    output_char ('0'+exp%10, p);
    charsOutputted += 2;
    return charsOutputted;
  }

  // split the float
  integerPart=f;
  decimalPart=f-integerPart;

  // fill the buffer with the integerPart (in reversed order!)
  while (integerPart) {
    fpBuffer[fpBI++]='0' + integerPart%10;
    integerPart /= 10;
  }
  if (!fpBI) {
    // we need at least a 0
    fpBuffer[fpBI++]='0';
  }

  // display some decimals as default
  if (reqDecimals==-1)
    reqDecimals=DEFAULT_FLOAT_PRECISION;

  // fill buffer with the decimalPart (in normal order)
  fpBD=fpBI;
  if (i=reqDecimals /* that's an assignment */) {
    do {
      decimalPart *= 10.0;
      // truncate the float
      integerPart=decimalPart;
      fpBuffer[fpBD++]='0' + integerPart;
      decimalPart-=integerPart;
    } while (--i);
  }

  minWidth=fpBI; // we need at least these
  minWidth+=reqDecimals?reqDecimals+1:0; // maybe these
  if (negative || sign || space)
    minWidth++; // and maybe even this :)

  if (!left && reqWidth>i) {
    if (zero) {
      if (negative)
      {
        output_char('-', p);
        charsOutputted++;
      }
      else if (sign)
      {
        output_char('+', p);
        charsOutputted++;
      }
      else if (space)
      {
        output_char(' ', p);
        charsOutputted++;
      }
      while (reqWidth-->minWidth)
      {
        output_char('0', p);
        charsOutputted++;
      }
    } else {
      while (reqWidth-->minWidth)
      {
        output_char(' ', p);
        charsOutputted++;
      }
      if (negative)
      {
        output_char('-', p);
        charsOutputted++;
      }
      else if (sign)
      {
        output_char('+', p);
        charsOutputted++;
      }
      else if (space)
      {
        output_char(' ', p);
        charsOutputted++;
      }
    }
  } else {
    if (negative)
    {
      output_char('-', p);
      charsOutputted++;
    }
    else if (sign)
    {
      output_char('+', p);
      charsOutputted++;
    }
    else if (space)
    {
      output_char(' ', p);
      charsOutputted++;
    }
  }

  // output the integer part
  i=fpBI-1;
  do {
    output_char (fpBuffer[i], p);
    charsOutputted++;
  } while (i--);

  // ouput the decimal part
  if (reqDecimals) {
    output_char ('.', p);
    charsOutputted++;
    i=fpBI;
    while (reqDecimals--)
    {
      output_char (fpBuffer[i++], p);
      charsOutputted++;
    }
  }

  if (left && reqWidth>minWidth) {
    while (reqWidth-->minWidth)
    {
      output_char(' ', p);
      charsOutputted++;
    }
  }
  return charsOutputted;
}
#endif

int _print_format (pfn_outputchar pfn, void* pvoid, const char *format, va_list ap)
{
  BOOL   left_justify;
  BOOL   zero_padding;
  BOOL   prefix_sign;
  BOOL   prefix_space;
  BOOL   signed_argument;
  BOOL   char_argument;
  BOOL   long_argument;
  BOOL   float_argument;
#ifdef SDCC_STACK_AUTO
  BOOL   lower_case;
  value_t value;
#endif
#ifndef ASM_ALLOWED
  BOOL   lsd;
#endif

  unsigned char radix;
  int charsOutputted;
  unsigned char  width;
  signed char decimals;
  unsigned char  length;
  char           c;

#ifdef SDCC_STACK_AUTO
  #define output_char   pfn
  #define p             pvoid
#else
  output_char = pfn;
  p = pvoid;
#endif

  // reset output chars
  charsOutputted=0;

#ifdef SDCC_ds390
  if (format==0) {
    format=NULL_STRING;
  }
#endif

  while( c=*format++ )
  {
    if ( c=='%' )
    {
      left_justify    = 0;
      zero_padding    = 0;
      prefix_sign     = 0;
      prefix_space    = 0;
      signed_argument = 0;
      radix           = 0;
      char_argument   = 0;
      long_argument   = 0;
      float_argument  = 0;
      width           = 0;
      decimals        = -1;

get_conversion_spec:

      c = *format++;

      if (c=='%') {
        output_char(c, p);
        charsOutputted++;
        continue;
      }

      if (isdigit(c)) {
        if (decimals==-1) {
          width = 10*width + (c - '0');
          if (width == 0) {
            /* first character of width is a zero */
            zero_padding = 1;
          }
        } else {
          decimals = 10*decimals + (c-'0');
        }
        goto get_conversion_spec;
      }

      if (c=='.') {
        if (decimals=-1) decimals=0;
        else
          ; // duplicate, ignore
        goto get_conversion_spec;
      }

      lower_case = islower(c);
      if (lower_case)
      {
        c = toupper(c);
      }

      switch( c )
      {
      case '-':
        left_justify = 1;
        goto get_conversion_spec;
      case '+':
        prefix_sign = 1;
        goto get_conversion_spec;
      case ' ':
        prefix_space = 1;
        goto get_conversion_spec;
      case 'B':
        char_argument = 1;
        goto get_conversion_spec;
      case 'L':
        long_argument = 1;
        goto get_conversion_spec;

      case 'C':
        output_char( va_arg(ap,int), p );
        charsOutputted++;
        break;

      case 'S':
        PTR = va_arg(ap,ptr_t);

#ifdef SDCC_ds390
        if (PTR==0) {
          PTR=NULL_STRING;
          length=NULL_STRING_LENGTH;
        } else {
          length = strlen(PTR);
        }
#else
        length = strlen(PTR);
#endif
        if ( ( !left_justify ) && (length < width) )
        {
          width -= length;
          while( width-- != 0 )
          {
            output_char( ' ', p );
            charsOutputted++;
          }
        }

        while ( *PTR )
        {
          output_char( *PTR++, p );
          charsOutputted++;
        }

        if ( left_justify && (length < width))
        {
          width -= length;
          while( width-- != 0 )
          {
            output_char( ' ', p );
            charsOutputted++;
          }
        }
        break;

      case 'P':
        PTR = va_arg(ap,ptr_t);

#ifdef SDCC_ds390
        output_char(memory_id[(value.byte[3] > 3) ? 4 : value.byte[3]], p );
        output_char(':', p);
        output_char('0', p);
        output_char('x', p);
        OUTPUT_2DIGITS( value.byte[2] );
        OUTPUT_2DIGITS( value.byte[1] );
        OUTPUT_2DIGITS( value.byte[0] );
        charsOutputted += 10;
#else
        output_char( memory_id[(value.byte[2] > 3) ? 4 : value.byte[2]], p );
        output_char(':', p);
        output_char('0', p);
        output_char('x', p);
        if ((value.byte[2] != 0x00 /* DSEG */) &&
            (value.byte[2] != 0x03 /* SSEG */))
        {
          OUTPUT_2DIGITS( value.byte[1] );
          charsOutputted += 2;
        }
        OUTPUT_2DIGITS( value.byte[0] );
        charsOutputted += 6;
#endif
        break;

      case 'D':
      case 'I':
        signed_argument = 1;
        radix = 10;
        break;

      case 'O':
        radix = 8;
        break;

      case 'U':
        radix = 10;
        break;

      case 'X':
        radix = 16;
        break;

      case 'F':
        float_argument=1;
        break;

      default:
        // nothing special, just output the character
        output_char( c, p );
        charsOutputted++;
        break;
      }

      if (float_argument) {
        value.f=va_arg(ap,float);
#if !USE_FLOATS
        PTR="<NO FLOAT>";
        while (c=*PTR++)
        {
          output_char (c, p);
          charsOutputted++;
        }
        // treat as long hex
        //radix=16;
        //long_argument=1;
        //zero_padding=1;
        //width=8;
#else
        // ignore b and l conversion spec for now
        charsOutputted += OUTPUT_FLOAT(value.f, width, decimals, left_justify,
                                     zero_padding, prefix_sign, prefix_space);
#endif
      } else if (radix != 0)
      {
        // Apperently we have to output an integral type
        // with radix "radix"
#ifndef ASM_ALLOWED
        unsigned char store[6];
        unsigned char NEAR *pstore = &store[5];
#endif

        // store value in byte[0] (LSB) ... byte[3] (MSB)
        if (char_argument)
        {
          value.l = va_arg(ap,char);
          if (!signed_argument)
          {
            value.byte[1] = 0x00;
            value.byte[2] = 0x00;
            value.byte[3] = 0x00;
          }
        }
        else if (long_argument)
        {
          value.l = va_arg(ap,long);
        }
        else // must be int
        {
          value.l = va_arg(ap,int);
          if (!signed_argument)
          {
            value.byte[2] = 0x00;
            value.byte[3] = 0x00;
          }
        }

        if ( signed_argument )
        {
          if (value.l < 0)
            value.l = -value.l;
          else
            signed_argument = 0;
        }

        length=0;
        lsd = 1;

        do {
          value.byte[4] = 0;
#if defined SDCC_STACK_AUTO
          calculate_digit(&value, radix);
#else
          calculate_digit(radix);
#endif
#if defined ASM_ALLOWED
_asm
  jb   _lsd,1$
  pop  b                ; b = <lsd>
  mov  a,_value+4       ; a = <msd>
  swap a
  orl  b,a              ; b = <msd><lsd>
  push b
  sjmp 2$
1$:
  mov  a,_value+4       ; a = <lsd>
  push acc
2$:
_endasm;
#else
          if (!lsd)
          {
            *pstore = (value.byte[4] << 4) | (value.byte[4] >> 4) | *pstore;
            pstore--;
          }
          else
          {
            *pstore = value.byte[4];
          }
#endif
          length++;
          lsd = !lsd;
        } while( (value.byte[0] != 0) || (value.byte[1] != 0) ||
                 (value.byte[2] != 0) || (value.byte[3] != 0) );

        if (width == 0)
        {
          // default width. We set it to 1 to output
          // at least one character in case the value itself
          // is zero (i.e. length==0)
          width=1;
        }

        /* prepend spaces if needed */
        if (!zero_padding && !left_justify)
        {
          while ( width > (unsigned char) (length+1) )
          {
            output_char( ' ', p );
            charsOutputted++;
            width--;
          }
        }

        if (signed_argument) // this now means the original value was negative
        {
          output_char( '-', p );
          charsOutputted++;
          // adjust width to compensate for this character
          width--;
        }
        else if (length != 0)
        {
          // value > 0
          if (prefix_sign)
          {
            output_char( '+', p );
            charsOutputted++;
            // adjust width to compensate for this character
            width--;
          }
          else if (prefix_space)
          {
            output_char( ' ', p );
            charsOutputted++;
            // adjust width to compensate for this character
            width--;
          }
        }

        /* prepend zeroes/spaces if needed */
        if (!left_justify)
          while ( width-- > length )
          {
            output_char( zero_padding ? '0' : ' ', p );
            charsOutputted++;
          }
        else
        {
          /* spaces are appended after the digits */
          if (width > length)
            width -= length;
          else
            width = 0;
        }

        /* output the digits */
        while( length-- )
        {
          lsd = !lsd;
#ifdef ASM_ALLOWED
_asm
  jb   _lsd,3$
  pop  acc              ; a = <msd><lsd>
  nop                   ; to disable the "optimizer"
  push acc
  swap a
  anl  a,#0x0F          ; a = <msd>
  sjmp 4$
3$:
  pop  acc
  anl  a,#0x0F          ; a = <lsd>
4$:
  mov  _value+4,a
_endasm;
#else
          if (!lsd)
          {
            pstore++;
            value.byte[4] = *pstore >> 4;
          }
          else
          {
            value.byte[4] = *pstore & 0x0F;
          }
#endif
#ifdef SDCC_STACK_AUTO
          output_digit( value.byte[4], lower_case, output_char, p );
#else
          output_digit( value.byte[4] );
#endif
          charsOutputted++;
        }
        if (left_justify)
          while (width-- > 0)
          {
            output_char(' ', p);
            charsOutputted++;
          }
      }
    }
    else
    {
      // nothing special, just output the character
      output_char( c, p );
      charsOutputted++;
    }
  }

  return charsOutputted;
}

/****************************************************************************/
