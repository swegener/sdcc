/*
 * malloc.c - dynamic memory allocation
 *
 * written by Vangelis Rokas, 2004 (vrokas@otenet.gr)
 *
 */

#include "malloc.h"

extern char _MALLOC_SPEC *_dynamicHeap;			/* pointer to heap */

void free(unsigned char _MALLOC_SPEC *buf)
{
	/* mark block as deallocated */
	((_malloc_rec *)(buf - 1))->bits.alloc = 0;
}

