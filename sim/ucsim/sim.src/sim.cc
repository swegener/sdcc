/*
 * Simulator of microcontrollers (sim.cc)
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
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif
#include "i_string.h"

// prj
#include "globals.h"

// cmd
#include "cmdsetcl.h"
#include "infocl.h"
#include "setcl.h"
#include "getcl.h"
#include "showcl.h"
#include "bpcl.h"
#include "cmdguicl.h"
#include "cmdconfcl.h"

// local
#include "simcl.h"


/*
 * Simulator
 */

cl_sim::cl_sim(char *more_args, int iargc, char *iargv[]):
  cl_base()
{
  argc= iargc; argv= iargv;
  uc= 0;
  cmd= 0;
  arguments= new cl_list(2, 2);
  accept_args= more_args?strdup(more_args):0;
  in_files= new cl_ustrings(2, 2);
  gui= new cl_gui(this);
}

int
cl_sim::init(void)
{
  int i;

  cl_base::init();
  proc_arguments(argc, argv);
  class cl_cmdset *cmdset= new cl_cmdset(this);
  cmdset->init();
  build_cmd_set(cmdset);
  if (!(uc= mk_controller()))
    return(1);
  uc->init();
  cmd= new cl_commander(cmdset, this);
  cmd->init();
  char *Config= get_sarg(0, "Config");
  if (Config)
    {
      class cl_console *con= cmd->mk_console(Config, 0/*"/dev/tty"*/, this);
      cmd->add_console(con);
    }
  if (cmd->cons->get_count() == 0)
    {
      fprintf(stderr, "No command console available.\n");
      exit(1);
    }
  for (i= 0; i < in_files->count; i++)
    {
      char *fname= (char *)(in_files->at(i));
      long l;
      if ((l= uc->read_hex_file(fname)) >= 0)
	{
	  cmd->all_printf("%ld words read from %s\n", l, fname);
	  fprintf(stderr, "%ld words read from %s\n", l, fname);
	}
      char *prompt;
      if (arg_avail('P'))
	cmd->all_print("\0", 1);
      else
	cmd->all_printf("%s", (prompt= get_sarg(0, "prompt"))?prompt:"> ") ;
    }
  return(0);
}

cl_sim::~cl_sim(void)
{
  if (uc)
    delete uc;
}

int
cl_sim::proc_arguments(int argc, char *argv[])
{
  int i, c;
  char *opts, *cp;

  opts= (char*)malloc((accept_args?strlen(accept_args):0)+100);
  strcpy(opts, "c:C:p:PX:vV");
#ifdef SOCKET_AVAIL
  strcat(opts, "Z:r:");
#endif
  if (accept_args)
    strcat(opts, accept_args);
  opterr= 0;
  while((c= getopt(argc, argv, opts)) != -1)
    switch (c)
      {

      case 'c':
	arguments->add(new cl_prg_arg('c', 0, optarg));
	break;

      case 'C':
	arguments->add(new cl_prg_arg(0, "Config", optarg));
	break;

#ifdef SOCKET_AVAIL
      case 'Z':
	// By Sandeep
	arguments->add(new cl_prg_arg('Z', 0, (long)1));
	if (!optarg || !isdigit(*optarg))
	  fprintf(stderr, "expected portnumber to follow -Z\n");
	else {
	  char *p;
	  long l= strtol(optarg, &p, 0);
	  arguments->add(new cl_prg_arg(0, "Zport", l));
	}
	break;
#endif

      case 'p':
	arguments->add(new cl_prg_arg(0, "prompt", optarg));
	break;

      case 'P':
	arguments->add(new cl_prg_arg('P', 0, (long)1));
	break;

#ifdef SOCKET_AVAIL
      case 'r':
	arguments->add(new cl_prg_arg('r', 0,
				      (long)strtol(optarg, NULL, 0)));
	break;
#endif

      case 'X':
	{
	  double XTAL;
	  for (cp= optarg; *cp; *cp= toupper(*cp), cp++);
	  XTAL= strtod(optarg, &cp);
	  if (*cp == 'K')
	    XTAL*= 1e3;
	  if (*cp == 'M')
	    XTAL*= 1e6;
	  if (XTAL == 0)
	    {
	      fprintf(stderr, "Xtal frequency must be greather than 0\n");
	      exit(1);
	    }
	  arguments->add(new cl_prg_arg('X', 0, XTAL));
	  break;
	}

      case 'v':
	printf("%s: %s\n", argv[0], VERSIONSTR);
        exit(0);
        break;

      case 'V':
	arguments->add(new cl_prg_arg('V', 0, (long)1));
	break;

      case '?':
	if ((c= proc_arg(c, optarg)))
	  exit(c);
        break;

      default:
        if ((c= proc_arg(c, optarg)))
	  exit(c);
      }
  if (!arg_avail("prompt"))
    arguments->add(new cl_prg_arg(0, "prompt", "> "));

  for (i= optind; i < argc; i++)
    in_files->add(argv[i]);

  free(opts);
  return(0);
}

int
cl_sim::proc_arg(char arg, char *optarg)
{
  return(0);
}

int
cl_sim::arg_avail(char name)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if (a->short_name == name)
	return(1);
    }
  return(0);
}

int
cl_sim::arg_avail(char *name)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if (a->long_name &&
	  strcmp(a->long_name, name) == 0)
	return(1);
    }
  return(0);
}

long long
cl_sim::get_iarg(char sname, char *lname)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if ((sname && a->short_name == sname) ||
	  (lname && a->long_name && strcmp(a->long_name, lname) == 0))
	{
	  long iv;
	  if (a->get_ivalue(&iv))
	    return(iv);
	  else
	    //FIXME
	    return(0);
	}
    }
  return(0);
}

char *
cl_sim::get_sarg(char sname, char *lname)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if ((sname && a->short_name == sname) ||
	  (lname && a->long_name && strcmp(a->long_name, lname) == 0))
	return(a->get_svalue());
    }
  return(0);
}


double
cl_sim::get_farg(char sname, char *lname)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if ((sname && a->short_name == sname) ||
	  (lname && a->long_name && strcmp(a->long_name, lname) == 0))
	return(a->get_fvalue());
    }
  return(0);
}

void *
cl_sim::get_parg(char sname, char *lname)
{
  class cl_prg_arg *a;
  int i;

  for (i= 0; i < arguments->count; i++)
    {
      a= (class cl_prg_arg *)(arguments->at(i));
      if ((sname && a->short_name == sname) ||
	  (lname && a->long_name && strcmp(a->long_name, lname) == 0))
	return(a->get_pvalue());
    }
  return(0);
}

class cl_uc *
cl_sim::mk_controller(void)
{
  return(new cl_uc(this));
}

class cl_cmd_arg *
cl_sim::mk_cmd_int_arg(long long i)
{
  class cl_cmd_arg *arg= new cl_cmd_int_arg(uc, i);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_sym_arg(char *s)
{
  class cl_cmd_arg *arg= new cl_cmd_sym_arg(uc, s);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_str_arg(char *s)
{
  class cl_cmd_arg *arg= new cl_cmd_str_arg(uc, s);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_bit_arg(class cl_cmd_arg *sfr, class cl_cmd_arg *bit)
{
  class cl_cmd_arg *arg= new cl_cmd_bit_arg(uc, sfr, bit);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_array_arg(class cl_cmd_arg *aname, class cl_cmd_arg *aindex)
{
  class cl_cmd_arg *arg= new cl_cmd_array_arg(uc, aname, aindex);
  arg->init();
  return(arg);
}


/*
 * Main cycle of the simulator
 */

int
cl_sim::main(void)
{
  int done= 0;

  while (!done &&
	 (state & SIM_QUIT) == 0)
    {
      if (state & SIM_GO)
	{
	  uc->do_inst(-1);
	  if (cmd->input_avail())
	    {
	      done= cmd->proc_input();
	    }
	}
      else
	{
	  cmd->wait_input();
	  done= cmd->proc_input();
	}
    }
  return(0);
}

/*int
cl_sim::do_cmd(char *cmdstr, class cl_console *console)
{
  class cl_cmdline *cmdline;
  class cl_cmd *cm;
  int retval= 0;

  cmdline= new cl_cmdline(cmdstr, console);
  cmdline->init();
  cm= cmd->cmdset->get_cmd(cmdline);
  if (cm)
    retval= cm->work(cmdline, console);
  delete cmdline;
  if (cm)
    return(retval);
  return(console->interpret(cmdstr));
}*/

void
cl_sim::start(class cl_console *con)
{
  state|= SIM_GO;
  con->flags|= CONS_FROZEN;
  cmd->frozen_console= con;
  cmd->set_fd_set();
}

void
cl_sim::stop(int reason)
{
  state&= ~SIM_GO;
  if (cmd->frozen_console)
    {
      if (reason == resUSER &&
	  cmd->frozen_console->input_avail())
	cmd->frozen_console->read_line();
      cmd->frozen_console->printf("Stop at 0x%06x: (%d) ", uc->PC, reason);
      switch (reason)
	{
	case resHALT:
	  cmd->frozen_console->printf("Halted\n");
	  break;
	case resINV_ADDR:
	  cmd->frozen_console->printf("Invalid address\n");
	  break;
	case resSTACK_OV:
	  cmd->frozen_console->printf("Stack overflow\n");
	  break;
	case resBREAKPOINT:
	  cmd->frozen_console->printf("Breakpoint\n");
	  break;
	case resINTERRUPT:
	  cmd->frozen_console->printf("Interrupt\n");
	  break;
	case resWDTRESET:
	  cmd->frozen_console->printf("Watchdog reset\n");
	  break;
	case resUSER:
	  cmd->frozen_console->printf("User stopped\n");
	  break;
	case resINV_INST:
	  cmd->frozen_console->printf("Invalid instruction 0x%04x\n",
				      uc->get_mem(MEM_ROM, uc->PC));
	  break;
	default:
	  cmd->frozen_console->printf("Unknown reason\n");
	  break;
	}
      cmd->frozen_console->printf("F 0x%06x\n", uc->PC); // for sdcdb
      //if (cmd->actual_console != cmd->frozen_console)
      cmd->frozen_console->flags&= ~CONS_FROZEN;
      cmd->frozen_console->print_prompt();
      cmd->frozen_console= 0;
    }
  cmd->set_fd_set();
}


/*
 */

void
cl_sim::build_cmd_set(class cl_cmdset *cmdset)
{
  class cl_cmd *cmd;
  class cl_cmdset *cset;

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_conf_cmd("_no_parameters_", 0,
"conf               Configuration",
"long help of conf"));
    cmd->init();
    cset->add(cmd= new cl_conf_addmem_cmd("addmem", 0,
"conf addmem\n"
"                   Make memory",
"long help of conf addmem"));
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("conf", 0,
"conf subcommand    Information, see `conf' command for more help",
"long help of conf", cset));
  cmd->init();

  cmdset->add(cmd= new cl_state_cmd("state", 0,
"state              State of simulator",
"long help of state"));
  cmd->init();

  cmdset->add(cmd= new cl_file_cmd("file", 0,
"file \"FILE\"        Load FILE into ROM",
"long help of file"));
  cmd->init();
  cmd->add_name("load");

  cmdset->add(cmd= new cl_dl_cmd("download", 0,
"download,dl          Load (intel.hex) data",
"long help of download"));
  cmd->init();
  cmd->add_name("dl");

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_info_bp_cmd("breakpoints", 0, 
"info breakpoints   Status of user-settable breakpoints",
"long help of info breakpoints"));
    cmd->add_name("bp");
    cmd->init();
    cset->add(cmd= new cl_info_reg_cmd("registers", 0, 
"info registers     List of integer registers and their contents",
"long help of info registers"));
    cmd->init();
    cset->add(cmd= new cl_info_hw_cmd("hardware", 0, 
"info hardware cathegory\n"
"                   Status of hardware elements of the CPU",
"long help of info hardware"));
    cmd->add_name("h	w");
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("info", 0,
"info subcommand    Information, see `info' command for more help",
"long help of info", cset));
  cmd->init();

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_get_sfr_cmd("sfr", 0,
"get sfr address...\n"
"                   Get value of addressed SFRs",
"long help of get sfr"));
    cmd->init();
    cset->add(cmd= new cl_get_option_cmd("option", 0,
"get option name\n"
"                   Get value of an option",
"long help of get option"));
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("get", 0,
"get subcommand     Get, see `get' command for more help",
"long help of get", cset));
  cmd->init();

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_set_mem_cmd("memory", 0,
"set memory memory_type address data...\n"
"                   Place list of data into memory",
"long help of set memory"));
    cmd->init();
    cset->add(cmd= new cl_set_bit_cmd("bit", 0,
"set bit addr 0|1   Set specified bit to 0 or 1",
"long help of set bit"));
    cmd->init();
    cset->add(cmd= new cl_set_port_cmd("port", 0,
"set port hw data   Set data connected to port",
"long help of set port"));
    cmd->init();
    cset->add(cmd= new cl_set_option_cmd("option", 0,
"set option name value\n"
"                   Set value of an option",
"long help of set option"));
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("set", 0,
"set subcommand     Set, see `set' command for more help",
"long help of set", cset));
  cmd->init();

  cmdset->add(cmd= new cl_timer_cmd("timer", 0,
"timer a|d|g|r|s|v id [direction|value]\n"
"                   Timer add|del|get|run|stop|value",
"timer add|create|make id [direction] -- create a new timer\n"
"timer del id -- delete a timer\n"
"timer get id -- list timers\n"
"timer run id -- turn a timer ON\n"
"timer stop id -- turn a timer OFF\n"
"timer value id val -- set value of a timer to `val'"));
  cmd->init();

  cmdset->add(cmd= new cl_run_cmd("run", 0,
"run [start [stop]] Go",
"long help of run"));
  cmd->init();
  cmd->add_name("go");
  cmd->add_name("r");

  cmdset->add(cmd= new cl_stop_cmd("stop", 0,
"stop               Stop",
"long help of stop"));
  cmd->init();

  cmdset->add(cmd= new cl_step_cmd("step", 0,
"step               Step",
"long help of step"));
  cmd->init();
  cmd->add_name("s");

  cmdset->add(cmd= new cl_next_cmd("next", 0,
"next               Next",
"long help of next"));
  cmd->init();
  cmd->add_name("n");

  cmdset->add(cmd= new cl_pc_cmd("pc", 0,
"pc [addr]          Set/get PC",
"long help of pc"));
  cmd->init();

  cmdset->add(cmd= new cl_reset_cmd("reset", 0,
"reset              Reset",
"long help of reset"));
  cmd->init();

  cmdset->add(cmd= new cl_dump_cmd("dump", 0,
"dump memory_type [start [stop [bytes_per_line]]]\n"
"                   Dump memory of specified type\n"
"dump bit...        Dump bits",
"long help of dump"));
  cmd->init();

  cmdset->add(cmd= new cl_di_cmd("di", 0,
"di [start [stop]]  Dump Internal RAM",
"long help of di"));
  cmd->init();

  cmdset->add(cmd= new cl_dx_cmd("dx", 0,
"dx [start [stop]]  Dump External RAM",
"long help of dx"));
  cmd->init();

  cmdset->add(cmd= new cl_ds_cmd("ds", 0,
"ds [start [stop]]  Dump SFR",
"long help of ds"));
  cmd->init();

  cmdset->add(cmd= new cl_dch_cmd("dch", 0,
"dch [start [stop]] Dump code in hex form",
"long help of dch"));
  cmd->init();

  cmdset->add(cmd= new cl_dc_cmd("dc", 0,
"dc [start [stop]]  Dump code in disass form",
"long help of dc"));
  cmd->init();

  cmdset->add(cmd= new cl_disassemble_cmd("disassemble", 0,
"disassemble [start [offset [lines]]]\n"
"                   Disassemble code",
"long help of disassemble"));
  cmd->init();

  cmdset->add(cmd= new cl_fill_cmd("fill", 0,
"fill memory_type start end data\n"
"                   Fill memory region with data",
"long help of fill"));
  cmd->init();

  cmdset->add(cmd= new cl_where_cmd("where", 0,
"where memory_type data...\n"
"                   Case unsensitive search for data",
"long help of where"));
  cmd->init();

  cmdset->add(cmd= new cl_Where_cmd("Where", 0,
"Where memory_type data...\n"
"                   Case sensitive search for data",
"long help of Where"));
  cmd->init();

  cmdset->add(cmd= new cl_break_cmd("break", 0,
"break addr [hit]   Set fix breakpoint\n"
"break mem_type r|w addr [hit]\n"
"                   Set fix event breakpoint",
"long help of break"));
  cmd->init();

  cmdset->add(cmd= new cl_tbreak_cmd("tbreak", 0,
"tbreak addr [hit]  Set temporary breakpoint\n"
"tbreak mem_type r|w addr [hit]\n"
"                   Set temporary event breakpoint",
"long help of tbreak"));
  cmd->init();

  cmdset->add(cmd= new cl_clear_cmd("clear", 0,
"clear [addr...]    Clear fix breakpoint",
"long help of clear"));
  cmd->init();

  cmdset->add(cmd= new cl_delete_cmd("delete", 0,
"delete [nr...]     Delete breakpoint(s)",
"long help of clear"));
  cmd->init();

  cmdset->add(cmd= new cl_help_cmd("help", 0,
"help [command]     Help about command(s)",
"long help of help"));
  cmd->init();
  cmd->add_name("?");

  cmdset->add(cmd= new cl_quit_cmd("quit", 0,
"quit               Quit",
"long help of quit"));
  cmd->init();

  cmdset->add(cmd= new cl_kill_cmd("kill", 0,
"kill               Shutdown simulator",
"long help of kill"));
  cmd->init();

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_show_copying_cmd("copying", 0, 
"show copying       Conditions for redistributing copies of uCsim",
"long help of show copying"));
    cmd->init();
    cset->add(cmd= new cl_show_warranty_cmd("warranty", 0, 
"show warranty      Various kinds of warranty you do not have",
"long help of show warranty"));
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("show", 0,
"show subcommand    Generic command for showing things about the uCsim",
"long help of show", cset));
  cmd->init();

  {
    cset= new cl_cmdset(this);
    cset->init();
    cset->add(cmd= new cl_gui_start_cmd("start", 0, 
"gui start          Start interfacing with GUI tool",
"long help of gui start"));
    cmd->init();
    cset->add(cmd= new cl_gui_stop_cmd("stop", 0, 
"gui stop           Stop interfacing with GUI tool",
"long help of gui stop"));
    cmd->init();
  }
  cmdset->add(cmd= new cl_super_cmd("gui", 0,
"gui subcommand     Operations to support GUI tools",
"long help of gui", cset));
  cmd->init();
}


/* End of sim.src/sim.cc */
