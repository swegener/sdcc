/* lksym.c

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
 * 28-Oct-97 JLH:
 *           - lkpsym: Use StoreString for sym construction
 *           - change symeq() to do length-independent string compare
 *           - change hash() to do length-independent hash calculation
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aslink.h"

/*)Module       lksym.c
 *
 *      The module lksym.c contains the functions that operate
 *      on the symbol structures.
 *
 *      lksym.c contains the following functions:
 *              int     hash()
 *              sym *   lkpsym()
 *              VOID *  new()
 *              sym *   newsym()
 *              VOID    symdef()
 *              int     symeq()
 *              VOID    syminit()
 *              VOID    symmod()
 *              a_uint  symval()
 *
 *      lksym.c contains no local/static variables.
 */

/*)Function     VOID    syminit()
 *
 *      The function syminit() is called to clear the hashtable.
 *
 *      local variables:
 *              int     h               computed hash value
 *              sym **  spp             pointer to an array of
 *                                      sym structure pointers
 *
 *      global variables:
 *              sym * symhash[]         array of pointers to NHASH
 *                                      linked symbol lists
 *
 *      functions called:
 *              none
 *
 *      side effects:
 *              (1)     The symbol hash tables are cleared
 */

VOID
syminit(void)
{
        struct sym **spp;

        spp = &symhash[0];
        while (spp < &symhash[NHASH])
                *spp++ = NULL;
}

/*)Function     sym *   newsym()
 *
 *      The function newsym() is called to evaluate the symbol
 *      definition/reference directive from the .rel file(s).
 *      If the symbol is not found in the symbol table a new
 *      symbol structure is created.  Evaluation of the
 *      directive determines if this is a reference or a definition.
 *      Multiple definitions of the same variable will be flagged
 *      as an error if the values are not identical.  A symbol
 *      definition places the symbol value and area extension
 *      into the symbols data structure.  And finally, a pointer
 *      to the symbol structure is placed into the head structure
 *      symbol list.  Refer to the description of the header, symbol,
 *      area, and areax structures in lkdata.c for structure and
 *      linkage details.
 *
 *      local variables:
 *              int     c               character from input text
 *              int     i               evaluation value
 *              char    id[]            symbol name
 *              int     nglob           number of symbols in this header
 *              sym *   tsp             pointer to symbol structure
 *              sym **  s               list of pointers to symbol structures
 *
 *      global variables:
 *              areax   *axp            Pointer to the current
 *                                      areax structure
 *              head    *headp          The pointer to the first
 *                                      head structure of a linked list
 *              int     lkerr           error flag
 *
 *      functions called:
 *              a_uint  eval()          lkeval.c
 *              VOID    exit()          c_library
 *              int     fprintf()       c_library
 *              char    getSid()        lklex.c
 *              char    get()           lklex.c
 *              char    getnb()         lklex.c
 *              sym *   lkpsym()        lksym.c
 *
 *      side effects:
 *              A symbol structure is created and/or modified.
 *              If structure space allocation fails linker will abort.
 *              Several severe errors (these are internal errors
 *              indicating a corrupted .rel file or corrupted
 *              assembler or linker) will terminated the linker.
 */

/*
 * Find/Create a global symbol entry.
 *
 * S xxxxxx Defnnnn
 *   |      |  |
 *   |      |  `-- sp->s_addr
 *   |      `----- sp->s_type
 *   `------------ sp->s_id
 *
 */
struct sym *
newsym(void)
{
  register unsigned i ;
  register unsigned nglob ;
        register int c ;
        struct sym *tsp;
        struct sym **s;
        char id[NCPS];

        getSid(id);     // old: getid(id, -1);
        tsp = lkpsym(id, 1);
        c = getnb();get();get();
        if (c == 'R') {
                tsp->s_type |= S_REF;
                if (eval()) {
                        fprintf(stderr, "Non zero S_REF\n");
                        lkerr++;
                }
        } else
        if (c == 'D') {
                i = eval();
				if (tsp->s_type & S_DEF &&
					!(tsp->s_addr == i && ((tsp->s_axp->a_bap->a_flag & A_ABS) == A_ABS))) {
                        fprintf(stderr, "Multiple definition of %s\n", id);
                        lkerr++;
                }
                tsp->s_type |= S_DEF;
                /*
                 * Set value and area extension link.
                 */
                tsp->s_addr = i;
                tsp->s_axp = axp;
        } else {
                fprintf(stderr, "Invalid symbol type %c for %s\n", c, id);
                lkexit(1);
        }
        /*
         * Place pointer in header symbol list
         */
        if (headp == NULL) {
                fprintf(stderr, "No header defined\n");
                lkexit(1);
        }
        nglob = hp->h_nglob;
        s = hp->s_list;
        for (i=0; i < nglob ;++i) {
                if (s[i] == NULL) {
                        s[i] = tsp;
                        return(tsp);
                }
        }
        fprintf(stderr, "Header symbol list overflow\n");
        lkexit(1);

        /* Never reached */
        return(0);
}

/*)Function     sym *   lkpsym(id,f)
 *
 *              char *  id              symbol name string
 *              int     f               f == 0, lookup only
 *                                      f != 0, create if not found
 *
 *      The function lookup() searches the symbol hash tables for
 *      a symbol name match returning a pointer to the sym structure.
 *      If the symbol is not found then a sym structure is created,
 *      initialized, and linked to the appropriate hash table if f != 0.
 *      A pointer to this new sym structure is returned or a NULL
 *      pointer is returned if f == 0.
 *
 *      local variables:
 *              int     h               computed hash value
 *              sym *   sp              pointer to a sym structure
 *
 *      global varaibles:
 *              sym * symhash[]         array of pointers to NHASH
 *                                      linked symbol lists
 *
 *      functions called:
 *              int     hash()          lksym.c
 *              VOID *  new()           lksym.c
 *              int     symeq()         lksym.c
 *
 *      side effects:
 *              If the function new() fails to allocate space
 *              for the new sym structure the linker terminates.
 */

struct sym *
lkpsym(char *id, int f)
{
        register struct sym *sp;
        register int h;

        h = hash(id, zflag);
        sp = symhash[h];
        while (sp != NULL) {
                if (symeq(id, sp->s_id, zflag))
                        return (sp);
                sp = sp->s_sp;
        }
        if (f == 0)
                return (NULL);
        sp = (struct sym *) new (sizeof(struct sym));
        sp->s_sp = symhash[h];
        symhash[h] = sp;
        sp->s_id = strsto(id);   /* JLH */
        return (sp);
}

/*)Function     a_uint  symval(tsp)
 *
 *              sym *   tsp             pointer to a symbol structure
 *
 *      The function symval() returns the value of the
 *      relocated symbol by adding the variable definition
 *      value to the areax base address.
 *
 *      local variables:
 *              a_uint  val             relocated address value
 *
 *      global variables:
 *              none
 *
 *      functions called:
 *              none
 *
 *      side effects:
 *              none
 */

a_uint
symval(register struct sym *tsp)
{
        register a_uint val;

        val = tsp->s_addr;
        if (tsp->s_axp) {
                val += tsp->s_axp->a_addr;
        }
        return(val);
}

/*)Function     VOID    symdef(fp)
 *
 *              FILE *  fp              file handle for output
 *
 *      The function symdef() scans the hashed symbol table
 *      searching for variables referenced but not defined.
 *      Undefined variables are linked to the default
 *      area "_CODE" and reported as referenced by the
 *      appropriate module.
 *
 *      local variables:
 *              int     i               hash table index loop variable
 *              sym *   sp              pointer to linked symbol structure
 *
 *      global variables:
 *              area    *areap          The pointer to the first
 *                                      area structure of a linked list
 *              sym *symhash[NHASH]     array of pointers to NHASH
 *                                      linked symbol lists
 *
 *      functions called:
 *              symmod()                lksym.c
 *
 *      side effects:
 *              Undefined variables have their areas set to "_CODE".
 */

VOID
symdef(FILE *fp)
{
        register struct sym *sp;
        register int i;

        for (i=0; i<NHASH; ++i) {
                sp = symhash[i];
                while (sp) {
                        if (sp->s_axp == NULL)
                                sp->s_axp = areap->a_axp;
                        if ((sp->s_type & S_DEF) == 0)
                                symmod(fp, sp);
                        sp = sp->s_sp;
                }
        }
}

/*)Function     VOID    symmod(fp,tsp)
 *
 *              FILE *  fp              output file handle
 *              sym *   tsp             pointer to a symbol structure
 *
 *      The function symmod() scans the header structures
 *      searching for a reference to the symbol structure
 *      pointer to by tsp.  The function then generates an error
 *      message whichs names the module having referenced the
 *      undefined variable.
 *
 *      local variables:
 *              int     i               loop counter
 *              sym **  p               pointer to a list of pointers
 *                                      to symbol structures
 *
 *      global variables:
 *              head    *headp          The pointer to the first
 *                                      head structure of a linked list
 *              head    *hp             Pointer to the current
 *                                      head structure
 *              int     lkerr           error flag
 *
 *      functions called:
 *              int     fprintf()       c_library
 *
 *      side effects:
 *              Error output generated.
 */

VOID
symmod(FILE *fp, struct sym *tsp)
{
        register int i;
        struct sym **p;

        if ((hp = headp) != NULL) {
                while(hp) {
                        p = hp->s_list;
                        for (i=0; i<hp->h_nglob; ++i) {
                                if (p[i] == tsp) {
                                        fprintf(fp, "\n?ASlink-Warning-Undefined Global '%s' ", tsp->s_id);
                                        fprintf(fp, "referenced by module '%s'\n", hp->m_id);
                                        lkerr++;
                                }
                        }
                        hp = hp->h_hp;
                }
        }
}

/*)Function     int     symeq(p1, p2, cflag)
 *
 *		int	cflag		case sensitive flag
 *              char *  p1              name string
 *              char *  p2              name string
 *
 *      The function symeq() compares the two name strings for a match.
 *      The return value is 1 for a match and 0 for no match.
 *
 *		cflag == 0	case insensitve compare
 *		cflag != 0	case sensitive compare
 *
 *      local variables:
 *              int     n               loop counter
 *
 *      global variables:
 *              char    ccase[]         an array of characters which
 *                                      perform the case translation function
 *
 *      functions called:
 *              none
 *
 *      side effects:
 *              none
 *
 */

int
symeq(p1, p2, cflag)
register char *p1, *p2;
int cflag;
{
	register int n;

	n = strlen(p1) + 1;
	if(cflag) {
		/*
		 * Case Sensitive Compare
		 */
		do {
			if (*p1++ != *p2++)
				return (0);
		} while (--n);
	} else {
		/*
		 * Case Insensitive Compare
		 */
		do {
			if (ccase[*p1++ & 0x007F] != ccase[*p2++ & 0x007F])
				return (0);
		} while (--n);
	}
	return (1);
}

/*)Function	int	hash(p, cflag)
 *
 *		char *	p		pointer to string to hash
 *		int	cflag		case sensitive flag
 *
 *	The function hash() computes a hash code using the sum
 *	of all characters mod table size algorithm.
 *
 *		cflag == 0	case insensitve hash
 *		cflag != 0	case sensitive hash
 *
 *	local variables:
 *		int	h		accumulated character sum
 *
 *	global variables:
 *		char	ccase[]		an array of characters which
 *					perform the case translation function
 *
 *	functions called:
 *		none
 *
 *	side effects:
 *		none
 */

int
hash(p, cflag)
register char *p;
register int cflag;
{
	register int h;

	h = 0;
	while (*p) {
		if(cflag) {
			/*
			 * Case Sensitive Hash
			 */
			h += *p++;
		} else {
			/*
			 * Case Insensitive Hash
			 */
			h += ccase[*p++ & 0x007F];
		}
	}
	return (h&HMASK);
}

#if	decus

/*)Function	char *	strsto(str)
 *
 *		char *	str		pointer to string to save
 *
 *	Allocate space for "str", copy str into new space.
 *	Return a pointer to the allocated string.
 *
 *	This function based on code by
 *		John L. Hartman
 *		jhartman@compuserve.com
 *
 *	local variables:
 *		int	l		string length + 1
 *		char *	p		string location
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		char *	new()		assym.c
 *		char *	strncpy()	c_library
 *
 *	side effects:
 *		Space allocated for string, string copied
 *		to space.  Out of Space terminates linker.
 */

char *
strsto(str)
char *str;
{
	int  l;
	char *p;

	/*
	 * What we need, including a null.
	 */
	l = strlen(str) + 1;
	p = (char *) new (l);

	/*
	 * Copy the name and terminating null.
	 */
	strncpy(p, str, l);
	return(p);
}

/*
 * This code is optimized for the PDP-11 (decus)
 * which has a limited program space of 56K Bytes !
 * Short strings and small structures are allocated
 * from a memory hunk in new() to reduce the overhead
 * from allocations directly by malloc().  Longer
 * allocations are made directly by malloc.
 * PDP-11 addressing requires that variables
 * are allocated on a word boundary, (strings donot
 * have this restriction,) all allocations will have
 * at most 1 extra byte to maintain the word boundary
 * requirement.
 */

/*)Function	char *	new(n)
 *
 *		unsigned int	n	allocation size in bytes
 *
 *	The function new() allocates n bytes of space and returns
 *	a pointer to this memory.  If no space is available the
 *	linker is terminated.
 *
 *	Allocate space for "str", copy str into new space.
 *	Return a pointer to the allocated string.
 *
 *	This function based on code by
 *		John L. Hartman
 *		jhartman@compuserve.com
 *
 *	local variables:
 *		int	bytes		bytes remaining in buffer area
 *		int	i		loop counter
 *		char *	p		pointer to head of copied string
 *		char *	pnext		next location in buffer area
 *		char *	q		a general pointer
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		int	fprintf()	c_library
 *		VOID *	malloc()	c_library
 *
 *	side effects:
 *		Memory is allocated, if allocation fails
 *		the linker is terminated.
 */

/*
 * To avoid wasting memory headers on small allocations, we
 * allocate a big chunk and parcel it out as required.
 * These static variables remember our hunk.
 */

#define	STR_SPC	1024
#define	STR_MIN	16
static	char *	pnext = NULL;
static	int	bytes = 0;

char *
new(n)
unsigned int n;
{
	char *p,*q;
	unsigned int i;

	/*
	 * Always an even byte count
	 */
	n = (n+1) & 0xFFFE;

	if (n > STR_MIN) {
		/*
		 * For allocations larger than
		 * most structures and short strings
		 * allocate the space directly.
		 */
		p = (char *) malloc(n);
	} else {
		/*
		 * For smaller structures and
		 * strings allocate from the hunk.
		 */
		if (n > bytes) {
			/*
			 * No space.  Allocate a new hunk.
			 * We lose the pointer to any old hunk.
			 * We don't care, as the pieces are never deleted.
			*/
			pnext = (char *) malloc (STR_SPC);
			bytes = STR_SPC;
		}
		p = pnext;
		pnext += n;
		bytes -= n;
	}
	if (p == NULL) {
		fprintf(stderr, "Out of space!\n");
		lkexit(ER_FATAL);
	}
	for (i=0,q=p; i<n; i++) {
		*q++ = 0;
	}
	return (p);
}

#else

/*)Function	char *	strsto(str)
 *
 *		char *	str		pointer to string to save
 *
 *	Allocate space for "str", copy str into new space.
 *	Return a pointer to the allocated string.
 *
 *	This function based on code by
 *		John L. Hartman
 *		jhartman@compuserve.com
 *
 *	local variables:
 *		int	l		string length + 1
 *		int	bytes		bytes remaining in buffer area
 *		char *	p		pointer to head of copied string
 *		char *	pnext		next location in buffer area
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		char *	new()		assym.c
 *		char *	strncpy()	c_library
 *
 *	side effects:
 *		Space allocated for string, string copied
 *		to space.  Out of Space terminates assembler.
 */

/*
 * To avoid wasting memory headers on small allocations, we
 * allocate a big chunk and parcel it out as required.
 * These static variables remember our hunk
 */

#define	STR_SPC	1024
static	char *	pnext = NULL;
static	int	bytes = 0;

char *
strsto(str)
char *str;
{
	int  l;
	char *p;

	/*
	 * What we need, including a null.
	 */
	l = strlen(str) + 1;

	if (l > bytes) {
		/*
		 * No space.  Allocate a new hunk.
		 * We lose the pointer to any old hunk.
		 * We don't care, as the strings are never deleted.
		*/
		pnext = (char *) new (STR_SPC);
		bytes = STR_SPC;
	}

	/*
	 * Copy the name and terminating null.
	 */
	p = pnext;
	strncpy(p, str, l);

	pnext += l;
	bytes -= l;

	return(p);
}

/*)Function	char *	new(n)
 *
 *		unsigned int	n	allocation size in bytes
 *
 *	The function new() allocates n bytes of space and returns
 *	a pointer to this memory.  If no space is available the
 *	linker is terminated.
 *
 *	local variables:
 *		char *	p		a general pointer
 *		char *	q		a general pointer
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		int	fprintf()	c_library
 *		VOID *	malloc()	c_library
 *
 *	side effects:
 *		Memory is allocated, if allocation fails
 *		the linker is terminated.
 */

char *
new(n)
unsigned int n;
{
	char *p,*q;
	unsigned int i;

	if ((p = (char *) malloc(n)) == NULL) {
		fprintf(stderr, "Out of space!\n");
		lkexit(ER_FATAL);
	}
	for (i=0,q=p; i<n; i++) {
		*q++ = 0;
	}
	return (p);
}

#endif

