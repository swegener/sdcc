/*-------------------------------------------------------------------------
   schar2fs.c :- 

   Adopted for float and pic16 port by
	- Vangelis Rokas, vrokas@otenet.gr (2004)

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

#include <float.h>

/* convert signed char to float */
float __schar2fs (signed char sc) _FS_REENTRANT
{
  return __slong2fs(sc);
}
