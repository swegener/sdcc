/*
 * Simulator of microcontrollers (f8cl.h)
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

#ifndef F8CL_HEADER
#define F8CL_HEADER

#include "uccl.h"

#include "glob.h"
#include "decode.h"

#ifdef WORDS_BIGENDIAN
#define REGPAIR(N,N16,NH,NL) union			\
		      {				\
			u16_t N16;		\
			struct {		\
			  u8_t NH;		\
			  u8_t NL;		\
			} r;			\
  } N
#else
#define REGPAIR(N,N16,NH,NL) union			\
		      {				\
			u16_t N16;		\
			struct {		\
			  u8_t NL;		\
			  u8_t NH;		\
			} r;			\
  } N
#endif

#define rX (rpX.X)
#define rXL (rpX.r.XL)
#define rXH (rpX.r.XH)
#define rY (rpY.Y)
#define rYL (rpY.r.YL)
#define rYH (rpY.r.YH)
#define rZ (rpZ.Z)
#define rZL (rpZ.r.ZL)
#define rZH (rpZ.r.ZH)


enum {
  flagO	= 0x01,
  flagZ	= 0x02,
  flagN	= 0x04,
  flagC	= 0x08,
  flagH	= 0x10,

  flagS = flagN,
  flagV = flagO,
  
  fAll  = flagO|flagZ|flagN|flagC|flagH,
  fAll_H= flagO|flagZ|flagN|flagC
};


enum {
  P_NONE	= 0,
  P_SWAP	= 0x01, // (0) swapop
  P_ALT0	= 0x02, // (1) altacc    XH
  P_ALT1	= 0x04, // (2) altacc'   YL  X
  P_ALT2	= 0x08  // (2) altacc''  ZL  Z
};

#define IFSWAP if (prefixes&P_SWAP)

/*
 * Base of f8 processor
 */
class cl_f8: public cl_uc
{
public:
  REGPAIR(rpX, X, XH, XL); class cl_cell8 cXH, cXL; class cl_cell16 cX;
  REGPAIR(rpY, Y, YH, YL); class cl_cell8 cYH, cYL; class cl_cell16 cY;
  REGPAIR(rpZ, Z, ZH, ZL); class cl_cell8 cZH, cZL; class cl_cell16 cZ;
  u16_t rSP; class cl_cell16 cSP;
  u8_t rF; class cl_cell8 cF;
  t_addr sp_limit;
  class cl_cell8 *acc8;
  class cl_cell16 *acc16;
  int prefixes;
public:
  cl_f8(class cl_sim *asim);
  virtual int init(void);
  virtual const char *id_string(void);
  virtual void reset(void);
  virtual void set_PC(t_addr addr);

  virtual void mk_hw_elements(void);
  virtual void make_cpu_hw(void);
  virtual void make_memories(void);
  
  virtual int clock_per_cycle(void) { return 1; }
  virtual struct dis_entry *dis_tbl(void);
  virtual struct dis_entry *get_dis_entry(t_addr addr);
  virtual int inst_length(t_addr addr);
  virtual u8_t a8(u8_t prefs);
  virtual u16_t a16(u8_t prefs);
  virtual const char *a8_name(u8_t prefs);
  virtual const char *a16_name(u8_t prefs);
  virtual const char *a16h_name(u8_t prefs);
  virtual const char *a16l_name(u8_t prefs);
  virtual char *disassc(t_addr addr, chars *comment=NULL);
  virtual int longest_inst(void) { return 5; }

  virtual void print_regs(class cl_console_base *con);

  virtual u16_t fetch16(void) { u16_t v= fetch(); v+= fetch()*256; return v; }
  virtual i8_t d(void) { return fetch(); }
  virtual i16_t sexd(void);
  virtual void push1(u8_t v);
  virtual void push2(u16_t v);
  virtual void push2(u8_t h, u8_t l);
  virtual u8_t pop1(void);
  virtual u16_t pop2(void);
  virtual void stack_check_overflow(t_addr sp_before);
  
  // memory cells addressed by 8 bit addressing modes
  // call necessary fetches
  virtual class cl_cell8 &m_mm(void);
  virtual class cl_cell8 &m_n_sp(void);
  virtual class cl_cell8 &m_nn_z(void);
  virtual class cl_cell8 &m_y(void);
  virtual class cl_cell8 &m_n_y(void);

  // memory addresses by addressing modes
  // call necessary fetches
  virtual u16_t a_mm(void);
  virtual u16_t a_n_sp(void);
  virtual u16_t a_nn_z(void);
  virtual u16_t a_y(void);
  virtual u16_t a_n_y();
  virtual u16_t a_acc16(void);
  virtual u16_t a_n_acc16(void);
  virtual u16_t a_nn_acc16(void);
  
  virtual void clear_prefixes();
  virtual int exec_inst(void);

  // data moves: imove.cc
  // 8 bit moves
  int ld8_a_i(u8_t op2);
  int ld8_a_m(class cl_cell8 &m);
  int ld8_m_a(class cl_cell8 &m);
  int ld8_a_r(class cl_cell8 &r);
  int LD8_A_I(t_mem code)   { return ld8_a_i(fetch()); }
  int LD8_A_M(t_mem code)   { return ld8_a_m(m_mm()); }
  int LD8_A_NSP(t_mem code) { return ld8_a_m(m_n_sp()); }
  int LD8_A_NNZ(t_mem code) { return ld8_a_m(m_nn_z()); }
  int LD8_A_Y(t_mem code)   { return ld8_a_m(m_y()); }
  int LD8_A_NY(t_mem code)  { return ld8_a_m(m_n_y()); }
  int LD8_A_XH(t_mem code)  { return ld8_a_r(cXH); }
  int LD8_A_YL(t_mem code)  { return ld8_a_r(cYL); }
  int LD8_A_YH(t_mem code)  { return ld8_a_r(cYH); }
  int LD8_A_ZL(t_mem code)  { return ld8_a_r(cZL); }
  int LD8_A_ZH(t_mem code)  { return ld8_a_r(cZH); }
  int LD8_M_A(t_mem code)   { return ld8_m_a(m_mm()); }
  int LD8_NSP_A(t_mem code) { return ld8_m_a(m_n_sp()); }
  int LD8_NNZ_A(t_mem code) { return ld8_m_a(m_nn_z()); }
  int LD8_Y_A(t_mem code)   { return ld8_m_a(m_y()); }
  int LD8_NY_A(t_mem code)  { return ld8_m_a(m_n_y()); }
  int LD8_YL_I(t_mem code);
  // 16 bit moves
  int ldw_a_i(u16_t op2);
  int ldw_a_m(u16_t addr);
  int ldw_m_a(u16_t addr);
  int ldw_m_r(u16_t addr, u16_t r);
  int LDW_A_I(t_mem code)    { return ldw_a_i(fetch16()); }
  int LDW_A_M(t_mem code)    { return ldw_a_m(a_mm()); }
  int LDW_A_NSP(t_mem code)  { return ldw_a_m(a_n_sp()); }
  int LDW_A_NNZ(t_mem code)  { return ldw_a_m(a_nn_z()); }
  int LDW_A_NY(t_mem code)   { return ldw_a_m(a_n_y()); }
  int LDW_A_Y(t_mem code)    { return ldw_a_m(rY); }
  int LDW_A_X(t_mem code)    { return ldw_a_m(a_n_y()); }
  int LDW_A_D(t_mem code)    { return ldw_a_i(sexd()); }
  int LDW_M_A(t_mem code)    { return ldw_m_a(a_mm()); }
  int LDW_NSP_A(t_mem code)  { return ldw_m_a(a_n_sp()); }
  int LDW_NNZ_A(t_mem code)  { return ldw_m_a(a_nn_z()); }
  int LDW_X_A(t_mem code)    { cX.W(acc16->get()); return resGO; }
  int LDW_Z_A(t_mem code)    { cZ.W(acc16->get()); return resGO; }
  int LDW_AM_X(t_mem code)   { return ldw_m_r(a_acc16(), rX); }
  int LDW_NAM_X(t_mem code)  { return ldw_m_r(a_n_acc16(), rX); }
  int LDW_NNAM_X(t_mem code) { return ldw_m_r(a_nn_acc16(), rX); }
  int LDW_SP_A(t_mem code);
  int LDW_DSP_A(t_mem code);
  // other moves
  int PUSH_M(t_mem code);
  int PUSH_NSP(t_mem code);
  int PUSH_A(t_mem code);
  int PUSH_ZH(t_mem code);
  int PUSH_I(t_mem code);
  int PUSHW_M(t_mem code);
  int PUSHW_NSP(t_mem code);
  int PUSHW_NNZ(t_mem code);
  int PUSHW_A(t_mem code);
  int PUSHW_I(t_mem code);
  int POP_A(t_mem code);
  int POPW_A(t_mem code);
  int XCH_A_NSP(t_mem code);
  int XCH_A_Y(t_mem code);
  int XCH_A_A(t_mem code);
  int XCHW_Y_Z(t_mem code);
  int XCHW_Z_NSP(t_mem code);
  int CAX(t_mem code);
  int CAXW(t_mem code);
  int CLR_M(t_mem code);
  int CLR_NSP(t_mem code);
  int CLR_A(t_mem code);
  int CLR_ZH(t_mem code);
  int CLRW_M(t_mem code);
  int CLRW_NSP(t_mem code);
  int CLRW_NNZ(t_mem code);
  int CLRW_A(t_mem code);
  
  // aritmetic (ALU) instuctions: ialu.cc

  // branches: ibranch.cc
  
  // other instructions: inst.cc
  virtual int NOP(t_mem code);
  virtual int TRAP(t_mem code);
};


enum f8cpu_confs
  {
   f8cpu_sp_limit	= 0,
   f8cpu_nuof		= 1
  };

class cl_f8_cpu: public cl_hw
{
public:
  cl_f8_cpu(class cl_uc *auc);
  virtual int init(void);
  virtual unsigned int cfg_size(void) { return f8cpu_nuof; }
  virtual const char *cfg_help(t_addr addr);

  virtual t_mem conf_op(cl_memory_cell *cell, t_addr addr, t_mem *val);
};


#endif

/* End of f8.src/f8cl.h */
