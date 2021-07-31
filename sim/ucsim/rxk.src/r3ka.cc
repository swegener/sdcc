/*
 * Simulator of microcontrollers (r3ka.cc)
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

#include "glob.h"
#include "gp0m3.h"
#include "gpddm3.h"
#include "gpedm3.h"
#include "gpedm3a.h"

#include "r3kacl.h"


cl_r3ka::cl_r3ka(class cl_sim *asim):
  cl_r3k(asim)
{
}

const char *
cl_r3ka::id_string(void)
{
  return "R3KA";
}

struct dis_entry *
cl_r3ka::dis_entry(t_addr addr)
{
  u8_t code= rom->get(addr);
  int i;
  struct dis_entry *dt;

  if (code == 0xed)
    {
      code= rom->get(addr+1);

      dt= disass_pedm3;
      i= 0;
      while (((code & dt[i].mask) != dt[i].code) &&
	     dt[i].mnemonic)
	i++;
      if (dt[i].mnemonic != NULL)
	return &dt[i];

      dt= disass_pedm3a;
      i= 0;
      while (((code & dt[i].mask) != dt[i].code) &&
	     dt[i].mnemonic)
	i++;
      if (dt[i].mnemonic != NULL)
	return &dt[i];

      return NULL;
    }
  if ((code & 0xdd) == 0xdd)
    {
      if (code == 0xdd)
	{
	  cIR= &cIX;
	}
      else
	{
	  cIR= &cIY;
	}
      code= rom->get(addr+1);
      dt= disass_pddm3;
      i= 0;
      while (((code & dt[i].mask) != dt[i].code) &&
	     dt[i].mnemonic)
	i++;
      if (dt[i].mnemonic != NULL)
	return &dt[i];
      return NULL;
    }
  
  dt= disass_rxk;
  i= 0;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  if (dt[i].mnemonic != NULL)
    return &dt[i];

  dt= disass_p0m3;
  i= 0;
  while (((code & dt[i].mask) != dt[i].code) &&
	 dt[i].mnemonic)
    i++;
  if (dt[i].mnemonic != NULL)
    return &dt[i];
  
  return &dt[i];
}


/* End of rxk.src/r3ka.cc */
