/*
 * Simulator of microcontrollers (cmd51.cc)
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
#include "i_string.h"

#include "globals.h"

#include "cmd51cl.h"


/*
 * Special console for s51 it uses old version of the command interpreter
 */

cl_51cons::cl_51cons(char *fin, char *fout, class cl_sim *asim):
  cl_console(fin, fout, asim)
{}

cl_51cons::cl_51cons(FILE *fin, FILE *fout, class cl_sim *asim):
  cl_console(fin, fout, asim)
{}

#ifdef SOCKET_AVAIL
cl_51cons::cl_51cons(int portnumber, class cl_sim *asim):
  cl_console(portnumber, asim)
{}
#endif

int
cl_51cons::proc_input(void)
{
  return(cl_console::proc_input());
  /*char *cmd= read_line();
  if (!cmd)
    return(1);
  int retval= interpret(cmd);
  prompt();
  free(cmd);
  return(retval);*/
}

bool
cl_51cons::interpret(char *cmd)
{
  int i;
  char *c, *s;
  bool repeat= FALSE, retval= FALSE;
  
  if (*cmd)
    {
      c= strdup(cmd);
      if (last_command)
	free(last_command);
      last_command= strdup(cmd);
    }
  else
    if (last_command == NULL)
      return(FALSE);
    else
      {
	c= strdup(last_command);
	repeat= TRUE;
      }
  i= 0;
  s= strtok(c, delimiters);
  while ((cmd_table[i].name != NULL) &&
	 /*(strstr(c, cmd_table[i].name) != c)*/
	 (strcmp(s, cmd_table[i].name) != 0))
    i++;
  if (cmd_table[i].name == NULL)
    {
      fprintf(sim->cmd_out(), "Unknown command.\n");
      if (last_command)
	{
	  free(last_command);
	  last_command= NULL;
	}
    }
  else
    {
      if (!repeat)
	retval= cmd_table[i].func(c, sim->uc, sim);
      else
	if (cmd_table[i].can_repeat)
	  retval= cmd_table[i].func(c, sim->uc, sim);
    }
  free(c);
  return(retval);
}

bool
cl_51cons::old_command(class cl_cmdline *cmdline)
{
  int i= 0;

  if (cmdline->name == 0 ||
      *(cmdline->name) == '\0' ||
      *(cmdline->name) == '\n')
    return(TRUE);
  while ((cmd_table[i].name != NULL) &&
	 /*(strstr(c, cmd_table[i].name) != c)*/
	 (strcmp(cmdline->name, cmd_table[i].name) != 0))
    i++;
  return(cmd_table[i].name != NULL);
}


/*
 * Special commander for s51 it uses special console
 */

cl_51cmd::cl_51cmd(class cl_sim *asim):
  cl_commander(asim)
{}

class cl_console *
cl_51cmd::mk_console(char *fin, char *fout, class cl_sim *asim)
{
  return(new cl_51cons(fin, fout, asim));
}

class cl_console *
cl_51cmd::mk_console(FILE *fin, FILE *fout, class cl_sim *asim)
{
  return(new cl_51cons(fin, fout, asim));
}


/* End of s51.src/cmd51.cc */
