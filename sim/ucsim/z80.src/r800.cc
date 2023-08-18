/*
 * Simulator of microcontrollers (r800.cc)
 *
 * Copyright (C) 1999,2023 Drotos Daniel, Talker Bt.
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

#include "r800cl.h"


cl_r800::cl_r800(struct cpu_entry *Itype, class cl_sim *asim):
  cl_ez80(Itype, asim)
{
}

int
cl_r800::init(void)
{
  return cl_ez80::init();
}

const char *
cl_r800::id_string(void)
{
  return ("R800");
}


/* End of z80.src/r800.cc */
