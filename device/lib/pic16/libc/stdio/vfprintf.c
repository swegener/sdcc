/*-----------------------------------------------------------------
    vfprintf.c - source file for reduced version of printf

    Modified for pic16 port, by Vangelis Rokas, 2005 (vrokas@otenet.gr)

    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1999)

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by the
    Free Software Foundation; either version 2, or (at your option) any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    In other words, you are welcome to use, share and improve this program.
    You are forbidden to forbid anyone else to use, share and improve
    what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

/*
** $Id$
*/

/* following formats are supported :-
   format     output type       argument-type
     %%        -                   -
     %u*       unsigned            *
     %b	       binary              int
     %lb       binary              long
     %hb       binary              char
     %d        decimal             int
     %ld       decimal             long
     %hd       decimal             char
     %[xX]     hexadecimal         int
     %l[xX]    hexadecimal         long
     %h[xX]    hexadecimal         char
     %o        octal               int
     %lo       octal               long
     %ho       octal               char
     %c        character           char
     %s        character           generic pointer
*/

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#if _DEBUG
extern void io_long(unsigned long);
extern void io_str(char *);
extern void io_int(unsigned int);
#endif

unsigned int vfprintf(FILE *stream, char *fmt, va_list ap)
{
  unsigned char radix;
  unsigned char flong, fstr;
  unsigned char fchar, nosign;
  unsigned char upcase;
  unsigned int count=0;
  char *str, *ch;
  __data char *str1;
  long val;
//  static char buffer[16];
  char buffer[16];


#if _DEBUG
  io_str( "vfprintf: " );
  io_long( (unsigned long)stream );
  io_long( (unsigned long)fmt );
#endif

//    va_start(ap,fmt);
  ch = fmt;

  while( *ch ) {      //for (; *fmt ; fmt++ )
    if (*ch == '%') {
      flong = 0;
      fstr = 0;
      fchar = 0;
      nosign = 0;
      radix = 0;
      upcase = 0;
      ch++;

      if(*ch == '%') {
        __stream_putchar(stream, *ch);
        ++count;
        ++ch;
        continue;
      }

      if(*ch == 'u') {
        nosign = 1;
        ch++;
      }

      if(*ch == 'l') {
        flong = 1;
        ch++;
      } else if(*ch == 'h') {
        fchar = 1;
        ch++;
      }

      if(*ch == 's')fstr = 1;
      else if(*ch == 'd')radix = 10;
      else if(*ch == 'x'){ radix = 16; upcase = 0; }
      else if(*ch == 'X'){ radix = 16; upcase = 1; }
      else if(*ch == 'c')radix = 0;
      else if(*ch == 'o')radix = 8;
      else if(*ch == 'b')radix = 2;
      else {
        __stream_putchar(stream, *ch);
        ++count;
        ++ch;
        continue;
      }

      if(fstr) {
        str = va_arg(ap, char *);
        while(*str) { __stream_putchar(stream, *str); str++; count++; }
      } else {
        val = 0;
        if(flong) {
          val = va_arg(ap, long);
#if _DEBUG
          io_long(val);
#endif
        }
        else if(fchar) {
          val = (unsigned char)va_arg(ap, int); // FIXME: SDCC passes 1-byte char varargs as 2-byte ints...
	  if (!nosign) val = (char) val; // (FIXME cont'd) sign extend if required
#if _DEBUG
          io_long(val);
#endif
        }
        else {
          val = va_arg(ap, int);
#if _DEBUG
          io_long(val);
#endif
        }

        if(radix) {
          if(nosign)
            ultoa(val, buffer, radix);
          else
            ltoa (val, buffer, radix);

          str1 = buffer;
          while( (*str1) ) {
            radix = *str1;
            if(upcase)
              radix = toupper( radix );
            __stream_putchar(stream, (unsigned char)radix);
            count++;
            str1++;
          }
        } else {
          __stream_putchar(stream, (unsigned char)val);
          count++;
        }
      }
    } else {
      __stream_putchar(stream, *ch);
      count++;
    }

    ch++;
  }

  return (count);
}
