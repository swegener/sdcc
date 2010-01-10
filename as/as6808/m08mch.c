/* m08mch.c

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
	op = mp->m_valu;
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
			v1 = e1.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e1, R_PCR);
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
			outrb(&e1, R_PAG0);
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
			outrb(&e1, R_USGN);
			break;
		}
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9e);
			outab(op+0x30);
			outrb(&e1, R_USGN);
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
			outrb(&e1, R_PAG0);
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
			outrb(&e1, R_USGN);
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
			outrb(&e1, R_USGN);
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
		outrb(&e2, R_PAG0);
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
		outrb(&e2, R_PAG0);
		if (mchpcr(&e3)) {
			v1 = e3.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e3, R_PCR);
		}
		if (e3.e_mode != S_USER)
			rerr();
		break;

	case S_TYPAI:
		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			outab(op);
			if (e1.e_flag == 0 && e1.e_base.e_ap == NULL) {
				v1 = e1.e_addr;
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
		t1 = addr(&e1);
		if (t1 == S_IMMED) {
			if (op == 0x25)
				aerr();
			outab(op);
			outrw(&e1, 0);
			break;
		}
		if (t1 == S_DIR || t1 == S_EXT) {
			outab(op | 0x10);
			outrb(&e1, R_PAG0);
			break;
		}
		aerr();
		break;

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
			outrb(&e1, R_PAG0);
		} else
		if (t1 == S_IXP) {
			outab(op+0x40);
		} else
		if (t1 == S_IX1P || t1 == S_IX2P) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x30);
			outrb(&e1, R_USGN);
		} else
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9E);
			outab(op+0x30);
			outrb(&e1, R_USGN);
		} else {
			aerr();
			break;
		}
		if (mchpcr(&e2)) {
			v1 = e2.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R_PCR);
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
			v1 = e2.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R_PCR);
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
			outrb(&e1, R_PAG0);
		} else
		if (t1 == S_IX) {
			outab(op+0x40);
		} else
		if (t1 == S_IX1 || t1 == S_IX2) {
			if (chkindx(&e1))
				aerr();
			outab(op+0x30);
			outrb(&e1, R_USGN);
		} else
		if (t1 == S_SP1 || t1 == S_SP2) {
			if (chkindx(&e1))
				aerr();
			outab(0x9E);
			outab(op+0x30);
			outrb(&e1, R_USGN);
		} else {
			aerr();
			break;
		}
		if (mchpcr(&e2)) {
			v1 = e2.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e2, R_PCR);
		}
		if (e2.e_mode != S_USER)
			rerr();
		break;

	case S_DZAX:
		expr(&e1, 0);
		outab(op);
		if (mchpcr(&e1)) {
			v1 = e1.e_addr - dot.s_addr - 1;
			if ((v1 < -128) || (v1 > 127))
				aerr();
			outab(v1);
		} else {
			outrb(&e1, R_PCR);
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
			outrb(&e1, R_PAG0);
			break;
		}
		comma(1);
		t2 = addr(&e2);
		if (t1 == S_IMMED) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op+0x20);
				outrb(&e1, 0);
				outrb(&e2, R_PAG0);
				break;
			}
		}
		if (t1 == S_DIR || t1 == S_EXT) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op);
				outrb(&e1, R_PAG0);
				outrb(&e2, R_PAG0);
				break;
			}
		}
		if (t1 == S_IXP) {
			if (t2 == S_DIR || t2 == S_EXT) {
				outab(op+0x30);
				outrb(&e2, R_PAG0);
				break;
			}
		}
		aerr();
		break;

	default:
		err('o');
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
}
