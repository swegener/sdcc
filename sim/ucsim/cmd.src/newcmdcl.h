/*
 * Simulator of microcontrollers (cmdcl.h)
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

#ifndef CMDCL_HEADER
#define CMDCL_HEADER


#include "ddconfig.h"

#include <stdio.h>
#if FD_HEADER_OK
# include HEADER_FD
#endif

#include "pobjcl.h"


/*
 * Command line with parameters
 */

class cl_cmdline: cl_base
{
public:
  char *cmd;
  char *name;
  class cl_list *params;
  class cl_ustrings *tokens;

public:
  cl_cmdline(char *cmd);
  virtual ~cl_cmdline(void);
  virtual int init(void);

  virtual int split(void);
  virtual int shift(void);
  virtual int repeat(void);
  virtual class cl_cmd_arg *param(int num);
  virtual void insert_param(int pos, class cl_cmd_arg *param);
private:
  char *skip_delims(char *start);
};


/*
 * Command and container
 */

// simple command
class cl_cmd: public cl_base
{
public:
  class cl_sim *sim;
  class cl_strings *names;
  int  can_repeat;
  char *short_help;
  char *long_help;

public:
  cl_cmd(class cl_sim *asim,
	 char *aname,
	 int  can_rep,
	 char *short_hlp,
	 char *long_hlp);
  ~cl_cmd(void);

  virtual void add_name(char *name);
  virtual int name_match(char *aname, int strict);
  virtual int name_match(class cl_cmdline *cmdline, int strict);
  virtual int syntax_ok(class cl_cmdline *cmdline);
  virtual int work(class cl_cmdline *cmdline, class cl_console *con);
  virtual int do_work(class cl_cmdline *cmdline, class cl_console *con);
};

// Command set is list of cl_cmd objects
class cl_cmdset: public cl_list
{
public:
  class cl_sim *sim;
  class cl_cmd *last_command;

public:
  cl_cmdset(class cl_sim *asim);

  virtual class cl_cmd *get_cmd(class cl_cmdline *cmdline);
  virtual void del(char *name);
  virtual void replace(char *name, class cl_cmd *cmd);
};

// subset of commands
class cl_super_cmd: public cl_cmd
{
public:
  class cl_cmdset *commands;

public:
  cl_super_cmd(class cl_sim *asim,
	       char *aname,
	       int  can_rep,
	       char *short_hlp,
	       char *long_hlp,
	       class cl_cmdset *acommands);
  ~cl_super_cmd(void);

  virtual int work(class cl_cmdline *cmdline, class cl_console *con);
};


/*
 * Command console
 */

class cl_console: public cl_base
{
public:
  class cl_sim *sim;
  FILE *in, *out;
  char *last_command;
  int flags; // See CONS_XXXX
  char *prompt;

public:
  cl_console(): cl_base() {}
  cl_console(char *fin, char *fout, class cl_sim *asim);
  cl_console(FILE *fin, FILE *fout, class cl_sim *asim);
#ifdef SOCKET_AVAIL
  cl_console(int portnumber, class cl_sim *asim);
#endif
  ~cl_console(void);
  virtual int init(void);

  virtual void welcome(void);
  virtual void print_prompt(void);
  virtual int  printf(char *format, ...);
  virtual void print_bin(long data, int bits);
  virtual int  match(int fdnum);
  virtual int  get_in_fd(void);
  virtual int  input_avail(void);
  virtual char *read_line(void);
  virtual int  proc_input(void);
  virtual bool interpret(char *cmd);
  virtual bool old_command(class cl_cmdline *cmdline) { return(FALSE); }
};

#ifdef SOCKET_AVAIL
class cl_listen_console: public cl_console
{
public:
  int sock;
public:
  cl_listen_console(int serverport, class cl_sim *asim);

  virtual void welcome(void) {}
  virtual void prompt(void) {}

  virtual int match(int fdnum);
  virtual int get_in_fd(void);
  virtual int proc_input(void);
};
#endif


/*
 * Command interpreter
 */

class cl_commander: public cl_base
{
public:
  class cl_list *cons;
  fd_set read_set, active_set;
  int fd_num;
  class cl_sim *sim;
  class cl_console *actual_console, *frozen_console;

public:
  cl_commander(class cl_sim *asim);
  ~cl_commander(void);
  virtual int init(void);

  virtual class cl_console *mk_console(char *fin, char *fout,
				       class cl_sim *asim);
  virtual class cl_console *mk_console(FILE *fin, FILE *fout,
				       class cl_sim *asim);
#ifdef SOCKET_AVAIL
  virtual class cl_console *mk_console(int portnumber, class cl_sim *asim);
#endif
  void add_console(class cl_console *console);
  void del_console(class cl_console *console);
  void set_fd_set(void);

  int all_printf(char *format, ...);	// print to all consoles
  int printf(char *format, ...);	// print to actual_console
  int debug(char *format, ...);		// print consoles with debug flag set
  int flag_printf(int iflags, char *format, ...);
  int input_avail(void);
  int input_avail_on_frozen(void);
  int wait_input(void);
  int proc_input(void);
};


#endif

/* End of cmdcl.h */
