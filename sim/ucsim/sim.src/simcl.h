/*
 * Simulator of microcontrollers (simcl.h)
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

#ifndef SIMCL_HEADER
#define SIMCL_HEADER

#include <stdio.h>

// prj
#include "pobjcl.h"

// cmd
#include "newcmdcl.h"

// local
#include "uccl.h"
#include "argcl.h"


class cl_sim: public cl_base
{
public:
  int state; // See SIM_XXXX
  int argc; char **argv;

  class cl_commander *cmd;
  class cl_uc *uc;
  class cl_cmdset *cmdset;
  //class cl_console *frozen_console;

  char *accept_args;
  class cl_ustrings *in_files;
  class cl_list *arguments;
  
public:
  cl_sim(char *more_args, int iargc, char *iargv[]);
  //cl_sim(class cl_uc *auc);
  ~cl_sim(void);
  virtual int init(void);
  
  virtual int proc_arguments(int argc, char *argv[]);
  virtual int proc_arg(char arg, char *optarg);

  virtual class cl_commander *mk_commander(void);
  virtual class cl_uc *mk_controller(void);
  virtual class cl_cmdset *mk_cmdset(void);
  virtual void build_cmd_set(void);
  virtual class cl_cmd_arg *mk_cmd_int_arg(long long i);
  virtual class cl_cmd_arg *mk_cmd_sym_arg(char *s);
  virtual class cl_cmd_arg *mk_cmd_str_arg(char *s);
  virtual class cl_cmd_arg *mk_cmd_bit_arg(class cl_cmd_arg *sfr,
					   class cl_cmd_arg *bit);

  int arg_avail(char name);
  int arg_avail(char *name);
  virtual long long get_iarg(char sname, char *lname);
  virtual char *get_sarg(char sname, char *lname);
  virtual double get_farg(char sname, char *lname);
  virtual void *get_parg(char sname, char *lname);

  virtual int main(void);
  virtual int do_cmd(char *cmd, class cl_console *console);
  virtual void start(class cl_console *con);
  virtual void stop(int reason);

  // Obsolete, for old commander
public:
  FILE *cmd_out(void);
  FILE *cmd_in(void);
};


#endif

/* End of simcl.h */
