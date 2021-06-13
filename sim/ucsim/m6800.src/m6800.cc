/*
 * Simulator of microcontrollers (m6800.cc)
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
#include "ciacl.h"
#include "piacl.h"

#include "glob.h"
#include "irqcl.h"

#include "m6800cl.h"


cl_m6800::cl_m6800(class cl_sim *asim):
  cl_uc(asim)
{
}

int
cl_m6800::init(void)
{
  cl_uc::init();
  fill_def_wrappers(itab);
  
  xtal= 1000000;
    
#define RCV(R) reg_cell_var(&c ## R , &r ## R , "" #R "" , "CPU register " #R "")
  RCV(A);
  RCV(B);
  RCV(CC);
  RCV(IX);
  RCV(SP);
#undef RCV
    
  class cl_memory_operator *op= new cl_cc_operator(&cCC);
  cCC.append_operator(op);

  wai= false;
  
  return 0;
}

const char *
cl_m6800::id_string(void)
{
  return "M6800";
}

void
cl_m6800::reset(void)
{
  cl_uc::reset();

  CC= 0xc0;
  PC= rom->read(0xfffe)*256 + rom->read(0xffff);
  tick(6);
}
  
void
cl_m6800::set_PC(t_addr addr)
{
  PC= addr;
}

void
cl_m6800::mk_hw_elements(void)
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
			 h->cfg_cell(m68_irq), 1,
			 IRQ_AT,
			 true,
			 true,
			 "Interrupt request",
			 0);
  src_irq->set_cid('i');
  src_irq->init();
  src_irq->set_ie_value(0);
  it_sources->add(src_irq);
  
  src_nmi= new cl_it_src(this,
			 irq_nmi,
			 h->cfg_cell(m68_nmi_en), 1,
			 h->cfg_cell(m68_nmi), 1,
			 NMI_AT,
			 true,
			 true,
			 "Non-maskable interrupt request",
			 0);
  src_nmi->set_cid('n');
  src_nmi->init();
  it_sources->add(src_nmi);
  
  src_swi= new cl_it_src(this,
			 irq_swi,
			 h->cfg_cell(m68_swi_en), 1,
			 h->cfg_cell(m68_swi), 1,
			 SWI_AT,
			 true,
			 true,
			 "SWI",
			 0);
  src_swi->set_cid('s');
  src_swi->init();
  it_sources->add(src_swi);
  
  add_hw(h= new cl_cia(this, 0, 0x8000));
  h->init();

  add_hw(h= new cl_cia(this, 1, 0x8008));
  h->init();

  class cl_pia *p0, *p1;
  
  add_hw(p0= new cl_pia(this, 0, 0x8010));
  p0->init();
  add_hw(p1= new cl_pia(this, 1, 0x8020));
  p1->init();

  class cl_port_ui *d;
  add_hw(d= new cl_port_ui(this, 0, "dport"));
  d->init();

  class cl_port_data pd;
  pd.init();
  pd.set_name("P0A");
  pd.cell_dir= p0->ddra;
  pd.cell_p  = p0->ora;
  pd.cell_in = p0->ina;
  pd.keyset  = keysets[0];
  pd.basx    = 1;
  pd.basy    = 5;
  d->add_port(&pd, 0);

  pd.set_name("P0B");
  pd.cell_dir= p0->ddrb;
  pd.cell_p  = p0->orb;
  pd.cell_in = p0->inb;
  pd.keyset  = keysets[1];
  pd.basx    = 20;
  pd.basy    = 5;
  d->add_port(&pd, 1);

  pd.set_name("P0CA");
  pd.cell_dir= p0->ddca;
  pd.cell_p  = p0->oca;
  pd.cell_in = p0->inca;
  pd.cell_dir= p0->ddca;
  pd.keyset  = keysets[2];
  pd.basx    = 40;
  pd.basy    = 5;
  pd.width   = 2;
  d->add_port(&pd, 2);

  pd.set_name("P0CB");
  pd.cell_dir= p0->ddcb;
  pd.cell_p  = p0->ocb;
  pd.cell_in = p0->incb;
  pd.cell_dir= p0->ddcb;
  pd.keyset  = keysets[3];
  pd.basx    = 54;
  pd.basy    = 5;
  pd.width   = 2;
  d->add_port(&pd, 3);

  // Port #1
  pd.init();
  pd.set_name("P1A");
  pd.cell_dir= p1->ddra;
  pd.cell_p  = p1->ora;
  pd.cell_in = p1->ina;
  pd.keyset  = keysets[4];
  pd.basx    = 1;
  pd.basy    = 11;
  d->add_port(&pd, 4);

  pd.set_name("P1B");
  pd.cell_dir= p1->ddrb;
  pd.cell_p  = p1->orb;
  pd.cell_in = p1->inb;
  pd.keyset  = keysets[5];
  pd.basx    = 20;
  pd.basy    = 11;
  d->add_port(&pd, 5);

  pd.set_name("P1CA");
  pd.cell_dir= p1->ddca;
  pd.cell_p  = p1->oca;
  pd.cell_in = p1->inca;
  pd.cell_dir= p1->ddca;
  pd.keyset  = keysets[6];
  pd.basx    = 40;
  pd.basy    = 11;
  pd.width   = 2;
  d->add_port(&pd, 6);

  pd.set_name("P1CB");
  pd.cell_dir= p1->ddcb;
  pd.cell_p  = p1->ocb;
  pd.cell_in = p1->incb;
  pd.cell_dir= p1->ddcb;
  pd.keyset  = keysets[7];
  pd.basx    = 54;
  pd.basy    = 11;
  pd.width   = 2;
  d->add_port(&pd, 7);
}

void
cl_m6800::make_cpu_hw(void)
{
}

void
cl_m6800::make_memories(void)
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
cl_m6800::dis_tbl(void)
{
  return(disass_m6800);
}

struct dis_entry *
cl_m6800::get_dis_entry(t_addr addr)
{
  struct dis_entry *dt= dis_tbl();//, *dis_e;
  int i= 0;
  t_mem code= rom->get(addr);

  if (dt == NULL)
    return NULL;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  return &dt[i];
}

char *
cl_m6800::disassc(t_addr addr, chars *comment)
{
  chars work= chars(), temp= chars();
  const char *b;
  //t_mem code= rom->get(addr);
  struct dis_entry *dis_e;
  int i;
  bool first;
  
  if ((dis_e= get_dis_entry(addr)) == NULL)
    return NULL;
  if (dis_e->mnemonic == NULL)
    return strdup("-- UNKNOWN/INVALID");
  b= dis_e->mnemonic;

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
	  t_addr a;
	  u8_t h, l;
	  i++;
	  temp= "";
	  switch (b[i])
	    {
	    case 'x': case 'X': // indexed
	      h= rom->read(addr+1);
	      a= rX+h;
	      work.appendf("$%02x,X", h);
	      //add_spaces(&work, 20);
	      if (b[i]=='x')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
		temp.appendf("; [$%04x]=$%04x", a, read_addr(rom, a));
	      break;
	    case 'e': case 'E': // extended
	      h= rom->read(addr+1);
	      l= rom->read(addr+2);
	      a= h*256 + l;
	      work.appendf("$%04x", a);
	      //add_spaces(&work, 20);
	      if (b[i]=='e')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
		temp.appendf("; [$%04x]=$%04x", a,
			     read_addr(rom, a));
	      break;
	    case 'd': case 'D': // direct
	      h= a= rom->read(addr+1);
	      work.appendf("$00%02x", h);
	      //add_spaces(&work, 20);
	      if (b[i]=='d')
		temp.appendf("; [$%04x]=$%02x", a, rom->read(a));
	      else
	        temp.appendf("; [$%04x]=$%04x", a,
			     read_addr(rom, a));
	      break;
	    case 'b': // immediate 8 bit
	      work.appendf("#$%02x",
			   rom->read(addr+1));
	      break;
	    case 'B': // immediate 16 bit
	      work.appendf("#$%04x",
			   read_addr(rom, addr+1));
	      break;
	    case 'r': // relative
	      work.appendf("$%04x",
			   (addr+2+(i8_t)(rom->read(addr+1))) & 0xffff );
	      break;
	    }
	  //work+= temp;
	  if (comment && temp.nempty())
	    comment->append(temp);
	}
      else
	work+= b[i];
    }

  return(strdup(work.c_str()));
}

void
cl_m6800::analyze(t_addr addr)
{
  struct dis_entry *di;
  t_addr pa, ta;
  
  di= get_dis_entry(addr);
  while (!inst_at(addr) && di && (di->mnemonic!=NULL))
    {
      pa= addr;
      set_inst_at(addr);
      switch (di->branch)
	{
	case 'r': // uncond jump rel
	  {
	    i8_t r= rom->read(addr+1);
	    ta= addr+2+r;
	    addr= ta;
	  }
	  break;
	case 'R': // conditional jump rel
	  {
	    i8_t r= rom->read(addr+1);
	    ta= addr+2+r;
	    analyze(ta);
	  }
	  break;
	case 's': // SWI
	  ta= read_addr(rom, SWI_AT);
	  analyze(ta);
	  break;
	case 'E': // call extended
	  ta= read_addr(rom, addr+1);
	  analyze(ta);
	  break;
	case 'e': // jump extended
	  addr= read_addr(rom, addr+1);
	  break;
	case '_':
	  return;
	default:
	  addr= rom->validate_address(addr+di->length);
	  break;
	}
      if (pa == addr)
	return;
      di= get_dis_entry(addr);
    }
}

t_addr
cl_m6800::read_addr(class cl_memory *m, t_addr start_addr)
{
  u8_t h, l;
  h= m->read(start_addr);
  l= m->read(start_addr+1);
  return h*256 + l;
}

void
cl_m6800::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("A= $%02x %3d %+4d %c  ", A, A, (i8_t)A, isprint(A)?A:'.');
  con->dd_printf("B= $%02x %3d %+4d %c  ", B, B, (i8_t)B, isprint(B)?B:'.');
  con->dd_printf("\n");
  con->dd_printf("CC= "); con->print_bin(CC, 8); con->dd_printf("\n");
  con->dd_printf("      HINZVC\n");

  con->dd_printf("IX= ");
  rom->dump(0, IX, IX+7, 8, con);
  con->dd_color("answer");
  
  con->dd_printf("SP= ");
  rom->dump(0, SP, SP+7, 8, con);
  con->dd_color("answer");
  
  print_disass(PC, con);
}

int
cl_m6800::exec_inst(void)
{
  int res= resGO;

  if ((res= exec_inst_tab(itab)) != resNOT_DONE)
    return res;

  inst_unknown(rom->read(instPC));
  return(res);
}

int
cl_m6800::accept_it(class it_level *il)
{
  class cl_it_src *is= il->source;

  if (!wai)
    push_regs();
  wai= false;
  
  if ((is == src_irq) ||
      (is == src_swi))
    rCC|= flagI;
  
  t_addr a= read_addr(rom, is->addr);
  PC= a;
  
  is->clear();
  it_levels->push(il);
  
  return resGO;
}

void
cl_m6800::push_regs(void)
{
  rom->write(rSP--, PC&0xff);
  rom->write(rSP--, PC>>8);
  rom->write(rSP--, rIX&0xff);
  rom->write(rSP--, rIX>>8);
  rom->write(rSP--, rA);
  rom->write(rSP--, rB);
  rom->write(rSP--, rCC);
  tick(7);
}

void
cl_m6800::pull_regs(void)
{
  u8_t l, h;
  rCC= rom->read(++rSP);
  rB= rom->read(++rSP);
  rA= rom->read(++rSP);
  l= rom->read(++rSP);
  h= rom->read(++rSP);
  rIX= h*256+l;
  l= rom->read(++rSP);
  h= rom->read(++rSP);
  PC= h*256+l;
  tick(7);
}

class cl_cell8 &
cl_m6800::idx(void)
{
  t_addr a= fetch();
  a+= rX;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  tick(3);
  return *c;
}

class cl_cell8 &
cl_m6800::ext(void)
{
  t_addr a;
  u8_t h, l;
  h= fetch();
  l= fetch();
  a= h*256 + l;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  tick(2);
  return *c;
}

class cl_cell8 &
cl_m6800::dir(void)
{
  t_addr a= fetch();
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  return *c;
}

u16_t
cl_m6800::iop16(void)
{
  t_addr a= iaddr();
  vc.rd+= 2;
  return read_addr(rom, a);
}

u16_t
cl_m6800::eop16(void)
{
  t_addr a= eaddr();
  vc.rd+= 2;
  return read_addr(rom, a);
}

u16_t
cl_m6800::dop16(void)
{
  t_addr a= daddr();
  vc.rd+= 2;
  return read_addr(rom, a);
}

t_addr
cl_m6800::iaddr(void)
{
  t_addr a= fetch();
  a+= rX;
  return a;
}

t_addr
cl_m6800::eaddr(void)
{
  t_addr a;
  u8_t h, l;
  h= fetch();
  l= fetch();
  a= h*256 + l;
  return a;
}

t_addr
cl_m6800::daddr(void)
{
  t_addr a= fetch();
  return a;
}

t_addr
cl_m6800::raddr(void)
{
  i8_t a= fetch();
  return PC+a;
}

/* End of m6800.src/m6800.cc */
