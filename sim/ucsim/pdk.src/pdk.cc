/*
 * Simulator of microcontrollers (pdk.cc)
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

//#include "ddconfig.h"

//#include <ctype.h>
//#include <stdarg.h> /* for va_list */
//#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "i_string.h"

// prj
#include "globals.h"
//#include "pobjcl.h"

// sim
//#include "simcl.h"
#include "dregcl.h"

// local
#include "glob.h"
#include "pdk16cl.h"
//#include "portcl.h"
//#include "regspdk.h"

/*******************************************************************/

cl_fppa::cl_fppa(int aid, class cl_pdk *the_puc, class cl_sim *asim):
  cl_uc(asim)
{
  id= aid;
  puc= the_puc;
  PCmask= 0xfff;
}


/*
 * Base type of PDK controllers
 */

cl_fppa::cl_fppa(int aid, class cl_pdk *the_puc, struct cpu_entry *IType, class cl_sim *asim) : cl_uc(asim)
{
  id= aid;
  puc= the_puc;
  type = IType;
}

int cl_fppa::init(void) {
  cl_uc::init(); /* Memories now exist */

  //set_xtal(8000000);

  // rom = address_space(MEM_ROM_ID);
  // ram = mem(MEM_XRAM);
  // ram = rom;

  // zero out ram(this is assumed in regression tests)
  // for (int i = 0x0; i < 0x8000; i++) {
  //   ram->set((t_addr)i, 0);
  // }

  cA= new cl_cell8();
  cA->init();
  cA->decode(&rA);

  return (0);
}


void
cl_fppa::act(void)
{
  cSP->decode(&rSP);
  cF ->decode(&rF);
}

void cl_fppa::reset(void) {
  cl_uc::reset();
  sp_most = 0x00;

  PC = id;
  rA = 0;
  for (t_addr i = 0; i < io_size; ++i) {
    store_io(i, 0);
  }
}

void cl_fppa::mk_hw_elements(void)
{
  // TODO: Add hardware stuff here.
  class cl_hw *h;
  cl_uc::mk_hw_elements();

  add_hw(h= new cl_dreg(this, 0, "dreg"));
  h->init();
}

void cl_fppa::make_memories(void)
{
  class cl_address_space *as;
  int rom_storage, ram_storage;

  if (puc != NULL)
    {
      ram= puc->ram;
      rom= puc->rom;
      sfr= puc->sfr;
    }
  else
    {
      rom_storage = 0x1000;
      ram_storage = 0x100;
      rom = as = new cl_address_space("rom", 0, rom_storage, 16);
      as->init();
      address_spaces->add(as);
      ram = as = new cl_address_space("ram", 0, ram_storage, 8);
      as->init();
      address_spaces->add(as);
      sfr = as = new cl_address_space("regs8", 0, io_size + 1, 8);
      as->init();
      address_spaces->add(as);
      
      class cl_address_decoder *ad;
      class cl_memory_chip *chip;
    
      chip = new cl_chip16("rom_chip", rom_storage, 16);
      chip->init();
      memchips->add(chip);
      
      ad = new cl_address_decoder(as = rom, chip, 0, rom_storage-1, 0);
      ad->init();
      as->decoders->add(ad);
      ad->activate(0);
      
      chip = new cl_chip16("ram_chip", ram_storage, 8);
      chip->init();
      memchips->add(chip);
      
      ad = new cl_address_decoder(as = ram, chip, 0, ram_storage-1, 0);
      ad->init();
      as->decoders->add(ad);
      ad->activate(0);
      
      chip = new cl_chip16("io_chip", io_size, 8);
      chip->init();
      memchips->add(chip);
      
      ad = new cl_address_decoder(as = sfr, chip, 0, io_size-1, 0);
      ad->init();
      as->decoders->add(ad);
      ad->activate(0);
   
      // extra byte of the IO memory will point to the A register just for the debugger
      sfr->get_cell(io_size)->decode(&(rA));
    }
  
  cSP= sfr->get_cell(2);
  cF = sfr->get_cell(0);
  act();
}


void
cl_fppa::build_cmdset(class cl_cmdset *cmdset)
{
  if (puc == NULL)
    cl_uc::build_cmdset(cmdset);
}


/*
 * Help command interpreter
 */

struct dis_entry *cl_fppa::dis_tbl(void) {
  switch (type->type) {
  case CPU_PDK13:
    return (disass_pdk_13);
  case CPU_PDK14:
    return (disass_pdk_14);
  case CPU_PDK15:
    return (disass_pdk_15);
  default:
    return NULL;//__builtin_unreachable();
  }
}


int cl_fppa::inst_length(t_addr /*addr*/) {
  return 1;
}

int cl_fppa::inst_branch(t_addr addr) {
  int b;

  get_disasm_info(addr, NULL, &b, NULL, NULL);

  return b;
}

bool cl_fppa::is_call(t_addr addr) {
  struct dis_entry *e;

  get_disasm_info(addr, NULL, NULL, NULL, &e);

  return e ? (e->is_call) : false;
}

int cl_fppa::longest_inst(void) { return 1; }

const char *cl_fppa::get_disasm_info(t_addr addr, int *ret_len, int *ret_branch,
                                    int *immed_offset,
                                    struct dis_entry **dentry) {
  const char *b = NULL;
  int immed_n = 0;
  int start_addr = addr;
  struct dis_entry *instructions;

  switch (type->type) {
      /* Dispatch to appropriate pdk port. */
      case CPU_PDK13:
        instructions = disass_pdk_13;
        break;

      case CPU_PDK14:
        instructions = disass_pdk_14;
        break;

      case CPU_PDK15:
        instructions = disass_pdk_15;
        break;

      default:
        return "";//__builtin_unreachable();
  }

  uint code = rom->get(addr++);
  int i = 0;
  while ((code & instructions[i].mask) != instructions[i].code &&
         instructions[i].mnemonic)
    i++;
  dis_entry *dis_e = &instructions[i];
  b = instructions[i].mnemonic;

  if (ret_branch) {
    *ret_branch = dis_e->branch;
  }

  if (immed_offset) {
    if (immed_n > 0)
      *immed_offset = immed_n;
    else
      *immed_offset = (addr - start_addr);
  }

  if (ret_len) *ret_len = 1;

  if (dentry) *dentry = dis_e;

  return b;
}

char *cl_fppa::disass(t_addr addr)
{
  chars work, temp;
  const char *b;
  int len = 0;
  int immed_offset = 0;
  struct dis_entry *dis_e;
  bool first= true;
  
  work= "";

  b = get_disasm_info(addr, &len, NULL, &immed_offset, &dis_e);

  if (b == NULL)
    {
      return (strdup("UNKNOWN/INVALID"));
    }

  while (*b)
    {
      if ((*b == ' ') && first)
	{
	  first= false;
	  while (work.len() < 6) work.append(' ');
	}
      if (*b == '%')
	{
	  temp= "";
	  b++;
	  uint code = rom->get(addr) & ~(uint)dis_e->mask;
	  switch (*(b++))
	    {
	    case 'k':  // k    immediate addressing
	      temp.format("#0x%x", code);
	      break;
	    case 'm':  // m    memory addressing
	      if (*b == 'n') {
		switch (type->type) {
		case CPU_PDK13:
		  code &= 0x0F;
		  break;
		case CPU_PDK14:
		  code &= 0x3F;
		  break;
		case CPU_PDK15:
		  code &= 0x7F;
		  break;
		default:
		  ;//__builtin_unreachable();
		}
		++b;
	      }
	      temp.format("0x%x", code);
	      break;
	    case 'i':  // i    IO addressing
	      // TODO: Maybe add pretty printing.
	      if (*b == 'n') {
		switch (type->type) {
		case CPU_PDK13:
		  code &= 0x1F;
		  break;
		case CPU_PDK14:
		  code &= 0x3F;
		  break;
		case CPU_PDK15:
		  code &= 0x7F;
		  break;
		default:
		  ;//__builtin_unreachable();
		}
		
		++b;
	      }
	      temp.format("[0x%x]", code);
	      break;
	    case 'n':  // n    N-bit addressing
	      uint n;
	      switch (type->type) {
	      case CPU_PDK13:
		n = (code & 0xE0) >> 5;
		break;
	      case CPU_PDK14:
		n = (code & 0x1C0) >> 6;
		break;
	      case CPU_PDK15:
		n = (code & 0x380) >> 7;
		break;
	      default:
		n= 0;//__builtin_unreachable();
	      }
	      temp.format("#0x%x", n);
	      break;
	    default:
	      temp= "%?";
	      break;
	    }
	  work+= temp;
	}
      else
	work+= *(b++);
    }

  return strdup(work.c_str());
}

void
cl_fppa::print_regs(class cl_console_base *con)
{
  act();
  con->dd_color("answer");
  con->dd_printf("A = %02x %3u\n", rA, rA);
  con->dd_printf("       OACZ\n");
  con->dd_printf("F = %02x ", rF);
  con->dd_printf("%d%d%d%d\n", fO, fA, fC, fZ);
  con->dd_printf("SP= %02x\n", rSP);
  print_disass(PC, con);
}

/*
 * Execution
 */

int cl_fppa::exec_inst(void)
{
  t_mem code;

  act();
  instPC= PC;
  if (fetch(&code)) {
    return (resBREAKPOINT);
  }
  tick(1);

  int status = execute(code);
  if (status == resINV_INST)
    {
      //PC = instPC;//rom->inc_address(PC, -1);
      return (resINV_INST);
    }
  return (status);
}


void
cl_fppa::stack_check_overflow(void)
{
  if (0)
    {
      class cl_stack_op *op;
      op= new cl_stack_op(stack_push, instPC, rSP-1, rSP);
      class cl_error_stack_overflow *e=
	new cl_error_stack_overflow(op);
      e->init();
      error(e);
    }
}


/****************************************************************************/


/* Set nr of active FPP */

t_mem
cl_act_cell::write(t_mem val)
{
  val= puc->set_act(val);
  return cl_pdk_cell::write(val);
}

/* Set nr of FPPs */

t_mem
cl_nuof_cell::write(t_mem val)
{
  val= puc->set_nuof(val);
  return cl_pdk_cell::write(val);
}


cl_fppen_op::cl_fppen_op(class cl_pdk *the_puc, class cl_memory_cell *acell):
  cl_memory_operator(acell)
{
  puc= the_puc;
}

t_mem
cl_fppen_op::write(t_mem val)
{
  return puc->set_fppen(val);
}


/*
 * PDK uc
 */

cl_pdk::cl_pdk(struct cpu_entry *IType, class cl_sim *asim):
  cl_uc(asim)
{
  int i;
  type = IType;
  for (i= 0; i<8; i++)
    fpps[i]= NULL;
}

int
cl_pdk::init(void)
{
  cl_uc::init();
  class cl_fppen_op *op;
  fpps[0]= mk_fppa(0);

  cFPPEN= sfr->get_cell(1);
  op= new cl_fppen_op(this, cFPPEN);
  op->init();
  cFPPEN->append_operator(op);
  reg_cell_var(cFPPEN, &rFPPEN, "FPPEN", "FPP unit Enable Register");
  mk_cvar(sfr->get_cell(0), "FLAG", "ACC Status Flag Register");
  mk_cvar(sfr->get_cell(2), "SP", "Stack Pointer Register");

  cact= new cl_act_cell(this);
  reg_cell_var(cact, &act, "fpp", "ID of actual FPPA");
  nuof_fpp= 1;
  cnuof_fpp= new cl_nuof_cell(this);
  reg_cell_var(cnuof_fpp, &nuof_fpp, "nuof_fpp", "Number of FPPs");

  if (type->type == CPU_PDKX)
    {
      fpps[1]= mk_fppa(1);
      fpps[2]= mk_fppa(2);
      fpps[3]= mk_fppa(3);
      fpps[4]= mk_fppa(4);
      fpps[5]= mk_fppa(5);
      fpps[6]= mk_fppa(6);
      fpps[7]= mk_fppa(7);
      nuof_fpp= 8;
    }
  
  act= 0;
  rFPPEN= 1;
  return 0;
}

const char *
cl_pdk::id_string(void)
{
  switch (type->type)
    {
    case CPU_PDK13:
      return("pdk13");
    case CPU_PDK14:
      return("pdk14");
    case CPU_PDK15:
      return("pdk15");
    case CPU_PDK16:
      return("pdk16");
    default:
      return("pdk");
  }
}

void
cl_pdk::make_memories(void)
{
  class cl_address_space *as;
  class cl_address_decoder *ad;
  class cl_memory_chip *chip;
  int rom_size= 0x1000, ram_size=0x100;

  rom = as = new cl_address_space("rom", 0, rom_size, 16);
  as->init();
  address_spaces->add(as);
  ram = as = new cl_address_space("ram", 0, ram_size, 8);
  as->init();
  address_spaces->add(as);
  sfr = as = new cl_address_space("regs8", 0, io_size + 1, 8);
  as->init();
  address_spaces->add(as);

  chip = new cl_chip16("rom_chip", rom_size, 16);
  chip->init();
  memchips->add(chip);

  ad = new cl_address_decoder(as = rom, chip, 0, rom_size-1, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  chip = new cl_chip16("ram_chip", ram_size, 8);
  chip->init();
  memchips->add(chip);
  
  ad = new cl_address_decoder(as = ram, chip, 0, ram_size-1, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
    
  chip = new cl_chip16("io_chip", io_size, 8);
  chip->init();
  memchips->add(chip);

  ad = new cl_address_decoder(as = sfr, chip, 0, io_size-1, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
}

class cl_fppa *
cl_pdk::mk_fppa(int id)
{
  class cl_fppa *fppa;
  switch (type->type)
    {
    case CPU_PDK13: fppa= new cl_fppa13(id, this, sim); break;
    case CPU_PDK14: fppa= new cl_fppa14(id, this, sim); break;
    case CPU_PDK15: fppa= new cl_fppa15(id, this, sim); break;
    case CPU_PDK16: fppa= new cl_fppa16(id, this, sim); break;
    case CPU_PDKX:  fppa= new cl_fppa15(id, this, sim); break;
    default: fppa= new cl_fppa14(id, this, sim); break;
    }  
  fppa->init();
  return fppa;
}


u8_t
cl_pdk::set_fppen(u8_t val)
{
  int i;
  u8_t m;
  if (val == 0)
    val= 1;
  for (i=0, m=1; i<8; i++, m<<=1)
    {
      if (fpps[i] == NULL)
	val&= ~m;
    }
  return val;
}

u8_t
cl_pdk::set_act(u8_t val)
{
  if (val < nuof_fpp)
    return val;
  return 0;
}

u8_t
cl_pdk::set_nuof(u8_t val)
{
  int i;
  if (val > 8)
    val= 8;
  if (val<1)
    val= 1;
  for (i=0; i<8; i++)
    {
      if (i<val)
	{
	  if (fpps[i] == NULL)
	    fpps[i]= mk_fppa(i);
	  else
	    fpps[i]->reset();
	}
      else
	{
	  if (fpps[i] != NULL)
	    {
	      delete fpps[i];
	      fpps[i]= NULL;
	    }
	}
    }
  if (rFPPEN == 0)
    set_fppen(1);
  else
    set_fppen(rFPPEN);
  return val;
}


int
cl_pdk::exec_inst(void)
{
  while (!(rFPPEN & (1<<act)))
    act= (act+1)%nuof_fpp;
  fpps[act]->pre_inst();
  int ret= fpps[act]->exec_inst();
  fpps[act]->post_inst();
  tick(inst_ticks= fpps[act]->inst_ticks);
  if (rFPPEN != 1)
    {
      do
	act= (act+1)%nuof_fpp;
      while (!(rFPPEN & (1<<act)));
    }
  return ret;
}


void
cl_pdk::print_regs(class cl_console_base *con)
{
  int i;
  
  for (i= 0; i<nuof_fpp; i++)
    {
      //con->dd_color((i==act)?"result":"answer");
      if (rFPPEN & (1<<i))
	con->dd_cprintf("ui_run", "FPP%d:EN   ", i);
      else
	con->dd_cprintf("ui_stop", "FPP%d:DIS  ", i);
    }
  con->dd_printf("\n");
  for (i= 0; i<nuof_fpp; i++)
    {
      con->dd_color((i==act)?"result":"answer");
      con->dd_printf("A=%02x %3u  ", fpps[i]->rA, fpps[i]->rA);
    }
  con->dd_printf("\n");
  for (i= 0; i<nuof_fpp; i++)
    {
      con->dd_color((i==act)?"result":"answer");
      con->dd_printf("  OACZ    ");
    }
  con->dd_printf("\n");
  for (i= 0; i<nuof_fpp; i++)
    {
      con->dd_color((i==act)?"result":"answer");
      con->dd_printf("F=", fpps[i]->rF);
      con->dd_printf("%d%d%d%d    ",
		     ((fpps[i]->rF&BIT_OV)>>BITPOS_OV),
		     ((fpps[i]->rF&BIT_AC)>>BITPOS_AC),
		     ((fpps[i]->rF&BIT_C )>>BITPOS_C ),
		     ((fpps[i]->rF&BIT_Z )>>BITPOS_Z ));
    }
  con->dd_printf("\n");
  for (i= 0; i<nuof_fpp; i++)
    {
      con->dd_color((i==act)?"result":"answer");
      con->dd_printf("SP=%02x     ", fpps[i]->rSP);
    }
  con->dd_printf("\n");

  for (i=0; i<nuof_fpp; i++)
    {
      con->dd_color((i==act)?"result":"answer");
      con->dd_printf("FPP%d: ", i);
      fpps[0]->print_disass(fpps[i]->PC, con);
    }
}


/* End of pdk.src/pdk.cc */
