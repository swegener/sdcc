/*
 * Simulator of microcontrollers (stm8.src/timercl.h)
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

/* $Id: timercl.h 367 2016-07-16 18:35:09Z  $ */

#ifndef STM8_TIMERCL_HEADER
#define STM8_TIMERCL_HEADER

#include "hwcl.h"


class cl_tim: public cl_hw
{
 protected:
  t_addr base;
  cl_memory_cell *regs[32];

  int bits, mask;
  int cnt;
  int ar;
  
 public:
  cl_tim(class cl_uc *auc, int aid, t_addr abase);
  virtual int init(void);

  virtual int tick(int cycles);
  virtual void reset(void);
};


// Advanced
class cl_tim1: public cl_tim
{
 public:
  cl_tim1(class cl_uc *auc, int aid, t_addr abase);
};


// General purpose
class cl_tim235: public cl_tim
{
 public:
  cl_tim235(class cl_uc *auc, int aid, t_addr abase);
};


// Basic
class cl_tim46: public cl_tim
{
 public:
  cl_tim46(class cl_uc *auc, int aid, t_addr abase);
};


#endif

/* End of stm8.src/timercl.h */
