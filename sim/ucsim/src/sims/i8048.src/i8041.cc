/*
 * Simulator of microcontrollers (i8041.cc)
 *
 * Copyright (C) 2022 Drotos Daniel
 * 
 * To contact author send email to dr.dkdb@gmail.com
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

#include "i8041cl.h"


/*
 * CPU
 */

cl_i8041::cl_i8041(class cl_sim *asim):
  cl_i8048(asim)
{
  rom_size= 1024;
  ram_size= 64;
  info_ch= '4';
}

cl_i8041::cl_i8041(class cl_sim *asim,
		   unsigned int rom_siz,
		   unsigned int ram_siz):
  cl_i8048(asim, rom_siz, ram_siz)
{
  info_ch= '4';
}

int
cl_i8041::init(void)
{
  cl_i8048::init();
  return 0;
}

class cl_memory_operator *
cl_i8041::make_flagop(void)
{
  class cl_memory_operator *o;
  o= new cl_flag48_op(cpsw);
  o->init();
  o->set_name("MCS41 flag operator");
  return o;
}

void
cl_i8041::make_irq_sources()
{
  class cl_it_src *is;

  it_sources->add
    (
     is= new cl_it_src
     (this, 0,
      &ints->cene, 1, // enable cell/mask
      cpu->cfg_cell(i8041cpu_status), stat_ibf, // requ cell/mask
      3, // addr
      false, //clr
      false, // indirect
      "External", // name
      1 // poll_priority
      ));
  is->init();

  it_sources->add
    (
     is= new cl_it_src
     (this, 1,
      &timer->cint_enabled, 1, // enable cell/mask
      &timer->cint_request, 1, // requ cell/mask
      7, // addr
      true, //clr
      false, // indirect
      "Timer", // name
      2 // poll_priority
      ));
  is->init();
}


void
cl_i8041::make_cpu_hw(void)
{
  cpu= new cl_i8041_cpu(this);
  add_hw(cpu);
  cpu->init();
}

void
cl_i8041::decode_regs(void)
{
  int i;
  cl_banker *b= new cl_banker(aspsw, 0, flagBS, //0,
			      regs, 0, 7);
  b->init();
  b->set_name("def_regs_banker");
  regs->decoders->add(b);
  b->add_bank(0, memory("iram_chip"), 0);
  b->add_bank(1, memory("iram_chip"), 24);
  cpsw->write(0);
  for (i= 0; i < 8; i++)
    R[i]= (cl_cell8*)regs->get_cell(i);
}


int
cl_i8041::OUTDBBA(MP)
{
  cpu->cfg_write(i8041cpu_out, rA);
  return resGO;
}

int
cl_i8041::INADBB(MP)
{
  cA.W(cpu->cfg_read(i8041cpu_in));
  u8_t s= cpu->cfg_read(i8041cpu_status);
  s&= ~stat_ibf;
  cpu->cfg_write(i8041cpu_status, s);
  return resGO;
}

int
cl_i8041::MOVSTSA(MP)
{
  u8_t s= cpu->cfg_read(i8041cpu_status);
  s&= 0x0f;
  s|= (rA & 0xf0);
  cpu->cfg_write(i8041cpu_status, s);
  return resGO;
}

int
cl_i8041::ENFLAGS(MP)
{
  cpu->cfg_set(i8041cpu_enflags, 1);
  return resGO;
}


/*
                               8041 CPU
*/

cl_i8041_cpu::cl_i8041_cpu(class cl_uc *auc):
  cl_i8020_cpu(auc)
{
}

int
cl_i8041_cpu::init(void)
{
  cl_var *v;
  cl_i8020_cpu::init();
  // variables...
  uc->vars->add(v= new cl_var("DBBIN_DATA", cfg, i8041cpu_in,
			      cfg_help(i8041cpu_in)));
  v->init();
  uc->vars->add(v= new cl_var("DBBIN_CTRL", cfg, i8041cpu_ctrl,
			      cfg_help(i8041cpu_ctrl)));
  v->init();
  uc->vars->add(v= new cl_var("DBBOUT", cfg, i8041cpu_out,
			      cfg_help(i8041cpu_out)));
  v->init();
  uc->vars->add(v= new cl_var("STATUS", cfg, i8041cpu_status,
			      cfg_help(i8041cpu_status)));
  v->init();
  uc->vars->add(v= new cl_var("ENFLAGS", cfg, i8041cpu_enflags,
			      cfg_help(i8041cpu_enflags)));
  v->init();
  uc->vars->add(v= new cl_var("OBFCLEAR", cfg, i8041cpu_obfclear,
			      cfg_help(i8041cpu_obfclear)));
  v->init();
  
  MCELL *cc= cfg_cell(i8041cpu_ctrl);
  MCELL *ci= cfg_cell(i8041cpu_in);
  cc->decode(ci);
  
  return 0;
}

void
cl_i8041_cpu::reset(void)
{
  cfg_set(i8041cpu_status, 0);
  cfg_set(i8041cpu_enflags, 0);
}

const char *
cl_i8041_cpu::cfg_help(t_addr addr)
{
  if (addr < i8020cpu_nuof)
    return cl_i8020_cpu::cfg_help(addr);
  switch (addr)
    {
    case i8041cpu_in: return "Input Buffer (as Data) register (int, RW)";
    case i8041cpu_ctrl: return "Input Buffer (as Control) register (int, RW)";
    case i8041cpu_out: return "Output Buffer register (int, RW)";
    case i8041cpu_status: return "Status register (int, RW)";
    case i8041cpu_obfclear: return "Clear OBF when written (int, WO)";
    case i8041cpu_enflags: return "Flags mode enable (bool, RW)";
    default:
      return "Not Used";
    }
  return "Not used";
}

t_mem
cl_i8041_cpu::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  class cl_i8041 *u= (class cl_i8041 *)uc;
  if (val)
    cell->set(*val);
  if (addr < i8020cpu_nuof)
    return cl_i8020_cpu::conf_op(cell, addr, val);
  switch (addr)
    {
    case i8041cpu_in: // input buffer, A0=0 -> F1=0
      if (val)
	{
	  cell->set(*val&= 0xff);
	  u->flagF1= 0;
	  u8_t stat= cfg_read(i8041cpu_status);
	  stat&= ~stat_ibf;
	  cfg_write(i8041cpu_status, stat);
	}
      break;
    case i8041cpu_ctrl: // input buffer, A0=1 -> F1=1
      if (val)
	{
	  cell->set(*val&= 0xff);
	  u->flagF1= 1;
	  u8_t stat= cfg_read(i8041cpu_status);
	  stat&= ~stat_ibf;
	  cfg_write(i8041cpu_status, stat);
	}
      break;
    case i8041cpu_out:
      if (val)
	cell->set(*val & 0xff);
      /*else
	{
	  u8_t stat= cfg_get(i8041cpu_status);
	  stat&= ~stat_obf;
	  cfg_set(i8041cpu_status, stat);
	  }*/
      break;
    case i8041cpu_obfclear:
      if (val)
	{
	  u8_t stat= cfg_read(i8041cpu_status);
	  stat&= ~stat_obf;
	  cfg_write(i8041cpu_status, stat);
	}
      cell->set(0);
      break;
    case i8041cpu_status:
      // replace F0, F1 bits from CPU
      if (val)
	{
	  *val&= 0xff;
	  *val&= ~(stat_f0|stat_f1);
	  if (u->flagF1) *val|= stat_f1;
	  if (u->psw & flagF0) *val|= stat_f0;
	  cell->set(*val);
	  set_flags();
	}
      else
	{
	  u8_t stat= cell->get() & ~(stat_f0|stat_f1);
	  if (u->flagF1) stat|= stat_f1;
	  if (u->psw & flagF0) stat|= stat_f0;
	  cell->set(stat);
	}
      break;
    case i8041cpu_enflags:
      if (val)
	{
	  cell->set((*val)?1:0);
	  set_flags();
	}
      break;
    }
  return cell->get();
}

void
cl_i8041_cpu::set_flags(void)
{
  class cl_i8041 *u= (class cl_i8041 *)uc;
  if (cfg_get(i8041cpu_enflags))
    {
      u8_t v= 0xff;
      u8_t s= cfg_get(i8041cpu_status);
      if (!(s & stat_obf))
	v&= ~0x10;
      if ((s & stat_ibf))
	v&= ~0x20;
      u->p2->flags41= v;
    }
  else
    u->p2->flags41= 0xff;
}

void
cl_i8041_cpu::print_info(class cl_console_base *con)
{
  class cl_i8041 *u= (class cl_i8041 *)uc;
   u8_t s= cfg_get(i8041cpu_status);
  con->dd_printf("DBBIN = 0x%02x\n", cfg_get(i8041cpu_in));
  con->dd_printf("DBBOUT= 0x%02x\n", cfg_get(i8041cpu_out));
  con->dd_printf("STATUS= 0x%02x IBF=%d OBF=%d flags41=0x%02x\n",
		 s, (s&stat_ibf)?1:0, (s&stat_obf)?1:0,
		 u->p2->flags41);
}


/* End of i8048.src/i8041.cc */
