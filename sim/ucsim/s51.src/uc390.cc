/*
 * Simulator of microcontrollers (uc390.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 * uc390.cc - module created by Karl Bongers 2001, karl@turbobit.com
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

#include "uc390cl.h"
#include "regs51.h"


/*
 * Making an 390 CPU object
 */

t_uc390::t_uc390(int Itype, int Itech, class cl_sim *asim):
  t_uc52(Itype, Itech, asim)
{
}

/*
 * 0x05 2 12 INC addr
 *____________________________________________________________________________
 *
 */
int
t_uc390::inst_inc_addr(uchar code)
{
  uchar *addr;

  addr= get_direct(fetch(), &event_at.wi, &event_at.ws);

  /* mask off the 2Hex bit adjacent to the 1H bit which selects
     which DPTR we use.  This is a feature of 80C390.
     You can do INC DPS and it only effects bit 1. */
  if (code  == DPS)
    (*addr) ^= 1;  /* just toggle */
  else {
    (*addr)++;
  }

  proc_write(addr);
  return(resGO);
}

/*
 * 0xa3 1 24 INC DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_inc_dptr(uchar code)
{
  uint dptr;

  unsigned char pl,ph,dps;

  dps = sfr->get(DPS);
  if (dps & 1) {
    pl = DPL1;
    ph = DPH1;
  } else {
    pl = DPL;
    ph = DPH;
  }

  if (dps & 1) { /* alternate DPTR */
    if (dps & 0x80)  /* decr set */
      dptr= sfr->get(ph)*256 + sfr->get(pl) - 1;
    else
      dptr= sfr->get(ph)*256 + sfr->get(pl) + 1;
  } else {
    if (dps & 0x40)  /* decr set */
      dptr= sfr->get(ph)*256 + sfr->get(pl) - 1;
    else
      dptr= sfr->get(ph)*256 + sfr->get(pl) + 1;
  }

  sfr->set(event_at.ws= ph, (dptr >> 8) & 0xff);
  sfr->set(pl, dptr & 0xff);

  if (dps & 0x20) {                  /* auto-switch dptr */
    sfr->set(DPS, (dps ^ 1));  /* toggle dual-dptr switch */
  }
  tick(1);
  return(resGO);
}

/*
 * 0x73 1 24 JMP @A+DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_jmp_$a_dptr(uchar code)
{
    unsigned char pl,ph,dps;

    dps = sfr->get(DPS);
    if (dps & 1) {
      pl = DPL1;
      ph = DPH1;
    } else {
      pl = DPL;
      ph = DPH;
    }

    PC= (sfr->get(ph)*256 + sfr->get(pl) +
         read_mem(MEM_SFR, ACC)) &
      (EROM_SIZE - 1);

  tick(1);
  return(resGO);
}

/*
 * 0x90 3 24 MOV DPTR,#data
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_mov_dptr_$data(uchar code)
{
    unsigned char pl,ph,dps;

    dps = sfr->get(DPS);
    if (dps & 1) {
      pl = DPL1;
      ph = DPH1;
    } else {
      pl = DPL;
      ph = DPH;
    }

    sfr->set(event_at.ws= ph, fetch());
    sfr->set(pl, fetch());

    if (dps & 0x20) {                  /* auto-switch dptr */
      sfr->set(DPS, (dps ^ 1));  /* toggle dual-dptr switch */
    }

  tick(1);
  return(resGO);
}


/*
 * 0x93 1 24 MOVC A,@A+DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_movc_a_$a_dptr(uchar code)
{
    unsigned char pl,ph,dps;

    dps = sfr->get(DPS);
    if (dps & 1) {
      pl = DPL1;
      ph = DPH1;
    } else {
      pl = DPL;
      ph = DPH;
    }

    sfr->set(ACC, get_mem(MEM_ROM, event_at.rc=
          			(sfr->get(ph)*256+sfr->get(pl) +
         			 sfr->get(ACC)) & (EROM_SIZE-1)));

#if 0
    if (dps & 0x10) {                  /* auto-incr */
      unsigned char t;
      t = (sfr->get(pl) + 1) & 0xff;
      sfr->set(pl, t);
      if (t == 0) { /* overflow */
        t = (sfr->get(ph) + 1) & 0xff;
        sfr->set(ph, t);
      }
    }
#endif

    if (dps & 0x20) {                  /* auto-switch dptr */
      sfr->set(DPS, (dps ^ 1));  /* toggle dual-dptr switch */
    }

  tick(1);
  return(resGO);
}

/*
 * 0xe0 1 24 MOVX A,@DPTR
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_movx_a_$dptr(uchar code)
{
    unsigned char pl,ph,dps;

    dps = sfr->get(DPS);
    if (dps & 1) {
      pl = DPL1;
      ph = DPH1;
    } else {
      pl = DPL;
      ph = DPH;
    }

    sfr->set(event_at.ws= ACC,
  	   get_mem(MEM_XRAM, event_at.rx=sfr->get(ph)*256+sfr->get(pl)));

#if 0
    if (dps & 0x10) {                  /* auto-incr */
      unsigned char t;
      t = (sfr->get(pl) + 1) & 0xff;
      sfr->set(pl, t);
      if (t == 0) { /* overflow */
        t = (sfr->get(ph) + 1) & 0xff;
        sfr->set(ph, t);
      }
    }
#endif

    if (dps & 0x20) {                  /* auto-switch dptr */
      sfr->set(DPS, (dps ^ 1));  /* toggle dual-dptr switch */
    }
  tick(1);
  return(resGO);
}

/*
 * 0xf0 1 24 MOVX @DPTR,A
 *____________________________________________________________________________
 *
 */

int
t_uc390::inst_movx_$dptr_a(uchar code)
{
    unsigned char pl,ph,dps;

    dps = sfr->get(DPS);
    if (dps & 1) {
      pl = DPL1;
      ph = DPH1;
    } else {
      pl = DPL;
      ph = DPH;
    }

    set_mem(MEM_XRAM, event_at.wx= sfr->get(ph)*256+sfr->get(pl),
  	  sfr->get(event_at.rs= ACC));

#if 0
    if (dps & 0x10) {                  /* auto-incr */
      unsigned char t;
      t = (sfr->get(pl) + 1) & 0xff;
      sfr->set(pl, t);
      if (t == 0) { /* overflow */
        t = (sfr->get(ph) + 1) & 0xff;
        sfr->set(ph, t);
      }
    }
#endif

    if (dps & 0x20) {                  /* auto-switch dptr */
      sfr->set(DPS, (dps ^ 1));  /* toggle dual-dptr switch */
    }
  tick(1);
  return(resGO);
}



/* End of s51.src/uc390.cc */
