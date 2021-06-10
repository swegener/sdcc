/*
 * Simulator of microcontrollers (m68hc11.cc)
 *
 * Copyright (C) 2020,20 Drotos Daniel, Talker Bt.
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

#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "utils.h"

#include "dregcl.h"

#include "m68hc11cl.h"

cl_m68hc11::cl_m68hc11(class cl_sim *asim):
  cl_m6800(asim)
{
}


int
cl_m68hc11::init(void)
{
  cl_m6800::init();
  
  xtal= 8000000;
  
#define RCV(R) reg_cell_var(&c ## R , &r ## R , "" #R "" , "CPU register " #R "")
  RCV(IY);
  RCV(D);
#undef RCV
  
  return 0;
}


const char *
cl_m68hc11::id_string(void)
{
  return "M68HC11";
}

void
cl_m68hc11::reset(void)
{
  cl_m6800::reset();
}


void
cl_m68hc11::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("A= $%02x %3d %+4d %c  ", rA, rA, (i8_t)rA, isprint(rA)?rA:'.');
  con->dd_printf("B= $%02x %3d %+4d %c  ", rB, rB, (i8_t)rB, isprint(rB)?rB:'.');
  con->dd_printf("D= $%04x %5d %+5d ", rD, rD, (i16_t)rD);
  con->dd_printf("\n");
  con->dd_printf("CC= "); con->print_bin(CC, 8); con->dd_printf("\n");
  con->dd_printf("      HINZVC\n");

  con->dd_printf("IX= ");
  rom->dump(0, IX, IX+7, 8, con);
  con->dd_color("answer");
  
  con->dd_printf("IY= ");
  rom->dump(0, IY, IY+7, 8, con);
  con->dd_color("answer");
  
  con->dd_printf("SP= ");
  rom->dump(0, SP, SP+7, 8, con);
  con->dd_color("answer");
  
  print_disass(PC, con);
}

/* End of m68hc12.src/m68hc11.cc */
