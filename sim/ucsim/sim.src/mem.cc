/*
 * Simulator of microcontrollers (mem.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "i_string.h"

// prj
#include "utils.h"
#include "globals.h"

// cmd
#include "newcmdcl.h"

// local
#include "memcl.h"
#include "hwcl.h"


/*
 * Memory location handled specially by a hw element
 */

cl_memloc::cl_memloc(long addr):
  cl_base()
{
  address= addr;
  hws= new cl_list(2, 2);
  hws->init();
}

cl_memloc::~cl_memloc(void)
{
  hws->disconn_all();
  delete hws;
}

ulong
cl_memloc::read(class cl_mem *mem)
{
  uchar ret= 0;
  class cl_hw *hw;

  if (!hws ||
      hws->count == 0)
    return(ret);
  if ((hw= (class cl_hw *)(hws->at(0))))
    ret= hw->read(mem, address);
  return(ret);
}

void
cl_memloc::write(class cl_mem *mem, long addr, ulong *val)
{
  class cl_hw *hw;
  int i;

  if (!hws)
    return;
  for (i= 0; i < hws->count; i++)
    {
      hw= (class cl_hw *)hws->at(0);
      hw->write(mem, addr, val);
    }
}


/* Sorted collection of memory locations */

cl_memloc_coll::cl_memloc_coll(void):
  cl_sorted_list(2, 2)
{
  Duplicates= DD_FALSE;
}

void *
cl_memloc_coll::key_of(void *item)
{
  return(&(((class cl_memloc *)item)->address));
}

int
cl_memloc_coll::compare(void *key1, void *key2)
{
  if (*(long*)key1 > *(long*)key2)
    return(1);
  else
    if (*(long*)key1 < *(long*)key2)
      return(-1);
    else
      return(0);
}

class cl_memloc *
cl_memloc_coll::get_loc(long address)
{
  t_index i;

  if (search(&address, i))
    return((class cl_memloc*)(at(i)));
  return(0);
}


/*
 * Memory
 ******************************************************************************
 */

cl_mem::cl_mem(enum mem_class atype, t_addr asize, int awidth):
  cl_base()
{
  int i;

  type= atype;
  width= awidth;
  size= asize;
  mem= 0;
  for (i= width, mask= 0; i; i--)
    mask= (mask<<1) | 1;
  if (width <= 8)
    mem= (TYPE_UBYTE *)malloc(size);
  else if (width <= 16)
    mem= (TYPE_UWORD *)malloc(size*sizeof(TYPE_WORD));
  else
    mem= (TYPE_UDWORD *)malloc(size*sizeof(TYPE_DWORD));
  read_locs= new cl_memloc_coll();
  write_locs= new cl_memloc_coll();
  dump_finished= 0;
}

cl_mem::~cl_mem(void)
{
  if (mem)
    free(mem);
  delete read_locs;
  delete write_locs;
}

int
cl_mem::init(void)
{
  t_addr i;

  for (i= 0; i < size; i++)
    set(i, (type==MEM_ROM)?(-1):0);
  return(0);
}

char *
cl_mem::id_string(void)
{
  char *s= get_id_string(mem_ids, type);

  return(s?s:(char*)"NONE");
}

ulong
cl_mem::read(t_addr addr)
{
  class cl_memloc *loc;

  if (addr >= size)
    {
      //FIXME
      fprintf(stderr, "Address 0x%06lx is over 0x%06lx\n", addr, size);
      return(0);
    }
  if ((loc= read_locs->get_loc(addr)))
    return(loc->read(this));
  if (width <= 8)
    return((((TYPE_UBYTE*)mem)[addr])&mask);
  else if (width <= 16)
    return((((TYPE_UWORD*)mem)[addr])&mask);
  else
    return((((TYPE_UDWORD*)mem)[addr])&mask);
}

ulong
cl_mem::get(t_addr addr)
{
  if (addr >= size)
    return(0);
  if (width <= 8)
    return((((TYPE_UBYTE*)mem)[addr])&mask);
  else if (width <= 16)
    return((((TYPE_UWORD*)mem)[addr])&mask);
  else
    return((((TYPE_UDWORD*)mem)[addr])&mask);
}


/*
 * Modify memory location
 */

/* Write calls callbacks of HW elements */

void
cl_mem::write(t_addr addr, t_mem *val)
{
  class cl_memloc *loc;

  if (addr >= size)
    return;
  if ((loc= write_locs->get_loc(addr)))
    loc->write(this, addr, val);
  if (width <= 8)
    ((TYPE_UBYTE*)mem)[addr]= (*val)&mask;
  else if (width <= 16)
    ((TYPE_UWORD*)mem)[addr]= (*val)&mask;
  else
    ((TYPE_UDWORD*)mem)[addr]= (*val)&mask;
}

/* Set doesn't call callbacks */

void
cl_mem::set(t_addr addr, t_mem val)
{
  if (addr >= size)
    return;
  if (width <= 8)
    ((TYPE_UBYTE*)mem)[addr]= val&mask;
  else if (width <= 16)
    ((TYPE_UWORD*)mem)[addr]= val&mask;
  else
    ((TYPE_UDWORD*)mem)[addr]= val&mask;
}

/* Set or clear bits, without callbacks */

void
cl_mem::set_bit1(t_addr addr, t_mem bits)
{
  if (addr >= size)
    return;
  bits&= mask;
  if (width <= 8)
    ((TYPE_UBYTE*)mem)[addr]|= bits;
  else if (width <= 16)
    ((TYPE_UWORD*)mem)[addr]|= bits;
  else
    ((TYPE_UDWORD*)mem)[addr]|= bits;
}

void
cl_mem::set_bit0(t_addr addr, t_mem bits)
{
  if (addr >= size)
    return;
  bits&= mask;
  if (width <= 8)
    ((TYPE_UBYTE*)mem)[addr]&= ~bits;
  else if (width <= 16)
    ((TYPE_UWORD*)mem)[addr]&= ~bits;
  else
    ((TYPE_UDWORD*)mem)[addr]&= ~bits;
}

void
cl_mem::dump(t_addr start, t_addr stop, int bpl, class cl_console *con)
{
  int i;

  if (start < 0)
    {
      start= dump_finished;
      stop= start+stop;
    }
  while ((start <= stop) &&
	 (start < size))
    {
      con->printf("%06x ", start);
      for (i= 0; (i < bpl) &&
	     (start+i < size) &&
	     (start+i <= stop);
	   i++)
	{
	  char format[10];
	  sprintf(format, "%%0%dx ", width/4);
	  con->printf(format/*"%02x "*/, get(start+i));
	}
      while (i < bpl)
	{
	  //FIXME
	  con->printf("   ");
	  i++;
	}
      for (i= 0; (i < bpl) &&
	     (start+i < size) &&
	     (start+i <= stop);
	   i++)
	{
	  long c= get(start+i);
	  con->printf("%c", isprint(255&c)?(255&c):'.');
	  if (width > 8)
	    con->printf("%c", isprint(255&(c>>8))?(255&(c>>8)):'.');
	  if (width > 16)
	    con->printf("%c", isprint(255&(c>>16))?(255&(c>>16)):'.');
	  if (width > 24)
	    con->printf("%c", isprint(255&(c>>24))?(255&(c>>24)):'.');
	}
      con->printf("\n");
      dump_finished= start+i;
      start+= bpl;
    }
}


/*
 * Bitmap
 */

cl_bitmap::cl_bitmap(long asize):
  cl_base()
{
  map= (uchar*)malloc(size= asize/(8*SIZEOF_CHAR));
  memset(map, 0, size);
}

cl_bitmap::~cl_bitmap(void)
{
  free(map);
}

void
cl_bitmap::set(long pos)
{
  int i;

  if ((i= pos/(8*SIZEOF_CHAR)) < size)
    map[i]|= (1 << (pos & ((8*SIZEOF_CHAR)-1)));
}

void
cl_bitmap::clear(long pos)
{
  int i;

  if ((i= pos/(8*SIZEOF_CHAR)) < size)
    map[i]&= ~(1 << (pos & ((8*SIZEOF_CHAR)-1)));
}

bool
cl_bitmap::get(long pos)
{
  return(map[pos/(8*SIZEOF_CHAR)] & (1 << (pos & ((8*SIZEOF_CHAR)-1))));
}

bool
cl_bitmap::empty(void)
{
  int i;

  for (i= 0; i < size && map[i] == 0; i++) ;
  return(i == size);
}

/*
 * Special memory for code (ROM)
 */

cl_rom::cl_rom(long asize, int awidth):
  cl_mem(MEM_ROM, asize, awidth)
{
  bp_map= new cl_bitmap(asize);
  inst_map= new cl_bitmap(asize);
}

cl_rom::~cl_rom(void)
{
  delete bp_map;
  delete inst_map;
}


/* End of mem.cc */
