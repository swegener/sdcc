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
<<<<<<< uc89c51r.cc
t_uc89c51r::reset(void)
{
  t_uc51r::reset();
  mem(MEM_SFR)->set_bit1(CCAPM0, bmECOM);
  mem(MEM_SFR)->set_bit1(CCAPM1, bmECOM);
  mem(MEM_SFR)->set_bit1(CCAPM2, bmECOM);
  mem(MEM_SFR)->set_bit1(CCAPM3, bmECOM);
  mem(MEM_SFR)->set_bit1(CCAPM4, bmECOM);
  t0_overflows= 0;
  dpl0= dph0= dpl1= dph1= 0;
  set_mem(MEM_SFR, IPH, 0);
}

void
t_uc89c51r::proc_write(uchar *addr)
=======
t_uc89c51r::mk_hw_elements(void)
>>>>>>> 1.2.2.3
{
<<<<<<< uc89c51r.cc
  t_uc51r::proc_write(addr);

  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP0L]))
    mem(MEM_SFR)->set_bit0(CCAPM0, bmECOM);
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP0H]))
    mem(MEM_SFR)->set_bit1(CCAPM0, bmECOM);

  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP1L]))
    mem(MEM_SFR)->set_bit0(CCAPM1, bmECOM);
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP1H]))
    mem(MEM_SFR)->set_bit1(CCAPM1, bmECOM);

  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP2L]))
    mem(MEM_SFR)->set_bit0(CCAPM2, bmECOM);
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP2H]))
    mem(MEM_SFR)->set_bit1(CCAPM2, bmECOM);

  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP3L]))
    mem(MEM_SFR)->set_bit0(CCAPM3, bmECOM);
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP3H]))
    mem(MEM_SFR)->set_bit1(CCAPM3, bmECOM);

  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP4L]))
    mem(MEM_SFR)->set_bit0(CCAPM4, bmECOM);
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[CCAP4H]))
    mem(MEM_SFR)->set_bit1(CCAPM4, bmECOM);
=======
  class cl_hw *h;
>>>>>>> 1.2.2.3

<<<<<<< uc89c51r.cc
  if (addr == &(/*MEM(MEM_SFR)*/sfr->umem8[AUXR]))
    mem(MEM_SFR)->set_bit0(AUXR, 0x04);
=======
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
>>>>>>> 1.2.2.3
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
<<<<<<< uc89c51r.cc
 * Simulating timers
 *
 * Calling inherited method to simulate timer #0, #1, #2 and then 
 * simulating Programmable Counter Array
=======
>>>>>>> 1.2.2.3
 */

int
t_uc89c51r::do_timers(int cycles)
{
  int res;

<<<<<<< uc89c51r.cc
  if ((res= t_uc51r::do_timers(cycles)) != resGO)
    return(res);
  return(do_pca(cycles));
}
=======
cl_89c51r_dummy_hw::cl_89c51r_dummy_hw(class cl_uc *auc):
  cl_hw(auc, HW_DUMMY, 0, "_89c51r_dummy")
{}
>>>>>>> 1.2.2.3

int
<<<<<<< uc89c51r.cc
t_uc89c51r::t0_overflow(void)
=======
cl_89c51r_dummy_hw::init(void)
>>>>>>> 1.2.2.3
{
<<<<<<< uc89c51r.cc
  uchar cmod= get_mem(MEM_SFR, CMOD) & (bmCPS0|bmCPS1);

  if (cmod == bmCPS1)
    t0_overflows++;
  return(0);
}


/*
 * Simulating Programmable Counter Array
 */

int
t_uc89c51r::do_pca(int cycles)
{
  int ret= resGO;
  uint ccon= get_mem(MEM_SFR, CCON);

  if (!(ccon & bmCR))
    return(resGO);
  if (state == stIDLE &&
      (ccon & bmCIDL))
    return(resGO);

  switch (get_mem(MEM_SFR, CMOD) & (bmCPS1|bmCPS0))
    {
    case 0:
      ret= do_pca_counter(cycles);
      break;
    case bmCPS0:
      ret= do_pca_counter(cycles*3);
      break;
    case bmCPS1:
      ret= do_pca_counter(t0_overflows);
      t0_overflows= 0;
      break;
    case (bmCPS0|bmCPS1):
      if ((prev_p1 & bmECI) != 0 &
	  (get_mem(MEM_SFR, P1) & bmECI) == 0)
	do_pca_counter(1);
      break;
    }
  return(ret);
}

int
t_uc89c51r::do_pca_counter(int cycles)
{
  while (cycles--)
=======
  class cl_mem *sfr= uc->mem(MEM_SFR);
  if (!sfr)
>>>>>>> 1.2.2.3
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
