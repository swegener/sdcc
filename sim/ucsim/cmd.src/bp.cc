/*
 * Simulator of microcontrollers (bp.cc)
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

#include "stdlib.h"

// sim
#include "brkcl.h"
#include "argcl.h"
#include "simcl.h"

// cmd
#include "cmdsetcl.h"


/*
 * BREAK command
 */

int
cl_break_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  long addr;
  int  hit= 1;
  class cl_cmd_arg *params[2]= { cmdline->param(0),
				 cmdline->param(1) };

  if (params[0] == 0)
    {
      con->printf("Address is missing.\n");
      return(FALSE);
    }
  addr= (params[0])->get_address();
  if (params[1])
    hit= (params[1])->get_ivalue();
  if (hit > 99999)
    {
      con->printf("Hit value %d is too big.\n", hit);
      return(0);
    }
  if (sim->uc->fbrk->bp_at(addr))
    con->printf("Breakpoint at 0x%06x is already set.\n", addr);
  else
    {
      class cl_brk *b= new cl_fetch_brk(sim->uc->fbrk->make_new_nr(),
					addr, perm, hit);
      b->init();
      sim->uc->fbrk->add_bp(b);
      char *s= sim->uc->disass(addr, NULL);
      con->printf("Breakpoint %d at 0x%06x: %s\n", b->nr, addr, s);
      free(s);
    }
  return(0);
}


/*
 * CLEAR address
 */

int
cl_clear_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  uint addr;
  int idx;
  class cl_brk *brk= sim->uc->fbrk->get_bp(sim->uc->PC, &idx);

  if (cmdline->param(0) == 0)
    {
      if (!brk)
	{
	  con->printf("No breakpoint at this address.\n");
	  return(0);
	}
      sim->uc->fbrk->del_bp(sim->uc->PC);
      return(0);
    }

  int i= 0;
  class cl_cmd_arg *param;
  while ((param= cmdline->param(i++)))
    {
      addr= param->get_ivalue();
      if (sim->uc->fbrk->bp_at(addr) == 0)
	sim->cmd->printf("No breakpoint at 0x%06x\n", addr);
      else
	sim->uc->fbrk->del_bp(addr);
    }

  return(0);
}


/* End of bp.cc */
