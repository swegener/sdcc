/*
 * Simulator of microcontrollers (sim.src/serial_hw.cc)
 *
 * Copyright (C) 2016,16 Drotos Daniel, Talker Bt.
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "globals.h"
//#include "fiocl.h"

#include "serial_hwcl.h"


bool
cl_serial_io::input_avail(void)
{
  return cl_hw_io::input_avail();
}


cl_serial_hw::cl_serial_hw(class cl_uc *auc, int aid, chars aid_string):
  cl_hw(auc, HW_UART, aid, (const char *)aid_string)
{
  listener_io= listener_i= listener_o= NULL;
}

cl_serial_hw::~cl_serial_hw(void)
{
  delete serial_in_file_option;
  delete serial_out_file_option;
  delete serial_ifirst_option;
  delete io;
}

int
cl_serial_hw::init(void)
{
  chars cs;
  bool raw= false;
  
  cl_hw::init();
  
  make_io();
  input_avail= false;
  
  cs.format("serial%d_in_file", id);
  serial_in_file_option= new cl_optref(this, cs.c_str());
  cs.format("serial%d_out_file", id);
  serial_out_file_option= new cl_optref(this, cs.c_str());
  cs.format("serial%d_ifirst", id);
  serial_ifirst_option= new cl_optref(this, cs.c_str());
  cs.format("serial%d_raw", id);
  serial_raw_option= new cl_optref(this, cs.c_str());
  serial_raw_option->get_value(&raw);
  
  cs.format("serial%d_port", id);
  serial_port_option= new cl_optref(this, cs.c_str());

  long port= -1;
  if (serial_port_option->get_value(&port))
    {
      if (port < 0) {}
      if (port > 0)
	{
	  listener_io= new cl_serial_listener(port, application, this, sl_io);
	  listener_io->init();
	  class cl_commander_base *c= application->get_commander();
	  c->add_console(listener_io);
	}
    }
  
  cs.format("serial%d_iport", id);
  serial_iport_option= new cl_optref(this, cs.c_str());
  port= -1;
  if (serial_iport_option->get_value(&port))
    {
      if (port < 0) {}
      if (port > 0)
	{
	  listener_i= new cl_serial_listener(port, application, this, sl_i);
	  listener_i->init();
	  class cl_commander_base *c= application->get_commander();
	  c->add_console(listener_i);
	}
    }

  cs.format("serial%d_oport", id);
  serial_oport_option= new cl_optref(this, cs.c_str());
  port= -1;
  if (serial_oport_option->get_value(&port))
    {
      if (port < 0) {}
      if (port > 0)
	{
	  listener_o= new cl_serial_listener(port, application, this, sl_o);
	  listener_o->init();
	  class cl_commander_base *c= application->get_commander();
	  c->add_console(listener_o);
	}
    }
  
  const char *f_serial_in = serial_in_file_option->get_value("");
  const char *f_serial_out= serial_out_file_option->get_value("");
  bool ifirst= false;
  ifirst= serial_ifirst_option->get_value(ifirst);
  class cl_f *fi, *fo;

  if (ifirst)
    {
      if (f_serial_in && *f_serial_in)
	{
	  if (f_serial_in[0] == '\001')
	    fi= (class cl_f *)(strtoll(&f_serial_in[1], 0, 0));
	  else
	    fi= mk_io(f_serial_in, "r");
	  if (!fi->tty && !raw)
	    fprintf(stderr, "Warning: serial input interface connected to a "
		    "non-terminal file.\n");
	}
      else
	fi= 0;
      if (f_serial_out && *f_serial_out)
	{
	  if (f_serial_out[0] == '\001')
	    fo= (class cl_f *)(strtoll(&f_serial_out[1], 0, 0));
	  else
	    fo= mk_io(chars(f_serial_out), "w");
	  if (!fo->tty && !raw)
	    fprintf(stderr, "Warning: serial output interface connected to a "
		    "non-terminal file.\n");
	}
      else
	fo= 0;
    }
  else
    {
      if (f_serial_out && *f_serial_out)
	{
	  if (f_serial_out[0] == '\001')
	    fo= (class cl_f *)(strtoll(&f_serial_out[1], 0, 0));
	  else
	    fo= mk_io(chars(f_serial_out), "w");
	  if (!fo->tty && !raw)
	    fprintf(stderr, "Warning: serial output interface connected to a "
		    "non-terminal file.\n");
	}
      else
	fo= 0;
      if (f_serial_in && *f_serial_in)
	{
	  if (f_serial_in[0] == '\001')
	    fi= (class cl_f *)(strtoll(&f_serial_in[1], 0, 0));
	  else
	    fi= mk_io(f_serial_in, "r");
	  if (!fi->tty && !raw)
	    fprintf(stderr, "Warning: serial input interface connected to a "
		    "non-terminal file.\n");
	}
      else
	fi= 0;
    }
  
  io->replace_files(true, fi, fo);
  
  if (fi)
    {
      if (!raw)
	fi->interactive(NULL);
      fi->raw();
      fi->echo(NULL);
    }

  menu= 0;
  
  cfg_set(serconf_on, true);
  cfg_set(serconf_check_often, false);
  cfg_set(serconf_escape, 'x'-'a'+1);

  chars pn("", "%s%d_", id_string, id);
  uc->vars->add(pn+"on", cfg, serconf_on, cfg_help(serconf_on));
  uc->vars->add(pn+"check_often", cfg, serconf_check_often, cfg_help(serconf_check_often));
  uc->vars->add(pn+"esc_char", cfg, serconf_escape, cfg_help(serconf_escape));
  uc->vars->add(pn+"received_char", cfg, serconf_received, cfg_help(serconf_received));
  uc->vars->add(pn+"flowctrl", cfg, serconf_flowctrl, cfg_help(serconf_flowctrl));
  uc->vars->add(pn+"able_receive", cfg, serconf_able_receive, cfg_help(serconf_able_receive));

  cfg_set(serconf_able_receive, 1);
  return 0;
}

const char *
cl_serial_hw::cfg_help(t_addr addr)
{
  switch (addr)
    {
    case serconf_on:
      return "Turn ticking of UART on/off (bool, RW)";
    case serconf_check_often:
      return "Check input file at every cycle (bool, RW)";
    case serconf_escape:
      return "Escape char on display (int, RW)";
    case serconf_common:
      return "Not used";
    case serconf_received:
      return "Received char written by simulator (int, R)";
    case serconf_flowctrl:
      return "Flow-control simulation on/off (bool, RW)";
    case serconf_able_receive:
      return "UART enabled to receive by flow-control (bool, RW)";
    }
  return "Not used";
}

void
cl_serial_hw::set_cmd(class cl_cmdline *cmdline,
		      class cl_console_base *con)
{
  class cl_cmd_arg *params[2]= {
    cmdline->param(0),
    cmdline->param(1)
  };
  class cl_commander_base *c= application->get_commander();
  if (cmdline->syntax_match(uc, STRING NUMBER))
    {
      char *p1= params[0]->value.string.string;
      int port= params[1]->value.number;
      if (strcmp(p1, "port")==0)
	{
	  del_listener_i();
	  del_listener_o();
	  if ((port > 0) && c)
	    {
	      listener_io= new cl_serial_listener(port, application, this, sl_io);
	      listener_io->init();
	      c->add_console(listener_io);
	    }
	}
      if (strcmp(p1, "iport")==0)
	{
	  del_listener_i();
	  if ((port > 0) && c)
	    {
	      listener_i= new cl_serial_listener(port, application, this, sl_i);
	      listener_i->init();
	      c->add_console(listener_i);
	    }
	}
      if (strcmp(p1, "oport")==0)
	{
	  del_listener_o();
	  if ((port > 0) && c)
	    {
	      listener_o= new cl_serial_listener(port, application, this, sl_o);
	      listener_o->init();
	      c->add_console(listener_o);
	    }
	}
    }
  else if (cmdline->syntax_match(uc, STRING STRING))
    {
      class cl_f *fi, *fo;
      char *p1= params[0]->value.string.string;
      const char *p2= params[1]->value.string.string;
      if (strcmp(p1, "file")==0)
	{
	  if (p2 && *p2)
	    {
	      fi= mk_io(p2, "r");
	      fo= mk_io(p2, "w");
	      new_io(fi, fo);
	    }
	}
      if (strcmp(p1, "in")==0)
	{
	  if (p2 && *p2)
	    {
	      fi= mk_io(p2, "r");
	      if (fi->opened())
		new_i(fi);
	      else
		con->dd_cprintf("error", "Error opening file \"%s\"\n",p2);
	    }
	}
      if (strcmp(p1, "out")==0)
	{
	  if (p2 && *p2)
	    {
	      fo= mk_io(p2, "w");
	      new_o(fo);
	    }
	}
      if (strcmp(p1, "raw")==0)
	{
	  serial_raw_option->option->set_value(p2);
	}
    }
  else
    {
      con->dd_printf("set hardware uart[%d] raw   0|1\n", id);
      con->dd_printf("set hardware uart[%d] file  \"file\"\n", id);
      con->dd_printf("set hardware uart[%d] in    \"file\"\n", id);
      con->dd_printf("set hardware uart[%d] out   \"file\"\n", id);
      con->dd_printf("set hardware uart[%d] port  nr\n", id);
      con->dd_printf("set hardware uart[%d] iport nr\n", id);
      con->dd_printf("set hardware uart[%d] oport nr\n", id);
    }
}

t_mem
cl_serial_hw::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  switch ((enum serial_cfg)addr)
    {
    case serconf_on: // turn this HW on/off
      if (val)
	{
	  if (*val)
	    on= true;
	  else
	    on= false;
	}
      else
	{
	  cell->set(on?1:0);
	}
      break;
    case serconf_check_often:
      if (val)
	{
	  cell->set(*val?1:0);
	}
      break;
    case serconf_escape:
      if (val)
	{
	  char c= tolower(*val);
	  if ((c >= 'a') &&
	      (c <= 'z'))
	    cell->set(c - 'a'+1);
	}
    default:
      break;
    }
  return cell->get();
}

void
cl_serial_hw::make_io()
{
  if (!io)
    {
      io= new cl_serial_io(this);
      application->get_commander()->add_console(io);
    }
}

void
cl_serial_hw::new_io(class cl_f *f_in, class cl_f *f_out)
{
  //bool raw= false;
  //serial_raw_option->get_value(&raw);
  make_io();
  if (!io)
    return;
  new_o(f_out);
  new_i(f_in);
}

void
cl_serial_hw::new_i(class cl_f *f_in)
{
  bool raw= false;

  serial_raw_option->get_value(&raw);
  io->replace_files(true, f_in, io->get_fout());
  if (!f_in)
    return;
  if (!raw)
    f_in->interactive(NULL);
  f_in->set_telnet(!raw);
  f_in->raw();
  if (f_in->tty)
    {
      char esc= (char)cfg_get(serconf_escape);
      io->tu_reset();
      io->dd_printf("%s[%d] terminal display, press ^%c to access control menu\n",
		    id_string, id,
		    'a'+esc-1);
    }
  menu= 0;
}

void
cl_serial_hw::new_o(class cl_f *f_out)
{
  bool raw= false;

  serial_raw_option->get_value(&raw);
  io->replace_files(true, io->get_fin(), f_out);
  if (!f_out)
    return;
  enum file_type ft= f_out->determine_type();
  if ((ft != F_FILE) && (ft != F_CHAR))
    {
      f_out->set_telnet(!raw);
      if (!raw)
	{
	  io->tu_reset();
	}
    }
}

void
cl_serial_hw::del_listener_i(void)
{
  class cl_commander_base *c= application->get_commander();
  if (c && listener_io)
    c->del_console(listener_io);
  if (listener_io)
    {
      if (listener_i == listener_io)
	listener_i= NULL;
      if (listener_o == listener_io)
	listener_o= NULL;
      delete listener_io;
    }
  listener_io= NULL;
  if (c && listener_i)
    c->del_console(listener_i);
  if (listener_i)
    delete listener_i;
  listener_i= NULL;
}

void
cl_serial_hw::del_listener_o(void)
{
  class cl_commander_base *c= application->get_commander();
  if (c && listener_io)
    c->del_console(listener_io);
  if (listener_io)
    {
      if (listener_i == listener_io)
	listener_i= NULL;
      if (listener_o == listener_io)
	listener_o= NULL;
      delete listener_io;
    }
  listener_io= NULL;
  if (c && listener_o)
    c->del_console(listener_o);
  if (listener_o)
    delete listener_o;
  listener_o= NULL;
}

bool
cl_serial_hw::proc_input(void)
{
  int c;
  char esc= (char)cfg_get(serconf_escape);
  bool run= uc->sim->state & SIM_GO;
  class cl_f *fin, *fout;
  int flw= cfg_get(serconf_flowctrl);
  int able= cfg_get(serconf_able_receive);

  fin= io->get_fin();
  fout= io->get_fout();

  if (fin->eof())
    {
      if (fout &&
	  (fout->file_id == fin->file_id))
	{
	  delete fout;
	  io->replace_files(false, fin, 0);
	  fout= 0;
	}
      delete fin;
      io->replace_files(false, 0, fout);
      return true;
    }
  if (menu == 0)
    {
      if (fin->tty && !flw)
	{
	  if (fin->read(&c, 1))
	    {
	      if (c == esc)
		{
		  menu= 'm';
		  io->dd_printf("\n");
		  io->dd_cprintf("ui_title", "Simulator control menu\n");
		  io->dd_cprintf("ui_mkey", " %c      ", 'a'+esc-1);
		  io->dd_cprintf("ui_mitem", "Insert ^%c\n", 'a'+esc-1);
		  io->dd_cprintf("ui_mkey", " s,r,g  ");
		  io->dd_cprintf("ui_mitem", "Start simulation\n");
		  io->dd_cprintf("ui_mkey", " p      ");
		  io->dd_cprintf("ui_mitem", "Stop simulation\n");
		  io->dd_cprintf("ui_mkey", " T      ");
		  io->dd_cprintf("ui_mitem", "Reset CPU\n");
		  io->dd_cprintf("ui_mkey", " q      ");
		  io->dd_cprintf("ui_mitem", "Quit simulator\n");
		  io->dd_cprintf("ui_mkey", " o      ");
		  io->dd_cprintf("ui_mitem", "Close serial terminal\n");
		  io->dd_cprintf("ui_mkey", " e      ");
		  io->dd_cprintf("ui_mitem", "Exit menu\n");
		  io->dd_cprintf("ui_mkey", " n      ");
		  io->dd_cprintf("ui_mitem", "Change display\n");
		}
	      else if (!input_avail)
		{
		  input= c;
		  input_avail= true;
		}
	    }
	}
      else if (!input_avail)
	{
	  if (!flw ||
	      able)
	    {
	      if (fin->read(&c, 1))
		{
		  input= c;
		  input_avail= true;
		  cfg_set(serconf_able_receive, 0);
		}
	    }
	}
    }
  else
    {
      if (fin->read(&c, 1))
	{
	  switch (menu)
	    {
	    case 'm':
	      if ((c == esc-1+'a') ||
		  (c == esc-1+'A') ||
		  (c == esc))
		{
		  // insert ^esc
		  if (run && !input_avail)
		    {
		      input= esc, input_avail= true;
		      io->dd_printf("^%c entered.\n", 'a'+esc-1);
		    }
		  else
		    io->dd_printf("Control menu exited.\n");
		  menu= 0;
		}
	      switch (c)
		{
		case 'e': case 'E': case 'e'-'a'+1:
		  // exit menu
		  menu= 0;
		  io->dd_printf("Control menu exited.\n");
		  break;
		case 's': case 'S': case 's'-'a'+1:
		case 'r': case 'R': case 'r'-'a'+1:
		case 'g': case 'G': case 'g'-'a'+1:
		  // start
		  uc->sim->start(0, 0);
		  menu= 0;
		  io->dd_printf("Simulation started.\n");
		  break;
		case 'p': case 'P': case 'p'-'a'+1:
		  uc->sim->stop(resSIMIF);
		  // stop
		  menu= 0;
		  io->dd_printf("Simulation stopped.\n");
		  break;
		case 'T':
		  uc->reset();
		  menu= 0;
		  io->dd_printf("CPU reset.\n");
		  break;
		case 'q': case 'Q': case 'q'-'a'+1:
		  // kill
		  uc->sim->state|= SIM_QUIT;
		  menu= 0;
		  io->dd_printf("Exit simulator.\n");
		  break;
		case 'o': case 'O': case 'o'-'a'+1:
		  {
		    // close
		    io->dd_printf("Closing terminal.\n");
		    menu= 0;
		    io->convert2console();
		    break;
		  }
		case 'n': case 'N': case 'n'-'a'+1:
		  {
		    class cl_hw *h= next_displayer();
		    if (!h)
		      io->dd_printf("No other displayer.\n");
		    else
		      {
			io->tu_reset();
			io->tu_cls();
			io->pass2hw(h);
		      }
		    menu= 0;
		    break;
		  }
		default:
		  menu= 0;
		  io->dd_printf("Control menu closed (%d).\n", c);
		  break;
		}
	      break;
	    }
	}
    }
  return true;
}

void
cl_serial_hw::reset(void)
{
  cfg_set(serconf_able_receive, 1);
}

cl_serial_listener::cl_serial_listener(int serverport, class cl_app *the_app,
				       class cl_serial_hw *the_serial,
				       enum ser_listener_for slf):
  cl_listen_console(serverport, the_app)
{
  serial_hw= the_serial;
  sl_for= slf;
}

int
cl_serial_listener::init(void)
{
  if (serial_hw)
    set_name(chars("", "serial_listener_%s_%d\n", serial_hw->get_name(), serial_hw->id));
  return 0;
}

int
cl_serial_listener::proc_input(class cl_cmdset *cmdset)
{
  class cl_f *i, *o;
  bool raw= false;

  serial_hw->serial_raw_option->get_value(&raw);
  switch (sl_for)
    {
    case sl_io:
      srv_accept(fin, &i, &o);
      serial_hw->new_io(i, o);
      break;
    case sl_i:
      srv_accept(fin, &i, NULL);
      serial_hw->new_i(i);
      break;
    case sl_o:
      srv_accept(fin, NULL, &o);
      serial_hw->new_o(o);
      break;
    }
  return 0;
}


/* End of sim.src/serial_hw.cc */
