/*
 * malloc.c - dynamic memory allocation
 *
 * written by Vangelis Rokas, 2004 (vrokas@otenet.gr)
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

/* this is an external pointer to HEAP. It should be defined in
 * the user's program, or it can be a symbol created by linker */
extern unsigned char _MALLOC_SPEC *heap;

unsigned char _MALLOC_SPEC *malloc(unsigned char len)
{
  _malloc_rec _MALLOC_SPEC *pHeap;			/* pointer to block header */
  _malloc_rec _MALLOC_SPEC *temp;
  unsigned char bLen, eLen;			/* size of block  */
#if MALLOC_MAX_FIRST
  unsigned char pass=1;
#endif

    if(len >= MAX_BLOCK_SIZE)
      goto do_end;

    pHeap = (_malloc_rec _MALLOC_SPEC *)&heap;

    while(1) {
      bLen = pHeap->bits.count;
      
      /* if datum is zero, then last block, return NULL */
      if(pHeap->datum == 0) {
#if !MALLOC_MAX_FIRST
        goto do_end;
#else
        if(!pass)
          goto do_end;
        
        /* in the first pass, we search for blocks that have
         * the requested size, in the second pass, try to merge
         * adjacent blocks to 'make' the requested block */
        pHeap = (_malloc_rec _MALLOC_SPEC *)&heap;
        pass--;
        continue;
#endif
      }

      /* if current block is allocated then proceed to next */
      if(pHeap->bits.alloc)
        goto do_continue;

		
      /* current block is not allocated, try to allocate */
			
      /* if current block is not enough for allocation, then proceed to next */
      if(bLen <= len) {

#if MALLOC_MAX_FIRST
        /* if we are in the first pass, check next block */
        if(pass)
          goto do_continue;

        /* otherwise try merge */
#endif

        temp = _mergeHeapBlock(pHeap, len);
        
        if(!temp)
          /* otherwise proceed with next block */
          goto do_continue;

        pHeap = temp;
        bLen = pHeap->bits.count;
      }

      /* current block is enough to hold the new block */

      /* allocate by filling the fields */
      eLen = (len+1);
      pHeap->datum = 0x80 | eLen;

      if(bLen > eLen) {
        /* if current block size is greater than the requested one,
         * create a new empty block at the end of the newly allocated */
        temp = (_malloc_rec _MALLOC_SPEC *)((unsigned int)pHeap + eLen);
        temp->datum = (bLen - eLen);
      }

      return ((unsigned char _MALLOC_SPEC *)((unsigned int)pHeap + 1));

do_continue:
      pHeap = (_malloc_rec _MALLOC_SPEC *)((unsigned int)pHeap + bLen);
	//pHeap->bits.count);
    }

do_end:
  return ((unsigned char _MALLOC_SPEC *)0);

}
