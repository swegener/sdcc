/*
 * Simulator of microcontrollers (app.cc)
 *
 * Copyright (C) 2001,01 Drotos Daniel, Talker Bt.
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

// prj
#include "i_string.h"

// sim.src
#include "simcl.h"

// local
#include "appcl.h"


/*
 * Program options
 */

/*cl_option::cl_option(int atype, char sn, char *ln)
{
  type= atype;
  short_name= sn;
  if (!ln)
    long_name= NULL;
  else
    long_name= strdup(ln);
  values= new cl_ustrings(1, 1);
}

cl_option::~cl_option(void)
{
  if (long_name)
    free(long_name);
  delete values;
}

int
cl_option::add_value(char *value)
{
  values->add(value);
  return(values->count - 1);
}

char *
cl_option::get_value(int index)
{
  if (index > values->count - 1)
    return(0);
  return((char*)(values->at(index)));
}*/

/* List of options */

/*cl_options::cl_options(void):
  cl_list(2, 2)
{
}*/


/*
 * Application
 ****************************************************************************
 */

cl_app::cl_app(void)
{
  //options= new cl_options();
  sim= 0;
}

cl_app::~cl_app(void)
{
  //delete options;
  remove_simulator();
}

int
cl_app::init(void)
{
  cl_base::init();

  return(0);
}

/* Main cycle */

int
cl_app::run(void)
{
  return(0);
}

void
cl_app::done(void)
{
}


/* Command handling */

class cl_cmd *
cl_app::get_cmd(class cl_cmdline *cmdline)
{
  return(0);
}


/* Adding and removing comonents */

void
cl_app::set_simulator(class cl_sim *simulator)
{
  if (sim)
    remove_simulator();
  sim= simulator;
  
}

void
cl_app::remove_simulator(void)
{
  if (!sim)
    return;
  delete sim;
  sim= 0;
}

void
cl_app::build_cmdset(class cl_cmdset *cs)
{
  
}


/* End of sim.src/app.cc */
