/*
 * Simulator of microcontrollers (hcwrap.cc)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
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

#include "glob12.h"

#include "hcwrapcl.h"


int wrap_INV(class CL12 *uc, t_mem code) { return resINV; }

cl_wrap::cl_wrap():
  cl_base()
{
  int i;
  for (i= 0; i < 257; i++)
    {
      disass12p0[i].code= (i<=255)?i:0;
      disass12p0[i].mask= 0xff;
      disass12p0[i].branch= ' ';
      disass12p0[i].length= 1;
      disass12p0[i].mnemonic= "";
      disass12p0[i].is_call= false;
      disass12p0[i].ticks= 1;
    }
}

void
cl_wrap::set_disass(int page, int code, const char *mnemo, char branch, int len)
{
  struct dis_entry *tab= NULL;
  switch (page)
    {
    case 0: tab= disass12p0; break;
    }
  if (tab == NULL)
    return;
  tab[code].code= code;
  tab[code].branch= branch;
  tab[code].length= len;
  tab[code].mnemonic= mnemo;
}


#include "wdefs.h"

/* End of m68hc12.src/hcwrap.cc */
