/*
 * Simulator of microcontrollers (cmd.src/cmduc.cc)
 *
 * Copyright (C) 2001,01 Drotos Daniel, Talker Bt.
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

#include <ctype.h>
#include <string.h>

// prj
#include "globals.h"
#include "utils.h"

// sim.src
//#include "uccl.h"

// local, cmd.src
#include "cmd_uccl.h"


/*
 * Command: state
 *----------------------------------------------------------------------------
 */

//int
//cl_state_cmd::do_work(class cl_sim *sim,
//		      class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_state_cmd)
{
  con->dd_printf("CPU state= %s PC= 0x%06x XTAL= %g\n",
		 get_id_string(cpu_states, uc->state),
		 AU(uc->PC), 
		 uc->xtal);
  con->dd_printf("Operation since last reset= (%lu vclks)\n",
		 (unsigned long)(uc->vc.fetch) +
		 (unsigned long)(uc->vc.rd) +
		 (unsigned long)(uc->vc.wr));
  con->dd_printf("Inst= %lu ", (unsigned long)(uc->vc.inst));
  con->dd_printf("Fetch= %lu ", (unsigned long)(uc->vc.fetch));
  con->dd_printf("Read= %lu ", (unsigned long)(uc->vc.rd));
  con->dd_printf("Write= %lu\n", (unsigned long)(uc->vc.wr));
  con->dd_printf("Total time since last reset= %g sec (%lu clks)\n",
		 uc->get_rtime(), (unsigned long)(uc->ticks->ticks));
  con->dd_printf("Time in isr = %g sec (%lu clks) %3.2g%%\n",
		 uc->isr_ticks->get_rtime(uc->xtal),
		 uc->isr_ticks->ticks,
		 (uc->ticks->ticks == 0)?0.0:
		 (100.0*((double)(uc->isr_ticks->ticks)/
			 (double)(uc->ticks->ticks))));
  con->dd_printf("Time in idle= %g sec (%lu clks) %3.2g%%\n",
		 uc->idle_ticks->get_rtime(uc->xtal),
		 uc->idle_ticks->ticks,
		 (uc->ticks->ticks == 0)?0.0:
		 (100.0*((double)(uc->idle_ticks->ticks)/
			 (double)(uc->ticks->ticks))));
  con->dd_printf("Max value of stack pointer= 0x%06x, avg= 0x%06x\n",
		 AU(uc->sp_max), AU(uc->sp_avg));
  con->dd_printf("Simulation: %s\n",
		 (uc->sim->state & SIM_GO)?"running":"stopped");
  return(0);
}

CMDHELP(cl_state_cmd,
	"state",
	"State of microcontroller",
	"long help of state")

/*
 * Command: file
 *----------------------------------------------------------------------------
 */

//int
//cl_file_cmd::do_work(class cl_sim *sim,
//		     class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_file_cmd)
{
  const char *fname= 0;
  long l;

  if ((cmdline->param(0) == 0) ||
      ((fname= cmdline->param(0)->get_svalue()) == NULL))
    {
      con->dd_printf("File name is missing.\n");
      return(0);
    }

  if ((l= uc->read_file(fname, con)) >= 0)
    {
      //con->dd_printf("%ld words read from %s\n", l, fname);
    }

  return(0);
}

CMDHELP(cl_file_cmd,
	"file \"FILE\"",
        "Load FILE into ROM",
	"long help of file")

/*
 * Command: download
 *----------------------------------------------------------------------------
 */

//int
//cl_dl_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_dl_cmd)
{
  long l;
  
  if ((l= uc->read_hex_file(con)) >= 0)
    con->dd_printf("%ld words loaded\n", l);

  return(0);
}

CMDHELP(cl_dl_cmd,
	"download",
	"Load (intel.hex) data",
	"long help of download")

/*
 * Command: pc
 *----------------------------------------------------------------------------
 */

//int
//cl_pc_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_pc_cmd)
{
  t_addr addr;
  class cl_cmd_arg *params[1]= { cmdline->param(0) };

  if (params[0])
    {
      if (!(params[0]->get_address(uc, &addr)))
	{
	  con->dd_printf("Error: wrong parameter\n");
	  return(false);
	}
      class cl_address_space *rom= uc->rom;
      if (rom)
	{
	  if (addr > rom->highest_valid_address())
	    addr= rom->highest_valid_address();
	}
      if (!uc->inst_at(addr))
	con->dd_printf("Warning: maybe not instruction at 0x%06x\n", AU(addr));
      uc->set_PC(addr);
    }
  uc->print_disass(uc->PC, con);
  return(false);
}

CMDHELP(cl_pc_cmd,
	"pc [addr]",
	"Set/get PC",
	"long help of pc")

/*
 * Command: reset
 *----------------------------------------------------------------------------
 */

//int
//cl_reset_cmd::do_work(class cl_sim *sim,
//		      class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_reset_cmd)
{
  uc->reset();
  return(0);
}

CMDHELP(cl_reset_cmd,
	"reset",
	"Reset processor to start state",
	"long help of reset")

/*
 * Command: tick
 *----------------------------------------------------------------------------
 */

COMMAND_DO_WORK_UC(cl_tick_cmd)
{
  class cl_cmd_arg *params[1];
  params[0]= cmdline->param(0);
  int cycles= 1;

  if (params[0] != NULL)
    cycles= params[0]->i_value;

  uc->tick(cycles);

  return 0;
}

CMDHELP(cl_tick_cmd,
	"tick [number]",
	"Tick a given number of clock cycles without execution",
	"Ticks a given number of clock cycles. Timers and HW update but no"
	" execution takes place.")


/*
 * Command: dump
 *----------------------------------------------------------------------------
 */

//int
//cl_dump_cmd::do_work(class cl_sim *sim,
//		     class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_dump_cmd)
{
  class cl_memory *mem= uc->rom;
  t_addr start = -1, end = -1;
  long bpl= -1;

  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };
  char fmt = 0;

  if (params[0] &&
      params[0]->as_string())
    {
      char *s= params[0]->get_svalue();
      if (s && *s && s[0] == '/')
        {
          for (size_t i = 1; i < strlen(s); i++)
            {
              char c = tolower(s[i]);
              switch (c)
                {
		case 'b':
		  if (con->get_fout() && con->get_fout()->tty)
		    {
		      con->dd_printf("Error: binary format not supported on tty\n");
		      return false;
		    }
		  break;
		case 'x':
		case 'h': // hex
		case 'i': // ihex
		case 's': // string
		  break;
		default:
		  con->dd_printf("Error: unknown format option '%c'\n", c);
		  return false;
                }
              fmt = c;
            }
          cmdline->params->free_at(0);
          params[0]= cmdline->param(0);
          params[1]= cmdline->param(1);
          params[2]= cmdline->param(2);
          params[3]= cmdline->param(3);
        }
    }

  if (params[0] &&
      params[0]->as_bit(uc) &&
      params[0]->value.bit.bitnr_low >= 0)
    {
      int i= 0;
      while (params[0] &&
	     params[0]->as_bit(uc))
	{
          if (!fmt)
	    params[0]->value.bit.mem->dump(params[0]->value.bit.mem_address,
                                           params[0]->value.bit.bitnr_high,
                                           params[0]->value.bit.bitnr_low,
                                           con);
          else
            con->dd_printf("Format options may not be specified for bits\n");

	  i++;
	  params[0]= cmdline->param(i);
	}
      if (params[0])
	syntax_error(con);
      return false;
    }

  if (params[0] == 0)
    ;
  else if (cmdline->syntax_match(uc, BIT)) {
    mem= params[0]->value.bit.mem;
    start= params[0]->value.bit.mem_address;
  }
  else if (cmdline->syntax_match(uc, BIT BIT)) {
    mem= params[0]->value.bit.mem;
    if (mem != params[1]->value.bit.mem) {
      con->dd_printf("Start and end must be in the same address space\n");
      return false;
    }
    start= params[0]->value.bit.mem_address;
    end= params[1]->value.bit.mem_address;
  }
  else if (cmdline->syntax_match(uc, BIT BIT NUMBER)) {
    mem= params[0]->value.bit.mem;
    if (mem != params[1]->value.bit.mem) {
      con->dd_printf("Start and end must be in the same address space\n");
      return false;
    }
    start= params[0]->value.bit.mem_address;
    end= params[1]->value.bit.mem_address;
    bpl  = params[2]->value.number;
  }
  else if (cmdline->syntax_match(uc, MEMORY)) {
      mem= params[0]->value.memory.memory;
  }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS)) {
    mem  = params[0]->value.memory.memory;
    start= params[1]->value.address;
  }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS ADDRESS)) {
    mem  = params[0]->value.memory.memory;
    start= params[1]->value.address;
    end  = params[2]->value.address;
  }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS ADDRESS NUMBER)) {
    mem  = params[0]->value.memory.memory;
    start= params[1]->value.address;
    end  = params[2]->value.address;
    bpl  = params[3]->value.number;
  }
  else {
    syntax_error(con);
    return false;
  }

  switch (fmt)
    {
    case 0: // default
      mem->dump(1, start, end, bpl, con);
      break;
    case 'b': // binary
      mem->dump_b(start, end, bpl, con);
      break;
    case 'h': case 'x':// hex
      mem->dump(0, start, end, bpl, con);
      break;
    case 'i': // ihex
      mem->dump_i(start, end, 32, con);
      break;
    case 's': // string
      mem->dump_s(start, end, bpl, con);
      break;
    }

  return(false);;
}

CMDHELP(cl_dump_cmd,
	"dump [/format] memory_type [start [stop [bytes_per_line]]] | dump bit...",
	"Dump memory of specified type or bit(s)",
	"long help of dump")

/*
 * Command: di
 *----------------------------------------------------------------------------
 */

//int
//cl_di_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_di_cmd)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("/h"));
  cmdline->insert_param(1, new cl_cmd_sym_arg("iram"));
  cl_dump_cmd::do_work(uc, cmdline, con);
  return(0);
}

CMDHELP(cl_di_cmd,
	"di [start [stop]]",
	"Dump Internal RAM",
	"long help of di")

/*
 * Command: dx
 *----------------------------------------------------------------------------
 */

//int
//cl_dx_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_dx_cmd)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("/h"));
  cmdline->insert_param(1, new cl_cmd_sym_arg("xram"));
  cl_dump_cmd::do_work(uc, cmdline, con);
  return(0);
}

CMDHELP(cl_dx_cmd,
	"dx [start [stop]]",
	"Dump External RAM",
	"long help of dx")

/*
 * Command: dch
 *----------------------------------------------------------------------------
 */

//int
//cl_dch_cmd::do_work(class cl_sim *sim,
//		    class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_dch_cmd)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("/h"));
  cmdline->insert_param(1, new cl_cmd_sym_arg(/*"rom"*/uc->rom->get_name("rom")));
  cl_dump_cmd::do_work(uc, cmdline, con);
  return(0);
}

CMDHELP(cl_dch_cmd,
	"dch [start [stop]]",
	"Dump code in hex form",
	"long help of dch")

/*
 * Command: ds
 *----------------------------------------------------------------------------
 */

//int
//cl_ds_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_ds_cmd)
{
  cmdline->insert_param(0, new cl_cmd_sym_arg("/h"));
  cmdline->insert_param(1, new cl_cmd_sym_arg("sfr"));
  cl_dump_cmd::do_work(uc, cmdline, con);
  return(0);
}

CMDHELP(cl_ds_cmd,
	"ds [start [stop]]",
	"Dump SFR",
	"long help of ds")

/*
 * Command: dc
 *----------------------------------------------------------------------------
 */

//int
//cl_dc_cmd::do_work(class cl_sim *sim,
//		   class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_dc_cmd)
{
  t_addr start= last, end= last+20;
  class cl_cmd_arg *params[2]= { cmdline->param(0),
				 cmdline->param(1) };
  class cl_address_space *rom= uc->rom;

  if (!rom)
    return(false);
  if (params[0] == 0)
    ;
  else if (cmdline->syntax_match(uc, ADDRESS)) {
    start= params[0]->value.address;
    end= start+20;
  }
  else if (cmdline->syntax_match(uc, ADDRESS ADDRESS)) {
    start= params[0]->value.address;
    end= params[1]->value.address;
  }
  if (start > rom->highest_valid_address())
    {
      con->dd_printf("Error: start address is too high\n");
      return(false);
    }
  if (end > rom->highest_valid_address())
    {
      con->dd_printf("Error: end address is too high\n");
      return(false);
    }

  for (;
       start <= end;
       start+= uc->inst_length(start))
    uc->print_disass(start, con);
  last= start;
  return(false);
}

CMDHELP(cl_dc_cmd,
	"dc [start [stop]]",
	"Dump code in disass form",
	"long help of dc")

/*
 * Command: disassemble
 *----------------------------------------------------------------------------
 */

static int disass_last_stop= 0;

//int
//cl_disassemble_cmd::do_work(class cl_sim *sim,
//			    class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_disassemble_cmd)
{
  t_addr start, realstart;
  int offset= -1, dir, lines= 20;
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };

  start= disass_last_stop;
  if (params[0] == 0) ;
  else if (cmdline->syntax_match(uc, ADDRESS)) {
    start= params[0]->value.address;
  }
  else if (cmdline->syntax_match(uc, ADDRESS NUMBER)) {
    start= params[0]->value.address;
    offset= params[1]->value.number;
  }
  else if (cmdline->syntax_match(uc, ADDRESS NUMBER NUMBER)) {
    start= params[0]->value.address;
    offset= params[1]->value.number;
    lines= params[2]->value.number;
  }
  else
    {
      syntax_error(con);
      return(false);
    }

  if (lines < 1)
    {
      con->dd_printf("Error: wrong `lines' parameter\n");
      return(false);
    }
  if (!uc->there_is_inst())
    return(false);
  realstart= start;
  class cl_address_space *rom= uc->rom;
  if (!rom)
    return(false);
  while (realstart <= rom->highest_valid_address() &&
	 !uc->inst_at(realstart))
    realstart= realstart+1;
  if (offset)
    {
      dir= (offset < 0)?-1:+1;
      while (offset)
	{
	  realstart= rom->inc_address(realstart, dir);
	  while (!uc->inst_at(realstart))
	    realstart= rom->inc_address(realstart, dir);
	  offset+= -dir;
	}
    }
  
  
  while (lines)
    {
      int len;
      uc->print_disass(realstart, con);
      /* fix for #2383: start search next instruction after the actual one */
      len= uc->inst_length(realstart);
      realstart= rom->inc_address(realstart, /*+1*/len) + rom->start_address;
      while (!uc->inst_at(realstart))
        realstart= rom->inc_address(realstart, +1) + rom->start_address;
      lines--;
    }

  disass_last_stop= realstart;

  return(false);;
}

CMDHELP(cl_disassemble_cmd,
	"disassemble [start [offset [lines]]]",
	"Disassemble code",
	"long help of disassemble")

/*
 * Command: fill
 *----------------------------------------------------------------------------
 */

//int
//cl_fill_cmd::do_work(class cl_sim *sim,
//		     class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_fill_cmd)
{
  class cl_memory *mem= 0;
  t_mem what= 0;
  t_addr start= 0, end;
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3) };

  if (cmdline->syntax_match(uc, MEMORY ADDRESS ADDRESS NUMBER)) {
    mem  = params[0]->value.memory.memory;
    start= params[1]->value.address;
    end  = params[2]->value.address;
    what = params[3]->value.number;
    t_addr i;
    for (i= start; i <= end; i++)
      {
	t_mem d;
	d= what;
	mem->write(i, d);
      }
  }
  else
    syntax_error(con);

  return(false);;
}

CMDHELP(cl_fill_cmd,
	"fill memory_type start end data",
	"Fill memory region with data",
	"long help of fill")

/*
 * Command: where
 *----------------------------------------------------------------------------
 */

int
cl_where_cmd::do_real_work(class cl_uc *uc,
			   class cl_cmdline *cmdline, class cl_console_base *con,
			   bool case_sensitive)
{
  class cl_memory *mem= 0;
  class cl_cmd_arg *params[2]= { cmdline->param(0),
				 cmdline->param(1) };

  if (cmdline->syntax_match(uc, MEMORY DATALIST)) {
    mem= params[0]->value.memory.memory;
    t_mem *array= params[1]->value.data_list.array;
    int len= params[1]->value.data_list.len;
    if (!len)
      {
	con->dd_printf("Error: nothing to search for\n");
	return(false);
      }
    t_addr addr= 0;
    bool found= mem->search_next(case_sensitive, array, len, &addr);
    while (found)
      {
	if (con->get_fout())
	  mem->dump(0, addr, addr+len-1, -1, con);
	addr++;
	found= mem->search_next(case_sensitive, array, len, &addr);
      }
  }
  else
    syntax_error(con);

  return(false);
}

//int
//cl_where_cmd::do_work(class cl_sim *sim,
//		      class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_where_cmd)
{
  return(do_real_work(uc, cmdline, con, false));
}

CMDHELP(cl_where_cmd,
	"where memory_type data...",
	"Case unsensitive search for data",
	"long help of where")

//int
//cl_Where_cmd::do_work(class cl_sim *sim,
//		      class cl_cmdline *cmdline, class cl_console *con)
COMMAND_DO_WORK_UC(cl_Where_cmd)
{
  return(do_real_work(uc, cmdline, con, true));
}

CMDHELP(cl_Where_cmd,
	"Where memory_type data...",
	"Case sensitive search for data",
	"long help of Where")


/*
 * Command: hole
 *----------------------------------------------------------------------------
 */

COMMAND_DO_WORK_UC(cl_hole_cmd)
{
  class cl_cmd_arg *params[4]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2) };
  class cl_memory *m= uc->rom;
  
  if (m)
    {
      t_mem v, a;
      if (cmdline->syntax_match(uc, MEMORY NUMBER NUMBER))
	{
	  m= params[0]->value.memory.memory;
	  a= params[1]->value.number;
	  v= params[2]->value.number;
	}
      else if (cmdline->syntax_match(uc, MEMORY NUMBER))
	{
	  m= params[0]->value.memory.memory;
	  a= params[1]->value.number;
	  v= 0;
	}
      else if (cmdline->syntax_match(uc, MEMORY))
	{
	  m= params[0]->value.memory.memory;
	  a= 100;
	  v= 0;
	}
      else if (cmdline->syntax_match(uc, NUMBER NUMBER))
	{
	  a= params[0]->value.number;
	  v= params[1]->value.number;
	}
      else if (cmdline->syntax_match(uc, NUMBER))
	{
	  a= params[0]->value.number;
	  v= 0;
	}
      else
	{
	  a= 100;
	  v= 0;
	}
      t_addr ad, l, h, sa= 0, len= 0;
      t_mem mv;
      bool in= false;
      l= m->lowest_valid_address();
      h= m->highest_valid_address();
      //con->dd_printf("%s[0x%x-0x%0x] len=%d val=%d\n", m->get_name("mem"),
      //	     l, h, a, v);
      for (ad= l; ad <= h; ad++)
	{
	  mv= m->read(ad);
	  if (!in && (mv==v))
	    {
	      // found start
	      sa= ad;
	      in= true;
	      len= 0;
	    }
	  else if (in && (mv==v))
	    {
	      // still inside
	      len++;
	    }
	  else if (in && (mv!=v))
	    {
	      // found end
	      if (len >= a)
		{
		  con->dd_printf(m->addr_format, sa);
		  con->dd_printf(" %u\n", AU(len));
		}
	      in= false;
	    }
	}
      if (in &&
	  len >= a)
	{
	  // found end after highest reached
	  con->dd_printf(m->addr_format, sa);
	  con->dd_printf(" %u\n", AU(len));
	}
    }
  return false;
}

CMDHELP(cl_hole_cmd,
	"hole [memory [length [value]]]",
	"search area in memory (min length), filled with value",
	"long help of hole")


/*
 * Command: var
 *----------------------------------------------------------------------------
 */

COMMAND_DO_WORK_UC(cl_var_cmd)
{
  class cl_cmd_arg *params[5]= { cmdline->param(0),
				 cmdline->param(1),
				 cmdline->param(2),
				 cmdline->param(3),
				 cmdline->param(4) };
  class cl_memory *m= NULL;
  t_addr addr= -1;
  int bitnr_low= -1;
  int bitnr_high= -1;

  if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS NUMBER NUMBER))
    {
      m= params[1]->value.memory.memory;
      addr= params[2]->value.address;
      bitnr_low= bitnr_high= params[3]->value.number;
      bitnr_high= params[4]->value.number;
    }
  else if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS NUMBER))
    {
      m= params[1]->value.memory.memory;
      addr= params[2]->value.address;
      bitnr_low= bitnr_high= params[3]->value.number;
    }
  else if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS))
    {
      m= params[1]->value.memory.memory;
      addr= params[2]->value.address;
    }
  else if (cmdline->syntax_match(uc, STRING BIT))
    {
      m= params[1]->value.bit.mem;
      addr= params[1]->value.bit.mem_address;
      bitnr_low= params[1]->value.bit.bitnr_low;
      bitnr_high= params[1]->value.bit.bitnr_high;
    }
  else if (cmdline->syntax_match(uc, STRING CELL))
    {
      m= uc->address_space(params[1]->value.cell, &addr);
    }
  else if (cmdline->syntax_match(uc, STRING))
    {
    }
  else
    return syntax_error(con), false;

  if (!valid_sym_name(params[0]->value.string.string))
    return con->dd_printf("name is invalid\n"),
      false;
  
  if (m)
    if (!m->is_address_space())
      return con->dd_printf("%s is not address space\n", m->get_name()),
	false;
  if (addr > 0)
    if (!m->valid_address(addr))
      return con->dd_printf("invalid address\n"),
	false;
  if (bitnr_low >= (int)sizeof(t_mem)*8 ||
      bitnr_high >= (int)sizeof(t_mem)*8)
    return con->dd_printf("max bit number is %d\n", (int)sizeof(t_mem)*8),
      false;

  if (m)
    uc->vars->add(params[0]->value.string.string, m, addr, bitnr_high, bitnr_low, "");
  else
    {
      if (bitnr_low < 0)
	{
	  if (addr < 0)
	    {
	      t_index i;
	      for (addr= 0; addr < uc->variables->get_size(); addr++)
		if (!uc->vars->by_addr.search(uc->variables, addr, i))
		  break;
	      if (addr == uc->variables->get_size())
		return con->dd_printf("no space\n"),
		  false;
	    }
	  if (!uc->variables->valid_address(addr))
	    return con->dd_printf("out of range\n"),
	      false;
          uc->vars->add(params[0]->value.string.string,
                        uc->variables, addr, bitnr_high, bitnr_low, "");
	}
      else
	{
	}
    }
  
  return false;
}

CMDHELP(cl_var_cmd,
	"var name [memory addr [bit_nr]]",
	"Create new variable",
	"long help of var")

/*
 * Command: rmvar
 *----------------------------------------------------------------------------
 */

COMMAND_DO_WORK_UC(cl_rmvar_cmd)
{
  if (cmdline->syntax_match(uc, STRING))
    uc->vars->del(cmdline->param(0)->value.string.string);
  else
    return syntax_error(con), false;

  return false;
}

CMDHELP(cl_rmvar_cmd,
	"rmvar name",
	"Remove variable",
	"Deletes the name variable")

/* End of cmd.src/cmd_uc.cc */
