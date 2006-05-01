/* ---------------------------------------------------------------------------
   _mulchar.c : routine for 8 bit multiplication

  	Written By	Raphael Neider <rneider AT web.de> (2005)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 2, or (at your option) any
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

   $Id$
   ------------------------------------------------------------------------ */

#pragma save
#pragma disable_warning 126 /* unreachable code */
#pragma disable_warning 116 /* left shifting more than size of object */
char
_mulchar (char a, char b)
{
  char result = 0;
  unsigned char i;

  /* check all bits in a byte */
  for (i = 0; i < 8u; i++) {
    /* check all bytes in operand (generic code, optimized by the compiler) */
    if (a & (unsigned char)0x0001u) result += b;
    if (sizeof (a) > 1 && (a & 0x00000100ul)) result += (b << 8u);
    if (sizeof (a) > 2 && (a & 0x00010000ul)) result += (b << 16u);
    if (sizeof (a) > 3 && (a & 0x01000000ul)) result += (b << 24u);
    a = ((unsigned char)a) >> 1u;
    b <<= 1u;
  } // for i

  return result;
}
#pragma restore
