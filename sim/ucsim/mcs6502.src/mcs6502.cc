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
cl_mcs6502::disass(t_addr addr)
{
  chars work= chars(), temp= chars();
  const char *b;
  t_mem code= rom->get(addr);
  struct dis_entry *dt= dis_tbl();//, *dis_e;
  int i;
  bool first;
  
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
	  switch (b[i])
	    {
	    }
	}
      else
	work+= b[i];
    }

  return(strdup(work.c_str()));
}

void
cl_mcs6502::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("A= 0x%02x %3d %+4d %c  ", A, A, (i8_t)A, isprint(A)?A:'.');
  con->dd_printf("X= 0x%02x %3d %+4d %c  ", X, X, (i8_t)X, isprint(X)?X:'.');
  con->dd_printf("Y= 0x%02x %3d %+4d %c  ", Y, Y, (i8_t)Y, isprint(Y)?Y:'.');
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
  t_mem code;
  int res= resGO;

  if ((res= exec_inst_tab(itab)) != resNOT_DONE)
    return res;

  instPC= PC;
  if (fetch(&code))
    return(resBREAKPOINT);
  tick(1);
  res= inst_unknown(code);
  return(res);
}

int
cl_mcs6502::accept_it(class it_level *il)
{
  class cl_it_src *is= il->source;

  tick(2);

  rom->write(0x0100 + rSP, (PC>>8)&0xff);
  rSP--;
  rom->write(0x0100 + rSP, (PC)&0xff);
  rSP--;
  rom->write(0x0100 + rSP, rF);
  rSP--;
  tick(3);
  vc.wr+= 3;
  
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


/* End of mcs6502.src/mcs6502.cc */
