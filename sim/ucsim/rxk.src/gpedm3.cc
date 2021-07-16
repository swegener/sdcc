/*
 * Simulator of microcontrollers (gpedm3.cc)
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

#include "gpedm3.h"

struct dis_entry disass_pedm3[]=
  {
    { 0x47, 0xff, ' ', 2, "LD EIR,A" },
    { 0x4f, 0xff, ' ', 2, "LD IIR,A" },
    { 0x57, 0xff, ' ', 2, "LD A,EIR" },
    { 0x5f, 0xff, ' ', 2, "LD A,IIR" },
    { 0xa0, 0xff, ' ', 2, "LDI" },
    { 0xa8, 0xff, ' ', 2, "LDD" },
    { 0x54, 0xff, ' ', 2, "EXX (SP),HL" },
    { 0x4b, 0xff, ' ', 4, "LD BC,(%w)" },
    { 0x5b, 0xff, ' ', 4, "LD DE,(%w)" },
    { 0x6b, 0xff, ' ', 4, "LD HL,(%w)" },
    { 0x7b, 0xff, ' ', 4, "LD SP,(%w)" },
    
    { 0x42, 0xff, ' ', 2, "SBC HL,BC" },
    { 0x52, 0xff, ' ', 2, "SBC HL,DE" },
    { 0x62, 0xff, ' ', 2, "SBC HL,HL" },
    { 0x72, 0xff, ' ', 2, "SBC HL,SP" },
    { 0x4a, 0xff, ' ', 2, "ADC HL,BC" },
    { 0x5a, 0xff, ' ', 2, "ADC HL,DE" },
    { 0x6a, 0xff, ' ', 2, "ADC HL,HL" },
    { 0x7a, 0xff, ' ', 2, "ADC HL,SP" },

    { 0, 0, 0, 0, 0, 0 }
  };
  
/* End of rxk.src/gpedm3.cc */
