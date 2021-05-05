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
struct dis_entry disass_mcs6502[]=
  {
    { 0xea, 0xff, ' ', 1, "NOP" },
    { 0x00, 0xff, ' ', 1, "BRK" },
    { 0x40, 0xff, ' ', 1, "RTI" },
    { 0x58, 0xff, ' ', 1, "CLI" },
    { 0x78, 0xff, ' ', 1, "SEI" },
    { 0x08, 0xff, ' ', 1, "PHP" },
    { 0x18, 0xff, ' ', 1, "CLC" },
    { 0x28, 0xff, ' ', 1, "PLP" },
    { 0x38, 0xff, ' ', 1, "SEC" },
    { 0x48, 0xff, ' ', 1, "PHA" },
    { 0x68, 0xff, ' ', 1, "PLA" },
    { 0x88, 0xff, ' ', 1, "DEY" },
    { 0x98, 0xff, ' ', 1, "TYA" },
    { 0xa8, 0xff, ' ', 1, "TAY" },
    { 0xb8, 0xff, ' ', 1, "CLV" },
    { 0xc8, 0xff, ' ', 1, "INY" },
    { 0xd8, 0xff, ' ', 1, "CLD" },
    { 0xe8, 0xff, ' ', 1, "INX" },
    { 0xf8, 0xff, ' ', 1, "SED" },
    { 0x8a, 0xff, ' ', 1, "TXA" },
    { 0x9a, 0xff, ' ', 1, "TXS" },
    { 0xaa, 0xff, ' ', 1, "TAX" },
    { 0xba, 0xff, ' ', 1, "TSX" },
    { 0xca, 0xff, ' ', 1, "DEX" },
    { 0, 0, 0, 0, 0, 0 }
  };

/* End of mcs6502.src/glob.cc */
