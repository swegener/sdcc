/*
 * Simulator of microcontrollers (where.cc)
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
#include "simcl.h"
#include "memcl.h"
#include "uccl.h"
#include "globals.h"

#include "cmdutil.h"
#include "dump.h"


/*
 * Searching for a string in memory
 */

static void
where_memory(cl_mem *mem, bool cs, class cl_sim *sim)
{
  char *s;
  uchar *str;
  int len, i, found;
  t_addr start;

  if ((s= strtok(NULL, "\0")) == NULL)
    return;
  str= (uchar *)proc_escape(s, &len);
  // assume len < size!
  if (!cs)
    for (i= 0; i < len; i++)
      str[i]= toupper(str[i]);
  start= 0;
  while (start < mem->size-len)
    {
      t_addr tmp= start;
      found= TRUE;
      for (i= 0; found && (i<len); i++)
	found= str[i] == (cs?mem->get(start+i):toupper(mem->get(start+i)));
      if (found)
	{
	  dump_memory(mem, &tmp, start+len-1, 8, sim->cmd_out(), sim);
	  start+= len;
	}
      else
	start++;
    }
  free(str);
}


/*
 * WHERE IRAM string
 */

bool
cmd_where_iram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  where_memory(uc->mem(MEM_IRAM), FALSE, sim);
  return(FALSE);
}

bool
cmd_Where_iram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  where_memory(uc->mem(MEM_IRAM), TRUE, sim);
  return(FALSE);
}


/*
 * WHERE XRAM string
 */

bool
cmd_where_xram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  uc->eram2xram/*FIXME*/();
  where_memory(uc->mem(MEM_XRAM), FALSE, sim);
  return(FALSE);
}

bool
cmd_Where_xram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  uc->eram2xram/*FIXME*/();
  where_memory(uc->mem(MEM_XRAM), TRUE, sim);
  return(FALSE);
}


/*
 * WHERE CODE string
 */

bool
cmd_where_code(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  where_memory(uc->mem(MEM_ROM), FALSE, sim);
  return(FALSE);
}

bool
cmd_Where_code(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  where_memory(uc->mem(MEM_ROM), TRUE, sim);
  return(FALSE);
}


/* End of s51.src/where.cc */
