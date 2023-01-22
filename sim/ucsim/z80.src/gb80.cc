/*
 * Simulator of microcontrollers (z80.src/gb80.cc)
 *
 * Copyright (C) 2021,21 Drotos Daniel, Talker Bt.
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

//#include "ddconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// prj
//#include "pobjcl.h"
#include "utils.h"

// sim
#include "simcl.h"

// local
//#include "z80cl.h"
#include "gb80cl.h"
#include "glob.h"

#define uint32 t_addr
#define uint8 unsigned char

/*******************************************************************/

lr35902_memory::lr35902_memory( cl_uc &uc_parent_ref ):uc_r(uc_parent_ref) { }

cl_gb80::cl_gb80(struct cpu_entry *Itype, class cl_sim *asim):
  cl_z80(Itype, asim), mem(*this)
{
  type= Itype;
  BIT_C= 0x10;
  BIT_A= 0x20;
  BIT_N= 0x40;
  BIT_Z= 0x80;
  BIT_ALL= (BIT_C |BIT_N |BIT_A |BIT_Z);
  BIT_P= 0;
  BIT_S= 0;
  regs.raf.F= 0;
  regs.ralt_af.aF= 0;
}

int
cl_gb80::init(void)
{
  cl_uc::init(); /* Memories now exist */

  //rom= address_space(MEM_ROM_ID);  // code goes here...
  
  //  ram= mem(MEM_XRAM);
  //ram= address_space(MEM_XRAM_ID);  // data goes here...
  
  
  // zero out ram(this is assumed in regression tests)
  for (int i=0xA000; i<0xFF80; i++) {
    ram->set((t_addr) i, 0);
  }
  
  return(0);
}

const char *
cl_gb80::id_string(void)
{
  return("GB80");
}


void
cl_gb80::mk_hw_elements(void)
{
  //class cl_base *o;
  cl_uc::mk_hw_elements();
}

void lr35902_memory::init(void) {
  cl_address_space *as_rom;
  cl_address_space *as_ram;
  
  as_rom = new cl_address_space("rom"/*MEM_ROM_ID*/,
				lr35902_rom_start, lr35902_rom_size, 8);
  as_rom->init();
  uc_r.address_spaces->add(as_rom);
  rom = as_rom;
  
  as_ram = new cl_address_space(MEM_XRAM_ID,
				lr35902_ram_start, lr35902_ram_size, 8);
  as_ram->init();
  uc_r.address_spaces->add(as_ram);
  ram = as_ram;
}

void
cl_gb80::make_memories(void)
{
  mem.init( );
  rom= mem.rom;
  ram= mem.ram;
  
  regs8= new cl_address_space("regs8", 0, 16, 8);
  regs8->init();
  regs8->get_cell(0)->decode((t_mem*)&regs.raf.A);
  regs8->get_cell(1)->decode((t_mem*)&regs.raf.F);
  regs8->get_cell(2)->decode((t_mem*)&regs.bc.h);
  regs8->get_cell(3)->decode((t_mem*)&regs.bc.l);
  regs8->get_cell(4)->decode((t_mem*)&regs.de.h);
  regs8->get_cell(5)->decode((t_mem*)&regs.de.l);
  regs8->get_cell(6)->decode((t_mem*)&regs.hl.h);
  regs8->get_cell(7)->decode((t_mem*)&regs.hl.l);

  regs8->get_cell(8)->decode((t_mem*)&regs.ralt_af.aA);
  regs8->get_cell(9)->decode((t_mem*)&regs.ralt_af.aF);
  regs8->get_cell(10)->decode((t_mem*)&regs.a_bc.h);
  regs8->get_cell(11)->decode((t_mem*)&regs.a_bc.l);
  regs8->get_cell(12)->decode((t_mem*)&regs.a_de.h);
  regs8->get_cell(13)->decode((t_mem*)&regs.a_de.l);
  regs8->get_cell(14)->decode((t_mem*)&regs.a_hl.h);
  regs8->get_cell(15)->decode((t_mem*)&regs.a_hl.l);

  regs16= new cl_address_space("regs16", 0, 11, 16);
  regs16->init();

  regs16->get_cell(0)->decode((t_mem*)&regs.AF);
  regs16->get_cell(1)->decode((t_mem*)&regs.BC);
  regs16->get_cell(2)->decode((t_mem*)&regs.DE);
  regs16->get_cell(3)->decode((t_mem*)&regs.HL);
  regs16->get_cell(4)->decode((t_mem*)&regs.IX);
  regs16->get_cell(5)->decode((t_mem*)&regs.IY);
  regs16->get_cell(6)->decode((t_mem*)&regs.SP);
  regs16->get_cell(7)->decode((t_mem*)&regs.aAF);
  regs16->get_cell(8)->decode((t_mem*)&regs.aBC);
  regs16->get_cell(9)->decode((t_mem*)&regs.aDE);
  regs16->get_cell(10)->decode((t_mem*)&regs.aHL);

  address_spaces->add(regs8);
  address_spaces->add(regs16);

  class cl_var *v;
  vars->add(v= new cl_var("A", regs8, 0, ""));
  v->init();
  vars->add(v= new cl_var("F", regs8, 1, ""));
  v->init();
  vars->add(v= new cl_var("B", regs8, 2, ""));
  v->init();
  vars->add(v= new cl_var("C", regs8, 3, ""));
  v->init();
  vars->add(v= new cl_var("D", regs8, 4, ""));
  v->init();
  vars->add(v= new cl_var("E", regs8, 5, ""));
  v->init();
  vars->add(v= new cl_var("H", regs8, 6, ""));
  v->init();
  vars->add(v= new cl_var("L", regs8, 7, ""));
  v->init();

  vars->add(v= new cl_var("ALT_A", regs8, 8, ""));
  v->init();
  vars->add(v= new cl_var("ALT_F", regs8, 9, ""));
  v->init();
  vars->add(v= new cl_var("ALT_B", regs8, 10, ""));
  v->init();
  vars->add(v= new cl_var("ALT_C", regs8, 11, ""));
  v->init();
  vars->add(v= new cl_var("ALT_D", regs8, 12, ""));
  v->init();
  vars->add(v= new cl_var("ALT_E", regs8, 13, ""));
  v->init();
  vars->add(v= new cl_var("ALT_H", regs8, 14, ""));
  v->init();
  vars->add(v= new cl_var("ALT_L", regs8, 15, ""));
  v->init();

  vars->add(v= new cl_var("AF", regs16, 0, ""));
  v->init();
  vars->add(v= new cl_var("BC", regs16, 1, ""));
  v->init();
  vars->add(v= new cl_var("DE", regs16, 2, ""));
  v->init();
  vars->add(v= new cl_var("HL", regs16, 3, ""));
  v->init();
  vars->add(v= new cl_var("IX", regs16, 4, ""));
  v->init();
  vars->add(v= new cl_var("IY", regs16, 5, ""));
  v->init();
  vars->add(v= new cl_var("SP", regs16, 6, ""));
  v->init();
  vars->add(v= new cl_var("ALT_AF", regs16, 7, ""));
  v->init();
  vars->add(v= new cl_var("ALT_BC", regs16, 8, ""));
  v->init();
  vars->add(v= new cl_var("ALT_DE", regs16, 9, ""));
  v->init();
  vars->add(v= new cl_var("ALT_HL", regs16, 10, ""));
  v->init();
}

void cl_gb80::store1( u16_t addr, t_mem val ) {
  mem.store1( addr, val );
}

void cl_gb80::store2( u16_t addr, u16_t val ) {
  mem.store2( addr, val );
}

u8_t  cl_gb80::get1( u16_t addr ) {
  return mem.get1( addr );
}

u16_t  cl_gb80::get2( u16_t addr ) {
  return mem.get2( addr );
}

void lr35902_memory::store1( u16_t addr, t_mem val ) {
  if (addr < lr35902_ram_start) {
    /* flag illegal operation ? */
    return;
  }
  
  if ((addr- lr35902_ram_start) < lr35902_ram_size) {
    ram->write(addr, val);
  }
}

void lr35902_memory::store2( u16_t addr, u16_t val ) {
  store1(addr,   val & 0xff);
  store1(addr+1, (val >> 8) & 0xff);
}

u8_t  lr35902_memory::get1( u16_t addr ) {
  if (addr < lr35902_rom_size) {
    return rom->read(addr);    
  }
  
  if (addr < lr35902_ram_start) {
    /* flag illegal operation ? */
    return (addr & 0xff);
  }
  
  if ((addr-lr35902_ram_start) < lr35902_ram_size) {
    return ram->read(addr);
  }
  
  return (addr & 0xff);
}

u16_t  lr35902_memory::get2( u16_t addr ) {
  u16_t  l, h;
  
  l = get1(addr  );
  h = get1(addr+1);
  
  return (h << 8) | l;
}

/*
 * Help command interpreter
 */

struct dis_entry *
cl_gb80::dis_tbl(void)
{
  return(disass_gb80);
}


int
cl_gb80::inst_length(t_addr addr)
{
  int len = 0;

  get_disasm_info(addr, &len, NULL, NULL);

  return len;
}

int
cl_gb80::inst_branch(t_addr addr)
{
  int b;

  get_disasm_info(addr, NULL, &b, NULL);

  return b;
}

int
cl_gb80::longest_inst(void)
{
  return 4;
}


const char *
cl_gb80::get_disasm_info(t_addr addr,
                        int *ret_len,
                        int *ret_branch,
                        int *immed_offset)
{
  const char *b = NULL;
  uint code;
  int len = 0;
  int immed_n = 0;
  int i;
  int start_addr = addr;
  struct dis_entry *dis_e;

  code= rom->get(addr++);
  dis_e = NULL;

  switch(code) {
    case 0xcb:  /* ESC code to lots of op-codes, all 2-byte */
      code= rom->get(addr++);
      i= 0;
      while ((code & disass_gb80_cb[i].mask) != disass_gb80_cb[i].code &&
        disass_gb80_cb[i].mnemonic)
        i++;
      dis_e = &disass_gb80_cb[i];
      b= disass_gb80_cb[i].mnemonic;
      if (b != NULL)
        len += (disass_gb80_cb[i].length + 1);
    break;

    default:
      i= 0;
      while ((code & disass_gb80[i].mask) != disass_gb80[i].code &&
             disass_gb80[i].mnemonic)
        i++;
      dis_e = &disass_gb80[i];
      b= disass_gb80[i].mnemonic;
      if (b != NULL)
        len += (disass_gb80[i].length);
    break;
  }


  if (ret_branch) {
    *ret_branch = dis_e->branch;
  }

  if (immed_offset) {
    if (immed_n > 0)
         *immed_offset = immed_n;
    else *immed_offset = (addr - start_addr);
  }

  if (len == 0)
    len = 1;

  if (ret_len)
    *ret_len = len;

  return b;
}

char *
cl_gb80::disass(t_addr addr)
{
  chars work, temp;
  const char *b;
  int len = 0;
  int immed_offset = 0;
  bool first= true;
  
  work= "";
  b = get_disasm_info(addr, &len, NULL, &immed_offset);
  if (b == NULL)
    {
      return strdup("UNKNOWN/INVALID");
    }

  while (*b)
    {
      if ((*b == ' ') && first)
	{
	  first= false;
	  while (work.len() < 6) work.append(' ');
	}
      if (*b == '%')
        {
	  temp= "";
          b++;
          switch (*(b++))
            {
            case 'd': // d    jump relative target, signed? byte immediate operand
              temp.format("#%d", (char)rom->get(addr+immed_offset));
              ++immed_offset;
              break;
            case 'w': // w    word immediate operand
              temp.format("#0x%04x",
                 (uint)((rom->get(addr+immed_offset)) |
                        (rom->get(addr+immed_offset+1)<<8)) );
              ++immed_offset;
              ++immed_offset;
              break;
            case 'b': // b    byte immediate operand
              temp.format("#0x%02x", (uint)rom->get(addr+immed_offset));
              ++immed_offset;
              break;
            default:
              temp= "?";
              break;
            }
	  work+= temp;
        }
      else
        work+= *(b++);
    }
  return strdup(work.c_str());
}


void
cl_gb80::print_regs(class cl_console_base *con)
{
  con->dd_color("answer");
  con->dd_printf("ZNHC----  Flags= 0x%02x %3d %c  ",
                 regs.raf.F, regs.raf.F, isprint(regs.raf.F)?regs.raf.F:'.');
  con->dd_printf("A= 0x%02x %3d %c\n",
                 regs.raf.A, regs.raf.A, isprint(regs.raf.A)?regs.raf.A:'.');
  con->dd_printf("%s\n", cbin(regs.raf.F,8).c_str());
		 
  con->dd_printf("BC= 0x%04x [BC]= %02x %3d %c  ",
                 regs.BC, ram->get(regs.BC), ram->get(regs.BC),
                 isprint(ram->get(regs.BC))?ram->get(regs.BC):'.');
  con->dd_printf("DE= 0x%04x [DE]= %02x %3d %c  ",
                 regs.DE, ram->get(regs.DE), ram->get(regs.DE),
                 isprint(ram->get(regs.DE))?ram->get(regs.DE):'.');
  con->dd_printf("HL= 0x%04x [HL]= %02x %3d %c\n",
                 regs.HL, ram->get(regs.HL), ram->get(regs.HL),
                 isprint(ram->get(regs.HL))?ram->get(regs.HL):'.');
  con->dd_printf("SP= 0x%04x [SP]= %02x %3d %c\n",
                 regs.SP, ram->get(regs.SP), ram->get(regs.SP),
                 isprint(ram->get(regs.SP))?ram->get(regs.SP):'.');

  print_disass(PC, con);
}

/*
 * Execution
 */

int
cl_gb80::exec_inst(void)
{
  t_mem code;

  instPC= PC;

  if (fetch(&code))
    return(resBREAKPOINT);
  tick(1);
  inc_R();
  
  switch (code)
    {
    case 0x00: return(inst_nop(code));
    case 0x01: tick(1);
    case 0x02: case 0x06: tick(1);return(inst_ld(code));
    case 0x03: tick(2);
    case 0x04: return(inst_inc(code));
    case 0x05: return(inst_dec(code));
    case 0x07: {
      int ret= (inst_rlca(code));
      regs.raf.F&= ~(BIT_Z|BIT_A|BIT_N);
      return ret;
    }
    case 0x08: return(inst_st_sp_abs(code));
    case 0x09: return(inst_add(code));
    case 0x0a: case 0x0e: tick(1);return(inst_ld(code));
    case 0x0b: tick(2);
    case 0x0d: return(inst_dec(code));
    case 0x0c: return(inst_inc(code));
    case 0x0f: {
      int ret= (inst_rrca(code));
      regs.raf.F&= ~(BIT_Z|BIT_A|BIT_N);
      return ret;
    }

    case 0x10: return(inst_stop0(code));
    case 0x11: tick(1);
    case 0x12: case 0x16: tick(1);return(inst_ld(code));
    case 0x13: tick(2);
    case 0x14: return(inst_inc(code));
    case 0x15: return(inst_dec(code));
    case 0x17: {
      int ret= (inst_rla(code));
      regs.raf.F&= ~(BIT_Z|BIT_A|BIT_N);
      return ret;
    }
    case 0x18: return(inst_jr(code));
    case 0x19: return(inst_add(code));
    case 0x1a: case 0x1e: tick(1);return(inst_ld(code));
    case 0x1b: tick(2);
    case 0x1d: return(inst_dec(code));
    case 0x1c: return(inst_inc(code));
    case 0x1f: {
      int ret= (inst_rra(code));
      regs.raf.F&= ~(BIT_Z|BIT_A|BIT_N);
      return ret;
    }

    case 0x20: return(inst_jr(code));
    case 0x21: tick(1);
    case 0x26: tick(1);return(inst_ld(code));
    case 0x22: return inst_ldi(code);
    case 0x23: tick(2);
    case 0x24: return(inst_inc(code));
    case 0x25: return(inst_dec(code));
    case 0x27: return(inst_daa(code));
      
    case 0x28: return(inst_jr(code));
    case 0x29: return(inst_add(code));
    case 0x2a: return(inst_ldi(code));
    case 0x2b: tick(2);
    case 0x2d: return(inst_dec(code));
    case 0x2c: return(inst_inc(code));
    case 0x2e: tick(1);return(inst_ld(code));
    case 0x2f: return(inst_cpl(code));

    case 0x30: return(inst_jr(code));
    case 0x31: case 0x36: tick(2);return(inst_ld(code));
    case 0x32: return(inst_ldd(code));
    case 0x33: tick(2);
    case 0x34: return(inst_inc(code));
    case 0x35: return(inst_dec(code));
    case 0x37: return(inst_scf(code));
      
    case 0x38: return(inst_jr(code));
    case 0x39: return(inst_add(code));
    case 0x3a: return inst_ldd(code);
    case 0x3b: tick(2);
    case 0x3d: return(inst_dec(code));
    case 0x3c: return(inst_inc(code));
    case 0x3e: tick(1);return(inst_ld(code));
    case 0x3f: return(inst_ccf(code));

    case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47:
    case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4f:
      return(inst_ld(code));
    case 0x46: case 0x4e: tick(1);return(inst_ld(code));

    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57:
    case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5f:
      return(inst_ld(code));
    case 0x56: case 0x5e: tick(1);return(inst_ld(code));

    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67:
    case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6f:
      return(inst_ld(code));
    case 0x66: case 0x6e: tick(1);return(inst_ld(code));

    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: case 0x7e:
      tick(1);return(inst_ld(code));
    case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7f:
      return(inst_ld(code));
    case 0x76: return(inst_halt(code));

    case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87:
      return(inst_add(code));
    case 0x86: tick(1);return(inst_add(code));
    case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8f:
      return(inst_adc(code));
    case 0x8e: tick(1);return(inst_adc(code));

    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97:
      return(inst_sub(code));
    case 0x96: tick(1);return(inst_sub(code));
    case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9f:
      return(inst_sbc(code));
    case 0x9e: tick(1);return(inst_sbc(code));

    case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa7:
      return(inst_and(code));
    case 0xa6: tick(1);return(inst_and(code));
    case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xaf:
      return(inst_xor(code));
    case 0xae: tick(1);return(inst_xor(code));

    case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb7:
      return(inst_or(code));
    case 0xb6: tick(1);return(inst_or(code));
    case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbf:
      return(inst_cp(code));
    case 0xbe: tick(1);return(inst_cp(code));

    case 0xc0: tick(3);return(inst_ret(code));
    case 0xc1: tick(2);return(inst_pop(code));
    case 0xc2: tick(2);return(inst_jp(code));
    case 0xc3: tick(6);return(inst_jp(code));
    case 0xc4: tick(2);return(inst_call(code));
    case 0xc5: tick(5);return(inst_push(code));
    case 0xc6: tick(1);return(inst_add(code));
    case 0xc7: tick(5);return(inst_rst(code));

    case 0xc8: tick(3);return(inst_ret(code));
    case 0xc9: tick(6);return(inst_ret(code));
    case 0xca: tick(2);return(inst_jp(code));

      /* CB escapes out to 2 byte opcodes(CB include), opcodes
         to do register bit manipulations */
    case 0xcb: return(inst_cb( ));
    case 0xcc: tick(2);return(inst_call(code));
    case 0xcd: tick(7);return(inst_call(code));
    case 0xce: tick(1);return(inst_adc(code));
    case 0xcf: tick(5);return(inst_rst(code));

    case 0xd0: tick(3);return(inst_ret(code));
    case 0xd1: tick(2);return(inst_pop(code));
    case 0xd2: tick(2);return(inst_jp(code));
    case 0xd3: break;
    case 0xd4: tick(2);return(inst_call(code));
    case 0xd5: tick(5);return(inst_push(code));
    case 0xd6: tick(1);return(inst_sub(code));
    case 0xd7: tick(5);return(inst_rst(code));

    case 0xd8: tick(3);return(inst_ret(code));
 case 0xd9: return(inst_reti(code));
    case 0xda: tick(2);return(inst_jp(code));
    case 0xdb: break;
    case 0xdc: tick(2);return(inst_call(code));
      
 case 0xdd: break;  /* IX register doesn't exist on the GB80 */
    case 0xde: tick(1);return(inst_sbc(code));
    case 0xdf: tick(5);return(inst_rst(code));
      
      
    case 0xe0: return(inst_ldh(code));
    case 0xe1: tick(2);return(inst_pop(code));
    case 0xe2: return(inst_ldh(code));
    case 0xe3:
 case 0xe4: break;
    case 0xe5: tick(5);return(inst_push(code));
    case 0xe6: tick(1);return(inst_and(code));
    case 0xe7: tick(5);return(inst_rst(code));

    case 0xe8: return(inst_add_sp_d(code));
    case 0xe9: tick(2);return(inst_jp(code));
    case 0xea: return(inst_ld16(code));
    case 0xeb:
    case 0xec: case 0xed: break;
    case 0xee: tick(1);return(inst_xor(code));
    case 0xef: tick(5);return(inst_rst(code));
      
    case 0xf0: return(inst_ldh(code));
    case 0xf1: {
      int ret= (inst_pop(code));
      regs.raf.F&= 0xf0;
      tick(2);
      return ret;
    };
 case 0xf2: return(inst_ldh(code));
    case 0xf3: return(inst_di(code));
    case 0xf4: break;
    case 0xf5: tick(5);return(inst_push(code));
    case 0xf6: tick(1);return(inst_or(code));
    case 0xf7: tick(5);return(inst_rst(code));

    case 0xf8: return(inst_ldhl_sp(code));
    case 0xf9: tick(2);return(inst_ld(code));
    case 0xfa: return(inst_ld16(code));
    case 0xfb: return(inst_ei(code));
    case 0xfc: 
    case 0xfd: break;
    case 0xfe: tick(1);return(inst_cp(code));
    case 0xff: tick(5);return(inst_rst(code));
    }

  //PC= instPC;//rom->inc_address(PC, -1);
  return(resINV_INST);
}

/* End of z80.src/gb80.cc */
