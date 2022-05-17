/*
 * Simulator of microcontrollers (simmos6502.cc)
 *
 * Copyright (C) 2020,20 Drotos Daniel, Talker Bt.
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

// local
#include "simmos6502cl.h"
#include "mos6502cl.h"
#include "mos6510cl.h"
#include "mos65c02cl.h"
#include "mos65c02scl.h"
#include "mos65ce02cl.h"
#include "glob.h"


cl_simmos6502::cl_simmos6502(class cl_app *the_app):
  cl_sim(the_app)
{}

class cl_uc *
cl_simmos6502::mk_controller(void)
{
  int i;
  const char *typ= 0;
  class cl_optref type_option(this);

  type_option.init();
  type_option.use("cpu_type");
  i= 0;
  if ((typ= type_option.get_value(typ)) == 0)
    typ= "65C02S";
  while ((cpus_6502[i].type_str != NULL) &&
	 (strcasecmp(typ, cpus_6502[i].type_str) != 0))
    i++;
  if (cpus_6502[i].type_str == NULL)
    {
      fprintf(stderr, "Unknown processor type. "
	      "Use -H option to see known types.\n");
      return(NULL);
    }
  switch (cpus_6502[i].type)
    {
    case CPU_6502:
      return(new cl_mos6502(this));
    case CPU_6510:
      printf("Not implemented yet.\n"); return(NULL);
      return(new cl_mos6510(this));
    case CPU_65C02:
      return(new cl_mos65c02(this));
    case CPU_65C02S:
      return(new cl_mos65c02s(this));
    case CPU_65CE02:
      printf("Not implemented yet.\n"); return(NULL); 
      return(new cl_mos65ce02(this));
    default:
      fprintf(stderr, "Unknown processor type\n");
      return NULL;
    }
  return NULL;
}


/* End of mos6502.src/simmos6502.cc */
