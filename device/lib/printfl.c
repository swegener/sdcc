/*-----------------------------------------------------------------
    printfl.c - source file for reduced version of printf

    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1999)

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
    
    2001060401: Improved by was@icb.snz.chel.su
-------------------------------------------------------------------------*/


/* following formats are supported :-
   format     output type       argument-type
     %d        decimal             int
     %ld       decimal             long
     %hd       decimal             short/char
     %x        hexadecimal         int
     %lx       hexadecimal         long
     %hx       hexadecimal         short/char
     %o        octal               int
     %lo       octal               long
     %ho       octal               short/char
     %c        character           char/short
     %s        character           _generic pointer
*/

#include <stdarg.h>
#include <stdio.h>
#ifndef __ds390
#include <reg51.h> // for the SP
#endif

static data volatile char ch;
static data char radix ;
static bit  long_flag = 0;
static bit  string_flag =0;
static bit  short_flag = 0;
static bit sign;
static char * data str ;
static data long val;

static void pval(void)
{
        char sp;
        unsigned long lval;
        sp = SP;

        if (val < 0 && radix != 16)
        {
           lval = -val;
           sign = 1;
        }
        else { sign = 0; lval = val;}

	if (!long_flag) {
	  lval &= 0x0000ffff;
	}
        if (short_flag) {
	  lval &= 0x000000ff;
	}

        do
        {
#if 1
                if(radix != 16)  ch = (lval % radix) + '0';
                else ch = "0123456789ABCDEF"[(unsigned short)lval & 0x0f];
                _asm push _ch _endasm;
                lval /= radix;
#else
		// This only looks more efficient, but isn't. see the .map
                ch = (lval % radix) + '0'; 
                if (ch>'9') ch+=7; 
                _asm push _ch _endasm; 
                lval /= radix;
#endif
        }
        while (lval);

        if (sign) {
                ch = '-';
                _asm push _ch _endasm;
        }

        while (sp != SP) {
                _asm pop _ch _endasm;
                putchar(ch);
        }
}

void printf_small (char * fmt, ... ) reentrant
{
    va_list ap ;

    va_start(ap,fmt);

    for (; *fmt ; fmt++ ) {
        if (*fmt == '%') {
            long_flag = string_flag = short_flag = 0;
            fmt++ ;
            switch (*fmt) {
            case 'l':
                long_flag = 1;
                fmt++;
                break;
            case 'h':
                short_flag = 1;
                fmt++;
            }

            switch (*fmt) {
            case 's':
                string_flag = 1;
                break;
            case 'd':
                radix = 10;
                break;
            case 'x':
                radix = 16;
                break;
            case 'c':
                radix = 0;
                break;
            case 'o':
                radix = 8;
                break;
            }

            if (string_flag) {
                str = va_arg(ap, char _generic *);
                while (*str) putchar(*str++);
                continue ;
            }

            if (long_flag)
                val = va_arg(ap,long);
            else
                if (short_flag)
                    val = va_arg(ap,short);
                else
                    val = va_arg(ap,int);

            if (radix) pval();
            else putchar((char)val);

        } else
            putchar(*fmt);
    }
}
