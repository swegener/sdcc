/*
 * Simulator of microcontrollers (inst.cc)
 *
 * Copyright (C) 2022 Drotos Daniel
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

#include "i8048cl.h"


int
CL2::DISI(MP)
{
  if (ints)
    ints->dis_e();
  return resGO;
}

int
CL2::ENI(MP)
{
  if (ints)
    ints->en_e();
  return resGO;
}

int
CL4::JNI(MP)
{   
  return jif(!(((class cl_i8020_cpu*)cpu)->ipins & ipm_int));
}


/* End of i8048.src/inst.cc */
