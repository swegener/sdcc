/*
 * Simulator of microcontrollers (tlcs.src/inst_cpu_others.cc)
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

/* $Id: inst_cpu_others.cc 226 2016-06-21 10:30:04Z  $ */

#include "tlcscl.h"


// 0B
int
cl_tlcs::inst_daa_a()
{
  if (((reg.a & 0x0f) > 9) ||
      (reg.f & FLAG_H))
    {
      int al= reg.a & 0x0f;
      if (al + 6 > 15)
	reg.f|= FLAG_H;
      else
	reg.f&= ~FLAG_H;
      if ((int)reg.a + 6 > 255)
	reg.f|= FLAG_C|FLAG_X;
      else
	reg.f&= ~(FLAG_C|FLAG_X);
      reg.a+= 6;
    }
  if (((reg.a & 0xf0) > 0x90) ||
      (reg.f & FLAG_C))
    {
      if ((int)reg.a + 0x60 > 255)
	reg.f|= FLAG_C;
      else
	reg.f&= ~FLAG_C;
      reg.a+= 0x60;
    }
  return resGO;
}


// 10
int
cl_tlcs::inst_cpl_a()
{
  reg.a= ~reg.a;
  reg.f|= FLAG_H|FLAG_N;
  return resGO;
}


// 11
int
cl_tlcs::inst_neg_a()
{
  reg.f&= ~(FLAG_S|FLAG_Z|FLAG_H/*|FLAG_X*/|FLAG_V|FLAG_C);
  reg.f|= FLAG_N;
  if (reg.a == 0x80)
    reg.f|= FLAG_V;
  if (reg.a)
    reg.f|= (FLAG_C|FLAG_X);

  //uint8_t a= ~reg.a;
  if ((reg.a & 0x0f) == 0)//if (a&0xf + 1 > 15)
    reg.f|= FLAG_H;
  reg.a= 0-reg.a;

  if (reg.a & 0x80)
    reg.f|= FLAG_S;
  if (!reg.a)
    reg.f|= FLAG_Z;
  
  return resGO;
}


// 0e
int
cl_tlcs::inst_ccf()
{
  if (reg.f & FLAG_C)
    reg.f&= ~(FLAG_C);
  else
    reg.f|= FLAG_C;
  if (reg.f & FLAG_X)
    reg.f&= ~(FLAG_X);
  else
    reg.f|= FLAG_X;
  reg.f&= ~FLAG_N;
  return resGO;
}


// 0d
int
cl_tlcs::inst_scf()
{
  reg.f|= FLAG_C|FLAG_X;
  reg.f&= ~(FLAG_N|FLAG_H);
  return resGO;
}


// 0c
int
cl_tlcs::inst_rcf()
{
  reg.f&= ~(FLAG_C|FLAG_X|FLAG_N|FLAG_H);
  return resGO;
}


// ff
int
cl_tlcs::inst_swi()
{
  t_addr iPC= PC-1;
  reg.f&= ~FLAG_I;
  exec_intr(iPC, 0x0010, PC);
  exec_push(iPC, reg.af);
  PC= 0x0010;
  return resGO;
}


// MUL HL,mem
int
cl_tlcs::inst_mul_hl(class cl_memory_cell *cell)
{
  reg.hl= reg.l * cell->read();
  return resGO;
}


// DIV HL,mem
int
cl_tlcs::inst_div_hl(class cl_memory_cell *cell)
{
  uint8_t m= cell->read();
  return inst_div_hl(m);
}


// DIV HL,val
int
cl_tlcs::inst_div_hl(uint8_t d)
{
  uint8_t m= d;
  reg.f&= ~FLAG_V;
  if ((m == 0) ||
      ((reg.hl / m) > 255))
    reg.f|= FLAG_V;
  else
    reg.l= reg.hl / m;
  reg.h= reg.hl % m;
  return resGO;
}


/* End of tlcs.src/inst_cpu_others.cc */
