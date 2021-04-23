/*
 * Simulator of microcontrollers (rxkcl.h)
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

#ifndef RXKCL_HEADER
#define RXKCL_HEADER

#include "uccl.h"
#include "memcl.h"


/*
 * Base of RXK processor
 */

#ifdef WORDS_BIGENDIAN
#define RP(N.N16,NH,NL) union			\
		      {				\
			u16_t N16;		\
			struct {		\
			  u8_t NH;		\
			  u8_t NL;		\
			} r;			\
  } N
#else
#define RP(N,N16,NH,NL) union			\
		      {				\
			u16_t N16;		\
			struct {		\
			  u8_t NL;		\
			  u8_t NH;		\
			} r;			\
  } N
#endif

#define rA (AF.r.A)
#define rF (AF.r.F)
#define rB (BC.r.B)
#define rC (BC.r.C)
#define rD (DE.r.D)
#define rE (DE.r.E)
#define rH (HL.r.H)
#define rL (HL.r.L)
#define rAF (AF.AF)
#define rBC (BC.BC)
#define rDE (DE.DE)
#define rHL (HL.HL)
#define rIX (IX)
#define rIY (IY)
#define rSP (SP)

  
class cl_rxk: public cl_uc  
{
public:
  RP(AF,AF,A,F);
  RP(BC,BC,B,C);
  RP(DE,DE,D,E);
  RP(HL,HL,H,L);
  RP(aAF,AF,A,F);
  RP(aBC,BC,B,C);
  RP(aDE,DE,D,E);
  RP(aHL,HL,H,L);
  u16_t IX, IY, SP;
  class cl_memory_cell cA, cF, cAF;
  class cl_memory_cell cB, cC, cBC;
  class cl_memory_cell cD, cE, cDE;
  class cl_memory_cell cH, cL, cHL;
  class cl_memory_cell cIX, cIY, cSP;
  class cl_ras *mem;
  class cl_address_space *ioi, *ioe;
  bool ioi_prefix, ioe_prefix;
public:
  cl_rxk(class cl_sim *asim);
  virtual int init(void);
  virtual const char *id_string(void);
  virtual void reset(void);
  virtual void set_PC(t_addr addr);

  virtual void mk_hw_elements(void);
  virtual void make_cpu_hw(void);
  virtual void make_memories(void);

  virtual int clock_per_cycle(void) { return 1; }

  virtual void print_regs(class cl_console_base *con);
};


enum rxkcpu_cfg {
  rxk_cpu_segsize	= 0,
  rxk_cpu_dataseg	= 1,
  rxk_cpu_stackseg	= 2,
  rxk_cpu_xpc		= 3,

  rxkcpu_nuof	= 4
};

class cl_rxk_cpu: public cl_hw
{
protected:
  class cl_rxk *ruc;
public:
  cl_rxk_cpu(class cl_uc *auc);
  virtual int cfg_size() { return rxkcpu_nuof; }
};


#endif

/* End of rxk.src/rxk.cc */
