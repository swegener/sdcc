/*
 * Simulator of microcontrollers (stm8.cc)
 *
 * some stm8 code base from Karl Bongers karl@turbobit.com
 * and Valentin Dudouyt valentin.dudouyt@gmail.com
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

//#include <stdarg.h> /* for va_list */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// prj
//#include "pobjcl.h"
#include "globals.h"

// sim
//#include "simcl.h"
#include "dregcl.h"

// local
#include "stm8cl.h"
#include "glob.h"
//#include "regsstm8.h"
#include "stm8mac.h"
//#include "itccl.h"
#include "serialcl.h"
#include "rstcl.h"
#include "timercl.h"
#include "portcl.h"
#include "clkcl.h"
#include "uidcl.h"
#include "bl.h"
#include "flashcl.h"

/*******************************************************************/

static class cl_stm8_error_registry stm8_error_registry;


/*
 * Base type of STM8 controllers
 */

cl_stm8::cl_stm8(struct cpu_entry *IType, class cl_sim *asim):
  cl_uc(asim)
{
  type= IType;
  flash_ctrl= NULL;
}

int
cl_stm8::init(void)
{
  cl_uc::init(); /* Memories now exist */
  sp_limit= 0x1500;

  pipetrace_file = NULL;
  pipetrace_fd = NULL;
  pipetrace_fold = true;

  //rom = address_space(MEM_ROM_ID);
  //ram = mem(MEM_XRAM);
  //ram = rom;

  //printf("******************** leave the RAM dirty now \n");
  // zero out ram(this is assumed in regression tests)
  //for (int i=0x0; i<0x8000; i++) {
  //  ram->set((t_addr) i, 0);
  //}

  trap_src= new cl_it_src(this, -1,
			  (cl_memory_cell*)NULL, (t_mem)0,
			  (cl_memory_cell*)NULL, (t_mem)0,
			  (t_addr)0x8004,
			  false, false,
			  "trap", 0);
  trap_src->init();
  return(0);
}


void
cl_stm8::reset(void)
{
  cl_uc::reset();

  regs.SP = 0x17ff;
  regs.A = 0;
  regs.X = 0;
  regs.Y = 0;
  regs.CC = 0x28;
  //regs.VECTOR = 1;
  PC= 0x8000;

  div_cycle = 0;

  pipeline_flush(false);
}


const char *
cl_stm8::id_string(void)
{
  switch (type->type)
    {
    case CPU_STM8S:
      return("STM8 S,AF");
    case CPU_STM8L:
      return("STM8 AL,L");
    case CPU_STM8L101:
      return("STM8 L101");
    default:
      return("STM8");
    }
}


/*
 * Making elements of the controller
 */
/*
t_addr
cl_stm8::get_mem_size(enum mem_class type)
{
  switch(type)
    {
    case MEM_ROM: return(0x10000);
    case MEM_XRAM: return(0x10000);
    default: return(0);
    }
 return(cl_uc::get_mem_size(type));
}
*/

 /*
   L15x46 uid: 0x4926 00 5b 00 16 11 47 30 31 38 35 35 36
 */

static class cl_port_ui *d= NULL;
static int puix= 1;
static int puiy= 5;
static int puik= 0;
static int puis= 1;
static const char *puiks= keysets[puik];
static class cl_port_data pd;

void
cl_stm8::mk_port(int portnr, chars n)
{
  class cl_port *p;
  add_hw(p= new cl_port(this, portnr, n));
  p->init();

  pd.set_name(n);
  pd.cell_p  = p->cell_odr;
  pd.cell_in = p->cell_idr;
  pd.cell_dir= p->cell_ddr;
  pd.keyset  = puiks;
  pd.basx    = puix;
  pd.basy    = puiy;
  d->add_port(&pd, puis++);
  
  if ((puix+= 20) > 80)
    {
      puix= 1;
      if ((puiy+= 7) > 20)
        {}
    }
  if ((puik+= 1) > 7)
    puiks= NULL;
  else
    puiks= keysets[puik];
}

void
cl_stm8::make_cpu_hw(void)
{
  add_hw(cpu= new cl_stm8_cpu(this));
  cpu->init();
}

void
cl_stm8::mk_hw_elements(void)
{
  class cl_hw *h;
  class cl_it_src *is;
  cl_uc::mk_hw_elements();
  class cl_option *o;

  if ((o= application->options->get_option("serial1_in_file")) == NULL)
    {
      o= new cl_string_option(this, "serial1_in_file",
			      "Input file for serial line uart1 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  if ((o= application->options->get_option("serial1_out_file")) == NULL)
    {
      o= new cl_string_option(this, "serial1_out_file",
			      "Output file for serial line uart1 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  
  if ((o= application->options->get_option("serial2_in_file")) == NULL)
    {
      o= new cl_string_option(this, "serial2_in_file",
			      "Input file for serial line uart2 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  if ((o= application->options->get_option("serial2_out_file")) == NULL)
    {
      o= new cl_string_option(this, "serial2_out_file",
			      "Output file for serial line uart2 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  
  if ((o= application->options->get_option("serial3_in_file")) == NULL)
    {
      o= new cl_string_option(this, "serial3_in_file",
			      "Input file for serial line uart3 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  if ((o= application->options->get_option("serial3_out_file")) == NULL)
    {
      o= new cl_string_option(this, "serial3_out_file",
			      "Output file for serial line uart3 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  
  if ((o= application->options->get_option("serial4_in_file")) == NULL)
    {
      o= new cl_string_option(this, "serial4_in_file",
			      "Input file for serial line uart4 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  if ((o= application->options->get_option("serial4_out_file")) == NULL)
    {
      o= new cl_string_option(this, "serial4_out_file",
			      "Output file for serial line uart4 (-S)");
      application->options->new_option(o);
      o->init();
      o->hide();
    }
  
  add_hw(h= new cl_dreg(this, 0, "dreg"));
  h->init();

  add_hw(d= new cl_port_ui(this, 0, "dport"));
  d->init();
  pd.init();
  
  if (type->type == CPU_STM8S)
    {
      add_hw(clk= new cl_clk_saf(this));
      clk->init();
      if (type->subtype & (DEV_STM8S003|
			   DEV_STM8S007|
			   DEV_STM8S103|
			   DEV_STM8S207|
			   DEV_STM8S208|
			   DEV_STM8S903|
			   DEV_STM8AF52))
	{
	  add_hw(h= new cl_serial(this, 0x5230, 1, 17, 18));
	  h->init();
	}
      if (type->subtype & (DEV_STM8S005|
			   DEV_STM8S105|
			   DEV_STM8AF62_46))
	{
	  add_hw(h= new cl_serial(this, 0x5240, 2, 20, 21));
	  h->init();
	}
      if (type->subtype & (DEV_STM8S007|
			   DEV_STM8S207|
			   DEV_STM8S208|
			   DEV_STM8AF52))
	{
	  add_hw(h= new cl_serial(this, 0x5240, 3, 20, 21));
	  h->init();
	}
      if (type->subtype & (DEV_STM8AF62_12))
	{
	  add_hw(h= new cl_serial(this, 0x5230, 4, 17, 18));
	  h->init();
	}
    }
  if (type->type == CPU_STM8L)
    {
      add_hw(clk= new cl_clk_all(this));
      clk->init();
      add_hw(h= new cl_serial(this, 0x5230, 1, 27, 28));
      h->init();
      if (type->subtype & (DEV_STM8AL3xE|
			   DEV_STM8AL3x8|
			   DEV_STM8L052R|
			   DEV_STM8L15x8|
			   DEV_STM8L162))
	{
	  add_hw(h= new cl_serial(this, 0x53e0, 2, 19, 20));
	  h->init();
	}
      if (type->subtype & (DEV_STM8AL3xE|
			   DEV_STM8AL3x8|
			   DEV_STM8L052R|
			   DEV_STM8L15x8|
			   DEV_STM8L162))
	{
	  add_hw(h= new cl_serial(this, 0x53f0, 3, 21, 22));
	  h->init();
	}
    }
  if (type->type == CPU_STM8L101)
    {
      add_hw(clk= new cl_clk_l101(this));
      clk->init();
      add_hw(h= new cl_serial(this, 0x5230, 1, 27, 28));
      h->init();
    }

  int n_ports = 4;

  if (type->type == CPU_STM8S)
    {
      switch (type->subtype)
        {
          case DEV_STM8AF62_46:
            n_ports += 3;
            break;

          case DEV_STM8S005:
          case DEV_STM8S007:
          case DEV_STM8S105:
          case DEV_STM8S207:
          case DEV_STM8S208:
          case DEV_STM8AF52:
            n_ports += 1;
            break;
        }
    }
  else if (type->type == CPU_STM8L)
    {
      if (type->subtype != DEV_STM8L051)
        n_ports += 2;

      if (type->subtype & (DEV_STM8AL3xE|
                           DEV_STM8AL3x8|
                           DEV_STM8L052R|
                           DEV_STM8L15x8|
                           DEV_STM8L162))
        {
          n_ports += 1;

          if (type->subtype != DEV_STM8L052R)
            n_ports += 2;
        }
    }

  ports= new cl_address_space("ports", 0, n_ports, 8);
  ports->init();
  address_spaces->add(ports);

  ports_chip= new cl_chip8("ports_chip", n_ports, 8);
  ports_chip->init();
  memchips->add(ports_chip);

  class cl_address_decoder *ad= new cl_address_decoder(ports, ports_chip, 0, n_ports - 1, 0);
  ad->init();
  ports->decoders->add(ad);
  ad->activate(0);

  add_hw(itc= new cl_itc(this));
  itc->init();

  mk_port(0, "pa");
  mk_port(1, "pb");
  mk_port(2, "pc");
  mk_port(3, "pd");
  
  if (type->type == CPU_STM8S)
    {
      // all S and AF
      mk_port(4, "pe");
      mk_port(5, "pf");

      char name[] = "EXTI0";
      for (int i= 0; i <= 4; i++, name[4]++)
        {
          it_sources->add(is= new cl_it_src(this, 3 + i,
                    itc->exti_sr1, 1 << i,
                    itc->exti_sr1, 1 << i,
                    0x8014 + i * 4,
                    true, // STM8S has no EXTI_SR[12] so port interrupts autoclear and are not ack'd.
                    false,
                    strdup(name),
                    25*10+i));
          is->init();
        }

      if (type->subtype & (DEV_STM8S005|
			   DEV_STM8S007|
			   DEV_STM8S105|
			   DEV_STM8S207|
			   DEV_STM8S208|
			   DEV_STM8AF52|
			   DEV_STM8AF62_46))
	{
	  mk_port(6, "pg");
	  if (type->subtype != DEV_STM8AF62_46)
	    {
	      mk_port(7, "ph");
	      mk_port(8, "pi");
	    }
	}
      add_hw(h= new cl_rst(this, 0x50b3, 0x1f));
      h->init();
      add_hw(h= new cl_tim1_saf(this, 1, 0x5250));
      h->init();
      // some S, some AF
      if (type->subtype & (DEV_STM8S005|
			   DEV_STM8S007|
			   DEV_STM8S105|
			   DEV_STM8S207|
			   DEV_STM8S208|
			   DEV_STM8AF52|
			   DEV_STM8AF62_46))
	{
	  add_hw(h= new cl_tim2_saf_a(this, 2, 0x5300));
	  h->init();
	  add_hw(h= new cl_tim3_saf(this, 3, 0x5320));
	  h->init();
	  add_hw(h= new cl_tim4_saf_a(this, 4, 0x5340));
	  h->init();
	}
      if (type->subtype & (DEV_STM8S903|
			   DEV_STM8AF62_12))
	{
	  add_hw(h= new cl_tim5_saf(this, 5, 0x5300));
	  h->init();
	  add_hw(h= new cl_tim6_saf(this, 6, 0x5340));
	  h->init();
	}
      if (type->subtype & (DEV_STM8S003|
			   DEV_STM8S103))
	{
	  add_hw(h= new cl_tim2_saf_b(this, 2, 0x5300));
	  h->init();
	  // tim4 B
	  add_hw(h= new cl_tim4_saf_b(this, 4, 0x5340));
	  h->init();
	}
    }
  else
    {
      char name[] = "EXTI0";
      for (int i= 0; i <= 7; i++, name[4]++)
        {
          it_sources->add(is= new cl_it_src(this, 8 + i,
                    itc->exti_sr1, 1 << i,
                    itc->exti_sr1, 1 << i,
                    0x8028 + i * 4,
                    false,
                    false,
                    strdup(name),
                    25*10+i));
          is->init();
        }

      if (type->type == CPU_STM8L)
        {
          if (type->subtype != DEV_STM8L051)
	    {
	      mk_port(0x5014, "pe");
	      mk_port(0x5019, "pf");

              it_sources->add(is= new cl_it_src(this, 5,
                        itc->exti_sr2, (1 << 3) | (1 << 2),
                        itc->exti_sr2, (1 << 3) | (1 << 2),
                        0x801c,
                        false,
                        false,
                        "EXTIE/F/PVD",
                        25*10+5));
              is->init();
	    }

          if (type->subtype & (DEV_STM8AL3xE|
			       DEV_STM8AL3x8|
			       DEV_STM8L052R|
			       DEV_STM8L15x8|
			       DEV_STM8L162))
	    {
	      mk_port(0x501e, "pg");

              it_sources->add(is= new cl_it_src(this, 6,
                        itc->exti_sr2, (1 << 4) | (1 << 0),
                        itc->exti_sr2, (1 << 4) | (1 << 0),
                        0x8020,
                        false,
                        false,
                        "EXTIB/G",
                        25*10+6));
              is->init();

	      if (type->subtype != DEV_STM8L052R)
	        {
	          mk_port(0x5023, "ph");
	          mk_port(0x5028, "pi");

                  it_sources->add(is= new cl_it_src(this, 7,
                            itc->exti_sr2, (1 << 5) | (1 << 1),
                            itc->exti_sr2, (1 << 5) | (1 << 1),
                            0x8024,
                            false,
                            false,
                            "EXTID/H",
                            25*10+7));
                  is->init();
	        }
              else
                {
                  it_sources->add(is= new cl_it_src(this, 7,
                            itc->exti_sr2, (1 << 1),
                            itc->exti_sr2, (1 << 1),
                            0x8024,
                            false,
                            false,
                            "EXTID",
                            25*10+7));
                  is->init();
                }
	    }
          else
            {
              it_sources->add(is= new cl_it_src(this, 6,
                        itc->exti_sr2, (1 << 0),
                        itc->exti_sr2, (1 << 0),
                        0x8020,
                        false,
                        false,
                        "EXTIB",
                        25*10+6));
              is->init();

              it_sources->add(is= new cl_it_src(this, 7,
                        itc->exti_sr2, (1 << 1),
                        itc->exti_sr2, (1 << 1),
                        0x8024,
                        false,
                        false,
                        "EXTID",
                        25*10+7));
              is->init();
            }

          add_hw(h= new cl_rst(this, 0x50b0+1, 0x3f));
          h->init();
          add_hw(h= new cl_tim2_all(this, 2, 0x5250));
          h->init();
          add_hw(h= new cl_tim3_all(this, 3, 0x5280));
          h->init();
          add_hw(h= new cl_tim4_all(this, 4, 0x52E0));
          h->init();
          // all AL
          if (type->subtype & DEV_STM8AL)
	    {
	      add_hw(h= new cl_tim1_all(this, 1, 0x52b0));
	      h->init();
	    }
          // some L
          if (type->subtype & (DEV_STM8L052C |
			       DEV_STM8L052R |
			       DEV_STM8L15x46 |
			       DEV_STM8L15x8 |
			       DEV_STM8L162))
	    {
	      add_hw(h= new cl_tim1_all(this, 1, 0x52b0));
	      h->init();
	    }
          if (type->subtype & (DEV_STM8AL3xE |
			       DEV_STM8AL3x8 |
			       DEV_STM8L052R |
			       DEV_STM8L15x8 |
			       DEV_STM8L162))
	    {
	      add_hw(h= new cl_tim5_all(this, 5, 0x5300));
	      h->init();
	    }
        }
      else if (type->type == CPU_STM8L101)
        {
          add_hw(h= new cl_rst(this, 0x50b0+1, 0x0f));
          h->init();
          add_hw(h= new cl_tim2_l101(this, 2, 0x5250));
          h->init();
          add_hw(h= new cl_tim3_l101(this, 2, 0x5280));
          h->init();
          add_hw(h= new cl_tim4_l101(this, 4, 0x52E0));
          h->init();

          it_sources->add(is= new cl_it_src(this, 6,
                    itc->exti_sr2, (1 << 0),
                    itc->exti_sr2, (1 << 0),
                    0x8020,
                    false,
                    false,
                    "EXTIB",
                    25*10+6));
          is->init();
          it_sources->add(is= new cl_it_src(this, 7,
                    itc->exti_sr2, (1 << 1),
                    itc->exti_sr2, (1 << 1),
                    0x8024,
                    false,
                    false,
                    "EXTID",
                    25*10+7));
          is->init();
        }
    }

  // UID
  if (type->subtype & (DEV_STM8S103 |
		       DEV_STM8S903 |
		       DEV_STM8AF62_12))
    {
      add_hw(h= new cl_uid(this, 0x4865));
      h->init();
    }
  else if (type->subtype & (DEV_STM8AL |
			    DEV_STM8L151x23 |
			    DEV_STM8L15x46 |
			    DEV_STM8L15x8 |
			    DEV_STM8L162))
    {
      add_hw(h= new cl_uid(this, 0x4926));
      h->init();
    }
  else if (type->subtype & (DEV_STM8L101))
    {
      add_hw(h= new cl_uid(this, 0x4925));
      h->init();
    }

  // FLASH
  if (type->subtype & (DEV_STM8SAF))
    {
      add_hw(flash_ctrl= new cl_saf_flash(this, 0x505a));
      flash_ctrl->init();
    }
  else if (type->subtype & (DEV_STM8ALL |
			    DEV_STM8L101))
    {
      add_hw(flash_ctrl= new cl_l_flash(this, 0x5050));
      flash_ctrl->init();
    }
  //add_hw(h= new cl_tim235(this, 3, 0x5320));
  //h->init();
  //add_hw(h= new cl_tim46(this, 4, 0x5340));
  //h->init();
}

class cl_memory_chip *c;

void
cl_stm8::make_memories(void)
{
  class cl_address_space *as;

  rom= ram= as= new cl_flash_as/*address_space*/("rom", 0, 0x28000/*, 8*/);
  as->init();
  address_spaces->add(as);

  class cl_address_decoder *ad;
  class cl_memory_chip /* *chip,*/ *rom_chip;

  c= rom_chip= NULL;/*new cl_memory_chip("rom_chip", 0x20000, 8, 0);
  rom_chip->init();
  memchips->add(rom_chip);*/

  ram_chip= new cl_chip8("ram_chip", 0x1800, 8);
  ram_chip->init();
  memchips->add(ram_chip);
  eeprom_chip= new cl_chip8("eeprom_chip", 0x0800, 8, 0);
  eeprom_chip->init();
  memchips->add(eeprom_chip);
  option_chip= new cl_chip8("option_chip", 0x0800, 8, 0);
  option_chip->init();
  memchips->add(option_chip);
  io_chip= new cl_chip8("io_chip", 0x0800, 8);
  io_chip->init();
  memchips->add(io_chip);
  if (type->subtype & DEV_STM8S105)
    boot_chip= new cl_chip8("boot_chip_s105",
			    bl_s105_length, 8, bl_s105, bl_s105_length);
  else if (type->subtype & DEV_STM8L15x46)
    boot_chip= new cl_chip8("boot_chip_l15x46",
			    bl_l15x46_length, 8, bl_l15x46, bl_l15x46_length);
  /*else if (type->subtype & DEV_STM8L101)
    boot_chip= new cl_memory_chip("boot_chip_l101", bl_l15x46_length, 8, bl_l15x46);*/
  else
    boot_chip= new cl_chip8("boot_chip", 0x0800, 8);
  boot_chip->init();
  memchips->add(boot_chip);
  cpu_chip= new cl_chip8("cpu_chip", 0x0100, 8);
  cpu_chip->init();
  memchips->add(cpu_chip);
  flash_chip= new cl_chip8("flash_chip", 0x20000, 8, 0);
  flash_chip->init();
  memchips->add(flash_chip);
  /*
  ad= new cl_address_decoder(as= address_space("rom"), rom_chip, 0, 0x1ffff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
  */
  ad= new cl_address_decoder(as= rom, ram_chip, 0, 0x17ff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  ad= new cl_address_decoder(as= rom, eeprom_chip, 0x4000, 0x47ff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  ad= new cl_address_decoder(as= rom, option_chip, 0x4800, 0x4fff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  ad= new cl_address_decoder(as= rom, io_chip, 0x5000, 0x57ff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  ad= new cl_address_decoder(as= rom, boot_chip, 0x6000, 0x67ff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
  rom->set_cell_flag(0x6000, 0x67ff, true, CELL_READ_ONLY);

  ad= new cl_address_decoder(as= rom, cpu_chip, 0x7f00, 0x7fff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  ad= new cl_address_decoder(as= rom, flash_chip, 0x8000, 0x27fff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);
  class cl_option *o= application->options->get_option("writable_flash");
  bool wv= false;
  if (o)
    o->get_value(&wv);
  if (!wv)
    rom->set_cell_flag(0x8000, 0x27fff, true, CELL_READ_ONLY);
  
  regs8= new cl_address_space("regs8", 0, 2, 8);
  regs8->init();
  regs8->get_cell(0)->decode((t_mem*)&regs.A);
  regs8->get_cell(1)->decode((t_mem*)&regs.CC);

  regs16= new cl_address_space("regs16", 0, 3, 16);
  regs16->init();

  regs16->get_cell(0)->decode((t_mem*)&regs.X);
  regs16->get_cell(1)->decode((t_mem*)&regs.Y);
  regs16->get_cell(2)->decode((t_mem*)&regs.SP);

  address_spaces->add(regs8);
  address_spaces->add(regs16);

  vars->add("A",     regs8, 0, 7, 0, "Accumulator");
  vars->add("CC",    regs8, 1, -1, -1, "Condition Code");
  vars->add("CC_C",  regs8, 1, BITPOS_C,  BITPOS_C,  "Carry");
  vars->add("CC_Z",  regs8, 1, BITPOS_Z,  BITPOS_Z,  "Zero");
  vars->add("CC_N",  regs8, 1, BITPOS_N,  BITPOS_N,  "Negative");
  vars->add("CC_I0", regs8, 1, BITPOS_I0, BITPOS_I0, "Interrupt mask level 0");
  vars->add("CC_H",  regs8, 1, BITPOS_H,  BITPOS_H,  "Half carry");
  vars->add("CC_I1", regs8, 1, BITPOS_I1, BITPOS_I1, "Interrupt mask level 1");
  vars->add("CC_V",  regs8, 1, BITPOS_V,  BITPOS_V,  "Overflow");
  
  vars->add("X",  regs16, 0, 15, 0, "X index register");
  vars->add("Y",  regs16, 1, 15, 0, "Y index register");
  vars->add("SP", regs16, 2, 15, 0, "Stack pointer");
}


/*
 * Help command interpreter
 */

struct dis_entry *
cl_stm8::dis_tbl(void)
{
  return(disass_stm8);
}

/*struct name_entry *
cl_stm8::sfr_tbl(void)
{
  return(0);
}*/

/*struct name_entry *
cl_stm8::bit_tbl(void)
{
  //FIXME
  return(0);
}*/

int
cl_stm8::inst_length(t_addr addr)
{
  int len = 0;

  get_disasm_info(addr, &len, NULL, NULL, NULL);

  return len;
}
int
cl_stm8::inst_branch(t_addr addr)
{
  int b;

  get_disasm_info(addr, NULL, &b, NULL, NULL);

  return b;
}

bool
cl_stm8::is_call(t_addr addr)
{
  struct dis_entry *e;

  get_disasm_info(addr, NULL, NULL, NULL, &e);

  return e?(e->is_call):false;
}

int
cl_stm8::longest_inst(void)
{
  return 5;
}



const char *
cl_stm8::get_disasm_info(t_addr addr,
			 int *ret_len,
			 int *ret_branch,
			 int *immed_offset,
			 struct dis_entry **dentry)
{
  const char *b = NULL;
  uint code;
  int len = 0;
  int i;
  int start_addr = addr;
  struct dis_entry *dis_e;

  code= rom->get(addr++);
  dis_e = NULL;

  switch(code) {
	/* here will be all the prefixes for STM8 */
	case 0x72 :
	  code= rom->get(addr++);
      i= 0;
      while ((code & disass_stm8_72[i].mask) != disass_stm8_72[i].code &&
        disass_stm8_72[i].mnemonic)
        i++;
      dis_e = &disass_stm8_72[i];
      b= disass_stm8_72[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8_72[i].length + 1);
    break;

    	case 0x71 :
	  code= rom->get(addr++);
      i= 0;
      while ((code & disass_stm8_71[i].mask) != disass_stm8_71[i].code &&
        disass_stm8_71[i].mnemonic)
        i++;
      dis_e = &disass_stm8_71[i];
      b= disass_stm8_71[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8_71[i].length + 1);
    break;

	case 0x90 :
	  code= rom->get(addr++);
      i= 0;
      while ((code & disass_stm8_90[i].mask) != disass_stm8_90[i].code &&
        disass_stm8_90[i].mnemonic)
        i++;
      dis_e = &disass_stm8_90[i];
      b= disass_stm8_90[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8_90[i].length + 1);
    break;
	  
	case 0x91 :
	  code= rom->get(addr++);
      i= 0;
      while ((code & disass_stm8_91[i].mask) != disass_stm8_91[i].code &&
        disass_stm8_91[i].mnemonic)
        i++;
      dis_e = &disass_stm8_91[i];
      b= disass_stm8_91[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8_91[i].length + 1);
    break;
	  
	case 0x92 :
	  code= rom->get(addr++);
      i= 0;
      while ((code & disass_stm8_92[i].mask) != disass_stm8_92[i].code &&
        disass_stm8_92[i].mnemonic)
        i++;
      dis_e = &disass_stm8_92[i];
      b= disass_stm8_92[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8_92[i].length + 1);
    break;
	  
	
    default:
      i= 0;
      while ((code & disass_stm8[i].mask) != disass_stm8[i].code &&
             disass_stm8[i].mnemonic)
        i++;
      dis_e = &disass_stm8[i];
      b= disass_stm8[i].mnemonic;
      if (b != NULL)
        len += (disass_stm8[i].length);
    break;
  }

  if (ret_branch) {
    *ret_branch = dis_e->branch;
  }

  if (immed_offset)
    *immed_offset = (addr - start_addr);

  if (len == 0)
    len = 1;

  if (ret_len)
    *ret_len = len;

  if (dentry)
    *dentry= dis_e;
  
  return b;
}

void
cl_stm8::analyze_start(void)
{
  // Look for interrupts that _are_ interrupts. Nothing stops you from
  // using the space for other code or data if the interrupts aren't needed.

  // First those for elements that are implemented in the simulator
  for (t_index i = 0; i < it_sources->count; i++)
    {
      class cl_it_src *is= (class cl_it_src *)(it_sources->at(i));

      t_addr addr = is->addr;

      if (rom->get(addr) == 0x82) // int
        {
          addr = (rom->get(is->addr+1)<<16) |
                  (rom->get(is->addr+2)<<8) |
                  (rom->get(is->addr+3));

          t_index var_i;
          if (!vars->by_addr.search(rom, addr, -1, -1, var_i))
            {
              chars label;

              if (rom->get(addr) == 0x80) // jumps straight to iret
                label.format("%s", ".isr_unused");
              else
                label.format(".%s", is->get_name());

              class cl_var *v = new cl_var(label.subst(" ", '_'), rom, addr, chars("Auto-generated by analyze"), -1, -1);
              v->init();
              vars->add(v);
            }

          analyze(is->addr);
        }
    }

  // And now for everything else. One day this will be nothing but reset :-)
  for (t_addr is_addr = 0x8000; is_addr <= 0x807c; is_addr += 4)
    {
      if (!inst_at(is_addr) && rom->get(is_addr) == 0x82) // int
        {
          t_addr addr = (rom->get(is_addr+1)<<16) |
                    (rom->get(is_addr+2)<<8) |
                    (rom->get(is_addr+3));

          t_index var_i;
          if (!vars->by_addr.search(rom, addr, -1, -1, var_i))
            {
              chars label;

              if (is_addr == 0x8000)
                label.format("%s", ".reset");
              else if (rom->get(addr) == 0x80) // jumps straight to iret
                label.format("%s", ".isr_unused");
              else
                label.format("%s%lu", ".interrupt", ((unsigned long)is_addr - 0x8008) / 4);

              class cl_var *v = new cl_var(label, rom, addr, chars("Auto-generated by analyze"), -1, -1);
              v->init();
              vars->add(v);
            }

          analyze(is_addr);
        }
    }
}

void
cl_stm8::analyze(t_addr addr)
{
  const char *mnemonic;
  int length = 0, branch = 0, immed_offset = 0;

  while (!inst_at(addr) && (mnemonic = get_disasm_info(addr, &length, &branch, &immed_offset, NULL)))
    {
      set_inst_at(addr);

      // Returns or indirect jumps end this execution path immediately
      if (branch == 'x')
        break;

      if (branch != ' ')
        {
          t_addr target = 0;
          unsigned int bit= 0;

          // With jumps, branches and calls the target address is always
          // given by the last operand. We just need to find it.
          for (const char *b = mnemonic; *b; b++)
            {
              if (b[0] == '%' && b[1])
                {
                  b++;
                  switch (*b)
                    {
                      case '3': // 3    24bit index offset
                        ++immed_offset;
                       // Fall through
                      case '2': // 2    word index offset
                      case 'w': // w    word immediate operand
                        ++immed_offset;
                        // Fall through
                      case '1': // b    byte index offset
                      case 'b': // b    byte immediate operand
                      case 'd': // d    direct addressing
                      case 's': // s    signed byte immediate
                        ++immed_offset;
                        break;

                      case 'B': // B    bit number
                        bit = (rom->get(addr+1) & 0xf) >> 1;
                        break;

                      case 'e': // e    extended 24bit immediate operand
                        target= ((rom->get(addr+immed_offset)<<16) |
                                  (rom->get(addr+immed_offset+1)<<8) |
                                  (rom->get(addr+immed_offset+2)));
                        ++immed_offset;
                        ++immed_offset;
                        ++immed_offset;
                        break;
                      case 'x': // x    extended addressing
                        target= ((rom->get(addr+immed_offset)<<8) |
                                  (rom->get(addr+immed_offset+1)));
                        ++immed_offset;
                        ++immed_offset;
                        break;
                      case 'p': // p    pc relative
                        {
                          long int base;
                          i8_t offs;
                          base= addr+immed_offset+1;
                          offs= rom->get(addr+immed_offset);
                          target= base+offs;
                          ++immed_offset;
                        }
                        break;
                      default:
                        break;
                    }
                }
            }

          analyze_jump(addr, target, branch, bit);

          // Unconditional jumps end this execution path
          if (branch == 'j')
            break;
        }

      addr= rom->validate_address(addr + length);
    }
}

char *
cl_stm8::disass(t_addr addr)
{
  const class cl_var *context = 0;
  chars work, temp;
  const char *b;
  int len = 0;
  int immed_offset = 0;
  t_addr operand= 0;
  bool first= true;

  work= "";

  b = get_disasm_info(addr, &len, NULL, &immed_offset, NULL);

  if (b == NULL)
    {
      return strdup("UNKNOWN/INVALID");
    }

  int operand_n = 0;
  while (*b)
    {
      if ((*b == ' ') && first)
	{
	  first= false;
	  while (work.len() < 6) work.append(' ');
	}
      if (*b == '%')
        {

          b++;
          switch (*(b++))
            {
            case 's': // s    signed byte immediate
              temp.format("#%d", (i8_t)rom->get(addr+immed_offset));
              ++immed_offset;
              break;
            case 'e': // e    extended 24bit immediate operand
	      operand= ((rom->get(addr+immed_offset)<<16) |
			  (rom->get(addr+immed_offset+1)<<8) |
			  (rom->get(addr+immed_offset+2)));
              temp.format("#0x%06lx", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              ++immed_offset;
              ++immed_offset;
              break;
            case 'w': // w    word immediate operand
	      operand=  ((rom->get(addr+immed_offset)<<8) |
			 (rom->get(addr+immed_offset+1)));
              temp.format("#0x%04x", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              ++immed_offset;
              break;
            case 'b': // b    byte immediate operand
              operand= (uint)rom->get(addr+immed_offset);
              temp.format("#0x%02x", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              break;
            case 'x': // x    extended addressing
              if (rom->get(addr) == 0x35) // mov %x,%b - mov $8000,#$ff is 35 ff 80 00
                operand= (uint)((rom->get(addr+immed_offset+1)<<8) |
                           (rom->get(addr+immed_offset+2)));
              else if (rom->get(addr) == 0x55 && operand_n == 0) // mov %x,%x - mov $8000,$9fff is 9f ff 80 00
                operand= (uint)((rom->get(addr+immed_offset+2)<<8) |
                           (rom->get(addr+immed_offset+3)));
              else
                {
                  operand= (uint)((rom->get(addr+immed_offset)<<8) |
                             (rom->get(addr+immed_offset+1)));
                  ++immed_offset;
                  ++immed_offset;
                }
              temp.format("0x%04x", operand);
              context = addr_name(operand, rom, &temp);
              break;
            case 'd': // d    direct addressing
              if (rom->get(addr) == 0x45 && operand_n == 0) // mov %d,%d - mov $80,$10 is 45 10 80
                  operand= (uint)rom->get(addr+immed_offset+1);
              else
                {
                  operand= (uint)rom->get(addr+immed_offset);
                  ++immed_offset;
                }
              temp.format("0x%02x", operand);
              break;
            case '3': // 3    24bit index offset
              // Assumption: the 24bit offset address is the address of a
              // fixed table and the index register selects an entry.
	      operand= (ulong)((rom->get(addr+immed_offset)<<16) |
			  (rom->get(addr+immed_offset+1)<<8) |
			  (rom->get(addr+immed_offset+2)));
              temp.format("0x%06lx", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              ++immed_offset;
              ++immed_offset;
              break;
            case '2': // 2    word index offset
              // Assumption: the word offset address is the address of a
              // fixed table and the index register selects an entry.
	      operand= (uint)((rom->get(addr+immed_offset)<<8) |
			 (rom->get(addr+immed_offset+1)));
              temp.format("0x%04x", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              ++immed_offset;
              break;
            case '1': // b    byte index offset
              // Assumption: the index register points to a struct/record
              // and the byte offset selects an entry.
              operand= (uint)rom->get(addr+immed_offset);
              temp.format("0x%02x", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              break;
            case 'p': // p    pc relative
              operand = (addr+immed_offset+1 + (i8_t)rom->get(addr+immed_offset)) & 0xffff;
              temp.format("0x%04lx", operand);
              addr_name(operand, rom, &temp);
              ++immed_offset;
              break;
            case 'B': // B    bit number
              {
                uint bit = (rom->get(addr+1) & 0xf) >> 1;
                temp.format("%u", bit);
                // N.B. The address comes before the bit so operand has already
                // been set to the address when we get here.
                addr_name(operand, rom, bit, bit, &temp, context);
              }
              break;
            default:
	      temp= "?";
              break;
            }
	  work+= temp;
        }
      else
        {
          if (*b == ',')
            operand_n++;
          work+= *(b++);
        }
    }

  return strdup(work.c_str());
}


void
cl_stm8::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("V-IHINZC  Flags= 0x%02x %3d %c  ",
                 regs.CC, regs.CC, isprint(regs.CC)?regs.CC:'.');
  con->dd_printf("A= 0x%02x %3d %c\n",
                 regs.A, regs.A, isprint(regs.A)?regs.A:'.');
  con->dd_printf("%c-%c%c%c%c%c%c  ",
                 (regs.CC&BIT_V)?'1':'0',
                 (regs.CC&BIT_I1)?'1':'0',
                 (regs.CC&BIT_H)?'1':'0',
                 (regs.CC&BIT_I0)?'1':'0',
                 (regs.CC&BIT_N)?'1':'0',
                 (regs.CC&BIT_Z)?'1':'0',
                 (regs.CC&BIT_C)?'1':'0');
  con->dd_printf("X= 0x%04x %3d %c    ",
                 regs.X, regs.X, isprint(regs.X)?regs.X:'.');
  con->dd_printf("Y= 0x%04x %3d %c\n",
                 regs.Y, regs.Y, isprint(regs.Y)?regs.Y:'.');
  con->dd_printf("SP= 0x%04x [SP+1]= %02x %3d %c  Limit= 0x%04x\n",
                 regs.SP, ram->get(regs.SP+1), ram->get(regs.SP+1),
                 isprint(ram->get(regs.SP+1))?ram->get(regs.SP+1):'.',
		 AU(sp_limit));

  print_disass(PC, con);
}

/*
 * Execution
 */

// There are three clocks derived from each other. f_OSC is
// the oscillator frequency. This is then scaled down to give
// the master clock, f_MASTER, which is the clock used to drive
// the hardware elements. This, in turn, is further scaled down
// to give the CPU clock, f_CPU, which is used to drive the CPU.
// Only some variants of STM8 support both scaling factors and
// both factors and f_OSC can be changed programmatically.

int
cl_stm8::clock_per_cycle(void)
{
  return clk->clock_per_cycle();
}

int
cl_stm8::tick_hw(int cycles_cpu)
{
  if (state != stPD)
    cl_uc::tick_hw(cycles_cpu * clock_per_cycle());

  return 0;
}

int
cl_stm8::tick(int cycles_cpu)
{
  if (state != stPD)
    {
      for (int i = 0; i < cycles_cpu; i++, pipetrace_ticks++)
        {
          // Refill the pipeline if there is space and we didn't use the memory this tick.
          if (pipeline_bytes >= 9)
              pipetrace_tick(NULL);
          else
            {
              class cl_address_decoder *d = rom->get_decoder_of(PC);
              if (d && !(d->memchip != flash_chip ? pipeline_busy.data : pipeline_busy.program))
                {
                  int step = (d->memchip != flash_chip ? 1 : 4);
                  pipetrace_tick("F");
                  pipeline_bytes += step;
                }
              // If the other bus is not busy this is a genuine stall due to running out of fetch data
              else if (!pipeline_busy.program || !pipeline_busy.data)
                {
                  error(new cl_error_stm8_pipeline_fetch_stall());
                  pipetrace_tick("FS");
                }
              // Otherwise if both buses are busy there was a flush and the stall is expected
              else
                pipetrace_tick(NULL);
            }
        }

      pipeline_busy.program = false;
      pipeline_busy.data = false;
      pipeline_busy.regs = false;
      if (pipetrace_ticks > pipetrace_max_ticks)
        pipetrace_max_ticks = pipetrace_ticks;
    }

  return cl_uc::tick(cycles_cpu);
}

t_mem
cl_stm8::fetch(void)
{
  if (!div_cycle)
    {
      if (pipeline_bytes - (pipeline_index & ~3) < 4)
        {
          // We have to have a full word before we can use any of it.
          while (pipeline_bytes - (pipeline_index & ~3) < 4)
            {
              error(new cl_error_stm8_pipeline_decode_stall());
              pipetrace_type("Sfetch");
              tick(1);
              pipetrace_type("D");
            }
        }

      pipeline_index++;
    }

  return cl_uc::fetch();
}

bool
cl_stm8::fetch(t_mem *code)
{
  return cl_uc::fetch(code);
}

int
cl_stm8::exec_inst(void)
{
  t_mem code;
  unsigned char cprefix; // prefix used for processing in functions
  /*
  if (regs.VECTOR) {
    PC = get1(0x8000);
	if (PC == 0x82) { // this is reserved opcode for vector table
		regs.VECTOR = 0;
		PC = get1(0x8001)*(1<<16);
		PC += get2(0x8002);
		return(resGO);
	} else {
		return( resERROR);
	}
  }
  */

  instPC= PC;

  if (do_brk())
    return(resBREAKPOINT);

  if (!div_cycle)
    {
      pipetrace_instr_end();
      pipetrace_instr_start(instPC);

      // If there was a flush in this cycle nothing happens in the overlapped cycle
      if (pipeline_busy.flush)
        tick(1);

      pipeline_busy.flush = false;
      pipetrace_type("D");
    }

  code = fetch();

  if (!div_cycle)
    pipeline_busy.instr = false;

  switch (code)
    { // get prefix
    case 0x72:
    case 0x90:
    case 0x91:
    case 0x92:
      cprefix = code;
      code = fetch();
      break;
    case 0x82: // INT
      {
	int ce= fetch();
	int ch= fetch();
	int cl= fetch();
	// Documentation is unclear but we treat it as JP and allow
	// the prefetch of the next instruction to occur during the
	// execute tick.
	pipeline_flush(true);
	PC= ce*0x10000 + ch*0x100 + cl;
	return resGO;
      }
    case 0x8b: // BREAK
      {
        store_regs();
        return resSTOP;
      }
    default:
      cprefix = 0x00;
      break;
    }

   // exceptions
   if((cprefix==0x90)&&((code&0xf0)==0x10)) {
      return ( inst_bccmbcpl( code, cprefix));
   }
   if((cprefix==0x72)&&((code&0xf0)==0x10)) {
      return ( inst_bresbset( code, cprefix));
   }
   if((cprefix==0x72)&&((code&0xf0)==0x00)) {
      return ( inst_btjfbtjt( code, cprefix));
   }
   if ((code &0xf0) == 0x20) {
      return ( inst_jr( code, cprefix));
   }
   if (cprefix == 0x72) {
      switch (code) {
      	 // addw
     case 0xa9:
	 case 0xb9:
	 case 0xbb:
	 case 0xf9:
	 case 0xfb:
            return( inst_addw( code, cprefix));
	 // subw
	 case 0xa2:
	 case 0xb2:
	 case 0xb0:
	 case 0xf2:
	 case 0xf0:
            return( inst_addw( code, cprefix));
			//default is processing in the next switch statement
         default:
            break;
      }
   }

   // main switch
   switch (code & 0xf) {
   unsigned int tempi;
   int opaddr;
      case 0x0:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // neg
               return( inst_neg( code, cprefix));
               break;
            case 0x80: // IRET
               pop1( regs.CC);
               pop1( regs.A);
               pop2( regs.X);
               pop2( regs.Y);
               pop1( tempi);
               pop2( PC);
               store_regs();
               PC += (tempi <<16); //Add PCE to PC
	       {
		 class it_level *il= (class it_level *)(it_levels->top());
		 if (il &&
		     il->level >= 0)
		   {
		     il= (class it_level *)(it_levels->pop());
		     delete il;
		   }
	       }
	       div_cycle = 0;
	       pipeline_flush(false);
               return(resGO);
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // SUB
               return( inst_sub( code, cprefix));
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x1:
         switch ( code & 0xf0) {
            case 0x00: // RRWA
               store_regs();
               if (cprefix == 0x00) { // rrwa X,A
                  tempi = regs.X;
                  regs.X >>= 8;
                  regs.X |= (regs.A << 8);
                  regs.A = tempi & 0xff;
                  FLAG_ASSIGN (BIT_N, 0x8000 & regs.X);
                  FLAG_ASSIGN (BIT_Z, regs.X == 0x0000);
               } else if (cprefix == 0x90) { // rrwa Y,A
                  tempi = regs.Y;
                  regs.Y >>= 8;
                  regs.Y |= (regs.A << 8);
                  regs.A = tempi & 0xff;
                  FLAG_ASSIGN (BIT_N, 0x8000 & regs.Y);
                  FLAG_ASSIGN (BIT_Z, regs.Y == 0x0000);
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x30: // exg A,longmem
               opaddr = fetch2();
               tempi = get1(opaddr);
               // FIXME: PM0044 says 3 cycles so the stores are in separate cycles
               // rather than overlapped. But which comes first?
               store1( opaddr, regs.A);
               exec_cycle();
               store_regs();
               regs.A = tempi;
               return(resGO);
            case 0x40: // exg A,XL
               store_regs();
               tempi = regs.X;
               regs.X = (regs.X &0xff00) | regs.A;
               regs.A = tempi & 0xff;
               return(resGO);
            case 0x50: // exgw X,Y
               store_regs();
               tempi = regs.Y;
               regs.Y = regs.X;
               regs.X = tempi;
               return(resGO);
            case 0x60: // exg A,YL
               store_regs();
               tempi = regs.Y;
               regs.Y = (regs.Y &0xff00) | regs.A;
               regs.A = tempi & 0xff;
               return(resGO);
            case 0x70: // special opcodes
               code = fetch();
               switch(code) {
                  case 0xEC: exec_cycle(); return(resHALT);
                  case 0xED: exec_cycle(); putchar(regs.A); fflush(stdout); return(resGO);
                  default:
		    //printf("************* bad code !!!!\n");
                     return(resINV_INST);
               }
            case 0x80: // ret
               pop2( PC);
               // PM0044: 5.4.3: For the CALL instruction, it [the fetch of the next
               // instruction] starts after the last cycle of the CALL execution.
               // Although nothing is mentioned for RET PM0044 says it takes 4 cycles
               // which implies the fetch follows the last execute cycle.
               exec_cycle();
               pipeline_flush(false);
               return(resGO);
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // CP
               return( inst_cp( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x2:
         switch ( code & 0xf0) {
            case 0x00: // RLWA
               if (cprefix == 0x00) { // rlwa X,A
                  store_regs();
                  tempi = regs.X;
                  regs.X <<= 8;
                  regs.X |= regs.A ;
                  regs.A = tempi >> 8;
                  FLAG_ASSIGN (BIT_N, 0x8000 & regs.X);
                  FLAG_ASSIGN (BIT_Z, regs.X == 0x0000);
               } else if (cprefix == 0x90) { // rlwa Y,A
                  store_regs();
                  tempi = regs.Y;
                  regs.Y <<= 8;
                  regs.Y |= regs.A ;
                  regs.A = tempi >> 8;
                  FLAG_ASSIGN (BIT_N, 0x8000 & regs.Y);
                  FLAG_ASSIGN (BIT_Z, regs.Y == 0x0000);
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x30: // POP longmem
               opaddr = fetch2();
               pop1(tempi);
               store1(opaddr, tempi);
               return(resGO);
            case 0x40: // mul
               // mul low, [store low, mul high], add carry, store high
               // with the store of the low byte result and the multiply of the
               // high byte overlapped.
               exec_cycle();
               exec_cycle();
               exec_cycle();
               store_regs();
               if(cprefix==0x90) {
                  regs.Y = (regs.Y&0xff) * regs.A;
               } else if(cprefix==0x00) {
                  regs.X = (regs.X&0xff) * regs.A;
               } else {
                  return(resHALT);
               }
               FLAG_CLEAR(BIT_H);
               FLAG_CLEAR(BIT_C);
               return(resGO);
               break;
            case 0x50: // sub sp,#val
               {
                 t_mem v = fetch();
                 store_regs();
                 regs.SP -= v;
                 return(resGO);
               }
               break;            
            case 0x60: //div
               return(inst_div(code, cprefix));
               break;
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // SBC
               return( inst_sbc( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x3:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // CPL, CPLW
               return( inst_cpl( code, cprefix));
               break;
            case 0x80: // TRAP
	       {
		 class it_level *il= new it_level(3, 0x8004, PC, trap_src);
		 accept_it(il);
	       }
               return(/*resHALT*/resGO);
            case 0x90: // EXGW
               get_regs();
               store_regs();
               if(cprefix==0x90) {
                  regs.Y = regs.X;
               } else if(cprefix==0x00) {
                  regs.X = regs.Y;
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // CPW
               return( inst_cpw( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x4:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // SRL
               return( inst_srl( code, cprefix));
               break;
            case 0x80: // POP A
	       get_regs();
               pop1(regs.A);
               return(resGO);
            case 0x90: // LDW SP,[XY]
               store_regs();
               if(cprefix==0x90) {
                  regs.SP = regs.Y;
               } else if(cprefix==0x00) {
                  regs.SP = regs.X;
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // AND
               return( inst_and( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x5:
         switch ( code & 0xf0) {
            case 0x30: // MOV
               tempi = fetch1();
               opaddr = fetch2();
               store1(opaddr, tempi);
               return( resGO);
               break;
            case 0x40:
               tempi = get1(fetch1());
               opaddr = fetch1();
               store1(opaddr, tempi);
               return( resGO);
               break;
            case 0x50:
               tempi = get1(fetch2());
               opaddr = fetch2();
               store1(opaddr, tempi);
               return( resGO);
               break;
            case 0x60: // DIVW
               return( inst_div( code, cprefix));
               break;
            case 0x80:
               if(cprefix==0x90) {
                  pop2(regs.Y);
               } else if(cprefix==0x00) {
                  pop2(regs.X);
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x90: // LD [XY]H,A
               store_regs();
               if(cprefix==0x90) {
                  regs.Y = (regs.Y & 0xff) | (regs.A<<8);
               } else if(cprefix==0x00) {
                  regs.X = (regs.X & 0xff) | (regs.A<<8);
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // BCP
               return( inst_bcp( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x6:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // RRC
               return( inst_rrc( code, cprefix));
               break;
            case 0x10:
               return(inst_ldxy( code, cprefix));
               break;
            case 0x80: 
               pop1( regs.CC);
               return(resGO);
            case 0x90: // LDW [XY],SP
               store_regs();
               if(cprefix==0x90) {
                  regs.Y = regs.SP;
               } else if(cprefix==0x00) {
                  regs.X = regs.SP;
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // LD A,...
               return( inst_lda( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x7:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // SRA
               return( inst_sra( code, cprefix));
               break;
            case 0x10:
               opaddr = fetch1()+regs.SP;
               store2(opaddr, regs.Y);
               FLAG_ASSIGN (BIT_Z, (regs.Y & 0xffff) == 0x0000);
               FLAG_ASSIGN (BIT_N, regs.Y & 0x8000);
               return(resGO);
               break;
            case 0x80: // RETF
               pop1( tempi);
               pop2( PC);
               PC += (tempi <<16); //Add PCE to PC
               // PM0044: 5.4.3: For the CALL instruction, it [the fetch of the next
               // instruction] starts after the last cycle of the CALL execution.
               // Although nothing is mentioned for RETF PM0044 says it takes 5 cycles
               // which implies the fetch follows the last execute cycle.
               exec_cycle();
               pipeline_flush(false);
               return(resGO);
            case 0x90: // LD[XY]L,A
	       store_regs();
               if(cprefix==0x90) {
                  regs.Y = (regs.Y & 0xff00) | regs.A;
               } else if(cprefix==0x00) {
                  regs.X = (regs.X & 0xff00) | regs.A;
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0xA0:
               opaddr = fetch2();
               if (cprefix == 0x92) {
                  opaddr = get3(opaddr);
                  store1(opaddr + regs.X,regs.A);
               } else if(cprefix==0x91) {
                  opaddr = get3(opaddr);
                  store1(opaddr + regs.Y,regs.A);
               } else if(cprefix==0x90) {
                  opaddr = (opaddr << 8) + fetch();
                  store1(opaddr + regs.Y, regs.A);
               } else if(cprefix==0x00) {
                  opaddr = (opaddr << 8) + fetch();
                  store1(opaddr + regs.X, regs.A);
               } else {
                  return(resHALT);
               }
               FLAG_NZ (regs.A);
               return(resGO);
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // LD dst,A
               return( inst_lddst( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x8:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // SLL
               return( inst_sll( code, cprefix));
               break;
            case 0x80: 
               push1( regs.A);
               return(resGO);
            case 0x90: // RCF
               exec_cycle();
               FLAG_CLEAR(BIT_C);
               return(resGO);
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // XOR
               return( inst_xor( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0x9:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // RLC
               return( inst_rlc( code, cprefix));
               break;
            case 0x80: // PUSHW
               if(cprefix==0x90) {
                  push2(regs.Y);
               } else if(cprefix==0x00) {
                  push2(regs.X);
               } else {
                  return(resHALT);
               }
               return(resGO);
               break;
            case 0x90: // SCF
               exec_cycle();
               FLAG_SET(BIT_C);
               return(resGO);
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // ADC
               return( inst_adc( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xa:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // DEC
               return( inst_dec( code, cprefix));
               break;
            case 0x80: 
               push1( regs.CC);
               return(resGO);
            case 0x90: // RIM
               exec_cycle();
               FLAG_CLEAR(BIT_I0);
               FLAG_SET(BIT_I1);
               return(resGO);
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // OR
               return( inst_or( code, cprefix));
               break;
             default:
               //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xb:
         switch ( code & 0xf0) {
            case 0x30: // push longmem
	      {
		t_addr a= fetch2();
		t_mem v= get1(a);
		push1( v /*get1(fetch2())*/);
		return(resGO);
	      }
            case 0x40: // push #byte
	      {
		t_mem v= fetch1();
		push1(v);
		return(resGO);
	      }
            case 0x50: // addw sp,#val
               tempi = fetch1();
               exec_cycle();
               store_regs();
               regs.SP += tempi;
               return(resGO);
               break;
            case 0x60: // ld (shortoff,SP),A
               tempi = fetch1();
               store1(tempi + regs.SP, regs.A);
               FLAG_NZ(regs.A);
               return(resGO);
               break;
            case 0x70: // ld A,(shortoff,SP)
               tempi = get1(fetch1()+regs.SP);
               store_regs();
               regs.A = tempi;
               FLAG_NZ(regs.A);
               return(resGO);
               break;
            case 0x80: // BREAK
               exec_cycle();
               return(resSTOP);
            case 0x90: // SIM - disable INT
               exec_cycle();
               FLAG_SET(BIT_I0);
               FLAG_SET(BIT_I1);
               return(resGO);
            case 0x10:
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // ADD
               return( inst_add( code, cprefix));
               break;
            default:
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xc:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // INC
               return( inst_inc( code, cprefix));
               break;
            case 0x10: // ADDW X,#word
               return( inst_addw( code, cprefix));
               break;
            case 0x80: // CCF
               exec_cycle();
               regs.CC ^= BIT_C;
               return(resGO);
               break;            
            case 0x90: // RVF
               exec_cycle();
               FLAG_CLEAR(BIT_V);
               return(resGO);
            case 0xA0: // JPF
               opaddr = fetch2();
               // PM0044: 5.4.3: For a JP instruction, the fetch [of the next
               // instruction] can start during the first cycle of the "dummy"
               // execution.
               if (cprefix == 0x92) {
                  PC = get3(opaddr);
               } else {
                  PC = (opaddr << 8) + fetch();
               }
               pipeline_flush(false);
               return(resGO);
               break;
            case 0xb0: // LDF A,$eehhll or LDF A,[$eehhll.e]
               opaddr = fetch2();
               if (cprefix == 0x92) {
                  tempi = get1(get3(opaddr));
                  decode_cycle();
               } else {
                  tempi = get1((opaddr << 8) + fetch());
               }
               store_regs();
               regs.A = tempi;
               FLAG_NZ (regs.A);
               return(resGO);
               break;
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // JP
               return( inst_jp( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xd:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // tnz
               return( inst_tnz( code, cprefix));
               break;
            case 0x10: // SUBW X,#word
               return( inst_addw( code, cprefix));
               break;
            case 0x80: // CALLF
               opaddr = fetch2();
               if (cprefix == 0x92) {
                   push2(PC & 0xffff);
                   push1(PC >> 16);
                   PC = get3(opaddr);
               } else {
                   unsigned char c = fetch();
                   push2(PC & 0xffff);
                   push1(PC >> 16);
                   PC = (opaddr << 8) + c;
               }
               // PM0044: 5.4.3: For the CALL instruction, it [the fetch of the next
               // instruction] starts after the last cycle of the CALL execution.
               exec_cycle();
               pipeline_flush(false);
               return(resGO);
               break;
            case 0x90: // NOP
               exec_cycle();
               return(resGO);
               break;
            case 0xA0: // CALLR
             {
               signed char c = (signed char) fetch1();
               push2(PC);
               // PM0044: 5.4.3: For the CALL instruction, it [the fetch of the next
               // instruction] starts after the last cycle of the CALL execution.
               PC += c;
               exec_cycle();
               pipeline_flush(false);
               return(resGO);
             }
               break;            
            case 0xb0: // LDF
               opaddr = fetch2();
               if (cprefix == 0x92) {
                  tempi = get3(opaddr);
                  store1(tempi, regs.A);
               } else {
                  tempi = (opaddr << 8) + fetch();
                  store1(tempi, regs.A);
               }
               FLAG_NZ (regs.A);
               return(resGO);
               break;
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // CALL
               return( inst_call( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xe:
         switch ( code & 0xf0) {
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // swap
               return( inst_swap( code, cprefix));
               break;
            case 0x80: 
	      //printf("************* HALT instruction reached !!!!\n");
               exec_cycle();
               return(resHALT);
            case 0x90: // LD A, YH / XH
               if(cprefix==0x90) {
                  tempi = (regs.Y >> 8) & 0xff;
               } else if(cprefix==0x00) {
                  tempi = (regs.X >> 8) & 0xff;
               } else {
                  return(resHALT);
               }
               store_regs();
               regs.A = tempi;
               return(resGO);
               break;
            case 0x10: 
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // LDXY
               return( inst_ldxy( code, cprefix));
               break;
            default:
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      case 0xf:
         switch ( code & 0xf0) {
            case 0x10:
               // ldw   (offset,SP),X
               return( inst_ldxydst( code, cprefix ) );
               break;
            case 0x00: 
            case 0x30:
            case 0x40:
            case 0x50:
            case 0x60:
            case 0x70: // CLR
               return( inst_clr( code, cprefix));
               break;
            case 0x80: 
	      //printf("************* WFI/WFE instruction not implemented !!!!\n");
               return(resINV_INST);
            case 0x90: // LD A,[XY]L
               if(cprefix==0x90) {
                  tempi = (regs.Y & 0xff);
               } else if(cprefix==0x00) {
                  tempi = (regs.X & 0xff);
               } else {
                  return(resHALT);
               }
               store_regs();
               regs.A = tempi;
               return(resGO);
            case 0xA0: // LDF
               opaddr = fetch2();
               if (cprefix == 0x92) {
                  regs.A = get1(get3(opaddr)+regs.X);
                  decode_cycle();
               } else if(cprefix==0x91) {
                  regs.A = get1(get3(opaddr)+regs.Y);
                  decode_cycle();
               } else if(cprefix==0x90) {
                  regs.A = get1((opaddr << 8) + fetch() + regs.Y);
               } else if(cprefix==0x00) {
                  regs.A = get1((opaddr << 8) + fetch() + regs.X);
               } else {
                  return(resHALT);
               }
               store_regs();
               FLAG_NZ (regs.A);
               return(resGO);
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
            case 0xF0: // LDXYDST
               return( inst_ldxydst( code, cprefix));
               break;
            default: 
	      //printf("************* bad code !!!!\n");
               return(resINV_INST);
         }
         break;
      default:
	//printf("************* bad code !!!!\n");
         return(resINV_INST);
      
   }

   //printf("************* bad code !!!!\n");
  /*if (PC)
    PC--;
  else
  PC= get_mem_size(MEM_ROM_ID)-1;*/
  //PC= rom->inc_address(PC, -1);

  //sim->stop(resINV_INST);
  return(resINV_INST);
}


/*
 * Checking for interrupt requests and accept one if needed
 */

int
cl_stm8::do_interrupt(void)
{
  return cl_uc::do_interrupt();
}

int
cl_stm8::priority_of(uchar nuof_it)
{
  t_addr ra= 0x7f70;
  int idx= nuof_it / 4;
  u8_t i1_mask, i0_mask, i0, i1;
  cl_memory_cell *c;
  t_mem cv;
  int levels[4]= { 2, 1, 0, 3 };

  if (nuof_it > 31)
    return 0;
  i1_mask= 2 << ((nuof_it % 4) * 2);
  i0_mask= 1 << ((nuof_it % 4) * 2);
  c= ram->get_cell(ra + idx);
  cv= c->read();
  i0= (cv & i0_mask)?1:0;
  i1= (cv & i1_mask)?2:0;
  return levels[i1+i0];
}

int
cl_stm8::priority_main(void)
{
  t_mem cv= regs.CC;
  u8_t i1, i0;
  int levels[4]= { 2, 1, 0, 3 };
  i0= (cv & BIT_I0)?1:0;
  i1= (cv & BIT_I1)?2:0;
  return levels[i1+i0];
}


/*
 * Accept an interrupt
 */

int
cl_stm8::accept_it(class it_level *il)
{
  //class cl_it_src *is= il->source;
  push2( PC & 0xffff);
  push1( PC >> 16); //extended PC
  push2( regs.Y);
  push2( regs.X);
  push1( regs.A);
  push1( regs.CC);
  // set I1 and I0 status bits
  if (il->level == 0)
    { FLAG_SET(BIT_I1) FLAG_CLEAR(BIT_I0) }
  else if (il->level == 1)
    { FLAG_CLEAR(BIT_I1) FLAG_SET(BIT_I0) }
  else if (il->level == 2)
    { FLAG_CLEAR(BIT_I1) FLAG_CLEAR(BIT_I0) }
  else // 3
    { FLAG_SET(BIT_I1) FLAG_SET(BIT_I0) }

  PC = il->addr;
  pipeline_flush(false);

  it_levels->push(il);
  return resGO;//resINTERRUPT;
}


/* check if interrupts are enabled (globally)
 */

bool
cl_stm8::it_enabled(void)
{
  return !(regs.CC & BIT_I0) || !(regs.CC & BIT_I1);
}

void
cl_stm8::stack_check_overflow(class cl_stack_op *op)
{
  if (op)
    {
      if (op->get_op() & stack_write_operation)
	{
	  t_addr a= op->get_after();
	  if (a < sp_limit)
	    {
	      class cl_error_stack_overflow *e=
		new cl_error_stack_overflow(op);
	      e->init();
	      error(e);
	    }
	}
    }
}

void
cl_stm8::pipetrace_end_table(void)
{
  fputs("\" /></tr>\n"
        "    </tbody>\n"
        "    <thead>\n"
        "        <tr><th>Address</th><th>Instruction</th>",
        pipetrace_fd);

  for (unsigned int i = 1; i <= pipetrace_max_ticks + 1; i++)
    fprintf(pipetrace_fd, "<th class=\"n\">%u</th>", i);

  fputs("\n"
        "    </thead>\n"
        "</table>\n"
        "</a>\n\n",
        pipetrace_fd);

  pipetrace_in_table = false;
}

void
cl_stm8::pipetrace_instr_start(t_addr addr)
{
  if (pipetrace_running)
    {
      if (!pipetrace_in_table)
        {
          fputs("<a title=\"Click for legend\">\n"
                "<table class=\"pipetrace\" onclick=\"toggle('pipetrace_legend')\">\n"
                "    <tbody>\n",
                pipetrace_fd);
          pipetrace_in_table = true;
          pipetrace_max_ticks = 0;
        }

      if (pipeline_busy.flush)
        fputs("        <tr><td /><td class=\"Flushed\" /></tr>\n", pipetrace_fd);

      fputs("        <tr><td>", pipetrace_fd);
      fprintf(pipetrace_fd, rom->addr_format, addr);
      fprintf(pipetrace_fd, "</td><td class=\"instr\">");
      const char *s = disass(addr);
      while (*s)
        {
          int n = strcspn(s, "<>");
          fprintf(pipetrace_fd, "%*.*s", n, n, s);
          s += n;

          switch (*s) {
            case '<':
              fprintf(pipetrace_fd, "&lt;");
              s++;
              break;

            case '>':
              fprintf(pipetrace_fd, "&gt;");
              s++;
              break;

            default:
              // Not possible
              break;
          }
        }
      fprintf(pipetrace_fd, "</td><td class=\"");
      for (unsigned int i = 1; i <= pipetrace_ticks; i++)
        fputs("\" /><td class=\"", pipetrace_fd);
    }
}

void
cl_stm8::pipetrace_instr_end(void)
{
  if (pipetrace_running && pipetrace_in_table)
    fputs("\" /></tr>\n", pipetrace_fd);
}

void
cl_stm8::pipetrace_tick(const char *aux)
{
  if (pipetrace_running)
    {
      fputs("\">", pipetrace_fd);
      if (aux) fprintf(pipetrace_fd, "<span class=\"%s\" />", aux);
      fputs("</td><td class=\"", pipetrace_fd);
    }
}

void
cl_stm8::pipetrace_type(const char *event)
{
  if (pipetrace_running)
    fprintf(pipetrace_fd, " %s", event);
}

cl_stm8_cpu::cl_stm8_cpu(class cl_uc *auc):
  cl_hw(auc, HW_DUMMY, 0, "cpu")
{
}

int
cl_stm8_cpu::init(void)
{
  int i;
  cl_hw::init();
  for (i= 0; i < 11; i++)
    {
      regs[i]= register_cell(uc->rom, 0x7f00+i);
    }
  cl_var *v;
  uc->vars->add(v= new cl_var(chars("sp_limit"), cfg, cpuconf_sp_limit,
			      cfg_help(cpuconf_sp_limit)));
  v->init();
  
  return 0;
}

void
cl_stm8_cpu::set_cmd(class cl_cmdline *cmdline, class cl_console_base *con)
{
  class cl_stm8 *stm8 = static_cast<cl_stm8 *>(uc);
  const char *p1, *p2;

  if (cmdline->syntax_match(uc, STRING STRING))
    {
      if ((p1 = cmdline->param(0)->value.string.string) &&
          strstr(p1, "pi") == p1 &&
          (p1 = cmdline->param(1)->value.string.string))
        {
          if (strstr(p1, "paus") == p1)
            {
              if (stm8->pipetrace_running)
                {
                  if (stm8->pipetrace_in_table)
                    stm8->pipetrace_end_table();
                  stm8->pipetrace_running = false;
                }
              else if (!stm8->pipetrace_fd)
                con->dd_printf("Not currently tracing pipeline acivity\n");
              return;
            }
          else if (strstr(p1, "re") == p1)
            {
              if (!stm8->pipetrace_running)
                {
                  if (stm8->pipetrace_fd)
                    {
                      stm8->pipetrace_ticks = 0;
                      stm8->pipetrace_running = true;
                    }
                  else
                    con->dd_printf("Not currently tracing pipeline acivity\n");
                }
              return;
            }
          else if (!strcmp(p1, "stop") || !strcmp(p1, "close"))
            {
              if (stm8->pipetrace_running)
                {
                  if (stm8->pipetrace_in_table)
                    stm8->pipetrace_end_table();
                  stm8->pipetrace_running = false;
                }
              if (stm8->pipetrace_fd)
                {
                  fputs("</body>\n</html>\n", stm8->pipetrace_fd);
                  fclose(stm8->pipetrace_fd);
                }
              if (stm8->pipetrace_file)
                free(stm8->pipetrace_file);
              stm8->pipetrace_fd = NULL;
              stm8->pipetrace_file= NULL;
              return;
            }
        }
    }
  else if (cmdline->syntax_match(uc, STRING STRING STRING))
    {
      if ((p1 = cmdline->param(0)->value.string.string) &&
          !strcmp(p1, "pipetrace") &&
          (p1 = cmdline->param(1)->value.string.string) &&
          (p2 = cmdline->param(2)->value.string.string))
        {
          if (!strcmp(p1, "fold") || !strcmp(p1, "folding"))
            {
              if (!strcmp(p2, "on"))
                {
                  stm8->pipetrace_fold = true;
                  return;
                }
              else if (!strcmp(p2, "off"))
                {
                  stm8->pipetrace_fold = false;
                  return;
                }
            }
          else if (!strcmp(p1, "title"))
            {
              if (stm8->pipetrace_title)
                free(stm8->pipetrace_title);
              stm8->pipetrace_title = strdup(p2);
              return;
            }
	  else if (!strcmp(p1, "style"))
            {
              if (stm8->pipetrace_style)
                free(stm8->pipetrace_style);
              stm8->pipetrace_style = strdup(p2);
              return;
            }
          else if (!strcmp(p1, "data") || !strcmp(p1, "text"))
            {
              if (stm8->pipetrace_fd)
                {
                  if (stm8->pipetrace_in_table)
                    stm8->pipetrace_end_table();
                  fputs(p2, stm8->pipetrace_fd);
                  putc('\n', stm8->pipetrace_fd);
                  stm8->pipetrace_ticks = 0;
                }
              else
                  con->dd_printf("Not currently tracing pipeline acivity\n");
              return;
            }
          else if (!strcmp(p1, "start") || !strcmp(p1, "output") || !strcmp(p1, "file"))
            {
              if (stm8->pipetrace_fd)
                {
                  con->dd_printf("Already writing pipeline trace to \"%s\". Close this first.\n", stm8->pipetrace_file);
                  return;
                }

              if (stm8->pipetrace_file)
                {
                  free(stm8->pipetrace_file);
                  stm8->pipetrace_file = NULL;
                }
              stm8->pipetrace_file = NULL;

              if ((stm8->pipetrace_fd = fopen(p2, "w")) == NULL)
                con->dd_printf("Can't open `%s': %s\n", p2, strerror(errno));
              else
                stm8->pipetrace_file = strdup(p2);

              fprintf(stm8->pipetrace_fd, "<html>\n<head>\n");

              if (stm8->pipetrace_title && stm8->pipetrace_title[0])
                fprintf(stm8->pipetrace_fd, "<title>%s</title>\n", stm8->pipetrace_title);

              if (stm8->pipetrace_style && stm8->pipetrace_style[0])
                fprintf(stm8->pipetrace_fd, "<link rel=\"stylesheet\" href=\"%s\" />\n", stm8->pipetrace_style);
              else
                fprintf(stm8->pipetrace_fd,
                  "<script>\n"
                  "    function toggle(id) {\n"
                  "        var obj = document.getElementById(id);\n"
                  "        if (obj.style.display === 'none')\n"
                  "            obj.style.display = 'block';\n"
                  "        else\n"
                  "            obj.style.display = 'none';\n"
                  "    }\n"
                  "</script>\n"
                  "\n"
                  "<style>\n"
                  "    .pipetrace { margin: 1em 0; }\n"
                  "    .pipetrace, .pipetrace td { border: 1px solid black; border-collapse: collapse; padding: 0 0.2em; white-space: nowrap; }\n"
                  "    .pipetrace thead { background-color: #d2d2d2; position: sticky; top: 0; }\n"
                  "    .pipetrace thead th { border: none; box-shadow: inset -1px -1px #000; }\n"
                  "    .pipetrace tr:nth-child(even) { background-color: #f2f2f2; }\n"
                  "    .pipetrace td { min-width: 1.6em; }\n"
                  "    .pipetrace td.instr:before { content: \"\"; }\n"
                  "    .pipetrace td.instr { width: 9.5em; }\n"
                  "    .pipetrace td.Sbus:before { content: \" S₀\"; }\n"
                  "    .pipetrace td.Sfetch:before { content: \" S₁\"; }\n"
                  "    .pipetrace td.Sraw:before { content: \" S₂\"; }\n"
                  "    .pipetrace td:before { content: attr(class); }\n"
                  "    .pipetrace td.D { background-color: lawngreen; }\n"
                  "    .pipetrace td.E { background-color: navajowhite; }\n"
                  "    .pipetrace td.Sbus { background-color: red; }\n"
                  "    .pipetrace td.Sfetch { background-color: red; }\n"
                  "    .pipetrace td.Sraw { background-color: red; }\n"
                  "    .pipetrace td.X, .pipetrace td.Flushed { background-color: red; }\n"
                  "    .pipetrace td span:after { content: attr(class); }\n"
                  "    .pipetrace td span { padding: 0 0.2em; margin: 0 0 0 0.1em; }\n"
                  "    .pipetrace td span.FS { background-color: lightgrey; }\n"
                  "    .pipetrace td span.F { background-color: lightskyblue; }\n"
                  "    #pipetrace_legend { border: none; background-color: white; position: fixed; bottom: 0; right: 0; }\n"
                  "    #pipetrace_legend tr:first-of-type { font-weight: bold; }\n"
                  "    #pipetrace_legend td { text-align: center; }\n"
                  "    #pipetrace_legend td.defn { text-align: left; }\n"
                  "    #pipetrace_legend td.defn:before { content: \"\"; }\n"
                  "</style>\n");

              fputs(
                "</head>\n"
                "\n"
                "<body onload=\"toggle('pipetrace_legend')\">\n",
                stm8->pipetrace_fd);

              if (stm8->pipetrace_title)
                fprintf(stm8->pipetrace_fd, "\n<h1>%s</h1>\n", stm8->pipetrace_title);

              fputs("\n"
                "<table id=\"pipetrace_legend\" class=\"pipetrace\">\n"
                "    <tr><td colspan=\"2\">Legend</td></tr>\n"
                "    <tr><td class=\"D\" /><td class=\"defn\">Decode</td></tr>\n"
                "    <tr><td class=\"E\" /><td class=\"defn\">Execute</td></tr>\n"
                "    <tr><td><span class=\"F\" /></td><td class=\"defn\">Fetch</td></tr>\n"
                "    <tr><td class=\"X\" /><td class=\"defn\">Flush</td></tr>\n"
                "    <tr><td><span class=\"FS\" /></td><td class=\"defn\">Fetch Stall - space in prefetch buffer but bus busy</td></tr>\n"
                "    <tr><td class=\"Sbus\" /><td class=\"defn\">Decode Stall - bus busy</td></tr>\n"
                "    <tr><td class=\"Sfetch\" /><td class=\"defn\">Decode Stall - insufficient data in prefetch buffer</td></tr>\n"
                "    <tr><td class=\"Sraw\" /><td class=\"defn\">Decode Stall - read after write</td></tr>\n"
                "</table>\n"
                "\n",
                stm8->pipetrace_fd);

              stm8->pipetrace_ticks = 0;
              stm8->pipetrace_running = true;
              stm8->pipetrace_in_table = false;
              return;
            }
        }
    }

    con->dd_printf("set hardware cpu[id] pipetrace title \"title\"\n");
    con->dd_printf("set hardware cpu[id] pipetrace style \"url\"\n");
    con->dd_printf("set hardware cpu[id] pipetrace start|output|file \"filename\"\n");
    con->dd_printf("set hardware cpu[id] pipetrace pause\n");
    con->dd_printf("set hardware cpu[id] pipetrace resume|restart\n");
    con->dd_printf("set hardware cpu[id] pipetrace data \"html...\"\n");
    con->dd_printf("set hardware cpu[id] pipetrace stop|close\n");
    con->dd_printf("set hardware cpu[id] pipetrace fold on|off\n");
}

void
cl_stm8_cpu::print_info(class cl_console_base *con)
{
  class cl_stm8 *stm8 = static_cast<cl_stm8 *>(uc);

  con->dd_printf("%s[%d]\n", id_string, id);
  con->dd_printf("  Pipetrace title: %s\n", (stm8->pipetrace_title ? stm8->pipetrace_title : "<not set>"));
  con->dd_printf("  Pipetrace style: %s\n", (stm8->pipetrace_style ? stm8->pipetrace_style : "<not set>"));
  con->dd_printf("  Pipetrace file: %s\n", (stm8->pipetrace_file ? stm8->pipetrace_file : "<not set>"));
  con->dd_printf("  Pipetrace folding: %s\n", (stm8->pipetrace_fold ? "on" : "off"));

  con->dd_printf("\n");
  print_cfg_info(con);
}


void
cl_stm8_cpu::write(class cl_memory_cell *cell, t_mem *val)
{
  t_addr a;
  cl_stm8 *u= (cl_stm8*)uc;

  if (conf(cell, val))
    return;
  
  if (conf(cell, NULL))
    return;

  *val&= 0xff;
  if (!uc->rom->is_owned(cell, &a))
    return;  
  if ((a < 0x7f00) ||
      (a > 0x7f0a))
    return;

  a-= 0x7f00;
  switch (a)
    {
    case 0:
      u->regs.A= *val;
      break;
    case 1:
      u->PC= (u->PC & 0xffff) + (*val << 16);
      break;
    case 2:
      u->PC= (u->PC & 0xff00ff) | (*val << 8);
      break;
    case 3:
      u->PC= (u->PC & 0xffff00) | (*val);
      break;
    case 4:
      u->regs.X= (u->regs.X & 0xff) | (*val << 8);
      break;
    case 5:
      u->regs.X= (u->regs.X & 0xff00) | (*val);
      break;
    case 6:
      u->regs.Y= (u->regs.Y & 0xff) | (*val << 8);
      break;
    case 7:
      u->regs.Y= (u->regs.Y & 0xff00) | (*val);
      break;
    case 8:
      u->regs.SP= (u->regs.SP & 0xff) | (*val << 8);
      break;
    case 9:
      u->regs.SP= (u->regs.SP & 0xff00) | (*val);
      break;
    case 0xa:
      u->regs.CC= (u->regs.CC & 0xff00) | (*val);
      break;
    }
}

t_mem
cl_stm8_cpu::read(class cl_memory_cell *cell)
{
  t_mem v= cell->get();
  t_addr a;
  cl_stm8 *u= (cl_stm8*)uc;
  
  if (conf(cell, NULL))
    return v;
  if (!uc->rom->is_owned(cell, &a))
    return v;
  if ((a < 0x7f00) ||
      (a > 0x7f0a))
    return v;

  a-= 0x7f00;
  switch (a)
    {
    case 0:
      v= u->regs.A;
      break;
    case 1:
      v= (u->PC >> 16) & 0xff;
      break;
    case 2:
      v= (u->PC >> 8) & 0xff;
      break;
    case 3:
      v= u->PC & 0xff;
      break;
    case 4:
      v= (u->regs.X >> 8) & 0xff;
      break;
    case 5:
      v= u->regs.X & 0xff;
      break;
    case 6:
      v= (u->regs.Y >> 8) & 0xff;
      break;
    case 7:
      v= u->regs.Y & 0xff;
      break;
    case 8:
      v= (u->regs.SP >> 8) & 0xff;
      break;
    case 9:
      v= u->regs.SP & 0xff;
      break;
    case 0xa:
      v= u->regs.CC;
      break;
    }
  return v;
}

t_mem
cl_stm8_cpu::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  class cl_stm8 *u= (class cl_stm8 *)uc;
  if (val)
    cell->set(*val);
  switch ((enum stm8_cpu_cfg)addr)
    {
    case cpuconf_sp_limit:
      if (val)
	u->sp_limit= *val & 0xffff;
      else
	cell->set(u->sp_limit);
      break;
    }
  return cell->get();
}

const char *
cl_stm8_cpu::cfg_help(t_addr addr)
{
  switch (addr)
    {
    case cpuconf_sp_limit:
      return "Stack overflows when SP is below this limit";
    }
  return "Not used";
}

/*
 * Errors in STM8 execution
 */

/* All of memory errors */

cl_error_stm8::cl_error_stm8(void)
{
  classification= stm8_error_registry.find("stm8");
}

cl_error_stm8_pipeline::
cl_error_stm8_pipeline(void):
  cl_error_stm8()
{
  classification= stm8_error_registry.find("pipeline");
}

cl_error_stm8_pipeline_decode_stall::
cl_error_stm8_pipeline_decode_stall(void):
  cl_error_stm8_pipeline()
{
  classification= stm8_error_registry.find("decode_stall");
}

void
cl_error_stm8_pipeline_decode_stall::print(class cl_commander_base *c)
{
  c->dd_printf("%s: decode stalled\n", get_type_name());
}

cl_error_stm8_pipeline_fetch_stall::
cl_error_stm8_pipeline_fetch_stall(void):
  cl_error_stm8_pipeline()
{
  classification= stm8_error_registry.find("fetch_stall");
}

void
cl_error_stm8_pipeline_fetch_stall::print(class cl_commander_base *c)
{
  c->dd_printf("%s: fetch stalled\n", get_type_name());
}

cl_stm8_error_registry::cl_stm8_error_registry(void)
{
  class cl_error_class *prev = stm8_error_registry.find("non-classified");
  prev = register_error(new cl_error_class(err_error, "stm8", prev, ERROR_OFF));
  prev = register_error(new cl_error_class(err_warning, "pipeline", prev));
  register_error(new cl_error_class(err_warning, "decode_stall", prev));
  register_error(new cl_error_class(err_warning, "fetch_stall", prev));
}

/* End of stm8.src/stm8.cc */
