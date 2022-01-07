/*
 * Simulator of microcontrollers (m68hc12cl.h)
 *
 * Copyright (C) 2020,20 Drotos Daniel, Talker Bt.
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

#ifndef M68HC12CL_HEADER
#define M68HC12CL_HEADER

#include "uccl.h"
#include "memcl.h"

#include "m68hc11cl.h"

#define CL12 cl_m68hc12

class CL12;

typedef int (*hcwrapper_fn)(class CL12 *uc, t_mem code);


#define rTMP2 (TMP2)
#define rTMP3 (TMP3)

/*
 * Base of M68HC12 processor
 */

class cl_m68hc12: public cl_m68hcbase
{
public:
  i8_t post_inc_dec;
  class cl_cell16 *post_idx_reg;
  class cl_wrap *hc12wrap;
  u16_t TMP2, TMP3;
  class cl_cell16 cTMP2, cTMP3;
public:
  cl_m68hc12(class cl_sim *asim);
  virtual int init(void);
  virtual const char *id_string(void);
  virtual void reset(void);
  virtual void make_memories(void);
  virtual void make_cpu_hw(void);
  
  virtual int proba(int,t_mem);
  virtual int prob1(int,t_mem) {return 1;}

  virtual struct dis_entry *dis_tbl(void);
  virtual struct dis_entry *get_dis_entry(t_addr addr);
  virtual char *disassc(t_addr addr, chars *comment=NULL);
  virtual int longest_inst(void) { return 6; }

  virtual void post_inst(void);
  virtual i16_t s8_16(u8_t op);
  virtual t_addr naddr(void);

  virtual void print_regs(class cl_console_base *con);
};


enum hc12cpu_cfg {
  hc12_cpu_nuof	= 0
};

class cl_hc12_cpu: public cl_hw
{
protected:
  class cl_m68hc12 *muc;
  class cl_memory_cell *dpage, *ppage, *epage, *windef;
public:
  cl_hc12_cpu(class cl_uc *auc);
  virtual int init(void);
  virtual void reset(void);
  virtual void print_info(class cl_console_base *con);
};


#endif

/* End of m68hc12.src/m68hc12cl.h */
