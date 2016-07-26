/*
 * Simulator of microcontrollers (tlcs.src/tlcscl.h)
 *
 * Copyright (C) 2016,16 Drotos Daniel, Talker Bt.
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

/* $Id: tlcscl.h 296 2016-07-04 20:10:18Z  $ */

#ifndef TLCSCL_HEADER
#define TLCSCL_HEADER

// sim.src
#include "uccl.h"
#include "memcl.h"


enum tlcs_flags {
  FLAG_S= 0x80,
  FLAG_Z= 0x40,
  FLAG_I= 0x20,
  FLAG_H= 0x10,
  FLAG_X= 0x08,
  FLAG_V= 0x04,
  FLAG_N= 0x02,
  FLAG_C= 0x01
};

/*
 * Base type of TLCS microcontrollers
 */

class cl_tlcs: public cl_uc
{
 protected:
  struct {
    union {
      uint16_t af;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t a;
	uint8_t f;
#else
	uint8_t f;
	uint8_t a;
#endif
      };
    };
    union {
      uint16_t bc;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t b;
	uint8_t c;
#else
	uint8_t c;
	uint8_t b;
#endif
      };
    };
    union {
      uint16_t de;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t d;
	uint8_t e;
#else
	uint8_t e;
	uint8_t d;
#endif
      };
    };
    union {
      uint16_t hl;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t h;
	uint8_t l;
#else
	uint8_t l;
	uint8_t h;
#endif
      };
    };
    uint16_t ix;
    uint16_t iy;
    uint16_t sp;
    uint8_t dummy;
    uint16_t dummy16;
    union {
      uint16_t alt_af;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t alt_a;
	uint8_t alt_f;
#else
	uint8_t alt_f;
	uint8_t alt_a;
#endif
      };
    };
    union {
      uint16_t alt_bc;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t alt_b;
	uint8_t alt_c;
#else
	uint8_t alt_c;
	uint8_t alt_b;
#endif
      };
    };
    union {
      uint16_t alt_de;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t alt_d;
	uint8_t alt_e;
#else
	uint8_t alt_e;
	uint8_t alt_d;
#endif
      };
    };
    union {
      uint16_t alt_hl;
      struct {
#ifdef WORDS_BIGENDIAN
	uint8_t alt_h;
	uint8_t alt_l;
#else
	uint8_t alt_l;
	uint8_t alt_h;
#endif
      };
    };
  } reg;

 public:
  class cl_address_space *nas;
  class cl_address_space *xas;
  class cl_address_space *yas;
  class cl_address_space *regs8;
  class cl_address_space *regs16;
 public:
  cl_tlcs(class cl_sim *asim);
  virtual int init(void);
  virtual char *id_string(void);

  //virtual t_addr get_mem_size(enum mem_class type);
  //virtual int get_mem_width(enum mem_class type);
  virtual void mk_hw_elements(void);
  virtual void make_memories(void);

  virtual struct dis_entry *dis_tbl(void);
  //virtual struct name_entry *sfr_tbl(void);
  //virtual struct name_entry *bit_tbl(void);
  virtual const char *regname_r(uint8_t r);
  virtual const char *regname_Q(uint8_t Q);
  virtual const char *regname_R(uint8_t R);
  virtual const char *regname_i(uint8_t i);
  virtual const char *bitname(uint8_t b);
  virtual const char *condname_cc(uint8_t cc);
  virtual const char *condname_C(uint8_t cc);
  virtual char *disass(t_addr addr, const char *sep);
  virtual void print_regs(class cl_console_base *con);
  virtual int inst_length(t_addr addr);

  virtual int exec_inst(void);
  virtual int exec_inst2(uint8_t c1);
  virtual int exec_inst2_f3(uint8_t c1);
  virtual int exec_inst2_fe(uint8_t c1);
  virtual int exec_inst2_f7(uint8_t c1);
  virtual int exec_inst2_e0gg(uint8_t c1, uint8_t c2);
  virtual int exec_inst2_e8gg(uint8_t c1, uint8_t c2);
  virtual int exec_inst2_f8gg(uint8_t c1, uint8_t c2);
  virtual int exec_inst3_e7(uint8_t c1, uint8_t c2, uint8_t c3);
  virtual int exec_inst3(uint8_t c1, uint8_t c2);
  virtual int exec_inst3_f0ix(uint8_t c1);
  virtual int exec_inst4_f4ix(uint8_t c1, uint8_t c2, uint8_t c3);
  virtual int exec_inst4_e3(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
  virtual int exec_inst4_ef(uint8_t c1, uint8_t c2, uint8_t c3);
  virtual int exec_inst4_eb(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
  
  virtual t_addr do_push(t_mem data);
  virtual t_addr do_pop(t_mem *data);
  virtual int exec_push(t_addr PC_of_inst, t_mem data);
  virtual int exec_ret(t_addr PC_of_inst, t_mem *data);
  virtual int exec_reti(t_addr PC_of_inst, t_mem *data);
  virtual int exec_pop(t_addr PC_of_inst, t_mem *data);
  virtual int exec_intr(t_addr PC_of_inst, t_addr called, t_mem data);
  virtual int exec_call(t_addr PC_of_inst, t_addr called, t_mem data);
  virtual void set_p(uint8_t data);
  virtual uint8_t *aof_reg8(uint8_t data_r);
  virtual uint16_t *aof_reg16_rr(uint8_t data_rr);
  virtual uint16_t *aof_reg16_qq(uint8_t data_qq);
  virtual uint16_t *aof_reg16_ix(uint8_t data_ix);
  virtual uint16_t *aof_reg16_gg(uint8_t data_gg);
  virtual class cl_memory_cell *cell_hl_a();
  virtual class cl_memory_cell *cell_gg(uint8_t gg);
  virtual class cl_memory_cell *cell_n(uint8_t n);
  virtual class cl_memory_cell *cell_ixd(uint8_t ix, int8_t d);
  
  virtual uint16_t mem16(t_addr addr);
  virtual uint16_t mem16gg(uint8_t gg);
  virtual uint16_t mem16ixd(uint8_t ix, int8_t d);
  virtual void write16(t_addr addr, uint16_t val);
  virtual void write16gg(uint8_t gg, uint16_t val);
  virtual void write16ixd(uint8_t ix, int8_t d, uint16_t val);
  virtual bool flag(enum tlcs_flags f);
  virtual bool cc(uint8_t cc);
  
  // (1) 8-bit data transfer

  // (2) 16-bit data transfer
  virtual int inst_pop(t_mem c1);	// 58+qq
  
  // (3) exchange, block transfer and search
  virtual int ex_de_hl();		// 08
  virtual int ex_af_alt_af();		// 09
  virtual int exx();			// 0a
  virtual int ldi();			// Fe 58
  virtual int ldir();			// Fe 59
  virtual int ldd();			// Fe 5a
  virtual int lddr();			// Fe 5b
  virtual int cpi();			// Fe 5c
  virtual int cpir();		       	// Fe 5d
  virtual int cpd();			// Fe 5e
  virtual int cpdr();			// Fe 5f
  
  // (4) 8-bit arithmetic and logic operation
  virtual uint8_t op_inc(uint8_t data);			// INC 8 bit
  virtual void inst_inc(cl_memory_cell *cell);		// INC mem
  virtual void inst_incx(cl_memory_cell *cell);		// INCX mem
  virtual uint8_t op_dec(uint8_t data);			// DEC 8 bit
  virtual void inst_dec(cl_memory_cell *cell);		// DEC mem
  virtual void inst_decx(cl_memory_cell *cell);		// DECX mem
  virtual uint8_t op_add8(uint8_t d1, uint8_t d2);	// ADD 8-bit
  virtual uint8_t op_add_a(uint8_t d);			// ADD A,8-bit
  virtual int inst_add_a(class cl_memory_cell *cell);	// ADD A,mem
  virtual uint8_t op_adc8(uint8_t d1, uint8_t d2);	// ADC 8-bit
  virtual int inst_adc_a(uint8_t d);			// ADC A,8-bit
  virtual int inst_adc_a(class cl_memory_cell *cell);	// ADC A,mem
  virtual uint8_t op_sub8(uint8_t d1, uint8_t d2);	// SUB 8-bit
  virtual int inst_sub_a(uint8_t d);			// SUB A,8-bit
  virtual int inst_sub_a(class cl_memory_cell *cell);	// SUB A,mem
  virtual uint8_t op_sbc8(uint8_t d1, uint8_t d2);	// SBC 8-bit
  virtual int inst_sbc_a(uint8_t d);			// SBC A,8-bit
  virtual int inst_sbc_a(class cl_memory_cell *cell);	// SBC A,mem
  virtual uint8_t op_and8(uint8_t d1, uint8_t d2);	// AND 8-bit
  virtual int inst_and_a(uint8_t d);			// AND A,8-bit
  virtual int inst_and_a(class cl_memory_cell *cell);	// AND A,mem
  virtual uint8_t op_xor8(uint8_t d1, uint8_t d2);	// XOR 8-bit
  virtual int inst_xor_a(uint8_t d);			// XOR A,8-bit
  virtual int inst_xor_a(class cl_memory_cell *cell);	// XOR A,mem
  virtual uint8_t op_or8(uint8_t d1, uint8_t d2);	// OR 8-bit
  virtual int inst_or_a(uint8_t d);			// OR A,8-bit
  virtual int inst_or_a(class cl_memory_cell *cell);	// OR A,mem
  virtual uint8_t op_cp8(uint8_t d1, uint8_t d2);	// CP 8-bit
  virtual int op_cp_a(uint8_t d);			// CP A,8-bit
  virtual int op_cp_a(class cl_memory_cell *cell);	// CP A,mem

  // (5) 16-bit arithmetic and logic operation
  virtual uint16_t op_inc16(uint16_t data);		// INC 16 bit
  virtual uint16_t inst_inc16(t_addr addr);		// INCW mem
  virtual uint16_t inst_inc16gg(uint8_t gg,t_addr addr);// INCW mem
  virtual uint16_t inst_inc16ix(uint8_t ix,t_addr addr);// INCW mem
  virtual uint16_t op_dec16(t_mem data);		// DEC 16 bit
  virtual uint16_t inst_dec16(t_addr addr);		// DECW mem
  virtual uint16_t inst_dec16gg(uint8_t gg,t_addr addr);// DECW mem
  virtual uint16_t inst_dec16ix(uint8_t ix,t_addr addr);// DECW mem
  virtual uint16_t op_add_hl(t_mem val);		// ADD HL,16-bit
  virtual uint16_t op_add_hl(t_addr addr);		// ADD HL,mem
  virtual uint16_t op_adc_hl(t_mem val);		// ADC HL,16-bit
  virtual uint16_t op_adc_hl(t_addr addr);		// ADC HL,mem
  virtual uint16_t op_sub_hl(t_mem val);		// SUB HL,16-bit
  virtual uint16_t op_sub_hl(t_addr addr);		// SUB HL,mem
  virtual uint16_t op_sbc_hl(t_mem val);		// SBC HL,16-bit
  virtual uint16_t op_sbc_hl(t_addr addr);		// SBC HL,mem
  virtual uint16_t op_and_hl(t_mem val);		// AND HL,16-bit
  virtual uint16_t op_and_hl(t_addr addr);		// AND HL,mem
  virtual uint16_t op_xor_hl(t_mem val);		// XOR HL,16-bit
  virtual uint16_t op_xor_hl(t_addr addr);		// XOR HL,mem
  virtual uint16_t op_or_hl(t_mem val);			// OR HL,16-bit
  virtual uint16_t op_or_hl(t_addr addr);		// OR HL,mem
  virtual uint16_t op_add16(t_mem op1, t_mem op2);	// ADD 16-bit
  virtual uint16_t op_sub16(t_mem op1, t_mem op2);	// SUB 16-bit

  // (6) cpu control and others
  virtual int inst_daa_a();		// 0b
  virtual int inst_cpl_a();		// 10
  virtual int inst_neg_a();		// 11
  virtual int inst_ccf();		// 0e
  virtual int inst_scf();		// 0d
  virtual int inst_rcf();		// 0c
  //virtual int nop();			// 00
  //virtual int halt();			// 01
  //virtual int di();			// 02
  //virtual int ei();			// 03
  virtual int inst_swi();		// ff
  virtual int inst_mul_hl(class cl_memory_cell *cell);
  virtual int inst_div_hl(class cl_memory_cell *cell);
  virtual int inst_div_hl(uint8_t d);
  
  // (7) rotate and shift
  virtual uint8_t op_rlc(uint8_t data, bool set_sz);	// RLC 8-bit
  virtual uint8_t inst_rlc(cl_memory_cell *cell);	// RLC mem
  virtual uint8_t op_rrc(uint8_t data, bool set_sz);	// RRC 8-bit
  virtual uint8_t inst_rrc(cl_memory_cell *cell);	// RRC mem
  virtual uint8_t op_rl(uint8_t data, bool set_sz);	// RL 8-bit
  virtual uint8_t inst_rl(cl_memory_cell *cell);	// RL mem
  virtual uint8_t op_rr(uint8_t data, bool set_sz);	// RR 8-bit
  virtual uint8_t inst_rr(cl_memory_cell *cell);	// RR mem
  virtual uint8_t op_sla(uint8_t data, bool set_sz);	// SLA 8-bit
  virtual uint8_t inst_sla(cl_memory_cell *cell);	// SLA mem
  virtual uint8_t op_sra(uint8_t data, bool set_sz);	// SRA 8-bit
  virtual uint8_t inst_sra(cl_memory_cell *cell);	// SRA mem
  virtual uint8_t op_srl(uint8_t data, bool set_sz);	// SRL 8-bit
  virtual uint8_t inst_srl(cl_memory_cell *cell);	// SRL mem
  virtual int inst_rld(class cl_memory_cell *cell);	// RLD
  virtual int inst_rrd(class cl_memory_cell *cell);	// RLD

  // (8) bit manipulation
  virtual uint8_t op_tset(uint8_t val, uint8_t bitnr);	// TSET 8-bit
  virtual uint8_t inst_tset(cl_memory_cell *cell, uint8_t bitnr);
  virtual uint8_t op_bit(uint8_t val, uint8_t bitnr);	// BIT 8-bit
  virtual uint8_t inst_bit(cl_memory_cell *cell, uint8_t bitnr);
  virtual uint8_t op_res(uint8_t val, uint8_t bitnr);	// RES 8-bit
  virtual uint8_t inst_res(cl_memory_cell *cell, uint8_t bitnr);
  virtual uint8_t op_set(uint8_t val, uint8_t bitnr);	// SET 8-bit
  virtual uint8_t inst_set(cl_memory_cell *cell, uint8_t bitnr);

  // (9) jump, call and return
  virtual int inst_ret();			// 1e
  virtual int inst_reti();			// 1f
  virtual int inst_call(t_addr PC_of_inst, uint16_t addr);	// CALL addr
  virtual int inst_djnz_b(int8_t d);
  virtual int inst_djnz_bc(int8_t d);
  
  // ?
};


#endif

/* End of tlcs.src/tlcscl.h */
