/*
 * Simulated instructions specific to the GB80, the Z-80 derivative used
 * in the gameboy.
 *
 * 2011-12-21  created by Leland Morrison
 *
 *

This file is part of microcontroller simulator: ucsim.

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

#include "gb80cl.h"

static u8_t  swap_nibbles(u8_t  val) {
  return ((val >> 4) & 0x0f) | ((val << 4) & 0xf0);
}

int cl_gb80::inst_cb(void) {
  u8_t  result;
  t_mem       code;
  
  if ( (peek1( ) & 0xf8) != 0x30 ) {
    // (hl) has to be rounded, but not for bit
    if ((peek1( ) & 0x7) == 0x6)
      if (peek1( ) < 0x50 || peek1( ) >= 0x80)
        tick(1);
    return cl_z80::inst_cb( );
  }
  
  code = fetch1();
  tick(7);
  
  /* perform SWAP instead of slia */
  switch(code) {
  case 0x30: result = regs.bc.h = swap_nibbles(regs.bc.h); break; /* b */
  case 0x31: result = regs.bc.l = swap_nibbles(regs.bc.l); break; /* c */
  case 0x32: result = regs.de.h = swap_nibbles(regs.de.h); break; /* d */
  case 0x33: result = regs.de.l = swap_nibbles(regs.de.l); break; /* e */
  case 0x34: result = regs.hl.h = swap_nibbles(regs.hl.h); break; /* h */
  case 0x35: result = regs.hl.l = swap_nibbles(regs.hl.l); break; /* l */
  case 0x36: /* SWAP (HL) */
    {
      tick(8);
      result = swap_nibbles(get1(regs.HL));
      store1(regs.HL, result);
      vc.rd++;
      vc.wr++;
    }
    break;
    
  case 0x37: result = regs.raf.A = swap_nibbles(regs.raf.A); break; /* swap a */
  default: return resINV_INST;
  }
  regs.raf.F = (result)?0:BIT_Z;  // all except zero are simply cleared
  return(resGO);
}

int cl_gb80::inst_st_sp_abs(t_mem code) {
  if (code == 0x08) {
    u16_t addr = fetch2( );
    store2( addr, regs.SP );
    vc.wr+= 2;
    tick(19);
    return(resGO);
  }
  
  return resINV_INST;
}

int cl_gb80::inst_stop0    (t_mem code) {
  // TODO: change to wait for a signal for simulated hardware
  tick(3);//can be 7 sometimes
  return resHALT;
}

int cl_gb80::inst_ldi   (t_mem code) {
  if (code == 0x22) {
    store1( regs.HL, regs.raf.A );
    regs.HL ++;
    vc.wr++;
    tick(7);
    return resGO;
  } else if (code == 0x2A) {
    regs.raf.A = get1( regs.HL );
    regs.HL ++;
    vc.rd++;
    tick(7);
    return resGO;
  }
  
  return resINV_INST;
}

int cl_gb80::inst_ldd   (t_mem code) {
  if (code == 0x32) {
    store1( regs.HL, regs.raf.A );
    regs.HL --;
    vc.wr++;
    tick(7);
    return resGO;
  } else if (code == 0x3A) {
    regs.raf.A = get1( regs.HL );
    regs.HL --;
    vc.rd++;
    tick(7);
    return resGO;
  }
  
  return resINV_INST;
}

int cl_gb80::inst_ldh   (t_mem code) {
  u16_t addr = 0xFF00;

  if (code & 0x2) {
    addr += regs.bc.l;
  } else {
    addr += fetch1( );
    tick(4);
  }
  
  if ((code & 0xFD) == 0xE0) {
    store1( addr, regs.raf.A );
    vc.wr++;
    tick(7);
    return resGO;
  } else if ((code & 0xFD) == 0xF0) {
    regs.raf.A = get1( addr );
    vc.rd++;
    tick(7);
    return resGO;
  }
  
  return resINV_INST;
}
  
int cl_gb80::inst_reti  (t_mem code) {
  /* enable interrupts */
  cl_z80::inst_ei(0xFB);
  tick(12);
  
  /* pop2(PC); */
  PC=get2(regs.SP);
  regs.SP+=2;
  vc.rd+= 2;
  
  return resGO;
}

int cl_gb80::inst_add_sp_d(t_mem code) {
  u16_t  d = fetch( );
  /* sign-extend d from 8-bits to 16-bits */
  d |= (d>>7)*0xFF00;
  
  regs.raf.F &= ~(BIT_ALL);  /* clear these */
  if ((regs.SP & 0x0FFF) + (d & 0x0FFF) > 0x0FFF)
    regs.raf.F |= BIT_A;
  if (regs.SP + (int)(d) > 0xffff)
    regs.raf.F |= BIT_C;
  
  regs.SP = (regs.SP + d) & 0xffff;

  tick(15);
  return(resGO);
}

int cl_gb80::inst_ld16  (t_mem code) {
  u16_t addr = fetch2( );
  if (code == 0xEA) {
    store1( addr, regs.raf.A );
    vc.wr++;
    tick(15);
    return resGO;
  } else if (code == 0xFA) {
    regs.raf.A = get1( addr );
    vc.rd++;
    tick(15);
    return resGO;
  }
  
  return resINV_INST;
}

int cl_gb80::inst_ldhl_sp (t_mem code) {
  u16_t  d = fetch( );
  /* sign-extend d from 8-bits to 16-bits */
  d |= (d>>7)*0xFF00;

  regs.raf.F &= ~(BIT_ALL);  /* clear these */
  if ((regs.SP & 0x0FFF) + (d & 0x0FFF) > 0x0FFF)
    regs.raf.F |= BIT_A;
  if (regs.SP + (int)(d) > 0xffff)
    regs.raf.F |= BIT_C;
  
  regs.HL = (regs.SP + d) & 0xffff;
  tick(11);
  return resGO;
}

/* End of z80.src/inst_gb80.cc */
