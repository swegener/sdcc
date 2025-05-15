/*
 * Simulator of microcontrollers (intscl.h)
 *
 * Copyright (C) 1999 Drotos Daniel
 * 
 * To contact author send email to dr.dkdb@gmail.com
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

#ifndef I8048_IRQCL_HEADER
#define I8048_IRQCL_HEADER

#include "stypes.h"
#include "pobjcl.h"
#include "uccl.h"
#include "newcmdcl.h"

#include "i8020cl.h"


class cl_hw;

class cl_ints: public cl_hw
{
 public:
  u8_t ene; // enable for external
  C8 cene; // enable cell
  class cl_i8020 *u;
 public:
  cl_ints(class cl_uc *auc);
  virtual int init(void);
  virtual void added_to_uc(void);
  virtual void write(MCELL *cell, t_mem *val);
  virtual int tick(int cycles);
  virtual void reset(void);
  //virtual void happen(class cl_hw *where, enum hw_event he, void *params);
  virtual void print_info(class cl_console_base *con);
  virtual void en_e(void) { ene= 1; }
  virtual void dis_e(void) { ene= 0; }
};


#endif

/* End of i8048.src/intscl.h */
