/*
 * Simulator of microcontrollers (go.cc)
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

#include "ddconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "i_string.h"

#include "simcl.h"

#include "uc51cl.h" //FIXME
#include "globals.h"
#include "dump.h"


/*
 * GO [start [stop]]
 */
//FIXME
bool
cmd_go(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *start_str, *stop_str;
  t_addr start= uc->PC;
  long stop= -1;
  class cl_brk *b;
  bool brk_at_stop= FALSE;

  if (sim->state & SIM_GO)
    {
      fprintf(sim->cmd_out(),
	      "Execution is already running.\n");
      return(0);
    }
  if ((start_str= strtok(NULL, delimiters)) != NULL)
    {
      start= strtol(start_str, NULL, 0);
      if ((stop_str= strtok(NULL, delimiters)) != NULL)
	stop= strtol(stop_str, NULL, 0);
    }
  if (uc->PC != start)
    {
      if (!uc->inst_at(start) &&
	  uc->debug)
	fprintf(sim->cmd_out(),
		"Warning: maybe not instruction at 0x%06lx\n", start);
      uc->PC= start;
    }
  if (stop >= 0)
    {
      if (start == (t_addr)stop)
	{
	  fprintf(sim->cmd_out(), "Addresses must be different.\n");
	  return(FALSE);
	}
      if ((b= uc->fbrk_at(stop)))
	{
	  brk_at_stop= TRUE;
	  b->cnt= 1;
	}
      else
	{
	  b= new cl_fetch_brk(uc->fbrk->make_new_nr(), stop, brkDYNAMIC, 1);
	  uc->fbrk->add_bp(b);
	}
    }
  if (uc->fbrk_at(start))
    uc->do_inst(1);
  //fprintf(sim->cmd_out(), "Simulation started, PC=0x%06x\n", uc->PC);
  sim->cmd->printf("Simulation started, PC=0x%06x\n", uc->PC);
  sim->start(sim->cmd->actual_console);
  //uc->do_inst(-1);
  /*if ((stop >= 0) &&
      !brk_at_stop)
      uc->fbrk->del_bp(stop);*/
  /*fprintf(sim->cmd_out(), "%d\n", uc->result);
    uc->print_disass(uc->PC);*/
  return(FALSE);
}


bool
cmd_stop(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  sim->stop(resUSER);
  uc->print_disass(uc->PC, sim->cmd->actual_console);
  return(FALSE);
}


static void
dump_step(class t_uc51 *uc, class cl_sim *sim)
{
  //cmd_dump_regs(NULL, uc, sim);
  uc->print_regs(sim->cmd->actual_console);
}


/*
 * STEP [step]
 */
//FIXME
bool
cmd_step(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  int step= 1;
  char *s;

  if ((s= strtok(NULL, delimiters)) != NULL)
    step= strtol(s, NULL, 0);
  while (step--)
    {
      uc->do_inst(1);
      dump_step(uc, sim);
    }
  return(FALSE);
}


/*
 * NEXT [step]
 */
//FIXME
bool
cmd_next(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  int step= 1;
  char *s;
  class cl_brk *b;
  uint next;

  if ((s= strtok(NULL, delimiters)) != NULL)
    step= strtol(s, NULL, 0);
  while (step--)
    {
      if ((uc->dis_tbl()[uc->get_mem(MEM_ROM, uc->PC)].branch == 'a') ||
	  (uc->dis_tbl()[uc->get_mem(MEM_ROM, uc->PC)].branch == 'l'))
	{
	  next= uc->PC+2;
	  if (uc->dis_tbl()[uc->get_mem(MEM_ROM, uc->PC)].branch == 'l')
	    next++;
	  if (!uc->fbrk_at(next))
	    {
	      b= new cl_fetch_brk(uc->fbrk->make_new_nr(),
				  next, brkDYNAMIC, 1);
	      uc->fbrk->add(b);
	    }
	  uc->do_inst(-1);
	}
      else
	uc->do_inst(1);
      dump_step(uc, sim);
    }
  return(FALSE);
}


/*
 * PC [address]
 */
//FIXME
bool
cmd_pc(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s, *p= NULL;
  long pc;

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      uc->print_disass(uc->PC, sim->cmd->actual_console);
      return(FALSE);
    }
  pc= strtol(s, &p, 0);
  if (p &&
      ((p == s) ||
       *p))
    fprintf(sim->cmd_out(), "Wrong parameter, PC unchanged.\n");
  else
    {
      if (pc >= EROM_SIZE)
	pc= 0;
      if (!uc->inst_at(pc) &&
	  uc->debug)
	fprintf(sim->cmd_out(),
		"Warning: maybe not instruction at %06lx\n", pc);
      uc->PC= pc;
      uc->print_disass(uc->PC, sim->cmd->actual_console);
    }
  return(FALSE);
}


/* End of s51.src/go.cc */
