/*
 * Simulator of microcontrollers (cmd51cl.h)
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

#ifndef CMD51CL_HEADER
#define CMD51CL_HEADER

#include "ddconfig.h"

// cmd
#include "newcmdcl.h"

// local
#include "cmd.h"
#include "simcl.h"


class cl_51cons: public cl_console
{
public:
  cl_51cons(char *fin, char *fout, class cl_sim *asim);
  cl_51cons(FILE *fin, FILE *fout, class cl_sim *asim);
#ifdef SOCKET_AVAIL
  cl_51cons(int portnumber, class cl_sim *asim);
#endif
  virtual int proc_input(void);
  virtual bool interpret(char *cmd);
  virtual bool old_command(class cl_cmdline *cmdline);
};

class cl_51cmd: public cl_commander
{
public:
  cl_51cmd(class cl_sim *asim);
  virtual class cl_console *mk_console(char *fin, char *fout,
				       class cl_sim *asim);
  virtual class cl_console *mk_console(FILE *fin, FILE *fout,
				       class cl_sim *asim);
};


#endif

/* End of s51.src/cmd51cl.h */
