/*
 * malloc.c - dynamic memory allocation
 *
 * written by Vangelis Rokas, 2004 (vrokas@otenet.gr)
 *
 */

#include "malloc.h"

extern unsigned char *_dynamicHeap;			/* pointer to heap */

unsigned char *malloc(unsigned char len)
{
  _malloc_rec *pHeap;			/* pointer to block header */
  _malloc_rec *temp;
  unsigned char bLen;			/* size of block  */
  unsigned char eLen;

	if(len > _MAX_HEAP_SIZE)
		return ((unsigned char *)0);

	pHeap = (_malloc_rec *)&_dynamicHeap;


	while(1) {
		bLen = pHeap->bits.count;
		
		/* if datum is zero, then last block, return NULL */
		if(pHeap->datum == 0)
			return ((unsigned char *)0);

		/* if current block is allocated then proceed to next */
		if(pHeap->bits.alloc) {
			pHeap += pHeap->bits.count;
			continue;
		}

		
		/* current block is not allocated, try to allocate */
			
		/* if current block is not enough for allocation, then proceed to next */
		if(bLen <= len) {

			/* current block is not enough see if we can merge some adjacent
			 * memory blocks to make it fit */
			temp = pHeap + pHeap->bits.count;
			eLen = bLen;
			while((temp->datum) && (!temp->bits.alloc) && (eLen < len)) {
				eLen += temp->bits.count;
				temp += temp->bits.count;
			}

			if(eLen >= len) {
			  int i;
				/* yes, there are some free blocks that can be merged, merge them... */
				
				temp = pHeap;
			    while(eLen > 0) {
					if(eLen > MAX_BLOCK_SIZE)i = MAX_BLOCK_SIZE;
					else i = eLen;

					temp->bits.count = i;
					temp->bits.alloc = 0;

					temp += i;
					eLen -= i;
				}

				bLen = pHeap->bits.count;
			} else {
				/* otherwise proceed with next block */
				pHeap += pHeap->bits.count;
				continue;
			}
		}


		/* current block is enough to hold the new block */

		/* allocate by filling the fields */
		pHeap->bits.count = len+1;
		pHeap->bits.alloc = 1;

		if(bLen > len+1) {
			/* if current block size is greater than the requested one,
			 * create a new empty block at the end of the newly allocated */
			temp = pHeap + len+1;
			temp->bits.count = bLen - len - 1;
			temp->bits.alloc = 0;
		}

		return ((unsigned char *)pHeap + 1);
	}
}
