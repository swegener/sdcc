/*
 * malloc.h - dynamic memory allocation header
 *
 * written by Vangelis Rokas, 2004 (vrokas@otenet.gr)
 *
 */


/*
 * Structure of memory block header:
 * bit 7 (MSB): allocated flag
 * bits 0-6: pointer to next block (max length: 126)
 *
 */


#ifndef __MALLOC_H__
#define __MALLOC_H__

#pragma library c

#define EMULATION	0

#if EMULATION
#define malloc	pic16_malloc
#define free	pic16_free
#define realloc	pic16_realloc
#define calloc	pic16_calloc
#define _MALLOC_SPEC
#endif

#define _MALLOC_SPEC	data

#define MAX_BLOCK_SIZE	0x7f		/* 126 bytes */
#define	MAX_HEAP_SIZE	0x200		/* 512 bytes */
#define _MAX_HEAP_SIZE	(MAX_HEAP_SIZE-1)

#define ALLOC_FLAG		0x80
#define HEADER_SIZE		1


typedef union {
	unsigned char datum;
	struct {
		unsigned count: 7;
		unsigned alloc: 1;
	} bits;
} _malloc_rec;


unsigned char _MALLOC_SPEC *malloc(unsigned char);
void free(unsigned char _MALLOC_SPEC *);
unsigned char _MALLOC_SPEC *calloc(unsigned char num);		//, unsigned char len);
unsigned char _MALLOC_SPEC *realloc(unsigned char _MALLOC_SPEC *mblock, unsigned char len);


#endif /* __MALLOC_H__ */


