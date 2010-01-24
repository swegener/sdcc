/* m08pst.c

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

#include <stdio.h>
#include <setjmp.h>
#include "asxxxx.h"
#include "m6808.h"

struct	mne	mne[] = {

	/* machine */

    {	NULL,	".setdp",	S_SDP,		0,	0	},

	/* system */

    {	NULL,	"CON",		S_ATYP,		0,	A_CON	},
    {	NULL,	"OVR",		S_ATYP,		0,	A_OVR	},
    {	NULL,	"REL",		S_ATYP,		0,	A_REL	},
    {	NULL,	"ABS",		S_ATYP,		0,	A_ABS	},
    {	NULL,	"NOPAG",	S_ATYP,		0,	A_NOPAG	},
    {	NULL,	"PAG",		S_ATYP,		0,	A_PAG	},
    {	NULL,	"CODE",		S_ATYP,		0,	A_CODE	},
    {	NULL,	"DATA",		S_ATYP,		0,	A_DATA	},
    {	NULL,	"LOAD",		S_ATYP,		0,	A_LOAD	},
    {	NULL,	"NOLOAD",	S_ATYP,		0,	A_NOLOAD },

    {	NULL,	".uleb128",	S_ULEB128,	0,	0	},
    {	NULL,	".sleb128",	S_SLEB128,	0,	0	},
    {	NULL,	".byte",	S_BYTE,		0,	1	},
    {	NULL,	".db",		S_BYTE,		0,	1	},
    {	NULL,	".word",	S_WORD,		0,	2	},
    {	NULL,	".dw",		S_WORD,		0,	2	},
    {	NULL,	".ascii",	S_ASCII,	0,	0	},
    {	NULL,	".asciz",	S_ASCIZ,	0,	0	},
    {	NULL,	".blkb",	S_BLK,		0,	1	},
    {	NULL,	".ds",		S_BLK,		0,	1	},
    {	NULL,	".blkw",	S_BLK,		0,	2	},
    {	NULL,	".page",	S_PAGE,		0,	0	},
    {	NULL,	".title",	S_TITLE,	0,	0	},
    {	NULL,	".sbttl",	S_SBTL,		0,	0	},
    {	NULL,	".globl",	S_GLOBL,	0,	0	},
    {	NULL,	".area",	S_DAREA,	0,	0	},
    {	NULL,	".even",	S_EVEN,		0,	0	},
    {	NULL,	".odd",		S_ODD,		0,	0	},
    {	NULL,	".if",		S_IF,		0,	0	},
    {	NULL,	".else",	S_ELSE,		0,	0	},
    {	NULL,	".endif",	S_ENDIF,	0,	0	},
    {	NULL,	".include",	S_INCL,		0,	0	},
    {	NULL,	".radix",	S_RADIX,	0,	0	},
    {	NULL,	".org",		S_ORG,		0,	0	},
    {	NULL,	".module",	S_MODUL,	0,	0	},
    {	NULL,	".ascis",	S_ASCIS,	0,	0	},
/* sdas specific */
    {	NULL,	".optsdcc",	S_OPTSDCC,	0,	0	},
/* end sdas specific */
//    {	NULL,	".assume",	S_ERROR,	0,	0	},
//    {	NULL,	".error",	S_ERROR,	0,	1	},

	/* 68HC08 */

    {	NULL,	"neg",		S_TYP1,		0,	0x30	},
    {	NULL,	"com",		S_TYP1,		0,	0x33	},
    {	NULL,	"lsr",		S_TYP1,		0,	0x34	},
    {	NULL,	"ror",		S_TYP1,		0,	0x36	},
    {	NULL,	"asr",		S_TYP1,		0,	0x37	},
    {	NULL,	"asl",		S_TYP1,		0,	0x38	},
    {	NULL,	"lsl",		S_TYP1,		0,	0x38	},
    {	NULL,	"rol",		S_TYP1,		0,	0x39	},
    {	NULL,	"dec",		S_TYP1,		0,	0x3A	},
    {	NULL,	"inc",		S_TYP1,		0,	0x3C	},
    {	NULL,	"tst",		S_TYP1,		0,	0x3D	},
    {	NULL,	"clr",		S_TYP1,		0,	0x3F	},

    {	NULL,	"sub",		S_TYP2,		0,	0xA0	},
    {	NULL,	"cmp",		S_TYP2,		0,	0xA1	},
    {	NULL,	"sbc",		S_TYP2,		0,	0xA2	},
    {	NULL,	"cpx",		S_TYP2,		0,	0xA3	},
    {	NULL,	"and",		S_TYP2,		0,	0xA4	},
    {	NULL,	"bit",		S_TYP2,		0,	0xA5	},
    {	NULL,	"lda",		S_TYP2,		0,	0xA6	},
    {	NULL,	"sta",		S_TYP2,		0,	0xA7	},
    {	NULL,	"eor",		S_TYP2,		0,	0xA8	},
    {	NULL,	"adc",		S_TYP2,		0,	0xA9	},
    {	NULL,	"ora",		S_TYP2,		0,	0xAA	},
    {	NULL,	"add",		S_TYP2,		0,	0xAB	},
    {	NULL,	"jmp",		S_TYP2,		0,	0xAC	},
    {	NULL,	"jsr",		S_TYP2,		0,	0xAD	},
    {	NULL,	"ldx",		S_TYP2,		0,	0xAE	},
    {	NULL,	"stx",		S_TYP2,		0,	0xAF	},

    {	NULL,	"bset",		S_TYP3,		0,	0x10	},
    {	NULL,	"bclr",		S_TYP3,		0,	0x11	},

    {	NULL,	"brset",	S_TYP4,		0,	0x00	},
    {	NULL,	"brclr",	S_TYP4,		0,	0x01	},

    {	NULL,	"ais",		S_TYPAI,	0,	0xA7	},
    {	NULL,	"aix",		S_TYPAI,	0,	0xAF	},

    {	NULL,	"sthx",		S_TYPHX,	0,	0x25	},
    {	NULL,	"ldhx",		S_TYPHX,	0,	0x45	},
    {	NULL,	"cphx",		S_TYPHX,	0,	0x65	},

    {	NULL,	"cbeq",		S_CBEQ,		0,	0x31	},
    {	NULL,	"cbeqa",	S_CQAX,		0,	0x41	},
    {	NULL,	"cbeqx",	S_CQAX,		0,	0x51	},

    {	NULL,	"dbnz",		S_DBNZ,		0,	0x3B	},
    {	NULL,	"dbnza",	S_DZAX,		0,	0x4B	},
    {	NULL,	"dbnzx",	S_DZAX,		0,	0x5B	},

    {	NULL,	"mov",		S_MOV,		0,	0x4E	},

    {	NULL,	"bra",		S_BRA,		0,	0x20	},
    {	NULL,	"brn",		S_BRA,		0,	0x21	},
    {	NULL,	"bhi",		S_BRA,		0,	0x22	},
    {	NULL,	"bls",		S_BRA,		0,	0x23	},
    {	NULL,	"bcc",		S_BRA,		0,	0x24	},
    {	NULL,	"bhs",		S_BRA,		0,	0x24	},
    {	NULL,	"bcs",		S_BRA,		0,	0x25	},
    {	NULL,	"blo",		S_BRA,		0,	0x25	},
    {	NULL,	"bne",		S_BRA,		0,	0x26	},
    {	NULL,	"beq",		S_BRA,		0,	0x27	},
    {	NULL,	"bhcc",		S_BRA,		0,	0x28	},
    {	NULL,	"bhcs",		S_BRA,		0,	0x29	},
    {	NULL,	"bpl",		S_BRA,		0,	0x2A	},
    {	NULL,	"bmi",		S_BRA,		0,	0x2B	},
    {	NULL,	"bmc",		S_BRA,		0,	0x2C	},
    {	NULL,	"bms",		S_BRA,		0,	0x2D	},
    {	NULL,	"bil",		S_BRA,		0,	0x2E	},
    {	NULL,	"bih",		S_BRA,		0,	0x2F	},
    {	NULL,	"bge",		S_BRA,		0,	0x90	},
    {	NULL,	"blt",		S_BRA,		0,	0x91	},
    {	NULL,	"bgt",		S_BRA,		0,	0x92	},
    {	NULL,	"ble",		S_BRA,		0,	0x93	},
    {	NULL,	"bsr",		S_BRA,		0,	0xAD	},

    {	NULL,	"nega",		S_INH,		0,	0x40	},
    {	NULL,	"mul",		S_INH,		0,	0x42	},
    {	NULL,	"coma",		S_INH,		0,	0x43	},
    {	NULL,	"lsra",		S_INH,		0,	0x44	},
    {	NULL,	"rora",		S_INH,		0,	0x46	},
    {	NULL,	"asra",		S_INH,		0,	0x47	},
    {	NULL,	"asla",		S_INH,		0,	0x48	},
    {	NULL,	"lsla",		S_INH,		0,	0x48	},
    {	NULL,	"rola",		S_INH,		0,	0x49	},
    {	NULL,	"deca",		S_INH,		0,	0x4A	},
    {	NULL,	"inca",		S_INH,		0,	0x4C	},
    {	NULL,	"tsta",		S_INH,		0,	0x4D	},
    {	NULL,	"clra",		S_INH,		0,	0x4F	},

    {	NULL,	"negx",		S_INH,		0,	0x50	},
    {	NULL,	"div",		S_INH,		0,	0x52	},
    {	NULL,	"comx",		S_INH,		0,	0x53	},
    {	NULL,	"lsrx",		S_INH,		0,	0x54	},
    {	NULL,	"rorx",		S_INH,		0,	0x56	},
    {	NULL,	"asrx",		S_INH,		0,	0x57	},
    {	NULL,	"aslx",		S_INH,		0,	0x58	},
    {	NULL,	"lslx",		S_INH,		0,	0x58	},
    {	NULL,	"rolx",		S_INH,		0,	0x59	},
    {	NULL,	"decx",		S_INH,		0,	0x5A	},
    {	NULL,	"incx",		S_INH,		0,	0x5C	},
    {	NULL,	"tstx",		S_INH,		0,	0x5D	},
    {	NULL,	"clrx",		S_INH,		0,	0x5F	},

    {	NULL,	"nsa",		S_INH,		0,	0x62	},

    {	NULL,	"daa",		S_INH,		0,	0x72	},

    {	NULL,	"rti",		S_INH,		0,	0x80	},
    {	NULL,	"rts",		S_INH,		0,	0x81	},
    {	NULL,	"swi",		S_INH,		0,	0x83	},
    {	NULL,	"tap",		S_INH,		0,	0x84	},
    {	NULL,	"tpa",		S_INH,		0,	0x85	},
    {	NULL,	"pula",		S_INH,		0,	0x86	},
    {	NULL,	"psha",		S_INH,		0,	0x87	},
    {	NULL,	"pulx",		S_INH,		0,	0x88	},
    {	NULL,	"pshx",		S_INH,		0,	0x89	},
    {	NULL,	"pulh",		S_INH,		0,	0x8A	},
    {	NULL,	"pshh",		S_INH,		0,	0x8B	},
    {	NULL,	"clrh",		S_INH,		0,	0x8C	},
    {	NULL,	"stop",		S_INH,		0,	0x8E	},
    {	NULL,	"wait",		S_INH,		0,	0x8F	},

    {	NULL,	"txs",		S_INH,		0,	0x94	},
    {	NULL,	"tsx",		S_INH,		0,	0x95	},
    {	NULL,	"tax",		S_INH,		0,	0x97	},
    {	NULL,	"clc",		S_INH,		0,	0x98	},
    {	NULL,	"sec",		S_INH,		0,	0x99	},
    {	NULL,	"cli",		S_INH,		0,	0x9A	},
    {	NULL,	"sei",		S_INH,		0,	0x9B	},
    {	NULL,	"rsp",		S_INH,		0,	0x9C	},
    {	NULL,	"nop",		S_INH,		0,	0x9D	},
    {	NULL,	"txa",		S_INH,		S_END,	0x9F	}
};
