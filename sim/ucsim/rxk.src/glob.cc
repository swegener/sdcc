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
    { 0x17, 0xff, ' ', 1, "RLA" },
    { 0x0f, 0xff, ' ', 1, "RRCA" },
    { 0x1f, 0xff, ' ', 1, "RRA" },
    { 0x02, 0xff, ' ', 1, "LD (BC),A" },
    { 0x12, 0xff, ' ', 1, "LD (DE),A" },
    { 0x77, 0xff, ' ', 1, "LD (HL),A" },
    { 0x70, 0xff, ' ', 1, "LD (HL),B" },
    { 0x71, 0xff, ' ', 1, "LD (HL),C" },
    { 0x72, 0xff, ' ', 1, "LD (HL),D" },
    { 0x73, 0xff, ' ', 1, "LD (HL),E" },
    { 0x74, 0xff, ' ', 1, "LD (HL),H" },
    { 0x75, 0xff, ' ', 1, "LD (HL),L" },
    { 0x32, 0xff, ' ', 3, "LD (%w),A" },
    { 0x0a, 0xff, ' ', 1, "LD A,(BC)" },
    { 0x1a, 0xff, ' ', 1, "LD A,(DE)" },
    { 0x3a, 0xff, ' ', 3, "LD A,(%w)" },
    { 0x7e, 0xff, ' ', 1, "LD A,(HL)" },
    { 0x46, 0xff, ' ', 1, "LD B,(HL)" },
    { 0x4e, 0xff, ' ', 1, "LD C,(HL)" },
    { 0x56, 0xff, ' ', 1, "LD D,(HL)" },
    { 0x5e, 0xff, ' ', 1, "LD E,(HL)" },
    { 0x66, 0xff, ' ', 1, "LD H,(HL)" },
    { 0x6e, 0xff, ' ', 1, "LD L,(HL)" },
    { 0x37, 0xff, ' ', 1, "SCF" },
    { 0x2f, 0xff, ' ', 1, "CPL" },
    { 0x3f, 0xff, ' ', 1, "CCF" },
    { 0x08, 0xff, ' ', 1, "EX AF,AF'" }, // '
    { 0x09, 0xff, ' ', 1, "ADD HL,BC" },
    { 0x19, 0xff, ' ', 1, "ADD HL,DE" },
    { 0x29, 0xff, ' ', 1, "ADD HL,HL" },
    { 0x39, 0xff, ' ', 1, "ADD HL,SP" },
    { 0x10, 0xff, ' ', 2, "DJNZ %r" },
    { 0x18, 0xff, ' ', 2, "JR %r" },
    { 0x20, 0xff, ' ', 2, "JR NZ,%r" },
    { 0x28, 0xff, ' ', 2, "JR Z,%r" },
    { 0x30, 0xff, ' ', 2, "JR NC,%r" },
    { 0x38, 0xff, ' ', 2, "JR C,%r" },
    { 0x27, 0xff, ' ', 2, "ADD SP,%d" },
    
    { 0, 0, 0, 0, 0, 0 }
  };

/* End of rxk.src/glob.cc */
