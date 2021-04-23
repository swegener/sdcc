/*
 * Simulator of microcontrollers (@@F@@)
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

#include "rmemcl.h"


cl_ras::cl_ras(chars id, class cl_memory_chip *achip):
  cl_address_space(id, 0, 0x10000, 8)
{
  chip= achip;
}

t_addr
cl_ras::log2phy(t_addr log)
{
  u8_t h= (log >> 12)&0xf;
  if (h < (segsize&0xf))
    {
      // code space
      return log;
    }
  if (h < (segsize>>4))
    {
      // data space
      return log + (dataseg<<12);
    }
  if (h < 0xe)
    {
      // stack space
      return log + (stackseg<<12);
    }
  // else
  // extended program space
  return log + (xpc<<12);
}

t_mem
cl_ras::read(t_addr addr)
{
  return chip->read(log2phy(addr));
}

t_mem
cl_ras::get(t_addr addr)
{
  return chip->get(log2phy(addr));
}

t_mem
cl_ras::write(t_addr addr, t_mem val)
{
  return chip->write(log2phy(addr), val);
}

void
cl_ras::set(t_addr addr, t_mem val)
{
  chip->set(log2phy(addr), val);
}

void
cl_ras::download(t_addr addr, t_mem val)
{
  chip->set(log2phy(addr), val);
}


/* End of rxk.src/rmem.cc */
