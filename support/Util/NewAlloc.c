/*
===============================================================================
NEWALLOC - SDCC Memory allocation functions

These functions are wrappers for the standard malloc, realloc and free
functions.


     This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

===============================================================================
*/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>

#include "newalloc.h"
/*
-------------------------------------------------------------------------------
Clear_realloc - Reallocate a memory block and clear any memory added with
out of memory error detection

-------------------------------------------------------------------------------
*/

void *Clear_realloc(void *OldPtr,size_t OldSize,size_t NewSize)

{
void *NewPtr ;

NewPtr = realloc(OldPtr,NewSize) ;

if (!NewPtr)
  {
  printf("ERROR - No more memory\n") ;
/*  werror(E_OUT_OF_MEM,__FILE__,NewSize);*/
  exit (1);
  }

if (NewPtr)
  if (NewSize > OldSize)
    memset((char *) NewPtr + OldSize,0x00,NewSize - OldSize) ;

return NewPtr ;
}
/*
-------------------------------------------------------------------------------
Safe_realloc - Reallocate a memory block with out of memory error detection

-------------------------------------------------------------------------------
*/

void *Safe_realloc(void *OldPtr,size_t NewSize)

{
void *NewPtr ;

NewPtr = realloc(OldPtr,NewSize) ;

if (!NewPtr)
  {
  printf("ERROR - No more memory\n") ;
/*  werror(E_OUT_OF_MEM,__FILE__,NewSize);*/
  exit (1);
  }

return NewPtr ;
}
/*
-------------------------------------------------------------------------------
Safe_calloc - Allocate a block of memory from the application heap, clearing
all data to zero and checking for out of memory errors.

-------------------------------------------------------------------------------
*/

void *Safe_calloc(size_t Elements,size_t Size)

{
void *NewPtr ;

NewPtr = calloc(Elements,Size) ;

if (!NewPtr)
  {
  printf("ERROR - No more memory\n") ;
/*  werror(E_OUT_OF_MEM,__FILE__,Size);*/
  exit (1);
  }

return NewPtr ;
}
/*
-------------------------------------------------------------------------------
Safe_malloc - Allocate a block of memory from the application heap
and checking for out of memory errors.

-------------------------------------------------------------------------------
*/

void *Safe_malloc(size_t Size)

{
void *NewPtr ;

NewPtr = malloc(Size) ;

if (!NewPtr)
  {
  printf("ERROR - No more memory\n") ;
/*  werror(E_OUT_OF_MEM,__FILE__,Size);*/
  exit (1);
  }

return NewPtr ;
}
