/*
 * Simulator of microcontrollers (sim.src/memcl.h)
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

#ifndef SIM_MEMCL_HEADER
#define SIM_MEMCL_HEADER


// prj
#include "ddconfig.h"
#include "stypes.h"
#include "pobjcl.h"

// gui.src
#include "guiobjcl.h"


class cl_mem;
class cl_event_handler;


// Cell types
#define CELL_NORMAL	0x00	/* Nothing special */
#define CELL_HW_READ	0x01	/* Hw handles read */
#define CELL_HW_WRITE	0x02	/* Hw catches write */
#define CELL_INST	0x04	/* Marked as instruction */
#define CELL_FETCH_BRK	0x08	/* Fetch breakpoint */
#define CELL_READ_BRK	0x10	/* Read event breakpoint */
#define CELL_WRITE_BRK	0x20	/* Write event breakpoint */

#define CELL_GENERAL	(CELL_NORMAL|CELL_INST|CELL_FETCH_BRK)


class cl_cell: public cl_base
{
#ifdef STATISTIC
public:
  unsigned long nuof_writes, nuof_reads;
#endif
public:
  cl_cell(void);
public:
  virtual TYPE_UBYTE get_type(void)= 0;
  virtual void set_type(TYPE_UBYTE what)= 0;
  virtual t_mem get_mask(void)= 0;

  virtual t_mem read(void)=0 ;
  virtual t_mem read(enum hw_cath /*skip*/)=0;
  virtual t_mem get(void)=0;
  virtual t_mem write(t_mem val)=0;
  virtual t_mem set(t_mem val)=0;
  virtual t_mem add(long what)=0;
  virtual t_mem wadd(long what)=0;

  virtual void set_bit1(t_mem bits)=0;
  virtual void set_bit0(t_mem bits)=0;

  virtual class cl_cell *add_hw(class cl_hw */*hw*/, int */*ith*/)=0;
  virtual class cl_hw *get_hw(int ith)=0;
  virtual class cl_event_handler *get_event_handler(void)=0;
};

class cl_normal_cell: public cl_cell
{
public:
  t_mem data;
  TYPE_UBYTE type;	// See CELL_XXXX
  //protected:
  t_mem mask;
  uchar width;

public:
  cl_normal_cell(uchar awidth);
  //virtual void destroy(void) {}

  virtual TYPE_UBYTE get_type(void) { return(type); }
  virtual void set_type(TYPE_UBYTE what) { type= what; }
  virtual t_mem get_mask(void) { return(mask); }

  virtual t_mem read(void) {
#ifdef STATISTIC
    nuof_reads++;
#endif
    return(data);
  }
  virtual t_mem read(enum hw_cath /*skip*/) { return(data); }
  virtual t_mem get(void)  { return(data); }
  virtual t_mem write(t_mem val) {
    data= val & mask;
#ifdef STATISTIC
    nuof_writes++;
#endif
    return(data);
  }
  virtual t_mem set(t_mem val) { return(data= val & mask); }
  virtual t_mem add(long what);
  virtual t_mem wadd(long what);

  virtual void set_bit1(t_mem bits);
  virtual void set_bit0(t_mem bits);

  virtual class cl_cell *add_hw(class cl_hw */*hw*/, int */*ith*/)
  { return(0); }
  virtual class cl_hw *get_hw(int ith) { return(0); }
  //virtual class cl_brk *get_brk(void) { return(0); }
  virtual class cl_event_handler *get_event_handler(void) { return(0); }
};


/* Memory location handled specially by a hw element */

/*class cl_memloc: public cl_base
{
public:
  t_addr address;
  class cl_list *hws;

public:
  cl_memloc(t_addr addr);
  virtual ~cl_memloc(void);

  virtual ulong read(class cl_mem *mem);
  virtual void write(class cl_mem *mem, t_addr addr, t_mem *val);
};*/

/*class cl_memloc_coll: public cl_sorted_list
{
public:
  cl_memloc_coll(void);

  virtual void *key_of(void *item);
  virtual int compare(void *key1, void *key2);

  class cl_memloc *get_loc(t_addr address);
};*/

/* Memory */

class cl_mem: public cl_guiobj
{
public:
  enum mem_class type;
  char *class_name;
  char *addr_format, *data_format;
  t_addr size;
  ulong mask;
  int width; // in bits
  union {
    void *mem;
    uchar *umem8;
  };
  //class cl_memloc_coll *read_locs, *write_locs;
  class cl_uc *uc;
  t_addr dump_finished;

public:
  cl_mem(enum mem_class atype, char *aclass_name, t_addr asize, int awidth,
	 class cl_uc *auc);
  virtual ~cl_mem(void);
  virtual int init(void);
  virtual char *id_string(void);
  virtual int get_cell_flag(t_addr /*addr*/) { return(CELL_NORMAL); }
  virtual bool get_cell_flag(t_addr /*addr*/, int /*flag*/)
  { return(DD_FALSE); }
  virtual void set_cell_flag(t_addr addr, bool set_to, int flag) {}

  virtual t_mem read(t_addr addr);
  virtual t_mem read(t_addr addr, enum hw_cath /*skip*/) {return(read(addr));}
  virtual t_mem get(t_addr addr);
  virtual t_mem write(t_addr addr, t_mem val);
  virtual void set(t_addr addr, t_mem val);
  virtual void set_bit1(t_addr addr, t_mem bits);
  virtual void set_bit0(t_addr addr, t_mem bits);
  virtual void write_bit1(t_addr addr, t_mem bits) { set_bit1(addr, bits); }
  virtual void write_bit0(t_addr addr, t_mem bits) { set_bit0(addr, bits); }
  virtual t_mem add(t_addr addr, long what);
  virtual t_mem wadd(t_addr addr, long what) { return(add(addr, what)); }
  virtual t_addr dump(t_addr start, t_addr stop, int bpl,
		      class cl_console *con);
  virtual t_addr dump(class cl_console *con);
  virtual bool search_next(bool case_sensitive,
			   t_mem *array, int len, t_addr *addr);

  virtual class cl_cell *get_cell(t_addr addr) {return(0);}
  virtual class cl_cell *register_hw(t_addr addr, class cl_hw *hw, int *ith,
				     bool announce)
  { return(0); }
  virtual void set_brk(t_addr /*addr*/, class cl_brk */*brk*/) {}
  virtual void del_brk(t_addr addr, class cl_brk *brk) {}
#ifdef STATISTIC
  virtual unsigned long get_nuof_reads(void) {return(0);}
  virtual unsigned long get_nuof_writes(void) {return(0);}
  virtual void set_nuof_reads(unsigned long value) {}
  virtual void set_nuof_writes(unsigned long value) {}
#endif
};

/* Spec for CODE */

/*class cl_bitmap: public cl_base
{
public:
  uchar *map;
  int size;
public:
  cl_bitmap(t_addr asize);
  virtual ~cl_bitmap(void);
  virtual void set(t_addr pos);
  virtual void clear(t_addr pos);
  virtual bool get(t_addr pos);
  virtual bool empty(void);
};

class cl_rom: public cl_mem
{
public:
  class cl_bitmap *bp_map;
  class cl_bitmap *inst_map;
public:
  cl_rom(t_addr asize, int awidth, class cl_uc *auc);
  virtual ~cl_rom(void);
};*/

/*
 * New type of memory simulation
 */

class cl_registered_cell: public cl_normal_cell
{
public:
  //class cl_list *hws;
  class cl_hw **hardwares;
  int nuof_hws;
public:
  cl_registered_cell(uchar awidth);
  virtual ~cl_registered_cell(void);
  //virtual void destroy(void);

  virtual t_mem read(void);
  virtual t_mem read(enum hw_cath skip);
  virtual t_mem write(t_mem val);

  virtual class cl_cell *add_hw(class cl_hw *hw, int *ith);
  virtual class cl_hw *get_hw(int ith);
};

class cl_event_handler: public cl_base
{
public:
  class cl_list *read_bps, *write_bps;
  class cl_uc *uc;
public:
  cl_event_handler(class cl_uc *auc);
  virtual ~cl_event_handler(void);

  virtual void write(void);
  virtual void read(void);

  virtual int add_bp(class cl_brk *bp);
  virtual int copy_from(class cl_event_handler *eh);
  virtual bool del_bp(class cl_brk *bp);
};

class cl_event_cell: public cl_normal_cell
{
protected:
  class cl_event_handler *eh;
public:
  cl_event_cell(uchar awidth, class cl_uc *auc);
  virtual ~cl_event_cell(void);

  virtual t_mem read(void);
  virtual t_mem write(t_mem val);
  //virtual void event(void);

  //virtual class cl_brk *get_brk(void) { return(brk); }
  virtual class cl_event_handler *get_event_handler(void) { return(eh); }
};

class cl_ev_reg_cell: public cl_registered_cell
{
protected:
  class cl_event_handler *eh;
public:
  cl_ev_reg_cell(uchar awidth, class cl_uc *auc);
  virtual ~cl_ev_reg_cell(void);
 
  virtual t_mem read(void);
  virtual t_mem write(t_mem val);
  //virtual void event(void);
  
  //virtual class cl_brk *get_brk(void) { return(brk); }
  virtual class cl_event_handler *get_event_handler(void) { return(eh); }
};

class cl_mapped_cell: public cl_cell
{
protected:
  class cl_cell *real_cell;
public:
  cl_mapped_cell(class cl_cell *realcell);
  virtual ~cl_mapped_cell(void);

  virtual t_mem read(void);
  virtual t_mem read(enum hw_cath skip);
  virtual t_mem get(void);
  virtual t_mem write(t_mem val);
  virtual t_mem set(t_mem val);
  virtual t_mem add(long what);
  virtual t_mem wadd(long what);

  virtual void set_bit1(t_mem bits);
  virtual void set_bit0(t_mem bits);

  virtual class cl_cell *add_hw(class cl_hw *hw, int *ith);
  virtual class cl_hw *get_hw(int ith);
  virtual class cl_event_handler *get_event_handler(void);
};

class cl_m: public cl_mem
{
protected:
  class cl_cell **array;
  class cl_cell *dummy;
  t_addr bus_mask;
public:
  //t_addr size;
  //int width;

public:
  cl_m(enum mem_class atype, char *aclass_name, t_addr asize, int awidth,
       class cl_uc *auc);
  cl_m(t_addr asize, int awidth);
  virtual ~cl_m(void);
  virtual void err_inv_addr(t_addr addr);
  virtual int get_cell_flag(t_addr addr);
  virtual bool get_cell_flag(t_addr addr, int flag);
  virtual void set_cell_flag(t_addr addr, bool set_to, int flag);

  virtual t_mem read(t_addr addr);
  virtual t_mem read(t_addr addr, enum hw_cath skip);
  virtual t_mem get(t_addr addr);
  virtual t_mem write(t_addr addr, t_mem val);
  virtual void set(t_addr addr, t_mem val);
  virtual class cl_cell *get_cell(t_addr addr);

  virtual void set_bit1(t_addr addr, t_mem bits);
  virtual void set_bit0(t_addr addr, t_mem bits);
  virtual void write_bit1(t_addr addr, t_mem bits);
  virtual void write_bit0(t_addr addr, t_mem bits);
  virtual t_mem add(t_addr addr, long what);
  virtual t_mem wadd(t_addr addr, long what);

  virtual class cl_cell *register_hw(t_addr addr, class cl_hw *hw, int *ith,
				     bool announce);
  virtual void set_brk(t_addr addr, class cl_brk *brk);
  virtual void del_brk(t_addr addr, class cl_brk *brk);

#ifdef STATISTIC
  virtual unsigned long get_nuof_reads(void);
  virtual unsigned long get_nuof_writes(void);
  virtual void set_nuof_reads(unsigned long value);
  virtual void set_nuof_writes(unsigned long value);
#endif
};


#include "errorcl.h"

/*
 * Errors in memory handling
 */

class cl_err_inv_addr: public cl_error
{
protected:
  class cl_mem *mem;
  t_addr addr;
public:
  cl_err_inv_addr(class cl_mem *amem, t_addr aaddr);
  virtual void print(class cl_commander *c);
};


#endif

/* End of memcl.h */
