/*
 * Simulator of microcontrollers (stm8.src/portcl.h)
 *
 * Copyright (C) 2017,17 Drotos Daniel, Talker Bt.
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

#include "port_hwcl.h"


class cl_port: public cl_hw
{
 private:
  t_addr base;
  int portnr;
  t_mem last_idr;
 public:
  class cl_memory_cell *cell_pins, *cell_odr, *cell_idr, *cell_ddr, *cell_cr1, *cell_cr2;
 public:
  cl_port(class cl_uc *auc, int iportnr/*, int aid*/, const char *aname);
  virtual int init(void);
  virtual void reset(void);

  virtual void write(class cl_memory_cell *cell, t_mem *val);

  virtual void print_info(class cl_console_base *con);

 private:
  bool high_bits_are_port_interrupt(const struct state *s);
  bool low_bits_are_port_interrupt(const struct state *s);
  bool port_used_for_interrupt(const struct state *s);
  int port_sensitivity(const struct state *s);
  void port_interrupt(struct state *s);
  void port_check_interrupt(struct state *s, t_mem bitmask, int bithigh, int bitlow);
  void pin_check_interrupt(struct state *s, t_mem exti_cr, int bithigh, int bitlow);
};


#endif

/* End of stm8.src/portcl.h */
