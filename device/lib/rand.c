/*-------------------------------------------------------------------------
   rand.c - random number generator

   Written By - Maarten Brock, sourceforge.brock@dse.nl (2006)

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#include <stdlib.h>

static unsigned long int next = 1;

int rand(void)
{
    next = next * 1103515245UL + 12345;
    return (unsigned int)(next/65536) % (RAND_MAX + 1U);
}

void srand(unsigned int seed)
{
    next = seed;
}
