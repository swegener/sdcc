/*
 * Simulator of microcontrollers (glob.cc)
 *
 * Copyright (C) 2022 Drotos Daniel, Talker Bt.
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

#include "f8cl.h"

#include "glob.h"

instruction_wrapper_fn itab[256];

// code mask branch len mn call tick
struct dis_entry disass_f8[]=
  {
    // move
    { 0x80, 0xff, ' ', 2, "ld %a,#'i8'" },
    { 0x81, 0xff, ' ', 3, "ld %a,'a16_8'" },
    { 0x82, 0xff, ' ', 2, "ld %a,('nsp_8')" },
    { 0x83, 0xff, ' ', 3, "ld %a,('nnz_8')" },
    { 0x84, 0xff, ' ', 1, "ld %a,('y_8')" },
    { 0x85, 0xff, ' ', 2, "ld %a,('ny_8')" },
    { 0x86, 0xff, ' ', 1, "ld %a,xh" },
    { 0x87, 0xff, ' ', 1, "ld %a,yl" },
    { 0x88, 0xff, ' ', 1, "ld %a,yh" },
    { 0x89, 0xff, ' ', 1, "ld %a,zl" },
    { 0x8a, 0xff, ' ', 1, "ld %a,zh" },
    { 0x8b, 0xff, ' ', 3, "ld 'a16_8',%a" },
    { 0x8c, 0xff, ' ', 2, "ld ('nsp_8'),%a" },
    { 0x8d, 0xff, ' ', 3, "ld ('nnz_8'),%a" },
    { 0x8e, 0xff, ' ', 1, "ld ('y_8'),%a" },
    { 0x8f, 0xff, ' ', 2, "ld ('ny_8'),%a" },
    { 0x94, 0xff, ' ', 2, "ld yl,#'i8'" },
    
    { 0xc0, 0xff, ' ', 3, "ldw %A,#'i16'" },
    { 0xc1, 0xff, ' ', 3, "ldw %A,'a16_16'" },
    { 0xc2, 0xff, ' ', 2, "ldw %A,('nsp_16')" },
    { 0xc3, 0xff, ' ', 3, "ldw %A,('nnz_16')" },
    { 0xc4, 0xff, ' ', 2, "ldw %A,('ny_16')" },
    { 0xc5, 0xff, ' ', 1, "ldw %A,('y_16')" },
    { 0xc6, 0xff, ' ', 1, "ldw %A,x" },
    { 0xc7, 0xff, ' ', 2, "ldw %A,#%d" },
    { 0xc8, 0xff, ' ', 3, "ldw 'a16_16',%A" },
    { 0xc9, 0xff, ' ', 2, "ldw ('nsp_16'),%A" },
    { 0xca, 0xff, ' ', 3, "ldw ('nnz_16'),%A" },
    { 0xcb, 0xff, ' ', 1, "ldw x,%A" },
    { 0xcc, 0xff, ' ', 1, "ldw z,%A" },
    { 0xcd, 0xff, ' ', 1, "ldw (%A),x" },
    { 0xce, 0xff, ' ', 2, "ldw ('nA_16'),x" },
    { 0xcf, 0xff, ' ', 3, "ldw ('nnA_16'),x" },
    { 0x70, 0xff, ' ', 1, "ldw sp,%A" },
    { 0x74, 0xff, ' ', 1, "ldw ('dsp_16'),%A" },

    { 0x60, 0xff, ' ', 3, "push 'a16_8'" },
    { 0x60, 0xff, ' ', 2, "push ('nsp_8')" },
    { 0x60, 0xff, ' ', 1, "push %a" },
    { 0x60, 0xff, ' ', 1, "push zh" },
    { 0x90, 0xff, ' ', 2, "push #'i8'" },

    { 0xb0, 0xff, ' ', 3, "pushw 'a16_16'" },
    { 0xb1, 0xff, ' ', 2, "pushw ('nsp_16')" },
    { 0xb2, 0xff, ' ', 3, "pushw ('nnz_16')" },
    { 0xb3, 0xff, ' ', 1, "pushw %A" },
    { 0xe8, 0xff, ' ', 3, "pushw #'i16'" },

    { 0x99, 0xff, ' ', 1, "pop %a" },
    { 0xe9, 0xff, ' ', 1, "popw %A" },

    { 0x91, 0xff, ' ', 2, "xch %a,('nsp_8')" },
    { 0x92, 0xff, ' ', 1, "xch %a,('y_8')" },
    { 0x93, 0xff, ' ', 1, "xch %L,%H," },
    { 0xf4, 0xff, ' ', 1, "xch y,('z_16')" },
    { 0xf5, 0xff, ' ', 1, "xch z,('nsp_16')" },

    { 0x9b, 0xff, ' ', 1, "cax ('y_8'),xh,yl" },
    { 0xf9, 0xff, ' ', 1, "caxw ('y_16'),z,x" },

    { 0x58, 0xff, ' ', 3, "clr 'a16_8'" },
    { 0x59, 0xff, ' ', 2, "clr ('nsp_8')" },
    { 0x5a, 0xff, ' ', 1, "clr %a" },
    { 0x5b, 0xff, ' ', 1, "clr zh" },
    { 0xa0, 0xff, ' ', 3, "clrw 'a16_16'" },
    { 0xa1, 0xff, ' ', 2, "clrw ('nsp_16')" },
    { 0xa2, 0xff, ' ', 3, "clrw ('nnz_16')" },
    { 0xa3, 0xff, ' ', 1, "clrw %A" },

    // alu

    // branch

    // other
    { 0x00, 0xff, ' ', 1, "TRAP" },
    { 0x08, 0xff, ' ', 1, "NOP" },

    { 0, 0, 0, 0, 0, 0 }
  };


struct cpu_entry cpus_f8[]=
  {
    {"F8"	, CPU_F8, 0		, "F8", ""},

    {NULL, CPU_NONE, 0, "", ""}
  };

u16_t tick_tab_f8[256]= {
  /*           _0    _1    _2    _3      _4    _5    _6    _7      _8    _9    _a    _b      _c    _d    _e    _f */
  /* 0_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 1_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 2_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 3_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 4_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 5_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 6_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 7_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 8_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* 9_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* a_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* b_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* c_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* d_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* e_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 
  /* f_ */    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0
};

// Bitmasks of P_XXXX
enum {
  // shorts for allowed prefixes
  PN		= P_NONE,                      // none
  PA		= P_SWAP|P_ALT0|P_ALT1|P_ALT2, // 012
  P8		= P_ALT0,                      // 1
  P6		= P_ALT1|P_ALT2,               // 2
  PD		= P8|P6,                       // 12
  P1		= P_SWAP|P8,                   // 01
  P2		= P_SWAP|P6                    // 02
};

u8_t allowed_prefs[256]= {
  /*          _0  _1  _2  _3    _4  _5  _6  _7    _8  _9  _a  _b    _c  _d  _e  _f */
  /* 0_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 1_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 2_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 3_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 4_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0, PN, 
  /* 5_ */     0,  0,  0,  0,    0,  0,  0,  0,   PD, PD, PD, PD,    0,  0,  0, PN, 
  /* 6_ */    PD, PD, PD, PD,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 7_ */    PD,  0,  0,  0,   PD,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* 8_ */    PD, PD, PD, PD,   PD, PD, PA, PA,   PA, PA, PA, PD,   PD, PD, PD, PD, 
  /* 9_ */    PN, PD, PD, P6,    0,  0,  0,  0,    0, PD,  0, PN,    0,  0, P6,  0, 
  /* a_ */    P6, P6, P6, P6,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* b_ */    P6, P6, P6, P6,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* c_ */    P6, P6, P6, P6,   P6, P6, P6, P6,   P6, P6, P6, P6,   P6, P6, P6, P6, 
  /* d_ */     0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0, 
  /* e_ */     0,  0,  0,  0,    0,  0,  0,  0,   PN,  0,  0,  0,    0,  0,  0,  0, 
  /* f_ */     0,  0,  0,  0,    0,  0,  0,  0,    0, PN,  0,  0,    0,  0,  0,  0
};


/* End of f8.src/glob.cc */
