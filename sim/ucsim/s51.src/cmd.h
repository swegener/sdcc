/*
 * Simulator of microcontrollers (cmd.h)
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

#ifndef CMD_HEADER
#define CMD_HEADER

#include "ddconfig.h"

#include <stdio.h>


/* Type of entries in command table */

struct cmd_entry
{
  // Name of the command
  char *name;
  // Function to execute
  bool (*func)(char *cmd, /*class t_uc51*/void *uc, class cl_sim *sim);
  // Command can be repeated by empty command
  bool can_repeat;
  // Help about the command
  char *help;
};

extern struct cmd_entry cmd_table[];


#endif

/* End of s51.src/cmd.h */
