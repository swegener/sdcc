/*
 * Simulator of microcontrollers (set.cc)
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

#include "regs51.h" //FIXME


/*
 * Parsing command and write data into specified buffer
 */

static void
set_memory(cl_mem *mem, t_addr first,
	   class cl_uc *uc, struct name_entry tabl[], class cl_sim *sim)
{
  t_addr start, addr;
  char *s, *p;
  uchar data, *what= 0;
  struct name_entry *ne= NULL;

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Address has not given.\n");
      return;
    }
  if (tabl)
    ne= get_name_entry(tabl, s, uc);
  if (ne)
    addr= ne->addr;
  else
    {
      addr= strtol(s, &p, 0);
      if (p &&
	  *p)
	{
	  sim->cmd->printf("Bad address.\n");
	  return;
	}
    }
  start= addr;
  if (start >= mem->size)
    {
      sim->cmd->printf("Address %ld(0x%lx) is bigger than %ld(0x%lx).\n",
		       start, start, mem->size-1, mem->size-1);
      return;
    }
  if (!(start >= first))
    {
      sim->cmd->printf("Address %ld(0x%lx) is less than %ld(0x%lx).\n",
		       start, start, first, first);
      return;
    }
  if ((s= strtok(NULL, " \t\v")) == NULL)
    {
      sim->cmd->printf("Data has not given.\n");
      return;
    }
  while (s &&
	 addr < mem->size)
    {
      if (isdigit(*s))
	{
	  data= strtol(s, &p, 0);
	  if (p &&
	      *p)
	    {
	      sim->cmd->printf("Bad data %s\n", s);
	      break;
	    }
	  mem->set(addr, data);
	  if (what == uc->MEM(MEM_SFR))
	    {
	      uc->proc_write/*FIXME*/(&what[addr]);
	      if (addr == ACC)
		uc->set_p_flag/*FIXME*/();
	    }
	  if (what == uc->MEM(MEM_ROM))
	    uc->del_inst_at/*FIXME*/(addr);
	  addr++;
	}
      else
	{
	  int i, len;
	  p= proc_escape(s, &len);
	  i= 0;
	  while ((i < len) &&
		 addr < mem->size)
	    {
	      what[addr]= p[i++];
	      if (what == uc->MEM(MEM_SFR))
		{
		  uc->proc_write/*FIXME*/(&what[addr]);
		  if (addr == ACC)
		    uc->set_p_flag/*FIXME*/();
		}
	      if (what == uc->MEM(MEM_ROM))
		uc->del_inst_at/*FIXME*/(addr);
	      addr++;
	    }
	  free(p);
	}
      s= strtok(NULL, " \t\v");
    }
  dump_memory(mem, &start, addr-1, 16, sim->cmd->actual_console, sim);
}


/*
 * SET IRAM addr data...
 */

bool
cmd_set_iram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  set_memory(uc->mem(MEM_IRAM), 0, uc, NULL, sim);
  return(DD_FALSE);
}


/*
 * SET XRAM addr data...
 */

bool
cmd_set_xram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  uc->eram2xram();
  set_memory(uc->mem(MEM_XRAM), 0, uc, NULL, sim);
  uc->xram2eram();
  return(DD_FALSE);
}


/*
 * SET CODE addr data...
 */

bool
cmd_set_code(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  set_memory(uc->mem(MEM_ROM), 0, uc, NULL, sim);
  return(DD_FALSE);
}


/*
 * SET SFR addr data...
 */

bool
cmd_set_sfr(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  set_memory(uc->mem(MEM_SFR), SFR_START, uc, uc->sfr_tbl(), sim);
  return(DD_FALSE);
}


/*
 * SET BIT addr data...
 */

bool
cmd_set_bit(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  char *s;
  uchar *cell, addr;
  uchar bitaddr, bitmask;

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Address has not given.\n");
      return(DD_FALSE);
    }
  if (!interpret_bitname(s, uc, &cell, &addr, &bitaddr, &bitmask, NULL))
    {
      sim->cmd->printf("Bad address %s\n", s);
      return(DD_FALSE);
    }

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Data has not given.\n");
      return(DD_FALSE);
    }
  while (s)
    {
      if (!isdigit(*s))
	{
	  sim->cmd->printf("Bad data %s\n", s);
	  return(DD_FALSE);
	}
      if (*s == '0')
	*cell&= ~bitmask;
      else
	*cell|= bitmask;
      bitmask<<= 1;
      bitmask&= 0xff;
      s= strtok(NULL, delimiters);
    }
  return(DD_FALSE);
}


/*
 * SET PORT port data
 */

bool
cmd_set_port(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  char *s, *p;
  uchar port, data;
  
  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Port number has not given.\n");
      return(DD_FALSE);
    }
  port= strtol(s, &p, 0);
  if ((p && *p) ||
      (port > 3))
    {
      sim->cmd->printf("Port number %s is wrong.\n", s);
      return(DD_FALSE);
    }

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Date has not given.\n");
      return(DD_FALSE);
    }
  data= strtol(s, &p, 0);
  if (p && *p)
    {
      sim->cmd->printf("Data %s is wrong.\n", s);
      return(DD_FALSE);
    }
  uc->port_pins[port]= data;
  return(DD_FALSE);
}


/*
 * Fill memory commands
 */

static void
fill_memory(uchar *what, int size, int first, class cl_uc *uc,
	    class cl_sim *sim)
{
  char *s, *p;
  int start, stop;
  uchar data;

  if ((s= strtok(NULL, delimiters)) == NULL)
    return;
  start= strtol(s, &p, 0);
  if ((p && *p) ||
      (start < 0) ||
      (start < first) ||
      (start >= size))
    sim->cmd->printf("Start address %s is wrong.\n", s);

  if ((s= strtok(NULL, delimiters)) == NULL)
    return;
  stop= strtol(s, &p, 0);
  if ((p && *p) ||
      (stop < start) ||
      (stop >= size))
    sim->cmd->printf("Stop address %s is wrong.\n", s);

  if ((s= strtok(NULL, delimiters)) == NULL)
    return;
  data= strtol(s, &p, 0);
  if (p && *p)
    sim->cmd->printf("Data %s is wrong.\n", s);
  
  while (start <= stop)
    {
      what[start]= data;
      if (what == uc->MEM(MEM_SFR))
	{
	  uc->proc_write/*FIXME*/(&what[start]);
	  if (start == ACC)
	    uc->set_p_flag/*FIXME*/();
	}
      if (what == uc->MEM(MEM_ROM))
	uc->del_inst_at/*FIXME*/(start);
      start++;
    }
}


/*
 * FILL IRAM from to data
 */

bool
cmd_fill_iram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  fill_memory(uc->MEM(MEM_IRAM), uc->get_mem_size(MEM_IRAM), 0, uc, sim);
  return(DD_FALSE);
}


/*
 * FILL XRAM from to data
 */

bool
cmd_fill_xram(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  fill_memory(uc->MEM(MEM_XRAM), uc->get_mem_size(MEM_XRAM), 0, uc, sim);
  uc->xram2eram/*FIXME*/();
  return(DD_FALSE);
}


/*
 * FILL SFR from to data
 */

bool
cmd_fill_sfr(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  fill_memory(uc->MEM(MEM_SFR), uc->get_mem_size(MEM_SFR), SFR_START, uc, sim);
  return(DD_FALSE);
}


/*
 * FILL CODE from to data
 */

bool
cmd_fill_code(char *cmd, class cl_uc *uc, class cl_sim *sim)
{
  fill_memory(uc->MEM(MEM_ROM), EROM_SIZE, 0, uc, sim);
  return(DD_FALSE);
}


/* End of s51.src/set.cc */
