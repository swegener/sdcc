/*-------------------------------------------------------------------------
  sprintf.c - formatted output conversion

             Written By - Martijn van Balen aed@iae.nl (1999)
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

#include <stdarg.h>
#include <stdio.h>

static void put_char_to_string( char c, void* p ) _REENTRANT
{
  char **buf = (char **)p;
  *(*buf)++ = c;
}

int vsprintf (char *buf, const char *format, va_list ap)
{
  int i;
  i = _print_format( put_char_to_string, &buf, format, ap );
  *buf = 0;
  return i;
}

int sprintf (char *buf, const char *format, ...)
{
  va_list arg;
  int i;

  va_start (arg, format);
  i = _print_format( put_char_to_string, &buf, format, arg );
  *buf = 0;
  va_end (arg);

  return i;
}
