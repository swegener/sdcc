/*
 * Simulator of microcontrollers (cmd.src/cmdsetcl.h)
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

#ifndef CMD_CMDSETCL_HEADER
#define CMD_CMDSETCL_HEADER

#include "newcmdcl.h"

// STATE
COMMAND(cl_state_cmd);
COMMAND(cl_file_cmd);
COMMAND(cl_dl_cmd);
COMMAND(cl_run_cmd);
COMMAND(cl_stop_cmd);
COMMAND(cl_step_cmd);
COMMAND(cl_next_cmd);
COMMAND(cl_pc_cmd);
COMMAND(cl_reset_cmd);
COMMAND(cl_dump_cmd);
COMMAND_ANCESTOR(cl_di_cmd,cl_dump_cmd);
COMMAND_ANCESTOR(cl_dx_cmd,cl_dump_cmd);
COMMAND_ANCESTOR(cl_dch_cmd,cl_dump_cmd);
COMMAND_ANCESTOR(cl_ds_cmd,cl_dump_cmd);
COMMAND_DATA(cl_dc_cmd, t_addr last);
COMMAND_DATA(cl_disassemble_cmd,int last);
COMMAND_DATA(cl_fill_cmd,int last);
COMMAND_HEAD(cl_where_cmd)
  public: int last;
COMMAND_METHODS(cl_where_cmd)
  virtual int do_real_work(class cl_sim *sim,
			   class cl_cmdline *cmdline, class cl_console *con,
			   bool case_sensitive);
COMMAND_TAIL;
COMMAND_DATA_ANCESTOR(cl_Where_cmd,cl_where_cmd,int last);
COMMAND(cl_help_cmd);
COMMAND(cl_quit_cmd);
COMMAND(cl_kill_cmd);

COMMAND_HEAD(cl_timer_cmd)
public:
  class cl_ticker *ticker;
  long what;
  char *name;
COMMAND_METHODS(cl_timer_cmd)
  void set_ticker(class cl_sim *sim,
		  class cl_cmd_arg *param);
  virtual int add(class cl_sim *sim,
		  class cl_cmdline *cmdline, class cl_console *con);
  virtual int del(class cl_sim *sim,
		  class cl_cmdline *cmdline, class cl_console *con);
  virtual int get(class cl_sim *sim,
		  class cl_cmdline *cmdline, class cl_console *con);
  virtual int run(class cl_sim *sim,
		  class cl_cmdline *cmdline, class cl_console *con);
  virtual int stop(class cl_sim *sim,
		   class cl_cmdline *cmdline, class cl_console *con);
  virtual int val(class cl_sim *sim,
		  class cl_cmdline *cmdline, class cl_console *con);
COMMAND_TAIL;


#endif

/* End of cmd.src/cmdsetcl.h */
