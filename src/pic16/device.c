/*-------------------------------------------------------------------------

  device.c - Accomodates subtle variations in PIC16 devices

   Written By -  Scott Dattalo scott@dattalo.com
   Ported to PIC16 By -  Martin Dubuc m.dubuc@rogers.com

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
-------------------------------------------------------------------------*/


/*
	VR - Began writing code to make PIC16 C source files independent from
	the header file (created by the inc2h.pl)

	- adding maximum RAM memory into PIC_Device structure

*/

#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "pcode.h"
#include "ralloc.h"
#include "device.h"


static PIC16_device Pics16[] = {
  {
    {"p18f242", "18f242", "pic18f242", "f242"},		// aliases
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0,
	{
		/* PIC18F242 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x03, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x03, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x03, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f248", "18f248", "pic18f248", "f248"},		// aliases
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0,
	{
		/* PIC18F248 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { -1, 0, 0xff } /* 5 */  , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x03, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x03, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x03, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f252", "18f252", "pic18f252", "f252"},		// aliases
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0,
	{
		/* PIC18F252 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f258", "18f258", "pic18f258", "f258"},		// aliases
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0,
	{
		/* PIC18F258 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { -1, 0, 0xff } /* 5 */  , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f442", "18f442", "pic18f442", "f442"},		// aliases
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0,
	{
		/* PIC18F442 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x03, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x03, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x03, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f448", "18f448", "pic18f448", "f448"},		// aliases
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0,
	{
		/* PIC18F448 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { -1, 0, 0xff } /* 5 */  , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x03, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x03, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x03, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },
  
  {
    {"p18f452", "18f452", "pic18f452", "f452"},		// aliases
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0,
	{
		/* PIC18F452 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f458", "18f458", "pic18f458", "f458"},		// aliases
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0,
	{
		/* PIC18F458 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { -1, 0, 0xff } /* 5 */  , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f1220", "18f1220", "pic18f1220", "f1220"},	// aliases
    0,
    0x200,
    0x200,
    0,
	{
		/* PIC18F1220 configuration words */
		0x300001,
		0x30000d,
		{ { 0xcf, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x1f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x80, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x03, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x03, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x03, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },


  {
    {"p18f6520", "18f6520", "pic18f6520", "f6520"},	// aliases
    0,
    0x800,						// bank mask
    0x800,						// RAMsize
    1,
	{
		/* PIC18F6520 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ 0x80, 0, 0xff } /* 4 */ , { 0x88, 0, 0xff } /* 5 */ , { -1, 0, 0xff } /* 6 */  , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f6620", "18f6620", "pic18f6620", "f6620"},	// aliases
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1,
	{
		/* PIC18F6620 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f6680", "18f6680", "pic18f6680", "f6680"},	// aliases
    0,
    0xc00,						// bank mask
    0xc00,						// RAMsize
    1,
	{
		/* PIC18F6680 configuration words */
		0x300001,
		0x30000d,
		{ { 0x2f, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x1f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x83, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f6720", "18f6720", "pic18f6720", "f6720"},	// aliases
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1,
	{
		/* PIC18F6720 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ -1, 0, 0xff } /* 4 */  , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0xff, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0xff, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0xff, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f8520", "18f8520", "pic18f8520", "f8520"},	// aliases
    0,
    0x800,						// bank mask
    0x800,						// RAMsize
    1,
	{
		/* PIC18F8520 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ 0x83, 0, 0xff } /* 4 */ , { 0x88, 0, 0xff } /* 5 */ , { -1, 0, 0xff } /* 6 */  , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ } 
	}
  },

  {
    {"p18f8620", "18f8620", "pic18f8620", "f8620"},	// aliases
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1,
	{
		/* PIC18F8620 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ 0x83, 0, 0xff } /* 4 */ , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ }
	}
  },
  {
    {"p18f8680", "18f8680", "pic18f8680", "f8680"},	// aliases
    0,
    0xc00,						// bank mask
    0x800,						// RAMsize
    1,
	{
		/* PIC18F8680 configuration words */
		0x300001,
		0x30000d,
		{ { 0x2f, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x1f, 0, 0xff } /* 3 */ , 
		{ 0x83, 0, 0xff } /* 4 */ , { 0x83, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0x0f, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0x0f, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0x0f, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ }
	}
  },

  {
    {"p18f8720", "18f8720", "pic18f8720", "f8720"},	// aliases
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1,
	{
		/* PIC18F8720 configuration words */
		0x300001,
		0x30000d,
		{ { 0x27, 0, 0xff } /* 1 */ , { 0x0f, 0, 0xff } /* 2 */ , { 0x0f, 0, 0xff } /* 3 */ , 
		{ 0x83, 0, 0xff } /* 4 */ , { 0x01, 0, 0xff } /* 5 */ , { 0x85, 0, 0xff } /* 6 */ , 
		{ -1, 0, 0xff } /* 7 */  , { 0xff, 0, 0xff } /* 8 */ , { 0xc0, 0, 0xff } /* 9 */ , 
		{ 0xff, 0, 0xff } /* a */ , { 0xe0, 0, 0xff } /* b */ , { 0xff, 0, 0xff } /* c */ , 
		{ 0x40, 0, 0xff } /* d */ }
	}
  }

};

static int num_of_supported_PICS = sizeof(Pics16)/sizeof(PIC16_device);

#define DEFAULT_PIC "452"

PIC16_device *pic16=NULL;
unsigned int stackPos = 0;

extern regs* newReg(short type, short pc_type, int rIdx, char *name, int size, int alias, operand *refop);

void pic16_setMaxRAM(int size)
{
	pic16->maxRAMaddress = size;
	stackPos = pic16->RAMsize-1;

	if (pic16->maxRAMaddress < 0) {
		fprintf(stderr, "invalid \"#pragma maxram 0x%x\" setting\n",
			pic16->maxRAMaddress);
	  return;
	}
}

extern char *iComments2;

void pic16_dump_equates(FILE *of, set *equs)
{
  regs *r;

	r = setFirstItem(equs);
	if(!r)return;
	
	fprintf(of, "%s", iComments2);
	fprintf(of, ";\tEquates to used internal registers\n");
	fprintf(of, "%s", iComments2);
	
	for(; r; r = setNextItem(equs)) {
		fprintf(of, "%s\tequ\t0x%02x\n", r->name, r->address);
	}
}


int regCompare(const void *a, const void *b)
{
  const regs *const *i = a;
  const regs *const *j = b;

	/* sort primarily by the address */
	if( (*i)->address > (*j)->address)return 1;
	if( (*i)->address < (*j)->address)return -1;
	
	/* and secondarily by size */
	/* register size sorting may have strange results use with care */
	if( (*i)->size > (*j)->size)return 1;
	if( (*i)->size < (*j)->size)return -1;
	
	/* finally if in same address and same size sort by name */
	return (strcmp( (*i)->name, (*j)->name));

  return 0;
}

int symCompare(const void *a, const void *b)
{
  const symbol *const *i = a;
  const symbol *const *j = b;

	/* sort primarily by the address */
	if( SPEC_ADDR((*i)->etype) > SPEC_ADDR((*j)->etype))return 1;
	if( SPEC_ADDR((*i)->etype) < SPEC_ADDR((*j)->etype))return -1;
	
	/* and secondarily by size */
	/* register size sorting may have strange results use with care */
	if( getSize((*i)->etype) > getSize((*j)->etype))return 1;
	if( getSize((*i)->etype) < getSize((*j)->etype))return -1;

	/* finally if in same address and same size sort by name */
	return (strcmp( (*i)->rname, (*j)->rname));

  return 0;
}

void pic16_dump_usection(FILE *of, set *section, int fix)
{
  static int abs_usection_no=0;
  regs *r, *rprev;
  int init_addr, i;
  regs **rlist;
  regs *r1;

	/* put all symbols in an array */
	rlist = Safe_calloc(elementsInSet(section), sizeof(regs *));
	r = rlist[0]; i = 0;
	for(rprev = setFirstItem(section); rprev; rprev = setNextItem(section)) {
		rlist[i] = rprev; i++;
	}
	
	if(!i) {
		if(rlist)free(rlist);
	  return;
	}

	/* sort symbols according to their address */
	qsort(rlist, i	/*elementsInSet(section)*/, sizeof(regs *), regCompare);
	
	if(!fix) {
		fprintf(of, "\n\n\tudata\n");
		for(r = setFirstItem(section); r; r = setNextItem(section)) {
			fprintf(of, "%s\tres\t%d\n", r->name, r->size);
		}
	} else {
	  int j=0;
	  int deb_addr=0;

		rprev = NULL;
		init_addr = rlist[j]->address;
		deb_addr = init_addr;
		fprintf(of, "\n\nustat_%s_%02d\tudata\t0X%04X\n", moduleName, abs_usection_no++, init_addr);
	
		for(j=0;j<i;j++) {
			r = rlist[j];
			if(j < i-1)r1 = rlist[j+1]; else r1 = NULL;
			
			init_addr = r->address;
			deb_addr = init_addr;
			
			if((rprev && (init_addr > (rprev->address + rprev->size)))) {
				fprintf(of, "\n\nustat_%s_%02d\tudata\t0X%04X\n", moduleName, abs_usection_no++, init_addr);
			}

			if(r1 && (init_addr == r1->address)) {
				fprintf(of, "\n%s\tres\t0\n", r->name);
			} else {
				fprintf(of, "%s\tres\t%d\n", r->name, r->size);
				deb_addr += r->size;
			}
			
			rprev = r;
		}
	}
	free(rlist);
}


/* forward declaration */
void print_idata(FILE *of, symbol * sym, sym_link * type, initList * ilist);

void pic16_dump_isection(FILE *of, set *section, int fix)
{
  static int abs_isection_no=0;
  symbol *s, *sprev;
  int init_addr, i;
  symbol **slist;

	/* put all symbols in an array */
	slist = Safe_calloc(elementsInSet(section), sizeof(symbol *));
	s = slist[0]; i = 0;
	for(sprev = setFirstItem(section); sprev; sprev = setNextItem(section)) {
		slist[i] = sprev; i++;
	}
	
	if(!i) {
		if(slist)free(slist);
	  return;
	}

	/* sort symbols according to their address */
	qsort(slist, i, sizeof(symbol *), symCompare);
	
	if(!fix) {
		fprintf(of, "\n\n\tidata\n");
		for(s = setFirstItem(section); s; s = setNextItem(section)) {
			print_idata(of, s, s->type, s->ival);
		}
	} else {
	  int j=0;
	  symbol *s1;
	  
		sprev = NULL;
		init_addr = SPEC_ADDR(slist[j]->etype);
		fprintf(of, "\n\nistat_%s_%02d\tidata\t0X%04X\n", moduleName, abs_isection_no++, init_addr);
	
		for(j=0;j<i;j++) {
			s = slist[j];
			if(j < i-1)s1 = slist[j+1]; else s1 = NULL;
			
			init_addr = SPEC_ADDR(s->etype);

			if(sprev && (init_addr > (SPEC_ADDR(sprev->etype) + getSize(sprev->etype)))) {
				fprintf(of, "\nistat_%s_%02d\tidata\t0X%04X\n", moduleName, abs_isection_no++, init_addr);
			}

			print_idata(of, s, s->type, s->ival);
			
#if 0
			if(r1 && (init_addr == r1->address)) {
				fprintf(of, "%s\tres\t0\n\n", r->name);
			} else {
				fprintf(of, "%s\tres\t%d\n", r->name, r->size);
			}
#endif
			
			sprev = s;
		}
	}
	free(slist);
}


void pic16_dump_int_registers(FILE *of, set *section)
{
  regs *r, *rprev;
  int i;
  regs **rlist;

	/* put all symbols in an array */
	rlist = Safe_calloc(elementsInSet(section), sizeof(regs *));
	r = rlist[0]; i = 0;
	for(rprev = setFirstItem(section); rprev; rprev = setNextItem(section)) {
		rlist[i] = rprev; i++;
	}

	/* sort symbols according to their address */
	qsort(rlist, elementsInSet(section), sizeof(regs *), regCompare);
	
	if(!i) {
		if(rlist)free(rlist);
	  return;
	}
	
	fprintf(of, "\n\n; Internal registers\n");
	
	fprintf(of, "%s\tudata_ovr\t0x0000\n", ".registers");
	for(r = setFirstItem(section); r; r = setNextItem(section))
		fprintf(of, "%s\tres\t%d\n", r->name, r->size);

	free(rlist);
}


#ifdef WORDS_BIGENDIAN
  #define _ENDIAN(x)  (3-x)
#else
  #define _ENDIAN(x)  (x)
#endif

#define BYTE_IN_LONG(x,b) ((x>>(8*_ENDIAN(b)))&0xff)



/*-----------------------------------------------------------------*/
/* printIvalType - generates ival for int/char                     */
/*-----------------------------------------------------------------*/
void print_idataType (FILE *of, symbol *sym, sym_link * type, initList * ilist)
{
  value *val;
  unsigned long ulval;

  //fprintf(stderr, "%s\n",__FUNCTION__);

  /* if initList is deep */
  if (ilist->type == INIT_DEEP)
    ilist = ilist->init.deep;

  if (!IS_AGGREGATE(sym->type) && getNelements(type, ilist)>1) {
    werror (W_EXCESS_INITIALIZERS, "scalar", sym->name, sym->lineDef);
  }

  if (!(val = list2val (ilist))) {
    // assuming a warning has been thrown
    val=constVal("0");
  }

  if (val->type != type) {
    val = valCastLiteral(type, floatFromVal(val));
  }

  if(val) 
    ulval = (unsigned long) floatFromVal (val);
  else
    ulval =0;

  switch (getSize (type)) {
    case 1:
	if(isprint(BYTE_IN_LONG(ulval, 0)))
		fprintf(of, "%s\tdata\t\"%c\"\n", sym->rname, (unsigned char)BYTE_IN_LONG(ulval, 0));
	else
		fprintf(of, "%s\tdata\t0x%02x\n", sym->rname, (unsigned char)BYTE_IN_LONG(ulval, 0));
//	pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
	break;

    case 2:
	fprintf(of, "%s\tdw\t0x%02x, 0x%02x\n", sym->rname, (unsigned int)(BYTE_IN_LONG(ulval, 0)),
					(unsigned int)(BYTE_IN_LONG(ulval, 1)));
//	fprintf(of, "%s\tdata\t0x%04x\n", sym->rname, (unsigned int)BYTE_IN_LONG(ulval, 0) +
//					(unsigned int)(BYTE_IN_LONG(ulval, 1) << 8));
	break;

    case 4:
	fprintf(of, "%s\tdw\t0x%04x,0x%04x\n", sym->rname, (unsigned int)(BYTE_IN_LONG(ulval, 0)
					+ (BYTE_IN_LONG(ulval, 1) << 8)),
					(unsigned)(BYTE_IN_LONG(ulval, 2)
					+ (BYTE_IN_LONG(ulval, 3) << 8)));
//	pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,0))));
//	pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,1))));
//	pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,2))));
//	pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(BYTE_IN_LONG(ulval,3))));
	break;
  }
}


/*-----------------------------------------------------------------*/
/* print_idataChar - generates initital value for character array    */
/*-----------------------------------------------------------------*/
static int 
print_idataChar (FILE *of, symbol *sym, sym_link * type, initList * ilist, char *s)
{
  value *val;
  int remain;
  char old_ch=0, *vchar;

  // fprintf(stderr, "%s\n",__FUNCTION__);
  if (!s)
    {

      val = list2val (ilist);
      /* if the value is a character string  */
      if (IS_ARRAY (val->type) && IS_CHAR (val->etype))
	{
	  if (!DCL_ELEM (type))
	    DCL_ELEM (type) = strlen (SPEC_CVAL (val->etype).v_char) + 1;


	  fprintf(of, "%s\tdata\t", sym->rname);
	  vchar = SPEC_CVAL(val->etype).v_char;
	  for(remain=0; remain<DCL_ELEM(type); remain++) {
		if(!isprint(old_ch)) {
			if(isprint(vchar[remain]))fprintf(of, "%s\"%c", (remain?", ":""), vchar[remain]);
			else fprintf(of, "0x%02x", vchar[remain]);
		} else
			if(!isprint(vchar[remain]))fprintf(of, "\", 0x%02x", vchar[remain]);
			else fprintf(of, "%c", vchar[remain]);
		old_ch = vchar[ remain ];
	  }
//	     fprintf(of, "0x%02x%s", SPEC_CVAL(val->etype).v_char[ remain ], (remain==DCL_ELEM(type)-1?"":","));
			

	  if ((remain = (DCL_ELEM (type) - strlen (SPEC_CVAL (val->etype).v_char) - 1)) > 0)
            {
	      while (remain--)
                {
		  fprintf(of, "0x%02x%s", 0x00 /*SPEC_CVAL(val->etype).v_char[ remain ]*/ , (remain==DCL_ELEM(type)-1?"":","));
                }
            }
		fprintf(of, "\n");
	  return 1;
	}
      else
	return 0;
    }
  else {
    fprintf(of, "%s\tdata\t", sym->rname);
    vchar = s; old_ch = 0;
    for(remain=0; remain<strlen(s); remain++) 
      {
	if(!isprint(old_ch)) {
		if(isprint(vchar[remain]))fprintf(of, "%s\"%c", (remain?", ":""), vchar[remain]);
		else fprintf(of, "0x%02x", vchar[remain]);
	} else
		if(!isprint(vchar[remain]))fprintf(of, "\", 0x%02x", vchar[remain]);
		else fprintf(of, "%c", vchar[remain]);
	old_ch = vchar[ remain ];
//	fprintf(of, "0x%02x%s", s[ remain ], (remain==strlen(s)-1?"":","));
      }
  }
  return 1;
}

/*-----------------------------------------------------------------*/
/* print_idataArray - generates code for array initialization        */
/*-----------------------------------------------------------------*/
static void 
print_idataArray (FILE *of, symbol * sym, sym_link * type, initList * ilist)
{
  initList *iloop;
  int lcnt = 0, size = 0;

  /* take care of the special   case  */
  /* array of characters can be init  */
  /* by a string                      */
  if (IS_CHAR (type->next)) {
//    fprintf(stderr,"%s:%d - is_char\n",__FUNCTION__,__LINE__);
    if (!IS_LITERAL(list2val(ilist)->etype)) {
      werror (W_INIT_WRONG);
      return;
    }
    if (print_idataChar (of, sym, type,
		       (ilist->type == INIT_DEEP ? ilist->init.deep : ilist),
		       SPEC_CVAL (sym->etype).v_char))
      return;
  }
  /* not the special case             */
  if (ilist->type != INIT_DEEP)
    {
      werror (E_INIT_STRUCT, sym->name);
      return;
    }

  iloop = ilist->init.deep;
  lcnt = DCL_ELEM (type);

  for (;;)
    {
      //fprintf(stderr,"%s:%d - is_char\n",__FUNCTION__,__LINE__);
      size++;
      print_idata (of, sym, type->next, iloop);
      iloop = (iloop ? iloop->next : NULL);


      /* if not array limits given & we */
      /* are out of initialisers then   */
      if (!DCL_ELEM (type) && !iloop)
	break;

      /* no of elements given and we    */
      /* have generated for all of them */
      if (!--lcnt) {
	/* if initializers left */
	if (iloop) {
	  werror (W_EXCESS_INITIALIZERS, "array", sym->name, sym->lineDef);
	}
	break;
      }
    }

  /* if we have not been given a size  */
  if (!DCL_ELEM (type))
    DCL_ELEM (type) = size;

  return;
}

/*-----------------------------------------------------------------*/
/* print_idata - generates code for initial value                    */
/*-----------------------------------------------------------------*/
void print_idata(FILE *of, symbol * sym, sym_link * type, initList * ilist)
{
  if (!ilist)
    return;

  /* if structure then    */
  if (IS_STRUCT (type))
    {
      //fprintf(stderr,"%s struct\n",__FUNCTION__);
      //printIvalStruct (sym, type, ilist, oFile);
      fprintf(stderr, "%s:%d: PIC16 port error: structure initialisation is not implemented yet.\n",
      	__FILE__, __LINE__);
      abort();
      return;
    }

  /* if this is a pointer */
  if (IS_PTR (type))
    {
      //fprintf(stderr,"%s pointer\n",__FUNCTION__);
      //printIvalPtr (sym, type, ilist, oFile);
      fprintf(stderr, "%s:%d: PIC16 port error: pointer initialisation is not immplemented yet.\n",
      	__FILE__, __LINE__);
      abort();
      return;
    }

  /* if this is an array   */
  if (IS_ARRAY (type))
    {
      //fprintf(stderr,"%s array\n",__FUNCTION__);
      print_idataArray (of, sym, type, ilist);
      return;
    }

  /* if type is SPECIFIER */
  if (IS_SPEC (type))
    {
//	fprintf(stderr,"%s spec\n",__FUNCTION__);
      print_idataType(of, sym, type, ilist);
      return;
    }
}




/*-----------------------------------------------------------------*
 *  void pic16_list_valid_pics(int ncols, int list_alias)
 *
 * Print out a formatted list of valid PIC devices
 *
 * ncols - number of columns in the list.
 *
 * list_alias - if non-zero, print all of the supported aliases
 *              for a device (e.g. F84, 16F84, etc...)
 *-----------------------------------------------------------------*/
void pic16_list_valid_pics(int ncols, int list_alias)
{
  int col,longest;
  int i,j,k,l;

  if(list_alias)
    list_alias = sizeof(Pics16[0].name) / sizeof(Pics16[0].name[0]);

  /* decrement the column number if it's greater than zero */
  ncols = (ncols > 1) ? ncols-1 : 4;

  /* Find the device with the longest name */
  for(i=0,longest=0; i<num_of_supported_PICS; i++) {
    for(j=0; j<=list_alias; j++) {
      k = strlen(Pics16[i].name[j]);
      if(k>longest)
	longest = k;
    }
  }

  col = 0;

  for(i=0;  i < num_of_supported_PICS; i++) {
    j = 0;
    do {

      fprintf(stderr,"%s", Pics16[i].name[j]);
      if(col<ncols) {
	l = longest + 2 - strlen(Pics16[i].name[j]);
	for(k=0; k<l; k++)
	  fputc(' ',stderr);

	col++;

      } else {
	fputc('\n',stderr);
	col = 0;
      }

    } while(++j<list_alias);

  }
  if(col != ncols)
    fputc('\n',stderr);

}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
PIC16_device *pic16_find_device(char *name)
{

  int i,j;

  if(!name)
    return NULL;

  for(i = 0; i<num_of_supported_PICS; i++) {

    for(j=0; j<PROCESSOR_NAMES; j++)
      if(!STRCASECMP(Pics16[i].name[j], name) )
	return &Pics16[i];
  }

  /* not found */
  return NULL; 
}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
void pic16_init_pic(char *pic_type)
{
	pic16 = pic16_find_device(pic_type);

	if(!pic16) {
		if(pic_type)
			fprintf(stderr, "'%s' was not found.\n", pic_type);
		else
			fprintf(stderr, "No processor has been specified (use -pPROCESSOR_NAME)\n");

		fprintf(stderr,"Valid devices are:\n");

		pic16_list_valid_pics(4,0);
		exit(1);
	}

//	printf("PIC processor found and initialized: %s\n", pic_type);
	pic16_setMaxRAM( 0xfff  );
}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
int pic16_picIsInitialized(void)
{
  if(pic16 && pic16->maxRAMaddress > 0)
    return 1;

  return 0;

}

/*-----------------------------------------------------------------*
 *  char *pic16_processor_base_name(void) - Include file is derived from this.
 *-----------------------------------------------------------------*/
char *pic16_processor_base_name(void)
{

  if(!pic16)
    return NULL;

  return pic16->name[0];
}

#define DEBUG_CHECK	0

/*
 * return 1 if register wasn't found and added, 0 otherwise
 */
int checkAddReg(set **set, regs *reg)
{
  regs *tmp;

#if DEBUG_CHECK
	fprintf(stderr, "%s: about to insert REGister: %s ... ", __FUNCTION__, reg->name);
#endif

	for(tmp = setFirstItem(*set); tmp; tmp = setNextItem(*set)) {
		if(!strcmp(tmp->name, reg->name))break;
	}
	
	if(!tmp) {
		addSet(set, reg);
#if DEBUG_CHECK
		fprintf(stderr, "added\n");
#endif
		return 1;
	}

#if DEBUG_CHECK
	fprintf(stderr, "already added\n");
#endif
  return 0;
}

int checkAddSym(set **set, symbol *sym)
{
  symbol *tmp;

#if DEBUG_CHECK
	fprintf(stderr, "%s: about to add SYMbol: %s ... ", __FUNCTION__, sym->name);
#endif

	for(tmp = setFirstItem( *set ); tmp; tmp = setNextItem(*set)) {
		if(!strcmp(tmp->name, sym->name))break;
	}
	
	if(!tmp) {
		addSet(set, sym);
#if DEBUG_CHECK
		fprintf(stderr, "added\n");
#endif
		return 1;
	}

#if DEBUG_CHECK
	fprintf(stderr, "already added\n");
#endif

  return 0;
}


/*-----------------------------------------------------------------*
 * void pic16_groupRegistersInSection - add each register to its   *
 *	corresponding section                                      *
 *-----------------------------------------------------------------*/
void pic16_groupRegistersInSection(set *regset)
{
  regs *reg;

	for(reg=setFirstItem(regset); reg; reg = setNextItem(regset)) {

//		fprintf(stderr, "%s:%d group registers in section, reg: %s\n", __FILE__, __LINE__, reg->name);

		if(reg->wasUsed
			&& !(reg->regop && SPEC_EXTR(OP_SYM_ETYPE(reg->regop)))) {
			
			/* avoid grouping registers that have an initial value,
			 * they will be added later in idataSymSet */
			if(reg->regop && (OP_SYMBOL(reg->regop)->ival && !OP_SYMBOL(reg->regop)->level))
				continue;

#if 0
			fprintf(stderr, "%s:%d register %s alias:%d fix:%d ival=%i level=%i\n",
				__FILE__, __LINE__, reg->name, reg->alias, reg->isFixed,
					(reg->regop?(OP_SYMBOL(reg->regop)->ival?1:0):-1),
					(reg->regop?(OP_SYMBOL(reg->regop)->level):-1) );
#endif

			if(reg->alias) {
				checkAddReg(&pic16_equ_data, reg);
			} else
			if(reg->isFixed) {
				checkAddReg(&pic16_fix_udata, reg);
			} else
			if(!reg->isFixed) {
				if(reg->pc_type == PO_GPR_TEMP)
					checkAddReg(&pic16_int_regs, reg);
				else
					checkAddReg(&pic16_rel_udata, reg);
			}
		}
	}
}





/*-----------------------------------------------------------------*
 *  void pic16_assignConfigWordValue(int address, int value)
 *
 * All high performance RISC CPU PICs have seven config word starting
 * at address 0x300000.
 * This routine will assign a value to that address.
 *
 *-----------------------------------------------------------------*/
void pic16_assignConfigWordValue(int address, int value)
{
  int i;


        for(i=0;i<pic16->cwInfo.confAddrEnd-pic16->cwInfo.confAddrStart+1;i++) {
                if((address == pic16->cwInfo.confAddrStart+i)
                  && (pic16->cwInfo.crInfo[i].mask != -1)) {
//                        fprintf(stderr, "setting location 0x%X to value 0x%x\n", /*address*/ pic16->cwInfo.confAddrStart+i, val
                        pic16->cwInfo.crInfo[i].value = value;
                        pic16->cwInfo.crInfo[i].emit = 1;
                        return;
                }
        }
}

