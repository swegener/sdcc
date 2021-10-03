/*
 * Simulator of microcontrollers (g11p0.cc)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
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

#include "glob11.h"

// code, mask, branch, len, mnem, iscall, ticks
struct dis_entry disass11p0[]= {

  { 0x00, 0xff, ' ', 1, "TEST" },
  { 0x02, 0xff, ' ', 1, "IDIV" },
  { 0x03, 0xff, ' ', 1, "FDIV" },
  { 0x04, 0xff, ' ', 1, "LSRD" },
  { 0x05, 0xff, ' ', 1, "ASLD" },
  { 0x21, 0xff, 'R', 2, "BRN %r" },
  { 0x38, 0xff, ' ', 1, "PULX" },
  { 0x3a, 0xff, ' ', 1, "ABX" },
  { 0x3c, 0xff, ' ', 1, "PSHX" },
  { 0x3d, 0xff, ' ', 1, "MUL" },
  { 0x83, 0xff, ' ', 3, "SUBD %B" },
  
  { 0, 0, 0, 0, 0, 0 }
};

/* End of m6800.src/g11p0.cc */
