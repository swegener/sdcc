/*-------------------------------------------------------------------------
  vprintf.c - formatted output conversion
 
             Written By - Martijn van Balen aed@iae.nl (1999)

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

#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

extern void putchar(const char);

#define PTR value.p 

/****************************************************************************/

typedef char _generic *ptr_t;

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
  char _generic *p;
} value_t;


static code char memory_id[] = "IXCP-";

ptr_t output_ptr;
bit   output_to_string;
bit   lower_case;
bit   lsd;

data value_t        value;

     unsigned short radix;

/****************************************************************************/

static void output_char( char c ) reentrant
{
  if (output_to_string)
  {
    *output_ptr++ = c;
  }
  else
  {
    putchar( c );
  }
}

/*--------------------------------------------------------------------------*/

static void output_digit( unsigned char n ) reentrant
{
  output_char( n <= 9 ? '0'+n : (lower_case ? n+(char)('a'-10) : n+(char)('A'-10)) );
}

/*--------------------------------------------------------------------------*/

static void output_2digits( unsigned char b ) reentrant
{
  output_digit( b>>4 );
  output_digit( b&0x0F );
}
        
/*--------------------------------------------------------------------------*/

static void calculate_digit( void )
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

/*--------------------------------------------------------------------------*/

int vsprintf (const char *buf, const char *format, va_list ap)
{
  bit            left_justify;
  bit            zero_padding;
  bit            prefix_sign;
  bit            prefix_space;
  bit            signed_argument;
  bit            char_argument;
  bit            long_argument;

  unsigned char  width;
  unsigned char  length;
  char           c;

  output_ptr = buf;
  if ( !buf )
  {
    output_to_string = 0;
  }
  else
  {
    output_to_string = 1;
  }

  while( c=*format++ )
  {
    if ( c == '%' )
    {
      left_justify    = 0;
      zero_padding    = 0;
      prefix_sign     = 0;
      prefix_space    = 0;
      signed_argument = 0;
      radix           = 0;
      char_argument   = 0;
      long_argument   = 0;
      width           = 0;

get_conversion_spec:

      c = *format++;

      if (isdigit(c))
      {
        width = 10*width + (c - '0');

	if (width == 0)
	{
	  /* first character of width is a zero */
	  zero_padding = 1;
	}
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
       	output_char( va_arg(ap,unsigned char) );
	break;

      case 'S':
	PTR = va_arg(ap,ptr_t);

	length = strlen(PTR);
	if ( ( !left_justify ) && (length < width) )
	{
	  width -= length;
	  while( width-- != 0 )
	  {
	    output_char( ' ' );
	  }
	}

        while ( *PTR )
	  output_char( *PTR++ );

	if ( left_justify && (length < width))
	{
	  width -= length;
	  while( width-- != 0 )
	  {
	    output_char( ' ' );
	  }
	}
	break;

      case 'P':
	PTR = va_arg(ap,ptr_t);

#ifdef SDCC_MODEL_FLAT24
	output_char(memory_id[(value.byte[3] > 3) ? 4 : value.byte[3]] );
	output_char(':');
	output_char('0');
	output_char('x');
	output_2digits(value.byte[2]);
	output_2digits(value.byte[1]);
	output_2digits(value.byte[0]);
#else
	output_char( memory_id[(value.byte[2] > 3) ? 4 : value.byte[2]] );
	output_char(':');
	output_char('0');
	output_char('x');
	if ((value.byte[2] != 0x00 /* DSEG */) && (value.byte[2] != 0x03 /* SSEG */))
	  output_2digits( value.byte[1] );
	output_2digits( value.byte[0] );
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

      default:
	// nothing special, just output the character
	output_char( c );
	break;
      }

      if (radix != 0)
      {
	// Apperently we have to output an integral type
        // with radix "radix"

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
	else
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

	//jwk20000814: do this at least once, e.g.: printf ("%d", (int)0);
	do {
          value.byte[4] = 0;
	  calculate_digit();

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

	  length++;
          lsd = ~lsd;
	} while( (value.byte[0] != 0) || (value.byte[1] != 0) ||
		 (value.byte[2] != 0) || (value.byte[3] != 0) );
	
	if (width == 0)
	{
	  // default width. We set it to 1 to output
          // at least one character is case the value itself
          // is zero (i.e. length==0)
	  width=1;
	}

	/* prepend spaces if needed */
	if (!zero_padding)
	{
	  while ( width > length+1 )
	  {
	    output_char( ' ' );
	    width--;
	  }
	}

        if (signed_argument) // this now means the original value was negative
        {
          output_char( '-' );
	  // adjust width to compensate for this character
	  width--;
        }
        else if (length != 0)
	{
	  // value > 0
	  if (prefix_sign)
	  {
	    output_char( '+' );
	    // adjust width to compensate for this character
	    width--;
	  }
	  else if (prefix_space)
	  {
	    output_char( ' ' );
	    // adjust width to compensate for this character
            width--;
	  }
	}

	/* prepend zeroes/spaces if needed */
	while ( width-- > length )
	{
	  output_char( zero_padding ? '0' : ' ' );
	}

	/* output the digits */
	while( length-- )
	{
          lsd = ~lsd;

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

	  output_digit( value.byte[4] );
	}
      }
    }
    else
    {
      // nothing special, just output the character
      output_char( c );
    }
  }
       
  // Copy \0 to the end of buf
  // Modified by JB 17/12/99
  if (output_to_string) output_char(0);
}

/*--------------------------------------------------------------------------*/

int vprintf (const char *format, va_list ap)
{
  return vsprintf( 0, format, ap );
}
