/*-------------------------------------------------------------------------
   memccpy.c - part of string library functions

   Written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
   
   Based on source
             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

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
#include <string.h>

void *memccpy (void *dst, void *src, char c, size_t acount) 
{
  void *ret = dst;
  char *d = dst;
  char *s = src;
	
    /*
     * copy from lower addresses to higher addresses
     */
    while (acount--) {
      if( *s == c)
        return (++s);

      *d++ = *s++;
    }

  return((void *)0x00);
}
