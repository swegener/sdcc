/*
 * Simulator of microcontrollers (cmdsetcl.h)
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

#ifndef CMDSETCL_HEADER
#define CMDSETCL_HEADER

#include "newcmdcl.h"


// CONF
class cl_conf_cmd: public cl_cmd
{
public:
  cl_conf_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// STATE
class cl_state_cmd: public cl_cmd
{
public:
  cl_state_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// FILE
class cl_file_cmd: public cl_cmd
{
public:
  cl_file_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DOWNLOAD
class cl_dl_cmd: public cl_cmd
{
public:
  cl_dl_cmd(class cl_sim *asim,
	    char *aname,
	    int  can_rep,
	    char *short_hlp,
	    char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// GET
class cl_get_cmd: public cl_cmd
{
public:
  cl_get_cmd(class cl_sim *asim,
	     char *aname,
	     int  can_rep,
	     char *short_hlp,
	     char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
  virtual int timer(class cl_cmdline *cmdline, class cl_console *con);
};

// SET
class cl_set_cmd: public cl_cmd
{
public:
  cl_set_cmd(class cl_sim *asim,
	     char *aname,
	     int  can_rep,
	     char *short_hlp,
	     char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
  virtual int timer(class cl_cmdline *cmdline, class cl_console *con);
};

// TIMER
class cl_timer_cmd: public cl_cmd
{
public:
  class cl_ticker *ticker;
  int what;
  char *name;

  cl_timer_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
  void set_ticker(class cl_cmd_arg *param);
  virtual int add(class cl_cmdline *cmdline, class cl_console *con);
  virtual int del(class cl_cmdline *cmdline, class cl_console *con);
  virtual int get(class cl_cmdline *cmdline, class cl_console *con);
  virtual int run(class cl_cmdline *cmdline, class cl_console *con);
  virtual int stop(class cl_cmdline *cmdline, class cl_console *con);
  virtual int val(class cl_cmdline *cmdline, class cl_console *con);
};

// RUN
class cl_run_cmd: public cl_cmd
{
public:
  cl_run_cmd(class cl_sim *asim,
	     char *aname,
	     int  can_rep,
	     char *short_hlp,
	     char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// STEP
class cl_step_cmd: public cl_cmd
{
public:
  cl_step_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// RESET
class cl_reset_cmd: public cl_cmd
{
public:
  cl_reset_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DUMP
class cl_dump_cmd: public cl_cmd
{
public:
  cl_dump_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  //cl_dump_cmd(class cl_sim *asim): cl_cmd(asim) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DI
class cl_di_cmd: public cl_dump_cmd
{
public:
  cl_di_cmd(class cl_sim *asim,
	    char *aname,
	    int  can_rep,
	    char *short_hlp,
	    char *long_hlp):
    cl_dump_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DX
class cl_dx_cmd: public cl_dump_cmd
{
public:
  cl_dx_cmd(class cl_sim *asim,
	    char *aname,
	    int  can_rep,
	    char *short_hlp,
	    char *long_hlp):
    cl_dump_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DCH
class cl_dch_cmd: public cl_dump_cmd
{
public:
  cl_dch_cmd(class cl_sim *asim,
	     char *aname,
	     int  can_rep,
	     char *short_hlp,
	     char *long_hlp):
    cl_dump_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DS
class cl_ds_cmd: public cl_dump_cmd
{
public:
  cl_ds_cmd(class cl_sim *asim,
	    char *aname,
	    int  can_rep,
	    char *short_hlp,
	    char *long_hlp):
    cl_dump_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// DC
class cl_dc_cmd: public cl_cmd
{
public:
  int last;
public:
  cl_dc_cmd(class cl_sim *asim,
	    char *aname,
	    int  can_rep,
	    char *short_hlp,
	    char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {last=0;}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// BREAK
class cl_break_cmd: public cl_cmd
{
public:
  enum brk_perm perm;
public:
  cl_break_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) { perm= brkFIX; }
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// TBREAK
class cl_tbreak_cmd: public cl_break_cmd
{
public:
  cl_tbreak_cmd(class cl_sim *asim,
		char *aname,
		int  can_rep,
		char *short_hlp,
		char *long_hlp):
    cl_break_cmd(asim, aname, can_rep, short_hlp, long_hlp) {perm=brkDYNAMIC;}
};

// CLEAR
class cl_clear_cmd: public cl_cmd
{
public:
  cl_clear_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// HELP
class cl_help_cmd: public cl_cmd
{
public:
  cl_help_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// QUIT
class cl_quit_cmd: public cl_cmd
{
public:
  cl_quit_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// KILL
class cl_kill_cmd: public cl_cmd
{
public:
  cl_kill_cmd(class cl_sim *asim,
	      char *aname,
	      int  can_rep,
	      char *short_hlp,
	      char *long_hlp):
    cl_cmd(asim, aname, can_rep, short_hlp, long_hlp) {}
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};


#endif

/* End of cmdsetcl.h */
