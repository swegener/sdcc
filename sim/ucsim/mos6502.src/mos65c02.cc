/*
 * Simulator of microcontrollers (mos65c02.cc)
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

#include "mos65c02cl.h"


cl_mos65c02::cl_mos65c02(class cl_sim *asim):
  cl_mos6502(asim)
{
}

int
cl_mos65c02::init(void)
{
  cl_mos6502::init();
  return 0;
}

const char *
cl_mos65c02::id_string(void)
{
  return "MOS65C02";
}


/* End of mos6502.src/mos65c02.cc */
