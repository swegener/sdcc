/*
 * Simulator of microcontrollers (glob.cc)
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

#include <stdio.h>

#include "stypes.h"

/* PDK14 instructions described in PFS154. */


/*
%k - immediate addressing
%m - memory addressing
%mn - memory addressing when using N-bit
%i - IO addressing 
%in - IO addressing when using N-bit
%n - N-bit addressing
*/

/*  uint  code, mask;  char  branch;  uchar length;  char  *mnemonic; */

struct dis_entry disass_pdk_14[]= {
  { 0x0000, ~0x0000, ' ', 2, "nop" },
  { 0x007A, ~0x0000, ' ', 2, "ret" },
  { 0x0200, ~0x00FF, ' ', 2, "ret %k" },
  { 0x007B, ~0x0000, ' ', 2, "reti" },
  { 0x2F00, ~0x00FF, ' ', 2, "mov a, %k" },
  { 0x0180, ~0x003F, ' ', 2, "mov %i, a" },
  { 0x01C0, ~0x003F, ' ', 2, "mov a, %i" },
  { 0x0B80, ~0x007F, ' ', 2, "mov %m, a" },
  { 0x0F80, ~0x007F, ' ', 2, "mov a, %m" },
  { 0x0301, ~0x007E, ' ', 2, "ldt16 %m" },
  { 0x0300, ~0x007E, ' ', 2, "stt16 %m" },
  { 0x0381, ~0x007E, ' ', 2, "idxm a, %m" },
  { 0x0380, ~0x007E, ' ', 2, "idxm %m, a" },
  { 0x1380, ~0x007F, ' ', 2, "xch %m" },
  { 0x0072, ~0x0000, ' ', 2, "push af" },
  { 0x0073, ~0x0000, ' ', 2, "pop af" },

  { 0x2800, ~0x00FF, ' ', 2, "add %k" },
  { 0x0800, ~0x007F, ' ', 2, "add %m, a" },
  { 0x0C00, ~0x007F, ' ', 2, "add a, %m" },
  { 0x2900, ~0x00FF, ' ', 2, "sub %k" },
  { 0x0880, ~0x007F, ' ', 2, "sub %m, a" },
  { 0x0C80, ~0x007F, ' ', 2, "sub a, %m" },
  { 0x0060, ~0x0000, ' ', 2, "addc" },
  { 0x0900, ~0x007F, ' ', 2, "addc %m, a" },
  { 0x0D00, ~0x007F, ' ', 2, "addc a, %m" },
  { 0x1000, ~0x007F, ' ', 2, "addc %m" },
  { 0x0061, ~0x0000, ' ', 2, "subc" },
  { 0x0980, ~0x007F, ' ', 2, "subc %m, a" },
  { 0x0D80, ~0x007F, ' ', 2, "subc a, %m" },
  { 0x1080, ~0x007F, ' ', 2, "subc %m" },

  { 0x1200, ~0x007F, ' ', 2, "inc %m" },
  { 0x1280, ~0x007F, ' ', 2, "dec %m" },
  { 0x1300, ~0x007F, ' ', 2, "clear %m" },
  { 0x006a, ~0x0000, ' ', 2, "sr" },
  { 0x1500, ~0x007F, ' ', 2, "sr %m" },
  { 0x006B, ~0x0000, ' ', 2, "sl" },
  { 0x1580, ~0x007F, ' ', 2, "sl %m" },
  { 0x006C, ~0x0000, ' ', 2, "src" },
  { 0x1600, ~0x007F, ' ', 2, "src %m"},
  { 0x006D, ~0x0000, ' ', 2, "slc" },
  { 0x1680, ~0x007F, ' ', 2, "slc %m" },

  { 0x2C00, ~0x00FF, ' ', 2, "and %k" },
  { 0x0A00, ~0x007F, ' ', 2, "and %m, a" },
  { 0x0E00, ~0x007F, ' ', 2, "and a, %m" },
  { 0x2D00, ~0x00FF, ' ', 2, "or %k" },
  { 0x0A80, ~0x007F, ' ', 2, "or %m, a" },
  { 0x0E80, ~0x007F, ' ', 2, "or a, %m" },
  { 0x2E00, ~0x00FF, ' ', 2, "xor %k" },
  { 0x0B00, ~0x007F, ' ', 2, "xor %m, a" },
  { 0x0F00, ~0x007F, ' ', 2, "xor a, %m" },
  { 0x00C0, ~0x003F, ' ', 2, "xor %i, a" },

  { 0x0068, ~0x00FF, ' ', 2, "not" },
  { 0x1400, ~0x007F, ' ', 2, "not %m" },
  { 0x0069, ~0x00FF, ' ', 2, "neg" },
  { 0x1480, ~0x007F, ' ', 2, "neg %m" },

  { 0x1C00, ~0x01FF, ' ', 2, "set0 %in, %n" },
  { 0x2400, ~0x01FF, ' ', 2, "set0 %mn, %n" },
  { 0x1E00, ~0x01FF, ' ', 2, "set1 %in, %n" },
  { 0x2600, ~0x01FF, ' ', 2, "set1 %mn, %n" },
  { 0x1800, ~0x01FF, ' ', 2, "t0sn %in, %n" },
  { 0x2000, ~0x01FF, ' ', 2, "t0sn %mn, %n" },
  { 0x1A00, ~0x01FF, ' ', 2, "t1sn %in, %n" },
  { 0x2200, ~0x01FF, ' ', 2, "t1sn %mn, %n" },

  { 0x2A00, ~0x00FF, ' ', 2, "ceqsn %k" },
  { 0x1700, ~0x007F, ' ', 2, "ceqsn %m" },
  { 0x2B00, ~0x00FF, ' ', 2, "cneqsn %k" },
  { 0x1780, ~0x007F, ' ', 2, "cneqsn %m" },

  { 0x0062, ~0x0000, ' ', 2, "izsn" },
  { 0x1100, ~0x007F, ' ', 2, "izsn %m" },
  { 0x0063, ~0x0000, ' ', 2, "dzsn" },
  { 0x1180, ~0x007F, ' ', 2, "dzsn %m" },

  { 0x3800, ~0x07FF, ' ', 2, "call %k" },
  { 0x3000, ~0x07FF, ' ', 2, "goto %k" },

  { 0x0600, ~0x007F, ' ', 2, "comp a, %m" },
  { 0x6800, ~0x007F, ' ', 2, "comp %m, a" },
  { 0x0700, ~0x007F, ' ', 2, "nadd a, %m" },
  { 0x0780, ~0x007F, ' ', 2, "nadd %m, a" },

  { 0x006E, ~0x0000, ' ', 2, "swap" },
  { 0x0067, ~0x0000, ' ', 2, "pcadd" },
  { 0x0078, ~0x0000, ' ', 2, "engint" },
  { 0x0079, ~0x0000, ' ', 2, "disint" },
  { 0x0076, ~0x0000, ' ', 2, "stopsys" },
  { 0x0077, ~0x0000, ' ', 2, "stopexe" },
  { 0x0075, ~0x0000, ' ', 2, "reset" },
  { 0x0070, ~0x0000, ' ', 2, "wdreset" },
  { 0x0400, ~0x0000, ' ', 2, "swapc" },
  { 0x0006, ~0x0000, ' ', 2, "ldsptl" },
  { 0x0007, ~0x0000, ' ', 2, "ldspth" },

  { 0xFF00, ~0x0000, ' ', 2, "putchar" },
 
  { 0, 0, 0, 0, NULL }
};

/* glob.cc */
