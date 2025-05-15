/*
 * Simulator of microcontrollers (i8048.cc)
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

#include "dregcl.h"

#include "i8048cl.h"


t_mem
cl_flag48_op::write(t_mem val)
{
  val|= 0x08;
  return val;
}


/*
 * CPU
 */

cl_i8048::cl_i8048(class cl_sim *asim):
  cl_i8020(asim)
{
  rom_size= 1024;
  ram_size= 64;
  info_ch= '8';
  inner_rom= rom_size;
}

cl_i8048::cl_i8048(class cl_sim *asim,
		   unsigned int rom_siz,
		   unsigned int ram_siz):
  cl_i8020(asim)
{
  rom_size= rom_siz;
  ram_size= ram_siz;
  info_ch= '8';
  inner_rom= rom_size;
}

int
cl_i8048::init(void)
{
  class cl_memory_operator *o;

  cl_i8020::init();

  cflagF1.init();
  cflagF1.decode(&flagF1);
  o= new cl_bool_op(&cflagF1);
  o->init();
  cflagF1.append_operator(o);
  cflagF1.set_mask(1);

  mk_cvar(&cmb, "DBF", "CPU code bank selector");
  mk_cvar(&cmb, "A11", "CPU code bank selector");
  mk_cvar(&cflagF1, "F1", "CPU flag F1");

  return 0;
}

void
cl_i8048::reset(void)
{
  cl_i8020::reset();
  cflagF1.W(0);
}

void
cl_i8048::mk_hw_elements(void)
{
  cl_uc::mk_hw_elements();
  class cl_hw *h;
  
  timer= new cl_timer(this);
  timer->init();
  add_hw(timer);

  ints= new cl_ints(this);
  ints->init();
  add_hw(ints);
  
  bus= new cl_bus(this);
  bus->init();
  add_hw(bus);

  p0= NULL;
  p1= new cl_qport(this, 1, ports, 1, port_8bit);
  p1->init();
  add_hw(p1);
  p2= new cl_p2(this, 2, ports, 2, port_8bit);
  p2->init();
  add_hw(p2);
  
  pext= new cl_pext(this, 0, ports, 4, p2);
  pext->init();
  add_hw(pext);

  h= new cl_dreg(this, 0, "dreg");
  h->init();
  add_hw(h);

  mk_dport();
}


void
cl_i8048::mk_dport(void)
{
  cl_i8020::mk_dport();

  class cl_port_data d;
  d.init();
  d.width= 8;

  d.set_name("P1");
  d.cell_dir= NULL;
  d.cell_p  = p1->pcell;
  d.cell_in = p1->cfg_cell(port_pin);
  d.keyset  = keysets[1];
  d.basx    = 1+20;
  d.basy    = 6;
  dport->add_port(&d, 0);

  d.set_name("P2");
  d.cell_dir= NULL;
  d.cell_p  = p2->pcell;
  d.cell_in = p2->cfg_cell(port_pin);
  d.keyset  = keysets[2];
  d.basx    = 1+20+20;
  d.basy    = 6;
  dport->add_port(&d, 1);
}

class cl_memory_operator *
cl_i8048::make_flagop(void)
{
  class cl_memory_operator *o;
  o= new cl_flag48_op(cpsw);
  o->init();
  o->set_name("MCS48 flag operator");
  return o;
}

void
cl_i8048::make_irq_sources(void)
{
  class cl_it_src *is;

  it_sources->add
    (
     is= new cl_it_src
     (this, 0,
      &ints->cene, 1, // enable cell/mask
      &((class cl_i8020_cpu*)cpu)->cpins, ipm_int, // requ cell/mask
      3, // addr
      false, //clr
      false, // indirect
      "External", // name
      1 // poll_priority
      ));
  is->init();
  is->src_value= 0; // External is low level

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
cl_i8048::make_cpu_hw(void)
{
  cpu= new cl_i8048_cpu(this);
  add_hw(cpu);
  cpu->init();
}

void
cl_i8048::decode_regs(void)
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

u8_t
cl_i8048::movxrd(u8_t addr)
{
  bus->latch(addr);
  return xram->read(addr);
}

void
cl_i8048::movxwr(u8_t addr, u8_t val)
{
  xram->write(addr, val);
  bus->latch(val);
}

int
cl_i8048::orlbus(u8_t i8)
{
  RD;
  WR;
  bus->orl(i8);
  return resGO;
}

int
cl_i8048::anlbus(u8_t i8)
{
  RD;
  WR;
  bus->anl(i8);
  return resGO;
}

int
cl_i8048::call(MP)
{
  u16_t a= (code&0xe0)<<3;
  a+= fetch();
  if (A11) a|= 0x800;
  push(true);
  PC= a;
  return resGO;
}

int
cl_i8048::accept_it(class it_level *il)
{
  in_isr= true;
  // no tracking needed
  push(true);
  PC= il->addr;
  delete il;
  return resGO;
}


void
cl_i8048::print_regs(class cl_console_base *con)
{
  int start, stop, i;
  //t_mem data;
  u16_t dp;
  // show regs
  start= (psw & flagBS)?24:0;
  con->dd_color("answer");
  con->dd_printf("        R0 R1 R2 R3 R4 R5 R6 R7    PSW= CAF%c-SSS    ACC= ",
		 (type->type & CPU_MCS21)?'-':'B');
  con->dd_color("dump_number");
  con->dd_printf("0x%02x %+3d %c", ACC, ACC, (isprint(ACC)?ACC:'?'));
  con->dd_printf("\n");
  con->dd_cprintf("dump_address", "   0x%02x", start);
  for (int ii= 0; ii < 8; ii++)
    con->dd_cprintf("dump_number", " %02x", iram->get(start + ii));
  con->dd_cprintf("dump_number", "    0x%02x ", psw);
  con->dd_color("dump_number");
  con->print_bin(psw, 8);
  con->dd_printf("    DBF=%d F1=%d", mb, flagF1);
  con->dd_printf("\n");
  // show indirectly addressed IRAM and some basic regs
  start= R[0]->get();
  stop= start+7;
  con->dd_color("answer");
  con->dd_printf("R0=");
  class cl_dump_ads ads(start, stop);
  iram->dump(0, /*start, stop*/&ads, 8, con);
  start= R[1]->get();
  stop= start+7;
  con->dd_color("answer");
  con->dd_printf("R1=");
  ads._ss(start, stop);
  iram->dump(0, /*start, stop*/&ads, 8, con);

  con->dd_cprintf("answer", "SP=%d", psw&7);
  start= 8;
  stop= psw&7;
  for (i= 7; i>=0; i--)
    {
      dp= iram->read(start+2*i+1) * 256 + iram->read(start+2*i);
      con->dd_cprintf("dump_address", " %x", dp>>12);
      if (i<stop)
	con->dd_color("dump_address");
      else
	con->dd_color("dump_number");
      con->dd_printf(".%03x", dp&0xfff);
    }
  con->dd_printf("\n ");
  con->dd_color("answer");
  for (i= 7; i>=0; i--)
    {
      con->dd_printf("    %d", i);
      con->dd_printf("%c", (i==(psw&7))?'^':' ');
	
    }
  con->dd_printf("\n");

  print_disass(PC, con);
}


int
cl_i8048::OUTLB(MP)
{
  bus->latch(rA);
  return resGO;
}

int
cl_i8048::INS(MP)
{
  cA.W(bus->cfg_read(tbus_in));
  return resGO;
}


int
cl_i8048::ORLP1I8(MP)
{
  u8_t i8= fetch();
  u8_t v= p1->get_odr();
  RD;
  v|= i8;
  ports->write(1, v);
  WR;
  return resGO;
}

int
cl_i8048::ORLP2I8(MP)
{
  u8_t i8= fetch();
  u8_t v= p2->get_odr();
  RD;
  v|= i8;
  ports->write(2, v);
  WR;
  return resGO;
}


int
cl_i8048::ANLP1I8(MP)
{
  u8_t i8= fetch();
  u8_t v= p1->get_odr();
  RD;
  v&= i8;
  ports->write(1, v);
  WR;
  return resGO;
}

int
cl_i8048::ANLP2I8(MP)
{
  u8_t i8= fetch();
  u8_t v= p2->get_odr();
  RD;
  v&= i8;
  ports->write(2, v);
  WR;
  return resGO;
}

int
cl_i8048::RETR(MP)
{
  in_isr= false;
  PC= pop(true);
  return resGO;
}



cl_i8048_cpu::cl_i8048_cpu(class cl_uc *auc):
  cl_i8020_cpu(auc)
{
}

int
cl_i8048_cpu::init(void)
{
  cl_var *v;
  cl_i8020_cpu::init();
  // variables...
  uc->vars->add(v= new cl_var("INT", cfg, i8048cpu_int,
			      cfg_help(i8048cpu_int)));
  v->init();
  return 0;
}

const char *
cl_i8048_cpu::cfg_help(t_addr addr)
{
  if (addr < i8020cpu_nuof)
    return cl_i8020_cpu::cfg_help(addr);
  switch (addr)
    {
    case i8048cpu_int: return "INT input pin (bool, RW)";
    default:
      return "Not Used";
    }
  return "Not used";
}

t_mem
cl_i8048_cpu::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  class cl_i8020 *u= (class cl_i8020 *)uc;
  if (val)
    cell->set(*val);
  if (addr < i8020cpu_nuof)
    return cl_i8020_cpu::conf_op(cell, addr, val);
  switch (addr)
    {
    case i8048cpu_int:
      if (val)
	{
	  *val= (*val)?1:0;
	  ipins&= ~ipm_int;
	  ipins|= (*val)?ipm_int:0;
	}
      else
	cell->set((ipins & ipm_int)?1:0);
      break;
    }
  return cell->get();
}


/* End of i8048.src/i8048.cc */
