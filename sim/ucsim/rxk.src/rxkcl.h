/*
 * Simulator of microcontrollers (rxkcl.h)
 *
 * Copyright (C) 2020,2021 Drotos Daniel, Talker Bt.
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
#include "rmemcl.h"
#include "decode.h"
#include "dp0m3.h"
#include "dpedm3.h"
#include "dpddm3.h"


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
#define rXPC (mem->xpc)
#define cXPC (*XPC)

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
  flagC = 0x01,
  flagAll= flagS|flagZ|flagL|flagC
};

#define CPU ((class cl_rxk_cpu *)cpu)

class cl_rxk_base: public cl_uc
{
public:
  cl_rxk_base(class cl_sim *asim);
#include "r4kcl_instructions.h"
#include "dd_instructions.h"
#include "ed_instructions.h"
};
  
class cl_rxk: public cl_rxk_base
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
  class cl_cell8 *cRtab[8], *caRtab[8];
  class cl_cell16 cAF, cBC, cDE, cHL, cIX, cIY, cSP;
  class cl_cell16 caAF, caBC, caDE, caHL;
  class cl_cell16 *cIR;
  class cl_memory_cell *XPC;
  class cl_ras *mem;
  class cl_address_space *ioi, *ioe;
  class cl_address_space *rwas;
  bool prefix, altd;
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
  //virtual struct dis_entry *dis_tbl(void);
  virtual struct dis_entry *dis_entry(t_addr addr);
  virtual char *disassc(t_addr addr, chars *comment= NULL);
  virtual char *disassc_cb(t_addr addr, chars *comment= NULL);
  virtual char *disassc_dd_cb(t_addr addr, chars *comment= NULL);
  virtual int inst_length(t_addr addr);
  virtual int longest_inst(void) { return 4; }
  
  virtual void print_regs(class cl_console_base *con);

  virtual class cl_cell8 *cR(u8_t z);
  virtual class cl_cell8 *destR(u8_t z);
  virtual u8_t rR(u8_t z);
  virtual int exec_inst(void);
  virtual int inst_unknown(t_mem code);
  virtual void tick5p1(int n) { tick(n); }
  virtual void tick5p2(int n) { tick(n); }
  virtual void tick5p3(int n) { tick(n); }
  virtual void tick5m1(int n) { tick(n+2); }
  virtual void tick5m2(int n) { tick(n+2); }

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
  class cl_cell8  &dest8iBC(void) { return *((cl_cell8*)rwas->get_cell(rBC)); }
  class cl_cell8  &dest8iDE(void) { return *((cl_cell8*)rwas->get_cell(rDE)); }
  class cl_cell8  &dest8iHL(void) { return *((cl_cell8*)rwas->get_cell(rHL)); }
  class cl_cell8  &dest8imn(void) { u8_t l, h;
    l= fetch(); h= fetch();
    return *((cl_cell8*)rwas->get_cell(h*256+l));
  }
  class cl_cell8 &dest8iIRd(i8_t d) { return *((cl_cell8*)rwas->get_cell(cIR->get()+d)); }
  
  u8_t op8_BC(void);
  u8_t op8_DE(void);
  u8_t op8_HL(void);
  u16_t op16_BC(void);
  u16_t op16_DE(void);
  u16_t op16_HL(void);
  void write8(t_addr a, u8_t v) { vc.wr++; rom->write(a, v); }
  void write8io(t_addr a, u8_t v) { vc.wr++; rwas->write(a, v); }
  void write16(t_addr a, u16_t v) { vc.wr+=2;
    rom->write(a, v); rom->write(a+1, v>>8);
  }
  void write16io(t_addr a, u16_t v) { vc.wr+=2;
    rwas->write(a, v); rwas->write(a+1, v>>8);
  }
  u8_t read8(t_addr a) { vc.rd++; return rom->read(a); }
  u8_t read8io(t_addr a) { vc.rd++; return rwas->read(a); }
  u16_t read16(t_addr a) { u8_t l, h; vc.rd+=2;
    l= rom->read(a); h= rom->read(a+1);
    return h*256+l;
  }
  u16_t read16io(t_addr a) { u8_t l, h; vc.rd+=2;
    l= rwas->read(a); h= rwas->read(a+1);
    return h*256+l;
  }
  u16_t fetch16(void) { u8_t l, h;
    l= fetch(); h= fetch();
    return h*256 + l;
  }
  
  virtual int ld_dd_mn(class cl_cell16 &dd);			// 2f,6t,0r,0w
  virtual int ld_r_n(class cl_cell8 &r);			// 1f,4t,0r,0w
  virtual int ld_ihl_r(u8_t op);				// 0f,6t,0r,1w
  virtual int ld_r_ihl(class cl_cell8 &destr);			// 0f,5t,1r,0w
  virtual int ld_r_g(class cl_cell8 &dest, u8_t op);		// 0f,2t,0r,0w
  virtual int pop_zz(class cl_cell16 &dest);			// 0f,6t,2r,0w
  virtual int push_zz(u16_t op);				// 0f,9t,0r,2w
  virtual int ld_d_i(int dif);					// 0f,10t,1r,1w
  virtual int ld_iIRd_r(u8_t op);				// 1f,10t,0r,1w
  virtual int ld_r_iIRd(class cl_cell8 &op);			// 1f,9t,1r,0w
  virtual int ld_dd_imn(class cl_cell16 &dest);			// 2f,13t,2r,0w
  
  virtual int inc_ss(class cl_cell16 &rp, u16_t op);
  virtual int inc_r(class cl_cell8 &cr, u8_t op);
  virtual int dec_ss(class cl_cell16 &rp, u16_t op);
  virtual int dec_r(class cl_cell8 &cr, u8_t op);
  virtual int rot8left(class cl_cell8 &dest, u8_t op);		// 0f,1t,0r,0w
  virtual int rlc(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int rot9left(class cl_cell8 &dest, u8_t op);		// 0f,1t,0r,0w
  virtual int rl(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int rot17left(class cl_cell16 &dest, u16_t op);	// 0f,1t,0r,0w
  virtual int rot8right(class cl_cell8 &dest, u8_t op);		// 0f,1t,0r,0w
  virtual int rrc(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int rot9right(class cl_cell8 &dest, u8_t op);		// 0f,1t,0r,0w
  virtual int rr(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int rot17right(class cl_cell16 &dest, u16_t op);	// 0f,1t,0r,0w
  virtual int sla(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int sra(class cl_cell8 &dest, i8_t op);		// 0f,4t,0r,0w
  virtual int srl(class cl_cell8 &dest, u8_t op);		// 0f,4t,0r,0w
  virtual int bit_r(u8_t b, u8_t op);				// 0f,4t,0r,0w
  virtual int bit_iHL(u8_t b);					// 0f,7t,1r,0w
  virtual int bit_iIRd(u8_t b, i8_t d);				// 0f,10t,1r,0w
  virtual int res_r(u8_t b, class cl_cell8 &dest, u8_t op);	// 0f,4t,0r,0w
  virtual int res_iHL(u8_t b);					// 0f,10t,1r,1w
  virtual int res_iIRd(u8_t b, i8_t d);				// 0f,13t,1r,1w
  virtual int set_r(u8_t b, class cl_cell8 &dest, u8_t op);	// 0f,4t,0r,0w
  virtual int set_iHL(u8_t b);					// 0f,10t,1r,1w
  virtual int set_iIRd(u8_t b, i8_t d);				// 0f,12t,1r,1w
  
  virtual int add_hl_ss(u16_t op);
  virtual int adc_hl_ss(u16_t op);
  virtual int add8(u8_t op2, bool cy);				// 0f,4t,0r,0w
  virtual int sub8(u8_t op2, bool cy);				// 0f,4t,0r,0w
  virtual int sub16(u16_t op2, bool cy);			// 0f,4t,0r,0w
  
  virtual int inc_i8(t_addr addr);
  virtual int dec_i8(t_addr addr);
  virtual int add_ir_xy(u16_t op);				// 0f,4t,0r,0r
  virtual int xor8(class cl_cell8 &dest, u8_t op1, u8_t op2);	// 0f,1t,0r,0w
  virtual int or8(class cl_cell8 &dest, u8_t op1, u8_t op2);	// 0f,1t,0r,0w
  virtual int or16(class cl_cell16 &dest,
		    u16_t op1, u16_t op2);			// 0f,1t,0r,0w
  virtual int and8(class cl_cell8 &dest, u8_t op1, u8_t op2);	// 0f,1t,0r,0w
  virtual int and16(class cl_cell16 &dest,
		    u16_t op1, u16_t op2);			// 0f,1t,0r,0w
  virtual int cp8(u8_t op1, u8_t op2);				// 0f,3t,0r,0w
  
  virtual int jr_cc(bool cond);
  virtual int ret_f(bool f);					// 0f,7t,2r,0w
  virtual int jp_f_mn(bool f);					// 2f,6t,2r,0w
  virtual int rst_v(t_mem code);				// 0f,7t,0r,2w
  
  virtual int ALTD(t_mem code);
  virtual int IOI(t_mem code);
  virtual int IOE(t_mem code);

  virtual int NOP(t_mem code);
  virtual int LD_BC_mn(t_mem code) { return ld_dd_mn(destBC()); }
  virtual int LD_DE_mn(t_mem code) { return ld_dd_mn(destDE()); }
  virtual int LD_HL_mn(t_mem code) { return ld_dd_mn(destHL()); }
  virtual int LD_SP_mn(t_mem code) { return ld_dd_mn(cSP     ); }
  virtual int LD_imn_HL(t_mem code);
  virtual int LD_HL_imn(t_mem code);
  virtual int INC_BC(t_mem code) { return inc_ss(destBC(), rBC); }
  virtual int INC_DE(t_mem code) { return inc_ss(destDE(), rDE); }
  virtual int INC_HL(t_mem code) { return inc_ss(destHL(), rHL); }
  virtual int INC_SP(t_mem code) { return inc_ss(cSP     , rSP); }
  virtual int INC_A(t_mem code) { return inc_r(destA(), rA); }
  virtual int INC_B(t_mem code) { return inc_r(destB(), rB); }
  virtual int INC_C(t_mem code) { return inc_r(destC(), rC); }
  virtual int INC_D(t_mem code) { return inc_r(destD(), rD); }
  virtual int INC_E(t_mem code) { return inc_r(destE(), rE); }
  virtual int INC_H(t_mem code) { return inc_r(destH(), rH); }
  virtual int INC_L(t_mem code) { return inc_r(destL(), rL); }
  virtual int DEC_BC(t_mem code) { return dec_ss(destBC(), rBC); }
  virtual int DEC_DE(t_mem code) { return dec_ss(destDE(), rDE); }
  virtual int DEC_HL(t_mem code) { return dec_ss(destHL(), rHL); }
  virtual int DEC_SP(t_mem code) { return dec_ss(cSP     , rSP); }
  virtual int DEC_A(t_mem code) { return dec_r(destA(), rA); }
  virtual int DEC_B(t_mem code) { return dec_r(destB(), rB); }
  virtual int DEC_C(t_mem code) { return dec_r(destC(), rC); }
  virtual int DEC_D(t_mem code) { return dec_r(destD(), rD); }
  virtual int DEC_E(t_mem code) { return dec_r(destE(), rE); }
  virtual int DEC_H(t_mem code) { return dec_r(destH(), rH); }
  virtual int DEC_L(t_mem code) { return dec_r(destL(), rL); }
  virtual int LD_A_n(t_mem code) { return ld_r_n(destA()); }
  virtual int LD_B_n(t_mem code) { return ld_r_n(destB()); }
  virtual int LD_C_n(t_mem code) { return ld_r_n(destC()); }
  virtual int LD_D_n(t_mem code) { return ld_r_n(destD()); }
  virtual int LD_E_n(t_mem code) { return ld_r_n(destE()); }
  virtual int LD_H_n(t_mem code) { return ld_r_n(destH()); }
  virtual int LD_L_n(t_mem code) { return ld_r_n(destL()); }
  virtual int RLCA(t_mem code) { return rot8left(destA(), rA); }
  virtual int RLA(t_mem code) { return rot9left(destA(), rA); }
  virtual int RRCA(t_mem code) { return rot8right(destA(), rA); }
  virtual int RRA(t_mem code) { return rot9right(destA(), rA); }
  virtual int LD_iBC_A(t_mem code);
  virtual int LD_iDE_A(t_mem code);
  virtual int LD_iHL_A(t_mem code) { return ld_ihl_r(rA); }
  virtual int LD_iHL_B(t_mem code) { return ld_ihl_r(rB); }
  virtual int LD_iHL_C(t_mem code) { return ld_ihl_r(rC); }
  virtual int LD_iHL_D(t_mem code) { return ld_ihl_r(rD); }
  virtual int LD_iHL_E(t_mem code) { return ld_ihl_r(rE); }
  virtual int LD_iHL_H(t_mem code) { return ld_ihl_r(rH); }
  virtual int LD_iHL_L(t_mem code) { return ld_ihl_r(rL); }
  virtual int LD_iMN_A(t_mem code);
  virtual int LD_A_iBC(t_mem code);
  virtual int LD_A_iDE(t_mem code);
  virtual int LD_A_iMN(t_mem code);
  virtual int LD_A_iHL(t_mem code) { return ld_r_ihl(destA()); }
  virtual int LD_B_iHL(t_mem code) { return ld_r_ihl(destB()); }
  virtual int LD_C_iHL(t_mem code) { return ld_r_ihl(destC()); }
  virtual int LD_D_iHL(t_mem code) { return ld_r_ihl(destD()); }
  virtual int LD_E_iHL(t_mem code) { return ld_r_ihl(destE()); }
  virtual int LD_H_iHL(t_mem code) { return ld_r_ihl(destH()); }
  virtual int LD_L_iHL(t_mem code) { return ld_r_ihl(destL()); }
  virtual int SCF(t_mem code);
  virtual int CPL(t_mem code);
  virtual int CCF(t_mem code);
  virtual int EX_AF_aAF(t_mem code);
  virtual int ADD_HL_BC(t_mem code) { return add_hl_ss(rBC); }
  virtual int ADD_HL_DE(t_mem code) { return add_hl_ss(rDE); }
  virtual int ADD_HL_HL(t_mem code) { return add_hl_ss(rHL); }
  virtual int ADD_HL_SP(t_mem code) { return add_hl_ss(rSP); }
  virtual int DJNZ(t_mem code);
  virtual int JR(t_mem code);
  virtual int JR_NZ(t_mem code) { return jr_cc(!(rF&flagZ)); }
  virtual int JR_Z (t_mem code) { return jr_cc( (rF&flagZ)); }
  virtual int JR_NC(t_mem code) { return jr_cc(!(rF&flagC)); }
  virtual int JR_C (t_mem code) { return jr_cc( (rF&flagC)); }
  virtual int ADD_SP_d(t_mem code);
  virtual int INC_iHL(t_mem code) { return inc_i8(rHL); }
  virtual int DEC_iHL(t_mem code) { return dec_i8(rHL); }
  virtual int LD_iHL_n(t_mem code) { tick(6);write8io(rHL, fetch());return resGO; }
  virtual int LD_B_A(t_mem code) { return ld_r_g(destB(), rA); }
  virtual int LD_C_A(t_mem code) { return ld_r_g(destC(), rA); }
  virtual int LD_D_A(t_mem code) { return ld_r_g(destD(), rA); }
  virtual int LD_E_E(t_mem code) { return ld_r_g(destE(), rE); }
  virtual int LD_E_A(t_mem code) { return ld_r_g(destE(), rA); }
  virtual int LD_L_A(t_mem code) { return ld_r_g(destL(), rA); }
  virtual int LD_H_A(t_mem code) { return ld_r_g(destH(), rA); }
  virtual int LD_A_B(t_mem code) { return ld_r_g(destA(), rB); }
  virtual int LD_A_C(t_mem code) { return ld_r_g(destA(), rC); }
  virtual int LD_A_D(t_mem code) { return ld_r_g(destA(), rD); }
  virtual int LD_A_E(t_mem code) { return ld_r_g(destA(), rE); }
  virtual int LD_A_H(t_mem code) { return ld_r_g(destA(), rH); }
  virtual int LD_A_L(t_mem code) { return ld_r_g(destA(), rL); }
  virtual int XOR_A(t_mem code) { return xor8(destA(), rA, rA); }
  virtual int OR_A(t_mem code) { return or8(destA(), rA, rA); }
  virtual int RET_NZ(t_mem code) { return ret_f(!(rF&flagZ)); }
  virtual int RET_Z (t_mem code) { return ret_f( (rF&flagZ)); }
  virtual int RET   (t_mem code) { return ret_f( (true    )); }
  virtual int RET_NC(t_mem code) { return ret_f(!(rF&flagC)); }
  virtual int RET_C (t_mem code) { return ret_f( (rF&flagC)); }
  virtual int RET_LZ(t_mem code) { return ret_f(!(rF&flagV)); } // NV
  virtual int RET_LO(t_mem code) { return ret_f( (rF&flagV)); } // V
  virtual int RET_P (t_mem code) { return ret_f(!(rF&flagS)); }
  virtual int RET_M (t_mem code) { return ret_f( (rF&flagS)); }
  virtual int POP_AF(t_mem code) { return pop_zz(destAF()); }
  virtual int POP_BC(t_mem code) { return pop_zz(destBC()); }
  virtual int POP_DE(t_mem code) { return pop_zz(destDE()); }
  virtual int POP_HL(t_mem code) { return pop_zz(destHL()); }
  virtual int JP_NZ_mn(t_mem code) { return jp_f_mn(!(rF&flagZ)); }
  virtual int JP_Z_mn (t_mem code) { return jp_f_mn( (rF&flagZ)); }
  virtual int JP_NC_mn(t_mem code) { return jp_f_mn(!(rF&flagC)); }
  virtual int JP_C_mn (t_mem code) { return jp_f_mn( (rF&flagC)); }
  virtual int JP_LZ_mn(t_mem code) { return jp_f_mn(!(rF&flagV)); }
  virtual int JP_LO_mn(t_mem code) { return jp_f_mn( (rF&flagV)); }
  virtual int JP_P_mn (t_mem code) { return jp_f_mn(!(rF&flagS)); }
  virtual int JP_M_mn (t_mem code) { return jp_f_mn( (rF&flagS)); }
  virtual int JP_mn   (t_mem code) { return jp_f_mn( (true    )); }
  virtual int LD_HL_iSPn(t_mem code);
  virtual int PUSH_AF(t_mem code) { return push_zz(rAF); }
  virtual int PUSH_BC(t_mem code) { return push_zz(rBC); }
  virtual int PUSH_DE(t_mem code) { return push_zz(rDE); }
  virtual int PUSH_HL(t_mem code) { return push_zz(rHL); }
  virtual int ADD_A_n(t_mem code) { return add8(fetch(), false); }
  virtual int LJP(t_mem code);
  virtual int BOOL_HL(t_mem code);
  virtual int CALL_mn(t_mem code);
  virtual int ADC_A_n(t_mem code) { return add8(fetch(), true); }
  virtual int LCALL_lmn(t_mem code);
  virtual int LD_iSPn_HL(t_mem code);
  virtual int SUB_A_n(t_mem code) { return sub8(fetch(), false); }
  virtual int RST_10(t_mem code) { return rst_v(code); }
  virtual int RST_18(t_mem code) { return rst_v(code); }
  virtual int RST_20(t_mem code) { return rst_v(code); }
  virtual int RST_28(t_mem code) { return rst_v(code); }
  virtual int RST_38(t_mem code) { return rst_v(code); }
  virtual int EXX(t_mem code);
  virtual int AND_HL_DE(t_mem code) { return and16(destHL(), rHL, rDE); }
  virtual int OR_HL_DE (t_mem code) { return or16 (destHL(), rHL, rDE); }
  virtual int SBC_A_n(t_mem code) { return sub8(fetch(), true); }
  virtual int EX_aDE_HL(t_mem code);
  virtual int EX_DE_HL(t_mem code);
  virtual int LD_HL_iIXd(t_mem code);
  virtual int LD_iIXd_HL(t_mem code);
  virtual int AND_n(t_mem code) { tick(2); return and8(destA(), rA, fetch()); }
  virtual int JP_HL(t_mem code) { tick(3); PC= rHL; return resGO; }
  virtual int XOR_n(t_mem code) { tick(3); return xor8(destA(), rA, fetch()); }
  virtual int RL_DE(t_mem code) { return rot17left(destDE(), rDE); }
  virtual int OR_n(t_mem code) { tick(3); return or8(destA(), rA, fetch()); }
  virtual int MUL(t_mem code);
  virtual int LD_SP_HL(t_mem code) { tick(1); cSP.W(rHL); return resGO; }
  virtual int RR_DE(t_mem code) { return rot17right(destDE(), rDE); }
  virtual int RR_HL(t_mem code) { return rot17right(destHL(), rHL); }
  virtual int CP_n(t_mem code) { return cp8(rA, fetch()); }

  // Page0, mode 3k only, invalid in 4k mode
  virtual int LD_B_B(t_mem code) { return ld_r_g(destB(), rB); }
  virtual int LD_B_C(t_mem code) { return ld_r_g(destB(), rC); }
  virtual int LD_B_E(t_mem code) { return ld_r_g(destB(), rE); }
  virtual int LD_B_H(t_mem code) { return ld_r_g(destB(), rH); }
  virtual int LD_C_C(t_mem code) { return ld_r_g(destC(), rC); }
  virtual int LD_C_D(t_mem code) { return ld_r_g(destC(), rD); }
  virtual int LD_C_E(t_mem code) { return ld_r_g(destC(), rE); }

  virtual int LD_D_D(t_mem code) { return ld_r_g(destD(), rD); }
  virtual int LD_D_E(t_mem code) { return ld_r_g(destD(), rE); }
  virtual int LD_E_B(t_mem code) { return ld_r_g(destE(), rB); }
  virtual int LD_E_C(t_mem code) { return ld_r_g(destE(), rC); }
  virtual int LD_E_D(t_mem code) { return ld_r_g(destE(), rD); }
  virtual int LD_E_H(t_mem code) { return ld_r_g(destE(), rH); }
  virtual int LD_E_L(t_mem code) { return ld_r_g(destE(), rL); }

  virtual int LD_H_H(t_mem code) { return ld_r_g(destH(), rH); }
  virtual int LD_L_B(t_mem code) { return ld_r_g(destL(), rB); }
  virtual int LD_L_C(t_mem code) { return ld_r_g(destL(), rC); }
  virtual int LD_L_D(t_mem code) { return ld_r_g(destL(), rD); }
  virtual int LD_L_E(t_mem code) { return ld_r_g(destL(), rE); }
  virtual int LD_L_H(t_mem code) { return ld_r_g(destL(), rH); }

  virtual int ADD_A_B(t_mem code) { return add8(rB, false); }
  virtual int ADC_A_B(t_mem code) { return add8(rB, true ); }

  virtual int SUB_A_B(t_mem code) { return sub8(rB, false); }

  virtual int LD_B_D(t_mem code) { return ld_r_g(destB(), rD); }
  virtual int LD_B_L(t_mem code) { return ld_r_g(destB(), rL); }
  virtual int LD_C_B(t_mem code) { return ld_r_g(destC(), rB); }
  virtual int LD_C_H(t_mem code) { return ld_r_g(destC(), rH); }
  virtual int LD_C_L(t_mem code) { return ld_r_g(destC(), rL); }

  virtual int LD_D_B(t_mem code) { return ld_r_g(destD(), rB); }
  virtual int LD_D_C(t_mem code) { return ld_r_g(destD(), rC); }
  virtual int LD_D_H(t_mem code) { return ld_r_g(destD(), rH); }
  virtual int LD_D_L(t_mem code) { return ld_r_g(destD(), rL); }

  virtual int LD_H_B(t_mem code) { return ld_r_g(destH(), rB); }
  virtual int LD_H_C(t_mem code) { return ld_r_g(destH(), rC); }
  virtual int LD_H_D(t_mem code) { return ld_r_g(destH(), rD); }
  virtual int LD_H_E(t_mem code) { return ld_r_g(destH(), rE); }
  virtual int LD_H_L(t_mem code) { return ld_r_g(destH(), rL); }
  virtual int LD_L_L(t_mem code) { return ld_r_g(destL(), rL); }

  virtual int LD_A_A(t_mem code) { return ld_r_g(destA(), rA); }

  virtual int ADD_A_C(t_mem code) { return add8(rC, false); }
  virtual int ADD_A_D(t_mem code) { return add8(rD, false); }
  virtual int ADD_A_E(t_mem code) { return add8(rE, false); }
  virtual int ADD_A_H(t_mem code) { return add8(rH, false); }
  virtual int ADD_A_L(t_mem code) { return add8(rL, false); }
  virtual int ADD_A_iHL(t_mem code) { tick(3); return add8(read8io(rHL), false); }
  virtual int ADD_A_A(t_mem code) { return add8(rA, false); }
  virtual int ADC_A_C(t_mem code) { return add8(rC, true); }
  virtual int ADC_A_D(t_mem code) { return add8(rD, true); }
  virtual int ADC_A_E(t_mem code) { return add8(rE, true); }
  virtual int ADC_A_H(t_mem code) { return add8(rH, true); }
  virtual int ADC_A_L(t_mem code) { return add8(rL, true); }
  virtual int ADC_A_iHL(t_mem code) { tick(3); return add8(read8io(rHL), true); }
  virtual int ADC_A_A(t_mem code) { return add8(rA, true); }

  virtual int SUB_A_C(t_mem code) { return sub8(rC, false); }
  virtual int SUB_A_D(t_mem code) { return sub8(rD, false); }
  virtual int SUB_A_E(t_mem code) { return sub8(rE, false); }
  virtual int SUB_A_H(t_mem code) { return sub8(rH, false); }
  virtual int SUB_A_L(t_mem code) { return sub8(rL, false); }
  virtual int SUB_A_iHL(t_mem code) { tick(3); return sub8(read8io(rHL), false); }
  virtual int SUB_A_A(t_mem code) { return sub8(rA, false); }
  virtual int SBC_A_B(t_mem code) { return sub8(rB, true); }
  virtual int SBC_A_C(t_mem code) { return sub8(rC, true); }
  virtual int SBC_A_D(t_mem code) { return sub8(rD, true); }
  virtual int SBC_A_E(t_mem code) { return sub8(rE, true); }
  virtual int SBC_A_H(t_mem code) { return sub8(rH, true); }
  virtual int SBC_A_L(t_mem code) { return sub8(rL, true); }
  virtual int SBC_A_iHL(t_mem code) { tick(3); return sub8(read8io(rHL), true); }
  virtual int SBC_A_A(t_mem code) { return sub8(rA, true); }

  virtual int AND_A_B(t_mem code) { return and8(destA(), rA, rB); }
  virtual int AND_A_C(t_mem code) { return and8(destA(), rA, rC); }
  virtual int AND_A_D(t_mem code) { return and8(destA(), rA, rD); }
  virtual int AND_A_E(t_mem code) { return and8(destA(), rA, rE); }
  virtual int AND_A_H(t_mem code) { return and8(destA(), rA, rH); }
  virtual int AND_A_L(t_mem code) { return and8(destA(), rA, rL); }
  virtual int AND_A_iHL(t_mem code) { tick(3); return and8(destA(), rA, read8io(rHL)); }
  virtual int AND_A_A(t_mem code) { return and8(destA(), rA, rA); }
  virtual int XOR_A_B(t_mem code) { return xor8(destA(), rA, rB); }
  virtual int XOR_A_C(t_mem code) { return xor8(destA(), rA, rC); }
  virtual int XOR_A_D(t_mem code) { return xor8(destA(), rA, rD); }
  virtual int XOR_A_E(t_mem code) { return xor8(destA(), rA, rE); }
  virtual int XOR_A_H(t_mem code) { return xor8(destA(), rA, rH); }
  virtual int XOR_A_L(t_mem code) { return xor8(destA(), rA, rL); }
  virtual int XOR_A_iHL(t_mem code) { tick(3); return xor8(destA(), rA, read8io(rHL)); }

  virtual int OR_A_B(t_mem code) { return or8(destA(), rA, rB); }
  virtual int OR_A_C(t_mem code) { return or8(destA(), rA, rC); }
  virtual int OR_A_D(t_mem code) { return or8(destA(), rA, rD); }
  virtual int OR_A_E(t_mem code) { return or8(destA(), rA, rE); }
  virtual int OR_A_H(t_mem code) { return or8(destA(), rA, rH); }
  virtual int OR_A_L(t_mem code) { return or8(destA(), rA, rL); }
  virtual int OR_A_iHL(t_mem code) { tick(3); return or8(destA(), rA, read8io(rHL)); }
  virtual int CP_A_B(t_mem code) { return cp8(rA, rB); }
  virtual int CP_A_C(t_mem code) { return cp8(rA, rC); }
  virtual int CP_A_D(t_mem code) { return cp8(rA, rD); }
  virtual int CP_A_E(t_mem code) { return cp8(rA, rE); }
  virtual int CP_A_H(t_mem code) { return cp8(rA, rH); }
  virtual int CP_A_L(t_mem code) { return cp8(rA, rL); }
  virtual int CP_A_iHL(t_mem code) { tick(3); return cp8(rA, read8io(rHL)); }
  virtual int CP_A_A(t_mem code) { return cp8(rA, rA); }

  virtual int PAGE_CB(t_mem code);

  // Page ED, 3k mode
  virtual int LD_EIR_A(t_mem code);
  virtual int LD_IIR_A(t_mem code);
  virtual int LD_A_EIR(t_mem code);
  virtual int LD_A_IIR(t_mem code);
  virtual int LDD(t_mem code) { return ld_d_i(-1); }
  virtual int LDI(t_mem code) { return ld_d_i(+1); }
  virtual int EXX_iSP_HL(t_mem code);
  virtual int LD_BC_imn(t_mem code) { return ld_dd_imn(destBC()); }
  virtual int LD_DE_imn(t_mem code) { return ld_dd_imn(destDE()); }
  virtual int LD_HL_imn_ped(t_mem code) { return ld_dd_imn(destHL()); }
  virtual int LD_SP_imn(t_mem code) { return ld_dd_imn(cSP); }
  virtual int SBC_HL_BC(t_mem code) { return sub16(rBC, true); }
  virtual int SBC_HL_DE(t_mem code) { return sub16(rDE, true); }
  virtual int SBC_HL_HL(t_mem code) { return sub16(rHL, true); }
  virtual int SBC_HL_SP(t_mem code) { return sub16(rSP, true); }
  virtual int ADC_HL_BC(t_mem code) { return adc_hl_ss(rBC); }
  virtual int ADC_HL_DE(t_mem code) { return adc_hl_ss(rDE); }
  virtual int ADC_HL_HL(t_mem code) { return adc_hl_ss(rHL); }
  virtual int ADC_HL_SP(t_mem code) { return adc_hl_ss(rSP); }
  
  // Page DD/FD, 3k mode
  virtual int LD_IR_mn(t_mem code);
  virtual int ADD_IR_BC(t_mem code) { return add_ir_xy(rBC); }
  virtual int ADD_IR_DE(t_mem code) { return add_ir_xy(rDE); }
  virtual int ADD_IR_IR(t_mem code) { return add_ir_xy(cIR->get()); }
  virtual int ADD_IR_SP(t_mem code) { return add_ir_xy(rSP); }
  virtual int INC_iIRd(t_mem code);
  virtual int DEC_iIRd(t_mem code);
  virtual int CP_A_iIRd(t_mem code) { tick5p1(7); vc.rd++; return cp8(rA, dest8iIRd(fetch()).R()); }
  virtual int SBC_A_iIRd(t_mem code) { tick5p1(5); vc.rd++; return sub8(dest8iIRd(fetch()).R(), true); }
  virtual int SUB_A_iIRd(t_mem code) { tick5p1(5); vc.rd++; return sub8(dest8iIRd(fetch()).R(), false); }
  virtual int ADD_A_iIRd(t_mem code) { tick5p1(5); vc.rd++; return add8(dest8iIRd(fetch()).R(), false); }
  virtual int ADC_A_iIRd(t_mem code) { tick5p1(5); vc.rd++; return add8(dest8iIRd(fetch()).R(), true); }
  virtual int OR_A_iIRd(t_mem code);
  virtual int AND_A_iIRd(t_mem code);
  virtual int POP_IR(t_mem code);
  virtual int PUSH_IR(t_mem code);
  virtual int LD_iIRd_A(t_mem code) { return ld_iIRd_r(rA); }
  virtual int LD_iIRd_B(t_mem code) { return ld_iIRd_r(rB); }
  virtual int LD_iIRd_C(t_mem code) { return ld_iIRd_r(rC); }
  virtual int LD_iIRd_D(t_mem code) { return ld_iIRd_r(rD); }
  virtual int LD_iIRd_E(t_mem code) { return ld_iIRd_r(rE); }
  virtual int LD_iIRd_H(t_mem code) { return ld_iIRd_r(rH); }
  virtual int LD_iIRd_L(t_mem code) { return ld_iIRd_r(rL); }
  virtual int LD_A_iIRd(t_mem code) { return ld_r_iIRd(destA()); }
  virtual int LD_B_iIRd(t_mem code) { return ld_r_iIRd(destB()); }
  virtual int LD_C_iIRd(t_mem code) { return ld_r_iIRd(destC()); }
  virtual int LD_D_iIRd(t_mem code) { return ld_r_iIRd(destD()); }
  virtual int LD_E_iIRd(t_mem code) { return ld_r_iIRd(destE()); }
  virtual int LD_H_iIRd(t_mem code) { return ld_r_iIRd(destH()); }
  virtual int LD_L_iIRd(t_mem code) { return ld_r_iIRd(destL()); }
  virtual int LD_SP_IR(t_mem code);
  virtual int LD_IR_iSPn(t_mem code);
  virtual int LD_iSPn_IR(t_mem code);
  virtual int LD_HL_iIRd(t_mem code);
  virtual int LD_iIRd_n(t_mem code);
  virtual int PAGE_DD_CB(t_mem code);
};


enum rxkcpu_cfg {
  //rxk_cpu_xpc		= 0,

  rxk_cpu_nuof	= 0
};

class cl_rxk_cpu: public cl_hw
{
protected:
  class cl_rxk *ruc;
  class cl_cell8 *segsize, *dataseg, *stackseg;
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
