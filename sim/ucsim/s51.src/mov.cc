/*
 * Simulator of microcontrollers (mov.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
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

/* Bugs fixed by Sandeep Dutta:
 *	source<->dest bug in "mov direct,direct"
 *	get register in "mov @ri,address"
 */
 
#include "ddconfig.h"

#include <stdio.h>

// sim
#include "memcl.h"

// local
#include "uc51cl.h"
#include "regs51.h"


/*
 * 0x74 2 12 MOV A,#data
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_a_Sdata(uchar code)
{
  acc->write(fetch());
  return(resGO);
}


/*
 * 0x75 3 24 MOV addr,#data
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_addr_Sdata(uchar code)
{
  class cl_cell *cell;

  cell= get_direct(fetch());
  cell->write(fetch());
  tick(1);
  return(resGO);
}


/*
 * 0x76-0x77 2 12 MOV @Ri,#data
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_Sri_Sdata(uchar code)
{
  class cl_cell *cell;
  
  cell= iram->get_cell(get_reg(code & 0x01)->read());
  t_mem d= fetch();
  cell->write(d);
  return(resGO);
}


/*
 * 0x78-0x7f 2 12 MOV Rn,#data
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_rn_Sdata(uchar code)
{
  class cl_cell *reg;

  reg= get_reg(code & 0x07);
  reg->write(fetch());
  return(resGO);
}


/*
 * 0x93 1 24 MOVC A,@A+DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movc_a_Sa_pc(uchar code)
{
  acc->write(mem(MEM_ROM)->read(PC + acc->read()));
  tick(1);
  return(resGO);
}


/*
 * 0x85 3 24 MOV addr,addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_addr_addr(uchar code)
{
  class cl_cell *d, *s;

  /* SD reversed s & d here */
  s= get_direct(fetch());
  d= get_direct(fetch());
  d->write(s->read());
  tick(1);
  return(resGO);
}


/*
 * 0x86-0x87 2 24 MOV addr,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_addr_Sri(uchar code)
{
  class cl_cell *d, *s;

  d= get_direct(fetch());
  s= iram->get_cell(get_reg(code & 0x01)->read());
  d->write(s->read());
  tick(1);
  return(resGO);
}


/*
 * 0x88-0x8f 2 24 MOV addr,Rn
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_addr_rn(uchar code)
{
  class cl_cell *cell;

  cell= get_direct(fetch());
  cell->write(get_reg(code & 0x07)->read());
  tick(1);
  return(resGO);
}


/*
 * 0x90 3 24 MOV DPTR,#data
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_dptr_Sdata(uchar code)
{
  sfr->write(DPH, fetch());
  sfr->write(DPL, fetch());
  tick(1);
  return(resGO);
}


/*
 * 0x93 1 24 MOVC A,@A+DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movc_a_Sa_dptr(uchar code)
{
  acc->write(get_mem(MEM_ROM,
		     sfr->read(DPH)*256+sfr->read(DPL) +
		     acc->read()));
  tick(1);
  return(resGO);
}


/*
 * 0xa6-0xa7 2 24 MOV @Ri,addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_Sri_addr(uchar code)
{
  class cl_cell *d, *s;

  d= iram->get_cell(get_reg(code & 0x01)->read());
  s= get_direct(fetch());
  d->write(s->read());
  tick(1);
  return(resGO);
}


/*
 * 0xa8-0xaf 2 24 MOV Rn,addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_rn_addr(uchar code)
{
  class cl_cell *reg, *cell;

  reg = get_reg(code & 0x07);
  cell= get_direct(fetch());
  reg->write(cell->read());
  tick(1);
  return(resGO);
}


/*
 * 0xc0 2 24 PUSH addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_push(uchar code)
{
  t_addr sp;
  class cl_cell *stck, *cell;

  cell= get_direct(fetch());
  sp= sfr->wadd(SP, 1);
  stck= iram->get_cell(sp);
  stck->write(cell->read());
  tick(1);
  return(resGO);
}


/*
 * 0xc5 2 12 XCH A,addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_xch_a_addr(uchar code)
{
  t_mem temp;
  class cl_cell *cell;

  cell= get_direct(fetch());
  temp= acc->read();
  acc->write(cell->read());
  cell->write(temp);
  return(resGO);
}


/*
 * 0xc6-0xc7 1 12 XCH A,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_xch_a_Sri(uchar code)
{
  t_mem temp;
  class cl_cell *cell;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  temp= acc->read();
  acc->write(cell->read());
  cell->write(temp);
  return(resGO);
}


/*
 * 0xc8-0xcf 1 12 XCH A,Rn
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_xch_a_rn(uchar code)
{
  t_mem temp;
  class cl_cell *reg;

  reg = get_reg(code & 0x07);
  temp= acc->read();
  acc->write(reg->read());
  reg->write(temp);
  return(resGO);
}


/*
 * 0xd0 2 24 POP addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_pop(uchar code)
{
  t_addr sp;
  class cl_cell *cell, *stck;

  cell= get_direct(fetch());
  stck= iram->get_cell(sfr->get(SP));
  cell->write(stck->read());
  sp= sfr->wadd(SP, -1);
  tick(1);
  return(resGO);
}


/*
 * 0xd6-0xd7 1 12 XCHD A,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_xchd_a_Sri(uchar code)
{
  t_mem temp, d;
  class cl_cell *cell;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  temp= (d= cell->read()) & 0x0f;
  cell->write((d & 0xf0) | (acc->read() & 0x0f));
  acc->write((acc->get() & 0xf0) | temp);
  return(resGO);
}


/*
 * 0xe0 1 24 MOVX A,@DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movx_a_Sdptr(uchar code)
{
  acc->write(read_mem(MEM_XRAM,
		      sfr->read(DPH)*256 + sfr->read(DPL)));
  tick(1);
  return(resGO);
}


/*
 * 0xe2-0xe3 1 24 MOVX A,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movx_a_Sri(uchar code)
{
  t_mem d;

  d= get_reg(code & 0x01)->read();
  acc->write(read_mem(MEM_XRAM, sfr->read(P2)*256 + d));
  tick(1);
  return(resGO);
}


/*
 * 0xe5 2 12 MOV A,addr
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_a_addr(uchar code)
{
  class cl_cell *cell;
  int address = fetch();

  /* If this is ACC, it is an invalid instruction */
  if (address == ACC)
    {
      sim->app->get_commander()->
        debug ("Invalid Instruction : E5 E0  MOV A,ACC  at  %06x\n", PC);

      return(resHALT);
    }
  else
    {
      cell= get_direct(address);
      acc->write(cell->read());
      return(resGO);
    }
  
}


/*
 * 0xe6-0xe7 1 12 MOV A,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_a_Sri(uchar code)
{
  class cl_cell *cell;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  acc->write(cell->read());
  return(resGO);
}


/*
 * 0xe8-0xef 1 12 MOV A,Rn
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_a_rn(uchar code)
{
  acc->write(get_reg(code & 0x07)->read());
  return(resGO);
}


/*
 * 0xf0 1 24 MOVX @DPTR,A
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movx_Sdptr_a(uchar code)
{
  write_mem(MEM_XRAM, sfr->read(DPH)*256 + sfr->read(DPL), acc->read());
  tick(1);
  return(resGO);
}


/*
 * 0xf2-0xf3 1 24 MOVX @Ri,A
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_movx_Sri_a(uchar code)
{
  t_mem d;

  d= get_reg(code & 0x01)->read();
  write_mem(MEM_XRAM, sfr->read(P2)*256 + d, acc->read());
  tick(1);
  return(resGO);
}


/*
 * 0xf5 2 12 MOV addr,A
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_addr_a(uchar code)
{
  class cl_cell *cell;
  
  cell= get_direct(fetch());
  cell->write(acc->read());
  return(resGO);
}


/*
 * 0xf6-0xf7 1 12 MOV @Ri,A
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_Sri_a(uchar code)
{
  class cl_cell *cell;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  cell->write(acc->read());
  return(resGO);
}


/*
 * 0xf8-0xff 1 12 MOV Rn,A
 *____________________________________________________________________________
 *
 */

int
t_uc51::inst_mov_rn_a(uchar code)
{
  class cl_cell *reg;

  reg= get_reg(code &0x07);
  reg->write(acc->read());
  return(resGO);
}


/* End of s51.src/mov.cc */
