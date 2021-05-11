/*
 * Simulator of microcontrollers (m6800cl.h)
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

#ifndef M6800CL_HEADER
#define M6800CL_HEADER

#include "uccl.h"
#include "memcl.h"
#include "decode.h"


#define rA  (A)
#define rB  (B)
#define rCC (CC)
#define rF  (CC)
#define rIX (IX)
#define rX  (IX)
#define rSP (SP)

#define cF  (cCC)
#define cX  (cIX)


//  Flag bit masks
enum {
  mC	= 0x01,
  flagC	= 0x01,
  mO	= 0x02,
  flagO	= 0x02,
  mV	= 0x02,
  flagV	= 0x02,
  mZ	= 0x04,
  flagZ	= 0x04,
  mN	= 0x08,
  flagN	= 0x08,
  mS	= 0x08,
  flagS	= 0x08,
  mI	= 0x10,
  flagI	= 0x10,
  mH	= 0x20,
  flagH	= 0x20,
  mA	= 0x40,
  flagA	= 0x40
};


/*
 * Base of M6800 processor
 */

class cl_m6800: public cl_uc
{
public:
  u8_t A, B, CC;
  u16_t IX, SP;
  class cl_cell8 cA, cB, cCC;
  class cl_cell16 cIX, cSP;
public:
  cl_m6800(class cl_sim *asim);
  virtual int init(void);
  virtual const char *id_string(void);
  virtual void reset(void);
  virtual void set_PC(t_addr addr);

  virtual void mk_hw_elements(void);
  virtual void make_cpu_hw(void);
  virtual void make_memories(void);

  virtual int clock_per_cycle(void) { return 1; }
  virtual struct dis_entry *dis_tbl(void);
  virtual char *disass(t_addr addr);

  virtual void print_regs(class cl_console_base *con);

  virtual int exec_inst(void);
  virtual class cl_cell8 &idx(void);
  virtual class cl_cell8 &ext(void);
  virtual class cl_cell8 &dir(void);
  u8_t i8(void) { return fetch(); }
  u16_t i16(void) { u8_t h, l; h= fetch(); l= fetch(); return h*256+l; }
  virtual int sub(class cl_cell8 &dest, u8_t op, bool c);
  virtual int cmp(u8_t op1, u8_t op2);
  virtual int add(class cl_cell8 &dest, u8_t op, bool c);
  virtual int neg(class cl_cell8 &dest);
  virtual int com(class cl_cell8 &dest);
  virtual int lsr(class cl_cell8 &dest);
  virtual int ror(class cl_cell8 &dest);
  virtual int asr(class cl_cell8 &dest);
  virtual int asl(class cl_cell8 &dest);
  virtual int rol(class cl_cell8 &dest);
  virtual int dec(class cl_cell8 &dest);
  virtual int inc(class cl_cell8 &dest);
  virtual int tst(u8_t op);
  virtual int clr(class cl_cell8 &dest);

  virtual int NOP(t_mem code);
  virtual int TAP(t_mem code);
  virtual int TPA(t_mem code);
  virtual int INX(t_mem code);
  virtual int DEX(t_mem code);
  virtual int CLV(t_mem code);
  virtual int SEV(t_mem code);
  virtual int CLC(t_mem code);
  virtual int SEc(t_mem code);
  virtual int CLI(t_mem code);
  virtual int SEI(t_mem code);

  virtual int SBA(t_mem code) { return sub(cA, rB, false); }
  virtual int CBA(t_mem code) { return cmp(rA, rB); }
  virtual int TAB(t_mem code);
  virtual int TBA(t_mem code);
  virtual int DAA(t_mem code);
  virtual int ABA(t_mem code) { return add(cA, rB, false); }

  virtual int TSX(t_mem code);
  virtual int INS(t_mem code);
  virtual int PULA(t_mem code);
  virtual int PULB(t_mem code);
  virtual int DES(t_mem code);
  virtual int TXS(t_mem code);
  virtual int PSHA(t_mem code);
  virtual int PSHB(t_mem code);
  virtual int RTS(t_mem code);

  virtual int NEGA(t_mem code) { return neg(cA); }
  virtual int COMA(t_mem code) { return com(cA); }
  virtual int LSRA(t_mem code) { return lsr(cA); }
  virtual int RORA(t_mem code) { return ror(cA); }
  virtual int ASRA(t_mem code) { return asr(cA); }
  virtual int ASLA(t_mem code) { return asl(cA); }
  virtual int ROLA(t_mem code) { return rol(cA); }
  virtual int DECA(t_mem code) { return dec(cA); }
  virtual int INCA(t_mem code) { return inc(cA); }
  virtual int TSTA(t_mem code) { return tst(rA); }
  virtual int CLRA(t_mem code) { return clr(cA); }

  virtual int NEGB(t_mem code) { return neg(cB); }
  virtual int COMB(t_mem code) { return com(cB); }
  virtual int LSRB(t_mem code) { return lsr(cB); }
  virtual int RORB(t_mem code) { return ror(cB); }
  virtual int ASRB(t_mem code) { return asr(cB); }
  virtual int ASLB(t_mem code) { return asl(cB); }
  virtual int ROLB(t_mem code) { return rol(cB); }
  virtual int DECB(t_mem code) { return dec(cB); }
  virtual int INCB(t_mem code) { return inc(cB); }
  virtual int TSTB(t_mem code) { return tst(rB); }
  virtual int CLRB(t_mem code) { return clr(cB); }
};


/* Unused bits of CC forced to be 1 */

class cl_cc_operator: public cl_memory_operator
{
public:
  cl_cc_operator(class cl_memory_cell *acell): cl_memory_operator(acell) {}
  virtual t_mem write(t_mem val) { return val|= 0xc0; }
};


#endif

/* End of m6800.src/m6800cl.h */
