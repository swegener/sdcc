/*
 * Simulator of microcontrollers (pdk16.cc)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
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

#include "pdk16cl.h"


cl_fppa16::cl_fppa16(int aid, class cl_pdk *the_puc, class cl_sim *asim):
  cl_fppa15(aid, the_puc, asim)
{
  type= new struct cpu_entry;
  type->type= CPU_PDK16;
}

cl_fppa16::cl_fppa16(int aid, class cl_pdk *the_puc, struct cpu_entry *IType, class cl_sim *asim):
  cl_fppa15(aid, the_puc, IType, asim)
{
}


int
cl_fppa16::init(void)
{
  cl_fppa::init();
  return 0;
}

const char *
cl_fppa16::id_string(void)
{
  return "pdk16";
}

void
cl_fppa16::reset(void)
{
  cl_fppa::reset();
}

int
cl_fppa16::execute(unsigned int code)
{
  return resINV;
}


/* End of pdk.src/pdk16.cc */
