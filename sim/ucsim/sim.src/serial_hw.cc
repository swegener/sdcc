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
  fin= 0;
  fout= 0;
  listener= 0;
}

cl_serial_hw::~cl_serial_hw(void)
{
  if (fout)
    delete fout;
  if (fin)
    delete fin;
  delete serial_in_file_option;
  delete serial_out_file_option;
}

int
cl_serial_hw::init(void)
{
  char *s;

  cl_hw::init();

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
	fin= (class cl_f *)(strtoll(&f_serial_in[1], 0, 0));
      else
	fin= mk_io(chars(f_serial_in), cchars("r"));
      //fin->save_attributes();
      fin->set_terminal();
      if (!fin->tty)
	fprintf(stderr, "Warning: serial input interface connected to a "
		"non-terminal file.\n");
    }
  else
    fin= mk_io(chars(""), chars(""));
  if (f_serial_out)
    {
      if (f_serial_out[0] == '\001')
	fout= (class cl_f *)(strtoll(&f_serial_out[1], 0, 0));
      else
	fout= mk_io(chars(f_serial_out), "w");
      //fout->save_attributes();
      fout->set_terminal();
      if (!fout->tty)
	fprintf(stderr, "Warning: serial output interface connected to a "
		"non-terminal file.\n");
    }
  else
    fout= mk_io(chars(""), chars(""));

  return 0;
}

void
cl_serial_hw::new_io(class cl_f *f_in, class cl_f *f_out)
{
  if (fin)
    delete fin;
  if (fout)
    delete fout;
  fin= f_in;
  fout= f_out;
  fin->set_terminal();
  fout->set_terminal();
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
  serial_hw->new_io(i, o);
  return 0;
}


/* End of sim.src/serial_hw.cc */
