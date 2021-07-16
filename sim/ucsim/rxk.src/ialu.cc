/*
 * Simulator of microcontrollers (ialu.cc)
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

#include "rxkcl.h"


/*
 *                                                   Increment, decrement
 */

int
cl_rxk::inc_ss(class cl_cell16 &rp, u16_t op)
{
  rp.W(op+1);
  tick(1);
  return resGO;
}

int
cl_rxk::inc_r(class cl_cell8 &cr, u8_t op)
{
  class cl_cell8 &rf= destF();
  u8_t r, f= rF & ~(flagS|flagZ|flagV), na7, r7;
  na7= (op&0x80)^0x80;
  cr.W(r= op+1);
  r7= r&0x80;
  if (r & 0x80) f|= flagS;
  if (!r) f|= flagZ;
  if (na7 & r7) f|= flagV;
  rf.W(f);
  tick(1);
  return resGO;
}

int
cl_rxk::INC_iIRd(t_mem code)
{
  i8_t d= fetch();
  class cl_cell8 &dest= dest8iIRd(d);
  u8_t op= dest.read();
  vc.wr++;
  vc.rd++;
  tick5p1(10);
  return inc_r(dest, op);
}

int
cl_rxk::dec_ss(class cl_cell16 &rp, u16_t op)
{
  rp.W(op-1);
  tick(1);
  return resGO;
}

int
cl_rxk::dec_r(class cl_cell8 &cr, u8_t op)
{
  class cl_cell8 &rf= destF();
  u8_t r, f= rF & ~(flagS|flagZ|flagV), a7, nr7;
  a7= op&0x80;
  cr.W(r= op-1);
  nr7= (r&0x80)^0x80;
  if (r & 0x80) f|= flagS;
  if (!r) f|= flagZ;
  if (a7 & nr7) f|= flagV;
  rf.W(f);
  tick(1);
  return resGO;
}


int
cl_rxk::DEC_iIRd(t_mem code)
{
  i8_t d= fetch();
  class cl_cell8 &dest= dest8iIRd(d);
  u8_t op= dest.read();
  vc.wr++;
  vc.rd++;
  tick5p1(10);
  return dec_r(dest, op);
}

int
cl_rxk::inc_i8(t_addr addr)
{
  class cl_cell8 &f= destF();
  u8_t forg= f.R() & ~(flagS|flagZ|flagV);
  class cl_memory_cell *dest= rwas->get_cell(addr);
  u8_t org= dest->R();
  u8_t res= org+1;
  if (res & 0x80) forg|= flagS;
  if (!res) forg|= flagZ;
  if (!(org&0x80) && (res&0x80)) forg|= flagV;
  f.W(forg);
  dest->W(res);
  vc.rd++;
  vc.wr++;
  tick(7);
  return resGO;
}

int
cl_rxk::dec_i8(t_addr addr)
{
  class cl_cell8 &f= destF();
  u8_t forg= f.R() & ~(flagS|flagZ|flagV);
  class cl_memory_cell *dest= rwas->get_cell(addr);
  u8_t org= dest->R();
  u8_t res= org-1;
  if (res & 0x80) forg|= flagS;
  if (!res) forg|= flagZ;
  if (org&res&0x80) forg|= flagV;
  f.W(forg);
  dest->W(res);
  vc.rd++;
  vc.wr++;
  tick(7);
  return resGO;
}

int
cl_rxk::add_ir_xy(u16_t op)
{
  u16_t op1= cIR->get();
  u32_t res= op1 + op;
  class cl_cell8 &f= destF();
  u8_t forg= rF & ~flagC;
  if (res > 0xffff) forg|= flagC;
  cIR->write(res);
  f.W(forg);
  tick(3);
  return resGO;
}


/*
 *                                                             Rotate, shift
 */

/*
     C <-- 7..<-...0 <--+
        |               |
        +---------------+
 */
int
cl_rxk::rot8left(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a7;
  a7= op&0x80;
  dest.W((op<<1) | (a7?1:0));
  f.W((rF & ~flagC) | (a7?flagC:0));
  tick(1);
  return resGO;
}

int
cl_rxk::rlc(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a7, res, forg= rF & ~flagAll;
  a7= op&0x80;
  dest.W(res= ((op<<1) | (a7?1:0)));
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  if (res&0xf0) forg|= flagL;
  if (a7) forg|= flagC;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
     C <-- 7..<-...0 <--+
     |                  |
     +------------------+
 */
int
cl_rxk::rot9left(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a7, c= rF&flagC;
  a7= op&0x80;
  dest.W((op<<1) | (c?1:0));
  f.W((rF & ~flagC) | (a7?flagC:0));
  tick(1);
  return resGO;
}

int
cl_rxk::rl(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a7, c= rF&flagC, res, forg;
  a7= op&0x80;
  dest.W(res= ((op<<1) | (c?1:0)));
  forg= rF & ~flagAll;
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  if (res&0xf0) forg|= flagL;
  if (a7) forg|= flagC;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
     C <-- 15..<-...0 <--+
     |                   |
     +-------------------+
 */
int
cl_rxk::rot17left(class cl_cell16 &dest, u16_t op)
{
  class cl_cell8 &f= destF();
  u8_t forg;
  u16_t a15, c= rF & flagC, res;
  forg= rF & ~flagAll;
  a15= op&0x8000;
  dest.W(res= (op<<1) | (c?1:0));
  if (a15) forg|= flagC;
  if (res & 0x8000) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res & 0xf000) forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}

/*
     7..->...0 --> C
     |         |
     +---------+
 */
int
cl_rxk::rot8right(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a0;
  a0= op&0x01;
  dest.W((op>>1) | (a0?0x80:0));
  f.W((rF & ~flagC) | (a0?flagC:0));
  tick(1);
  return resGO;
}

int
cl_rxk::rrc(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a0, res, forg= rF & ~flagAll;
  a0= op&0x01;
  dest.W(res= ((op>>1) | (a0?0x80:0)));
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  if (res&0xf0) forg|= flagL;
  if (a0) forg|= flagC;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
     7..->...0 --> C
     |             |
     +-------------+
 */
int
cl_rxk::rot9right(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a0, c= rF&flagC;
  a0= op&0x01;
  dest.W((op>>1) | (c?0x80:0));
  f.W((rF & ~flagC) | (a0?flagC:0));
  tick(1);
  return resGO;
}

int
cl_rxk::rr(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t a0, c= rF&flagC, res, forg;
  a0= op&0x01;
  dest.W(res= ((op>>1) | (c?0x80:0)));
  forg= rF & ~flagAll;
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  if (res&0xf0) forg|= flagL;
  if (a0) forg|= flagC;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
     C --> 15..->...0 -->+
     |                   |
     +<------------------+
 */
int
cl_rxk::rot17right(class cl_cell16 &dest, u16_t op)
{
  class cl_cell8 &f= destF();
  u8_t forg;
  u16_t a0, c= rF & flagC, res;
  forg= rF & ~flagAll;
  a0= op&1;
  dest.W(res= (c?0x8000:0) | (op>>1));
  if (a0) forg|= flagC;
  if (res & 0x8000) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res & 0xf000) forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}


/*
   C <--  7..<-..0  <-- 0
*/
int
cl_rxk::sla(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t res, forg= rF & ~flagAll;
  if (op & 0x80) forg|= flagC;
  dest.W(res= (op<<1));
  if (res & 0x8000) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res & 0xf000) forg|= flagL;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
  +--> 7..->..0  --> C
  |    |
  +----+  
*/
int
cl_rxk::sra(class cl_cell8 &dest, i8_t op)
{
  class cl_cell8 &f= destF();
  i8_t res, forg= rF & ~flagAll;
  if (op & 1) forg|= flagC;
  dest.W(res= (op>>1));
  if (res & 0x8000) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res & 0xf000) forg|= flagL;
  f.W(forg);
  tick(3);
  return resGO;
}


/*
  0-->  7..->..0  --> C
*/
int
cl_rxk::srl(class cl_cell8 &dest, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t res, forg= rF & ~flagAll;
  if (op & 1) forg|= flagC;
  dest.W(res= (op>>1));
  if (res & 0x8000) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res & 0xf000) forg|= flagL;
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::bit_r(u8_t b, u8_t op)
{
  class cl_cell8 &f= destF();
  u8_t forg= rF & ~flagZ, res;
  res= (1<<b) & op;
  if (!res) forg|= flagZ;
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::bit_iHL(u8_t b)
{
  u8_t forg= rF & ~flagZ, res, op;
  op= rwas->read(rHL);
  vc.rd++;
  res= (1<<b) & op;
  if (!res) forg|= flagZ;
  cF.W(forg);
  tick(6);
  return resGO;
}

int
cl_rxk::bit_iIRd(u8_t b, i8_t d)
{
  u8_t forg= rF & ~flagZ, res, op;
  op= rwas->read(cIR->get()+d);
  vc.rd++;
  res= (1<<b) & op;
  if (!res) forg|= flagZ;
  cF.W(forg);
  tick5p1(9);
  return resGO;
}

int
cl_rxk::res_r(u8_t b, class cl_cell8 &dest, u8_t op)
{
  u8_t res;
  res= ~(1<<b) & op;
  dest.W(res);
  tick(3);
  return resGO;
}

int
cl_rxk::res_iHL(u8_t b)
{
  u8_t res, op;
  op= rwas->read(rHL);
  vc.rd++;
  res= ~(1<<b) & op;
  rwas->write(rHL, res);
  vc.wr++;
  tick(9);
  return resGO;
}

int
cl_rxk::res_iIRd(u8_t b, i8_t d)
{
  u8_t res, op;
  t_addr a= cIR->get()+d;
  op= rwas->read(a);
  vc.rd++;
  res= ~(1<<b) & op;
  rwas->write(a, res);
  vc.wr++;
  tick5p1(12);
  return resGO;
}


int
cl_rxk::set_r(u8_t b, class cl_cell8 &dest, u8_t op)
{
  u8_t res;
  res= (1<<b) | op;
  dest.W(res);
  tick(3);
  return resGO;
}

int
cl_rxk::set_iHL(u8_t b)
{
  u8_t res, op;
  op= rwas->read(rHL);
  vc.rd++;
  res= (1<<b) | op;
  rwas->write(rHL, res);
  vc.wr++;
  tick(9);
  return resGO;
}

int
cl_rxk::set_iIRd(u8_t b, i8_t d)
{
  u8_t res, op;
  t_addr a= cIR->get()+d;
  op= rwas->read(a);
  vc.rd++;
  res= (1<<b) | op;
  rwas->write(a, res);
  vc.wr++;
  tick5m1(11);
  return resGO;
}


/*
 *                                                           Bitwise, logical
 */

int
cl_rxk::BOOL_HL(t_mem code)
{
  class cl_cell16 &dhl= destHL();
  class cl_cell8 &f= destF();
  u8_t forg= rF & ~flagAll;
  if (rHL)
    dhl.W(1);
  else
    forg|= flagZ;
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::xor8(class cl_cell8 &dest, u8_t op1, u8_t op2)
{
  class cl_cell8 &f= destF();
  u8_t forg= f.R() & ~flagAll;
  u8_t res= op1 ^ op2;
  dest.W(res);
  if (res & 0x80) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res&0xf0) forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::or8(class cl_cell8 &dest, u8_t op1, u8_t op2)
{
  class cl_cell8 &f= destF();
  u8_t forg= f.R() & ~flagAll;
  u8_t res= op1 | op2;
  dest.W(res);
  if (res & 0x80) forg|= flagS;
  if (!res) forg|= flagZ;
  if (res&0xf0) forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::or16(class cl_cell16 &dest, u16_t op1, u16_t op2)
{
  class cl_cell8 &f= destF();
  //class cl_cell16 &dhl= destHL();
  u16_t res= op1/*rHL*/ | op2/*rDE*/;
  u8_t forg= rF & ~flagAll;
  dest.W(res);
  if (res & 0x8000)
    forg|= flagS;
  if (!res)
    forg|= flagZ;
  // flagL?
  if (res & 0xf000)
    forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::and8(class cl_cell8 &dest, u8_t op1, u8_t op2)
{
  class cl_cell8 &f= destF();
  u8_t res= op1 & op2, forg= rF & ~flagAll;
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  if (res&0xf0) forg|= flagL;
  dest.W(res);
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::and16(class cl_cell16 &dest, u16_t op1, u16_t op2)
{
  class cl_cell8 &f= destF();
  //class cl_cell16 &dhl= destHL();
  u16_t res= op1/*rHL*/ & op2/*rDE*/;
  u8_t forg= rF & ~flagAll;
  dest.W(res);
  if (res & 0x8000)
    forg|= flagS;
  if (!res)
    forg|= flagZ;
  // flagL?
  if (res & 0xf000)
    forg|= flagL;
  f.W(forg);
  tick(1);
  return resGO;
}

int
cl_rxk::OR_A_iIRd(t_mem code)
{
  i8_t d= fetch();
  class cl_cell8 &a= destA(), &f= destF();
  u8_t forg= rF & ~flagAll, res= rA | rwas->read(cIR->get() + d);
  vc.rd++;
  if (!res) forg|= flagZ;
  if (res & 0x80) forg|= flagS;
  if (res & 0xf0) forg|= flagL;
  a.W(res);
  f.W(forg);
  tick5p1(8);
  return resGO;
}

int
cl_rxk::AND_A_iIRd(t_mem code)
{
  i8_t d= fetch();
  class cl_cell8 &a= destA(), &f= destF();
  u8_t forg= rF & ~flagAll, res= rA & rwas->read(cIR->get() + d);
  vc.rd++;
  if (!res) forg|= flagZ;
  if (res & 0x80) forg|= flagS;
  if (res & 0xf0) forg|= flagL;
  a.W(res);
  f.W(forg);
  tick5p1(8);
  return resGO;
}


/*
 *                                                                Arithmetic
 */

int
cl_rxk::CPL(t_mem code)
{
  destA().W(~rA);
  tick(1);
  return resGO;
}

int
cl_rxk::add_hl_ss(u16_t op)
{
  class cl_cell16 &hl= destHL();
  class cl_cell8 &f= destF();
  u8_t forg= f.R();
  u32_t res= rHL + op;
  hl.W(res);
  if (res > 0xffff)
    f.W(forg|= flagC);
  else
    f.W(forg|=~flagC);
  tick(1);
  return resGO;
}

int
cl_rxk::adc_hl_ss(u16_t op)
{
  class cl_cell16 &hl= destHL();
  class cl_cell8 &f= destF();
  u32_t res= rHL + op + ((rF&flagC)?1:0);
  u8_t forg= f.R() & ~flagAll;
  u16_t c1= 0, c2;
  if (res > 0xffff)
    {
      forg|= flagC;
      c1= 0x8000;
    }
  if (!res) forg|= flagZ;
  if (res & 0x8000) forg|= flagS;
  res= (rHL&0x7fff)+(op&0x7fff)+((rF&flagC)?1:0);
  c2= res&0x8000;
  if (c1^c2) forg|= flagV;
  f.W(forg);
  hl.W(res);
  tick(3);
  return resGO;
}

int
cl_rxk::add8(u8_t op2, bool cy)
{
  class cl_cell8 &a= destA(), &f= destF();
  u8_t v1= rA, forg;
  u16_t res= v1+op2+(cy?((rF&flagC)?1:0):0);
  u8_t a7, b7, r7, na7, nb7, nr7;
  forg= rF & ~flagAll;
  a7= v1&0x80; na7= a7^0x80;
  b7= op2&0x80; nb7= b7^0x80;
  r7= res&0x80; nr7= r7^0x80;
  if ((a7&b7&nr7) | (na7&nb7&r7)) forg|= flagV;
  if (res > 0xff) forg|= flagC;
  if (!(res & 0xff)) forg|= flagZ;
  if (res & 0x80) forg|= flagS;
  a.W(res);
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::sub8(u8_t op2, bool cy)
{
  class cl_cell8 &a= destA(), &f= destF();
  u8_t v1= rA, forg;
  u16_t res= v1+(~op2)+(cy?((rF&flagC)?1:0):1);
  u8_t a7, b7, r7, na7, nb7, nr7;
  i8_t op1= rA;
  i8_t o2= op2;
  i8_t r= op1-o2;
  if (cy && (rF&flagC)) r--;
  res= r;
  forg= rF & ~(flagS|flagZ|flagV);
  a7= v1&0x80; na7= a7^0x80;
  b7= op2&0x80; nb7= b7^0x80;
  r7= res&0x80; nr7= r7^0x80;
  if ((a7&nb7&nr7) | (na7&b7&r7)) forg|= flagV;
  //if (res > 0xff) forg|= flagC;
  if (rA<op2) forg|= flagC;
  if ((rA>op2) || (!cy && (rA==op2))) forg&= ~flagC;
  if (!(res & 0xff)) forg|= flagZ;
  if (res & 0x80) forg|= flagS;
  a.W(res);
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::sub16(u16_t op2, bool cy)
{
  class cl_cell16 &hl= destHL();
  class cl_cell8 &f= destF();
  u16_t v1= rHL;
  u8_t forg;
  u32_t res;//= //v1+(~op2)+(cy?((rF&flagC)?1:0):1);
  u16_t a15, b15, r15, na15, nb15, nr15;
  i16_t op1= rHL;
  i16_t o2= op2;
  i16_t r= op1-o2;
  if (cy && (rF&flagC)) r--;
  res= r;
  forg= rF & ~(flagS|flagZ|flagV);
  a15= v1&0x8000; na15= a15^0x8000;
  b15= op2&0x8000; nb15= b15^0x8000;
  r15= res&0x8000; nr15= r15^0x8000;
  if ((a15&nb15&nr15) | (na15&b15&r15)) forg|= flagV;
  //if (res > 0xffff) forg|= flagC;
  if (rHL<op2) forg|= flagC;
  if ((rHL>op2) || (!cy && (rA==op2))) forg&= ~flagC;
  if (!(res & 0xffff)) forg|= flagZ;
  if (res & 0x8000) forg|= flagS;
  hl.W(res);
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::cp8(u8_t op1, u8_t op2)
{
  class cl_cell8 &f= destF();
  u8_t forg= rF & ~flagAll;
  u8_t res= op1-op2;
  if (op1<op2) forg|= (flagS|flagC);
  if (op1==op2) forg|= flagZ;
  if ( 0x80 & ( (res&op1&~op2) | (~res&~op1&op2) ) ) forg|= flagV;
  f.W(forg);
  tick5m2(1);
  return resGO;
}

int
cl_rxk::ADD_SP_d(t_mem code)
{
  i8_t d= fetch();
  u32_t res= rSP+d;
  class cl_cell8 &f= destF();
  u8_t forg= f.R();
  cSP.W(res);
  if (res > 0xffff)
    f.W(forg|= flagC);
  else
    f.W(forg&= ~flagC);
  tick(3);
  return resGO;
}

int
cl_rxk::MUL(t_mem code)
{
  i16_t op1= rBC, op2= rDE;
  i32_t res= op1*op2;
  cHL.W((res>>16)&0xffff);
  cBC.W(res);
  tick(11);
  return resGO;
}


/* End of rxk.src/ialu.cc */
