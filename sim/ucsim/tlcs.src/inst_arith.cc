/*
 * Simulator of microcontrollers (tlcs.src/inst_arith.cc)
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

/* $Id: inst_arith.cc 312 2016-07-06 06:26:43Z  $ */

#include "tlcscl.h"


// INC 8-bit
uint8_t
cl_tlcs::op_inc(uint8_t data)
{
  uint16_t n= data+1;
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X);

  if (n > 255)
    reg.f|= FLAG_X;
  if (n & 0x80)
    reg.f|= FLAG_S;
  if ((n & 0xff) == 0)
    reg.f|= FLAG_Z;
  if (data == 0x7f)
    reg.f|= FLAG_V;
  if ((n & 0x0f) == 0x00)
    reg.f|= FLAG_H;

  return n;
}


// INC mem
void
cl_tlcs::inst_inc(cl_memory_cell *cell)
{
  uint8_t d= cell->read();
  d= op_inc(d);
  cell->write(d);
}


// INCX mem
void
cl_tlcs::inst_incx(cl_memory_cell *cell)
{
  if (reg.f & FLAG_X)
    {
      uint8_t d= cell->read();
      d= op_inc(d);
      cell->write(d);
    }
}


// INC 8-bit
uint8_t
cl_tlcs::op_dec(uint8_t data)
{
  uint16_t n= data-1;
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X);
  reg.f|= FLAG_N;
  
  if (n > 255)
    reg.f|= FLAG_X;
  if (n & 0x80)
    reg.f|= FLAG_S;
  if ((n & 0xff) == 0)
    reg.f|= FLAG_Z;
  if (data == 0x80)
    reg.f|= FLAG_V;
  if ((n & 0x0f) == 0x00)
    reg.f|= FLAG_H;

  return n & 0xff;
}


// DEC mem
void
cl_tlcs::inst_dec(cl_memory_cell *cell)
{
  uint8_t d= cell->read();
  d= op_dec(d);
  cell->write(d);
}


// DECX mem
void
cl_tlcs::inst_decx(cl_memory_cell *cell)
{
  if (reg.f & FLAG_X)
    {
      uint8_t d= cell->read();
      d= op_dec(d);
      cell->write(d);
    }
}


// INC 16-bit
uint16_t
cl_tlcs::op_inc16(uint16_t data)
{
  uint16_t n= data+1;
  reg.f&= ~(FLAG_X);

  if (n == 0)
    reg.f|= FLAG_X;

  return n;
}


// INCW mem
uint16_t
cl_tlcs::inst_inc16gg(uint8_t gg, t_addr addr)
{
  cl_address_space *as= nas;

  if ((gg&7)==4)
    as= xas;
  else if ((gg&7)==5)
    as= yas;
  uint8_t l= as->read(addr);
  uint8_t h= as->read(addr+1);
  uint16_t d= h*256 + l;

  if (((int)d + 1) > 0xffff)
    reg.f|= FLAG_V;
  
  d= op_inc16(d);
  reg.f&= ~FLAG_N;
  if (d & 0x8000)
    reg.f|= FLAG_S;
  if (d == 0)
    reg.f|= FLAG_Z;

  as->write(addr, d & 0xff);
  as->write(addr+1, d >> 8);
  
  return d;
}


// INCW mem
uint16_t
cl_tlcs::inst_inc16(t_addr addr)
{
  return inst_inc16gg(0, addr);
}


// INCW mem
uint16_t
cl_tlcs::inst_inc16ix(uint8_t ix, t_addr addr)
{
  if ((ix&3) == 0)
    return inst_inc16gg(4, addr);
  else if ((ix&3) == 1)
    return inst_inc16gg(5, addr);
  return inst_inc16gg(0, addr);
}


// DEC 16-bit
uint16_t
cl_tlcs::op_dec16(t_mem data)
{
  uint16_t n= data-1;
  reg.f&= ~(FLAG_X);

  if (n == 0xffff)
    reg.f|= FLAG_X;

  return n;
}


// DECW mem
uint16_t
cl_tlcs::inst_dec16gg(uint8_t gg, t_addr addr)
{
  class cl_address_space *as= nas;

  if ((gg&7)==4)
    as= xas;
  else if ((gg&7)==5)
    as= yas;
  uint8_t l= as->read(addr);
  uint8_t h= as->read(addr+1);
  uint16_t d= h*256 + l;

  if (((int)d - 1) < 0)
    reg.f|= FLAG_V;
  
  d= op_dec16(d);
  reg.f&= ~FLAG_N;
  if (d & 0x8000)
    reg.f|= FLAG_S;
  if (d == 0)
    reg.f|= FLAG_Z;

  as->write(addr, d & 0xff);
  as->write(addr+1, d >> 8);
  
  return d;
}


// DECW mem
uint16_t
cl_tlcs::inst_dec16(t_addr addr)
{
  return inst_dec16gg(0, addr);
}


// DECW mem
uint16_t
cl_tlcs::inst_dec16ix(uint8_t ix, t_addr addr)
{
  if ((ix&3)==0)
    return inst_dec16gg(4, addr);
  else if ((ix&3)==1)
    return inst_dec16gg(5, addr);
  return inst_dec16gg(0, addr);
}


// ADD 8-bit
uint8_t
cl_tlcs::op_add8(uint8_t d1, uint8_t d2)
{
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_V|FLAG_N|FLAG_C);

  int r= d1 + d2;
  int new_c= 0, new_c6;
  
  if (((d1 & 0xf) + (d2 & 0xf)) > 0xf)
    reg.f|= FLAG_H;
  new_c6= (((d1&0x7f) + (d2&0x7f)) > 0x7f)?1:0;
  
  if (r & 0x80)
    reg.f|= FLAG_S;
  if ((r&0xff) == 0)
    reg.f|= FLAG_Z;
  if (r > 255)
    {
      reg.f|= FLAG_X|FLAG_C;
      new_c= 1;
    }
  if (new_c ^ new_c6)
    reg.f|= FLAG_V;

  return r & 0xff;
}


// ADD A,8-bit
uint8_t
cl_tlcs::op_add_a(uint8_t d)
{
  return op_add8(reg.a, d);
}


// ADD A,mem
int
cl_tlcs::inst_add_a(class cl_memory_cell *cell)
{
  reg.a= op_add_a((uint8_t)(cell->read()));
  return resGO;
}


// ADC 8-bit
uint8_t
cl_tlcs::op_adc8(uint8_t d1, uint8_t d2)
{
  int oldc= (reg.f&FLAG_C)?1:0;
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_V|FLAG_N|FLAG_C);

  int r= d1 + d2 + oldc;
  int new_c= 0, new_c6;
  
  if (((d1 & 0xf) + (d2 & 0xf) + oldc) > 0xf)
    reg.f|= FLAG_H;
  new_c6= (((d1&0x7f) + (d2&0x7f) + oldc) > 0x7f)?1:0;
  
  if (r & 0x80)
    reg.f|= FLAG_S;
  if ((r & 0xff) == 0)
    reg.f|= FLAG_Z;
  if (r > 255)
    {
      reg.f|= FLAG_X|FLAG_C;
      new_c= 1;
    }
  if (new_c ^ new_c6)
    reg.f|= FLAG_V;

  return r;
}


// ADC A,8-bit
int
cl_tlcs::inst_adc_a(uint8_t d)
{
  reg.a= op_adc8(reg.a, d);
  return resGO;
}


// ADC A,mem
int
cl_tlcs::inst_adc_a(class cl_memory_cell *cell)
{
  return inst_adc_a((uint8_t)(cell->read()));
}


// SUB 8-bit
uint8_t
cl_tlcs::op_sub8(uint8_t d1, uint8_t d2)
{
  unsigned int op1= (unsigned int)d1;
  unsigned int op2= (unsigned int)d2;
  signed int res= (signed char)d1 - (signed char)d2;
  uint8_t r;
  
  reg.f&= ~(FLAG_H|FLAG_V|FLAG_C|FLAG_Z|FLAG_S);
  reg.f|= FLAG_N;

  if ((op1 & 0xf) < (op2 & 0xf))
    reg.f|= FLAG_H;
  if ((res < -128) || (res > 127))
    reg.f|= FLAG_V;
  if (op1 < op2)
    reg.f|= FLAG_C|FLAG_X;

  r= d1 - op2;
  //r= op_add8(d1, ~d2 + 1);
  if (r == 0)
    reg.f|= FLAG_Z;
  if (r & 0x80)
    reg.f|= FLAG_S;
  
  //reg.f|= FLAG_N;
  return r;
}


// SUB A,8-bit
int
cl_tlcs::inst_sub_a(uint8_t d)
{
  reg.a= op_sub8(reg.a, d);
  return resGO;
}


// SUB A,mem
int
cl_tlcs::inst_sub_a(class cl_memory_cell *cell)
{
  return inst_sub_a((uint8_t)(cell->read()));
}


// SBC 8-bit
uint8_t
cl_tlcs::op_sbc8(uint8_t d1, uint8_t d2)
{
  uint8_t r;
  unsigned int op1= (unsigned int)d1;
  unsigned int op2= (unsigned int)d2;
  signed int res= (signed char)d1 - (signed char)d2;

  if (reg.f & FLAG_C)
    {
      ++op2;
      --res;
    }
  reg.f&= ~(FLAG_H|FLAG_V|FLAG_C|FLAG_S|FLAG_Z);
  reg.f|= FLAG_N;

  if ((op1 & 0xf) < (op2 & 0xf))
    reg.f|= FLAG_H;
  if ((res < -128) || (res > 127))
    reg.f|= FLAG_V;
  if (d1 < op2)
    reg.f|= FLAG_C;

  r= d1 - op2;

  if (r == 0)
    reg.f|= FLAG_Z;
  if (r & 0x80)
    reg.f|= FLAG_S;
  
  //r= op_adc8(d1, ~d2 + 1);
  //reg.f|= FLAG_N;
  return r;
}


// SBC A,8-bit
int
cl_tlcs::inst_sbc_a(uint8_t d)
{
  reg.a= op_sbc8(reg.a, d);
  return resGO;
}


// SBC A,mem
int
cl_tlcs::inst_sbc_a(class cl_memory_cell *cell)
{
  return inst_sbc_a((uint8_t)(cell->read()));
}


// AND 8-bit
uint8_t
cl_tlcs::op_and8(uint8_t d1, uint8_t d2)
{
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_X|FLAG_N|FLAG_C);
  reg.f|= FLAG_H;

  uint8_t r= d1 & d2;
  set_p(r);
  if (r & 0x80)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;

  return r;
}


// AND A,8-bit
int
cl_tlcs::inst_and_a(uint8_t d)
{
  reg.a= op_and8(reg.a, d);
  return resGO;
}


// AND A,mem
int
cl_tlcs::inst_and_a(class cl_memory_cell *cell)
{
  return inst_and_a((uint8_t)(cell->read()));
}


// XOR 8-bit
uint8_t
cl_tlcs::op_xor8(uint8_t d1, uint8_t d2)
{
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_N|FLAG_C);

  uint8_t r= d1 ^ d2;
  set_p(r);
  if (r & 0x80)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;

  return r;
}


// XOR A,8-bit
int
cl_tlcs::inst_xor_a(uint8_t d)
{
  reg.a= op_xor8(reg.a, d);
  return resGO;
}


// XOR A,mem
int
cl_tlcs::inst_xor_a(class cl_memory_cell *cell)
{
  return inst_xor_a((uint8_t)(cell->read()));
}


// OR 8-bit
uint8_t
cl_tlcs::op_or8(uint8_t d1, uint8_t d2)
{
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_N|FLAG_C);

  uint8_t r= d1 | d2;
  set_p(r);
  if (r & 0x80)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;

  return r;
}


// OR A,8-bit
int
cl_tlcs::inst_or_a(uint8_t d)
{
  reg.a= op_or8(reg.a, d);
  return resGO;
}


// OR A,mem
int
cl_tlcs::inst_or_a(class cl_memory_cell *cell)
{
  return inst_or_a((uint8_t)(cell->read()));
}


// CP 8-bit
uint8_t
cl_tlcs::op_cp8(uint8_t d1, uint8_t d2)
{
  uint8_t r= op_sub8(d1, d2);
  reg.f|= FLAG_N;
  return r;
}


// CP A,8-bit
int
cl_tlcs::op_cp_a(uint8_t d)
{
  op_cp8(reg.a, d);
  return resGO;
}


// CP A,mem
int
cl_tlcs::op_cp_a(class cl_memory_cell *cell)
{
  return op_cp_a((uint8_t)(cell->read()));
}


// ADD 16-bit
uint16_t
cl_tlcs::op_add16(t_mem op1, t_mem op2)
{
  uint16_t d1, d;
  int r, newc15;
  
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_X|FLAG_N|FLAG_C);

  d1= op1;
  d= op2;

  r= d1 + d;
  newc15= (((d1&0x7fff)+(d&0x7fff)) > 0x7fff)?0x10000:0;
  
  if (r & 0x8000)
    reg.f|= FLAG_S;
  if ((r & 0xffff) == 0)
    reg.f|= FLAG_Z;
  if (r > 0xffff)
    reg.f|= FLAG_C|FLAG_X;
  if (newc15 ^ (r&0x10000))
    reg.f|= FLAG_V;
  
  return r & 0xffff;
}


// ADD HL,mem
uint16_t
cl_tlcs::op_add_hl(t_addr addr)
{
  uint8_t dh, dl;
  uint16_t d;
  
  dl= nas->read(addr);
  dh= nas->read(addr+1);
  d= dh*256 + dl;

  return op_add16(reg.hl, d);
}


// ADD HL,16-bit
uint16_t
cl_tlcs::op_add_hl(t_mem val)
{
  return op_add16(reg.hl, val);
}


// ADC HL,mem
uint16_t
cl_tlcs::op_adc_hl(t_mem val)
{
  uint8_t dl= val & 0xff;
  uint8_t dh= val / 256;
  uint16_t d= dh*256 + dl;
  int oldc= (reg.f & FLAG_C)?1:0;
  
  return op_add_hl((t_mem)d + oldc);
}


// ADC HL,mem
uint16_t
cl_tlcs::op_adc_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;
  int oldc= (reg.f & FLAG_C)?1:0;
  
  return op_add_hl((t_mem)d + oldc);
}


// SUB 16-bit
uint16_t
cl_tlcs::op_sub16(t_mem d1, t_mem d2)
{
  uint16_t r;

  unsigned int op1= (unsigned int)d1;
  unsigned int op2= (unsigned int)d2;
  signed int res= (int16_t)d1 - (int16_t)d2;

  reg.f&= ~(FLAG_C|FLAG_V|FLAG_Z|FLAG_S);
  reg.f|= FLAG_N;
  
  if ((res < -32768) || (res > 32767))
    reg.f|= FLAG_V;
  if (op1 < op2)
    reg.f|= FLAG_C|FLAG_X;

  r= d1 - op2;

  if (r == 0)
    reg.f|= FLAG_Z;
  if (r & 0x8000)
    reg.f|= FLAG_S;
  
  return r;
}


// SUB HL,16-bit
uint16_t
cl_tlcs::op_sub_hl(t_mem val)
{
  return op_sub16(reg.hl, val);
}


// SUB HL,mem
uint16_t
cl_tlcs::op_sub_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;

  return op_sub16(reg.hl, d);
}


// SBC HL,16-bit
uint16_t
cl_tlcs::op_sbc_hl(t_mem val)
{
  uint16_t r;

  unsigned int op1= (unsigned int)reg.hl;
  unsigned int op2= (unsigned int)val;
  signed int res= (int16_t)reg.hl - (int16_t)val;

  if (reg.f & FLAG_C)
    {
      ++op2;
      --res;
    }
  reg.f&= ~(FLAG_C|FLAG_V|FLAG_Z|FLAG_S);
  reg.f|= FLAG_N;
  
  if ((op1 & 0xfff) < (op2 & 0xfff))
    reg.f|= FLAG_H;
  if ((res < -32768) || (res > 32767))
    reg.f|= FLAG_V;
  if (op1 < op2)
    reg.f|= FLAG_C;

  r= reg.hl - op2;

  if (r == 0)
    reg.f|= FLAG_Z;
  if (r & 0x8000)
    reg.f|= FLAG_S;
  
  return r;
}


// SBC HL,mem
uint16_t
cl_tlcs::op_sbc_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;
  
  return op_sbc_hl((t_mem)d);
}


// AND HL,16-bit
uint16_t
cl_tlcs::op_and_hl(t_mem val)
{
  uint16_t d= val;
  uint16_t r;

  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_X|FLAG_N|FLAG_C);
  reg.f|= FLAG_H;
  
  r= reg.hl & d;
  if (r & 0x8000)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;
  
  return r;
}


// AND HL,mem
uint16_t
cl_tlcs::op_and_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;
  
  return op_and_hl((t_mem)d);
}


// XOR HL,16-bit
uint16_t
cl_tlcs::op_xor_hl(t_mem val)
{
  uint16_t d= val;
  uint16_t r;

  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_N|FLAG_C);
  
  r= reg.hl ^ d;
  if (r & 0x8000)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;
  
  return r;
}


// XOR HL,mem
uint16_t
cl_tlcs::op_xor_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;
  
  return op_xor_hl((t_mem)d);
}


// OR HL,16-bit
uint16_t
cl_tlcs::op_or_hl(t_mem val)
{
  uint16_t d= val;
  uint16_t r;

  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H|FLAG_X|FLAG_N|FLAG_C);
  
  r= reg.hl | d;
  if (r & 0x8000)
    reg.f|= FLAG_S;
  if (r == 0)
    reg.f|= FLAG_Z;
  
  return r;
}


// OR HL,mem
uint16_t
cl_tlcs::op_or_hl(t_addr addr)
{
  uint8_t dl= nas->read(addr);
  uint8_t dh= nas->read(addr+1);
  uint16_t d= dh*256 + dl;
  
  return op_or_hl((t_mem)d);
}


/* End of tlcs/inst_arith.cc */
