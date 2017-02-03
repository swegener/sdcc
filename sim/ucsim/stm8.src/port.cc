/*
 * Simulator of microcontrollers (stm8.src/port.cc)
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

#include "portcl.h"

cl_port::cl_port(class cl_uc *auc, t_addr abase/*, int aid*/, const char *aname):
	cl_hw(auc, HW_PORT, /*aid*/0, aname)
{
  base = abase;
  set_name(aname);
}

int
cl_port::init(void)
{
  cl_hw::init();
  cell_p= register_cell(uc->rom, base + 0);
  cell_in= register_cell(uc->rom, base + 1);
  cell_dir= register_cell(uc->rom, base + 2);
  return 0;
}

/* End of stm8.src/port.cc */
