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

// local
#include "uc89c51rcl.h"
#include "regs51.h"
#include "pcacl.h"


t_uc89c51r::t_uc89c51r(int Itype, int Itech, class cl_sim *asim):
  t_uc51r(Itype, Itech, asim)
{
  /*it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCCF4, 0x0033, false,
				      "PCA module #4"));
  it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCCF3, 0x0033, false,
				      "PCA module #3"));
  it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCCF2, 0x0033, false,
				      "PCA module #2"));
  it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCCF1, 0x0033, false,
				      "PCA module #1"));
  it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCCF0, 0x0033, false,
				      "PCA module #0"));
  it_sources->add_at(4, new cl_it_src(bmEC, CCON, bmCF, 0x0033, false,
  "PCA counter"));*/
}


void
t_uc89c51r::mk_hw_elements(void)
{
  class cl_hw *h;

  t_uc51r::mk_hw_elements();
  hws->add(h= new cl_pca(this, 0));
  h->init();
  /*hws->add(h= new cl_pca(this, 1));
  h->init();
  hws->add(h= new cl_pca(this, 2));
  h->init();
  hws->add(h= new cl_pca(this, 3));
  h->init();
  hws->add(h= new cl_pca(this, 4));
  h->init();*/
  hws->add(h= new cl_89c51r_dummy_hw(this));
  h->init();
}


void
t_uc89c51r::reset(void)
{
  t_uc51r::reset();
  sfr->set_bit1(CCAPM0, bmECOM);
  sfr->set_bit1(CCAPM1, bmECOM);
  sfr->set_bit1(CCAPM2, bmECOM);
  sfr->set_bit1(CCAPM3, bmECOM);
  sfr->set_bit1(CCAPM4, bmECOM);
  //t0_overflows= 0;
  dpl0= dph0= dpl1= dph1= 0;
  sfr->set(IPH, 0);
}

int
t_uc89c51r::it_priority(uchar ie_mask)
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
t_uc89c51r::pre_inst(void)
{
  if (sfr->get(AUXR1) & bmDPS)
    {
      sfr->set(DPL, dpl1);
      sfr->set(DPH, dph1);
    }
  else
    {
      sfr->set(DPL, dpl0);
      sfr->set(DPH, dph0);
    }
  t_uc51r::pre_inst();
}

void
t_uc89c51r::post_inst(void)
{
  if (sfr->get(AUXR1) & bmDPS)
    {
      dpl1= sfr->get(DPL);
      dph1= sfr->get(DPH);
    }
  else
    {
      dpl0= sfr->get(DPL);
      dph0= sfr->get(DPH);
    }
  t_uc51r::post_inst();
}


/*
 */

cl_89c51r_dummy_hw::cl_89c51r_dummy_hw(class cl_uc *auc):
  cl_hw(auc, HW_DUMMY, 0, "_89c51r_dummy")
{}

int
cl_89c51r_dummy_hw::init(void)
{
  class cl_mem *sfr= uc->mem(MEM_SFR);
  if (!sfr)
    {
      fprintf(stderr, "No SFR to register %s[%d] into\n", id_string, id);
    }
  //auxr= sfr->register_hw(AUXR, this, 0);
  register_cell(sfr, AUXR, &auxr, wtd_restore);
  return(0);
}

void
cl_89c51r_dummy_hw::write(class cl_cell *cell, t_mem *val)
{
  if (cell == auxr)
    auxr->set_bit0(0x04);
}


/* End of s51.src/uc89c51r.cc */
