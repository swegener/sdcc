/*
 * Simulator of microcontrollers (uc51r.cc)
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
#include "uc51rcl.h"
#include "regs51.h"
#include "types51.h"
#include "wdtcl.h"


/*
 * Making an 8051r CPU object
 */

t_uc51r::t_uc51r(int Itype, int Itech, class cl_sim *asim):
  t_uc52(Itype, Itech, asim)
{
  int i;

  for (i= 0; i < ERAM_SIZE; i++)
    ERAM[i]= 0;
  clock_out= 0;
}


void
t_uc51r::mk_hw_elements(void)
{
  class cl_hw *h;

  t_uc52::mk_hw_elements();
  hws->add(h= new cl_wdt(this, 0x3fff));
  h->init();
}


/*
 * Resetting of the microcontroller
 *
 * Original method is extended with handling of WDT.
 */

void
t_uc51r::reset(void)
{
  t_uc52::reset();
  sfr->set(SADDR, 0);
  sfr->set(SADEN, 0);
}


/*
 * Copying ERAM to XRAM and vice versa
 *
 * This two methods are used by command interpreter to make ERAM and
 * beginning of XRAM to be equivalent.
 */

void
t_uc51r::eram2xram(void)
{
  int i;

  for (i= 0; i < ERAM_SIZE; i++)
    set_mem(MEM_XRAM, i, ERAM[i]);
}

void
t_uc51r::xram2eram(void)
{
  int i;

  for (i= 0; i < ERAM_SIZE; i++)
    ERAM[i]= get_mem(MEM_XRAM, i);
}


void
t_uc51r::received(int c)
{
  t_mem br= sfr->get(SADDR) | sfr->get(SADEN);
  int scon= sfr->get(SCON);

  if ((0 < scon >> 6) &&
      (scon & bmSM2))
    {
      if (/* Check for individual address */
	  ((sfr->get(SADDR) & sfr->get(SADEN)) == (c & sfr->get(SADEN)))
	  ||
	  /* Check for broadcast address */
	  (br == (br & c)))
	sfr->set_bit1(SCON, bmRI);
      return;
    }
  sfr->set_bit1(SCON, bmRI);
}


/*
 * 0xe0 1 24 MOVX A,@DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc51r::inst_movx_a_$dptr(uchar code)
{
  if ((sfr->get(AUXR) & bmEXTRAM) ||
      sfr->get(DPH))
    acc->write(read_mem(MEM_XRAM,
			sfr->read(DPH)*256+
			sfr->read(DPL)));
  else
    acc->write(ERAM[sfr->read(DPL)]);
  tick(1);
  return(resGO);
}


/*
 * 0xe2-0xe3 1 24 MOVX A,@Ri
 *____________________________________________________________________________
 *
 */

int
t_uc51r::inst_movx_a_$ri(uchar code)
{
  class cl_cell *cell;
  t_mem d= 0;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  d= cell->read();
  if (sfr->get(AUXR) & bmEXTRAM)
    acc->write(read_mem(MEM_XRAM, sfr->read(P2)*256+d));
  else
    acc->write(ERAM[d]);
  tick(1);
  return(resGO);
}


/*
 * 0xf0 1 24 MOVX @DPTR,A
 *____________________________________________________________________________
 *
 */

int
t_uc51r::inst_movx_$dptr_a(uchar code)
{
  if ((sfr->get(AUXR) & bmEXTRAM) ||
      sfr->get(DPH))
    write_mem(MEM_XRAM, sfr->read(DPH)*256 + sfr->read(DPL), acc->read());
  else
    ERAM[sfr->read(DPL)]= acc->read();
  return(resGO);
}


/*
 * 0xf2-0xf3 1 24 MOVX @Ri,A
 *____________________________________________________________________________
 *
 */

int
t_uc51r::inst_movx_$ri_a(uchar code)
{
  class cl_cell *cell;
  t_mem d= 0;

  cell= iram->get_cell(get_reg(code & 0x01)->read());
  d= cell->read();
  if (sfr->get(AUXR) & bmEXTRAM)
    write_mem(MEM_XRAM, sfr->read(P2)*256 + d, acc->read());
  else
    ERAM[d]= acc->read();
  tick(1);
  return(resGO);
}


/* End of s51.src/uc51r.cc */
