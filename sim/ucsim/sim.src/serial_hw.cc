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

/* $Id$ */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "globals.h"

#include "serial_hwcl.h"


cl_serial_hw::cl_serial_hw(class cl_uc *auc, int aid, chars aid_string):
  cl_hw(auc, HW_UART, aid, (const char *)aid_string)
{
  listener= 0;
  io= new cl_hw_io(this);
  io->init();
}

cl_serial_hw::~cl_serial_hw(void)
{
  delete serial_in_file_option;
  delete serial_out_file_option;
  delete io;
}

int
cl_serial_hw::init(void)
{
  char *s;

  cl_hw::init();
  
  input_avail= false;
  
  s= format_string("serial%d_in_file", id);
  serial_in_file_option= new cl_optref(this);
  serial_in_file_option->init();
  serial_in_file_option->use(s);
  free(s);
  s= format_string("serial%d_out_file", id);
  serial_out_file_option= new cl_optref(this);
  serial_out_file_option->init();
  serial_out_file_option->use(s);
  free(s);
  s= format_string("serial%d_port", id);
  serial_port_option= new cl_optref(this);
  serial_port_option->init();
  class cl_option *o= serial_port_option->use(s);
  free(s);
  if (o)
    {
      int port= serial_port_option->get_value((long)0);
      if (port > 0)
	listener= new cl_serial_listener(port, application, this);
      class cl_commander_base *c= application->get_commander();
      c->add_console(listener);
    }
  
  char *f_serial_in = (char*)serial_in_file_option->get_value((char*)0);
  char *f_serial_out= (char*)serial_out_file_option->get_value((char*)0);
  if (f_serial_in)
    {
      if (f_serial_in[0] == '\001')
	io->fin= (class cl_f *)(strtoll(&f_serial_in[1], 0, 0));
      else
	io->fin= mk_io(chars(f_serial_in), cchars("r"));
      if (!io->fin->tty)
	fprintf(stderr, "Warning: serial input interface connected to a "
		"non-terminal file.\n");
    }
  else
    io->fin= mk_io(chars(""), chars(""));
  if (f_serial_out)
    {
      if (f_serial_out[0] == '\001')
	io->fout= (class cl_f *)(strtoll(&f_serial_out[1], 0, 0));
      else
	io->fout= mk_io(chars(f_serial_out), "w");
      io->fout->set_terminal();
      if (!io->fout->tty)
	fprintf(stderr, "Warning: serial output interface connected to a "
		"non-terminal file.\n");
    }
  else
    io->fout= mk_io(chars(""), chars(""));

  if (io->fin)
    {
      io->fin->interactive(NULL);
      io->fin->raw();
      io->fin->echo(NULL);
      deb("** serial io fin %d\n", io->fin->file_id);
    }
  if (io->fout)
    {
      deb("** serial io fount %d\n", io->fout->file_id);
    }

  menu= 0;
  application->get_commander()->add_console(io);
  
  cfg_set(serconf_on, true);
  cfg_set(serconf_check_often, false);
  cfg_set(serconf_escape, 'x'-'a'+1);

  cl_var *v;
  chars pn(id_string);
  pn.append("%d_", id);
  uc->vars->add(v= new cl_var(pn+chars("on"), cfg, serconf_on));
  v->init();
  uc->vars->add(v= new cl_var(pn+chars("check_often"), cfg, serconf_check_often));
  v->init();
  uc->vars->add(v= new cl_var(pn+chars("esc_char"), cfg, serconf_escape));
  v->init();
		
  return 0;
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
    default:
      break;
    }
  return cell->get();
}

void
cl_serial_hw::new_io(class cl_f *f_in, class cl_f *f_out)
{
  if (io->fin)
    delete io->fin;
  if (io->fout)
    delete io->fout;
  io->fin= f_in;
  io->fout= f_out;
  if (io->fin)
    {
      io->fin->interactive(NULL);
      io->fin->raw();
      io->fin->echo(NULL);
    }
  if (io->fout)
    io->fout->set_terminal();
  application->get_commander()->update_active();
}

void
cl_serial_hw::proc_input(class cl_f *fi, class cl_f *fo)
{
  char c, esc= (char)cfg_get(serconf_escape);
  bool run= uc->sim->state & SIM_GO;
  
  if (fi->eof())
    {
      if (io->fout &&
	  (io->fout->file_id == io->fin->file_id))
	{
	  delete io->fout;
	  io->fout= mk_io("", "");
	}
      delete io->fin;
      io->fin= mk_io("", "");
      application->get_commander()->update_active();
      return;
    }
  if (menu == 0)
    {
      if (!input_avail || !run)
	{
	  if (fi->read(&c, 1))
	    {
	      if (c == esc)
		{
		  menu= 'm';
		  io->dd_printf("\n"
				"Simulator control menu\n"
				" %c      Insert ^%c\n"
				" s,r,g  Start simulation\n"
				" p      Stop simulation\n"
				" T      Reset CPU\n"
				" q      Quit simulator\n"
				" c      Close serial terminal\n"
				" e      Exit menu\n"
				,
				'a'+esc-1, 'a'+esc-1
				);
		}
	      else if (run)
		{
		  input= c;
		  input_avail= true;
		}
	    }
	}
    }
  else
    {
      if (fi->read(&c, 1))
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
		      io->dd_printf("^%c interted.\n", 'a'+esc-1);
		    }
		  else
		    io->dd_printf("Control menu exited.\n");
		  menu= 0;
		}
	      switch (c)
		{
		case 'z': case 'z'-'a'-1: case 'Z':
		  break;
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
		  io->dd_printf("CPU reseted.\n");
		  break;
		case 'q': case 'Q': case 'q'-'a'+1:
		  // kill
		  uc->sim->state|= SIM_QUIT;
		  menu= 0;
		  io->dd_printf("Exit simulator.\n");
		  break;
		case 'c': case 'C': case 'c'-'a'+1:
		  // close
		  menu= 0;
		  io->dd_printf("Closing terminal.\n");
		  if (io->fout)
		    {
		      delete io->fout;
		      io->fout= mk_io("", "");
		    }
		  if (io->fin)
		    {
		      delete io->fin;
		      io->fin= mk_io("", "");
		    }
		  break;
		default:
		  menu= 0;
		  io->dd_printf("Control menu closed (%d).\n", c);
		  break;
		}
	      break;
	    }
	}
    }
}


cl_serial_listener::cl_serial_listener(int serverport, class cl_app *the_app,
				       class cl_serial_hw *the_serial):
  cl_listen_console(serverport, the_app)
{
  serial_hw= the_serial;
}

int
cl_serial_listener::proc_input(class cl_cmdset *cmdset)
{
  class cl_f *i, *o;

  srv_accept(fin, &i, &o);
  i->set_telnet(true);
  serial_hw->new_io(i, o);
  return 0;
}


/* End of sim.src/serial_hw.cc */
