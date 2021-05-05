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

#include "mcs6502cl.h"


int
cl_mcs6502::TYA(t_mem code)
{
  cA.W(rY);
  if (rA)
    rF&= ~flagZ;
  else
    rF|= flagZ;
  if (rA & 0x80)
    rF|= flagS;
  else
    rF&= ~flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::TAY(t_mem code)
{
  cY.W(rA);
  if (rY)
    rF&= ~flagZ;
  else
    rF|= flagZ;
  if (rY & 0x80)
    rF|= flagS;
  else
    rF&= ~flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::TXA(t_mem code)
{
  cA.W(rX);
  if (rA)
    rF&= ~flagZ;
  else
    rF|= flagZ;
  if (rA & 0x80)
    rF|= flagS;
  else
    rF&= ~flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::TXS(t_mem code)
{
  cSP.W(rX);
  tick(1);
  return resGO;
}

int
cl_mcs6502::TAX(t_mem code)
{
  cX.W(rA);
  if (rX)
    rF&= ~flagZ;
  else
    rF|= flagZ;
  if (rX & 0x80)
    rF|= flagS;
  else
    rF&= ~flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::TSX(t_mem code)
{
  cX.W(rSP);
  if (rX)
    rF&= ~flagZ;
  else
    rF|= flagZ;
  if (rX & 0x80)
    rF|= flagS;
  else
    rF&= ~flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}


/* End of mcs6502.src/imove.cc */
