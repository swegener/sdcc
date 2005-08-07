/*
 * memfree.c - return size of all unallocated heap
 *
 * written by Vangelis Rokas <vrokas AT otenet.gr> 2005
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * 
 * In other words, you are welcome to use, share and improve this program.
 * You are forbidden to forbid anyone else to use, share and improve
 * what you give them.   Help stamp out software-hoarding!  
 *
 * $Id$
 */

#include <malloc.h>

extern unsigned char _MALLOC_SPEC *heap;

unsigned int memfree(void)
{
  _malloc_rec _MALLOC_SPEC *pHeap;
  unsigned int hsize=0;
  unsigned char bLen;
  
  pHeap = (_malloc_rec _MALLOC_SPEC *)&heap;
  
  while ((bLen = pHeap->bits.count)) {
    if(!pHeap->bits.alloc)
      hsize += bLen - 1;
    
    pHeap = (_malloc_rec _MALLOC_SPEC *)((unsigned int)pHeap + bLen);
  }
  
  return (hsize);
}
