/*
 * Simulator of microcontrollers (z80mac.h)
 *
 * some z80 code base from Karl Bongers karl@turbobit.com
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

// shift positions
#define BITPOS_C 0  // 1
#define BITPOS_P 2  // 4H
#define BITPOS_A 4  // 10H
#define BITPOS_Z 6  // 40H
#define BITPOS_S 7  // 80H

#define store2(addr, val) { ram->set((t_addr) (addr), val & 0xff); \
                            ram->set((t_addr) (addr+1), (val >> 8) & 0xff); }
#define store1(addr, val) ram->set((t_addr) (addr), val)
#define get1(addr) ram->get((t_addr) (addr))
#define get2(addr) (ram->get((t_addr) (addr)) | (ram->get((t_addr) (addr+1)) << 8) )
#define fetch2() (fetch() | (fetch() << 8))
#define fetch1() fetch()
#define push2(val) {regs.SP-=2; store2(regs.SP,(val));}
#define push1(val) {regs.SP-=1; store1(regs.SP,(val));}
#define pop2(var) {var=get2(regs.SP),regs.SP+=2;}
//#define pop1(var) {var=get1(regs.SP),regs.SP+=1;}
#define add_u16_disp(_w, _d) (( (unsigned short)(_w) + (char)(_d) ) & 0xffff)

#define sub_A_bytereg(br) { \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      if (regs.A < br) regs.F |= (BIT_C | BIT_P); \
      regs.A -= (br); \
      regs.F |= BIT_N; /* not addition */ \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
      /* Skip BIT_A for now */ \
  }


#define rr_byte(reg) { \
   if (regs.F & BIT_C) {  \
     regs.F &= ~(BIT_ALL);  /* clear these */ \
     if (reg & 0x01)   \
       regs.F |= BIT_C;   \
     reg = (reg >> 1) | 0x80; \
   } else { \
     regs.F &= ~(BIT_ALL);  /* clear these */ \
     if (reg & 0x01)   \
       regs.F |= BIT_C;   \
     reg >>= 1; \
   } \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define rl_byte(reg) { \
   if (regs.F & BIT_C) {  \
     regs.F &= ~(BIT_ALL);  /* clear these */ \
     if (reg & 0x80)   \
       regs.F |= BIT_C;   \
     reg = (reg << 1) | 0x01; \
   } else { \
     regs.F &= ~(BIT_ALL);  /* clear these */ \
     if (reg & 0x80)   \
       regs.F |= BIT_C;   \
     reg = (reg << 1); \
   } \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define rrc_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x01) { \
     regs.F |= BIT_C;   \
     reg = (reg >> 1) | 0x80; \
   } \
   else \
     reg = (reg >> 1); \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define rlc_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x80) {  \
     regs.F |= BIT_C;   \
     reg = (reg << 1) | 0x01; \
   } else \
     reg <<= 1; \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define sla_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x80)      \
     regs.F |= BIT_C;   \
   reg <<= 1; \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define sra_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x80) {  \
     if (reg & 0x01)  \
       regs.F |= BIT_C;   \
     reg = (reg >> 1) | 0x80; \
   } else { \
     if (reg & 0x01)  \
       regs.F |= BIT_C;   \
     reg >>= 1; \
   } \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define srl_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x01)  \
     regs.F |= BIT_C;   \
   reg >>= 1; \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

/* following not in my book, best guess based on z80.txt comments */
#define slia_byte(reg) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   if (reg & 0x80)      \
     regs.F |= BIT_C;   \
   reg = (reg << 1) | 1; \
   if (reg == 0) regs.F |= BIT_Z; \
   if (reg & 0x80) regs.F |= BIT_S; \
   /* fixme: BIT_P(lookup table?) */ \
}

#define bit_byte(reg, _bitnum) { \
   regs.F &= ~(BIT_ALL);  /* clear these */ \
   regs.F |= BIT_A; \
   if (!(reg & (1 << (_bitnum))))  \
     regs.F |= BIT_Z; \
   /* book shows BIT_S & BIT_P as unknown state */ \
}

#define add_A_bytereg(br) { \
    unsigned int tmp; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      tmp = (unsigned short)regs.A + (unsigned short)(br); \
      regs.A = (unsigned short) tmp; \
      /* C Z S A */ \
      if (tmp > 0xff) regs.F |= (BIT_C | BIT_P); \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
      /* Skip BIT_A for now */ \
  }

#define adc_A_bytereg(br) { \
    unsigned int tmp; \
      tmp = (unsigned short)regs.A + (unsigned short)(br); \
      if (regs.F & BIT_C) ++tmp; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      regs.A = (unsigned char) tmp; \
      if (tmp > 0xff) regs.F |= (BIT_C | BIT_P); \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
      /* Skip BIT_A for now */ \
  }

#define adc_HL_wordreg(reg) { \
    unsigned int tmp; \
      tmp = (unsigned int)regs.HL + (unsigned int)(reg); \
      if (regs.F & BIT_C) ++tmp; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      regs.HL = (unsigned short) tmp; \
      if (tmp > 0xffff) regs.F |= (BIT_C | BIT_P); \
      if (regs.HL == 0) regs.F |= BIT_Z; \
      if (regs.HL & 0x8000) regs.F |= BIT_S; \
      /* Skip BIT_A for now */ \
  }

#define sbc_A_bytereg(br) { \
    unsigned int tmp; \
      tmp = (unsigned short)regs.A - (unsigned short)(br); \
      if (regs.F & BIT_C) --tmp; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      regs.A = (unsigned char) tmp; \
      if (tmp > 0xff) regs.F |= (BIT_C | BIT_P); \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
      regs.F |= BIT_N; \
      /* Skip BIT_A for now */ \
  }

#define sbc_HL_wordreg(reg) { \
    unsigned int tmp; \
      tmp = (unsigned int)regs.HL - (unsigned int)(reg); \
      if (regs.F & BIT_C) --tmp; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      regs.HL = (unsigned short) tmp; \
      if (tmp > 0xffff) regs.F |= (BIT_C | BIT_P); \
      if (regs.HL == 0) regs.F |= BIT_Z; \
      if (regs.HL & 0x8000) regs.F |= BIT_S; \
      regs.F |= BIT_N; \
      /* Skip BIT_A for now */ \
  }

#define and_A_bytereg(br) { \
      regs.A &= (br); \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
  }

#define xor_A_bytereg(br) { \
      regs.A ^= (br); \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
  }

#define or_A_bytereg(br) { \
      regs.A |= (br); \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      if (regs.A == 0) regs.F |= BIT_Z; \
      if (regs.A & 0x80) regs.F |= BIT_S; \
  }

#define cp_bytereg(br) { unsigned char _tmp1; \
      regs.F &= ~(BIT_ALL);  /* clear these */ \
      if (regs.A < br) regs.F |= BIT_C; \
      _tmp1 = regs.A - (br); \
      regs.F |= BIT_N; /* not addition */ \
      if (_tmp1 == 0) regs.F |= BIT_Z; \
      if (_tmp1 & 0x80) regs.F |= BIT_S; \
      /* Skip BIT_A for now */ \
  }

#define inc(var) /* 8-bit increment */ { var++; \
   regs.F &= ~(BIT_N |BIT_P |BIT_A |BIT_Z |BIT_S);  /* clear these */ \
   if (var == 0) regs.F |= BIT_Z; \
   if (var & 0x80) regs.F |= BIT_S; \
   if ((var & 0x0f) == 0) regs.F |= BIT_A; \
   }

#define dec(var)  { \
 --var; \
 regs.F &= ~(BIT_N |BIT_P |BIT_A |BIT_Z |BIT_S);  /* clear these */ \
 regs.F |= BIT_N;  /* Not add */ \
 if (var == 0) regs.F |= BIT_Z; \
 if (var & 0x80) regs.F |= BIT_S; \
 if ((var & 0x0f) == 0) regs.F |= BIT_A; \
 }


