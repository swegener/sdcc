/*
 * Simulator of microcontrollers (s51.src/mducl.h)
 *
 * Copyright (C) 2016,16 Drotos Daniel, Talker Bt.
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

#ifndef MDUCL_HEADER
#define MDUCL_HEADER

#include "hwcl.h"


class cl_mdu517: public cl_hw
{
 protected:
  u8_t v[7];
  class cl_memory_cell *regs[7];//, *arcon;
  u64_t writes;
  int nuof_writes;
  //bool calcing;
 public:
  cl_mdu517(class cl_uc *auc, int aid);
  virtual int init(void);
  
  virtual t_mem read(class cl_memory_cell *cell);
  virtual void write(class cl_memory_cell *cell, t_mem *val);
  virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);
};


#endif

/* End of s51.src/mducl.h */
