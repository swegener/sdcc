/*-----------------------------------------------------------------
    printf_small.c - source file for reduced version of printf

    Modified for pic16 port, by Vangelis Rokas, 2004 (vrokas@otenet.gr)
    
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

/* This function uses function putchar() to dump a character
 * to standard output. putchar() is defined in libc18f.lib
 * as dummy function, which will be linked if no putchar()
 * function is provided by the user.
 * The user can write his own putchar() function and link it
 * with the source *BEFORE* the libc18f.lib library. This way
 * the linker will link the first function (i.e. the user's function) */

/* following formats are supported :-
   format     output type       argument-type
     %d        decimal             int
     %ld       decimal             long
     %hd       decimal             char
     %x        hexadecimal         int
     %lx       hexadecimal         long
     %hx       hexadecimal         char
     %o        octal               int
     %lo       octal               long
     %ho       octal               char
     %c        character           char
     %s        character           generic pointer
     %f        float               float
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void printf_small(char *fmt, ...) __reentrant
{
  char *ch;
  char radix;
  char flong;
  char fstr;
  char fchar;
  char ffloat;
  float flt;
  char *str;
  __data char *str1;
  long val;
  static char buffer[16];
  va_list ap ;

    ch = fmt;
    va_start(ap,fmt);

    while( *ch ) {			//for (; *fmt ; fmt++ )
        if (*ch == '%') {
            flong = fstr = fchar = ffloat = 0;
            radix = 0;
            ch++;

            if(*ch == 'l') {
              flong = 1;
              ch++;
            } else
            if(*ch == 'h') {
              fchar = 1;
              ch++;
            }
            
            if(*ch == 's')fstr = 1;
            else if(*ch == 'f')ffloat = 1;
            else if(*ch == 'd')radix = 10;
            else if(*ch == 'x')radix = 16;
            else if(*ch == 'c')radix = 0;
            else if(*ch == 'o')radix = 8;
            
            if(fstr) {
                str = va_arg(ap, char *);
                while (*str) putchar(*str++);
            } else
            if(ffloat) {
                flt = va_arg(ap, float);
                x_ftoa(flt, buffer, 32, 6);
                str1 = buffer;
                while( *str1 )str1++; str1--;
                while( *str1 == '0' )str1--; str1++;
                *str1 = 0; str1 = buffer;
                while( *str1 )putchar(*str1++);
            } else {
              if(flong)val = va_arg(ap, long);
              else
              if(fchar)val = va_arg(ap, char);
              else {
                  val = va_arg(ap, int);
              }
            
              if(radix) {
                ltoa (val, buffer, radix);

                str1 = buffer;
                while ( *str1 ) {
                  putchar ( *str1++ );
                }
              }	else
                putchar((char)val);
            }
          } else
            putchar(*ch);

        ch++;
    }
}
