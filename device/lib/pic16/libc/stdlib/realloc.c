/*
 * malloc.c - dynamic memory allocation
 *
 * written by Vangelis Rokas, 2004 (vrokas@otenet.gr)
 *
 */

#include "malloc.h"

extern unsigned char _MALLOC_SPEC *_dynamicHeap;			/* pointer to heap */

unsigned char _MALLOC_SPEC *realloc(unsigned char _MALLOC_SPEC *mblock, unsigned char len)
{
  _malloc_rec _MALLOC_SPEC *pHeap;			/* pointer to block header */
  _malloc_rec _MALLOC_SPEC *temp;
  unsigned char bLen;			/* size of block  */
  unsigned char eLen;

	if(len > MAX_BLOCK_SIZE)
		return ((unsigned char _MALLOC_SPEC *)0);

	len++;		/* increase to count header too */

	pHeap = (_malloc_rec _MALLOC_SPEC *)(mblock-1);
	bLen = pHeap->bits.count;

	/* new size is same as old, return pointer */
	if(bLen == len)return (mblock);

	if(bLen > len) {
		/* new segment is smaller than the old one, that's easy! */
		pHeap->bits.count = len;
		temp = pHeap + len;
		temp->bits.alloc = 0;
		temp->bits.count = bLen - len;

		return (((unsigned char _MALLOC_SPEC *)pHeap) + 1);
	}

	/* so, new segment has size bigger than the old one
	 * we can only return a valid pointer only when after
	 * the block there is an empty block that can be merged
	 * to produce a new block of the requested size, otherwise
	 * we return NULL */

	temp = pHeap + pHeap->bits.count;
	eLen = bLen;
	while((temp->datum) && (!temp->bits.alloc) && (eLen < len)) {
		eLen += temp->bits.count;
		temp += temp->bits.count;
	}

	if(eLen >= len) {
		int i;
		/* so we found one, adjust memory blocks */
		temp = pHeap;

		temp->bits.count = len;
		eLen -= len;
		temp += len;

		while(eLen>0) {
			if(eLen > MAX_BLOCK_SIZE)i = MAX_BLOCK_SIZE;
			else i = eLen;

			temp->bits.count = i;
			temp->bits.alloc = 0;
			temp += i;
			eLen -= i;
		}

		return (((unsigned char _MALLOC_SPEC *)pHeap) + 1);
	}


	/* no could not find a valid block, return NULL */

  return ((unsigned char _MALLOC_SPEC *)0);
}
