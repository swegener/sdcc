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
}

int
cl_sim::init(void)
{
  int i;

  cl_base::init();
  proc_arguments(argc, argv);
  cmdset= mk_cmdset();
  cmdset->init();
  build_cmd_set();
  if (!(uc= mk_controller()))
    return(1);
  uc->init();
  cmd= mk_commander();
  cmd->init();
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
	/*FIXME: putc('\0', f)*/;
      else
	cmd->all_printf("%s", arg_avail('P')?"\0":
			((prompt= get_sarg(0, "prompt"))?prompt:"> "));
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
  strcpy(opts, "c:p:PX:vV");
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

#ifdef SOCKET_AVAIL
      case 'Z':
	// By Sandeep
	arguments->add(new cl_prg_arg('Z', 0, (long long)1));
	if (!optarg || !isdigit(*optarg))
	  fprintf(stderr, "expected portnumber to follow -Z\n");
	else {
	  char *p;
	  long long l= strtol(optarg, &p, 0);
	  arguments->add(new cl_prg_arg(0, "Zport", l));
	}
	break;
#endif

      case 'p':
	arguments->add(new cl_prg_arg(0, "prompt", optarg));
	break;

      case 'P':
	arguments->add(new cl_prg_arg('P', 0, (long long)1));
	break;

#ifdef SOCKET_AVAIL
      case 'r':
	arguments->add(new cl_prg_arg('r', 0,
				      (long long)strtol(optarg, NULL, 0)));
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
	arguments->add(new cl_prg_arg('V', 0, (long long)1));
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
	return(a->get_ivalue());
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

class cl_commander *
cl_sim::mk_commander()
{
  class cl_commander *cmd= new cl_commander(this);
  return(cmd);
}

class cl_uc *
cl_sim::mk_controller(void)
{
  return(new cl_uc(this));
}

class cl_cmdset *
cl_sim::mk_cmdset(void)
{
  return(new cl_cmdset(this));
}

class cl_cmd_arg *
cl_sim::mk_cmd_int_arg(long long i)
{
  class cl_cmd_arg *arg= new cl_cmd_int_arg(i);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_sym_arg(char *s)
{
  class cl_cmd_arg *arg= new cl_cmd_sym_arg(s);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_str_arg(char *s)
{
  class cl_cmd_arg *arg= new cl_cmd_str_arg(s);
  arg->init();
  return(arg);
}

class cl_cmd_arg *
cl_sim::mk_cmd_bit_arg(class cl_cmd_arg *sfr, class cl_cmd_arg *bit)
{
  class cl_cmd_arg *arg= new cl_cmd_bit_arg(sfr, bit);
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
	    done= cmd->proc_input();
	}
      else
	{
	  cmd->wait_input();
	  done= cmd->proc_input();
	}
    }
  return(0);
}

int
cl_sim::do_cmd(char *cmdstr, class cl_console *console)
{
  class cl_cmdline *cmdline;
  class cl_cmd *cmd;
  int retval= 0;

  cmdline= new cl_cmdline(cmdstr);
  cmdline->init();
  if (console->old_command(cmdline))
    return(console->interpret(cmdstr));
  cmd= cmdset->get_cmd(cmdline);
  if (cmd)
    retval= cmd->work(cmdline, console);
  delete cmdline;
  if (cmd)
    return(retval);
  return(console->interpret(cmdstr));
}

void
cl_sim::start(class cl_console *con)
{
  state|= SIM_GO;
  con->flags|= CONS_FROZEN;
  cmd->frozen_console= con;
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
}


/*
 * Obsolete methods for old commander
 */

FILE *
cl_sim::cmd_in(void)
{
  if (!cmd ||
      cmd->cons->get_count() == 0)
    return(stdin);
  if (cmd->actual_console)
    return(cmd->actual_console->in?cmd->actual_console->in:stdin);
  class cl_console *con= (class cl_console *)(cmd->cons->at(0));
  return(con->in?con->in:stdin);
}

FILE *
cl_sim::cmd_out(void)
{
  if (!cmd ||
      cmd->cons->get_count() == 0)
    return(stdout);
  if (cmd->actual_console)
    return(cmd->actual_console->out?cmd->actual_console->out:stdout);
  class cl_console *con= (class cl_console *)(cmd->cons->at(0));
  return(con->out?con->out:stdout);
}


/*
 */

void
cl_sim::build_cmd_set(void)
{
  class cl_cmd *cmd;
  class cl_cmdset *cset;

  cmdset->add(cmd= new cl_conf_cmd(this, "conf", 0,
"conf               Configuration",
"long help of conf"));
  cmd->init();

  cmdset->add(cmd= new cl_state_cmd(this, "state", 0,
"state              State of simulator",
"long help of state"));
  cmd->init();

  cmdset->add(cmd= new cl_file_cmd(this, "file", 0,
"file \"FILE\"        Load FILE into ROM",
"long help of file"));
  cmd->init();
  cmd->add_name("load");

  cmdset->add(cmd= new cl_dl_cmd(this, "download", 0,
"download,dl          Load (intel.hex) data",
"long help of download"));
  cmd->init();
  cmd->add_name("dl");

  cset= new cl_cmdset(this);
  cset->init();
  cset->add(cmd= new cl_info_bp_cmd(this, "breakpoints", 0, 
"info breakpoints   Status of user-settable breakpoints",
"long help of info breakpoints"));
  cmd->add_name("bp");
  cmd->init();
  cset->add(cmd= new cl_info_reg_cmd(this, "registers", 0, 
"info registers     List of integer registers and their contents",
"long help of info registers"));
  cmd->init();
  cset->add(cmd= new cl_info_hw_cmd(this, "hardware", 0, 
"info hardware cathegory\n"
"                   Status of hardware elements of the CPU",
"long help of info hardware"));
  cmd->add_name("hw");
  cmd->init();

  cmdset->add(cmd= new cl_super_cmd(this, "info", 0,
"info subcommand    Information, see `info' command for more help",
"long help of info", cset));
  cmd->init();

  cmdset->add(cmd= new cl_get_cmd(this, "get", 0,
"get                Get",
"long help of get"));
  cmd->init();

  cmdset->add(cmd= new cl_set_cmd(this, "set", 0,
"set                Set",
"long help of set"));
  cmd->init();

  cmdset->add(cmd= new cl_timer_cmd(this, "timer", 0,
"timer a|d|g|r|s|v id [direction|value]\n"
"                   Timer add|del|get|run|stop|value",
"timer add|create|make id [direction] -- create a new timer\n"
"timer del id -- delete a timer\n"
"timer get id -- list timers\n"
"timer run id -- turn a timer ON\n"
"timer stop id -- turn a timer OFF\n"
"timer value id val -- set value of a timer to `val'"));
  cmd->init();

  cmdset->add(cmd= new cl_run_cmd(this, "run", 0,
"run                Go",
"long help of run"));
  cmd->init();
  //cmd->add_name("g");

  cmdset->add(cmd= new cl_step_cmd(this, "step", 0,
"step               Step",
"long help of step"));
  cmd->init();
  cmd->add_name("s");

  cmdset->add(cmd= new cl_reset_cmd(this, "reset", 0,
"reset              Reset",
"long help of reset"));
  cmd->init();

  cmdset->add(cmd= new cl_dump_cmd(this, "dump", 0,
"dump i|x|r|s [start [stop]]\n"
"                   Dump memory",
"long help of dump"));
  cmd->init();

  cmdset->add(cmd= new cl_di_cmd(this, "di", 0,
"di [start [stop]]  Dump Internal RAM",
"long help of di"));
  cmd->init();

  cmdset->add(cmd= new cl_dx_cmd(this, "dx", 0,
"dx [start [stop]]  Dump External RAM",
"long help of dx"));
  cmd->init();

  cmdset->add(cmd= new cl_ds_cmd(this, "ds", 0,
"ds [start [stop]]  Dump SFR",
"long help of ds"));
  cmd->init();

  cmdset->add(cmd= new cl_dch_cmd(this, "dch", 0,
"dch [start [stop]] Dump code in hex form",
"long help of dch"));
  cmd->init();

  cmdset->add(cmd= new cl_dc_cmd(this, "dc", 0,
"dc [start [stop]]  Dump code in disass form",
"long help of dc"));
  cmd->init();

  cmdset->add(cmd= new cl_break_cmd(this, "break", 0,
"break addr [hit]   Set fix breakpoint",
"long help of break"));
  cmd->init();

  cmdset->add(cmd= new cl_tbreak_cmd(this, "tbreak", 0,
"tbreak addr [hit]  Set temporary breakpoint",
"long help of tbreak"));
  cmd->init();

  cmdset->add(cmd= new cl_clear_cmd(this, "clear", 0,
"clear [addr...]    Clear fix breakpoint",
"long help of clear"));
  cmd->init();

  cmdset->add(cmd= new cl_help_cmd(this, "help", 0,
"help               Help",
"long help of help"));
  cmd->init();
  cmd->add_name("?");

  cmdset->add(cmd= new cl_quit_cmd(this, "quit", 0,
"quit               Quit",
"long help of quit"));
  cmd->init();

  cmdset->add(cmd= new cl_kill_cmd(this, "kill", 0,
"kill               Shutdown simulator",
"long help of kill"));
  cmd->init();
}


/* End of sim.src/sim.cc */
