/*
 * Simulator of microcontrollers (tlcs.src/inst_rot_sh.cc)
 *
 * Copyright (C) 2016,16 Drotos Daniel, Talker Bt.
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

/* $Id: inst_rot_sh.cc 228 2016-06-21 11:49:57Z  $ */

#include "tlcscl.h"


// RLC 8-bit
uint8_t
cl_tlcs::op_rlc(uint8_t data, bool set_sz)
{
  uint8_t c= data & 0x80;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  data<<= 1;
  if (c)
    {
      data|= 1;
      reg.f|= FLAG_C;
    }

  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// RLC mem
uint8_t
cl_tlcs::inst_rlc(cl_memory_cell *cell)
{
  uint8_t d= op_rlc(cell->read(), true);
  cell->write(d);
  return d;
}


// RRC 8-bit
uint8_t
cl_tlcs::op_rrc(uint8_t data, bool set_sz)
{
  uint8_t c= data & 0x01;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  data>>= 1;
  if (c)
    {
      data|= 0x80;
      reg.f|= FLAG_C;
    }

  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// RRC mem
uint8_t
cl_tlcs::inst_rrc(cl_memory_cell *cell)
{
  uint8_t d= op_rrc(cell->read(), true);
  cell->write(d);
  return d;
}


// RL 8-bit
uint8_t
cl_tlcs::op_rl(uint8_t data, bool set_sz)
{
  uint8_t c= data & 0x80;
  data<<= 1;
  if (reg.f & FLAG_C)
    data|= 1;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  if (c)
    reg.f|= FLAG_C;

  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// RL mem
uint8_t
cl_tlcs::inst_rl(cl_memory_cell *cell)
{
  uint8_t d= op_rl(cell->read(), true);
  cell->write(d);
  return d;
}


// RR 8-bit
uint8_t
cl_tlcs::op_rr(uint8_t data, bool set_sz)
{
  uint8_t c= data & 0x01;
  data>>= 1;
  if (reg.f & FLAG_C)
    data|= 0x80;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  if (c)
    reg.f|= FLAG_C;

  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// RR mem
uint8_t
cl_tlcs::inst_rr(cl_memory_cell *cell)
{
  uint8_t d= op_rr(cell->read(), true);
  cell->write(d);
  return d;
}


// SLA 8-bit
uint8_t
cl_tlcs::op_sla(uint8_t data, bool set_sz)
{
  uint8_t c= data & 0x80;
  data<<= 1;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  if (c)
    reg.f|= FLAG_C;

  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// SLA mem
uint8_t
cl_tlcs::inst_sla(cl_memory_cell *cell)
{
  uint8_t d= op_sla(cell->read(), true);
  cell->write(d);
  return d;
}


// SRA 8-bit
uint8_t
cl_tlcs::op_sra(uint8_t data, bool set_sz)
{
  uint8_t c7= data & 0x80;
  uint8_t c0= data & 0x01;
  data>>= 1;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  if (c0)
    reg.f|= FLAG_C;
  data|= c7;
  
  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// SRA mem
uint8_t
cl_tlcs::inst_sra(cl_memory_cell *cell)
{
  uint8_t d= op_sra(cell->read(), true);
  cell->write(d);
  return d;
}


// SRL 8-bit
uint8_t
cl_tlcs::op_srl(uint8_t data, bool set_sz)
{
  uint8_t c0= data & 0x01;
  data>>= 1;
  reg.f&= ~((set_sz?(FLAG_S|FLAG_Z):0)|FLAG_H|FLAG_N|FLAG_C);
  if (c0)
    reg.f|= FLAG_C;
  
  if (set_sz)
    {
      if (!data)
	reg.f|= FLAG_Z;
      if (data&0x80)
	reg.f|= FLAG_S;
      set_p(data);
    }
  
  return data;
}


// SRL mem
uint8_t
cl_tlcs::inst_srl(cl_memory_cell *cell)
{
  uint8_t d= op_srl(cell->read(), true);
  cell->write(d);
  return d;
}


// RLD mem
int
cl_tlcs::inst_rld(class cl_memory_cell *cell)
{
  reg.f&= ~(FLAG_H|FLAG_X|FLAG_N);

  uint8_t c= cell->read();
  uint8_t temp= reg.a & 0x0f;
  reg.a= (reg.a & 0xf0) + (c >> 4);
  cell->write((c << 4) + temp);
  set_p(reg.a);
  return resGO;
}


// RRD mem
int
cl_tlcs::inst_rrd(class cl_memory_cell *cell)
{
  reg.f&= ~(FLAG_H|FLAG_X|FLAG_N);

  uint8_t c= cell->read();
  uint8_t temp= reg.a & 0x0f;
  reg.a= (reg.a & 0xf0) + (c & 0x0f);
  cell->write((temp << 4) + (c >> 4));
  set_p(reg.a);
  return resGO;
}


/* End of tlcs.src/inst_rot_sh.cc */
