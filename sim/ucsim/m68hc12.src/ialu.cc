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

/* End of m68hc12.src/ialu.cc */
