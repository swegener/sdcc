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

#include "m68hc12cl.h"


int
CL12::sub16(class cl_cell16 &dest, u16_t op)
{
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  u16_t a= dest.R(), b= op, r;
  r= a-b;
  if (r&0x8000) f|= flagN;
  if (!r) f|= flagZ;
  if (( (~a&b)|(b&r)|(r&~a) ) & 0x8000) f|= flagC;
  if (( (a&~b&~r)|(~a&b&r) ) & 0x8000) f|= flagV;
  dest.W(r);
  cF.W(f);
  return resGO;
}

int
CL12::add16(class cl_cell16 &dest, u16_t op)
{
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  u16_t a= dest.R(), b= op, r;
  r= a+b;
  if (r&0x8000) f|= flagN;
  if (!r) f|= flagZ;
  if (( (a&b)|(b&~r)|(~r&a) ) & 0x8000) f|= flagC;
  if (( (a&b&~r)|(~a&~b&r) ) & 0x8000) f|= flagV;
  dest.W(r);
  cF.W(f);
  return resGO;
}

int
CL12::cp16(u16_t op1, u16_t op2)
{
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  u16_t a= op1, b= op2, r;
  r= a-b;
  if (r&0x8000) f|= flagN;
  if (!r) f|= flagZ;
  if (( (~a&b)|(b&r)|(r&~a) ) & 0x8000) f|= flagC;
  if (( (a&~b&~r)|(~a&b&r) ) & 0x8000) f|= flagV;
  cF.W(f);
  return resGO;
}

int
CL12::lsr16(class cl_memory_cell &dest)
{
  u16_t op= dest.R(), f= rF & ~(flagN|flagZ|flagV|flagC);
  if (op&1) f|= flagC|flagV;
  op>>= 1;
  dest.W(op);
  if (!op) f|= flagZ;
  cCC.W(f);
  return resGO;
}

int
CL12::asl16(class cl_memory_cell &dest)
{
  u16_t op= dest.R();
  u8_t f;
  f= rF & ~(flagN|flagZ|flagV|flagC);
  if (op&0x8000) f|= flagC;
  op<<= 1;
  dest.W(op);
  if (!op) f|= flagZ;
  if (op&0x8000) f|= flagN;
  if (((f&flagN)?1:0) ^ ((f&flagC)?1:0)) f|= flagV;
  cCC.W(f);
  return resGO;
}

int
CL12::inxy(class cl_memory_cell &dest)
{
  u8_t f= rF & ~flagZ, v;
  dest.W(v= dest.R()+1);
  if (!v)
    f|= flagZ;
  cF.W(f);
  return resGO;
}

int
CL12::dexy(class cl_memory_cell &dest)
{
  u8_t f= rF & ~flagZ, v;
  dest.W(v= dest.R()-1);
  if (!v)
    f|= flagZ;
  cF.W(f);
  return resGO;
}

int
CL12::ediv(void)
{
  u32_t op1= (((u32_t)rY)<<16)+rD;
  u16_t op2= rX;
  u32_t res, rem;
  u8_t f= rF & ~(flagN|flagZ|flagV|flagC);
  if (op2 == 0)
    f|= flagC;
  else
    {
      res= op1/op2;
      rem= op1%op2;
      if (res & 0x8000)
	f|= flagN;
      if (res > 0xffff)
	f|= flagV;
      res&= 0xffff;
      if (!res)
	f|= flagZ;
      cY.W(res);
      cD.W(rem);
    }
  cF.W(f);
  return resGO;
}

int
CL12::mul(void)
{
  u8_t f= rF&~flagC;
  rD= rA*rB;
  if (rB & 0x80)
    f|= flagC;
  cD.W(rD);
  cF.W(f);
  return resGO;
}

int
CL12::emul(void)
{
  u8_t f= rF&~(flagC|flagZ|flagN);
  u32_t res= rD*rY;
  if (res & 0x8000)
    f|= flagC;
  if (res & 0x80000000)
    f|= flagN;
  if (!res)
    f|= flagZ;
  cD.W(res);
  cY.W(res>>16);
  cF.W(f);
  return resGO;
}

int
CL12::daa(void)
{
  u8_t ah= rA>>4;
  u8_t al= rA&0xf;
  int C= rF&flagC;
  int H= rF&flagH;
  u8_t nc= 0;

  if (!C && (ah>=0x0) && (ah<=0x9) && !H && (al>=0x0) && (al<=0x9)) { rA+= 0x00; nc= 0; }
  if (!C && (ah>=0x0) && (ah<=0x8) && !H && (al>=0xa) && (al<=0xf)) { rA+= 0x06; nc= 0; }
  if (!C && (ah>=0x0) && (ah<=0x9) &&  H && (al>=0x0) && (al<=0x3)) { rA+= 0x06; nc= 0; }

  if (!C && (ah>=0xa) && (ah<=0xf) && !H && (al>=0x0) && (al<=0x9)) { rA+= 0x60; nc= 1; }
  if (!C && (ah>=0x9) && (ah<=0xf) && !H && (al>=0xa) && (al<=0xf)) { rA+= 0x66; nc= 1; }
  if (!C && (ah>=0xa) && (ah<=0xf) &&  H && (al>=0x0) && (al<=0x3)) { rA+= 0x66; nc= 1; }

  if ( C && (ah>=0x0) && (ah<=0x2) && !H && (al>=0x0) && (al<=0x9)) { rA+= 0x60; nc= 1; }
  if ( C && (ah>=0x0) && (ah<=0x2) && !H && (al>=0xa) && (al<=0xf)) { rA+= 0x66; nc= 1; }
  if ( C && (ah>=0x0) && (ah<=0x3) &&  H && (al>=0x0) && (al<=0x3)) { rA+= 0x66; nc= 1; }

  rF&= ~(flagN|flagZ|flagC);
  if (nc) rF|= flagC;
  if (rA==0) rF|= flagZ;
  if (rA&0x80) rF|= flagN;
  cF.W(rF);
  cA.W(rA);
  return resGO;
}


/* End of m68hc12.src/ialu.cc */
