/*
 * Simulator of microcontrollers (mcs6502.cc)
 *
 * Copyright (C) 2020,20 Drotos Daniel, Talker Bt.
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

#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "utils.h"

#include "dregcl.h"

#include "glob.h"
#include "irqcl.h"

#include "mcs6502cl.h"


cl_mcs6502::cl_mcs6502(class cl_sim *asim):
  cl_uc(asim)
{
}

int
cl_mcs6502::init(void)
{
  cl_uc::init();
  fill_def_wrappers(itab);

  xtal= 1000000;
    
#define RCV(R) reg_cell_var(&c ## R , &r ## R , "" #R "" , "CPU register " #R "")
  RCV(A);
  RCV(X);
  RCV(Y);
  RCV(SP);
  RCV(P);
#undef RCV
  
  class cl_memory_operator *op= new cl_cc_operator(&cCC);
  cCC.append_operator(op);

  return 0;
}


const char *
cl_mcs6502::id_string(void)
{
  return "MCS6502";
}

void
cl_mcs6502::reset(void)
{
  cl_uc::reset();

  CC= 0x20 | flagI;
  PC= read_addr(rom, RESET_AT);
  tick(7);
}

  
void
cl_mcs6502::set_PC(t_addr addr)
{
  PC= addr;
}

void
cl_mcs6502::mk_hw_elements(void)
{
  class cl_hw *h;
  
  cl_uc::mk_hw_elements();

  add_hw(h= new cl_dreg(this, 0, "dreg"));
  h->init();

  add_hw(h= new cl_irq_hw(this));
  h->init();

  src_irq= new cl_it_src(this,
			 irq_irq,
			 &cCC, flagI,
			 h->cfg_cell(m65_irq), 1,
			 IRQ_AT, false, true,
			 "Interrupt request",
			 0);
  src_irq->set_cid('i');
  src_irq->set_ie_value(0);
  src_irq->init();
  it_sources->add(src_irq);
  
  src_nmi= new cl_it_src(this,
			 irq_nmi,
			 h->cfg_cell(m65_nmi_en), 1,
			 h->cfg_cell(m65_nmi), 1,
			 NMI_AT, false, true,
			 "Non-maskable interrupt request",
			 0);
  src_nmi->set_cid('n');
  src_nmi->set_nmi(true);
  src_nmi->init();
  it_sources->add(src_nmi);
  
  src_brk= new cl_it_src(this,
			 irq_brk,
			 h->cfg_cell(m65_brk_en), 1,
			 h->cfg_cell(m65_brk), 1,
			 IRQ_AT, false, true,
			 "BRK",
			 0);
  src_brk->set_cid('b');
  src_brk->init();
  it_sources->add(src_brk);
}

void
cl_mcs6502::make_cpu_hw(void)
{
}

void
cl_mcs6502::make_memories(void)
{
  class cl_address_space *as;
  class cl_address_decoder *ad;
  class cl_memory_chip *chip;
  
  rom= as= new cl_address_space("rom", 0, 0x10000, 8);
  as->init();
  address_spaces->add(as);

  chip= new cl_chip8("rom_chip", 0x10000, 8);
  chip->init();
  memchips->add(chip);
  ad= new cl_address_decoder(as= rom,
			     chip, 0, 0xffff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
}

struct dis_entry *
cl_mcs6502::dis_tbl(void)
{
  return(disass_mcs6502);
}

char *
cl_mcs6502::disassc(t_addr addr, chars *comment)
{
  chars work= chars(), temp= chars();
  const char *b;
  t_mem code= rom->get(addr);
  struct dis_entry *dt= dis_tbl();//, *dis_e;
  int i;
  bool first;
  u8_t h, l;
  u16_t a;
  
  if (!dt)
    return NULL;

  i= 0;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  //dis_e= &dt[i];
  if (dt[i].mnemonic == NULL)
    return strdup("-- UNKNOWN/INVALID");
  b= dt[i].mnemonic;

  first= true;
  work= "";
  for (i=0; b[i]; i++)
    {
      if ((b[i] == ' ') && first)
	{
	  first= false;
	  while (work.len() < 6) work.append(' ');
	}
      if (b[i] == '%')
	{
	  i++;
	  temp= "";
	  switch (b[i])
	    {
	    case 'x': // (ind,X)
	      l= rom->read(addr+1);
	      work.appendf("($%02x,X)", l);
	      l+= rX;
	      a= read_addr(rom, l);
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'y': // (ind),Y
	      l= rom->read(addr+1);
	      work.appendf("($%02x),Y", l);
	      a= read_addr(rom, l) + rY;
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'a': // abs
	      l= rom->read(addr+1);
	      h= rom->read(addr+2);
	      a= h*256+l;
	      work.appendf("$%04x", a);
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'j': // JMP abs
	      l= rom->read(addr+1);
	      h= rom->read(addr+2);
	      a= h*256+1;
	      work.appendf("$%04x", a);
	      break;
	    case 'J': // JMP (ind)
	      l= rom->read(addr+1);
	      h= rom->read(addr+2);
	      a= h*256+1;
	      work.appendf("($%04x)", a);
	      temp.appendf("; [$%04x]=$%04x", a, read_addr(rom, a));
	      break;
	    case 'z': // zpg
	      l= rom->read(addr+1);
	      work.appendf("$%04x", a= l);
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'X': // zpg.X
	      l= rom->read(addr+1);
	      work.appendf("$%04x,X", l);
	      l+= rX;
	      a= l;
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'Y': // zpg.Y
	      l= rom->read(addr+1);
	      work.appendf("$%04x,Y", l);
	      l+= rY;
	      a= l;
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'i': // abs,X
	      l= rom->read(addr+1);
	      h= rom->read(addr+2);
	      a= h*256+l;
	      work.appendf("$%04x,X", a);
	      a+= rX;
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'p': // abs,Y
	      l= rom->read(addr+1);
	      h= rom->read(addr+2);
	      a= h*256+l;
	      work.appendf("$%04x,Y", a);
	      a+= rY;
	      temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      break;
	    case 'r': // rel
	      l= rom->read(addr+1);
	      a= PC + (i8_t)l + 2;
	      work.appendf("$%04x", a);
	      break;
	    case '#': // imm8
	      l= rom->read(addr+1);
	      work.appendf("#$%02x", l);
	      break;
	    }
	  if (comment && temp.nempty())
	    comment->append(temp);
	}
      else
	work+= b[i];
    }

  return(strdup(work.c_str()));
}

t_addr
cl_mcs6502::read_addr(class cl_memory *m, t_addr start_addr)
{
  u8_t h, l;
  l= m->read(start_addr);
  h= m->read(start_addr+1);
  return h*256+l;
}

class cl_cell8 &
cl_mcs6502::imm8(void)
{
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(PC);
  fetch();
  tick(1);
  return *c;
}

class cl_cell8 &
cl_mcs6502::zpg(void)
{
  u8_t a= fetch();
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd++;
  tick(2);
  return *c;
}

class cl_cell8 &
cl_mcs6502::zpgX(void)
{
  u16_t a= fetch() + rX;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd++;
  tick(3);
  return *c;
}

class cl_cell8 &
cl_mcs6502::zpgY(void)
{
  u16_t a= fetch() + rY;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd++;
  tick(3);
  return *c;
}

class cl_cell8 &
cl_mcs6502::abs(void)
{
  u16_t a= i16();
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd++;
  tick(3);
  return *c;
}

class cl_cell8 &
cl_mcs6502::absX(void)
{
  u16_t a1= i16();
  u16_t a2= a1 + rX;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a2);
  vc.rd++;
  tick(3);
  if ((a1&0xff00) != (a2&0xff00))
    tick(1);
  return *c;
}

class cl_cell8 &
cl_mcs6502::absY(void)
{
  u16_t a1= i16();
  u16_t a2= a1 + rY;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a2);
  vc.rd++;
  tick(3);
  if ((a1&0xff00) != (a2&0xff00))
    tick(1);
  return *c;
}

class cl_cell8 &
cl_mcs6502::ind(void)
{
  u16_t a= i16();
  a= read_addr(rom, a);
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd+= 3;
  tick(3);
  return *c;
}

class cl_cell8 &
cl_mcs6502::indX(void)
{
  u8_t a0= fetch() + rX;
  u16_t a= read_addr(rom, a0);
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  vc.rd+= 3;
  tick(5);
  return *c;
}

class cl_cell8 &
cl_mcs6502::indY(void)
{
  u16_t a1= read_addr(rom, fetch());
  u16_t a2= a1 + rY;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a2);
  vc.rd+= 3;
  tick(4);
  if ((a1&0xff00) != (a2&0xff00))
    tick(1);
  return *c;
}

void
cl_mcs6502::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("A= $%02x %3d %+4d %c  ", A, A, (i8_t)A, isprint(A)?A:'.');
  con->dd_printf("X= $%02x %3d %+4d %c  ", X, X, (i8_t)X, isprint(X)?X:'.');
  con->dd_printf("Y= $%02x %3d %+4d %c  ", Y, Y, (i8_t)Y, isprint(Y)?Y:'.');
  con->dd_printf("\n");
  con->dd_printf("P= "); con->print_bin(CC, 8); con->dd_printf("\n");
  con->dd_printf("   NV BDIZC\n");

  con->dd_printf("S= ");
  rom->dump(0, 0x100+SP, 0x100+SP+7, 8, con);
  con->dd_color("answer");
  
  print_disass(PC, con);
}

int
cl_mcs6502::exec_inst(void)
{
  int res;

  if ((res= exec_inst_tab(itab)) != resNOT_DONE)
    return res;

  inst_unknown(rom->read(instPC));
  return(resINV_INST);
}

int
cl_mcs6502::accept_it(class it_level *il)
{
  class cl_it_src *is= il->source;

  tick(2);

  push_addr(PC);
  rom->write(0x0100 + rSP, rF);
  cSP.W(rSP-1);
  tick(1);
  vc.wr++;
  
  t_addr a= read_addr(rom, is->addr);
  tick(2);
  vc.rd+= 2;
  PC= a;

  rF|= flagI;
  is->clear();
  it_levels->push(il);
  
  return resGO;
}

bool
cl_mcs6502::it_enabled(void)
{
  return !(rF & flagI);
}

void
cl_mcs6502::push_addr(t_addr a)
{
  rom->write(0x0100 + rSP, (a>>8));
  cSP.W(rSP-1);
  rom->write(0x0100 + rSP, (a));
  cSP.W(rSP-1);
  tick(2);
  vc.wr+= 2;
}

t_addr
cl_mcs6502::pop_addr(void)
{
  u8_t h, l;
  cSP.W(rSP+1);
  l= rom->read(0x0100 + rSP);
  cSP.W(rSP+1);
  h= rom->read(0x0100 + rSP);
  tick(2);
  vc.rd+= 2;
  return h*256+l;
}


/* End of mcs6502.src/mcs6502.cc */
