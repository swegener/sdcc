/* m08mch.c */

/*
 *  Copyright (C) 1993-2009  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include "asxxxx.h"
#include "m6808.h"

int allow_cs08_code;

char	*cpu	= "Motorola 68HC(S)08";
int	hilo	= 1;
char	*dsft	= "asm";

/*
 * Process a machine op.
 */
VOID
machine(mp)
struct mne *mp;
{
	int op, t1, t2, type;
	struct expr e1, e2, e3;
	a_uint espv;
	struct area *espa;
	char id[NCPS];
	int c, v1;

	clrexpr(&e1);
	clrexpr(&e2);
	clrexpr(&e3);
	op = (int) mp->m_valu;
	type = mp->m_type;
	switch (type) {

	case S_SDP:
		espa = NULL;
		if (more()) {
			expr(&e1, 0);
			if (e1.e_flag == 0 && e1.e_base.e_ap == NULL) {
				if (e1.e_addr) {
					err('b');
				}
			}
			if ((c = getnb()) == ',') {
				getid(id, -1);
				espa = alookup(id);
				if (espa == NULL) {
					err('u');
				}
			} else {
				unget(c);
			}
		}
		if (espa) {
			outdp(espa, &e1);
		} else {
			outdp(dot.s_area, &e1);
		}
		lmode = SLIST;
		break;

	case S_INH:
		outab(op);
		break;

	case S_BRA:
		expr(&e1, 0);
		outab(op);
		if (mchpcr(&e1)) {
			v1 = (int) (e1.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e1, R3_PCR);
		}
		if (e1.e_mode != S_USER)
			rerr();
		break;

	case S_TYP1:
		t1 = addr(&e1);
		if (t1 == S_A) {
			outab(op+0x10);
			break;
		}
		if (t1 == S_X) {
			outab(op+0x20);
			break;
		}
		if (t1 == S_DIR || t1 == S_EXT) {
			outab(op);
			outrb(&e1, R3_PAG0);
			break;
		}
		if (t1 == S_IX) {
			outab(op+0x40);
			break;
		}
		if (t1 == S_IX1 || t1 == S_IX2) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x30);
			outrb(&e1, R3_USGN);
			break;
		}
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9e);
			outab(op+0x30);
			outrb(&e1, R3_USGN);
			break;
		}
		aerr();
		break;

	case S_TYP2:
		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			if ((op == 0xA7) || (op == 0xAC) ||
			    (op == 0xAD) || (op == 0xAF))
				aerr();
			outab(op);
			outrb(&e1, 0);
			break;
		}
		if (t1 == S_DIR) {
			outab(op+0x10);
			outrb(&e1, R3_PAG0);
			break;
		}
		if (t1 == S_EXT) {
			outab(op+0x20);
			outrw(&e1, 0);
			break;
		}
		if (t1 == S_IX) {
			outab(op+0x50);
			break;
		}
		if (t1 == S_IX1) {
			outab(op+0x40);
			outrb(&e1, R3_USGN);
			break;
		}
		if (t1 == S_IX2) {
			outab(op+0x30);
			outrw(&e1, 0);
			break;
		}
		if (t1 == S_SP1) {
			if (op == 0xAC || op == 0xAD)
				aerr();
			outab(0x9e);
			outab(op+0x40);
			outrb(&e1, R3_USGN);
			break;
		}
		if (t1 == S_SP2) {
			if (op == 0xAC || op == 0xAD)
				aerr();
			outab(0x9e);
			outab(op+0x30);
			outrw(&e1, 0);
			break;
		}
		aerr();
		break;

	case S_TYP3:
		t1 = addr(&e1);
		espv = e1.e_addr;
		if (t1 != S_IMMED || espv & ~0x07)
			aerr();
		comma(1);
		t2 = addr(&e2);
		if (t2 != S_DIR)
			aerr();
		outab(op + 2*(espv&0x07));
		outrb(&e2, R3_PAG0);
		break;

	case S_TYP4:
		t1 = addr(&e1);
		espv = e1.e_addr;
		if (t1 != S_IMMED || espv & ~0x07)
			aerr();
		comma(1);
		t2 = addr(&e2);
		if (t2 != S_DIR)
			aerr();
		comma(1);
		expr(&e3, 0);
		outab(op + 2*(espv&0x07));
		outrb(&e2, R3_PAG0);
		if (mchpcr(&e3)) {
			v1 = (int) (e3.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e3, R3_PCR);
		}
		if (e3.e_mode != S_USER)
			rerr();
		break;

	case S_TYPAI:
		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			outab(op);
			if (e1.e_flag == 0 && e1.e_base.e_ap == NULL) {
				v1 = (int) e1.e_addr;
				if ((v1 < -128) || (v1 > 127))
					aerr();
				outab(v1);
			} else {
				outrb(&e1, 0);
			}
			break;
		}
		aerr();
		break;

	case S_TYPHX:
// Code below from Gary Osborne (gary@s-4.com)
// G.O.
// This block of code was copied from a later version of the asxxxx assembler
// distribution.  The later version supports the cs08.  The SDCC version of
// the assembler did not.  The cs08 ldhx, sthx, and cphx instructions allow
// more addressing modes than the hc08.  That is the only difference in the
// instruction sets.

// The new asxxxx assemblers have other nice features that could be retrofitted
// one day, but we can limp along without them for a few years yet.  They
// are less important when the primary user writes in C.  It would probably
// be easier to customize the new asxxxx files rather than try to upgrade
// the SDCC version.

		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			if (op == 0x25)
			aerr();
			outab(op);
			outrw(&e1, 0);
			break;
		}
		if (allow_cs08_code) {
			if (t1 == S_EXT) {
				switch (op) {
					default:
					case 0x25:  outab(0x96);   break;
					case 0x45:  outab(0x32);   break;
					case 0x65:  outab(0x3E);   break;
				}
				outrw(&e1, 0);
				break;
			}
			if ((t1 == S_SP1) || (t1 == S_SP2)) {
				outab(0x9E);
				switch (op) {
				default:
				case 0x25:  outab(0xFF);   break;
				case 0x45:  outab(0xFE);   break;
				case 0x65:  outab(0xF3);   break;
				}
				outrb(&e1, R3_USGN);
				// The SDCC version of outrb() is not fully compatible.  Work around it.
				// Not an asxxxx error.
				if (t1==S_SP2) {      // ldhx 0x100,s etc should show error tag
					aerr();
				}
				break;
			}
			if ((t1 == S_IX) && (op == 0x45)) {
				outab(0x9E);
				outab(0xAE);
				break;
			}
			if ((t1 == S_IX1) && (op == 0x45)) {
				outab(0x9E);
				outab(0xCE);
				outrb(&e1, R3_USGN);
				break;
			}
			if ((t1 == S_IX2) && (op == 0x45)) {
				outab(0x9E);
				outab(0xBE);
				outrw(&e1, 0);
				break;
			}
		} else {
			if (t1 == S_EXT) {
				t1 = S_DIR;
				// The SDCC version of outrb() is not fully compatible.  Work around it.
				// Not an asxxxx error, at least not in the newer versions.
				if (e1.e_addr > 0xFF) {
					err('d');
				}
			}
		}
		if (t1 == S_DIR) {
			outab(op | 0x10);
			outrb(&e1, R3_PAG0);
			break;
		}
		aerr();
		break;

// end G.O.

	case S_CBEQ:
		t1 = addr(&e1);
		comma(1);
		expr(&e2, 0);
		if (t1 == S_IMMED) {
			outab(op);
			outrb(&e1, 0);
		} else
		if (t1 == S_DIR || t1 == S_EXT) {
			outab(op);
			outrb(&e1, R3_PAG0);
		} else
		if (t1 == S_IXP) {
			outab(op+0x40);
		} else
		if (t1 == S_IX1P || t1 == S_IX2P) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x30);
			outrb(&e1, R3_USGN);
		} else
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9E);
			outab(op+0x30);
			outrb(&e1, R3_USGN);
		} else {
			aerr();
			break;
		}
		if (mchpcr(&e2)) {
			v1 = (int) (e2.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R3_PCR);
		}
		if (e2.e_mode != S_USER)
			rerr();
		break;

	case S_CQAX:
		t1 = addr(&e1);
		if (t1 != S_IMMED)
			aerr();
		comma(1);
		expr(&e2, 0);
		outab(op);
		outrb(&e1, 0);
		if (mchpcr(&e2)) {
			v1 = (int) (e2.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R3_PCR);
		}
		if (e2.e_mode != S_USER)
			rerr();
		break;

	case S_DBNZ:
		t1 = addr(&e1);
		comma(1);
		expr(&e2, 0);
		if (t1 == S_DIR || t1 == S_EXT) {
			outab(op);
			outrb(&e1, R3_PAG0);
		} else
		if (t1 == S_IX) {
			outab(op+0x40);
		} else
		if (t1 == S_IX1 || t1 == S_IX2) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x30);
			outrb(&e1, R3_USGN);
		} else
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9E);
			outab(op+0x30);
			outrb(&e1, R3_USGN);
		} else {
			aerr();
			break;
		}
		if (mchpcr(&e2)) {
			v1 = (int) (e2.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R3_PCR);
		}
		if (e2.e_mode != S_USER)
			rerr();
		break;

	case S_DZAX:
		expr(&e1, 0);
		outab(op);
		if (mchpcr(&e1)) {
			v1 = (int) (e1.e_addr - dot.s_addr - 1);
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e1, R3_PCR);
		}
		if (e1.e_mode != S_USER)
			rerr();
		break;

	case S_MOV:
		t1 = addr(&e1);
		if (t1 == S_IX1P || t1 == S_IX2P) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x10);
			outrb(&e1, R3_PAG0);
			break;
		}
		comma(1);
		t2 = addr(&e2);
		if (t1 == S_IMMED) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op+0x20);
				outrb(&e1, 0);
				outrb(&e2, R3_PAG0);
				break;
			}
		}
		if (t1 == S_DIR || t1 == S_EXT) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op);
				outrb(&e1, R3_PAG0);
				outrb(&e2, R3_PAG0);
				break;
			}
		}
		if (t1 == S_IXP) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op+0x30);
				outrb(&e2, R3_PAG0);
				break;
			}
		}
		aerr();
		break;

	case X_CS08:
		++allow_cs08_code;
		break;

	default:
		err('o');
		break;
	}
}

/*
 * Check index byte
 */
int
chkindx(exp)
struct expr *exp;
{
	if (exp->e_flag == 0 && exp->e_base.e_ap == NULL) {
		if (exp->e_addr & ~0xFF) {
			return(1);
		}
	}
	return(0);
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
 * Machine specific initialization.
 */
VOID
minit()
{
	/*
	 * Byte Order
	 */
	hilo = 1;

}
