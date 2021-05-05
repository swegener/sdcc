/*
 * Simulator of microcontrollers (mcs6502cl.h)
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

#ifndef MCS6502CL_HEADER
#define MCS6502CL_HEADER

#include "uccl.h"
#include "memcl.h"
#include "decode.h"
#include "itsrccl.h"


#define rA  (A)
#define rX  (X)
#define rY  (Y)
#define rSP (SP)
#define rS  (SP)
#define rCC (CC)
#define rP  (CC)
#define rF  (CC)
#define cP  (cCC)
#define cF  (cCC)
#define cS  (cSP)

enum {
  mN	= 0x80,
  flagN	= 0x80,
  mS	= 0x80,
  flagS	= 0x80,
  mV	= 0x40,
  flagV	= 0x40,
  mO	= 0x40,
  flagO	= 0x40,
  mB	= 0x10,
  flagB	= 0x10,
  mD	= 0x08,
  flagD	= 0x08,
  mI	= 0x04,
  flagI	= 0x04,
  mZ	= 0x02,
  flagZ	= 0x02,
  mC	= 0x01,
  flagC	= 0x01
};

// Vectors
enum {
  RESET_AT	= 0xfffc,
  IRQ_AT	= 0xfffe,
  NMI_AT	= 0xfffa
};
  
/*
 * Base of MCS6502 processor
 */

class cl_mcs6502: public cl_uc  
{
public:
  u8_t A, X, Y, SP, CC;
  class cl_cell8 cA, cX, cY, cSP, cCC;
  class cl_m6xxx_src *src_irq, *src_nmi, *src_brk;
public:
  cl_mcs6502(class cl_sim *asim);
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
  virtual int priority_of(uchar nuof_it) { return nuof_it; }
  virtual int accept_it(class it_level *il);
  virtual bool it_enabled(void);
  virtual int NOP(t_mem code);
  virtual int BRK(t_mem code);
  virtual int RTI(t_mem code);
  virtual int CLI(t_mem code);
  virtual int SEI(t_mem code);
  virtual int PHP(t_mem code);
  virtual int CLC(t_mem code);
  virtual int PLP(t_mem code);
  virtual int SEC(t_mem code);
  virtual int PHA(t_mem code);
  virtual int PLA(t_mem code);
  virtual int DEY(t_mem code);
  virtual int TYA(t_mem code);
  virtual int TAY(t_mem code);
  virtual int CLV(t_mem code);
  virtual int INY(t_mem code);
  virtual int CLD(t_mem code);
  virtual int INX(t_mem code);
  virtual int SED(t_mem code);
  virtual int TXA(t_mem code);
  virtual int TXS(t_mem code);
  virtual int TAX(t_mem code);
  virtual int TSX(t_mem code);
  virtual int DEX(t_mem code);
};


/* Unused bits of CC forced to be 1 */

class cl_cc_operator: public cl_memory_operator
{
public:
  cl_cc_operator(class cl_memory_cell *acell): cl_memory_operator(acell) {}
  virtual t_mem write(t_mem val) { return val|= 0x20; }
};


#endif

/* End of mcs6502.src/mcs6502.cc */
