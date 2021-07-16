/*
 * Simulator of microcontrollers (@@F@@)
 *
 * Copyright (C) 2021,2021 Drotos Daniel, Talker Bt.
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

#ifndef RMEMCL_HEADER
#define RMEMCL_HEADER

#include "memcl.h"

class cl_ras: public cl_address_space
{
 public:
  class cl_memory_chip *chip;
 public:
  u8_t xpc, segsize, dataseg, stackseg;
 public:
  cl_ras(chars id, class cl_memory_chip *achip);
 public:
  virtual t_addr log2phy(t_addr log);
  virtual t_addr px2phy(u32_t px);
  virtual t_mem read(t_addr addr);
  virtual t_mem phread(t_addr phaddr) { return phget(phaddr); }
  virtual t_mem get(t_addr addr);
  virtual t_mem phget(t_addr phaddr);
  virtual t_mem write(t_addr addr, t_mem val);
  virtual void set(t_addr addr, t_mem val);
  virtual void download(t_addr phaddr, t_mem val);
};

#endif

/* End of rxk.src/rmemcl.h */
