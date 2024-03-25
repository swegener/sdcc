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

#define PDK16 cl_pdk16

PDK16::PDK16(class cl_sim *asim):
  cl_pdk(asim)
{
  PCmask= 0xffff;
}

int
PDK16::init(void)
{
  cl_pdk::init();
  return 0;
}

const char *
PDK16::id_string(void)
{
  return "pdk16";
}

void
PDK16::reset(void)
{
  cl_pdk::reset();
}



/* End of pdk.src/pdk16.cc */
