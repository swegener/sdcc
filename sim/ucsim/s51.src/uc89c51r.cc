/*
 * Simulator of microcontrollers (uc89c51r.cc)
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
#include <ctype.h>

// local
#include "uc89c51rcl.h"
#include "regs51.h"
#include "pcacl.h"
#include "wdtcl.h"


cl_uc89c51r::cl_uc89c51r(int Itype, int Itech, class cl_sim *asim):
  cl_uc51r(Itype, Itech, asim)
{
}


void
cl_uc89c51r::mk_hw_elements(void)
{
  class cl_hw *h;

  cl_uc52::mk_hw_elements();
  add_hw(h= new cl_wdt(this, 0x3fff));
  h->init();
  add_hw(h= new cl_pca(this, 0));
  h->init();
  add_hw(h= new cl_89c51r_dummy_hw(this));
  h->init();
}

void
cl_uc89c51r::make_memories(void)
{
  cl_uc52::make_memories();
  dptr->decoders->free_all();

  cl_memory_chip *dptr1_chip= new cl_memory_chip("dptr1_chip", 2, 8, 0);
  dptr1_chip->init();
  memchips->add(dptr1_chip);

  cl_banker *banker= new cl_banker(sfr, AUXR1, bmDPS,
				   dptr, 0, 1);
  banker->init();
  dptr->decoders->add(banker);
  
  banker->add_bank(0, memory("sfr_chip"), 2);
  banker->add_bank(1, dptr1_chip, 0);
  banker->activate(0);
  sfr->write(AUXR1, 0);
}


void
cl_uc89c51r::reset(void)
{
  cl_uc51r::reset();
  sfr->set_bit1(CCAPM0, bmECOM);
  sfr->set_bit1(CCAPM1, bmECOM);
  sfr->set_bit1(CCAPM2, bmECOM);
  sfr->set_bit1(CCAPM3, bmECOM);
  sfr->set_bit1(CCAPM4, bmECOM);
  //t0_overflows= 0;
  //dpl0= dph0= dpl1= dph1= 0;
  sfr->write(IPH, 0);
}

int
cl_uc89c51r::it_priority(uchar ie_mask)
{
  uchar l, h;

  l= sfr->get(IP) & ie_mask;
  h= sfr->get(IPH) & ie_mask;
  if (!h && !l)
    return(0);
  if (!h && l)
    return(1);
  if (h && !l)
    return(2);
  if (h && l)
    return(3);
  return(0);
}

void
cl_uc89c51r::pre_inst(void)
{
  //printf("pre dptr0:%02X%02X dptr1:%02X%02X\n", dph0, dpl0, dph1, dpl1);
  /*dps = (sfr->get(AUXR1) & bmDPS);
  if (dps)
    {
      sfr->set(DPL, dpl1);
      sfr->set(DPH, dph1);
    }
  else
    {
      sfr->set(DPL, dpl0);
      sfr->set(DPH, dph0);
      }*/
  cl_uc51r::pre_inst();
}

void
cl_uc89c51r::post_inst(void)
{
  /*if (dps)
    {
      dpl1= sfr->get(DPL);
      dph1= sfr->get(DPH);
    }
  else
    {
      dpl0= sfr->get(DPL);
      dph0= sfr->get(DPH);
      }*/
  //dps = (sfr->get(AUXR1) & bmDPS);
  /*if (dps)
    {
      sfr->set(DPL, dpl1);
      sfr->set(DPH, dph1);
    }
  else
    {
      sfr->set(DPL, dpl0);
      sfr->set(DPH, dph0);
      }*/
  //printf("post dptr0:%02X%02X dptr1:%02X%02X\n", dph0, dpl0, dph1, dpl1);
  cl_uc51r::post_inst();
}


void
cl_uc89c51r::print_regs(class cl_console_base *con)
{
  t_addr start;
  uchar data, acc, dps, h, l;

  start= psw->get() & 0x18;
  iram->dump(start, start+7, 8, con);
  data= iram->get(iram->get(start));
  con->dd_printf("@R0 %02x %c", data, isprint(data) ? data : '.');

  acc= sfr->get(ACC);
  con->dd_printf("  ACC= 0x%02x %3d %c  B= 0x%02x\n", acc, acc,
              isprint(acc)?(acc):'.', sfr->get(B)); 
  //eram2xram();

  data= iram->get(iram->get(start+1));
  con->dd_printf("@R1 %02x %c\n", data, isprint(data) ? data : '.');

  dps = sfr->get(AUXR1) & bmDPS;
  h= memory("sfr_chip")->get(3);
  l= memory("sfr_chip")->get(2);
  data= xram->get(h*256+l);
  con->dd_printf(" %cDPTR0= 0x%02x%02x @DPTR0= 0x%02x %3d %c\n",
              dps?' ':'*', h, l,
              data, data, isprint(data)?data:'.');
  h= memory("dptr1_chip")->get(3);
  l= memory("dptr1_chip")->get(2);
  data= xram->get(h*256+l);
  con->dd_printf(" %cDPTR1= 0x%02x%02x @DPTR1= 0x%02x %3d %c\n",
              dps?'*':' ', h, l,
              data, data, isprint(data)?data:'.');

  data= psw->get();
  con->dd_printf("  PSW= 0x%02x CY=%c AC=%c OV=%c P=%c\n", data,
              (data&bmCY)?'1':'0', (data&bmAC)?'1':'0',
              (data&bmOV)?'1':'0', (data&bmP)?'1':'0');
  /* show stack pointer */
  start = sfr->get (SP);
  con->dd_printf ("SP ", start);
  iram->dump (start, start - 7, 8, con);

  sfr->undecode_cell(DPL);
  sfr->undecode_cell(DPH);
  
  print_disass(PC, con);
}


/*
 */

cl_89c51r_dummy_hw::cl_89c51r_dummy_hw(class cl_uc *auc):
  cl_hw(auc, HW_DUMMY, 0, "_89c51r_dummy")
{}

int
cl_89c51r_dummy_hw::init(void)
{
  class cl_address_space *sfr= uc->address_space(MEM_SFR_ID);
  cl_hw::init();
  if (!sfr)
    {
      fprintf(stderr, "No SFR to register %s[%d] into\n", id_string, id);
    }
  dpl= register_cell(sfr, DPL);
  dph= register_cell(sfr, DPH);
  return(0);
}

t_mem
cl_89c51r_dummy_hw::read(class cl_memory_cell *cell)
{
  if (cell == dpl)
    return ((cl_uc89c51r*)uc)->dptr->read(0);
  else if (cell == dph)
    return ((cl_uc89c51r*)uc)->dptr->read(1);
  return cell->get();
}

void
cl_89c51r_dummy_hw::write(class cl_memory_cell *cell, t_mem *val)
{
  if (cell == dpl)
    ((cl_uc89c51r*)uc)->dptr->write(0, *val);
  else if (cell == dph)
    ((cl_uc89c51r*)uc)->dptr->write(1, *val);
}


/* End of s51.src/uc89c51r.cc */
