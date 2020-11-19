/*
 * Simulator of microcontrollers (sim.src/simcl.h)
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

#ifndef SIM_SIMCL_HEADER
#define SIM_SIMCL_HEADER

#include <stdio.h>

// prj
#include "pobjcl.h"

// cmd
#include "newcmdcl.h"

// gui
#include "guicl.h"

// local
#include "uccl.h"
#include "argcl.h"

struct t_hist_elem
{
  int nr;
  t_addr addr;
};

class cl_exec_hist: public cl_base
{
protected:
  int len;
  int ff, lu;
  struct t_hist_elem *hist;
  class cl_uc *uc;
public:
  cl_exec_hist(class cl_uc *auc);
  virtual ~cl_exec_hist(void);
  virtual int init(void);
};
  
class cl_sim: public cl_base
{
public:
  class cl_app *app;
  int state; // See SIM_XXXX
  int argc; char **argv;

  //class cl_commander *cmd;
  class cl_uc *uc;
  class cl_gui *gui;
  class cl_hw *simif;
  class cl_exec_hist *hist;
  
  double start_at, stop_at;
  unsigned long start_tick;
  unsigned long steps_done;
  unsigned long steps_todo; // use this if not 0
  
public:
  cl_sim(class cl_app *the_app);
  virtual ~cl_sim(void);
  virtual int init(void);
  
  virtual class cl_uc *mk_controller(void);
  virtual void build_cmdset(class cl_cmdset *cmdset);

  virtual class cl_uc *get_uc(void) { return(uc); }

  virtual void start(class cl_console_base *con, unsigned long steps_to_do);
  virtual void stop(int reason, class cl_ev_brk *ebrk= NULL);
  virtual void change_run(int reason= resSIMIF);
  //virtual void stop(class cl_ev_brk *brk);
  virtual int step(void);
};


#endif

/* End of simcl.h */
