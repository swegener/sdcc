/*
 * Simulator of microcontrollers (newcmd.cc)
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

// sim
#include "simcl.h"
#include "argcl.h"

// local
#include "newcmdcl.h"
#include "cmdutil.h"


extern "C" int vasprintf();
extern "C" int vsnprintf();

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
 * Command line
 *____________________________________________________________________________
 */

cl_cmdline::cl_cmdline(char *acmd)
{
  cmd= strdup(acmd);
  params= new cl_list(2, 2);
  tokens= new cl_ustrings(2, 2);
  name= 0;
}

cl_cmdline::~cl_cmdline(void)
{
  if (cmd)
    free(cmd);
  if (name)
    free(name);
  delete params;
  delete tokens;
}

int
cl_cmdline::init(void)
{
  split();
  return(0);
}

char *
cl_cmdline::skip_delims(char *start)
{
  while (*start &&
	 strchr(" \t\v\r,", *start))
    start++;
  return(start);
}

int
cl_cmdline::split(void)
{
  char *start= cmd;
  int i;

  name= 0;
  if (!cmd ||
      !*cmd)
    return(0);
  start+= strspn(start, " \t\v\r,");
  if (start &&
      *start == '\n')
    {
      name= (char*)malloc(2);
      strcpy(name, "\n");
      return(0);
    }
  if (!*start)
    return(0);
  i= strcspn(start, " \t\v\r,");
  if (i)
    {
      name= (char*)malloc(i+1);
      strncpy(name, start, i);
      name[i]= '\0';
    }
  start+= i;
  start= skip_delims(start);
  // skip delimiters
  while (*start)
    {
      char *end, *param_str;
      if (*start == '"')
	{
	  // string
	  start++;
	  end= start;
	  while (*end &&
		 *end != '"')
	    end++;
	  if (*end == '"')
	    end--;
	  else
	    simulator->cmd->printf("Unterminated string\n");
	  param_str= (char *)malloc(end-start+2);
	  strncpy(param_str, start, 1+end-start);
	  param_str[1+end-start]= '\0';
	  tokens->add(strdup(param_str));
	  params->add(simulator->mk_cmd_str_arg(param_str));
	  free(param_str);
	  if (*end)
	    end++;
	  if (*end == '"')
	    end++;
	}
      else
	{
	  char *dot;
	  i= strcspn(start, " \t\v\r,");
	  end= start+i;
	  param_str= (char *)malloc(i+1);
	  strncpy(param_str, start, i);
	  param_str[i]= '\0';
	  tokens->add(strdup(param_str));
	  if ((dot= strchr(param_str, '.')) != NULL)
	    {
	      // bit
	      class cl_cmd_arg *sfr, *bit;
	      *dot= '\0';
	      dot++;
	      if (strchr("0123456789", *param_str) != NULL)
		sfr= simulator->mk_cmd_int_arg((long long)
					       strtol(param_str, 0, 0));
	      else
		sfr= simulator->mk_cmd_sym_arg(param_str);
	      if (*dot == '\0')
		{
		  bit= 0;
		  simulator->cmd->printf("Uncomplete bit address\n");
		}
	      else
		{
		  if (strchr("0123456789", *dot) != NULL)
		    bit= simulator->mk_cmd_int_arg((long long)
						   strtol(dot, 0, 0));
		  else
		    bit= simulator->mk_cmd_sym_arg(dot);
		}
	      params->add(simulator->mk_cmd_bit_arg(sfr, bit));
	    }
	  else if (strchr("0123456789", *param_str) != NULL)
	    {
	      // number
	      params->add(simulator->mk_cmd_int_arg((long long)
						    strtol(param_str, 0, 0)));
	    }
	  else
	    {
	      // symbol
	      params->add(simulator->mk_cmd_sym_arg(param_str));
	    }
	  free(param_str);
	}
      start= end;
      start= skip_delims(start);
    }
  return(0);
}

int
cl_cmdline::shift(void)
{
  char *s= skip_delims(cmd);

  free(name);
  name= NULL;
  if (s && *s)
    {
      while (*s &&
	     strchr(" \t\v\r,", *s) == NULL)
	s++;
      s= skip_delims(s);
      char *p= strdup(s);
      free(cmd);
      cmd= p;
      delete params;
      params= new cl_list(2, 2);
      split();
    }
  return(name && *name);
}

int
cl_cmdline::repeat(void)
{
  return(name &&
	 *name == '\n');
}

class cl_cmd_arg *
cl_cmdline::param(int num)
{
  if (num >= params->count)
    return(0);
  return((class cl_cmd_arg *)(params->at(num)));
}

void
cl_cmdline::insert_param(int pos, class cl_cmd_arg *param)
{
  if (pos >= params->count)
    params->add(param);
  else
    params->add_at(pos, param);
}


/*
 * Command
 *____________________________________________________________________________
 */

cl_cmd::cl_cmd(class cl_sim *asim,
	       char *aname,
	       int can_rep,
	       char *short_hlp,
	       char *long_hlp):
  cl_base()
{
  sim= asim;
  names= new cl_strings(1, 1);
  names->add(aname?strdup(aname):strdup("unknown"));
  can_repeat= can_rep;
  short_help= short_hlp?strdup(short_hlp):NULL;
  long_help= long_hlp?strdup(long_hlp):NULL;
}

/*cl_cmd::cl_cmd(class cl_sim *asim):
  cl_base()
{
  sim= asim;
  name= short_help= long_help= 0;
  can_repeat= 0;
}*/

cl_cmd::~cl_cmd(void)
{
  delete names;
  if (short_help)
    free(short_help);
  if (long_help)
    free(long_help);
}

void
cl_cmd::add_name(char *name)
{
  if (name)
    names->add(strdup(name));
}

int
cl_cmd::name_match(char *aname, int strict)
{
  int i;
  
  if (names->count == 0 &&
      !aname)
    return(1);
  if (!aname)
    return(0);
  if (strict)
    {
      for (i= 0; i < names->count; i++)
	{
	  char *n= (char*)(names->at(i));
	  if (strcmp(aname, n) == 0)
	    return(1);
	}
    }
  else
    {
      for (i= 0; i < names->count; i++)
	{
	  char *n= (char*)(names->at(i));
	  if (strstr(n, aname) == n)
	    return(1);
	}
    }
  return(0);
}

int
cl_cmd::name_match(class cl_cmdline *cmdline, int strict)
{
  return(name_match(cmdline->name, strict));
}

int
cl_cmd::syntax_ok(class cl_cmdline *cmdline)
{
  return(1);
}

int
cl_cmd::work(class cl_cmdline *cmdline, class cl_console *con)
{
  if (!syntax_ok(cmdline))
    return(0);
  return(do_work(cmdline, con));
}

int
cl_cmd::do_work(class cl_cmdline *cmdline, class cl_console *con)
{
  con->printf("Command \"%s\" does nothing.\n", (char*)(names->at(0)));
  return(0);
}


/*
 * Set of commands
 *____________________________________________________________________________
 */

cl_cmdset::cl_cmdset(class cl_sim *asim):
  cl_list(5, 5)
{
  sim= asim;
  last_command= 0;
}

class cl_cmd *
cl_cmdset::get_cmd(class cl_cmdline *cmdline)
{
  int i;

  if (cmdline->repeat())
    {
      if (last_command)
	return(last_command);
      else
	return(0);
    }
  // exact match
  for (i= 0; i < count; i++)
    {
      class cl_cmd *c= (class cl_cmd *)at(i);
      if (c->name_match(cmdline, 1))
	return(c);
    }
  // not exact match
  class cl_cmd *c_matched= 0;
  for (i= 0; i < count; i++)
    {
      class cl_cmd *c= (class cl_cmd *)at(i);
      if (c->name_match(cmdline, 0))
	{
	  if (!c_matched)
	    c_matched= c;
	  else
	    return(0);
	}
    }
  return(c_matched);
  //return(0);
}

void
cl_cmdset::del(char *name)
{
  int i;

  if (!name)
    return;
  for (i= 0; i < count; i++)
    {
      class cl_cmd *cmd= (class cl_cmd *)(at(i));
      if (cmd->name_match(name, 1))
	free_at(i);
    }
}

void
cl_cmdset::replace(char *name, class cl_cmd *cmd)
{
  int i;

  if (!name)
    return;
  for (i= 0; i < count; i++)
    {
      class cl_cmd *c= (class cl_cmd *)(at(i));
      if (c->name_match(name, 1))
	{
	  delete c;
	  put_at(i, cmd);
	}
    }
}


/*
 * Composed command: subset of commands
 *____________________________________________________________________________
 */

cl_super_cmd::cl_super_cmd(class cl_sim *asim,
			   char *aname,
			   int  can_rep,
			   char *short_hlp,
			   char *long_hlp,
			   class cl_cmdset *acommands):
  cl_cmd(asim, aname, can_rep, short_hlp, long_hlp)
{
  commands= acommands;
}

cl_super_cmd::~cl_super_cmd(void)
{
  if (commands)
    delete commands;
}

int
cl_super_cmd::work(class cl_cmdline *cmdline, class cl_console *con)
{
  class cl_cmd *cmd;

  if (!commands)
    return(0);
  
  if (!cmdline->shift())
    {
      int i;
      con->printf("\"%s\" must be followed by the name of a subcommand\n"
		  "List of subcommands:\n", (char*)(names->at(0)));
      for (i= 0; i < commands->count; i++)
	{
	  cmd= (class cl_cmd *)(commands->at(i));
	  con->printf("%s\n", cmd->short_help);
	}
      return(0);
    }
  if ((cmd= commands->get_cmd(cmdline)) == NULL)
    {
      con->printf("Undefined subcommand: \"%s\". Try \"help %s\".\n",
		  cmdline->name, (char*)(names->at(0)));
      return(0);
    }
  return(cmd->work(cmdline, con));
}


/*
 * Command console
 *____________________________________________________________________________
 */

cl_console::cl_console(char *fin, char *fout, class cl_sim *asim):
  cl_base()
{
  FILE *f;

  last_command= NULL;
  sim= asim;
  in= stdin;
  if (fin)
    if (f= fopen(fin, "r+"), in= f?f:stdin, !f)
      fprintf(stderr, "Can't open `%s': %s\n", fin, strerror(errno));
  out= stdout;
  if (fout)
    if (f= fopen(fout, "w+"), out= f?f:stdout, !f)
      fprintf(stderr, "Can't open `%s': %s\n", fout, strerror(errno));
  prompt= 0;
  flags= CONS_NONE;
}

cl_console::cl_console(FILE *fin, FILE *fout, class cl_sim *asim):
  cl_base()
{
  last_command= NULL;
  sim= asim;
  in = fin;
  out= fout;
  flags= CONS_NONE;
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

cl_console::cl_console(int portnumber, class cl_sim *asim)
{
  int sock= connect_to_port(portnumber);

  last_command= NULL;
  sim= asim;
  if (!(in= fdopen(sock, "r+")))
    fprintf(stderr, "cannot open port for input\n");
  if (!(out= fdopen(sock, "w+")))
    fprintf(stderr, "cannot open port for output\n");
  //fprintf(stderr, "init socket done\n");
}
#endif

int
cl_console::init(void)
{
  cl_base::init();
  welcome();
  flags&= ~CONS_PROMPT;
  print_prompt();
  return(0);
}

cl_console::~cl_console(void)
{
  if (in)
    fclose(in);
  if (out)
    {
      fprintf(out, "\n");
      fflush(out);
      fclose(out);
    }
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
  fprintf(out,
	  "ucsim %s, Copyright (C) 1997 Daniel Drotos, Talker Bt.\n"
	  "ucsim comes with ABSOLUTELY NO WARRANTY; for details type "
	  "`show w'.\n"
	  "This is free software, and you are welcome to redistribute it\n"
	  "under certain conditions; type `show c' for details.\n",
	  VERSIONSTR);
  fflush(out);
}

void
cl_console::print_prompt(void)
{
  char *p;

  if (flags & (CONS_PROMPT|CONS_FROZEN))
    return;
  flags|= CONS_PROMPT;
  if (!out)
    return;
  if (sim->arg_avail('P'))
    putc('\0', out);
  else
    fprintf(out, "%s", (prompt && prompt[0])?prompt:
	    ((p= sim->get_sarg(0, "prompt"))?p:"> "));
  fflush(out);
}

int
cl_console::printf(char *format, ...)
{
  va_list ap;
  int ret= 0;

  if (out)
    {
      va_start(ap, format);
      ret= cmd_do_print(out, format, ap);
      va_end(ap);
    }
  return(ret);
}

void
cl_console::print_bin(long data, int bits)
{
  long mask= 1;

  if (!out)
    return;
  mask= mask << ((bits >= 1)?(bits-1):0);
  while (bits--)
    {
      fprintf(out, "%c", (data&mask)?'1':'0');
      mask>>= 1;
    }
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
cl_console::proc_input(void)
{
  int retval;

  if (feof(in))
    {
      fprintf(out, "End\n");
      return(1);
    }
  char *cmd= read_line();
  if (!cmd)
    return(1);
  if (flags & CONS_FROZEN)
    {
      sim->stop(resUSER);
      flags&= ~CONS_FROZEN;
      retval= 0;
    }
  else
    retval= sim->do_cmd(cmd, this);
  if (!retval)
    print_prompt();
  free(cmd);
  return(retval);
}

/*
 * Old version, sim->do_cmd() falls into this if it doesn't find a new
 * command object which can handle enetered command
 */

int
cl_console::interpret(char *cmd)
{
  fprintf(out, "New interpreter does not known this command\n");
  return(0);
}


/*
 * This console listen on a socket and can accept connection requests
 */
#ifdef SOCKET_AVAIL

cl_listen_console::cl_listen_console(int serverport, class cl_sim *asim)
{
  last_command= NULL;
  sim= asim;
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
cl_listen_console::proc_input(void)
{
  int newsock;
  ACCEPT_SOCKLEN_T size;
  struct sockaddr_in sock_addr;

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
  sim->cmd->add_console(sim->cmd->mk_console(in, out, sim));
  in= out= 0;
  return(0);
}

#endif /* SOCKET_AVAIL */


/*
 * Command interpreter
 *____________________________________________________________________________
 */

cl_commander::cl_commander(class cl_sim *asim):
  cl_base()
{
  cons= new cl_list(1, 1); 
  actual_console= frozen_console= 0;
  sim= asim;
}

int
cl_commander::init(void)
{
  cl_base::init();
  if (!sim)
    return(1);
  if (sim->arg_avail('c'))
    add_console(mk_console(sim->get_sarg('c', 0),
			   sim->get_sarg('c', 0), sim));
#ifdef SOCKET_AVAIL
  if (sim->arg_avail('Z'))
    add_console(mk_console(sim->get_iarg(0, "Zport"), sim));
  if (sim->arg_avail('r'))
    add_console(mk_console(sim->get_iarg('r', 0), sim));
#endif
  if (cons->get_count() == 0)
    add_console(mk_console(stdin, stdout, sim));
  return(0);
}

cl_commander::~cl_commander(void)
{
  delete cons;
}

class cl_console *
cl_commander::mk_console(char *fin, char *fout, class cl_sim *asim)
{
  return(new cl_console(fin, fout, asim));
}

class cl_console *
cl_commander::mk_console(FILE *fin, FILE *fout, class cl_sim *asim)
{
  return(new cl_console(fin, fout, asim));
}

#ifdef SOCKET_AVAIL
class cl_console *
cl_commander::mk_console(int portnumber, class cl_sim *asim)
{
  return(new cl_listen_console(portnumber, asim));
}
#endif

void
cl_commander::add_console(class cl_console *console)
{
  console->init();
  cons->add(console);
  set_fd_set();
}

void
cl_commander::del_console(class cl_console *console)
{
  cons->disconn(console);
  set_fd_set();
}

void
cl_commander::set_fd_set(void)
{
  int i;
  
  FD_ZERO(&read_set);
  fd_num= 0;
  for (i= 0; i < cons->count; i++)
    {
      int fd;
      class cl_console *c= (class cl_console*)(cons->at(i));
      if ((fd= c->get_in_fd()) >= 0)
	{
	  if (fd > fd_num)
	    fd_num= fd;
	  FD_SET(fd, &read_set);
	}
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
      if (c->out)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(c->out, format, ap);
	  va_end(ap);
	}
    }
  return(ret);
}

int
cl_commander::all_print(char *string, int length)
{
  int i;
  
  for (i= 0; i < cons->count; i++)
    {
      class cl_console *c= (class cl_console*)(cons->at(i));
      if (c->out)
	{
	  for (int j= 0; j < length; j++)
	    putc(string[j], c->out);
	}
    }
  return(0);
}

/*
 * Printing to actual_console
 */

int
cl_commander::printf(char *format, ...)
{
  va_list ap;
  int ret= 0;

  if (actual_console &&
      actual_console->out)
    {
      va_start(ap, format);
      ret= cmd_do_print(actual_console->out, format, ap);
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
      if (c->out &&
	  c->flags & CONS_DEBUG)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(c->out, format, ap);
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
      if (c->out &&
	  (c->flags & iflags) == iflags)
	{
	  va_start(ap, format);
	  ret= cmd_do_print(c->out, format, ap);
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
  i= select(fd_num/*FD_SETSIZE*/, &active_set, NULL, NULL, &tv);
  return(i);
}

int
cl_commander::input_avail_on_frozen(void)
{
  if (!frozen_console)
    return(0);
  return(frozen_console->input_avail());
}

int
cl_commander::wait_input(void)
{
  int i;

  active_set= read_set;
  i= select(fd_num/*FD_SETSIZE*/, &active_set, NULL, NULL, NULL);
  return(i);
}

int
cl_commander::proc_input(void)
{
  int i;

  for (i= 0; i < fd_num/*FD_SETSIZE*/; i++)
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
		int retval= c->proc_input();
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


/* End of newcmd.cc */
