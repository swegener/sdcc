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

/* $Id: timer.cc 582 2017-01-05 18:33:41Z drdani $ */

#include "timercl.h"


enum stm8_tim_reg_idx {
  cr1	=  0, // control 1
  cr2	=  1, // control 2
  smcr	=  2, // slave mode control
  etr	=  3, // external trigger
  ier	=  4, // interrupt enable
  sr1	=  5, // status 1
  sr2	=  6, // status 2
  egr	=  7, // event generation
  ccmr1	=  8, // capture/compare mode 1
  ccmr2	=  9, // capture/compare mode 2
  ccmr3	= 10, // capture/compare mode 3
  ccmr4	= 11, // capture/compare mode 4
  ccer1	= 12, // capture/compare enable 1
  ccer2	= 13, // capture/compare enable 2
  cntrh	= 14, // counter high
  cntrl	= 15, // counter low
  pscrh	= 16, // prescaler high
  pscrl	= 17, // prescaler low
  arrh	= 18, // auto-reload high
  arrl	= 19, // auto-reload low
  rcr	= 20, // repetition counter
  ccr1h	= 21, // capture/compare 1 high
  ccr1l	= 22, // capture/compare 1 low
  ccr2h	= 23, // capture/compare 2 high
  ccr2l	= 24, // capture/compare 2 low
  ccr3h	= 25, // capture/compare 3 high
  ccr3l	= 26, // capture/compare 3 low
  ccr4h	= 27, // capture/compare 4 high
  ccr4l	= 28, // capture/compare 4 low
  bkr	= 29, // break
  dtr	= 30, // deadtime
  oisr	= 31  // output idle state  
};

enum tim_cr1_bits {
  cen	= 0x01,
  udis	= 0x02,
  urs	= 0x04,
  opm	= 0x08,
  dir	= 0x10,
  cms	= 0x60,
  cms0	= 0x00, // edge aligned mode
  cms1	= 0x20, // center aligned 1 (irq during downcount)
  cms2	= 0x40, // center aligned 2 (irq during upcount)
  cms3	= 0x60, // center aligned 3 (irq in both counting dir)
  arpe	= 0x80
};


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
  if (!on)
    return resGO;

  while (cycles--)
    {
      // count prescaler
      if (prescaler_cnt)
	prescaler_cnt--;
      if (prescaler_cnt == 0)
	{
	  prescaler_cnt= prescaler_preload;
	  // count
	  if (regs[cr1]->get() & cen)
	    {
	      count();
	    }
	}
    }
  
  return resGO;
}

void
cl_tim::reset(void)
{
  int i;
  
  cnt= 0;
  prescaler_cnt= 0;
  prescaler_preload= 0;

  for (i= 0; i<32; i++)
    regs[i]->set(0);
  regs[arrh]->set(0xff);
  regs[arrl]->set(0xff);
}

t_mem
cl_tim::read(class cl_memory_cell *cell)
{
  t_mem v= cell->get();
  t_addr a;
  
  if (conf(cell, NULL))
    return v;
  if (uc->rom->is_owned(cell, &a))
    {
      if ((a >= base) &&
	  (a < base+32))
	{
	  a-= base;
	  switch ((enum stm8_tim_reg_idx )a)
	    {
	    case pscrl:
	      v= prescaler_preload && 0xff;
	      break;
	    case pscrh:
	      v= (prescaler_preload >> 8) & 0xff;
	      break;
	    case cntrh:
	      timer_ls_buffer= regs[cntrl]->get();
	      break;
	    case cntrl:
	      v= timer_ls_buffer;
	      break;
	    default:
	      break;
	    }
	}
    }
  
  return v;
}

void
cl_tim::write(class cl_memory_cell *cell, t_mem *val)
{
  t_addr a;

  if (conf(cell, val))
    return;
  
  if (conf(cell, NULL))
    return;

  *val&= 0xff;
  if (uc->rom->is_owned(cell, &a))
    {
      if ((a >= base) &&
	  (a < base+32))
	{
	  a-= base;
	  switch ((enum stm8_tim_reg_idx )a)
	    {
	    case pscrh:
	      prescaler_ms_buffer= *val;
	      *val= cell->get();
	      break;
	    case pscrl:
	      prescaler_preload= prescaler_ms_buffer * 256 + *val;
	      break;
	    case arrh:
	      if ((regs[cr1]->get() & arpe) != 0)
		{
		  arr_ms_buffer= *val;
		  *val= cell->get();
		}
	      break;
	    case arrl:
	      {
		u8_t l, h;
		if ((regs[cr1]->get() & arpe) != 0)
		  {
		    regs[arrl]->set(l= *val);
		    regs[arrh]->set(h= arr_ms_buffer);
		    if ((regs[cr1]->get() & arpe) == 0)
		      set_counter(h*256 + l);
		  }
		break;
	      }
	    default:
	      break;
	    }
	}
    }
}

t_mem
cl_tim::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  switch ((enum stm8_tim_cfg)addr)
    {
    case stm8_tim_on:
      if (val)
	{
	  if (*val)
	    on= true;
	  else
	    on= false;
	}
      else
	cell->set(on?1:0);
      break;
    case stm8_tim_nuof_cfg:
      break;
    }
  return cell->get();
}


void
cl_tim::count(void)
{
  u8_t c1= regs[cr1]->get();
  if (regs[cr1]->get() & dir)
    {
      // down
      set_counter(cnt-1);
      if (cnt == 0)
	{
	  if ((c1 & cms) == cms0)
	    // edge aligned
	    set_counter(regs[arrh]->get()*256+regs[arrl]->get());
	  else
	    // center aligned
	    regs[cr1]->set(c1&= ~dir);
	  if ((c1 & udis) == 0)
	    update_event();
	}
    }
  else
    {
      // up
      u16_t arr= regs[arrh]->get() * 256 + regs[arrl]->get();
      set_counter(cnt+1);
      if (cnt == arr)
	{
	  if ((c1 & cms) == cms0)
	    // edge aligned
	    set_counter(0);
	  else
	    // center aligned
	    regs[cr1]->set(c1|= dir);
	  if ((c1 & udis) == 0)
	    update_event();
	}
    }	      
}

u16_t
cl_tim::set_counter(u16_t val)
{
  cnt= val & mask;
  regs[cntrl]->set(val&0xff);
  if (bits > 8)
    regs[cntrh]->set(val>>8);
  return val;
}

void
cl_tim::update_event(void)
{
  u8_t c1= regs[cr1]->get();

  if (c1 & opm)
    regs[cr1]->set_bit0(cen);
  else
    {
      if (c1 & dir)
	{
	  // down
	  u16_t ar= regs[arrh]->get() * 256 + regs[arrl]->get();
	  set_counter(ar);
	}
      else
	{
	  // up
	  set_counter(0);
	}
    }
}


void
cl_tim::print_info(class cl_console_base *con)
{
  u8_t c1= regs[cr1]->get();
  con->dd_printf("%s 0x%04x %d %s\n", get_name(), cnt, cnt, (c1&cen)?"on":"off");
  con->dd_printf("dir= %s\n", (c1&dir)?"down":"up");
  con->dd_printf("prescaler= 0x%04x %d of 0x%04x %d\n",
		 prescaler_cnt, prescaler_cnt,
		 prescaler_preload, prescaler_preload);
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
