/*
 * Simulator of microcontrollers (glob.cc)
 *
 * Copyright (C) 1999,99 Drotos Daniel, Talker Bt.
 *
 * Written by Karl Bongers karl@turbobit.com
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

#include <stdio.h>

#include "stypes.h"
#include "glob.h"

char *op_mnemonic_str[] = {
"BAD_OPCODE",
"ADD",
"ADDC",
"SUB",
"SUBB",
"CMP",
"AND",
"OR",
"XOR",
"ADDS",
"NEG",
"SEXT",
"MUL",
"DIV",
"DA",
"ASL",
"ASR",
"LEA",
"CPL",
"LSR",
"NORM",
"RL",
"RLC",
"RR",
"RRC",
"MOVS",
"MOVC",
"MOVX",
"PUSH",
"POP",
"XCH",
"SETB",
"CLR",
"MOV",
"ANL",
"ORL",
"BR",
"JMP",
"CALL",
"RET",
"Bcc",
"JB",
"JNB",
"CJNE",
"DJNZ",
"JZ",
"JNZ",
"NOP",
"BKPT",
"TRAP",
"RESET"
};

struct dis_entry glob_disass_xa[]= {
  { 0x0000, 0x00ff, ' ', 1, "nop" },
  { 0x0000, 0x00, 0, 0, NULL}
};

struct xa_dis_entry disass_xa[]= {
  { 0x0000, 0xffff, ' ', 1, NOP, NO_OPERANDS }, //  NOP   0 0 0 0 0 0 0 0

  { 0x0840, 0xfffc, ' ', 3, ANL, C_BIT    }, //  ANL C, bit   0 0 0 0 1 0 0 0  0 1 0 0 0 0 bit: 2
  { 0x0850, 0xfffc, ' ', 3, ANL, NOTC_BIT }, //  ANL C, /bit  0 0 0 0 1 0 0 0  0 1 0 1 0 0 bit: 2
  { 0x0850, 0xfffc, ' ', 3, ASL, REG_REG  }, //  ASL Rd, Rs  1 1 0 0 SZ1 SZ0 0 1  d d d d s s s s

  { 0x1408, 0xf780, ' ', 2, ADDS, REG_DATA4      }, // ADDS Rd, #data4     1 0 1 0 SZ 0 0 1  d d d d #data4
  { 0x1408, 0xf780, ' ', 2, ADDS, IREG_DATA4     }, // ADDS [Rd], #data4   1 0 1 0 SZ 0 1 0  0 d d d #data4
  { 0x1408, 0xf780, ' ', 2, ADDS, IREGINC_DATA4  }, // ADDS [Rd+], #data4  1 0 1 0 SZ 0 1 1  0 d d d #data4
  { 0x1408, 0xf780, ' ', 3, ADDS, IREGOFF8_DATA4 }, // ADDS [Rd+offset8], #data4   1 0 1 0 SZ 1 0 0  0 d d d #data4
  { 0x1408, 0xf780, ' ', 4, ADDS, IREGOFF16_DATA4}, // ADDS [Rd+offset16], #data4  1 0 1 0 SZ 1 0 1  0 d d d #data4
  { 0x1408, 0xf780, ' ', 3, ADDS, DIRECT_DATA4   }, // ADDS direct, #data4         1 0 1 0 SZ 1 1 0  0 direct: 3 bits #data4

  { 0x0100, 0xf700, ' ', 2, ADD, REG_REG         },  // ADD Rd, Rs    0 0 0 0 SZ 0 0 1   d d d d s s s s
  { 0x0200, 0xf708, ' ', 2, ADD, REG_IREG        },  // ADD Rd, [Rs]  0 0 0 0 SZ 0 1 0   d d d d 0 s s s
  { 0x0208, 0xf708, ' ', 2, ADD, IREG_REG        },  // ADD [Rd], Rs  0 0 0 0 SZ 0 1 0   s s s s 1 d d d
  { 0x0400, 0xf708, ' ', 3, ADD, REG_IREGOFF8    },  // ADD Rd, [Rs+offset8]   0 0 0 0 SZ 1 0 0  d d d d 0 s s s
  { 0x0408, 0xf708, ' ', 3, ADD, IREGOFF8_REG    },  // ADD [Rd+offset8], Rs   0 0 0 0 SZ 1 0 0  s s s s 1 d d d
  { 0x0500, 0xf708, ' ', 4, ADD, REG_IREGOFF16   },  // ADD Rd, [Rs+offset16] 0 0 0 0 SZ 1 0 1  d d d d 0 s s s
  { 0x0508, 0xf708, ' ', 4, ADD, IREGOFF16_REG   },  // ADD [Rd+offset16], Rs  0 0 0 0 SZ 1 0 1  s s s s 1 d d d
  { 0x0300, 0xf708, ' ', 2, ADD, REG_IREGINC     },  // ADD Rd, [Rs+]   0 0 0 0 SZ 0 1 1  d d d d 0 s s s
  { 0x0308, 0xf708, ' ', 2, ADD, IREGINC_REG     },  // ADD [Rd+], Rs   0 0 0 0 SZ 0 1 1 s s s s 1 d d d
  { 0x0608, 0xf708, ' ', 3, ADD, DIRECT_REG      },  // ADD direct, Rs  0 0 0 0 SZ 1 1 0 s s s s 1 direct: 3 bits
  { 0x0600, 0xf708, ' ', 3, ADD, REG_DIRECT      },  // ADD Rd, direct  0 0 0 0 SZ 1 1 0  d d d d 0 direct: 3 bits
  { 0x9100, 0xff0f, ' ', 3, ADD, REG_DATA8       },  // ADD Rd, #data8       1 0 0 1 0 0 0 1  d d d d 0 0 0 0
  { 0x9900, 0xff0f, ' ', 4, ADD, REG_DATA16      },  // ADD Rd, #data16      1 0 0 1 1 0 0 1  d d d d 0 0 0 0
  { 0x9200, 0xff8f, ' ', 3, ADD, IREG_DATA8      },  // ADD [Rd], #data8     1 0 0 1 0 0 1 0  0 d d d 0 0 0 0
  { 0x9a00, 0xff8f, ' ', 4, ADD, IREG_DATA16     },  // ADD [Rd], #data16    1 0 0 1 1 0 1 0  0 d d d 0 0 0 0
  { 0x9300, 0xff8f, ' ', 3, ADD, IREGINC_DATA8   },  // ADD [Rd+], #data8    1 0 0 1 0 0 1 1  0 d d d 0 0 0 0 
  { 0x9b00, 0xff8f, ' ', 4, ADD, IREGINC_DATA16  },  // ADD [Rd+], #data16   1 0 0 1 1 0 1 1  0 d d d 0 0 0 0 (Rd) <-- (Rd) + 2
  { 0x9400, 0xff8f, ' ', 4, ADD, IREGOFF8_DATA8  },  // ADD [Rd+offset8], #data8   1 0 0 1 0 1 0 0  0 d d d 0 0 0 0
  { 0x9c00, 0xff8f, ' ', 5, ADD, IREGOFF8_DATA16 },  // ADD [Rd+offset8], #data16  1 0 0 1 1 1 0 0  0 d d d 0 0 0 0
  { 0x9500, 0xff8f, ' ', 5, ADD, IREGOFF16_DATA8 },  // ADD [Rd+offset16], #data8  1 0 0 1 0 1 0 1  0 d d d 0 0 0 0
  { 0x9d00, 0xff8f, ' ', 6, ADD, IREGOFF16_DATA16},  // ADD [Rd+offset16], #data16  1 0 0 1 1 1 0 1  0 d d d 0 0 0 0
  { 0x9600, 0xff8f, ' ', 4, ADD, DIRECT_DATA8    },  // ADD direct, #data8   1 0 0 1 0 1 1 0   0 direct: 3 bits 0 0 0 0 
  { 0x9e00, 0xff8f, ' ', 5, ADD, DIRECT_DATA16   },  // ADD direct, #data16 1 0 0 1 0 1 1 0   1 0 0 1 1 1 1 0  0 direct: 3 bits 0 0 0 0 

  { 0x1100, 0xf700, ' ', 2,ADDC, REG_REG         }, // ADDC Rd, Rs      0 0 0 1 SZ 0 0 1  d d d d s s s s
  { 0x1200, 0xf780, ' ', 2,ADDC, REG_IREG        }, // ADDC Rd, [Rs]  0 0 0 1 SZ 0 1 0  d d d d 0 s s s
  { 0x1208, 0xf780, ' ', 2,ADDC, IREG_REG        }, // ADDC [Rd], Rs  0 0 0 1 SZ 0 1 0  s s s s 1 d d d
  { 0x1400, 0xf780, ' ', 3,ADDC, REG_IREGOFF8    }, // ADDC Rd, [Rs+offset8]  0 0 0 1 SZ 1 0 0  d d d d 0 s s s
  { 0x1408, 0xf780, ' ', 3,ADDC, IREGOFF8_REG    }, // ADDC [Rd+offset8], Rs   0 0 0 1 SZ 1 0 0  s s s s 1 d d d
  { 0x1500, 0xf780, ' ', 4,ADDC, REG_IREGOFF16   }, // ADDC Rd, [Rs+offset16]   0 0 0 1 SZ 1 0 1  d d d d 0 s s s
  { 0x1508, 0xf780, ' ', 4,ADDC, IREGOFF16_REG   }, // ADDC [Rd+offset16], Rs   0 0 0 1 SZ 1 0 1  s s s s 1 d d d
  { 0x1300, 0xf780, ' ', 2,ADDC, REG_IREGINC     }, // ADDC Rd, [Rs+]   0 0 0 1 SZ 0 1 1  d d d d 0 s s s
  { 0x1308, 0xf780, ' ', 2,ADDC, IREGINC_REG     }, // ADDC [Rd+], Rs   0 0 0 1 SZ 0 1 1  s s s s 1 d d d
  { 0x1608, 0xf780, ' ', 3,ADDC, DIRECT_REG      }, // ADDC direct, Rs  0 0 0 1 SZ 1 1 0  s s s s 1 direct: 3 bits
  { 0x1600, 0xf780, ' ', 3,ADDC, REG_DIRECT      }, // ADDC Rd, direct  0 0 0 1 SZ 1 1 0  d d d d 0 direct: 3 bits
  { 0x9101, 0xff0f, ' ', 3,ADDC, REG_DATA8       }, // ADDC Rd, #data8       1 0 0 1 0 0 0 1  d d d d 0 0 0 1
  { 0x9901, 0xff0f, ' ', 4,ADDC, REG_DATA16      }, // ADDC Rd, #data16     1 0 0 1 1 0 0 1  d d d d 0 0 0 1
  { 0x9201, 0xff8f, ' ', 3,ADDC, IREG_DATA8      }, // ADDC [Rd], #data8   1 0 0 1 0 0 1 0  0 d d d 0 0 0 1
  { 0x9a01, 0xff8f, ' ', 4,ADDC, IREG_DATA16     }, // ADDC [Rd], #data16 1 0 0 1 1 0 1 0  0 d d d 0 0 0 1
  { 0x9301, 0xff8f, ' ', 3,ADDC, IREGINC_DATA8   }, // ADDC [Rd+], #data8 1 0 0 1 0 0 1 1  0 d d d 0 0 0 1
  { 0x9b01, 0xff8f, ' ', 4,ADDC, IREGINC_DATA16  }, // ADDC [Rd+], #data16  1 0 0 1 1 0 1 1  0 d d d 0 0 0 1
  { 0x9401, 0xff8f, ' ', 4,ADDC, IREGOFF8_DATA8  }, // ADDC [Rd+offset8], #data8   1 0 0 1 0 1 0 0  0 d d d 0 0 0 1
  { 0x9c01, 0xff8f, ' ', 5,ADDC, IREGOFF8_DATA16 }, // ADDC [Rd+offset8], #data16  1 0 0 1 1 1 0 0  0 d d d 0 0 0 1
  { 0x9501, 0xff8f, ' ', 5,ADDC, IREGOFF16_DATA8 }, // ADDC [Rd+offset16], #data8  1 0 0 1 0 1 0 1  0 d d d 0 0 0 1
  { 0x9d01, 0xff8f, ' ', 6,ADDC, IREGOFF16_DATA16}, // ADDC [Rd+offset16], #data16 1 0 0 1 1 1 0 1  0 d d d 0 0 0 1
  { 0x9601, 0xff8f, ' ', 4,ADDC, DIRECT_DATA8    }, // ADDC direct, #data8   1 0 0 1 0 1 1 0  0 direct: 3 bits 0 0 0 1 
  { 0x9e01, 0xff8f, ' ', 5,ADDC, DIRECT_DATA16   }, // ADDC direct, #data16 1 0 0 1 1 1 1 0  0 direct: 3 bits 0 0 0 1 

  { 0x5100, 0xf700, ' ', 2, AND, REG_REG         },  // AND Rd, Rs      0 1 0 1 SZ 0 0 1   d d d d s s s s
  { 0x5200, 0xf708, ' ', 2, AND, REG_IREG        },  // AND Rd, [Rs]  0 1 0 1 SZ 0 1 0   d d d d 0 s s s
  { 0x5208, 0xf708, ' ', 2, AND, IREG_REG        },  // AND [Rd], Rs  0 1 0 1 SZ 0 1 0   s s s s 1 d d d
  { 0x5400, 0xf708, ' ', 3, AND, REG_IREGOFF8    },  // AND Rd, [Rs+offset8]   0 1 0 1 SZ 1 0 0  d d d d 0 s s s
  { 0x5408, 0xf708, ' ', 3, AND, IREGOFF8_REG    },  // AND [Rd+offset8], Rs   0 1 0 1 SZ 1 0 0  s s s s 1 d d d
  { 0x5500, 0xf708, ' ', 4, AND, REG_IREGOFF16   },  // AND Rd, [Rs+offset16] 0 1 0 1 SZ 1 0 1  d d d d 0 s s s
  { 0x5508, 0xf708, ' ', 4, AND, IREGOFF16_REG   },  // AND [Rd+offset16], Rs  0 1 0 1 SZ 1 0 1  s s s s 1 d d d
  { 0x5300, 0xf708, ' ', 2, AND, REG_IREGINC     },  // AND Rd, [Rs+]   0 1 0 1 SZ 0 1 1  d d d d 0 s s s
  { 0x5308, 0xf708, ' ', 2, AND, IREGINC_REG     },  // AND [Rd+], Rs   0 1 0 1 SZ 0 1 1 s s s s 1 d d d
  { 0x5608, 0xf708, ' ', 3, AND, DIRECT_REG      },  // AND direct, Rs  0 1 0 1 SZ 1 1 0 s s s s 1 direct: 3 bits
  { 0x5600, 0xf708, ' ', 3, AND, REG_DIRECT      },  // AND Rd, direct  0 1 0 1 SZ 1 1 0  d d d d 0 direct: 3 bits
  { 0x9105, 0xff0f, ' ', 3, AND, REG_DATA8       },  // AND Rd, #data8       1 0 0 1 0 0 0 1  d d d d 0 1 0 1
  { 0x9905, 0xff0f, ' ', 4, AND, REG_DATA16      },  // AND Rd, #data16     1 0 0 1 1 0 0 1  d d d d 0 1 0 1
  { 0x9205, 0xff8f, ' ', 3, AND, IREG_DATA8      },  // AND [Rd], #data8   1 0 0 1 0 0 1 0  0 d d d 0 1 0 1
  { 0x9a05, 0xff8f, ' ', 4, AND, IREG_DATA16     },  // AND [Rd], #data16 1 0 0 1 1 0 1 0  0 d d d 0 1 0 1
  { 0x9305, 0xff8f, ' ', 3, AND, IREGINC_DATA8   },  // AND [Rd+], #data8 1 0 0 1 0 0 1 1  0 d d d 0 1 0 1
  { 0x9b05, 0xff8f, ' ', 4, AND, IREGINC_DATA16  },  // AND [Rd+], #data16  1 0 0 1 1 0 1 1  0 d d d 0 1 0 1 (Rd) <-- (Rd) + 2
  { 0x9405, 0xff8f, ' ', 4, AND, IREGOFF8_DATA8  },  // AND [Rd+offset8], #data8   1 0 0 1 0 1 0 0  0 d d d 0 1 0 1
  { 0x9c05, 0xff8f, ' ', 5, AND, IREGOFF8_DATA16 },  // AND [Rd+offset8], #data16  1 0 0 1 1 1 0 0  0 d d d 0 1 0 1
  { 0x9505, 0xff8f, ' ', 5, AND, IREGOFF16_DATA8 },  // AND [Rd+offset16], #data8  1 0 0 1 0 1 0 1  0 d d d 0 1 0 1
  { 0x9d05, 0xff8f, ' ', 6, AND, IREGOFF16_DATA16},  // AND [Rd+offset16], #data16  1 0 0 1 1 1 0 1  0 d d d 0 1 0 1
  { 0x9605, 0xff8f, ' ', 4, AND, DIRECT_DATA8    },  // AND direct, #data8   1 0 0 1 0 1 1 0   0 direct: 3 bits 0 1 0 1 
  { 0x9e05, 0xff8f, ' ', 5, AND, DIRECT_DATA16   },  // AND direct, #data16 1 0 0 1 0 1 1 0   1 0 0 1 1 1 1 0  0 direct: 3 bits 0 1 0 1 

  { 0x4100, 0xf700, ' ', 2, CMP, REG_REG         },  // CMP Rd, Rs      0 1 0 0 SZ 0 0 1   d d d d s s s s
  { 0x4200, 0xf708, ' ', 2, CMP, REG_IREG        },  // CMP Rd, [Rs]  0 1 0 0 SZ 0 1 0   d d d d 0 s s s
  { 0x4208, 0xf708, ' ', 2, CMP, IREG_REG        },  // CMP [Rd], Rs  0 1 0 0 SZ 0 1 0   s s s s 1 d d d
  { 0x4400, 0xf708, ' ', 3, CMP, REG_IREGOFF8    },  // CMP Rd, [Rs+offset8]   0 1 0 0 SZ 1 0 0  d d d d 0 s s s
  { 0x4408, 0xf708, ' ', 3, CMP, IREGOFF8_REG    },  // CMP [Rd+offset8], Rs   0 1 0 0 SZ 1 0 0  s s s s 1 d d d
  { 0x4500, 0xf708, ' ', 4, CMP, REG_IREGOFF16   },  // CMP Rd, [Rs+offset16] 0 1 0 0 SZ 1 0 1  d d d d 0 s s s
  { 0x4508, 0xf708, ' ', 4, CMP, IREGOFF16_REG   },  // CMP [Rd+offset16], Rs  0 1 0 0 SZ 1 0 1  s s s s 1 d d d
  { 0x4300, 0xf708, ' ', 2, CMP, REG_IREGINC     },  // CMP Rd, [Rs+]   0 1 0 0 SZ 0 1 1  d d d d 0 s s s
  { 0x4308, 0xf708, ' ', 2, CMP, IREGINC_REG     },  // CMP [Rd+], Rs   0 1 0 0 SZ 0 1 1 s s s s 1 d d d
  { 0x4608, 0xf708, ' ', 3, CMP, DIRECT_REG      },  // CMP direct, Rs  0 1 0 0 SZ 1 1 0 s s s s 1 direct: 3 bits
  { 0x4600, 0xf708, ' ', 3, CMP, REG_DIRECT      },  // CMP Rd, direct  0 1 0 0 SZ 1 1 0  d d d d 0 direct: 3 bits
  { 0x9104, 0xff0f, ' ', 3, CMP, REG_DATA8       },  // CMP Rd, #data8       1 0 0 1 0 0 0 1  d d d d 0 1 0 0
  { 0x9904, 0xff0f, ' ', 4, CMP, REG_DATA16      },  // CMP Rd, #data16     1 0 0 1 1 0 0 1  d d d d 0 1 0 0
  { 0x9204, 0xff8f, ' ', 3, CMP, IREG_DATA8      },  // CMP [Rd], #data8   1 0 0 1 0 0 1 0  0 d d d 0 1 0 0
  { 0x9a04, 0xff8f, ' ', 4, CMP, IREG_DATA16     },  // CMP [Rd], #data16 1 0 0 1 1 0 1 0  0 d d d 0 1 0 0
  { 0x9304, 0xff8f, ' ', 3, CMP, IREGINC_DATA8   },  // CMP [Rd+], #data8 1 0 0 1 0 0 1 1  0 d d d 0 1 0 0 
  { 0x9b04, 0xff8f, ' ', 4, CMP, IREGINC_DATA16  },  // CMP [Rd+], #data16  1 0 0 1 1 0 1 1  0 d d d 0 1 0 0 (Rd) <-- (Rd) + 2
  { 0x9404, 0xff8f, ' ', 4, CMP, IREGOFF8_DATA8  },  // CMP [Rd+offset8], #data8   1 0 0 1 0 1 0 0  0 d d d 0 1 0 0
  { 0x9c04, 0xff8f, ' ', 5, CMP, IREGOFF8_DATA16 },  // CMP [Rd+offset8], #data16  1 0 0 1 1 1 0 0  0 d d d 0 1 0 0
  { 0x9504, 0xff8f, ' ', 5, CMP, IREGOFF16_DATA8 },  // CMP [Rd+offset16], #data8  1 0 0 1 0 1 0 1  0 d d d 0 1 0 0
  { 0x9d04, 0xff8f, ' ', 6, CMP, IREGOFF16_DATA16},  // CMP [Rd+offset16], #data16  1 0 0 1 1 1 0 1  0 d d d 0 1 0 0
  { 0x9604, 0xff8f, ' ', 4, CMP, DIRECT_DATA8    },  // CMP direct, #data8   1 0 0 1 0 1 1 0   0 direct: 3 bits 0 1 0 0 
  { 0x9e04, 0xff8f, ' ', 5, CMP, DIRECT_DATA16   },  // CMP direct, #data16 1 0 0 1 0 1 1 0   1 0 0 1 1 1 1 0  0 direct: 3 bits 0 1 0 0 

  { 0x8100, 0xf700, ' ', 2, MOV, REG_REG         },  // MOV Rd, Rs      1 0 0 0 SZ 0 0 1   d d d d s s s s
  { 0x8200, 0xf708, ' ', 2, MOV, REG_IREG        },  // MOV Rd, [Rs]  1 0 0 0 SZ 0 1 0   d d d d 0 s s s
  { 0x8208, 0xf708, ' ', 2, MOV, IREG_REG        },  // MOV [Rd], Rs  1 0 0 0 SZ 0 1 0   s s s s 1 d d d
  { 0x8400, 0xf708, ' ', 3, MOV, REG_IREGOFF8    },  // MOV Rd, [Rs+offset8]   1 0 0 0 SZ 1 0 0  d d d d 0 s s s
  { 0x8408, 0xf708, ' ', 3, MOV, IREGOFF8_REG    },  // MOV [Rd+offset8], Rs   1 0 0 0 SZ 1 0 0  s s s s 1 d d d
  { 0x8500, 0xf708, ' ', 4, MOV, REG_IREGOFF16   },  // MOV Rd, [Rs+offset16] 1 0 0 0 SZ 1 0 1  d d d d 0 s s s
  { 0x8508, 0xf708, ' ', 4, MOV, IREGOFF16_REG   },  // MOV [Rd+offset16], Rs  1 0 0 0 SZ 1 0 1  s s s s 1 d d d
  { 0x8300, 0xf708, ' ', 2, MOV, REG_IREGINC     },  // MOV Rd, [Rs+]   1 0 0 0 SZ 0 1 1  d d d d 0 s s s
  { 0x8308, 0xf708, ' ', 2, MOV, IREGINC_REG     },  // MOV [Rd+], Rs   1 0 0 0 SZ 0 1 1 s s s s 1 d d d
  { 0x8608, 0xf708, ' ', 3, MOV, DIRECT_REG      },  // MOV direct, Rs  1 0 0 0 SZ 1 1 0 s s s s 1 direct: 3 bits
  { 0x8600, 0xf708, ' ', 3, MOV, REG_DIRECT      },  // MOV Rd, direct  1 0 0 0 SZ 1 1 0  d d d d 0 direct: 3 bits
  { 0x9108, 0xff0f, ' ', 3, MOV, REG_DATA8       },  // MOV Rd, #data8       1 0 0 1 0 0 0 1  d d d d 1 0 0 0
  { 0x9908, 0xff0f, ' ', 4, MOV, REG_DATA16      },  // MOV Rd, #data16     1 0 0 1 1 0 0 1  d d d d 1 0 0 0
  { 0x9208, 0xff8f, ' ', 3, MOV, IREG_DATA8      },  // MOV [Rd], #data8   1 0 0 1 0 0 1 0  0 d d d 1 0 0 0
  { 0x9a08, 0xff8f, ' ', 4, MOV, IREG_DATA16     },  // MOV [Rd], #data16 1 0 0 1 1 0 1 0  0 d d d 1 0 0 0
  { 0x9308, 0xff8f, ' ', 3, MOV, IREGINC_DATA8   },  // MOV [Rd+], #data8 1 0 0 1 0 0 1 1  0 d d d 1 0 0 0 
  { 0x9b08, 0xff8f, ' ', 4, MOV, IREGINC_DATA16  },  // MOV [Rd+], #data16  1 0 0 1 1 0 1 1  0 d d d 1 0 0 0 (Rd) <-- (Rd) + 2
  { 0x9408, 0xff8f, ' ', 4, MOV, IREGOFF8_DATA8  },  // MOV [Rd+offset8], #data8   1 0 0 1 0 1 0 0  0 d d d 1 0 0 0
  { 0x9c08, 0xff8f, ' ', 5, MOV, IREGOFF8_DATA16 },  // MOV [Rd+offset8], #data16  1 0 0 1 1 1 0 0  0 d d d 1 0 0 0
  { 0x9508, 0xff8f, ' ', 5, MOV, IREGOFF16_DATA8 },  // MOV [Rd+offset16], #data8  1 0 0 1 0 1 0 1  0 d d d 1 0 0 0
  { 0x9d08, 0xff8f, ' ', 6, MOV, IREGOFF16_DATA16},  // MOV [Rd+offset16], #data16  1 0 0 1 1 1 0 1  0 d d d 1 0 0 0
  { 0x9608, 0xff8f, ' ', 4, MOV, DIRECT_DATA8    },  // MOV direct, #data8   1 0 0 1 0 1 1 0   0 direct: 3 bits 1 0 0 0 
  { 0x9e08, 0xff8f, ' ', 5, MOV, DIRECT_DATA16   },  // MOV direct, #data16 1 0 0 1 0 1 1 0   1 0 0 1 1 1 1 0  0 direct: 3 bits 1 0 0 0 
  { 0x0000,   0x00,   0, 1, BAD_OPCODE, REG_REG}
};


/* End of xa.src/glob.cc */
