/* ---------------------------------------------------------------------------
   _divulong.c : 32 bit division routines for pic14 devices

  	Written By	Raphael Neider <rneider AT web.de> (2005)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the 
   Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
   Boston, MA  02111-1307  USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

   $Id$
   ------------------------------------------------------------------------ */

unsigned long
_divulong (unsigned long a, unsigned long b)
{
  unsigned long result = 0;
  unsigned long mask = 0x01;

  /* prevent endless loop (division by zero exception?!?) */
  if (!b) return (unsigned long)-1;

  /* it would suffice to make b >= a, but that test is
   * more complex and will fail if a has its MSB set */
  while (!(b & (1UL << (8*sizeof(unsigned long)-1)))) {
    b <<= 1;
    mask <<= 1;
  } // while

  /* now add up the powers of two (of b) that "fit" into a */
  /* we might stop if (a == 0), but that's an additional test in every iteration... */
  while (mask) {
    if (a >= b) {
      result += mask;
      a -= b;
    } // if
    b >>= 1;
    mask >>= 1;
  } // while

  return result;
}

