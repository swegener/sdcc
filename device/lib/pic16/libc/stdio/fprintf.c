/*-----------------------------------------------------------------
    fprintf.c - 

    Written for pic16 port, by Vangelis Rokas, 2005 (vrokas@otenet.gr)
    
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

#include <stdarg.h>
#include <stdio.h>


#if _DEBUG
extern void io_long(unsigned long);
extern void io_str(char *);
#endif

unsigned int fprintf(FILE *stream, char *fmt, ...)
{
  unsigned int i;
  va_list ap;
  
#if _DEBUG
    io_str( "fprintf: " );
    io_long((unsigned long)stream);
    io_long((unsigned long)fmt);
#endif

    va_start(ap, fmt);
    i = vfprintf(stream, fmt, ap);
    
  return (i);
}
