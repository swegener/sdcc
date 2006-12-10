/*-------------------------------------------------------------------------
   _heap.c - memory heap for malloc and friends

   Copyright (C) 2006 - Maarten Brock, sourceforge.brock@dse.nl

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

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

/*
   This is the default heap. If you need a different size (make a copy and)
   recompile it with -D HEAP_SIZE=<size> where <size> is whatever you need.
   Link the resulting object explicitly with your project.
*/

#ifndef HEAP_SIZE
#define HEAP_SIZE 1024
#endif

__xdata char _sdcc_heap[HEAP_SIZE];
const unsigned int _sdcc_heap_size = HEAP_SIZE;
