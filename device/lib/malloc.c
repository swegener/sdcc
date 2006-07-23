/*-------------------------------------------------------------------------
   malloc.c - allocate memory.

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

#if defined(SDCC_STACK_AUTO) || defined(SDCC_z80) || defined(SDCC_gbz80)
  #define CRITICAL critical
#else
  #define CRITICAL
#endif

#if _SDCC_MALLOC_TYPE_MLH

typedef struct _MEMHEADER MEMHEADER;

struct _MEMHEADER
{
  MEMHEADER *    next;
  MEMHEADER *    prev;
  unsigned int   len;
  unsigned char  mem;
};

#define HEADER_SIZE (sizeof(MEMHEADER)-sizeof(char))

/* These variables are defined through the crt0 functions. */
/* Base of this variable is the first byte of the heap. */
extern MEMHEADER _sdcc_heap_start;
/* Address of this variable is the last byte of the heap. */
extern char _sdcc_heap_end;

void
_sdcc_heap_init(void)
{
  MEMHEADER *pbase = &_sdcc_heap_start;
  unsigned int size = &_sdcc_heap_end - (char *)pbase;

  pbase->next = (MEMHEADER *)((char *)pbase + size - HEADER_SIZE);
  pbase->next->next = NULL; //And mark it as last
  pbase->prev       = NULL; //and mark first as first
  pbase->len        = 0;    //Empty and ready.
}

void *
malloc (unsigned int size)
{
  register MEMHEADER * current_header;
  register MEMHEADER * new_header;
  register void * ret;

  if (size>(0xFFFF-HEADER_SIZE))
    {
      return NULL; //To prevent overflow in next line
    }

  size += HEADER_SIZE; //We need a memory for header too
  current_header = &_sdcc_heap_start;

  CRITICAL
    {
      while (1)
        {
          //    current
          //    |   len       next
          //    v   v         v
          //....*****.........******....
          //         ^^^^^^^^^
          //           spare

          if ((((unsigned int)current_header->next) -
               ((unsigned int)current_header) -
               current_header->len) >= size)
            { //if spare is more than needed
              ret = &current_header->mem;
              break;
            }
          current_header = current_header->next;    //else try next
          if (!current_header->next)
            { //if end_of_list reached
              ret = NULL;
              break;
            }
        }

      if (ret)
        {
          if (!current_header->len)
            { //This code works only for first_header in the list and only
              current_header->len = size; //for first allocation
            }
          else
            {
              //else create new header at the begin of spare
              new_header = (MEMHEADER * )((char *)current_header + current_header->len);
              new_header->next = current_header->next; //and plug it into the chain
              new_header->prev = current_header;
              current_header->next  = new_header;
              if (new_header->next)
                {
                  new_header->next->prev = new_header;
                }
              new_header->len  = size; //mark as used
              ret = &new_header->mem;
            }
        }
    }
  return ret;
}

#else

            //--------------------------------------------------------------------
            //Written by Dmitry S. Obukhov, 1997
            //dso@usa.net
            //--------------------------------------------------------------------
            //Modified for SDCC by Sandeep Dutta, 1999
            //sandeep.dutta@usa.net
            //--------------------------------------------------------------------
            //malloc and free functions implementation for embedded system
            //Non-ANSI keywords are C51 specific.
            // xdata - variable in external memory (just RAM)
            //--------------------------------------------------------------------

            #define HEADER_SIZE sizeof(MEMHEADER)

            MEMHEADER xdata * _sdcc_first_memheader;

            void init_dynamic_memory(void xdata * heap, unsigned int size)
            {

            //This function MUST be called after the RESET.
            //Parameters: heap - pointer to memory allocated by the linker
            //            size - size of this memory pool
            //Example:
            //     #define DYNAMIC_MEMORY_SIZE 0x2000
            //     .....
            //     unsigned char xdata dynamic_memory_pool[DYNAMIC_MEMORY_SIZE];
            //     unsigned char xdata * current_buffer;
            //     .....
            //     void main(void)
            //     {
            //         ...
            //         init_dynamic_memory(dynamic_memory_pool,DYNAMIC_MEMORY_SIZE);
            //         Now it is possible to use malloc.
            //         ...
            //         current_buffer = malloc(0x100);
            //
            //
              char xdata * array = (char xdata *)heap;

              if ( !array ) //Reserved memory starts at 0x0000 but that's NULL...
              {             //So, we lost one byte!
                 array++;
                 size--;
              }
              _sdcc_first_memheader = (MEMHEADER xdata * ) array;
              //Reserve a mem for last header
              _sdcc_first_memheader->next = (MEMHEADER xdata * )(array + size - sizeof(MEMHEADER xdata *));
              _sdcc_first_memheader->next->next = (MEMHEADER xdata * ) NULL; //And mark it as last
              _sdcc_first_memheader->len        = 0;    //Empty and ready.
            }

            void xdata * malloc (unsigned int size)
            {
              register MEMHEADER xdata * current_header;
              register MEMHEADER xdata * new_header;
              register void xdata * ret;

              if (size>(0xFFFF-HEADER_SIZE)) return (void xdata *) NULL; //To prevent overflow in next line
              size += HEADER_SIZE; //We need a memory for header too
              current_header = _sdcc_first_memheader;
              CRITICAL
              {
                while (1)
                {

                  //    current
                  //    |   len       next
                  //    v   v         v
                  //....*****.........******....
                  //         ^^^^^^^^^
                  //           spare

                  if ((((unsigned int)current_header->next) -
                       ((unsigned int)current_header) -
                       current_header->len) >= size)
                  { //if spare is more than needed
                    ret = current_header->mem;
                    break;
                  }
                  current_header = current_header->next;    //else try next
                  if (!current_header->next)
                  { //if end_of_list reached
                    ret = (void xdata *) NULL;
                    break;
                  }
                }
                if (ret)
                {
                  if (!current_header->len)
                  { //This code works only for first_header in the list and only
                     current_header->len = size; //for first allocation
                  }
                  else
                  { //else create new header at the begin of spare
                    new_header = (MEMHEADER xdata * )((char xdata *)current_header + current_header->len);
                    new_header->next = current_header->next; //and plug it into the chain
                    current_header->next  = new_header;
                    new_header->len  = size; //mark as used
                    ret = new_header->mem;
                  }
                }
              }
              return ret;
            }

            //END OF MODULE
#endif
