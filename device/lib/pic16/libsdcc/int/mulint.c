/*-------------------------------------------------------------------------
  _mulint.c :- routine for (unsigned) int (16 bit) multiplication

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)
	     Modified for pic16 port by - Vangelis Rokas, vrokas@otenet.gr (2004)

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

/* Signed and unsigned multiplication are the same - as long as the output
   has the same precision as the input.
*/

#include <sdcc-lib.h>

union uu {
	struct { unsigned char lo,hi ;} s;
        unsigned int t;
} ;

int _mulint (int a, int b) _IL_REENTRANT
{
  union uu x;
  union uu y;
  union uu t;

    x.t = a;
    y.t = b;
    t.t = x.s.lo * y.s.lo;
    t.s.hi += (x.s.lo * y.s.hi) + (x.s.hi * y.s.lo);

  return t.t;
}
