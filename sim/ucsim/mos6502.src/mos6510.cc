/*
 * Simulator of microcontrollers (mos6510.cc)
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

#include <stdlib.h>
#include <ctype.h>

#include "mos6510cl.h"

#include "port10cl.h"


cl_mos6510::cl_mos6510(class cl_sim *asim):
  cl_mos6502(asim)
{
  *my_id= "MOS6510";
}

int
cl_mos6510::init(void)
{
  cl_mos6502::init();
  return 0;
}

void
cl_mos6510::mk_hw_elements(void)
{
  class cl_hw *h;

  cl_mos6502::mk_hw_elements();

  add_hw(h= new cl_port10(this, "port"));
  h->init();
}

/*
 * 8502
 */

cl_mos8502::cl_mos8502(class cl_sim *asim):
  cl_mos6510(asim)
{
  *my_id= "MOS8502";
}

int
cl_mos8502::init(void)
{
  cl_mos6510::init();
  set_xtal(2000000);
  return 0;
}


/* End of mos6502.src/mos6510.cc */
