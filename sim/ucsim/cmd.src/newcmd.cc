/*
 * Simulator of microcontrollers (cmd.src/newcmd.cc)
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
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
#endif
#include <sys/time.h>
#if FD_HEADER_OK
# include HEADER_FD
#endif
#include <unistd.h>
#include "i_string.h"

// prj
#include "globals.h"
#include "utils.h"

// sim
#include "simcl.h"
#include "argcl.h"
#include "appcl.h"

// local
#include "newcmdcl.h"
#include "cmdutil.h"


extern "C" int vasprintf(char **strp, const  char *format, va_list ap);
extern "C" int vsnprintf(char *str, size_t size,const char *format,va_list ap);

static int
cmd_do_print(FILE *f, char *format, va_list ap)
{
  int ret;
#ifdef HAVE_VASPRINTF
  char *msg= NULL;
  vasprintf(&msg, format, ap);
  ret= fprintf(f, "%s", msg);
  free(msg);
#else
#  ifdef HAVE_VSNPRINTF
  char msg[80*25];
  vsnprintf(msg, 80*25, format, ap);
  ret= fprintf(f, "%s", msg);
#  else
#    ifdef HAVE_VPRINTF
  char msg[80*25];
  vsprintf(msg, format, ap); /* Dangerous */
  ret= fprintf(f, "%s", msg);
#    else
#      ifdef HAVE_DOPRNT
  /* ??? */
  /*strcpy(msg, "Unimplemented printf has called.\n");*/
#      else
  /*strcpy(msg, "printf can not be implemented, upgrade your libc.\n");*/
#      endif
#    endif
#  endif
#endif
  fflush(f);
  return(ret);
}


/*
 * Options of console
 */

cl_prompt_option::cl_prompt_option(class cl_console *console):
  cl_optref(console)
{
  con= console;
}

int
cl_prompt_option::init(void)
{
  char *help;
  help= format_string("Prompt string of console%d", con->get_id());
  create(con, string_opt, "prompt", help);
  //fprintf(stderr," **new prompt option %p\"%s\", value=%p str=%p\n",option,object_name(option),option->get_value(),option->get_value()->sval);
  free(help);
  default_option("prompt");
  //fprintf(stderr,"opt=%p\"%s\" value after default set=%p str=%p\n",option,object_name(option),option->get_value(),option->get_value()->sval);
  return(0);
}

void
cl_prompt_option::option_changed(void)
{
  if (!con)
    return;
  char *s;
  option->get_value(&s);
  con->set_prompt(s);
}


cl_debug_option::cl_debug_option(class cl_console *console):
  cl_prompt_option(console)
{}

int
cl_debug_option::init(void)
{
  char *help;
  help= format_string("Debug messages to console%d", con->get_id());
  create(con, bool_opt, "debug", help);
  free(help);
  default_option("debug");
  return(0);
}

void
cl_debug_option::option_changed(void)
{
  if (!con)
    return;
  bool b;
  option->get_value(&b);
  if (b)
    con->flags|= CONS_DEBUG;
  else
    con->flags&= ~CONS_DEBUG;
}


/*
 * Command console
 *____________________________________________________________________________
 */

cl_console::cl_console(char *fin, char *fout, class cl_app *the_app):
  cl_base()
{
  FILE *f;

  last_command= NULL;
  app= the_app;
  in= 0;
  if (fin)
    if (f= fopen(fin, "r"), in= f, !f)
      fprintf(stderr, "Can't open `%s': %s\n", fin, strerror(errno));
  out= 0;
  if (fout)
    if (f= fopen(fout, "w"), out= f, !f)
      fprintf(stderr, "Can't open `%s': %s\n", fout, strerror(errno));
  prompt= 0;
  flags= CONS_NONE;
  if (in &&
      isatty(fileno(in)))
    flags|= CONS_INTERACTIVE;
  else
    ;//fprintf(stderr, "Warning: non-interactive console\n");
  id= 0;
}

cl_console::cl_console(FILE *fin, FILE *fout, class cl_app *the_app):
  cl_base()
{
  last_command= NULL;
  app= the_app;
  in = fin;
  out= fout;
  rout=(FILE *)0;
  prompt= 0;
  flags= CONS_NONE;
  if (in &&
      isatty(fileno(in)))
    flags|= CONS_INTERACTIVE;
  else
    ;//fprintf(stderr, "Warning: non-interactive console\n");
  id= 0;
}

/*
 * use the port number supplied to connect to localhost for 
 * (by Sandeep)
 */

#ifdef SOCKET_AVAIL
static int
connect_to_port(int portnum)
{
  int sock= socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(portnum);
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sock, (struct sockaddr *)&sin, sizeof(sin))) {
    fprintf(stderr, "Connect to port %d: %s\n", portnum, strerror(errno));
    return -1;
  }
  return sock;
}

cl_console::cl_console(int portnumber, class cl_app *the_app)
{
  int sock= connect_to_port(portnumber);

  last_command= NULL;
  app= the_app;
  if (!(in= fdopen(sock, "r")))
    fprintf(stderr, "cannot open port for input\n");
  if (!(out= fdopen(sock, "w")))
    fprintf(stderr, "cannot open port for output\n");
  //fprintf(stderr, "init socket done\n");
  id= 0;
}
#endif

class cl_console *
cl_console::clone_for_exec(char *fin)
{
  FILE *fi= 0, *fo= 0;

  if (fin)
    if (fi= fopen(fin, "r"), !fi)
      {
	fprintf(stderr, "Can't open `%s': %s\n", fin, strerror(errno));
	return(0);
      }
  if ((fo= fdopen(dup(fileno(out)), "a")) == 0)
    {
      fclose(fi);
      fprintf(stderr, "Can't re-open output file: %s\n", strerror(errno));
      return(0);
    }
  class cl_console *con= new cl_sub_console(this, fi, fo, app);
  return(con);
}

int
cl_console::init(void)
{
  cl_base::init();
  prompt_option= new cl_prompt_option(this);
  prompt_option->init();
  null_prompt_option= new cl_optref(this);
  null_prompt_option->init();
  null_prompt_option->use("null_prompt");
  debug_option= new cl_debug_option(this);
  debug_option->init();
  welcome();
  flags&= ~CONS_PROMPT;
  //print_prompt();
  return(0);
}

cl_console::~cl_console(void)
{
  if (in)
    fclose(in);
  un_redirect();
  if (out)
    {
      if (flags & CONS_PROMPT)
	fprintf(out, "\n");
      fflush(out);
      fclose(out);
    }
  delete prompt_option;
  delete null_prompt_option;
  delete debug_option;
#ifdef SOCKET_AVAIL
  /*  if (sock)
    {
      shutdown(sock, 2);
      close(sock);
      }*/
#endif
}


/*
 * Output functions
 */

void
cl_console::welcome(void)
{
  FILE *Out= rout?rout:out;

  if (!Out ||
      (flags & CONS_NOWELCOME))
    return;
  fprintf(Out, "uCsim %s, Copyright (C) 1997 Daniel Drotos, Talker Bt.\n"
	  "uCsim comes with ABSOLUTELY NO WARRANTY; for details type "
	  "`show w'.\n"
	  "This is free software, and you are welcome to redistribute it\n"
	  "under certain conditions; type `show c' for details.\n",
	  VERSIONSTR);
  fflush(Out);
}

void
cl_console::redirect(char *fname, char *mode)
{
  if ((rout= fopen(fname, mode)) == NULL)
    dd_printf("Unable to open file '%s' for %s: %s\n",
	      fname, (mode[0]=='w')?"write":"append", strerror(errno));
}

void
cl_console::un_redirect(void)
{
  if (!rout)
    return;
  fclose(rout);
  rout= NULL;
}
    
void
cl_console::print_prompt(void)
{
  //char *p;
  FILE *Out= rout?rout:out;

  if (flags & (CONS_PROMPT|CONS_FROZEN|CONS_INACTIVE))
    return;
  flags|= CONS_PROMPT;
  if (!Out)
    return;
  if (/*app->args->arg_avail('P')*/null_prompt_option->get_value(bool(0)))
    putc('\0', Out);
  else
    {
      fprintf(Out, "%d", id);
      fprintf(Out, "%s", (prompt && prompt[0])?prompt:"> ");
      //	      ((p= app->args->get_sarg(0, "prompt"))?p:"> "));
    }
  fflush(Out);
}

int
cl_console::dd_printf(char *format, ...)
{
  va_list ap;
  int ret= 0;
  FILE *Out= rout?rout:out;

  if (Out)
    {
      va_start(ap, format);
      ret= cmd_do_print(Out, format, ap);
      va_end(ap);
    }
  return(ret);
}

void
cl_console::print_bin(long data, int bits)
{
  long mask= 1;
  FILE *Out= rout?rout:out;

  if (!Out)
    return;
  mask= mask << ((bits >= 1)?(bits-1):0);
  while (bits--)
    {
      fprintf(Out, "%c", (data&mask)?'1':'0');
      mask>>= 1;
    }
}

void
cl_console::print_char_octal(char c)
{
  FILE *Out= rout?rout:out;

  if (Out)
    ::print_char_octal(c, Out);
}


/*
 * Input functions
 */

int
cl_console::match(int fdnum)
{
  if (in &&
      fileno(in) == fdnum)
    return(1);
  return(0);
}

int
cl_console::get_in_fd(void)
{
  if (flags & CONS_INACTIVE)
    return(-2);
  return(in?fileno(in):-1);
}

int
cl_console::input_avail(void)
{
  struct timeval tv;
  int i;
  
  if ((i= get_in_fd()) < 0)
    return(0);
  fd_set s;
  FD_ZERO(&s);
  FD_SET(i, &s);
  tv.tv_sec= tv.tv_usec= 0;
  i= select(i+1, &s, NULL, NULL, &tv);
  return(i);
}

char *
cl_console::read_line(void)
{
  char *s= NULL;

#ifdef HAVE_GETLINE
  if (getline(&s, 0, in) < 0)
    return(0);
#else

# ifdef HAVE_GETDELIM
  size_t n= 30;
  s= (char *)malloc(n);
  if (getdelim(&s, &n, '\n', in) < 0)
    {
      free(s);
      return(0);
    }
# else

#  ifdef HAVE_FGETS
  s= (char *)malloc(300);
  if (fgets(s, 300, in) == NULL)
    {
      free(s);
      return(0);
    }
#  endif
# endif
#endif
  s[strlen(s)-1]= '\0';
  if (s[strlen(s)-1] == '\r')
    s[strlen(s)-1]= '\0';
  flags&= ~CONS_PROMPT;
  return(s);
}

int
cl_console::proc_input(class cl_cmdset *cmdset)
{
  int retval= 0;

  un_redirect();
  if (feof(in))
    {
      fprintf(out, "End\n");
      return(1);
    }
  char *cmdstr= read_line();
  if (!cmdstr)
    return(1);
  if (flags & CONS_FROZEN)
    {
      app->get_sim()->stop(resUSER);
      flags&= ~CONS_FROZEN;
      retval= 0;
    }
  else
    {
      if (cmdstr &&
	  *cmdstr == '\004')
	retval= 1;
      else
	{
	  class cl_cmdline *cmdline;
	  class cl_cmd *cm;
	  if (flags & CONS_ECHO)
	    dd_printf("%s\n", cmdstr);
	  cmdline= new cl_cmdline(app, cmdstr, this);
	  cmdline->init();
	  cm= cmdset->get_cmd(cmdline);
	  if (cm)
	    retval= cm->work(app, cmdline, this);
	  delete cmdline;
	  if (!cm)
	    retval= interpret(cmdstr);
	}
    }
  //retval= sim->do_cmd(cmd, this);
  un_redirect();
  /*if (!retval)
    print_prompt();*/
  free(cmdstr);
  return(retval);
}

/*
 * Old version, sim->do_cmd() falls into this if it doesn't find a new
 * command object which can handle entered command
 */

int
cl_console::interpret(char *cmd)
{
  FILE *Out= rout?rout:out;

  fprintf(Out, "Unknown command\n");
  return(0);
}

void
cl_console::set_id(int new_id)
{
  char *s;

  id= new_id;
  set_name(s= format_string("console%d", id));
  free(s);
}

void
cl_console::set_prompt(char *p)
{
  if (prompt)
    free(prompt);
  if (p &&
      *p)
    prompt= strdup(p);
  else
    prompt= 0;
}


/*
 * This console listen on a socket and can accept connection requests
 */
#ifdef SOCKET_AVAIL

cl_listen_console::cl_listen_console(int serverport, class cl_app *the_app)
{
  last_command= NULL;
  app= the_app;
  if ((sock= make_server_socket(serverport)) >= 0)
    {
      if (listen(sock, 10) < 0)
	fprintf(stderr, "Listen on port %d: %s\n",
		serverport, strerror(errno));
    }
  in= out= 0;
}

int
cl_listen_console::match(int fdnum)
{
  return(sock == fdnum);
}

int
cl_listen_console::get_in_fd(void)
{
  return(sock);
}

int
cl_listen_console::proc_input(class cl_cmdset *cmdset)
{
  int newsock;
  ACCEPT_SOCKLEN_T size;
  struct sockaddr_in sock_addr;
  class cl_commander *cmd;

  cmd= app->get_commander();
  size= sizeof(struct sockaddr); 
  newsock= accept(sock, (struct sockaddr*)&sock_addr, &size);
  if (newsock < 0)
    {
      perror("accept");
      return(0);
    }
  if (!(in= fdopen(newsock, "r+")))
    fprintf(stderr, "cannot open port for input\n");
  if (!(out= fdopen(newsock, "w+")))
    fprintf(stderr, "cannot open port for output\n");
  class cl_console *c= cmd->mk_console(in, out);
  c->flags|= CONS_INTERACTIVE;
  cmd->add_console(c);
  in= out= 0;
  return(0);
}

#endif /* SOCKET_AVAIL */


/*
 * Sub-console
 */

cl_sub_console::cl_sub_console(class cl_console *the_parent,
			       FILE *fin, FILE *fout, class cl_app *the_app):
  cl_console(fin, fout, the_app)
{
  parent= the_parent;
}

cl_sub_console::~cl_sub_console(void)
{
  class cl_commander *c= app->get_commander();

  if (parent && c)
    {
      c->activate_console(parent);
    }
}

int
cl_sub_console::init(void)
{
  class cl_commander *c= app->get_commander();

  if (parent && c)
    {
      c->deactivate_console(parent);
    }
  cl_console::init();
  flags|= CONS_ECHO;
  return(0);
}


/*
 * Command interpreter
 *____________________________________________________________________________
 */

cl_commander::cl_commander(class cl_app *the_app, class cl_cmdset *acmdset
			   /*, class cl_sim *asim*/):
  cl_base()
{
  app= the_app;
  cons= new cl_list(1, 1); 
  actual_console= frozen_console= 0;
  cmdset= acmdset;
}

int
cl_commander::init(void)
{
  class cl_optref console_on_option(this);

  console_on_option.init();
  console_on_option.use("console_on");

  cl_base::init();
  set_name("Commander");
  
  char *Config= app->args->get_sarg(0, "Config");
  if (Config)
    {
      class cl_console *con= mk_console(0/*"/dev/tty"*/);
      add_console(con);
    }

  //if (app->args->arg_avail('c'))
    {
      char *cn= console_on_option.get_value(cn);
      if (cn)
	add_console(mk_console(cn, cn));
    }
#ifdef SOCKET_AVAIL
  if (app->args->arg_avail('Z'))
    add_console(mk_console(app->args->get_iarg(0, "Zport")));
  if (app->args->arg_avail('r'))
    add_console(mk_console(app->args->get_iarg('r', 0)));
#endif
  if (cons->get_count() == 0)
    add_console(mk_console(stdin, stdout));
  return(0);
}

cl_commander::~cl_commander(void)
{
  delete cons;
  delete cmdset;
}

class cl_console *
cl_commander::mk_console(char *fin, char *fout)
{
  return(new cl_console(fin, fout, app));
}

class cl_console *
cl_commander::mk_console(FILE *fin, FILE *fout)
{
  return(new cl_console(fin, fout, app));
}

#ifdef SOCKET_AVAIL
class cl_console *
cl_commander::mk_console(int portnumber)
{
  return(new cl_listen_console(portnumber, app));
}
#endif

void
cl_commander::add_console(class cl_console *console)
{
  if (!console)
    return;
  int i=cons->add(console);
  console->set_id(i);
  console->init();
  set_fd_set();
}

void
cl_commander::del_console(class cl_console *console)
{
  cons->disconn(console);
  set_fd_set();
}

void
cl_commander::activate_console(class cl_console *console)
{
  console->flags&= ~CONS_INACTIVE;
  //console->print_prompt();
  set_fd_set();
}

void
cl_commander::deactivate_console(class cl_console *console)
{
  console->flags|= CONS_INACTIVE;
  set_fd_set();
}

void
cl_commander::set_fd_set(void)
{
  int i;

  //fprintf(stderr, "** Setting fd set\n");  
  FD_ZERO(&read_set);
  fd_num= 0;
  for (i= 0; i < cons->count; i++)
    {
      int fd;
      class cl_console *c= (class cl_console*)(cons->at(i));
      if ((fd= c->get_in_fd()) >= 0)
	{
	  if ((c->flags & CONS_FROZEN) == 0 ||
	      (c->flags & CONS_INTERACTIVE) != 0)
	    {
	      FD_SET(fd, &read_set);
	      if (fd > fd_num)
		fd_num= fd;
	    }
	}
      else
	;//fprintf(stderr, "** Skipping console %p\n",c);
    }
  fd_num++;
}


/*
 * Printing to all consoles
 */

int
cl_commander::all_printf(char *format, ...)
{
  va_list ap;
  int i, ret= 0;
  
  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      FILE *Out= c->get_out();
      if (Out)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(Out, format, ap);
	  va_end(ap);
	}
    }
  return(ret);
}

void
cl_commander::prompt(void)
{
  int i;
  
  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      c->print_prompt();
    }
}

int
cl_commander::all_print(char *string, int length)
{
  int i;
  
  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      FILE *Out= c->get_out();
      if (Out)
	{
	  for (int j= 0; j < length; j++)
	    putc(string[j], Out);
	}
    }
  return(0);
}

/*
 * Printing to actual_console
 */

int
cl_commander::dd_printf(char *format, ...)
{
  va_list ap;
  int ret= 0;
  FILE *f;

  if (actual_console)
    f= actual_console->get_out();
  else if (frozen_console)
    f= frozen_console->get_out();
  else
    f= 0;
  if (/*actual_console &&
	actual_console->out*/f)
    {
      va_start(ap, format);
      ret= cmd_do_print(f/*actual_console->out*/, format, ap);
      va_end(ap);
    }
  return(ret);
}

/*
 * Printing to consoles which have CONS_DEBUG flag set
 */

int
cl_commander::debug(char *format, ...)
{
  va_list ap;
  int i, ret= 0;

  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      FILE *Out= c->get_out();
      if (Out &&
	  c->flags & CONS_DEBUG)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(Out, format, ap);
	  va_end(ap);
	}
    }
  return(ret);
}

int
cl_commander::flag_printf(int iflags, char *format, ...)
{
  va_list ap;
  int i, ret= 0;

  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      FILE *Out= c->get_out();
      if (Out &&
	  (c->flags & iflags) == iflags)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(Out, format, ap);
	  va_end(ap);
	}
    }
  return(ret);
}

int
cl_commander::input_avail(void)
{
  struct timeval tv;
  int i;

  tv.tv_sec= tv.tv_usec= 0;
  active_set= read_set;
  i= select(fd_num, &active_set, NULL, NULL, &tv);
  return(i);
}

int
cl_commander::input_avail_on_frozen(void)
{
  int fd;

  if (!frozen_console)
    return(0);
  if ((fd= frozen_console->get_in_fd()) >= 0 &&
      !isatty(fd))
    return(0);
  return(frozen_console->input_avail());
}

int
cl_commander::wait_input(void)
{
  int i;

  active_set= read_set;
  prompt();
  i= select(fd_num, &active_set, NULL, NULL, NULL);
  return(i);
}

int
cl_commander::proc_input(void)
{
  int i;

  for (i= 0; i < fd_num; i++)
    if (FD_ISSET(i, &active_set))
      {
	class cl_console *c;
	int j;
	for (j= 0; j < cons->count; j++)
	  {
	    c= (class cl_console*)(cons->at(j));
	    if (c->match(i))
	      {
		actual_console= c;
		int retval= c->proc_input(cmdset);
		if (retval)
		  {
		    del_console(c);
		    delete c;
		  }
		actual_console= 0;
		return(cons->count == 0);
	      }
	  }
      }
  return(0);
}


/* End of cmd.src/newcmd.cc */
