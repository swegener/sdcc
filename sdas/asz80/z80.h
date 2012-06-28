/* z80.h

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

/*)BUILD
	$(PROGRAM) =	ASZ80
	$(INCLUDE) = {
		ASXXXX.H
		Z80.H
	}
	$(FILES) = {
		Z80EXT.C
		Z80MCH.C
		Z80ADR.C
		Z80PST.C
		ASMAIN.C
		ASLEX.C
		ASSYM.C
		ASSUBR.C
		ASEXPR.C
		ASDATA.C
		ASLIST.C
		ASOUT.C
	}
	$(STACK) = 3000
*/

/*
 * Indirect Addressing delimeters
 */
#define	LFIND	'('
#define RTIND	')'

/*
 * Registers
 */
#define B	0
#define C	1
#define D	2
#define E	3
#define H	4
#define L	5
#define A	7

#define I	0107
#define R	0117

#define BC	0
#define DE	1
#define HL	2
#define SP	3
#define AF	4
#define IX	5
#define IY	6

/*
 * Conditional definitions
 */
#define	NZ	0
#define	Z	1
#define	NC	2
#define	CS	3
#define	PO	4
#define	PE	5
#define	P	6
#define	M	7

/*
 * Symbol types
 */
#define	S_IMMED	30
#define	S_R8	31
#define	S_R8X	32
#define	S_R16	33
#define	S_R16X	34
#define	S_CND	35
#define	S_FLAG	36

/*
 * Indexing modes
 */
#define	S_INDB	40
#define	S_IDC	41
#define	S_INDR	50
#define	S_IDBC	50
#define	S_IDDE	51
#define	S_IDHL	52
#define	S_IDSP	53
#define	S_IDIX	55
#define	S_IDIY	56
#define	S_INDM	57

/*
 * Instruction types
 */
#define	S_LD	60
#define	S_CALL	61
#define	S_JP	62
#define	S_JR	63
#define	S_RET	64
#define	S_BIT	65
#define	S_INC	66
#define	S_DEC	67
#define	S_ADD	68
#define	S_ADC	69
#define	S_AND	70
#define	S_EX	71
#define	S_PUSH	72
#define	S_IN	73
#define	S_OUT	74
#define	S_RL	75
#define	S_RST	76
#define	S_IM	77
#define	S_INH1	78
#define	S_INH2	79
#define	S_DJNZ	80
#define	S_SUB	81
#define	S_SBC	82

/*
 * HD64180 Instructions
 */
#define	X_HD64	90
#define	X_INH2	91
#define	X_IN	92
#define	X_OUT	93
#define	X_MLT	94
#define	X_TST	95
#define	X_TSTIO	96

struct adsym
{
	char	a_str[4];	/* addressing string */
	int	a_val;		/* addressing mode value */
};

extern	struct	adsym	R8[];
extern	struct	adsym	R8X[];
extern	struct	adsym	R16[];
extern	struct	adsym	R16X[];
extern	struct	adsym	CND[];

	/* machine dependent functions */

#ifdef	OTHERSYSTEM
	
	/* z80adr.c */
extern	int		addr(struct expr *esp);
extern	int		admode(struct adsym *sp);
extern	int		any(char c, char *str);
extern	int		srch(char *str);


	/* z80mch.c */
extern	int		genop(int pop, int op, struct expr *esp, int f);
extern	int		gixiy(int v);
extern	int		mchpcr(struct expr *esp);

#else

	/* z80adr.c */
extern	int		addr();
extern	int		admode();
extern	int		any();
extern	int		srch();

	/* z80mch.c */
extern	int		genop();
extern	int		gixiy();
extern	int		mchpcr();

#endif

