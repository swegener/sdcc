/*
 * Simulator of microcontrollers (inst.cc)
 *
 * stm8 code base from Vaclav Peroutka vaclavpe@users.sourceforge.net
 * and Valentin Dudouyt valentin.dudouyt@gmail.com
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
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
//#include <stdlib.h>

// local
#include "stm8cl.h"
//#include "regsstm8.h"
#include "stm8mac.h"

int
cl_stm8::fetchea(t_mem code, unsigned char prefix)
{
   int ret = resHALT;
   int ftc;

   switch ((code >> 4) & 0x0f) {
   case 0x1: {
     unsigned char index = fetch();
     get_regs();
     ret = index+regs.SP;  // SP indexed
     break;
   }

   case 0xb:
     ret = fetch();           // direct short
     break;


   case 0xc:
      if ( 0 == prefix) {              // direct long
         ret = fetch2();
      } else if ( 0x72 == prefix) {    // long pointer
         ftc = fetch2();
         ret = get2(ftc);
         decode_cycle();
      } else if ( 0x92 == prefix) {    // short pointer
         ftc = fetch();
         ret = get2(ftc);
         decode_cycle();
      }
      break;

   case 0xd:
      if ( 0 == prefix) {              // long offset with X reg
         int offset = fetch2();
         get_regs();
         ret = offset+regs.X;
      } else if ( 0x72 == prefix) {    // long pointer to offset with X reg
         ftc = get2(fetch2());
         decode_cycle();
         ret = ftc+regs.X;
      } else if ( 0x90 == prefix) {    // long offset with Y reg
         int offset = fetch2();
         get_regs();
         ret = offset+regs.Y;
      } else if ( 0x91 == prefix) {    // short pointer to offset with Y reg
         ftc = get2(fetch());
         decode_cycle();
         ret = ftc+regs.Y;
      } else if ( 0x92 == prefix) {    // short pointer to offset with X reg
         ftc = get2(fetch());
         decode_cycle();
         ret = ftc+regs.X;
      }
      break;


   case 0xe: {
      int offset = fetch();
      get_regs();
      if ( 0 == prefix) {               // short offset with X reg
         ret = offset+regs.X;
      } else if ( 0x90 == prefix) {     // short offset with Y reg
         ret = offset+regs.Y;
      }
      break;
   }

   case 0xf:
      get_regs();
      if ( 0 == prefix) {               // X index
         ret = regs.X;
      } else if ( 0x90 == prefix) {     // Y index
         ret = regs.Y;
      }
      break;
   }

   return ret;
}

int
cl_stm8::get_dest(t_mem code, unsigned char prefix)
{
  int resaddr, ftc;

  switch ((code >> 4) & 0x0f) {
    case 0x0:
	  if ( 0 == prefix) {              // short offset with SP
		 resaddr = fetch();
		 get_regs();
		 resaddr += regs.SP;
	  } else {
	    resaddr = ( resHALT);
	  }
     break;

    case 0x3:
	  if ( 0 == prefix) {              // short direct
		 resaddr = fetch();
     } else if ( 0x72 == prefix) {    // long indirect - pointer
         ftc = fetch2();
         resaddr = get2(ftc);
         decode_cycle();
     } else if ( 0x92 == prefix) {    // short indirect - pointer
         ftc = fetch();
         resaddr = get2(ftc);
         decode_cycle();
	  } else {
	    resaddr = ( resHALT);
	  }
     break;

    case 0x4:
	  if ( 0x72 == prefix) {           // long offset with X
         ftc = fetch2();
         get_regs();
         resaddr = ftc + regs.X;
     } else if ( 0x90 == prefix) {    // long offset with Y
         ftc = fetch2();
         get_regs();
         resaddr = ftc + regs.Y;
	  } else {
         resaddr = ( resHALT);
	  }
     break;

    case 0x5:
	  if ( 0x72 == prefix) {           // long direct
         resaddr = fetch2();
	  } else {
         resaddr = ( resHALT);
	  }
     break;

    case 0x6:
          if ( 0 == prefix) {              // short offset with X
		resaddr = fetch();
		get_regs();
		resaddr += regs.X;
	  } else if ( 0x72 == prefix) {    // long pointer to offset with X
          ftc = get2(fetch2());
          decode_cycle();
	  resaddr = ftc+regs.X;
	  } else if ( 0x90 == prefix) {    // short offset with Y
	    resaddr = fetch();
	    get_regs();
	    resaddr += regs.Y;
	  } else if ( 0x91 == prefix) {    // short pointer to offset with Y
          ftc = get2(fetch());
          decode_cycle();
	    resaddr = ftc+regs.Y;
	  } else if ( 0x92 == prefix) {    // short pointer to offset with X
          ftc = get2(fetch());
          decode_cycle();
	    resaddr = ftc+regs.X;
	  } else {
	    resaddr =( resHALT);
	  }
     break;

    case 0x7:
	  if ( 0 == prefix) {              // X index
		get_regs();
		resaddr = regs.X;
	  } else if ( 0x90 == prefix) {    // Y index
	    get_regs();
	    resaddr = regs.Y;
	  } else {
	    resaddr =( resHALT);
	  }
     break;

    default:
      resaddr =(resHALT);
      break;
  }

  return resaddr;
}

void
cl_stm8::flag_cvh(int x, int m, int r, bool byte, bool add, int mask)
{
  bool xt, xs, xh, mt, ms, mh, rt, rs, rh;

  if (byte)
    {
      xt = !!(x & 0x80);
      xs = !!(x & 0x40);
      xh = !!(x & 0x08);
      mt = !!(m & 0x80);
      ms = !!(m & 0x40);
      mh = !!(m & 0x08);
      rt = !!(r & 0x80);
      rs = !!(r & 0x40);
      rh = !!(r & 0x08);
    }
  else
    {
      xt = !!(x & 0x8000);
      xs = !!(x & 0x4000);
      xh = !!(x & 0x0080);
      mt = !!(m & 0x8000);
      ms = !!(m & 0x4000);
      mh = !!(m & 0x0080);
      rt = !!(r & 0x8000);
      rs = !!(r & 0x4000);
      rh = !!(r & 0x0080);
    }

  if (add)
    {
      if (mask & BIT_V)
        FLAG_ASSIGN (BIT_V, ((xt && mt) || (mt && !rt) || (!rt && xt)) ^ ((xs && ms) || (ms && !rs) || (!rs && xs)));
      if (mask & BIT_C)
        FLAG_ASSIGN (BIT_C, (xt && mt) || (mt && !rt) || (!rt && xt));
      if (mask & BIT_H)
        FLAG_ASSIGN (BIT_H, (xh && mh) || (mh && !rh) || (!rh && xh));
    }
  else
    {
      if (mask & BIT_V)
        FLAG_ASSIGN (BIT_V, ((!xt && mt) || (!xt && rt) || (xt && mt && rt)) ^ ((!xs && ms) || (!xs && rs) || (xs && ms && rs)));
      if (mask & BIT_H)
        FLAG_ASSIGN (BIT_H, (!xh && mh) || (!xh && rh) || (xh && mh && rh));
      if (mask & BIT_C)
        FLAG_ASSIGN (BIT_C, (!xt && mt) || (!xt && rt) || (xt && mt && rt));
    }
}

#define FLAG_CVH_BYTE_ADD(x, m, r, mask) flag_cvh(x, m, r, true, true, mask)
#define FLAG_CVH_BYTE_SUB(x, m, r, mask) flag_cvh(x, m, r, true, false, mask)
#define FLAG_CVH_WORD_ADD(x, m, r, mask) flag_cvh(x, m, r, false, true, mask)
#define FLAG_CVH_WORD_SUB(x, m, r, mask) flag_cvh(x, m, r, false, false, mask)

int
cl_stm8::inst_adc(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;
  int carryin = !!(regs.CC & BIT_C);

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);
  result = operand1 + operand2 + carryin;

  store_regs();

  FLAG_NZ (result);
  FLAG_CVH_BYTE_ADD(operand1, operand2, result, BIT_C | BIT_V | BIT_H);

  regs.A = result & 0xff;
  return(resGO);
}

int
cl_stm8::inst_add(t_mem code, unsigned char prefix)
{
  FLAG_CLEAR(BIT_C);
  return inst_adc(code, prefix);
}

void
cl_stm8::pipeline_flush(bool need_exec)
{
  pipetrace_type("X");

  if (!pipeline_busy.instr)
    pipeline_busy.instr = true;

  pipeline_index = PC & 3;
  pipeline_bytes = 0;

  // Suppress all fetches in the flush cycle
  pipeline_busy.program = pipeline_busy.data = true;

  if (need_exec)
    exec_cycle();

  pipeline_busy.flush = true;

  if (pipetrace_fold)
    pipetrace_ticks = 0;
}

void
cl_stm8::decode_cycle(void)
{
  tick(1);
  pipetrace_type("D");
  //pipeline_busy.instr = true;
}

void
cl_stm8::get_regs(void)
{
  if (pipeline_busy.regs)
    {
      error(new cl_error_stm8_pipeline_fetch_stall());
      pipetrace_type("Sraw");
      decode_cycle();
    }

  //pipeline_busy.instr = true;
  //pipeline_busy.regs = true;
}

int
cl_stm8::get_1(unsigned int addr)
{
  class cl_address_decoder *d = rom->get_decoder_of(addr);
  bool *busy = (d && d->memchip != flash_chip ? &pipeline_busy.data : &pipeline_busy.program);

  if (*busy && !pipeline_busy.instr)
    pipetrace_type("Sbus");

  if (*busy || pipeline_busy.instr)
    decode_cycle();

  pipeline_busy.instr = true;

  vc.rd++;
  int v = ram->read((t_addr) (addr));
  *busy = true;

  return v;
}

int
cl_stm8::get2(unsigned int addr)
{
  class cl_address_decoder *d = rom->get_decoder_of(addr);
  bool *busy = (d && d->memchip != flash_chip ? &pipeline_busy.data : &pipeline_busy.program);

  if (*busy && !pipeline_busy.instr)
    pipetrace_type("Sbus");

  if (*busy || pipeline_busy.instr)
    decode_cycle();

  pipeline_busy.instr = true;

  vc.rd++;
  int lo = ram->read((t_addr)addr+1);
  *busy = true;

  decode_cycle();

  vc.rd++;
  int hi = ram->read((t_addr)addr);
  *busy = true;

  return((hi << 8) | lo);
}

int
cl_stm8::get3(unsigned int addr)
{
  class cl_address_decoder *d = rom->get_decoder_of(addr);
  bool *busy = (d && d->memchip != flash_chip ? &pipeline_busy.data : &pipeline_busy.program);

  if (*busy && !pipeline_busy.instr)
    pipetrace_type("Sbus");

  if (*busy || pipeline_busy.instr)
    decode_cycle();

  pipeline_busy.instr = true;

  vc.rd++;
  int lo = ram->read((t_addr)addr+2);
  *busy = true;

  decode_cycle();

  vc.rd++;
  int hi = ram->read((t_addr)addr+1);
  *busy = true;

  decode_cycle();

  vc.rd++;
  int ex = ram->read((t_addr)addr);
  *busy = true;

  return((ex << 16) | (hi << 8) | lo);
}

void
cl_stm8::exec_cycle(void)
{
  // Fully consumed 32bit words in the prefetch buffer are discarded
  // once we pass into the execute phase.
  // Note that the discard happens early enough that a fetch can happen
  // in the decode cycle (as seen in PM0044 table 6).
  pipeline_bytes -= pipeline_index & (~3);
  pipeline_index &= 3;

  tick(1);
  pipetrace_type("E");
  pipeline_busy.instr = true;
}

void
cl_stm8::store_regs(void)
{
  exec_cycle();
  pipeline_busy.regs = true;
}

void
cl_stm8::store1(unsigned int addr, unsigned int val)
{
  class cl_address_decoder *d = rom->get_decoder_of(addr);
  bool *busy = (d && d->memchip != flash_chip ? &pipeline_busy.data : &pipeline_busy.program);

  exec_cycle();
  vc.wr++;
  ram->write((t_addr) (addr), val);
  *busy = true;
}

void
cl_stm8::store2(unsigned int addr, unsigned int val)
{
  class cl_address_decoder *d = rom->get_decoder_of(addr);
  bool *busy = (d && d->memchip != flash_chip ? &pipeline_busy.data : &pipeline_busy.program);

  // RM0016 17.3.3 Prescalar
  // "Do not use the LDW instruction for this purpose as it writes the LSB first"
  //
  // Writing words LSB first allows any second ALU cycle to be overlapped
  // with the write of the LSB effectively reducing the cycle count by one
  // for word-wide arithmetic operations.
  exec_cycle();
  vc.wr++;
  ram->write((t_addr)addr+1, val & 0xff);
  *busy = true;

  exec_cycle();
  vc.wr++;
  ram->write((t_addr)addr, (val >> 8) & 0xff);
  *busy = true;
}

int
cl_stm8::inst_addw(t_mem code, unsigned char prefix)
{
  long int result, operand1, operand2, nibble_high, nibble_low;
  u16_t *dest_ptr;
  bool sub;

  nibble_high = (code >> 4) & 0x0f;
  nibble_low = code & 0x0f;
  dest_ptr = nibble_low == 0x09 || nibble_low == 0x02 ? &regs.Y : &regs.X;
  operand1 = *dest_ptr;

  switch(nibble_high)
  {
    case 0x1:
    case 0xa: // Immediate
      {
        operand2 = fetch2();
        // We have the data so we only need two ALU execution cycles
        // but cannot overlap the first with the previous decode cycle.
        exec_cycle();
        break;
      }
    case 0xb: // Long
      {
        operand2 = get2(fetch2());
        // The data is fetched LSB first and the first ALU execution
        // overlaps with the MSB fetch so we only have the final ALU
        // execution to account for.
        break;
      }
    case 0xf: // sp-indexed
      {
        operand2 = get2(regs.SP + fetch());
        // The data is fetched LSB first and the first ALU execution
        // overlaps with the MSB fetch so we only have the final ALU
        // execution to account for.
        break;
      }
    default: return(resHALT);
  }

  switch(nibble_low)
  {
    case 0x0:
    case 0x2:
    case 0xd: sub = true; break;
    case 0x9:
    case 0xb:
    case 0xc: sub = false; break;
    default: return(resHALT);
  }

  if (sub)
    result = operand1 - operand2;
  else
    result = operand1 + operand2;

  store_regs();

  FLAG_ASSIGN (BIT_N, 0x8000 & result);
  FLAG_ASSIGN (BIT_Z, (result & 0xffff) == 0);
  if (sub)
    FLAG_CVH_WORD_SUB(operand1, operand2, result, BIT_C | BIT_H | BIT_V);
  else
    FLAG_CVH_WORD_ADD(operand1, operand2, result, BIT_C | BIT_H | BIT_V);

  *dest_ptr = result & 0xffff;
  return(resGO);
}

int
cl_stm8::inst_and(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);

  store_regs();
  result = operand1 & operand2;
  FLAG_NZ (result);

  regs.A = result & 0xff;
  return(resGO);
}

int
cl_stm8::inst_bccmbcpl(t_mem code, unsigned char prefix)
{
   int ea = fetch2();
   unsigned char dbyte;
   dbyte= get1( ea);

   if (code & 0x01)  { // bccm
      char pos = (code - 0x11) >> 1;
      dbyte = dbyte & (~(1<<pos));
      if (regs.CC & BIT_C) {
         dbyte |= (1<<pos);
      }
   } else { // bcpl
      char pos = (code - 0x10) >> 1;
      dbyte = dbyte ^ (1<<pos);
   }

   store1(ea, dbyte);
   return(resGO);
}
int
cl_stm8::inst_bcp(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);
  exec_cycle();
  result = operand1 & operand2 & 0xff;
  FLAG_NZ (result);

  return(resGO);
}

int
cl_stm8::inst_bresbset(t_mem code, unsigned char prefix)
{
  int ea = fetch2();
  unsigned char dbyte;
  dbyte= get1( ea);

  if (code & 0x01) { // bres
	char pos = (code - 0x11) >> 1;
	dbyte = dbyte & (~(1<<pos));
  } else { // bset
 	char pos = (code - 0x10) >> 1;
	dbyte = dbyte | (1<<pos);
  }

  store1(ea, dbyte);
  return(resGO);
}
int
cl_stm8::inst_btjfbtjt(t_mem code, unsigned char prefix)
{
  int ea = fetch2();
  char reljump = fetch();

  char pos = code >> 1; // No need to pick out bits 3:1 - the others are all 0
  unsigned char dbyte = get1(ea);

  store_regs();

  FLAG_ASSIGN(BIT_C, (dbyte >> pos) & 1);

  if ((regs.CC & BIT_C) ^ (code & 1)) { // Bit 0 of code selects branch on true (0) or false (1)
    PC += reljump;

    // PM0044 5.4.3 Pipeline with Call/Jump
    // For a JP instruction, the fetch [of the next instruction] can start
    // during the first cycle of the "dummy" execution.
    pipeline_flush(true);
  }

  return(resGO);
}

int
cl_stm8::inst_call(t_mem code, unsigned char prefix)
{
  t_addr newPC = (PC & 0xff0000ul) + fetchea(code, prefix);
  push2(PC);
  // PM0044 5.4.3 Pipeline with Call/Jump
  // "For the CALL instruction, it [the fetch of the next instruction] starts
  // after the last cycle of the CALL execution."
  PC = newPC;
  pipeline_flush(false);

  return(resGO);
}

int
cl_stm8::inst_clr(t_mem code, unsigned char prefix)
{
  unsigned int opaddr = 0;


  FLAG_SET (BIT_Z);
  FLAG_CLEAR (BIT_N);

  switch(((code & 0xf0) | (prefix << 8)) >> 4) {
    /* clr */
    case 0x004: regs.A = 0; return(resGO);
    case 0x003: opaddr = fetch(); break;
    case 0x725: opaddr = fetch2(); break;
    case 0x007: get_regs(); opaddr = regs.X; break;
    case 0x006: opaddr = fetch(); get_regs(); opaddr += regs.X; break;
    case 0x724: opaddr = fetch2(); get_regs(); opaddr += regs.X; break;
    case 0x907: get_regs(); opaddr = regs.Y; break;
    case 0x906: opaddr = fetch(); get_regs(); opaddr += regs.Y; break;
    case 0x904: opaddr = fetch2(); get_regs(); opaddr += regs.Y; break;
    case 0x000: opaddr = fetch(); get_regs(); opaddr += regs.SP; break;
    case 0x923: opaddr = get2(fetch()); decode_cycle(); break; // short indirect
    case 0x723: opaddr = get2(fetch2()); decode_cycle(); break; // long indirect
    case 0x926: opaddr = get2(fetch()); decode_cycle(); opaddr += regs.X; break; // short x-indexed indirect
    case 0x726: opaddr = get2(fetch2()); decode_cycle(); opaddr += regs.X; break; // long x-indexed indirect
    case 0x916: opaddr = get2(fetch()); decode_cycle(); opaddr += regs.Y; break; // short y-indexed indirect
    /* clrw */
    case 0x005: regs.X = 0; exec_cycle(); return(resGO);
    case 0x905: regs.Y = 0; exec_cycle(); return(resGO);
    default: return(resHALT);
  }

  get1(opaddr);
  store1(opaddr, 0);
  return(resGO);
}

int
cl_stm8::inst_cp(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);

  store_regs();

  result = (operand1 - operand2) & 0xff;

  FLAG_NZ (result);
  FLAG_CVH_BYTE_SUB(operand1, operand2, result, BIT_C | BIT_V);

  return(resGO);
}

int
cl_stm8::inst_cpw(t_mem code, unsigned char prefix)
{
  long int operand1, operand2, result;
  int reversed = 0;

  operand1 = prefix == 0x90 ? regs.Y : regs.X;
  operand2 = prefix == 0x90 ? regs.X : regs.Y;

  switch((code & 0xf0) >> 4)
  {
    case 0xa: operand2 = fetch2(); exec_cycle(); break; // Immediate
    case 0xb: operand2 = get2(fetch()); break; // Short
    case 0xc:
      switch (prefix)
      {
        case 0x00:
        case 0x90: operand2 = get2(fetch2()); break; // Long direct
        case 0x92: operand2 = get2(fetch()); decode_cycle(); operand2 = get2(operand2); break; // short indirect
        case 0x72: operand2 = get2(fetch2()); decode_cycle(); operand2 = get2(operand2); break; // long indirect
        case 0x91: operand2 = get2(fetch()); decode_cycle(); operand2 = get2(operand2); operand1 = regs.Y; break; // short indirect
        default: return(resHALT);
      }
      break;
    case 0x1: operand2 = get2(fetch() + regs.SP); break; // SP-indexed
    case 0xf: get_regs(); operand1 = get2(operand1); reversed = 1; break; // cpw X|Y, (Y|X)
    case 0xe: get_regs(); operand1 = get2(operand1 + fetch()); reversed = 1; break; // short indexed direct
    case 0xd:
      switch (prefix)
      {
        case 0x00:
        case 0x90: get_regs(); operand1 = get2(fetch2() + operand1); break; // short indexed direct
        case 0x91: operand1 = regs.Y + get2(fetch()); decode_cycle(); operand1 = get2(operand1); operand2 = regs.X; break; // short y-indexed indirect
        case 0x92: { operand1 += get2(fetch()); decode_cycle(); operand1 = get2(operand1); break; } // short x-indexed indirect
        case 0x72: { operand1 += get2(fetch2()); decode_cycle(); operand1 = get2(operand1); break; } // long x-indexed indirect
        default: return(resHALT);
      }
      reversed = 1;
      break;
    default: return(resHALT);
  }

  if (!reversed)
    result = (operand1 - operand2) & 0xffff;
  else
    result = (operand2 - operand1) & 0xffff;

  store_regs();

  FLAG_ASSIGN (BIT_Z, (result & 0xffff) == 0);
  FLAG_ASSIGN (BIT_N, 0x8000 & result);
  if (!reversed)
    FLAG_CVH_WORD_SUB(operand1, operand2, result, BIT_C | BIT_V);
  else
    FLAG_CVH_WORD_SUB(operand2, operand1, result, BIT_C | BIT_V);

  return(resGO);
}

int
cl_stm8::inst_cpl(t_mem code, unsigned char prefix)
{
  long int operand;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   operand ^= 0xffff;

   FLAG_SET (BIT_C);

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = operand&0xff;
      FLAG_ASSIGN (BIT_Z, (operand & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & operand);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      regs.X = operand & 0xffff;
      FLAG_ASSIGN (BIT_Z, (operand & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & operand);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      regs.Y = operand & 0xffff;
      FLAG_ASSIGN (BIT_Z, (operand & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & operand);
   } else {
      store1(opaddr, operand &0xff);
      FLAG_ASSIGN (BIT_Z, (operand & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & operand);
   }

   return(resGO);
}

int
cl_stm8::inst_dec(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand - 1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_CVH_BYTE_ADD(operand, 0xff, resval, BIT_V);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      store_regs();
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_CVH_WORD_ADD(operand, 0xffff, resval, BIT_V);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      store_regs();
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_CVH_WORD_ADD(operand, 0xffff, resval, BIT_V);
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_CVH_BYTE_ADD(operand, 0xff, resval, BIT_V);
   }

   return(resGO);
}

int
cl_stm8::inst_div(t_mem code, unsigned char prefix)
{
  if (div_cycle == 0) { // Starting or restarting a division
    dividend = regs.X;
    divisor = regs.A;
    if (code == 0x65)
      divisor = regs.Y;
    else if (prefix == 0x90)
      dividend = regs.Y;
    else if (prefix != 0x00)
      return resINV_INST;

    //printf("init:   ");
    //for (int i = 31; i >= 0; i--)
    //  printf("%c%s", (dividend & (1 << i)) ? '1' : '0', (i % 4) == 0 ? " " : "");
    //printf(" / ");
    //for (int i = 15; i >= 0; i--)
    //  printf("%c%s", (divisor & (1 << i)) ? '1' : '0', (i % 4) == 0 ? " " : "");
    //printf(" (%u / %u)\n", dividend, divisor);

    store_regs();
    FLAG_CLEAR(BIT_C);

    if (divisor == 0x00) {
      FLAG_SET(BIT_C);
      return resGO;
    }

    div_cycle = 16;

    PC = instPC;
    return resNOT_DONE;
  }

#if 0
  // SRT division

  exec_cycle();
  while (--div_cycle && ((dividend & 0xe0000000) == 0 || (dividend & 0x0e0000000) == 0x0e0000000))
    dividend <<= 1;

  if (div_cycle) {
    if ((dividend & 0x80000000) == 0x80000000) {
      quotient = quotient - (1 << div_cycle);
      dividend <<= 1;
      dividend = (((dividend >> 16) + divisor) << 16) | (dividend & 0xffff);
    } else {
      quotient |= (1 << div_cycle);
      dividend <<= 1;
      dividend = (((dividend >> 16) - divisor) << 16) | (dividend & 0xffff);
    }
  }
  //printf("%2d: ", div_cycle);
  //for (int i = 31; i >= 0; i--)
  //  printf("%c%s", (dividend & (1 << i)) ? '1' : '0', (i % 4) == 0 ? " " : "");
  //printf("\n");

  if (div_cycle == 0) {
    if ((dividend & 0x80000000) == 0x80000000) {
      dividend = ((dividend >> 16) + divisor) << 16;
      quotient--;
    }
    dividend |= quotient;
  }
#else
  // Restoring algorithm with collapsed shifting

  exec_cycle();
  while (div_cycle) {
    div_cycle--;

    dividend <<= 1;

    //printf("%2d: << 1: ", div_cycle);
    //for (int i = 31; i >= 0; i--)
    //  printf("%c%s", (dividend & (1 << i)) ? '1' : '0', (i % 4) == 0 ? " " : "");
    //printf("\n");

    if ((dividend >> 16) >= divisor) {
      dividend = (((dividend >> 16) - divisor) << 16) | (dividend & 0xffff) | 1;

      //printf("%2d: sub : ", div_cycle);
      //for (int i = 31; i >= 0; i--)
      //  printf("%c%s", (dividend & (1 << i)) ? '1' : '0', (i % 4) == 0 ? " " : "");
      //printf("\n");

      break;
    }
  }
#endif

  if (div_cycle) {
    PC = instPC;
    return resNOT_DONE;
  }

  //printf("res : %u r %u\n", (dividend & 0xffff), (dividend >> 16));

  if (code == 0x65)
    regs.Y = (dividend >> 16);
  else
    regs.A = (dividend >> 16);

  if (prefix == 0x90) {
    FLAG_ASSIGN(BIT_Z, (regs.Y = (dividend & 0xffff)));
  } else {
    FLAG_ASSIGN(BIT_Z, (regs.X = (dividend & 0xffff)));
  }

  FLAG_CLEAR(BIT_N);
  FLAG_CLEAR(BIT_H);
  FLAG_CLEAR(BIT_V);

  pipeline_busy.regs = true;
  return resGO;
}

int
cl_stm8::inst_inc(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand + 1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_CVH_BYTE_ADD(operand, 0x01, resval, BIT_V);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      store_regs();
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_CVH_WORD_ADD(operand, 0x0001, resval, BIT_V);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      store_regs();
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_CVH_WORD_ADD(operand, 0x0001, resval, BIT_V);
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_CVH_BYTE_ADD(operand, 0x01, resval, BIT_V);
   }

   return(resGO);
}

int
cl_stm8::inst_jp(t_mem code, unsigned char prefix)
{
  t_addr newPC = (PC & 0xff0000ul) + fetchea(code, prefix);
  // PM0044 5.4.3 Pipeline with Call/Jump
  // For a JP instruction, the fetch [of the next instruction] can start
  // during the first cycle of the "dummy" execution.
  PC = newPC;
  pipeline_flush(true);
  return(resGO);
}

int
cl_stm8::inst_jr(t_mem code, unsigned char prefix)
{
  bool taken;
  signed char ofs;
  unsigned char bz, bn, bv;

  if (prefix ==0x00) {
    switch ((code>>1) & 7) {
      case 0: // JRT - JRA (20) / JRF (21)
        taken = 1;
        break;
      case 1: // JRUGT (22) / JRULE (23)
        taken = !(regs.CC & (BIT_C | BIT_Z));
        break;
      case 2: // JRUGE (24) / JRULT (25)
        taken = !(regs.CC & BIT_C);
        break;
      case 3: // JRNE (26) / JREQ (27)
        taken = !(regs.CC & BIT_Z);
        break;
      case 4: // JRNV (28) / JRV (29)
        taken = !(regs.CC & BIT_V);
        break;
      case 5: // JRPL (2A) / JRMI (2B)
        taken = !(regs.CC & BIT_N);
        break;
      case 6: // JRSGT (2C) - Z or (N xor V) = 0 / JRSLE (2D) - Z or (N xor V) = 1
        bz = !!(regs.CC & BIT_Z);
        bn = !!(regs.CC & BIT_N);
        bv = !!(regs.CC & BIT_V);
        taken = !(bz | (bn ^ bv));
        break;
      case 7: // JRSGE (2E) - N xor V = 0 / / JRSLT(2F) N xor V = 1
        bn = !!(regs.CC & BIT_N);
        bv = !!(regs.CC & BIT_V);
        taken = !(bn ^ bv);
        break;
      default:
        return(resHALT);
    }
  }
  else if (prefix==0x90) {
    switch ((code>>1) & 7) {
      case 4: // JRNH (28) / JRH (29)
         taken = !(regs.CC & BIT_H);
       break;
      case 6: // JRNM (2C) / JRM (2D)
        taken = !(regs.CC & (BIT_I1|BIT_I0));
        break;
      case 7: // JRIL (2E) / JRIH (2F), no means to test this ???
        taken = 0;
        break;
      default:
        return(resHALT);
    }
  }
  else
    return(resHALT);

  if (code & 1)
    taken = ! taken;

  ofs = fetch();

  if (taken)
    {
      // PM0044 5.4.3 Pipeline with Call/Jump
      // For a JP instruction, the fetch [of the next instruction] can start
      // during the first cycle of the "dummy" execution.
      PC += ofs;
      pipeline_flush(true);
    }
  else
    exec_cycle();

  return(resGO);
}

int
cl_stm8::inst_lda(t_mem code, unsigned char prefix)
{
  int operand;
  operand = OPERAND(code, prefix);
  store_regs();
  FLAG_NZ (operand);
  regs.A = operand;
  return(resGO);
}

int
cl_stm8::operandw(t_mem code, unsigned char prefix)
{
       if(EA_IMM(code)) {
               return(fetch2());
       } else {
               int addr = fetchea(code,prefix);
               int result = get2(addr);
               return(result);
       }
}

int
cl_stm8::inst_ldxy(t_mem code, unsigned char prefix)
{
  unsigned int operand;
  u16_t *dest_ptr;

  dest_ptr = (prefix == 0x90) ? &regs.Y : &regs.X;
  if((prefix == 0x00 && code == 0x16) || (prefix == 0x91 && code == 0xce) || (prefix == 0x91 && code == 0xde)) dest_ptr = &regs.Y;

  switch((code & 0xf0) >> 4) {
     case 0xa: operand = fetch2(); exec_cycle(); break; // Immediate
     case 0xb: operand = get2(fetch()); break; // Short
     case 0xc:
       switch (prefix) {
       case 0x90:
       case 0x00:
         operand = get2(fetch2()); // Long direct
         break;
       case 0x92:
       case 0x91:
         operand = get2(fetch()); // short indirect
         decode_cycle();
         operand = get2(operand);
         break;
       case 0x72:
         operand = get2(fetch2()); // long indirect
         decode_cycle();
         operand = get2(operand);
         break;
       default:
         return(resHALT);
       }
       break;
     case 0xf: get_regs(); operand = get2(*dest_ptr); break;
     case 0xe: get_regs(); operand = get2(*dest_ptr + fetch()); break;
     case 0xd:
       switch (prefix) {
       case 0x90:
       case 0x00:
         get_regs();
         operand = get2(*dest_ptr + fetch2()); // Long x/y-indexed direct
         break;
       case 0x92:
       case 0x91:
         operand = get2(fetch()); // short x/y-indexed indirect
         decode_cycle();
         operand = get2(operand + *dest_ptr);
         break;
       case 0x72:
         operand = get2(fetch2()); // long x-indexed indirect
         decode_cycle();
         operand = get2(operand + *dest_ptr);
         break;
       default:
         return(resHALT);
       }
       break;
     case 0x1: operand = get2(regs.SP + fetch()); break;
     default:  return(resHALT);
  }

  store_regs();

  if ((code & 0xf0) != 0x90)
    {
      FLAG_ASSIGN (BIT_Z, (operand & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & operand);
    }

  *dest_ptr = operand;

  return(resGO);
}

int
cl_stm8::inst_lddst(t_mem code, unsigned char prefix)
{
  unsigned int opaddr;

   opaddr = fetchea(code,prefix);

   get1(opaddr);

   FLAG_ASSIGN (BIT_Z, (regs.A & 0xff) == 0);
   FLAG_ASSIGN (BIT_N, 0x80 & regs.A);

   store1(opaddr, regs.A);

   return(resGO);
}

int
cl_stm8::inst_ldxydst(t_mem code, unsigned char prefix)
{
  /* ldw dst, REG */
  unsigned int opaddr, operand;

  switch ((((code & 0xf0) | (prefix << 8)) >> 4) & 0xfff)
    {
      case 0x00b:
      case 0x00c:
      case 0x92c:
      case 0x72c:
      case 0x90f:
      case 0x90e:
      case 0x90d:
      case 0x91d:
        operand = regs.X;
        break;
      case 0x00f:
      case 0x00e:
      case 0x00d:
      case 0x92d:
      case 0x72d:
      case 0x90b:
      case 0x90c:
      case 0x91c:
        operand = regs.Y;
        break;
      case 0x001:
        switch (code)
          {
            case 0x17:
              operand = regs.Y;
              break;
            case 0x1f:
              operand = regs.X;
              break;
            default:
              return resHALT;
          }
        break;
      default:
        return resHALT;
    }

  switch((code & 0xf0) >> 4) {
    case 0x1: opaddr = regs.SP + fetch(); break;
    case 0xb: opaddr = fetch(); break;
    case 0xc:
      switch (prefix) {
      case 0x00:
      case 0x90:
        opaddr = fetch2(); // long direct
        break;
      case 0x91:
      case 0x92:
        opaddr = get2(fetch()); // short indirect
        decode_cycle();
        opaddr += 0;
        decode_cycle();
        break;
      case 0x72:
        opaddr = get2(fetch2()); // long indirect
        decode_cycle();
        opaddr += 0;
        decode_cycle();
        break;
      default:
        return (resHALT);
      }
      break;
    case 0xf: get_regs(); opaddr = (prefix == 0x90) ? regs.Y : regs.X; break;
    case 0xe: get_regs(); opaddr = ((prefix == 0x90) ? regs.Y : regs.X) + fetch(); break;
    case 0xd:
      switch (prefix) {
      case 0x90:
        get_regs();
        opaddr = regs.Y + fetch2(); // long y-indexed direct
        break;
      case 0x00:
        get_regs();
        opaddr = regs.X + fetch2(); // long x-indexed direct
        break;
      case 0x92:
        opaddr = get2(fetch()); // short x-indexed indirect
        decode_cycle();
        opaddr += regs.X;
        decode_cycle();
        break;
      case 0x91:
        opaddr = get2(fetch()); // short y-indexed indirect
        decode_cycle();
        opaddr += regs.Y;
        decode_cycle();
        break;
      case 0x72:
        opaddr = get2(fetch2()); // long x-indexed indirect
        decode_cycle();
        opaddr += regs.X;
        decode_cycle();
        break;
      default:
        return (resHALT);
      }
      break;
    default: return(resHALT);
  }

  FLAG_ASSIGN (BIT_Z, (operand & 0xffff) == 0);
  FLAG_ASSIGN (BIT_N, 0x8000 & operand);

  store2(opaddr, operand);

  return(resGO);
}

int
cl_stm8::inst_neg(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = 0 - operand;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_V, (0x80 == operand));
      FLAG_ASSIGN (BIT_C, 0x100 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      regs.X = resval&0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_V, (0x8000 == operand));
      FLAG_ASSIGN (BIT_C, 0x10000 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      regs.Y = resval&0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_V, (0x8000 == operand));
      FLAG_ASSIGN (BIT_C, 0x10000 & resval);
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_V, (0x80 == operand));
      FLAG_ASSIGN (BIT_C, 0x100 & resval);
   }

   return(resGO);
}

int
cl_stm8::inst_or(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);
  result = (operand1 | operand2) & 0xff;
  FLAG_NZ (result);

  store_regs();

  regs.A = result & 0xff;
  return(resGO);
}

int
cl_stm8::inst_rlc(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) && (prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) && (prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) && (prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest (code, prefix);
      operand = get1 (opaddr);
   }

   resval = operand << 0x1;

   if (regs.CC & BIT_C) {
      resval++;
   }

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x100));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x10000));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x10000));
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x100));
   }

   return (resGO);
}

int
cl_stm8::inst_rrc(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand >> 1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      if (regs.CC & BIT_C) {      resval |= 0x80;   }
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      if (regs.CC & BIT_C) {      resval |= 0x8000;   }
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      if (regs.CC & BIT_C) {      resval |= 0x8000;   }
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else {
      if (regs.CC & BIT_C) {      resval |= 0x80;   }
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   }

   return(resGO);
}

int
cl_stm8::inst_sbc(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;
  int carryin = !!(regs.CC & BIT_C);

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);

  store_regs();

  result = (operand1 - operand2 - carryin) & 0xff;

  FLAG_NZ (result);
  FLAG_CVH_BYTE_SUB(operand1, operand2, result, BIT_C | BIT_V);

  regs.A = result;
  return(resGO);
}

int
cl_stm8::inst_sll(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand << 0x1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x100));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x10000));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x10000));
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (resval & 0x100));
   }

   return(resGO);
}

int
cl_stm8::inst_sra(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand >> 1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      if (operand & 0x80) {      resval |= 0x80;   }
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      if (operand & 0x8000) {      resval |= 0x8000;   }
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      if (operand & 0x8000) {      resval |= 0x8000;   }
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else {
      if (operand & 0x80) {      resval |= 0x80;   }
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   }

   return(resGO);
}

int
cl_stm8::inst_srl(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      operand = regs.A;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      operand = regs.X;
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      operand = regs.Y;
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
   }

   resval = operand >> 1;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      regs.X = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      regs.Y = resval & 0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   } else {
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
      FLAG_ASSIGN (BIT_C, (operand & 0x1));
   }

   return(resGO);
}

int
cl_stm8::inst_sub(t_mem code, unsigned char prefix)
{
  FLAG_CLEAR(BIT_C);
  return inst_sbc(code, prefix);
}

int
cl_stm8::inst_swap(t_mem code, unsigned char prefix)
{
  long int operand, resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      operand = regs.A;
      resval = (operand << 4) | (operand >> 4);
      regs.A = resval&0xff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      store_regs();
      operand = regs.X;
      resval = (operand << 8) | (operand >> 8);
      regs.X = resval&0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      store_regs();
      operand = regs.Y;
      resval = (operand << 8) | (operand >> 8);
      regs.Y = resval&0xffff;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
   } else {
      opaddr = get_dest(code,prefix);
      operand = get1(opaddr);
      resval = (operand << 4) | (operand >> 4);
      store1(opaddr, resval &0xff);
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
   }

   return(resGO);
}

int
cl_stm8::inst_tnz(t_mem code, unsigned char prefix)
{
  unsigned int resval;
  unsigned int opaddr = 0;

   if (((code&0xf0)==0x40) &&(prefix == 0x00)) {
      store_regs();
      resval = regs.A;
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x00)) {
      exec_cycle();
      store_regs();
      resval = regs.X;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
   } else if (((code&0xf0)==0x50) &&(prefix == 0x90)) {
      exec_cycle();
      store_regs();
      resval = regs.Y;
      FLAG_ASSIGN (BIT_Z, (resval & 0xffff) == 0);
      FLAG_ASSIGN (BIT_N, 0x8000 & resval);
   } else {
      opaddr = get_dest(code,prefix);
      resval = get1(opaddr);
      exec_cycle();
      FLAG_ASSIGN (BIT_Z, (resval & 0xff) == 0);
      FLAG_ASSIGN (BIT_N, 0x80 & resval);
   }

   return(resGO);
}

int
cl_stm8::inst_xor(t_mem code, unsigned char prefix)
{
  int result, operand1, operand2;

  operand1 = regs.A;
  operand2 = OPERAND(code, prefix);
  result = operand1 ^ operand2;

  store_regs();

  FLAG_NZ (result);

  regs.A = result & 0xff;
  return(resGO);
}

/* End of stm8.src/inst.cc */
