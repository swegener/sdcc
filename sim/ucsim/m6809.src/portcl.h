/*
 * Simulator of microcontrollers (portcl.h)
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

#ifndef PORTCL_HEADER
#define PORTCL_HEADER

#include "hwcl.h"
#include "port_hwcl.h"


enum port_cfg
  {
   cfg_on	= 0,
   cfg_base	= 1,
   cfg_cra	= 2,
   cfg_ddra	= 3,
   cfg_ora	= 4,
   cfg_ina	= 5,
   cfg_crb	= 6,
   cfg_ddrb	= 7,
   cfg_orb	= 8,
   cfg_inb	= 9,
  };

class cl_port: public cl_hw
{
public:
  t_addr base;
  class cl_memory_cell *cra;
  class cl_memory_cell *ddra;
  class cl_memory_cell *ora;
  class cl_memory_cell *ina;
  class cl_memory_cell *crb;
  class cl_memory_cell *ddrb;
  class cl_memory_cell *orb;
  class cl_memory_cell *inb;
  class cl_memory_cell *rs[4];
 public:
  cl_port(class cl_uc *auc, int aid);
  cl_port(class cl_uc *auc, int aid, t_addr the_addr);
  virtual int init(void);
  virtual int cfg_size(void) { return 10; }
  virtual const char *cfg_help(t_addr addr);
  virtual void set_cmd(class cl_cmdline *cmdline, class cl_console_base *con);

  virtual class cl_memory_cell *reg(class cl_memory_cell *cell_rs);
  virtual t_mem read(class cl_memory_cell *cell);
  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);

  virtual void reset(void);
  
  virtual void print_info(class cl_console_base *con);
};


#endif

/* End of m6898.src/portcl.h */
