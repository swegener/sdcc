/*
 * Simulator of microcontrollers (glob.h)
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

#ifndef GLOB_HEADER
#define GLOB_HEADER

#include "stypes.h"


#if 0
enum {
  REG,
  IND_REG,
  IND_REG_PLUS,
  IND_REG_OFFSET,
  DIRECT,
  DATA8,
  DATA16
};
#endif

enum {
BAD_OPCODE=0,
ADD,
ADDC,
SUB,
SUBB,
CMP,
AND,
OR,
XOR,
ADDS,
NEG,
SEXT,
MUL,
DIV,
DA,
ASL,
ASR,
LEA,
CPL,
LSR,
NORM,
RL,
RLC,
RR,
RRC,
MOVS,
MOVC,
MOVX,
PUSH,
POP,
XCH,
SETB,
CLR,
MOV,
ANL,
ORL,
BR,
JMP,
CALL,
RET,
Bcc,
JB,
JNB,
CJNE,
DJNZ,
JZ,
JNZ,
NOP,
BKPT,
TRAP,
RESET,
};

extern char *op_mnemonic_str[];

enum op_operands {
   // the repeating common parameter encoding for ADD, ADDC, SUB, AND...
  REG_REG         ,
  REG_IREG        ,
  IREG_REG        ,
  REG_IREGOFF8    ,
  IREGOFF8_REG    ,
  REG_IREGOFF16   ,
  IREGOFF16_REG   ,
  REG_IREGINC     ,
  IREGINC_REG     ,
  DIRECT_REG      ,
  REG_DIRECT      ,
  REG_DATA8       ,
  REG_DATA16      ,
  IREG_DATA8      ,
  IREG_DATA16     ,
  IREGINC_DATA8   ,
  IREGINC_DATA16  ,
  IREGOFF8_DATA8  ,
  IREGOFF8_DATA16 ,
  IREGOFF16_DATA8 ,
  IREGOFF16_DATA16,
  DIRECT_DATA8    ,
  DIRECT_DATA16   ,

// odd-ball ones
  NO_OPERANDS,  // for NOP
  C_BIT,
  NOTC_BIT,
  REG_DATA4,
  IREG_DATA4,
  IREGINC_DATA4,
  IREGOFF8_DATA4,
  IREGOFF16_DATA4,
  DIRECT_DATA4
};

// table of dissassembled instructions
struct xa_dis_entry
{
  uint  code, mask;
  char  branch;
  uchar length;
//  enum op_mnemonic mnemonic;
//  enum op_operands operands;
  int mnemonic;
  int operands;
};

extern struct dis_entry glob_disass_xa[];

extern struct xa_dis_entry disass_xa[];

#endif

/* End of xa.src/glob.h */
