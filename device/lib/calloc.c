/*-------------------------------------------------------------------------
   calloc.c - allocate cleared memory.

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

#define MEMHEADER   struct MAH// Memory Allocation Header

MEMHEADER
{
  MEMHEADER __xdata *  next;
  unsigned int         len;
  unsigned char        mem[];
};

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
