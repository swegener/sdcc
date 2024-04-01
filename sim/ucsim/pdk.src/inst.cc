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


unsigned char cl_fppa::add_to(unsigned char initial, int value, bool carry) {
  store_flag(flag_z, initial + value + carry == 0);
  store_flag(flag_c, initial + value + carry > 0xFF);
  store_flag(flag_ac, (initial & 0xF) + (value & 0xF) + carry > 0xF);
  store_flag(
      flag_ov,
      fC ^ ((initial & 0x7F) + (value & 0x7F) + carry > 0x7F));

  return initial + value + carry;
}

unsigned char cl_fppa::sub_to(unsigned char initial, int value, bool carry) {
  store_flag(flag_z, initial - value - carry == 0);
  store_flag(flag_c, initial < value + carry);
  store_flag(flag_ac, (value & 0xF) > (initial & 0xF) - carry);
  store_flag(
      flag_ov,
      fC ^ ((initial & 0x7F) - (value & 0x7F) - carry < 0));

  return initial - value - carry;
}

int cl_fppa::get_mem(unsigned int addr) {
  vc.rd++;
  return ram->read((t_addr)(addr));
}

unsigned char cl_fppa::get_io(t_addr addr) {
  return sfr->read(addr);
}

int cl_fppa::store_io(t_addr addr, int value) {
  
  sfr->write(addr, value & 0xFF);
  if (addr == 0x02)
    {
      if (rSP > sp_most)
        sp_most = rSP;
      if (!ram->valid_address(value))
        return resSTACK_OV;
    }
  return resGO;
}

void cl_fppa::store_flag(flag n, int value) {
  value= value?1:0;
  switch (n) {
  case flag_z: cF->W((rF & ~1) | (value << BITPOS_Z)); break;
  case flag_c: cF->W((rF & ~2) | (value << BITPOS_C)); break;
  case flag_ac: cF->W((rF & ~4) | (value << BITPOS_AC)); break;
  case flag_ov: cF->W((rF & ~8) | (value << BITPOS_OV)); break;
  default:
    assert(!"invalid bit store to FLAG");
  }
}


/* End of pdk.src/inst.cc */

