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
    { 0xa2, 0xff, ' ', 6, "LLJP GT,%X,%w" },
    { 0xaa, 0xff, ' ', 6, "LLJP GTU,%X,%w" },
    { 0xb2, 0xff, ' ', 6, "LLJP LT,%X,%w" },
    { 0xba, 0xff, ' ', 6, "LLJP V,%X,%w" },
    { 0xc2, 0xff, ' ', 6, "LLJP NZ,%X,%w" },
    { 0xca, 0xff, ' ', 6, "LLJP Z,%X,%w" },
    { 0xd2, 0xff, ' ', 6, "LLJP NC,%X,%w" },
    { 0xda, 0xff, ' ', 6, "LLJP C,%X,%w" },
    { 0xa5, 0xff, ' ', 4, "PUSH %w" },
    { 0xa3, 0xff, ' ', 4, "JRE GT,%R" },
    { 0xb3, 0xff, ' ', 4, "JRE LT,%R" },
    { 0xab, 0xff, ' ', 4, "JRE GTU,%R" },
    { 0xbb, 0xff, ' ', 4, "JRE V,%R" },
    { 0xc3, 0xff, ' ', 4, "JRE NZ,%R" },
    { 0xcb, 0xff, ' ', 4, "JRE Z,%R" },
    { 0xd3, 0xff, ' ', 4, "JRE NC,%R" },
    { 0xdb, 0xff, ' ', 4, "JRE C,%R" },
    { 0xc4, 0xff, ' ', 2, "FLAG NZ,HL" },
    { 0xcc, 0xff, ' ', 2, "FLAG Z,HL" },
    { 0xd4, 0xff, ' ', 2, "FLAG NC,HL" },
    { 0xdc, 0xff, ' ', 2, "FLAG C,HL" },
    { 0xa4, 0xff, ' ', 2, "FLAG GT,HL" },
    { 0xb4, 0xff, ' ', 2, "FLAG LT,HL" },
    { 0xac, 0xff, ' ', 2, "FLAG GTU,HL" },
    { 0xbc, 0xff, ' ', 2, "FLAG V,HL" },
    { 0xea, 0xff, ' ', 2, "CALL (HL)" },
    { 0x8b, 0xff, ' ', 2, "LLRET" },
    { 0xd9, 0xff, ' ', 2, "EXP" },
    { 0x40, 0xff, ' ', 2, "LD HTR,A" },
    { 0x50, 0xff, ' ', 2, "LD A,HTR" },
    { 0x55, 0xff, ' ', 2, "SCALL" },
    { 0x83, 0xff, ' ', 2, "SRET" },
    { 0xb5, 0xff, ' ', 4, "SETUSRP %w" },
    { 0xb1, 0xff, ' ', 4, "SETSYSP %w" },
    { 0xfa, 0xff, ' ', 2, "LLCALL (JKHL)" },

    { 0, 0, 0, 0, 0, 0, 0 }
  };
  
/* End of rxk.src/gpedm4.cc */
