/*  sqrtf.c:  Computes square root of a 32-bit float as outlined in [1]

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

/* [1] William James Cody and W.  M.  Waite.  _Software manual for the
   elementary functions_, Englewood Cliffs, N.J.:Prentice-Hall, 1980. */

/* Version 1.0 - Initial release */

/*
** $Id: sqrtf.c 3654 2005-01-24 10:38:53Z vrokas $
*/

#include <float.h>
#include <math.h>
#include <errno.h>

float sqrtf(const float x) _MATH_REENTRANT
{
    float f, y;
    FS_STATIC int n;

    if (x==0.0) return x;
    else if (x==1.0) return 1.0;
    else if (x<0.0)
    {
        errno=EDOM;
        return 0.0;
    }
    f=frexpf(x, &n);
    y=0.41731+0.59016*f; /*Educated guess*/
    /*For a 24 bit mantisa (float), two iterations are sufficient*/
    y+=f/y;
    y=ldexpf(y, -2) + f/y; /*Faster version of 0.25 * y + f/y*/

    if (n&1)
    {
        y*=0.7071067812;
        ++n;
    }
    return ldexpf(y, n/2);
}
