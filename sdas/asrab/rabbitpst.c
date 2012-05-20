/* rabbitpst.c

   Copyright (C) 1989-1995 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * Extensions: P. Felber
 * 
 * Altered by Leland Morrison to support rabbit 2000 
 *   and rabbit 4000 instruction sets (2011)
 */

#include "asxxxx.h"
#include "rabbit.h"

struct  mne     mne[] = {

        /* machine */

        /* system */

    { NULL,     "CON",          S_ATYP,         0,      A3_CON },
    { NULL,     "OVR",          S_ATYP,         0,      A3_OVR },
    { NULL,     "REL",          S_ATYP,         0,      A3_REL },
    { NULL,     "ABS",          S_ATYP,         0,      A3_ABS },
    { NULL,     "NOPAG",        S_ATYP,         0,      A3_NOPAG },
    { NULL,     "PAG",          S_ATYP,         0,      A3_PAG },

    { NULL,     ".byte",        S_BYTE,         0,      0 },
    { NULL,     ".db",          S_BYTE,         0,      0 },
    { NULL,     ".word",        S_WORD,         0,      0 },
    { NULL,     ".dw",          S_WORD,         0,      0 },
    { NULL,     ".df",          S_FLOAT,        0,      0 },
    { NULL,     ".ascii",       S_ASCII,        0,      0 },
    { NULL,     ".asciz",       S_ASCIZ,        0,      0 },
    { NULL,     ".blkb",        S_BLK,          0,      1 },
    { NULL,     ".ds",          S_BLK,          0,      1 },
    { NULL,     ".blkw",        S_BLK,          0,      2 },
    { NULL,     ".page",        S_PAGE,         0,      0 },
    { NULL,     ".title",       S_TITLE,        0,      0 },
    { NULL,     ".sbttl",       S_SBTL,         0,      0 },
    { NULL,     ".globl",       S_GLOBL,        0,      0 },
    { NULL,     ".area",        S_DAREA,        0,      0 },
    { NULL,     ".even",        S_EVEN,         0,      0 },
    { NULL,     ".odd",         S_ODD,          0,      0 },
    { NULL,     ".if",          S_IF,           0,      0 },
    { NULL,     ".else",        S_ELSE,         0,      0 },
    { NULL,     ".endif",       S_ENDIF,        0,      0 },
    { NULL,     ".include",     S_INCL,         0,      0 },
    { NULL,     ".radix",       S_RADIX,        0,      0 },
    { NULL,     ".org",         S_ORG,          0,      0 },
    { NULL,     ".module",      S_MODUL,        0,      0 },
    { NULL,     ".ascis",       S_ASCIS,        0,      0 },
/* sdas specific */
    { NULL,     ".optsdcc",     S_OPTSDCC,      0,      0 },
/* end sdas specific */

        /* rabbit (warning: derived from "z80 / hd64180") */

    { NULL,     "ld",           S_LD,           0,      0x40 },

    { NULL,     "call",         S_CALL,         0,      0xC4 },
    { NULL,     "jp",           S_JP,           0,      0xC2 },
    { NULL,     "jr",           S_JR,           0,      0x18 },
    { NULL,     "djnz",         S_DJNZ,         0,      0x10 },
    { NULL,     "ret",          S_RET,          0,      0xC0 },

    { NULL,     "bit",          S_BIT,          0,      0x40 },
    { NULL,     "res",          S_BIT,          0,      0x80 },
    { NULL,     "set",          S_BIT,          0,      0xC0 },

    { NULL,     "inc",          S_INC,          0,      0x04 },
    { NULL,     "dec",          S_DEC,          0,      0x05 },

    { NULL,     "add",          S_ADD,          0,      0x80 },
    { NULL,     "adc",          S_ADC,          0,      0x88 },
    { NULL,     "sub",          S_SUB,          0,      0x90 },
    { NULL,     "sbc",          S_SBC,          0,      0x98 },

    { NULL,     "and",          S_AND,          0,      0xA0 },
    { NULL,     "cp",           S_AND,          0,      0xB8 },
    { NULL,     "or",           S_AND,          0,      0xB0 },
    { NULL,     "xor",          S_AND,          0,      0xA8 },

    { NULL,     "ex",           S_EX,           0,      0xE3 },

    { NULL,     "push",         S_PUSH,         0,      0xC5 },
    { NULL,     "pop",          S_PUSH,         0,      0xC1 },

    /* { NULL,  "in",           S_IN,           0,      0xDB }, */
    /* { NULL,  "out",          S_OUT,          0,      0xD3 }, */
    { NULL,     "ioi",          S_IN,           0,      0xD3 },
    { NULL,     "ioe",          S_OUT,          0,      0xDB },

    { NULL,     "rl",           S_RL,           0,      0x10 },
    { NULL,     "rlc",          S_RL,           0,      0x00 },
    { NULL,     "rr",           S_RL,           0,      0x18 },
    { NULL,     "rrc",          S_RL,           0,      0x08 },
    { NULL,     "sla",          S_RL,           0,      0x20 },
    { NULL,     "sra",          S_RL,           0,      0x28 },
    { NULL,     "srl",          S_RL,           0,      0x38 },

    { NULL,     "rst",          S_RST,          0,      0xC7 },

    /* { NULL,  "im",           S_IM,           0,      0xED }, */

    { NULL,     "ccf",          S_INH1,         0,      0x3F },
    { NULL,     "cpl",          S_INH1,         0,      0x2F },
    /* { NULL,  "daa",          S_INH1,         0,      0x27 }, */
    /* { NULL,  "di",           S_INH1,         0,      0xF3 }, */
    /* { NULL,  "ei",           S_INH1,         0,      0xFB }, */
    { NULL,     "ipset0",       S_INH2,         0,      0x46 },
    { NULL,     "ipset1",       S_INH2,         0,      0x56 },
    { NULL,     "ipset2",       S_INH2,         0,      0x4E },
    { NULL,     "ipset3",       S_INH2,         0,      0x5E },
    { NULL,     "ipres",        S_INH2,         0,      0x5D },
    { NULL,     "exx",          S_INH1,         0,      0xD9 },
    { NULL,     "nop",          S_INH1,         0,      0x00 },

    /* { NULL,  "halt",         S_INH1,         0,      0x76 }, */
    { NULL,     "altd",         S_INH1,         0,      0x76 },

    { NULL,     "rla",          S_INH1,         0,      0x17 },
    { NULL,     "rlca",         S_INH1,         0,      0x07 },
    { NULL,     "rra",          S_INH1,         0,      0x1F },
    { NULL,     "rrca",         S_INH1,         0,      0x0F },
    { NULL,     "scf",          S_INH1,         0,      0x37 },

    /* { NULL,  "cpd",          S_INH2,         0,      0xA9 }, */
    /* { NULL,  "cpdr",         S_INH2,         0,      0xB9 }, */
    /* { NULL,  "cpi",          S_INH2,         0,      0xA1 }, */
    /* { NULL,  "cpir",         S_INH2,         0,      0xB1 }, */
    /* { NULL,  "ind",          S_INH2,         0,      0xAA }, */
    /* { NULL,  "indr",         S_INH2,         0,      0xBA }, */
    /* { NULL,  "ini",          S_INH2,         0,      0xA2 }, */
    /* { NULL,  "inir",         S_INH2,         0,      0xB2 }, */
    { NULL,     "ldd",          S_INH2,         0,      0xA8 },
    { NULL,     "lddr",         S_INH2,         0,      0xB8 },
    { NULL,     "ldi",          S_INH2,         0,      0xA0 },
    { NULL,     "ldir",         S_INH2,         0,      0xB0 },
    { NULL,     "neg",          S_NEG,          0,      0x44 },
    /* { NULL,  "outd",         S_INH2,         0,      0xAB }, */
    /* { NULL,  "otdr",         S_INH2,         0,      0xBB }, */
    /* { NULL,  "outi",         S_INH2,         0,      0xA3 }, */
    /* { NULL,  "otir",         S_INH2,         0,      0xB3 }, */
    { NULL,     "reti",         S_INH2,         0,      0x4D },
    /* { NULL,  "retn",         S_INH2,         0,      0x45 },*/
    { NULL,     "lret",         S_INH2,         0,      0x45 },
    /* { NULL,  "rld",          S_INH2,         0,      0x6F }, */
    /* { NULL,  "rrd",          S_INH2,         S_EOL,  0x67 } */
    
    { NULL,     "mul",          S_INH1,         0,      0xF7 },
    
    { NULL,     ".r3k",         X_R3K_MODE,     0,      0 },
    { NULL,     "idet",         R3K_INH1,       0,      0x5B },
    { NULL,     "lddsr",        R3K_INH2,       0,      0x98 },
    { NULL,     "ldisr",        R3K_INH2,       0,      0x90 },
    { NULL,     "lsddr",        R3K_INH2,       0,      0xD8 },
    { NULL,     "lsdr",         R3K_INH2,       0,      0xF8 },
    { NULL,     "lsidr",        R3K_INH2,       0,      0xD0 },
    { NULL,     "lsir",         R3K_INH2,       0,      0xF0 },
    { NULL,     "rdmode",       R3K_INH2,       0,      0x7F },
    { NULL,     "setusr",       R3K_INH2,       0,      0x6F },
    { NULL,     "sures",        R3K_INH2,       0,      0x7D },
    { NULL,     "uma",          R3K_INH2,       0,      0xC0 },
    { NULL,     "ums",          R3K_INH2,       0,      0xC8 },
    
    { NULL,     ".r4k",         X_R4K_MODE,     0,      0 },
    { NULL,     "jre",          X_JRE,          0,      0xA3 },
    { NULL,     "clr",          X_CLR,          0,      0xBF },
    { NULL,     "ljp",          X_LJP,          0,      0xC7 },
    { NULL,     "lcall",        X_LCALL,        0,      0xCF },
    { NULL,     "lret",         S_INH2,         0,      0x45 },
    { NULL,     "bool",         X_BOOL,         0,      0xCC },
    { NULL,     "syscall",      R4K_INH2,       0,      0x75 },
    { NULL,     "sysret",       R4K_INH2,       0,      0x83 },
    { NULL,     "mulu",         X_R4K_MULU,     S_EOL,  0xA7 }
};
