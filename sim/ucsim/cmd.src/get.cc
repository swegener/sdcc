/*
 * Simulator of microcontrollers (get.cc)
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

// sim
#include "simcl.h"

// local
#include "cmdsetcl.h"


/*
 * GET TIMER
 */

int
cl_get_cmd::timer(class cl_cmdline *cmdline, class cl_console *con)
{
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };
  int what;
  class cl_ticker *ticker;
  
  if (params[1] != 0)
    {
      what= (params[1])->get_ivalue();
      if ((ticker= sim->uc->get_counter(what)) == 0)
	{
	  con->printf("Timer %d does not exist\n", what);
	  return(0);
	}
      con->printf("timer #%d: %g sec (%lu clks)\n", what,
		  ticker->get_rtime(sim->uc->xtal), ticker->ticks);
    }
  else
    {
      con->printf("time: %g sec (%lu clks)\n",
		  sim->uc->get_rtime(), sim->uc->ticks->ticks);
      for (what= 0; what < sim->uc->counters->count; what++)
	{
	  ticker= sim->uc->get_counter(what);
	  if (ticker)
	    con->printf("timer #%d: %g sec (%lu clks)\n", what,
			ticker->get_rtime(sim->uc->xtal), ticker->ticks);
	}
    }

  return(0);
}


/* End of get.cc */
