/*
 * Simulator of microcontrollers (serial.cc)
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

/* $Id: serial.cc 435 2016-08-26 17:57:43Z drdani $ */

#include "ddconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <strings.h>

// prj
#include "globals.h"
#include "utils.h"

// cmd
#include "cmdutil.h"

// sim
#include "itsrccl.h"

// local
#include "serialcl.h"


cl_serial::cl_serial(class cl_uc *auc,
		     t_addr abase,
		     int ttype):
  cl_serial_hw(auc, ttype, "uart")
{
  type= ttype;
  base= abase;
}


cl_serial::~cl_serial(void)
{
}

int
cl_serial::init(void)
{
  int i;

  set_name("stm8_uart");
  cl_serial_hw::init();
  for (i= 0; i < 12; i++)
    {
      regs[i]= register_cell(uc->rom, base+i);
    }
  pick_div();
  pick_ctrl();

  uc->it_sources->add(new cl_it_src(uc, 20,
				    regs[cr2], 0x80,
				    regs[sr], 0x80,
				    0x8058, false, false,
				    "usart transmit register empty", 20*10+1));
  uc->it_sources->add(new cl_it_src(uc, 20,
				    regs[cr2], 0x40,
				    regs[sr], 0x40,
				    0x8058, false, false,
				    "usart trasnmit complete", 20*10+2));
  uc->it_sources->add(new cl_it_src(uc, 21,
				    regs[cr2], 0x20,
				    regs[sr], 0x20,
				    0x805C, false, false,
				    "usart receive", 20*10+3));

  sr_read= false;

  cl_var *v;
  chars pn(id_string);
  pn.append("%d_", id);
  uc->vars->add(v= new cl_var(pn+chars("on"), cfg, serial_on));
  v->init();
  
  return(0);
}


void
cl_serial::new_hw_added(class cl_hw *new_hw)
{
  /*  if (new_hw->cathegory == HW_TIMER &&
      new_hw->id == 2)
    {
      there_is_t2= DD_TRUE;
      t_mem d= sfr->get(T2CON);
      t2_baud= d & (bmRCLK | bmTCLK);
    }
  */
}

void
cl_serial::added_to_uc(void)
{
}

t_mem
cl_serial::read(class cl_memory_cell *cell)
{
  if (cell == regs[dr])
    {
      if (sr_read)
	regs[sr]->set_bit0(0x1f);
      regs[sr]->set_bit0(0x20);
      printf("** read DR=0x%02x\n", s_in);
      return s_in;
    }
  sr_read= (cell == regs[sr]);
  conf(cell, NULL);
  return cell->get();
}

void
cl_serial::write(class cl_memory_cell *cell, t_mem *val)
{
  printf("** write 0x%02x,'%c'\n", *val, *val);
  
  if (conf(cell, val))
    return;
  if (cell == regs[sr])
    {
      uint8_t v= cell->get();
      if ((*val & 0x40) == 0)
	v&= ~0x40;
      *val= v;
    }
  else
    {
      cell->set(*val);
      if ((cell == regs[brr1]) ||
	  (cell == regs[brr2]))
	{
	  printf("** BRR1,2 %x\n", *val);
	  pick_div();
	}
      else if ((cell == regs[cr1]) ||
	       (cell == regs[cr2]))
	{
	  printf("** CR1,2 %x\n", *val);
	  pick_ctrl();
	}
  
      else if (cell == regs[dr])
	{
	  printf("** DR %x txd=%c\n", *val, *val);
	  s_txd= *val;
	  s_tx_written= true;
	  show_writable(false);
	  if (sr_read)
	    show_tx_complete(false);
	  if (!s_sending)
	    {
	      start_send();
	    }      
	}
    }

  sr_read= false;
}

t_mem
cl_serial::conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val)
{
  switch ((enum serial_cfg)addr)
    {
    case serial_on: // turn this HW on/off
      if (val)
	{
	  if (*val)
	    on= true;
	  else
	    on= false;
	}
      else
	{
	  cell->set(on?1:0);
	}
      break;
    }
  return cell->get();
}

int
cl_serial::tick(int cycles)
{
  char c;

  if (!en)
    return 0;
  
  if ((mcnt+= cycles) >= div)
    {
      mcnt-= div;
      if (ten)
	s_tr_bit++;
      if (ren)
	s_rec_bit++;
    }
  else
    return 0;
  
  if (s_sending &&
      (s_tr_bit >= bits))
    {
      printf("** sent %c\n", s_out);
      s_sending= false;
      if (fout)
	{
	  fout->write((char*)(&s_out), 1);
	}
      s_tr_bit-= bits;
      if (s_tx_written)
	restart_send();
      else
	finish_send();
    }
  if ((ren) &&
      fin &&
      !s_receiving)
    {
      if (fin->input_avail())
	{
	  s_receiving= true;
	  s_rec_bit= 0;
	}
      else
	show_idle(true);
    }
  if (s_receiving &&
      (s_rec_bit >= bits))
    {
      if (fin->read(&c, 1) == 1)
	{
	  s_in= c;
	  received();
	}
      s_receiving= false;
      s_rec_bit-= bits;
    }
  
  return(0);
}

void
cl_serial::start_send()
{
  printf("** start_send ten=%d %c\n", ten, s_txd);
  if (ten)
    {
      s_out= s_txd;
      s_tx_written= false;
      s_sending= true;
      s_tr_bit= 0;
      show_writable(true);
    }
}

void
cl_serial::restart_send()
{
  printf("** restart_send ten=%d %c\n", ten, s_txd);
  if (ten)
    {
      s_out= s_txd;
      s_tx_written= false;
      s_sending= true;
      s_tr_bit= 0;
      show_writable(true);
    }
}

void
cl_serial::finish_send()
{
  show_writable(true);
  show_tx_complete(true);
}

void
cl_serial::received()
{
  set_dr(s_in);
  if (regs[sr]->get() & 0x20)
    regs[sr]->set_bit1(0x08); // overrun
  show_readable(true);
}

void
cl_serial::reset(void)
{
  int i;
  printf("** reset\n");
  regs[sr]->set(0xc0);
  for (i= 2; i < 12; i++)
    regs[i]->set(0);
}

void
cl_serial::pick_div()
{
  uint8_t b1= regs[brr1]->get();
  uint8_t b2= regs[brr2]->get();
  div= ((((b2&0xf0)<<4) + b1)<<4) + (b2&0xf);
  mcnt= 0;
  printf("pick_div %d\n", div);
}

void
cl_serial::pick_ctrl()
{
  uint8_t c1= regs[cr1]->get();
  uint8_t c2= regs[cr2]->get();
  en= !(c1 & 0x20);
  ten= c2 & 0x08;
  ren= c2 & 0x04;
  bits= 10;
  s_rec_bit= s_tr_bit= 0;
  s_receiving= false;
  s_tx_written= false;
  printf("pick_ctrl en=%d ten=%d ren=%d\n", en, ten, ren);
}

void
cl_serial::show_writable(bool val)
{
  if (val)
    // TXE=1
    regs[sr]->set_bit1(0x80);
  else
    // TXE=0
    regs[sr]->set_bit0(0x80);
  printf("** TXE=%d sr=%x\n", val, regs[sr]->get());
}

void
cl_serial::show_readable(bool val)
{
  if (val)
    regs[sr]->set_bit1(0x20);
  else
    regs[sr]->set_bit0(0x20);
  printf("** RX=%d sr=%x\n", val, regs[sr]->get());
}

void
cl_serial::show_tx_complete(bool val)
{
  if (val)
    regs[sr]->set_bit1(0x40);
  else
    regs[sr]->set_bit0(0x40);
  printf("** TC=%d sr=%x\n", val, regs[sr]->get());
}

void
cl_serial::show_idle(bool val)
{
  if (val)
    regs[sr]->set_bit1(0x10);
  else
    regs[sr]->set_bit0(0x10);
}

void
cl_serial::set_dr(t_mem val)
{
  regs[dr]->set(val);
  printf("** DR<- %x %c\n", val, val);
}

void
cl_serial::print_info(class cl_console_base *con)
{
  con->dd_printf("%s[%d] %s\n", id_string, id, on?"on":"off");
  con->dd_printf("Input: ");
  if (fin)
    con->dd_printf("%s/%d ", fin->get_file_name(), fin->file_id);
  con->dd_printf("Output: ");
  if (fout)
    con->dd_printf("%s/%d\n", fout->get_file_name(), fout->file_id);
}


/* End of stm8.src/serial.cc */
