/*
 * Simulator of microcontrollers (cmd_brk.cc)
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
#include <ctype.h>
#include "i_string.h"

#include "simcl.h"

#include "uc51cl.h" //FIXME
#include "globals.h"


/*
 * BREAKPOINT SET f|d addr [hit]
 */
//FIXME
/*bool
cmd_brk_setf(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s;
  uint addr;
  int  hit= 1;
  enum brk_perm perm;
  class cl_brk *b;

  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Permanency has not given\n");
      return(FALSE);
    }
  switch (*s)
    {
    case 'f': case 'F':
      perm= brkFIX;
      break;
    case 'd': case 'D':
      perm= brkDYNAMIC;
      break;
    default:
      sim->cmd->printf("Unknow permanency\n");
      return(FALSE);
    }
  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Address has not given\n");
      return(FALSE);
    }
  addr= (uint)strtol(s, NULL, 0);
  if ((s= strtok(NULL, delimiters)) != NULL)
    hit= strtol(s, NULL, 0);
  if (uc->fbrk_at(addr))
    sim->cmd->printf("Breakpoint at %06x is already set.\n", addr);
  else
    {
      b= new cl_fetch_brk(addr, perm, hit);
      uc->fbrk->add_bp(b);
    }
  return(FALSE);
}
*/

/*
 * BREAKPOINT SET EVENT wi|ri|wx|rx|ws|rs|rc f|d addr [hit]
 */
//FIXME
bool
cmd_brk_sete(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  char *s;
  char *id;
  uint addr;
  int  hit= 1;
  enum brk_perm perm;
  class cl_ev_brk *b= NULL;

  if ((id= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Event has not given.\n");
      return(DD_FALSE);
    }
  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Permanency has not given\n");
      return(DD_FALSE);
    }
  switch (*s)
    {
    case 'f': case 'F':
      perm= brkFIX;
      break;
    case 'd': case 'D':
      perm= brkDYNAMIC;
      break;
    default:
      sim->cmd->printf("Unknow permanency\n");
      return(DD_FALSE);
    }
  if ((s= strtok(NULL, delimiters)) == NULL)
    {
      sim->cmd->printf("Address has not given\n");
      return(DD_FALSE);
    }
  addr= (uint)strtol(s, NULL, 0);
  if ((s= strtok(NULL, delimiters)) != NULL)
    hit= strtol(s, NULL, 0);
  else
    {
      if (!strcmp(id, "wi"))
	b= new cl_wi_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "ri"))
	b= new cl_ri_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "wx"))
	b= new cl_wx_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "rx"))
	b= new cl_rx_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "ws"))
	b= new cl_ws_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "rs"))
	b= new cl_rs_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else if (!strcmp(id, "rc"))
	b= new cl_rc_brk(uc->ebrk->make_new_nr(), addr, perm, hit);
      else
	sim->cmd->printf("Event %s is unknown.\n", id);
      if (b)
	uc->ebrk->add_bp(b);
    }
  return(DD_FALSE);
}


/*
 * BREAKPOINT DELETE addr
 */
//FIXME
/*bool
cmd_brk_delf(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  uint addr;
  char *s;

  if ((s= strtok(NULL, delimiters)) == NULL)
    sim->cmd->printf("Address has not given.\n");
  else
    {
      addr= (uint)strtol(s, NULL, 0);
      if (uc->fbrk_at(addr) == NULL)
	sim->cmd->printf("No breakpoint at %06x\n", addr);
      else
	uc->fbrk->del_bp(addr);
    }
  return(DD_FALSE);
}*/


/*
 * BREAKPOINT DELETE EVENT wi|ri|wx|rx|ws|rs|rc addr
 */
//FIXME
bool
cmd_brk_dele(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  uint addr;
  char *s, *id;

  if ((id= strtok(NULL, delimiters)) == NULL)
    {
      fprintf(sim->cmd_out(), "Event has not given.\n");
      return(DD_FALSE);
    }
  if ((s= strtok(NULL, delimiters)) == NULL)
    fprintf(sim->cmd_out(), "Address has not given.\n");
  else
    {
      addr= (uint)strtol(s, NULL, 0);
      if (uc->ebrk_at(addr, id) == NULL)
	fprintf(sim->cmd_out(), "No %s breakpoint at %06x\n", id, addr);
      else
	uc->rm_ebrk(addr, id);
    }
  return(DD_FALSE);
}


/*
 * BREAKPOINT DELETE ALL
 */
//FIXME
bool
cmd_brk_delall(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  while (uc->fbrk->count)
    {
      class cl_brk *brk= (class cl_brk *)(uc->fbrk->at(0));
      uc->fbrk->del_bp(brk->addr);
    }
  while (uc->ebrk->count)
    uc->ebrk->free_at(0);
  return(DD_FALSE);
}


/*
 * BREAKPOINT LIST
 */
//FIXME
/*bool
cmd_brk_lst(char *cmd, class t_uc51 *uc, class cl_sim *sim)
{
  class cl_fetch_brk *fb;
  class cl_ev_brk *eb;
  int i;
  char *s;

  for (i= 0; i < uc->fbrk->count; i++)
    {
      fb= (class cl_fetch_brk *)(uc->fbrk->at(i));
      s = uc->disass(fb->addr, NULL);
      fprintf(sim->cmd_out(), "%c %d(%d) %06x %02x %s\n",
	      (fb->perm==brkFIX)?'F':'D',
	      fb->hit, fb->cnt,
	      fb->addr, uc->get_mem(MEM_ROM, fb->addr), s);
      free(s);
    }
  for (i= 0; i < uc->ebrk->count; i++)
    {
      eb= (class cl_ev_brk *)(uc->ebrk->at(i));
      fprintf(sim->cmd_out(), "%c %d(%d) %06x %s\n",
	      (eb->perm==brkFIX)?'F':'D',
	      eb->hit, eb->cnt,
	      eb->addr, eb->id);
    }
  return(DD_FALSE);
}*/


/* End of s51.src/cmd_brk.cc */
