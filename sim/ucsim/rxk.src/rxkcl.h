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
#define RP(N,N16,NH,NL) union			\
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

#define raA (aAF.r.A)
#define raF (aAF.r.F)
#define raB (aBC.r.B)
#define raC (aBC.r.C)
#define raD (aDE.r.D)
#define raE (aDE.r.E)
#define raH (aHL.r.H)
#define raL (aHL.r.L)
#define raAF (aAF.AF)
#define raBC (aBC.BC)
#define raDE (aDE.DE)
#define raHL (aHL.HL)

  
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
  u8_t rIP, rIIR, rEIR;
  u16_t rIX, rIY, rSP;
  class cl_cell8 cIP, cIIR, cEIR;
  class cl_cell8 cA, cF, cB, cC, cD, cE, cH, cL;
  class cl_cell16 cAF, cBC, cDE, cHL, cIX, cIY, cSP;
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
  rxk_cpu_xpc		= 0,

  rxk_cpu_nuof	= 1
};

class cl_rxk_cpu: public cl_hw
{
protected:
  class cl_rxk *ruc;
  class cl_cell8 *xpc, *segsize, *dataseg, *stackseg;
public:
  cl_rxk_cpu(class cl_uc *auc);
  virtual int init(void);
  //virtual int cfg_size() { return rxk_cpu_nuof; }
  virtual const char *cfg_help(t_addr addr);
  
  //virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);

  virtual void print_info(class cl_console_base *con);
};


#endif

/* End of rxk.src/rxk.cc */
