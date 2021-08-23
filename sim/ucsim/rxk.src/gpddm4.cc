/*
 * Simulator of microcontrollers (gpddm4.cc)
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

#include "gpddm4.h"

struct dis_entry disass_pddm4[]=
  {
    { 0x06, 0xff, ' ', 2, "LD A,(%I+A)" },
    { 0x4c, 0xff, ' ', 2, "TEST %I" },
    { 0x1a, 0xff, ' ', 2, "LD %3,(HL)" },
    { 0xce, 0xff, ' ', 3, "LD %3,(IX%d)" },
    { 0xde, 0xff, ' ', 3, "LD %3,(IY%d)" },
    { 0xee, 0xff, ' ', 3, "LD %3,(SP+%b)" },
    { 0x1b, 0xff, ' ', 2, "LD (HL),%3" },
    { 0xcf, 0xff, ' ', 3, "LD (IX%d),%3" },
    { 0xdf, 0xff, ' ', 3, "LD (IY%d),%3" },
    { 0xef, 0xff, ' ', 3, "LD (SP+%b),%3" },
    { 0x4d, 0xff, ' ', 2, "NEG %3" },
    { 0xf1, 0xff, ' ', 2, "POP %3" },
    { 0xf5, 0xff, ' ', 2, "PUSH %3" },
    { 0x68, 0xff, ' ', 2, "RL 1,%3" },
    { 0x69, 0xff, ' ', 2, "RL 2,%3" },
    { 0x6b, 0xff, ' ', 2, "RL 4,%3" },
    { 0x48, 0xff, ' ', 2, "RLC 1,%3" },
    { 0x49, 0xff, ' ', 2, "RLC 2,%3" },
    { 0x4b, 0xff, ' ', 2, "RLC 4,%3" },
    
    { 0, 0, 0, 0, 0, 0, 0 }
  };
  
/* End of rxk.src/gpddm4.cc */
