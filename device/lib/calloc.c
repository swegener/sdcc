#include <sdcc-lib.h>
#include <malloc.h>
#include <string.h>

//--------------------------------------------------------------------
//Written by Maarten Brock, 2004
//--------------------------------------------------------------------
//calloc function implementation for embedded system
//Non-ANSI keywords are C51 specific.
// xdata - variable in external memory (just RAM)
//--------------------------------------------------------------------

#if _SDCC_MALLOC_TYPE_MLH

#define xdata

typedef struct _MEMHEADER MEMHEADER;

struct _MEMHEADER
{
  MEMHEADER *   next;
  MEMHEADER *   prev;
  unsigned int  len;
  unsigned char mem;
};

#define HEADER_SIZE (sizeof(MEMHEADER)-sizeof(char))

#else

#define HEADER_SIZE sizeof(MEMHEADER)

#endif

void xdata * calloc (size_t nmemb, size_t size)
{
  register void xdata * ptr;

  ptr = malloc(nmemb * size);
  if (ptr)
  {
    memset(ptr, 0, nmemb * size);
  }
  return ptr;
}
//END OF MODULE
