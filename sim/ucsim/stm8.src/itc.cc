/*
 * Simulator of microcontrollers (itc.cc)
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

// prj

//#include <stdarg.h>
#include "utils.h"

// sim
#include "itsrccl.h"

// local
#include "itccl.h"


cl_itc::cl_itc(class cl_uc *auc):
  cl_hw(auc, HW_INTERRUPT, 0, "itc")
{
}

int
cl_itc::init(void)
{
  int i;
  cl_hw::init();
  for (i= 0; i < 8; i++)
    spr[i]= register_cell(uc->rom, 0x7f70+i);

  exti_cr1= uc->rom->get_cell(0x50a0);
  exti_cr2= uc->rom->get_cell(0x50a1);
  exti_cr3= uc->rom->get_cell(0x50a2);
  exti_cr4= uc->rom->get_cell(0x50aa);

  exti_sr1= uc->rom->get_cell(0x50a3);
  exti_sr2= uc->rom->get_cell(0x50a4);

  exti_conf1= uc->rom->get_cell(0x50a5);
  exti_conf2= uc->rom->get_cell(0x50ab);

  // Although we set up all registers above not all STM8 variants
  // use or expose them all. We only add vars for those that are
  // user-visible on this particular MCU.
  cl_var *v;
  char name[] = "ITC_SPR1";
  char desc[] = "Software priority register 1";
  for (i= 0; i < 8; i++, name[7]++, desc[27]++)
    {
      uc->vars->add(v= new cl_var(name, uc->rom, 0x7f70+i, desc, 7, 0));
      v->init();
    }

    uc->vars->add(v= new cl_var("EXTI_CR1", uc->rom, 0x50a0,
                                "External interrupt control register 1", 7, 0));
    v->init();
    uc->vars->add(v= new cl_var("EXTI_CR2", uc->rom, 0x50a1,
                                "External interrupt control register 2", 7, 0));
    v->init();

    if (uc->type->type == CPU_STM8L ||
        uc->type->type == CPU_STM8L101)
      {
        uc->vars->add(v= new cl_var("EXTI_CR3", uc->rom, 0x50a2,
                                    "External interrupt control register 3", 7, 0));
        v->init();

        uc->vars->add(v= new cl_var("EXTI_SR1", uc->rom, 0x50a3,
                                    "External interrupt status register 1", 7, 0));
        v->init();
        uc->vars->add(v= new cl_var("EXTI_SR2", uc->rom, 0x50a4,
                                    "External interrupt status register 2", 7, 0));
        v->init();

        uc->vars->add(v= new cl_var("EXTI_CONF", uc->rom, 0x50a5,
                                    "External interrupt port select register", 7, 0));
        v->init();
      }

    if (uc->type->type == CPU_STM8L)
      {
        uc->vars->add(v= new cl_var("EXTI_CR4", uc->rom, 0x50aa,
                                    "External interrupt control register 4", 7, 0));
        v->init();

        uc->vars->add(v= new cl_var("EXTI_CONF2", uc->rom, 0x50ab,
                                    "External interrupt port select register 2", 7, 0));
        v->init();
      }

  return(0);
}

void
cl_itc::added_to_uc(void)
{
}

void
cl_itc::new_hw_added(class cl_hw *new_hw)
{
}

t_mem
cl_itc::read(class cl_memory_cell *cell)
{
  if (conf(cell, NULL))
    return cell->get();
  
  return cell->get();
}

void
cl_itc::write(class cl_memory_cell *cell, t_mem *val)
{
  t_addr a;

  if (conf(cell, val))
    return;
  
  if (uc->rom->is_owned(cell, &a) &&
      (a >= 0x7f70) &&
      (a <  0x7f70+8))
    {
      u8_t mask= 0xff;
      if ((*val & 0x03) == 0x02)
	mask&= ~0x03;
      if ((*val & 0x0c) == 0x08)
	mask&= ~0x0c;
      if ((*val & 0x30) == 0x20)
	mask&= ~0x30;
      if ((*val & 0xc0) == 0x80)
	mask&= ~0xc0;
      u8_t o= cell->get(), v= *val;
      o&= ~mask;
      v&= mask;
      o|= v;
      *val= o;
    }
}

/*void
cl_itc::mem_cell_changed(class cl_m *mem, t_addr addr)
{
}*/

int
cl_itc::tick(int cycles)
{
  return(resGO);
}

void
cl_itc::reset(void)
{
  int i;
  for (i= 0; i < 8; i++)
    spr[i]->write(0xff);

  // These belong to the ITC but are normally handled in cl_port.
  exti_cr1->set(0);
  exti_cr2->set(0);
  exti_cr3->set(0);
  exti_cr4->set(0);
  exti_conf1->set(0);
  exti_conf2->set(0);
  exti_sr1->set(0);
  exti_sr2->set(0);
}

void
cl_itc::happen(class cl_hw *where, enum hw_event he, void *params)
{
}


void
cl_itc::print_info(class cl_console_base *con)
{
  int i;

  con->dd_printf("Interrupts are %s. Interrupt sources:\n",
		 (uc->it_enabled())?"enabled":"disabled");
  con->dd_printf("  Handler  En  Pr Req Act Name\n");
  for (i= 0; i < uc->it_sources->count; i++)
    {
      class cl_it_src *is= (class cl_it_src *)(uc->it_sources->at(i));
      con->dd_printf("  0x%06x", AU(is->addr));
      con->dd_printf(" %-3s", (is->enabled())?"en":"dis");
      con->dd_printf(" %2d", uc->priority_of(/*is->ie_mask*/is->nuof));
      con->dd_printf(" %-3s", (is->pending())?"YES":"no");
      con->dd_printf(" %-3s", (is->active)?"act":"no");
      con->dd_printf(" %s", object_name(is));
      con->dd_printf("\n");
    }
  con->dd_printf("Active interrupt service(s):\n");
  con->dd_printf("  Pr Handler  PC       Source\n");
  for (i= 0; i < uc->it_levels->count; i++)
    {
      class it_level *il= (class it_level *)(uc->it_levels->at(i));
      if (il->level >= 0)
	{
	  con->dd_printf("  %2d", il->level);
	  con->dd_printf(" 0x%06x", AU(il->addr));
	  con->dd_printf(" 0x%06x", AU(il->PC));
	  con->dd_printf(" %s", (il->source)?(object_name(il->source)):
			 "nothing");
	  con->dd_printf("\n");
	}
    }
  //print_cfg_info(con);
}


/* End of s51.src/itc.cc */
