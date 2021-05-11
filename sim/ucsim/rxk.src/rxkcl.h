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
#include "decode.h"


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

enum {
  flagS = 0x80,
  flagZ = 0x40,
  flagL = 0x04,
  flagV = 0x04,
  flagC = 0x01
};

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
  class cl_cell8 caA, caF, caB, caC, caD, caE, caH, caL;
  class cl_cell16 cAF, cBC, cDE, cHL, cIX, cIY, cSP;
  class cl_cell16 caAF, caBC, caDE, caHL;
  class cl_ras *mem;
  class cl_address_space *ioi, *ioe;
  class cl_address_space *rwas;
  bool io_prefix;
  int altd;
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
  virtual struct dis_entry *dis_tbl(void);
  virtual char *disass(t_addr addr);

  virtual void print_regs(class cl_console_base *con);

  virtual int exec_inst(void);
  
  class cl_cell16 &destAF(void) { return altd?caAF:cAF; }
  class cl_cell16 &destBC(void) { return altd?caBC:cBC; }
  class cl_cell16 &destDE(void) { return altd?caDE:cDE; }
  class cl_cell16 &destHL(void) { return altd?caHL:cHL; }
  class cl_cell8  &destA(void)  { return altd?caA:cA; }
  class cl_cell8  &destF(void)  { return altd?caF:cF; }
  class cl_cell8  &destB(void)  { return altd?caB:cB; }
  class cl_cell8  &destC(void)  { return altd?caC:cC; }
  class cl_cell8  &destD(void)  { return altd?caD:cD; }
  class cl_cell8  &destE(void)  { return altd?caE:cE; }
  class cl_cell8  &destH(void)  { return altd?caH:cH; }
  class cl_cell8  &destL(void)  { return altd?caL:cL; }
  
  virtual int ALTD(t_mem code);
  virtual int IOI(t_mem code);
  virtual int IOE(t_mem code);
  
  virtual int NOP(t_mem code);
  virtual int ld_dd_mn(class cl_cell16 &dd);
  virtual int LD_BC_mn(t_mem code) { return ld_dd_mn(destBC()); }
  virtual int LD_DE_mn(t_mem code) { return ld_dd_mn(destDE()); }
  virtual int LD_HL_mn(t_mem code) { return ld_dd_mn(destHL()); }
  virtual int LD_SP_mn(t_mem code) { return ld_dd_mn(cSP     ); }
  virtual int inc_ss(class cl_cell16 &rp, u16_t op);
  virtual int INC_BC(t_mem code) { return inc_ss(destBC(), rBC); }
  virtual int INC_DE(t_mem code) { return inc_ss(destDE(), rDE); }
  virtual int INC_HL(t_mem code) { return inc_ss(destHL(), rHL); }
  virtual int INC_SP(t_mem code) { return inc_ss(cSP     , rSP); }
  virtual int inc_r(class cl_cell8 &cr, u8_t op);
  virtual int INC_A(t_mem code) { return inc_r(destA(), rA); }
  virtual int INC_B(t_mem code) { return inc_r(destB(), rB); }
  virtual int INC_C(t_mem code) { return inc_r(destC(), rC); }
  virtual int INC_D(t_mem code) { return inc_r(destD(), rD); }
  virtual int INC_E(t_mem code) { return inc_r(destE(), rE); }
  virtual int INC_H(t_mem code) { return inc_r(destH(), rH); }
  virtual int INC_L(t_mem code) { return inc_r(destL(), rL); }
  virtual int dec_ss(class cl_cell16 &rp, u16_t op);
  virtual int DEC_BC(t_mem code) { return dec_ss(destBC(), rBC); }
  virtual int DEC_DE(t_mem code) { return dec_ss(destDE(), rDE); }
  virtual int DEC_HL(t_mem code) { return dec_ss(destHL(), rHL); }
  virtual int DEC_SP(t_mem code) { return dec_ss(cSP     , rSP); }
  virtual int dec_r(class cl_cell8 &cr, u8_t op);
  virtual int DEC_A(t_mem code) { return dec_r(destA(), rA); }
  virtual int DEC_B(t_mem code) { return dec_r(destB(), rB); }
  virtual int DEC_C(t_mem code) { return dec_r(destC(), rC); }
  virtual int DEC_D(t_mem code) { return dec_r(destD(), rD); }
  virtual int DEC_E(t_mem code) { return dec_r(destE(), rE); }
  virtual int DEC_H(t_mem code) { return dec_r(destH(), rH); }
  virtual int DEC_L(t_mem code) { return dec_r(destL(), rL); }
  virtual int ld_r_n(class cl_cell8 &r);
  virtual int LD_A_n() { return ld_r_n(destA()); }
  virtual int LD_B_n() { return ld_r_n(destB()); }
  virtual int LD_C_n() { return ld_r_n(destC()); }
  virtual int LD_D_n() { return ld_r_n(destD()); }
  virtual int LD_E_n() { return ld_r_n(destE()); }
  virtual int LD_H_n() { return ld_r_n(destH()); }
  virtual int LD_L_n() { return ld_r_n(destL()); }
  virtual int rot8left(class cl_cell8 &dest, u8_t op);
  virtual int RLCA(t_mem code) { return rot8left(destA(), rA); }
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
