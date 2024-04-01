/*
 * Simulator of microcontrollers (pdk13.cc)
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

#include "pdk13cl.h"


cl_fppa13::cl_fppa13(int aid, class cl_pdk *the_puc, class cl_sim *asim):
  cl_fppa(aid, the_puc, asim)
{
  type= new struct cpu_entry;
  type->type= CPU_PDK13;
}

cl_fppa13::cl_fppa13(int aid, class cl_pdk *the_puc, struct cpu_entry *IType, class cl_sim *asim):
  cl_fppa(aid, the_puc, IType, asim)
{
}


int
cl_fppa13::execute(unsigned int code)
{
  int write_result = resGO;
  if (code == 0x0000) {
    // nop
  } else if (CODE_MASK(0x0100, 0xFF)) {
    // ret k
    rA = code & 0xFF;
    write_result = store_io(0x2, rSP - 2);
    if (write_result == resGO)
      PC = get_mem(rSP) | (get_mem(rSP + 1) << 8);
  } else if (code == 0x003A) {
    // ret
    write_result = store_io(0x2, rSP - 2);
    if (write_result == resGO)
      PC = get_mem(rSP) | (get_mem(rSP + 1) << 8);
  } else if (CODE_MASK(0x1700, 0xFF)) {
    // mov a, k
    rA = code & 0xFF;
  } else if (CODE_MASK(0x0080, 0x1F)) {
    // mov i, a
    write_result = store_io(code & 0x1F, rA);
  } else if (CODE_MASK(0x00A0, 0x1F)) {
    // mov a, i
    rA = get_io(code & 0x1F);
  } else if (CODE_MASK(0x05C0, 0x3F)) {
    // mov m, a
    ram->write(code & 0x3F, rA);
  } else if (CODE_MASK(0x07C0, 0x3F)) {
    // mov a, m
    rA = get_mem(code & 0x3F);
  } else if (CODE_MASK(0x00C1, 0x1E)) {
    // TODO: ldt16
  } else if (CODE_MASK(0x00C0, 0x1E)) {
    // TODO: stt16
  } else if ((CODE_MASK(0x0E1, 0x1E))) {
    // idxm a, m
    rA = get_mem(get_mem(code & 0x1E));
  } else if ((CODE_MASK(0x0E0, 0x1E))) {
    // idxm m, a
    ram->write(get_mem(code & 0x1E), rA);
  } else if (CODE_MASK(0x09C0, 0x3F)) {
    // xch m
    int mem = get_mem(code & 0x3F);
    ram->write(code & 0x3F, rA);
    rA = mem;
  } else if (code == 0x0032) {
    // pushaf
    ram->write(rSP, rA);
    ram->write(rSP + 1, rF);
    write_result = store_io(0x2, rSP + 2);
  } else if (code == 0x0033) {
    // popaf
    cF->W(get_mem(rSP - 1));
    rA = get_mem(rSP - 2);
    write_result = store_io(0x2, rSP - 2);
  } else if (CODE_MASK(0x1000, 0xFF)) {
    // add a, k
    rA = add_to(rA, code & 0xFF);
  } else if (CODE_MASK(0x0600, 0x3F)) {
    // add a, m
    rA = add_to(rA, get_mem(code & 0x3F));
  } else if (CODE_MASK(0x0400, 0x3F)) {
    // add m, a
    int addr = code & 0x3F;
    ram->write(addr, add_to(rA, get_mem(addr)));
  } else if (CODE_MASK(0x1100, 0xFF)) {
    // sub a, k
    rA = sub_to(rA, code & 0xFF);
  } else if (CODE_MASK(0x0640, 0x3F)) {
    // sub a, m
    rA = sub_to(rA, get_mem(code & 0x3F));
  } else if (CODE_MASK(0x0440, 0x3F)) {
    // sub m, a
    int addr = code & 0x3F;
    ram->write(addr, sub_to(get_mem(addr), rA));
  } else if (CODE_MASK(0x0680, 0x3F)) {
    // addc a, m
    rA = add_to(rA, get_mem(code & 0x3F), fC);
  } else if (CODE_MASK(0x0480, 0x3F)) {
    // addc m, a
    int addr = code & 0x3F;
    ram->write(addr, add_to(rA, get_mem(addr), fC));
  } else if (code == 0x0010) {
    // addc a
    rA = add_to(rA, fC);
  } else if (CODE_MASK(0x0800, 0x3F)) {
    // addc m
    int addr = code & 0x3F;
    ram->write(addr, add_to(get_mem(addr), fC));
  } else if (CODE_MASK(0x06C0, 0x3F)) {
    // subc a, m
    rA = sub_to(rA, get_mem(code & 0x3F), fC);
  } else if (CODE_MASK(0x04C0, 0x3F)) {
    // subc m, a
    int addr = code & 0x3F;
    ram->write(addr, sub_to(get_mem(addr), rA, fC));
  } else if (code == 0x0011) {
    // subc a
    rA = sub_to(rA, fC);
  } else if (CODE_MASK(0x0840, 0x3F)) {
    // subc m
    int addr = code & 0x3F;
    ram->write(addr, sub_to(get_mem(addr), fC));
  } else if (CODE_MASK(0x0900, 0x3F)) {
    // inc m
    int addr = code & 0x3F;
    ram->write(addr, add_to(get_mem(addr), 1));
  } else if (CODE_MASK(0x0940, 0x3F)) {
    // dec m
    int addr = code & 0x3F;
    ram->write(addr, sub_to(get_mem(addr), 1));
  } else if (CODE_MASK(0x0980, 0x3F)) {
    // clear m
    ram->write(code & 0x3F, 0);
  } else if (code == 0x001A) {
    // sr a
    store_flag(flag_c, rA & 1);
    rA >>= 1;
  } else if (CODE_MASK(0x0A80, 0x3F)) {
    // sr m
    int value = get_mem(code & 0x3F);
    store_flag(flag_c, value & 1);
    ram->write(code & 0x3F, value >> 1);
  } else if (code == 0x001B) {
    // sl a
    store_flag(flag_c, (rA & 0x80) >> 7);
    rA <<= 1;
  } else if (CODE_MASK(0x0AC0, 0x3F)) {
    // sl m
    int value = get_mem(code & 0x3F);
    store_flag(flag_c, (value & 0x80) >> 7);
    ram->write(code & 0x3F, value << 1);
  } else if (code == 0x001C) {
    // src a
    int c = rA & 1;
    rA >>= 1;
    rA |= fC << 7;
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x0B00, 0x3F)) {
    // src m
    int value = get_mem(code & 0x3F);
    int c = value & 1;
    ram->write(code & 0x3F, (value >> 1) | (fC << 7));
    store_flag(flag_c, c);
  } else if (code == 0x001D) {
    // slc a
    int c = (rA & 0x80) >> 7;
    rA <<= 1;
    rA |= fC;
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x0B40, 0x3F)) {
    // slc m
    int value = get_mem(code & 0x3F);
    int c = (value & 0x80) >> 7;
    ram->write(code & 0x3F, (value << 1) | fC);
    store_flag(flag_c, c);
  } else if (CODE_MASK(0x1400, 0xFF)) {
    // and a, k
    rA &= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0700, 0x3F)) {
    // and a, m
    rA &= get_mem(code & 0x3F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0500, 0x3F)) {
    // and m, a
    int store = rA & get_mem(code & 0x3F);
    store_flag(flag_z, !store);
    ram->write(code & 0x3F, store);
  } else if (CODE_MASK(0x1500, 0xFF)) {
    // or a, k
    rA |= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0740, 0x3F)) {
    // or a, m
    rA |= get_mem(code & 0x3F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0540, 0x3F)) {
    // or m, a
    int store = rA | get_mem(code & 0x3F);
    store_flag(flag_z, !store);
    ram->write(code & 0x3F, store);
  } else if (CODE_MASK(0x1600, 0xFF)) {
    // xor a, k
    rA ^= code & 0xFF;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0780, 0x3F)) {
    // xor a, m
    rA ^= get_mem(code & 0x3F);
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0580, 0x3F)) {
    // xor m, a
    int store = rA ^ get_mem(code & 0x3F);
    store_flag(flag_z, !store);
    ram->write(code & 0x3F, store);
  } else if (CODE_MASK(0x0060, 0x1F)) {
    // xor io, a
    write_result = store_io(code & 0x1F, rA ^ get_io(code & 0x1F));
  } else if (code == 0x0018) {
    // not a
    rA = ~rA;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0A00, 0x3F)) {
    // not m
    int store = (~get_mem(code & 0x3F) & 0xff);
    store_flag(flag_z, !store);
    ram->write(code & 0x3F, store);
  } else if (code == 0x0019) {
    // neg a
    rA = -rA;
    store_flag(flag_z, !rA);
  } else if (CODE_MASK(0x0A40, 0x3F)) {
    // neg m
    int store = (-get_mem(code & 0x3F) & 0xff);
    store_flag(flag_z, !store);
    ram->write(code & 0x3F, store);
  } else if (CODE_MASK(0x0E00, 0xFF)) {
    // set0 io, k
    const u8_t bit = (code & 0xE0) >> 5;
    const u8_t addr = code & 0x1F;
    write_result = store_io(addr, get_io(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x0300, 0xEF)) {
    // set0 m, k
    const u8_t bit = (code & 0xE0) >> 5;
    const u8_t addr = code & 0x0F;
    ram->write(addr, get_mem(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x0F00, 0xFF)) {
    // set1 io, k
    const u8_t bit = (code & 0xE0) >> 5;
    const u8_t addr = code & 0x1F;
    write_result = store_io(addr, get_io(addr) | (1 << bit));
  } else if (CODE_MASK(0x0310, 0xEF)) {
    // set1 m, k
    const u8_t bit = (code & 0xE0) >> 5;
    const u8_t addr = code & 0x0F;
    ram->write(addr, get_mem(addr) | (1 << bit));
  } else if (CODE_MASK(0x0C00, 0xFF)) {
    // t0sn io, k
    int n = (code & 0xE0) >> 5;
    if (!(get_io(code & 0x1F) & (1 << n)))
      ++PC;
  } else if (CODE_MASK(0x0200, 0xEF)) {
    // t0sn m, k
    int n = (code & 0xE0) >> 5;
    if (!(get_mem(code & 0x0F) & (1 << n)))
      ++PC;
  } else if (CODE_MASK(0x0D00, 0xFF)) {
    // t1sn io, k
    int n = (code & 0xE0) >> 5;
    if (get_io(code & 0x1F) & (1 << n))
      ++PC;
  } else if (CODE_MASK(0x0210, 0xEF)) {
    // t1sn m, k
    int n = (code & 0xE0) >> 5;
    if (get_mem(code & 0x0F) & (1 << n))
      ++PC;
  } else if (CODE_MASK(0x1200, 0xFF)) {
    // ceqsn a, k
    sub_to(rA, code & 0xFF);
    if (rA == (code & 0xFF))
      ++PC;
  } else if (CODE_MASK(0x0B80, 0x3F)) {
    // ceqsn a, m
    int addr = code & 0x3F;
    sub_to(rA, get_mem(addr));
    if (rA == get_mem(addr))
      ++PC;
  } else if (code == 0x0012) {
    // izsn
    rA = add_to(rA, 1);
    if (!rA)
      ++PC;
  } else if (CODE_MASK(0x0880, 0x3F)) {
    // izsn m
    const int addr = code & 0x3F;
    int result = add_to(get_mem(addr), 1);
    ram->write(addr, result);
    if (!result)
      ++PC;
  } else if (code == 0x0013) {
    // dzsn
    rA = sub_to(rA, 1);
    if (!rA)
      ++PC;
  } else if (CODE_MASK(0x08C0, 0x3F)) {
    // dzsn m
    const int addr = code & 0x3F;
    int result = sub_to(get_mem(addr), 1);
    ram->write(addr, result);
    if (!result)
      ++PC;
  } else if (CODE_MASK(0x1C00, 0x3FF)) {
    // call k
    ram->write(rSP, PC);
    ram->write(rSP + 1, PC >> 8);
    PC = code & 0x3FF;
    write_result = store_io(0x2, rSP + 2);
  } else if (CODE_MASK(0x1800, 0x3FF)) {
    // goto k
    PC = code & 0x3FF;
  } else if (code == 0x001E) {
    // swap
    int high = rA & 0xF;
    rA = (high << 4) | (rA >> 4);
  } else if (code == 0x0017) {
    // pcadd
    PC += rA - 1;
  }
  // TODO: engint
  // TODO: disint
  else if (code == 0x0036) {
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
    // ldregs[0x02]th
    rA = (rom->get(rSP) & 0xFF00) >> 8;
  } else if (code == 0x003C) {
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


/* End of pdk.src/pdk13.cc */
