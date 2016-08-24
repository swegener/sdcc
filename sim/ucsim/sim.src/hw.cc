/*
 * Simulator of microcontrollers (hw.cc)
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

#include <stdlib.h>
#include "i_string.h"

#include "stypes.h"
#include "hwcl.h"


/*
 *____________________________________________________________________________
 */

cl_hw::cl_hw(class cl_uc *auc, enum hw_cath cath, int aid, const char *aid_string):
  cl_guiobj()
{
  flags= HWF_INSIDE;
  uc= auc;
  cathegory= cath;
  id= aid;
  if (aid_string &&
      *aid_string)
    id_string= strdup(aid_string);
  else
    id_string= strdup("unknown hw element");
  char *s= (char*)malloc(strlen(get_name("hw"))+100);
  sprintf(s, "partners of %s", get_name("hw"));
  partners= new cl_list(2, 2, s);
  sprintf(s, "watched cells of %s", get_name("hw"));
  free(s);
  cfg= 0;
}

cl_hw::~cl_hw(void)
{
  free((void*)id_string);
  delete partners;
}

int
cl_hw::init(void)
{
  chars n(id_string);
  char s[100];
  int i;

  on= true;
  
  snprintf(s, 99, "%d", id);
  n+= '_';
  n+= s;
  n+= cchars("_cfg");

  cfg= new cl_address_space(n, 0, cfg_size(), sizeof(t_mem)*8);
  cfg->init();
  cfg->hidden= true;
  uc->address_spaces->add(cfg);

  for (i= 0; i < cfg_size(); i++)
    {
      cfg->register_hw(i, this, false);
    }
  
  return 0;
}

void
cl_hw::new_hw_adding(class cl_hw *new_hw)
{
}

void
cl_hw::new_hw_added(class cl_hw *new_hw)
{
  int i;

  for (i= 0; i < partners->count; i++)
    {
      class cl_partner_hw *ph= (class cl_partner_hw *)(partners->at(i));
      ph->refresh(new_hw);
    }
}

class cl_hw *
cl_hw::make_partner(enum hw_cath cath, int id)
{
  class cl_partner_hw *ph;
  class cl_hw *hw;

  ph= new cl_partner_hw(uc, cath, id);
  partners->add(ph);
  hw= ph->get_partner();
  return(hw);
}

t_mem
cl_hw::read(class cl_memory_cell *cell)
{
  conf(cell, NULL);
  return cell->get();
}

void
cl_hw::write(class cl_memory_cell *cell, t_mem *val)
{
  conf(cell, val);    
}

bool
cl_hw::conf(class cl_memory_cell *cell, t_mem *val)
{
  t_addr a;
  if (cfg->is_owned(cell, &a))
    {
      conf_op(cell, a, val);
      if (val)
	cell->set(*val);
      return true;
    }
  return false;
}

t_mem
cl_hw::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  return cell->get();
}

void
cl_hw::cfg_set(t_addr addr, t_mem val)
{
  cfg->set(addr, val);
}

void
cl_hw::set_cmd(class cl_cmdline *cmdline, class cl_console_base *con)
{
  con->dd_printf("Nothing to do\n");
}

class cl_memory_cell *
cl_hw::register_cell(class cl_address_space *mem, t_addr addr)
{
  if (mem)
    mem->register_hw(addr, this, false);
  else
    printf("regcell JAJ no mem\n");
  return mem->get_cell(addr);
}

void
cl_hw::unregister_cell(class cl_memory_cell *the_cell)
{
  if (the_cell)
    the_cell->remove_hw(this);
}


/*
 * Simulating `cycles' number of machine cycle
 */

int
cl_hw::tick(int cycles)
{
  return(0);
}

void
cl_hw::inform_partners(enum hw_event he, void *params)
{
  int i;

  for (i= 0; i < partners->count; i++)
    {
      class cl_partner_hw *ph= (class cl_partner_hw *)(partners->at(i));
      ph->happen(this, he, params);
    }
}


void
cl_hw::print_info(class cl_console_base *con)
{
  con->dd_printf("%s[%d]\n", id_string, id);
}


/*
 * List of hw
 */

t_index
cl_hws::add(void *item)
{
  int i;
  t_index res;

  // pre-add
  for (i= 0; i < count; i++)
    {
      class cl_hw *hw= (class cl_hw *)(at(i));
      hw->new_hw_adding((class cl_hw *)item);
    }
  // add
  res= cl_list::add(item);
  // post-add
  for (i= 0; i < count; i++)
    {
      class cl_hw *hw= (class cl_hw *)(at(i));
      hw->new_hw_added((class cl_hw *)item);
    }
  ((class cl_hw *)item)->added_to_uc();
  return(res);
}

/*
 *____________________________________________________________________________
 */

cl_partner_hw::cl_partner_hw(class cl_uc *auc, enum hw_cath cath, int aid):
  cl_base()
{
  uc= auc;
  cathegory= cath;
  id= aid;
  partner= uc->get_hw(cathegory, id, 0);
}

class cl_hw *
cl_partner_hw::get_partner(void)
{
  return(partner);
}

void
cl_partner_hw::refresh(void)
{
  class cl_hw *hw= uc->get_hw(cathegory, id, 0);

  if (!hw)
    return;
  if (partner)
    {
      // partner is already set
      if (partner != hw)
	{
	  // partner changed?
	  partner= hw;
	}
      else
	partner= hw;
    }
  partner= hw;
}

void
cl_partner_hw::refresh(class cl_hw *new_hw)
{
  if (!new_hw)
    return;
  if (cathegory == new_hw->cathegory &&
      id == new_hw->id)
    {
      if (partner)
	{
	  // partner changed?
	  partner= new_hw;
	}
      else
	partner= new_hw;
    }
}

void
cl_partner_hw::happen(class cl_hw *where, enum hw_event he, void *params)
{
  if (partner)
    partner->happen(where, he, params);
}


/* End of hw.cc */
