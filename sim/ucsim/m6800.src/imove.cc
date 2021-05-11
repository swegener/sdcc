/*
 * Simulator of microcontrollers (imove.cc)
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

#include "m6800cl.h"


int
cl_m6800::clr(class cl_cell8 &dest)
{
  u8_t f= rF & ~(flagN|flagV|flagC);
  dest.W(0);
  f|= flagZ;
  cCC.W(f);
  tick(1);
  return resGO;
}


int
cl_m6800::TAP(t_mem code)
{
  cF.W(rA);
  tick(1);
  return resGO;
}

int
cl_m6800::TPA(t_mem code)
{
  cA.W(rF);
  tick(1);
  return resGO;
}

int
cl_m6800::TAB(t_mem code)
{
  u8_t f= rCC & ~(flagN|flagZ|flagV);
  cB.W(rA);
  if (!rA) f|= flagZ;
  if (rA&0x80) f|= flagN;
  cCC.W(f);
  return resGO;
}

int
cl_m6800::TBA(t_mem code)
{
  u8_t f= rF & ~(flagN|flagZ|flagV);
  cA.W(rB);
  if (!rB) f|= flagZ;
  if (rB&0x80) f|= flagN;
  cF.W(f);
  return resGO;
}

int
cl_m6800::TSX(t_mem code)
{
  cIX.W(rSP+1);
  tick(3);
  return resGO;
}

int
cl_m6800::PULA(t_mem code)
{
  cSP.W(rSP+1);
  cA.W(rom->read(rSP));
  tick(3);
  return resGO;
}

int
cl_m6800::PULB(t_mem code)
{
  cSP.W(rSP+1);
  cB.W(rom->read(rSP));
  tick(3);
  return resGO;
}

int
cl_m6800::TXS(t_mem code)
{
  cSP.W(rIX-1);
  tick(3);
  return resGO;
}

int
cl_m6800::PSHA(t_mem code)
{
  rom->write(rSP, rA);
  cSP.W(rSP-1);
  tick(3);
  return resGO;
}

int
cl_m6800::PSHB(t_mem code)
{
  rom->write(rSP, rB);
  cSP.W(rSP-1);
  tick(3);
  return resGO;
}


/* End of m6800.src/imove.cc */
