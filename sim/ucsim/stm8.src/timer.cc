/*
 * Simulator of microcontrollers (stm8.src/timer.cc)
 *
 * Copyright (C) 2016,16 Drotos Daniel, Talker Bt.
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

/* $Id: timer.cc 367 2016-07-16 18:35:09Z  $ */

#include "timercl.h"


cl_tim::cl_tim(class cl_uc *auc, int aid, t_addr abase):
  cl_hw(auc, HW_TIMER, aid, "tim")
{
  base= abase;
  int i;
  for (i= 0; i<32; i++)
    regs[i]= 0;
}

int
cl_tim::init(void)
{
  int i;
  chars s("tim");
  s.append("%d", id);
  set_name(s);
  id_string= strdup(s);
  cl_hw::init();
  for (i= 0; i < 32; i++)
    {
      regs[i]= register_cell(uc->rom, base+i);
    }

  switch (id)
    {
    case 2:
      bits= 16; mask= 0xffff;      
      break;
    case 3:
      bits= 16; mask= 0xffff;      
      break;
    case 5:
      bits= 16; mask= 0xffff;      
      break;
    case 4:
      bits= 8; mask= 0xff;      
      break;
    case 6:
      bits= 8; mask= 0xff;      
      break;
    default: // 1
      bits= 16; mask= 0xffff;      
      break;
    }
 
  return 0;
}

int
cl_tim::tick(int cycles)
{
  return resGO;
}

void
cl_tim::reset(void)
{
  cnt= 0;
}


/************************************************************************ 1 */

cl_tim1::cl_tim1(class cl_uc *auc, int aid, t_addr abase):
  cl_tim(auc, aid, abase)
{
}


/********************************************************************** 235 */

cl_tim235::cl_tim235(class cl_uc *auc, int aid, t_addr abase):
  cl_tim(auc, aid, abase)
{
}


/*********************************************************************** 46 */

cl_tim46::cl_tim46(class cl_uc *auc, int aid, t_addr abase):
  cl_tim(auc, aid, abase)
{
}


/* End of stm8.src/timer.cc */
