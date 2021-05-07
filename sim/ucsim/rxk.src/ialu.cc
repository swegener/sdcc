/*
 * Simulator of microcontrollers (ialu.cc)
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

#include "rxkcl.h"


int
cl_rxk::INC_BC(t_mem code)
{
  class cl_cell16 &rp= destBC();
  rp.W(rBC+1);
  tick(1);
  return resGO;
}

int
cl_rxk::INC_B(t_mem code)
{
  class cl_cell8 &rb= destB(), &rf= destF();
  u8_t r, f= rF & ~(flagS|flagZ|flagV), na7, r7;
  na7= (rB&0x80)^0x80;
  rb.W(r= rB+1);
  r7= r&0x80;
  if (r & 0x80) f|= flagS;
  if (!r) f|= flagZ;
  if (na7 & r7) f|= flagV;
  rf.W(f);
  tick(1);
  return resGO;
}

int
cl_rxk::DEC_B(t_mem code)
{
  class cl_cell8 &rb= destB(), &rf= destF();
  u8_t r, f= rF & ~(flagS|flagZ|flagV), a7, nr7;
  a7= rB&0x80;
  rb.W(r= rB-1);
  nr7= (r&0x80)^0x80;
  if (r & 0x80) f|= flagS;
  if (!r) f|= flagZ;
  if (a7 & nr7) f|= flagV;
  rf.W(f);
  tick(1);
  return resGO;
}


/* End of rxk.src/ialu.cc */
