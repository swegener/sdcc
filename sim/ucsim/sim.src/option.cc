/*
 * Simulator of microcontrollers (option.cc)
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

#include "stypes.h"

#include "optioncl.h"
#include "simcl.h"


/*
 * Base class for option's objects
 *____________________________________________________________________________
 *
 */

cl_option::cl_option(void *opt, char *Iid, char *Ihelp):
  cl_base()
{
  option= opt;
  id    = strdup(Iid);
  help  = strdup(Ihelp);
}

cl_option::~cl_option(void)
{
  free(id);
  free(help);
}


/*
 * BOOL type of option
 *____________________________________________________________________________
 *
 */

cl_bool_opt::cl_bool_opt(bool *opt, char *Iid, char *Ihelp):
  cl_option(opt, Iid, Ihelp)
{}

void
cl_bool_opt::print(FILE *f)
{
  if (*(bool *)option)
    fprintf(f, "TRUE");
  else
    fprintf(f, "FALSE");
}

bool
cl_bool_opt::get_value(void)
{
  return(*((bool *)option));
}

void
cl_bool_opt::set_value(bool opt)
{
  *((bool *)option)= opt;
}

void
cl_bool_opt::set_value(char *s)
{
  char c;

  if (s)
    {
      c= toupper(*s);
      if (c == '1' ||
	  c == 'T' ||
	  c == 'Y')
	*(bool *)option= TRUE;
      else
	*(bool *)option= FALSE;
    }
};


/*
 * Debug on console
 */

cl_cons_debug_opt::cl_cons_debug_opt(class cl_sim *Asim,
				     char *Iid,
				     char *Ihelp):
  cl_option(0, Iid, Ihelp)
{
  sim= Asim;
}

void
cl_cons_debug_opt::print(FILE *f)
{
  if (sim->cmd->actual_console &&
      sim->cmd->actual_console->flags & CONS_DEBUG)
    fprintf(f, "TRUE");
  else
    fprintf(f, "FALSE");
}

bool
cl_cons_debug_opt::get_value(void)
{
  return(sim->cmd->actual_console?
	 (sim->cmd->actual_console->flags & CONS_DEBUG):
	 0);
}

void
cl_cons_debug_opt::set_value(bool opt)
{
  if (sim->cmd->actual_console)
    {
      if (opt)
	sim->cmd->actual_console->flags|= CONS_DEBUG;
      else
	sim->cmd->actual_console->flags&= ~CONS_DEBUG;
    }
    
}

void
cl_cons_debug_opt::set_value(char *s)
{
  char c;

  if (s &&
      sim->cmd->actual_console)
    {
      c= toupper(*s);
      if (c == '1' ||
	  c == 'T' ||
	  c == 'Y')
	sim->cmd->actual_console->flags|= CONS_DEBUG;
      else
	sim->cmd->actual_console->flags&= ~CONS_DEBUG;
    }
}


/* End of option.cc */
