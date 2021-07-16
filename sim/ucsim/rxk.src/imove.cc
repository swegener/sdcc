/*
 * Simulator of microcontrollers (imove.cc)
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


int
cl_rxk::ld_dd_mn(class cl_cell16 &dd)
{
  u8_t l, h;
  l= fetch();
  h= fetch();
  dd.W(h*256+l);
  tick(5);
  return resGO;
}

int
cl_rxk::LD_imn_HL(t_mem code)
{
  u8_t l, h;
  l= fetch();
  h= fetch();
  t_addr a= h*256+l;
  rwas->write(a, rL);
  rwas->write(a+1, rH);
  vc.wr+= 2;
  tick(12);
  return resGO;
}

int
cl_rxk::LD_HL_imn(t_mem code)
{
  u8_t l, h;
  l= fetch();
  h= fetch();
  t_addr a= h*256+l;
  l= rwas->read(a);
  h= rwas->read(a+1);
  vc.rd+= 2;
  destH().W(h);
  destL().W(l);
  tick(10);
  return resGO;
}

int
cl_rxk::ld_r_n(class cl_cell8 &r)
{
  r.W(fetch());
  tick(3);
  return resGO;
}

int
cl_rxk::ld_ihl_r(u8_t op)
{
  class cl_cell8 &c= dest8iHL();
  c.W(op);
  vc.wr++;
  tick(5);
  return resGO;
}

int
cl_rxk::ld_r_ihl(class cl_cell8 &destr)
{
  u8_t v= read8(rHL);
  destr.W(v);
  tick(4);
  return resGO;
}

int
cl_rxk::ld_r_g(class cl_cell8 &dest, u8_t op)
{
  dest.W(op);
  tick(1);
  return resGO;
}

int
cl_rxk::pop_zz(class cl_cell16 &dest)
{
  u8_t l, h;
  l= rom->read(rSP++);
  h= rom->read(rSP++);
  cSP.W(rSP);
  dest.W(h*256+l);
  vc.rd+= 2;
  tick(6);
  return resGO;
}

int
cl_rxk::push_zz(u16_t op)
{
  u8_t h, l;
  h= op>>8;
  l= op&0xff;
  cSP.W(rSP-1);
  rom->write(rSP, h);
  cSP.W(rSP-1);
  rom->write(rSP, l);
  vc.wr+= 2;
  tick5p1(9);
  return resGO;
}

int
cl_rxk::ld_d_i(int dif)
{
  int t= 9;
  if (rwas == ioi)
    t+= 1;
  else if (rwas == ioe)
    t+= 2;
  rwas->write(rDE, rom->read(rHL));
  vc.rd++;
  vc.wr++;
  cBC.W(rBC-1);
  cDE.W(rDE+dif);
  cHL.W(rHL+dif);
  tick(t);
  if (rBC == 0)
    cF.W(rF & ~flagV);
  else
    cF.W(rF | flagV);
  return resGO;
}

int
cl_rxk::ld_iIRd_r(u8_t op)
{
  i8_t d= fetch();
  t_addr a= cIR->get();
  a+= d;
  rwas->write(a, op);
  vc.wr++;
  tick5p1(9);
  return resGO;
}

int
cl_rxk::ld_r_iIRd(class cl_cell8 &op)
{
  i8_t d= fetch();
  t_addr a= cIR->get() + d;
  op.W(rwas->read(a));
  vc.rd++;
  tick5p1(8);
  return resGO;
}

int
cl_rxk::LD_iBC_A(t_mem code)
{
  class cl_cell8 &c= dest8iBC();
  c.W(rA);
  vc.wr++;
  tick(6);
  return resGO;
}

int
cl_rxk::LD_iDE_A(t_mem code)
{
  class cl_cell8 &c= dest8iDE();
  c.W(rA);
  vc.wr++;
  tick(6);
  return resGO;
}

int
cl_rxk::LD_iMN_A(t_mem code)
{
  class cl_cell8 &c= dest8imn();
  c.W(rA);
  vc.wr++;
  tick(9);
  return resGO;
}

int
cl_rxk::LD_A_iBC(t_mem code)
{
  destA().W(read8(rBC));
  tick(5);
  return resGO;
}

int
cl_rxk::LD_A_iDE(t_mem code)
{
  destA().W(read8(rDE));
  tick(5);
  return resGO;
}

int
cl_rxk::LD_A_iMN(t_mem code)
{
  destA().W(read8(fetch16()));
  tick(8);
  return resGO;
}

int
cl_rxk::EX_AF_aAF(t_mem code)
{
  u16_t temp= rAF;
  cAF.W(raAF);
  caAF.W(temp);
  tick(1);
  return resGO;
}

int
cl_rxk::EX_aDE_HL(t_mem code)
{
  class cl_cell16 &dhl= destHL();
  u16_t t= raDE;
  caDE.W(dhl.get());
  dhl.W(t);
  tick(1);
  return resGO;
}

int
cl_rxk::EX_DE_HL(t_mem code)
{
  class cl_cell16 &dhl= destHL();
  u16_t t= rDE;
  cDE.W(dhl.get());
  dhl.W(t);
  tick(1);
  return resGO;
}

int
cl_rxk::LD_HL_iSPn(t_mem code)
{
  class cl_cell16 &dest= destHL();
  u8_t n= fetch(), l, h;
  l= rom->read(rSP+n);
  h= rom->read(rSP+n+1);
  dest.W(h*256+l);
  vc.rd+= 2;
  tick5p1(8);
  return resGO;
}

int
cl_rxk::LD_HL_iIXd(t_mem code)
{
  i8_t d= fetch();
  class cl_cell16 &dhl= destHL();
  t_addr a= (rIX+d)&0xffff;
  u8_t h, l;
  l= rwas->read(a);
  h= rwas->read(a+1);
  dhl.W(h*256+l);
  vc.rd+= 2;
  tick5p1(8);
  return resGO;
}

int
cl_rxk::LD_iIXd_HL(t_mem code)
{
  i8_t d= fetch();
  t_addr a= (rIX+d)&0xffff;
  rwas->write(a, rL);
  rwas->write(a+1, rH);
  vc.wr+= 2;
  tick5p1(10);
  return resGO;
}

int
cl_rxk::LD_iSPn_HL(t_mem code)
{
  u8_t n= fetch();
  rom->write(rSP+n, rL);
  rom->write(rSP+n+1, rH);
  vc.wr+= 2;
  tick5p1(10);
  return resGO;
}

int
cl_rxk::LD_EIR_A(t_mem code)
{
  cEIR.W(rA);
  tick(3);
  return resGO;
}

int
cl_rxk::LD_IIR_A(t_mem code)
{
  cIIR.W(rA);
  tick(3);
  return resGO;
}

int
cl_rxk::LD_A_EIR(t_mem code)
{
  class cl_cell8 &f= destF();
  u8_t res= rEIR, forg= rF & ~(flagS|flagZ);
  destA().W(res);
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::LD_A_IIR(t_mem code)
{
  class cl_cell8 &f= destF();
  u8_t res= rIIR, forg= rF & ~(flagS|flagZ);
  destA().W(res);
  if (!res) forg|= flagZ;
  if (res&0x80) forg|= flagS;
  f.W(forg);
  tick(3);
  return resGO;
}

int
cl_rxk::LD_IR_mn(t_mem code)
{
  u8_t h, l;
  l= fetch();
  h= fetch();
  cIR->W(h*256+l);
  tick(7);
  return resGO;
}

int
cl_rxk::POP_IR(t_mem code)
{
  u8_t h, l;
  l= rom->read(rSP++);
  h= rom->read(rSP++);
  cSP.W(rSP);
  vc.rd+= 2;
  cIR->write(h*256+l);
  tick(8);
  return resGO;
}

int
cl_rxk::PUSH_IR(t_mem code)
{
  u16_t v= cIR->get();
  u8_t h, l;
  h= v>>8;
  l= v;
  rom->write(--rSP, h);
  rom->write(--rSP, l);
  cSP.W(rSP);
  vc.wr+= 2;
  tick5p1(11);
  return resGO;
}

int
cl_rxk::LD_SP_IR(t_mem code)
{
  cSP.W(cIR->get());
  tick5p2(1);
  return resGO;
}

int
cl_rxk::LD_IR_iSPn(t_mem code)
{
  u8_t n= fetch(), h, l;
  l= rom->read(rSP+n);
  h= rom->read(rSP+n+1);
  vc.rd+= 2;
  cIR->write(h*256+l);
  tick5p1(10);
  return resGO;
}

int
cl_rxk::LD_iSPn_IR(t_mem code)
{
  u16_t v= cIR->get();
  u8_t n= fetch();
  rom->write(rSP+n, v>>8);
  rom->write(rSP+n+1, v);
  vc.wr+= 2;
  tick5p1(12);
  return resGO;
}

int
cl_rxk::LD_HL_iIRd(t_mem code)
{
  class cl_cell16 *ir= cIR;
  if (cIR == &cIX)
    ir= &cHL;
  i8_t d= fetch();
  t_addr a= ir->get() + d;
  destHL().W(read16io(a));
  tick5p1(10);
  return resGO;
}

int
cl_rxk::LD_iIRd_n(t_mem code)
{
  i8_t d= fetch();
  u8_t n= fetch();
  t_addr a= cIR->get()+d;
  rwas->write(a, n);
  tick5p1(10);
  vc.wr++;
  return resGO;
}

int
cl_rxk::ld_dd_imn(class cl_cell16 &dest)
{
  u16_t a= fetch16();
  dest.W(read16io(a));
  tick(12);
  return resGO;
}

int
cl_rxk::EXX_iSP_HL(t_mem code)
{
  class cl_cell16 &hl= destHL();
  u16_t temp= read16(rSP);
  write16(rSP, rHL);
  hl.W(temp);
  tick(14);
  return resGO;
}


/* End of rxk.src/imove.cc */
