/*
 * Simulator of microcontrollers (argcl.h)
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

#ifndef ARGCL_HEADER
#define ARGCL_HEADER

#include "pobjcl.h"


/*
 * Base type of arguments/parameters
 */

class cl_arg: public cl_base
{
public:
  union {
    long long i_value;
    double f_value;
    void *p_value;
  };
  char *s_value;

public:  
  cl_arg(long long lv);
  cl_arg(char *lv);
  cl_arg(double fv);
  cl_arg(void *pv);
  ~cl_arg(void);

  virtual long long get_ivalue(void);
  virtual char *get_svalue(void);
  virtual double get_fvalue(void);
  virtual void *get_pvalue(void);
};


/*
 * Command parameters
 */

class cl_cmd_arg: public cl_arg
{
public:
  cl_cmd_arg(long long i): cl_arg(i) {}
  cl_cmd_arg(char *s): cl_arg(s) {}

  virtual int is_string(void) { return(0); }
  virtual long get_address(void) { return(-1); }
};

class cl_cmd_int_arg: public cl_cmd_arg
{
public:
  cl_cmd_int_arg(long long addr);

  virtual long get_address(void) { return(get_ivalue()); }
};

class cl_cmd_sym_arg: public cl_cmd_arg
{
public:
  cl_cmd_sym_arg(char *sym);

  virtual long get_address(void);
};

class cl_cmd_str_arg: public cl_cmd_arg
{
public:
  cl_cmd_str_arg(char *str);

  virtual int is_string(void) { return(1); }
};

class cl_cmd_bit_arg: public cl_cmd_arg
{
public:
  class cl_cmd_arg *sfr, *bit;

public:
  cl_cmd_bit_arg(class cl_cmd_arg *asfr, class cl_cmd_arg *abit);
  ~cl_cmd_bit_arg(void);

  virtual long get_address(void);
};


/*
 * Program arguments
 */

class cl_prg_arg: public cl_arg
{
public:
  char short_name;
  char *long_name;

public:
  cl_prg_arg(char sn, char *ln, long long lv);
  cl_prg_arg(char sn, char *ln, char *lv);
  cl_prg_arg(char sn, char *ln, double fv);
  cl_prg_arg(char sn, char *ln, void *pv);
  ~cl_prg_arg(void);
};


#endif

/* End of argcl.h */
