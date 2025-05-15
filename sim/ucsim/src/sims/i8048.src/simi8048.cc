/*
 * Simulator of microcontrollers (simi8048.cc)
 *
 * Copyright (C) 2022 Drotos Daniel
 * 
 * To contact author send email to dr.dkdb@gmail.com
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

#include "simi8048cl.h"
#include "i8041cl.h"
#include "glob.h"


struct {
  int t;
  unsigned int rom_siz;
  unsigned int ram_siz;
}
  mem_sizes[]= {
  { CPU_I8021,   1024,  64 },
  { CPU_I8022,   2048,  64 },
  { CPU_I8035,   4096,  64 },
  { CPU_I8039,   4096, 128 },
  { CPU_I8040,   4096, 256 },
  { CPU_I8041,   1024,  64 },
  { CPU_I8041A,  1024,  64 },
  { CPU_I8041AH, 1024, 128 },
  { CPU_I8042,   2048, 128 },
  { CPU_I8042AH, 2048, 256 },
  { CPU_I80C42,  4096, 256 },
  { CPU_I80L42,  4096, 256 },
  { CPU_I8048,   1024,  64 },
  { CPU_I8049,   2048, 128 },
  { CPU_I8050,   4096, 256 },
  { 0, 0, 0 }
};

cl_simi8048::cl_simi8048(class cl_app *the_app):
  cl_sim(the_app)
{}

class cl_uc *
cl_simi8048::mk_controller(void)
{
  int i;
  const char *typ= 0;
  class cl_optref type_option(this);
  class cl_i8020 *uc;

  type_option.init();
  type_option.use("cpu_type");
  i= 0;
  if ((typ= type_option.get_value(typ)) == 0)
    typ= "I8050";
  while ((cpus_8048[i].type_str != NULL) &&
	 (strcasecmp(typ, cpus_8048[i].type_str) != 0))
    i++;
  if (cpus_8048[i].type_str == NULL)
    {
      fprintf(stderr, "Unknown processor type. "
	      "Use -H option to see known types.\n");
      return(NULL);
    }
  int j;
  unsigned int roms= 0, rams= 0;
  for (j= 0; mem_sizes[j].t; j++)
    {
      if (mem_sizes[j].t == cpus_8048[i].type)
	{
	  roms= mem_sizes[j].rom_siz;
	  rams= mem_sizes[j].ram_siz;
	}
    }
  if (cpus_8048[i].type & CPU_MCS21)
    {
      if (!roms || !rams)
	uc= new cl_i8021(this);
      else
	uc= new cl_i8021(this, roms, rams);
      uc->set_id(cpus_8048[i].type_help);
      uc->type= &cpus_8048[i];
      return uc;
    }
  else if (cpus_8048[i].type & CPU_MCS22)
    {
      if (!roms || !rams)
	uc= new cl_i8022(this);
      else
	uc= new cl_i8022(this, roms, rams);
      uc->set_id(cpus_8048[i].type_help);
      uc->type= &cpus_8048[i];
      return uc;
    }
  else  if (cpus_8048[i].type & CPU_MCS48)
    {
      if (!roms || !rams)
	uc= new cl_i8048(this);
      else
	uc= new cl_i8048(this, roms, rams);
      uc->set_id(cpus_8048[i].type_help);
      uc->type= &cpus_8048[i];
      return uc;
    }
  else if (cpus_8048[i].type & CPU_MCS41)
    {
      if (!roms || !rams)
	uc= new cl_i8041(this);
      else
	uc= new cl_i8041(this, roms, rams);
      uc->set_id(cpus_8048[i].type_help);
      uc->type= &cpus_8048[i];
      return uc;
    }
  return NULL;
}


/* End of i8048.src/simi8048.cc */
