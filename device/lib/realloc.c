#include <sdcc-lib.h>
#include <malloc.h>
#include <string.h>

//--------------------------------------------------------------------
//Written by Maarten Brock, 2004
//--------------------------------------------------------------------
//realloc function implementation for embedded system
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

extern MEMHEADER xdata * _sdcc_prev_memheader;

// apart from finding the header
// this function also finds it's predecessor
extern MEMHEADER xdata * _sdcc_find_memheader(void xdata * p);

void xdata * realloc (void * p, size_t size)
{
  register MEMHEADER xdata * pthis;
  register MEMHEADER xdata * pnew;

  pthis = _sdcc_find_memheader(p);
  if (pthis)
  {
    if (size>(0xFFFF-HEADER_SIZE)) return (void xdata *) NULL; //To prevent overflow in next line
    size += HEADER_SIZE; //We need a memory for header too

    if ((((unsigned int)pthis->next) - ((unsigned int)pthis)) >= size)
    {//if spare is more than need
      pthis->len = size;
      return p;
    }

    if ((_sdcc_prev_memheader) &&
        ((((unsigned int)pthis->next) -
          ((unsigned int)_sdcc_prev_memheader) -
          _sdcc_prev_memheader->len) >= size))
    {
      pnew = (MEMHEADER xdata * )((char xdata *)_sdcc_prev_memheader + _sdcc_prev_memheader->len);
      _sdcc_prev_memheader->next = pnew;
      memmove(pnew, pthis, pthis->len);
      pnew->len = size;
      return pnew->mem;
    }

    pnew = malloc(size - HEADER_SIZE);
    if (pnew)
    {
      memcpy(pnew, pthis->mem, pthis->len - HEADER_SIZE);
      free(p);
    }
    return pnew;
  }
  else
  {
  	return malloc(size);
  }
}
//END OF MODULE
