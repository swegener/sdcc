/*
 * Simulator of microcontrollers (arg.cc)
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
#include <stdlib.h>
#include "i_string.h"

// prj
#include "globals.h"

// sim
#include "simcl.h"

// cmd
#include "cmdutil.h"

// local
#include "argcl.h"


/*
 * Making the argument
 */

cl_arg::cl_arg(long long lv):
  cl_base()
{
  i_value= lv;
  s_value= 0;
}

cl_arg::cl_arg(char *sv):
  cl_base()
{
  s_value= sv?strdup(sv):0;
}

cl_arg::cl_arg(double fv):
  cl_base()
{
  f_value= fv;
  s_value= 0;
}

cl_arg::cl_arg(void *pv):
  cl_base()
{
  p_value= pv;
  s_value= 0;
}

cl_arg::~cl_arg(void)
{
  if (s_value)
    free(s_value);
}


/*
 * Getting value of the argument
 */

long long
cl_arg::get_ivalue(void)
{
  return(i_value);
}

char *
cl_arg::get_svalue(void)
{
  return(s_value);
}

double
cl_arg::get_fvalue(void)
{
  return(f_value);
}

void *
cl_arg::get_pvalue(void)
{
  return(p_value);
}


/*
 * Command parameters
 *----------------------------------------------------------------------------
 */

cl_cmd_int_arg::cl_cmd_int_arg(long long addr):
  cl_cmd_arg(addr)
{}

cl_cmd_sym_arg::cl_cmd_sym_arg(char *sym):
  cl_cmd_arg(sym)
{}

long
cl_cmd_sym_arg::get_address(void)
{
  struct name_entry *ne;

  if ((ne= get_name_entry(simulator->uc->sfr_tbl(),
			  get_svalue(),
			  simulator->uc)) != NULL)
    {
      return(ne->addr);
    }
  return(-1);
}

cl_cmd_str_arg::cl_cmd_str_arg(char *str):
  cl_cmd_arg(str)
{}

cl_cmd_bit_arg::cl_cmd_bit_arg(class cl_cmd_arg *asfr, class cl_cmd_arg *abit):
  cl_cmd_arg((long long)0)
{
  sfr= asfr;
  bit= abit;
}

cl_cmd_bit_arg::~cl_cmd_bit_arg(void)
{
  if (sfr)
    delete sfr;
  if (bit)
    delete bit;
}

long
cl_cmd_bit_arg::get_address(void)
{
  if (sfr)
    return(sfr->get_address());
  return(-1);
}


/*
 * Program arguments
 *----------------------------------------------------------------------------
 */

cl_prg_arg::cl_prg_arg(char sn, char *ln, long long lv):
  cl_arg(lv)
{
  short_name= sn;
  long_name = ln?strdup(ln):0;
}

cl_prg_arg::cl_prg_arg(char sn, char *ln, char *sv):
  cl_arg(sv)
{
  short_name= sn;
  long_name = ln?strdup(ln):0;
}

cl_prg_arg::cl_prg_arg(char sn, char *ln, double fv):
  cl_arg(fv)
{
  short_name= sn;
  long_name = ln?strdup(ln):0;
}

cl_prg_arg::cl_prg_arg(char sn, char *ln, void *pv):
  cl_arg(pv)
{
  short_name= sn;
  long_name = ln?strdup(ln):0;
}

cl_prg_arg::~cl_prg_arg(void)
{
  if (long_name)
    free(long_name);
}


/* End of arg.cc */
