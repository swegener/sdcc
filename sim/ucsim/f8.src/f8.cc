/*
 * Simulator of microcontrollers (i8080.cc)
 *
 * Copyright (C) 2022 Drotos Daniel, Talker Bt.
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

#include <ctype.h>

#include "utils.h"

#include "dregcl.h"

#include "f8cl.h"


/* 
 * CPU
 */

cl_f8::cl_f8(class cl_sim *asim):
  cl_uc(asim)
{
}

int
cl_f8::init(void)
{
  cl_uc::init();
  fill_def_wrappers(itab);
  set_xtal(25000000);

#define RCV(R) reg_cell_var(&c ## R , &r ## R , "" #R "" , "CPU register " #R "")
  RCV(X); RCV(XH); RCV(XL);
  RCV(Y); RCV(YH); RCV(YL);
  RCV(Z); RCV(ZH); RCV(ZL);
  RCV(SP);
  RCV(F);
#undef RCV
  sp_limit= 0;
  prefixes= P_NONE;
  
  cF.W(urnd());
  cX.W(urnd());
  cY.W(urnd());
  cZ.W(urnd());
  cSP.W(urnd());
  
  reset();
  return 0;
}

const char *
cl_f8::id_string(void)
{
  return "F8";
}

void
cl_f8::reset(void)
{
  cl_uc::reset();
  clear_prefixes();
  PC= 0;
}

void
cl_f8::set_PC(t_addr addr)
{
  PC= addr&0xffff;
}


void
cl_f8::mk_hw_elements(void)
{
  class cl_hw *h;
  cl_uc::mk_hw_elements();

  add_hw(h= new cl_dreg(this, 0, "dreg"));
  h->init();
}

void
cl_f8::make_cpu_hw(void)
{
  cpu= new cl_f8_cpu(this);
  add_hw(cpu);
  cpu->init();
}

void
cl_f8::make_memories(void)
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


void
cl_f8::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("---HCNZO  Flags= 0x%02x\n", rF);
  con->dd_printf("%s\n", cbin(rF, 8).c_str());
  con->dd_printf("X= 0x%04x [X]= 0x%02x %3d %c\n",
                 rX, rom->get(rX), rom->get(rX),
                 isprint(rom->get(rX))?rom->get(rX):'.');
  con->dd_printf("Y= 0x%04x [Y]= 0x%02x %3d %c\n",
                 rY, rom->get(rY), rom->get(rY),
                 isprint(rom->get(rY))?rom->get(rY):'.');
  con->dd_printf("Z= 0x%04x [Z]= 0x%02x %3d %c\n",
                 rZ, rom->get(rZ), rom->get(rZ),
                 isprint(rom->get(rZ))?rom->get(rZ):'.');

  int i;
  con->dd_cprintf("answer", "SP= ");
  con->dd_cprintf("dump_address", "0x%04x ->", rSP);
  for (i= 0; i < 18; i++)
    {
      t_addr al;
      al= (rSP+i)&0xffff;
      con->dd_cprintf("dump_number", " %02x",
		      (u8_t)(rom->read(al)));
    }
  con->dd_printf("\n");
  
  print_disass(PC, con);
}


i16_t
cl_f8::sexd(void)
{
  i16_t v= fetch();
  if (v & 0x80) v|= 0xff00;
  return v;
}

void
cl_f8::push1(u8_t v)
{
  t_addr sp_before= rSP;
  rom->write(--rSP, v);
  cSP.W(rSP);
  vc.wr++;
  stack_write(sp_before);
}

void
cl_f8::push2(u16_t v)
{
  t_addr sp_before= rSP;
  rom->write(--rSP, v>>8);
  rom->write(--rSP, v);
  vc.wr+= 2;
  cSP.W(rSP);
  stack_write(sp_before);
}

void
cl_f8::push2(u8_t h, u8_t l)
{
  t_addr sp_before= rSP;
  rom->write(--rSP, h);
  rom->write(--rSP, l);
  vc.wr+= 2;
  cSP.W(rSP);
  stack_write(sp_before);
}

u8_t
cl_f8::pop1(void)
{
  u8_t v= rom->read(rSP++);
  cSP.W(rSP);
  vc.rd++;
  return v;
}

u16_t
cl_f8::pop2(void)
{
  u8_t h, l;
  l= rom->read(rSP++);
  h= rom->read(rSP++);
  cSP.W(rSP);
  vc.rd+= 2;
  return h*256+l;
}

void
cl_f8::stack_check_overflow(t_addr sp_before)
{
  if (rSP < sp_limit)
    {
      class cl_error_stack_overflow *e=
	new cl_error_stack_overflow(instPC, sp_before, rSP);
      e->init();
      error(e);
    }
}


// Memory cells according to 8 bit addressing modes

class cl_cell8 &
cl_f8::m_mm(void)
{
  u16_t a= fetch();
  a+= fetch()*256;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  return *c;
}

class cl_cell8 &
cl_f8::m_n_sp(void)
{
  u8_t n= fetch();
  u16_t a= rSP+n;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  return *c;
}

class cl_cell8 &
cl_f8::m_nn_z(void)
{
  u16_t nn= fetch();
  nn+= fetch()*256;
  u16_t a= nn+rZ;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  return *c;
}

class cl_cell8 &
cl_f8::m_y(void)
{
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(rY);
  return *c;
}

class cl_cell8 &
cl_f8::m_n_y(void)
{
  i8_t n= fetch();
  u16_t a= rY+n;
  class cl_cell8 *c= (class cl_cell8 *)rom->get_cell(a);
  return *c;
}

u16_t
cl_f8::a_mm(void)
{
  u16_t a= fetch();
  a+= fetch()*256;
  return a;
}

u16_t
cl_f8::a_n_sp(void)
{
  u8_t n= fetch();
  return n + rSP;
}

u16_t
cl_f8::a_nn_z(void)
{
  u16_t nn= fetch();
  nn+= fetch()*256;
  return nn + rZ;
}

u16_t
cl_f8::a_y(void)
{
  return rY;
}

u16_t
cl_f8::a_n_y(void)
{
  u8_t n= fetch();
  return n + rY;
}

u16_t
cl_f8::a_acc16(void)
{
  return acc16->get();
}

u16_t
cl_f8::a_n_acc16(void)
{
  u8_t n= fetch();
  return n + acc16->get();
}

u16_t
cl_f8::a_nn_acc16(void)
{
  u16_t nn= fetch();
  nn+= fetch()*256;
  return nn + acc16->get();
}


void
cl_f8::clear_prefixes()
{
  prefixes= P_NONE;
  acc8= &cXL;
  acc16= &cY;
}

int
cl_f8::exec_inst(void)
{
  int res= resGO;
  t_mem code;

  instPC= PC;
  if (fetch(&code))
    return resBREAKPOINT;
  while ((code & PREF_MASK) == PREF)
    {
      switch (code)
	{
	case PREF_SWAPOP: // swapop
	  prefixes|= P_SWAP;
	  break;
	case PREF_ALT0: // altacc
	  prefixes|= P_ALT0;
	  break;
	case PREF_ALT1: // altacc'
	  prefixes|= P_ALT1;
	  break;
	case PREF_ALT2: // altacc''
	  prefixes|= P_ALT2;
	  break;
	}
      if (fetch(&code))
	return resBREAKPOINT;
    }
  prefixes&= allowed_prefs[code]; // drop swap if not allowed!
  // select accumulators according to prefixes
  if (prefixes & P_ALT0)
    {
      acc8 = &cXH;
      acc16= &cY;
    }
  else if (prefixes & P_ALT1)
    {
      acc8 = &cYL;
      acc16= &cX;
    }
  else if (prefixes & P_ALT2)
    {
      acc8 = &cZL;
      acc16= &cY;
    }
  /*
    // clear_prefixes() prepares this state
    else
    {
      acc8 = &cXL;
      acc16= &cY;
    }
  */
  if (itab[code] == NULL)
    {
      PC= instPC;
      clear_prefixes();
      return resNOT_DONE;
    }
  tick(1);
  res= itab[code](this, code);
  if (res == resNOT_DONE)
    {
      //PC= instPC;
      clear_prefixes();
      return res;
    }
  clear_prefixes();
  return res;
}


/*
 * CPU hw
 */

cl_f8_cpu::cl_f8_cpu(class cl_uc *auc):
  cl_hw(auc, HW_CPU, 0, "cpu")
{
}

int
cl_f8_cpu::init(void)
{
  cl_hw::init();

  cl_var *v;
  uc->vars->add(v= new cl_var("sp_limit", cfg, f8cpu_sp_limit,
			      cfg_help(f8cpu_sp_limit)));
  v->init();
  
  return 0;
}

const char *
cl_f8_cpu::cfg_help(t_addr addr)
{
  switch (addr)
    {
    case f8cpu_sp_limit:
      return "Stack overflows when SP is below this limit";
    }
  return "Not used";
}

t_mem
cl_f8_cpu::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  class cl_f8 *u= (class cl_f8 *)uc;
  if (val)
    cell->set(*val);
  switch ((enum f8cpu_confs)addr)
    {
    case f8cpu_sp_limit:
      if (val)
	u->sp_limit= *val & 0xffff;
      else
	cell->set(u->sp_limit);
      break;
    case f8cpu_nuof: break;
    }
  return cell->get();
}


/* End of f8.src/f8.cc */
