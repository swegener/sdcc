/* Paul's XA51 Assembler, Copyright 1997,2002 Paul Stoffregen (paul@pjrc.com)
 *
 * Paul's XA51 Assembler is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* adapted from the osu8asm project, 1995 */
/* http://www.pjrc.com/tech/osu8/index.html */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define printf(x...) fprintf(stderr,x)

#include "xa_main.h"

extern void yyrestart(FILE *new_file);
extern void hexout(int byte, int memory_location, int end);
extern int yyparse();


/* global variables */

FILE *fhex, *fmem, *list_fp, *sym_fp;
extern FILE *yyin;
extern char *yytext;
extern char last_line_text[];
struct symbol *sym_list=NULL;
struct target *targ_list=NULL;
int lineno=1;
int p1=0, p2=0, p3=0;
int expr_result, expr_ok, jump_dest, inst;
int opcode, operand;
char symbol_name[1000];
struct area_struct area[NUM_AREAS];
int current_area=AREA_CSEG;


char *areaToString (int area) {
  switch (area) 
    {
    case AREA_CSEG: return "CSEG";
    case AREA_DSEG: return "DSEG";
    case AREA_OSEG: return "OSEG";
    case AREA_ISEG: return "ISEG";
    case AREA_BSEG: return "BSEG";
    case AREA_XSEG: return "XSEG";
    case AREA_XISEG: return "XISEG";
    case AREA_XINIT: return "XINIT";
    }
  return ("UNKNOW");
}

/* "mem" is replaced by area[current_area].alloc_position */
/* int mem=0; */   /* mem is location in memory */

/* add symbols to list when we find their definition in pass #1 */
/* we will evaluate their values in pass #2, and figure out if */
/* they are branch targets betweem passes 1 and 2.  Every symbol */
/* should appear exactly once in this list, since it can't be redefined */

struct symbol * build_sym_list(char *thename)
{
	struct symbol *new, *p;

/*	printf("  Symbol: %s  Line: %d\n", thename, lineno); */
	new = (struct symbol *) malloc(sizeof(struct symbol));
	new->name = (char *) malloc(strlen(thename)+1);
	strcpy(new->name, thename);
	new->value = 0;
	new->istarget = 0;
	new->isdef = 0;
	new->isbit = 0;
	new->isreg = 0;
	new->line_def = lineno - 1;
	new->area = current_area;
	new->next = NULL;
	if (sym_list == NULL) return (sym_list = new);
	p = sym_list;
	while (p->next != NULL) p = p->next;
	p->next = new;
	return (new);
}

int assign_value(char *thename, int thevalue)
{
	struct symbol *p;

	p = sym_list;
 	while (p != NULL) {
		if (!(strcasecmp(thename, p->name))) {
			p->value = thevalue;
			p->isdef = 1;
			return (0);
		}
		p = p->next;
	}
	fprintf(stderr, "Internal Error!  Couldn't find symbol\n");
	exit(1);
}

int mk_bit(char *thename)
{
        struct symbol *p;

        p = sym_list;
        while (p != NULL) {
                if (!(strcasecmp(thename, p->name))) {
                        p->isbit = 1;
                        return (0);
                }
                p = p->next;
        }
        fprintf(stderr, "Internal Error!  Couldn't find symbol\n");
        exit(1);
}


int mk_reg(char *thename)
{
        struct symbol *p;

        p = sym_list;
        while (p != NULL) {
                if (!(strcasecmp(thename, p->name))) {
                        p->isreg = 1;
                        return (0);
                }
                p = p->next;
        }
        fprintf(stderr, "Internal Error!  Couldn't find symbol\n");
        exit(1);
}



int get_value(char *thename)
{
	struct symbol *p;
	p = sym_list;
	while (p != NULL) {
		if (!(strcasecmp(thename, p->name)))
			return (p->value);
		p = p->next;
	}
	fprintf(stderr, "Internal Error!  Couldn't find symbol value\n");
	exit(1);
}
		


/* add every branch target to this list as we find them */
/* ok if multiple entries of same symbol name in this list */

struct target * build_target_list(char *thename)
{
	struct target *new, *p;
	new = (struct target *) malloc(sizeof(struct target));
	new->name = (char *) malloc(strlen(thename)+1);
	strcpy(new->name, thename);
	new->next = NULL;
	if (targ_list == NULL) return (targ_list = new);
	p = targ_list;
	while (p->next != NULL) p = p->next;
	p->next = new;
	return (new);
}

/* figure out which symbols are branch targets */

void flag_targets()
{
	struct symbol *p_sym;
	struct target *p_targ;
	p_targ = targ_list;
	while (p_targ != NULL) {
		p_sym = sym_list;
		while (p_sym != NULL) {
			if (!strcasecmp(p_sym->name, p_targ->name))
				p_sym->istarget = 1;
			p_sym = p_sym->next;
		}
		p_targ = p_targ->next;
	}
}

void print_symbol_table()
{
  struct symbol *p;
  p = sym_list;
  while (p != NULL) {
    fprintf(sym_fp, "Sym in %-5s: %s\n", areaToString(p->area), p->name);
    fprintf(sym_fp, "  at: 0x%04X (%5d)", p->value, p->value);
    fprintf(sym_fp, " Def:%s", p->isdef ? "Yes" : "No ");
    fprintf(sym_fp, " Bit:%s", p->isbit ? "Yes" : "No ");
    fprintf(sym_fp, " Target:%s", p->istarget ? "Yes" : "No ");
    fprintf(sym_fp, " Line %d\n", p->line_def);
    p = p->next;
  }
}

/* check that every symbol is in the table only once */

void check_redefine()
{
	struct symbol *p1, *p2;
	p1 = sym_list;
	while (p1 != NULL) {
		p2 = p1->next;
		while (p2 != NULL) {
			if (!strcasecmp(p1->name, p2->name)) {
				fprintf(stderr, "Error: symbol '%s' redefined on line %d", p1->name, p2->line_def);
				fprintf(stderr, ", first defined on line %d\n", p1->line_def);
			exit(1);
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
}

int is_target(char *thename)
{
	struct symbol *p;
	p = sym_list;
	while (p != NULL) {
		if (!strcasecmp(thename, p->name)) return (p->istarget);
		p = p->next;
	}
	return (0);
}

int is_bit(char *thename)
{
        struct symbol *p;
        p = sym_list;
        while (p != NULL) {
                if (!strcasecmp(thename, p->name)) return (p->isbit);
                p = p->next;
        }
        return (0);
}

int is_reg(char *thename)
{
        struct symbol *p;
        p = sym_list;
        while (p != NULL) {
                if (!strcasecmp(thename, p->name)) return (p->isreg);
                p = p->next;
        }
        return (0);
}


int is_def(char *thename)
{
	struct symbol *p;
	p = sym_list;
	while (p != NULL) {
		if (!strcasecmp(thename, p->name) && p->isdef) return(1);
		p = p->next;
	}
	return (0);
}

/* this routine is used to dump a group of bytes to the output */
/* it is responsible for generating the list file and sending */
/* the bytes one at a time to the object code generator */
/* this routine is also responsible for generatine the list file */
/* though is it expected that the lexer has placed all the actual */
/* original text from the line in "last_line_text" */

void out(int *byte_list, int num)
{
	int i, first=1;

	if (num > 0) fprintf(list_fp, "%06X: ", MEM_POS);
	else fprintf(list_fp, "\t");

	for (i=0; i<num; i++) {
		hexout(byte_list[i], MEM_POS + i, 0);
		if (!first && (i % 4) == 0) fprintf(list_fp, "\t");
		fprintf(list_fp, "%02X", byte_list[i]);
		if ((i+1) % 4 == 0) {
			if (first) fprintf(list_fp, "\t%s\n", last_line_text);
			else fprintf(list_fp, "\n");
			first = 0;
		} else {
			if (i<num-1) fprintf(list_fp, " ");
		}
	}
	if (first) {
		if (num < 3) fprintf(list_fp, "\t");
		fprintf(list_fp, "\t%s\n", last_line_text);
	} else {
		if (num % 4) fprintf(list_fp, "\n");
	}
}


/* add NOPs to align memory location on a valid branch target address */

void pad_with_nop()
{
	static int nops[] = {NOP_OPCODE, NOP_OPCODE, NOP_OPCODE, NOP_OPCODE};
	int num;

	last_line_text[0] = '\0';

	for(num=0; (MEM_POS + num) % BRANCH_SPACING; num++) ;
	if (p3) out(nops, num);
	MEM_POS += num;
}

/* print branch out of bounds error */

void boob_error()
{
	fprintf(stderr, "Error: branch out of bounds");
	fprintf(stderr, " in line %d\n", lineno);
	exit(1);
}

/* output the jump either direction on carry */
/* jump_dest and MEM_POS must have the proper values */

/* 
void do_jump_on_carry()
{
	if (p3) {
		operand = REL4(jump_dest, MEM_POS);
		if (operand < 0) {
			operand *= -1;
			operand -= 1;
			if (operand > 15) boob_error();
			out(0x20 + (operand & 15));
		} else {
			if (operand > 15) boob_error();
			out(0x30 + (operand & 15));
		}
	}
}
*/ 

/* turn a string like "10010110b" into an int */

int binary2int(char *str)
{
	register int i, j=1, sum=0;
	
	for (i=strlen(str)-2; i >= 0; i--) {
		sum += j * (str[i] == '1');
		j *= 2;
	}
	return (sum);
}

void print_usage(int);


/* todo: someday this will allow the user to control where the */
/* various memory areas go, and it will take care of assigning */
/* positions to area which follow others (such as OSEG getting */
/* set just after DSEG on the 2nd and 3rd passes when we have */
/* leared the size needed for each segment */

void init_areas(void)
{
  area[AREA_CSEG].start=area[AREA_CSEG].alloc_position = 0;
  area[AREA_DSEG].start=area[AREA_DSEG].alloc_position = 0x30;
  area[AREA_OSEG].start=area[AREA_OSEG].alloc_position = 0x80;
  area[AREA_ISEG].start=area[AREA_ISEG].alloc_position = 0;
  area[AREA_BSEG].start=area[AREA_BSEG].alloc_position = 0;
  area[AREA_XSEG].start=area[AREA_XSEG].alloc_position = 0;
  area[AREA_XISEG].start=area[AREA_XISEG].alloc_position = 0;
  area[AREA_XINIT].start=area[AREA_XINIT].alloc_position = 0;
  area[AREA_GSINIT].start=area[AREA_GSINIT].alloc_position = 0;
  area[AREA_GSFINAL].start=area[AREA_GSFINAL].alloc_position = 0;
  area[AREA_HOME].alloc_position = 0;
}

void printVersion() {
  printf("\nPaul's XA51 Assembler\n");
  printf("Copyright 1997,2002 Paul Stoffregen\n\n");
  printf("This program is free software; you can redistribute it\n");
  printf("and/or modify it under the terms of the GNU General Public\n");
  printf("License, Version 2, published by the Free Software Foundation\n\n");
  printf("This program is distributed in the hope that it will be useful,\n");
  printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

char infilename[PATH_MAX];
char outfilename[PATH_MAX];
char listfilename[PATH_MAX];
char symfilename[PATH_MAX];
//char mapfilename[PATH_MAX];

int verbose=0, createSymbolFile=0;

void process_args(int argc, char **argv) 
{
  int i=0;

  if (argc < 2) print_usage(1);
  
  while (++i<argc && *argv[i]=='-') {
    if (strcmp(argv[i], "--version")==0) {
      printVersion();
      exit (0);
    }
    if (strcmp(argv[i], "--help")==0) {
      print_usage(0);
    }
    if (strcmp(argv[i], "-v")==0) {
      verbose++;
      continue;
    }
    if (strcmp(argv[i], "-s")==0) {
      createSymbolFile++;
      continue;
    }
    print_usage(1);
  }

  if (i!=argc-1) {
    // only 1 source file for now
    print_usage(1);
  }

  strcpy(infilename, argv[i]);

  if (strncasecmp(infilename+strlen(infilename)-3, ".xa", 3)) {
    fprintf (stderr, "unrecognized input file: \"%s\"\n", argv[i]);
    print_usage(1);
  }

  strcpy(outfilename, infilename);
  outfilename[strlen(outfilename)-3] = '\0';
  strcpy(listfilename, outfilename);
  if (createSymbolFile) {
    strcpy(symfilename, outfilename);
    strcat(symfilename, ".sym");
  }
  //strcpy(mapfilename, outfilename);
  strcat(outfilename, ".hex");
  strcat(listfilename, ".lst");
  //strcat(mapfilename, ".map");
}

/* pass #1 (p1=1) find all symbol defs and branch target names */
/* pass #2 (p2=1) align branch targets, evaluate all symbols */
/* pass #3 (p3=1) produce object code */

int main(int argc, char **argv)
{
	process_args (argc, argv);

	yyin = fopen(infilename, "r");
	if (yyin == NULL) {
		fprintf(stderr, "Can't open file '%s'.\n", infilename);
		exit(1);
	}
	fhex = fopen(outfilename, "w");
	if (fhex == NULL) {
		fprintf(stderr, "Can't write file '%s'.\n", outfilename);
		exit(1);
	}
	list_fp = fopen(listfilename, "w");
	if (list_fp == NULL) {
		fprintf(stderr, "Can't write file '%s'.\n", listfilename);
		exit(1);
	}
	if (createSymbolFile) {
	  sym_fp = fopen(symfilename, "w");
	  if (sym_fp == NULL) {
	    fprintf(stderr, "Can't write file '%s'.\n", symfilename);
	    exit(1);
	  }
	}

	if (verbose) printf("Pass 1: Building Symbol Table:\n");
	p1 = 1;
	init_areas();
	yyparse();
	flag_targets();
	if (createSymbolFile) print_symbol_table();
	check_redefine();

	if (verbose) printf("Pass 2: Aligning Branch Targets:\n");
	p1 = 0;
	p2 = 1;
	rewind(yyin);
	yyrestart(yyin);
	lineno = 1;
	init_areas();
	yyparse();

	if (verbose) printf("Pass 3: Generating Object Code:\n");
	p2 = 0;
	p3 = 1;
	rewind(yyin);
	yyrestart(yyin);
	lineno = 1;
	init_areas();
	yyparse();

	fclose(yyin);
	hexout(0, 0, 1);  /* flush and close intel hex file output */
	return 0;
}


void print_usage(int fatal)
{
  FILE *out = fatal ? stderr : stdout;

  fprintf (out, "Usage: xa_asm [-s] [-v] file.xa\n");
  fprintf (out, "  -v            verbose: show progress\n");
  fprintf (out, "  -s            create symbol file\n");
  fprintf (out, "  --version     show version/copyright info and exit\n");
  fprintf (out, "  --help        show this and exit\n");
#if 0
  // some usefull options I can think of.
  fprintf (out, "  -m            create map file\n");
  fprintf (out, "  -ss           create symbol file sorted by symbol\n");
  fprintf (out, "  -sa           create symbol file sorted by segment/address\n");
  fprintf (out, "  --no-temps    supress temp symbols in map and sym file\n");
  fprintf (out, "  --code-loc=#  sets the start address of the code\n");
  fprintf (out, "  --xdata-loc=# sets the start address of the external data\n");
  fprintf (out, "  --stack-loc=# sets the start address of the stack\n");
#endif
  exit(fatal);
}

