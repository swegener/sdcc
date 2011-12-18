/* z80mch.c

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
 */

#include "asxxxx.h"
#include "z80.h"

char	imtab[3] = { 0x46, 0x56, 0x5E };
int	hd64;

/*
 * Process a machine op.
 */
VOID
machine(mp)
struct mne *mp;
{
	int op, t1, t2;
	struct expr e1, e2;
	int rf, v1, v2;

	clrexpr(&e1);
	clrexpr(&e2);
	op = (int) mp->m_valu;
	rf = mp->m_type;
	if (!hd64 && rf>X_HD64)
		rf = 0;
	switch (rf) {

	case S_INH1:
		outab(op);
		break;

	case S_INH2:
		outab(0xED);
		outab(op);
		break;

	case S_RET:
		if (more()) {
			if ((v1 = admode(CND)) != 0) {
				outab(op | (v1<<3));
			} else {
				qerr();
			}
		} else {
			outab(0xC9);
		}
		break;

	case S_PUSH:
		if (admode(R16X)) {
			outab(op+0x30);
			break;
		} else
		if ((v1 = admode(R16)) != 0 && (v1 &= 0xFF) != SP) {
			if (v1 != gixiy(v1)) {
				outab(op+0x20);
				break;
			}
			outab(op | (v1<<4));
			break;
		}
		aerr();
		break;

	case S_RST:
		v1 = (int) absexpr();
		if (v1 & ~0x38) {
			aerr();
			v1 = 0;
		}
		outab(op|v1);
		break;

	case S_IM:
		expr(&e1, 0);
		abscheck(&e1);
		if (e1.e_addr > 2) {
			aerr();
			e1.e_addr = 0;
		}
		outab(op);
		outab(imtab[(int) e1.e_addr]);
		break;

	case S_BIT:
		expr(&e1, 0);
		t1 = 0;
		v1 = (int) e1.e_addr;
		if (v1 > 7) {
			++t1;
			v1 &= 0x07;
		}
		op |= (v1<<3);
		comma(1);
		addr(&e2);
		abscheck(&e1);
		if (genop(0xCB, op, &e2, 0) || t1)
			aerr();
		break;

	case S_RL:
		t1 = 0;
		t2 = addr(&e2);
		if (more()) {
			if ((t2 != S_R8) || (e2.e_addr != A))
				++t1;
			comma(1);
			clrexpr(&e2);
			t2 = addr(&e2);
		}
		if (genop(0xCB, op, &e2, 0) || t1)
			aerr();
		break;

	case S_AND:
	case S_SUB:
		t1 = 0;
		t2 = addr(&e2);
		if (more()) {
			if ((t2 != S_R8) || (e2.e_addr != A))
				++t1;
			comma(1);
			clrexpr(&e2);
			t2 = addr(&e2);
		}
		if (genop(0, op, &e2, 1) || t1)
			aerr();
		break;

	case S_ADD:
	case S_ADC:
	case S_SBC:
		t1 = addr(&e1);
		t2 = 0;
		if (more()) {
			comma(1);
			t2 = addr(&e2);
		}
		if (t2 == 0) {
			if (genop(0, op, &e1, 1))
				aerr();
			break;
		}
		if ((t1 == S_R8) && (e1.e_addr == A)) {
			if (genop(0, op, &e2, 1))
				aerr();
			break;
		}
		if ((t1 == S_R16) && (t2 == S_R16)) {
			if (rf == S_ADD)
				op = 0x09;
			if (rf == S_ADC)
				op = 0x4A;
			if (rf == S_SBC)
				op = 0x42;
			v1 = (int) e1.e_addr;
			v2 = (int) e2.e_addr;
			if ((v1 == HL) && (v2 <= SP)) {
				if (rf != S_ADD)
					outab(0xED);
				outab(op | (v2<<4));
				break;
			}
			if (rf != S_ADD) {
				aerr();
				break;
			}
			if ((v1 == IX) && (v2 != HL) && (v2 != IY)) {
				if (v2 == IX)
					v2 = HL;
				outab(0xDD);
				outab(op | (v2<<4));
				break;
			}
			if ((v1 == IY) && (v2 != HL) && (v2 != IX)) {
				if (v2 == IY)
					v2 = HL;
				outab(0xFD);
				outab(op | (v2<<4));
				break;
			}
		}
		aerr();
		break;

	case S_LD:
		t1 = addr(&e1);
		comma(1);
		t2 = addr(&e2);
		if (t1 == S_R8) {
			v1 = op | e1.e_addr<<3;
			if (genop(0, v1, &e2, 0) == 0)
				break;
			if (t2 == S_IMMED) {
				outab((e1.e_addr<<3) | 0x06);
				outrb(&e2,0);
				break;
			}
		}
		v1 = (int) e1.e_addr;
		v2 = (int) e2.e_addr;
		if ((t1 == S_R16) && (t2 == S_IMMED)) {
			v1 = gixiy(v1);
			outab(0x01|(v1<<4));
			outrw(&e2, 0);
			break;
		}
		if ((t1 == S_R16) && (t2 == S_INDM)) {
			if (gixiy(v1) == HL) {
				outab(0x2A);
			} else {
				outab(0xED);
				outab(0x4B | (v1<<4));
			}
			outrw(&e2, 0);
			break;
		}
		if ((t1 == S_INDM) && (t2 == S_R16)) {
			if (gixiy(v2) == HL) {
				outab(0x22);
			} else {
				outab(0xED);
				outab(0x43 | (v2<<4));
			}
			outrw(&e1, 0);
			break;
		}
		if ((t1 == S_R8) && (v1 == A) && (t2 == S_INDM)) {
			outab(0x3A);
			outrw(&e2, 0);
			break;
		}
		if ((t1 == S_INDM) && (t2 == S_R8) && (v2 == A)) {
			outab(0x32);
			outrw(&e1, 0);
			break;
		}
		if ((t2 == S_R8) && (gixiy(t1) == S_IDHL)) {
			outab(0x70|v2);
			if (t1 != S_IDHL)
				outrb(&e1, 0);
			break;
		}
		if ((t2 == S_IMMED) && (gixiy(t1) == S_IDHL)) {
			outab(0x36);
			if (t1 != S_IDHL)
				outrb(&e1, 0);
			outrb(&e2, 0);
			break;
		}
		if ((t1 == S_R8X) && (t2 == S_R8) && (v2 == A)) {
			outab(0xED);
			outab(v1);
			break;
		}
		if ((t1 == S_R8) && (v1 == A) && (t2 == S_R8X)) {
			outab(0xED);
			outab(v2|0x10);
			break;
		}
		if ((t1 == S_R16) && (v1 == SP)) {
			if ((t2 == S_R16) && (gixiy(v2) == HL)) {
				outab(0xF9);
				break;
			}
		}
		if ((t1 == S_R8) && (v1 == A)) {
			if ((t2 == S_IDBC) || (t2 == S_IDDE)) {
				outab(0x0A | ((t2-S_INDR)<<4));
				break;
			}
		}
		if ((t2 == S_R8) && (v2 == A)) {
			if ((t1 == S_IDBC) || (t1 == S_IDDE)) {
				outab(0x02 | ((t1-S_INDR)<<4));
				break;
			}
		}
		aerr();
		break;




	case S_EX:
		t1 = addr(&e1);
		comma(1);
		t2 = addr(&e2);
		if (t2 == S_R16) {
			v1 = (int) e1.e_addr;
			v2 = (int) e2.e_addr;
			if ((t1 == S_IDSP) && (v1 == 0)) {
				if (gixiy(v2) == HL) {
					outab(op);
					break;
				}
			}
			if (t1 == S_R16) {
				if ((v1 == DE) && (v2 == HL)) {
					outab(0xEB);
					break;
				}
			}
		}
		if ((t1 == S_R16X) && (t2 == S_R16X)) {
			outab(0x08);
			break;
		}
		aerr();
		break;

	case S_IN:
	case S_OUT:
		if (rf == S_IN) {
			t1 = addr(&e1);
			comma(1);
			t2 = addr(&e2);
		} else {
			t2 = addr(&e2);
			comma(1);
			t1 = addr(&e1);
		}
		v1 = (int) e1.e_addr;
		v2 = (int) e2.e_addr;
		if (t1 == S_R8) {
			if ((v1 == A) && (t2 == S_INDM)) {
				outab(op);
				outab(v2);
				break;
			}
			if (t2 == S_IDC) {
				outab(0xED);
				outab(((rf == S_IN) ? 0x40 : 0x41) + (v1<<3));
				break;
			}
		}
		aerr();
		break;

	case S_DEC:
	case S_INC:
		t1 = addr(&e1);
		v1 = (int) e1.e_addr;
		if (t1 == S_R8) {
			outab(op|(v1<<3));
			break;
		}
		if (t1 == S_IDHL) {
			outab(op|0x30);
			break;
		}
		if (t1 != gixiy(t1)) {
			outab(op|0x30);
			outrb(&e1,0);
			break;
		}
		if (t1 == S_R16) {
			v1 = gixiy(v1);
			if (rf == S_INC) {
				outab(0x03|(v1<<4));
				break;
			}
			if (rf == S_DEC) {
				outab(0x0B|(v1<<4));
				break;
			}
		}
		aerr();
		break;

	case S_DJNZ:
	case S_JR:
		if ((v1 = admode(CND)) != 0 && rf != S_DJNZ) {
			if ((v1 &= 0xFF) <= 0x03) {
				op += (v1+1)<<3;
			} else {
				aerr();
			}
			comma(1);
		}
		expr(&e2, 0);
		outab(op);
		if (mchpcr(&e2)) {
			v2 = (int) (e2.e_addr - dot.s_addr - 1);
			if (pass == 2 && ((v2 < -128) || (v2 > 127)))
				aerr();
			outab(v2);
		} else {
			outrb(&e2, R3_PCR);
		}
		if (e2.e_mode != S_USER)
			rerr();
		break;

	case S_CALL:
		if ((v1 = admode(CND)) != 0) {
			op |= (v1&0xFF)<<3;
			comma(1);
		} else {
			op = 0xCD;
		}
		expr(&e1, 0);
		outab(op);
		outrw(&e1, 0);
		break;

	case S_JP:
		if ((v1 = admode(CND)) != 0) {
			op |= (v1&0xFF)<<3;
			comma(1);
			expr(&e1, 0);
			outab(op);
			outrw(&e1, 0);
			break;
		}
		t1 = addr(&e1);
		if (t1 == S_USER) {
			outab(0xC3);
			outrw(&e1, 0);
			break;
		}
		if ((e1.e_addr == 0) && (gixiy(t1) == S_IDHL)) {
			outab(0xE9);
			break;
		}
		aerr();
		break;

	case X_HD64:
		++hd64;
		break;

	case X_INH2:
		outab(0xED);
		outab(op);
		break;

	case X_IN:
	case X_OUT:
		if (rf == X_IN) {
			t1 = addr(&e1);
			comma(1);
			t2 = addr(&e2);
		} else {
			t2 = addr(&e2);
			comma(1);
			t1 = addr(&e1);
		}
		if ((t1 == S_R8) && (t2 == S_INDM)) {
			outab(0xED);
			outab(op | (e1.e_addr<<3));
			outrb(&e2, 0);
			break;
		}
		aerr();
		break;

	case X_MLT:
		t1 = addr(&e1);
		if ((t1 == S_R16) && ((v1 = (int) e1.e_addr) <= SP)) {
			outab(0xED);
			outab(op | (v1<<4));
			break;
		}
		aerr();
		break;

	case X_TST:
		t1 = addr(&e1);
		if (t1 == S_R8) {
			outab(0xED);
			outab(op | (e1.e_addr<<3));
			break;
		}
		if (t1 == S_IDHL) {
			outab(0xED);
			outab(0x34);
			break;
		}
		if (t1 == S_IMMED) {
			outab(0xED);
			outab(0x64);
			outrb(&e1, 0);
			break;
		}
		aerr();
		break;

	case X_TSTIO:
		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			outab(0xED);
			outab(op);
			outrb(&e1, 0);
			break;
		}
		aerr();
		break;

	default:
		err('o');
	}
}

/*
 * general addressing evaluation
 * return(0) if general addressing mode output, else
 * return(esp->e_mode)
 */
int
genop(pop, op, esp, f)
int pop, op;
struct expr *esp;
int f;
{
	int t1;
	if ((t1 = esp->e_mode) == S_R8) {
		if (pop)
			outab(pop);
		outab(op|esp->e_addr);
		return(0);
	}
	if (t1 == S_IDHL) {
		if (pop)
			outab(pop);
		outab(op|0x06);
		return(0);
	}
	if (gixiy(t1) == S_IDHL) {
		if (pop) {
			outab(pop);
			outrb(esp,0);
			outab(op|0x06);
		} else {
			outab(op|0x06);
			outrb(esp,0);
		}
		return(0);
	}
	if ((t1 == S_IMMED) && (f)) {
		if (pop)
			outab(pop);
		outab(op|0x46);
		outrb(esp,0);
		return(0);
	}
	return(t1);
}

/*
 * IX and IY prebyte check
 */
int
gixiy(v)
int v;
{
	if (v == IX) {
		v = HL;
		outab(0xDD);
	} else if (v == IY) {
		v = HL;
		outab(0xFD);
	} else if (v == S_IDIX) {
		v = S_IDHL;
		outab(0xDD);
	} else if (v == S_IDIY) {
		v = S_IDHL;
		outab(0xFD);
	}
	return(v);
}

/*
 * Branch/Jump PCR Mode Check
 */
int
mchpcr(esp)
struct expr *esp;
{
	if (esp->e_base.e_ap == dot.s_area) {
		return(1);
	}
	if (esp->e_flag==0 && esp->e_base.e_ap==NULL) {
		/*
		 * Absolute Destination
		 *
		 * Use the global symbol '.__.ABS.'
		 * of value zero and force the assembler
		 * to use this absolute constant as the
		 * base value for the relocation.
		 */
		esp->e_flag = 1;
		esp->e_base.e_sp = &sym[1];
	}
	return(0);
}

/*
 * Machine dependent initialization
 */
VOID
minit()
{
	hd64 = 0;
}
