/*
 * Simulator of microcontrollers (uc52.cc)
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
#include "uc52cl.h"
#include "regs51.h"
#include "timer2cl.h"


/*
 * Making an 8052 CPU object
 */

cl_uc52::cl_uc52(int Itype, int Itech, class cl_sim *asim):
  cl_51core(Itype, Itech, asim)
{
}


void
cl_uc52::mk_hw_elements(void)
{
  class cl_hw *h;

  cl_51core::mk_hw_elements();
  add_hw(h= new cl_timer2(this, 2, "timer2", t2_default|t2_down));
  h->init();
}

void
cl_uc52::make_memories(void)
{
  class cl_address_space *as;
  int i;
  
  rom= as= new cl_address_space("rom", 0, 0x10000, 8);
  as->init();
  address_spaces->add(as);
  iram= as= new cl_address_space("iram", 0, 0x100, 8);
  as->init();
  address_spaces->add(as);
  sfr= as= new cl_address_space("sfr", 0x80, 0x80, 8);
  as->init();
  address_spaces->add(as);
  xram= as= new cl_address_space("xram", 0, 0x10000, 8);
  as->init();
  address_spaces->add(as);

  class cl_address_decoder *ad;
  class cl_memory_chip *chip, *iram_chip, *sfr_chip;

  chip= new cl_memory_chip("rom_chip", 0x10000, 8);
  chip->init();
  memchips->add(chip);
  ad= new cl_address_decoder(as= address_space("rom"), chip, 0, 0xffff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  chip= iram_chip= new cl_memory_chip("iram_chip", 0x100, 8);
  chip->init();
  memchips->add(chip);
  ad= new cl_address_decoder(as= address_space("iram"), chip, 0, 0xff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  chip= new cl_memory_chip("xram_chip", 0x10000, 8);
  chip->init();
  memchips->add(chip);
  ad= new cl_address_decoder(as= address_space("xram"), chip, 0, 0xffff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  sfr_chip= chip= new cl_memory_chip("sfr_chip", 0x80, 8);
  chip->init();
  memchips->add(chip);
  ad= new cl_address_decoder(as= address_space("sfr"), chip, 0x80, 0xff, 0);
  ad->init();
  as->decoders->add(ad);
  ad->activate(0);

  acc= sfr->get_cell(ACC);
  psw= sfr->get_cell(PSW);
  regs= new cl_address_space("regs", 0, 8, 8);
  regs->init();
  address_spaces->add(regs);
  
  cl_banker *b= new cl_banker(sfr, 0xd0, 0x18,
			      regs, 0, 7);
  b->init();
  regs->decoders->add(b);
  b->add_bank(0, memory("iram_chip"), 0);
  b->add_bank(1, memory("iram_chip"), 8);
  b->add_bank(2, memory("iram_chip"), 16);
  b->add_bank(3, memory("iram_chip"), 24);
  psw->write(0);
  for (i= 0; i < 8; i++)
    R[i]= regs->get_cell(i);
  
  cl_var *v;
  vars->add(v= new cl_var(cchars("R0"), regs, 0));
  v->init();
  vars->add(v= new cl_var(cchars("R1"), regs, 1));
  v->init();
  vars->add(v= new cl_var(cchars("R2"), regs, 2));
  v->init();
  vars->add(v= new cl_var(cchars("R3"), regs, 3));
  v->init();
  vars->add(v= new cl_var(cchars("R4"), regs, 4));
  v->init();
  vars->add(v= new cl_var(cchars("R5"), regs, 5));
  v->init();
  vars->add(v= new cl_var(cchars("R6"), regs, 6));
  v->init();
  vars->add(v= new cl_var(cchars("R7"), regs, 7));
  v->init();

  dptr= new cl_address_space("dptr", 0, 2, 8);
  dptr->init();
  address_spaces->add(dptr);

  if (type == CPU_C521)
    {
      /*
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
      */
      // DPL=82  DPH=83
      // DPL1=84 DPH1=85
      // DPS=86
      cl_banker *banker= new cl_banker(sfr, 0x86, 1,
				       dptr, 0, 1);
      banker->init();
      dptr->decoders->add(banker);

      banker->add_bank(0, memory("sfr_chip"), 0x82-0x80);
      banker->add_bank(1, memory("sfr_chip"), 0x84-0x80);
      banker->activate(0);
      sfr->write(0x86, 0);
    }
  else
    {
      ad= new cl_address_decoder(dptr, sfr_chip, 0, 1, DPL-0x80);
      ad->init();
      dptr->decoders->add(ad);
      ad->activate(0);
    }

  bits= as= new cl_address_space("bits", 0, 0x100, 1);
  as->init();
  address_spaces->add(as);
  ad= new cl_bander(bits, 0, 127,
		    iram_chip, 32,
		    8, 1);
  ad->init();
  bits->decoders->add(ad);
  ad->activate(0);
  ad= new cl_bander(bits, 128, 255,
		    sfr_chip, 0,
		    8, 8);
  ad->init();
  bits->decoders->add(ad);
  ad->activate(0);
}


void
cl_uc52::clear_sfr(void)
{
  cl_51core::clear_sfr();
  sfr->write(T2CON, 0);
  sfr->write(TH2, 0);
  sfr->write(TL2, 0);
  sfr->write(RCAP2L, 0);
  sfr->write(RCAP2H, 0);
}


/*
 * Calculating address of indirectly addressed IRAM cell
 *
 */

class cl_memory_cell *
cl_uc52::get_indirect(uchar addr, int *res)
{
  *res= resGO;
  return(iram->get_cell(addr));
}


/* End of s51.src/uc52.cc */
