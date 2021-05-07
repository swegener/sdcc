/*
 * Simulator of microcontrollers (inst.cc)
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
cl_rxk::ALTD(t_mem code)
{
  altd= 2;
  tick(1);
  return resGO;
}

int
cl_rxk::IOI(t_mem code)
{
  io_prefix= true;
  rwas= ioi;
  tick(1);
  return resGO;
}

int
cl_rxk::IOE(t_mem code)
{
  io_prefix= true;
  rwas= ioe;
  tick(1);
  return resGO;
}

int
cl_rxk::NOP(t_mem code)
{
  tick(1);
  return resGO;
}

/* End of m6800.src/inst.cc */
