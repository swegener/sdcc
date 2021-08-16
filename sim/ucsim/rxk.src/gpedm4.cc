/*
 * Simulator of microcontrollers (gpedm4.cc)
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

#include "gpedm4.h"

struct dis_entry disass_pedm4[]=
  {
    { 0x00, 0xff, ' ', 3, "CBM %b" },
    { 0x01, 0xff, ' ', 2, "LD PW,(HTR+HL)" },
    { 0x11, 0xff, ' ', 2, "LD PX,(HTR+HL)" },
    { 0x21, 0xff, ' ', 2, "LD PY,(HTR+HL)" },
    { 0x31, 0xff, ' ', 2, "LD PZ,(HTR+HL)" },
    { 0x02, 0xff, ' ', 2, "SBOX A" },
    { 0x12, 0xff, ' ', 2, "IBOX A" },
    { 0x10, 0xff, ' ', 3, "DWJNZ %r" },
    { 0x48, 0xff, ' ', 2, "CP HL,DE" },
    { 0x4c, 0xff, ' ', 2, "TEST BC" },

    { 0, 0, 0, 0, 0, 0 }
  };
  
/* End of rxk.src/gpedm4.cc */
