/*
 * Simulator of microcontrollers (gp0m4.cc)
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

#include "gp0m4.h"

struct dis_entry disass_p0m4[]=
  {
    { 0x42, 0xff, ' ', 1, "RL HL" },
    { 0x62, 0xff, ' ', 1, "RL BC" },
    { 0x45, 0xff, ' ', 1, "SUB HL,JK" },
    { 0x55, 0xff, ' ', 1, "SUB HL,DE" },
    { 0x4c, 0xff, ' ', 1, "TEST HL" },

    { 0, 0, 0, 0, 0, 0 }
  };
  
/* End of rxk.src/gp0m4.cc */
