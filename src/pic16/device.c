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

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

static PIC_device Pics[] = {
  {
    {"p18f242", "18f242", "pic18f242", "f242"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0
  },

  {
    {"p18f252", "18f252", "pic18f252", "f252"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0
  },

  {
    {"p18f442", "18f442", "pic18f442", "f442"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0
  },

  {
    {"p18f452", "18f452", "pic18f452", "f452"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0
  },

  {
    {"p18f248", "18f248", "pic18f248", "f248"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0
  },

  {
    {"p18f258", "18f258", "pic18f258", "f258"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0
  },

  {
    {"p18f448", "18f448", "pic18f448", "f448"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x300,						// bank mask
    0x300,						// RAMsize
    0
  },

  {
    {"p18f458", "18f458", "pic18f458", "f458"},		// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x600,						// bank mask
    0x600,						// RAMsize
    0
  },

  {
    {"p18f6520", "18f6520", "pic18f6520", "f6520"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x800,						// bank mask
    0x800,						// RAMsize
    1
  },

  {
    {"p18f6620", "18f6620", "pic18f6620", "f6620"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1
  },
  {
    {"p18f6680", "18f6680", "pic18f6680", "f6680"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xc00,						// bank mask
    0xc00,						// RAMsize
    1
  },
  {
    {"p18f6720", "18f6720", "pic18f6720", "f6720"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1
  },
  {
    {"p18f8520", "18f8520", "pic18f8520", "f8520"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0x800,						// bank mask
    0x800,						// RAMsize
    1
  },
  {
    {"p18f8620", "18f8620", "pic18f8620", "f8620"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1
  },
  {
    {"p18f8680", "18f8680", "pic18f8680", "f8680"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xc00,						// bank mask
    0x800,						// RAMsize
    1
  },
  {
    {"p18f8720", "18f8720", "pic18f8720", "f8720"},	// aliases
    (memRange *)NULL,					// ram memory map
    (memRange *)NULL,					// sfr memory map
    0,
    0xf00,						// bank mask
    0xf00,						// RAMsize
    1
  },

};

static int num_of_supported_PICS = sizeof(Pics)/sizeof(PIC_device);

#define DEFAULT_PIC "452"

static PIC_device *pic=NULL;

//AssignedMemory *pic16_finalMapping=NULL;
//int pic16_finalMappingSize=0;

#define DEFAULT_CONFIG_BYTE 0xff

#define CONFIG1H_WORD_ADDRESS 0x300001
#define DEFAULT_CONFIG1H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG2L_WORD_ADDRESS 0x300002
#define DEFAULT_CONFIG2L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG2H_WORD_ADDRESS 0x300003
#define DEFAULT_CONFIG2H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG3H_WORD_ADDRESS 0x300005
#define DEFAULT_CONFIG3H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG4L_WORD_ADDRESS 0x300006
#define DEFAULT_CONFIG4L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG5L_WORD_ADDRESS 0x300008
#define DEFAULT_CONFIG5L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG5H_WORD_ADDRESS 0x300009
#define DEFAULT_CONFIG5H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG6L_WORD_ADDRESS 0x30000a
#define DEFAULT_CONFIG6L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG6H_WORD_ADDRESS 0x30000b
#define DEFAULT_CONFIG6H_WORD DEFAULT_CONFIG_BYTE

#define CONFIG7L_WORD_ADDRESS 0x30000c
#define DEFAULT_CONFIG7L_WORD DEFAULT_CONFIG_BYTE

#define CONFIG7H_WORD_ADDRESS 0x30000d
#define DEFAULT_CONFIG7H_WORD DEFAULT_CONFIG_BYTE

static unsigned int config1h_word = DEFAULT_CONFIG1H_WORD;
static unsigned int config2l_word = DEFAULT_CONFIG2L_WORD;
static unsigned int config2h_word = DEFAULT_CONFIG2H_WORD;
static unsigned int config3h_word = DEFAULT_CONFIG3H_WORD;
static unsigned int config4l_word = DEFAULT_CONFIG4L_WORD;
static unsigned int config5l_word = DEFAULT_CONFIG5L_WORD;
static unsigned int config5h_word = DEFAULT_CONFIG5H_WORD;
static unsigned int config6l_word = DEFAULT_CONFIG6L_WORD;
static unsigned int config6h_word = DEFAULT_CONFIG6H_WORD;
static unsigned int config7l_word = DEFAULT_CONFIG7L_WORD;
static unsigned int config7h_word = DEFAULT_CONFIG7H_WORD;

unsigned int stackPos = 0;

extern regs* newReg(short type, short pc_type, int rIdx, char *name, int size, int alias, operand *refop);

void pic16_setMaxRAM(int size)
{
  pic->maxRAMaddress = size;
  stackPos = pic->RAMsize-1;
  regs * reg;

	reg=newReg(REG_SFR, PO_SFR_REGISTER, stackPos, "stack", 1, 0, NULL);
	addSet(&pic16_fix_udata, reg);

  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "invalid \"#pragma maxram 0x%x\" setting\n",
	    pic->maxRAMaddress);
    return;
  }

//  pic16_finalMapping = Safe_calloc(1+pic->maxRAMaddress,
//			     sizeof(AssignedMemory));

  /* Now initialize the pic16_finalMapping array */

//  for(i=0; i<=pic->maxRAMaddress; i++) {
//    pic16_finalMapping[i].reg = NULL;
//    pic16_finalMapping[i].isValid = 0;
//  }
}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
#if 0
int pic16_isREGinBank(regs *reg, int bank)
{

  if(!reg || !pic)
    return 0;

  if(pic16_finalMapping[reg->address].bank == bank)
    return 1;

  return 0;
}
#endif
/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
int pic16_REGallBanks(regs *reg)
{

  if(!reg || !pic)
    return 0;

  if ((int)reg->address > pic->maxRAMaddress)
    return 0;

  return 1;

}

/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/

/*
 *  pic16_dump_map -- debug stuff
 */
#if 0
void pic16_dump_map(void)
{
  int i;

  for(i=0; i<=pic->maxRAMaddress; i++) {
    //fprintf(stdout , "addr 0x%02x is %s\n", i, ((pic16_finalMapping[i].isValid) ? "valid":"invalid"));

    if(pic16_finalMapping[i].isValid) {
      fprintf(stderr,"addr: 0x%02x",i);
      if(pic16_finalMapping[i].isSFR)
	fprintf(stderr," isSFR");
      if(pic16_finalMapping[i].reg) 
	fprintf( stderr, "  reg %s", pic16_finalMapping[i].reg->name);
      fprintf(stderr, "\n");
    }
  }

}
#endif

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

	if( (*i)->address > (*j)->address)return 1;
	if( (*i)->address < (*j)->address)return -1;

  return 0;
}

void pic16_dump_section(FILE *of, set *section, int fix)
{
  static int abs_section_no=0;
  regs *r, *rprev;
  int init_addr, i;
  regs **rlist;

	/* put all symbols in an array */
	rlist = Safe_calloc(elementsInSet(section), sizeof(regs *));
	r = rlist[0]; i = 0;
	for(rprev = setFirstItem(section); rprev; rprev = setNextItem(section)) {
		rlist[i] = rprev; i++;
	}

	/* sort symbols according to their address */
	qsort(rlist, elementsInSet(section), sizeof(regs *), regCompare);
	
	if(!i)return;
	
	if(!fix) {
		fprintf(of, "\n\n\tudata\n");
		for(r = setFirstItem(section); r; r = setNextItem(section)) {
			fprintf(of, "%s\tres\t%d\n", r->name, r->size);
		}
	} else {
	  int j=0;
		  
		rprev = NULL;
		init_addr = rlist[j]->address;
		fprintf(of, "\n\nstatic_%s_%02d\tudata\t0X%04X\n", moduleName, abs_section_no++, init_addr);
	
		for(j=0;j<i;j++) {
			r = rlist[j];
			init_addr = r->address;
			if(rprev && (init_addr != (rprev->address + rprev->size))) {
				fprintf(of, "\nstatic_%s_%02d\tudata\t0X%04X\n", moduleName, abs_section_no++, init_addr);
			}

			fprintf(of, "%s\tres\t%d\n", r->name, r->size);
			rprev = r;
		}
	}
	free(rlist);
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
    list_alias = sizeof(Pics[0].name) / sizeof(Pics[0].name[0]);

  /* decrement the column number if it's greater than zero */
  ncols = (ncols > 1) ? ncols-1 : 4;

  /* Find the device with the longest name */
  for(i=0,longest=0; i<num_of_supported_PICS; i++) {
    for(j=0; j<=list_alias; j++) {
      k = strlen(Pics[i].name[j]);
      if(k>longest)
	longest = k;
    }
  }

  col = 0;

  for(i=0;  i < num_of_supported_PICS; i++) {
    j = 0;
    do {

      fprintf(stderr,"%s", Pics[i].name[j]);
      if(col<ncols) {
	l = longest + 2 - strlen(Pics[i].name[j]);
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
PIC_device *pic16_find_device(char *name)
{

  int i,j;

  if(!name)
    return NULL;

  for(i = 0; i<num_of_supported_PICS; i++) {

    for(j=0; j<PROCESSOR_NAMES; j++)
      if(!STRCASECMP(Pics[i].name[j], name) )
	return &Pics[i];
  }

  /* not found */
  return NULL; 
}

/*-----------------------------------------------------------------*
 *  
 *-----------------------------------------------------------------*/
void pic16_init_pic(char *pic_type)
{
	pic = pic16_find_device(pic_type);

	if(!pic) {
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
  if(pic && pic->maxRAMaddress > 0)
    return 1;

  return 0;

}

/*-----------------------------------------------------------------*
 *  char *pic16_processor_base_name(void) - Include file is derived from this.
 *-----------------------------------------------------------------*/
char *pic16_processor_base_name(void)
{

  if(!pic)
    return NULL;

  return pic->name[0];
}


#if 0
/*-----------------------------------------------------------------*
 *-----------------------------------------------------------------*/
static int validAddress(int address, int reg_size)
{
  int i;

#if 0
  if (pic->maxRAMaddress < 0) {
    fprintf(stderr, "missing \"#pragma maxram\" setting\n");
    return 0;
  }
//  fprintf(stderr, "validAddress: Checking 0x%04x (max=0x%04x) (reg_size = %d)\n",address, pic->maxRAMaddress, reg_size);
  if(address > pic->maxRAMaddress)
    return 0;

  for (i=0; i<reg_size; i++)
    if(!pic16_finalMapping[address + i].isValid || 
       pic16_finalMapping[address+i].reg ||
       pic16_finalMapping[address+i].isSFR )
      return 0;
#endif

  return 1;
}
#endif

void checkAddReg(set **set, regs *reg)
{
  regs *tmp;


	for(tmp = setFirstItem(*set); tmp; tmp = setNextItem(*set)) {
		if(!strcmp(tmp->name, reg->name))break;
	}
	
	if(!tmp)
		addSet(set, reg);
}

/*-----------------------------------------------------------------*
 * void pic16_groupRegistersInSection - add each register to its   *
 *	corresponding section                                      *
 *-----------------------------------------------------------------*/
void pic16_groupRegistersInSection(set *regset)
{
  regs *reg;

	for(reg=setFirstItem(regset); reg; reg = setNextItem(regset)) {
		if(reg->wasUsed
			&& !(reg->regop && SPEC_EXTR(OP_SYM_ETYPE(reg->regop)))) {

//			fprintf(stderr, "%s:%d register %s\n", __FILE__, __LINE__, reg->name);

			if(reg->alias) {
				checkAddReg(&pic16_equ_data, reg);
			} else
			if(reg->isFixed) {
				checkAddReg(&pic16_fix_udata, reg);
			} else
			if(!reg->isFixed) {
//				fprintf(stderr, "%s:%d adding symbol %s in relocatable udata section\n",
//					__FILE__, __LINE__, reg->name);
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
  switch(address) {
  case CONFIG1H_WORD_ADDRESS:
    config1h_word = value;
    break;
  case CONFIG2L_WORD_ADDRESS:
    config2l_word = value;
    break;
  case CONFIG2H_WORD_ADDRESS:
    config2h_word = value;
    break;
  case CONFIG3H_WORD_ADDRESS:
    config3h_word = value;
    break;
  case CONFIG4L_WORD_ADDRESS:
    config4l_word = value;
    break;
  case CONFIG5L_WORD_ADDRESS:
    config5l_word = value;
    break;
  case CONFIG5H_WORD_ADDRESS:
    config5h_word = value;
    break;
  case CONFIG6L_WORD_ADDRESS:
    config6l_word = value;
    break;
  case CONFIG6H_WORD_ADDRESS:
    config6h_word = value;
    break;
  case CONFIG7L_WORD_ADDRESS:
    config7l_word = value;
    break;
  case CONFIG7H_WORD_ADDRESS:
    config7h_word = value;
    break;
  }

	fprintf(stderr,"setting config word to 0x%x\n",value);

}
/*-----------------------------------------------------------------*
 * int pic16_getConfigWord(int address)
 *
 * Get the current value of the config word.
 *
 *-----------------------------------------------------------------*/

int pic16_getConfigWord(int address)
{
  switch(address) {
  case CONFIG1H_WORD_ADDRESS:
    return config1h_word;
  case CONFIG2L_WORD_ADDRESS:
    return config2l_word;
  case CONFIG2H_WORD_ADDRESS:
    return config2h_word;
  case CONFIG3H_WORD_ADDRESS:
    return config3h_word;
  case CONFIG4L_WORD_ADDRESS:
    return config4l_word;
  case CONFIG5L_WORD_ADDRESS:
    return config5l_word;
  case CONFIG5H_WORD_ADDRESS:
    return config5h_word;
  case CONFIG6L_WORD_ADDRESS:
    return config6l_word;
  case CONFIG6H_WORD_ADDRESS:
    return config6h_word;
  case CONFIG7L_WORD_ADDRESS:
    return config7l_word;
  case CONFIG7H_WORD_ADDRESS:
    return config7h_word;
  default:
    return 0;
  }
}
