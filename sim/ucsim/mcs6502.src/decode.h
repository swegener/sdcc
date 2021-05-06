/*
 * Simulator of microcontrollers (decode.h)
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

#ifndef DECODE_HEADER
#define DECODE_HEADER

#define NOP	instruction_ea
#define BRK	instruction_00
#define RTI	instruction_40
#define CLI	instruction_58
#define SEI	instruction_78
#define PHP	instruction_08
#define CLC	instruction_18
#define PLP	instruction_28
#define SEc	instruction_38
#define PHA	instruction_48
#define PLA	instruction_68
#define DEY	instruction_88
#define TYA	instruction_98
#define TAY	instruction_a8
#define CLV	instruction_b8
#define INY	instruction_c8
#define CLD	instruction_d8
#define INX	instruction_e8
#define SED	instruction_f8
#define TXA	instruction_8a
#define TXS	instruction_9a
#define TAX	instruction_aa
#define TSX	instruction_ba
#define DEX	instruction_ca

#endif

/* End of mcs6502.src/decode.h */
