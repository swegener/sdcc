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

#include "mcs6502cl.h"

int
cl_mcs6502::NOP(t_mem code)
{
  return resGO;
}

int
cl_mcs6502::BRK(t_mem code)
{
  src_brk->request();
  cF.W(flagB);
  return resGO;
}

int
cl_mcs6502::RTI(t_mem code)
{
  u8_t l, h;
  rSP++;
  cF.W(rom->read(0x0100 + rSP));
  rSP++;
  l= rom->read(0x0100 + rSP);
  rSP++;
  h= rom->read(0x0100 + rSP);
  vc.rd+= 3;
  PC= h*256 + l;
  tick(5);
  return resGO;
}

int
cl_mcs6502::CLI(t_mem code)
{
  rF&= ~flagI;
  tick(1);
  return resGO;
}

int
cl_mcs6502::SEI(t_mem code)
{
  rF|= flagI;
  tick(1);
  return resGO;
}

int
cl_mcs6502::PHP(t_mem code)
{
  rom->write(0x0100 + rSP, rF);
  vc.wr++;
  rSP--;
  tick(2);
  return resGO;
}

int
cl_mcs6502::CLC(t_mem code)
{
  rF&= ~flagC;
  tick(1);
  return resGO;
}

int
cl_mcs6502::PLP(t_mem code)
{
  rSP++;
  cF.W(rom->read(0x0100 + rSP));
  vc.rd++;
  tick(3);
  return resGO;
}

int
cl_mcs6502::SEc(t_mem code)
{
  rF|= flagC;
  tick(1);
  return resGO;
}

int
cl_mcs6502::PHA(t_mem code)
{
  rom->write(0x0100 + rSP, rA);
  vc.wr++;
  rSP--;
  tick(2);
  return resGO;
}

int
cl_mcs6502::PLA(t_mem code)
{
  rSP++;
  cA.W(rom->read(0x0100 + rSP));
  vc.rd++;
  tick(3);
  return resGO;
}

int
cl_mcs6502::CLV(t_mem code)
{
  rF&= ~flagV;
  tick(1);
  return resGO;
}

int
cl_mcs6502::CLD(t_mem code)
{
  rF&= ~flagD;
  tick(1);
  return resGO;
}


int
cl_mcs6502::SED(t_mem code)
{
  rF|= flagD;
  tick(1);
  return resGO;
}


/* End of mcs6502.src/inst.cc */
