/*
 * Simulator of microcontrollers (glob.cc)
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

#include "glob.h"


instruction_wrapper_fn itab[256];

// code mask branch len mn call tick
struct dis_entry disass_m6800[]=
  {
    { 0x01, 0xff, ' ', 1, "NOP" },
    { 0x06, 0xff, ' ', 1, "TAP" },
    { 0x07, 0xff, ' ', 1, "TAP" },
    { 0x08, 0xff, ' ', 1, "INX" },
    { 0x09, 0xff, ' ', 1, "DEX" },
    { 0x0a, 0xff, ' ', 1, "CLV" },
    { 0x0b, 0xff, ' ', 1, "SEV" },
    { 0x0c, 0xff, ' ', 1, "CLC" },
    { 0x0d, 0xff, ' ', 1, "SEC" },
    { 0x0e, 0xff, ' ', 1, "CLI" },
    { 0x0f, 0xff, ' ', 1, "SEI" },
    { 0, 0, 0, 0, 0, 0 }
  };

/* End of m6800.src/glob.cc */
