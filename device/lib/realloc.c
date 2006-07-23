/*-------------------------------------------------------------------------
   realloc.c - reallocate allocated memory.

   Copyright (C) 2004 - Maarten Brock, sourceforge.brock@dse.nl

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
-------------------------------------------------------------------------*/

#include <sdcc-lib.h>
#include <malloc.h>
#include <string.h>

#if defined(SDCC_STACK_AUTO) || defined(SDCC_z80) || defined(SDCC_gbz80)
  #define CRITICAL critical
#else
  #define CRITICAL
#endif

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
#define MEM(x)      (&x->mem)

#else

#define HEADER_SIZE sizeof(MEMHEADER)
#define MEM(x)      (x->mem)

#endif

extern MEMHEADER xdata * _sdcc_prev_memheader;

// apart from finding the header
// this function also finds it's predecessor
extern MEMHEADER xdata * _sdcc_find_memheader(void xdata * p);

void xdata * realloc (void * p, size_t size)
{
  register MEMHEADER xdata * pthis;
  register MEMHEADER xdata * pnew;
  register void xdata * ret;

  CRITICAL
  {
    pthis = _sdcc_find_memheader(p);
    if (pthis)
    {
      if (size > (0xFFFF-HEADER_SIZE))
      {
        ret = (void xdata *) NULL; //To prevent overflow in next line
      }
      else
      {
        size += HEADER_SIZE; //We need a memory for header too

        if ((((unsigned int)pthis->next) - ((unsigned int)pthis)) >= size)
        {//if spare is more than needed
          pthis->len = size;
          ret = p;
        }
        else
        {
          if ((_sdcc_prev_memheader) &&
              ((((unsigned int)pthis->next) -
                ((unsigned int)_sdcc_prev_memheader) -
                _sdcc_prev_memheader->len) >= size))
          {
            pnew = (MEMHEADER xdata * )((char xdata *)_sdcc_prev_memheader + _sdcc_prev_memheader->len);
            _sdcc_prev_memheader->next = pnew;

#if _SDCC_MALLOC_TYPE_MLH
            pthis->next->prev = pnew;
#endif

            memmove(pnew, pthis, pthis->len);
            pnew->len = size;
            ret = MEM(pnew);
          }
          else
          {
            ret = malloc(size - HEADER_SIZE);
            if (ret)
            {
              memcpy(ret, MEM(pthis), pthis->len - HEADER_SIZE);
              free(p);
            }
          }
        }
      }
    }
    else
    {
      ret = malloc(size);
    }
  }
  return ret;
}
//END OF MODULE
