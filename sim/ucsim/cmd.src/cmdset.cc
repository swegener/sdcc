/*
 * Simulator of microcontrollers (cmd.src/cmdset.cc)
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

#include "i_string.h"

#include "globals.h"
#include "utils.h"

// sim
#include "simcl.h"

// local
#include "cmdsetcl.h"


/*
 * Command: conf
 *----------------------------------------------------------------------------
 */

int
cl_conf_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  int i;

  con->printf("ucsim version %s\n", VERSIONSTR);
  con->printf("Type of microcontroller: %s\n", sim->uc->id_string());
  con->printf("Controller has %d hardware element(s).\n",
	      sim->uc->hws->count);
  for (i= 0; i < sim->uc->hws->count; i++)
    {
      class cl_hw *hw= (class cl_hw *)(sim->uc->hws->at(i));
      con->printf("  %s[%d]\n", hw->id_string, hw->id);
    }
  con->printf("Memories:\n");
  for (i= MEM_ROM; i < MEM_TYPES; i++)
    {
      class cl_mem *mem= (class cl_mem *)(sim->uc->mems->at(i));
      if (mem)
	con->printf("  %s size= 0x%06x %d\n",
		    mem->id_string(), mem->size, mem->size);
    }
  return(0);
}


/*
 * Command: state
 *----------------------------------------------------------------------------
 */

int
cl_state_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  //con->printf("sim state= %d\n", sim->state);
  con->printf("CPU state= %s PC= 0x%06x XTAL= %g\n",
	      get_id_string(cpu_states, sim->uc->state),
	      sim->uc->PC, 
	      sim->uc->xtal);
  con->printf("Total time since last reset= %g sec (%lu clks)\n",
	      sim->uc->get_rtime(), sim->uc->ticks->ticks);
  con->printf("Time in isr = %g sec (%lu clks) %3.2g%%\n",
	      sim->uc->isr_ticks->get_rtime(sim->uc->xtal),
	      sim->uc->isr_ticks->ticks,
	      (sim->uc->ticks->ticks == 0)?0.0:
	      (100.0*((double)(sim->uc->isr_ticks->ticks)/
		      (double)(sim->uc->ticks->ticks))));
  con->printf("Time in idle= %g sec (%lu clks) %3.2g%%\n",
	      sim->uc->idle_ticks->get_rtime(sim->uc->xtal),
	      sim->uc->idle_ticks->ticks,
	      (sim->uc->ticks->ticks == 0)?0.0:
	      (100.0*((double)(sim->uc->idle_ticks->ticks)/
		      (double)(sim->uc->ticks->ticks))));
  con->printf("Max value of stack pointer= 0x%06x, avg= 0x%06x\n",
	      sim->uc->sp_max, sim->uc->sp_avg);
  return(0);
}


/*
 * Command: file
 *----------------------------------------------------------------------------
 */

int
cl_file_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  char *fname= 0;
  long l;
  
  if ((cmdline->param(0) == 0) ||
      ((fname= cmdline->param(0)->get_svalue()) == NULL))
    {
      con->printf("File name is missing.\n");
      return(0);
    }
  if ((l= sim->uc->read_hex_file(fname)) >= 0)
    con->printf("%ld words read from %s\n", l, fname);

  return(0);
}


/*
 * Command: download
 *----------------------------------------------------------------------------
 */

int
cl_dl_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  long l;
  
  if ((l= sim->uc->read_hex_file(NULL)) >= 0)
    con->printf("%ld words loaded\n", l);

  return(0);
}


/*
 * Command: get
 *----------------------------------------------------------------------------
 */

int
cl_get_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  char *s;

  if (cmdline->param(0) == 0)
    {
      con->printf("Get what?\n");
      return(0);
    }
  if ((s= cmdline->param(0)->get_svalue()))
    {
      if (strstr(s, "t") == s)
	return(timer(cmdline, con));
      else
	con->printf("Unknow keyword of get command\n");
    }
  return(0);
}


/*
 * Command: set
 *----------------------------------------------------------------------------
 */


int
cl_set_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  char *s;

  if (cmdline->param(0) == 0)
    {
      con->printf("Set what?\n");
      return(0);
    }
  if ((s= cmdline->param(0)->get_svalue()))
    {
      if (strstr(s, "t") == s)
	return(timer(cmdline, con));
      else
	con->printf("Unknow keyword of set command\n");
    }
  return(0);
}


/*
 * Command: run
 *----------------------------------------------------------------------------
 */


int
cl_run_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  sim->start(con);
  return(0);
}


/*
 * Command: step
 *----------------------------------------------------------------------------
 */


int
cl_step_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  sim->uc->do_inst(1);
  sim->uc->print_regs(con);
  return(0);
}


/*
 * Command: reset
 *----------------------------------------------------------------------------
 */


int
cl_reset_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  sim->uc->reset();
  return(0);
}


/*
 * Command: dump
 *----------------------------------------------------------------------------
 */

int
cl_dump_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  class cl_mem *mem;
  char *s;
  long l, start= -1, end= 10*8-1, bpl= 8;
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };

  if (params[0] == 0)
    {
      con->printf("Memory type missing\n");
      return(0);
    }
  if ((s= params[0]->get_svalue()))
    {
      if (strstr(s, "i") == s)
	mem= sim->uc->mem(MEM_IRAM);
      else if (strstr(s, "x") == s)
	mem= sim->uc->mem(MEM_XRAM);
      else if (strstr(s, "r") == s)
	mem= sim->uc->mem(MEM_ROM);
      else if (strstr(s, "s") == s)
	mem= sim->uc->mem(MEM_SFR);
      else
	{
	  con->printf("Unknown memory type\n");
	  return(0);
	}
      if (!mem)
	{
	  con->printf("No such memory\n");
	  return(0);
	}
    }
  else
    {
      con->printf("Wrong memory type\n");
      return(0);
    }

  if (params[1])
    {
      if ((start= params[1]->get_address()) >= 0)
	{
	  end+= start;
	  if (params[2])
	    {
	      if ((l= params[2]->get_address()) >= 0)
		end= l;
	      else
		{
		  con->printf("End address is wrong\n");
		  return(0);
		}
	    }
	}
      else
	{
	  con->printf("Start address is wrong\n");
	  return(0);
	}
    }
  if (params[3] &&
      (l= params[3]->get_address()) >= 0)
    bpl= l;
  mem->dump(start, end, bpl, con);
  return(0);
}


/*
 * Command: di
 *----------------------------------------------------------------------------
 */

int
cl_di_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("i"));
  cl_dump_cmd::do_work(cmdline, con);
  return(0);
}


/*
 * Command: dx
 *----------------------------------------------------------------------------
 */

int
cl_dx_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("x"));
  cl_dump_cmd::do_work(cmdline, con);
  return(0);
}


/*
 * Command: dch
 *----------------------------------------------------------------------------
 */

int
cl_dch_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("r"));
  cl_dump_cmd::do_work(cmdline, con);
  return(0);
}


/*
 * Command: ds
 *----------------------------------------------------------------------------
 */

int
cl_ds_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("s"));
  cl_dump_cmd::do_work(cmdline, con);
  return(0);
}


/*
 * Command: dc
 *----------------------------------------------------------------------------
 */

int
cl_dc_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  long i, l, start= last, end= -1;
  class cl_cmd_arg *params[2]= { cmdline->param(0),
				 cmdline->param(1) };

  if (params[0])
    {
      if ((start= params[0]->get_address()) >= 0)
	{
	  if (params[1])
	    {
	      if ((l= params[1]->get_address()) >= 0)
		end= l;
	      else
		{
		  con->printf("End address is wrong\n");
		  return(0);
		}
	    }
	}
      else
	{
	  con->printf("Start address is wrong\n");
	  return(0);
	}
    }
  i= 0;
  for (l= start;
       (end < 0 && i < 20) || l <= end;
       l+= sim->uc->inst_length(sim->uc->get_mem(MEM_ROM, l)), i++)
    sim->uc->print_disass(l, con);
  last= l;
  return(0);
}


/*
 * Command: help
 *----------------------------------------------------------------------------
 */

int
cl_help_cmd::do_work(class cl_cmdline */*cmdline*/, class cl_console *con)
{
  class cl_cmd *c;
  int i;

  for (i= 0; i < sim->cmdset->count; i++)
    {
      c= (class cl_cmd *)(sim->cmdset->at(i));
      if (c->short_help)
	con->printf("%s\n", c->short_help);
      else
	con->printf("%s\n", (char*)(c->names->at(0)));
    }
  return(0);
}


/*
 * Command: quit
 *----------------------------------------------------------------------------
 */

int
cl_quit_cmd::do_work(class cl_cmdline */*cmdline*/, class cl_console */*con*/)
{
  return(1);
}

/*
 * Command: kill
 *----------------------------------------------------------------------------
 */

int
cl_kill_cmd::do_work(class cl_cmdline */*cmdline*/, class cl_console */*con*/)
{
  sim->state|= SIM_QUIT;
  return(1);
}


/* End of cmd.src/cmdset.cc */
