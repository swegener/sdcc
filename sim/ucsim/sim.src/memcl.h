/*
 * Simulator of microcontrollers (memcl.h)
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

#ifndef MEMCL_HEADER
#define MEMCL_HEADER

#include "stypes.h"
#include "pobjcl.h"


class cl_mem;

/* Memory location handled specially by a hw element */

class cl_memloc: public cl_base
{
public:
  long address;
  class cl_list *hws;

public:
  cl_memloc(long addr);
  ~cl_memloc(void);

  virtual ulong read(class cl_mem *mem);
  virtual void write(class cl_mem *mem, long addr, ulong *val);
};

class cl_memloc_coll: public cl_sorted_list
{
public:
  cl_memloc_coll(void);

  virtual void *key_of(void *item);
  virtual int compare(void *key1, void *key2);

  class cl_memloc *get_loc(long address);
};


/* Memory */

class cl_mem: public cl_base
{
public:
  enum mem_class type;
  t_addr size;
  ulong mask;
  int width; // in bits
  union {
    void *mem;
    uchar *umem8;
  };
  class cl_memloc_coll *read_locs, *write_locs;
  int dump_finished;

public:
  cl_mem(enum mem_class atype, t_addr asize, int awidth);
  ~cl_mem(void);
  virtual int init(void);
  virtual char *id_string(void);

  virtual ulong read(t_addr addr);
  virtual ulong get(t_addr addr);
  virtual void write(t_addr addr, t_mem *val);
  virtual void set(t_addr addr, t_mem val);
  virtual void set_bit1(t_addr addr, t_mem bits);
  virtual void set_bit0(t_addr addr, t_mem bits);
  virtual void dump(t_addr start, t_addr stop, int bpl, class cl_console *con);
};

/* Spec for CODE */

class cl_bitmap: public cl_base
{
public:
  uchar *map;
  int size;
public:
  cl_bitmap(long asize);
  ~cl_bitmap(void);
  virtual void set(long pos);
  virtual void clear(long pos);
  virtual bool get(long pos);
  virtual bool empty(void);
};

class cl_rom: public cl_mem
{
public:
  class cl_bitmap *bp_map;
  class cl_bitmap *inst_map;
public:
  cl_rom(long asize, int awidth);
  ~cl_rom(void);
};


#endif

/* End of memcl.h */
