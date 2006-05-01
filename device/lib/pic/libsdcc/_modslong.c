/* ---------------------------------------------------------------------------
   _modslong.c : 32 bit modulus routines for pic14 devices

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

extern unsigned long _modulong (unsigned long a, unsigned long b);

long
_modslong (long a, long b)
{
  if (a < 0) {
    if (b < 0)
      return _modulong ((unsigned long)-a, (unsigned long)-b);
    else
      return _modulong ((unsigned long)-a, (unsigned long)b);
  } else {
    if (b < 0)
      return _modulong ((unsigned long)a, (unsigned long)-b);
    else
      return _modulong ((unsigned long)a, (unsigned long)b);
  }
  /* we never reach here */
}

