/*  cotf.c: Computes cot(x) where x is a 32-bit float.

    Copyright (C) 2001, 2002  Jesus Calvino-Fraga, jesusc@ieee.org 

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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA */

/* Version 1.0 - Initial release */

/*
** $Id: cotf.c 3654 2005-01-24 10:38:53Z vrokas $
*/

#include <math.h>
#include <errno.h>

float tancotf(const float x, const int iscot);

float cotf(const float x) _MATH_REENTRANT
{
    float y;

    y=fabsf(x);
    if (y<1.0E-30) //This one requires more thinking...
    {
        errno = ERANGE;
        if (x<0.0)
            return -XMAX;
        else
            return XMAX;
    }
    return tancotf(x, 1);
}

