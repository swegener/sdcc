/* lklist.c */

/*
 * (C) Copyright 1989-1995
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 * 28-Oct-97 JLH: 
 *           - lstarea: show s_id as string rather than array [NCPS]
 *           - lstarea: show a_id as string rather than array [NCPS]
 * 31-Oct-97 JLH: add NoICE output file genration in lstarea
 * 02-Apr-98 JLH: add XDATA, DATA, BIT flags to area output
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aslink.h"

/*)Module	lklist.c
 *
 *	The module lklist.c contains the functions which
 *	output the linker .map file and produce a relocated
 *	listing .rst file.
 *
 *	lklist.c contains the following functions:
 *		int	dgt()
 *		VOID	lstarea()
 *		VOID	lkulist()
 *		VOID	lkalist()
 *		VOID	lkglist()
 *		VOID	newpag()
 *		VOID	slew()
 *
 *	lklist.c contains no local variables.
 */

/*)Function	VOID	slew(fp)
 *
 *		FILE *	fp		output file handle
 *
 *	The function slew() increments the page line counter.
 *	If the number of lines exceeds the maximum number of
 *	lines per page then a page skip and a page header are
 *	output.
 *
 *	local variables:
 *		int	i		loop counter
 *
 *	global variables:
 *		int	lop		current line number on page
 *		int	xflag		Map file radix type flag
 *
 *	functions called:
 *		int	fprintf()	c_library
 *		VOID	newpag()	lklist.c
 *
 *	side effects:
 *		The page line and the page count may be updated.
 */

VOID
slew(fp)
FILE *fp;
{
	register int i;

	if (lop++ >= NLPP) {
		newpag(fp);
		if (xflag == 0) {
			fprintf(fp, "Hexadecimal\n\n");
		} else
		if (xflag == 1) {
			fprintf(fp, "Octal\n\n");
		} else
		if (xflag == 2) {
			fprintf(fp, "Decimal\n\n");
		}
		fprintf(fp, "Area       Addr   Size");
		fprintf(fp, "   Decimal Bytes (Attributes)\n");
		for(i=0;i<4;++i)
			fprintf(fp, "      Value--Global");
		fprintf(fp, "\n\n");
		lop += 6;
	}
}

/*)Function	VOID	newpag()
 *
 *	The function newpag() outputs a page skip, writes the
 *	first page header line, sets the line count to 1, and
 *	increments the page counter.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		int	lop		current line number on page
 *		int	page		current page number
 *
 *	functions called:
 *		int	fprintf()	c_library
 *
 *	side effects:
 *		The page and line counters are updated.
 */

VOID
newpag(fp)
FILE *fp;
{
	fprintf(fp, "\fASxxxx Linker %s,  page %u.\n", VERSION, ++page);
	lop = 1;
}

/* Used for qsort call in lstsym */
static int _cmpSymByAddr(const void *p1, const void *p2)
{
    struct sym **s1 = (struct sym **)(p1);
    struct sym **s2 = (struct sym **)(p2);
    int delta = ((*s1)->s_addr + (*s1)->s_axp->a_addr) -
     	   	((*s2)->s_addr + (*s2)->s_axp->a_addr);

    /* Sort first by address, then by name. */
    if (delta)
    {
    	return delta;
    }
    return strcmp((*s1)->s_id,(*s2)->s_id);    
}


#if	NCPS-8

/* NCPS != 8 */
/*)Function	VOID	lstarea(xp)
 *
 *		area *	xp		pointer to an area structure
 *
 *	The function lstarea() creates the linker map output for
 *	the area specified by pointer xp.  The generated output
 *	area header includes the area name, starting address,
 *	size of area, number of words (in decimal), and the
 *	area attributes.  The symbols defined in this area are
 *	sorted by ascending address and output one per line
 *	in the selected radix.
 *
 *	local variables:
 *		areax *	oxp		pointer to an area extension structure
 *		int	c		character value
 *		int	i		loop counter
 *		int	j		bubble sort update status
 *		char *	ptr		pointer to an id string
 *		int	nmsym		number of symbols in area
 *		Addr_T	a0		temporary
 *		Addr_T	ai		temporary
 *		Addr_T	aj		temporary
 *		sym *	sp		pointer to a symbol structure
 *		sym **	p		pointer to an array of
 *					pointers to symbol structures
 *
 *	global variables:
 *		FILE	*mfp		Map output file handle
 *		sym *symhash[NHASH] 	array of pointers to NHASH
 *				      	linked symbol lists
 *		int	xflag		Map file radix type flag
 *
 *	functions called:
 *		int	fprintf()	c_library
 *		VOID	free()		c_library
 *		char *	malloc()	c_library
 *		char	putc()		c_library
 *		VOID	slew()		lklist.c
 *
 *	side effects:
 *		Map output generated.
 */

VOID
lstarea(xp)
struct area *xp;
{
	register struct areax *oxp;
	register int i;
	/* int j; */
	register char *ptr;
	int nmsym;
	/* Addr_T a0; */
	Addr_T 	   ai, aj;
	struct sym *sp;
	struct sym **p;
	int memPage;

	putc('\n', mfp);
	if (xflag == 0) {
		fprintf(mfp, "Hexadecimal\n\n");
	} else
	if (xflag == 1) {
		fprintf(mfp, "Octal\n\n");
	} else
	if (xflag == 2) {
		fprintf(mfp, "Decimal\n\n");
	}
	fprintf(mfp, "Area                               ");
	fprintf(mfp, "Addr   Size   Decimal %s (Attributes)\n",
		(xp->a_flag & A_BIT)?"Bits ":"Bytes");/* JCF: For BIT print bits...*/
	fprintf(mfp, "--------------------------------   ");
	fprintf(mfp, "----   ----   ------- ----- ------------\n");
	/*
	 * Output Area Header
	 */
	ptr = &xp->a_id[0];
	fprintf(mfp, "%-32s", ptr );	/* JLH: width matches --- above */
	ai = xp->a_addr;
	aj = xp->a_size;
	if (xflag == 0) {
		fprintf(mfp, "   %04X   %04X", ai, aj);
	} else
	if (xflag == 1) {
		fprintf(mfp, " %06o %06o", ai, aj);
	} else
	if (xflag == 2) {
		fprintf(mfp, "  %05u  %05u", ai, aj);
	}
	fprintf(mfp, " = %6u. %s ", aj,
		(xp->a_flag & A_BIT)?"bits ":"bytes"); /* JCF: For BIT print bits...*/
	if (xp->a_flag & A_ABS) {
		fprintf(mfp, "(ABS");
	} else {
		fprintf(mfp, "(REL");
	}
	if (xp->a_flag & A_OVR) {
		fprintf(mfp, ",OVR");
	} else {
		fprintf(mfp, ",CON");
	}
	if (xp->a_flag & A_PAG) {
		fprintf(mfp, ",PAG");
	}

	memPage = 0x00;
	if (xp->a_flag & A_CODE) {
		fprintf(mfp, ",CODE");
		memPage = 0x0C;
	}
	if (xp->a_flag & A_XDATA) {
		fprintf(mfp, ",XDATA");
		memPage = 0x0D;
	}
	if (xp->a_flag & A_BIT) {
		fprintf(mfp, ",BIT");
		memPage = 0x0B;
	}
	fprintf(mfp, ")");
	if (xp->a_flag & A_PAG) {
		ai = (ai & 0xFF);
		aj = (aj > 256);
		if (ai || aj) { fprintf(mfp, "  "); }
		if (ai)      { fprintf(mfp, " Boundary"); }
		if (ai & aj)  { fprintf(mfp, " /"); }
		if (aj)      { fprintf(mfp, " Length"); }
		if (ai || aj) { fprintf(mfp, " Error"); }
	}

	/*
	 * Find number of symbols in area
	 */
	nmsym = 0;
	oxp = xp->a_axp;
	while (oxp) {
		for (i=0; i<NHASH; i++) {
			sp = symhash[i];
			while (sp != NULL) {
				if (oxp == sp->s_axp)
					++nmsym;
				sp = sp->s_sp;
			}
		}
		oxp = oxp->a_axp;
	}
	if (nmsym == 0) {
		putc('\n', mfp);
		return;
	}

	/*
	 * Allocate space for an array of pointers to symbols
	 * and load array.
	 */
	if ( (p = (struct sym **) malloc(nmsym*sizeof(struct sym *)))
		== NULL) {
		fprintf(mfp, "\nInsufficient space to build Map Segment.\n");
		return;
	}
	nmsym = 0;
	oxp = xp->a_axp;
	while (oxp) {
		for (i=0; i<NHASH; i++) {
			sp = symhash[i];
			while (sp != NULL) {
				if (oxp == sp->s_axp) {
					p[nmsym++] = sp;
				}
				sp = sp->s_sp;
			}
		}
		oxp = oxp->a_axp;
	}

#if 0
	/*
	 * Bubble Sort of Addresses in Symbol Table Array
	 */
	j = 1;
	while (j) {
		j = 0;
		sp = p[0];
		a0 = sp->s_addr + sp->s_axp->a_addr;
		for (i=1; i<nmsym; ++i) {
			sp = p[i];
			ai = sp->s_addr + sp->s_axp->a_addr;
			if (a0 > ai) {
				j = 1;
				p[i] = p[i-1];
				p[i-1] = sp;
			}
			a0 = ai;
		}
	}
#else
	qsort(p, nmsym, sizeof(struct sym *), _cmpSymByAddr);
#endif	

	/*
	 * Symbol Table Output
	 */

	i = 0;
	fprintf(mfp, "\n\n");
	fprintf(mfp, "      Value  Global\n");
	fprintf(mfp, "   --------  --------------------------------");
	while (i < nmsym) {
		fprintf(mfp, "\n");
		if (memPage != 0) 
			fprintf(mfp, "  %02X:", memPage);
		else
			fprintf(mfp, "     ");

		sp = p[i];
		aj = sp->s_addr + sp->s_axp->a_addr;
		if (xflag == 0) {
			fprintf(mfp, "%04X    ", aj);
		} else
		if (xflag == 1) {
			fprintf(mfp, "%06o  ", aj);
		} else
		if (xflag == 2) {
			fprintf(mfp, "%05u   ", aj);
		}
		ptr = &sp->s_id[0];
		fprintf(mfp, "%s", ptr );
		
		/* if cdb flag set the output cdb Information 
		   and the symbol has a '$' sign in it then */
		if (dflag &&
		    strchr(ptr,'$'))
		    fprintf(dfp,"L:%s:%X\n",ptr,aj);
		/* NoICE output of symbol */
		if (jflag) DefineNoICE( ptr, aj, memPage );

		i++;
	}
	putc('\n', mfp);
	free(p);
}

#else

/* NCPS == 8 */
/*)Function	VOID	lstarea(xp)
 *
 *		area *	xp		pointer to an area structure
 *
 *	The function lstarea() creates the linker map output for
 *	the area specified by pointer xp.  The generated output
 *	area header includes the area name, starting address,
 *	size of area, number of words (in decimal), and the
 *	area attributes.  The symbols defined in this area are
 *	sorted by ascending address and output four per line
 *	in the selected radix.
 *
 *	local variables:
 *		areax *	oxp		pointer to an area extension structure
 *		int	c		character value
 *		int	i		loop counter
 *		int	j		bubble sort update status
 *		char *	ptr		pointer to an id string
 *		int	nmsym		number of symbols in area
 *		Addr_T	a0		temporary
 *		Addr_T	ai		temporary
 *		Addr_T	aj		temporary
 *		sym *	sp		pointer to a symbol structure
 *		sym **	p		pointer to an array of
 *					pointers to symbol structures
 *
 *	global variables:
 *		FILE	*mfp		Map output file handle
 *		sym *symhash[NHASH] 	array of pointers to NHASH
 *				      	linked symbol lists
 *		int	xflag		Map file radix type flag
 *
 *	functions called:
 *		int	fprintf()	c_library
 *		VOID	free()		c_library
 *		char *	malloc()	c_library
 *		char	putc()		c_library
 *		VOID	slew()		lklist.c
 *
 *	side effects:
 *		Map output generated.
 */

VOID
lstarea(xp)
struct area *xp;
{
	register struct areax *oxp;
	register c, i, j;
	register char *ptr;
	int nmsym;
	Addr_T a0, ai, aj;
	struct sym *sp;
	struct sym **p;
        int page;

	putc('\n', mfp);
	slew(mfp);
	/*
	 * Output Area Header
	 */
	ptr = &xp->a_id[0];
	while (ptr < &xp->a_id[NCPS]) {
		if ((c = *ptr++) != 0) {
			putc(c, mfp);
		} else {
			putc(' ', mfp);
		}
	}
	ai = xp->a_addr;
	aj = xp->a_size;
	if (xflag == 0) {
		fprintf(mfp, "   %04X   %04X", ai, aj);
	} else
	if (xflag == 1) {
		fprintf(mfp, " %06o %06o", ai, aj);
	} else
	if (xflag == 2) {
		fprintf(mfp, "  %05u  %05u", ai, aj);
	}
	fprintf(mfp, " = %6u. bytes ", aj);
	if (xp->a_flag & A_ABS) {
		fprintf(mfp, "(ABS");
	} else {
		fprintf(mfp, "(REL");
	}
	if (xp->a_flag & A_OVR) {
		fprintf(mfp, ",OVR");
	} else {
		fprintf(mfp, ",CON");
	}
	if (xp->a_flag & A_PAG) {
		fprintf(mfp, ",PAG");
	}

	page = 0x00;
	if (xp->a_flag & A_CODE) {
		fprintf(mfp, ",CODE");
		memPage = 0x0C;
	}
	if (xp->a_flag & A_XDATA) {
		fprintf(mfp, ",XDATA");
		memPage = 0x0D;
	}
	if (xp->a_flag & A_BIT) {
		fprintf(mfp, ",BIT");
		memPage = 0x0B;
	}
	fprintf(mfp, ")");
	if (xp->a_flag & A_PAG) {
		ai = (ai & 0xFF);
		aj = (aj > 256);
		if (ai || aj) { fprintf(mfp, "  "); }
		if (ai)      { fprintf(mfp, " Boundary"); }
		if (ai & aj)  { fprintf(mfp, " /"); }
		if (aj)      { fprintf(mfp, " Length"); }
		if (ai || aj) { fprintf(mfp, " Error"); }
	}

	/*
	 * Find number of symbols in area
	 */
	nmsym = 0;
	oxp = xp->a_axp;
	while (oxp) {
		for (i=0; i<NHASH; i++) {
			sp = symhash[i];
			while (sp != NULL) {
				if (oxp == sp->s_axp)
					++nmsym;
				sp = sp->s_sp;
			}
		}
		oxp = oxp->a_axp;
	}
	if (nmsym == 0) {
		putc('\n', mfp);
		slew(mfp);
		return;
	}

	/*
	 * Allocate space for an array of pointers to symbols
	 * and load array.
	 */
	if ( (p = (struct sym **) malloc(nmsym*sizeof(struct sym *)))
		== NULL) {
		fprintf(mfp, "\nInsufficient space to build Map Segment.\n");
		slew(mfp);
		return;
	}
	nmsym = 0;
	oxp = xp->a_axp;
	while (oxp) {
		for (i=0; i<NHASH; i++) {
			sp = symhash[i];
			while (sp != NULL) {
				if (oxp == sp->s_axp) {
					p[nmsym++] = sp;
				}
				sp = sp->s_sp;
			}
		}
		oxp = oxp->a_axp;
	}

#if 0
	/*
	 * Bubble Sort of Addresses in Symbol Table Array
	 */
	j = 1;
	while (j) {
		j = 0;
		sp = p[0];
		a0 = sp->s_addr + sp->s_axp->a_addr;
		for (i=1; i<nmsym; ++i) {
			sp = p[i];
			ai = sp->s_addr + sp->s_axp->a_addr;
			if (a0 > ai) {
				j = 1;
				p[i] = p[i-1];
				p[i-1] = sp;
			}
			a0 = ai;
		}
	}
#else
	qsort(p, nmsym, sizeof(struct sym *), _cmpSymByAddr);
#endif	

	/*
	 * Symbol Table Output
	 */
	i = 0;
	while (i < nmsym) {
		fprintf(mfp, "\n");
		slew(mfp);
		fprintf(mfp, "     ");
		sp = p[i];
		aj = sp->s_addr + sp->s_axp->a_addr;
		if (xflag == 0) {
			fprintf(mfp, "  %04X  ", aj);
		} else
		if (xflag == 1) {
			fprintf(mfp, "%06o  ", aj);
		} else
		if (xflag == 2) {
			fprintf(mfp, " %05u  ", aj);
		}
		ptr = &sp->s_id[0];
		fprintf(mfp, "%s", ptr );

		/* NoICE output of symbol */
		if (jflag) DefineNoICE( ptr, aj, memPage );
	}
	putc('\n', mfp);
	free(p);
	slew(mfp);
}
#endif

/*)Function	VOID	lkulist(i)
 *
 *		int	i	i # 0	process LST to RST file
 *				i = 0	copy remainder of LST file
 *					to RST file and close files
 *
 *	The function lkulist() creates a relocated listing (.rst)
 *	output file from the ASxxxx assembler listing (.lst)
 *	files.  The .lst file's program address and code bytes
 *	are changed to reflect the changes made by ASlink as
 *	the .rel files are combined into a single relocated
 *	output file.
 *
 *	local variables:
 *		Addr_T	pc		current program counter address
 *
 *	global variables:
 *		int	hilo		byte order
 *		int	gline		get a line from the LST file
 *					to translate for the RST file
 *		char	rb[]		read listing file text line
 *		FILE	*rfp		The file handle to the current
 *					output RST file
 *		int	rtcnt		count of data words
 *		int	rtflg[]		output the data flag
 *		Addr_T	rtval[]		relocated data
 *		FILE	*tfp		The file handle to the current
 *					LST file being scanned
 *
 *	functions called:
 *		int	fclose()	c_library
 *		int	fgets()		c_library
 *		int	fprintf()	c_library
 *		VOID	lkalist()	lklist.c
 *		VOID	lkglist()	lklist.c
 *
 *	side effects:
 *		A .rst file is created for each available .lst
 *		file associated with a .rel file.
 */

VOID
lkulist(i)
int i;
{
	Addr_T pc;

	/*
	 * Exit if listing file is not open
	 */
	if (tfp == NULL)
		return;

	/*
	 * Normal processing of LST to RST
	 */
	if (i) {
		/*
		 * Evaluate current code address
		 */
		if (hilo == 0) {
			pc = ((rtval[1] & 0xFF) << 8) + (rtval[0] & 0xFF);
		} else {
			pc = ((rtval[0] & 0xFF) << 8) + (rtval[1] & 0xFF);
		}

		/*
		 * Line with only address
		 */
		if (rtcnt == 2) {
			lkalist(pc);

		/*
		 * Line with address and code
		 */
		} else {
			for (i=2; i < rtcnt; i++) {
				if (rtflg[i]) {
					lkglist(pc++, rtval[i] & 0xFF);
				}
			}
		}

	/*
	 * Copy remainder of LST to RST
	 */
	} else {
		if (gline == 0)
			fprintf(rfp, "%s", rb);

		while (fgets(rb, sizeof(rb), tfp) != 0) {
			fprintf(rfp, "%s", rb);
		}
		fclose(tfp);
		tfp = NULL;
		fclose(rfp);
		rfp = NULL;
	}
}

/*)Function	VOID	lkalist(pc)
 *
 *		int	pc		current program counter value
 *
 *	The function lkalist() performs the following functions:
 *
 *	(1)	if the value of gline = 0 then the current listing
 *		file line is copied to the relocated listing file output.
 *
 *	(2)	the listing file is read line by line and copied to
 *		the relocated listing file until a valid source
 *		line number and a program counter value of the correct
 *		radix is found.  The new relocated pc value is substituted
 *		and the line is written to the RST file.
 *
 *	local variables:
 *		int	i		loop counter
 *		char	str[]		temporary string
 *
 *	global variables:
 *		int	gcntr		data byte counter
 *		int	gline		get a line from the LST file
 *					to translate for the RST file
 *		char	rb[]		read listing file text line
 *		char	*rp		pointer to listing file text line
 *		FILE	*rfp		The file handle to the current
 *					output RST file
 *		FILE	*tfp		The file handle to the current
 *					LST file being scanned
 *
 *	functions called:
 *		int	dgt()		lklist.c
 *		int	fclose()	c_library
 *		int	fgets()		c_library
 *		int	fprintf()	c_library
 *		int	sprintf()	c_library
 *		char *	strncpy()	c_library
 *
 *	side effects:
 *		Lines of the LST file are copied to the RST file,
 *		the last line copied has the code address
 *		updated to reflect the program relocation.
 */

VOID
lkalist(pc)
Addr_T pc;
{
	char str[8];
	int i;

	/*
	 * Exit if listing file is not open
	 */
loop:	if (tfp == NULL)
		return;

	/*
	 * Copy current LST to RST
	 */
	if (gline == 0) {
		fprintf(rfp, "%s", rb);
		gline = 1;
	}

	/*
	 * Clear text line buffer
	 */
	for (i=0,rp=rb; i<sizeof(rb); i++) {
		*rp++ = 0;
	}

	/*
	 * Get next LST text line
	 */
	if (fgets(rb, sizeof(rb), tfp) == NULL) {
		fclose(tfp);
		tfp = NULL;
		fclose(rfp);
		rfp = NULL;
		return;
	}

	/*
	 * Must have an ASxxxx Listing line number
	 */
	if (!dgt(RAD10, &rb[30], 1)) {
		fprintf(rfp, "%s", rb);
		goto loop;
	}

	/*
	 * Must have an address in the expected radix
	 */
	if (radix == 16) {
		if (!dgt(RAD16, &rb[3], 4)) {
			fprintf(rfp, "%s", rb);
			goto loop;
		}
		sprintf(str, "%04X", pc);
		strncpy(&rb[3], str, 4);
	} else
	if (radix == 10) {
		if (!dgt(RAD10, &rb[3], 5)) {
			fprintf(rfp, "%s", rb);
			goto loop;
		}
		sprintf(str, "%05d", pc);
		strncpy(&rb[3], str, 5);
	} else
	if (radix == 8) {
		if (!dgt(RAD8, &rb[3], 6)) {
			fprintf(rfp, "%s", rb);
			goto loop;
		}
		sprintf(str, "%06o", pc);
		strncpy(&rb[3], str, 6);
	}

	/*
	 * Copy updated LST text line to RST
	 */
	fprintf(rfp, "%s", rb);
	gcntr = 0;
}

/*)Function	VOID	lkglist(pc,v)
 *
 *		int	pc		current program counter value
 *		int 	v		value of byte at this address
 *
 *	The function lkglist() performs the following functions:
 *
 *	(1)	if the value of gline = 1 then the listing file
 *		is read line by line and copied to the
 *		relocated listing file until a valid source
 *		line number and a program counter value of the correct
 *		radix is found.
 *
 *	(2)	The new relocated values and code address are
 *		substituted and the line may be written to the RST file.
 *
 *	local variables:
 *		int	i		loop counter
 *		char	str[]		temporary string
 *
 *	global variables:
 *		int	gcntr		data byte counter
 *					set to -1 for a continuation line
 *		int	gline		get a line from the LST file
 *					to translate for the RST file
 *		char	rb[]		read listing file text line
 *		char	*rp		pointer to listing file text line
 *		FILE	*rfp		The file handle to the current
 *					output RST file
 *		FILE	*tfp		The file handle to the current
 *					LST file being scanned
 *
 *	functions called:
 *		int	dgt()		lklist.c
 *		int	fclose()	c_library
 *		int	fgets()		c_library
 *		int	fprintf()	c_library
 *		int	sprintf()	c_library
 *		char *	strncpy()	c_library
 *
 *	side effects:
 *		Lines of the LST file are copied to the RST file
 *		with updated data values and code addresses.
 */

VOID
lkglist(pc,v)
Addr_T pc;
int v;
{
	char str[8];
	int i;

	/*
	 * Exit if listing file is not open
	 */
loop:	if (tfp == NULL)
		return;

	/*
	 * Get next LST text line
	 */
	if (gline) {
		/*
		 * Clear text line buffer
		 */
		for (i=0,rp=rb; i<sizeof(rb); i++) {
			*rp++ = 0;
		}

		/*
		 * Get next LST text line
		 */
		if (fgets(rb, sizeof(rb), tfp) == NULL) {
			fclose(tfp);
			tfp = NULL;
			fclose(rfp);
			rfp = NULL;
			return;
		}

		/*
		 * Check for a listing line number if required
		 */
		if (gcntr != -1) {
			if (!dgt(RAD10, &rb[30], 1)) {
				fprintf(rfp, "%s", rb);
				goto loop;
			}
			gcntr = 0;
		}
		gline = 0;
	}

	/*
	 * Hex Listing
	 */
	if (radix == 16) {
		/*
		 * Data Byte Pointer
		 */
		if (gcntr == -1) {
			rp = &rb[8];
		} else {
			rp = &rb[8 + (3 * gcntr)];
		}
		/*
		 * Number must be of proper radix
		 */
		if (!dgt(RAD16, rp, 2)) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			goto loop;
		}
		/*
		 * Output new data value, overwrite relocation codes
		 */
		sprintf(str, " %02X", v);
		strncpy(rp-1, str, 3);
		if (gcntr == -1) {
			gcntr = 0;
		}
		/*
		 * Output relocated code address
		 */
		if (gcntr == 0) {
			if (dgt(RAD16, &rb[3], 4)) {
				sprintf(str, "%04X", pc);
				strncpy(&rb[3], str, 4);
			}
		}
		/*
		 * Output text line when updates finished
		 */
		if (++gcntr == 6) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			gcntr = -1;
		}
	} else
	/*
	 * Decimal Listing
	 */
	if (radix == 10) {
		/*
		 * Data Byte Pointer
		 */
		if (gcntr == -1) {
			rp = &rb[9];
		} else {
			rp = &rb[9 + (3 * gcntr)];
		}
		/*
		 * Number must be of proper radix
		 */
		if (!dgt(RAD10, rp, 3)) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			goto loop;
		}
		/*
		 * Output new data value, overwrite relocation codes
		 */
		sprintf(str, " %03d", v);
		strncpy(rp-1, str, 4);
		if (gcntr == -1) {
			gcntr = 0;
		}
		/*
		 * Output relocated code address
		 */
		if (gcntr == 0) {
			if (dgt(RAD10, &rb[3], 5)) {
				sprintf(str, "%05d", pc);
				strncpy(&rb[3], str, 5);
			}
		}
		/*
		 * Output text line when updates finished
		 */
		if (++gcntr == 4) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			gcntr = -1;
		}
	} else
	/*
	 * Octal Listing
	 */
	if (radix == 8) {
		/*
		 * Data Byte Pointer
		 */
		if (gcntr == -1) {
			rp = &rb[10];
		} else {
			rp = &rb[10 + (3 * gcntr)];
		}
		/*
		 * Number must be of proper radix
		 */
		if (!dgt(RAD8, rp, 3)) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			goto loop;
		}
		/*
		 * Output new data value, overwrite relocation codes
		 */
		sprintf(str, " %03o", v);
		strncpy(rp-1, str, 4);
		if (gcntr == -1) {
			gcntr = 0;
		}
		/*
		 * Output relocated code address
		 */
		if (gcntr == 0) {
			if (dgt(RAD8, &rb[3], 6)) {
				sprintf(str, "%06o", pc);
				strncpy(&rb[3], str, 6);
			}
		}
		/*
		 * Output text line when updates finished
		 */
		if (++gcntr == 4) {
			fprintf(rfp, "%s", rb);
			gline = 1;
			gcntr = -1;
		}
	}
}

/*)Function	int	dgt(rdx,str,n)
 *
 *		int	rdx		radix bit code
 *		char	*str		pointer to the test string
 *		int	n		number of characters to check
 *
 *	The function dgt() verifies that the string under test
 *	is of the specified radix.
 *
 *	local variables:
 *		int	i		loop counter
 *
 *	global variables:
 *		ctype[]			array of character types
 *
 *	functions called:
 *		none
 *
 *	side effects:
 *		none
 */

int
dgt(rdx, str, n)
int rdx, n;
char *str;
{
	int i;

	for (i=0; i<n; i++) {
		if ((ctype[(int)*str++] & rdx) == 0)
			return(0);
	}
	return(1);
}

/*JCF: Create a memory summary file with extension .mem*/
int summary(struct area * areap) 
{
	#define EQ(A,B) !strcmpi((A),(B))
	#define MIN_STACK 16
	#define REPORT_ERROR(A) { fprintf(of, "%s", (A)); \
		                      printf("%s", (A)); \
							  toreturn=1; }

	#define REPORT_WARNING(A) { fprintf(of, "%s", (A)); \
		                      printf("%s", (A));}

	char buff[128];
	int j, toreturn=0;
	unsigned int Total_Last=0, k; 

	struct area * xp;
	FILE * of;
	
	/*Artifacts used for printing*/
	char start[8], end[8], size[8];
	char format[]="   %-20.20s %-7.7s %-7.7s %-7.7s\n";
	char line[]="---------------------";

	typedef struct
	{
		unsigned int Start;
		unsigned int Size;
		char Name[NCPS];
		unsigned int flag;
	} _Mem;

	unsigned int dram[0x100];
	_Mem Ram[]={
		{0,		8, "REG_BANK_0", 0x0001},
		{0x8,	8, "REG_BANK_1", 0x0002},
		{0x10,	8, "REG_BANK_2", 0x0004},
		{0x18,	8, "REG_BANK_3", 0x0008},
		{0x20,	0, "BSEG_BYTES", 0x0010},
		{0,		0, "UNUSED",     0x0000},
		{0x7f,	0, "DATA",       0x0020},
		{0,		0, "TOTAL:",     0x0000}
	};
	
	_Mem IRam= {0xff,   0, "INDIRECT RAM", 0x0080};
	_Mem Stack={0xff,   0, "STACK", 0x0000};
	_Mem XRam= {0xffff, 0, "EXTERNAL RAM", 0x0100};
	_Mem Rom=  {0xffff, 0, "ROM/EPROM/FLASH", 0x0200};

	for(j=0; j<0x100; j++) dram[j]=0;

	/* Open Memory Summary File*/
	of = afile(linkp->f_idp, "mem", 1);
	if (of == NULL)
	{
		lkexit(1);
	}

	xp=areap;
	while (xp)
	{
		/**/ if (EQ(xp->a_id, "REG_BANK_0"))
		{
			Ram[0].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_1"))
		{
			Ram[1].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_2"))
		{
			Ram[2].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "REG_BANK_3"))
		{
			Ram[3].Size=xp->a_size;
		}
		else if (EQ(xp->a_id, "BSEG_BYTES"))
		{
			Ram[4].Size=xp->a_size;
		}
		else if ( EQ(xp->a_id, "DSEG") || EQ(xp->a_id, "OSEG") )
		{
			Ram[6].Size+=xp->a_size;
			if(xp->a_addr<Ram[6].Start) Ram[6].Start=xp->a_addr;
		}

		else if( EQ(xp->a_id, "CSEG") || EQ(xp->a_id, "GSINIT") ||
				 EQ(xp->a_id, "GSFINAL") || EQ(xp->a_id, "HOME") )
		{
			Rom.Size+=xp->a_size;
			if(xp->a_addr<Rom.Start) Rom.Start=xp->a_addr;
		}
		
		else if (EQ(xp->a_id, "SSEG"))
		{
			Stack.Size+=xp->a_size;
			if(xp->a_addr<Stack.Start) Stack.Start=xp->a_addr;
		}

		else if (EQ(xp->a_id, "XSEG") || EQ(xp->a_id, "XISEG")) 
		{
			XRam.Size+=xp->a_size;
			if(xp->a_addr<XRam.Start) XRam.Start=xp->a_addr;
		}

		else if (EQ(xp->a_id, "ISEG"))
		{
			IRam.Size+=xp->a_size;
			if(xp->a_addr<IRam.Start) IRam.Start=xp->a_addr;
		}
		xp=xp->a_ap;
	}

	for(j=0; j<7; j++)
		for(k=Ram[j].Start; (k<(Ram[j].Start+Ram[j].Size))&&(k<0x100); k++)
			dram[k]|=Ram[j].flag; /*Mark as used*/
	
	for(k=IRam.Start; (k<(IRam.Start+IRam.Size))&&(k<0x100); k++)
		dram[k]|=IRam.flag; /*Mark as used*/

	/*Compute the amount of unused memory in direct data Ram.  This is the
	gap between the last register bank or bit segment and the data segment.*/
	for(k=Ram[6].Start-1; (dram[k]==0) && (k>0); k--);
	Ram[5].Start=k+1;
	Ram[5].Size=Ram[6].Start-Ram[5].Start; /*It may be zero (which is good!)*/

	/*Compute the data Ram totals*/
	for(j=0; j<7; j++)
	{
		if(Ram[7].Start>Ram[j].Start) Ram[7].Start=Ram[j].Start;
		Ram[7].Size+=Ram[j].Size;
	}
	Total_Last=Ram[6].Size+Ram[6].Start-1;

	/*Report the Ram totals*/
	fprintf(of, "Direct Internal RAM:\n");
	fprintf(of, format, "Name", "Start", "End", "Size");

	for(j=0; j<8; j++)
	{
		if((j==0) || (j==7)) fprintf(of, format, line, line, line, line);
		if((j!=5) || (Ram[j].Size>0))
		{
			sprintf(start, "0x%02x", Ram[j].Start);
			if(Ram[j].Size==0)
				end[0]=0;/*Empty string*/
			else
				sprintf(end,  "0x%02x", j==7?Total_Last:Ram[j].Size+Ram[j].Start-1);
			sprintf(size, "%5u", Ram[j].Size);
			fprintf(of, format, Ram[j].Name, start, end, size);
		}
	}

	for(k=Ram[6].Start; (k<(Ram[6].Start+Ram[6].Size))&&(k<0x100); k++)
	{
		if(dram[k]!=Ram[6].flag)
		{
			sprintf(buff, "*** ERROR: There is a memory overlap starting at 0x%02x.\n", k);
			REPORT_ERROR(buff);
			break;
		}
	}

	if((Ram[6].Start+Ram[6].Size)>0x80)
	{
		sprintf(buff, "*** ERROR: Insuficient DRAM memory.  "
					"%d byte%s short.\n", (Ram[6].Start+Ram[6].Size)-0x80,
					((Ram[6].Start+Ram[6].Size)==1)?"":"s");
		REPORT_ERROR(buff);
	}

	if(Ram[5].Size!=0)
	{
		sprintf(buff, "*** WARNING: %d bytes in DRAM wasted.  "
		            "Link could use: --data-loc 0x%02x\n",
					Ram[5].Size, Ram[6].Start-Ram[5].Size);
		REPORT_WARNING(buff);
	}

	/*Report the position of the begining of the stack*/
	fprintf(of, "\nStack starts at: 0x%02x", Stack.Start);

	/*Check that the stack pointer is landing in a safe place:*/
	if(dram[Stack.Start])
	{
		fprintf(of, ".\n");
		sprintf(buff, "*** ERROR: Stack overlaps area ");
		REPORT_ERROR(buff);
		for(j=0; j<7; j++)
		{
			if(dram[Stack.Start]&Ram[j].flag)
			{
				sprintf(buff, "'%s'\n", Ram[j].Name);
				break;
			}
		}
		if(dram[Stack.Start]&IRam.flag)
		{
			sprintf(buff, "'%s'\n", IRam.Name);
		}
		REPORT_ERROR(buff);
	}
	else	
	{
		for(j=Stack.Start, k=0; (j<0x100)&&(dram[j]==0); j++, k++);
		fprintf(of, " with %d bytes available\n", k);
		if (k<MIN_STACK)
		{
			sprintf(buff, "*** WARNING: Only %d byte%s available for stack.\n",
					k, (k==1)?"":"s");
			REPORT_WARNING(buff);
		}
	}

	fprintf(of, "\nOther memory:\n");
	fprintf(of, format, "Name", "Start", "End", "Size");
	fprintf(of, format, line, line, line, line);

	/*Report IRam totals:*/
	sprintf(start, "0x%02x", IRam.Start);
	if(IRam.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%02x", IRam.Size+IRam.Start-1);
	sprintf(size, "%5u", IRam.Size);
	fprintf(of, format, IRam.Name, start, end, size);

	/*Report XRam totals:*/
	sprintf(start, "0x%04x", XRam.Start);
	if(XRam.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%04x", XRam.Size+XRam.Start-1);
	sprintf(size, "%5u", XRam.Size);
	fprintf(of, format, XRam.Name, start, end, size);

	/*Report Rom/Flash totals:*/
	sprintf(start, "0x%04x", Rom.Start);
	if(Rom.Size==0)
		end[0]=0;/*Empty string*/
	else
		sprintf(end,  "0x%04x", Rom.Size+Rom.Start-1);
	sprintf(size, "%5u", Rom.Size);
	fprintf(of, format, Rom.Name, start, end, size);

	/*Report any excess:*/
	if(IRam.Size>0x80)
	{
		sprintf(buff, "*** ERROR: Insuficient INDIRECT RAM memory.\n");
		REPORT_ERROR(buff);
	}
	if(XRam.Size>0x10000)
	{
		sprintf(buff, "*** ERROR: Insuficient EXTERNAL RAM memory.\n");
		REPORT_ERROR(buff);
	}
	if(Rom.Size>0x10000)
	{
		sprintf(buff, "*** ERROR: Insuficient ROM/EPROM/FLASH memory.\n");
		REPORT_ERROR(buff);
	}

	fclose(of);
	return toreturn;		
}
