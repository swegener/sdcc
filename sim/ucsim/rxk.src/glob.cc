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

/* 
%d - signed compl.,byte jump 
%w - 2-byte jump or imm. value
%b - byte imm. value
*/
// code mask branch len mn call tick
struct dis_entry disass_rxk[]=
  {
    { 0x76, 0xff, ' ', 1, "ALTD" },
    { 0xd3, 0xff, ' ', 1, "IOI" },
    { 0xdb, 0xff, ' ', 1, "IOE" },

    { 0x00, 0xff, ' ', 1, "NOP" },
    { 0x01, 0xff, ' ', 3, "LD BC,%w" },
    { 0x11, 0xff, ' ', 3, "LD DE,%w" },
    { 0x21, 0xff, ' ', 3, "LD HL,%w" },
    { 0x31, 0xff, ' ', 3, "LD SP,%w" },
    { 0x03, 0xff, ' ', 1, "INC BC" },
    { 0x13, 0xff, ' ', 1, "INC DE" },
    { 0x23, 0xff, ' ', 1, "INC HL" },
    { 0x33, 0xff, ' ', 1, "INC SP" },
    { 0x3c, 0xff, ' ', 1, "INC A" },
    { 0x04, 0xff, ' ', 1, "INC B" },
    { 0x0c, 0xff, ' ', 1, "INC C" },
    { 0x14, 0xff, ' ', 1, "INC D" },
    { 0x1c, 0xff, ' ', 1, "INC E" },
    { 0x24, 0xff, ' ', 1, "INC H" },
    { 0x2c, 0xff, ' ', 1, "INC L" },
    { 0x0b, 0xff, ' ', 1, "DEC BC" },
    { 0x1b, 0xff, ' ', 1, "DEC DE" },
    { 0x2b, 0xff, ' ', 1, "DEC HL" },
    { 0x3b, 0xff, ' ', 1, "DEC SP" },
    { 0x3d, 0xff, ' ', 1, "DEC A" },
    { 0x05, 0xff, ' ', 1, "DEC B" },
    { 0x0d, 0xff, ' ', 1, "DEC C" },
    { 0x15, 0xff, ' ', 1, "DEC D" },
    { 0x1d, 0xff, ' ', 1, "DEC E" },
    { 0x25, 0xff, ' ', 1, "DEC H" },
    { 0x2d, 0xff, ' ', 1, "DEC L" },
    { 0x3e, 0xff, ' ', 2, "LD A,%b" },
    { 0x06, 0xff, ' ', 2, "LD B,%b" },
    { 0x0e, 0xff, ' ', 2, "LD C,%b" },
    { 0x16, 0xff, ' ', 2, "LD D,%b" },
    { 0x1e, 0xff, ' ', 2, "LD E,%b" },
    { 0x26, 0xff, ' ', 2, "LD H,%b" },
    { 0x2e, 0xff, ' ', 2, "LD L,%b" },
    { 0x07, 0xff, ' ', 1, "RLCA" },
    { 0, 0, 0, 0, 0, 0 }
  };

/* End of rxk.src/glob.cc */
