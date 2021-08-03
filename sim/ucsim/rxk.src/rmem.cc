/*
 * Simulator of microcontrollers (@@F@@)
 *
 * Copyright (C) 2020,2021 Drotos Daniel, Talker Bt.
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

int
cl_ras::init(void)
{
  cl_address_space::init();
  return 0;
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

t_addr
cl_ras::px2phy(u32_t px)
{
  if ((px & 0xffff0000) == 0xffff0000)
    {
      return log2phy(px & 0xffff);
    }
  return px;
}

t_mem
cl_ras::read(t_addr addr)
{
  if (addr >= size)
    {
      err_inv_addr(addr);
      return dummy->read();
    }
  return cella[addr].read();
}

t_mem
cl_ras::pxread(t_addr pxaddr)
{
  if ((pxaddr & 0xffff0000) == 0xffff0000)
    return read(pxaddr & 0xffff);
  return phread(pxaddr);
}

t_mem
cl_ras::get(t_addr addr)
{
  if (addr >= size)
    {
      err_inv_addr(addr);
      return dummy->get();
    }
  return cella[addr].get();
}

t_mem
cl_ras::phget(t_addr phaddr)
{
  if (phaddr >= chip->get_size())
    {
      err_inv_addr(phaddr);
      return dummy->read();
    }
  u8_t *slot= (u8_t*)(chip->get_slot(phaddr));
  return *slot;
}

t_mem
cl_ras::write(t_addr addr, t_mem val)
{
  if (addr >= size)
    {
      err_inv_addr(addr);
      return dummy->write(val);
    }
  return cella[addr].write(val);
}

t_mem
cl_ras::pxwrite(t_addr pxaddr, t_mem val)
{
  if ((pxaddr & 0xffff0000) == 0xffff0000)
    return write(pxaddr & 0xffff, val);
  return phwrite(pxaddr, val);
}

void
cl_ras::set(t_addr addr, t_mem val)
{
  if (addr >= size)
    {
      err_inv_addr(addr);
      dummy->set(val);
    }
  cella[addr].set(val);
}

void
cl_ras::phset(t_addr phaddr, t_mem val)
{
  if (phaddr >- chip->get_size())
    {
      err_inv_addr(phaddr);
      dummy->set(val);
    }
  u8_t *slot= (u8_t*)(chip->get_slot(phaddr));
  *slot= val;
}

void
cl_ras::download(t_addr phaddr, t_mem val)
{
  if (phaddr >= chip->get_size())
    {
      err_inv_addr(phaddr);
      dummy->set(val);
    }
  chip->set(phaddr, val);
}


void
cl_ras::re_decode(void)
{
  t_addr a;
  for (a= 0; a < get_size(); a++)
    {
      t_addr ph= log2phy(a);
      void *slot= chip->get_slot(ph);
      cella[a].decode(slot);
    }
}

void
cl_ras::set_xpc(u8_t val)
{
  xpc= val;
  re_decode();
}

void
cl_ras::set_segsize(u8_t val)
{
  segsize= val;
  re_decode();
}

void
cl_ras::set_dataseg(u8_t val)
{
  dataseg= val;
  re_decode();
}

void
cl_ras::set_stackseg(u8_t val)
{
  stackseg= val;
  re_decode();
}

/* End of rxk.src/rmem.cc */
