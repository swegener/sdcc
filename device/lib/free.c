#include <sdcc-lib.h>
#include <malloc.h>

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

MEMHEADER * _sdcc_prev_memheader;
// apart from finding the header
// this function also finds it's predecessor
MEMHEADER *
_sdcc_find_memheader(void * p)
{
  register MEMHEADER * pthis;
  if (!p)
    return NULL;
  pthis = (MEMHEADER * )((char *)  p - HEADER_SIZE); //to start of header
  _sdcc_prev_memheader = pthis->prev;

  return (pthis);
}

void
free (void *p)
{
  MEMHEADER *prev_header, *pthis;

  if ( p ) //For allocated pointers only!
    {
      pthis = (MEMHEADER * )((char *)  p - HEADER_SIZE); //to start of header
      if ( pthis->prev ) // For the regular header
        {
          prev_header = pthis->prev;
          prev_header->next = pthis->next;
          if (pthis->next)
            {
              pthis->next->prev = prev_header;
            }
        }
      else
        {
          pthis->len = 0; //For the first header
        }
    }
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

            //Static here means: can be accessed from this module only
            extern MEMHEADER xdata * _sdcc_first_memheader;

            MEMHEADER xdata * _sdcc_prev_memheader;
            // apart from finding the header
            // this function also finds it's predecessor
            MEMHEADER xdata * _sdcc_find_memheader(void xdata * p)
            {
              register MEMHEADER xdata * pthis;
              register MEMHEADER xdata * cur_header;

              if (!p)
                  return NULL;
              pthis = (MEMHEADER xdata *) p;
              pthis -= 1; //to start of header
              cur_header = _sdcc_first_memheader;
              _sdcc_prev_memheader = NULL;
              while (cur_header && pthis != cur_header)
                {
                  _sdcc_prev_memheader = cur_header;
                  cur_header = cur_header->next;
                }
              return (cur_header);
            }

            void free (void * p)
            {
              register MEMHEADER xdata * pthis;

              pthis = _sdcc_find_memheader(p);
              if (pthis) //For allocated pointers only!
              {
                if (!_sdcc_prev_memheader)
                {
                  pthis->len = 0;
                }
                else
                {
                  _sdcc_prev_memheader->next = pthis->next;
                }
              }
            }
            //END OF MODULE
#endif
