/* ---------------------------------------------------------------------------
   _divschar.c : 8 bit division routines for pic14 devices

  	Written By	Raphael Neider, rneider@web.de (2005)

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

extern unsigned char _divuchar (unsigned char a, unsigned char b);

signed char
_divschar (signed char a, signed char b)
{
  if (a < 0) {
    /* a < 0 */
    if (b < 0)
      return _divuchar ((unsigned int)-a, (unsigned int)-b);
    else
      return -_divuchar ((unsigned int)-a, (unsigned int)b);
  } else {
    /* a >= 0 */
    if (b < 0)
      return -_divuchar ((unsigned int)a, (unsigned int)-b);
    else
      return _divuchar ((unsigned int)a, (unsigned int)b);
  }
  /* we never reach here */
}

