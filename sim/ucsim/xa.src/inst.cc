/*
 * Simulator of microcontrollers (inst.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 *
 * Written by Karl Bongers karl@turbobit.com
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

#include "ddconfig.h"

// local
#include "glob.h"
#include "xacl.h"
#include "regsxa.h"

int
cl_xa::get_reg(int word_flag, unsigned int index)
{
  if (index < 3) { /* banked */
    if (word_flag)
      return get_word_direct(0x400+index);
    else
      return mem_direct[0x400+index];
  } else { /* non-banked */
    if (word_flag)
      return get_word_direct(0x400+index);
    else
      return mem_direct[0x400+index];
  }
}

int
cl_xa::inst_NOP(uint code)
{
  return(resGO);
}

#define RI_F0 ((code >> 4) & 0xf)
#define RI_70 ((code >> 4) & 0x7)
#define RI_0F (code & 0xf)
#define RI_07 (code & 0x7)

int
cl_xa::inst_ADD(uint code)
{
  int operands = code >> 16;  // kludgy, param info

#define FUNC1 add1
#define FUNC2 add2
#include "inst_gen.cc"

  return(resGO);
}

int
cl_xa::inst_ADDC(uint code)
{
  int operands = code >> 16;  // kludgy, param info

#define FUNC1 addc1
#define FUNC2 addc2
#include "inst_gen.cc"

  return(resGO);
}

int
cl_xa::inst_SUB(uint code)
{
  return(resGO);
}

int
cl_xa::inst_SUBB(uint code)
{
  return(resGO);
}

int
cl_xa::inst_CMP(uint code)
{
  return(resGO);
}
int
cl_xa::inst_AND(uint code)
{
  return(resGO);
}
int
cl_xa::inst_OR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_XOR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_ADDS(uint code)
{
  return(resGO);
}
int
cl_xa::inst_NEG(uint code)
{
  return(resGO);
}
int
cl_xa::inst_SEXT(uint code)
{
  return(resGO);
}
int
cl_xa::inst_MUL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_DIV(uint code)
{
  return(resGO);
}
int
cl_xa::inst_DA(uint code)
{
  return(resGO);
}
int
cl_xa::inst_ASL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_ASR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_LEA(uint code)
{
  return(resGO);
}
int
cl_xa::inst_CPL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_LSR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_NORM(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RLC(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RRC(uint code)
{
  return(resGO);
}
int
cl_xa::inst_MOVS(uint code)
{
  return(resGO);
}
int
cl_xa::inst_MOVC(uint code)
{
  return(resGO);
}
int
cl_xa::inst_MOVX(uint code)
{
  return(resGO);
}
int
cl_xa::inst_PUSH(uint code)
{
  return(resGO);
}
int
cl_xa::inst_POP(uint code)
{
  return(resGO);
}
int
cl_xa::inst_XCH(uint code)
{
  return(resGO);
}
int
cl_xa::inst_SETB(uint code)
{
  return(resGO);
}
int
cl_xa::inst_CLR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_MOV(uint code)
{
  return(resGO);
}
int
cl_xa::inst_ANL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_ORL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_BR(uint code)
{
  return(resGO);
}
int
cl_xa::inst_JMP(uint code)
{
  return(resGO);
}
int
cl_xa::inst_CALL(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RET(uint code)
{
  return(resGO);
}
int
cl_xa::inst_Bcc(uint code)
{
  return(resGO);
}
int
cl_xa::inst_JB(uint code)
{
  return(resGO);
}
int
cl_xa::inst_JNB(uint code)
{
  return(resGO);
}
int
cl_xa::inst_CJNE(uint code)
{
  return(resGO);
}
int
cl_xa::inst_DJNZ(uint code)
{
  return(resGO);
}
int
cl_xa::inst_JZ(uint code)
{
  return(resGO);
}
int
cl_xa::inst_JNZ(uint code)
{
  return(resGO);
}
int
cl_xa::inst_BKPT(uint code)
{
  return(resGO);
}
int
cl_xa::inst_TRAP(uint code)
{
  return(resGO);
}
int
cl_xa::inst_RESET(uint code)
{
  return(resGO);
}


/* End of xa.src/inst.cc */
