/*
 * Simulator of microcontrollers (ucsim.cc)
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

// prj
#include "globals.h"
#include "appcl.h"

#include "charscl.h"

class cl_general_uc: public cl_uc
{
public:
  cl_general_uc(cl_sim *s): cl_uc(s) {}
};

class cl_general_sim: public cl_sim
{
public:
  cl_general_sim(cl_app *a): cl_sim(a) {}
  virtual class cl_uc *mk_constroller(void)
  {
    return new cl_general_uc(this);
  }
};

int
main(int argc, char *argv[])
{
  int ret;
  class cl_sim *sim;
  
  app_start_at= dnow();
  application= new cl_app();
  application->set_name("ucsim");
  application->init(argc, argv);
  sim= new cl_general_sim(application);
  if (sim->init())
    sim->state|= SIM_QUIT;
  application->set_simulator(sim);
  ret= application->run();
  application->done();
  return(ret);
}

/* End of sim.src/ucsim.cc */
