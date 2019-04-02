/*
 * Simulator of microcontrollers (inst.cc)
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

#include <stdlib.h>
#include <cassert>
#include "ddconfig.h"
#include "stdio.h"

// local
#include "pdkcl.h"
#include "regspdk.h"

unsigned char cl_pdk::add_to(unsigned char initial, int value, bool carry) {
  store_flag(/*flag::*/flag_z, initial + value + carry == 0);
  store_flag(/*flag::*/flag_c, initial + value + carry > 0xFF);
  store_flag(/*flag::*/flag_ac, (initial & 0xF) + (value & 0xF) + carry > 0xF);
  store_flag(
      /*flag::*/flag_ov,
      get_flag(/*flag::*/flag_c) ^ ((initial & 0x7F) + (value & 0x7F) + carry > 0x7F));

  return initial + value + carry;
}

unsigned char cl_pdk::sub_to(unsigned char initial, int value, bool carry) {
  store_flag(/*flag::*/flag_z, initial - value - carry == 0);
  store_flag(/*flag::*/flag_c, initial < value + carry);
  store_flag(/*flag::*/flag_ac, (value & 0xF) > (initial & 0xF) - carry);
  store_flag(
      /*flag::*/flag_ov,
      get_flag(/*flag::*/flag_c) ^ ((initial & 0x7F) - (value & 0x7F) - carry < 0));

  return initial - value - carry;
}

int cl_pdk::get_mem(unsigned int addr) {
  vc.rd++;
  return ram->read((t_addr)(addr));
}

unsigned char cl_pdk::get_io(t_addr addr) {
#define WRITE_ONLY(reg) puts("can't read from write-only register " reg "!");

  switch (addr) {
  case 0x00: return regs.flag;
  case 0x02: return regs.sp;
  case 0x03: return regs.clkmd;
  case 0x04: return regs.inten;
  case 0x05: return regs.intrq;
  case 0x06: return regs.t16m;
  case 0x08: WRITE_ONLY("misc");
  case 0x09: WRITE_ONLY("tm2b");
  case 0x0A: WRITE_ONLY("eoscr");
  case 0x0C: WRITE_ONLY("integs");
  case 0x0D: WRITE_ONLY("padier");
  case 0x0E: WRITE_ONLY("pbdier");
  case 0x10: return regs.pa;
  case 0x11: return regs.pac;
  case 0x12: return regs.paph;
  case 0x14: return regs.pb;
  case 0x15: return regs.pbc;
  case 0x16: return regs.pbph;
  case 0x17: WRITE_ONLY("tm2s");
  case 0x18: return regs.gpcc;
  case 0x19: WRITE_ONLY("gpcs");
  case 0x1C: return regs.tm2c;
  case 0x1D: return regs.tm2ct;
  case 0x20: return regs.pwmg0c;
  case 0x21: WRITE_ONLY("pwmg0s");
  case 0x22: WRITE_ONLY("pwmg0dth");
  case 0x23: WRITE_ONLY("pwmg0dtl");
  case 0x24: WRITE_ONLY("pwmg0cubh");
  case 0x25: WRITE_ONLY("pwmg0cubl");
  case 0x26: return regs.pwmg1c;
  case 0x27: WRITE_ONLY("pwmg1s");
  case 0x28: WRITE_ONLY("pwmg1dth");
  case 0x29: WRITE_ONLY("pwmg1dtl");
  case 0x2A: WRITE_ONLY("pwmg1cubh");
  case 0x2B: WRITE_ONLY("pwmg1cubl");
  case 0x2C: return regs.pwmg2c;
  case 0x2D: WRITE_ONLY("pwmg2s");
  case 0x2E: WRITE_ONLY("pwmg2dth");
  case 0x2F: WRITE_ONLY("pwmg2dtl");
  case 0x30: WRITE_ONLY("pwmg2cubh");
  case 0x31: WRITE_ONLY("pwmg2cubl");
  case 0x32: return regs.tm3c;
  case 0x33: return regs.tm3ct;
  case 0x34: WRITE_ONLY("tm3s");
  case 0x35: WRITE_ONLY("tm3b");
  default:
    printf("unknown register %d\n", AI(addr));
  }

  return 0;
}

void cl_pdk::store_io(t_addr addr, unsigned char value) {
  #define RESERVE(reg, m) \
    reg &= m; \
    reg |= value & ~m

  switch (addr) {
  case 0x00: regs.flag = (regs.flag & ~0xFF) | (value & 0xFF); break;
  case 0x02:
    regs.sp = value;
    if (regs.sp > sp_max)
      sp_max = regs.sp;
    break;
  case 0x03: regs.clkmd = value; break;
  case 0x04: regs.inten = value; break;
  case 0x05: RESERVE(regs.intrq, 0x8); break;
  case 0x06: regs.t16m = value; break;
  case 0x08:
    value &= 0xC0;
    RESERVE(regs.misc, 0x8);
    break;
  case 0x09: regs.tm2b = value; break;
  case 0x0A: regs.eoscr = value & 0x1E; break;
  case 0x0C: RESERVE(regs.integs, 0xE0); break;
  case 0x0D: RESERVE(regs.padier, 0x6); break;
  case 0x0E: regs.pbdier = value; break;
  case 0x10: regs.pa = value; break;
  case 0x11: regs.pac = value; break;
  case 0x12: regs.paph = value; break;
  case 0x14: regs.pb = value; break;
  case 0x15: regs.pbc = value; break;
  case 0x16: regs.pbph = value; break;
  case 0x17: regs.tm2s = value; break;
  case 0x18: RESERVE(regs.gpcc, 0x40); break;
  case 0x19: RESERVE(regs.gpcs, 0x40); break;
  case 0x1C: regs.tm2c = value; break;
  case 0x1D: regs.tm2ct = value; break;
  case 0x20: RESERVE(regs.pwmg0c, 0x40); break;
  case 0x21: regs.pwmg0s = value; break;
  case 0x22: regs.pwmg0dth = value; break;
  case 0x23: RESERVE(regs.pwmg0dtl, 0x1F); break;
  case 0x24: regs.pwmg0cubh = value; break;
  case 0x25: RESERVE(regs.pwmg0cubl, 0x3F); break;
  case 0x26: RESERVE(regs.pwmg1c, 0x40); break;
  case 0x27: regs.pwmg1s = value; break;
  case 0x28: regs.pwmg1dth = value; break;
  case 0x29: RESERVE(regs.pwmg1dtl, 0x1F); break;
  case 0x2A: regs.pwmg1cubh = value; break;
  case 0x2B: RESERVE(regs.pwmg1cubl, 0x3F); break;
  case 0x2C: RESERVE(regs.pwmg2c, 0x40); break;
  case 0x2D: regs.pwmg2s = value; break;
  case 0x2E: regs.pwmg2dth = value; break;
  case 0x2F: RESERVE(regs.pwmg2dtl, 0x1F); break;
  case 0x30: regs.pwmg2cubh = value; break;
  case 0x31: RESERVE(regs.pwmg2cubl, 0x3F); break;
  case 0x32: regs.tm3c = value; break;
  case 0x33: regs.tm3ct = value; break;
  case 0x34: regs.tm3s = value; break;
  case 0x35: regs.tm3b = value; break;
  default:
    printf("unknown register %d\n", AI(addr));
  }
}

/*bool*/int cl_pdk::get_flag(flag n) {
  switch (n) {
  case /*flag::*/flag_z: return regs.flag & BIT_Z;
  case /*flag::*/flag_c: return (regs.flag & BIT_C) >> 1;
  case /*flag::*/flag_ac: return (regs.flag & BIT_AC) >> 2;
  case /*flag::*/flag_ov: return (regs.flag & BIT_OV) >> 3;
  default:
    assert(!"invalid bit access to FLAG");
  }
  return 0;
}

void cl_pdk::store_flag(flag n, /*bool*/int value) {
  switch (n) {
  case /*flag::*/flag_z: regs.flag = (regs.flag & ~1) | value; break;
  case /*flag::*/flag_c: regs.flag = (regs.flag & ~2) | (value << 1); break;
  case /*flag::*/flag_ac: regs.flag = (regs.flag & ~4) | (value << 2); break;
  case /*flag::*/flag_ov: regs.flag = (regs.flag & ~8) | (value << 3); break;
  default:
    assert(!"invalid bit store to FLAG");
  }
}

int cl_pdk::execute(unsigned int code) {
#define CODE_MASK(op, m) ((code & ~(m)) == (op))

  if (code == 0x0000) {
    // nop
  } else if (CODE_MASK(0x0200, 0xFF)) {
    // ret k
    regs.a = code & 0xFF;
    store_io(0x2, regs.sp - 2);
    PC = get_mem(regs.sp) | (get_mem(regs.sp + 1) << 8);
  } else if (code == 0x007A) {
    // ret
    store_io(0x2, regs.sp - 2);
    PC = get_mem(regs.sp) | (get_mem(regs.sp + 1) << 8);
  } else if (CODE_MASK(0x2F00, 0xFF)) {
    // mov a, k
    regs.a = code & 0xFF;
  } else if (CODE_MASK(0x0180, 0x3F)) {
    // mov i, a
    store_io(code & 0x3F, regs.a);
  } else if (CODE_MASK(0x01C0, 0x3F)) {
    // mov a, i
    regs.a = get_io(code & 0x3F);
  } else if (CODE_MASK(0x0B80, 0x7F)) {
    // mov m, a
    ram->write(code & 0x7F, regs.a);
  } else if (CODE_MASK(0x0F80, 0x7F)) {
    // mov a, m
    regs.a = get_mem(code & 0x7F);
  } else if (CODE_MASK(0x0301, 0x7E)) {
    // TODO: ldt16
  } else if (CODE_MASK(0x0300, 0x7E)) {
    // TODO: stt16
  } else if ((CODE_MASK(0x381, 0x7E))) {
    // idxm a, m
    regs.a = get_mem(get_mem(code & 0x7E));
  } else if ((CODE_MASK(0x380, 0x7E))) {
    // idxm m, a
    ram->write(get_mem(code & 0x7E), regs.a);
  } else if (CODE_MASK(0x1380, 0x7F)) {
    // xch m
    int mem = get_mem(code & 0x7F);
    ram->write(code & 0x7F, regs.a);
    regs.a = mem;
  } else if (code == 0x0072) {
    // pushaf
    ram->write(regs.sp, regs.a);
    ram->write(regs.sp + 1, regs.flag);
    store_io(0x2, regs.sp + 2);
  } else if (code == 0x0073) {
    // popaf
    regs.flag = get_mem(regs.sp - 1);
    regs.a = get_mem(regs.sp - 2);
    store_io(0x2, regs.sp - 2);
  } else if (CODE_MASK(0x2800, 0xFF)) {
    // add a, k
    regs.a = add_to(regs.a, code & 0xFF);
  } else if (CODE_MASK(0x0C00, 0x7F)) {
    // add a, m
    regs.a = add_to(regs.a, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0800, 0x7F)) {
    // add m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(regs.a, get_mem(addr)));
  } else if (CODE_MASK(0x2900, 0xFF)) {
    // sub a, k
    regs.a = sub_to(regs.a, code & 0xFF);
  } else if (CODE_MASK(0x0C80, 0x7F)) {
    // sub a, m
    regs.a = sub_to(regs.a, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0880, 0x7F)) {
    // sub m, a
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), regs.a));
  } else if (CODE_MASK(0x2800, 0xFF)) {
    // addc a, k
    regs.a = add_to(regs.a, code & 0xFF, get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x0D00, 0x7F)) {
    // addc a, m
    regs.a = add_to(regs.a, get_mem(code & 0x7F), get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x0800, 0x7F)) {
    // addc m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(regs.a, get_mem(addr), get_flag(/*flag::*/flag_c)));
  } else if (code == 0x0060) {
    // addc a
    regs.a = add_to(regs.a, get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x1000, 0x7F)) {
    // addc m
    int addr = code & 0x7F;
    ram->write(addr, add_to(get_mem(addr), get_flag(/*flag::*/flag_c)));
  } else if (CODE_MASK(0x2900, 0xFF)) {
    // subc a, k
    regs.a = sub_to(regs.a, code & 0xFF, get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x0D80, 0x7F)) {
    // subc a, m
    regs.a = sub_to(regs.a, get_mem(code & 0x7F), get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x0880, 0x7F)) {
    // subc m, a
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), regs.a, get_flag(/*flag::*/flag_c)));
  } else if (code == 0x0061) {
    // subc a
    regs.a = sub_to(regs.a, get_flag(/*flag::*/flag_c));
  } else if (CODE_MASK(0x1080, 0x7F)) {
    // subc m
    int addr = code & 0x7F;
    ram->write(addr, sub_to(get_mem(addr), get_flag(/*flag::*/flag_c)));
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
    store_flag(/*flag::*/flag_c, regs.a & 1);
    regs.a >>= 1;
  } else if (CODE_MASK(0x1500, 0x7F)) {
    // sr m
    int value = get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_c, value & 1);
    ram->write(code & 0x7F, value >> 1);
  } else if (code == 0x006B) {
    // sl a
    store_flag(/*flag::*/flag_c, (regs.a & 0x80) >> 7);
    regs.a <<= 1;
  } else if (CODE_MASK(0x1580, 0x7F)) {
    // sl m
    int value = get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_c, (value & 0x80) >> 7);
    ram->write(code & 0x7F, value << 1);
  } else if (code == 0x006C) {
    // src a
    int c = regs.a & 1;
    regs.a >>= 1;
    regs.a |= get_flag(/*flag::*/flag_c) << 7;
    store_flag(/*flag::*/flag_c, c);
  } else if (CODE_MASK(0x1600, 0x7F)) {
    // src m
    int value = get_mem(code & 0x7F);
    int c = value & 1;
    ram->write(code & 0x7F, (value >> 1) | (get_flag(/*flag::*/flag_c) << 7));
    store_flag(/*flag::*/flag_c, c);
  } else if (code == 0x006D) {
    // slc a
    int c = (regs.a & 0x80) >> 7;
    regs.a <<= 1;
    regs.a |= get_flag(/*flag::*/flag_c);
    store_flag(/*flag::*/flag_c, c);
  } else if (CODE_MASK(0x1680, 0x7F)) {
    // slc m
    int value = get_mem(code & 0x7F);
    int c = (value & 0x80) >> 7;
    ram->write(code & 0x7F, (value << 1) | get_flag(/*flag::*/flag_c));
    store_flag(/*flag::*/flag_c, c);
  } else if (CODE_MASK(0x2C00, 0xFF)) {
    // and a, k
    regs.a &= code & 0xFF;
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0E00, 0x7F)) {
    // and a, m
    regs.a &= get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0A00, 0x7F)) {
    // and m, a
    int store = regs.a & get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x2D00, 0xFF)) {
    // or a, k
    regs.a |= code & 0xFF;
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0E80, 0x7F)) {
    // or a, m
    regs.a |= get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0A80, 0x7F)) {
    // or m, a
    int store = regs.a | get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x2E00, 0xFF)) {
    // xor a, k
    regs.a ^= code & 0xFF;
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0F00, 0x7F)) {
    // xor a, m
    regs.a ^= get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !regs.a);
  } else if (CODE_MASK(0x0B00, 0x7F)) {
    // xor m, a
    int store = regs.a ^ get_mem(code & 0x7F);
    store_flag(/*flag::*/flag_z, !store);
    ram->write(code & 0x7F, store);
  } else if (CODE_MASK(0x00C0, 0x3F)) {
    // xor io, a
    store_io(code & 0x3F, regs.a ^ get_io(code & 0x3F));
  } else if (code == 0x0068) {
    // not a
    regs.a = ~regs.a;
  } else if (CODE_MASK(0x1400, 0x7F)) {
    // not m
    ram->write(code & 0x7F, ~get_mem(code & 0x7F));
  } else if (code == 0x0069) {
    // neg a
    regs.a = -regs.a;
  } else if (CODE_MASK(0x1480, 0x7F)) {
    // neg m
    ram->write(code & 0x7F, -get_mem(code & 0x7F));
  } else if (CODE_MASK(0x1C00, 0x1FF)) {
    // set0 io, k
    const u8_t bit = 0x1C0 >> 6;
    const u8_t addr = code & 0x3F;
    store_io(addr, get_io(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x2400, 0x1FF)) {
    // set0 m, k
    const u8_t bit = 0x1C0 >> 6;
    const u8_t addr = code & 0x3F;
    ram->write(addr, get_mem(addr) & ~(1 << bit));
  } else if (CODE_MASK(0x1E00, 0x1FF)) {
    // set1 io, k
    const u8_t bit = 0x1C0 >> 6;
    const u8_t addr = code & 0x3F;
    store_io(addr, get_io(addr) | (1 << bit));
  } else if (CODE_MASK(0x2600, 0x1FF)) {
    // set1 m, k
    const u8_t bit = 0x1C0 >> 6;
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
    sub_to(regs.a, code & 0xFF);
    if (regs.a == (code & 0xFF))
      ++PC;
  } else if (CODE_MASK(0x1700, 0x7F)) {
    // ceqsn a, m
    int addr = code & 0x7F;
    sub_to(regs.a, get_mem(addr));
    if (regs.a == get_mem(addr))
      ++PC;
  } else if (CODE_MASK(0x2B00, 0xFF)) {
    // cneqsn a, k
    sub_to(regs.a, code & 0xFF);
    if (regs.a != (code & 0xFF))
      ++PC;
  } else if (CODE_MASK(0x1780, 0x7F)) {
    // cneqsn a, m
    int addr = code & 0x7F;
    sub_to(regs.a, get_mem(addr));
    if (regs.a != get_mem(addr))
      ++PC;
  } else if (code == 0x0062) {
    // izsn
    regs.a = add_to(regs.a, 1);
    if (!regs.a)
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
    regs.a = sub_to(regs.a, 1);
    if (!regs.a)
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
    ram->write(regs.sp, PC);
    ram->write(regs.sp + 1, PC >> 8);
    PC = code & 0x7FF;
    store_io(0x2, regs.sp + 2);
  } else if (CODE_MASK(0x3000, 0x7FF)) {
    // goto k
    PC = code & 0x7FF;
  } else if (CODE_MASK(0x0600, 0x7F)) {
    // comp a, m
    sub_to(regs.a, get_mem(code & 0x7F));
  } else if (CODE_MASK(0x0680, 0x7F)) {
    // comp m, a
    sub_to(get_mem(code & 0x7F), regs.a);
  } else if (CODE_MASK(0x0700, 0x7F)) {
    // nadd a, m
    regs.a = add_to(get_mem(code & 0x7F), -regs.a);
  } else if (CODE_MASK(0x0780, 0x7F)) {
    // nadd m, a
    int addr = code & 0x7F;
    ram->write(addr, add_to(-get_mem(addr), regs.a));
  } else if (code == 0x006E) {
    // swap
    int high = regs.a & 0xF;
    regs.a = (high << 4) | (regs.a >> 4);
  } else if (code == 0x0067) {
    // pcadd
    PC += regs.a - 1;
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
    regs.a = rom->get(regs.sp) & 0xFF;
  } else if (code == 0x0007) {
    // ldspth
    regs.a = (rom->get(regs.sp) & 0xFF00) >> 8;
  } else if (code == 0xFF00) {
    // putchar - usim specific instruction
    putchar(regs.a);
    fflush(stdout);
  } else {
    return (resINV_INST);
  }
  return (resGO);
}

/* End of pdk.src/inst.cc */
