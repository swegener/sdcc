/*-------------------------------------------------------------------------

   device.c - Accomodates subtle variations in PIC devices
   Written By -  Scott Dattalo scott@dattalo.com

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

/* 16F627 */
memRange p16f627_mem[] = {
  {0x20,  0x6f,  0x00,  0},
  {0xa0,  0xef,  0x00,  1},
  {0x120, 0x14f, 0x00,  2},
  {0x70,  0x7f,  0x180, 0},
  {-1,    -1,    -1,   -1}     /* end indicator */
};
memRange p16f627_sfr[] = {
  {0x00,  0x00,  0x180, 0},
  {0x01,  0x01,  0x100, 0},
  {0x02,  0x04,  0x180, 0},
  {0x05,  0x05,  0x000, 0},
  {0x06,  0x06,  0x100, 0},
  {0x81,  0x81,  0x100, 1},
  {0x85,  0x85,  0x000, 1},
  {0x86,  0x86,  0x100, 1},
  {0x0a,  0x0b,  0x180, 0},
  {0x0c,  0x0c,  0x000, 0},
  {0x0e,  0x12,  0x000, 0},
  {0x15,  0x1a,  0x000, 0},
  {0x1f,  0x1f,  0x000, 0},
  {0x8e,  0x8e,  0x000, 1},
  {0x92,  0x92,  0x000, 1},
  {0x98,  0x9d,  0x000, 1},
  {0x9f,  0x9f,  0x000, 1},

  {-1,    -1,    -1,   -1}     /* end indicator */
};

/* 16F84 */
memRange p16f84_mem[] = {
  {0x0c,  0x4f,  0x80,  0},
  {-1,    -1,    -1,   -1}     /* end indicator */
};
memRange p16f84_sfr[] = {
  {0x01,  0x01,  0x00, 0},
  {0x02,  0x04,  0x80, 0},
  {0x05,  0x06,  0x00, 0},
  {0x81,  0x81,  0x00, 1},
  {0x85,  0x86,  0x00, 1},
  {0x08,  0x09,  0x00, 0},
  {0x88,  0x89,  0x00, 1},
  {0x0a,  0x0b,  0x80, 0},
  {-1,    -1,    -1,   -1}     /* end indicator */
};

/* 16F877 */
memRange p16f877_mem[] = {
  {0x20,  0x6f,  0x00,  0},
  {0xa0,  0xef,  0x00,  1},
  {0x110, 0x16f, 0x00,  2},
  {0x190, 0x1ef, 0x00,  3},
  {0x70,  0x7f,  0x180, 0},
  {-1,    -1,    -1,   -1}     /* end indicator */
};
memRange p16f877_sfr[] = {
  {0x00,  0x00,  0x180, 0},
  {0x01,  0x01,  0x100, 0},
  {0x02,  0x04,  0x180, 0},
  {0x05,  0x05,  0x000, 0},
  {0x85,  0x85,  0x000, 1},
  {0x81,  0x81,  0x100, 1},
  {0x06,  0x06,  0x100, 0},
  {0x86,  0x86,  0x100, 1},
  {0x07,  0x09,  0x000, 0},
  {0x87,  0x89,  0x000, 1},
  {0x0a,  0x0b,  0x180, 0},
  {0x0c,  0x1f,  0x000, 0},
  {0x8c,  0x8e,  0x000, 1},
  {0x91,  0x94,  0x000, 1},
  {0x98,  0x99,  0x000, 1},
  {0x9e,  0x9f,  0x000, 1},
  {0x10c, 0x10f, 0x000, 2},
  {0x18c, 0x18f, 0x000, 3},

  {-1,    -1,    -1,   -1}     /* end indicator */
};


static PIC_device Pics[] = {
  {
    {"p16f627", "16f627", "pic16f627", "f627"}, /* processor name */
    p16f627_mem,                     /* ram mem map */
    p16f627_sfr,                     /* sfr mem map */
    0,                               /* max ram address (calculated) */
  },

  {
    {"p16f628", "16f628", "pic16f628", "f628"},
    p16f627_mem,
    p16f627_sfr,
    0,
  },

  {
    {"p16f84", "16f84", "pic16f84", "f84"},
    p16f84_mem,
    p16f84_sfr,
    0,
  },

  {
    {"p16f877", "16f877", "pic16f877", "f877"},
    p16f877_mem,
    p16f877_sfr,
    0,
  },

};

static int num_of_supported_PICS = sizeof(Pics)/sizeof(PIC_device);

#define DEFAULT_PIC "f877"

static PIC_device *pic=NULL;

AssignedMemory *finalMapping=NULL;

#define CONFIG_WORD_ADDRESS 0x2007
#define DEFAULT_CONFIG_WORD 0x3fff

static unsigned int config_word = DEFAULT_CONFIG_WORD;

/*-----------------------------------------------------------------*
 *
 * void addMem(memRange *ranges,int type)
 *
 *
 *-----------------------------------------------------------------*/

static void addMem(memRange *ranges,int type)
{
  memRange *r = ranges;
  int i;

  do {

    int alias = r->alias;

    do {

      for(i=r->start_address; i<= r->end_address; i++) {
	if(i <= pic->maxRAMaddress) {
	  finalMapping[i | alias].isValid = 1;
	  finalMapping[i | alias].alias = r->alias;
	  finalMapping[i | alias].bank  = r->bank;
	  if(type) {
	    /* hack for now */
	    finalMapping[i | alias].isSFR  = 1;
	  } else
	    finalMapping[i | alias].isSFR  = 0;
	}
      }

      /* Decrement alias */
      if(alias)
	alias -= ((alias & (alias - 1)) ^ alias);
	else
	  alias--;
	
    } while(alias >= 0);

    r++;

  } while (r->start_address >= 0);


}

static void addMaps(PIC_device *pPic)
{
  int i;
  memRange *r;

  if(!pPic)
    return;


  /* First, find the maximum address */

  r = pPic->ram;
  pPic->maxRAMaddress = 0;

  do {

    if((r->end_address | r->alias) > pPic->maxRAMaddress)
      pPic->maxRAMaddress = r->end_address | r->alias;

    r++;

  } while (r->start_address >= 0);



  finalMapping = Safe_calloc(1+pPic->maxRAMaddress, sizeof(AssignedMemory));

  /* Now initialize the finalMapping array */

  for(i=0; i<=pPic->maxRAMaddress; i++) {
    finalMapping[i].reg = NULL;
    finalMapping[i].isValid = 0;
  }

  addMem(pPic->ram,0); /* add general purpose regs to the map */
  addMem(pPic->sfr,1); /* Add SFR's to the memmap */

}

/*
 *  dump_map -- debug stuff
 */

void dump_map(void)
{
  int i;

  for(i=0; i<=pic->maxRAMaddress; i++) {
    //fprintf(stdout , "addr 0x%02x is %s\n", i, ((finalMapping[i].isValid) ? "valid":"invalid"));

    if(finalMapping[i].isValid) {
      fprintf(stderr,"addr: 0x%02x",i);
      if(finalMapping[i].isSFR)
	fprintf(stderr," isSFR");
      if(finalMapping[i].reg) 
	fprintf( stderr, "  reg %s", finalMapping[i].reg->name);
      fprintf(stderr, "\n");
    }
  }

}

void dump_cblock(FILE *of)
{
  int start=-1;
  int addr=0;
  int bank_base;

  //dump_map();   /* display the register map */

  do {

    if(finalMapping[addr].reg && !finalMapping[addr].reg->isEmitted) {

      if(start<0)
	start = addr;
    } else {
      if(start>=0) {

	/* clear the lower 7-bits of the start address of the first
	 * variable declared in this bank. The upper bits for the mid
	 * range pics are the bank select bits.
	 */

	bank_base = start & 0xfff8;

	/* The bank number printed in the cblock comment tacitly
	 * assumes that the first register in the contiguous group
	 * of registers represents the bank for the whole group */

	fprintf(of,"  cblock  0X%04X\t; Bank %d\n",start,finalMapping[start].bank);

	for( ; start < addr; start++) {
	  if((finalMapping[start].reg) && !finalMapping[start].reg->isEmitted ) {
	    fprintf(of,"\t%s",finalMapping[start].reg->name);

	    /* If this register is aliased in multiple banks, then
	     * mangle the variable name with the alias address: */
	    if(finalMapping[start].alias & start)
	      fprintf(of,"_%x",bank_base);

	    if(finalMapping[start].instance)
	      fprintf(of,"_%d",finalMapping[start].instance);

	    
	    fputc('\n',of);

	    //finalMapping[start].reg->isEmitted = 1;
	  }
	}

	fprintf(of,"  endc\n");

	start = -1;
      }

    }

    addr++;

  } while(addr <= pic->maxRAMaddress);
  

}

/*-----------------------------------------------------------------*
 *  void list_valid_pics(int ncols, int list_alias)
 *
 * Print out a formatted list of valid PIC devices
 *
 * ncols - number of columns in the list.
 *
 * list_alias - if non-zero, print all of the supported aliases
 *              for a device (e.g. F84, 16F84, etc...)
 *-----------------------------------------------------------------*/
void list_valid_pics(int ncols, int list_alias)
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
PIC_device *find_device(char *name)
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
void init_pic(char *pic_type)
{
  pic = find_device(pic_type);

  if(!pic) {
    if(pic_type)
      fprintf(stderr, "'%s' was not found.\n", pic_type);
    else
      fprintf(stderr, "No processor has been specified (use -pPROCESSOR_NAME)\n");

    fprintf(stderr,"Valid devices are:\n");

    list_valid_pics(4,0);
    exit(1);
  }

  
  addMaps(pic);


}

/*-----------------------------------------------------------------*
 *  char *processor_base_name(void) - Include file is derived from this.
 *-----------------------------------------------------------------*/
char *processor_base_name(void)
{

  if(!pic)
    return NULL;

  return pic->name[0];
}

int isSFR(int address)
{

  if( (address > pic->maxRAMaddress) || !finalMapping[address].isSFR)
    return 0;

  return 1;

}

int validAddress(int address, int reg_size)
{
  int i;

  if(address > pic->maxRAMaddress)
    return 0;

  for (i=0; i<reg_size; i++)
    if(!finalMapping[address + i].isValid || 
       finalMapping[address+i].reg ||
       finalMapping[address+i].isSFR )
      return 0;

  return 1;
}

void mapRegister(regs *reg)
{

  int i;
  int alias;

  if(!reg || !reg->size) {
    fprintf(stderr,"WARNING: %s:%s:%d Bad register\n",__FILE__,__FUNCTION__,__LINE__);
    return;
  }

  for(i=0; i<reg->size; i++) {

    alias = finalMapping[reg->address].alias;
    reg->alias = alias;

    do {

      // fprintf(stdout,"mapping %s to address 0x%02x, reg size = %d\n",reg->name, (reg->address+alias+i),reg->size);

      finalMapping[reg->address + alias + i].reg = reg;
      finalMapping[reg->address + alias + i].instance = i;

      /* Decrement alias */
      if(alias)
	alias -= ((alias & (alias - 1)) ^ alias);
      else
	alias--;

    } while (alias>=0);
  }

  //  fprintf(stderr,"%s - %s addr = 0x%03x, size %d\n",__FUNCTION__,reg->name, reg->address,reg->size);

  reg->isMapped = 1;

}

int assignRegister(regs *reg, int start_address)
{
  int i;

  //fprintf(stderr,"%s -  %s start_address = 0x%03x\n",__FUNCTION__,reg->name, start_address);
  if(reg->isFixed) {

    if (validAddress(reg->address,reg->size)) {

      mapRegister(reg);
      return reg->address;
    }

    if( isSFR(reg->address)) {
      mapRegister(reg);
      return reg->address;
    }
    fprintf(stderr, "WARNING: Ignoring Out of Range register assignment at fixed address %d, %s\n",
	    reg->address, reg->name);

  } else {

    /* This register does not have a fixed address requirement
     * so we'll search through all availble ram address and
     * assign the first one */

    for (i=start_address; i<=pic->maxRAMaddress; i++) {

      if (validAddress(i,reg->size)) {
	reg->address = i;
	mapRegister(reg);
	return i;
      }
    }

    fprintf(stderr, "WARNING: No more RAM available\n");

  }

  return -1;
}

void assignFixedRegisters(set *regset)
{
  regs *reg;

  for (reg = setFirstItem(regset) ; reg ; 
       reg = setNextItem(regset)) {

    if(reg->isFixed) 
      assignRegister(reg,0);
  }

}

void assignRelocatableRegisters(set *regset, int used)
{

  regs *reg;
  int address = 0;

  for (reg = setFirstItem(regset) ; reg ; 
       reg = setNextItem(regset)) {

    // fprintf(stdout,"assigning %s\n",reg->name);

    if((!reg->isFixed) && ( (used==0) || reg->wasUsed))
      address = assignRegister(reg,address);

  }

}


/*-----------------------------------------------------------------*
 *  void assignConfigWordValue(int address, int value)
 *
 * All midrange PICs have one config word at address 0x2007.
 * This routine will assign a value to that address.
 *
 *-----------------------------------------------------------------*/

void assignConfigWordValue(int address, int value)
{
  if(CONFIG_WORD_ADDRESS == address)
    config_word = value;

  fprintf(stderr,"setting config word to 0x%x\n",value);

}
/*-----------------------------------------------------------------*
 * int getConfigWord(int address)
 *
 * Get the current value of the config word.
 *
 *-----------------------------------------------------------------*/

int getConfigWord(int address)
{
  if(CONFIG_WORD_ADDRESS == address)
    return config_word;

  else
    return 0;

}
