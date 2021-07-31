/*
 * Simulator of microcontrollers (r3kacl.h)
 *
 * Copyright (C) 2020,2021 Drotos Daniel, Talker Bt.
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

#ifndef R3KACL_HEADER
#define R3KACL_HEADER

#include "dpedm3a.h"

#include "r3kcl.h"


class cl_r3ka: public cl_r3k
{
 public:
  cl_r3ka(class cl_sim *asim);
  virtual const char *id_string(void);

  virtual struct dis_entry *dis_entry(t_addr addr);

  virtual int LSxDR(int dif);

  virtual int LSDDR(t_mem code) { return LSxDR(-1); }
  virtual int LSIDR(t_mem code) { return LSxDR(+1); }
};


#endif

/* End of rxk.src/r3kacl.h */
