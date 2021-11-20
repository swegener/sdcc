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

#include "mcs6502cl.h"


int
cl_mcs6502::INY(t_mem code)
{
  cY.W(rY+1);
  rF&= ~(flagZ|flagS);
  if (!rY) rF|= flagZ;
  if (rY & 0x80) rF|= flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::INX(t_mem code)
{
  cX.W(rX+1);
  rF&= ~(flagZ|flagS);
  if (!rX) rF|= flagZ;
  if (rX & 0x80) rF|= flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::inc(class cl_cell8 &op)
{
  u8_t v;
  op.W(v= op.R()+1);
  rF&= ~(flagZ|flagS);
  if (!v) rF|= flagZ;
  if (v & 0x80) rF|= flagS;
  cF.W(rF);
  return resGO;
}

int
cl_mcs6502::DEY(t_mem code)
{
  cY.W(rY-1);
  rF&= ~(flagZ|flagS);
  if (!rY) rF|= flagZ;
  if (rY & 0x80) rF|= flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::DEX(t_mem code)
{
  cX.W(rX-1);
  rF&= ~(flagZ|flagS);
  if (!rX) rF|= flagZ;
  if (rX & 0x80) rF|= flagS;
  cF.W(rF);
  tick(1);
  return resGO;
}

int
cl_mcs6502::dec(class cl_cell8 &op)
{
  u8_t v;
  op.W(v= op.R()-1);
  rF&= ~(flagZ|flagS);
  if (!v) rF|= flagZ;
  if (v & 0x80) rF|= flagS;
  cF.W(rF);
  return resGO;
}

int
cl_mcs6502::ora(class cl_cell8 &op)
{
  u8_t f= rF & ~(flagZ|flagS);
  cA.W(rA | op.R());
  if (!rA) f|= flagZ;
  if (rA&0x80) f|= flagS;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::And(class cl_cell8 &op)
{
  u8_t f= rF & ~(flagZ|flagS);
  cA.W(rA & op.R());
  if (!rA) f|= flagZ;
  if (rA&0x80) f|= flagS;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::eor(class cl_cell8 &op)
{
  u8_t f= rF & ~(flagZ|flagS);
  cA.W(rA ^ op.R());
  if (!rA) f|= flagZ;
  if (rA&0x80) f|= flagS;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::adc(class cl_cell8 &op)
{
  u8_t Op= op.R(), f, oA= rA;
  u16_t res;
  u8_t C= (rF&flagC)?1:0;
  f= rF & ~(flagZ|flagC|flagN|flagV);

  if (!(rF & flagD))
    {
      res= rA + Op + C;
      cA.W(res);
      if (!rA) f|= flagZ;
      if (rA & 0x80) f|= flagN;
      if (res > 255) f|= flagC;
      if ( ((res^oA)&0x80) && !((oA^Op)&0x80) ) f|= flagV;
    }
  else
    {
      u8_t ah, al;
      al= (rA & 0xf) + (Op & 0xf) + C;
      ah= (rA >> 4) + (Op >> 4) + ((al>15)?1:0);
      if (al > 9) al+= 6;
      if ((rA + Op + C) & 0xff) f|= flagZ;

      if (ah & 8) f|= flagN;
      if ( (((ah << 4) ^ rA) & 0x80) && !((rA ^ Op) & 0x80))  f|= flagV;

      if (ah > 9) ah+= 6;
      if (ah > 15) f|= flagC;
      cA.W((ah << 4) | (al & 0xf));
    }
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::sbc(class cl_cell8 &op)
{
  u8_t Op= op.R();
  u16_t res;
  u8_t C= (rF&flagC)?1:0;
  u8_t f= rF & ~(flagC|flagZ|flagV|flagN);
  
  res= rA-Op-1+C;
  if (0x80&( (res&Op&~rA) | (~res&~Op&rA) )) f|= flagV;
  if (res < 0x100) f|= flagC;
  if (!(res & 0xff)) f|= flagZ;
  if (res & 0x80) f|= flagN;
  if (!(rF & flagD))
    cA.W(res);
  else
    {
      u8_t ah, al;
      al= (rA&0xf)-(Op&0xf)-1+C;
      if (al&16) al-= 6;
      ah= (rA>>4)-(Op>>4)-(al&16);
      if (ah&16) ah-= 6;
      cA.W( ((ah&0xf)<<4)|(al&0xf) );
    }
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::cmp(class cl_cell8 &op1, class cl_cell8 &op2)
{
  u16_t res= op1.R() - op2.R();
  u8_t f= rF & ~(flagZ|flagN|flagC);
  if (res < 256) f|= flagC;
  if (!(res&0xff)) f|= flagZ;
  if (res&0x80) f|= flagN;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::asl(class cl_cell8 &op)
{
  u8_t f= rF & ~(flagZ|flagN|flagC);
  u8_t v= op.R();
  if (v&0x80) f|= flagC;
  op.W(v<<=1);
  if (!v) f|= flagZ;
  if (v&0x80) f|= flagN;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::lsr(class cl_cell8 &op)
{
  u8_t f= rF & ~(flagZ|flagN|flagC);
  u8_t v= op.R();
  if (v&1) f|= flagC;
  op.W(v>>=1);
  if (!v) f|= flagZ;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::rol(class cl_cell8 &op)
{
  u8_t C= (rF&flagC)?1:0;
  u8_t f= rF & ~(flagZ|flagN|flagC);
  u8_t v= op.R();
  if (v&0x80) f|= flagC;
  v= (v<<1)|C;
  op.W(v);
  if (!v) f|= flagZ;
  if (v&0x80) f|= flagN;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::ror(class cl_cell8 &op)
{
  u8_t C= (rF&flagC)?0x80:0;
  u8_t f= rF & ~(flagZ|flagN|flagC);
  u8_t v= op.R();
  if (v&1) f|= flagC;
  v= (v>>1)|C;
  op.W(v);
  if (!v) f|= flagZ;
  if (v&0x80) f|= flagN;
  cF.W(f);
  return resGO;
}

int
cl_mcs6502::bit(class cl_cell8 &op)
{
  u8_t v= op.R();
  u8_t f= rF & ~(flagZ|flagN|flagV);
  if (v&0x80) f|= flagN;
  if (v&0x40) f|= flagV;
  if (!(rA & v)) f|= flagZ;
  cF.W(f);
  return resGO;
}


/* End of mcs6502.src/ialu.cc */
