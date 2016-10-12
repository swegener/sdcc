/*
 * Simulator of microcontrollers (fiocl.h)
 *
 * Copyright (C) 1997,16 Drotos Daniel, Talker Bt.
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

#ifndef FIOCL_HEADER
#define FIOCL_HEADER

#include <stdio.h>

#include "pobjcl.h"


enum file_type {
  F_UNKNOWN,
  F_FILE,
  F_CHAR,
  F_SOCKET,
  F_LISTENER,
  F_PIPE,
  F_CONSOLE, // win only
  F_SERIAL // win only
};

enum tu_special_keys {
  TU_UP		= -1,
  TU_DOWN	= -2,
  TU_LEFT	= -3,
  TU_RIGHT	= -4,
  TU_HOME	= -5,
  TU_END	= -6,
  TU_PGUP	= -7,
  TU_PGDOWN	= -8,
  TU_DEL	= -9,
  TU_F1		= -10,
  TU_F2		= -11,
  TU_F3		= -12,
  TU_F4		= -13,
  TU_F5		= -14,
  TU_F6		= -15,
  TU_F7		= -16,
  TU_F8		= -17,
  TU_F9		= -18,
  TU_F10	= -19,
  TU_F11	= -20,
  TU_F12	= -21,
  TU_INS	= -22
};


/* History */

class cl_history: public cl_ustrings
{
 protected:
  int nr;
  //chars actual_line;
 public:
  cl_history(char *aname);
  cl_history(const char *aname);
  virtual ~cl_history(void);

 public:
  char *up(chars line);
  char *down(chars line);
  char *enter(chars line);
  void replace(chars line);
};


/* General file */

class cl_f: public cl_base
{
 public:
  int file_id;
  bool tty;
  enum file_type type;
 protected:
  chars file_name, file_mode;
  bool cooking;
  class cl_f *echo_to, *echo_of;
  bool own;
  int at_end;
  char line[1024];
  int cursor;
  char esc_buffer[100];
  char last_ln;
  char buffer[1024];
  int last_used, first_free;
  bool attributes_saved;
  class cl_history *hist;
  bool proc_telnet;
 public:
  cl_f(void);
  cl_f(chars fn, chars mode);
  cl_f(int the_server_port);
  virtual ~cl_f(void);
  virtual class cl_f *copy(chars mode);
  virtual int init(void);
  //virtual int open(void) { return init(); }
  virtual int open(char *fn);
  virtual int open(char *fn, char *mode);
  virtual int use_opened(int opened_file_id, char *mode);
  virtual int own_opened(int opened_file_id, char *mode);
  virtual int use_opened(FILE *f, chars mode);
  virtual int own_opened(FILE *f, chars mode);
  virtual enum file_type determine_type(void)= 0;
  virtual void changed(void);
  virtual int close(void);
  virtual int stop_use(void);

  virtual char *get_file_name() { return file_name; };
  virtual class cl_f *get_echo_to() { return echo_to; }
 protected:
  virtual int put(char c);
  virtual int get(void);
  virtual int finish_esc(int k);
  virtual int process_telnet(char ci);
  virtual int process_esc(char c);
  virtual int process(char c);
  virtual int pick(void);
  virtual int pick(char c);
  virtual int pick(const char *s);
 public:
  virtual int input_avail(void);
  virtual int read(char *buf, int max);

 public:
  //FILE *f(void) { return file_f; };
  int id(void) { return file_id; };

  virtual int check_dev(void)= 0;
  virtual int read_dev(char *buf, int max);
  virtual int write(char *buf, int count);
  virtual int write_str(char *s);
  virtual int write_str(const char *s);
  virtual int vprintf(char *format, va_list ap);
  virtual int prntf(char *format, ...);
  virtual bool eof(void);
  //virtual void flush(void);

  virtual void echo_cursor_save();
  virtual void echo_cursor_restore();
  virtual void echo_cursor_go_left(int n);
  virtual void echo_cursor_go_right(int n);
  virtual void echo_write(char *b, int l);
  virtual void echo_write_str(char *s);
  virtual void echo_write_str(const char *s);

  virtual void prepare_terminal();
  virtual void save_attributes();
  virtual void restore_attributes();
  virtual int raw(void);
  virtual int cooked(void);
  virtual void check(void) { return; }
  virtual int echo(class cl_f *out);
  virtual void interactive(class cl_f *echo_out);
  virtual int get_cooking() { return cooking; }
  virtual void set_telnet(bool val);
  virtual bool get_telnet() { return proc_telnet; }
 public:
  int server_port;

 public:
  //virtual int listen(int on_port);
  //virtual class cl_f *accept();
  //virtual int connect(chars host, int to_port);
};

extern void deb(chars format, ...);

extern int mk_srv_socket(int port);

extern class cl_f *mk_io(chars fn, chars mode);
extern class cl_f *cp_io(/*FILE *f*/int file_id, chars mode);
extern class cl_f *mk_srv(int server_port);
extern int srv_accept(class cl_f *listen_io,
		      class cl_f **fin, class cl_f **fout);

extern bool check_inputs(class cl_list *active, class cl_list *avail);

extern void msleep(int msec);
extern void loop_delay();

extern chars fio_type_name(enum file_type t);


#endif

/* End of fiocl.h */
