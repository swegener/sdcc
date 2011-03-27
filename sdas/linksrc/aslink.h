/* aslink.h */

/*
 *  Copyright (C) 1989-2010  Alan R. Baldwin
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
 *
 *   With enhancements from
 *	John L. Hartman	(JLH)
 *	jhartman@compuserve.com
 */

/*
 * System Include Files
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Local Definitions
 */

#define VERSION "V02.00 + NoICE + sdld"

/*
 * To include NoICE Debugging set non-zero
 */
#define NOICE   1

/*
 * To include SDCC Debugging set non-zero
 */
#define SDCDB   1

/*
 * The assembler requires certain variables to have
 * at least 32 bits to allow correct address processing.
 *
 * The type INT32 is defined so that compiler dependent
 * variable sizes may be specified in one place.
 *
 * LONGINT is defined when INT32 is 'long' to
 * select the 'l' forms for format strings.
 */

/* Turbo C++ 3.0 for DOS */
/* 'int' is 16-bits, 'long' is 32-bits */

#ifdef	__TURBOC__
#define		INT32	long
#define		LONGINT
#endif

/* Symantec C++ V6.x/V7.x for DOS (not DOSX) */
/* 'int' is 16-bits, 'long' is 32-bits */

#ifdef	__SC__
#define		INT32	long
#define		LONGINT
#endif

/* The DEFAULT is 'int' is 32 bits */
#ifndef	INT32
#define		INT32	int
#endif


/*)Module	aslink.h
 *
 *	The module aslink.h contains the definitions for constants,
 *	structures, global variables, and LKxxxx functions
 *	contained in the LKxxxx.c files.
 */

/*)BUILD
	$(PROGRAM) =	ASLINK
	$(INCLUDE) =	ASLINK.H
	$(FILES) = {
		LKMAIN.C
		LKLEX.C
		LKAREA.C
		LKHEAD.C
		LKSYM.C
		LKEVAL.C
		LKDATA.C
		LKLIST.C
		LKRLOC.C
		LKLIBR.C
		LKS19.C
		LKIHX.C
	}
	$(STACK) = 2000
*/

#if defined  decus
/* DECUS C void definition */
/* File/extension seperator */

#define VOID	    char
#define FSEPX	    '.'

#elif defined  PDOS
/* PDOS C void definition */
/* File/extension seperator */

#define VOID	    char
#define FSEPX	    ':'

#elif defined UNIX
/* UNIX void definition */
/* File/extension seperator */

#define VOID	    void
#define FSEPX	    '.'
#define LKDIRSEP    '/'
#define LKDIRSEPSTR "/"
#define OTHERSYSTEM

#else
/* DOS/WINDOWS void definition */
/* File/extension seperator */

#define VOID	    void
#define FSEPX	    '.'
#define LKDIRSEP    '\\'
#define LKDIRSEPSTR "\\"
#define OTHERSYSTEM

#endif

/*
 * PATH_MAX
 */
#include <limits.h>
#ifndef PATH_MAX		/* POSIX, but not required   */
 #if defined(__BORLANDC__) || defined(_MSC_VER)
  #include <stdlib.h>
  #define PATH_MAX	_MAX_PATH
 #else
  #define PATH_MAX	255	/* define a reasonable value */
 #endif
#endif

#define LKOBJEXT	"rel"

/*
 * Error definitions
 */
#define	ER_NONE		0	/* No error */
#define	ER_WARNING	1	/* Warning */
#define	ER_ERROR	2	/* Assembly error */
#define	ER_FATAL	3	/* Fatal error */

/*
 * This file defines the format of the
 * relocatable binary file.
 */

#define NCPS	PATH_MAX	/* characters per symbol */
#define NINPUT	PATH_MAX	/* Input buffer size */
#define NHASH	64		/* Buckets in hash table */
#define HMASK	077		/* Hash mask */
#define NLPP	60		/* Lines per page */
#define	NMAX	78		/* Maximum S19/IHX line length */
#define	FILSPC	PATH_MAX	/* File spec length */

#define NDATA	16		/* actual data */
/*
 * NTXT must be defined to have the same value in
 * the ASxxxx assemblers and ASLink.
 *
 * The R Line coding allows only 4-bits for coding
 * the T Line index.  The MAXIMUM value for NTXT
 * is 16.  It should not be changed.
 */
#define NTXT	16		/* T values */

/*
 *	ASLINK - Version 3 Definitions
 */

/*
 *	The "A3_" area constants define values used in
 *	generating the assembler area output data.
 *
 * Area flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	|     |     |     | PAG | ABS | OVR |     |     |
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define A3_CON	  000		/* concatenate */
#define A3_OVR	  004		/* overlay */
#define A3_REL	  000		/* relocatable */
#define A3_ABS	  010		/* absolute */
#define A3_NOPAG   000		/* non-paged */
#define A3_PAG	  020		/* paged */

/* sdld specific */
/* Additional flags for 8051 address spaces */
#define A_DATA	  0000		/* data space (default)*/
#define A_CODE	  0040		/* code space */
#define A_XDATA   0100		/* external data space */
#define A_BIT	  0200		/* bit addressable space */

/* Additional flags for hc08 */
#define A_NOLOAD  0400		/* nonloadable */
#define A_LOAD	  0000		/* loadable (default) */
/* end sdld specific */

/*
 *	The "R3_" relocation constants define values used in
 *	generating the assembler relocation output data for
 *	areas, symbols, and code.
 *
 *
 *	Relocation types.
 *
 *	       7     6	   5	 4     3     2	   1	 0
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 *	    | MSB | PAGn| PAG0| USGN| BYT2| PCR | SYM | BYT |
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define R3_WORD	0x00		/* 16 bit */
#define R3_BYTE	0x01		/*  8 bit */

#define R3_AREA	0x00		/* Base type */
#define R3_SYM	0x02

#define R3_NORM	0x00		/* PC adjust */
#define R3_PCR	0x04

#define R3_BYT1	0x00		/* Byte count for R_BYTE = 1 */
#define R3_BYTX	0x08		/* Byte count for R_BYTE = X */

#define R3_SGND	0x00		/* Signed value */
#define R3_USGN	0x10		/* Unsigned value */

#define R3_NOPAG 0x00		/* Page Mode */
#define R3_PAG0	0x20		/* Page '0' */
#define R3_PAG	0x40		/* Page 'nnn' */

#define R3_LSB	0x00		/* output low byte */
#define R3_MSB	0x80		/* output high byte */

/*
 *	Additional "R3_" functionality is required to support
 *	some microprocesssor architectures.   The 'illegal'
 *	"R3_" mode of R3_WORD | R3_BYTX is used as a designator
 *	of the extended R3_ modes.  The extended modes replace
 *	the PAGING modes and are being added in an adhoc manner
 *	as follows:
 *
 * Extended Mode relocation flags
 *
 *	   7	 6     5     4	   3	 2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	| MSB |  x  |  x  | USGN|  1  | PCR | SYM |  0	|
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define	R3_ECHEK	0011		/* Extended Mode Check Bits */
#define	R3_EXTND	0010		/* Extended Mode Code */
#define	R3_EMASK	0151		/* Extended Mode Mask */

/* #define R3_AREA 0000 */	/* Base type */
/* #define R3_SYM  0002 */

/* #define R3_NORM 0000 */	/* PC adjust */
/* #define R3_PCR  0004 */

/* #define R3_SGND 0000 */	/* Signed value */
/* #define R3_USGN 0020 */	/* Unsigned value */

/* #define R3_LSB  0000 */	/* output low byte */
/* #define R3_MSB  0200 */	/* output high byte */

#define R_J11	(R3_WORD|R3_BYTX) /* JLH: 11 bit JMP and CALL (8051) */
#define R_J19	(R3_WORD|R3_BYTX|R3_MSB) /* 19 bit JMP and CALL (DS80C390) */
#define R_C24	(R3_WORD|R3_BYT1|R3_MSB) /* 24 bit address (DS80C390) */
#define R_J19_MASK (R3_BYTE|R3_BYTX|R3_MSB)

#define IS_R_J19(x) (((x) & R_J19_MASK) == R_J19)
#define IS_R_J11(x) (((x) & R_J19_MASK) == R_J11)
#define IS_C24(x) (((x) & R_J19_MASK) == R_C24)

/* sdld specific */
#define R_BYT3	0x100		/* if R3_BYTE is set, this is a
				 * 3 byte address, of which
				 * the linker must select one byte.
				 */
#define R_HIB	0x200		/* If R3_BYTE & R_BYT3 are set, linker
				 * will select byte 3 of the relocated
				 * 24 bit address.
				 */

#define R_BIT	0x400		/* Linker will convert from byte-addressable
				 * space to bit-addressable space.
				 */

#define R_ESCAPE_MASK	0xf0	/* Used to escape relocation modes
				 * greater than 0xff in the .rel
				 * file.
				 */
/* end sdld specific */

/*
 * Global symbol types.
 */
#define S_REF	1		/* referenced */
#define S_DEF	2		/* defined */

/*
 * File types
 */
#define	F_OUT	0		/* File.ixx / File.sxx */
#define	F_STD	1		/* stdin */
#define	F_LNK	2		/* File.lnk */
#define	F_REL	3		/* File.rel */

/*
 *	The defined type 'a_uint' is used for all address and
 *	unsigned variable value calculations.  Its size is
 *	required to be at least 32-bits to allow upto
 *	32-bit addressing or 32-bit value manipulation.
 */
typedef	unsigned INT32 a_uint;

/*
 *	The defined type 'v_sint' is used for address and
 *	variable value calculations requiring a sign.
 *	Its size is required to be at least 32-bits to allow
 *	upto 32-bit addressing or 32-bit value manipulation.
 */
typedef	signed INT32 v_sint;

/*
 *	The structures of head, area, areax, and sym
 *	are created as the REL files are read during the first
 *	pass of the linker.  The struct head is created upon
 *	encountering a H directive in the REL file.  The
 *	structure contains a link to a link file structure
 *	(struct lfile) which describes the file containing the H
 *	directive, the number of data/code areas
 *	contained in this header segment, the number of
 *	symbols referenced/defined in this header segment, a pointer
 *	to an array of pointers to areax structures (struct areax)
 *	created as each A directive is read, and a pointer to an
 *	array of pointers to symbol structures (struct sym) for
 *	all referenced/defined symbols.  As H directives are read
 *	from the REL files a linked list of head structures is
 *	created by placing a link to the new head structure
 *	in the previous head structure.
 */
struct	head
{
	struct	head  *h_hp;	/* Header link */
	struct	lfile *h_lfile;	/* Associated file */
	int	h_narea;	/* # of areas */
	struct	areax **a_list; /* Area list */
	int	h_nsym;	/* # of symbols */
	struct	sym   **s_list; /* Symbol list */
	char *	m_id;		/* Module name */
};

/*
 *	A structure area is created for each 'unique' data/code
 *	area definition found as the REL files are read.  The
 *	struct area contains the name of the area, a flag byte
 *	which contains the area attributes (REL/CON/OVR/ABS),
 *	the area subtype (not used in this assembler), and the
 *	area base address and total size which will be filled
 *	in at the end of the first pass through the REL files.
 *	As A directives are read from the REL files a linked
 *	list of unique area structures is created by placing a
 *	link to the new area structure in the previous area structure.
 */
struct	area
{
	struct	area	*a_ap;	/* Area link */
	struct	areax	*a_axp; /* Area extension link */
	a_uint	a_addr;		/* Beginning address of area */
	a_uint	a_size;		/* Total size of the area */
/* sdld specific */
	a_uint	a_unaloc;	/* Total number of unallocated bytes, for error reporting */
/* end sdld specific */
	char	a_type;		/* Area subtype */
	int	a_flag;		/* Flags */
	char *	a_id;		/* Name */
/* sdld specific */
	char	*a_image;	/* Something for hc08/lkelf */
	char	*a_used;	/* Something for hc08/lkelf */
/* end sdld specific */
};

/*
 *	An areax structure is created for every A directive found
 *	while reading the REL files.  The struct areax contains a
 *	link to the 'unique' area structure referenced by the A
 *	directive and to the head structure this area segment is
 *	a part of.  The size of this area segment as read from the
 *	A directive is placed in the areax structure.  The beginning
 *	address of this segment will be filled in at the end of the
 *	first pass through the REL files.  As A directives are read
 *	from the REL files a linked list of areax structures is
 *	created for each unique area.  The final areax linked
 *	list has at its head the 'unique' area structure linked
 *	to the linked areax structures (one areax structure for
 *	each A directive for this area).
 */
struct	areax
{
	struct	areax	*a_axp; /* Area extension link */
	struct	area	*a_bap; /* Base area link */
	struct	head	*a_bhp; /* Base header link */
	a_uint	a_addr;		/* Beginning address of section */
	a_uint	a_size;		/* Size of the area in section */
};

/*
 *	A sym structure is created for every unique symbol
 *	referenced/defined while reading the REL files.  The
 *	struct sym contains the symbol's name, a flag value
 *	(not used in this linker), a symbol type denoting
 *	referenced/defined, and an address which is loaded
 *	with the relative address within the area in which
 *	the symbol was defined.  The sym structure also
 *	contains a link to the area where the symbol was defined.
 *	The sym structures are linked into linked lists using
 *	the symbol link element.
 */
struct	sym
{
	struct	sym	*s_sp;	/* Symbol link */
	struct	areax	*s_axp; /* Symbol area link */
	char	s_type;		/* Symbol subtype */
	char	s_flag;		/* Flag byte */
	a_uint	s_addr;		/* Address */
	char	*s_id;		/* Name (JLH) */
	char	*m_id;		/* Module symbol define in */
};

/*
 *	The structure lfile contains a pointer to a
 *	file specification string, the file type, and
 *	a link to the next lfile structure.
 */
struct	lfile
{
	struct	lfile	*f_flp; /* lfile link */
	int	f_type;		/* File type */
	char	*f_idp;		/* Pointer to file spec */
};

/*
 *	The struct base contains a pointer to a
 *	base definition string and a link to the next
 *	base structure.
 */
struct	base
{
	struct	base  *b_base;	/* Base link */
	char	      *b_strp;	/* String pointer */
};

/*
 *	The struct globl contains a pointer to a
 *	global definition string and a link to the next
 *	global structure.
 */
struct	globl
{
	struct	globl *g_globl; /* Global link */
	char	      *g_strp;	/* String pointer */
};

/*
 *	A structure sdp is created for each 'unique' paged
 *	area definition found as the REL files are read.
 *	As P directives are read from the REL files a linked
 *	list of unique sdp structures is created by placing a
 *	link to the new sdp structure in the previous area structure.
 */
struct	sdp
{
	struct	area  *s_area;	/* Paged Area link */
	struct	areax *s_areax; /* Paged Area Extension Link */
	a_uint	s_addr;		/* Page address offset */
};

/*
 *	The structure rerr is loaded with the information
 *	required to report an error during the linking
 *	process.  The structure contains an index value
 *	which selects the areax structure from the header
 *	areax structure list, a mode value which selects
 *	symbol or area relocation, the base address in the
 *	area section, an area/symbol list index value, and
 *	an area/symbol offset value.
 */
struct	rerr
{
	int	aindex;		/* Linking area */
	int	mode;		/* Relocation mode */
	a_uint	rtbase;		/* Base address in section */
	int	rindex;		/* Area/Symbol reloaction index */
	a_uint	rval;		/* Area/Symbol offset value */
};

/*
 *	The structure lbpath is created for each library
 *	path specification input by the -k option.  The
 *	lbpath structures are linked into a list using
 *	the next link element.
 */
struct lbpath {
	struct	lbpath	*next;
	char		*path;
};

/*
 *	The structure lbname is created for all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file.  The element path points to
 *	the path string, element libfil points to the library
 *	file string, and the element libspc is the concatenation
 *	of the valid path and libfil strings.
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 *
 *	Each library file contains a list of object files
 *	that are contained in the particular library. e.g.:
 *
 *		\iolib\termio
 *		\inilib\termio
 *
 *	Only one specification per line is allowed.
 */
struct lbname {
	struct	lbname	*next;
	char		*path;
	char		*libfil;
	char		*libspc;
};

/*
 *	The function fndsym() searches through all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file for a symbol definition.
 *
 *	The structure lbfile is created for the first library
 *	object file which contains the definition for the
 *	specified undefined symbol.
 *
 *	The element libspc points to the library file path specification
 *	and element relfil points to the object file specification string.
 *	The element filspc is the complete path/file specification for
 *	the library file to be imported into the linker.  The
 *	file specicifation may be formed in one of two ways:
 *
 *	(1)	If the library file contained an absolute
 *		path/file specification then this becomes filspc.
 *		(i.e. C:\...)
 *
 *	(2)	If the library file contains a relative path/file
 *		specification then the concatenation of the path
 *		and this file specification becomes filspc.
 *		(i.e. \...)
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 */
struct lbfile {
	struct	lbfile	*next;
	char		*libspc;
	char		*relfil;
	char		*filspc;
/* sdld specific */
	long		offset;
	unsigned int	type;
/* end sdld specific */
};

/*
 *	External Definitions for all Global Variables
 */

extern	char	*_abs_;		/*	= { ".  .ABS." };
				 */
extern	int	lkerr;		/*	ASLink error flag
				 */
extern	char	*ip;		/*	pointer into the REL file
				 *	text line in ib[]
				 */
extern	char	ib[NINPUT];	/*	REL file text line
				 */
extern	char	*rp;		/*	pointer into the LST file
				 *	text line in rb[]
				 */
extern	char	rb[NINPUT];	/*	LST file text line being
				 *	address relocated
				 */
extern	char	ctype[];	/*	array of character types, one per
				 *	ASCII character
				 */

/*
 *	Character Type Definitions
 */
#define	SPACE	'\000'
#define ETC	'\000'
#define	LETTER	'\001'
#define	DIGIT	'\002'
#define	BINOP	'\004'
#define	RAD2	'\010'
#define	RAD8	'\020'
#define	RAD10	'\040'
#define	RAD16	'\100'
#define	ILL	'\200'

#define	DGT2	DIGIT|RAD16|RAD10|RAD8|RAD2
#define	DGT8	DIGIT|RAD16|RAD10|RAD8
#define	DGT10	DIGIT|RAD16|RAD10
#define	LTR16	LETTER|RAD16

extern	char	afspec[];	/*	The filespec created by afile()
				 */
extern	char	ccase[];	/*	an array of characters which
				 *	perform the case translation function
				 */

extern	struct	lfile	*filep; /*	The pointers (lfile *) filep,
				 *	(lfile *) cfp, and (FILE *) sfp
				 *	are used in conjunction with
				 *	the routine nxtline() to read
				 *	aslink commands from
				 *	(1) the standard input or
				 *	(2) or a command file
				 *	and to read the REL files
				 *	sequentially as defined by the
				 *	aslink input commands.
				 *
				 *	The pointer *filep points to the
				 *	beginning of a linked list of
				 *	lfile structures.
				 */
extern	struct	lfile	*cfp;	/*	The pointer *cfp points to the
				 *	current lfile structure
				 */
extern	struct	lfile	*startp;/*	aslink startup file structure
				 */
extern	struct	lfile	*linkp; /*	pointer to first lfile structure
				 *	containing an input REL file
				 *	specification
				 */
extern	struct	lfile	*lfp;	/*	pointer to current lfile structure
				 *	being processed by parse()
				 */
extern	struct	head	*headp; /*	The pointer to the first
				 *	head structure of a linked list
				 */
extern	struct	head	*hp;	/*	Pointer to the current
				 *	head structure
				 */
extern	struct	area	*areap; /*	The pointer to the first
				 *	area structure of a linked list
				 */
extern	struct	area	*ap;	/*	Pointer to the current
				 *	area structure
				 */
extern	struct	areax	*axp;	/*	Pointer to the current
				 *	areax structure
				 */
extern	struct	sym *symhash[NHASH]; /* array of pointers to NHASH
				      * linked symbol lists
				      */
extern	struct	base	*basep; /*	The pointer to the first
				 *	base structure
				 */
extern	struct	base	*bsp;	/*	Pointer to the current
				 *	base structure
				 */
extern	struct	globl	*globlp;/*	The pointer to the first
				 *	globl structure
				 */
extern	struct	globl	*gsp;	/*	Pointer to the current
				 *	globl structure
				 */
extern	struct	sdp	sdp;	/*	Base Paged structure
				 */
extern	struct	rerr	rerr;	/*	Structure containing the
				 *	linker error information
				 */
extern	FILE	*ofp;		/*	Linker Output file handle
				 */

#if NOICE
extern	FILE	*jfp;		/*	NoICE output file handle
				 */
#endif

extern	FILE	*mfp;		/*	Map output file handle
				 */
extern	FILE	*rfp;		/*	File handle for output
				 *	address relocated ASxxxx
				 *	listing file
				 */
extern	FILE	*sfp;		/*	The file handle sfp points to the
				 *	currently open file
				 */
extern	FILE	*tfp;		/*	File handle for input
				 *	ASxxxx listing file
				 */

#if SDCDB
extern	FILE	*yfp;		/*	SDCDB output file handle
				 */
#endif

extern	int	oflag;		/*	Output file type flag
				 */
#if NOICE
extern	int	jflag;		/*	-j, enable NoICE Debug output
				 */
#endif

extern	int	mflag;		/*	Map output flag
				 */
extern	int	xflag;		/*	Map file radix type flag
				 */

#if SDCDB
extern	int	yflag;		/*	-y, enable SDCC Debug output
				 */
#endif

extern	int	pflag;		/*	print linker command file flag
				 */
extern	int	uflag;		/*	Listing relocation flag
				 */
extern	int	wflag;		/*	Enable wide format listing
				 */
extern	int	zflag;		/*	Enable symbol case sensitivity
				 */
extern	int	radix;		/*	current number conversion radix:
				 *	2 (binary), 8 (octal), 10 (decimal),
				 *	16 (hexadecimal)
				 */
extern	int	line;		/*	current line number
				 */
extern	int	page;		/*	current page number
				 */
extern	int	lop;		/*	current line number on page
				 */
extern	int	pass;		/*	linker pass number
				 */
extern	int	rtcnt;		/*	count of elements in the
				 *	rtval[] and rtflg[] arrays
				 */
extern	a_uint	rtval[];	/*	data associated with relocation
				 */
extern	int	rtflg[];	/*	indicates if rtval[] value is
				 *	to be sent to the output file.
				 */
extern	char	rtbuf[];	/*	S19/IHX output buffer
				 */
extern	a_uint	rtadr0;		/*	rtbuf[] processing
				 */
extern	a_uint	rtadr1;		/*
				 */
extern	a_uint	rtadr2;		/*
				 */
extern	int	hilo;		/*	REL file byte ordering
				 */
extern	int	gline;		/*	LST file relocation active
				 *	for current line
				 */
extern	int	gcntr;		/*	LST file relocation active
				 *	counter
				 */
extern	struct lbpath *lbphead; /*	pointer to the first
				 *	library path structure
				 */
extern	struct lbname *lbnhead; /*	pointer to the first
				 *	library name structure
				 */
extern	struct lbfile *lbfhead; /*	pointer to the first
				 *	library file structure
				 */
/* sdld specific */
extern	int	sflag;		/*	JCF: Memory usage output flag
				 */
extern	int	packflag;	/*	Pack data memory flag
				 */
extern	int	stacksize;	/*	Pack data memory flag
				 */
extern int	rflag;		/*	Extended linear address record flag.
				*/
extern	a_uint iram_size;	/*	internal ram size
				 */
extern	long xram_size;		/*	external ram size
				 */
extern	long code_size;		/*	code size
				 */
extern	char *sdld_output;	/*	output file name, --output argument
				 */
extern char *optsdcc;
extern char *optsdcc_module;
/* sdld 8015 specific */
extern	char idatamap[];	/* 0:not used, 1:used
				 */
/* end sdld 8051 specific */
/* end sdld specific */



/* C Library function definitions */
/* for reference only
extern	VOID		exit();
extern	int		fclose();
extern	char *		fgets();
extern	FILE *		fopen();
extern	int		fprintf();
extern	VOID		free();
extern	VOID *		malloc();
extern	char		putc();
extern	char *		sprintf();
extern	char *		strcpy();
extern	int		strlen();
extern	char *		strncpy();
extern	char *		strrchr();
*/

/* Program function definitions */

/* lkmain.c */
extern	FILE *		afile();
extern	VOID		bassav();
extern	int		fndext(char *str);
extern	int		fndidx(char *str);
extern	VOID		gblsav();
extern	VOID		iramsav();
extern	VOID		xramsav();
extern	VOID		codesav();
extern	VOID		iramcheck();
extern	VOID		link_main();
extern	VOID		lkexit();
extern	int		main();
extern	VOID		map();
extern	VOID		sym();
extern	int		parse();
extern	VOID		setbas();
extern	VOID		setgbl();
extern	VOID		usage();
extern	VOID		copyfile();

/* lklex.c */
extern	VOID		chopcrlf();
extern	char		endline();
extern	int		get();
extern	VOID		getfid();
extern	VOID		getid();
extern	VOID		getSid(char *id);
extern	int		getmap();
extern	int		getnb();
extern	int		more();
extern	int		nxtline();
extern	VOID		skip();
extern	VOID		unget();

/* lkarea.c */
extern	VOID		lkparea();
extern	VOID		lnkarea();
extern	VOID		lnkarea2();
extern	VOID		newarea();

/* lkhead.c */
extern	VOID		module();
extern	VOID		newhead();

/* lksym.c */
extern	int		hash();
extern	struct	sym *	lkpsym();
extern	char *		new(unsigned int n);
extern	struct	sym *	newsym(VOID);
extern	char *		strsto(char *str);
extern	VOID		symdef();
extern	int		symeq();
extern	VOID		syminit();
extern	VOID		symmod();
extern	a_uint		symval();

/* lkeval.c */
extern	int		digit();
extern	a_uint		eval();
extern	a_uint		expr();
extern	int		oprio();
extern	a_uint		term();

/* lklist.c */
extern	int		dgt();
extern	VOID		newpag();
extern	VOID		slew();
extern	VOID		lstarea();
extern	VOID		lkulist();
extern	VOID		lkalist();
extern	VOID		lkglist();

/* lknoice.c */
extern	void		DefineNoICE( char *name, a_uint value, int page );


/* lkrloc.c */
extern	a_uint		adb_b(register a_uint v, register int i);
extern	a_uint		adb_bit(register a_uint v, register int i);
extern	a_uint		adb_hi(a_uint  v, int i);
extern	a_uint		adb_lo(a_uint  v, int i);
extern	a_uint		adb_24_bit(register a_uint v, register int i);
extern	a_uint		adb_24_hi(a_uint v, int i);
extern	a_uint		adb_24_mid(a_uint v, int i);
extern	a_uint		adb_24_lo(a_uint v, int i);
extern	a_uint		adw_w(register a_uint v, register int i);
extern	a_uint		adw_24(a_uint v, int i);
extern	a_uint		adw_hi(a_uint  v, int i);
extern	a_uint		adw_lo(a_uint  v, int i);
extern	a_uint		evword(VOID);
extern	VOID		rele(VOID);
extern	VOID		reloc(char c);
extern	VOID		relt(VOID);
extern	VOID		relr(VOID);
extern	VOID		relp(VOID);
extern	VOID		relerr(char *str);
extern	char *		errmsg[];
extern	VOID		errdmp(FILE *fptr, char *str);
extern	VOID		relerp(char *str);
extern	VOID		erpdmp(FILE *fptr, char *str);
extern	VOID		prntval(FILE *fptr, a_uint v);
extern	int		lastExtendedAddress;

/* lklibr.c */
extern	int		addfile();
extern	VOID		addlib();
extern	VOID		addpath();
extern	int		fndsym();
extern	VOID		library();
extern	VOID		loadfile();
extern	VOID		search();

/* lks19.c */
extern	VOID		s19();
extern	VOID		sflush();

/* lkihx.c */
extern	VOID		ihx();
extern	VOID		iflush();
/* sdlk specific */
extern	VOID		ihxExtendedLinearAddress(a_uint);
extern	VOID		ihxNewArea();
/* end sdlk specific */

/* EEP: lkelf.c */
extern	VOID		elf();

/* JCF: lkmem.c */
extern int summary(struct area * xp);
extern int summary2(struct area * xp);

/* JCF: lkaomf51.c */
extern void SaveLinkedFilePath(char * filepath);
extern void CreateAOMF51(void);

/* lkgb.h */
VOID gb(int in);
VOID gg(int in);

/* strcmpi.h */
extern int as_strcmpi(const char *s1, const char *s2);
extern int as_strncmpi(const char *s1, const char *s2, size_t n);
