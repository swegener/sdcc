/*
 * Simulator of microcontrollers (info.cc)
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

#include <stdlib.h>
#include "i_string.h"

// sim.src
#include "simcl.h"
 
// local
#include "infocl.h"


/*
 * INFO BREAKPOINTS command
 */

int
cl_info_bp_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  int i;
  char *s;

  con->printf("Num Type       Disp Hit   Cnt   Address  What\n");
  for (i= 0; i < sim->uc->fbrk->count; i++)
    {
      class cl_brk *fb= (class cl_brk *)(sim->uc->fbrk->at(i));
      s= sim->uc->disass(fb->addr, NULL);
      con->printf("%-3d %-10s %s %-5d %-5d 0x%06x %s\n", fb->nr,
		  "fetch", (fb->perm==brkFIX)?"keep":"del ",
		  fb->hit, fb->cnt,
		  fb->addr, s);
      free(s);
    }
  for (i= 0; i < sim->uc->ebrk->count; i++)
    {
      class cl_ev_brk *eb= (class cl_ev_brk *)(sim->uc->ebrk->at(i));
      con->printf("%-3d %-10s %s %-5d %-5d 0x%06x %s\n", eb->nr,
		  "event", (eb->perm==brkFIX)?"keep":"del ",
		  eb->hit, eb->cnt,
		  eb->addr, eb->id);
    }
  /*t_addr a;
  class cl_rom *r= (class cl_rom *)(sim->uc->mem(MEM_ROM));
  for (a= 0; a < sim->uc->get_mem_size(MEM_ROM); a++)
    {
      if (r->bp_map->get(a))
	con->printf("0x%06x\n", a);
	}*/
  return(0);
}


/*
 * INFO REGISTERS command
 */

int
cl_info_reg_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  sim->uc->print_regs(con);
  return(0);
}


/*
 * INFO HW command
 */

int
cl_info_hw_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };
  char *p0;
  enum hw_cath cath;
  class cl_hw *hw;
  int i= 0;

  if (params[0] == 0)
    {
      con->printf("Cathegory missing\n");
      return(0);
    }
  p0= (params[0])->get_svalue();
  if (strstr(p0, "t") == p0)
    cath= HW_TIMER;
  else if (strstr(p0, "u") == p0)
    cath= HW_UART;
  else if (strstr(p0, "po") == p0)
    cath= HW_PORT;
  else if (strstr(p0, "pc") == p0)
    cath= HW_PCA;
  else if (strstr(p0, "i") == p0)
    cath= HW_INTERRUPT;
  else if (strstr(p0, "w") == p0)
    cath= HW_WDT;
  else
    {
      con->printf("Unknown cathegory\n");
      return(0);
    }
  if (params[1] == 0)
    {
      // no ID
      hw= sim->uc->get_hw(cath, &i);
      while (hw)
	{
	  hw->print_info(con);
	  i++;
	  hw= sim->uc->get_hw(cath, &i);
	}
    }
  else
    {
      // ID given
      int id= (params[1])->get_ivalue();
      hw= sim->uc->get_hw(cath, id, &i);
      while (hw)
	{
	  hw->print_info(con);
	  i++;
	  hw= sim->uc->get_hw(cath, id, &i);
	}      
    }
  return(0);
}


/* End of info.cc */
