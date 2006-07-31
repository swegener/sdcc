/*
 * Simulator of microcontrollers (cmd.src/cmdcl.h)
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

#ifndef CMD_NEWCMDCL_HEADER
#define CMD_NEWCMDCL_HEADER


#include "ddconfig.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#if FD_HEADER_OK
# include HEADER_FD
#endif

// prj
#include "pobjcl.h"

// sim.src
#include "appcl.h"

// local, cmd
#include "commandcl.h"


// Flags of consoles
#define CONS_NONE	 0
#define CONS_DEBUG	 0x01	// Print debug messages on this console
#define CONS_FROZEN	 0x02	// Console is frozen (g command issued)
#define CONS_PROMPT	 0x04	// Prompt is out, waiting for input
#define CONS_INTERACTIVE 0x08	// Interactive console
#define CONS_NOWELCOME	 0x10	// Do not print welcome message
#define CONS_INACTIVE	 0x20	// Do not do any action
#define CONS_ECHO	 0x40	// Echo commands

#define SY_ADDR		'a'
#define ADDRESS		"a"
#define SY_NUMBER	'n'
#define NUMBER		"n"
#define SY_DATA		'd'
#define DATA		"d"
#define SY_STRING	's'
#define STRING		"s"
#define SY_MEMORY	'm'
#define MEMORY		"m"
#define SY_HW		'h'
#define HW		"h"
#define SY_DATALIST	'D'
#define DATALIST	"D"
#define SY_BIT		'b'
#define BIT		"b"


class cl_prompt_option: public cl_optref
{
protected:
  class cl_console *con;
public:
  cl_prompt_option(class cl_console *console);
  virtual int init(void);
  virtual void option_changed(void);
};

class cl_debug_option: public cl_prompt_option
{
public:
  cl_debug_option(class cl_console *console);
  virtual int init(void);
  virtual void option_changed(void);
};

/*
 * Command console
 */

class cl_console: public cl_base
{
  friend class cl_commander;
protected:
  FILE *in, *out, *rout/*redirected output*/;
  int id;
  class cl_prompt_option *prompt_option;
  class cl_optref *null_prompt_option;
  class cl_debug_option *debug_option;
  class cl_ustrings *lines_printed;
  class cl_cmd *last_command;
  class cl_cmdline *last_cmdline;
public:
  class cl_app *app;
  int flags; // See CONS_XXXX
  char *prompt;
  
public:
  cl_console(void): cl_base() { app= 0; in= out= 0; flags= 0; }
  cl_console(char *fin, char *fout, class cl_app *the_app);
  cl_console(FILE *fin, FILE *fout, class cl_app *the_app);
  cl_console(char *fin, FILE *fout);
#ifdef SOCKET_AVAIL
  cl_console(int portnumber, class cl_app *the_app);
#endif
  virtual ~cl_console(void);
  virtual class cl_console *clone_for_exec(char *fin);
  virtual int init(void);

  virtual bool accept_last(void);

  virtual void welcome(void);
  virtual void redirect(char *fname, char *mode);
  virtual void un_redirect(void);
  virtual FILE *get_out(void) { return(rout?rout:out); }
  int cmd_do_print(FILE *f, char *format, va_list ap);
  virtual void print_prompt(void);
  virtual int  dd_printf(char *format, ...);
  virtual int  debug(char *format, ...);
  virtual void print_bin(long data, int bits);
  virtual void print_char_octal(char c);
  virtual int  match(int fdnum);
  virtual int  get_in_fd(void);
  virtual int  input_avail(void);
  virtual char *read_line(void);
  virtual int  proc_input(class cl_cmdset *cmdset);
  virtual bool interpret(char *cmd);
  virtual void set_id(int new_id);
  virtual int get_id(void) { return(id); }
  virtual void set_prompt(char *p);
};

#ifdef SOCKET_AVAIL
class cl_listen_console: public cl_console
{
public:
  int sock;
public:
  cl_listen_console(int serverport, class cl_app *the_app);

  virtual void welcome(void) {}
  virtual void prompt(void) {}

  virtual int match(int fdnum);
  virtual int get_in_fd(void);
  virtual int proc_input(class cl_cmdset *cmdset);
};
#endif


class cl_sub_console: public cl_console
{
  class cl_console *parent;
public:
  cl_sub_console(class cl_console *the_parent,
		 FILE *fin, FILE *fout, class cl_app *the_app);
  virtual ~cl_sub_console(void);
  virtual int init(void);
};


/*
 * Command interpreter
 */

class cl_commander: public cl_base
{
public:
  class cl_app *app;
  class cl_list *cons;
  fd_set read_set, active_set;
  UCSOCKET_T fd_num;
  //class cl_sim *sim;
  class cl_console *actual_console, *frozen_console;
  class cl_cmdset *cmdset;

public:
  cl_commander(class cl_app *the_app,
	       class cl_cmdset *acmdset/*, class cl_sim *asim*/);
  virtual ~cl_commander(void);
  virtual int init(void);

  virtual class cl_console *mk_console(char *fin, char *fout);
  virtual class cl_console *mk_console(FILE *fin, FILE *fout);
#ifdef SOCKET_AVAIL
  virtual class cl_console *mk_console(int portnumber);
#endif
  void add_console(class cl_console *console);
  void del_console(class cl_console *console);
  void activate_console(class cl_console *console);
  void deactivate_console(class cl_console *console);
  void set_fd_set(void);

  void prompt(void);
  FILE *get_out(void);
  int all_printf(char *format, ...);	// print to all consoles
  int all_print(char *string, int length);
  int dd_printf(char *format, ...);	// print to actual_console
  int dd_printf(char *format, va_list ap);// print to actual_console
  int debug(char *format, ...);		// print consoles with debug flag set
  int debug(char *format, va_list ap);	// print consoles with debug flag set
  int flag_printf(int iflags, char *format, ...);
  int input_avail(void);
  int input_avail_on_frozen(void);
  int wait_input(void);
  int proc_input(void);
  void exec_on(class cl_console *cons, char *file_name);
};


#endif

/* End of cmd.src/cmdcl.h */
