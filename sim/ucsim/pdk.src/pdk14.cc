/*
 * Simulator of microcontrollers (pdk14.cc)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
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

#include "pdk14cl.h"


cl_fppa14::cl_fppa14(int aid, class cl_pdk *the_puc, class cl_sim *asim):
  cl_fppa13(aid, the_puc, asim)
{
  type= new struct cpu_entry;
  type->type= CPU_PDK14;
}

cl_fppa14::cl_fppa14(int aid, class cl_pdk *the_puc, struct cpu_entry *IType, class cl_sim *asim):
  cl_fppa13(aid, the_puc, IType, asim)
{
}


int
cl_fppa14::execute(unsigned int code)
{
  int write_result = resGO;
  if (code == 0x0000) {
    // nop
  } else if (CODE_MASK(0x0200, 0xFF)) {
    // ret k
    rA = code & 0xFF;
    write_result = store_io(0x2, rSP - 2);
    if (write_result == resGO)
      PC = get_mem(rSP) | (get_mem(rSP + 1) << 8);
  } else if (code == 0x007A) {
    // ret
    write_result = store_io(0x2, rSP - 2);
    if (write_result == resGO)
      PC = get_mem(rSP) | (get_mem(rSP + 1) << 8);
  } else if (CODE_MASK(0x2F00, 0xFF)) {
    // mov a, k
    rA = code & 0xFF;
  } else if (CODE_MASK(0x0180, 0x3F)) {
    // mov i, a
    write_result = store_io(code & 0x3F, rA);
  } else if (CODE_MASK(0x01C0, 0x3F)) {
    // mov a, i
    rA = get_io(code & 0x3F);
  } else if (CODE_MASK(0x0B80, 0x7F)) {
    // mov m, a
    ram->write(code & 0x7F, rA);
  } else if (CODE_MASK(0x0F80, 0x7F)) {
    // mov a, m
    rA = get_mem(code & 0x7F);
  } else if (CODE_MASK(0x0301, 0x7E)) {
    // TODO: ldt16
  } else if (CODE_MASK(0x0300, 0x7E)) {
    // TODO: stt16
  } else if ((CODE_MASK(0x381, 0x7E))) {
    // idxm a, m
    rA = get_mem(get_mem(code & 0x7E));
  } else if ((CODE_MASK(0x380, 0x7E))) {
    // idxm m, a
    ram->write(get_mem(code & 0x7E), rA);
  } else if (CODE_MASK(0x1380, 0x7F)) {
    // xch m
    int mem = get_mem(code & 0x7F);
    ram->write(code & 0x7F, rA);
    rA = mem;
  } else if (code == 0x0072) {
    // pushaf
    ram->write(rSP, rA);
    ram->write(rSP + 1, rF);
    write_result = store_io(0x2, rSP + 2);
  } else if (code == 0x0073) {
    // popaf
    cF->W(get_mem(rSP - 1));
    rA = get_mem(rSP - 2);
    write_result = store_io(0x2, rSP - 2);
  } else if (CODE_MASK(0x2800, 0xFF)) {
    // add a, k
    rA = add_to(rA, code & 0xFF);
  } else if (CODE_MASK(0x0C00, 0x7F)) {
    // add a, m
    rA = add_to(rA, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0800, 0x7F)) {
    // add m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(rA, get_mem(addr)));
  } else if (CODE_MASK(0x2900, 0xFF)) {
    // sub a, k
    rA = sub_to(rA, code & 0xFF);
  } else if (CODE_MASK(0x0C80, 0x7F)) {
    // sub a, m
    rA = sub_to(rA, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0880, 0x7F)) {
    // sub m, a
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), rA));
  } else if (CODE_MASK(0x0D00, 0x7F)) {
    // addc a, m
    rA = add_to(rA, get_mem(code & 0x7F), fC);
  } else if (CODE_MASK(0x0900, 0x7F)) {
    // addc m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(rA, get_mem(addr), fC));
  } else if (code == 0x0060) {
    // addc a
    rA = add_to(rA, fC);
  } else if (CODE_MASK(0x1000, 0x7F)) {
    // addc m
    int addr = code & 0x7F;
    ram->write(addr, add_to(get_mem(addr), fC));
  } else if (CODE_MASK(0x0D80, 0x7F)) {
    // subc a, m
    rA = sub_to(rA, get_mem(code & 0x7F), fC);
  } else if (CODE_MASK(0x0980, 0x7F)) {
    // subc m, a
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), rA, fC));
  } else if (code == 0x0061) {
    // subc a
    rA = sub_to(rA, fC);
  } else if (CODE_MASK(0x1080, 0x7F)) {
    // subc m
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), fC));
  } else if (CODE_MASK(0x1200, 0x7F)) {
    // inc m
    int addr = code & 0x7F;
    ram->write(addr, add_to(get_mem(addr), 1));
  } else if (CODE_MASK(0x1280, 0x7F)) {
    // dec m
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), 1));
  } else if (CODE_MASK(0x1300, 0x7F)) {
    // clear m
    ram->write(code & 0x7F, 0);
  } else if (code == 0x006A) {
    // sr a
    store_flag(flag_c, rA & 1);
    rA >>= 1;
  } else if (CODE_MASK(0x1500, 0x7F)) {
    // sr m
    int value = get_mem(code & 0x7F);
    store_flag(flag_c, value & 1);
    ram->write(code & 0x7F, value >> 1);
  } else if (code == 0x006B) {
    // sl a
    store_flag(flag_c, (rA & 0x80) >> 7);
    rA <<= 1;
  } else if (CODE_MASK(0x1580, 0x7F)) {
    // sl m
    int value = get_mem(code & 0x7F);
    store_flag(flag_c, (value & 0x80) >> 7);
    ram->write(code & 0x7F, value << 1);
  } else if (code == 0x006C) {
    // src a
    int c = rA & 1;
    rA >>= 1;
    rA |= fC << 7;
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x1600, 0x7F)) {
    // src m
    int value = get_mem(code & 0x7F);
    int c = value & 1;
    ram->write(code & 0x7F, (value >> 1) | (fC << 7));
    store_flag(flag_c, c);
  } else if (code == 0x006D) {
    // slc a
    int c = (rA & 0x80) >> 7;
    rA <<= 1;
    rA |= fC;
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x1680, 0x7F)) {
    // slc m
    int value = get_mem(code & 0x7F);
    int c = (value & 0x80) >> 7;
    ram->write(code & 0x7F, (value << 1) | fC);
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x2C00, 0xFF)) {
    // and a, k
    rA &= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0E00, 0x7F)) {
    // and a, m
    rA &= get_mem(code & 0x7F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0A00, 0x7F)) {
    // and m, a
    int store = rA & get_mem(code & 0x7F);
    store_flag(flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x2D00, 0xFF)) {
    // or a, k
    rA |= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0E80, 0x7F)) {
    // or a, m
    rA |= get_mem(code & 0x7F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0A80, 0x7F)) {
    // or m, a
    int store = rA | get_mem(code & 0x7F);
    store_flag(flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x2E00, 0xFF)) {
    // xor a, k
    rA ^= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0F00, 0x7F)) {
    // xor a, m
    rA ^= get_mem(code & 0x7F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0B00, 0x7F)) {
    // xor m, a
    int store = rA ^ get_mem(code & 0x7F);
    store_flag(flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x00C0, 0x3F)) {
    // xor io, a
    write_result = store_io(code & 0x3F, rA ^ get_io(code & 0x3F));
  } else if (code == 0x0068) {
    // not a
    rA = ~rA;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x1400, 0x7F)) {
    // not m
    int store = (~get_mem(code & 0x7F) & 0xff);
    store_flag(flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (code == 0x0069) {
    // neg a
    rA = -rA;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x1480, 0x7F)) {
    // neg m
    int store = (-get_mem(code & 0x7F) & 0xff);
    store_flag(flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x1C00, 0x1FF)) {
    // set0 io, k
    const u8_t bit = (code & 0x1C0) >> 6;
    const u8_t addr = code & 0x3F;
    write_result = store_io(addr, get_io(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x2400, 0x1FF)) {
    // set0 m, k
    const u8_t bit = (code & 0x1C0) >> 6;
    const u8_t addr = code & 0x3F;
    ram->write(addr, get_mem(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x1E00, 0x1FF)) {
    // set1 io, k
    const u8_t bit = (code & 0x1C0) >> 6;
    const u8_t addr = code & 0x3F;
    write_result = store_io(addr, get_io(addr) | (1 << bit));
  } else if (CODE_MASK(0x2600, 0x1FF)) {
    // set1 m, k
    const u8_t bit = (code & 0x1C0) >> 6;
    const u8_t addr = code & 0x3F;
    ram->write(addr, get_mem(addr) | (1 << bit));
  } else if (CODE_MASK(0x1800, 0x1FF)) {
    // t0sn io, k
    int n = (code & 0x1C0) >> 6;
    if (!(get_io(code & 0x3F) & (1 << n)))
      ++PC;
  } else if (CODE_MASK(0x2000, 0x1FF)) {
    // t0sn m, k
    int n = (code & 0x1C0) >> 6;
    if (!(get_mem(code & 0x3F) & (1 << n)))
      ++PC;
  } else if (CODE_MASK(0x1A00, 0x1FF)) {
    // t1sn io, k
    int n = (code & 0x1C0) >> 6;
    if (get_io(code & 0x3F) & (1 << n))
      ++PC;
  } else if (CODE_MASK(0x2200, 0x1FF)) {
    // t1sn m, k
    int n = (code & 0x1C0) >> 6;
    if (get_mem(code & 0x3F) & (1 << n))
      ++PC;
  } else if (CODE_MASK(0x2A00, 0xFF)) {
    // ceqsn a, k
    sub_to(rA, code & 0xFF);
    if (rA == (code & 0xFF))
      ++PC;
  } else if (CODE_MASK(0x1700, 0x7F)) {
    // ceqsn a, m
    int addr = code & 0x7F;
    sub_to(rA, get_mem(addr));
    if (rA == get_mem(addr))
      ++PC;
  } else if (CODE_MASK(0x2B00, 0xFF)) {
    // cneqsn a, k
    sub_to(rA, code & 0xFF);
    if (rA != (code & 0xFF))
      ++PC;
  } else if (CODE_MASK(0x1780, 0x7F)) {
    // cneqsn a, m
    int addr = code & 0x7F;
    sub_to(rA, get_mem(addr));
    if (rA != get_mem(addr))
      ++PC;
  } else if (code == 0x0062) {
    // izsn
    rA = add_to(rA, 1);
    if (!rA)
      ++PC;
  } else if (CODE_MASK(0x1100, 0x7F)) {
    // izsn m
    const int addr = code & 0x7F;
    int result = add_to(get_mem(addr), 1);
    ram->write(addr, result);
    if (!result)
      ++PC;
  } else if (code == 0x0063) {
    // dzsn
    rA = sub_to(rA, 1);
    if (!rA)
      ++PC;
  } else if (CODE_MASK(0x1180, 0x7F)) {
    // dzsn m
    const int addr = code & 0x7F;
    int result = sub_to(get_mem(addr), 1);
    ram->write(addr, result);
    if (!result)
      ++PC;
  } else if (CODE_MASK(0x3800, 0x7FF)) {
    // call k
    ram->write(rSP, PC);
    ram->write(rSP + 1, PC >> 8);
    PC = code & 0x7FF;
    write_result = store_io(0x2, rSP + 2);
  } else if (CODE_MASK(0x3000, 0x7FF)) {
    // goto k
    PC = code & 0x7FF;
  } else if (CODE_MASK(0x0600, 0x7F)) {
    // comp a, m
    sub_to(rA, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0680, 0x7F)) {
    // comp m, a
    sub_to(get_mem(code & 0x7F), rA);
  } else if (CODE_MASK(0x0700, 0x7F)) {
    // nadd a, m
    rA = add_to(get_mem(code & 0x7F), -rA);
  } else if (CODE_MASK(0x0780, 0x7F)) {
    // nadd m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(-get_mem(addr), rA));
  } else if (code == 0x006E) {
    // swap
    int high = rA & 0xF;
    rA = (high << 4) | (rA >> 4);
  } else if (code == 0x0067) {
    // pcadd
    PC += rA - 1;
  }
  // TODO: engint
  // TODO: disint
  else if (code == 0x0076) {
    // stopsys
    return (resHALT);
  }
  // TODO: stopexe
  // TODO: reset
  // TODO: wdreset
  // TODO: swapc IO, k
  else if (code == 0x0006) {
    // ldsptl
    rA = rom->get(rSP) & 0xFF;
  } else if (code == 0x0007) {
    // ldspth
    rA = (rom->get(rSP) & 0xFF00) >> 8;
  } else if (code == 0x007C) {
    // mul
    unsigned result = rA * get_io(0x08);
    rA = result & 0xFF;
    write_result = store_io(0x08, (result & 0xFF00) >> 8);
  } else if (code == 0xFF00) {
    // putchar - usim specific instruction
    putchar(rA);
    fflush(stdout);
  } else {
    return (resINV_INST);
  }
  return (write_result);
}


/* End of pdk.src/pdk14.cc */
