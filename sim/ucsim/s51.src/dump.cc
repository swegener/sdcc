/*
 * Simulator of microcontrollers (dump.cc)
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
#include "regs51.h" //FIXME
#include "globals.h"
#include "cmdutil.h"


/*
 * Dump memory
 */

void
dump_memory(cl_mem *mem,
	    t_addr *start, t_addr stop, int bpl, FILE *f,
	    class cl_sim *sim)
{
  int i;

  while ((*start <= stop) &&
	 (*start < mem->size))
    {
      sim->cmd->printf("%06x ", *start);
      for (i= 0; (i < bpl) &&
	     (*start+i < mem->size) &&
	     (*start+i <= stop);
	   i++)
	{
	  char format[10];
	  sprintf(format, "%%0%dx ", mem->width/4);
	  fprintf(f, format/*"%02x "*/, mem->get(*start+i));
	}
      while (i < bpl)
	{
	  fprintf(f, "   ");
	  i++;
	}
      for (i= 0; (i < bpl) &&
	     (*start+i < mem->size) &&
	     (*start+i <= stop);
	   i++)
	fprintf(f, "%c",
		isprint(mem->get(*start+i))?(char)mem->get(*start+i):'.');
      fprintf(f, "\n");
      (*start)+= bpl;
    }
}


/*
 * DISASSEMBLE [start [offset [lines]]]
 */
//FIXME
static int disass_last_stop= 0;

bool
cmd_disass(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s;
  int  start, offset= -1, dir;
  int  lines= 20;
  uint realstart;

  if (!uc->there_is_inst())
    return(DD_FALSE);
  if ((s= strtok(NULL, delimiters)) != NULL)
    start= strtol(s, NULL, 0);
  else
    start= disass_last_stop;
  if ((s= strtok(NULL, delimiters)) != NULL)
    offset= strtol(s, NULL, 0);
  if ((s= strtok(NULL, delimiters)) != NULL)
    lines= strtol(s, NULL, 0);

  realstart= start;
  while (!uc->inst_at(realstart))
    realstart= (realstart+1) & (EROM_SIZE-1);
  if (offset)
    {
      dir= (offset < 0)?-1:+1;
      while (offset)
	{
	  realstart= (realstart+dir) & (EROM_SIZE-1);
	  while (!uc->inst_at(realstart))
	    realstart= (realstart+dir) & (EROM_SIZE-1);
	  offset+= -dir;
	}
    }
  
  while (lines)
    {
      uc->print_disass(realstart, sim->cmd->actual_console);
      realstart= (realstart+1) & (EROM_SIZE-1);
      while (!uc->inst_at(realstart))
	realstart= (realstart+1) & (EROM_SIZE-1);
      lines--;
    }

  disass_last_stop= realstart;
  return(DD_FALSE);
}


/*
 * DUMP PORTS
 */
//FIXME
bool
cmd_dump_port(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  uchar data;

  data= uc->get_mem(MEM_SFR, P0);
  sim->cmd->printf("P0    ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->get_mem(MEM_SFR, P1);
  sim->cmd->printf("    P1    ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  data= uc->port_pins[0];
  sim->cmd->printf("Pin0  ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->port_pins[1];
  sim->cmd->printf("    Pin1  ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  data= uc->port_pins[0] & uc->get_mem(MEM_SFR, P0);
  sim->cmd->printf("Port0 ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->port_pins[1] & uc->get_mem(MEM_SFR, P1);
  sim->cmd->printf("    Port1 ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  sim->cmd->printf("\n");

  data= uc->get_mem(MEM_SFR, P2);
  sim->cmd->printf("P2    ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->get_mem(MEM_SFR, P3);
  sim->cmd->printf("    P3    ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  data= uc->port_pins[2];
  sim->cmd->printf("Pin2  ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->port_pins[3];
  sim->cmd->printf("    Pin3  ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  data= uc->port_pins[2] & uc->get_mem(MEM_SFR, P2);
  sim->cmd->printf("Port2 ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c", data, data, isprint(data)?data:'.');

  data= uc->port_pins[3] & uc->get_mem(MEM_SFR, P3);
  sim->cmd->printf("    Port3 ");
  print_bin(data, 8, sim->cmd_out());
  sim->cmd->printf(" 0x%02x %3d %c\n", data, data, isprint(data)?data:'.');

  return(DD_FALSE);
}


/*
 * DUMP SFR [addr...]
 */
//FIXME
bool
cmd_dump_sfr(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s;
  t_addr start= 0x80;
  uchar data;
  struct name_entry *ne;

  if ((s= strtok(NULL, delimiters)) != NULL)
    while (s != NULL)
      {
	if ((ne= get_name_entry(uc->sfr_tbl(), s, uc)) != NULL)
	  {
	    data= uc->get_mem(MEM_SFR, ne->addr);
	    sim->cmd->printf("%6s %02x %3d %c\n", ne->name, data, data,
			     isprint(data)?data:'.');
	  }
	else
	  {
	    start= strtol(s, NULL, 0);
	    data = uc->get_mem(MEM_SFR, start);
	    if (start >= SFR_START)
	      sim->cmd->printf("%06x %02x %3d %c\n", start, data, data,
			       isprint(data)?data:'.');
	  }
	s= strtok(NULL, delimiters);
      }
  else
    // dump all
    dump_memory(uc->mem(MEM_SFR), &start, 255, 16, sim->cmd_out(), sim);
  return(DD_FALSE);
}


/*
 * DUMP BIT addr...
 */
//FIXME
bool
cmd_dump_bit(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s, *sym;
  uchar *cell, addr;
  uchar bitaddr, bitmask;

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Address has not given.\n");
      return(DD_FALSE);
    }
  while (s)
    {
      if (!interpret_bitname(s, uc, &cell, &addr, &bitaddr, &bitmask, &sym))
	{
	  sim->cmd->printf("Bad address %s\n", s);
	  return(DD_FALSE);
	}
      sim->cmd->printf("%06x %6s %c\n", addr, sym, (*cell & bitmask)?'1':'0');
      free(sym);
      s= strtok(NULL, delimiters);
    }
  return(DD_FALSE);
}


/* End of s51.src/dump.cc */
